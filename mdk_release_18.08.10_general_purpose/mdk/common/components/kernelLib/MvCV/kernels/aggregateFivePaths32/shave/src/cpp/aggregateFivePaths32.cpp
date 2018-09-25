#include "aggregateFivePaths32.h"

void mvcvAggregateFivePaths32(u8* path0, u8* path1, u8* path2, u8* path3, u8* path4, u8* out, u32 width)
{
    u32 disparities = 32;
    
    for (u32 k = 0; k < width; k++)
    {
        for (u32 disp = 0; disp < disparities; disp++)
        {
            out[k * disparities + disp] = (path0[k * disparities + disp] + 
                                           path1[k * disparities + disp] + 
                                           path2[k * disparities + disp] + 
                                           path3[k * disparities + disp] + 
                                           path4[k * disparities + disp] ) / 5;
        }
    }
}
