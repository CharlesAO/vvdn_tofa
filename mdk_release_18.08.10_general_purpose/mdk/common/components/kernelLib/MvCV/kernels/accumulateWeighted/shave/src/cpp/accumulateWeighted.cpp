#include "accumulateWeighted.h"

void mvcvAccumulateWeighted(u8** srcAddr, u8** maskAddr, fp32** destAddr,u32 width, fp32 alpha)
{
	u32 i;
	u8* src;
	u8* mask;
	fp32* dest;
	
	src = *srcAddr;
	mask = *maskAddr;
	dest = *destAddr;
	
	for (i = 0;i < width; i++)
	{
		if(mask[i]){
				dest[i] =  (1 - alpha) * dest[i] + alpha * src[i];
		}		
	}
	
	return;
}
