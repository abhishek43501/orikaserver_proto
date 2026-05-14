# MessageReceived `strtype` Branch Extraction — Design

**Date:** 2026-05-14
**File targeted:** `OrikaServer/server/Server.cpp`
**Function targeted:** `MessageReceived(SSL_session* psession, char* c_message, int datasize, CString key, CString login, CString HDSLno)` — line 306
**Type of change:** Mechanical refactor (no behavior changes)

## Background

`MessageReceived` is the single entry point that dispatches an incoming SSL message to the right business-logic branch based on its `strtype` field. The function currently spans lines 306–6184 of `OrikaServer/server/Server.cpp` (~5,880 lines) and contains roughly 95 top-level branches on `strtype`. The size of the function makes it hard to read, hard to navigate, and hard to review when individual branches need to change.

This design extracts each top-level `strtype` branch body into its own free function while keeping the dispatcher in place and preserving all behavior exactly.

## Goal

Reduce `MessageReceived` to a thin dispatcher. Each top-level `if (strtype == …)` / `else if (strtype == …)` / `else if (strtype.Find(L"…") >= 0 …)` branch body is moved into a static handler function defined below `MessageReceived` in the same translation unit. The dispatcher chain itself, including its combined `&&` guards, is preserved verbatim.

## Non-goals

- No algorithmic changes inside any branch body.
- No fixes of pre-existing bugs.
- No renaming of identifiers, no reformatting, no SAL/const-correctness changes.
- No removal or rewrite of commented-out code in the function.
- No consolidation of duplicated logic across branches.
- No changes to nested `if (strtype == …)` sub-dispatches inside a single branch body (e.g., the `COLUMN_TEMPLATE_SAVE`/`UPDATE` inner split, the `META_DATA` sub-dispatch). Those stay inside the handler that owns them.
- No changes to the outer structure of `MessageReceived`: the `if (!d.Parse… HasParseError())` block, the `if (d.HasMember("type"))` check, and the trailing `else { … "LOGIN_STATUS" "invalid input" … }` arm are untouched.
- No changes to `sendDataToAllClient`, `SendDataToClient`, or any other function in `Server.cpp`.
- No new files, no header changes, no `.vcxproj` edits.

## Architecture

`MessageReceived` keeps its existing setup code (JSON parse, `m_ClientContext` lookup, locals, etc.). Immediately before the existing dispatcher, a single `MsgContext` struct value is constructed binding references to the shared locals. The dispatcher chain stays in place — same conditions, same combined `&&` guards, same wide-string literals — but each branch body is replaced with a single-line call into the matching handler.

All handler definitions live below `MessageReceived` in the same `Server.cpp`. They are `static` so they have internal linkage and are not visible outside the translation unit. Forward declarations are grouped in a single block between the `MsgContext` definition and `MessageReceived`, so handler ordering below does not matter and individual handlers can be reordered freely.

## The `MsgContext` struct

Declared in `Server.cpp` immediately above `MessageReceived` (file-local; nothing exported to a header). Fields are references for mutable shared state, values for inputs that do not need to flow back.

```cpp
struct MsgContext {
    SSL_session*           psession;          // raw session pointer
    SSL_session*           client;            // alias used inside branches (== psession today)
    rapidjson::Document&   d;                 // parsed JSON document
    const char*            strforjson;        // d's source string (== str.c_str())
    std::string&           str;               // == message after the pre-parse loop; ORDER_UPDATE_REQUEST reads str.c_str() directly
    std::string&           message;           // mutable; pre-parse loop may rewrite it
    CString&               strtype;           // mutable; META_DATA branch & inner sub-dispatches read it
    CString&               strKey;            // mutable; login branch reassigns
    CString&               strloginuser;      // mutable
    int&                   m_ActiveClient;    // mutable
    int&                   checkLoginValidate;// mutable
    int&                   m_clientType;      // mutable
    CString                key;               // function parameter (by value)
    CString                login;             // function parameter (by value)
    CString                HDSLno;            // function parameter (by value)
    CString                _action;           // parsed from JSON early; some branches read it
};
```

Notes:

- `d` is held by reference (RapidJSON `Document` is non-copyable and large).
- `client` and `psession` are kept as separate fields because both names appear in branch bodies; aliasing them in the struct keeps each branch-body diff trivially small.
- Per-handler **local** variables (e.g., `strLogin`, `strslno`, `strTabData` inside the `"login"` branch) remain local to the handler — they are not promoted into the struct.
- A single `MsgContext ctx{ /* field initializers */ };` is constructed immediately before the dispatcher, after all existing setup at the top of `MessageReceived`.

## Naming convention

Each top-level strtype branch becomes one `static void Handle_<NAME>(MsgContext& ctx)`. The name mirrors the strtype literal exactly:

| strtype literal                          | handler name                                    |
| ---------------------------------------- | ----------------------------------------------- |
| `"login"`                                | `Handle_login`                                  |
| `"loginbytoken"`                         | `Handle_loginbytoken`                           |
| `"LOGIN_BY_KEY"`                         | `Handle_LOGIN_BY_KEY`                           |
| `L"FETCH_CLIENTDATA"`                    | `Handle_FETCH_CLIENTDATA`                       |
| `L"FETCH_CLIENT_POSITIONS_TIMERWISE_START"` | `Handle_FETCH_CLIENT_POSITIONS_TIMERWISE_START` |
| `"HB_R"`                                 | `Handle_HB_R`                                   |
| `"LOGOUT"`                               | `Handle_LOGOUT`                                 |

Special cases:

- **OR branches** → single handler named after the first strtype. Example: `(strtype == "COLUMN_TEMPLATE_SAVE" || strtype == "COLUMN_TEMPLATE_UPDATE")` → `Handle_COLUMN_TEMPLATE_SAVE`. The inner `if (strtype == …)` that distinguishes them stays inside that handler.
- **`strtype.Find(L"…")` branches** (lines 4891, 4929, 4941) → named after a representative substring: `Handle_META_DATA`, `Handle_START_HEART_BEAT`, `Handle_FETCH_TICK_DATA`.

## Dispatcher shape

The dispatcher inside `MessageReceived` keeps its existing structure — three standalone top-level `if (strtype == …)` arms (for `"login"`, `"loginbytoken"`, `"LOGIN_BY_KEY"`) followed by a long `else if` chain. Each branch body becomes a single-line handler call. Combined `&&` guards stay at the call site so the handler body is the pure original block.

Illustrative slice:

```cpp
// MsgContext built once, just before this chain
MsgContext ctx{ psession, /*client=*/psession, d, strforjson, str, message,
                strtype, strKey, strloginuser,
                m_ActiveClient, checkLoginValidate, m_clientType,
                key, login, HDSLno, _action };

if (strtype == "login")                                    Handle_login(ctx);
if (strtype == "loginbytoken")                             Handle_loginbytoken(ctx);
if (strtype == "LOGIN_BY_KEY")                             Handle_LOGIN_BY_KEY(ctx);
else if (strtype == L"FETCH_CLIENT_POSITIONS_TIMERWISE_START")
                                                           Handle_FETCH_CLIENT_POSITIONS_TIMERWISE_START(ctx);
else if (strtype == L"FETCH_CLIENT_POSITIONS_TIMERWISE_STOP")
                                                           Handle_FETCH_CLIENT_POSITIONS_TIMERWISE_STOP(ctx);
// ...
else if ((strtype == L"FETCH_CLIENT_POSITIONS"
       || strtype == L"FETCH_CLIENT_POSITIONS_PRENETQTY"
       || strtype == L"FETCH_ORDER_DATA"
       || strtype == L"FETCH_DEALING_DATA"
       || strtype == L"FETCH_LOGIN_DEVICE_LOG"
       || strtype == L"EXISTING_COMMENT_CHANGE_DATA")
      && checkLoginValidate == 1)
                                                           Handle_FETCH_CLIENT_POSITIONS(ctx);
else if (strtype == L"UNSUBSCRIBE" && checkLoginValidate == 1)
                                                           Handle_UNSUBSCRIBE(ctx);
// ...
else if (strtype.Find(L"META_DATA") >= 0 && checkLoginValidate == 1)
                                                           Handle_META_DATA(ctx);
// ... ~95 lines total
```

## Mechanical extraction rules

For each branch body, the transformation is purely textual:

1. Move the block between `{` and matching `}` into a new `static void Handle_<NAME>(MsgContext& ctx) { … }` defined below `MessageReceived`.
2. Inside the moved body, rewrite every reference to a shared local as `ctx.<field>`:
   - `psession` → `ctx.psession`
   - `client` → `ctx.client`
   - `d` → `ctx.d`
   - `strforjson` → `ctx.strforjson`
   - `str` → `ctx.str`
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
3. Do not touch identifiers that are branch-local declarations, struct field accesses through `ctx.psession->…`, `CStaticClass::…` qualifications, or RapidJSON helpers.
4. Do not change control flow, ordering, types, or any algorithm. A `return;` inside a branch body remains a `return;` inside the handler — and because handlers are `void`, that exits the handler only, not `MessageReceived`. **Audit step:** every `return;` originally inside a branch must be inspected to confirm it was intended to short-circuit only that branch, not the whole function. Spot-checked: the `loginbytoken` early-return at the current line 593 short-circuits its own branch — fine to remain a handler-level return. Each branch gets the same audit during execution.
5. Replace the original branch body in `MessageReceived` with a single-line call: `Handle_<NAME>(ctx);`.
6. The dispatcher line itself (the `if (…)` / `else if (…)`) is kept verbatim — same condition, same wide-string literals, same combined `&&` guards.

## Verification

This is a mechanical extraction; there are no unit tests against `MessageReceived` to lean on. Verification bar:

1. The project builds clean in every configuration the `.sln` defines (Debug|x64 and Release|x64 at minimum — confirmed before plan execution). Build the repo-root `.sln` so output lands at `x64\<Config>\` per the existing build conventions.
2. Each handler's body is a verbatim move of the original branch (modulo the `ctx.` field rewrites). The primary correctness check is a per-handler diff review.
3. After full extraction, `git diff --stat OrikaServer/server/Server.cpp` should show roughly: ~5,800 lines removed from inside `MessageReceived`, ~5,800 lines added below it as handler bodies, plus the new `MsgContext` struct, forward declarations, and ~95 one-line dispatcher replacements. Net line count is roughly flat.

## Execution shape

For the writing-plans step that follows this design:

- Single feature branch off `master`. One logical change.
- Commits broken up by chunks of handlers:
  - **Commit 1:** Introduce `MsgContext` struct, forward declarations for all ~95 handlers (as stubs that just contain the original branch body without `ctx.` rewrites yet, OR as empty stubs paired with the dispatcher rewrites in a single commit — the writing-plans step will pick which keeps the build green).
  - **Commits 2..N:** Each moves 15–20 branch bodies into their handlers and applies the `ctx.` rewrites. Build runs clean after every commit.
- Build is run after each commit, before the next is staged.

## Risks

- **Hidden `return;` short-circuits.** If any branch body contains a `return;` that the original author intended to abort the whole `MessageReceived` call (not just the branch), the extraction silently changes behavior — the handler returns, control flows back into the dispatcher, and subsequent unrelated work runs. The audit step in mechanical extraction rule #4 mitigates this. Each `return;` inside a branch must be examined.
- **Variable shadowing.** A branch-local variable in the original body may share a name with a `MsgContext` field. Rewriting `strKey` → `ctx.strKey` blindly could collide. Inspection during each branch move catches this; the move is one branch at a time, not a regex sweep across the file.
- **Compile-time impact.** Adding ~95 forward declarations and ~95 handler definitions to one TU keeps Server.cpp at roughly its current size. No measurable build-time risk.
- **Diff size.** The first review (the one that introduces `MsgContext` and the dispatcher rewrites) is large by nature. Chunked commits during execution mitigate this for incremental review.
