#include <maxTest3x3_fp16.h>
#include <mv_types.h>

void mvcvMaxTest3x3_fp16(half* inBufferCandidates, half** inBuffer, u32 width,
                     u32 maxLocationsIn[], u32 maxLocationsOut[],
                     u32 maxCountIn, u32* maxCountOut)
{
    s32 index;
    half candidate;
    s32 candidateX;
    width+=1;
    half* lineP[3];
    lineP[0] = inBuffer[0];
    lineP[1] = inBuffer[1];
    lineP[2] = inBuffer[2];
    *maxCountOut = 0;
    for (index = 0; index < (int)maxCountIn; index++)
    {
        candidateX = maxLocationsIn[index];
        candidate = inBufferCandidates[candidateX];
        if ((candidate > lineP[0][candidateX -1]) &&
                        (candidate > lineP[0][candidateX]) &&
                        (candidate > lineP[0][candidateX +1]) &&
                        (candidate > lineP[1][candidateX -1]) &&
                        (candidate > lineP[1][candidateX]) &&
                        (candidate > lineP[1][candidateX +1]) &&
                        (candidate > lineP[2][candidateX -1]) &&
                        (candidate > lineP[2][candidateX]) &&
                        (candidate > lineP[2][candidateX +1]))
        {
            /// The candidate is a maximum, add the X coordinate to the list
            maxLocationsOut[(*maxCountOut)++] = candidateX;
        }
    }
}
