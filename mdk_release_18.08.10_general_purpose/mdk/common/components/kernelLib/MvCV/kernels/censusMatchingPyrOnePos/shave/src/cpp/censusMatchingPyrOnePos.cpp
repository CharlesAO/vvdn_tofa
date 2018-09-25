#include "censusMatchingPyrOnePos.h"

void mvcvCensusMatchingPyrOnePos(u32 *in1, u32 *in2, u8 *predicted, u8 *out, u32 width)
{
    u32 resultXOR;
    u8 bitCount;
    s32 offsetRight;

    for (u32 positionL = 0; positionL < width; positionL++)
    {
        for (u32 indexR = 0; indexR < 3; indexR++)
        {
            // right img index = (left img index - predicted offset * 2) +/- 1
            // positionL/2 - because predicted line is downsampled with a factor of 2
            // predicted offset * 2 - because predicted offest is computed at half current resolution
            offsetRight = predicted[(u32)(positionL/2)] * 2 + indexR - 1;
            
            if (offsetRight < 0)
            {
                out[positionL * 3 + indexR] = 25; // any value bigger then 25
            }
            else
            {
                resultXOR = *(in1 + positionL) ^ *(in2 + positionL - offsetRight);

                for(bitCount = 0; resultXOR; resultXOR >>= 1)
                {
                    bitCount += resultXOR & 1;
                }

                out[positionL * 3 + indexR] = bitCount;
            }
        }
    }
}
