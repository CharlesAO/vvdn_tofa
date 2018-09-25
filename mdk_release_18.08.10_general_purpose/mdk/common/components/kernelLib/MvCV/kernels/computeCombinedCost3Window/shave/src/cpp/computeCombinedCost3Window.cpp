#include "computeCombinedCost3Window.h"

void mvcvComputeCombinedCost3Window(u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor)
{
    u32 wta_window = 3;
    u32 cost_window = wta_window + 2;
    u32 col, disp;
    u32 result = 0;

    // compute combined cost for 3 disparities
    for (col = 0; col < width; col++)
    {
        for (disp = 0; disp < cost_window; disp++)
        {
            result = (alpha * disparityCost[col * cost_window + disp] + beta * adCost[col * cost_window + disp]) / normFactor;
            if (result > 255) 
                result = 255;
            disparityCost[col * cost_window + disp] = result;
        }
    }
}
