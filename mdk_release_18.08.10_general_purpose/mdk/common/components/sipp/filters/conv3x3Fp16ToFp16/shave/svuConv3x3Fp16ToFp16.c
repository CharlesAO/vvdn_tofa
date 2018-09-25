#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv3x3Fp16ToFp16/conv3x3Fp16ToFp16.h>

/// Convolution 3x3Fp16ToFp16 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[in] out          - array of pointers to output lines
/// @param[in] conv         - array of values from convolution
/// @param[in] inWidth      - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution3x3Fp16ToFp16_asm      (half** in, half** out, half *conv, UInt32 inWidth);
#else
void convolution3x3Fp16ToFp16Implementation(half** in, half** out, half *conv, UInt32 inWidth)
{
    Int32 x, y;
    UInt32 i;
    half* lines[3];
    half sum;

   //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

   //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0f;
        for (x = 0; x < 3; x++){
            for (y = 0; y < 3; y++){
                half val =   (half)((float)lines[x][y-1]);
                sum += (half)(val * conv[x * 3 + y]);
            }
            lines[x]++;
        }
        out[0][i] = sum;
    }
}
#endif

void svuConv3x3Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[3];
    UInt32 plane;
    Conv3x3Fp16ToFp16Param *param = (Conv3x3Fp16ToFp16Param*)fptr->params;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

       // the input lines
        //iline[0] = (half *)getInPtr(fptr, 0, 0, 0);
        //iline[1] = (half *)getInPtr(fptr, 0, 1, 0);
        //iline[2] = (half *)getInPtr(fptr, 0, 2, 0);
        for(plane=0; plane<numPlanes; plane++)
        {
            //the output line
            output  =(half*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvConvolution3x3Fp16ToFp16_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            convolution3x3Fp16ToFp16Implementation(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
