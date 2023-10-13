#include <idt.h>

static uint64_t *const idtAddress = (void *)0x0;
static uint64_t *const idtEndAddress = (void *)0xFFF;

void clearIDT()
{
    memset(idtAddress, 0, (uint64_t)idtEndAddress / 16);
}

/**
 * @brief Sets the IDT entry at the given interrupt number to the given handler
 *
 * @param interrupt_number The interruption number
 * @param handler The function callback
 */
void *idtEntry(uint8_t interrupt_number, InterruptionHandler handler)
{
    idtAddress[interrupt_number] = (uint64_t)handler;
    return idtAddress + interrupt_number * sizeof(uint64_t);
}
