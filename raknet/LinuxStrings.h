#ifndef _GCC_WIN_STRINGS
#define _GCC_WIN_STRINGS

#if (defined(__GNUC__)  || defined(__GCCXML__))
int _stricmp(const char* s1, const char* s2);
int _strnicmp(const char* s1, const char* s2, size_t n);
//#ifndef __CYGWIN__
//int _unlink(const char* filename);
//#endif

#endif
#endif
