#include "localMaxMin3x3_s16.h"
#include <mv_types.h>

void mvcvLocalMaxMin3x3_s16( s16** inBuffer, u32 candidateLocationIn[],
                              u32 countIn, u32 minLocationList[], u32 maxLocationList[],
                              u32* minCount, u32* maxCount)
{
    s32 i;
    s32 candidateX;
    s16 candidate;
    s16* lineP[3];
    lineP[0] = inBuffer[0];
    lineP[1] = inBuffer[1];
    lineP[2] = inBuffer[2];
    *minCount = 0;
    *maxCount = 0;
    for (i = 0; i < (int)countIn; i++)
    {
        candidateX = candidateLocationIn[i];
        /// The value of the pixel which is checked.
        candidate = lineP[1][candidateX];
        // Valgrind may give a warning here if candidate position is 0. We just read those values, so we can live with this
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

