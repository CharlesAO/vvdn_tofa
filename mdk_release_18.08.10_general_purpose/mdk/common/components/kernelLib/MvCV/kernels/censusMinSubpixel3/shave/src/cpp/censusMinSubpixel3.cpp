#include "censusMinSubpixel3.h"
#include "stdio.h"
#include "stdlib.h"

half interpolateF(const unsigned char *buff, half minVal, half maxVal)
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

void mvcvCensusMinSubpixel3(u8 *in, half *out, u8 *outm, u32 width)
{
    u8 cost[3];
    u32 disparities = 3;
    u32 disp;

    for (u32 i = 0; i < width; i++)
    { // for all pixels
        disp = 0;
        u32 minCost = in[i * disparities];
        u32 preCost = in[i * disparities];
        u32 postCost = in[i * disparities + 1];
        
        u8 minPos = disp;
        disp = 1;
        if (in[i * disparities + disp] < minCost)
        { // new min
            minCost = in[i * disparities + disp];
            preCost = in[i * disparities + disp - 1];
            postCost = in[i * disparities + disp + 1];
                
            minPos = (u8)(disp);  
        }
        disp = 2;
        if (in[i * disparities + disp] < minCost)
        { // new min
            minCost = in[i * disparities + disp];
            preCost = in[i * disparities + disp - 1];
            postCost = in[i * disparities + disp];
                
            minPos = (u8)(disp);  
        }

        cost[0] = preCost;
        cost[1] = minCost;
        cost[2] = postCost;
        // Compute and store disparity with subpixel accuracy

        half f = interpolateF(&cost[0], -0.5f, 0.5f);
        out[i] = (half)minPos + f;

        // Store confidence
        outm[i] = minCost;
    }
}
