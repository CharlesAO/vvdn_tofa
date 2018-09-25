#include "accumulateSquare.h"

void mvcvAccumulateSquare(u8** srcAddr, u8** maskAddr, fp32** destAddr, u32 width, u32 height)
{
	u32 i,j;
	u8* src;
	u8* mask;
	fp32* dest;

	src = *srcAddr;
	mask = *maskAddr;
	dest = *destAddr;

	for (i = 0; i < height; i++){
		for (j = 0;j < width; j++){
			if(mask[j + i * width]){
				dest[j + i * width] =  dest[j + i * width] + src[j + i * width] * src[j + i * width];
			}
		}
	}
	return;
}

