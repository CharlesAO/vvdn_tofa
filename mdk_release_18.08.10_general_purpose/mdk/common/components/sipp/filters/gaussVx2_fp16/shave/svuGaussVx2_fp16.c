#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/gaussVx2_fp16/gaussVx2_fp16.h>

///Apply downscale 2x horizontal with a gaussian filters with kernel 5x5.
///Have to be used in combination with GaussVx2 to obtain correct output.
///@param[in]  inLine  -  input line
///@param[out] outLine - output resized line
///@param[in]  width   -  input line width (must be multiple of 16)

#ifdef SIPP_USE_MVCV
void mvcvGaussVx2_fp16_asm(half **inLine,half **outLine,Int32 width);
#else
void GaussVx2_fp16        (half **inLine,half **outLine,Int32 width)
{
    half gaus_matrix[3] = {0.0625, 0.25,0.375 };
    Int32 i;
    for (i = 0; i < width; i++)
    {
        outLine[0][i] = (((inLine[0][i] + inLine[4][i]) * gaus_matrix[0]) + ((inLine[1][i] + inLine[3][i]) * gaus_matrix[1]) + (inLine[2][i]  * gaus_matrix[2]));
    }
}
#endif

void svuGaussVx2_fp16(SippFilter *fptr)
{
    half *output;
    half *input[5];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the  input line
        input[0] =(half *)getInPtr(fptr, 0, iterNum, 0, 0);
        input[1] =(half *)getInPtr(fptr, 0, iterNum, 1, 0);
        input[2] =(half *)getInPtr(fptr, 0, iterNum, 2, 0);
        input[3] =(half *)getInPtr(fptr, 0, iterNum, 3, 0);
        input[4] =(half *)getInPtr(fptr, 0, iterNum, 4, 0);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvGaussVx2_fp16_asm(input, &output, fptr->sliceWidth);
    #else
        GaussVx2_fp16(input, &output, fptr->sliceWidth);
    #endif
    }
}
