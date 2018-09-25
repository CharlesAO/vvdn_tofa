#include "localMaxMin3x3_fp16.h"
#include <mv_types.h>

void mvcvLocalMaxMin3x3_fp16(half** inBuffer, u32 width,
                             u32 minLocationList[], u32 maxLocationList[],
                             u32* minCount, u32* maxCount)
{
    u32 candidateX;
    half candidate;
    half* lineP[3];
    lineP[0] = inBuffer[0];
    lineP[1] = inBuffer[1];
    lineP[2] = inBuffer[2];
    *minCount = 0;
    *maxCount = 0;
    for (candidateX = 1; candidateX < width; candidateX++)
    {

        /// The value of the pixel which is checked.
        candidate = lineP[1][candidateX];
        if ((candidate < lineP[0][candidateX -1]) &&
                        (candidate < lineP[0][candidateX]) &&
                        (candidate < lineP[0][candidateX +1]) &&
                        (candidate < lineP[1][candidateX -1]) &&
                        (candidate < lineP[1][candidateX +1]) &&
                        (candidate < lineP[2][candidateX -1]) &&
                        (candidate < lineP[2][candidateX]) &&
                        (candidate < lineP[2][candidateX +1]))
        {
            /// The candidate is a minimum, add the X coordinate to the list
            minLocationList[(*minCount)++] = candidateX;
        }
        else if ((candidate > lineP[0][candidateX -1]) &&
                        (candidate > lineP[0][candidateX]) &&
                        (candidate > lineP[0][candidateX +1]) &&
                        (candidate > lineP[1][candidateX -1]) &&
                        (candidate > lineP[1][candidateX +1]) &&
                        (candidate > lineP[2][candidateX -1]) &&
                        (candidate > lineP[2][candidateX]) &&
                        (candidate > lineP[2][candidateX +1]))
        {
            /// The candidate is a maximum, add the X coordinate to the list
            maxLocationList[(*maxCount)++] = candidateX;
        }
    }
}

