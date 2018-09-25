#include <sipp.h>
#include <sippShaveMacros.h>

//##########################################################################################
void svuAvg(SippFilter *fptr)
{
    UInt32 x;

    UInt8  *out;
    UInt8  *inA;
    UInt8  *inB;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        out = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        inA = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        inB = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);

        for(x=0; x<fptr->sliceWidth; x++)
        {
            out[x] = ((int)inA[x] + (int)inB[x] + 1)>>1;
        }
    }
}
