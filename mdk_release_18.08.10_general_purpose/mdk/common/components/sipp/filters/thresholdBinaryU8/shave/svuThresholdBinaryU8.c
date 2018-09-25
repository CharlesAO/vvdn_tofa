#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/thresholdBinaryU8/thresholdBinaryU8.h>

/// thresholdBinaryU8 kernel -this kernel set output to 0 if threshold value is less then input value and to 0xFF if threshold value is greater then input value
/// @param[in] in                - array of pointers to input lines
/// @param[out]out               - array of pointers for output lines
/// @param[threshold]threshold   - threshold value
/// @param[in] width             - width of input line

#ifdef SIPP_USE_MVCV
void mvcvThresholdBinaryU8_asm(UInt8** in, UInt8** out, UInt8 threshold, UInt32 width);
#else
void thresholdBinaryU8   (UInt8** in, UInt8** out, UInt8 threshold, UInt32 width)
{
    int i;
    for (i = 0; i < width; i++)
    {
        if (in[0][i] > threshold)
        {
            out[0][i] = 255;
        }
        else
        {
            out[0][i] = 0;
        }
    }
}
#endif

void svuThresholdBinaryU8(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[1];

    ThresholdBinaryU8Param *param = (ThresholdBinaryU8Param*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        iline[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvThresholdBinaryU8_asm(iline, &output, param->threshold, fptr->sliceWidth);
    #else
        thresholdBinaryU8(iline, &output, param->threshold, fptr->sliceWidth);
    #endif
    }
}
