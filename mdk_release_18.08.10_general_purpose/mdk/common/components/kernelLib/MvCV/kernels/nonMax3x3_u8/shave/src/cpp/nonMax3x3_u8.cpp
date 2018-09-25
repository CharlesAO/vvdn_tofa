///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API for some required Leon Math functions
///
#include <mv_types.h>
#include "nonMax3x3_u8.h"

void mvcvNonMax3x3_u8(u32 width, u8* corners1, u8* corners2,
						u8* corners3, u16* candOutPos, u32 *candOutCount)
{
    u32 i;
    u16 candidateX;
    u8 candidate;
    u32 out = 0;
    for(i = 0; i < width; i++)
    {
        candidateX = i;
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