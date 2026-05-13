# Debug OrikaServer Silent Launch Failure — Design

**Date**: 2026-05-13
**Scope**: One-shot diagnosis. No new infrastructure.
**Author**: Brainstormed via superpowers:brainstorming skill.

## Context

After the audit-fix work (commits `f207e4f` through `0b2fc5a`) and a Release|x64
rebuild, `OrikaServer.exe` launches but exits silently within seconds.
Symptoms observed:

- No `error_log.txt` written (suggests TLS init reached `BIO_new_file` and
  succeeded, OR the process died before reaching `ssl_init`).
- No Windows Event Log entries for the process.
- No crash dump on disk.
- No new daily log file at `D:\logs\2026-05-13.log` (the cwd-relative log
  location the `CLogFile` walks to via `_chdir("..")` x 20 — audit D9).
- `tasklist` briefly shows `OrikaServer.exe` then it's gone.
- User confirmed double-clicking opens a dialog, but by the time we re-checked
  the process was gone.

We cannot interact with the GUI from this background shell, and the process
provides no observable failure signal in normal Windows logging.

## Design — 3 phases

### Phase 0: Bisect with the backup binary

Run `x64/Release/OrikaServer.exe.before-20260513-175557.bak` (the pre-audit-fix
binary backed up before we overwrote it). Launch it from the same `x64/Release/`
working directory and observe whether it stays alive.

Outcomes:
- **A. Backup also exits silently** — Not an audit-fix regression. The issue is
  environmental: missing DLL variant, path-sensitive code, SQL connection
  failure, MT5 init failure, or an MFC dialog quirk in `OnInitDialog`.
- **B. Backup runs successfully** — One of the audit fixes regressed startup.
  Bisect among commits `f207e4f` → `2c2047b` → `c61482d` → `bd1e3f2` → `d93ff93`
  → `928fcdc` to find the offender.

### Phase 1: Branch on Phase 0 outcome

**If Outcome A (environment issue):**
- Run **Sysinternals Process Monitor** with filter `Process Name is
  OrikaServer.exe`. Launch the EXE.
- Read the last few entries before process exit. Typical signals:
  - `NAME_NOT_FOUND` on a DLL → missing runtime dep
  - `NAME_NOT_FOUND` on a config/data file → path-relative load failure
  - `CreateProcess` failure on a child → MT5 helper or postbuild
  - Last operation gives the cause.
- Fix the specific resource issue (copy file, fix path, etc.).

**If Outcome B (audit-fix regression):**
- Check out commit `dca9a62` (BFG-rewritten `f207e4f` — the pre-audit-fix
  cleanup commit) and rebuild. If still good, walk forward one fix at a time:
  - `e14a73b` (Phase 1: S1/S8/S11/S12/CR5) — most likely candidate is CR5
    (removed `CManager()` temporary from `CManager::Initialize`)
  - `2d5370a` (Phase 2: S2/S5/S9/CR4)
  - `936e047` (Phase 3 partial: S4/S6/CR1)
  - `9029de9` (S3 refcount lifetime — significant risk; new behaviour in
    `session_init` + `session_accept`/`session_recv`/`session_send` and
    `session_on_completed_packets`)
- First commit that breaks startup is the regression.

### Phase 2: Targeted fix

Apply the minimum-scope fix derived from Phase 1:
- Environment: copy the missing file, fix the path, update config, etc.
- Regression: revert just the breaking change, or correct it. If S3 refcount
  initial `psession->refcount = 1` is wrong (e.g., should be 2 if the listener
  socket's first AcceptEx also AddRef's, leading to a premature free), adjust.

## Out of scope

- New logging infrastructure (user picked "narrow scope" — diagnose now, don't
  build instrumentation).
- Visual Studio debugger workflow (deferred unless Phases 0+1 are inconclusive).
- Architectural improvements (e.g., separating GUI dialog from server core so
  headless launch is possible).
- SQL Server / MT5 credential rotation. Those remain audit R1/R2 follow-ups.

## Critical files / paths

- `D:\VSCodeProject\orikaserver_proto\x64\Release\OrikaServer.exe` — current build
- `D:\VSCodeProject\orikaserver_proto\x64\Release\OrikaServer.exe.before-20260513-175557.bak`
  — backup pre-audit binary
- `D:\VSCodeProject\orikaserver_proto\x64\Release\` — working directory at
  launch; expected to contain `Certificate.pem`, `key.pem`, `oreka.config`,
  `gd_bundle.crt`, runtime DLLs
- `D:\VSCodeProject\orikaserver_proto\OrikaServer\OrikaServerDlg.cpp` —
  `InitInstance`, `OnInitDialog`, `OnBnClickedStart`
- Audit fix commits (post-BFG SHAs):
  `dca9a62 e14a73b 2d5370a 936e047 9029de9 f8e5007 8572436 5fb37fc`

## Success criterion

`OrikaServer.exe` stays alive for more than 30 seconds after launch AND writes
at least one log line to `D:\logs\2026-05-13.log`. That confirms the MFC
dialog completed `OnInitDialog`, the logger initialised, and the process is
serving (or at least ready to serve, awaiting Start button click).

## Verification

After applying the fix:
1. `tasklist | grep -i orika` — process listed, with stable PID over multiple
   30 s checks.
2. `ls -la D:/logs/2026-05-13.log` — file exists, non-empty, growing.
3. (Optional) `openssl s_client -connect <host>:<port>` — TLS handshake
   completes with `notAfter=Jan 12 13:23:46 2027`.
