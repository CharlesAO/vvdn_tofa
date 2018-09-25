#include "computeADPyrOnePos.h"
#include "stdio.h"
#include "stdlib.h"

void mvcvComputeADPyrOnePos(u8* inputLeft, u8* inputRight, u8* output, u32 width)
{
	int disparities = 3;
	int relativePos = 1;
	int posL, posR;

	// compute absolute difference between each pixel (x, y) from left image and 3 neighbour pixels (x-1, y), (x, y), (x+1, y) from right image
    for (posL = 0; posL < (int)width; posL++)
    {
        for (posR = 0; posR < disparities; posR++)
        {
            output[posL * disparities + posR] = abs(inputLeft[posL] - inputRight[posL + posR - relativePos]);
        }
    }
}
