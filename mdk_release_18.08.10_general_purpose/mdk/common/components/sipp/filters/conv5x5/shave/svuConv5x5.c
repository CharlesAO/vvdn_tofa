#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv5x5/conv5x5.h>

/// Convolution 5x5 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - array of pointers to output lines
/// @param[in] conv       - array of values from convolution
/// @param[in] inWidth      - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution5x5_asm(UInt8** in, UInt8** out, half* conv, UInt32 inWidth);
#else
void Convolution5x5        (UInt8** in, UInt8** out, half conv[25], UInt32 inWidth)
{
    int x, y;
    unsigned int i;
    UInt8* lines[5];
    half sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (x = 0; x < 5; x++)
        {
            for (y = 0; y < 5; y++)
            {
                sum += (float)(lines[x][y - 2] * conv[x * 5 + y]);
            }
            lines[x]++;
        }
        if (sum >= 255.0f)
            sum  = 255.0f;
        if (sum <= 0.0f)
            sum  = 0.0f;
        out[0][i] = (UInt8)(sum);
    }
}
#endif

//##########################################################################################
void svuConv5x5(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[5];
    UInt32 plane;
    Conv5x5Param *param = (Conv5x5Param*)fptr->params;
    UInt32 numPlanes    = sippFilterGetNumOutPlanes(fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        for(plane=0; plane < numPlanes; plane++)
        {
            //the output line
            output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, 0/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            void mvcvConvolution5x5_asm(UInt8** in, UInt8** out, half* conv, UInt32 inWidth);
            mvcvConvolution5x5_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            Convolution5x5(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}


