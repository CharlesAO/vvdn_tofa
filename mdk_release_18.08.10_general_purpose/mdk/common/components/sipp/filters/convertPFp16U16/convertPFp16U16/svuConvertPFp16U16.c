#include "sipp.h"
#include "sippMacros.h"
#include "filters/convertPFp16U16/convertPFp16U16.h"

#define MAX_U16_VAL ((1<<10)) // this depend by nr of pixels 10 for now - have to be a parameter at some point

//##########################################################################################
void svuConvertPFp16U16 (SippFilter *fptr)
{
    UInt32 i;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //Input RGBW lines (parent[0] must be the 3xplane RGBW)
        half *inW = (half *)getInPtr(fptr, 0, iterNum, 0, 0);

        //Output lines RGBW
        UInt16 *outW = (UInt16 *)getOutPtr(fptr, iterNum, 0);

        //the actual processing
        for(i=0; i<fptr->sliceWidth; i++)
        {
            outW[i] = (UInt16)(inW[i] * (half)(float)MAX_U16_VAL);
        }
    }
}
