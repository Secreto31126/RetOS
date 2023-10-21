#include <stdint.h>
#include <lib.h>
#include <interruptions.h>
#include <modules.h>
#include <console.h>
#include <localization.h>
#include <video.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void *)((uint64_t)&endOfKernel + PageSize * 8 // The size of the stack itself, 32KiB
					- sizeof(uint64_t)					  // Begin at the top of the stack
	);
}

void *initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("Setting OS's language to ES_AR");
	set_language(ES_AR);
	ncPrint(" [Done]");

	ncPrint("Initializing kernel's IDT");
	initialize_idt();
	ncPrint(" [Done]");

	ncClear();

	return getStackBase();
}

int main()
{
	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char *)sampleDataModuleAddress);
	ncNewline();
	ncPrint("[Finished]");

	ncClear();

	// drawShape(redRectangleFigure, 100, 10, 100, 100);
	// drawShape(greenRectangleFigure, 200, 10, 100, 100);
	// drawShape(blueRectangleFigure, 300, 10, 100, 100);

	drawShape(RetOSbackground, 0, 0, get_width(), get_height());

	// uint16_t w = get_width();
	// uint16_t h = get_height();

	// for (int i = 0; i < h / 1 % 10; i++)
	// {
	// 	if (i % 2)
	// 	{
	// 		drawShape(redRectangleFigure, i * 5, i % 5 * 10, 10, 10);
	// 	}
	// 	else
	// 	{
	// 		drawShape(greenRectangleFigure, i * 5, i % 5 * 10, 10, 10);
	// 	}
	// }

	dump_regs();
	ncPrint(dump_reg_string);

	return 0;
}
