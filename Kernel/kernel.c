#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>

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

uint32_t drawRedLine(int x, int y)
{
	if (x < 10 && y < 1)
		return 0xFF000000 | HEX_RED;
	return 0;
}
uint32_t drawRedCircle(int x, int y)
{
	x -= 100;
	y -= 100;
	if ((x * x + y * y) < 10000)
		return 0xFF000000 | HEX_RED;
	return 0;
}
uint32_t drawAnEllipse(int x, int y)
{
	x -= 100;
	y -= 25;
	if ((x * x + 16 * (y * y)) < 10000)
		return 0xFF000000 | ((0xFFFFFF * x * y) & 0x00FFFFFF);
	return 0;
}
uint32_t drawACanvas(int x, int y)
{
	return 0xFF000000 | (((0xFFFFFF * x * y) & 0x00FFFFFF)) ^ 0x00FFFFFF;
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

	drawRectangle(0xFF0000, 10, 10, 10, 10);
	drawRectangle(0x00FF00, 20, 10, 10, 10);
	drawRectangle(0x0000FF, 30, 10, 10, 10);
	/*
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			drawShape(drawACanvas, 100 + i * 200, 100 + j * 50, 1000, 1000);
			*/
	drawShape(drawAnEllipse, 0, 0, 1024, 768);
	drawScaledShape(drawAnEllipse, 0, 0, 1024, 768, 2, 5);
	return 0;
}
