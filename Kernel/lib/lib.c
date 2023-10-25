#include <lib.h>

void *memset(void *destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char *dst = (char *)destination;

	while (length--)
		dst[length] = chr;

	return destination;
}

void *memcpy(void *destination, const void *source, uint64_t length)
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

void dump_regs_hex_magician(char *s, uint8_t r)
{
	char hextable[] = "0123456789ABCDEF";
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
