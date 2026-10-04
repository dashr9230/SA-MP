# San Andreas Multiplayer

This is an ongoing work-in-progress decompilation of the latest version of San Andreas Multiplayer (SA-MP), a free Massively Multiplayer Online game mod for the PC version of Rockstar Games Grand Theft Auto: San Andreas.

The project aims to be a matching decompilation. Meaning, producing a source code base that, when it's recompiled with the same compiler the original developers used, outputs the exact same binary as the original one.

Decompiling and extracting the containing binaries from the following version packages:

| Name | MD5 |
| --- | --- |
| sa-mp-0.3.7-R5-1-install.exe | `f7874cc8637e5ddb98b07ed40a24de58` |
| samp037_svr_R3_win32.zip | `1f35cc9fbfd4db61e0f8fe1efce362ec` |
| samp037svr_R3.tar.gz | `901286f80f8a5ac9bd784f6091337f84` |

## Progress

<!-- DECOMPINFOSTART -->
| Name | Status | MD5 |
| --- | --- | --- |
| samp.exe | 0% (0 / 6614) | `74B837FA2245116229D076D6863E41BC` |
| samp_debug.exe | 0% (0 / 835) | `2C00C60A5511C3A41A70296FD1879067` |
| samp.dll | 7.0% (558 / 8021) | `5BA5F0BE7AF99DFD03FB39E88A970A2B` |
| rcon.exe | 100.0% (123 / 123) | `3F4821CDA1DE6D7D10654E5537B4DF6E` |
| samp-server.exe | 0% (0 / 3860) | `4CB042A55B8837EA42A5C44D1BEA5AC8` |
| samp03svr | 0% (0 / 4181) | `8EF11F89BFF946F0BBCDEC32F98CECEB` |
| announce.exe | 100.0% (133 / 133) | `B4FCC1C57C10B87427FA1AE3E02B70C6` |
| announce | 0% (0 / 74) | `F6E1475A7F69B648D7809A777271B73F` |
| samp-npc.exe | 0% (0 / 1465) | `27F10BE9A4ED09E4B102952BC5652F3E` |
| samp-npc | 0% (0 / 2618) | `DEDDC5AA62C0F6E144C5DF3B4FBAD50E` |
<!-- DECOMPINFOEND -->

Status values may not be 100% accurate and will fluctuate during the analyzation of the binary files. These numbers currently are based on the number of functions marked in light green color and the number of visible functions in the IDA Pro database, and this does not represent the overall byte-for-byte accuracy of the output file with the original one.

## Building

### Toolchain

The client library is built with **Microsoft Visual C++ 7.1 (`cl.exe` 13.10.3077)**, the compiler that ships
with Visual Studio .NET 2003. This is not a preference. A matching decompilation only reproduces the original
bytes if the original compiler produces them, and no later MSVC release generates the same code for this
source. Building with a modern toolset may well succeed and will not match.

Two flags worth knowing before you start: `/GS-` does not exist in VC7.1 (buffer security checks are off by
default, and `BufferSecurityCheck="FALSE"` in the project reflects that), and `__thiscall` is not a usable
keyword in this compiler version.

### On Windows

Open `saco.sln` in Visual Studio .NET 2003 and build the `Release` configuration of the `saco` project. The
output is `saco/Release/samp.dll`. The `Debug` configuration exists but produces nothing comparable to the
original binary.

Other components have their own solutions: `announce/announce.sln`, `arctool2/arctool2.sln`, `bot/bot.sln`
and `server/server.sln`.

### On Linux

VC7.1 runs well under Wine. Install Visual Studio .NET 2003 into a Wine prefix, then invoke `cl.exe`
directly. Two things are easy to get wrong: the working directory has to be `saco/`, and `INCLUDE` has to be
set explicitly, because the Wine environment does not inherit the compiler's own registry settings.

```sh
export WINEPREFIX="$HOME/.wine"
VC='C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7'
export INCLUDE="$VC\\include;$VC\\PlatformSDK\\Include"

cd saco
wine "$VC\\bin\\cl.exe" /c /Ox /Og /Ob1 /Oi /Ot /Oy /MT /Zp1 /EHsc /GF \
  /D WIN32 /D NDEBUG /D _WINDOWS /D _USRDLL /D SACO_EXPORTS /D SAMPCLI \
  /I "d3d9\\include" /I "." net\\netgame.cpp /Fo"Z:\\tmp\\netgame.obj"
```

### Release settings

These are the settings the `Release` configuration in `saco/saco.vcproj` expands to. They matter because
changing any of them changes the generated code, so a function that matched before may stop matching.

| Flag | Project property |
| --- | --- |
| `/Ox` | `Optimization="3"` |
| `/Og` | `GlobalOptimizations="TRUE"` |
| `/Ob1` | `InlineFunctionExpansion="1"` |
| `/Oi` | `EnableIntrinsicFunctions="TRUE"` |
| `/Ot` | `FavorSizeOrSpeed="1"` |
| `/Oy` | `OmitFramePointers="TRUE"` |
| `/GF` | `StringPooling="TRUE"` |
| `/EHsc` | `ExceptionHandling="TRUE"` |
| `/MT` | `RuntimeLibrary="0"` |
| `/Zp1` | `StructMemberAlignment="1"` |

Preprocessor definitions are `WIN32`, `NDEBUG`, `_WINDOWS`, `_USRDLL`, `SACO_EXPORTS` and `SAMPCLI`, and the
only additional include directory is `d3d9\include`.

Five files carry per-file overrides, and compiling them with the defaults above will not match:

| File | Override |
| --- | --- |
| `game/entity.cpp` | `/O2` |
| `game/keystuff.cpp` | `/O2` |
| `game/patches.cpp` | `/O2` |
| `httpclient.cpp` | `/O2` |
| `d3d9/dxutil.cpp` | `/Ob2` |

The link is `/SUBSYSTEM:WINDOWS /MACHINE:X86` against `dxguid.lib`, `d3d9.lib`, `d3dx9.lib`, `comctl32.lib`,
`wsock32.lib`, `winmm.lib` and `bass.lib`, with `d3d9` as an additional library directory. Both
`/OPT:NOREF` and `/OPT:NOICF` are in effect (`OptimizeReferences="0"` and `EnableCOMDATFolding="0"`), so the
linker neither strips unreferenced functions nor folds identical ones. Identical functions therefore appear
separately in the original binary, which is why byte-identical siblings are expected rather than suspicious.

### Current state of the build

`samp.dll` does not link yet. `raknet/SocketDataEncryptor.cpp` is referenced by the project but is not
present in the tree, and it is the only such file. Every other translation unit the project lists compiles
cleanly.

Because of that, progress is measured per function rather than per binary. A function counts as done when the
bytes the compiler emits for it are identical to the bytes at the corresponding address in the original
`samp.dll`, after masking the four-byte slots that COFF marks as relocations. Masking is what makes an
unlinked object file comparable to a linked binary: call targets, global addresses and string addresses are
resolved at link time and cannot agree, while every other byte must.

