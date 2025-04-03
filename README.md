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
| samp.dll | 0.2% (14 / 6366) | `5BA5F0BE7AF99DFD03FB39E88A970A2B` |
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

TODO
