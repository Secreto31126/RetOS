#include "nstdlib.h"
#define MAX_DIGITS_IN_LONG 20
#define MAX_STDIN_STRING 256
#define BLOCK 10
#define BIG_BLOCK 400
#define null 0

extern uint64_t get_dump(char *buffer, uint64_t n);
uint64_t replaceWith(char *startAddress, char *replacement, uint64_t eatThisManyChars);
uint64_t concatFrom(char *s1, char *s2);
void addToAllocated(char *address);

void wait()
{
    halt_user();
}

char *getDumpString()
{
    uint64_t length = BIG_BLOCK;
    char *c = malloc(length * sizeof(char));
    char lastAdded = 0;
    if ((lastAdded = get_dump(c + length - BIG_BLOCK, BIG_BLOCK)))
    {
        if (lastAdded >= length)
        {
            c = realloc(c, length, length + BIG_BLOCK);
            length += BIG_BLOCK;
            while ((lastAdded = get_dump(c + length - BIG_BLOCK, BIG_BLOCK)) == BIG_BLOCK) // adds BLOCK characters to the end of c until get_dump no longer has BLOCK characters to add
            {
                c = realloc(c, length, length + BIG_BLOCK);
                length += BIG_BLOCK;
            }
        }
        addToAllocated(c);
        return c;
    }
    free(c);
    return "";
}

void *realloc(void *ptr, uint64_t oldSize, uint64_t newSize)
{
    void *aux = malloc(newSize);
    oldSize = oldSize > newSize ? newSize : oldSize;
    for (uint64_t i = 0; i < oldSize; i++)
        *((char *)(aux + i)) = *((char *)(ptr + i));
    free(ptr);
    return aux;
}

uint64_t atoiHex(char *s)
{
    char maxLength = 8; // max number of hex digits in a uint64_t
    uint64_t ret = 0;
    while (maxLength)
    {
        maxLength--;
        if (*s >= '0' && *s <= '9')
        {
            ret *= 16;
            ret += *s - '0';
            s++;
        }
        else if (*s >= 'A' && *s <= 'F')
        {
            ret *= 16;
            ret += *s - 'A' + 10;
            s++;
        }
        else if (*s >= 'a' && *s <= 'f')
        {
            ret *= 16;
            ret += *s - 'a' + 10;
            s++;
        }
        else
            break;
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
    print_sys(1, &c, 1);
    return c;
}

uint64_t puts(char *string)
{
    uint64_t len = strlen(string);
    print_sys(1, string, len);
    return len;
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
        if (*format == '%')
        {
            format++;
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
    while (*format != 0 && *format != EOF)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
            {
                char aux[1];
                if (!read_sys(0, aux, 1))
                    return count;
                replaceWith(format - 1, aux, 2);
                break;
            }
            case 's':
            {
                char aux[MAX_STDIN_STRING];
                read_sys(0, aux, MAX_STDIN_STRING);
                replaceWith(format - 1, aux, 2);
                break;
            }
            case 'i':
            case 'd':
            case 'u':
            case 'l':
            {
                char aux[MAX_STDIN_STRING];
                char c;
                for (int auxReader = 0; auxReader < MAX_STDIN_STRING && (c = readChar()) >= '0' && c <= '9'; auxReader++)
                    aux[auxReader] = c;
                replaceWith(format - 1, aux, 2);
                break;
            }
            default:
            {
                count--; // this is slightly inefficient, prevents adding count++ to every other condition in switch statement
                break;
            }
            }
            count++;
        }
        format++;
    }
    va_end(argp);
    return count;
}
/**
 * insertion is a string you want inserted into another
 * startAddress is the location in a string where insertion should be placed
 * return value is length of insertion string
 * startAddress string should have enough space allocated to add insertion string
 */
uint64_t insertString(char *startAddress, char *insertion)
{
    return replaceWith(startAddress, insertion, 0);
}

uint64_t replaceWith(char *startAddress, char *replacement, uint64_t eatThisManyChars)
{
    uint64_t len = strlen(replacement) - eatThisManyChars;
    if (len <= 0)
        return 0;
    char *aux = startAddress;
    while (*aux != 0)
        aux++;  // aux is now looking at null termination of string containing startAddress
    aux += len; // aux is now looking at where new null termination will be
    char *aux2 = startAddress + len;
    while (aux >= aux2)
    {
        *(aux + len) = *aux;
        aux--;
    } // enough space has been made to add insertion string
    concatFrom(startAddress, replacement);
    return len;
}

uint64_t concat(char *s1, char *s2)
{
    while (*s1 != 0)
        s1++;
    return concatFrom(s1, s2);
}
uint64_t concatFrom(char *sEnd, char *sAdd)
{
    uint64_t count = 0;
    while (*sAdd != 0)
    {
        *sEnd = *sAdd;
        sEnd++;
        sAdd++;
        count++;
    }
    return count;
}

uint64_t strlen(char *s)
{
    uint64_t len = 0;
    while (*(s + len))
        len++;
    return len;
}

char readChar()
{
    char c;
    return read_sys(0, &c, 1) ? c : 0;
}

char getChar()
{
    char c;
    read_sys(0, &c, 1);
    return c;
}

int read(char *buffer, int count)
{
    return read_sys(0, buffer, count);
}

uint64_t pow(double base, uint64_t exponent)
{
    uint64_t ans = 1.0;
    for (uint64_t i = 0; i < exponent; i++)
        ans *= base;
    return ans;
}

char strCompare(char *a, char *b)
{
    while (*a && *a == *b)
    {
        a++;
        b++;
    }
    int dist = (int)(unsigned char)(*a) - (int)(unsigned char)(*b);
    return dist ? dist > 0 ? 1 : -1 : 0;
}

char strcmp(const char *s1, const char *s2)
{
    while (*s1 || *s2)
    {
        if (*(s1++) != *(s2++))
            return 0;
    }
    return 1;
}

char isFirstWord(char *s1, char *firstWord)
{
    while ((*s1 && *s1 != ' ') || (*firstWord && *firstWord != ' '))
        if (*(s1++) != *(firstWord++))
            return 0;
    return 1;
}

// User is responsible for ensuring receiver has enough memory allocated to receive a char
char sPutChar(char *receiver, char c)
{
    while (*receiver)
        receiver++;
    *receiver = c;
    *(receiver + 1) = 0;
    return c;
}

// returns length of added string
uint64_t sPuts(char *receiver, const char *source)
{
    char count = 0;
    while (*receiver)
    {
        receiver++;
    }
    while (*source)
    {
        *(receiver++) = *(source++);
        count++;
    }
    *receiver = 0;
    return count;
}

// Returns length of string added
uint64_t addString(char **receiver, uint64_t *length, char *source, uint64_t *allocated)
{
    uint64_t len = strlen(source);
    if ((*length + len) >= *allocated)
    {
        uint64_t toAdd = ((*length + len - *allocated) / BLOCK) * BLOCK + BLOCK; // space that must be added, rounded to nearest block
        *receiver = realloc(*receiver, *allocated, (*allocated + toAdd) * sizeof(char));
        *allocated += toAdd;
    }
    sPuts(*receiver + *length - 1, source);
    *length += len;
    return len;
}

static char **allocatedPrints = null; // A pointer to pointers to char. Stores all allocated strings in sPrintf.
static int allocatedPrintCount = 0;   // These serve to keep track of allocated memory in sPrintf and other functions that allocate strings, so as not to offload free responsibility to users.
void addToAllocated(char *address)
{
    if (!(allocatedPrintCount % BLOCK))
    {
        allocatedPrints = realloc(allocatedPrints, allocatedPrintCount * sizeof(char *), (allocatedPrintCount + BLOCK) * sizeof(char *));
    }
    allocatedPrints[allocatedPrintCount++] = address;
}

void freePrints()
{
    for (int i = 0; i < allocatedPrintCount; i++)
        free(allocatedPrints[i]);
    free(allocatedPrints);
    allocatedPrints = null;
    allocatedPrintCount = 0;
}

/**
 * returns formatted string
 * User should call freePrints() to free memory allocated for returned string
 */
char *sPrintf(char *format, ...)
{
    va_list argp;
    va_start(argp, format);
    uint64_t allocated = BLOCK;
    uint64_t count = 1; // counts null termination
    char *toReturn = malloc(sizeof(char) * allocated);
    *toReturn = 0;
    while (*format != 0 && *format != EOF)
    {
        if (count == allocated) // Always checks if resize is necessary before checking to add any characters. Prevents copying code in every single-character edition.
        {
            toReturn = realloc(toReturn, allocated, (allocated + BLOCK) * sizeof(char));
            allocated += BLOCK;
        }
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
            {
                sPutChar(toReturn, va_arg(argp, int));
                count++;
                break;
            }
            case 's':
            {
                addString(&toReturn, &count, va_arg(argp, char *), &allocated);
                break;
            }
            case 'i':
            case 'd':
            {
                char aux[MAX_DIGITS_IN_LONG];
                addString(&toReturn, &count, itoa(va_arg(argp, int), aux, 10), &allocated);
                break;
            }
            case 'u':
            {
                char aux[MAX_DIGITS_IN_LONG];
                addString(&toReturn, &count, utoa(va_arg(argp, int), aux, 10), &allocated);
                break;
            }
            case 'l':
            {
                char aux[MAX_DIGITS_IN_LONG];
                addString(&toReturn, &count, ultoa(va_arg(argp, int), aux, 10), &allocated);
                break;
            }
            default:
            {
                sPutChar(toReturn, '%');
                break;
            }
            }
        }
        else
        {
            sPutChar(toReturn, *format);
            count++;
        }
        format++;
    }
    va_end(argp);
    toReturn = realloc(toReturn, allocated, count);
    addToAllocated(toReturn);
    return toReturn;
}

uint64_t getMinutes()
{
    uint64_t hexedTime = get_unix_time();
    return (hexedTime & 0xF) + (((hexedTime & 0xF0) >> 4) * 10);
}

uint64_t getHours()
{
    uint64_t hexedTime = get_unix_time();
    return ((hexedTime & 0xF00) >> 8) + (((hexedTime & 0xF000) >> 12) * 10);
}

// User should call freePrints() to free memory allocated for returned string
char *getTimeString()
{
    char *buffer = malloc(6 * sizeof(char)); // return string format will be 6 chars long. defining a value for this is unnecessary and confusing
    addToAllocated(buffer);
    uint64_t min = getMinutes(), hr = getHours();
    char addedZeroFlag = 0;
    if (hr < 10)
    {
        *buffer = '0';
        addedZeroFlag = 1;
    }
    itoa(hr, buffer + addedZeroFlag, 10);
    buffer += 2; // For the two digits added
    *(buffer++) = ':';
    addedZeroFlag = 0;
    if (min < 10)
    {
        *buffer = '0';
        addedZeroFlag = 1;
    }
    itoa(min, buffer + addedZeroFlag, 10);
    buffer += 2; // For the two digits added
    *buffer = 0;
    return buffer - 5; // In total, buffer was shifted 5 spaces to format as 'HH:MM\0'
}

char isPrefix(char *prefix, char *word)
{
    while (*prefix)
        if (*(prefix++) != *(word++))
            return 0;
    return 1;
}

char *concatUnlimited(char *s1, char *s2)
{
    uint64_t index = 0;
    char *toReturn = null;
    while (*s1)
    {
        if (!(index % BLOCK))
            toReturn = realloc(toReturn, index * sizeof(char), (index + BLOCK) * sizeof(char));
        *(toReturn + index++) = *s1;
    }
    while (*s2)
    {
        if (!(index % BLOCK))
            toReturn = realloc(toReturn, index * sizeof(char), (index + BLOCK) * sizeof(char));
        *(toReturn + index++) = *s2;
    }
    *(toReturn + index) = 0;
    addToAllocated(toReturn);
    return toReturn;
}

char strcmpHandleWhitespace(char *s1, char *s2)
{
    char wFlag = 0; //
    while (*s1 || *s2)
    {
        while (wFlag && *s1 == ' ')
            s1++;
        while (wFlag && *s2 == ' ')
            s2++;
        wFlag = 0;
        if (*s1 == ' ')
        {
            wFlag = 1;
        }
        if (*(s1++) != *(s2++))
        {
            printf("s1: \'%c\' ", *(s1 - 1));
            printf("s2: \'%c\' \n", *(s2 - 1));
            return 0;
        }
    }
    return 1;
}
char *shiftToWord(char *s)
{
    while (*s == ' ')
        s++;
    return s;
}
char *shiftToNextWord(char *s)
{
    while (*s != ' ' && *s != 0)
        s++;
    return shiftToWord(s);
}

int readNFromFd(int fd, char *buffer, int n)
{
    if (fd < 0)
        return 0;
    int count = 1, aux; // count starts as 1, save space for the '\0'
    while ((aux = read_sys(fd, buffer + count, n - count)) > 0 && count < n)
        count += aux;
    buffer[count - 1] = 0;
    return count;
}

char timeHasPassed(uint64_t start, uint64_t unit)
{
    return (get_tick() - start) > unit;
}
