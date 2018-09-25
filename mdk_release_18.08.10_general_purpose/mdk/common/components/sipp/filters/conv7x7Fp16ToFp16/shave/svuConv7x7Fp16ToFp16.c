#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv7x7Fp16ToFp16/conv7x7Fp16ToFp16.h>

/// Convolution 7x7 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - array of pointers to output lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution7x7Fp16ToFp16_asm(half** in, half** out, half *conv, UInt32 inWidth);
#else
void convolution7x7Fp16ToFp16        (half** in, half** out, half *conv, UInt32 inWidth)
{
    Int32 x, y;
    UInt32 i;
    half* lines[7];
    half sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0;
        for (x = 0; x < 7; x++)
        {
            for (y = -3; y <= 3; y++)
            {
                // convert value from u8 to u8f and after to fp16
                half val =   (half)((float)lines[x][y]);

                sum += val * conv[x * 7 + y + 3];
            }
            lines[x]++;
        }
        out[0][i] = (half)(sum);
    }
}
#endif

void svuConv7x7Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *iline[7];
    UInt32 plane;
    Conv7x7ParamFp16ToFp16 *param = (Conv7x7ParamFp16ToFp16*)fptr->params;
    UInt32 numPlanes = sippFilterGetNumOutPlanes (fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        for(plane=0; plane<numPlanes; plane++)
        {
        //the output line
        output  =(half*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
        mvcvConvolution7x7Fp16ToFp16_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
        convolution7x7Fp16ToFp16(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
