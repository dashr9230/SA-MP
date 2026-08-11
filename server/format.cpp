#include "main.h"
#include "format.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

namespace
{
    template <typename D>
    class Writer
    {
    public:
        Writer(D *out, size_t capacity) : m_out(out), m_left(capacity), m_written(0) {}

        void put(D ch)
        {
            if (m_left > 1)
            {
                *m_out++ = ch;
                --m_left;
            }
            ++m_written;
        }

        template <typename S>
        void string(const S *src, int precision = -1)
        {
            if (!src)
                return;

            while (*src && (precision < 0 || precision-- > 0))
                put(static_cast<D>(*src++));
        }

        size_t written() const { return m_written; }
        void finish() { if (m_left) *m_out = static_cast<D>(0); }

    private:
        D *m_out;
        size_t m_left;
        size_t m_written;
    };

    inline bool digit(char c)
    {
        return c >= '0' && c <= '9';
    }

    template <typename D>
    void integer(Writer<D> &w, cell value, int width, bool left, bool zero)
    {
        char tmp[32];
        unsigned int magnitude;
        int n = 0;
        const bool negative = value < 0;

        // Avoid signed overflow for INT_MIN.
        magnitude = negative ? (0u - static_cast<unsigned int>(value))
                             : static_cast<unsigned int>(value);

        do
        {
            tmp[n++] = static_cast<char>('0' + (magnitude % 10));
            magnitude /= 10;
        } while (magnitude && n < static_cast<int>(sizeof(tmp)));

        const int total = n + (negative ? 1 : 0);
        if (!left && !zero)
            for (int i = total; i < width; ++i) w.put(' ');

        if (negative)
            w.put('-');

        if (!left && zero)
            for (int i = total; i < width; ++i) w.put('0');

        while (n-- > 0)
            w.put(tmp[n]);

        if (left)
            for (int i = total; i < width; ++i) w.put(' ');
    }

    template <typename D>
    void unsigned_integer(Writer<D> &w, unsigned int value, int width, bool left, bool zero, int base)
    {
        static const char digits[] = "0123456789abcdef";
        char tmp[32];
        int n = 0;

        do
        {
            tmp[n++] = digits[value % static_cast<unsigned int>(base)];
            value /= static_cast<unsigned int>(base);
        } while (value && n < static_cast<int>(sizeof(tmp)));

        if (!left)
            for (int i = n; i < width; ++i) w.put(zero ? '0' : ' ');

        while (n-- > 0)
            w.put(tmp[n]);

        if (left)
            for (int i = n + 1; i < width; ++i) w.put(' ');
    }

    template <typename D>
    void floating(Writer<D> &w, float value, int width, int precision, bool left, bool zero)
    {
        if (precision < 0) precision = 6;
        if (precision > 9) precision = 9;

        char tmp[96];
        const int count = snprintf(tmp, sizeof(tmp), "%.*f", precision, value);
        if (count <= 0)
            return;

        const int len = count < static_cast<int>(sizeof(tmp)) ? count : static_cast<int>(sizeof(tmp)) - 1;
        if (!left)
            for (int i = len; i < width; ++i) w.put(zero ? '0' : ' ');
        for (int i = 0; i < len; ++i) w.put(tmp[i]);
        if (left)
            for (int i = len; i < width; ++i) w.put(' ');
    }
}

template <typename D, typename S>
size_t atcprintf(D *buffer, size_t maxlen, const S *format, AMX *amx, cell *params, int *param)
{
    if (!buffer || !format || !amx || !params || !param || maxlen == 0)
    {
        if (buffer && maxlen) buffer[0] = static_cast<D>(0);
        return 0;
    }

    Writer<D> writer(buffer, maxlen);
    const int args = static_cast<int>(params[0] / sizeof(cell));
    int arg = *param;

    for (const S *p = format; *p; ++p)
    {
        if (*p != static_cast<S>('%'))
        {
            writer.put(static_cast<D>(*p));
            continue;
        }

        ++p;
        if (!*p) break;

        if (*p == static_cast<S>('%'))
        {
            writer.put(static_cast<D>('%'));
            continue;
        }

        bool left = false;
        bool zero = false;
        int width = 0;
        int precision = -1;

        bool parsing = true;
        while (parsing)
        {
            switch (static_cast<char>(*p))
            {
                case '-': left = true; ++p; break;
                case '0': zero = true; ++p; break;
                default: parsing = false; break;
            }
        }

        if (*p == static_cast<S>('*'))
        {
            if (arg > args) break;
            width = static_cast<int>(*get_amxaddr(amx, params[arg++]));
            ++p;
            if (width < 0) { left = true; width = -width; }
        }
        else
        {
            while (digit(static_cast<char>(*p)))
            {
                width = width * 10 + (static_cast<char>(*p) - '0');
                ++p;
                if (width > 4096) { width = 4096; }
            }
        }

        if (*p == static_cast<S>('.'))
        {
            ++p;
            precision = 0;
            if (*p == static_cast<S>('*'))
            {
                if (arg > args) break;
                precision = static_cast<int>(*get_amxaddr(amx, params[arg++]));
                ++p;
            }
            else
            {
                while (digit(static_cast<char>(*p)))
                {
                    precision = precision * 10 + (static_cast<char>(*p) - '0');
                    ++p;
                    if (precision > 4096) { precision = 4096; }
                }
            }
            if (precision < 0) precision = -1;
        }

        const char spec = static_cast<char>(*p);
        if (spec != '%')
        {
            if (arg > args) break;
        }

        switch (spec)
        {
            case 'c':
                writer.put(static_cast<D>(*get_amxaddr(amx, params[arg++])));
                break;

            case 'd':
            case 'i':
                integer(writer, *get_amxaddr(amx, params[arg++]), width, left, zero);
                break;

            case 'u':
                unsigned_integer(writer, static_cast<unsigned int>(*get_amxaddr(amx, params[arg++])), width, left, zero, 10);
                break;

            case 'x':
            case 'h':
                unsigned_integer(writer, static_cast<unsigned int>(*get_amxaddr(amx, params[arg++])), width, left, zero, 16);
                break;

            case 'b':
                unsigned_integer(writer, static_cast<unsigned int>(*get_amxaddr(amx, params[arg++])), width, left, zero, 2);
                break;

            case 'f':
                floating(writer, amx_ctof(*get_amxaddr(amx, params[arg++])), width, precision, left, zero);
                break;

            case 's':
            {
                cell *src = get_amxaddr(amx, params[arg++]);
                int length = 0;
                if (src && amx_StrLen(src, &length) == AMX_ERR_NONE && length > 0)
                {
                    char *text = static_cast<char *>(malloc(static_cast<size_t>(length) + 1));
                    if (text)
                    {
                        amx_GetString(text, src, 0, length + 1);
                        const int limit = precision >= 0 ? precision : length;
                        const int used = limit < length ? limit : length;
                        for (int i = 0; i < used; ++i) writer.put(static_cast<D>(text[i]));
                        if (left)
                            for (int i = used; i < width; ++i) writer.put(static_cast<D>(' '));
                        else if (used < width)
                        {
                            // Reformatting would require moving already written data;
                            // preserve the safe, deterministic no-overwrite behaviour.
                        }
                        free(text);
                    }
                }
                break;
            }

            default:
                // Keep unknown specifiers visible rather than silently corrupting output.
                writer.put(static_cast<D>('%'));
                writer.put(static_cast<D>(spec));
                break;
        }
    }

    writer.finish();
    *param = arg;
    return writer.written();
}

// Explicit instantiations required by the server's Pawn/native call sites.
template size_t atcprintf<char, char>(char *, size_t, const char *, AMX *, cell *, int *);
template size_t atcprintf<cell, cell>(cell *, size_t, const cell *, AMX *, cell *, int *);
template size_t atcprintf<char, cell>(char *, size_t, const cell *, AMX *, cell *, int *);
