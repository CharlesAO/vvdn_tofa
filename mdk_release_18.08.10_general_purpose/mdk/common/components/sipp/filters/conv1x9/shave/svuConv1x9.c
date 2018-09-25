#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv1x9/conv1x9.h>

/// Convolution 1x9 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - array of pointers to output lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution1x9_asm(UInt8 **in, UInt8 **out, half* conv, UInt32 inWidth);
#else
void Convolution1x9        (UInt8 **in, UInt8 **out, half* conv, UInt32 inWidth)
{
    UInt32   i, y;
    float sum = 0.0;
    UInt8    *input  = *in;
    UInt8    *output = *out;

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 9; y++)
        {
            sum += ((float)input[y - 4]) * conv[y];
        }
        input++;

        if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f;
        output[i] = (UInt8)(sum);
    }
}
#endif

void svuConv1x9(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[1];
    Conv1x9Param *param = (Conv1x9Param*)fptr->params;
    UInt32 plane;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the input line
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        for(plane=0; plane < numPlanes; plane++)
        {
            //the output line
            output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
            mvcvConvolution1x9_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            Convolution1x9(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
