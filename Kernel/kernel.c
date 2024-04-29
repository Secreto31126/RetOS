#include <stdint.h>
#include <lib.h>
#include <interruptions.h>
#include <modules.h>
#include <console.h>
#include <localization.h>
#include <memory.h>
#include <proc.h>
#include <exec.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
static void *const tomyland = (void *)0x600000;

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void initializeKernelBinary()
{
	char buffer[10];
	ncPrint("Stack: ");
	ncPrintHex((uint64_t)buffer);
	ncPrint("\n");

	ncPrint("CPU Vendor: ");
	ncPrint(cpuVendor(buffer));
	ncNewline();
	ncNewline();

	ncPrint("[Loading memory manager]");
	init_memory_manager();
	ncPrint(" [Done]\n");

	ncPrint("[Loading modules]\n");
	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress,
		tomyland,
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]\n");

	ncPrint("[Initializing kernel's binary]");

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("\n\ttext: 0x");
	ncPrintHex((uint64_t)&text);
	ncPrint("\n\trodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncPrint("\n\tdata: 0x");
	ncPrintHex((uint64_t)&data);
	ncPrint("\n\tbss: 0x");
	ncPrintHex((uint64_t)&bss);

	ncPrint("\n[Done]\n\n");

	ncPrint("Setting OS's language to ES_AR");
	set_language(ES_AR);
	ncPrint(" [Done]\n");

	ncPrint("Initializing kernel's IDT");
	initialize_idt();
	ncPrint(" [Done]\n");

	ncClear();

	// Print it here before the scheduler starts
	ncPrint("\nRSP\t\tPID\tN_PID\tN_RSP\n");
	set_interrupt_flag();
}

void start_userland()
{
	kernel_fork();
	ncPrint("Running PID: ");
	ncPrintDec(get_pid());
	ncPrint("\n");
	if (!get_pid())
	{
		ncPrint("I'm the parent (Kernel)\n");

		while (1)
			halt_once();
	}
	else
	{
		ncPrint("I'm the child (Future Userland)\n");

		ncPrint("Executing userland code\n");
		execv("tomyland", NULL);
		ncPrint("Error executing userland code\n");

		while (1)
			halt_once();
	}
}
