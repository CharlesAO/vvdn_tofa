#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/integralImageSumF32M2/integralImageSumF32M2.h>

#define MAX_NR_PLANE_ON_INTEGRAL_IMAGE 8

static float *previsionOutLnPointer[MAX_NR_PLANE_ON_INTEGRAL_IMAGE];

/// integral image kernel - this kernel makes the sum of all pixels before it and on the left of it's column  ( this particular case makes sum of pixels in f32 format)
/// @param[in] in         - array of pointers to input lines
/// @param[out]out        - array of pointers for output lines
/// @param[in] sum        - sum of previous pixels . for this parameter we must have an array of u32 declared as global and having the width of the line
/// @param[in] width      - width of input line
#ifdef SIPP_USE_MVCV
void mvcvIntegralimage_sum_f32_M2_asm(float* out, UInt8* in, UInt32 runNr ,float** prevPointer, UInt32 width);
#else
void integralimage_sum_f32M2(float* out, UInt8* in, UInt32 runNr ,float** prevPointer, UInt32 width)
{
    if (0 == runNr)
    {
        UInt32 x;
        float onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x];
            out[x] = onLineSum;
        }
        *prevPointer = out;
    }
    else
    {
        UInt32 x;
        float onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x];
            out[x] = (*prevPointer)[x] + onLineSum;
        }
        *prevPointer = out;
    }
}
#endif

void svuIntegralImageSumF32M2(SippFilter *fptr)
{
    float *outLn;
    UInt8 *inLn;
    UInt32 pl;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi       = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo     = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        for (pl = 0; pl < numPlanes; pl ++)
        {
            //the input lines
            inLn = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, pl);
            //the output line
            outLn = (float *)getOutPtr(fptr, iterNum, pl);

    #ifdef SIPP_USE_MVCV
            mvcvIntegralimage_sum_f32_M2_asm(outLn, inLn, runNo, &previsionOutLnPointer[pl], fptr->sliceWidth);
    #else
            integralimage_sum_f32M2(outLn, inLn, runNo, &previsionOutLnPointer[pl], fptr->sliceWidth);
    #endif
        }
    }
}


