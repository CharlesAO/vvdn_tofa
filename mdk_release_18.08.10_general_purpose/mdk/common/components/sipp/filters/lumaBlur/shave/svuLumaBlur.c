#include <sipp.h>
#include <sippShaveMacros.h>

void lumaBlur(UInt8 *inY0, UInt8 *inY1, UInt8 *inY2, UInt8 *outY, int width)
{
    int x;

    //3x3 blur
    for(x=0; x<width; x++)
    {   //div by 16 as sum of kernels = 16
        outY[x] = clampU8((inY0[x-1]*1 + inY0[x]*2 + inY0[x+1]*1+
                           inY1[x-1]*2 + inY1[x]*4 + inY1[x+1]*2+
                           inY2[x-1]*1 + inY2[x]*2 + inY2[x+1]*1)>>4);
    }
}

//##########################################################################################
void svuLumaBlur(SippFilter *fptr)
{
    //I/O pointers
    UInt8 *outY;
    UInt8 *inY[3];
    UInt32 plane;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for(plane=0; plane < numPlanes; plane++)
        {
     #if 0
            inY[0] = (UInt8*)getInPtr(fptr, 0, iterNum, 0, plane);
            inY[1] = (UInt8*)getInPtr(fptr, 0, iterNum, 1, plane);
            inY[2] = (UInt8*)getInPtr(fptr, 0, iterNum, 2, plane);
            outY   = (UInt8*)getOutPtr(fptr, iterNum, plane);
    #else
            //Optimal way to gather IO pointers
            outY = (UInt8*)getPlaneIoPtrs(fptr, 0, iterNum, plane, inY);
    #endif

    #if defined(SIPP_PC)
            lumaBlur(inY[0], inY[1], inY[2], outY, fptr->sliceWidth);
    #elif defined(__myriad2__)
            extern void lumaBlurOpt(UInt8 *inY0, UInt8 *inY1, UInt8 *inY2, UInt8 *outY, int width);
            lumaBlurOpt(inY[0], inY[1], inY[2], outY, fptr->sliceWidth);
    #endif
        }
    }
}
