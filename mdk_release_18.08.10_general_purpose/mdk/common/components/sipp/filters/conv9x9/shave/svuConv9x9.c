#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/conv9x9/conv9x9.h>

/// Convolution 9x9 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[in] out          - array of pointers to output lines
/// @param[in] conv         - array of values from convolution
/// @param[in] inWidth      - width of input line

#ifdef SIPP_USE_MVCV
void mvcvConvolution9x9_asm(UInt8** in, UInt8** out, half* conv, UInt32 inWidth);
#else
void Convolution9x9        (UInt8** in, UInt8** out, half* conv, UInt32 inWidth)
{
    UInt32 x, y;
    UInt32 i;
    UInt8* lines[9];
    float sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];
    lines[7] = in[7];
    lines[8] = in[8];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0;
        for (x = 0; x < 9; x++)
        {
            for (y = 0; y < 9; y++)
            {
                sum += (float)(lines[x][y - 4] * conv[x * 9 + y]);
            }
            lines[x]++;
        }
        if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0;
        out[0][i] = (UInt8)(sum);
    }
}
#endif

//##########################################################################################
void svuConv9x9(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[9];
    UInt32 plane;
    Conv9x9Param *param = (Conv9x9Param*)fptr->params;
    UInt32 numPlanes = sippFilterGetNumOutPlanes(fptr, 0);
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the 9 input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]=(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]=(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);
        iline[5]=(UInt8 *)getInPtr(fptr, 0, iterNum, 5, 0);
        iline[6]=(UInt8 *)getInPtr(fptr, 0, iterNum, 6, 0);
        iline[7]=(UInt8 *)getInPtr(fptr, 0, iterNum, 7, 0);
        iline[8]=(UInt8 *)getInPtr(fptr, 0, iterNum, 8, 0);

        for(plane=0; plane < numPlanes; plane++)
        {
            output = (UInt8*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, plane/*plane*/, iline);

    #ifdef SIPP_USE_MVCV
            mvcvConvolution9x9_asm(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #else
            Convolution9x9(iline, &output, (half*)param->cMat, fptr->sliceWidth);
    #endif
        }
    }
}

