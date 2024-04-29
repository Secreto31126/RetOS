#include <lib.h>

void *memset(void *destination, int c, size_t length)
{
	uint8_t chr = (uint8_t)c;
	char *dst = (char *)destination;

	while (length--)
		dst[length] = chr;

	return destination;
}

void *memcpy(void *destination, const void *source, size_t length)
{
	/*
	 * memcpy does not support overlapping buffers, so always do it
	 * forwards. (Don't change this without adjusting memmove.)
	 *
	 * For speedy copying, optimize the common case where both pointers
	 * and the length are word-aligned, and copy word-at-a-time instead
	 * of byte-at-a-time. Otherwise, copy by bytes.
	 *
	 * The alignment logic below should be portable. We rely on
	 * the compiler to be reasonably intelligent about optimizing
	 * the divides and modulos out. Fortunately, it is.
	 */
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *)destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t *d = (uint8_t *)destination;
		const uint8_t *s = (const uint8_t *)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

int strcmp(const char *p1, const char *p2)
{
	const unsigned char *s1 = (const unsigned char *)p1;
	const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;
	do
	{
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
	} while (c1 == c2);
	return c1 - c2;
}

size_t strlen(const char *str)
{
	const char *s = str;
	while (*s)
		s++;
	return s - str;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	if (n)
	{
		char *d = dest;
		const char *s = src;

		do
		{
			if (!(*d++ = *s++))
			{
				/* NUL pad the remaining n-1 bytes */
				while (--n)
					*d++ = 0;
				break;
			}
		} while (--n);
	}

	return dest;
}

char *strcpy(char *dest, const char *src)
{
	char *save = dest;
	for (; (*dest = *src); ++src, ++dest)
		;
	return save;
}

void power_off()
{
	output_word(0x604, 0x2000);

	while (1)
	{
		unset_interrupt_flag();
		halt_once();
	}
}

void dump_regs_hex_magician(unsigned char *s, uint8_t r)
{
	unsigned char hextable[] = "0123456789ABCDEF";
	s[0] = hextable[r / 0x10];
	s[1] = hextable[r % 0x10];
}

/**
 * @brief Appends the RIP register dump to the dump_reg_string
 * @note Hard coding ftw
 *
 * @param rip The value of the RIP register to append to the dump
 */
void dump_regs_include_rip(uint64_t rip)
{
	memcpy(dump_reg_string + 336, "RIP:", 4);
	for (int i = 0; i < 8; i++)
	{
		dump_regs_hex_magician(dump_reg_string + 340 + i * 2, (rip >> (56 - 8 * i)) & 0xFF);
	}
	dump_reg_string[357] = '\n';
	dump_reg_string[358] = '\0';
}
