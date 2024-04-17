
#pragma once

//----------------------------------------------------------

unsigned int CalcCRC32(unsigned int crc, const void *buf, size_t size);
unsigned int CalcFileCRC32(char *filename);

void CalcFileSHA1(char *filename, unsigned int *digest);
void CalcSHA1(char *data, unsigned int len, unsigned int *digest);
char *GetSHA1Result(unsigned int *digest, char *buf);

//----------------------------------------------------------
