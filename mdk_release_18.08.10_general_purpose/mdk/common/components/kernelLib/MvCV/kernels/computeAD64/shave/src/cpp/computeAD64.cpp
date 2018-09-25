#include "computeAD64.h"
#include "stdio.h"
#include "stdlib.h"


void mvcvComputeAD64(u8* inputLeft, u8* inputRight, u8* output, u32 width, u32 flag)
{
    int disparities = 64;
    int posL, posR;

   // compute absolute difference between each pixel from left image and 64 left pixels from right image
    for (posL = 0; posL < (int)width; posL++)
    {
        for (posR = 0; posR < disparities; posR++)
        {
            if(flag == 0)
            {
                output[posL * disparities + posR] = abs(inputLeft[posL] - inputRight[posL - posR]);
            }
            else
            {
                output[posL * disparities + posR] = abs(inputLeft[posL] - inputRight[posL + posR]);
            }
        }
    }
}