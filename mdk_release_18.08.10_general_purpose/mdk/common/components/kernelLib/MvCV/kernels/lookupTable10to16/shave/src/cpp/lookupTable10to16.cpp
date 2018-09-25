#include "lookupTable10to16.h"

void mvcvLUT10to16(u16** src, u16** dest, const u16* lut, u32 width, u32 height)
{
    unsigned int j;
    u16* in_line = *src;
    u16* out_line = *dest;
	height+=1;
    for (j = 0; j < width; j++)
    {
    	out_line[j] = lut[in_line[j]&0x03FF];
    }
}
