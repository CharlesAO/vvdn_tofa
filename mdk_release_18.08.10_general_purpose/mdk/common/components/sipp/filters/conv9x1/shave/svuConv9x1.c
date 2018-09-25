#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv9x1/conv9x1.h>

/// Convolution 9x1 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - array of pointers to output lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution9x1_asm(UInt8 **in, UInt8 **out, half *conv, UInt32 inWidth);
#else
void Convolution9x1        (UInt8 **in, UInt8 **out, half *conv, UInt32 inWidth)
{
    UInt32 i, y;
    float sum = 0.0;
    UInt8 *output = *out;

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 9; y++)
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

void svuConv9x1(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[9];
    Conv9x1Param *param = (Conv9x1Param*)fptr->params;
    UInt32 plane;
    UInt32 i;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the input lines
        for(i = 0; i < 9; i++)
        {
            iline[i]=(UInt8 *)getInPtr(fptr, 0, iterNum, i, 0);
        }

        for(plane=0; plane < numPlanes; plane++)
        {
            //the output line
            output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvConvolution9x1_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            Convolution9x1(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
