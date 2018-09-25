#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv7x1/conv7x1.h>

/// Convolution 7x1 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - array of pointers to output lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution7x1_asm(UInt8 **in, UInt8 **out, half* conv, UInt32 inWidth);
#else
void Convolution7x1        (UInt8 **in, UInt8 **out, half* conv, UInt32 inWidth)
{
    UInt32   i, y;
    float sum = 0.0;
    UInt8    *output = *out;

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 7; y++)
        {
            sum += ((float)in[y][i]) * conv[y];
        }

        if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f;
        output[i] = (UInt8)(sum);
    }
}
#endif

void svuConv7x1(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[7];
    Conv7x1Param *param = (Conv7x1Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]=(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]=(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);
        iline[5]=(UInt8 *)getInPtr(fptr, 0, iterNum, 5, 0);
        iline[6]=(UInt8 *)getInPtr(fptr, 0, iterNum, 6, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvConvolution7x1_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
        Convolution7x1(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
    }
}
