# Mechanical strtype-handler extractor for OrikaServer/server/Server.cpp.
# One extraction per invocation. Edits the file in place.
#
# Usage:
#   .\scripts\extract-handler.ps1 -Name "login" -DispatcherRegex '^\t\t\tif \(strtype == "login"\)' -ReturnsBool $true
#
# Process:
#   1. Read Server.cpp lines.
#   2. Find the dispatcher line matching $DispatcherRegex.
#   3. Find the body { ... } that follows it (matching braces).
#   4. Apply MsgContext identifier rewrites to the body.
#   5. Construct a static handler function.
#   6. Replace the body with a single-line Handle_<Name>(ctx); call,
#      or { if (Handle_<Name>(ctx)) return; } for ReturnsBool.
#   7. Insert the handler definition immediately before `void sendDataToAllClient(`.
#   8. Insert a forward declaration between the `--- BEGIN/END handler forward decls ---` markers.
#   9. Write the file back with CRLF line endings preserved.
#
# Note: Word-boundary regex used for rewrites, with negative lookbehinds to skip
#       struct-member accesses like psession->key, psession->login, psession->HDSLno.

[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][string]$Name,
    [Parameter(Mandatory=$true)][string]$DispatcherRegex,
    [bool]$ReturnsBool = $false,
    [string]$File = "OrikaServer\server\Server.cpp"
)

$ErrorActionPreference = 'Stop'

$raw = [System.IO.File]::ReadAllText($File, [System.Text.Encoding]::UTF8)
# Use \r\n line endings throughout. Detect the file's line ending.
if ($raw -match "`r`n") { $nl = "`r`n" } else { $nl = "`n" }
$lines = $raw -split "`r?`n"
# Note: -split produces an array where the final element is the trailing-newline empty string;
# don't strip it — we'll restore newlines by joining.

# Find dispatcher line
$dispatcherIdx = -1
for ($i = 0; $i -lt $lines.Length; $i++) {
    if ($lines[$i] -match $DispatcherRegex) { $dispatcherIdx = $i; break }
}
if ($dispatcherIdx -lt 0) { throw "Dispatcher line not found for /$DispatcherRegex/" }

# Find the { that opens the body (could be on same line or following line)
$openIdx = $dispatcherIdx
if ($lines[$dispatcherIdx] -notmatch '\{\s*$') {
    # search forward for a line that is just `{` (possibly with whitespace)
    $openIdx = $dispatcherIdx + 1
    while ($openIdx -lt $lines.Length -and $lines[$openIdx].Trim() -ne '{') { $openIdx++ }
    if ($openIdx -ge $lines.Length) { throw "Could not find opening { after dispatcher line $($dispatcherIdx+1)" }
}

# Find matching closing }, skipping braces inside strings and comments.
$depth = 0
$closeIdx = -1
# Start scanning from $openIdx
for ($i = $openIdx; $i -lt $lines.Length; $i++) {
    $line = $lines[$i]
    # Strip line comments (// ...) — naive but adequate for this codebase.
    $codeOnly = $line -replace '//.*$', ''
    # Strip string literals (very naive — handles simple "..." and L"..." cases)
    $codeOnly = $codeOnly -replace '"(?:[^"\\]|\\.)*"', '""'
    foreach ($ch in $codeOnly.ToCharArray()) {
        if ($ch -eq '{') { $depth++ }
        elseif ($ch -eq '}') {
            $depth--
            if ($depth -eq 0) { $closeIdx = $i; break }
        }
    }
    if ($closeIdx -ge 0) { break }
}
if ($closeIdx -lt 0) { throw "Could not find matching } for opening { at line $($openIdx+1)" }

# Body is lines openIdx+1 .. closeIdx-1 (inclusive, 0-indexed).
$bodyLines = if ($openIdx + 1 -le $closeIdx - 1) { $lines[($openIdx+1)..($closeIdx-1)] } else { @() }

# Apply identifier rewrites.
$rewritten = @()
foreach ($line in $bodyLines) {
    $L = $line
    # Skip rewrites in pure line-comment lines (whitespace then //)
    if ($L -match '^\s*//') {
        $rewritten += $L
        continue
    }
    # Negative-lookbehind (?<![\w>]) to skip:
    #   - identifiers that contain the substring as a tail (\w before)
    #   - struct member accesses like psession->X (> before)
    # For very short names (d, str), this is essential.
    $L = $L -replace '(?<![\w>])psession\b', 'ctx.psession'
    $L = $L -replace '(?<![\w>])client\b', 'ctx.client'
    $L = $L -replace '(?<![\w>])strforjson\b', 'ctx.strforjson'
    $L = $L -replace '(?<![\w>])strKey\b', 'ctx.strKey'
    $L = $L -replace '(?<![\w>])strloginuser\b', 'ctx.strloginuser'
    $L = $L -replace '(?<![\w>])strtype\b', 'ctx.strtype'
    $L = $L -replace '(?<![\w>])m_ActiveClient\b', 'ctx.m_ActiveClient'
    $L = $L -replace '(?<![\w>])checkLoginValidate\b', 'ctx.checkLoginValidate'
    $L = $L -replace '(?<![\w>])m_clientType\b', 'ctx.m_clientType'
    $L = $L -replace '(?<![\w>])message\b', 'ctx.message'
    $L = $L -replace '(?<![\w>])_action\b', 'ctx._action'
    # `key`, `login`, `HDSLno` — function parameters. These appear as struct-member
    # access (psession->key etc.) all over this file; the (?<!>) lookbehind protects those.
    $L = $L -replace '(?<![\w>])key\b', 'ctx.key'
    $L = $L -replace '(?<![\w>])login\b', 'ctx.login'
    $L = $L -replace '(?<![\w>])HDSLno\b', 'ctx.HDSLno'
    # Single-letter `d` — restrict to common rapidjson::Document call sites: d., d[, d).
    # Without this restriction, `d` matches inside many other identifiers.
    $L = $L -replace '(?<![\w>])d(?=[\.\[])', 'ctx.d'
    # `str` — outer std::string. Match standalone-only.
    $L = $L -replace '(?<![\w>])str(?=[\.\(])', 'ctx.str'

    if ($ReturnsBool) {
        # `return;` → `return true;` (preserve leading whitespace)
        $L = $L -replace '(^\s*)return;\s*$', '$1return true;'
    }
    $rewritten += $L
}

# Construct handler function (preserve original body indentation; just wrap with sig/braces).
$sig = if ($ReturnsBool) { "static bool Handle_$Name(MsgContext& ctx)" } else { "static void Handle_$Name(MsgContext& ctx)" }
$handlerLines = @($sig, '{')
$handlerLines += $rewritten
if ($ReturnsBool) { $handlerLines += "`treturn false;" }
$handlerLines += '}'
$handlerLines += ''  # blank separator

# Build new dispatcher body. Indentation: detect from dispatcher line.
$dispatcherLine = $lines[$dispatcherIdx]
$dispatcherIndent = ([regex]::Match($dispatcherLine, '^\s*')).Value
$bodyIndent = $dispatcherIndent + "`t"

if ($ReturnsBool) {
    $newBody = @(
        "$dispatcherIndent{",
        "${bodyIndent}if (Handle_$Name(ctx)) return;",
        "$dispatcherIndent}"
    )
} else {
    # Inline the call right after the dispatcher condition, no braces.
    # But the recipe's example uses: "else if (...) Handle_X(ctx);" — i.e. condition + call,
    # without braces. We keep the dispatcher line untouched and emit ONE body line as the
    # next line. That preserves multi-line OR-conditions and `&&` guards.
    $newBody = @("${bodyIndent}Handle_$Name(ctx);")
}

# Splice: replace lines openIdx..closeIdx with $newBody.
# (Replaces both the opening `{` and closing `}` with the call form.)
$before = if ($openIdx -gt 0) { $lines[0..($openIdx-1)] } else { @() }
$after = if ($closeIdx + 1 -lt $lines.Length) { $lines[($closeIdx+1)..($lines.Length-1)] } else { @() }
$spliced = @($before) + @($newBody) + @($after)

# Insert handler function definition immediately before `void sendDataToAllClient(`.
$sendIdx = -1
for ($i = 0; $i -lt $spliced.Length; $i++) {
    if ($spliced[$i] -match '^void sendDataToAllClient\(') { $sendIdx = $i; break }
}
if ($sendIdx -lt 0) { throw "Could not find 'void sendDataToAllClient(' to insert handler before." }

$before2 = if ($sendIdx -gt 0) { $spliced[0..($sendIdx-1)] } else { @() }
$after2 = $spliced[$sendIdx..($spliced.Length-1)]
$final = @($before2) + @($handlerLines) + @($after2)

# Insert forward declaration between markers.
$beginMarker = -1
$endMarker = -1
for ($i = 0; $i -lt $final.Length; $i++) {
    if ($final[$i] -match 'BEGIN handler forward decls') { $beginMarker = $i }
    if ($final[$i] -match 'END handler forward decls') { $endMarker = $i; break }
}
if ($beginMarker -lt 0 -or $endMarker -lt 0) { throw "Forward-decl markers not found." }

$fwdSig = if ($ReturnsBool) { "static bool Handle_$Name(MsgContext& ctx);" } else { "static void Handle_$Name(MsgContext& ctx);" }
# Insert just before END marker.
$beforeFwd = $final[0..($endMarker-1)]
$afterFwd = $final[$endMarker..($final.Length-1)]
$final = @($beforeFwd) + @($fwdSig) + @($afterFwd)

# Write back with original line endings, no BOM.
$outText = ($final -join $nl)
[System.IO.File]::WriteAllText($File, $outText, (New-Object System.Text.UTF8Encoding($false)))

Write-Host "Extracted Handle_$Name : body lines=$($bodyLines.Count), dispatcher at line $($dispatcherIdx+1), inserted handler before sendDataToAllClient at original index $($sendIdx+1)."
