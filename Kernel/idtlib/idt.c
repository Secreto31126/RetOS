#include <idt.h>
#include <naiveConsole.h>

#define INTERRUPT_GATE_TYPE 0x8e
#define KERNEL_CS 0x08
#define KEYBOARD_INTERRUPT_NUMBER 33

/**
 * @link https://github.com/tinchovictory/BasicOperatingSystem/blob/master/x64barebones/Kernel/interruptions.c
 */
typedef struct
{
    uint16_t offset_low; // bit 0..15
    uint16_t selector;
    uint8_t zero_low;
    uint8_t type;
    uint16_t offset_middle; // 16..31
    uint32_t offset_high;   // 32..63
    uint32_t zero_high;
} IDTEntry;

static IDTEntry *idt = (void *)0x0;

static void set_idt_entry(int interrupt_number, InterruptionHandler handler, int type)
{
    uint64_t handler_address = (uint64_t)handler;

    idt[interrupt_number].offset_low = handler_address & 0xFFFF;
    idt[interrupt_number].offset_middle = (handler_address >> 16) & 0xFFFF;
    idt[interrupt_number].offset_high = (handler_address >> 32) & 0xFFFFFFFF;

    idt[interrupt_number].selector = KERNEL_CS;
    idt[interrupt_number].zero_low = 0;

    idt[interrupt_number].type = type;
    idt[interrupt_number].zero_high = 0;
}

static void keyboard_handler()
{
    uint8_t scancode;

    // Read the scancode from the keyboard controller
    scancode = input_byte(0x60);

    // Do something with the scancode
    ncPrintChar(scancode, 0x1F);

    // Acknowledge the interrupt to the PIC
    output_byte(0x20, 0x20);
}

void initialize_idt()
{
    memset(&idt, 0, sizeof(IDTEntry) * 256);

    set_idt_entry(KEYBOARD_INTERRUPT_NUMBER, keyboard_handler, INTERRUPT_GATE_TYPE);
    set_idt_entry(0x80, syscall_handler, INTERRUPT_GATE_TYPE);
}
