#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convSeparable7x7Fp16ToFp16/convSeparable7x7Fp16ToFp16.h>

//##########################################################################################
/// Convolution Separable 7x7 Fp16ToFp16 kernel
/// @param[in] out        - array of pointers to output lines
/// @param[in] in         - array of pointers to input lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvSeparable7x7Fp16ToFp16_asm(half** out, half** in, half conv[4], UInt32 inWidth);
#else
void convSeparable7x7Fp16ToFp16        (half** out, half** in, half conv[4], UInt32 inWidth)
{
    int i;
    half* lines[7];
    float sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++)
    {
        sum = 0.0f;
        sum = ((float)(lines[0][i-3] + lines[6][i-3]) * (float)conv[0] + (float)(lines[1][i-3] + lines[5][i-3]) * (float)conv[1] + (float)(lines[2][i-3] + lines[4][i-3]) * (float)conv[2] + (float)lines[3][i-3] * (float)conv[3] +
               (float)(lines[0][i+3] + lines[6][i+3]) * (float)conv[0] + (float)(lines[1][i+3] + lines[5][i+3]) * (float)conv[1] + (float)(lines[2][i+3] + lines[4][i+3]) * (float)conv[2] + (float)lines[3][i+3] * (float)conv[3]) * (float)conv[0] +
              ((float)(lines[0][i-2] + lines[6][i-2]) * (float)conv[0] + (float)(lines[1][i-2] + lines[5][i-2]) * (float)conv[1] + (float)(lines[2][i-2] + lines[4][i-2]) * (float)conv[2] + (float)lines[3][i-2] * (float)conv[3] +
               (float)(lines[0][i+2] + lines[6][i+2]) * (float)conv[0] + (float)(lines[1][i+2] + lines[5][i+2]) * (float)conv[1] + (float)(lines[2][i+2] + lines[4][i+2]) * (float)conv[2] + (float)lines[3][i+2] * (float)conv[3]) * (float)conv[1] +
              ((float)(lines[0][i-1] + lines[6][i-1]) * (float)conv[0] + (float)(lines[1][i-1] + lines[5][i-1]) * (float)conv[1] + (float)(lines[2][i-1] + lines[4][i-1]) * (float)conv[2] + (float)lines[3][i-1] * (float)conv[3] +
               (float)(lines[0][i+1] + lines[6][i+1]) * (float)conv[0] + (float)(lines[1][i+1] + lines[5][i+1]) * (float)conv[1] + (float)(lines[2][i+1] + lines[4][i+1]) * (float)conv[2] + (float)lines[3][i+1] * (float)conv[3]) * (float)conv[2] +
              ((float)(lines[0][i  ] + lines[6][i  ]) * (float)conv[0] + (float)(lines[1][i  ] + lines[5][i  ]) * (float)conv[1] + (float)(lines[2][i  ] + lines[4][i  ]) * (float)conv[2] + (float)lines[3][i  ] * (float)conv[3]) * (float)conv[3];

        out[0][i] = (half)sum;
    }
}
#endif

//##########################################################################################
void svuConvSeparable7x7Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[7];
    UInt32 plane;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    ConvSeparable7x7Fp16ToFp16Param *param = (ConvSeparable7x7Fp16ToFp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for(plane = 0; plane < numPlanes; plane++)
        {
            output = (half *)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvConvSeparable7x7Fp16ToFp16_asm(&output, iline, (half *)param->cMat, fptr->sliceWidth);
    #else
            convSeparable7x7Fp16ToFp16(&output, iline, (half *)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
