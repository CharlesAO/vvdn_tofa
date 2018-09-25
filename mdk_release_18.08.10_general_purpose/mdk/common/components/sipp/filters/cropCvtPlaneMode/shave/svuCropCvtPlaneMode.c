#include <sipp.h>
#include <stdio.h>
#include <sippShaveMacros.h>
#include <filters/cropCvtPlaneMode/cropCvtPlaneMode.h>

//##########################################################################################
void cropCvtPlaneMode(UInt8** inR, UInt8** inG, UInt8** inB, UInt8** Out, UInt32 width)
{
    UInt32 i,j;

    UInt8* r = inR[0];
    UInt8* g = inG[0];
    UInt8* b = inB[0];
    UInt8* o = Out[0];

    for (i = 0, j = 0; i < width; i+=3, j++)
    {
        o[i]   = r[j];
        o[i+1] = g[j];
        o[i+2] = b[j];
    }
}

//##########################################################################################
void svuCropCvtPlaneMode(SippFilter *fptr)
{
    UInt8 *inputR;
    UInt8 *inputG;
    UInt8 *inputB;

    UInt8 *output;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    UInt32 bpp = sippFilterGetOutputBpp(fptr);
    #else
    UInt32 bpp = sippFilterGetOutputBpp(fptr,0);
    #endif
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inputR = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        inputG = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
        inputB = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

        cropCvtPlaneMode(&inputR, &inputG, &inputB, &output, fptr->sliceWidth << bpp);
      }
  }
