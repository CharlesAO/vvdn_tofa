#include "minAggregate_line.h"
#include "minKernel32.h"
#include "minKernel64.h"
#include "aggregateCostSGBM32_clamp.h"
#include "aggregateCostSGBM64_clamp.h"
#include <stdlib.h>
#include <stdio.h> 

void mvcvMinAggregate_line(u8* input1, u8* input2, u8* output, u32* penaltyP1, u32* penaltyP2, u32 width, u8 activateClamp, u8 direction, u32 disparities)
{
    u8 min2;
    u32 directionIdx = 0;
    u32 cc;

    for (u32 column = 0; column < width; column++)
    {
        if (direction == 0) //left to right
        {
            directionIdx  = column;
        }
        else // right to left
        {
            directionIdx = (width - 1 - column);
        }
         cc                      = directionIdx * disparities;

    if (disparities == 32)
    {
        u8* matchingCostPathPtr	= &input1[cc];
        u8* matchingCostCurrentPtr	= &input2[cc];
        u8* aggregatedCostPtr		= &output[cc];

        // compute min2
        mvcvMinKernel32(matchingCostPathPtr, &min2);

        // compute min1 and aggregated cost
        mvcvAggregateCostSGBM32_clamp(matchingCostPathPtr, matchingCostCurrentPtr, aggregatedCostPtr, penaltyP1[directionIdx], penaltyP2[directionIdx], min2, activateClamp);

    } 

    if (disparities == 64)
    {
        u8* matchingCostPathPtr	= &input1[cc];
        u8* matchingCostCurrentPtr	= &input2[cc];
        u8* aggregatedCostPtr		= &output[cc];

        // compute min2
        mvcvMinKernel64(matchingCostPathPtr, &min2);

        // compute min1 and aggregated cost
        mvcvAggregateCostSGBM64_clamp(matchingCostPathPtr, matchingCostCurrentPtr, aggregatedCostPtr, penaltyP1[directionIdx], penaltyP2[directionIdx], min2, activateClamp);
    }
    }
}
