#include <idt.h>

static void *const idtAddress = (void *)0x0;
static void *const idtEndAddress = 0xFFF;

void clearIDT()
{
    memset((void *)idtAddress, 0, idtEndAddress);
}

/**
 * @brief Sets the IDT entry at the given interrupt number to the given handler
 *
 * @param interrupt_number The interruption number
 * @param handler The function callback
 */
void *idtEntry(uint8_t interrupt_number, InterruptionHandler handler)
{
    *((uint32_t *)(idtAddress + interrupt_number * 16)) = (uint32_t)handler;
}
