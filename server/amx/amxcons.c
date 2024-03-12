/* Console output module (terminal I/O) for the Pawn AMX
 *
 *  Since some of these routines go further than those of standard C, they
 *  cannot always be implemented with portable C functions. In other words,
 *  these routines must be ported to other environments.
 *
 *  Copyright (c) ITB CompuPhase, 1997-2005
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
 *  Version: $Id: amxcons.c 3363 2005-07-23 09:03:29Z thiadmer $
 */

#if defined _UNICODE || defined __UNICODE__ || defined UNICODE
# if !defined UNICODE   /* for Windows */
#   define UNICODE
# endif
# if !defined _UNICODE  /* for C library */
#   define _UNICODE
# endif
#endif

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#if defined __WIN32__ || defined _WIN32 || defined WIN32 || defined __MSDOS__
  #include <conio.h>
  #include <malloc.h>
#endif
#if defined USE_CURSES
  #include <curses.h>
#endif
#include "amx.h"
#if defined __WIN32__ || defined _WIN32 || defined WIN32 || defined __MSDOS__
  #include <windows.h>
#endif

#if defined _UNICODE
# include <tchar.h>
#elif !defined __T
  typedef char          TCHAR;
# define __T(string)    string
# define _tcschr        strchr
# define _tcscpy        strcpy
# define _tcsdup        strdup
# define _tcslen        strlen
# define _stprintf      sprintf
#endif

#if defined __MSDOS__
  #define EOL_CHAR       '\r'
#endif
#if defined __WIN32__ || defined _WIN32 || defined WIN32
  #define EOL_CHAR       '\r'
#endif
#if !defined EOL_CHAR
  /* if not a "known" operating system, assume Linux/Unix */
  #define EOL_CHAR     '\n'
#endif

#if defined AMX_TERMINAL
  /* required functions are implemented elsewhere */
  int amx_putstr(TCHAR *);
  int amx_putchar(int);
  int amx_fflush(void);
  int amx_getch(void);
  TCHAR *amx_gets(TCHAR *,int);
  int amx_termctl(int,int);
  void amx_clrscr(void);
  void amx_clreol(void);
  void amx_gotoxy(int x,int y);
  void amx_wherexy(int *x,int *y);
  unsigned int amx_setattr(int foregr,int backgr,int highlight);
  void amx_console(int columns, int lines, int flags);
#elif defined VT100 || defined LINUX || defined ANSITERM
  /* ANSI/VT100 terminal, or shell emulating "xterm" */
  #define amx_putstr(s)   printf("%s",(s))
  #define amx_putchar(c)  putchar(c)
  #define amx_fflush()    fflush(stdout)
  #define amx_getch()     getch()
  #define amx_gets(s,n)   fgets(s,n,stdin)

  int amx_termctl(int code,int value)
  {
    switch (code) {
    case 0:             /* query terminal support */
      return 1;

    case 1:             /* switch "auto-wrap" on or off */
      if (value)
        amx_putstr("\033[?7h"); /* enable "auto-wrap" */
      else
        amx_putstr("\033[?7l"); /* disable "auto-wrap" */
      return 1;

    #if 0
      /* next to swapping buffers, more information should be saved and swapped,
       * such as the cursor position and the current terminal attributes
       */
    case 2:             /* swap console buffers */
      amx_fflush();
      if (value==1) {
        amx_putstr("\033[?47h");
      } else {
        amx_putstr("\033[?47l");
      } /* if */
      amx_fflush();
      return 1;
    #endif

    case 3:             /* set bold/highlighted font */
      return 0;

    default:
      return 0;
    } /* switch */
  }
  void amx_clrscr(void)
  {
    amx_putstr("\033[2J");
    amx_fflush();        /* pump through the terminal codes */
  }
  void amx_clreol(void)
  {
    amx_putstr("\033[K");
    amx_fflush();        /* pump through the terminal codes */
  }
  void amx_gotoxy(int x,int y)
  {
    char str[30];
    _stprintf(str,"\033[%d;%dH",y,x);
    amx_putstr(str);
    amx_fflush();        /* pump through the terminal codes */
  }
  void amx_wherexy(int *x,int *y)
  {
    int val,i;
    char str[10];

    assert(x!=NULL && y!=NULL);
    amx_putstr("\033[6n");
    amx_fflush();
    while (amx_getch()!='\033')
      /* nothing */;
    val=amx_getch();
    assert(val=='[');
    for (i=0; i<8 && (val=amx_getch())!=';'; i++)
      str[i]=(char)val;
    str[i]='\0';
    *y=atoi(str);
    for (i=0; i<8 && (val=amx_getch())!='R'; i++)
      str[i]=(char)val;
    str[i]='\0';
    *x=atoi(str);
    #if defined ANSITERM
      val=amx_getch();
      assert(val=='\r');    /* ANSI driver adds CR to the end of the command */
    #endif
  }
  unsigned int amx_setattr(int foregr,int backgr,int highlight)
  {
    static short current=(0 << 8) | 7;
    short prev = current;
    char str[30];

    if (foregr>=0) {
      _stprintf(str,"\x1b[%dm",foregr+30);
      amx_putstr(str);
      current=(current & 0xff00) | (foregr & 0x0f);
    } /* if */
    if (backgr>=0) {
      _stprintf(str,"\x1b[%dm",backgr+40);
      amx_putstr(str);
      current=(current & 0x00ff) | ((backgr & 0x0f) << 8);
    } /* if */
    if (highlight>=0) {
      _stprintf(str,"\x1b[%dm",highlight);
      amx_putstr(str);
      current=(current & 0x7fff) | ((highlight & 0x01) << 15);
    } /* if */
    return prev;
  }
  void amx_console(int columns, int lines, int flags)
  {
    char str[30];

    (void)flags;
    /* There is no ANSI code (or VT100/VT220) to set the size of the console
     * (indeed, the terminal was that of the alphanumeric display). In xterm (a
     * terminal emulator) we can set the terminal size though, and most
     * terminals that in use today are in fact emulators.
     * Putty understands this code too, by many others do not.
     */
    sprintf(str,"\033[8;%d;%dt",lines,columns);
    amx_putstr(str);
    amx_fflush();
  }
#elif defined __WIN32__ || defined _WIN32 || defined WIN32
  /* Win32 console */
  #define amx_putstr(s)   printf("%s",(s))
  #define amx_putchar(c)  putchar(c)
  #define amx_fflush()    fflush(stdout)
  #define amx_getch()     getch()
  #define amx_gets(s,n)   fgets(s,n,stdin)

  int amx_termctl(int code,int value)
  {
    switch (code) {
    case 0:             /* query terminal support */
      return 1;

    case 1: {           /* switch auto-wrap on/off */
      /* only works in Windows 2000/XP */
      HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
      DWORD Flags=ENABLE_PROCESSED_OUTPUT;
      if (value)
        Flags |= ENABLE_WRAP_AT_EOL_OUTPUT;
      SetConsoleMode(hConsole,Flags);
      return 1;
    } /* case */

    /* case 2: */     /* create/switch to another console */
    /* case 3: */     /* set emphasized font */
    /* case 4: */     /* query whether a terminal is "open" */
    default:
      return 0;
    } /* switch */
  }
  void amx_clrscr(void)
  {
    COORD coordScreen={0,0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

    amx_fflush();       /* make sure the I/O buffer is empty */
    GetConsoleScreenBufferInfo(hConsole,&csbi);
    dwConSize=csbi.dwSize.X*csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole,' ',dwConSize,coordScreen,&cCharsWritten);
    FillConsoleOutputAttribute(hConsole,csbi.wAttributes,dwConSize,coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole,coordScreen);
  }
  void amx_clreol(void)
  {
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

    amx_fflush();       /* make sure all output is written */
    GetConsoleScreenBufferInfo(hConsole,&csbi);
    dwConSize=csbi.dwSize.X - csbi.dwCursorPosition.X;
    FillConsoleOutputCharacter(hConsole,' ',dwConSize,csbi.dwCursorPosition,&cCharsWritten);
    FillConsoleOutputAttribute(hConsole,csbi.wAttributes,dwConSize,csbi.dwCursorPosition,&cCharsWritten);
  }
  void amx_gotoxy(int x,int y)
  {
    COORD point;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    if (x<=0 || x>csbi.dwSize.X || y<=0 || y>csbi.dwSize.Y)
      return;
    amx_fflush();       /* make sure all output is written */
    point.X=(short)(x-1);
    point.Y=(short)(y-1);
    SetConsoleCursorPosition(hConsole,point);
  }
  void amx_wherexy(int *x,int *y)
  {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    amx_fflush();       /* make sure all output is written */
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    if (x!=NULL)
      *x=csbi.dwCursorPosition.X+1;
    if (y!=NULL)
      *y=csbi.dwCursorPosition.Y+1;
  }
  unsigned int amx_setattr(int foregr,int backgr,int highlight)
  {
    static int ansi_colours[] = { 0, 4, 2, 6, 1, 5, 3, 7 };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int f,b,h,prev;
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

    amx_fflush();       /* make sure all output is written */
    GetConsoleScreenBufferInfo(hConsole,&csbi);
    f=csbi.wAttributes & 0x07;
    b=(csbi.wAttributes >> 4) & 0x0f;
    h=(csbi.wAttributes & 0x08) ? 1 : 0;
    prev=(b << 8) | f | (h << 15);
    if (foregr>=0 && foregr<8)
      f=ansi_colours[foregr];
    if (backgr>=0 && backgr<8)
      b=ansi_colours[backgr];
    if (highlight>=0)
      h=highlight!=0;
    SetConsoleTextAttribute(hConsole, (WORD)((b << 4) | f | (h << 3)));
    return prev;
  }
  void amx_console(int columns, int lines, int flags)
  {
    SMALL_RECT rect;
    COORD dwSize;
    (void)flags;
    dwSize.X=(short)columns;
    dwSize.Y=(short)lines;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),dwSize);
    rect.Left=0;
    rect.Top=0;
    rect.Right=(short)(columns-1);
    rect.Bottom=(short)(lines-1);
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE),TRUE,&rect);
  }
#elif defined(USE_CURSES)
  /* Use the "curses" library to implement the console */
  #define amx_putstr(s)       printw("%s",(s))
  #define amx_putchar(c)      addch(c)
  #define amx_fflush()        (0)
  #define amx_getch()         getch()
  #define amx_gets(s,n)       getnstr(s,n)
  #define amx_clrscr()        clear()
  #define amx_clreol()        clrtoeol()
  #define amx_gotoxy(x,y)     (void)(0)
  #define amx_wherexy(x,y)    (*(x)=*(y)=0)
  #define amx_setattr(c,b,h)  (0)
  #define amx_termsup(c,v)    (0)
  #define amx_console(c,l,f)  (void)(0)
#else
  /* assume a streaming terminal; limited features (no colour, no cursor
   * control)
   */
  #define amx_putstr(s)       printf("%s",(s))
  #define amx_putchar(c)      putchar(c)
  #define amx_fflush()        fflush(stdout)
  #define amx_getch()         getch()
  #define amx_gets(s,n)       fgets(s,n,stdin)
  #define amx_clrscr()        (void)(0)
  #define amx_clreol()        (void)(0)
  #define amx_gotoxy(x,y)     (void)(0)
  #define amx_wherexy(x,y)    (*(x)=*(y)=0)
  #define amx_setattr(c,b,h)  (0)
  #define amx_termsup(c,v)    (0)
  #define amx_console(c,l,f)  (void)(0)
#endif

#if !defined AMX_TERMINAL && (defined __WIN32__ || defined _WIN32 || defined WIN32)
  void CreateConsole(void)
  { static int createdconsole=0;
    if (!createdconsole) {
  	  AllocConsole();
  	  createdconsole=1;
  	} /* if */
  }
#elif defined(USE_CURSES)
  // The Mac OS X build variant uses curses.
  void CreateConsole(void)
  { static int createdconsole=0;
    if (!createdconsole) {
  	  initscr();
      cbreak();
      noecho();
      nonl();
      intrflush(stdscr, FALSE);
      keypad(stdscr, TRUE);

  	  createdconsole=1;
  	} /* if */
  }
#else
  #define CreateConsole()
#endif

static int printstring(AMX *amx,cell *cstr,cell *params,int num);

enum {
  SV_DECIMAL,
  SV_HEX
};

static TCHAR *reverse(TCHAR *string,int stop)
{
	int start=0;
	TCHAR temp;

	/* swap the string */
	stop--;				/* avoid swapping the '\0' byte to the first position */
	while (stop - start > 0) {
		temp = string[start];
		string[start] = string[stop];
		string[stop] = temp;
		start++;
		stop--;
	} /* while */
	return string;
}

/* Converts an integral value to a string, with optional padding with spaces or
 * zeros.
 * The "format" must be decimal or hexadecimal
 * The number is right-aligned in the field with the size of the absolute value
 * of the "width" parameter.
 * If the width value is positive, the string is padded with spaces; if it is
 * negative, it is padded with zeros.
 */
static TCHAR *strval(TCHAR buffer[], long value, int format, int width)
{
	int start, stop;
	TCHAR temp;

	start = stop = 0;
	if (format == SV_DECIMAL) {
		if (value < 0) {
			buffer[0] = __T('-');
			start = stop = 1;
			value = -value;
		} /* if */
		do {
			buffer[stop++] = (TCHAR)((value % 10) + __T('0'));
			value /= 10;
		} while (value > 0);
	} else {
		/* hexadecimal */
		unsigned long v = (unsigned long)value;	/* copy to unsigned value for shifting */
		do {
			buffer[stop] = (TCHAR)((v & 0x0f) + __T('0'));
			if (buffer[stop] > __T('9'))
				buffer[stop] += (TCHAR)(__T('A') - __T('0') - 10);
			v >>= 4;
			stop++;
		} while (v != 0);
	} /* if */

	/* pad to given width */
	if (width < 0) {
		temp = __T('0');
		width = -width;
	} else {
		temp = __T(' ');
	} /* if */
	while (stop < width)
		buffer[stop++] = temp;

	buffer[stop] = __T('\0');

	/* swap the string, and we are done */
	reverse(buffer+start,stop-start);
	return buffer;
}

#if defined FIXEDPOINT
  #define FIXEDMULT     1000
  #define FIXEDDIGITS   3

static TCHAR *formatfixed(TCHAR *string,cell value,char align,int width,char decpoint,int digits)
{
  int i, len;
  cell ipart,v;
  TCHAR vsign=__T('\0');

  /* make the value positive (but keep the sign) */
  if (value<0) {
    value=-value;
    vsign='-';
  } /* if */

  /* "prepare" the value so that when it is truncated to the requested
   * number of digits, the result is rounded towards the dropped digits
   */
  v=FIXEDMULT/2;
  for (i=0; i<digits; i++)
    v/=10;
  value+=v;

  /* get the integer part and remove it from the value */
  ipart=value/FIXEDMULT;
  value-=FIXEDMULT*ipart;
  assert(ipart>=0);
  assert(value>=0);

  /* truncate the fractional part to the requested number of digits */
  for (i=FIXEDDIGITS; i>digits; i--)
    value/=10;

  string[0]='\0';

  /* add sign */
  i=_tcslen(string);
  string[i]=vsign;
  string[i+1]='\0';

  /* add integer part */
  strval(string+_tcslen(string),(long)ipart,SV_DECIMAL,0);

  /* add fractional part */
  if (digits>0) {
    i=_tcslen(string);
    string[i]=decpoint;
    strval(string+i+1,(long)value,SV_DECIMAL,-digits);
  } /* if */

  len=_tcslen(string);
  if (len<width) {
    /* pad to the requested width */
    for (i=len; i<width; i++)
      string[i]=__T(' ');
    string[i]=__T('\0');
    /* optionally move the padding to the beginning of the string, using the handwaving algorithm */
    if (align!='-') {
      assert(i==(int)_tcslen(string));
      assert(i>=len);
      reverse(string,len);
      reverse(string+len,i-len);
      reverse(string,i);
    } /* if */
  } /* if */

  return string;
}
#endif

static int dochar(AMX *amx,TCHAR ch,cell param,char sign,char decpoint,int width,int digits)
{
  cell *cptr;
  TCHAR buffer[40];
  #if defined FLOATPOINT
    TCHAR formatstring[40];
  #endif

  #if !defined FIXEDPOINT && !defined FLOATPOINT
    (void)decpoint;
    (void)digits;
  #endif

  switch (ch) {
  case '%':
    amx_putchar(ch);
    return 0;

  case 'c':
    amx_GetAddr(amx,param,&cptr);
    width--;            /* single character itself has a with of 1 */
    if (sign!='-')
      while (width-->0)
        amx_putchar(__T(' '));
    amx_putchar((TCHAR)*cptr);
    while (width-->0)
      amx_putchar(__T(' '));
    return 1;

  case 'd': {
    cell value;
    int length=1;
    amx_GetAddr(amx,param,&cptr);
    value=*cptr;
    if (value<0 || sign=='+') {
      length++;
      value=-value;
    } /* if */
    while (value>=10) {
      length++;
      value/=10;
    } /* while */
    width-=length;
    if (sign!='-')
      while (width-->0)
        amx_putchar(__T(' '));
    strval(buffer,*cptr,SV_DECIMAL,0);
    amx_putstr(buffer);
    while (width-->0)
      amx_putchar(__T(' '));
    return 1;
  } /* case */

#if defined FLOATPOINT
  case 'f': /* 32-bit floating point number */
  case 'r': /* if floating point is enabled, %r == %f */
    /* build a format string */
    _tcscpy(formatstring,__T("%"));
    if (sign!='\0')
      _stprintf(formatstring+_tcslen(formatstring),__T("%c"),sign);
    if (width>0)
      _stprintf(formatstring+_tcslen(formatstring),__T("%d"),width);
    _stprintf(formatstring+_tcslen(formatstring),__T(".%df"),digits);
    /* ??? decimal comma? */
    amx_GetAddr(amx,param,&cptr);
    _stprintf(buffer,formatstring,*(float*)cptr);
    amx_putstr(buffer);
    return 1;
#endif

#if defined FIXEDPOINT
  #define FIXEDMULT 1000
  case 'q': /* 32-bit fixed point number */
#if !defined FLOATPOINT
  case 'r': /* if fixed point is enabled, and floating point is not, %r == %q */
#endif
    amx_GetAddr(amx,param,&cptr);
    /* format the number */
    formatfixed(buffer,*cptr,sign,width,decpoint,digits);
    assert(_tcslen(buffer)<sizeof buffer);
    amx_putstr(buffer);
    return 1;
#endif

#if !defined FLOATPOINT && !defined FIXEDPOINT
  case 'f':
  case 'q':
  case 'r':
    amx_putstr(__T("(no rational number support)"));
    return 0; /* flag this as an error */
#endif

  case 's':
    amx_GetAddr(amx,param,&cptr);
    printstring(amx,cptr,NULL,0);
    return 1;

  case 'x': {
    ucell value;
    int length=1;
    amx_GetAddr(amx,param,&cptr);
    value=*(ucell*)cptr;
    while (value>=0x10) {
      length++;
      value>>=4;
    } /* while */
    width-=length;
    if (sign!='-')
      while (width-->0)
        amx_putchar(__T(' '));
    strval(buffer,(long)*cptr,SV_HEX,0);
    amx_putstr(buffer);
    while (width-->0)
      amx_putchar(__T(' '));
    return 1;
  } /* case */

  } /* switch */
  /* error in the string format, try to repair */
  amx_putchar(ch);
  return 0;
}

enum {
  FMT_NONE,   /* not in format state; accept '%' */
  FMT_START,  /* found '%', accept '+', ' ' (START), digit (WIDTH), '.' (DECIM) or format letter (done) */
  FMT_WIDTH,  /* found digit after '%' or sign, accept digit (WIDTH), '.' (DECIM) or format letter (done) */
  FMT_DECIM,  /* found digit after '.', accept accept digit (DECIM) or format letter (done) */
};

static int formatstate(TCHAR c,int *state,char *sign,char *decpoint,int *width,int *digits)
{
  switch (*state) {
  case FMT_NONE:
    if (c=='%') {
      *state=FMT_START;
      *sign='\0';
      *decpoint='.';
      *width=0;
      *digits=3;
    } else {
      return -1;  /* print a single character */
    } /* if */
    break;
  case FMT_START:
    if (c=='+' || c==' ') {
      *sign=' ';
    } else if (c>='0' && c<='9') {
      *width=(int)(c-'0');
      *state=FMT_WIDTH;
    } else if (c=='.' || c==',') {
      *decpoint=c;
      *digits=0;
      *state=FMT_DECIM;
    } else {
      return 1; /* print formatted character */
    } /* if */
    break;
  case FMT_WIDTH:
    if (c>='0' && c<='9') {
      *width=*width*10+(int)(c-'0');
    } else if (c=='.' || c==',') {
      *decpoint=c;
      *digits=0;
      *state=FMT_DECIM;
    } else {
      return 1; /* print formatted character */
    } /* if */
    break;
  case FMT_DECIM:
    if (c>='0' && c<='9') {
      *digits=*digits*10+(int)(c-'0');
    } else {
      return 1; /* print formatted character */
    } /* if */
    break;
  } /* switch */

  return 0;
}

static int printstring(AMX *amx,cell *cstr,cell *params,int num)
{
  int i,paramidx=0;
  int fmtstate=FMT_NONE,width,digits;
  char sign,decpoint;

  /* if no placeholders appear, we can use a quicker routine */
  if (params==NULL) {

    TCHAR cache[100];
    int idx=0;
    if ((ucell)*cstr>UNPACKEDMAX) {
      int j=sizeof(cell)-sizeof(char);
      char c;
      /* the string is packed */
      i=0;
      for ( ; ; ) {
        c=(char)((ucell)cstr[i] >> 8*j);
        if (c==0)
          break;
        assert(idx<sizeof cache);
        cache[idx++]=c;
        if (idx==sizeof cache - 1) {
          cache[idx]='\0';
          amx_putstr(cache);
          idx=0;
        } /* if */
        if (j==0)
          i++;
        j=(j+sizeof(cell)-sizeof(char)) % sizeof(cell);
      } /* for */
    } else {
      for (i=0; cstr[i]!=0; i++) {
        assert(idx<sizeof cache);
        cache[idx++]=(TCHAR)cstr[i];
        if (idx==sizeof cache - 1) {
          cache[idx]='\0';
          amx_putstr(cache);
          idx=0;
        } /* if */
      } /* for */
    } /* if */
    if (idx>0) {
      cache[idx]='\0';
      amx_putstr(cache);
    } /* if */

  } else {

    /* check whether this is a packed string */
    if ((ucell)*cstr>UNPACKEDMAX) {
      int j=sizeof(cell)-sizeof(char);
      char c;
      /* the string is packed */
      i=0;
      for ( ; ; ) {
        c=(char)((ucell)cstr[i] >> 8*j);
        if (c==0)
          break;
        switch (formatstate(c,&fmtstate,&sign,&decpoint,&width,&digits)) {
        case -1:
          amx_putchar(c);
          break;
        case 1:
          assert(params!=NULL);
          if (digits>25)
            digits=25;
          paramidx+=dochar(amx,c,params[paramidx],sign,decpoint,width,digits);
          fmtstate=FMT_NONE;
          break;
        case 0:
          if (params==NULL || paramidx>=num)  /* insufficient parameters passed */
            amx_RaiseError(amx, AMX_ERR_NATIVE);
          break;
        } /* switch */
        if (j==0)
          i++;
        j=(j+sizeof(cell)-sizeof(char)) % sizeof(cell);
      } /* for */
    } else {
      /* the string is unpacked */
      for (i=0; cstr[i]!=0; i++) {
        switch (formatstate((TCHAR)cstr[i],&fmtstate,&sign,&decpoint,&width,&digits)) {
        case -1:
          amx_putchar((TCHAR)cstr[i]);
          break;
        case 1:
          assert(params!=NULL);
          paramidx+=dochar(amx,(TCHAR)cstr[i],params[paramidx],sign,decpoint,width,digits);
          fmtstate=FMT_NONE;
          break;
        case 0:
          if (params==NULL || paramidx>=num)  /* insufficient parameters passed */
            amx_RaiseError(amx, AMX_ERR_NATIVE);
          break;
        } /* switch */
      } /* for */
    } /* if */

  } /* if (params==NULL) */

  return paramidx;
}

static cell AMX_NATIVE_CALL n_print(AMX *amx,cell *params)
{
  cell *cstr;
  int oldcolours;

  CreateConsole();

  /* set the new colours */
  oldcolours=amx_setattr((int)params[2],(int)params[3],(int)params[4]);

  amx_GetAddr(amx,params[1],&cstr);
  printstring(amx,cstr,NULL,0);

  /* reset the colours */
  (void)amx_setattr(oldcolours & 0xff,(oldcolours >> 8) & 0x7f,(oldcolours >> 15) & 0x01);
  amx_fflush();
  return 0;
}

static cell AMX_NATIVE_CALL n_printf(AMX *amx,cell *params)
{
  cell *cstr;

  CreateConsole();
  amx_GetAddr(amx,params[1],&cstr);
  printstring(amx,cstr,params+2,(int)(params[0]/sizeof(cell))-1);
  amx_fflush();
  return 0;
}

/* getchar(bool:echo=true) */
static cell AMX_NATIVE_CALL n_getchar(AMX *amx,cell *params)
{
  int c;

  (void)amx;
  CreateConsole();
  c=amx_getch();
  if (params[1]) {
    #if defined(SUPPRESS_ECHO)
 	  /* For Mac OS X, non-Curses, don't echo the character */
    #else
      amx_putchar((TCHAR)c);
      amx_fflush();
    #endif
  } /* if */
  return c;
}

/* getstring(string[], size=sizeof string, bool:pack=false) */
static cell AMX_NATIVE_CALL n_getstring(AMX *amx,cell *params)
{
  int c,chars,max;
  TCHAR *str;
  cell *cptr;

  CreateConsole();
  max=(int)params[2];
  if (max<=0)
    return 0;
  if (params[3])
    max*=sizeof(cell);

  chars=0;

  str=(TCHAR *)alloca(max*sizeof(TCHAR));
  if (str!=NULL) {

    c=amx_getch();
    while (c!=EOF && c!=EOL_CHAR && chars<max-1) {
      str[chars++]=(TCHAR)c;
      #if defined(SUPPRESS_ECHO)
 	    /* For Mac OS X, non-Curses, don't echo the character */
      #else
        amx_putchar((TCHAR)c);
        amx_fflush();
      #endif
      if (chars<max-1)
        c=amx_getch();
    } /* while */

    if (c==EOL_CHAR)
      amx_putchar('\n');
    assert(chars<max);
    str[chars]='\0';

    amx_GetAddr(amx,params[1],&cptr);
    amx_SetString(cptr,(char*)str,(int)params[3],sizeof(TCHAR)>1,max);

  } /* if */
  return chars;
}

static void acceptchar(int c,int *num)
{
  switch (c) {
  case '\b':
    amx_putchar('\b');
    *num-=1;
    #if defined amx_putchar && (defined __BORLANDC__ || defined __WATCOMC__)
      /* the backspace key does not erase the
       * character, so do this explicitly */
      amx_putchar(' ');     /* erase */
      amx_putchar('\b');    /* go back */
    #endif
    break;
  case EOL_CHAR:
    amx_putchar('\n');
    *num+=1;
    break;
  default:
    #if defined(SUPPRESS_ECHO)
 	  /* For Mac OS X, non-Curses, don't echo the character */
    #else
      amx_putchar((TCHAR)c);
    #endif
    *num+=1;
  } /* switch */
  amx_fflush();
}

static int inlist(AMX *amx,int c,cell *params,int num)
{
  int i, key;

  for (i=0; i<num; i++) {
    if (i==0) {
      /* first key is passed by value, others are passed by reference */
      key = (int)params[i];
    } else {
      cell *cptr;
      amx_GetAddr(amx,params[i],&cptr);
      key=(int)*cptr;
    } /* if */
    if (c==key || c==-key)
      return key;
  } /* for */
  return 0;
}

static cell AMX_NATIVE_CALL n_getvalue(AMX *amx,cell *params)
{
  cell value;
  int base,sign,c,d;
  int chars,n;

  CreateConsole();
  base=(int)params[1];
  if (base<2 || base>36)
    return 0;

  chars=0;
  value=0;
  sign=1;       /* to avoid a compiler warning (Microsoft Visual C/C++ 6.0) */

  c=amx_getch();
  while (c!=EOF) {
    /* check for sign (if any) */
    if (chars==0) {
      if (c=='-') {
        sign=-1;
        acceptchar(c,&chars);
        c=amx_getch();
      } else {
        sign=1;
      } /* if */
    } /* if */

    /* check end of input */
    #if defined LINUX || defined __FreeBSD__ || defined __OpenBSD__ || defined MACOS
      if (c=='\n' && inlist(amx,'\r',params+2,(int)params[0]/sizeof(cell)-1)!=0)
        c='\r';
    #endif
    if ((chars>1 || chars>0 && sign>0)
        && (n=inlist(amx,c,params+2,(int)params[0]/sizeof(cell)-1))!=0)
    {
      if (n>0)
        acceptchar(c,&chars);
      break;
    } /* if */

    /* get value */
    d=base;     /* by default, do not accept the character */
    if (c>='0' && c<='9') {
      d=c-'0';
    } else if (c>='a' && c<='z') {
      d=c-'a'+10;
    } else if (c>='A' && c<='Z') {
      d=c-'A'+10;
    } else if (c=='\b') {
      if (chars>0) {
        value/=base;
        acceptchar(c,&chars);
      } /* if */
    } /* if */
    if (d<base) {
      acceptchar(c,&chars);
      value=value*base + d;
    } /* if */
    c=amx_getch();
  } /* while */
  return sign*value;
}

static cell AMX_NATIVE_CALL n_clrscr(AMX *amx,cell *params)
{
  (void)amx;
  (void)params;
  CreateConsole();
  amx_clrscr();
  return 0;
}

static cell AMX_NATIVE_CALL n_clreol(AMX *amx,cell *params)
{
  (void)amx;
  (void)params;
  CreateConsole();
  amx_clreol();
  return 0;
}

static cell AMX_NATIVE_CALL n_gotoxy(AMX *amx,cell *params)
{
  (void)amx;
  CreateConsole();
  amx_gotoxy((int)params[1],(int)params[2]);
  return 0;
}

static cell AMX_NATIVE_CALL n_wherexy(AMX *amx,cell *params)
{
  cell *px,*py;
  int x,y;

  CreateConsole();
  amx_wherexy(&x,&y);
  amx_GetAddr(amx,params[1],&px);
  amx_GetAddr(amx,params[2],&py);
  if (px!=NULL) *px=x;
  if (py!=NULL) *py=y;
  return 0;
}

static cell AMX_NATIVE_CALL n_setattr(AMX *amx,cell *params)
{
  (void)amx;
  CreateConsole();
  (void)amx_setattr((int)params[1],(int)params[2],(int)params[3]);
  return 0;
}

static cell AMX_NATIVE_CALL n_consctrl(AMX *amx,cell *params)
{
  (void)amx;
  CreateConsole();
  (void)amx_termctl((int)params[1],(int)params[2]);
  return 0;
}

static cell AMX_NATIVE_CALL n_console(AMX *amx,cell *params)
{
  (void)amx;
  CreateConsole();
  amx_console((int)params[1],(int)params[2],(int)params[3]);
  return 0;
}


#if !defined AMXCONSOLE_NOIDLE
static AMX_DEBUG PrevIdle = NULL;
static int idxKeyPressed = -1;

static int AMXAPI amx_ConsoleIdle(AMX *amx)
{
  int err=0, key;

  assert(idxKeyPressed >= 0);

  if (PrevIdle != NULL)
    PrevIdle(amx);

  if (kbhit()) {
    key = getch();
    amx_Push(amx, key);
    err = amx_Exec(amx, NULL, idxKeyPressed);
    while (err == AMX_ERR_SLEEP)
      err = amx_Exec(amx, NULL, AMX_EXEC_CONT);
  } /* if */

  return err;
}
#endif

#if defined __cplusplus
  extern "C"
#endif
const AMX_NATIVE_INFO console_Natives[] = {
  { "getchar",   n_getchar },
  { "getstring", n_getstring },
  { "getvalue",  n_getvalue },
  { "print",     n_print },
  { "printf",    n_printf },
  { "clrscr",    n_clrscr },
  { "clreol",    n_clreol },
  { "gotoxy",    n_gotoxy },
  { "wherexy",   n_wherexy },
  { "setattr",   n_setattr },
  { "console",   n_console },
  { "consctrl",  n_consctrl },
  { NULL, NULL }        /* terminator */
};

int AMXEXPORT amx_ConsoleInit(AMX *amx)
{
  #if !defined AMXCONSOLE_NOIDLE
    /* see whether there is an @keypressed() function */
    if (amx_FindPublic(amx, "@keypressed", &idxKeyPressed) == AMX_ERR_NONE) {
      if (amx_GetUserData(amx, AMX_USERTAG('I','d','l','e'), (void**)&PrevIdle) != AMX_ERR_NONE)
        PrevIdle = NULL;
      amx_SetUserData(amx, AMX_USERTAG('I','d','l','e'), amx_ConsoleIdle);
    } /* if */
  #endif

  return amx_Register(amx, console_Natives, -1);
}

int AMXEXPORT amx_ConsoleCleanup(AMX *amx)
{
  (void)amx;
  #if !defined AMXCONSOLE_NOIDLE
    PrevIdle = NULL;
  #endif
  return AMX_ERR_NONE;
}
