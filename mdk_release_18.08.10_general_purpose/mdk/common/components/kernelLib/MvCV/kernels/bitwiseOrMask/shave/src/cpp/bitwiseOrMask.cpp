#include "bitwiseOrMask.h"

void mvcvBitwiseOrMask(u8** src1Addr, u8** src2Addr, u8** destAddr, u8** maskAddr, u32 width)
{
	u32 i;
	u8* src1;
	u8* src2;
	u8* mask;
	u8* dest;

	src1 = *src1Addr;
	src2 = *src2Addr;
	mask = *maskAddr;
	dest = *destAddr;

	for (i = 0; i < width; i++)
	{
		if(mask[i])
		{
			dest[i] = src1[i] | src2[i];
		}
	}
	return;
}
