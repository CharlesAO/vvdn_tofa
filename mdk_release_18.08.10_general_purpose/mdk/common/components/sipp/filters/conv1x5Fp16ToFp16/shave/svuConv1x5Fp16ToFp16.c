#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv1x5Fp16ToFp16/conv1x5Fp16ToFp16.h>

/// Convolution 1x5Fp16ToFp16 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[out] out         - array of pointers to output lines
/// @param[in] conv         - array of values from convolution
/// @param[in] inWidth      - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution1x5Fp16ToFp16_asm(half **in, half **out, half *conv, UInt32 inWidth);
#else
void Convolution1x5Fp16ToFp16        (half **in, half **out, half *conv, UInt32 inWidth)
{
    UInt32   i, y;
    half sum = 0.0;
    half *input  = *in;
    half *output = *out;

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (y = 0; y < 5; y++)
        {
             // convert value from u8 to u8f and after to fp16
            half val = (half)((float)input[y - 2]);
            sum += val * conv[y];
        }
        input++;

        if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f;
        output[i] = sum;
    }
}
#endif

void svuConv1x5Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[1];
    Conv1x5Fp16ToFp16Param *param = (Conv1x5Fp16ToFp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the input line
        iline[0]=(half *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvConvolution1x5Fp16ToFp16_asm(iline, &output,  (half*)param->cMat, fptr->sliceWidth);
    #else
        Convolution1x5Fp16ToFp16(iline, &output,  (half*)param->cMat, fptr->sliceWidth);
    #endif
    }
}
