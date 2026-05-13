# Debug OrikaServer Silent Launch Failure — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Find why `OrikaServer.exe` exits silently after launch and apply the minimum fix to make it stay alive.

**Architecture:** Diagnostic-first, not code-first. Three tasks form a decision tree:
1. **Task 1 (always run)** establishes whether this is an audit-fix regression or an environment issue, by re-running the pre-audit backup binary.
2. **Task 2A** (only if Task 1 shows regression) bisects the audit-fix commits.
3. **Task 2B** (only if Task 1 shows environment issue) captures a Process Monitor trace.
4. **Task 3** applies the targeted fix once cause is identified.

No new code, tests, or instrumentation. Read-only diagnosis until the cause is known.

**Tech Stack:** Windows 10, Git Bash, MSBuild 17 (VS 2022), Sysinternals Process Monitor (procmon, downloaded as needed).

**Spec:** `docs/superpowers/specs/2026-05-13-debug-server-launch-design.md`

---

## Task 1: Run the backup binary (Phase 0 bisect)

**Files:**
- Read-only: `D:\VSCodeProject\orikaserver_proto\x64\Release\OrikaServer.exe.before-20260513-175557.bak`
- Working dir: `D:\VSCodeProject\orikaserver_proto\x64\Release\`

- [ ] **Step 1: Confirm the backup binary is intact**

Run:
```bash
ls -la x64/Release/OrikaServer.exe.before-20260513-175557.bak
```

Expected output: file exists, size around 3.49 MB, owned by user, NOT zero bytes. If missing, the bisect is impossible — abort and skip to Task 2B.

- [ ] **Step 2: Confirm no `OrikaServer.exe` is currently running**

Run:
```bash
tasklist 2>&1 | grep -i orika
```

Expected: no output (empty). If something is listed, terminate it first:
```bash
taskkill /F /IM OrikaServer.exe
```

- [ ] **Step 3: Launch the backup binary from x64/Release**

Run:
```bash
(cd x64/Release && cmd //c start "OrikaServerBackup" OrikaServer.exe.before-20260513-175557.bak)
```

(`//c` is intentional — Git Bash needs the doubled slash for `cmd /c`.)

Note: this launches *visibly* (no `/MIN`) so any dialog/error popup is observable on screen.

- [ ] **Step 4: Wait 10 seconds, then check process state**

Run:
```bash
sleep 10
tasklist 2>&1 | grep -i orika
```

Decision based on output:

| Output | Outcome | Next Task |
|---|---|---|
| `OrikaServer.exe.before-...bak <PID> ...` listed | **B**: backup runs — audit-fix regression | Go to Task 2A |
| No output (process gone) | **A**: backup also dies — environment issue | Go to Task 2B |

- [ ] **Step 5: Record outcome in scratch notes**

Run:
```bash
echo "Task 1 outcome: $(date) - <A or B>" >> docs/superpowers/plans/2026-05-13-debug-server-launch.scratch.md
```

(Replace `<A or B>` with the actual letter.) This scratch file is gitignored output, not committed.

- [ ] **Step 6: Stop the backup binary**

If Outcome B (still running), shut it down so it doesn't interfere with Task 2A:
```bash
taskkill /F /IM OrikaServer.exe.before-20260513-175557.bak 2>&1
```

If Outcome A (already gone), nothing to do.

- [ ] **Step 7: Commit nothing**

Task 1 produces no code changes. The scratch file is not tracked. Skip commit.

---

## Task 2A: Bisect the audit-fix commits *(only if Task 1 Outcome B)*

**Files:**
- Read-only: git history `dca9a62` → `e14a73b` → `2d5370a` → `936e047` → `9029de9` → `f8e5007` → `8572436` → `5fb37fc` → `bfbd582`
- Modify: working tree (via `git checkout` to test each commit)
- Working dir: `D:\VSCodeProject\orikaserver_proto\`

- [ ] **Step 1: Save current HEAD reference**

Run:
```bash
git rev-parse HEAD > /tmp/orika-debug-original-head
cat /tmp/orika-debug-original-head
```

Expected output: a SHA like `bfbd582...`. Save it — we'll return to this at the end.

- [ ] **Step 2: Check out the first audit-fix commit**

Run:
```bash
git checkout e14a73b
git log -1 --oneline
```

Expected output: `e14a73b fix: Phase 1 audit fixes (S1, S8, S11, S12, CR5)`.

- [ ] **Step 3: Rebuild against this commit**

Run:
```bash
MSYS_NO_PATHCONV=1 "/c/Program Files/Microsoft Visual Studio/2022/Enterprise/MSBuild/Current/Bin/MSBuild.exe" OrikaServer/OrikaServer.vcxproj /p:Configuration=Release /p:Platform=x64 /p:SolutionDir="D:\VSCodeProject\orikaserver_proto\OrikaServer\\" /m /v:quiet /nologo /clp:Summary
```

Expected: `0 Error(s)` near the end. If build fails, log error and skip to Task 2B — build itself is the problem.

- [ ] **Step 4: Copy the just-built EXE into x64/Release**

Run:
```bash
cp OrikaServer/x64/Release/OrikaServer.exe x64/Release/OrikaServer.exe
ls -la x64/Release/OrikaServer.exe
```

Expected: timestamp is "just now".

- [ ] **Step 5: Launch and observe**

Run:
```bash
(cd x64/Release && cmd //c start "OrikaServerBisect" OrikaServer.exe)
sleep 10
tasklist 2>&1 | grep -i orika
```

Decision:
- **Process listed** → this commit is GOOD. Move forward one commit. Repeat steps 2-5 with the next commit in this order: `2d5370a` → `936e047` → `9029de9` → `f8e5007` → `8572436` → `5fb37fc` → `bfbd582`.
- **No output** → this commit BREAKS startup. The regression was introduced here. Continue to Step 6.

- [ ] **Step 6: Inspect the breaking commit's diff**

Run:
```bash
git show <breaking-sha> --stat
git show <breaking-sha>
```

Read each hunk carefully. The change in this commit is the regression cause.

- [ ] **Step 7: Stop any running instances and return to HEAD**

Run:
```bash
taskkill /F /IM OrikaServer.exe 2>&1
git checkout $(cat /tmp/orika-debug-original-head)
git log -1 --oneline
```

Expected output: original HEAD.

- [ ] **Step 8: Record the regression**

Append to scratch notes:
```bash
echo "Task 2A: regression at commit <SHA> <subject>" >> docs/superpowers/plans/2026-05-13-debug-server-launch.scratch.md
echo "Failing hunk(s): <brief description>" >> docs/superpowers/plans/2026-05-13-debug-server-launch.scratch.md
```

Proceed to Task 3.

---

## Task 2B: Process Monitor trace *(only if Task 1 Outcome A)*

**Files:**
- Download: `C:\Tools\procmon.exe` (Sysinternals Process Monitor)
- Read-only: `x64/Release/OrikaServer.exe` (current build)
- Working dir: `D:\VSCodeProject\orikaserver_proto\x64\Release\`

- [ ] **Step 1: Download Process Monitor**

Run:
```bash
mkdir -p /c/Tools
curl -fsSL -o /c/Tools/procmon.zip https://download.sysinternals.com/files/ProcessMonitor.zip
unzip -o /c/Tools/procmon.zip -d /c/Tools/ > /dev/null
ls /c/Tools/Procmon64.exe
```

Expected: `/c/Tools/Procmon64.exe` exists. If `unzip` is missing, use PowerShell:
```bash
powershell -NoProfile -Command "Expand-Archive -Path C:\Tools\procmon.zip -DestinationPath C:\Tools -Force"
```

- [ ] **Step 2: Launch Procmon in capture-from-start mode**

Run:
```bash
cmd //c start "Procmon" /c/Tools/Procmon64.exe /AcceptEula /Quiet /Backingfile C:\Tools\orika-trace.pml
```

Procmon opens, starts capturing. The `/Backingfile` writes to disk so we can save the trace.

- [ ] **Step 3: In Procmon, set a process-name filter**

This is a one-time GUI action: In Procmon's menu, **Filter → Filter…** Add a row:
- Column: `Process Name`
- Operator: `is`
- Value: `OrikaServer.exe`
- Action: `Include`

Click **Add**, then **OK**. This filters the capture to only OrikaServer events.

- [ ] **Step 4: Launch the OrikaServer binary**

Run:
```bash
(cd x64/Release && cmd //c start "OrikaServer" OrikaServer.exe)
sleep 5
```

Procmon will capture every file/registry/process operation OrikaServer does. The process will exit within seconds (we know).

- [ ] **Step 5: Stop Procmon capture (Ctrl+E in the Procmon window)**

GUI action. Then `File → Save…` and save the trace to `C:\Tools\orika-trace.pml`.

- [ ] **Step 6: Inspect the LAST 30 entries**

In Procmon, scroll to the bottom of the captured list. Look for the **last few operations before `Process Exit`**. Common culprits:

| Result column | Meaning |
|---|---|
| `NAME NOT FOUND` on `.dll` | Missing runtime dependency. Path of file = the missing DLL. |
| `NAME NOT FOUND` on a config file (e.g., `oreka.config`, `MT5\Manager\API\...`) | File missing or wrong path. |
| `ACCESS DENIED` | Permission issue (file/registry). |
| `Process Exit` with exit code `0xc0000135` | A DLL didn't load (STATUS_DLL_NOT_FOUND). Look at the `Load Image` events just before — last successful one tells you what was loading. |
| Process Exit with exit code `0xc0000409` | Stack buffer overrun. Probably an audit-fix regression that Phase 0 should have caught — re-check Task 1. |

- [ ] **Step 7: Record findings**

Append to scratch notes:
```bash
echo "Task 2B: last operations before exit:" >> docs/superpowers/plans/2026-05-13-debug-server-launch.scratch.md
# Then copy/paste the last 5-10 entries from Procmon
```

Save the .pml file for reference.

- [ ] **Step 8: Identify the missing/failing resource**

The Procmon "Path" column on the failing operation gives the exact file/registry key/etc. that wasn't found or wasn't accessible.

Example: if you see `NAME NOT FOUND C:\Windows\System32\vcruntime140.dll`, the Visual C++ runtime isn't installed.

Proceed to Task 3 with this specific cause.

---

## Task 3: Apply targeted fix

**Files:**
- Depends entirely on what Task 2A or 2B revealed.

- [ ] **Step 1: Form the fix hypothesis from your scratch notes**

Read `docs/superpowers/plans/2026-05-13-debug-server-launch.scratch.md`. State the cause in one sentence: "The server exited because X."

- [ ] **Step 2: Apply the fix**

Depending on what X is, choose ONE branch:

**Branch A: Missing DLL** (Task 2B found e.g. `vcruntime140.dll`)
```bash
# Copy from system32 or installer
cp "/c/Windows/System32/<missing>.dll" x64/Release/<missing>.dll
# Or for VC++ redist, install: https://aka.ms/vs/17/release/vc_redist.x64.exe
```

**Branch B: Missing config / cert / data file** (Task 2B found e.g. config file)
```bash
# Copy from known-good location
cp OrikaServer/<missing-file> x64/Release/<missing-file>
```

**Branch C: Audit-fix regression** (Task 2A pinpointed a commit)
Read the spec section "If Outcome B (audit-fix regression)" in `docs/superpowers/specs/2026-05-13-debug-server-launch-design.md`. Identify the specific change. Most likely candidates:
- **CR5** in `e14a73b`: `CManager()` deleted from `Initialize`. If THIS broke things, revert it:
  ```bash
  # Find the line and add CManager(); back where it was
  ```
  But CR5 was supposed to be safe — investigate carefully before reverting.
- **S3** in `9029de9`: refcount init `psession->refcount = 1` — wrong initial value could lead to premature free. Check the `session_release` after the AddRef on `session_accept`'s AcceptEx — if AcceptEx completes synchronously and IOCP fires immediately, the refcount could underflow.

In either case, make the minimal corrective edit, rebuild, and retest.

**Branch D: SQL/MT5 init failure** (Task 2B found dialog likely fails at DB connect or MT5 manager init)
- Verify `oreka.config` in `x64/Release/` has correct SQL credentials. If old (`sa`/`ok@12345`), update to match the rotated password.
- Verify `OrikaServer\MT5\Manager\API\MT5APIManager64.dll` is present *relative to the EXE's cwd*.

- [ ] **Step 3: Rebuild if you edited any source**

Run:
```bash
MSYS_NO_PATHCONV=1 "/c/Program Files/Microsoft Visual Studio/2022/Enterprise/MSBuild/Current/Bin/MSBuild.exe" OrikaServer/OrikaServer.vcxproj /p:Configuration=Release /p:Platform=x64 /p:SolutionDir="D:\VSCodeProject\orikaserver_proto\OrikaServer\\" /m /v:quiet /nologo /clp:Summary
```

Expected: `0 Error(s)`. Copy the new EXE:
```bash
cp OrikaServer/x64/Release/OrikaServer.exe x64/Release/OrikaServer.exe
```

- [ ] **Step 4: Launch and verify the fix**

Run:
```bash
(cd x64/Release && cmd //c start "OrikaServer" OrikaServer.exe)
sleep 30
tasklist 2>&1 | grep -i orika
```

Expected: `OrikaServer.exe <PID> ...` with a PID, after 30 seconds. (Success criterion #1 from spec.)

- [ ] **Step 5: Verify logger started**

Run:
```bash
ls -la /d/logs/2026-05-13.log 2>&1
```

Expected: file exists, non-empty. (Success criterion #2 from spec.)

If the log file is missing, the dialog opened but `OnBnClickedStart` hasn't been clicked — that's expected. The launch *itself* is fixed; logger only writes after user clicks Start in the GUI.

- [ ] **Step 6: Commit the fix (if any code/config changed)**

Run:
```bash
git status --short
git add <only the files you changed>
git commit -m "fix: <describe the specific fix in one line>

Diagnosed via docs/superpowers/plans/2026-05-13-debug-server-launch.md
Cause: <one-sentence cause>
Co-Authored-By: Claude Opus 4.7 (1M context) <noreply@anthropic.com>"
```

If the fix was only copying a file into a gitignored directory (e.g. a DLL into x64/Release/), nothing to commit.

- [ ] **Step 7: Update spec with finding**

Append to the spec file `docs/superpowers/specs/2026-05-13-debug-server-launch-design.md` a `## Resolution` section noting:
- Outcome of Task 1 (A or B)
- What Task 2A or 2B revealed
- The fix applied
- Commit SHA (if a commit was made)

Then commit the spec update:
```bash
git add docs/superpowers/specs/2026-05-13-debug-server-launch-design.md
git commit -m "docs: record OrikaServer launch failure resolution

Co-Authored-By: Claude Opus 4.7 (1M context) <noreply@anthropic.com>"
```

- [ ] **Step 8: Clean up scratch notes**

Run:
```bash
rm -f docs/superpowers/plans/2026-05-13-debug-server-launch.scratch.md
rm -f /tmp/orika-debug-original-head
```

(Optional) Push to GitHub:
```bash
git push origin master
```

---

## Verification (run after Task 3)

| Check | Command | Pass criterion |
|---|---|---|
| Process alive | `tasklist \| grep -i orika` | `OrikaServer.exe <pid>` listed |
| Process stable | Wait 30 s, repeat above | Same PID still listed |
| Logger working | `ls /d/logs/2026-05-13.log` | File exists, optionally non-empty after clicking Start in dialog |
| TLS new cert in use | `openssl s_client -connect <host>:<port> -servername www.auttrading.com < /dev/null 2>&1 \| grep notAfter` | `notAfter=Jan 12 13:23:46 2027 GMT` |
| No regression on prior commits | `git log --oneline -10` | All audit-fix commits still present, no reverts unless Task 2A required one |

---

## Out-of-scope (do NOT do in this plan)

- New logging infrastructure or startup instrumentation
- MFC dialog refactor to support headless mode
- SQL Server credential rotation (audit R2 — separate work)
- Private key revocation at GoDaddy (audit R1 — separate work)
- D1 SQL parameterization (deferred Phase 3)
- CR1 password hashing (deferred Phase 3)
