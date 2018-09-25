#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/meanStdDev/meanStdDev.h>
#include <math.h>

///This kernel calculates mean and standard deviation of an array of elements
///@param[in]  in     - Input line
///@param[out] mean   - Computed mean value
///@param[out] stddev - Computed standard deviation
///@param[out] width  - Width of line

#ifdef SIPP_USE_MVCV
void mvcvMeanstddev_asm(UInt8** in, float *mean, float *stddev, u32 width);
#else
void mvcvMeanstddev    (UInt8** in, float *mean, float *stddev, u32 width)
{
    float sum;
    float square_sum;
    float f0, f1;
    UInt8 *line = in[0];
    int i;

    sum = square_sum = 0.0f;

    for(i = 0; i < width; i++)
    {
        sum        += (float)line[i];
        square_sum += (((float)line[i]) * ((float)line[i]));
    }

    *mean = sum / (float)width;
    f0 = square_sum / (float)width;
    f1 = (*mean) * (*mean);
    if(__builtin_isgreater(f0, f1) == 1)
    {
        *stddev = sqrt(f0 - f1);
    }
    else
    {
        *stddev = 0;
    }
}
#endif

//#######################################################################################
void svuMeanStdDev(SippFilter *fptr)
{
    UInt8 *input[1];
    float *output;
    UInt32 i;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        input[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (float *)getOutPtr(fptr, iterNum, 0);
        for(i = 0; i < fptr->sliceWidth; i++)
            output[i] = 0;

    #ifdef SIPP_USE_MVCV
        mvcvMeanstddev_asm(input, &output[0], &output[1], fptr->sliceWidth);
    #else
        mvcvMeanstddev    (input, &output[0], &output[1], fptr->sliceWidth);
    #endif
    }
}
