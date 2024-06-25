// Taken, graciously, from AMXMODX
// 

#include "main.h"
#include "../server/amx/amx.h"

#include "format.h"

cell* get_amxaddr(AMX *amx,cell amx_addr);

#define ALT			0x00000001		/* alternate form */
#define HEXPREFIX	0x00000002		/* add 0x or 0X prefix */
#define LADJUST		0x00000004		/* left adjustment */
#define LONGDBL		0x00000008		/* long double */
#define LONGINT		0x00000010		/* long integer */
#define QUADINT		0x00000020		/* quad integer */
#define SHORTINT	0x00000040		/* short integer */
#define ZEROPAD		0x00000080		/* zero (as opposed to blank) pad */
#define FPT			0x00000100		/* floating point number */
#define to_digit(c)		((c) - '0')
#define is_digit(c)		((unsigned)to_digit(c) <= 9)
#define to_char(n)		((n) + '0')
#define CHECK_ARGS(n) \
	if ((arg+n) > args) { \
		LogError(amx, AMX_ERR_PARAMS, "String formatted incorrectly - parameter %d (total %d)", arg, args); \
		return 0; \
	}

template <typename U>
void AddString(U **buf_p, size_t &maxlen, const cell *string, int width, int prec)
{
	int		size = 0;
	U		*buf;
	static cell nlstr[] = {'(','n','u','l','l',')','\0'};

	buf = *buf_p;

	if (string == NULL)
	{
		string = nlstr;
		prec = -1;
	}

	if (prec >= 0)
	{
		for (size = 0; size < prec; size++) 
		{
			if (string[size] == '\0')
				break;
		}
	} else {
		while (string[size++]) ;
		size--;
	}

	if (size > (int)maxlen)
		size = maxlen;

	maxlen -= size;
	width -= size;

	while (size--)
		*buf++ = static_cast<U>(*string++);

	while (width-- > 0 && maxlen)
	{
		*buf++ = ' ';
		maxlen--;
	}

	*buf_p = buf;
}

template <typename U>
void AddFloat(U **buf_p, size_t &maxlen, double fval, int width, int prec)
{
	U		text[32];
	int		digits;
	double	signedVal;
	U		*buf;
	int		val;

	// get the sign
	signedVal = fval;
	if (fval < 0)
		fval = -fval;

	// write the float number
	digits = 0;
	val = (int)fval;
	do {
		text[digits++] = '0' + val % 10;
		val /= 10;
	} while (val);

	if (signedVal < 0)
		text[digits++] = '-';

	buf = *buf_p;

	while (digits < width && maxlen)
	{
		*buf++ = ' ';
		width--;
		maxlen--;
	}

	while (digits-- && maxlen)
	{
		*buf++ = text[digits];
		maxlen--;
	}

	*buf_p = buf;

	if (prec < 0)
		prec = 6;
	// write the fraction
	digits = 0;
	while (digits < prec)
	{
		fval -= (int) fval;
		fval *= 10.0;
		val = (int) fval;
		text[digits++] = '0' + val % 10;
	}

	if (digits > 0 && maxlen)
	{
		buf = *buf_p;
		*buf++ = '.';
		maxlen--;
		for (prec = 0; maxlen && prec < digits; prec++)
		{
			*buf++ = text[prec];
			maxlen--;
		}
		*buf_p = buf;
	}
}

template <typename U>
void AddInt(U **buf_p, size_t &maxlen, int val, int width, int flags)
{
	U		text[32];
	int		digits;
	int		signedVal;
	U		*buf;

	digits = 0;
	signedVal = val;
	if (val < 0)
		val = -val;
	do {
		text[digits++] = '0' + val % 10;
		val /= 10;
	} while (val);

	//if (signedVal < 0)
		//text[digits++] = '-';
		
	buf = *buf_p;

	if (signedVal < 0)
	{
		if (flags & ZEROPAD)
		{
			*buf++ = '-';
		}
		else
		{
			text[digits++] = '-';
		}
	}

	if( !(flags & LADJUST) )
	{
		while (digits < width && maxlen)
		{
			*buf++ = (flags & ZEROPAD) ? '0' : ' ';
			width--;
			maxlen--;
		}
	}

	while (digits-- && maxlen)
	{
		*buf++ = text[digits];
		width--;
		maxlen--;
	}

	if (flags & LADJUST)
	{
		while (width-- && maxlen)
		{
			*buf++ = (flags & ZEROPAD) ? '0' : ' ';
			maxlen--;
		}
	}

	*buf_p = buf;
}

template <typename U>
void AddHex(U **buf_p, size_t &maxlen, int val, int width, int flags)
{
	U		text[32];
	int		digits;
	//int		signedVal;
	U		*buf;

	digits = 0;
	
	do
	{
		text[digits] = '0' + val % 16;
		if (text[digits] > '9') text[digits] += 7; // Shift to letters
		digits++;
		val /= 16; // val >>= 4;
	}
	while (val);
	
	//text[digits++] = 'x';
	//text[digits++] = '0';

	buf = *buf_p;
	
	if(!(flags & LADJUST))
	{
		while (digits < width && maxlen)
		{
			*buf++ = (flags & ZEROPAD) ? '0' : ' ';
			width--;
			maxlen--;
		}
	}

	while (digits-- && maxlen)
	{
		*buf++ = text[digits];
		width--;
		maxlen--;
	}

	if (flags & LADJUST)
	{
		while (width-- && maxlen)
		{
			*buf++ = (flags & ZEROPAD) ? '0' : ' ';
			maxlen--;
		}
	}

	*buf_p = buf;
}

template <typename U>
void AddBin(U **buf_p, size_t &maxlen, int val, int width, int flags)
{
	U		text[32];
	int		digits;
	//int		signedVal;
	U		*buf;

	digits = 0;
	
	do
	{
		text[digits++] = '0' + (val & 1);
		val >>= 1; // val >>= 4;
	}
	while (val);
	
	//text[digits++] = 'x';
	//text[digits++] = '0';

	buf = *buf_p;
		
	if(!(flags & LADJUST))
	{
		while (digits < width && maxlen)
		{
			*buf++ = (flags & ZEROPAD) ? '0' : ' ';
			width--;
			maxlen--;
		}
	}

	while (digits-- && maxlen)
	{
		*buf++ = text[digits];
		width--;
		maxlen--;
	}

	if (flags & LADJUST)
	{
		while (width-- && maxlen)
		{
			*buf++ = (flags & ZEROPAD) ? '0' : ' ';
			maxlen--;
		}
	}

	*buf_p = buf;
}

template <typename D, typename S>
size_t atcprintf(D *buffer, size_t maxlen, const S *format, AMX *amx, cell *params, int *param)
{
	int		arg;
	int		args = params[0] / sizeof(cell);
	D		*buf_p;
	D		ch;
	int		flags;
	int		width;
	int		prec;
	int		n;
	char	sign;
	const S	*fmt;
	size_t	llen = maxlen;

	buf_p = buffer;
	arg = *param;
	fmt = format;

	while (true)
	{
		// run through the format string until we hit a '%' or '\0'
		for (ch = static_cast<D>(*fmt); 
			llen && ((ch = static_cast<D>(*fmt)) != '\0' && ch != '%');
			fmt++)
		{
			*buf_p++ = static_cast<D>(ch);
			llen--;
		}
		if (ch == '\0' || llen <= 0)
			goto done;

		// skip over the '%'
		fmt++;

		// reset formatting state
		flags = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:
		ch = static_cast<D>(*fmt++);
reswitch:
		switch(ch)
		{
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '.':
			if (( ch = static_cast<D>(*fmt)) == '*')
			{
				prec = *get_amxaddr(amx, params[arg++]);
				fmt++;
				goto rflag;
			}
			else
			{
				n = 0;
				while( is_digit( ( ch = static_cast<D>(*fmt++)) ) )
					n = 10 * n + ( ch - '0' );
				prec = n < 0 ? -1 : n;
				goto reswitch;
			}
		case '0':
			flags |= ZEROPAD;
			goto rflag;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			n = 0;
			do {
				n = 10 * n + ( ch - '0' );
				ch = static_cast<D>(*fmt++);
			} while( is_digit( ch ) );
			width = n;
			goto reswitch;
		case '*':
			width = *get_amxaddr(amx, params[arg++]);
			goto rflag;
		case 'b':
			AddBin(&buf_p, llen, *get_amxaddr(amx, params[arg]), width, flags);
			arg++;			
			break;
		case 'c':
			*buf_p++ = static_cast<D>(*get_amxaddr(amx, params[arg]));
			arg++;
			break;
		case 'd':
		case 'i':
			AddInt(&buf_p, llen, *get_amxaddr(amx, params[arg]), width, flags);
			arg++;
			break;
		case 'f':
			AddFloat(&buf_p, llen, amx_ctof(*get_amxaddr(amx, params[arg])), width, prec);
			arg++;
			break;
		case 's':
			AddString(&buf_p, llen, get_amxaddr(amx, params[arg]), width, prec);
			arg++;
			break;
		case 'h':
		case 'x':
			AddHex(&buf_p, llen, *get_amxaddr(amx, params[arg]), width, flags);
			arg++;
			break;
		case '%':
			*buf_p++ = static_cast<D>(ch);
			if (!llen)
				goto done;
			llen--;
			break;
		case '\0':
			*buf_p++ = static_cast<D>('%');
			if (!llen)
				goto done;
			llen--;
			goto done;
			break;
		default:
			*buf_p++ = static_cast<D>(ch);
			if (!llen)
				goto done;
			llen--;
			break;
		}
	}

done:
	*buf_p = static_cast<D>(0);
	*param = arg;
	return maxlen-llen;
}

/**
 * HACKHACK: The compiler will generate code for each case we need.
 * Don't remove this, otherwise files that use certain code generations
 *  will have extern problems.  For each case you need, add dummy code
 *  here.
 */
void __WHOA_DONT_CALL_ME_PLZ_K_lol_o_O()
{
	//acsprintf
	atcprintf((cell *)NULL, 0, (const char *)NULL, NULL, NULL, NULL);
	//accprintf
	atcprintf((cell *)NULL, 0, (cell *)NULL, NULL, NULL, NULL);
	//ascprintf
	atcprintf((char *)NULL, 0, (cell *)NULL, NULL, NULL, NULL);
}

