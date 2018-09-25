#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/sad11x11/sad11x11.h>

/// SAD (sum of absolute differences) 11x11
///@param[in] in1       - array of pointers to input lines from the first image
///@param[in] in2       - array of pointers to input lines from the second image
///@param[out] out      - array of pointers for output line
///@param[in] width     - width of input line

#ifdef SIPP_USE_MVCV
void mvcvSumOfAbsDiff11x11_asm(UInt8** in1, UInt8** in2, UInt8** out, UInt32 width);
#else
void sumOfAbsDiff11x11        (UInt8** in1, UInt8** in2, UInt8** out, UInt32 width)
{
    int x, y;
    unsigned int i;
    UInt8 *lines1[11], *lines2[11];
    float sum;

    for(i = 0; i < 11; i++)
    {
        lines1[i] = in1[i];
        lines2[i] = in2[i];
    }

    for (i = 0; i < width; i++){
        sum = 0.0;
        for (x = 0; x < 11; x++)
        {
            for (y = 0; y < 11; y++)
            {
                sum += fabs((float)(lines1[x][y - 5] - lines2[x][y - 5]));
            }
            lines1[x]++;
            lines2[x]++;
        }
        if (sum >= 255)
            sum = 255.0;
        out[0][i] = (UInt8)(sum);
    }
}
#endif

void svuSAD11x11(SippFilter *fptr)
{
    UInt8 *input1[11];
    UInt8 *input2[11];
    UInt8 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the (2 * 11) input lines
        for(int i = 0; i < 11; i++)
        {
            input1[i] = (UInt8 *)getInPtr(fptr, 0, iterNum, i, 0);
            input2[i] = (UInt8 *)getInPtr(fptr, 1, iterNum, i, 0);
        }

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvSumOfAbsDiff11x11_asm(input1, input2, &output, fptr->sliceWidth);
    #else
        sumOfAbsDiff11x11(input1, input2, &output, fptr->sliceWidth);
    #endif
    }
}
