#include "arithmeticSubFp16ToFp16.h"

void mvcvArithmeticSubFp16ToFp16(half** src1Addr, half** src2Addr, half** destAddr, u32 width)
{
	u32 i;
	half* src1;
	half* src2;
	half* dest;

	src1 = *src1Addr;
	src2 = *src2Addr;
	dest = *destAddr;
	
	for(i = 0; i < width; i++)
	{
		dest[i] = src1[i] - src2[i]; 
	}
	return;
}
