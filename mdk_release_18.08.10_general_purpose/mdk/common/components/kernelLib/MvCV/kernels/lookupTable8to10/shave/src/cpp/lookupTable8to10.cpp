#include "lookupTable8to10.h"

void mvcvLUT8to10(u8** src, u16** dest, const u16* lut, u32 width)
{
    unsigned int j;
    u8* in_line  = *src;
    u16* out_line = *dest;
    for (j = 0; j < width; j++)
    {
    	out_line[j] = (u16)(0x3ff & (lut[in_line[j]]));
    }
}
