#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convSeparable3x3Fp16ToFp16/convSeparable3x3Fp16ToFp16.h>

//##########################################################################################
/// Convolution Separable 3x3 Fp16ToFp16 kernel
/// @param[in] out        - array of pointers to output lines
/// @param[in] in         - array of pointers to input lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvSeparable3x3Fp16ToFp16_asm(half* out, half** in, half conv[2], UInt32 inWidth);
#else
void convSeparable3x3Fp16ToFp16        (half* out, half** in, half conv[2], UInt32 inWidth)
{
    int i;
    half sum;

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
        sum = 0.0f;
        sum = (((half)((float)(in[0][i-1] +  in[2][i-1])) * conv[0] + (half)((float)in[1][i-1]) * conv[1]) +
               ((half)((float)(in[0][i+1] +  in[2][i+1])) * conv[0] + (half)((float)in[1][i+1]) * conv[1]))  * conv[0] +
               ((half)((float)(in[0][i  ] +  in[2][i  ])) * conv[0] + (half)((float)in[1][i  ]) * conv[1])   * conv[1];

       out[i] = sum;
    }
}
#endif

//##########################################################################################
void svuConvSeparable3x3Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[3];
    UInt32 pl;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    ConvSeparable3x3Fp16ToFp16Param *param = (ConvSeparable3x3Fp16ToFp16Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for (pl = 0; pl < numPlanes; pl++)
        {
            //the 3 input lines
            iline[0]=(half *)getInPtr(fptr, 0, iterNum, 0, pl);
            iline[1]=(half *)getInPtr(fptr, 0, iterNum, 1, pl);
            iline[2]=(half *)getInPtr(fptr, 0, iterNum, 2, pl);

            //the output line
            output = (half *)getOutPtr(fptr, iterNum, pl);

    #ifdef SIPP_USE_MVCV
            mvcvConvSeparable3x3Fp16ToFp16_asm(output, iline, (half *)param->cMat, fptr->sliceWidth);
    #else
            convSeparable3x3Fp16ToFp16(output, iline, (half *)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
