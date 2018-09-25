#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv9x9Fp16ToFp16/conv9x9Fp16ToFp16.h>

/// Convolution 9x9Fp16ToFp16 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[in] out          - array of pointers to output lines
/// @param[in] conv         - array of values from convolution
/// @param[in] inWidth      - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution9x9Fp16ToFp16_asm(half** in, half** out, half* conv, UInt32 inWidth);
#else
void Convolution9x9Fp16ToFp16        (half** in, half** out, half* conv, UInt32 inWidth)
{
    UInt32 x, y;
    UInt32 i;
    half* lines[9];
    half sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];
    lines[7] = in[7];
    lines[8] = in[8];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0;
        for (x = 0; x < 9; x++)
        {
            for (y = 0; y < 9; y++)
            {
                sum += lines[x][y - 4] * conv[x * 9 + y];
            }
            lines[x]++;
        }
      /*  if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0; */
        out[0][i] = (half)sum;
    }
}
#endif

//##########################################################################################
void svuConv9x9Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[9];
    UInt32 plane;
    UInt32 numPlanes =  sippFilterGetNumOutPlanes(fptr, 0);
    Conv9x9Fp16ToFp16Param *param = (Conv9x9Fp16ToFp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        for(plane=0; plane < numPlanes; plane++)
        {
            output = (half*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvConvolution9x9Fp16ToFp16_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            Convolution9x9Fp16ToFp16(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
