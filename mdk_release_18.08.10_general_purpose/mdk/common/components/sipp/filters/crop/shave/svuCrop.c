#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/crop/crop.h>

//##########################################################################################
void svuCrop(SippFilter *fptr)
{
    UInt32 i,p;
    UInt8  *in;
    UInt8  *out;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    UInt32 bpp = sippFilterGetOutputBpp(fptr);
    #else
    UInt32 bpp = sippFilterGetOutputBpp(fptr,0);
    #endif
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        for(p=0; p < numPlanes; p++)
        {
            //Get pointers
            in  = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, p);
            out = (UInt8 *)getOutPtr(fptr, iterNum,p);

            //Do current plane (bpp is as pow or 2, so need to shift !)
            for(i=0; i<fptr->sliceWidth << bpp; i++)
            {
                out[i] = in[i];
            }
        }
    }
}

