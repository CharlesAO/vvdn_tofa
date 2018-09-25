#include "bitwiseNot.h"

void mvcvBitwiseNot(u8** srcAddr, u8** destAddr, u32 width)
{
	u32 i;
	u8* src;
	u8* dest;

	src = *srcAddr;
	dest = *destAddr;
	
	for(i = 0; i < width; i++)
	{
		dest[i] = ~src[i]; 
	}
	return;
}
