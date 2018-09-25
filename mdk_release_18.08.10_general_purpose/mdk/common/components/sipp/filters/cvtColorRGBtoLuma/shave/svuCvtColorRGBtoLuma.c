#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorRGBtoLuma/cvtColorRGBtoLuma.h>

/// cvtColorKernel to conversion RGB to Luma
///@param[in]  inR input R channel
///@param[in]  inG input G channel
///@param[in]  inB input B channel
///@param[out] yOut output Y channel

#ifdef SIPP_USE_MVCV
void mvcvCvtColorRGBtoLuma_asm(UInt8** inR, UInt8** inG, UInt8** inB, UInt8** yOut, UInt32 width);
#else
void cvtColorRGBtoLuma        (UInt8** inR, UInt8** inG, UInt8** inB, UInt8** yOut, UInt32 width)
{
    UInt32 i;
    UInt8* r = inR[0];
    UInt8* g = inG[0];
    UInt8* b = inB[0];
    UInt8* yo = yOut[0];
    int y;

    for (i = 0; i < width; i++)
    {
        y = 0.299f * r[i] + 0.587f * g[i] + 0.114f * b[i];
        yo[i] = (UInt8) y > 255 ? 255 : y < 0 ? 0 : y;
    }
}
#endif

//#######################################################################################
void svuCvtColorRGBtoLuma(SippFilter *fptr)
{
    UInt8 *inputR;
    UInt8 *inputG;
    UInt8 *inputB;
    UInt8 *output;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inputR = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        inputG = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
        inputB = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 2);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorRGBtoLuma_asm(&inputR, &inputG, &inputB, &output, fptr->sliceWidth);
    #else
        cvtColorRGBtoLuma(&inputR, &inputG, &inputB, &output, fptr->sliceWidth);
    #endif
    }
}