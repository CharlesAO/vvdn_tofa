#include "censusMinConfidence64Subpixel.h"
#include "stdio.h"
#include "stdlib.h"

half local_interpolateF64(const unsigned char *buff, half minVal, half maxVal)
{
    const unsigned char f0 = buff[0];
    const unsigned char f1 = buff[1];
    const unsigned char f2 = buff[2];

    unsigned short a = f0 + f2 - 2 * f1;  // assuming here f1 <= f0 && f1<= f2
    if (a == 0)
    {
        return 0.f;
    }

    int denom = 2 * a;
    int numer = f0 - f2;

    half x = (half)numer / (half)denom;
    if (minVal > x)
        x = minVal;
    if (x > maxVal)
        x = maxVal;

    return x;
}

void mvcvCensusMinConfidence64Subpixel(u8 *input, half *minimumPosition, u8* minimumValue, u32 width)
{
    //u32 j;
    u32 disparities = 64;
    u32 scaleFactor = 255;

    u8 minCost = 255, preCost, postCost;
    u8 secondMinCost = 255;
    u8 minPos = 0;
    u32 disp;

    u8 costInterp[3];

    for (u32 i = 0; i < width; i++)
    {
        secondMinCost = 255;
        // compute minimum value and minimum position for first disparity
        disp = 0;
        minCost = input[i * disparities];
        preCost = input[i * disparities];
        postCost = input[i * disparities + 1];
        minPos = 0;

        // compute minimum value and minimum position
        for (disp = 1; disp < (disparities-1); disp++)
        {
            if (input[i * disparities + disp] < minCost)
            {
                minCost = input[i * disparities + disp];
                preCost = input[i * disparities + disp - 1];
                postCost = input[i * disparities + disp + 1];
                minPos = disp;
            }
        }

        // compute minimum value and minimum position for last disparity
        disp = disparities-1;
        if (input[i * disparities + disp] < minCost)
        {
            minCost = input[i * disparities + disp];
            preCost = input[i * disparities + disp - 1];
            postCost = input[i * disparities + disp];
            minPos = disp;
        }
        ////
        // compute minimum disparity with subpixel precision
        costInterp[0] = preCost;
        costInterp[1] = minCost;
        costInterp[2] = postCost;

        half f = local_interpolateF64(&costInterp[0], -0.5f, 0.5f);
        minimumPosition[i] = (half)minPos + f;

        // check if first pixel is second min
        if (input[i * disparities + 0] < input[i * disparities + 1] && // is smaller than the right neighbour
            input[i * disparities + 0] != minCost
            &&  input[i * disparities + 0] < secondMinCost) // is not minimum value already found
        {
            secondMinCost = input[i * disparities + 0];
        }
        // compute second minimum value which is also local minimum (minimum of its two neighbours)
        for (disp = 1; disp < (disparities-1); disp++)
        {
            if (input[i * disparities + disp] < input[i * disparities + disp - 1] && // is smaller than the left neighbour
                input[i * disparities + disp] < input[i * disparities + disp + 1] && // is smaller than the right neighbour
                input[i * disparities + disp] != minCost
                &&  input[i * disparities + disp] < secondMinCost) // is not minimum value already found
            {
                secondMinCost = input[i * disparities + disp];
            }
        }
        // check if last pixel is second min
        if (input[i * disparities + (disparities - 1)] < input[i * disparities + (disparities - 1) - 1] && // is smaller than the left neighbour
            input[i * disparities + (disparities - 1)] != minCost
            &&  input[i * disparities + (disparities - 1)] < secondMinCost) // is not minimum value already found
        {
            secondMinCost = input[i * disparities + (disparities - 1)];
        }

        minimumValue[i] = scaleFactor * minCost / secondMinCost;
    }
}
