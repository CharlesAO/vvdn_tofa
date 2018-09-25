///
/// @file      mixMedian.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     mix median - Mix two 8-bit planes according to a third 8-bit reference plane
///
#include <mv_types.h>
#include "mixMedian.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static inline u8 clampU8(float in)
{
    in += 0.5; //round to neareset int
    if(in< 0)  { return 0; }
    if(in>255) { return 255; }
               return (u8)in;
}

/// mix median - Mix two 8-bit planes according to a third 8-bit reference plane
/// @param[out]        - first unsigned int contain number of valid points in the line, after score for every point
/// @param[in] in0       - array of pointers to input line of the image, 3 planes
/// @param[in] in1       - array of pointers to input line of the image, 3 planes
/// @param[in] ref       - array of pointers to input line of the image, 1 planes, reference
/// @param[in] offset    - offset
/// @param[in] slope     - slope
/// @param[in] width     - width of the input lines
void mvispMixMedian(u8 *out[3], u8 *in0[3], u8 *in1[3], u8 *ref, float offset, float slope, unsigned int width)
{
    unsigned int p = 0;
    unsigned int i = 0;
    for (p = 0; p < 3; p++) 
    {
        for(i = 0; i < width; i++) 
        {
            float alpha;
            alpha = ((float)ref[i] / 255.0f + offset) * slope;;
            alpha = alpha > 1.0f ? 1.0f : alpha;
            alpha = alpha < 0.0f ? 0.0f : alpha;
            out[0][i] = clampU8(alpha * (in0[0][i] - in1[0][i]) + in1[0][i]);
            out[1][i] = clampU8(alpha * (in0[1][i] - in1[1][i]) + in1[1][i]);
            out[2][i] = clampU8(alpha * (in0[2][i] - in1[2][i]) + in1[2][i]);
        }
    }
}
