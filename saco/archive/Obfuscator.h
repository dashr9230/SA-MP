
#pragma once

#define OBFUSCATE_KEY		0xC103D3E7
#define OBFUSCATE_DATA(a)	((((a) << 19) | ((a) >> 13)) ^ OBFUSCATE_KEY)
#define UNOBFUSCATE_DATA(a) ((((a) ^ OBFUSCATE_KEY) >> 19) | (((a) ^ OBFUSCATE_KEY) << 13))
