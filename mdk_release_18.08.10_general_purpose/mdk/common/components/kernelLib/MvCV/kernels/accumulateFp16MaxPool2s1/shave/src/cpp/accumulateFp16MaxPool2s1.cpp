#include "accumulateFp16MaxPool2s1.h"
#include <stdlib.h>

void mvcvAccumulateFp16MaxPool2s1(half** dst, half** input, u32 width)

{
    u32 i;
	half sum1, sum2, sumx;
    for (i = 0; i < width; i++)
    {
		sum1 = dst[0][i+0] + input[0][i+0]; 
		sum2 = dst[0][i+1] + input[0][i+1]; 
		
		sumx = sum1;
        if(sum2 > sumx) sumx = sum2;
		
        dst[0][i] = sumx;
    }
    return;
}
