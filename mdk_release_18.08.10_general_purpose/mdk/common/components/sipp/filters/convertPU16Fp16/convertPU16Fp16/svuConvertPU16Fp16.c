#include "sipp.h"
#include "sippMacros.h"
#include "filters/convertPU16Fp16/convertPU16Fp16.h"

#define MAX_U16_VAL ((1<<10)) // this depend by nr of pixels 10 for now - have to be a parameter at some point

//##########################################################################################
void svuConvertPU16Fp16 (SippFilter *fptr)
{
    UInt32 i;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //Input RGBW lines (parent[0] must be the 3xplane RGBW)
        UInt16 *inW = (UInt16 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //Output lines RGBW
        half *outW = (half *)getOutPtr(fptr, iterNum, 0);

        //the actual processing
        for(i=0; i<fptr->sliceWidth; i++)
        {
            outW[i] = (half)(float)inW[i] / (half)(float)MAX_U16_VAL;
        }
    }
}
