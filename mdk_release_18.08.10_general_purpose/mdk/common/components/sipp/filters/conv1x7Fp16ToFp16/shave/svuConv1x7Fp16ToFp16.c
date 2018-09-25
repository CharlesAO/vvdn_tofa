#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv1x7Fp16ToFp16/conv1x7Fp16ToFp16.h>

/// Convolution 1x7 kernel fp16
/// @param[in] in           - pointer to input line
/// @param[out] out         - pointer to output line
/// @param[in] conv         - array of values from convolution
/// @param[in] inWidth      - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution1x7Fp16ToFp16_asm(half *in, half *out, half *conv, UInt32 inWidth);
#else
void Convolution1x7Fp16ToFp16        (half *in, half *out, half *conv, UInt32 inWidth)
{
    UInt32   i, y;
    half sum = 0.0;

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (y = 0; y < 7; y++)
        {
            half val = (half)((float)in[y - 3]);
            sum += val * conv[y];
        }
        in++;

        if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f;
        out[i] = sum;
    }
}
#endif

void svuConv1x7Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline;
    Conv1x7Fp16ToFp16Param *param = (Conv1x7Fp16ToFp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the input line
        iline = (half*)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (half*)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvConvolution1x7Fp16ToFp16_asm(iline, output,  (half*)param->cMat, fptr->sliceWidth);
    #else
        Convolution1x7Fp16ToFp16(iline, output,  (half*)param->cMat, fptr->sliceWidth);
    #endif
    }
}
