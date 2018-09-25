///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API for some required Leon Math functions
///
#include <mv_types.h>
#include "nonMax3x3_fp32.h"

void mvcvNonMax3x3_fp32(u16* candPos, u32 candCount, fp32* corners1, fp32* corners2,
						fp32* corners3, u16* candOutPos, u32 *candOutCount)
{
    u32 i;
    u16 candidateX;
    fp32 candidate;
    u32 out = 0;
    for(i = 0; i < candCount; i++)
    {
        candidateX = candPos[i];
        candidate = corners2[candidateX];
        if((corners1[candidateX - 1]	< candidate) &&
           (corners1[candidateX]     	< candidate) &&
           (corners1[candidateX + 1] 	< candidate) &&
           (corners2[candidateX - 1] 	< candidate) &&
           (corners2[candidateX + 1] 	< candidate) &&
           (corners3[candidateX - 1] 	< candidate) &&
           (corners3[candidateX]     	< candidate) &&
           (corners3[candidateX + 1] 	< candidate))
        {
            candOutPos[out++] = candidateX;
        }
    }
    *candOutCount = out;
}
