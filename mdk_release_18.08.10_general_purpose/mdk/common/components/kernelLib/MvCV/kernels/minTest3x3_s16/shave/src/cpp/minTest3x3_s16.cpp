#include <minTest3x3_s16.h>
#include <mv_types.h>

void mvcvMinTest3x3_s16(s16* inBufferCandidates, s16** inBuffer,
                        u32 minLocationsIn[], u32 minLocationsOut[],
                        u32 minCountIn, u32* minCountOut)
{
    s32 index;
    s16 candidate;
    s32 candidateX;
    s16* lineP[3];
    lineP[0] = inBuffer[0];
    lineP[1] = inBuffer[1];
    lineP[2] = inBuffer[2];
    (*minCountOut) = 0;
    for (index = 0; index < (int)minCountIn; index++)
    {
        candidateX = minLocationsIn[index];
        candidate = inBufferCandidates[candidateX];
        if ((candidate < lineP[0][candidateX -1]) &&
                        (candidate < lineP[0][candidateX]) &&
                        (candidate < lineP[0][candidateX +1]) &&
                        (candidate < lineP[1][candidateX -1]) &&
                        (candidate < lineP[1][candidateX]) &&
                        (candidate < lineP[1][candidateX +1]) &&
                        (candidate < lineP[2][candidateX -1]) &&
                        (candidate < lineP[2][candidateX]) &&
                        (candidate < lineP[2][candidateX +1]))
        {
            /// The candidate is a minimum, add the X coordinate to the list
            minLocationsOut[(*minCountOut)++] = candidateX;
        }
    }
}
