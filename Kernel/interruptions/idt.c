#include "interruptions.h"

#include <handlers.h>

#define ACS_PRESENT 0x80 /* segmento presente en memoria */
#define ACS_CSEG 0x18    /* segmento de codigo */
#define ACS_DSEG 0x10    /* segmento de datos */
#define ACS_READ 0x02    /* segmento de lectura */
#define ACS_WRITE 0x02   /* segmento de escritura */
#define ACS_IDT ACS_DSEG
#define ACS_INT_386 0x0E /* Interrupt GATE 32 bits */
#define ACS_INT (ACS_PRESENT | ACS_INT_386)

#define ACS_CODE (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#define KERNEL_CS 0x08

#pragma pack(push)
#pragma pack(1)

typedef struct
{
    uint16_t offset_low; // Bit 0 to 15
    uint16_t selector;
    uint8_t zero_low;
    uint8_t access;
    uint16_t offset_middle; // 16 to 31
    uint32_t offset_high;   // 32 to 63
    uint32_t zero_high;
} IDTEntry;

#pragma pack(pop)

static IDTEntry *idt = (void *)0x0;

static void set_idt_entry(int interrupt_number, InterruptionHandler handler, int access)
{
    uint64_t handler_address = (uint64_t)handler;

    idt[interrupt_number].offset_low = handler_address & 0xFFFF;
    idt[interrupt_number].offset_middle = (handler_address >> 16) & 0xFFFF;
    idt[interrupt_number].offset_high = (handler_address >> 32) & 0xFFFFFFFF;

    idt[interrupt_number].selector = KERNEL_CS;
    idt[interrupt_number].zero_low = 0;

    idt[interrupt_number].access = access;
    idt[interrupt_number].zero_high = 0;
}

void initialize_idt()
{
    set_idt_entry(0x00, zero_division_exception_handler, ACS_INT);
    set_idt_entry(0x06, invalid_opcode_exception_handler, ACS_INT);
    set_idt_entry(0x20, tick_handler, ACS_INT);
    set_idt_entry(0x21, keyboard_handler, ACS_INT);
    set_idt_entry(0x80, syscall_handler, ACS_INT);

    // Enable master PIC
    output_byte(0x21, 0xFC); // Only tick and keyboard interruptions enabled for now
    // Enable slave PIC
    output_byte(0xA1, 0xFF);

    set_interrupt_flag();
}
