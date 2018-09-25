#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv7x7/conv7x7.h>

/// Convolution 7x7 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - array of pointers to output lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth    - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution7x7_asm(UInt8** in, UInt8** out, half conv[49], UInt32 inWidth);
#else
void convolution7x7        (UInt8** in, UInt8** out, half *conv,    UInt32 inWidth)
{
    Int32 x, y;
    UInt32 i;
    UInt8* lines[7];
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
        if (sum >= (half)255.0f)
            sum = (half)255.0f;
        if (sum <= (half)0.0f)
            sum = (half)0.0f;
        out[0][i] = (UInt8)(sum);
    }
}
#endif

void svuConv7x7(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[7];
    UInt32 plane;
    Conv7x7Param *param = (Conv7x7Param*)fptr->params;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for(plane=0; plane<numPlanes; plane++)
        {
            output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            //void mvcvConvolution7x7_asm(UInt8** in, UInt8** out, half conv[49], UInt32 inWidth);
            mvcvConvolution7x7_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            convolution7x7(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}
