# OrikaServer Stability Audit

**Date:** 2026-05-08
**Scope:** WebSocket / IOCP / OpenSSL server code
**Files reviewed:** `server\*.cpp/.h`, `ClientSocket\*.cpp/.h`, `SocketServer.cpp/.h`

This document lists every stability issue found, what to change, why, and the impact each fix has. **No code has been changed** — this is for your review and decision.

---

## Severity Legend

| Severity | Meaning |
|---|---|
| **CRITICAL** | Crashes, heap corruption, or remotely exploitable. Fix before next deployment. |
| **HIGH** | Will cause failures under load, memory growth, or DoS. Fix soon. |
| **MEDIUM** | Latent bugs, incorrect behavior in edge cases. Fix when convenient. |

---

# CRITICAL Issues

## C1. `DataBuffer::Clear()` allocates 1 byte but claims 10 MB

**File:** `server\DataBuffer.cpp:55`

**Current code:**
```cpp
char* new_char = new char(10000000);   // single byte initialized to (10000000 % 256)
pBuffer = new_char;
this->uMaxSize = 10000000;             // claims 10 MB
```

**Why it's a bug:** `new char(N)` (with parentheses) allocates **one** char and initializes it with value `N`. To allocate an array you need `new char[N]` (with square brackets). So the code allocates 1 byte, then `Append` / `Pop` / `memmove` writes up to 10 MB into it. The destructor uses `delete[]` on a non-array `new` — undefined behavior on top of undefined behavior.

**What to change:** `new char(10000000)` → `new char[10000000]`

**How it makes the software better:**
- Eliminates an immediate heap-corruption bomb that triggers any time `Clear()` is called.
- Random crashes that look unrelated (because heap corruption can manifest anywhere later) will stop.
- Removes a potential remote-code-execution class bug.

---

## C2. Wrong status check deletes live sessions on first packet

**File:** `server\openssl_iocp.cpp:691-769`

**Current code:**
```cpp
bool close_session = (CLOSED == p->psession->status);   // computed but never used
session_unlock(p->psession);
if (p->psession->status == 8)                            // 8 == CONNECTED, not CLOSED
{
    ...
    session_delete(p->psession);                         // frees a live session
}
```

**Why it's a bug:** `8` is the bit value of `CONNECTED`. `CLOSED` is `0x80` (= 128). The disconnection block runs the first time a connected session sees any IO completion, which deletes the session while it still has in-flight `OVERLAPPED` operations and is still in `m_ClientContext`. The variable `close_session` was clearly meant to be used in this `if` but was left dangling.

**What to change:** Use the existing `close_session` flag (or `(p->psession->status & CLOSED)`) instead of `== 8`.

**How it makes the software better:**
- Almost certainly stops the random crashes / "ghost client" bugs the system experiences today.
- This is the single highest-impact fix in the codebase.

---

## C3. Session lock released before reading session state — use-after-free race

**File:** `server\openssl_iocp.cpp:694-769`

**Why it's a bug:** After `session_unlock(p->psession)` on line 694, the code continues to read and modify `p->psession->status`, `->key`, calls `session_close` and `session_delete`. Once unlocked, another IOCP worker can complete an outstanding `WSARecv`/`WSASend` on the same session and call `session_on_completed_packets` concurrently. The `OVERLAPPED` structures live inside the freed `SSL_session`, so a concurrent completion dereferences freed memory.

**What to change:** Either keep the lock held through the whole deletion, or transition through a "pending delete" status that prevents `session_recv`/`session_send` from re-arming, drain outstanding ops, **then** delete.

**How it makes the software better:**
- Eliminates a class of random crashes and heap corruption in the IOCP worker pool.
- Predictable behavior under concurrent load.

---

## C4. WebSocket frame deframer — buffer overflow exploitable by any peer

**File:** `server\FrameAndDeframeMessage.cpp:75, 85, 96, 102, 120`

**Current code (selected):**
```cpp
// line 75 — 64-bit length path is broken AND truncates to int
payloadSize = ntohl(*(u_long*)(m_databuffer.GetBuffer() + 6));
// line 85 — copies payload chunk into psession->frame_buffer[BUFFER_SIZE] with no size check
memcpy(psession->frame_buffer,
       m_databuffer.GetBuffer() + masksOffset + extra_headerSize,
       m_databuffer.GetDataSize() - masksOffset - extra_headerSize);
```

**Why it's a bug:**
- `payloadSize` is signed `int`. A peer-supplied 64-bit length above 2^31 becomes negative, then a huge unsigned number when passed to `memcpy`.
- The 8-byte length path (`basicSize == 127`) reads only 4 bytes from offset 6, missing bytes 2–5. The high 32 bits of any 64-bit length are silently dropped.
- No check that `frame_buffer_size + chunk <= BUFFER_SIZE` (900,000) before each `memcpy`.
- No bound at all on `LastFrame_size + new` against `BUFFER_SIZE` for fragmented messages.

**What to change:**
- Use `uint64_t` for `payloadSize`, parse offset 2..9 correctly.
- Enforce a server-side max payload size (e.g. 1–4 MB), reject larger frames.
- Validate every chunk against `BUFFER_SIZE - current_size` before each `memcpy`.

**How it makes the software better:**
- A malicious or buggy WebSocket client can no longer crash the server with a single oversized frame.
- Closes a heap-corruption (and likely remote code execution) vector.
- Protocol parsing becomes spec-compliant for large messages.

---

## C5. Per-frame heap leak in deframer

**File:** `server\FrameAndDeframeMessage.cpp:112-117`

**Current code:**
```cpp
char* payload_t = new char[psession->frame_buffer_size + 1];
for (...) payload_t[i] = (psession->frame_buffer[i] ^ psession->masks[i % 4]);
payload_t[...] = '\0';
memcpy(psession->LastFrame + psession->LastFrame_size, payload_t,
       psession->frame_buffer_size);
// payload_t is never delete[]'d
```

**Why it's a bug:** Every WebSocket frame received leaks `frame_buffer_size + 1` bytes. In a busy server this is steady-state memory growth proportional to incoming traffic.

**What to change:** Add `delete[] payload_t;` after the `memcpy`. Better: unmask in place into `LastFrame` and skip the temporary entirely.

**How it makes the software better:**
- Stops continuous memory growth that eventually OOM-crashes the server.
- Reduces heap fragmentation and improves long-running stability.

---

## C6. Embedded private key in source + `SSL_VERIFY_NONE`

**Files:** `server\cert.cpp:46-71, 114-141`; `server\openssl_iocp.cpp:168`

**Why it's a bug:** The TLS private key is hardcoded as a string literal in `cert.cpp` (and additionally loaded from `key.pem`). Anyone with access to source or the compiled binary has the server's private key. Combined with `SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, nullptr)`, neither side authenticates the other — TLS becomes encryption-only and is trivially MITM-able once the key leaks.

**What to change:**
- Rotate the TLS key/cert immediately.
- Remove the literal key from `cert.cpp`. Load only from disk with restricted ACLs (Windows: SYSTEM + the service account only).
- Decide whether you actually need client cert auth; if yes, set proper verify flags and a CA bundle.

**How it makes the software better:**
- Prevents a complete loss of TLS confidentiality and authenticity if the binary is leaked or disassembled.
- Brings the deployment to a basic security baseline.

---

## C7. TLS certificate is currently expired

**File:** `server\cert.cpp` (server cert NotAfter = 2026-01-12; older bundled cert expired 2021-01-11)

**Why it's a bug:** Today is 2026-05-08. The cert expired four months ago. Browsers and modern clients will refuse the connection. Even self-signed-tolerant clients will warn or fail.

**What to change:** Issue a new certificate for `www.auttrading.com`, replace `Certificate.pem` and `key.pem`, and remove the embedded blob in `cert.cpp` at the same time.

**How it makes the software better:**
- Clients can connect again.
- Eliminates a hard outage that becomes worse the longer it sits.

---

## C8. `send_close_message_to_client` sends pointer-size, not buffer-size

**File:** `server\openssl_iocp.cpp:312-316`

**Current code:**
```cpp
BYTE closeMessage[] = { 0x88, 0x00 };
const char* closeMessageStr = reinterpret_cast<const char*>(closeMessage);
session_send_data(psession, closeMessageStr, sizeof(closeMessageStr), L"", 1);
```

**Why it's a bug:** `sizeof(closeMessageStr)` is the size of a pointer (4 bytes on x86, 8 on x64), not 2. The server sends 4–8 bytes — the 2 close-frame bytes plus 2–6 bytes of stack garbage — as the "close" message.

**What to change:** `sizeof(closeMessageStr)` → `sizeof(closeMessage)`.

**How it makes the software better:**
- Server now sends a spec-compliant close frame instead of garbage that some clients refuse.
- Eliminates a source of small-amount stack data leaking to peers.

---

## C9. `serverPrepare` accept loop never runs; busy-wait is undefined behavior

**File:** `SocketServer.cpp:9, 37, 56-58`

**Current code:**
```cpp
int SocketServer::m_Serverislive = 0;          // line 9
...
while (m_Serverislive == 1) { ... }            // line 37 — never enters
...
m_recvThreadSrynk = 1;
AfxBeginThread(...);  AfxBeginThread(...);
while (m_recvThreadSrynk == 1) { }             // line 56 — busy-wait
```

**Why it's a bug:** `m_Serverislive` is initialized to `0` and nothing in the file flips it before the loop, so the accept loop body never executes — that whole code path is dead. The inner busy-wait reads a non-`volatile`, non-atomic `int` from another thread; under MSVC release optimization the read can be hoisted out of the loop, leaving the thread spinning forever. Even when it works, it burns 100% CPU on one core.

**What to change:**
- Decide whether `SocketServer` is used at all. If not, delete it. Comments and the duplication suggest it's dead code.
- If it is used: set `m_Serverislive = 1` in the right place, replace the busy-wait with a Win32 `Event` (`SetEvent`/`WaitForSingleObject`), and mark shared flags `volatile` (or use `std::atomic`).

**How it makes the software better:**
- Removes a 100% CPU burn or a thread that "never starts" depending on optimization.
- Reduces code surface area — fewer files to keep in sync.

---

## C10. Debug code overrides login-search string with `"multi"`

**Files:** `ClientSocket\ClientSocket.cpp:550-556`, `ClientSocket\UniFeederSocket.cpp:558-563`

**Current code:**
```cpp
if(res = ReadString(buf, maxlen))

       //For Checking Pur
           str = "multi";
           //End
      found = strstr(buf, str) != NULL;
```

**Why it's a bug:** The login flow calls `ReadStringCheck(buf, maxlen, "Login: ", found)` etc., but the function overwrites the parameter `str` with the literal `"multi"` before the search. So the upstream-feeder login state machine in `Login()` only proceeds when `"multi"` happens to appear in the stream. Looks like leftover debug code that was committed.

(Side note: there are no braces on the `if`, so only the `str="multi"` assignment is conditional — the `found = strstr(...)` runs unconditionally on the current code. After removing the override, the `found =` line correctly becomes the if's body.)

**What to change:** Delete the `str = "multi";` line and its `//For Checking Pur` / `//End` markers in both files.

**How it makes the software better:**
- Login actually works against the upstream feeder.
- Removes debug code from production paths.

---

## C11. Operator-precedence bug in `recv` / `Receive` calls

**Files:** `SocketServer.cpp:160`, `ClientSocket\ClientSocket.cpp:573`, `ClientSocket\UniFeederSocket.cpp:582`

**Current code:**
```cpp
if(recvcheck = recv(m_socket, buf, maxlen, 0) != 1)   // ClientSocket.cpp:573
if(recvcheck = recv(m_socket, buf, 1, 0) != 1)        // UniFeederSocket.cpp:582
if (datasize = pSocket->Receive(cbuf + incrementval, sizeof(cbuf)-1) > 0)  // SocketServer.cpp:160
```

**Why it's a bug:** `!=` and `>` bind tighter than `=`. The compiler parses each line as `recvcheck = (recv(...) != 1)`, so `recvcheck`/`datasize` is always `0` or `1` — the actual byte count is discarded. Callers that depend on the byte count (advancing buffer pointers, deciding how much to parse) operate on garbage. In `SocketServer.cpp`, `incrementval` is also never incremented, so the receive loop overwrites position 0 of `cbuf` on every iteration.

**What to change:** Wrap the assignment in parentheses, and use a meaningful comparison:
```cpp
if ((recvcheck = recv(m_socket, buf, maxlen, 0)) < 1)   // success returns >=1
if ((recvcheck = recv(m_socket, buf, 1, 0)) != 1)       // exactly 1 byte expected
if ((datasize  = pSocket->Receive(cbuf + incrementval, sizeof(cbuf)-1)) > 0)
```

**How it makes the software better:**
- Read paths see the real byte counts again. Buffer parsing stops corrupting on the first call.
- Login negotiation / data ingestion against the upstream feeder becomes deterministic.

---

# HIGH Issues

## H1. Unsynchronized access to global `m_deletedClientlist` (CMap)

**File:** `server\openssl_iocp.cpp:30, 702-708`
**Why:** `CMap` is not thread-safe; touched from every IOCP worker without a lock. Map corruption / crash under concurrency.
**Change:** Guard with a `CRITICAL_SECTION`, or replace with a thread-safe collection.
**Better:** Removes intermittent crashes that look unrelated to anything.

## H2. `session_send_data` checks `ssl == nullptr` before locking — race

**File:** `server\openssl_iocp.cpp:809-837`
**Why:** Between the unlocked `ssl == nullptr` check and `session_lock`, another thread can run `session_close` and `SSL_free(psession->ssl)`. The 5-second busy-wait then sleeps with a freed pointer in scope.
**Change:** Lock first, then check; introduce a refcounted session lifetime so app threads cannot outlive the IOCP destroyer.
**Better:** Removes a race that gets worse the busier the server gets.

## H3. `Sending_Handler` — busy loop, unsynchronized map iteration, `CSocket::Attach` race

**File:** `SocketServer.cpp:81-122`
**Why:** `while (true)` with no sleep → 100% CPU when idle. Iterates `m_socket_detail_Map` with no lock while another thread mutates it. `CSocket m_socket; m_socket.Attach(m_mainSocket);` from multiple iterations attaches the same raw `SOCKET` to multiple wrappers — undefined; `Send` is not thread-safe across wrappers on the same handle. `Send` return value is ignored.
**Change:** Add a wait/sleep, lock the map iteration, use `send()` directly with a per-connection write lock, check return values.
**Better:** Frees a CPU core, eliminates rare data corruption on outbound traffic.

## H4. `SSL_session` is ~5–7 MB per connection — memory budget is unsustainable

**File:** `server\openssl_iocp.h:51-89, 39-40`
**Why:** `BUFFER_SIZE = 900000` and the struct contains six to seven 900 KB inline buffers (`socket_buffer_Rev`, `socket_buffer_Send`, `ssl_buffer_Rev`, `ssl_buffer_Send`, `pending_socket_buffer_Rev`, `frame_buffer`, `LastFrame`, `unprocessBuffer`). 100 clients = ~600 MB regardless of activity.
**Change:** Move buffers behind pointers, allocate on demand, drop unused buffers, and cap maximum concurrent connections.
**Better:** Server can serve 10×–50× more concurrent clients on the same machine. Connection cap prevents OOM.

## H5. No handshake / idle timeout — slow-loris DoS

**Files:** `server\Server.cpp:180-192`, `server\openssl_iocp.cpp` (overall)
**Why:** No timer closes a session that fails to finish the WebSocket handshake or stops sending/receiving. A peer that opens TCP+TLS but never sends `Upgrade` holds ~5 MB forever. Also: only **one** pre-posted `AcceptEx`, so accept rate is serial.
**Change:** Per-session deadline, close on expiry. Pre-post at least 16 `AcceptEx` operations.
**Better:** Server resists trivial denial-of-service attempts and accepts new connections faster under burst.

## H6. Per-message 1.5 MB allocations on hot paths

**Files:** `server\Server.cpp:57, 6163, 6193, 6245, 6251, 6294, 6330, 6340`
**Why:** Every `app_on_session_recv` / `SendDataToClient*` call creates `DataBuffer protodata; protodata.Allocate(1500000);`. Heavy heap churn / fragmentation under burst. Also `WebsocketHandshakeMessage *wshs = new ...` in `Server.cpp:60` is only `delete`d on the success branch, so failed parses leak.
**Change:** Reuse per-thread or per-session scratch buffers. `delete wshs` in both branches.
**Better:** Lower CPU / allocator pressure, smoother latency, less long-run memory growth.

## H7. `WebsocketHandshakeMessage::Parse` reads `request[-1]` on empty input

**File:** `server\WebsocketHandshakeMessage.cpp:42-43`
**Why:** `if (request[request.size() - 1] == '\r')` underflows to `SIZE_MAX` when `request.size() == 0`. Crash from a malformed first line.
**Change:** Guard with `if (!request.empty() && request.back() == '\r')`.
**Better:** Server can't be crashed by a single empty/malformed handshake line.

## H8. WebSocket protocol is only partially validated

**File:** `server\WebsocketHandshakeMessage.cpp:36-80`
**Why:** Parser does not check method == `GET`, `Upgrade: websocket`, `Connection: Upgrade`, `Sec-WebSocket-Version: 13`, or that `Sec-WebSocket-Key` decodes to 16 bytes. Frame parser does not enforce: MASK bit must be set on client→server frames, control frames must be unfragmented and ≤125 bytes, reserved bits must be zero, opcode must be known. There is no ping/pong handling and no close-frame handling.
**Change:** Add per-field validation; reject and close on violation. Implement ping/pong and close handling.
**Better:** Compatible with browsers and standards-compliant clients; rejects malformed/malicious input early.

## H9. Null returns from OpenSSL APIs not checked

**Files:** `server\Server.cpp:160-178`, `server\openssl_iocp.cpp:264-298`
**Why:** `PEM_read_bio_X509`, `PEM_read_bio_PrivateKey`, `SSL_new`, `BIO_new` can all return NULL. The result is passed straight into the next call. `session_new` calls `malloc` and never checks. Partial-init paths leak `bio` and an initialized `CRITICAL_SECTION`.
**Change:** Check every return; on failure, free what was allocated and return cleanly.
**Better:** Server reports a clean error and stays up under low-memory or bad-config conditions instead of crashing.

## H10. OpenSSL locking callbacks set on a version that ignores them

**File:** `server\openssl_iocp.cpp:131-149`
**Why:** `CRYPTO_set_locking_callback` / `CRYPTO_num_locks` are no-ops in OpenSSL ≥ 1.1, which this codebase uses (`TLS_server_method` is 1.1+). The locks are allocated and never used.
**Change:** Remove the dead init code or update for the actual OpenSSL version in use.
**Better:** Less confusing code; one less misleading "we have locks" assumption.

## H11. `BUFFER_SIZE_REV` vs `BUFFER_SIZE` inconsistency — silent overflow waiting to happen

**Files:** `server\openssl_iocp.cpp:498`, `server\openssl_iocp.h:39-40, 60`
**Why:** `ssl_buffer_Rev` is sized `BUFFER_SIZE_REV`, but `SSL_read(..., ssl_buffer_Rev, BUFFER_SIZE)` is called. Today both equal 900000 so no overflow, but a future tweak that lowers `BUFFER_SIZE_REV` becomes a silent buffer overflow.
**Change:** Use the buffer's own size constant in the `SSL_read` call.
**Better:** Eliminates a future-proofing trap.

---

# MEDIUM Issues

| # | File:line | Problem | Fix |
|---|---|---|---|
| M1 | `server\WebsocketDataMessage.cpp:47` | `raw.substr(0, separator-1)` underflows when `separator == 0` or `npos`. | Range-check before substring. |
| M2 | `server\FrameAndDeframeMessage.cpp:162` | `WebsocketDataMessage& dataMessage = WebsocketDataMessage(inputdata);` binds non-const lvalue ref to a temporary (MSVC extension; reference dangles). | Use `WebsocketDataMessage dataMessage(inputdata);` (value). |
| M3 | `ClientSocket\ClientSocket.cpp:187-195`, `ClientSocket\UniFeederSocket.cpp:195-200` | `while (len <= r_byte)` copies `r_byte+1` bytes plus a trailing NUL — off-by-one overflow when `r_byte == maxlen-1`. | Change to `<` and recheck terminator placement. |
| M4 | `ClientSocket\ClientSocket.cpp:150-152`, `UniFeederSocket.cpp:158-160` | `gethostbyname` is not thread-safe; `inet_addr` is deprecated. | Use `getaddrinfo` (thread-safe, IPv6-capable). |
| M5 | `server\openssl_iocp.cpp:239-257` | `ssl_get_error` passes the `SSL_get_error` code into `ERR_error_string_n`, which expects an `ERR_get_error` value — wrong API contract. | Drain `ERR_get_error()` in a loop and format each. |
| M6 | `server\Server.cpp:60` | Receive treats binary WebSocket bytes as a NUL-terminated C string (`strncpy_s` + `strlen`). Frames containing `0x00` truncate. | Pass length explicitly; never use `strlen` on raw frame bytes. |
| M7 | `server\Server.cpp:66-79` | `WebsocketHandshakeMessage* wshs = new ...` only deleted on success. | Use `std::unique_ptr` or delete in both branches. |
| M8 | `server\openssl_iocp.cpp:348` (`session_close`) | Touches `psession->status` and `->s` without consistent locking — called both with and without the lock held by different paths. | Document or enforce a single locking discipline. |

---

# Architecture-level Concerns

These aren't single line fixes — they're patterns visible across the codebase.

### A1. No ownership model for `SSL_session`
Application threads, IOCP completion threads, and the deletion path all touch the same struct with no clear "who owns it." The current discipline is "hold `psession->lock` sometimes" — used in `session_send_data`, ignored in `session_close`, deliberately released mid-deletion in `session_on_completed_packets`. This is the root cause of roughly half of the concrete bugs above.
**Direction:** Move to a refcounted `SSL_session` (each async op increments, completion decrements). Free only when refcount hits zero. This makes the lifetime invariants explicit instead of implicit.

### A2. No backpressure / flow control
`session_send_data` busy-loops with `Sleep(0)` and a 5-second wall-clock timeout instead of queueing on the `WSASend` completion. Under congestion this both starves IOCP workers (they hold `psession->lock`) and sometimes deletes clients incorrectly.
**Direction:** Replace the busy-wait with a per-session send queue serviced by `WSASend` completions.

### A3. No bounds anywhere
- No max concurrent connections.
- No max message size.
- No idle / handshake timeouts.
- Per-session inline buffers fixed at ~5 MB.

A single misbehaving peer can OOM the server. **Direction:** add explicit limits at every input boundary.

### A4. Coarse-grained global state
`m_ClientContext`, `m_ClientList_forDeal`, `m_ClientList_forOrder`, `m_SymbolHighLowTimeWiseArray`, `m_SymbolSourceArray` are all `CMap`s guarded by one `CMutex`. Lock granularity is coarse, iteration of these maps happens in hot paths (e.g. broadcast at `Server.cpp:6174`), and several places iterate without locking.
**Direction:** Per-shard locking, or per-session ownership of the data the session needs to broadcast.

### A5. Sleep-based polling everywhere
`Sleep(0)`, `Sleep(100)`, `Sleep(5000)`, `while (m_recvThreadSrynk == 1) {}`. These are stand-ins for proper synchronization primitives and make behavior depend on luck and CPU load.
**Direction:** Replace with `Event`s, condition variables, or completion-based notifications.

### A6. WebSocket protocol implementation is partial
No control-frame handling, no ping/pong, no close-frame handling, no MASK validation, no max frame size, no fragmentation invariants checked, no permessage-deflate.
**Direction:** Either implement RFC 6455 fully or adopt an existing library (libwebsockets, websocketpp, or Beast).

### A7. Three mostly-duplicated socket layers
`ClientSocket.cpp` and `UniFeederSocket.cpp` are ~80% identical. `SocketServer.cpp` is largely unused. Bugs need fixing in three places.
**Direction:** Consolidate into one socket abstraction. Delete the unused `SocketServer.cpp`.

### A8. Build artifacts in the repo
The git working tree shows `Debug/*.obj`, `*.sdf`, `*.suo` modified/deleted. These hide real source changes in diffs.
**Direction:** Add to `.gitignore`: `Debug/`, `Release/`, `*.sdf`, `*.suo`, `*.obj`, `*.pch`, `*.tlog/`, `*.idb`, `*.pdb`, `*.ipch`.

---

# Suggested Fix Order

The order below maximizes stability gain per hour spent.

### Phase 1 — One-line fixes (under 1 hour total)
| # | File:line | Why first |
|---|---|---|
| C1 | `DataBuffer.cpp:55` | Trivial, eliminates heap corruption. |
| C8 | `openssl_iocp.cpp:316` | Trivial, fixes broken close frames. |
| C10 | `ClientSocket.cpp:553`, `UniFeederSocket.cpp:561` | Trivial, restores login. |
| C11 | three `recv`/`Receive` sites | Trivial, restores correct byte counts. |

### Phase 2 — Half-day fixes
| # | Why |
|---|---|
| C2 | Single-condition fix; almost certainly resolves the random crashes. Test heavily after. |
| C5 | One `delete[]` line; stops continuous memory growth. |
| C7 | Reissue cert + replace files; clients can connect again. |
| C9 | Either delete dead `SocketServer` or fix flag/wait. |
| H7 | One `if` guard; closes a crash from malformed handshake. |

### Phase 3 — One-to-three days
| # | Why |
|---|---|
| C3 | Lifetime / locking redesign — also addresses A1. |
| C4 | Frame parser hardening + max-payload enforcement. |
| C6 | Cert/key separation, restricted ACLs, decision on client auth. |
| H1, H2, H3 | Add locking around each shared structure individually. |
| H8 | Add per-field validation in handshake and frame parser. |

### Phase 4 — Architectural
| # | Why |
|---|---|
| A1 | Refcounted session lifetime — removes the root cause of many residual races. |
| A2 | Real send queue / backpressure. |
| A3 | Bounded resources at every boundary. |
| H4 | Pointer-allocated buffers — concurrent capacity goes up by 10×–50×. |
| A6 | Either complete or replace the WebSocket implementation. |
| A7 | Consolidate the three socket layers. |

---

# Summary

The server has **eleven critical-severity issues**, of which several are exploitable by any peer. Most are concentrated in three files:

- `server\openssl_iocp.cpp` — session lifetime / locking
- `server\FrameAndDeframeMessage.cpp` — WebSocket frame parsing
- `server\DataBuffer.cpp` and the `*Socket` files — small but impactful one-liners

Fixing the **Phase 1 list alone** (under 1 hour of work) eliminates one heap-corruption bomb, one broken protocol message, the always-broken login flow, and three wrong-byte-count read paths. Fixing the **Phase 2 list** very likely stops the random crashes you experience today.

After Phases 1–2 the server will be *stable* under normal load. Phase 3–4 are needed for it to be *robust* under attack and high concurrency.
