#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/gaussHx2/gaussHx2.h>

///Apply downscale 2x horizontal with a gaussian filters with kernel 5x5.
///Have to be used in combination with GaussVx2 to obtain correct output.
///@param[in]  inLine    -  input line
///@param[out] outLine   - output resized line
///@param[in]  width     -  input line width (must be multiple of 16)

#ifdef SIPP_USE_MVCV
void mvcvGaussHx2_asm(UInt8 *inLine,UInt8 *outLine,int width);
#else
void mvcvGaussHx2    (UInt8 *inLine,UInt8 *outLine,int width)
{
    unsigned int gaus_matrix[3] = {16, 64,96 };
    int i;
    for (i = 0; i<(width<<1);i+=2)
    {
        outLine[i>>1] = (((inLine[i-2] + inLine[i+2]) * gaus_matrix[0]) + ((inLine[i-1] + inLine[i+1]) * gaus_matrix[1]) + (inLine[i]  * gaus_matrix[2]))>>8;
    }
}
#endif

void svuGaussHx2(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *input;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the  input line
        input =(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvGaussHx2_asm(input, output, fptr->sliceWidth);
    #else
        mvcvGaussHx2(input, output, fptr->sliceWidth);
    #endif
    }
}
