#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convSeparable5x5/convSeparable5x5.h>

//##########################################################################################
/// Convolution Separable 5x5 kernel
/// @param[in] out        - array of pointers to output lines
/// @param[in] in         - array of pointers to input lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvSeparable5x5_asm(UInt8** out, UInt8** in, float conv[3], UInt32 inWidth);
#else
void convSeparable5x5        (UInt8** out, UInt8** in, float conv[3], UInt32 inWidth)
{
    int i;
    UInt8* lines[5];
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
        sum = ((float)(lines[0][i-2] + lines[4][i-2]) * conv[0] + (float)(lines[1][i-2] + lines[3][i-2]) * conv[1] + (float)lines[2][i-2] * conv[2] +
               (float)(lines[0][i+2] + lines[4][i+2]) * conv[0] + (float)(lines[1][i+2] + lines[3][i+2]) * conv[1] + (float)lines[2][i+2] * conv[2]) * conv[0] +
              ((float)(lines[0][i-1] + lines[4][i-1]) * conv[0] + (float)(lines[1][i-1] + lines[3][i-1]) * conv[1] + (float)lines[2][i-1] * conv[2] +
               (float)(lines[0][i+1] + lines[4][i+1]) * conv[0] + (float)(lines[1][i+1] + lines[3][i+1]) * conv[1] + (float)lines[2][i+1] * conv[2]) * conv[1] +
              ((float)(lines[0][i  ] + lines[4][i  ]) * conv[0] + (float)(lines[1][i  ] + lines[3][i  ]) * conv[1] + (float)lines[2][i  ] * conv[2]) * conv[2];

        if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;
        out[0][i] = (UInt8)(sum);
    }
}
#endif

//##########################################################################################
void svuConvSeparable5x5(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[5];
    UInt32 plane;
    ConvSeparable5x5Param *param = (ConvSeparable5x5Param*)fptr->params;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]=(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]=(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);

        for(plane=0; plane < numPlanes; plane++)
        {
            output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvConvSeparable5x5_asm(&output, iline, (float*)param->cMat, fptr->sliceWidth);
    #else
            convSeparable5x5(&output, iline, (float*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
