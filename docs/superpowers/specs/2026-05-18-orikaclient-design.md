# OrikaClient — WebSocket Login Test Client — Design

**Date:** 2026-05-18
**Status:** Approved
**Author:** brainstorming session

## Purpose

`orikaClient` is a standalone C++ console application that tests a running
OrikaServer. It connects over `wss://`, performs one `login` round-trip, and
reports **PASS** or **FAIL** with a matching process exit code.

It is a single-purpose diagnostic tool: confirm that an OrikaServer instance
accepts a WebSocket connection and answers a login request correctly.

It lives in its **own folder**, separate from the server repository:

```
d:\VSCodeProject\OrikaClient
```

This supersedes the earlier `2026-05-15-ws-login-test` spec (a PowerShell
script that was specced and planned but never implemented). The driving
example is unchanged: log in as user `1001` with password `hello@12345`.

## Background — verified protocol

The wire protocol was confirmed by a live probe and is documented in
`docs/superpowers/specs/2026-05-15-ws-login-test-design.md`. Summary:

- The server speaks **WebSocket over TLS**. Each WebSocket **binary frame**
  payload is a **raw protobuf** `Oreka.ClientMessage` — no length prefix, no
  base64 wrapping.
- A login request is:
  `ClientMessage{ type:"login", loginrequest: LoginRequest{ type:"login", login, pwd, serialNo } }`
- The server replies with:
  `ClientMessage{ type:"LOGIN_STATUS", loginresponse: LoginResponse{ type:"LOGIN_STATUS", status, ... } }`
- `status` is one of: `success`, `invalid credentials`, `invalid SerialNo`,
  `Invalid ApiLogin`.
- `serialNo` is validated (DB table `orika_userLoginHDkeyMapping`, where
  `HDno='*'` is a wildcard) **before** the password. A bad serialNo
  short-circuits to `invalid SerialNo` and the password is never checked.

### Protobuf field numbers

| Message       | Field         | Number | Type          |
|---------------|---------------|--------|---------------|
| ClientMessage | type          | 1      | string        |
| ClientMessage | loginrequest  | 2      | LoginRequest  |
| ClientMessage | loginresponse | 3      | LoginResponse |
| LoginRequest  | type          | 1      | string        |
| LoginRequest  | login         | 2      | string        |
| LoginRequest  | pwd           | 3      | string        |
| LoginRequest  | serialNo      | 4      | string        |
| LoginResponse | type          | 1      | string        |
| LoginResponse | status        | 2      | string        |
| LoginResponse | logintoken    | 3      | string        |
| LoginResponse | data          | 4      | TabData       |

The protobuf wire format depends only on field numbers and wire types, so the
client hand-encodes the login message and hand-decodes the response. It does
**not** need the server's full ~90-message `.proto` schema.

### TLS note

The server's certificate fails chain validation. The client therefore **skips
certificate verification by default**, with an opt-in switch to enforce it.

## Approach

**Approach B — OpenSSL for TLS + hand-rolled WebSocket framing + hand-coded
protobuf.**

- TLS is provided by **OpenSSL** (`SSL_connect` over a plain TCP socket).
- The WebSocket `Upgrade` handshake and client-side frame masking/unmasking
  are written by hand — no WebSocket library.
- The login `ClientMessage` (~49 bytes) is hand-encoded and the `LOGIN_STATUS`
  reply hand-decoded — no protobuf library.

Alternatives considered and rejected:

- **Approach A — WinHTTP WebSocket API.** Windows' built-in WinHTTP provides
  TLS and WebSocket framing for free. Leaner, but the team chose B to mirror
  what the server itself does.
- **Approach C — full third-party libraries** (generated protobuf bindings +
  Boost.Beast / IXWebSocket). Heaviest: must build and vendor protobuf, a
  WebSocket library, and their dependencies. Overkill for a single login
  round-trip.

## Architecture

Four units, each with one responsibility and a narrow interface:

| Unit              | Responsibility                                                                      | Interface                                                              |
|-------------------|--------------------------------------------------------------------------------------|-------------------------------------------------------------------------|
| `TlsConnection`   | TCP connect + OpenSSL TLS client handshake                                           | `connect(host, port)`, `read(buf)`, `write(buf)`, `close()`             |
| `WebSocketClient` | HTTP `Upgrade` handshake; frame encode/decode (masking, length, control frames)      | `handshake(host, path)`, `sendBinary(bytes)`, `receiveMessage() -> bytes` |
| `ProtoCodec`      | Hand-coded protobuf — pure functions, no I/O                                         | `encodeLogin(login, pwd, serial) -> bytes`, `decodeLoginStatus(bytes) -> {type, status, token}` |
| `main.cpp`        | CLI parsing, orchestration, PASS/FAIL output, exit codes                             | —                                                                       |

`WebSocketClient` depends on `TlsConnection`. `ProtoCodec` depends on nothing
(pure). `main.cpp` wires the three together.

### Folder layout

```
d:\VSCodeProject\OrikaClient\
  OrikaClient.sln
  OrikaClient\
    OrikaClient.vcxproj          Debug|x64 and Release|x64; output orikaClient.exe
    main.cpp
    TlsConnection.h / .cpp
    WebSocketClient.h / .cpp
    ProtoCodec.h / .cpp
  third_party\openssl\           vendored: include\ + lib\ + runtime DLLs
  README.md
```

The build produces `orikaClient.exe`.

### OpenSSL sourcing

A clean **OpenSSL 3.x** is **vendored** into `third_party\openssl\`
(`include\`, `lib\`, and the runtime DLLs). Rationale:

- The client is fully self-contained — no dependency on the server repo.
- It sidesteps the OpenSSL version ambiguity the server audit flagged (mixed
  `libeay32`/`ssleay32` vs `libssl`/`libcrypto` naming; 1.0-vs-3 mismatch).

The `.vcxproj` adds `third_party\openssl\include` to include paths,
`third_party\openssl\lib` to library paths, links `libssl.lib` /
`libcrypto.lib`, and copies the matching DLLs next to `orikaClient.exe` via a
post-build step.

## Protocol & data flow

End-to-end sequence driven by `main`:

1. **Parse args** — host, port, path, login, pwd, serial, timeout, flags.
2. **`TlsConnection.connect`** — TCP connect to `host:port`, then OpenSSL
   `SSL_connect`. Certificate verification is **off by default**; `--verify-cert`
   turns it on.
3. **`WebSocketClient.handshake`** — send `GET <path> HTTP/1.1` with headers
   `Host`, `Upgrade: websocket`, `Connection: Upgrade`,
   `Sec-WebSocket-Key: <base64(16 random bytes)>`, `Sec-WebSocket-Version: 13`.
   Read the `101 Switching Protocols` response; verify `Sec-WebSocket-Accept`
   equals `base64(SHA1(key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"))`.
   SHA1 and base64 are taken from OpenSSL.
4. **`ProtoCodec.encodeLogin`** — build `LoginRequest{1:type="login", 2:login,
   3:pwd, 4:serialNo}`, then wrap as `ClientMessage{1:type="login",
   2:loginrequest}`.
5. **`WebSocketClient.sendBinary`** — send the bytes as one binary frame
   (opcode `0x2`, FIN set, **client-masked** with a fresh 4-byte key).
6. **`WebSocketClient.receiveMessage`** — read the reply frame(s); transparently
   handle `ping`(`0x9`), `close`(`0x8`), and continuation(`0x0`) frames; return
   the reassembled, unmasked payload. Bounded by `--timeout`.
7. **`ProtoCodec.decodeLoginStatus`** — walk the reply `ClientMessage` to
   field 3 (`loginresponse`), then `LoginResponse` field 2 (`status`) and
   field 3 (`logintoken`).
8. **Verdict** — `status == "success"` → PASS; any other value → FAIL.

### Protobuf wire format

Every field used here is **length-delimited** (wire type 2). Each field is:

```
tag byte  = (field_number << 3) | 2
length    = varint (payload byte count)
payload   = raw bytes (UTF-8 string, or a nested message's bytes)
```

`encodeLogin`:

1. Encode `LoginRequest` = concat of fields 1..4 (`type`, `login`, `pwd`,
   `serialNo`), each as `tag | varint-len | utf8-bytes`.
2. Encode `ClientMessage` = field 1 (`type="login"`) followed by field 2
   (tag for field 2, varint length of the `LoginRequest` bytes, then those
   bytes).

`decodeLoginStatus` is a minimal walker: iterate top-level fields of the reply
`ClientMessage`, find field 3 (`loginresponse`), then iterate that nested
message for field 2 (`status`) and field 3 (`logintoken`). Unknown fields are
skipped by reading their wire type and advancing. The decoder reads varints
defensively and never reads past the buffer.

### WebSocket framing

- **Client → server frames are masked.** A fresh 4-byte mask key per frame;
  payload XORed with the key cycled byte-by-byte. Opcode `0x2` (binary), FIN
  bit set.
- **Server → client frames are unmasked.**
- Payload length uses the standard 7-bit / 16-bit / 64-bit encoding. The
  encoder picks the smallest form; the decoder accepts all three.
- The receiver loop handles control frames: reply to `ping` with `pong` (or
  log and continue), treat `close` as end-of-stream, and reassemble
  `continuation` frames into one message. Per the verified protocol, the
  server does not complete a graceful close handshake — the client closes its
  own socket without waiting.

## CLI, output, error handling

### Command line

```
orikaClient.exe [--host H] [--port P] [--path /] [--login L] [--pwd P]
                --serial S [--timeout N] [--verify-cert] [--selftest] [--help]
```

| Parameter      | Default            | Notes                                                        |
|----------------|--------------------|--------------------------------------------------------------|
| `--host`       | `localhost`        | Target OrikaServer host.                                     |
| `--port`       | `85`               | Target port (`orikaPort` in `oreka.config`).                 |
| `--path`       | `/`                | WebSocket request path.                                      |
| `--login`      | `1001`             | User id.                                                     |
| `--pwd`        | `hello@12345`      | Password.                                                    |
| `--serial`     | *(mandatory)*      | Registered device serial. Missing → usage error before any connection. |
| `--timeout`    | `12`               | Seconds for connect / send / receive.                        |
| `--verify-cert`| *(off — switch)*   | When present, enforce TLS certificate validation.            |
| `--selftest`   | *(off — switch)*   | Run offline codec self-checks and exit; no network.          |
| `--help`       | —                  | Print usage and exit 0.                                      |

`--serial` is mandatory because the server validates `serialNo` against the DB
before the password; without it the test is not meaningful.

### Output

One short progress line per stage — `connected`, `TLS ok`, `WebSocket
upgraded`, `login sent`, `reply received` — followed by a final verdict line:

- PASS: `PASS - login '<login>' status: success` (plus the login token if the
  reply carries one).
- FAIL: `FAIL - status: <status>` (the server's status string, verbatim).

Failures are reported with a clear single-line message, not a raw stack trace
or OpenSSL error dump.

### Exit codes

| Code | Meaning                                                              |
|------|----------------------------------------------------------------------|
| 0    | PASS — `status == "success"` (also: `--selftest` passed, `--help`).  |
| 1    | FAIL — reply parsed, `status != "success"`.                          |
| 2    | Usage / parameter error (e.g. `--serial` missing).                   |
| 3    | Connection / TLS / WebSocket-handshake failure.                      |
| 4    | Connected and sent, but no reply within timeout, or reply unparseable. |

### Error-handling detail

| Condition                          | Message                                            | Exit |
|-------------------------------------|----------------------------------------------------|------|
| `--serial` omitted                  | `Serial is required` (before connecting)           | 2    |
| Unknown / malformed argument        | usage text                                         | 2    |
| TCP connect refused / no route      | `Cannot reach <host>:<port> (connection refused)`  | 3    |
| TLS handshake failure               | `TLS handshake failed: <reason>`                   | 3    |
| WebSocket upgrade rejected          | `WebSocket handshake rejected: <reason>`           | 3    |
| Connected, no reply within timeout  | `Connected and sent, but no reply within Ns`       | 4    |
| Reply received but not parseable    | `Could not parse server response` + hex dump       | 4    |
| Reply parsed, status != success     | `FAIL - status: <status>`                          | 1    |
| Reply parsed, status == success     | `PASS - login '<login>' status: success`           | 0    |

## Testing

- `ProtoCodec` and the WebSocket frame helpers (mask/unmask, length
  encode/decode, varint encode/decode) are **pure functions** — directly
  testable with golden byte vectors. The 49-byte reference login message from
  the earlier spec is one such vector.
- `orikaClient.exe --selftest` runs those codec assertions against the golden
  vectors with **no network**, prints `SELFTEST OK` or `SELFTEST FAIL: <which>`,
  and exits (`0` / `1`). No test framework and no separate test project — the
  project stays a single dependency-free build.
- The full login round-trip is an **integration test**, run manually against
  an OrikaServer listening on `localhost:85`. The five cases below mirror the
  earlier spec.

### Integration test cases

| Case               | Inputs                                          | Expected                              |
|--------------------|-------------------------------------------------|---------------------------------------|
| Valid login        | real `--serial`, correct `--login` / `--pwd`    | `PASS`, `status:"success"`, exit 0     |
| Wrong password     | real `--serial`, wrong `--pwd`                  | `FAIL - status: invalid credentials`, exit 1 |
| Bad serial         | bogus `--serial`                                | `FAIL - status: invalid SerialNo`, exit 1 |
| Missing serial     | `--serial` omitted                              | usage error, exit 2, no connection    |
| Unreachable server | `--port` pointing at a closed port              | connection error, exit 3              |

## Out of scope (YAGNI)

- Any post-login message exchange (fetch requests, subscriptions, heartbeats).
- Retry / reconnect logic.
- Generating or using the server's full ~90-message `.proto` schema.
- A non-Windows build, a CMake build, or a unit-test framework / test project.
- Connecting to anything other than an OrikaServer (`wss://`).

## Success criteria

- `OrikaClient.sln` builds in Visual Studio (Debug|x64 and Release|x64) and
  produces `orikaClient.exe` with OpenSSL DLLs alongside it.
- `orikaClient.exe --selftest` passes with no network.
- Against an OrikaServer reachable on `localhost:85`, the tool produces a clear
  PASS/FAIL line and a matching exit code for each of the five integration
  cases above.
- `README.md` documents the parameters, the five test cases, and how to obtain
  a valid `--serial`.
