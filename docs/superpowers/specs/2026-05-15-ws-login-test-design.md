# WebSocket Login Test Scenario — Design

**Date:** 2026-05-15
**Status:** Approved
**Author:** brainstorming session

## Purpose

Provide a repeatable test scenario that connects to a running OrikaServer over
`wss://`, sends a `login` request, and verifies the server's `LOGIN_STATUS`
response. The scenario must run on the current Windows development machine with
**no extra installs** (no Python, no `pip`, no `protoc`).

The driving example: connect to `wss://auttrading.com:85`, log in as user
`1001` with password `hello@12345`.

## Background — verified protocol

The protocol below was confirmed by a live probe against `wss://auttrading.com:85`
on 2026-05-15.

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

| Message       | Field      | Number | Type        |
|---------------|------------|--------|-------------|
| ClientMessage | type       | 1      | string      |
| ClientMessage | loginrequest  | 2   | LoginRequest  |
| ClientMessage | loginresponse | 3   | LoginResponse |
| LoginRequest  | type       | 1      | string      |
| LoginRequest  | login      | 2      | string      |
| LoginRequest  | pwd        | 3      | string      |
| LoginRequest  | serialNo   | 4      | string      |
| LoginResponse | type       | 1      | string      |
| LoginResponse | status     | 2      | string      |
| LoginResponse | logintoken | 3      | string      |
| LoginResponse | data       | 4      | TabData     |

Because the protobuf wire format only depends on field numbers and wire types,
the test hand-encodes the login message and hand-decodes the response. It does
**not** need the server's full 90-message `.proto` schema.

### TLS note

The server's certificate fails chain validation. The test therefore **skips
certificate verification by default**, with an opt-in switch to enforce it.

## Approach

A PowerShell script driving a small embedded C# helper.

- PowerShell is available on the target machine; Python is not.
- The C# helper (loaded via `Add-Type`) is required because the WebSocket
  connect runs on a thread-pool thread where a PowerShell scriptblock cannot
  serve as the TLS certificate-validation callback. A compiled delegate works
  on any thread.
- The login message is 49 bytes and trivially hand-encoded, so no `protoc`
  step or generated bindings are needed.

Alternatives considered and rejected: a Python script (requires installing
Python + `protobuf` + `websockets`); generating the full proto closure with
`protoc` (~90 files, heavyweight, import-path snags); a C++ test project inside
the solution (build-config and CRT/ABI overhead).

## File layout

```
tests/ws-login/
  login_test.ps1   the test script
  README.md        usage and expected outcomes
```

## Script: `login_test.ps1`

### Parameters

| Parameter     | Default                     | Notes                                              |
|---------------|-----------------------------|----------------------------------------------------|
| `-Url`        | `wss://auttrading.com:85`   | Target server.                                     |
| `-Login`      | `1001`                      | User id.                                           |
| `-Pwd`        | `hello@12345`               | Password.                                          |
| `-Serial`     | *(mandatory)*               | Registered device serial. Script errors before connecting if omitted. |
| `-Timeout`    | `12`                        | Seconds for connect / send / receive.              |
| `-VerifyCert` | *(off — switch)*            | When present, enforce TLS cert validation. Off by default because the server cert has chain errors. |

### Components

1. **Embedded C# helper (`Add-Type`)** — `WsProbe.Run(url, payload, timeoutMs)`:
   sets the certificate-validation callback (accept-all unless `-VerifyCert`),
   forces TLS 1.2, connects a `ClientWebSocket`, sends `payload` as one binary
   frame, receives one reply, aborts the socket (no lingering close handshake —
   the server does not complete one), and returns the reply bytes. Writes a
   one-line `STAGE:` marker after handshake, send, and receive.

2. **Protobuf encoder** — helper functions `S` (UTF-8 bytes of a string) and
   `LD` (length-delimited field: tag byte + varint length + payload). Used to
   build `LoginRequest`, then wrap it as `ClientMessage` field 2.

3. **Protobuf decoder** — a minimal walker that reads length-delimited fields
   from the reply: locate `ClientMessage` field 3 (`loginresponse`), then
   within it `LoginResponse` field 2 (`status`). Returns the `status` string.

### Flow

1. Validate parameters. Missing `-Serial` → write an error and exit `2`
   **before** opening any connection.
2. Build the login `ClientMessage` bytes.
3. Call `WsProbe.Run`.
4. Decode the reply to extract `status`.
5. Report:
   - `status == "success"` → print **PASS** with the login token if present;
     exit `0`.
   - any other `status` → print **FAIL** with the status verbatim; exit `1`.

### Error handling

Each failure mode is reported with its own clear message, not a raw stack trace:

| Condition                         | Message                                          | Exit |
|------------------------------------|--------------------------------------------------|------|
| `-Serial` omitted                  | `Serial is required` (before connecting)         | 2    |
| TCP connection refused / no route  | `Cannot reach <url> (connection refused)`        | 3    |
| TLS handshake failure              | `TLS handshake failed: <reason>`                 | 3    |
| WebSocket upgrade rejected         | `WebSocket handshake rejected: <reason>`         | 3    |
| Connected, no reply within timeout | `Connected and sent, but no reply within Ns`     | 4    |
| Reply received but not parseable   | `Could not parse server response` + hex dump     | 4    |
| Reply parsed, status != success    | `FAIL: <status>`                                 | 1    |
| Reply parsed, status == success    | `PASS`                                           | 0    |

## Test cases (documented in README.md)

| Case                  | Inputs                                              | Expected                          |
|-----------------------|-----------------------------------------------------|-----------------------------------|
| Valid login           | real `-Serial`, correct `-Login`/`-Pwd`             | `PASS`, `status:"success"`, exit 0 |
| Wrong password        | real `-Serial`, wrong `-Pwd`                        | `FAIL: invalid credentials`, exit 1 |
| Bad serial            | bogus `-Serial`                                     | `FAIL: invalid SerialNo`, exit 1   |
| Missing serial        | `-Serial` omitted                                   | param error, exit 2, no connection |
| Unreachable server    | `-Url` pointing at a closed port                    | connection error, exit 3           |

The probe run during design used `-Serial PROBE-SERIAL` and correctly produced
`FAIL: invalid SerialNo`, confirming the bad-serial case end to end.

## Out of scope (YAGNI)

- Heartbeat / keepalive messages.
- Any post-login message exchange (fetch requests, subscriptions).
- Retry / reconnect logic.
- Generating or using the server's full 90-message `.proto` schema.
- A Python or C++ variant of the test.

## Success criteria

- `login_test.ps1` runs on the current Windows machine with no installs.
- Against a reachable server, it produces a clear PASS/FAIL line and a matching
  exit code for each test case above.
- `README.md` documents parameters, the five test cases, and how to obtain a
  valid `-Serial`.
