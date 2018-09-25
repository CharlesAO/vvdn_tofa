#include "lookupTable8to8.h"

void mvcvLUT8to8(u8** src, u8** dest, const u8* lut, u32 width, u32 height)
{
    unsigned int j;
    u8* in_line  = *src;
    u8* out_line = *dest;
	height+=1;
    for (j = 0; j < width; j++)
    {
    	out_line[j] = lut[in_line[j]];
    }
}
