#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convSeparable3x3/convSeparable3x3.h>

//##########################################################################################
/// Convolution Separable 3x3 kernel
/// @param[in] out        - array of pointers to output lines
/// @param[in] in         - array of pointers to input lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#if defined(SIPP_USE_MVCV)
void mvcvConvSeparable3x3_asm(UInt8* out, UInt8** in, float conv[2], UInt32 inWidth);
#else
void convSeparable3x3        (UInt8* out, UInt8** in, float conv[2], UInt32 inWidth)
{
    int i;
    float sum;

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
        sum = 0.0f;
        sum = (((float)(in[0][i-1] +  in[2][i-1]) * conv[0] + (float)in[1][i-1] * conv[1]) +
               ((float)(in[0][i+1] +  in[2][i+1]) * conv[0] + (float)in[1][i+1] * conv[1])) * conv[0] +
               ((float)(in[0][i  ] +  in[2][i  ]) * conv[0] + (float)in[1][i  ] * conv[1])  * conv[1];
        if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;
        out[i] = (UInt8)(sum);
    }
}
#endif

//##########################################################################################
void svuConvSeparable3x3(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[3];
    UInt32 pl;
    ConvSeparable3x3Param *param = (ConvSeparable3x3Param*)fptr->params;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        for (pl = 0; pl < numPlanes; pl++)
        {
            //the 3 input lines
            iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, pl);
            iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, pl);
            iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, pl);

            //the output line
            output = (UInt8 *)getOutPtr(fptr, iterNum, pl);

    #ifdef SIPP_USE_MVCV
            mvcvConvSeparable3x3_asm(output, iline, param->cMat, fptr->sliceWidth);
    #else
            convSeparable3x3        (output, iline, param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
