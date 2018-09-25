#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv11x11/conv11x11.h>

/// Convolution 11x11 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - array of pointers to output lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution11x11_asm(UInt8** in, UInt8** out, half* conv, UInt32 inWidth);
#else
void Convolution11x11        (UInt8** in, UInt8** out, half* conv, UInt32 inWidth)
{
    UInt32 x, y;
    UInt32 i;
    UInt8* lines[11];
    float sum;

    //Initialize lines pointers
    lines[0]  = in[0];
    lines[1]  = in[1];
    lines[2]  = in[2];
    lines[3]  = in[3];
    lines[4]  = in[4];
    lines[5]  = in[5];
    lines[6]  = in[6];
    lines[7]  = in[7];
    lines[8]  = in[8];
    lines[9]  = in[9];
    lines[10] = in[10];

    //Go on the whole line
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (x = 0; x < 11; x++)
        {
            for (y = 0; y < 11; y++)
            {
                sum += (float)(lines[x][y - 5] * conv[x * 11 + y]);
            }
            lines[x]++;
        }
        if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0;
        out[0][i] = (UInt8)(sum);
    }
}
#endif

//##########################################################################################
void svuConv11x11(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[11];
    UInt32 plane;
    Conv11x11Param *param = (Conv11x11Param*)fptr->params;
    UInt32 i;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        for(plane=0; plane<numPlanes; plane++)
        {
            //the input lines
            for (i = 0; i < 11; i++)
            {
                iline[i]=(UInt8 *)getInPtr(fptr, 0, iterNum, i, 0);
            }

            output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
            mvcvConvolution11x11_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            Convolution11x11(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
