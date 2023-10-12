#include <stdint.h>

enum HEX_COLORS
{
	HEX_RED = 0xFF0000,
	HEX_GREEN = 0x00FF00,
	HEX_BLUE = 0x0000FF,
};

struct vbe_mode_info_structure
{
	uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;	  // deprecated
	uint8_t window_b;	  // deprecated
	uint16_t granularity; // deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;		   // number of bytes per horizontal line
	uint16_t width;		   // width in pixels
	uint16_t height;	   // height in pixels
	uint8_t w_char;		   // unused...
	uint8_t y_char;		   // ...
	uint8_t planes;
	uint8_t bpp;   // bits per pixel in this mode
	uint8_t banks; // deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__((packed));

typedef uint32_t (*shapeFunction)(int, int);
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void putPixelStd(char red, char green, char blue, uint64_t x, uint64_t y);
void drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, int width, int height);
/***
 * shapeFunction receives coordinates (two integers), and returns a uint32_t of form 0xPPRRGGBB. Where PP is boolean for whether to draw that pixel, and RRGGBB a hexColor
 */
void drawShape(shapeFunction f, int x, int y, int xRange, int yRange);
void drawScaledShape(shapeFunction f, int x, int y, int xRange, int yRange, double xScaleFactor, double yScaleFactor);
