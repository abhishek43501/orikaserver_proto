# OrikaClient Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build `orikaClient.exe`, a standalone C++ console app that connects to an OrikaServer over `wss://`, performs one `login` round-trip, and reports PASS/FAIL with a matching exit code.

**Architecture:** Four units — `TlsConnection` (TCP + OpenSSL TLS), `WebSocketClient` (HTTP `Upgrade` handshake + hand-rolled frame codec), `ProtoCodec` (hand-coded protobuf for the login message), and `main.cpp` (CLI + orchestration). Pure logic (protobuf, base64, SHA1 accept-key, WebSocket framing) is verified offline by a built-in `--selftest` mode using golden byte vectors; the network path is verified by an integration test against a running server.

**Tech Stack:** C++17, Visual Studio 2022 (`v143`, x64), OpenSSL 3.4.0 (vendored), Winsock2. No third-party WebSocket or protobuf library.

---

## Reference

Design spec: `docs/superpowers/specs/2026-05-18-orikaclient-design.md` (in the `orikaserver_proto` repo).

**Protobuf field numbers** (length-delimited / wire type 2 throughout):

| Message       | Field         | Number |
|---------------|---------------|--------|
| ClientMessage | type          | 1      |
| ClientMessage | loginrequest  | 2      |
| ClientMessage | loginresponse | 3      |
| LoginRequest  | type / login / pwd / serialNo | 1 / 2 / 3 / 4 |
| LoginResponse | type / status / logintoken    | 1 / 2 / 3 |

**OpenSSL source to vendor from** (already on this machine, OpenSSL 3.4.0):
`D:\VSCodeProject\orikaserver_proto\OrikaServer\Dependencies_Debug_64\OpenSSL`
— `include\`, `lib\libssl.lib`, `lib\libcrypto.lib`, `BIN\libssl-3-x64.dll`, `BIN\libcrypto-3-x64.dll`.

**Build / run commands** (run from a *Developer PowerShell for VS 2022* so `msbuild` is on PATH):

- Build: `msbuild D:\VSCodeProject\OrikaClient\OrikaClient.sln -p:Configuration=Debug -p:Platform=x64 -v:m`
- Self-test: `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe --selftest`

All `git` commands in this plan run inside the **`D:\VSCodeProject\OrikaClient`** repository (created in Task 1), which is separate from `orikaserver_proto`.

---

## File Structure

```
D:\VSCodeProject\OrikaClient\
  .gitignore
  README.md                       (Task 11)
  OrikaClient.sln
  OrikaClient\
    OrikaClient.vcxproj
    main.cpp                       CLI parsing + orchestration
    TlsConnection.h / .cpp         TCP connect + OpenSSL TLS client
    WebSocketClient.h / .cpp       ws:: pure helpers + WebSocketClient class
    ProtoCodec.h / .cpp            hand-coded protobuf (pure)
    SelfTest.h / .cpp              offline golden-vector test harness
  third_party\openssl\
    include\  lib\  bin\
  x64\Debug\orikaClient.exe        (build output)
```

---

## Task 1: Project skeleton — vendored OpenSSL, VS project, buildable `--help` / `--selftest`

Creates the repo, vendors OpenSSL, and lays down every source file (interfaces fully declared; not-yet-implemented bodies are stubs). End state: the solution builds, `--help` and `--selftest` work.

**Files:**
- Create: `D:\VSCodeProject\OrikaClient\.gitignore`
- Create: `D:\VSCodeProject\OrikaClient\OrikaClient.sln`
- Create: `D:\VSCodeProject\OrikaClient\OrikaClient\OrikaClient.vcxproj`
- Create: `D:\VSCodeProject\OrikaClient\OrikaClient\main.cpp`
- Create: `D:\VSCodeProject\OrikaClient\OrikaClient\SelfTest.h` / `.cpp`
- Create: `D:\VSCodeProject\OrikaClient\OrikaClient\ProtoCodec.h` / `.cpp`
- Create: `D:\VSCodeProject\OrikaClient\OrikaClient\WebSocketClient.h` / `.cpp`
- Create: `D:\VSCodeProject\OrikaClient\OrikaClient\TlsConnection.h` / `.cpp`
- Create: `third_party\openssl\` (copied)

- [ ] **Step 1: Create folders and vendor OpenSSL**

Run (PowerShell):
```powershell
$dst = "D:\VSCodeProject\OrikaClient"
New-Item -ItemType Directory -Force -Path "$dst\OrikaClient" | Out-Null
New-Item -ItemType Directory -Force -Path "$dst\third_party\openssl\bin" | Out-Null
$src = "D:\VSCodeProject\orikaserver_proto\OrikaServer\Dependencies_Debug_64\OpenSSL"
Copy-Item -Recurse -Force "$src\include" "$dst\third_party\openssl\include"
Copy-Item -Recurse -Force "$src\lib"     "$dst\third_party\openssl\lib"
Copy-Item -Force "$src\BIN\libssl-3-x64.dll"    "$dst\third_party\openssl\bin\"
Copy-Item -Force "$src\BIN\libcrypto-3-x64.dll" "$dst\third_party\openssl\bin\"
```
Expected: `third_party\openssl\include\openssl\ssl.h`, `lib\libssl.lib`, `lib\libcrypto.lib`, and the two DLLs in `bin\` all exist.

- [ ] **Step 2: Write `.gitignore`**

Create `D:\VSCodeProject\OrikaClient\.gitignore`:
```gitignore
x64/
.vs/
*.user
*.aps
*.VC.db
```

- [ ] **Step 3: Write `OrikaClient.sln`**

Create `D:\VSCodeProject\OrikaClient\OrikaClient.sln`:
```
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 17
VisualStudioVersion = 17.0.31903.59
MinimumVisualStudioVersion = 10.0.40219.1
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "OrikaClient", "OrikaClient\OrikaClient.vcxproj", "{A1B2C3D4-E5F6-47A8-9B0C-1D2E3F405162}"
EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|x64 = Debug|x64
		Release|x64 = Release|x64
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		{A1B2C3D4-E5F6-47A8-9B0C-1D2E3F405162}.Debug|x64.ActiveCfg = Debug|x64
		{A1B2C3D4-E5F6-47A8-9B0C-1D2E3F405162}.Debug|x64.Build.0 = Debug|x64
		{A1B2C3D4-E5F6-47A8-9B0C-1D2E3F405162}.Release|x64.ActiveCfg = Release|x64
		{A1B2C3D4-E5F6-47A8-9B0C-1D2E3F405162}.Release|x64.Build.0 = Release|x64
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
EndGlobal
```

- [ ] **Step 4: Write `OrikaClient.vcxproj`**

Create `D:\VSCodeProject\OrikaClient\OrikaClient\OrikaClient.vcxproj`:
```xml
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>17.0</VCProjectVersion>
    <ProjectGuid>{A1B2C3D4-E5F6-47A8-9B0C-1D2E3F405162}</ProjectGuid>
    <RootNamespace>OrikaClient</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)'=='Debug'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <CharacterSet>MultiByte</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)'=='Release'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>MultiByte</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <PropertyGroup>
    <OutDir>$(SolutionDir)x64\$(Configuration)\</OutDir>
    <IntDir>$(SolutionDir)x64\$(Configuration)\obj\</IntDir>
    <TargetName>orikaClient</TargetName>
  </PropertyGroup>
  <ItemDefinitionGroup>
    <ClCompile>
      <LanguageStandard>stdcpp17</LanguageStandard>
      <WarningLevel>Level3</WarningLevel>
      <AdditionalIncludeDirectories>$(SolutionDir)third_party\openssl\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <PreprocessorDefinitions>_CRT_SECURE_NO_WARNINGS;WIN32_LEAN_AND_MEAN;%(PreprocessorDefinitions)</PreprocessorDefinitions>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <AdditionalLibraryDirectories>$(SolutionDir)third_party\openssl\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>libssl.lib;libcrypto.lib;ws2_32.lib;crypt32.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
    <PostBuildEvent>
      <Command>xcopy /Y /D "$(SolutionDir)third_party\openssl\bin\*.dll" "$(OutDir)"</Command>
      <Message>Copying OpenSSL runtime DLLs</Message>
    </PostBuildEvent>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)'=='Debug'">
    <ClCompile>
      <Optimization>Disabled</Optimization>
      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>
    </ClCompile>
    <Link>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)'=='Release'">
    <ClCompile>
      <Optimization>MaxSpeed</Optimization>
      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
    </ClCompile>
    <Link>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClCompile Include="main.cpp" />
    <ClCompile Include="ProtoCodec.cpp" />
    <ClCompile Include="WebSocketClient.cpp" />
    <ClCompile Include="TlsConnection.cpp" />
    <ClCompile Include="SelfTest.cpp" />
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="ProtoCodec.h" />
    <ClInclude Include="WebSocketClient.h" />
    <ClInclude Include="TlsConnection.h" />
    <ClInclude Include="SelfTest.h" />
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
</Project>
```

- [ ] **Step 5: Write `ProtoCodec.h` (full interface)**

Create `D:\VSCodeProject\OrikaClient\OrikaClient\ProtoCodec.h`:
```cpp
#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace proto {

// Base-128 varint.
std::vector<uint8_t> encodeVarint(uint64_t value);
// Reads a varint starting at buf[pos], advances pos. Throws std::runtime_error on overrun.
uint64_t decodeVarint(const std::vector<uint8_t>& buf, size_t& pos);

// One length-delimited (wire type 2) field: tag + varint length + payload.
std::vector<uint8_t> encodeLenDelimField(int fieldNumber,
                                         const std::vector<uint8_t>& payload);

// Full login ClientMessage bytes.
std::vector<uint8_t> encodeLogin(const std::string& login,
                                 const std::string& pwd,
                                 const std::string& serial);

struct LoginStatus {
    bool ok = false;          // true once a loginresponse (field 3) was parsed
    std::string type;         // ClientMessage.type (field 1)
    std::string status;       // LoginResponse.status (field 2)
    std::string token;        // LoginResponse.logintoken (field 3)
};

// Walks a reply ClientMessage. Never throws; returns ok=false if unparseable.
LoginStatus decodeLoginStatus(const std::vector<uint8_t>& buf);

} // namespace proto
```

- [ ] **Step 6: Write `ProtoCodec.cpp` (stubs)**

Create `D:\VSCodeProject\OrikaClient\OrikaClient\ProtoCodec.cpp`:
```cpp
#include "ProtoCodec.h"

namespace proto {

std::vector<uint8_t> encodeVarint(uint64_t) { return {}; }

uint64_t decodeVarint(const std::vector<uint8_t>&, size_t&) { return 0; }

std::vector<uint8_t> encodeLenDelimField(int, const std::vector<uint8_t>&) { return {}; }

std::vector<uint8_t> encodeLogin(const std::string&, const std::string&, const std::string&) { return {}; }

LoginStatus decodeLoginStatus(const std::vector<uint8_t>&) { return LoginStatus{}; }

} // namespace proto
```

- [ ] **Step 7: Write `WebSocketClient.h` (full interface)**

Create `D:\VSCodeProject\OrikaClient\OrikaClient\WebSocketClient.h`:
```cpp
#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "TlsConnection.h"

namespace ws {

// --- Pure helpers (unit-tested via --selftest) ---
std::string base64Encode(const std::vector<uint8_t>& data);

// base64(SHA1(secWebSocketKey + RFC6455 GUID)).
std::string computeAcceptKey(const std::string& secWebSocketKey);

// One client->server binary frame: FIN+opcode 0x2, masked with the given 4-byte key.
std::vector<uint8_t> encodeBinaryFrame(const std::vector<uint8_t>& payload,
                                       const uint8_t mask[4]);

struct Frame {
    bool fin = false;
    uint8_t opcode = 0;
    std::vector<uint8_t> payload;
};
// Parses one frame from buf at 'offset'. Returns true and advances 'offset'
// if a complete frame is present; false if more bytes are needed.
bool decodeFrame(const std::vector<uint8_t>& buf, size_t& offset, Frame& out);

} // namespace ws

// --- WebSocket connection over a TlsConnection ---
class WebSocketClient {
public:
    explicit WebSocketClient(TlsConnection& tls);
    void handshake(const std::string& host, int port, const std::string& path);
    void sendBinary(const std::vector<uint8_t>& payload);
    std::vector<uint8_t> receiveMessage(int timeoutSec);
private:
    TlsConnection& tls_;
    std::vector<uint8_t> rx_;
};
```

- [ ] **Step 8: Write `WebSocketClient.cpp` (stubs)**

Create `D:\VSCodeProject\OrikaClient\OrikaClient\WebSocketClient.cpp`:
```cpp
#include "WebSocketClient.h"

namespace ws {

std::string base64Encode(const std::vector<uint8_t>&) { return ""; }

std::string computeAcceptKey(const std::string&) { return ""; }

std::vector<uint8_t> encodeBinaryFrame(const std::vector<uint8_t>&, const uint8_t[4]) { return {}; }

bool decodeFrame(const std::vector<uint8_t>&, size_t&, Frame&) { return false; }

} // namespace ws

WebSocketClient::WebSocketClient(TlsConnection& tls) : tls_(tls) {}

void WebSocketClient::handshake(const std::string&, int, const std::string&) {}

void WebSocketClient::sendBinary(const std::vector<uint8_t>&) {}

std::vector<uint8_t> WebSocketClient::receiveMessage(int) { return {}; }
```

- [ ] **Step 9: Write `TlsConnection.h` and `TlsConnection.cpp` (stubs)**

Create `D:\VSCodeProject\OrikaClient\OrikaClient\TlsConnection.h`:
```cpp
#pragma once
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <openssl/ssl.h>

class TlsConnection {
public:
    TlsConnection() = default;
    ~TlsConnection();
    TlsConnection(const TlsConnection&) = delete;
    TlsConnection& operator=(const TlsConnection&) = delete;

    // Throws std::runtime_error on failure.
    void connect(const std::string& host, int port, bool verifyCert, int timeoutSec);
    int read(char* buf, int len);     // >0 = bytes; 0 = closed/timeout; <0 = error
    int write(const char* buf, int len);
    void close();
private:
    SOCKET sock_ = INVALID_SOCKET;
    SSL_CTX* ctx_ = nullptr;
    SSL* ssl_ = nullptr;
    bool wsaInit_ = false;
};
```

Create `D:\VSCodeProject\OrikaClient\OrikaClient\TlsConnection.cpp`:
```cpp
#include "TlsConnection.h"

TlsConnection::~TlsConnection() { close(); }

void TlsConnection::connect(const std::string&, int, bool, int) {}

int TlsConnection::read(char*, int) { return 0; }

int TlsConnection::write(const char*, int) { return 0; }

void TlsConnection::close() {}
```

- [ ] **Step 10: Write `SelfTest.h` and `SelfTest.cpp`**

Create `D:\VSCodeProject\OrikaClient\OrikaClient\SelfTest.h`:
```cpp
#pragma once
// Runs offline golden-vector checks. Returns true if all pass.
bool RunSelfTest();
```

Create `D:\VSCodeProject\OrikaClient\OrikaClient\SelfTest.cpp`:
```cpp
#include "SelfTest.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace {

int g_failures = 0;

void check(bool cond, const char* name) {
    if (cond) {
        std::cout << "  ok   " << name << "\n";
    } else {
        std::cout << "  FAIL " << name << "\n";
        ++g_failures;
    }
}

// Bytes of an ASCII string.
std::vector<uint8_t> B(const std::string& s) {
    return std::vector<uint8_t>(s.begin(), s.end());
}

} // namespace

bool RunSelfTest() {
    g_failures = 0;
    std::cout << "Running orikaClient self-test...\n";

    check(true, "selftest harness");

    // === ADD CHECKS BELOW (later tasks insert here) ===

    // === END CHECKS ===

    if (g_failures == 0) {
        std::cout << "SELFTEST OK\n";
        return true;
    }
    std::cout << "SELFTEST FAIL: " << g_failures << " check(s) failed\n";
    return false;
}
```

The helpers `B` and `check`, and the variable `g_failures`, are used by every later task; the `(void)B;` is unnecessary because Task 2 onward references `B`. To avoid an unused-function warning until then, Task 1 leaves it as written — `B` is referenced starting in Task 2, which is committed immediately after. If a warning appears at Task 1 build, it is non-fatal (Level3 warning) and disappears in Task 2.

- [ ] **Step 11: Write `main.cpp` (minimal)**

Create `D:\VSCodeProject\OrikaClient\OrikaClient\main.cpp`:
```cpp
#include <iostream>
#include <cstring>
#include "SelfTest.h"

int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--help") == 0) {
            std::cout <<
                "orikaClient - OrikaServer wss login test client\n"
                "Usage: orikaClient.exe [options]\n"
                "  --selftest   run offline codec self-checks and exit\n"
                "  --help       show this help\n";
            return 0;
        }
        if (std::strcmp(argv[i], "--selftest") == 0) {
            return RunSelfTest() ? 0 : 1;
        }
    }
    std::cout << "orikaClient: no command given. Try --help.\n";
    return 2;
}
```

- [ ] **Step 12: Build the solution**

Run: `msbuild D:\VSCodeProject\OrikaClient\OrikaClient.sln -p:Configuration=Debug -p:Platform=x64 -v:m`
Expected: `Build succeeded`, and `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe` plus `libssl-3-x64.dll` / `libcrypto-3-x64.dll` exist.

- [ ] **Step 13: Verify `--help` and `--selftest`**

Run: `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe --selftest`
Expected: prints `  ok   selftest harness` then `SELFTEST OK`; exit code 0.

Run: `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe --help`
Expected: prints usage; exit code 0.

- [ ] **Step 14: Initialise the repo and commit**

Run (inside `D:\VSCodeProject\OrikaClient`):
```bash
git init
git add .gitignore OrikaClient.sln OrikaClient/ third_party/
git commit -m "chore: OrikaClient project skeleton with vendored OpenSSL"
```

---

## Task 2: Varint codec

**Files:**
- Modify: `OrikaClient\ProtoCodec.cpp` (replace `encodeVarint`, `decodeVarint` stubs)
- Test: `OrikaClient\SelfTest.cpp`

- [ ] **Step 1: Write the failing test**

In `SelfTest.cpp`, add `#include "ProtoCodec.h"` to the include block (after `#include "SelfTest.h"`). Then replace the line `    // === ADD CHECKS BELOW (later tasks insert here) ===` and the blank line after it with:
```cpp
    // === ADD CHECKS BELOW (later tasks insert here) ===

    // Task 2: varint
    check(proto::encodeVarint(0)   == std::vector<uint8_t>{0x00}, "varint encode 0");
    check(proto::encodeVarint(1)   == std::vector<uint8_t>{0x01}, "varint encode 1");
    check(proto::encodeVarint(127) == std::vector<uint8_t>{0x7F}, "varint encode 127");
    check(proto::encodeVarint(128) == (std::vector<uint8_t>{0x80, 0x01}), "varint encode 128");
    check(proto::encodeVarint(300) == (std::vector<uint8_t>{0xAC, 0x02}), "varint encode 300");
    {
        std::vector<uint8_t> v = proto::encodeVarint(123456);
        size_t pos = 0;
        check(proto::decodeVarint(v, pos) == 123456 && pos == v.size(), "varint round-trip 123456");
    }
```

- [ ] **Step 2: Build and run — verify it fails**

Run: `msbuild D:\VSCodeProject\OrikaClient\OrikaClient.sln -p:Configuration=Debug -p:Platform=x64 -v:m`
then `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe --selftest`
Expected: `SELFTEST FAIL` — the varint checks report `FAIL` (stub returns `{}` / `0`).

- [ ] **Step 3: Implement varint**

In `ProtoCodec.cpp`, replace `std::vector<uint8_t> encodeVarint(uint64_t) { return {}; }` with:
```cpp
std::vector<uint8_t> encodeVarint(uint64_t value) {
    std::vector<uint8_t> out;
    do {
        uint8_t b = static_cast<uint8_t>(value & 0x7F);
        value >>= 7;
        if (value != 0) b |= 0x80;
        out.push_back(b);
    } while (value != 0);
    return out;
}
```
And replace `uint64_t decodeVarint(const std::vector<uint8_t>&, size_t&) { return 0; }` with:
```cpp
uint64_t decodeVarint(const std::vector<uint8_t>& buf, size_t& pos) {
    uint64_t result = 0;
    int shift = 0;
    while (true) {
        if (pos >= buf.size())
            throw std::runtime_error("decodeVarint: buffer overrun");
        uint8_t b = buf[pos++];
        result |= static_cast<uint64_t>(b & 0x7F) << shift;
        if ((b & 0x80) == 0) break;
        shift += 7;
        if (shift >= 64)
            throw std::runtime_error("decodeVarint: varint too long");
    }
    return result;
}
```
Add `#include <stdexcept>` below `#include "ProtoCodec.h"` in `ProtoCodec.cpp`.

- [ ] **Step 4: Build and run — verify it passes**

Run the build + `--selftest` commands from Step 2.
Expected: all varint checks `ok`; `SELFTEST OK`; exit 0.

- [ ] **Step 5: Commit**
```bash
git add OrikaClient/ProtoCodec.cpp OrikaClient/SelfTest.cpp
git commit -m "feat: protobuf varint encode/decode"
```

---

## Task 3: Length-delimited field encoder

**Files:**
- Modify: `OrikaClient\ProtoCodec.cpp` (replace `encodeLenDelimField` stub)
- Test: `OrikaClient\SelfTest.cpp`

- [ ] **Step 1: Write the failing test**

In `SelfTest.cpp`, immediately before the `    // === END CHECKS ===` line, insert:
```cpp
    // Task 3: length-delimited field (field 1, "login")
    check(proto::encodeLenDelimField(1, B("login")) ==
              (std::vector<uint8_t>{0x0A, 0x05, 0x6C, 0x6F, 0x67, 0x69, 0x6E}),
          "field encode 1:'login'");
    // field 2, "1001"
    check(proto::encodeLenDelimField(2, B("1001")) ==
              (std::vector<uint8_t>{0x12, 0x04, 0x31, 0x30, 0x30, 0x31}),
          "field encode 2:'1001'");
```

- [ ] **Step 2: Build and run — verify it fails**

Run build + `--selftest`. Expected: `SELFTEST FAIL` — the two field-encode checks `FAIL` (stub returns `{}`).

- [ ] **Step 3: Implement the field encoder**

In `ProtoCodec.cpp`, replace `std::vector<uint8_t> encodeLenDelimField(int, const std::vector<uint8_t>&) { return {}; }` with:
```cpp
std::vector<uint8_t> encodeLenDelimField(int fieldNumber,
                                         const std::vector<uint8_t>& payload) {
    std::vector<uint8_t> out;
    // tag = (field_number << 3) | wire_type(2)
    std::vector<uint8_t> tag = encodeVarint((static_cast<uint64_t>(fieldNumber) << 3) | 2);
    out.insert(out.end(), tag.begin(), tag.end());
    std::vector<uint8_t> len = encodeVarint(payload.size());
    out.insert(out.end(), len.begin(), len.end());
    out.insert(out.end(), payload.begin(), payload.end());
    return out;
}
```

- [ ] **Step 4: Build and run — verify it passes**

Run build + `--selftest`. Expected: field-encode checks `ok`; `SELFTEST OK`.

- [ ] **Step 5: Commit**
```bash
git add OrikaClient/ProtoCodec.cpp OrikaClient/SelfTest.cpp
git commit -m "feat: protobuf length-delimited field encoder"
```

---

## Task 4: encodeLogin

**Files:**
- Modify: `OrikaClient\ProtoCodec.cpp` (replace `encodeLogin` stub)
- Test: `OrikaClient\SelfTest.cpp`

Golden vector for `encodeLogin("1001", "pw", "sn")` — 30 bytes:
`0A 05 6C 6F 67 69 6E 12 15 0A 05 6C 6F 67 69 6E 12 04 31 30 30 31 1A 02 70 77 22 02 73 6E`
(ClientMessage field 1 `type="login"`, then field 2 `loginrequest` = 21-byte LoginRequest with fields 1–4.)

- [ ] **Step 1: Write the failing test**

In `SelfTest.cpp`, immediately before `    // === END CHECKS ===`, insert:
```cpp
    // Task 4: encodeLogin golden vector
    {
        std::vector<uint8_t> expected = {
            0x0A,0x05,0x6C,0x6F,0x67,0x69,0x6E,       // CM field1 type="login"
            0x12,0x15,                                 // CM field2 loginrequest, len 21
            0x0A,0x05,0x6C,0x6F,0x67,0x69,0x6E,       //   LR field1 type="login"
            0x12,0x04,0x31,0x30,0x30,0x31,             //   LR field2 login="1001"
            0x1A,0x02,0x70,0x77,                       //   LR field3 pwd="pw"
            0x22,0x02,0x73,0x6E                        //   LR field4 serialNo="sn"
        };
        check(proto::encodeLogin("1001", "pw", "sn") == expected, "encodeLogin golden vector");
    }
```

- [ ] **Step 2: Build and run — verify it fails**

Run build + `--selftest`. Expected: `SELFTEST FAIL` — `encodeLogin golden vector` reports `FAIL`.

- [ ] **Step 3: Implement encodeLogin**

In `ProtoCodec.cpp`, add this helper just inside `namespace proto {` (above `encodeVarint`):
```cpp
static std::vector<uint8_t> bytesOf(const std::string& s) {
    return std::vector<uint8_t>(s.begin(), s.end());
}

static void appendAll(std::vector<uint8_t>& dst, const std::vector<uint8_t>& src) {
    dst.insert(dst.end(), src.begin(), src.end());
}
```
Then replace `std::vector<uint8_t> encodeLogin(const std::string&, const std::string&, const std::string&) { return {}; }` with:
```cpp
std::vector<uint8_t> encodeLogin(const std::string& login,
                                 const std::string& pwd,
                                 const std::string& serial) {
    // LoginRequest { 1:type="login", 2:login, 3:pwd, 4:serialNo }
    std::vector<uint8_t> lr;
    appendAll(lr, encodeLenDelimField(1, bytesOf("login")));
    appendAll(lr, encodeLenDelimField(2, bytesOf(login)));
    appendAll(lr, encodeLenDelimField(3, bytesOf(pwd)));
    appendAll(lr, encodeLenDelimField(4, bytesOf(serial)));

    // ClientMessage { 1:type="login", 2:loginrequest=lr }
    std::vector<uint8_t> cm;
    appendAll(cm, encodeLenDelimField(1, bytesOf("login")));
    appendAll(cm, encodeLenDelimField(2, lr));
    return cm;
}
```

- [ ] **Step 4: Build and run — verify it passes**

Run build + `--selftest`. Expected: `encodeLogin golden vector` `ok`; `SELFTEST OK`.

- [ ] **Step 5: Commit**
```bash
git add OrikaClient/ProtoCodec.cpp OrikaClient/SelfTest.cpp
git commit -m "feat: encode login ClientMessage"
```

---

## Task 5: decodeLoginStatus

**Files:**
- Modify: `OrikaClient\ProtoCodec.cpp` (replace `decodeLoginStatus` stub)
- Test: `OrikaClient\SelfTest.cpp`

Golden reply — a `ClientMessage{ 1:type="LOGIN_STATUS", 3:loginresponse: LoginResponse{ 1:type="LOGIN_STATUS", 2:status="success", 3:logintoken="tok" } }`, 44 bytes.

- [ ] **Step 1: Write the failing test**

In `SelfTest.cpp`, immediately before `    // === END CHECKS ===`, insert:
```cpp
    // Task 5: decodeLoginStatus golden reply
    {
        std::vector<uint8_t> reply = {
            0x0A,0x0C,0x4C,0x4F,0x47,0x49,0x4E,0x5F,0x53,0x54,0x41,0x54,0x55,0x53, // CM f1 type
            0x1A,0x1C,                                                             // CM f3 loginresponse len 28
            0x0A,0x0C,0x4C,0x4F,0x47,0x49,0x4E,0x5F,0x53,0x54,0x41,0x54,0x55,0x53, //   LR f1 type
            0x12,0x07,0x73,0x75,0x63,0x63,0x65,0x73,0x73,                          //   LR f2 status="success"
            0x1A,0x03,0x74,0x6F,0x6B                                               //   LR f3 token="tok"
        };
        proto::LoginStatus ls = proto::decodeLoginStatus(reply);
        check(ls.ok, "decodeLoginStatus ok");
        check(ls.type == "LOGIN_STATUS", "decodeLoginStatus type");
        check(ls.status == "success", "decodeLoginStatus status");
        check(ls.token == "tok", "decodeLoginStatus token");
    }
    // Garbage input must not throw and must report ok=false.
    check(proto::decodeLoginStatus(std::vector<uint8_t>{0xFF,0xFF,0xFF}).ok == false,
          "decodeLoginStatus rejects garbage");
```

- [ ] **Step 2: Build and run — verify it fails**

Run build + `--selftest`. Expected: `SELFTEST FAIL` — `decodeLoginStatus ok/type/status/token` report `FAIL` (stub returns empty `LoginStatus`). `rejects garbage` happens to pass (stub `ok` is false) — that is fine.

- [ ] **Step 3: Implement decodeLoginStatus**

In `ProtoCodec.cpp`, add this anonymous-namespace helper just above `LoginStatus decodeLoginStatus(...)`:
```cpp
namespace {

struct RawField {
    int number = 0;
    int wireType = 0;
    std::vector<uint8_t> data;   // for wire type 2
};

// Parses wire-type 0/1/2/5 fields of one message buffer. Throws on malformed input.
std::vector<RawField> parseFields(const std::vector<uint8_t>& buf) {
    std::vector<RawField> out;
    size_t pos = 0;
    while (pos < buf.size()) {
        uint64_t tag = proto::decodeVarint(buf, pos);
        RawField f;
        f.number = static_cast<int>(tag >> 3);
        f.wireType = static_cast<int>(tag & 7);
        if (f.wireType == 0) {              // varint
            proto::decodeVarint(buf, pos);
        } else if (f.wireType == 2) {       // length-delimited
            uint64_t len = proto::decodeVarint(buf, pos);
            if (pos + len > buf.size())
                throw std::runtime_error("parseFields: length overrun");
            f.data.assign(buf.begin() + pos, buf.begin() + pos + static_cast<size_t>(len));
            pos += static_cast<size_t>(len);
        } else if (f.wireType == 1) {       // 64-bit
            if (pos + 8 > buf.size()) throw std::runtime_error("parseFields: i64 overrun");
            pos += 8;
        } else if (f.wireType == 5) {       // 32-bit
            if (pos + 4 > buf.size()) throw std::runtime_error("parseFields: i32 overrun");
            pos += 4;
        } else {
            throw std::runtime_error("parseFields: unsupported wire type");
        }
        out.push_back(std::move(f));
    }
    return out;
}

} // namespace
```
Then replace `LoginStatus decodeLoginStatus(const std::vector<uint8_t>&) { return LoginStatus{}; }` with:
```cpp
LoginStatus decodeLoginStatus(const std::vector<uint8_t>& buf) {
    LoginStatus ls;
    try {
        std::vector<RawField> top = parseFields(buf);
        for (const RawField& f : top) {
            if (f.number == 1 && f.wireType == 2) {
                ls.type.assign(f.data.begin(), f.data.end());
            } else if (f.number == 3 && f.wireType == 2) {
                std::vector<RawField> inner = parseFields(f.data);
                for (const RawField& g : inner) {
                    if (g.number == 2 && g.wireType == 2)
                        ls.status.assign(g.data.begin(), g.data.end());
                    else if (g.number == 3 && g.wireType == 2)
                        ls.token.assign(g.data.begin(), g.data.end());
                }
                ls.ok = true;
            }
        }
    } catch (const std::exception&) {
        ls.ok = false;
    }
    return ls;
}
```

- [ ] **Step 4: Build and run — verify it passes**

Run build + `--selftest`. Expected: all `decodeLoginStatus` checks `ok`; `SELFTEST OK`.

- [ ] **Step 5: Commit**
```bash
git add OrikaClient/ProtoCodec.cpp OrikaClient/SelfTest.cpp
git commit -m "feat: decode LOGIN_STATUS reply"
```

---

## Task 6: base64 + WebSocket accept-key

**Files:**
- Modify: `OrikaClient\WebSocketClient.cpp` (replace `base64Encode`, `computeAcceptKey` stubs)
- Test: `OrikaClient\SelfTest.cpp`

Test vectors: `base64("Man")="TWFu"`, `base64("Ma")="TWE="`, `base64("M")="TQ=="`. RFC 6455 accept example: key `dGhlIHNhbXBsZSBub25jZQ==` → accept `s3pPLMBiTxaQ9kYGzzhZRbK+xOo=`.

- [ ] **Step 1: Write the failing test**

In `SelfTest.cpp`, add `#include "WebSocketClient.h"` to the include block. Then, immediately before `    // === END CHECKS ===`, insert:
```cpp
    // Task 6: base64
    check(ws::base64Encode(B("Man")) == "TWFu", "base64 'Man'");
    check(ws::base64Encode(B("Ma"))  == "TWE=", "base64 'Ma'");
    check(ws::base64Encode(B("M"))   == "TQ==", "base64 'M'");
    check(ws::base64Encode(std::vector<uint8_t>{}) == "", "base64 empty");
    // Task 6: WebSocket accept-key (RFC 6455 example)
    check(ws::computeAcceptKey("dGhlIHNhbXBsZSBub25jZQ==") == "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=",
          "computeAcceptKey RFC6455");
```

- [ ] **Step 2: Build and run — verify it fails**

Run build + `--selftest`. Expected: `SELFTEST FAIL` — base64 and accept-key checks `FAIL` (stubs return `""`).

- [ ] **Step 3: Implement base64Encode and computeAcceptKey**

In `WebSocketClient.cpp`, add includes at the top (below `#include "WebSocketClient.h"`):
```cpp
#include <openssl/evp.h>
```
Replace `std::string base64Encode(const std::vector<uint8_t>&) { return ""; }` with:
```cpp
std::string base64Encode(const std::vector<uint8_t>& data) {
    static const char T[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    size_t i = 0;
    while (i + 3 <= data.size()) {
        uint32_t n = (static_cast<uint32_t>(data[i]) << 16) |
                     (static_cast<uint32_t>(data[i + 1]) << 8) |
                      static_cast<uint32_t>(data[i + 2]);
        out += T[(n >> 18) & 63];
        out += T[(n >> 12) & 63];
        out += T[(n >> 6) & 63];
        out += T[n & 63];
        i += 3;
    }
    size_t rem = data.size() - i;
    if (rem == 1) {
        uint32_t n = static_cast<uint32_t>(data[i]) << 16;
        out += T[(n >> 18) & 63];
        out += T[(n >> 12) & 63];
        out += '=';
        out += '=';
    } else if (rem == 2) {
        uint32_t n = (static_cast<uint32_t>(data[i]) << 16) |
                     (static_cast<uint32_t>(data[i + 1]) << 8);
        out += T[(n >> 18) & 63];
        out += T[(n >> 12) & 63];
        out += T[(n >> 6) & 63];
        out += '=';
    }
    return out;
}
```
Replace `std::string computeAcceptKey(const std::string&) { return ""; }` with:
```cpp
std::string computeAcceptKey(const std::string& secWebSocketKey) {
    std::string s = secWebSocketKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int dlen = 0;
    // EVP one-shot SHA1 (avoids the deprecated SHA1() symbol in OpenSSL 3).
    EVP_Digest(s.data(), s.size(), digest, &dlen, EVP_sha1(), nullptr);
    return base64Encode(std::vector<uint8_t>(digest, digest + dlen));
}
```

- [ ] **Step 4: Build and run — verify it passes**

Run build + `--selftest`. Expected: base64 + `computeAcceptKey RFC6455` checks `ok`; `SELFTEST OK`.

- [ ] **Step 5: Commit**
```bash
git add OrikaClient/WebSocketClient.cpp OrikaClient/SelfTest.cpp
git commit -m "feat: base64 encode and WebSocket accept-key"
```

---

## Task 7: WebSocket frame encode/decode

**Files:**
- Modify: `OrikaClient\WebSocketClient.cpp` (replace `encodeBinaryFrame`, `decodeFrame` stubs)
- Test: `OrikaClient\SelfTest.cpp`

Vectors: encoding `"Hi"` with mask `{01,02,03,04}` → `82 82 01 02 03 04 49 6B`. Decoding the RFC 6455 masked example `81 85 37 FA 21 3D 7F 9F 4D 51 58` → opcode 1, fin, payload `"Hello"`.

- [ ] **Step 1: Write the failing test**

In `SelfTest.cpp`, immediately before `    // === END CHECKS ===`, insert:
```cpp
    // Task 7: encodeBinaryFrame ("Hi", mask 01 02 03 04)
    {
        uint8_t mask[4] = {0x01, 0x02, 0x03, 0x04};
        std::vector<uint8_t> frame = ws::encodeBinaryFrame(B("Hi"), mask);
        std::vector<uint8_t> expected = {0x82, 0x82, 0x01, 0x02, 0x03, 0x04, 0x49, 0x6B};
        check(frame == expected, "encodeBinaryFrame 'Hi'");
    }
    // Task 7: decodeFrame on RFC 6455 masked example -> "Hello"
    {
        std::vector<uint8_t> buf = {0x81,0x85,0x37,0xFA,0x21,0x3D,0x7F,0x9F,0x4D,0x51,0x58};
        size_t off = 0;
        ws::Frame f;
        bool ok = ws::decodeFrame(buf, off, f);
        check(ok && f.fin && f.opcode == 0x1, "decodeFrame header");
        check(std::string(f.payload.begin(), f.payload.end()) == "Hello", "decodeFrame payload");
        check(off == buf.size(), "decodeFrame consumed all bytes");
    }
    // Task 7: decodeFrame returns false when the buffer is short
    {
        std::vector<uint8_t> buf = {0x82, 0x05, 0x61};  // claims 5 bytes, only 1 present
        size_t off = 0;
        ws::Frame f;
        check(ws::decodeFrame(buf, off, f) == false, "decodeFrame incomplete -> false");
    }
    // Task 7: encode then decode round-trips the payload
    {
        uint8_t mask[4] = {0xAA, 0xBB, 0xCC, 0xDD};
        std::vector<uint8_t> frame = ws::encodeBinaryFrame(B("round-trip!"), mask);
        size_t off = 0;
        ws::Frame f;
        bool ok = ws::decodeFrame(frame, off, f);
        check(ok && std::string(f.payload.begin(), f.payload.end()) == "round-trip!",
              "frame encode/decode round-trip");
    }
```

- [ ] **Step 2: Build and run — verify it fails**

Run build + `--selftest`. Expected: `SELFTEST FAIL` — the frame checks `FAIL` (stubs return `{}` / `false`). `decodeFrame incomplete -> false` passes against the stub, which is fine.

- [ ] **Step 3: Implement the frame codec**

In `WebSocketClient.cpp`, replace `std::vector<uint8_t> encodeBinaryFrame(const std::vector<uint8_t>&, const uint8_t[4]) { return {}; }` with:
```cpp
std::vector<uint8_t> encodeBinaryFrame(const std::vector<uint8_t>& payload,
                                       const uint8_t mask[4]) {
    std::vector<uint8_t> f;
    f.push_back(0x82);  // FIN=1, opcode=0x2 (binary)
    size_t n = payload.size();
    if (n <= 125) {
        f.push_back(static_cast<uint8_t>(0x80 | n));        // MASK bit + 7-bit length
    } else if (n <= 0xFFFF) {
        f.push_back(0x80 | 126);
        f.push_back(static_cast<uint8_t>((n >> 8) & 0xFF));
        f.push_back(static_cast<uint8_t>(n & 0xFF));
    } else {
        f.push_back(0x80 | 127);
        for (int i = 7; i >= 0; --i)
            f.push_back(static_cast<uint8_t>((static_cast<uint64_t>(n) >> (8 * i)) & 0xFF));
    }
    for (int i = 0; i < 4; ++i) f.push_back(mask[i]);
    for (size_t i = 0; i < n; ++i)
        f.push_back(static_cast<uint8_t>(payload[i] ^ mask[i & 3]));
    return f;
}
```
Replace `bool decodeFrame(const std::vector<uint8_t>&, size_t&, Frame&) { return false; }` with:
```cpp
bool decodeFrame(const std::vector<uint8_t>& buf, size_t& offset, Frame& out) {
    size_t p = offset;
    if (buf.size() - p < 2) return false;
    uint8_t b0 = buf[p++];
    uint8_t b1 = buf[p++];
    bool fin = (b0 & 0x80) != 0;
    uint8_t opcode = b0 & 0x0F;
    bool masked = (b1 & 0x80) != 0;
    uint64_t len = b1 & 0x7F;
    if (len == 126) {
        if (buf.size() - p < 2) return false;
        len = (static_cast<uint64_t>(buf[p]) << 8) | buf[p + 1];
        p += 2;
    } else if (len == 127) {
        if (buf.size() - p < 8) return false;
        len = 0;
        for (int i = 0; i < 8; ++i) len = (len << 8) | buf[p + i];
        p += 8;
    }
    uint8_t mk[4] = {0, 0, 0, 0};
    if (masked) {
        if (buf.size() - p < 4) return false;
        for (int i = 0; i < 4; ++i) mk[i] = buf[p + i];
        p += 4;
    }
    if (buf.size() - p < len) return false;
    out.fin = fin;
    out.opcode = opcode;
    out.payload.resize(static_cast<size_t>(len));
    for (uint64_t i = 0; i < len; ++i) {
        uint8_t byte = buf[p + static_cast<size_t>(i)];
        out.payload[static_cast<size_t>(i)] =
            masked ? static_cast<uint8_t>(byte ^ mk[i & 3]) : byte;
    }
    p += static_cast<size_t>(len);
    offset = p;
    return true;
}
```

- [ ] **Step 4: Build and run — verify it passes**

Run build + `--selftest`. Expected: all Task 7 checks `ok`; `SELFTEST OK`.

- [ ] **Step 5: Commit**
```bash
git add OrikaClient/WebSocketClient.cpp OrikaClient/SelfTest.cpp
git commit -m "feat: WebSocket binary frame encode/decode"
```

---

## Task 8: TlsConnection (TCP + OpenSSL)

Network code — not unit-tested. Verified by a clean build here and by the integration test in Task 11.

**Files:**
- Modify: `OrikaClient\TlsConnection.cpp` (replace all stub bodies)

- [ ] **Step 1: Implement TlsConnection.cpp**

Replace the entire contents of `D:\VSCodeProject\OrikaClient\OrikaClient\TlsConnection.cpp` with:
```cpp
#include "TlsConnection.h"
#include <openssl/err.h>
#include <stdexcept>
#include <string>
#include <cstdio>

#pragma comment(lib, "ws2_32.lib")

namespace {
std::string sslErr() {
    unsigned long e = ERR_get_error();
    if (e == 0) return "(no OpenSSL error)";
    char buf[256] = {0};
    ERR_error_string_n(e, buf, sizeof(buf));
    return std::string(buf);
}
} // namespace

TlsConnection::~TlsConnection() { close(); }

void TlsConnection::connect(const std::string& host, int port,
                            bool verifyCert, int timeoutSec) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        throw std::runtime_error("WSAStartup failed");
    wsaInit_ = true;

    char portStr[16];
    std::snprintf(portStr, sizeof(portStr), "%d", port);

    addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* res = nullptr;
    if (getaddrinfo(host.c_str(), portStr, &hints, &res) != 0 || res == nullptr)
        throw std::runtime_error("Cannot resolve host " + host);

    sock_ = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock_ == INVALID_SOCKET) {
        freeaddrinfo(res);
        throw std::runtime_error("socket() failed");
    }
    if (::connect(sock_, res->ai_addr, static_cast<int>(res->ai_addrlen)) == SOCKET_ERROR) {
        freeaddrinfo(res);
        throw std::runtime_error("Cannot reach " + host + ":" + portStr +
                                 " (connection refused)");
    }
    freeaddrinfo(res);

    DWORD tv = static_cast<DWORD>(timeoutSec) * 1000;
    setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));
    setsockopt(sock_, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));

    ctx_ = SSL_CTX_new(TLS_client_method());
    if (ctx_ == nullptr)
        throw std::runtime_error("SSL_CTX_new failed: " + sslErr());
    SSL_CTX_set_verify(ctx_, verifyCert ? SSL_VERIFY_PEER : SSL_VERIFY_NONE, nullptr);
    if (verifyCert) SSL_CTX_set_default_verify_paths(ctx_);

    ssl_ = SSL_new(ctx_);
    if (ssl_ == nullptr)
        throw std::runtime_error("SSL_new failed: " + sslErr());
    SSL_set_fd(ssl_, static_cast<int>(sock_));
    SSL_set_tlsext_host_name(ssl_, host.c_str());  // SNI

    if (SSL_connect(ssl_) != 1)
        throw std::runtime_error("TLS handshake failed: " + sslErr());
}

int TlsConnection::read(char* buf, int len) {
    int n = SSL_read(ssl_, buf, len);
    if (n > 0) return n;
    int err = SSL_get_error(ssl_, n);
    if (err == SSL_ERROR_ZERO_RETURN) return 0;            // clean TLS close
    if (err == SSL_ERROR_SYSCALL || err == SSL_ERROR_WANT_READ)
        return 0;                                          // timeout / socket closed
    return -1;                                             // protocol error
}

int TlsConnection::write(const char* buf, int len) {
    return SSL_write(ssl_, buf, len);
}

void TlsConnection::close() {
    if (ssl_ != nullptr) {
        SSL_shutdown(ssl_);
        SSL_free(ssl_);
        ssl_ = nullptr;
    }
    if (ctx_ != nullptr) {
        SSL_CTX_free(ctx_);
        ctx_ = nullptr;
    }
    if (sock_ != INVALID_SOCKET) {
        closesocket(sock_);
        sock_ = INVALID_SOCKET;
    }
    if (wsaInit_) {
        WSACleanup();
        wsaInit_ = false;
    }
}
```

- [ ] **Step 2: Build — verify it compiles and links**

Run: `msbuild D:\VSCodeProject\OrikaClient\OrikaClient.sln -p:Configuration=Debug -p:Platform=x64 -v:m`
Expected: `Build succeeded`.

- [ ] **Step 3: Run the self-test — verify nothing regressed**

Run: `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe --selftest`
Expected: `SELFTEST OK` (all Task 2–7 checks still pass).

- [ ] **Step 4: Commit**
```bash
git add OrikaClient/TlsConnection.cpp
git commit -m "feat: TLS client connection over Winsock + OpenSSL"
```

---

## Task 9: WebSocketClient (handshake, send, receive)

Network code — verified by build here and by the integration test in Task 11.

**Files:**
- Modify: `OrikaClient\WebSocketClient.cpp` (replace the three `WebSocketClient` method stubs)

- [ ] **Step 1: Implement the WebSocketClient methods**

In `WebSocketClient.cpp`, add these includes directly below the existing `#include <openssl/evp.h>` line:
```cpp
#include <openssl/rand.h>
#include <stdexcept>
#include <string>
#include <cctype>
```
Then replace the three stub method bodies at the bottom of the file:
```cpp
WebSocketClient::WebSocketClient(TlsConnection& tls) : tls_(tls) {}

void WebSocketClient::handshake(const std::string&, int, const std::string&) {}

void WebSocketClient::sendBinary(const std::vector<uint8_t>&) {}

std::vector<uint8_t> WebSocketClient::receiveMessage(int) { return {}; }
```
with:
```cpp
WebSocketClient::WebSocketClient(TlsConnection& tls) : tls_(tls) {}

void WebSocketClient::handshake(const std::string& host, int port,
                                const std::string& path) {
    uint8_t keyBytes[16];
    RAND_bytes(keyBytes, sizeof(keyBytes));
    std::string key = ws::base64Encode(
        std::vector<uint8_t>(keyBytes, keyBytes + sizeof(keyBytes)));

    std::string req =
        "GET " + path + " HTTP/1.1\r\n"
        "Host: " + host + ":" + std::to_string(port) + "\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: " + key + "\r\n"
        "Sec-WebSocket-Version: 13\r\n\r\n";
    if (tls_.write(req.c_str(), static_cast<int>(req.size())) <= 0)
        throw std::runtime_error("WebSocket handshake rejected: request write failed");

    std::string resp;
    char buf[1024];
    while (resp.find("\r\n\r\n") == std::string::npos) {
        int n = tls_.read(buf, sizeof(buf));
        if (n <= 0)
            throw std::runtime_error("WebSocket handshake rejected: no response");
        resp.append(buf, n);
    }
    size_t hdrEnd = resp.find("\r\n\r\n") + 4;
    for (size_t i = hdrEnd; i < resp.size(); ++i)
        rx_.push_back(static_cast<uint8_t>(resp[i]));   // bytes past headers = frame stream

    if (resp.compare(0, 12, "HTTP/1.1 101") != 0)
        throw std::runtime_error("WebSocket handshake rejected: " +
                                 resp.substr(0, resp.find("\r\n")));

    std::string lower = resp;
    for (char& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    const std::string hdr = "sec-websocket-accept:";
    size_t ap = lower.find(hdr);
    if (ap == std::string::npos)
        throw std::runtime_error("WebSocket handshake rejected: no accept header");
    size_t vs = ap + hdr.size();
    size_t ve = resp.find("\r\n", vs);
    std::string accept = resp.substr(vs, ve - vs);
    while (!accept.empty() && (accept.front() == ' ' || accept.front() == '\t'))
        accept.erase(accept.begin());
    while (!accept.empty() && (accept.back() == ' ' || accept.back() == '\t' ||
                               accept.back() == '\r'))
        accept.pop_back();
    if (accept != ws::computeAcceptKey(key))
        throw std::runtime_error("WebSocket handshake rejected: bad Sec-WebSocket-Accept");
}

void WebSocketClient::sendBinary(const std::vector<uint8_t>& payload) {
    uint8_t mask[4];
    RAND_bytes(mask, sizeof(mask));
    std::vector<uint8_t> frame = ws::encodeBinaryFrame(payload, mask);
    if (tls_.write(reinterpret_cast<const char*>(frame.data()),
                   static_cast<int>(frame.size())) <= 0)
        throw std::runtime_error("Failed to send login frame");
}

std::vector<uint8_t> WebSocketClient::receiveMessage(int /*timeoutSec*/) {
    // The socket already carries SO_RCVTIMEO from TlsConnection::connect.
    std::vector<uint8_t> message;
    char buf[4096];
    for (;;) {
        size_t offset = 0;
        ws::Frame f;
        while (ws::decodeFrame(rx_, offset, f)) {
            rx_.erase(rx_.begin(), rx_.begin() + offset);
            offset = 0;
            if (f.opcode == 0x8)            // close
                return message;
            if (f.opcode == 0x9)            // ping — ignore
                continue;
            if (f.opcode == 0x0 || f.opcode == 0x1 || f.opcode == 0x2) {
                message.insert(message.end(), f.payload.begin(), f.payload.end());
                if (f.fin) return message;  // complete data message
            }
        }
        int n = tls_.read(buf, sizeof(buf));
        if (n <= 0) {
            if (!message.empty()) return message;
            throw std::runtime_error("Connected and sent, but no reply within timeout");
        }
        for (int i = 0; i < n; ++i)
            rx_.push_back(static_cast<uint8_t>(buf[i]));
    }
}
```

- [ ] **Step 2: Build — verify it compiles and links**

Run the Debug build command. Expected: `Build succeeded`.

- [ ] **Step 3: Run the self-test — verify nothing regressed**

Run: `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe --selftest`
Expected: `SELFTEST OK`.

- [ ] **Step 4: Commit**
```bash
git add OrikaClient/WebSocketClient.cpp
git commit -m "feat: WebSocket handshake, send, and receive"
```

---

## Task 10: main.cpp — CLI parsing and orchestration

**Files:**
- Modify: `OrikaClient\main.cpp` (full rewrite)

- [ ] **Step 1: Rewrite main.cpp**

Replace the entire contents of `D:\VSCodeProject\OrikaClient\OrikaClient\main.cpp` with:
```cpp
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

#include "SelfTest.h"
#include "TlsConnection.h"
#include "WebSocketClient.h"
#include "ProtoCodec.h"

namespace {

struct Options {
    std::string host = "localhost";
    int port = 85;
    std::string path = "/";
    std::string login = "1001";
    std::string pwd = "hello@12345";
    std::string serial;          // mandatory
    bool serialGiven = false;
    int timeout = 12;
    bool verifyCert = false;
};

void printUsage() {
    std::cout <<
        "orikaClient - OrikaServer wss login test client\n"
        "Usage: orikaClient.exe --serial <S> [options]\n"
        "  --host H        server host        (default localhost)\n"
        "  --port P        server port        (default 85)\n"
        "  --path PATH     WebSocket path     (default /)\n"
        "  --login L       user id            (default 1001)\n"
        "  --pwd P         password           (default hello@12345)\n"
        "  --serial S      device serial      (REQUIRED)\n"
        "  --timeout N     seconds            (default 12)\n"
        "  --verify-cert   enforce TLS certificate validation\n"
        "  --selftest      run offline codec self-checks and exit\n"
        "  --help          show this help\n"
        "Exit codes: 0 PASS, 1 FAIL, 2 usage error, 3 connection error, 4 no/bad reply\n";
}

// Returns true if parsing succeeded. On a usage problem, prints a message and
// sets exitCode to 2.
bool parseArgs(int argc, char** argv, Options& opt, int& exitCode) {
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto needValue = [&](const char* name) -> std::string {
            if (i + 1 >= argc)
                throw std::runtime_error(std::string("missing value for ") + name);
            return argv[++i];
        };
        if (a == "--host")            opt.host = needValue("--host");
        else if (a == "--port")       opt.port = std::atoi(needValue("--port").c_str());
        else if (a == "--path")       opt.path = needValue("--path");
        else if (a == "--login")      opt.login = needValue("--login");
        else if (a == "--pwd")        opt.pwd = needValue("--pwd");
        else if (a == "--serial")   { opt.serial = needValue("--serial"); opt.serialGiven = true; }
        else if (a == "--timeout")    opt.timeout = std::atoi(needValue("--timeout").c_str());
        else if (a == "--verify-cert") opt.verifyCert = true;
        else {
            std::cout << "Unknown argument: " << a << "\n\n";
            printUsage();
            exitCode = 2;
            return false;
        }
    }
    if (!opt.serialGiven) {
        std::cout << "Serial is required\n\n";
        printUsage();
        exitCode = 2;
        return false;
    }
    if (opt.port <= 0 || opt.port > 65535) {
        std::cout << "Invalid --port value\n";
        exitCode = 2;
        return false;
    }
    return true;
}

// Prints a hex dump of up to 64 bytes for diagnostics.
void hexDump(const std::vector<uint8_t>& data) {
    size_t n = data.size() < 64 ? data.size() : 64;
    std::cout << "  reply (" << data.size() << " bytes, first " << n << " shown): ";
    static const char* H = "0123456789abcdef";
    for (size_t i = 0; i < n; ++i) {
        std::cout << H[(data[i] >> 4) & 0xF] << H[data[i] & 0xF] << ' ';
    }
    std::cout << "\n";
}

} // namespace

int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--help") == 0) { printUsage(); return 0; }
        if (std::strcmp(argv[i], "--selftest") == 0) { return RunSelfTest() ? 0 : 1; }
    }

    Options opt;
    int exitCode = 0;
    if (!parseArgs(argc, argv, opt, exitCode))
        return exitCode;

    try {
        TlsConnection tls;
        tls.connect(opt.host, opt.port, opt.verifyCert, opt.timeout);
        std::cout << "connected\nTLS ok\n";

        WebSocketClient ws(tls);
        ws.handshake(opt.host, opt.port, opt.path);
        std::cout << "WebSocket upgraded\n";

        std::vector<uint8_t> loginMsg = proto::encodeLogin(opt.login, opt.pwd, opt.serial);
        ws.sendBinary(loginMsg);
        std::cout << "login sent\n";

        std::vector<uint8_t> reply = ws.receiveMessage(opt.timeout);
        std::cout << "reply received\n";

        proto::LoginStatus ls = proto::decodeLoginStatus(reply);
        if (!ls.ok) {
            std::cout << "Could not parse server response\n";
            hexDump(reply);
            return 4;
        }
        if (ls.status == "success") {
            std::cout << "PASS - login '" << opt.login << "' status: success";
            if (!ls.token.empty()) std::cout << " token: " << ls.token;
            std::cout << "\n";
            return 0;
        }
        std::cout << "FAIL - status: " << ls.status << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::string msg = e.what();
        std::cout << msg << "\n";
        // "no reply within timeout" is exit 4; all other thrown errors are exit 3.
        if (msg.find("no reply within timeout") != std::string::npos)
            return 4;
        return 3;
    }
}
```

- [ ] **Step 2: Build — verify it compiles and links**

Run the Debug build command. Expected: `Build succeeded`.

- [ ] **Step 3: Verify `--selftest`, `--help`, and the missing-serial path**

Run: `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe --selftest`
Expected: `SELFTEST OK`; exit 0.

Run: `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe`
Expected: prints `Serial is required` then usage; exit code 2 (check with `echo %ERRORLEVEL%` in cmd or `$LASTEXITCODE` in PowerShell).

- [ ] **Step 4: Commit**
```bash
git add OrikaClient/main.cpp
git commit -m "feat: CLI parsing and login round-trip orchestration"
```

---

## Task 11: Integration test and README

**Files:**
- Create: `D:\VSCodeProject\OrikaClient\README.md`

- [ ] **Step 1: Confirm an OrikaServer is listening**

Run (PowerShell): `Get-NetTCPConnection -LocalPort 85 -State Listen -ErrorAction SilentlyContinue`
Expected: one row owned by `OrikaServer`. If none, start OrikaServer and click **Start** in its dialog before continuing. (The listener is created by the Start button — see the OrikaServer dialog.)

- [ ] **Step 2: Run the integration cases**

Obtain a valid device serial from the DB table `orika_userLoginHDkeyMapping` (a row with `HDno='*'` is a wildcard that matches any device). Substitute it for `<SERIAL>` below.

| Case | Command | Expected output | Exit |
|------|---------|-----------------|------|
| Valid login | `orikaClient.exe --serial <SERIAL>` | `PASS - login '1001' status: success` | 0 |
| Wrong password | `orikaClient.exe --serial <SERIAL> --pwd wrongpw` | `FAIL - status: invalid credentials` | 1 |
| Bad serial | `orikaClient.exe --serial BOGUS-SERIAL` | `FAIL - status: invalid SerialNo` | 1 |
| Missing serial | `orikaClient.exe` | `Serial is required` + usage | 2 |
| Unreachable | `orikaClient.exe --serial <SERIAL> --port 65000` | `Cannot reach localhost:65000 (connection refused)` | 3 |

The exe is at `D:\VSCodeProject\OrikaClient\x64\Debug\orikaClient.exe`. Check the exit code with `$LASTEXITCODE` (PowerShell). Record any case whose output or exit code differs from the table — if a case fails, treat it with the `superpowers:systematic-debugging` skill before proceeding.

- [ ] **Step 3: Write README.md**

Create `D:\VSCodeProject\OrikaClient\README.md`:
```markdown
# orikaClient

A standalone C++ console tool that tests a running OrikaServer: it connects
over `wss://`, performs one `login` round-trip, and reports PASS/FAIL.

## Build

Open a *Developer PowerShell for VS 2022* and run:

```
msbuild D:\VSCodeProject\OrikaClient\OrikaClient.sln -p:Configuration=Debug -p:Platform=x64
```

Output: `x64\Debug\orikaClient.exe` (the OpenSSL DLLs are copied next to it).

## Usage

```
orikaClient.exe --serial <S> [options]
  --host H        server host        (default localhost)
  --port P        server port        (default 85)
  --path PATH     WebSocket path     (default /)
  --login L       user id            (default 1001)
  --pwd P         password           (default hello@12345)
  --serial S      device serial      (REQUIRED)
  --timeout N     seconds            (default 12)
  --verify-cert   enforce TLS certificate validation (off by default;
                  the server certificate fails chain validation)
  --selftest      run offline codec self-checks and exit
  --help          show this help
```

## Exit codes

| Code | Meaning |
|------|---------|
| 0 | PASS — login status is `success` |
| 1 | FAIL — server returned a non-success status |
| 2 | Usage error (e.g. `--serial` missing) |
| 3 | Connection / TLS / WebSocket-handshake failure |
| 4 | Connected and sent, but no reply, or reply unparseable |

## Test cases

| Case | Command | Expected | Exit |
|------|---------|----------|------|
| Valid login | `orikaClient.exe --serial <SERIAL>` | `PASS ... status: success` | 0 |
| Wrong password | `orikaClient.exe --serial <SERIAL> --pwd wrongpw` | `FAIL - status: invalid credentials` | 1 |
| Bad serial | `orikaClient.exe --serial BOGUS-SERIAL` | `FAIL - status: invalid SerialNo` | 1 |
| Missing serial | `orikaClient.exe` | `Serial is required` | 2 |
| Unreachable | `orikaClient.exe --serial <SERIAL> --port 65000` | connection error | 3 |

## Obtaining a valid `--serial`

The server validates `serialNo` against the SQL table
`orika_userLoginHDkeyMapping` **before** checking the password. A row with
`HDno='*'` is a wildcard that matches any device. Use a registered serial, or
add a wildcard row, for the valid-login case.

## Offline self-test

`orikaClient.exe --selftest` runs the protobuf, base64, accept-key, and
WebSocket-frame codec checks against golden vectors — no network, no server
required. Prints `SELFTEST OK` and exits 0 when all pass.
```

- [ ] **Step 4: Commit**
```bash
git add README.md
git commit -m "docs: README and integration test results"
```

---

## Self-Review

**Spec coverage:**
- Architecture (`TlsConnection`, `WebSocketClient`, `ProtoCodec`, `main.cpp`) — Tasks 8, 9, 2–7, 10. ✓
- Folder layout + vendored OpenSSL — Task 1. ✓
- TLS with cert verification off by default + `--verify-cert` — Task 8 (`connect`), Task 10 (flag). ✓
- WebSocket handshake + accept-key verification — Tasks 6, 9. ✓
- Client-masked binary frames + control-frame handling — Tasks 7, 9. ✓
- Hand-coded protobuf `encodeLogin` / `decodeLoginStatus` — Tasks 2–5. ✓
- CLI parameters, defaults, mandatory `--serial` — Task 10. ✓
- Output lines + exit codes 0/1/2/3/4 — Task 10. ✓
- `--selftest` with golden vectors, no test framework — SelfTest harness (Task 1) + checks (Tasks 2–7). ✓
- Five integration cases + README — Task 11. ✓

**Placeholder scan:** No `TBD`/`TODO`; every code step shows complete code; every command shows expected output. `<SERIAL>` in Task 11 is a deliberate user-supplied value, documented in Step 2 and the README.

**Type consistency:** `proto::LoginStatus{ok,type,status,token}`, `ws::Frame{fin,opcode,payload}`, and the signatures of `encodeVarint`/`decodeVarint`/`encodeLenDelimField`/`encodeLogin`/`decodeLoginStatus`/`base64Encode`/`computeAcceptKey`/`encodeBinaryFrame`/`decodeFrame` are declared in the Task 1 headers and used identically in every later task. `TlsConnection::connect(host,port,verifyCert,timeoutSec)` and `WebSocketClient(TlsConnection&)` match between header, implementation, and `main.cpp`.
