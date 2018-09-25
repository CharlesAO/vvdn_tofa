#include "censusMatchingPyrOnePosWindow.h"
#include "stdio.h"

void mvcvCensusMatchingPyrOnePosWindow(u32 *in1, u32 *in2, u8 *predicted, u8 *out, u32 width)
{
    u32 resultXOR;
    u8 bitCount;
    s32 offsetRight;

    u32 wta_window = 3;
    u32 cost_window = wta_window + 2;
    u32 relativePos = cost_window / 2;

    for (u32 positionL = 0; positionL < width; positionL++)
    {
        //std::cout << "Pixel " << positionL << std::endl;
        for (u32 indexR = 0; indexR < cost_window; indexR++)
        {
            // right img index = (left img index - predicted offset * 2) +/- 1
            // positionL/2 - because predicted line is downsampled with a factor of 2
            // predicted offset * 2 - because predicted offest is computed at half current resolution
            offsetRight = predicted[(u32)(positionL/2)] * 2 + indexR - relativePos;         
            if (offsetRight < 0)
            {
                out[positionL * cost_window + indexR] = 25; // any value bigger then 25
            }
            else
            {
                resultXOR = *(in1 + positionL) ^ *(in2 + positionL - offsetRight);

                for(bitCount = 0; resultXOR; resultXOR >>= 1)
                {
                    bitCount += resultXOR & 1;
                }
                out[positionL * cost_window + indexR] = bitCount;
            }
        }
    }
}
