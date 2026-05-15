# WebSocket Login Test Scenario Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build a PowerShell test script that connects to an OrikaServer over `wss://`, sends a `login` request, and reports PASS/FAIL based on the server's `LOGIN_STATUS` reply.

**Architecture:** A pure-function protobuf codec (`protocodec.ps1`) hand-encodes the login `ClientMessage` and hand-decodes the reply — no `protoc`, no generated bindings. The main script (`login_test.ps1`) embeds a small compiled C# `OrikaWsClient` helper (the TLS-bypass callback must run on the WebSocket worker thread, where a PowerShell scriptblock cannot). The codec is unit-tested against golden byte vectors captured from a verified live probe.

**Tech Stack:** Windows PowerShell 5.1, embedded C# via `Add-Type` (`System.Net.WebSockets.ClientWebSocket`). No external installs.

**Note on file layout:** The approved spec listed `login_test.ps1` + `README.md`. This plan adds two files — `protocodec.ps1` (so the protobuf logic is a dot-sourceable, unit-testable unit) and `protocodec.tests.ps1` (its tests). This is a deliberate testability refinement, not a scope change.

## File Structure

```
tests/ws-login/
  protocodec.ps1          Pure protobuf encode/decode functions. No network, no global state.
  protocodec.tests.ps1    Plain-PowerShell unit tests for protocodec.ps1 (golden vectors).
  login_test.ps1          Param block, embedded C# WebSocket client, orchestration, PASS/FAIL.
  README.md               Usage, parameters, exit codes, test cases, how to get a -Serial.
```

All work happens on branch `feature/ws-login-test` (already created).

---

### Task 1: Protobuf encoder + its test harness

**Files:**
- Create: `tests/ws-login/protocodec.ps1`
- Test: `tests/ws-login/protocodec.tests.ps1`

- [ ] **Step 1: Write the failing test file**

Create `tests/ws-login/protocodec.tests.ps1` with the test harness and Task 1 cases:

```powershell
# protocodec.tests.ps1 - unit tests for protocodec.ps1. Plain PowerShell, no
# framework. Run: powershell -ExecutionPolicy Bypass -File protocodec.tests.ps1
# Exit code = number of failed tests (0 = all passed).
. "$PSScriptRoot\protocodec.ps1"

$script:Failures = 0

function Test-Case {
    param([string]$Name, [scriptblock]$Body)
    try { & $Body; Write-Host "  PASS  $Name" -ForegroundColor Green }
    catch {
        Write-Host "  FAIL  $Name" -ForegroundColor Red
        Write-Host "        $($_.Exception.Message)" -ForegroundColor Red
        $script:Failures++
    }
}
function Assert-HexEqual {
    param([byte[]]$Expected, [byte[]]$Actual)
    $e = [BitConverter]::ToString([byte[]]$Expected)
    $a = [BitConverter]::ToString([byte[]]$Actual)
    if ($e -ne $a) { throw "expected [$e] but got [$a]" }
}
function Assert-Equal {
    param($Expected, $Actual)
    if ($Expected -ne $Actual) { throw "expected [$Expected] but got [$Actual]" }
}

# === TEST CASES (insert new Test-Case blocks above the summary marker) ===

Test-Case 'ConvertTo-ProtoBytes encodes UTF-8' {
    Assert-HexEqual ([byte[]]@(0x6C,0x6F,0x67,0x69,0x6E)) (ConvertTo-ProtoBytes 'login')
}
Test-Case 'ConvertTo-Varint 0'   { Assert-HexEqual ([byte[]]@(0x00))      (ConvertTo-Varint 0) }
Test-Case 'ConvertTo-Varint 5'   { Assert-HexEqual ([byte[]]@(0x05))      (ConvertTo-Varint 5) }
Test-Case 'ConvertTo-Varint 127' { Assert-HexEqual ([byte[]]@(0x7F))      (ConvertTo-Varint 127) }
Test-Case 'ConvertTo-Varint 128' { Assert-HexEqual ([byte[]]@(0x80,0x01)) (ConvertTo-Varint 128) }
Test-Case 'ConvertTo-Varint 300' { Assert-HexEqual ([byte[]]@(0xAC,0x02)) (ConvertTo-Varint 300) }
Test-Case 'New-ProtoField wraps tag+len+payload' {
    $f = New-ProtoField 1 (ConvertTo-ProtoBytes 'login')
    Assert-HexEqual ([byte[]]@(0x0A,0x05,0x6C,0x6F,0x67,0x69,0x6E)) $f
}
Test-Case 'New-ProtoField uses multi-byte varint length' {
    $f = New-ProtoField 1 (New-Object byte[] 200)
    Assert-HexEqual ([byte[]]@(0x0A,0xC8,0x01)) ([byte[]]$f[0..2])
    Assert-Equal 203 $f.Length
}

# --- summary ---
Write-Host ""
if ($script:Failures -gt 0) {
    Write-Host "$($script:Failures) test(s) FAILED." -ForegroundColor Red
    exit $script:Failures
}
Write-Host "All tests passed." -ForegroundColor Green
exit 0
```

- [ ] **Step 2: Run the test to verify it fails**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/protocodec.tests.ps1`
Expected: FAIL — dot-sourcing errors because `tests/ws-login/protocodec.ps1` does not exist yet.

- [ ] **Step 3: Write the minimal implementation**

Create `tests/ws-login/protocodec.ps1`:

```powershell
# protocodec.ps1 - minimal protobuf encode/decode for the OrikaServer login
# test. Pure functions only: no network, no global state. Dot-source to use.

function ConvertTo-ProtoBytes {
    # UTF-8 encode a string to a byte array.
    param([Parameter(Mandatory)][AllowEmptyString()][string]$Text)
    return [System.Text.Encoding]::UTF8.GetBytes($Text)
}

function ConvertTo-Varint {
    # Encode a non-negative integer as a protobuf base-128 varint.
    param([Parameter(Mandatory)][int]$Value)
    $bytes = New-Object System.Collections.Generic.List[byte]
    $v = $Value
    do {
        $b = $v -band 0x7F
        $v = [int]($v -shr 7)
        if ($v -gt 0) { $b = $b -bor 0x80 }
        $bytes.Add([byte]$b)
    } while ($v -gt 0)
    return ,$bytes.ToArray()
}

function New-ProtoField {
    # Build a length-delimited (wire type 2) protobuf field:
    # tag byte + varint length + payload.
    param(
        [Parameter(Mandatory)][int]$FieldNumber,
        [Parameter(Mandatory)][AllowEmptyCollection()][byte[]]$Payload
    )
    $tag = [byte](($FieldNumber -shl 3) -bor 2)
    $len = ConvertTo-Varint $Payload.Length
    return ,([byte[]](@($tag) + $len + $Payload))
}
```

- [ ] **Step 4: Run the test to verify it passes**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/protocodec.tests.ps1`
Expected: PASS — all 8 cases print `PASS`, final line `All tests passed.`, exit code 0.

- [ ] **Step 5: Commit**

```bash
git add tests/ws-login/protocodec.ps1 tests/ws-login/protocodec.tests.ps1
git commit -m "test(ws-login): protobuf field encoder + test harness"
```

---

### Task 2: Login message builder

**Files:**
- Modify: `tests/ws-login/protocodec.ps1`
- Test: `tests/ws-login/protocodec.tests.ps1`

- [ ] **Step 1: Write the failing test**

In `tests/ws-login/protocodec.tests.ps1`, insert this block immediately above the `# --- summary ---` line:

```powershell
Test-Case 'New-LoginMessage matches the verified 49-byte golden vector' {
    $expected = [byte[]]@(
        0x0A,0x05,0x6C,0x6F,0x67,0x69,0x6E,
        0x12,0x28,
        0x0A,0x05,0x6C,0x6F,0x67,0x69,0x6E,
        0x12,0x04,0x31,0x30,0x30,0x31,
        0x1A,0x0B,0x68,0x65,0x6C,0x6C,0x6F,0x40,0x31,0x32,0x33,0x34,0x35,
        0x22,0x0C,0x50,0x52,0x4F,0x42,0x45,0x2D,0x53,0x45,0x52,0x49,0x41,0x4C)
    $actual = New-LoginMessage -Login '1001' -Pwd 'hello@12345' -Serial 'PROBE-SERIAL'
    Assert-HexEqual $expected $actual
}
```

(This golden vector was captured from the verified live probe: a `ClientMessage{ type:"login", loginrequest:{ type:"login", login:"1001", pwd:"hello@12345", serialNo:"PROBE-SERIAL" } }`.)

- [ ] **Step 2: Run the test to verify it fails**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/protocodec.tests.ps1`
Expected: FAIL — the new case fails with `New-LoginMessage` not recognized; exit code 1.

- [ ] **Step 3: Write the minimal implementation**

Append to `tests/ws-login/protocodec.ps1`:

```powershell
function New-LoginMessage {
    # Build the full Oreka.ClientMessage bytes for a login request.
    # ClientMessage { type=1, loginrequest=2 };
    # LoginRequest { type=1, login=2, pwd=3, serialNo=4 }.
    param(
        [Parameter(Mandatory)][string]$Login,
        [Parameter(Mandatory)][string]$Pwd,
        [Parameter(Mandatory)][AllowEmptyString()][string]$Serial
    )
    $loginRequest = @()
    $loginRequest += New-ProtoField 1 (ConvertTo-ProtoBytes 'login')
    $loginRequest += New-ProtoField 2 (ConvertTo-ProtoBytes $Login)
    $loginRequest += New-ProtoField 3 (ConvertTo-ProtoBytes $Pwd)
    $loginRequest += New-ProtoField 4 (ConvertTo-ProtoBytes $Serial)

    $clientMessage = @()
    $clientMessage += New-ProtoField 1 (ConvertTo-ProtoBytes 'login')
    $clientMessage += New-ProtoField 2 ([byte[]]$loginRequest)
    return ,([byte[]]$clientMessage)
}
```

- [ ] **Step 4: Run the test to verify it passes**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/protocodec.tests.ps1`
Expected: PASS — all 9 cases pass; exit code 0.

- [ ] **Step 5: Commit**

```bash
git add tests/ws-login/protocodec.ps1 tests/ws-login/protocodec.tests.ps1
git commit -m "test(ws-login): build login ClientMessage from credentials"
```

---

### Task 3: Reply decoder

**Files:**
- Modify: `tests/ws-login/protocodec.ps1`
- Test: `tests/ws-login/protocodec.tests.ps1`

- [ ] **Step 1: Write the failing test**

In `tests/ws-login/protocodec.tests.ps1`, insert this block immediately above the `# --- summary ---` line:

```powershell
Test-Case 'Read-LoginStatus decodes the verified 48-byte reply' {
    $reply = [byte[]]@(
        0x0A,0x0C,0x4C,0x4F,0x47,0x49,0x4E,0x5F,0x53,0x54,0x41,0x54,0x55,0x53,
        0x1A,0x20,
        0x0A,0x0C,0x4C,0x4F,0x47,0x49,0x4E,0x5F,0x53,0x54,0x41,0x54,0x55,0x53,
        0x12,0x10,0x69,0x6E,0x76,0x61,0x6C,0x69,0x64,0x20,
        0x53,0x65,0x72,0x69,0x61,0x6C,0x4E,0x6F)
    $result = Read-LoginStatus -Reply $reply
    Assert-Equal 'invalid SerialNo' $result.Status
    Assert-Equal '' $result.Token
}
Test-Case 'Read-ProtoFields throws on truncated payload' {
    $bad = [byte[]]@(0x0A, 0x05, 0x41)  # field claims 5 bytes, only 1 present
    $threw = $false
    try { Read-ProtoFields $bad } catch { $threw = $true }
    Assert-Equal $true $threw
}
```

(The reply vector was captured from the verified live probe: a `ClientMessage{ type:"LOGIN_STATUS", loginresponse: LoginResponse{ type:"LOGIN_STATUS", status:"invalid SerialNo" } }`.)

- [ ] **Step 2: Run the test to verify it fails**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/protocodec.tests.ps1`
Expected: FAIL — the new cases fail with `Read-LoginStatus` / `Read-ProtoFields` not recognized.

- [ ] **Step 3: Write the minimal implementation**

Append to `tests/ws-login/protocodec.ps1`:

```powershell
function Read-ProtoFields {
    # Walk a protobuf message, returning each length-delimited (wire type 2)
    # field as an object with .Field (number) and .Bytes (payload).
    # Throws on a non-LEN wire type or truncated input.
    param([Parameter(Mandatory)][byte[]]$Bytes)
    $fields = @()
    $i = 0
    while ($i -lt $Bytes.Length) {
        $tag = 0; $shift = 0
        do {
            if ($i -ge $Bytes.Length) { throw 'truncated tag' }
            $b = $Bytes[$i]; $i++
            $tag = $tag -bor (($b -band 0x7F) -shl $shift)
            $shift += 7
        } while ($b -band 0x80)
        $fieldNumber = $tag -shr 3
        $wireType = $tag -band 0x07
        if ($wireType -ne 2) { throw "unsupported wire type $wireType for field $fieldNumber" }

        $len = 0; $shift = 0
        do {
            if ($i -ge $Bytes.Length) { throw 'truncated length' }
            $b = $Bytes[$i]; $i++
            $len = $len -bor (($b -band 0x7F) -shl $shift)
            $shift += 7
        } while ($b -band 0x80)
        if ($i + $len -gt $Bytes.Length) { throw 'truncated payload' }

        $payload = New-Object byte[] $len
        if ($len -gt 0) { [Array]::Copy($Bytes, $i, $payload, 0, $len) }
        $i += $len
        $fields += [PSCustomObject]@{ Field = $fieldNumber; Bytes = $payload }
    }
    return ,$fields
}

function Read-LoginStatus {
    # Decode a server reply (Oreka.ClientMessage) and return an object with
    # .Status and .Token. ClientMessage field 3 = loginresponse;
    # LoginResponse field 2 = status, field 3 = logintoken.
    param([Parameter(Mandatory)][byte[]]$Reply)
    $top = Read-ProtoFields $Reply
    $loginResponse = $top | Where-Object { $_.Field -eq 3 } | Select-Object -First 1
    if (-not $loginResponse) { throw 'no loginresponse field (3) in reply' }
    $inner = Read-ProtoFields $loginResponse.Bytes
    $statusField = $inner | Where-Object { $_.Field -eq 2 } | Select-Object -First 1
    $tokenField  = $inner | Where-Object { $_.Field -eq 3 } | Select-Object -First 1
    $status = if ($statusField) { [System.Text.Encoding]::UTF8.GetString($statusField.Bytes) } else { '' }
    $token  = if ($tokenField)  { [System.Text.Encoding]::UTF8.GetString($tokenField.Bytes) } else { '' }
    return [PSCustomObject]@{ Status = $status; Token = $token }
}
```

- [ ] **Step 4: Run the test to verify it passes**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/protocodec.tests.ps1`
Expected: PASS — all 11 cases pass; exit code 0.

- [ ] **Step 5: Commit**

```bash
git add tests/ws-login/protocodec.ps1 tests/ws-login/protocodec.tests.ps1
git commit -m "test(ws-login): decode LOGIN_STATUS reply"
```

---

### Task 4: Main script — parameter block and `-Serial` validation

**Files:**
- Create: `tests/ws-login/login_test.ps1`

- [ ] **Step 1: Define the expected behavior (the test)**

The test for this task is a direct invocation. Expected after Step 3: running the
script with no `-Serial` prints `ERROR: -Serial is required ...` and exits with
code 2, **without** opening any connection.

- [ ] **Step 2: Run to verify it fails**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/login_test.ps1`
Expected: FAIL — the file does not exist yet; PowerShell reports it cannot find the script.

- [ ] **Step 3: Write the minimal implementation**

Create `tests/ws-login/login_test.ps1`:

```powershell
<#
.SYNOPSIS
  Connects to an OrikaServer over wss://, sends a login request, and reports
  whether the server accepts it. See README.md for usage and test cases.
#>
[CmdletBinding()]
param(
    [string]$Url = 'wss://auttrading.com:85',
    [string]$Login = '1001',
    [string]$Pwd = 'hello@12345',
    [string]$Serial,
    [int]$Timeout = 12,
    [switch]$VerifyCert
)

$ErrorActionPreference = 'Stop'

if ([string]::IsNullOrWhiteSpace($Serial)) {
    Write-Host 'ERROR: -Serial is required (the registered device serial for this login).' -ForegroundColor Red
    exit 2
}

Write-Host "Parameters OK (Url=$Url, Login=$Login, Serial set)."
```

(The final `Write-Host` is a temporary marker; Task 5 replaces it with the real
orchestration.)

- [ ] **Step 4: Run to verify it passes**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/login_test.ps1`
Expected: prints `ERROR: -Serial is required ...`; `echo $LASTEXITCODE` shows `2`.

Also run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/login_test.ps1 -Serial TEST`
Expected: prints `Parameters OK ...`; exit code 0.

- [ ] **Step 5: Commit**

```bash
git add tests/ws-login/login_test.ps1
git commit -m "feat(ws-login): login_test.ps1 parameter block and -Serial check"
```

---

### Task 5: Main script — WebSocket client and orchestration

**Files:**
- Modify: `tests/ws-login/login_test.ps1`

- [ ] **Step 1: Define the expected behavior (the test)**

This is an integration test against the live server. Expected after Step 3:
running with a bogus serial connects, sends the login, receives the reply, and
prints `FAIL: login rejected (status='invalid SerialNo').` with exit code 1.
(The design-phase probe confirmed this exact outcome.)

- [ ] **Step 2: Run to verify it fails**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/login_test.ps1 -Serial PROBE-SERIAL`
Expected: prints only `Parameters OK ...` and exits 0 — the script does not yet connect or report PASS/FAIL.

- [ ] **Step 3: Write the implementation**

In `tests/ws-login/login_test.ps1`, replace the single line:

```powershell
Write-Host "Parameters OK (Url=$Url, Login=$Login, Serial set)."
```

with:

```powershell
. (Join-Path $PSScriptRoot 'protocodec.ps1')

$csharp = @'
using System;
using System.Net;
using System.Net.WebSockets;
using System.Threading;
public class OrikaWsClient {
    public static string LastStage = "";
    public static byte[] Send(string url, byte[] payload, int timeoutMs, bool verifyCert) {
        LastStage = "connect";
        if (!verifyCert) {
            ServicePointManager.ServerCertificateValidationCallback = delegate { return true; };
        }
        ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
        using (var ws = new ClientWebSocket()) {
            var cts = new CancellationTokenSource(timeoutMs);
            ws.ConnectAsync(new Uri(url), cts.Token).GetAwaiter().GetResult();
            LastStage = "send";
            ws.SendAsync(new ArraySegment<byte>(payload), WebSocketMessageType.Binary, true, cts.Token).GetAwaiter().GetResult();
            LastStage = "receive";
            var buf = new byte[65536];
            var res = ws.ReceiveAsync(new ArraySegment<byte>(buf), cts.Token).GetAwaiter().GetResult();
            LastStage = "done";
            var outb = new byte[res.Count];
            Array.Copy(buf, outb, res.Count);
            try { ws.Abort(); } catch {}
            return outb;
        }
    }
}
'@
if (-not ('OrikaWsClient' -as [type])) {
    Add-Type -TypeDefinition $csharp -ReferencedAssemblies System.dll
}

$payload = New-LoginMessage -Login $Login -Pwd $Pwd -Serial $Serial
Write-Host ("Connecting to {0} (login='{1}', {2}-byte login frame) ..." -f $Url, $Login, $payload.Length)

try {
    $reply = [OrikaWsClient]::Send($Url, $payload, ($Timeout * 1000), [bool]$VerifyCert)
} catch {
    $stage = [OrikaWsClient]::LastStage
    $ex = $_.Exception
    while ($ex.InnerException) { $ex = $ex.InnerException }
    $name = $ex.GetType().Name
    if ($stage -eq 'connect' -or $stage -eq '') {
        if ($name -eq 'AuthenticationException') {
            Write-Host "ERROR: TLS handshake failed: $($ex.Message)" -ForegroundColor Red
        } elseif ($name -eq 'SocketException') {
            Write-Host "ERROR: cannot reach $Url ($($ex.Message))" -ForegroundColor Red
        } else {
            Write-Host "ERROR: WebSocket handshake rejected: $($ex.Message)" -ForegroundColor Red
        }
        exit 3
    } else {
        Write-Host "ERROR: connected and sent, but no reply within $Timeout s ($name)." -ForegroundColor Red
        exit 4
    }
}

Write-Host ("Received {0} bytes from server." -f $reply.Length)

try {
    $result = Read-LoginStatus -Reply $reply
} catch {
    Write-Host "ERROR: could not parse server response: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host ("Raw reply hex: " + [BitConverter]::ToString($reply))
    exit 4
}

if ($result.Status -eq 'success') {
    Write-Host "PASS: login succeeded (status='success')." -ForegroundColor Green
    if ($result.Token) { Write-Host ("  logintoken: {0}" -f $result.Token) }
    exit 0
} else {
    Write-Host ("FAIL: login rejected (status='{0}')." -f $result.Status) -ForegroundColor Red
    exit 1
}
```

- [ ] **Step 4: Run to verify it passes**

Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/login_test.ps1 -Serial PROBE-SERIAL`
Expected output:
```
Connecting to wss://auttrading.com:85 (login='1001', 49-byte login frame) ...
Received 48 bytes from server.
FAIL: login rejected (status='invalid SerialNo').
```
`echo $LASTEXITCODE` shows `1`.

Also re-run the codec unit tests to confirm nothing regressed:
Run: `powershell -ExecutionPolicy Bypass -File tests/ws-login/protocodec.tests.ps1`
Expected: `All tests passed.`, exit code 0.

- [ ] **Step 5: Commit**

```bash
git add tests/ws-login/login_test.ps1
git commit -m "feat(ws-login): connect over wss, send login, report PASS/FAIL"
```

---

### Task 6: README

**Files:**
- Create: `tests/ws-login/README.md`

- [ ] **Step 1: Write the README**

Create `tests/ws-login/README.md`:

````markdown
# WebSocket Login Test

Connects to an OrikaServer over `wss://`, sends a `login` request, and reports
whether the server accepts it.

## Requirements

Windows PowerShell 5.1 (built into Windows 10). No other installs — the script
embeds a small C# WebSocket client and hand-encodes the protobuf message.

## Usage

```powershell
powershell -ExecutionPolicy Bypass -File login_test.ps1 -Serial <DEVICE_SERIAL>
```

Defaults target `wss://auttrading.com:85` as user `1001`. Override any value:

```powershell
powershell -ExecutionPolicy Bypass -File login_test.ps1 `
  -Url wss://auttrading.com:85 -Login 1001 -Pwd 'hello@12345' -Serial ABC123
```

## Parameters

| Parameter     | Default                   | Notes                                            |
|---------------|---------------------------|--------------------------------------------------|
| `-Url`        | `wss://auttrading.com:85` | Target server.                                   |
| `-Login`      | `1001`                    | User id.                                         |
| `-Pwd`        | `hello@12345`             | Password.                                        |
| `-Serial`     | *(required)*              | Registered device serial. Script exits 2 if missing. |
| `-Timeout`    | `12`                      | Seconds for connect / send / receive.            |
| `-VerifyCert` | *(off)*                   | When set, enforce TLS cert validation. Off by default because the server cert fails chain validation. |

## Exit codes

| Code | Meaning                                            |
|------|----------------------------------------------------|
| 0    | PASS — server returned `status:"success"`.         |
| 1    | FAIL — server rejected the login (see status text).|
| 2    | `-Serial` was not supplied.                        |
| 3    | Could not connect (TCP / TLS / WebSocket handshake).|
| 4    | Connected but no usable reply (timeout or unparseable).|

## Test cases

| Case               | Command                                                        | Expected                                |
|--------------------|----------------------------------------------------------------|-----------------------------------------|
| Valid login        | `-Serial <real serial>` with correct `-Login`/`-Pwd`           | `PASS`, status `success`, exit 0        |
| Wrong password     | `-Serial <real serial> -Pwd wrong`                             | `FAIL: ... invalid credentials`, exit 1 |
| Bad serial         | `-Serial PROBE-SERIAL`                                         | `FAIL: ... invalid SerialNo`, exit 1    |
| Missing serial     | (omit `-Serial`)                                               | param error, exit 2, no connection      |
| Unreachable server | `-Url wss://auttrading.com:9999 -Serial X`                     | connection error, exit 3                |

Note: `serialNo` is validated *before* the password, so a bad serial yields
`invalid SerialNo` and the password is never checked.

## Getting a valid `-Serial`

The serial must match a row in the server DB table `orika_userLoginHDkeyMapping`
for the given `userlogin` — either an exact `HDno` for that user, or a row with
`HDno = '*'` (wildcard). Ask the server/DB owner for a registered serial.

## Unit tests

The protobuf encode/decode logic has standalone unit tests:

```powershell
powershell -ExecutionPolicy Bypass -File protocodec.tests.ps1
```

Exit code 0 means all passed.
````

- [ ] **Step 2: Verify it renders**

Open `tests/ws-login/README.md` and confirm the tables render and there are no
broken code fences.

- [ ] **Step 3: Commit**

```bash
git add tests/ws-login/README.md
git commit -m "docs(ws-login): usage, exit codes, and test cases"
```

---

## Self-Review

**Spec coverage:**
- Purpose / no-installs constraint → Tasks 1–6 use only PowerShell + embedded C#. ✔
- Verified protocol & field numbers → encoded in Tasks 2–3 and their golden vectors. ✔
- PowerShell + C# helper approach → Task 5. ✔
- Parameters (`-Url`/`-Login`/`-Pwd`/`-Serial`/`-Timeout`/`-VerifyCert`) → Task 4 param block, `-VerifyCert` wired in Task 5. ✔
- Encoder / decoder components → Tasks 1–3. ✔
- Flow (validate → build → send → decode → PASS/FAIL → exit) → Tasks 4–5. ✔
- Error handling table (exit 2/3/4, distinct messages) → Task 4 (exit 2) and Task 5 (exit 3/4). ✔
- Test cases table → documented in Task 6 README; bad-serial and missing-serial cases executed in Tasks 4–5. ✔
- File layout → `protocodec.ps1`, `protocodec.tests.ps1`, `login_test.ps1`, `README.md`. ✔ (two files beyond the spec, noted above.)
- Out of scope items → none introduced. ✔

**Type consistency:** `ConvertTo-ProtoBytes`, `ConvertTo-Varint`, `New-ProtoField`, `New-LoginMessage`, `Read-ProtoFields`, `Read-LoginStatus` are defined once and called with matching signatures. `Read-LoginStatus` returns `.Status`/`.Token`, consumed exactly so in Task 5. The C# type `OrikaWsClient` with `Send(...)` and `LastStage` is defined and used consistently in Task 5.

**Placeholders:** none — every step contains complete code or exact commands. The one temporary code line in Task 4 is explicitly called out and replaced in Task 5.
