#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/pyrDown/pyrDown.h>

/// pyrdown filter - downsample even lines and even cols
/// @param[in] in     - array of pointers to input lines
/// @param[out]out    - array of pointers for output lines
/// @param[in] width  - width of input line

#ifdef SIPP_USE_MVCV
void mvcvPyrdown_asm(UInt8 **inLine,UInt8 **out,int width);
#else
void pyrdown        (UInt8 **inLine,UInt8 **out,int width)
{
    UInt32 gaus_matrix[3] = {16, 64,96 };
    int i, j;
    UInt8 outLine11[1924];
    UInt8 *outLine1;
    UInt8 *outLine;

    outLine1 = outLine11;
    outLine1 = outLine1 + 2;

    outLine=*out;

    for (i = -2; i < width +2; i++)
    {
        outLine1[i] = (((inLine[0][i] + inLine[4][i]) * gaus_matrix[0]) + ((inLine[1][i] + inLine[3][i]) * gaus_matrix[1]) + (inLine[2][i]  * gaus_matrix[2]))>>8;
    }


    for (j = 0; j<width;j+=2)
    {
        outLine[j>>1] = (((outLine1[j-2] + outLine1[j+2]) * gaus_matrix[0]) + ((outLine1[j-1] + outLine1[j+1]) * gaus_matrix[1]) + (outLine1[j]  * gaus_matrix[2]))>>8;
    }
}
#endif

void svuPyrDown(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[5];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]=(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]=(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvPyrdown_asm(iline, &output,  fptr->sliceWidth);
    #else
        pyrdown(iline, &output,  fptr->sliceWidth );
    #endif
    }
}
