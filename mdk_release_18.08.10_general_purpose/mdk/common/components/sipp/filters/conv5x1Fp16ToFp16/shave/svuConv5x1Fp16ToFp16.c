#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv5x1Fp16ToFp16/conv5x1Fp16ToFp16.h>

/// Convolution 5x1Fp16ToFp16 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[out] out         - array of pointers to output lines
/// @param[in] conv         - array of values from convolution
/// @param[in] inWidth      - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution5x1Fp16ToFp16_asm(half **in, half **out, half *conv, UInt32 inWidth);
#else
void Convolution5x1Fp16ToFp16        (half **in, half **out, half *conv, UInt32 inWidth)
{
    UInt32 i, y;
    half sum;
    half *output = *out;
    half *lines[5];
    half val;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (y = 0; y < 5; y++)
        {
             // convert value from u8 to u8f and after to fp16
            half val = (half)((float)lines[y][i]);
            sum += val * conv[y];
        }

        if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f;
        output[i] = sum;
    }
}
#endif

void svuConv5x1Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[5];
    Conv5x1Fp16ToFp16Param *param = (Conv5x1Fp16ToFp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the input line
        iline[0]=(half *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(half *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(half *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]=(half *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]=(half *)getInPtr(fptr, 0, iterNum, 4, 0);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);


    #ifdef SIPP_USE_MVCV
        mvcvConvolution5x1Fp16ToFp16_asm(iline, &output,  (half*)param->cMat, fptr->sliceWidth);
    #else
        Convolution5x1Fp16ToFp16(iline, &output,  (half*)param->cMat, fptr->sliceWidth);
    #endif
    }
}