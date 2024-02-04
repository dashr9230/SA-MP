/*  Date/time module for the Pawn Abstract Machine
 *
 *  Copyright (c) ITB CompuPhase, 2001-2005
 *
 *  This software is provided "as-is", without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from
 *  the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1.  The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software in
 *      a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *  2.  Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *  3.  This notice may not be removed or altered from any source distribution.
 *
 *  Version: $Id: amxtime.c 3363 2005-07-23 09:03:29Z thiadmer $
 */
#include <time.h>
#include <assert.h>
#include "amx.h"
#if defined __WIN32__ || defined _WIN32
  #include <windows.h>
  #include <mmsystem.h>
#endif

#if !defined CLOCKS_PER_SEC
  #define CLOCKS_PER_SEC CLK_TCK
#endif
#if defined __WIN32__ || defined _WIN32 || defined WIN32
  static int timerset = 0;
  /* timeGetTime() is more accurate on WindowsNT if timeBeginPeriod(1) is set */
  #define INIT_TIMER()    \
    if (!timerset) {      \
      timeBeginPeriod(1); \
      timerset=1;         \
    }
#else
  #define INIT_TIMER()
#endif
static unsigned long timestamp;
static unsigned long timelimit;
static int timerepeat;

static unsigned long gettimestamp(void)
{
  unsigned long value;

  #if defined __WIN32__ || defined _WIN32 || defined WIN32
    value=timeGetTime();        /* this value is already in milliseconds */
  #else
    value=(cell)clock();
    #if CLOCKS_PER_SEC!=1000
      /* convert to milliseconds */
      value=(cell)((1000L * (value+CLOCKS_PER_SEC/2)) / CLOCKS_PER_SEC);
    #endif
  #endif
  return value;
}

/* settime(hour, minute, second)
 * Always returns 0
 */
static cell AMX_NATIVE_CALL n_settime(AMX *amx, cell *params)
{
  #if defined __WIN32__ || defined _WIN32 || defined WIN32
    SYSTEMTIME systim;

    GetLocalTime(&systim);
    if (params[1]>=0)
      systim.wHour=(WORD)params[1];
    if (params[2]>=0)
      systim.wMinute=(WORD)params[2];
    if (params[3]>=0)
      systim.wSecond=(WORD)params[3];
    SetLocalTime(&systim);
  #else
    /* Linux/Unix (and some DOS compilers) have stime(); on Linux/Unix, you
     * must have "root" permission to call stime()
     */
    time_t sec1970;
    struct tm gtm;

    (void)amx;
    time(&sec1970);
    gtm=*localtime(&sec1970);
    if (params[1]>=0)
      gtm.tm_hour=params[1];
    if (params[2]>=0)
      gtm.tm_min=params[2];
    if (params[3]>=0)
      gtm.tm_sec=params[3];
    sec1970=mktime(&gtm);
    stime(&sec1970);
  #endif
  (void)amx;
  return 0;
}

/* gettime(&hour, &minute, &second)
 * The return value is the number of seconds since 1 January 1970 (Unix system
 * time).
 */
static cell AMX_NATIVE_CALL n_gettime(AMX *amx, cell *params)
{
  time_t sec1970;
  struct tm gtm;
  cell *cptr;

  assert(params[0]==(int)(3*sizeof(cell)));

  time(&sec1970);

  /* on DOS/Windows, the timezone is usually not set for the C run-time
   * library; in that case gmtime() and localtime() return the same value
   */
  gtm=*localtime(&sec1970);
  if (amx_GetAddr(amx,params[1],&cptr)==AMX_ERR_NONE)
    *cptr=gtm.tm_hour;
  if (amx_GetAddr(amx,params[2],&cptr)==AMX_ERR_NONE)
    *cptr=gtm.tm_min;
  if (amx_GetAddr(amx,params[3],&cptr)==AMX_ERR_NONE)
    *cptr=gtm.tm_sec;

  /* the time() function returns the number of seconds since January 1 1970
   * in Universal Coordinated Time (the successor to Greenwich Mean Time)
   */
  return sec1970;
}

/* setdate(year, month, day)
 * Always returns 0
 */
static cell AMX_NATIVE_CALL n_setdate(AMX *amx, cell *params)
{
  #if defined __WIN32__ || defined _WIN32 || defined WIN32
    SYSTEMTIME systim;

    GetLocalTime(&systim);
    if (params[1]!=0)
      systim.wYear=(WORD)params[1];
    if (params[2]!=0)
      systim.wMonth=(WORD)params[2];
    if (params[3]!=0)
      systim.wDay=(WORD)params[3];
    SetLocalTime(&systim);
  #else
    /* Linux/Unix (and some DOS compilers) have stime(); on Linux/Unix, you
     * must have "root" permission to call stime()
     */
    time_t sec1970;
    struct tm gtm;

    (void)amx;
    time(&sec1970);
    gtm=*localtime(&sec1970);
    if (params[1]!=0)
      gtm.tm_year=params[1]-1900;
    if (params[2]!=0)
      gtm.tm_mon=params[2]-1;
    if (params[3]!=0)
      gtm.tm_mday=params[3];
    sec1970=mktime(&gtm);
    stime(&sec1970);
  #endif
  (void)amx;
  return 0;
}

/* getdate(&year, &month, &day)
 * The return value is the number of days since the start of the year. January
 * 1 is day 1 of the year.
 */
static cell AMX_NATIVE_CALL n_getdate(AMX *amx, cell *params)
{
  time_t sec1970;
  struct tm gtm;
  cell *cptr;

  assert(params[0]==(int)(3*sizeof(cell)));

  time(&sec1970);

  gtm=*localtime(&sec1970);
  if (amx_GetAddr(amx,params[1],&cptr)==AMX_ERR_NONE)
    *cptr=gtm.tm_year+1900;
  if (amx_GetAddr(amx,params[2],&cptr)==AMX_ERR_NONE)
    *cptr=gtm.tm_mon+1;
  if (amx_GetAddr(amx,params[3],&cptr)==AMX_ERR_NONE)
    *cptr=gtm.tm_mday;

  return gtm.tm_yday+1;
}

/* tickcount(&granularity)
 * Returns the number of milliseconds since start-up. For a 32-bit cell, this
 * count overflows after approximately 24 days of continuous operation.
 */
static cell AMX_NATIVE_CALL n_tickcount(AMX *amx, cell *params)
{
  cell *cptr;

  assert(params[0]==(int)sizeof(cell));

  INIT_TIMER();
  #if defined __WIN32__ || defined _WIN32 || defined WIN32
    if (amx_GetAddr(amx,params[1],&cptr)==AMX_ERR_NONE)
      *cptr=1000;               /* granularity = 1 ms */
  #else
    if (amx_GetAddr(amx,params[1],&cptr)==AMX_ERR_NONE)
      *cptr=(cell)CLOCKS_PER_SEC;       /* in Unix/Linux, this is often 100 */
  #endif
  return gettimestamp() & 0x7fffffff;
}

/* settimer(milliseconds, bool: singleshot = false)
 * Sets the delay until the @timer() callback is called. The timer may either
 * be single-shot or repetitive.
 */
static cell AMX_NATIVE_CALL n_settimer(AMX *amx, cell *params)
{
  (void)amx;
  assert(params[0]==(int)(2*sizeof(cell)));
  timestamp = gettimestamp();
  timelimit = params[1];
  timerepeat = (int)(params[2] == 0);
  return 0;
}


#if !defined AMXTIME_NOIDLE
static AMX_DEBUG PrevIdle = NULL;
static int idxTimer = -1;

static int AMXAPI amx_TimeIdle(AMX *amx)
{
  int err=0;

  assert(idxTimer >= 0);

  if (PrevIdle != NULL)
    PrevIdle(amx);

  if (timelimit>0 && (gettimestamp()-timestamp)>=timelimit) {
    if (timerepeat)
      timestamp+=timelimit;
    else
      timelimit=0;      /* do not repeat single-shot timer */
    err = amx_Exec(amx, NULL, idxTimer);
    while (err == AMX_ERR_SLEEP)
      err = amx_Exec(amx, NULL, AMX_EXEC_CONT);
  } /* if */

  return err;
}
#endif


#if defined __cplusplus
  extern "C"
#endif
const AMX_NATIVE_INFO time_Natives[] = {
  { "gettime",   n_gettime },
  { "settime",   n_settime },
  { "getdate",   n_getdate },
  { "setdate",   n_setdate },
  { "tickcount", n_tickcount },
  { "settimer",  n_settimer },
  { NULL, NULL }        /* terminator */
};

int AMXEXPORT amx_TimeInit(AMX *amx)
{
  #if !defined AMXTIME_NOIDLE
    /* see whether there is a @timer() function */
    if (amx_FindPublic(amx,"@timer",&idxTimer) == AMX_ERR_NONE) {
      if (amx_GetUserData(amx, AMX_USERTAG('I','d','l','e'), (void**)&PrevIdle) != AMX_ERR_NONE)
        PrevIdle = NULL;
      amx_SetUserData(amx, AMX_USERTAG('I','d','l','e'), amx_TimeIdle);
    } /* if */
  #endif

  return amx_Register(amx, time_Natives, -1);
}

int AMXEXPORT amx_TimeCleanup(AMX *amx)
{
  (void)amx;
  #if !defined AMXTIME_NOIDLE
    PrevIdle = NULL;
  #endif
  return AMX_ERR_NONE;
}
