# MessageReceived `strtype` Handler Extraction Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Reduce `MessageReceived` in `OrikaServer/server/Server.cpp` from ~5,880 lines to a thin dispatcher by extracting every top-level `strtype` branch body into its own `static` handler function in the same translation unit. No behavior changes.

**Architecture:** A file-local `MsgContext` struct binds references to every shared local that branch bodies use. A single `MsgContext ctx{ ... };` is constructed once at the top of the dispatcher in `MessageReceived`. Each top-level `if`/`else if (strtype == ...)` branch body is moved into a `static void Handle_<NAME>(MsgContext& ctx)` function defined below `MessageReceived`. The dispatcher line itself (the condition and combined `&&` guards) is preserved verbatim; the body is replaced with a one-line handler call.

**Tech Stack:** C++ (MSVC), MFC (`CString`), RapidJSON, MSBuild on x64. No new dependencies introduced.

---

## Spec reference

Design doc: `docs/superpowers/specs/2026-05-14-messagereceived-strtype-extraction-design.md` (commit `547d60c`).

## File Structure

Only one source file is modified: **`OrikaServer/server/Server.cpp`**.

Additions to that file:
- `MsgContext` struct definition immediately above `MessageReceived` (around line 305).
- Forward declarations for ~103 static handler functions, added incrementally as each handler is extracted, grouped just below the `MsgContext` definition.
- ~103 handler function definitions below `MessageReceived` (between the closing `}` of `MessageReceived` near line 6184 and the `void sendDataToAllClient(...)` definition near line 6189), added incrementally one chunk per task.

No new files. No header changes. No edits to `OrikaServer.vcxproj` or `OrikaServer.vcxproj.filters`.

## Build & verification command

Build the repo-root solution after every task. Per project convention (see memory `build_output_location.md`), output must land at repo-root `x64\<Config>\`, and the build must come from the repo-root `.sln`:

```powershell
msbuild OrikaServer.sln /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:m
```

Repeat for `/p:Configuration=Release`. Expected: build succeeds, no new warnings. The "no new warnings" check is by inspection — compare against a pre-refactor baseline build the engineer runs once before starting Task 1.

There is no automated test suite covering `MessageReceived`. Verification per task is:
1. Build succeeds in both Debug|x64 and Release|x64.
2. The diff for that task is a verbatim move of branch bodies with the documented `ctx.` rewrites — reviewed by eye.
3. `git diff --stat OrikaServer/server/Server.cpp` after each task shows the expected shape (lines added below `MessageReceived` ≈ lines removed from inside it).

## The mechanical recipe (referenced by every extraction task)

For each top-level strtype branch being extracted, apply this recipe:

1. **Locate the branch.** In `OrikaServer/server/Server.cpp`, find the dispatcher line — e.g., `else if (strtype == "FETCH_CLIENTDATA")` at the line given in the task. The branch body is everything between the `{` after the condition and its matching `}`.

2. **Add a forward declaration.** Just above `MessageReceived`, in the forward-decl block established by Task 1, add:
   ```cpp
   static void Handle_<NAME>(MsgContext& ctx);
   ```
   where `<NAME>` is the strtype literal verbatim (e.g., `Handle_FETCH_CLIENTDATA`). For OR branches, name the handler after the first strtype in the OR (e.g., `(strtype == "COLUMN_TEMPLATE_SAVE" || strtype == "COLUMN_TEMPLATE_UPDATE")` → `Handle_COLUMN_TEMPLATE_SAVE`). For `strtype.Find(L"X") >= 0` branches, name after `X` (e.g., `Handle_META_DATA`).

3. **Define the handler below `MessageReceived`.** Above `void sendDataToAllClient(CString msg)` at line ~6189, append:
   ```cpp
   static void Handle_<NAME>(MsgContext& ctx)
   {
       <moved body, with ctx. rewrites applied>
   }
   ```
   The handler order is informational — keep them in the same order as their dispatcher lines so future readers can find them.

4. **Rewrite identifiers in the moved body.** Inside the handler body only, replace every reference to a shared local with the matching `MsgContext` field. Exact mapping:
   - `psession` → `ctx.psession`
   - `client` → `ctx.client`
   - `d` (the `rapidjson::Document`) → `ctx.d`
   - `strforjson` → `ctx.strforjson`
   - `str` (the outer `std::string`) → `ctx.str`
   - `message` → `ctx.message`
   - `strtype` → `ctx.strtype`
   - `strKey` → `ctx.strKey`
   - `strloginuser` → `ctx.strloginuser`
   - `m_ActiveClient` → `ctx.m_ActiveClient`
   - `checkLoginValidate` → `ctx.checkLoginValidate`
   - `m_clientType` → `ctx.m_clientType`
   - `key` → `ctx.key`
   - `login` → `ctx.login`
   - `HDSLno` → `ctx.HDSLno`
   - `_action` → `ctx._action`

   **Do not rewrite** identifiers that are: (a) branch-local declarations (e.g., a `CString m_theme = L"";` declared inside the branch), (b) field accesses like `psession->key` (the `psession` part still gets rewritten to `ctx.psession`, but the `->key` member access does not), (c) `CStaticClass::...` qualifications, (d) RapidJSON helpers like `Value`, `Document`, `MemberBegin()`, etc., (e) any STL or Win32 API name.

5. **Replace the dispatcher branch body with a single-line call.** In `MessageReceived`, replace the entire `{ ... }` block of the branch with `Handle_<NAME>(ctx);` on its own line, keeping the original `if`/`else if` condition untouched. Example:
   ```cpp
   // Before
   else if (strtype == L"FETCH_CLIENTDATA")
   {
       /* 17 lines of body */
   }

   // After
   else if (strtype == L"FETCH_CLIENTDATA")
       Handle_FETCH_CLIENTDATA(ctx);
   ```

6. **Audit each `return;` in the moved body.** Originally, `return;` inside a branch short-circuited the entire `MessageReceived` call. After extraction, `return;` only exits the handler — control flows back into the dispatcher and continues evaluating subsequent `else if` branches.
   - For all branches that are part of the long `else if` chain (line 972 onward), a returned-early branch is followed by no further `else if` execution at runtime anyway (because `else if` short-circuits once a prior arm matched). So a `return;` inside such a body is safe — leave it.
   - For the three **standalone** `if` branches at lines 399, 580, 776 (`login`, `loginbytoken`, `LOGIN_BY_KEY`), a `return;` inside the original body would also skip the next two standalone `if`s and the entire `else if` chain. After extraction, control returns to the dispatcher and the next `if`/`else if` runs. **Inspect every `return;` inside these three branches and decide:**
     - If the `return;` was intended to abort processing this message entirely (e.g., early-return on auth failure), promote the dispatcher call to: `if (Handle_<NAME>(ctx)) return;` and change the handler return type to `bool` (return `true` where the original code did `return;`, return `false` at end of body).
     - If the `return;` was intended only to skip the rest of that branch, leave it as `void`-returning.
   - Document the per-branch decision in the commit message for the task that extracts each standalone branch.

7. **Build, commit.** Build both Debug|x64 and Release|x64 using the command above. On success, commit the task's changes with a message like:
   ```
   refactor(server): extract Handle_<NAME> from MessageReceived
   ```

This recipe is referenced by every extraction task below.

---

### Task 1: Add `MsgContext` struct and forward-declaration block

**Files:**
- Modify: `OrikaServer/server/Server.cpp` immediately above the existing line `void MessageReceived(SSL_session* psession, ...)` at line 306.

- [ ] **Step 1: Run a baseline build before any changes**

Run:
```powershell
msbuild OrikaServer.sln /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:m
msbuild OrikaServer.sln /p:Configuration=Release /p:Platform=x64 /m /nologo /v:m
```

Expected: both builds succeed. Capture the warning count for each configuration so subsequent tasks can verify no new warnings were introduced.

- [ ] **Step 2: Insert the `MsgContext` struct definition**

In `OrikaServer/server/Server.cpp`, immediately above the line `void MessageReceived(SSL_session* psession, char* c_message, int datasize,CString  key, CString login, CString HDSLno)` (line 306), insert:

```cpp
struct MsgContext {
    SSL_session*           psession;
    SSL_session*           client;
    rapidjson::Document&   d;
    const char*            strforjson;
    std::string&           str;
    std::string&           message;
    CString&               strtype;
    CString&               strKey;
    CString&               strloginuser;
    int&                   m_ActiveClient;
    int&                   checkLoginValidate;
    int&                   m_clientType;
    CString                key;
    CString                login;
    CString                HDSLno;
    CString                _action;
};

// Forward declarations for MessageReceived strtype handlers.
// Each handler is defined below MessageReceived in this same file.
// Added incrementally as branches are extracted.
// --- BEGIN handler forward decls ---
// --- END handler forward decls ---

```

(The two marker comments establish an insertion zone for subsequent tasks. Each later task inserts its forward declarations between those marker lines.)

- [ ] **Step 3: Build both configurations**

Run:
```powershell
msbuild OrikaServer.sln /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:m
msbuild OrikaServer.sln /p:Configuration=Release /p:Platform=x64 /m /nologo /v:m
```

Expected: both builds succeed. Warning count matches the baseline from Step 1.

- [ ] **Step 4: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): introduce MsgContext for MessageReceived branch extraction"
```

---

### Task 2: Extract `Handle_login` (canonical extraction with full detail)

This task is fully detailed so it can serve as the worked example for every subsequent extraction task.

**Files:**
- Modify: `OrikaServer/server/Server.cpp`
  - Forward decl: between the `// --- BEGIN/END handler forward decls ---` markers
  - Dispatcher: replace the body of the `if (strtype == "login")` branch at line 399
  - Handler definition: append above `void sendDataToAllClient(CString msg)` at line ~6189
- Also modify: `MessageReceived` to construct `MsgContext ctx{...}` for the first time

- [ ] **Step 1: Add the forward declaration**

Between the marker comments inserted in Task 1, add:

```cpp
static void Handle_login(MsgContext& ctx);
```

- [ ] **Step 2: Construct `ctx` in `MessageReceived` immediately before the dispatcher**

In `MessageReceived`, find the line `if (strtype == "login")` at line 399. Immediately before that line (and still inside the `if (!d.Parse<0>(strforjson).HasParseError())` block and the `if (d.HasMember("type") == true)` block, after `_action` and `strtype` are assigned), insert:

```cpp
MsgContext ctx{
    psession,
    client,
    d,
    strforjson,
    str,
    message,
    strtype,
    strKey,
    strloginuser,
    m_ActiveClient,
    checkLoginValidate,
    m_clientType,
    key,
    login,
    HDSLno,
    _action
};
```

- [ ] **Step 3: Move the `login` branch body into `Handle_login`**

In `OrikaServer/server/Server.cpp`, locate the closing `}` of `MessageReceived` near line 6184. Immediately above the line `void sendDataToAllClient(CString  msg)` at line 6189, append the new handler:

```cpp
static void Handle_login(MsgContext& ctx)
{
    // Body moved verbatim from MessageReceived's `if (strtype == "login")`
    // branch (originally lines 400-578) with the following identifier rewrites:
    //   psession           -> ctx.psession
    //   client             -> ctx.client
    //   d                  -> ctx.d
    //   strforjson         -> ctx.strforjson
    //   strKey             -> ctx.strKey
    //   strloginuser       -> ctx.strloginuser
    //   m_ActiveClient     -> ctx.m_ActiveClient
    //   login              -> ctx.login
    //   _action            -> ctx._action
    //   (other ctx fields if referenced; do NOT rewrite branch-local vars)
    //
    // The branch body declares its own locals (strLogin, strslno, strTabData,
    // m_st, strKeyCraete, strTime, strtoken, etc.) -- those are NOT rewritten.
    // CStaticClass::... qualifiers, SendDataToClient(...) calls, and other
    // free functions are NOT rewritten.

    CUserLogin m_CUserLogin;
    int returnVal = 1;
    returnVal = m_CUserLogin.userValideate(ctx.strforjson, L"");
    if (returnVal == 0 || returnVal == 4)
    {
        CStaticClass::m_mutex_ClientList.Lock();

        const Value& Keylogin = ctx.d["login"];
        CString strLogin = L"";
        strLogin = Keylogin.GetString();

        CString strslno = L"";
        if (ctx.d.HasMember("serialNo"))
        {
            const Value& slno = ctx.d["serialNo"];
            if (slno.IsString())
            {
                strslno = slno.GetString();
            }
        }
        // ... continue moving lines 423-561 verbatim from the original branch,
        //     applying the identifier-rewrite table above to every identifier
        //     that matches a MsgContext field name.
        //
        // The last meaningful line of the original body (line 560) is:
        //     string strforsend = CT2A(m_rval.GetString());
        //     //SendDataToClient(client, strforsend, strKey, m_ActiveClient);
        // -> keep the comment-out as-is; this preserves existing behavior.
    }
    else if (returnVal == 1)
    {
        string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"invalid credentials\"}";
        SendDataToClient_UnRegistered(ctx.psession, strforsend);
    }
    else if (returnVal == 2)
    {
        string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"invalid SerialNo\"}";
        SendDataToClient_UnRegistered(ctx.psession, strforsend);
    }
    else if (returnVal == 3)
    {
        string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"Invalid ApiLogin\"}";
        SendDataToClient_UnRegistered(ctx.psession, strforsend);
    }
}
```

(The body above is shown to illustrate the recipe; the engineer must move the actual lines 400-577 verbatim and apply the identifier rewrites. Do not retype from memory.)

- [ ] **Step 4: Replace the dispatcher branch body with a one-line call**

In `MessageReceived`, after the `MsgContext ctx{...}` from Step 2, the original branch:

```cpp
if (strtype == "login")
{
    /* ~178 lines */
}
```

becomes:

```cpp
if (strtype == "login")
    Handle_login(ctx);
```

- [ ] **Step 5: Audit `return;` statements inside the moved body**

`Handle_login`'s body contains two early `return;` statements (originally lines 491 and 507). Inspect each:
- Line 491 (original): `return;` inside an `if (!SUCCEEDED(hr))` after an `m_mutex_ClientList.Unlock();`. Originally aborted `MessageReceived`. After extraction it now only exits `Handle_login`, control returns to the dispatcher. Since `login` is a **standalone** `if` (not part of the `else if` chain), the next standalone `if (strtype == "loginbytoken")` (line 580) would now run. **Decision required.**
- Line 507 (original): same situation — `return;` after `m_mutex_ClientList.Unlock();` on a different failure.

For both: the original semantics were "abort message processing entirely on this failure path". To preserve that, change `Handle_login`'s return type from `void` to `bool`, return `true` from the failure paths (where `return;` was), return `false` at the end of the body, and rewrite the dispatcher line:

```cpp
if (strtype == "login")
{
    if (Handle_login(ctx)) return;
}
```

Update the forward declaration to `static bool Handle_login(MsgContext& ctx);` to match.

- [ ] **Step 6: Build both configurations**

Run:
```powershell
msbuild OrikaServer.sln /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:m
msbuild OrikaServer.sln /p:Configuration=Release /p:Platform=x64 /m /nologo /v:m
```

Expected: both builds succeed, warning count unchanged from baseline.

- [ ] **Step 7: Diff sanity check**

Run:
```powershell
git diff --stat OrikaServer/server/Server.cpp
```

Expected: approximately equal lines added and removed (within ±10 for the struct, ctx construction, forward decl, and the `bool` return-type tweak). The handler body's line count should mirror the removed branch body's line count.

- [ ] **Step 8: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract Handle_login from MessageReceived

login is a standalone if (not else-if). Two original return; statements
inside the body intended to abort the whole message; preserved by returning
bool and using if (Handle_login(ctx)) return; at the call site."
```

---

### Task 3: Extract `Handle_loginbytoken` and `Handle_LOGIN_BY_KEY`

Both are **standalone** `if` branches like `login` (not part of the `else if` chain). Apply the **Task 2 recipe** to each, paying special attention to recipe Step 6 (the `return;` audit). Each branch may need to become `bool`-returning if any internal `return;` was intended to abort the entire message.

**Branches:**

| Line | Dispatcher | Handler | Body lines |
| ---- | ---------- | ------- | ---------- |
| 580 | `if (strtype == "loginbytoken")` | `Handle_loginbytoken` | 581–774 (~194 lines) |
| 776 | `if (strtype == "LOGIN_BY_KEY")` | `Handle_LOGIN_BY_KEY` | 777–970 (~194 lines) |

**Files:**
- Modify: `OrikaServer/server/Server.cpp` — forward decls, dispatcher bodies, handler definitions.

- [ ] **Step 1: Add forward declarations**

Between the `// --- BEGIN/END handler forward decls ---` markers, add (after `Handle_login`):

```cpp
static bool Handle_loginbytoken(MsgContext& ctx);   // bool if any return; aborts message; void otherwise
static bool Handle_LOGIN_BY_KEY(MsgContext& ctx);   // bool if any return; aborts message; void otherwise
```

Adjust to `void` if the audit in Step 4 below shows no aborting `return;` in a given body.

- [ ] **Step 2: Move the `loginbytoken` branch body into `Handle_loginbytoken`**

Apply Task 2 recipe steps 3 and 4 to the `loginbytoken` branch at line 580. Move lines 581–774 (the body between the opening `{` and matching `}`) into `Handle_loginbytoken(MsgContext& ctx)` defined above `sendDataToAllClient`, immediately after `Handle_login`. Apply the identifier rewrite table from the master recipe (above Task 1).

In `MessageReceived`, replace the original branch body with `Handle_loginbytoken(ctx);` (or `if (Handle_loginbytoken(ctx)) return;` if the audit determines abort semantics).

- [ ] **Step 3: Move the `LOGIN_BY_KEY` branch body into `Handle_LOGIN_BY_KEY`**

Apply the same procedure to the `LOGIN_BY_KEY` branch at line 776. Move lines 777–970 into `Handle_LOGIN_BY_KEY(MsgContext& ctx)`, appended after `Handle_loginbytoken`.

In `MessageReceived`, replace the original branch body with `Handle_LOGIN_BY_KEY(ctx);` (or the `if (...) return;` form if abort semantics are needed).

- [ ] **Step 4: Audit `return;` in each body**

For each of the two handlers, walk every `return;` and determine: was it meant to abort the whole `MessageReceived`, or just the branch? Standalone `if`s require the abort form (`bool` + `if (Handle_X(ctx)) return;`) wherever any `return;` exists. Document each decision in the commit message.

- [ ] **Step 5: Build both configurations**

Run:
```powershell
msbuild OrikaServer.sln /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:m
msbuild OrikaServer.sln /p:Configuration=Release /p:Platform=x64 /m /nologo /v:m
```

Expected: both builds succeed, warning count unchanged.

- [ ] **Step 6: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract Handle_loginbytoken and Handle_LOGIN_BY_KEY

Both are standalone ifs; return; semantics preserved via bool return
and if (Handle_X(ctx)) return; at call sites where needed."
```

---

### Task 4: Extract chunk A (lines 972–1670, 18 branches)

All branches in this chunk are part of the long `else if` chain — `return;` inside a body is safe to leave as-is (the `else if` chain wouldn't have evaluated subsequent arms anyway). All handlers can be `void`-returning.

Apply the **master recipe** (above Task 1, steps 1–7) to each branch listed below in order. For each branch:
1. Add forward declaration in the marker block.
2. Move the body verbatim into a new `static void Handle_<NAME>(MsgContext& ctx)` definition appended after the previous task's handlers.
3. Apply the identifier rewrite table.
4. Replace the dispatcher branch body with `Handle_<NAME>(ctx);` on its own line, keeping the original `if`/`else if` condition (including any `&& checkLoginValidate == 1`) untouched.

**Branches in chunk A:**

| Line | Dispatcher (abbreviated) | Handler |
| ---- | ------------------------ | ------- |
| 972  | `else if (strtype == L"FETCH_CLIENT_POSITIONS_TIMERWISE_START")` | `Handle_FETCH_CLIENT_POSITIONS_TIMERWISE_START` |
| 1009 | `else if (strtype == L"FETCH_CLIENT_POSITIONS_TIMERWISE_STOP")` | `Handle_FETCH_CLIENT_POSITIONS_TIMERWISE_STOP` |
| 1021 | `else if (strtype == L"FETCH_SUBBROKERDATA")` | `Handle_FETCH_SUBBROKERDATA` |
| 1036 | `else if (strtype == L"FETCH_BROKERDATA")` | `Handle_FETCH_BROKERDATA` |
| 1051 | `else if (strtype == L"FETCH_CLIENTDATA")` | `Handle_FETCH_CLIENTDATA` |
| 1068 | `else if (strtype == L"FETCH_DASHBOARD_DATA")` | `Handle_FETCH_DASHBOARD_DATA` |
| 1092 | `else if ((strtype == L"SUBSCRIBE_POSITION_FOR_TRANSFER") && checkLoginValidate == 1 && m_clientType==1)` | `Handle_SUBSCRIBE_POSITION_FOR_TRANSFER` |
| 1102 | `else if ((strtype == L"FETCH_DEALING_DATA_INTERVAL") && checkLoginValidate == 1 && m_clientType == 1)` | `Handle_FETCH_DEALING_DATA_INTERVAL` |
| 1142 | `else if ((strtype == L"FETCH_CLIENT_POSITIONS" \|\| strtype == L"FETCH_CLIENT_POSITIONS_PRENETQTY" \|\| strtype == L"FETCH_ORDER_DATA" \|\| strtype == L"FETCH_DEALING_DATA" \|\| strtype == L"FETCH_LOGIN_DEVICE_LOG" \|\| strtype == L"EXISTING_COMMENT_CHANGE_DATA") && checkLoginValidate == 1)` | `Handle_FETCH_CLIENT_POSITIONS` (note: this is the OR-branch handler; the inner branch at line 3836 will be `Handle_FETCH_CLIENT_POSITIONS_v2` to avoid the name collision — see Task 7 note) |
| 1350 | `else if (strtype == L"UNSUBSCRIBE" && checkLoginValidate == 1)` | `Handle_UNSUBSCRIBE` |
| 1419 | `else if (strtype == "CLOSING_UPDATE_UPLOAD" && checkLoginValidate == 1)` | `Handle_CLOSING_UPDATE_UPLOAD` |
| 1444 | `else if (strtype == "SYMBOL_FILE_TRANSFER" && checkLoginValidate == 1)` | `Handle_SYMBOL_FILE_TRANSFER` |
| 1462 | `else if (strtype == "IGNORE_CLOSINGANDSAVE" && checkLoginValidate == 1)` | `Handle_IGNORE_CLOSINGANDSAVE` |
| 1556 | `else if (strtype == "UPDATE_CLOSINGPRICE" && checkLoginValidate == 1)` | `Handle_UPDATE_CLOSINGPRICE` |
| 1629 | `else if (strtype == "FETCH_HIGH_LOW_MISMATCH_DATA" && checkLoginValidate == 1)` | `Handle_FETCH_HIGH_LOW_MISMATCH_DATA` |
| 1644 | `else if (strtype == "GET_DASHBOARD" && checkLoginValidate == 1)` | `Handle_GET_DASHBOARD` |
| 1652 | `else if (strtype == "GET_CANVAS" && checkLoginValidate == 1)` | `Handle_GET_CANVAS` |
| 1670 | `else if (strtype == "UPDATE_GATEWAY_POSITIONDEVIDERATIO" && checkLoginValidate == 1)` | `Handle_UPDATE_GATEWAY_POSITIONDEVIDERATIO` |

- [ ] **Step 1: Extract all 18 branches**

For each row above, in the order listed, apply recipe steps 2 (forward decl), 3 (handler definition), 4 (identifier rewrite), 5 (dispatcher replacement).

- [ ] **Step 2: Build both configurations**

Run:
```powershell
msbuild OrikaServer.sln /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:m
msbuild OrikaServer.sln /p:Configuration=Release /p:Platform=x64 /m /nologo /v:m
```

Expected: both builds succeed, warning count unchanged.

- [ ] **Step 3: Diff sanity check**

Run:
```powershell
git diff --stat OrikaServer/server/Server.cpp
git diff OrikaServer/server/Server.cpp | head -100
```

Expected: lines added below `MessageReceived` ≈ lines removed from inside it (within ±50 for the 18 dispatcher one-liners replacing 18 `{ ... }` blocks).

- [ ] **Step 4: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract 18 strtype handlers (chunk A, lines 972-1670)"
```

---

### Task 5: Extract chunk B (lines 1859–2368, 11 branches)

All branches are part of the `else if` chain — `void`-returning is safe. Apply the master recipe to each.

**Branches in chunk B:**

| Line | Dispatcher (abbreviated) | Handler |
| ---- | ------------------------ | ------- |
| 1859 | `else if (strtype == "FETCH_GATEWAY_POSITIONDEVIDERATIO" && checkLoginValidate == 1)` | `Handle_FETCH_GATEWAY_POSITIONDEVIDERATIO` |
| 1867 | `else if (strtype == "SAVE_DASHBOARD" && checkLoginValidate == 1)` | `Handle_SAVE_DASHBOARD` |
| 1930 | `else if (strtype == "FETCH_SYMBOLGROUPINFO" && checkLoginValidate == 1)` | `Handle_FETCH_SYMBOLGROUPINFO` |
| 1966 | `else if (strtype == "FETCH_ALL_SYMBOLGROUPINFO" && checkLoginValidate == 1)` | `Handle_FETCH_ALL_SYMBOLGROUPINFO` |
| 2002 | `else if (strtype == "UPDATE_SYMBOLMARGIN" && checkLoginValidate == 1)` | `Handle_UPDATE_SYMBOLMARGIN` |
| 2055 | `else if (strtype == "FETCH_CLIENTCREDIT" && checkLoginValidate == 1)` | `Handle_FETCH_CLIENTCREDIT` |
| 2092 | `else if (strtype == "UPDATE_CLIENTCREDIT" && checkLoginValidate == 1)` | `Handle_UPDATE_CLIENTCREDIT` |
| 2159 | `else if (strtype == "FETCH_REJECTLIMIT_SETTING" && checkLoginValidate == 1)` | `Handle_FETCH_REJECTLIMIT_SETTING` |
| 2221 | `else if (strtype == "UPDATE_REJECTLIMIT_SETTING" && checkLoginValidate == 1)` | `Handle_UPDATE_REJECTLIMIT_SETTING` |
| 2350 | `else if (strtype == "SET_SELECTED_THEME" && checkLoginValidate == 1)` | `Handle_SET_SELECTED_THEME` |
| 2368 | `else if (strtype == "FETCH_SELECTED_THEME" && checkLoginValidate == 1)` | `Handle_FETCH_SELECTED_THEME` |

- [ ] **Step 1: Extract all 11 branches**

For each row above, apply recipe steps 2–5.

- [ ] **Step 2: Build both configurations and verify**

Run the Debug|x64 and Release|x64 builds. Expected: both succeed, warning count unchanged.

- [ ] **Step 3: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract 11 strtype handlers (chunk B, lines 1859-2368)"
```

---

### Task 6: Extract chunk C (lines 2394–2974, 13 branches)

All branches are part of the `else if` chain — `void`-returning is safe.

**Branches in chunk C:**

| Line | Dispatcher (abbreviated) | Handler |
| ---- | ------------------------ | ------- |
| 2394 | `else if (strtype == "SAVE_PAGE" && checkLoginValidate == 1)` | `Handle_SAVE_PAGE` |
| 2503 | `else if (strtype == "UPDATE_PAGE" && checkLoginValidate == 1)` | `Handle_UPDATE_PAGE` |
| 2614 | `else if (strtype == "DELETE_PAGE" && checkLoginValidate == 1)` | `Handle_DELETE_PAGE` |
| 2673 | `else if (strtype == "FETCH_PAGE_LIST" && checkLoginValidate == 1)` | `Handle_FETCH_PAGE_LIST` |
| 2689 | `else if (strtype == "UPDATE_PAGE_SEQUENCE" && checkLoginValidate == 1)` | `Handle_UPDATE_PAGE_SEQUENCE` |
| 2748 | `else if (strtype == "FETCH_COLOR_THEMES" && checkLoginValidate == 1)` | `Handle_FETCH_COLOR_THEMES` |
| 2765 | `else if (strtype == "FETCH_COLOR_THEME" && checkLoginValidate == 1)` | `Handle_FETCH_COLOR_THEME` |
| 2781 | `else if (strtype == "SAVE_COLOR_THEME" && checkLoginValidate == 1)` | `Handle_SAVE_COLOR_THEME` |
| 2857 | `else if (strtype == "SAVE_COLOR_TEMPLATE" && checkLoginValidate == 1)` | `Handle_SAVE_COLOR_TEMPLATE` |
| 2900 | `else if (strtype == "DELETE_TEMPLATE" && checkLoginValidate == 1)` | `Handle_DELETE_TEMPLATE` |
| 2928 | `else if (strtype == "DELETE_COLOR_THEME" && checkLoginValidate == 1)` | `Handle_DELETE_COLOR_THEME` |
| 2956 | `else if (strtype == "FETCH_COLOR_TEMPLATES" && checkLoginValidate == 1)` | `Handle_FETCH_COLOR_TEMPLATES` |
| 2974 | `else if (strtype == "FETCH_PAGE_DETAILS" && checkLoginValidate == 1)` | `Handle_FETCH_PAGE_DETAILS` |

- [ ] **Step 1: Extract all 13 branches**

Apply recipe steps 2–5 to each.

- [ ] **Step 2: Build and verify**

Run both x64 builds. Expected: both succeed, warning count unchanged.

- [ ] **Step 3: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract 13 strtype handlers (chunk C, lines 2394-2974)"
```

---

### Task 7: Extract chunk D (lines 3035–3725, 13 branches)

All `else if` chain branches. **Special case:** the branch at line 3566 is an OR branch (`COLUMN_TEMPLATE_SAVE || COLUMN_TEMPLATE_UPDATE`). The nested `if (strtype == "COLUMN_TEMPLATE_SAVE")` at line 3682 and `if (strtype == "COLUMN_TEMPLATE_UPDATE")` at line 3686 are **inside** that branch's body and **stay inside `Handle_COLUMN_TEMPLATE_SAVE`** unchanged (with `strtype` rewritten to `ctx.strtype`).

**Branches in chunk D:**

| Line | Dispatcher (abbreviated) | Handler |
| ---- | ------------------------ | ------- |
| 3035 | `else if (strtype == "UPDATE_DATA_BY_TABLENAME" && checkLoginValidate == 1)` | `Handle_UPDATE_DATA_BY_TABLENAME` |
| 3242 | `else if (strtype == "SAVE_CANVAS" && checkLoginValidate == 1)` | `Handle_SAVE_CANVAS` |
| 3279 | `else if (strtype == "GET_ALL_ALERT" && checkLoginValidate == 1)` | `Handle_GET_ALL_ALERT` |
| 3294 | `else if (strtype == "SAVE_UPDATE_ALERT" && checkLoginValidate == 1)` | `Handle_SAVE_UPDATE_ALERT` |
| 3469 | `else if (strtype == "DELETE_ALERT" && checkLoginValidate == 1)` | `Handle_DELETE_ALERT` |
| 3520 | `else if (strtype == "GET_SYMBOLMAPPING" && checkLoginValidate == 1)` | `Handle_GET_SYMBOLMAPPING` |
| 3527 | `else if (strtype == "FETCH_CLOSINGPRICE" && checkLoginValidate == 1)` | `Handle_FETCH_CLOSINGPRICE` |
| 3534 | `else if (strtype == "FETCH_CLIENT_PREPOSITION" && checkLoginValidate == 1)` | `Handle_FETCH_CLIENT_PREPOSITION` |
| 3560 | `else if (strtype == "FETCH_CLIENT_MASTER_DATA" && checkLoginValidate == 1)` | `Handle_FETCH_CLIENT_MASTER_DATA` |
| 3566 | `else if ((strtype == "COLUMN_TEMPLATE_SAVE" \|\| strtype == "COLUMN_TEMPLATE_UPDATE") && checkLoginValidate == 1)` | `Handle_COLUMN_TEMPLATE_SAVE` (handles both; inner nested ifs stay inside) |
| 3693 | `else if (strtype == "COLUMN_TEMPLATE_DELETE" && checkLoginValidate == 1)` | `Handle_COLUMN_TEMPLATE_DELETE` |
| 3710 | `else if (strtype == "FETCH_COLUMN_TEMPLATE" && checkLoginValidate == 1)` | `Handle_FETCH_COLUMN_TEMPLATE` |
| 3725 | `else if (strtype == "FETCH_CLIENT_WISE_PREVIOUS_NET_VOLUME" && checkLoginValidate == 1)` | `Handle_FETCH_CLIENT_WISE_PREVIOUS_NET_VOLUME` |

- [ ] **Step 1: Extract all 13 branches**

Apply recipe steps 2–5 to each. For line 3566, copy the entire body lines 3567–3692 verbatim (the body ends just before `else if (strtype == "COLUMN_TEMPLATE_DELETE"...)` at line 3693). Inside that body, the nested `if (strtype == "COLUMN_TEMPLATE_SAVE")` and `if (strtype == "COLUMN_TEMPLATE_UPDATE")` checks remain in place — just rewrite `strtype` to `ctx.strtype` in those nested checks.

- [ ] **Step 2: Build and verify**

Run both x64 builds. Expected: both succeed, warning count unchanged.

- [ ] **Step 3: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract 13 strtype handlers (chunk D, lines 3035-3725)

Includes the COLUMN_TEMPLATE_SAVE||UPDATE OR-branch (line 3566); the
inner if (strtype == \"COLUMN_TEMPLATE_SAVE\") and if (strtype ==
\"COLUMN_TEMPLATE_UPDATE\") sub-dispatch at lines 3682/3686 remains
inside Handle_COLUMN_TEMPLATE_SAVE."
```

---

### Task 8: Extract chunk E (lines 3743–4470, 16 branches)

All `else if` chain branches. **Naming conflict:** the branch at line 3836 is `else if (strtype == "FETCH_CLIENT_POSITIONS" && checkLoginValidate == 1)`. The OR-branch handler from chunk A was already named `Handle_FETCH_CLIENT_POSITIONS`. To avoid a duplicate name, name this one `Handle_FETCH_CLIENT_POSITIONS_v2`. Document this in the commit message — the `_v2` suffix is the **only** naming deviation from the recipe and exists solely to avoid the collision.

**Branches in chunk E:**

| Line | Dispatcher (abbreviated) | Handler |
| ---- | ------------------------ | ------- |
| 3743 | `else if (strtype == "DELETE_COMMENT_CHANGE_DATA" && checkLoginValidate == 1)` | `Handle_DELETE_COMMENT_CHANGE_DATA` |
| 3800 | `else if (strtype == "COMMENT_DEALWISE_REQUEST" && checkLoginValidate == 1)` | `Handle_COMMENT_DEALWISE_REQUEST` |
| 3836 | `else if (strtype == "FETCH_CLIENT_POSITIONS" && checkLoginValidate == 1)` | `Handle_FETCH_CLIENT_POSITIONS_v2` (renamed to avoid collision with chunk A's `Handle_FETCH_CLIENT_POSITIONS`) |
| 3857 | `else if (strtype == "FETCH_SYMBOL_MASTER_DATA" && checkLoginValidate == 1)` | `Handle_FETCH_SYMBOL_MASTER_DATA` |
| 3865 | `else if (strtype == "FETCH_USERS_DATA" && checkLoginValidate == 1)` | `Handle_FETCH_USERS_DATA` |
| 3873 | `else if (strtype == "FETCH_USER_DETAILS" && checkLoginValidate == 1)` | `Handle_FETCH_USER_DETAILS` |
| 3892 | `else if (strtype == "DELETE_USER_DETAILS" && checkLoginValidate == 1)` | `Handle_DELETE_USER_DETAILS` |
| 3942 | `else if (strtype == "SAVE_USER_DETAILS" && checkLoginValidate == 1)` | `Handle_SAVE_USER_DETAILS` |
| 4236 | `else if (strtype == "FETCH_ALL_GROUP_DATA" && checkLoginValidate == 1)` | `Handle_FETCH_ALL_GROUP_DATA` |
| 4258 | `else if (strtype == "FETCH_USER_DATA" && checkLoginValidate == 1)` | `Handle_FETCH_USER_DATA` |
| 4270 | `else if (strtype == "SAVE_USER_DATA" && checkLoginValidate == 1)` | `Handle_SAVE_USER_DATA` |
| 4276 | `else if (strtype == "START_GATEWAY_POSITIONDEVIDERATIO" && checkLoginValidate == 1)` | `Handle_START_GATEWAY_POSITIONDEVIDERATIO` |
| 4290 | `else if (strtype == "STOP_GATEWAY_POSITIONDEVIDERATIO" && checkLoginValidate == 1)` | `Handle_STOP_GATEWAY_POSITIONDEVIDERATIO` |
| 4299 | `else if (strtype == "ACTIVE_COLUMNS_CHANGED" && checkLoginValidate == 1)` | `Handle_ACTIVE_COLUMNS_CHANGED` |
| 4399 | `else if (strtype == "ACTIVE_COLUMNS_DELETE" && checkLoginValidate == 1)` | `Handle_ACTIVE_COLUMNS_DELETE` |
| 4470 | `else if (strtype == "TDH_EXECUTION" && checkLoginValidate == 1)` | `Handle_TDH_EXECUTION` |

- [ ] **Step 1: Extract all 16 branches**

Apply recipe steps 2–5 to each. Note the `_v2` suffix on `Handle_FETCH_CLIENT_POSITIONS_v2` (line 3836 row).

- [ ] **Step 2: Build and verify**

Run both x64 builds. Expected: both succeed, warning count unchanged.

- [ ] **Step 3: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract 16 strtype handlers (chunk E, lines 3743-4470)

Handle_FETCH_CLIENT_POSITIONS_v2 (line 3836) uses _v2 suffix to avoid
name collision with chunk A's Handle_FETCH_CLIENT_POSITIONS (the OR
branch at line 1142). This is the only naming deviation from the recipe."
```

---

### Task 9: Extract chunk F (lines 4608–4941, 13 branches)

All `else if` chain branches. **Special cases:**
- Line 4805 is an OR branch (`COMMENT_CHANGE_SAVE_DATA || COMMENT_CHANGE_UPDATE_DATA`). The handler name uses the first OR'd strtype: `Handle_COMMENT_CHANGE_SAVE_DATA`. Any inner `if (strtype == ...)` distinguishing the two stays inside the handler body.
- Line 4891 is `strtype.Find(L"META_DATA") >= 0` — handler name uses the substring: `Handle_META_DATA`. The inner `if (strtype == "FETCH_CLIENT_META_DATA" || ...)` chain at lines 4896, 4900, 4905 stays inside `Handle_META_DATA` unchanged (with `strtype` rewritten to `ctx.strtype`).
- Lines 4929 and 4941 use `strtype.Find(L"...") >= 0` — handler names `Handle_START_HEART_BEAT` and `Handle_FETCH_TICK_DATA`.

**Branches in chunk F:**

| Line | Dispatcher (abbreviated) | Handler |
| ---- | ------------------------ | ------- |
| 4608 | `else if (strtype == "DELETE_USER_DATA" && checkLoginValidate == 1)` | `Handle_DELETE_USER_DATA` |
| 4633 | `else if (strtype == "ALERT_MESSAGE" && checkLoginValidate == 1)` | `Handle_ALERT_MESSAGE` |
| 4652 | `else if (strtype == "SET_AUTH_KEY" && checkLoginValidate == 1)` | `Handle_SET_AUTH_KEY` |
| 4683 | `else if (strtype == "LOGOUT" && checkLoginValidate == 1)` | `Handle_LOGOUT` |
| 4693 | `else if (strtype == "LOGOUT_USER" && checkLoginValidate == 1)` | `Handle_LOGOUT_USER` |
| 4702 | `else if (strtype == "SUBSCRIBE_TICKER" && checkLoginValidate == 1)` | `Handle_SUBSCRIBE_TICKER` |
| 4773 | `else if (strtype == "UNSUBSCRIBE_TICKER" && checkLoginValidate == 1)` | `Handle_UNSUBSCRIBE_TICKER` |
| 4805 | `else if ((strtype == "COMMENT_CHANGE_SAVE_DATA" \|\| strtype == "COMMENT_CHANGE_UPDATE_DATA") && checkLoginValidate == 1)` | `Handle_COMMENT_CHANGE_SAVE_DATA` |
| 4880 | `else if (strtype == "COMMENT_CHANGE_REQUEST" && checkLoginValidate == 1)` | `Handle_COMMENT_CHANGE_REQUEST` |
| 4886 | `else if (strtype == "HB_R" && checkLoginValidate == 1)` | `Handle_HB_R` |
| 4891 | `else if (strtype.Find(L"META_DATA") >= 0 && checkLoginValidate == 1)` | `Handle_META_DATA` (inner sub-dispatch at lines 4896/4900/4905 stays inside) |
| 4929 | `else if (strtype.Find(L"START_HEART_BEAT") >= 0 && checkLoginValidate == 1)` | `Handle_START_HEART_BEAT` |
| 4941 | `else if (strtype.Find(L"FETCH_TICK_DATA") >= 0 && checkLoginValidate == 1)` | `Handle_FETCH_TICK_DATA` |

- [ ] **Step 1: Extract all 13 branches**

Apply recipe steps 2–5 to each. For line 4891, the body to move is lines 4892–4921 (the closing `}` is at line 4921). The commented-out `/* else if (strtype.Find(L"DASHBOARD_META_DATA") ...) */` block at lines 4922–4928 is **not** part of any branch body — leave it in `MessageReceived` between the dispatcher line for `META_DATA` (now `Handle_META_DATA(ctx);`) and the next live dispatcher line at 4929. (Or move it to wherever feels natural; since it's a comment block, semantics are unchanged either way.)

- [ ] **Step 2: Build and verify**

Run both x64 builds. Expected: both succeed, warning count unchanged.

- [ ] **Step 3: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract 13 strtype handlers (chunk F, lines 4608-4941)

Includes the META_DATA strtype.Find branch and its inner sub-dispatch
(FETCH_CLIENT_META_DATA, FETCH_ALL_TAB_COLUMN_META_DATA, etc.) which
stays inside Handle_META_DATA. Two more strtype.Find branches
(START_HEART_BEAT, FETCH_TICK_DATA) handled the same way."
```

---

### Task 10: Extract chunk G (lines 5018–6014, 13 branches)

All `else if` chain branches.

**Branches in chunk G:**

| Line | Dispatcher (abbreviated) | Handler |
| ---- | ------------------------ | ------- |
| 5018 | `else if (strtype == "ORDER_ACTIVATE_REQUEST" && checkLoginValidate == 1)` | `Handle_ORDER_ACTIVATE_REQUEST` |
| 5089 | `else if (strtype == "ORDER_UPDATE_REQUEST" && checkLoginValidate == 1)` | `Handle_ORDER_UPDATE_REQUEST` (note: this body declares its own local `const char* strforjson = str.c_str();` at line 5092 — leave that local declaration intact; only rewrite `str` to `ctx.str`. The local then shadows `ctx.strforjson` inside the body, which is fine because the local was already shadowing the outer in the original code.) |
| 5394 | `else if (strtype == "FETCH_LOGIN_AND_SYMBOL" && checkLoginValidate == 1)` | `Handle_FETCH_LOGIN_AND_SYMBOL` |
| 5454 | `else if (strtype == "FETCH_ALL_LOGIN_AND_SYMBOL" && checkLoginValidate == 1)` | `Handle_FETCH_ALL_LOGIN_AND_SYMBOL` |
| 5513 | `else if (strtype == "FETCH_ALL_LOGIN_AND_SYMBOL_TDH" && checkLoginValidate == 1)` | `Handle_FETCH_ALL_LOGIN_AND_SYMBOL_TDH` |
| 5573 | `else if (strtype == "VERIFY_POSITION" && checkLoginValidate == 1)` | `Handle_VERIFY_POSITION` |
| 5597 | `else if (strtype == "MTPOSITION_FILE" && checkLoginValidate == 1)` | `Handle_MTPOSITION_FILE` |
| 5620 | `else if (strtype == "DELETE_DATA_REQUEST" && checkLoginValidate == 1)` | `Handle_DELETE_DATA_REQUEST` |
| 5657 | `else if (strtype == "BALANCE_TRANSFER_REQUEST" && checkLoginValidate == 1)` | `Handle_BALANCE_TRANSFER_REQUEST` |
| 5743 | `else if (strtype == "TRADE_DATE_CHANGE" && checkLoginValidate == 1)` | `Handle_TRADE_DATE_CHANGE` |
| 5750 | `else if (strtype == "DEAL_TRANSFER_REQUEST" && checkLoginValidate == 1)` | `Handle_DEAL_TRANSFER_REQUEST` |
| 5811 | `else if (strtype == "SAVE_VIEW_STATE" && checkLoginValidate == 1)` | `Handle_SAVE_VIEW_STATE` |
| 6014 | `else if (strtype == "UPDATE_REQUEST" && checkLoginValidate == 1)` | `Handle_UPDATE_REQUEST` |

- [ ] **Step 1: Extract all 13 branches**

Apply recipe steps 2–5 to each. Note the `ORDER_UPDATE_REQUEST` special case in the table above.

- [ ] **Step 2: Build and verify**

Run both x64 builds. Expected: both succeed, warning count unchanged.

- [ ] **Step 3: Commit**

```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): extract 13 strtype handlers (chunk G, lines 5018-6014)

Final chunk. Handle_ORDER_UPDATE_REQUEST preserves the body-local
const char* strforjson = str.c_str(); declaration (now uses ctx.str)
which shadows ctx.strforjson — same shadowing existed in the original."
```

---

### Task 11: Final verification

By this point every top-level strtype branch in `MessageReceived` has been replaced with a single-line `Handle_<NAME>(ctx);` call. `MessageReceived` itself should be roughly 300–400 lines (setup code + ~104-line dispatcher + a few outer `else` arms), down from ~5,880.

- [ ] **Step 1: Grep for any remaining inline branch bodies inside `MessageReceived`**

Verify that the only top-level `if (strtype == ...)` / `else if (strtype == ...)` / `else if (strtype.Find(...))` lines inside `MessageReceived` are now one-liners that call a `Handle_<NAME>` function. Run:
```powershell
# Show every line in MessageReceived that mentions strtype as a top-level dispatcher
# (rough heuristic; the engineer should eyeball the actual file)
```

Expected: every such line is followed by either a one-line `Handle_X(ctx);` call OR an `if (Handle_X(ctx)) return;` form for the standalone-if cases from Tasks 2 and 3. No `{` opening a multi-line body remains directly under any of these conditions.

- [ ] **Step 2: Count handlers**

Run:
```powershell
git diff master --stat OrikaServer/server/Server.cpp
```

Verify approximately:
- `MessageReceived` body length reduced by ~5,400 lines.
- Handler section below `MessageReceived` grew by ~5,400 lines.
- Net file size change: small (within ±200 lines for the struct, forward decls, `ctx` construction, and any `bool` wrappers).

Count handler definitions:
```powershell
# from the repo root
# expect ~103 results
Select-String -Path OrikaServer\server\Server.cpp -Pattern "^static (void|bool) Handle_"
```

Expected: matches the total number of branches extracted across Tasks 2–10 (3 standalone + 18 + 11 + 13 + 13 + 16 + 13 + 13 = 100, plus any OR-branch and `strtype.Find` handlers already accounted for — total per the task tables: 3 + 18 + 11 + 13 + 13 + 16 + 13 + 13 = **100 handlers**).

If the count is off, audit the missing branches against the per-task tables.

- [ ] **Step 3: Full clean build of both configurations**

Run:
```powershell
msbuild OrikaServer.sln /t:Clean /p:Configuration=Debug /p:Platform=x64 /m /nologo
msbuild OrikaServer.sln /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:m
msbuild OrikaServer.sln /t:Clean /p:Configuration=Release /p:Platform=x64 /m /nologo
msbuild OrikaServer.sln /p:Configuration=Release /p:Platform=x64 /m /nologo /v:m
```

Expected: both clean builds succeed. Warning count matches the Task 1 baseline.

- [ ] **Step 4: Smoke-test the server**

Launch the server binary at `x64\Debug\OrikaServer.exe` against the same test client / fixture set used for past regression checks (no automated suite exists; the engineer must consult the team for the manual smoke-test checklist). Spot-check at least: a `login` flow, a `LOGOUT` flow, a `FETCH_CLIENTDATA` request, a `HB_R` heartbeat, and one `META_DATA`-prefixed request. Each should behave identically to pre-refactor behavior.

- [ ] **Step 5: Final commit (if any verification fixes were needed)**

If the verification steps surfaced any issue (missed branch, identifier-rewrite bug, etc.), fix it with a focused commit:
```powershell
git add OrikaServer/server/Server.cpp
git commit -m "refactor(server): fix <specific issue found in final verification>"
```

If no fixes were needed, no commit is required for this task.

---

## Out of scope (not modified by this plan)

The following are explicitly **not** changed by any task in this plan. If the engineer is tempted to clean them up while in the area, resist — they belong to a separate refactor:

- The outer `if (!d.Parse<0>(strforjson).HasParseError())` block and its trailing `else { LOGIN_STATUS invalid input }` arm at line ~6178.
- The `if (d.HasMember("type") == true)` block.
- The pre-parse `sub_document` loop at lines 322–342.
- The `m_ClientContext.Lookup` block at lines 364–371.
- `sendDataToAllClient`, `sendDataToAll_Other_Client`, `SendDataToClient`, `SendDataToClient_AllClient`, `SendDataToClient_UnRegistered` definitions.
- Any other function in `Server.cpp` besides `MessageReceived`.
- Nested `if (strtype == ...)` sub-dispatches *inside* a single branch body (COLUMN_TEMPLATE_SAVE/UPDATE inner ifs, META_DATA inner chain).
- Pre-existing commented-out code.
- Variable shadowing in `ORDER_UPDATE_REQUEST` (`const char* strforjson = str.c_str();` shadowing the outer `strforjson`) — preserved as-is.
- Naming inconsistencies (`Handle_login` vs `Handle_LOGIN_BY_KEY` mirror strtype literal casing exactly — do not normalize).
- The `psession` / `client` duality (they alias the same pointer at the dispatcher; both names are kept in `MsgContext` to minimize per-branch diff size).
