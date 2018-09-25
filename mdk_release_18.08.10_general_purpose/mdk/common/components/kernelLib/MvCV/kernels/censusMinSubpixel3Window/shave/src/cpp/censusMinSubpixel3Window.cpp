#include "censusMinSubpixel3Window.h"
#include "stdio.h"
#include "stdlib.h"

static half interpolateF(const unsigned char *buff, half minVal, half maxVal)
{
    const unsigned char f0 = buff[0];
    const unsigned char f1 = buff[1];
    const unsigned char f2 = buff[2];

    unsigned short a = f0 + f2 - 2 * f1;  // assuming here f1 <= f0 && f1<= f2

    if (a == 0)
    {
        return 0.f;
    }

    short denom = 2 * a;
    short numer = f0 - f2;

    half x = (half)numer / (half)denom;
    if (minVal > x)
        x = minVal;
    if (x > maxVal)
        x = maxVal;

    return x;
}

void mvcvCensusMinSubpixel3Window(u8 *in, half *out, u8 *outm, u32 width)
{
    u8 cost[3];
    u32 wta_window = 3;
    u32 cost_window = wta_window + 2;
    u32 begin_wta_window = cost_window - wta_window - 1;
    u32 end_wta_window = wta_window;
    u32 disp;

    for (u32 i = 0; i < width; i++)
    { // for all pixels
        disp = 0;

        u32 minCost = in[i * cost_window + begin_wta_window];
        u32 preCost = in[i * cost_window + begin_wta_window - 1];
        u32 postCost = in[i * cost_window + begin_wta_window + 1];
        u8 minPos = disp;

        //WTA
        for(disp = 1; disp < end_wta_window; disp++){
            if (in[i * cost_window + begin_wta_window + disp] < minCost){//new min
                minCost = in[i * cost_window + begin_wta_window + disp];
                preCost = in[i * cost_window + begin_wta_window + disp - 1];
                postCost = in[i * cost_window + begin_wta_window + disp + 1];
                minPos = (u8)(disp);  
            }
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
