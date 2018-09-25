#include "lookupTable10to8.h"

void mvcvLUT10to8(u16** src, u8** dest, const u8* lut, u32 width, u32 height)
{
	unsigned int j;
	u16* in_line = *src;
	u8* out_line = *dest;
	height+=1;
	for (j = 0; j < width; j++)
	{
		out_line[j] = lut[in_line[j]&0x03FF];
	}
}
