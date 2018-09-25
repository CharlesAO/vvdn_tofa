#include "bitwiseAnd.h"

void mvcvBitwiseAnd(u8** src1Addr, u8** src2Addr, u8** destAddr, u32 width)
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
		dest[i] = src1[i] & src2[i]; 
	}
	return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvBitwiseAnd_opt(unsigned char ** src1Addr, unsigned char ** src2Addr, unsigned char ** destAddr, unsigned int width) {
	volatile uchar8 * src1 = (volatile uchar8 *) *src1Addr;
	volatile uchar8 * src2 = (volatile uchar8 *) *src2Addr;
	volatile uchar8 * dest = (volatile uchar8 *) *destAddr;

	for(unsigned int i = 0; i < width/8; i+=8) {
    uchar8 a0 = src1[i];
    uchar8 a1 = src1[i+1];
    uchar8 a2 = src1[i+2];
    uchar8 a3 = src1[i+3];
    uchar8 a4 = src1[i+4];
    uchar8 a5 = src1[i+5];
    uchar8 a6 = src1[i+6];
    uchar8 a7 = src1[i+7];
    
    uchar8 b0 = src2[i];
    uchar8 b1 = src2[i+1];
    uchar8 b2 = src2[i+2];
    uchar8 b3 = src2[i+3];
    uchar8 b4 = src2[i+4];
    uchar8 b5 = src2[i+5];
    uchar8 b6 = src2[i+6];
    uchar8 b7 = src2[i+7];
    
    dest[i] = a0 & b0;
    dest[i+1] = a1 & b1;
    dest[i+2] = a2 & b2;
    dest[i+3] = a3 & b3;
    dest[i+4] = a4 & b4;
    dest[i+5] = a5 & b5;
    dest[i+6] = a6 & b6;
    dest[i+7] = a7 & b7;
  }
}

#endif
