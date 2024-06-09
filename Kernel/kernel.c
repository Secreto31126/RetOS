#include <stdint.h>
#include <lib.h>
#include <interruptions.h>
#include <modules.h>
#include <console.h>
#include <localization.h>
#include <memory.h>
#include <proc.h>
#include <mman.h>
#include <unistd.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
static void *const init = (void *)0x600000;
static void *const cat = (void *)0x605000;
static void *const wc_module = (void *)0x610000;
static void *const filter = (void *)0x615000;
static void *const loop = (void *)0x620000;
static void *const grep = (void *)0x625000;
static void *const sing = (void *)0x630000;
static void *const less = (void *)0x635000;
static void *const phylos = (void *)0x640000;
static void *const tests = (void *)0x645000;

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *initializeKernelBinary()
{
	char buffer[10];
	ncPrint("CPU Vendor: ");
	ncPrint(cpuVendor(buffer));
	ncNewline();
	ncNewline();

	ncPrint("Loading memory manager");
	init_memory_manager();
	ncPrint(" [Done]\n");

	ncPrint("[Loading modules]\n");
	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress,
		init,
		cat,
		wc_module,
		filter,
		loop,
		grep,
		sing,
		less,
		phylos,
		tests,
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

	ncPrint("Initializing idle process with stack at 0x");
	void *rsp = create_process_idle();
	ncPrint(" [Done]\n");

	ncClear();

	return rsp;
}

void idle(pid_t child_pid)
{
	if (!child_pid)
	{
		ncPrintDec(getpid());
		ncPrint(": I will be init!\n"); // 🥚
		ncPrint(": Huh, I swear I used to be called something else\n");

		char *argv[] = {"I'm in Userland!", "Hell yeah!", ":]", NULL};
		ncPrintDec(execv("init", argv));
		ncPrint(" error: Failed to start userland\n");
	}
	else
	{
		if (child_pid < 0)
		{
			ncPrint("Error: Failed the master fork\n");
			return;
		}

		ncPrintDec(getpid());
		ncPrint(": I'm idle!\n");

		while (1)
		{
			halt_once();
		}
	}
}
