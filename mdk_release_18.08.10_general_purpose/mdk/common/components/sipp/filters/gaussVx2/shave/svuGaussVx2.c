#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/gaussVx2/gaussVx2.h>

///Apply downscale 2x vertical with a gaussian filters with kernel 5x5.
///Have to be used in combination with GaussVx2 to obtain correct output.
///@param[in]  inLine  -  input line
///@param[out] outLine - output resized line
///@param[in]  width   -  input line width (must be multiple of 16)

#ifdef SIPP_USE_MVCV
void mvcvGaussVx2_asm(UInt8 **inLine,UInt8 *outLine,int width);
#else
void mvcvGaussVx2(UInt8 **inLine,UInt8 *outLine,int width)
{
    unsigned int gaus_matrix[3] = {16, 64,96 };
    int i;
    for (i = 0; i < width; i++)
    {
        outLine[i] = (((inLine[0][i] + inLine[4][i]) * gaus_matrix[0]) + ((inLine[1][i] + inLine[3][i]) * gaus_matrix[1]) + (inLine[2][i]  * gaus_matrix[2]))>>8;
    }
}
#endif

void svuGaussVx2(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *input[5];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the  input line
        input[0] =(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        input[1] =(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        input[2] =(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        input[3] =(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        input[4] =(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvGaussVx2_asm(input, output, fptr->sliceWidth);
    #else
        mvcvGaussVx2(input, output, fptr->sliceWidth);
    #endif
    }
}
