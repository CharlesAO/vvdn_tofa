#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/integralImageSumU16U32/integralImageSumU16U32.h>

#define MAX_NR_PLANE_ON_INTEGRAL_IMAGE 8

static UInt32 *previsionOutLnPointer[MAX_NR_PLANE_ON_INTEGRAL_IMAGE];

/// integral image kernel  - this kernel makes the sum of all pixels before it and on the left of it's column  ( this particular case makes sum of pixels in u32 format)
/// @param[out] out        - array of pointers for output lines  U32 format
/// @param[in] in          - array of pointers to input lines U16 data format
/// @param[runNr]in        - output line number
/// @param[in] sum         - pointer to prevision generated line, will be used for calculate current pixels, and will be updated at the end to point to new output generated line
/// @param[in] width       - width of input line
#ifdef SIPP_USE_MVCV
void mvcvIntegralImageSumU16U32_asm(UInt32* out, UInt16* in, UInt32 runNr, UInt32 **previsionOutLnPointer, UInt32 width);
#else
void integralImageSumU16U32(UInt32* out, UInt16* in, UInt32 runNr, UInt32 **previsionOutLnPointer, UInt32 width)
{
    if (0 == runNr)
    {
        UInt32 x;
        UInt32 onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x];
            out[x] = onLineSum;
        }
        *previsionOutLnPointer = out;
    }
    else
    {
        UInt32 x;
        UInt32 onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x];
            out[x] = (*previsionOutLnPointer)[x] + onLineSum;
        }
        *previsionOutLnPointer = out;
    }
}
#endif

void svuIntegralImageSumU16U32(SippFilter *fptr)
{
    UInt32 *outLn;
    UInt16 *inLn;
    UInt32 pl;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi       = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo     = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        for (pl = 0; pl < numPlanes; pl ++)
        {
            //the input lines
            inLn = (UInt16 *)getInPtr(fptr, 0, iterNum, 0, pl);
            //the output line
            outLn = (UInt32 *)getOutPtr(fptr, iterNum, pl);
    #ifdef SIPP_USE_MVCV
            mvcvIntegralImageSumU16U32_asm(outLn, inLn, runNo, &previsionOutLnPointer[pl], fptr->sliceWidth);
    #else
            integralImageSumU16U32(outLn, inLn, runNo, &previsionOutLnPointer[pl], fptr->sliceWidth);
    #endif
        }
    }
}
