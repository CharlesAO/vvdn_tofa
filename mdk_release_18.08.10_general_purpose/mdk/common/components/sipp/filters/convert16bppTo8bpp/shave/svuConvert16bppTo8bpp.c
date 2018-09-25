#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convert16bppTo8bpp/convert16bppTo8bpp.h>

#define CLAMPU8(x) (x < 0 ? 0 : x > 255 ? 255 : x)

//##########################################################################################
void svuConvert16bppTo8bpp (SippFilter *fptr)
{
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //Input RGBW lines (parent[0] must be the 3xplane RGBW)
        UInt16 *inR   = (UInt16 *)getInPtr(fptr, 0, iterNum, 0, 0);
        UInt16 *inG   = (UInt16 *)getInPtr(fptr, 0, iterNum, 0, 1);
        UInt16 *inB   = (UInt16 *)getInPtr(fptr, 0, iterNum, 0, 2);
        UInt16 *inW   = (UInt16 *)getInPtr(fptr, 0, iterNum, 0, 3);

        //Output lines RGBW
        UInt8  *outR = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        UInt8  *outG = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        UInt8  *outB = (UInt8 *)getOutPtr(fptr, iterNum, 2);
        UInt8  *outW = (UInt8 *)getOutPtr(fptr, iterNum, 3);

        UInt32 i;
        //the actual processing
        for(i=0; i<fptr->sliceWidth; i++)
        {
            outR[i] =  CLAMPU8(inR[i] >> 2);
            outG[i] =  CLAMPU8(inG[i] >> 2);
            outB[i] =  CLAMPU8(inB[i] >> 2);
            outW[i] =  CLAMPU8(inW[i] >> 2);
        }
    }
}
