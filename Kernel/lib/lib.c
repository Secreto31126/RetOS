#include <lib.h>

void power_off()
{
    output_word(0x604, 0x2000);

    while (1)
    {
        unset_interrupt_flag();
        halt_once();
    }
}

uint16_t endian_word(uint16_t val)
{
    return (val >> 8) | (val << 8);
}

uint32_t endian_dword(uint32_t val)
{
    return ((val & 0xFF) << 24) | ((val & 0xFF00) << 8) | ((val & 0xFF0000) >> 8) | ((val & 0xFF000000) >> 24);
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
