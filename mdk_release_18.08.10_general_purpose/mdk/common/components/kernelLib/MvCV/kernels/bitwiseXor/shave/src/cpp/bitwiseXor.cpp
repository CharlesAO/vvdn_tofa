#include "bitwiseXor.h"

void mvcvBitwiseXor(u8** src1Addr, u8** src2Addr, u8** destAddr, u32 width)
{
	u32 i;
	u8* src1;
	u8* src2;
	u8* dest;

	src1 = *src1Addr;
	src2 = *src2Addr;
	dest = *destAddr;
	
	for(i = 0; i < width; i++)
	{
		dest[i] = src1[i] ^ src2[i]; 
	}
	return;
}
