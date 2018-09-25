#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/ssd7x7U8ToU32/ssd7x7U8ToU32.h>

/// SSD (Sum of Squared differences) 7x7U8ToU32
///@param[in] in1       - array of pointers to input lines from the first image
///@param[in] in2       - array of pointers to input lines from the second image
///@param[out] out      - array of pointers for output line
///@param[in] width     - width of input line

#ifdef SIPP_USE_MVCV
void mvcvSumOfSquaredDiff7x7U8ToU32_asm(UInt8** in1, UInt8** in2, UInt32** out, UInt32 width);
#else
void sumOfSquaredDiff7x7U8ToU32        (UInt8** in1, UInt8** in2, UInt32** out, UInt32 width)
{
    int x, y;
    unsigned int i;
    UInt8 *lines1[7], *lines2[7];
    UInt32 sum, diff;

    for(i = 0; i < 7; i++)
    {
        lines1[i] = in1[i];
        lines2[i] = in2[i];
    }

    for (i = 0; i < width; i++)
    {
        sum = 0;
        for (x = 0; x < 7; x++)
        {
            for (y = 0; y < 7; y++)
            {
                diff = (UInt32)lines1[x][y - 3] - (UInt32)lines2[x][y - 3];
                diff = diff * diff;
                sum += diff;
            }
            lines1[x]++;
            lines2[x]++;
        }
        out[0][i] = (UInt32)(sum);
    }
}
#endif

void svuSSD7x7U8ToU32(SippFilter *fptr)
{
    UInt8 *input1[7];
    UInt8 *input2[7];
    UInt32 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the (2 * 7) input lines
        for(int i = 0; i < 7; i++)
        {
            input1[i] = (UInt8 *)getInPtr(fptr, 0, iterNum, i, 0);
            input2[i] = (UInt8 *)getInPtr(fptr, 1, iterNum, i, 0);
        }

        //the output line
        output = (UInt32 *)getOutPtr(fptr, iterNum, 0);

     #ifdef SIPP_USE_MVCV
        mvcvSumOfSquaredDiff7x7U8ToU32_asm(input1, input2, &output, fptr->sliceWidth);
    #else
        sumOfSquaredDiff7x7U8ToU32(input1, input2, &output, fptr->sliceWidth);
    #endif
    }
}
