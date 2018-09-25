#include "thresholdFilterS16.h"
#include <mvcv_types.h>
#include <math.h>
#include <stdio.h>

/// ThresholdFilter
///@param[in] line        - pointer to the input line
///@param[in] threshold   - threshold value
///@param[in] width       - input line width
///@param[out] indexList  - pointer to the output line
///@param[out] count      - pointer to the width of the output line

void mvcvThresholdFilterS16(s16* line, u32 threshold, u32 width, u32* indexList, u32* count)
{
    u32 candidate;
    *count = 0;
    
    for (candidate = 0; candidate < width; candidate++)
    {
        if ((s32)line[candidate] * (s32)line[candidate] >= (s32)threshold)
        {
            indexList[*count] = candidate;
            *count = *count + 1;
        }
    }
    return;
}
