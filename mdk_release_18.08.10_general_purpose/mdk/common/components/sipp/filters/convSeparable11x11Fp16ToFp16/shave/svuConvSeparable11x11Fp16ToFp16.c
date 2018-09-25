#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convSeparable11x11Fp16ToFp16/convSeparable11x11Fp16ToFp16.h>

//##########################################################################################
/// Convolution Separable 11x11 Fp16ToFp16 kernel
/// @param[in] out        - array of pointers to output lines
/// @param[in] in         - array of pointers to input lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvSeparable11x11Fp16ToFp16_asm(half** out, half** in, half conv[6], UInt32 inWidth);
#else
void convSeparable11x11Fp16ToFp16        (half** out, half** in, half conv[6], UInt32 inWidth)
{
    int i;
    half* lines[11];
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
    for (i = 0; i < (int)inWidth; i++)
    {
        sum = 0.0f;
        sum = ((float)(lines[0][i-5] + lines[10][i-5]) * (float)conv[0] + (float)(lines[1][i-5] + lines[9][i-5]) * (float)conv[1] + (float)(lines[2][i-5] + lines[8][i-5]) * (float)conv[2] + (float)(lines[3][i-5] + lines[7][i-5]) * (float)conv[3] + (float)(lines[4][i-5] + lines[6][i-5]) * (float)conv[4] + (float)lines[5][i-5] * (float)conv[5] +
               (float)(lines[0][i+5] + lines[10][i+5]) * (float)conv[0] + (float)(lines[1][i+5] + lines[9][i+5]) * (float)conv[1] + (float)(lines[2][i+5] + lines[8][i+5]) * (float)conv[2] + (float)(lines[3][i+5] + lines[7][i+5]) * (float)conv[3] + (float)(lines[4][i+5] + lines[6][i+5]) * (float)conv[4] + (float)lines[5][i+5] * (float)conv[5]) * (float)conv[0] +
              ((float)(lines[0][i-4] + lines[10][i-4]) * (float)conv[0] + (float)(lines[1][i-4] + lines[9][i-4]) * (float)conv[1] + (float)(lines[2][i-4] + lines[8][i-4]) * (float)conv[2] + (float)(lines[3][i-4] + lines[7][i-4]) * (float)conv[3] + (float)(lines[4][i-4] + lines[6][i-4]) * (float)conv[4] + (float)lines[5][i-4] * (float)conv[5] +
               (float)(lines[0][i+4] + lines[10][i+4]) * (float)conv[0] + (float)(lines[1][i+4] + lines[9][i+4]) * (float)conv[1] + (float)(lines[2][i+4] + lines[8][i+4]) * (float)conv[2] + (float)(lines[3][i+4] + lines[7][i+4]) * (float)conv[3] + (float)(lines[4][i+4] + lines[6][i+4]) * (float)conv[4] + (float)lines[5][i+4] * (float)conv[5]) * (float)conv[1] +
              ((float)(lines[0][i-3] + lines[10][i-3]) * (float)conv[0] + (float)(lines[1][i-3] + lines[9][i-3]) * (float)conv[1] + (float)(lines[2][i-3] + lines[8][i-3]) * (float)conv[2] + (float)(lines[3][i-5] + lines[7][i-3]) * (float)conv[3] + (float)(lines[4][i-3] + lines[6][i-3]) * (float)conv[4] + (float)lines[5][i-3] * (float)conv[5] +
               (float)(lines[0][i+3] + lines[10][i+3]) * (float)conv[0] + (float)(lines[1][i+3] + lines[9][i+3]) * (float)conv[1] + (float)(lines[2][i+3] + lines[8][i+3]) * (float)conv[2] + (float)(lines[3][i+5] + lines[7][i+3]) * (float)conv[3] + (float)(lines[4][i+3] + lines[6][i+3]) * (float)conv[4] + (float)lines[5][i+3] * (float)conv[5]) * (float)conv[2] +
              ((float)(lines[0][i-2] + lines[10][i-2]) * (float)conv[0] + (float)(lines[1][i-2] + lines[9][i-2]) * (float)conv[1] + (float)(lines[2][i-2] + lines[8][i-2]) * (float)conv[2] + (float)(lines[3][i-5] + lines[7][i-2]) * (float)conv[3] + (float)(lines[4][i-2] + lines[6][i-2]) * (float)conv[4] + (float)lines[5][i-2] * (float)conv[5] +
               (float)(lines[0][i+2] + lines[10][i+2]) * (float)conv[0] + (float)(lines[1][i+2] + lines[9][i+2]) * (float)conv[1] + (float)(lines[2][i+2] + lines[8][i+2]) * (float)conv[2] + (float)(lines[3][i+5] + lines[7][i+2]) * (float)conv[3] + (float)(lines[4][i+2] + lines[6][i+2]) * (float)conv[4] + (float)lines[5][i+2] * (float)conv[5]) * (float)conv[3] +
              ((float)(lines[0][i-1] + lines[10][i-1]) * (float)conv[0] + (float)(lines[1][i-1] + lines[9][i-1]) * (float)conv[1] + (float)(lines[2][i-1] + lines[8][i-1]) * (float)conv[2] + (float)(lines[3][i-1] + lines[7][i-1]) * (float)conv[3] + (float)(lines[4][i-1] + lines[6][i-1]) * (float)conv[4] + (float)lines[5][i-1] * (float)conv[5] +
               (float)(lines[0][i+1] + lines[10][i+1]) * (float)conv[0] + (float)(lines[1][i+1] + lines[9][i+1]) * (float)conv[1] + (float)(lines[2][i+1] + lines[8][i+1]) * (float)conv[2] + (float)(lines[3][i+1] + lines[7][i+1]) * (float)conv[3] + (float)(lines[4][i+1] + lines[6][i+1]) * (float)conv[4] + (float)lines[5][i+1] * (float)conv[5]) * (float)conv[4] +
              ((float)(lines[0][i  ] + lines[10][i  ]) * (float)conv[0] + (float)(lines[1][i  ] + lines[9][i  ]) * (float)conv[1] + (float)(lines[2][i  ] + lines[8][i  ]) * (float)conv[2] + (float)(lines[3][i  ] + lines[7][i  ]) * (float)conv[3] + (float)(lines[4][i  ] + lines[6][i  ]) * (float)conv[4] + (float)lines[5][i  ] * (float)conv[5]) * (float)conv[5];

        out[0][i] = (half)(sum);
    }
}
#endif

//##########################################################################################
void svuConvSeparable11x11Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[11];
    ConvSeparable11x11Fp16ToFp16Param *param = (ConvSeparable11x11Fp16ToFp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        iline[0]  = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]  = (half *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]  = (half *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]  = (half *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]  = (half *)getInPtr(fptr, 0, iterNum, 4, 0);
        iline[5]  = (half *)getInPtr(fptr, 0, iterNum, 5, 0);
        iline[6]  = (half *)getInPtr(fptr, 0, iterNum, 6, 0);
        iline[7]  = (half *)getInPtr(fptr, 0, iterNum, 7, 0);
        iline[8]  = (half *)getInPtr(fptr, 0, iterNum, 8, 0);
        iline[9]  = (half *)getInPtr(fptr, 0, iterNum, 9, 0);
        iline[10] = (half *)getInPtr(fptr, 0, iterNum, 10, 0);

        output = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvConvSeparable11x11Fp16ToFp16_asm(&output, iline, (half *)param->cMat, fptr->sliceWidth);
    #else
        convSeparable11x11Fp16ToFp16(&output, iline, (half *)param->cMat, fptr->sliceWidth);
    #endif
    }
}
