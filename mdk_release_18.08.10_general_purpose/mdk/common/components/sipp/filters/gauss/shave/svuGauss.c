#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/gauss/gauss.h>

/// gaussian filter
/// @param[in] in        - array of pointers to input lines
/// @param[out] out      - array of pointers for output lines
/// @param[in] width     - width of input line

#ifdef SIPP_USE_MVCV
void mvcvGauss_asm(UInt8** inLine, UInt8** out, UInt32 width);
#else
void gauss        (UInt8** inLine, UInt8** out, UInt32 width)
{
    float gaus_matrix[3] = {1.4, 3.0, 3.8 };
    int i, j;
    float out1, out2, out3, out4, out5;
    UInt8 *outLine;

    outLine=*out;

    for (i = 0; i < width; i++)
    {
        out1 = (((inLine[0][i-2] + inLine[4][i-2]) * gaus_matrix[0]) + ((inLine[1][i-2] + inLine[3][i-2]) * gaus_matrix[1]) + (inLine[2][i-2]  * gaus_matrix[2]));
        out2 = (((inLine[0][i-1] + inLine[4][i-1]) * gaus_matrix[0]) + ((inLine[1][i-1] + inLine[3][i-1]) * gaus_matrix[1]) + (inLine[2][i-1]  * gaus_matrix[2]));
        out3 = (((inLine[0][i-0] + inLine[4][i-0]) * gaus_matrix[0]) + ((inLine[1][i-0] + inLine[3][i-0]) * gaus_matrix[1]) + (inLine[2][i-0]  * gaus_matrix[2]));
        out4 = (((inLine[0][i+1] + inLine[4][i+1]) * gaus_matrix[0]) + ((inLine[1][i+1] + inLine[3][i+1]) * gaus_matrix[1]) + (inLine[2][i+1]  * gaus_matrix[2]));
        out5 = (((inLine[0][i+2] + inLine[4][i+2]) * gaus_matrix[0]) + ((inLine[1][i+2] + inLine[3][i+2]) * gaus_matrix[1]) + (inLine[2][i+2]  * gaus_matrix[2]));
        outLine[i] = (UInt8)((((out1 + out5) * gaus_matrix[0]) + ((out2 + out4) * gaus_matrix[1]) + (out3  * gaus_matrix[2]))/159);
    }
}
#endif

void svuGauss(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *input[5];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 5 input lines
        input[0] =(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        input[1] =(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        input[2] =(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        input[3] =(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        input[4] =(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvGauss_asm(input, &output, fptr->sliceWidth);
    #else
        gauss(input, &output, fptr->sliceWidth);
    #endif
    }
}