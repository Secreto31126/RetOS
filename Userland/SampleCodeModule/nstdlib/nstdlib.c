#include "nstdlib.h"
#define MAX_DIGITS_IN_LONG 20

char *ultoa(unsigned long l, char *buffer, int radix)
{
    while (l > radix)
    {
        *buffer = (l % radix) + '0';
        if (*buffer > '9')
            *buffer += 'A' - '9' - 1;
        buffer++;
        l /= radix;
    }

    *buffer = l + '0';
    if (*buffer > '9')
        *buffer += 'A' - '9' - 1;

    buffer++;
    *buffer = 0;
    return buffer;
}

char *utoa(unsigned int n, char *buffer, int radix)
{
    return ultoa(n, buffer, radix);
}

char *itoa(int n, char *buffer, int radix)
{
    if (n < 0)
    {
        *buffer = '-';
        buffer++;
        n = -n;
    }

    return utoa(n, buffer, radix) - 1;
}

uint64_t atoi(char *s)
{
    uint64_t ret = 0;
    while (*s >= '0' && *s <= '9')
    {
        ret *= 10;
        ret += *s - '0';
        s++;
    }
    return ret;
}

uint64_t numLength(char *s)
{
    uint64_t count = 0;
    while (*s >= '0' && *s <= '9')
    {
        s++;
        count++;
    }
    return count;
}

char *dtoa(double n, char *buffer, unsigned int decPoints)
{
    int count = strlen(itoa((int)n, buffer, 10));
    if (n < 0)
        n *= -1;
    n = (n - ((int)n)); // handle 'integer' part of double
    while ((int)n != 0 && count < decPoints && count <= MAX_DOUBLE_LENGTH)
    {
        itoa((int)n, buffer + count, 10);
        count += DOUBLE_RESOLUTION;
        n = (n - ((int)n));
        for (int i = 1; i < DOUBLE_RESOLUTION; i++)
            n *= 10.0;
    }
    if (count > decPoints)
        *(buffer + decPoints) = 0;
    else
        *(buffer + count) = 0;
    return buffer;
}
char *ftoa(float n, char *buffer, int readLength)
{
    return dtoa(n, buffer, readLength);
}

char putChar(char c)
{
    print_sys(&c, 1);
}
/*
char getChar()
{
    char c[1];
    read_sys(c, 1);
    return *c;
}
*/
uint64_t puts(char *string)
{
    uint64_t len = strlen(string);
    print_sys(string, len);
    return len;
}

static const char formatCharacters[] = {'%', '\\'}, formatCharacterCount = 2;

char isFormatCharacter(char c)
{
    for (int i = 0; i < formatCharacterCount; i++)
        if (formatCharacters[i] == c)
            return 1;
    return 0;
}
/**
 * returns length of string printed
 */
uint64_t printf(char *format, ...)
{
    va_list argp;
    va_start(argp, format);
    uint64_t count = 0;
    while (*format != 0 && *format != EOF)
    {
        if (*format == '\\')
        {
            format++;
            if (isFormatCharacter(*format))
            {
                putChar(*format);
            }
            else
                putChar('\\');
            count++;
        }
        else if (*format == '%')
        {
            *format++;
            switch (*format)
            {
            case 'c':
            {
                putChar(va_arg(argp, int));
                count++;
                break;
            }
            case 's':
            {
                char *s = va_arg(argp, char *);
                count += strlen(s);
                puts(s);
                break;
            }
            case 'i':
            case 'd':
            {
                char aux[MAX_DIGITS_IN_LONG];
                count += puts(itoa(va_arg(argp, int), aux, 10));
                break;
            }
            case 'u':
            {
                char aux[MAX_DIGITS_IN_LONG];
                count += puts(utoa(va_arg(argp, unsigned int), aux, 10));
                break;
            }
            case 'l':
            {
                char aux[MAX_DIGITS_IN_LONG];
                count += puts(ultoa(va_arg(argp, long), aux, 10));
                break;
            }
            // varargs and float values do not play well without SSE
            /*
            case 'f':
            {
                char aux[MAX_DOUBLE_LENGTH + 1];
                count += puts(ftoa(va_arg(argp, float), aux, MAX_DOUBLE_LENGTH));
                break;
            }
            case 'g':
            {
                char aux[MAX_DOUBLE_LENGTH + 1];
                count += puts(dtoa(va_arg(argp, double), aux, MAX_DOUBLE_LENGTH));
                break;
            }
            case '.':
            {
                format++;
                uint64_t decs = atoi(format);
                if (decs != 0 || *format == '0')
                {
                    char toSkip = numLength(format);
                    char auxc = *(format + toSkip); // next char after number is found
                    if (auxc == 'f')
                    {
                        char aux[MAX_DOUBLE_LENGTH + 1];
                        count += puts(ftoa(va_arg(argp, float), aux, decs));
                        format += toSkip - 1;
                    }
                    else if (auxc == 'g')
                    {
                        char aux[MAX_DOUBLE_LENGTH + 1];
                        count += puts(dtoa(va_arg(argp, double), aux, decs));
                        format += toSkip - 1;
                    }
                    else
                    {
                        puts("%.");
                        format--;
                    }
                }
                break;
            }
            */
            default:
            {
                format--;
                puts("%");
                break;
            }
            }
        }
        else
        {
            putChar(*format);
        }
        format++;
        count++;
    }
    va_end(argp);
    return count;
}
/**
 * returns number of fields converted
 */
uint64_t scanf(char *format, ...)
{
    va_list argp;
    va_start(argp, format);
    uint64_t count = 0;

    va_end(argp);
    return count;
}

uint64_t strlen(char *s)
{
    uint64_t len = 0;
    while (*(s + len))
        len++;
    return len;
}
