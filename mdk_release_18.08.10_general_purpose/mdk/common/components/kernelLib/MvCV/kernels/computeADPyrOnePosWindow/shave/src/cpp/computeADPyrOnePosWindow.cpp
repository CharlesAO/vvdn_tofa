#include "computeADPyrOnePosWindow.h"

#include "stdio.h"
#include "stdlib.h"
#include <iostream>

void mvcvComputeADPyrOnePosWindow(u8* inputLeft, u8* inputRight, u8* output, u32 width)
{
    int wta_window = 3;
    int cost_window = wta_window + 2;
    int relativePos = cost_window / 2;

    int posL, posR;

    // compute absolute difference between each pixel (x, y) from left image and 3 neighbour pixels (x-1, y), (x, y), (x+1, y) from right image
    for (posL = 0; posL < (int)width; posL++)
    {
        //std::cout << "Computing AD for " << posL << std::endl;
        for (posR = 0; posR < cost_window; posR++)
        {
            //std::cout << "OffsetRight:" << posR - relativePos << std::endl;
            output[posL * cost_window + posR] = abs(inputLeft[posL] - inputRight[posL + posR - relativePos]);
        }
    }
}
