#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convSeparable5x5Fp16ToFp16/convSeparable5x5Fp16ToFp16.h>

//##########################################################################################
/// Convolution Separable 5x5 Fp16ToFp16 kernel
/// @param[in] out        - array of pointers to output lines
/// @param[in] in         - array of pointers to input lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvSeparable5x5Fp16ToFp16_asm(half** out, half** in, half conv[3], UInt32 inWidth);
#else
void convSeparable5x5Fp16ToFp16        (half** out, half** in, half conv[3], UInt32 inWidth)
{
    int i;
    half* lines[5];
    float sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
        sum = 0.0f;
        sum = ((float)(lines[0][i-2] + lines[4][i-2]) * (float)conv[0] + (float)(lines[1][i-2] + lines[3][i-2]) * (float)conv[1] + (float)lines[2][i-2] * (float)conv[2] +
               (float)(lines[0][i+2] + lines[4][i+2]) * (float)conv[0] + (float)(lines[1][i+2] + lines[3][i+2]) * (float)conv[1] + (float)lines[2][i+2] * (float)conv[2]) * (float)conv[0] +
              ((float)(lines[0][i-1] + lines[4][i-1]) * (float)conv[0] + (float)(lines[1][i-1] + lines[3][i-1]) * (float)conv[1] + (float)lines[2][i-1] * (float)conv[2] +
               (float)(lines[0][i+1] + lines[4][i+1]) * (float)conv[0] + (float)(lines[1][i+1] + lines[3][i+1]) * (float)conv[1] + (float)lines[2][i+1] * (float)conv[2]) * (float)conv[1] +
              ((float)(lines[0][i  ] + lines[4][i  ]) * (float)conv[0] + (float)(lines[1][i  ] + lines[3][i  ]) * (float)conv[1] + (float)lines[2][i  ] * (float)conv[2]) * (float)conv[2];

        out[0][i] = (half)sum;
    }
}
#endif

//##########################################################################################
void svuConvSeparable5x5Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[5];
    UInt32 plane;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    ConvSeparable5x5Fp16ToFp16Param *param = (ConvSeparable5x5Fp16ToFp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for(plane = 0; plane < numPlanes; plane++)
        {
            output = (half *)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvConvSeparable5x5Fp16ToFp16_asm(&output, iline, (half *)param->cMat, fptr->sliceWidth);
    #else
            convSeparable5x5Fp16ToFp16(&output, iline, (half *)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
