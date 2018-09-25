#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/thresholdBinaryRange/thresholdBinaryRange.h>

/// thresholdBinaryRange kernel -this kernel set output to 0xFF if source(x,y) is in specified range, otherwise  output = 0.
/// @param[in] in                - array of pointers to input lines
/// @param[out] out              - array of pointers for output lines
/// @param[in] lowerValue        - lowerValue
/// @param[in] upperValue        - upperValue
/// @param[in] width             - width of input line

#ifdef SIPP_USE_MVCV
void mvcvThresholdBinaryRange_asm(UInt8** in, UInt8** out, UInt8 lowerValue, UInt8 upperValue, UInt32 width);
#else
void thresholdBinaryRange        (UInt8** in, UInt8** out, UInt8 lowerValue, UInt8 upperValue, UInt32 width)
{
    int i;
    for (i = 0; i < width; i++)
    {
        if ((in[0][i] > lowerValue) && (in[0][i] < upperValue)){
            out[0][i] = 255;
        }
        else {
            out[0][i] = 0;
        }
    }
}
#endif

void svuThresholdBinaryRange(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[1];

    ThresholdBinaryRangeParam *param = (ThresholdBinaryRangeParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        iline[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvThresholdBinaryRange_asm(iline, &output, param->lowerValue, param->upperValue, fptr->sliceWidth);
    #else
        thresholdBinaryRange(iline, &output, param->lowerValue, param->upperValue, fptr->sliceWidth);
    #endif
    }
}
