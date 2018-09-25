#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv7x7Fp16ToU8/conv7x7Fp16ToU8.h>

/// Convolution 7x7 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - array of pointers to output lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution7x7Fp16ToU8_asm(half** in, UInt8** out, half *conv, UInt32 inWidth);
#else
void convolution7x7Fp16ToU8        (half** in, UInt8** out, half *conv, UInt32 inWidth)
{
    int x, y;
    unsigned int i;
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
                half val = lines[x][y];

                sum += val * conv[x * 7 + y + 3];
            }
            lines[x]++;
        }
        if (sum >= (half)1.0f)
            sum = (half)1.0f;
        if (sum <= (half)0.0f)
            sum = (half)0.0f;
        out[0][i] = (UInt8)((float)sum * 255.0f);
    }
}
#endif

void svuConv7x7Fp16ToU8(SippFilter *fptr)
{
    UInt8 *output;
    half *iline[7];
    UInt32 plane;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    Conv7x7ParamFp16ToU8 *param = (Conv7x7ParamFp16ToU8*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the input lines
        iline[0] = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1] = (half *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2] = (half *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3] = (half *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4] = (half *)getInPtr(fptr, 0, iterNum, 4, 0);
        iline[5] = (half *)getInPtr(fptr, 0, iterNum, 5, 0);
        iline[6] = (half *)getInPtr(fptr, 0, iterNum, 6, 0);

        for(plane=0; plane < numPlanes; plane++)
        {
           //the output line
            output  =(UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvConvolution7x7Fp16ToU8_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            convolution7x7Fp16ToU8(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
