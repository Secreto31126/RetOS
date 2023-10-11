#include <stdint.h>
#include "videoDriver.h"

typedef struct vbe_mode_info_structure *VBEInfoPtr;

VBEInfoPtr VBE_mode_info = 0x0000000000005C00; // hardcoded

/**
 * assume hexColor in rgb standard, array in brg for some reason idk so we translate in function
*/
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y)
{
    uint8_t *framebuffer = (uint8_t *) VBE_mode_info->framebuffer; // tira warning. Pasas que pasan
    // En el array, cada pixel tiene tres valores (RGB), por lo que la posición en el array es x*3
    uint64_t offset = x * (VBE_mode_info->bpp / 8) + y * VBE_mode_info->pitch; // es 3 lo primero, la cantidad de líneas (y) por el tamaño de una línea lo segundo
    framebuffer[offset]=(hexColor>>16) & 0xFF; //blue
    framebuffer[offset+1]=(hexColor) & 0xFF; //red
    framebuffer[offset+2]=(hexColor>>8) & 0xFF; //green
}

void putPixelStd(char red,char green, char blue, uint64_t x, uint64_t y){
    uint8_t *framebuffer = (uint8_t *) VBE_mode_info->framebuffer; // tira warning. Pasas que pasan
    // En el array, cada pixel tiene tres valores (RGB), por lo que la posición en el array es x*3
    uint64_t offset = x * (VBE_mode_info->bpp / 8) + y * VBE_mode_info->pitch; // es 3 lo primero, la cantidad de líneas (y) por el tamaño de una línea lo segundo
    framebuffer[offset]=blue; //blue
    framebuffer[offset+1]=red; //red
    framebuffer[offset+2]=green; //green
}

void drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, int width, int height){
	for(int i = 0; i < width; i++){
		for(int j = 0; i < height; i++){
			putPixel(hexColor,x+i,y+j);
		}
	}
}