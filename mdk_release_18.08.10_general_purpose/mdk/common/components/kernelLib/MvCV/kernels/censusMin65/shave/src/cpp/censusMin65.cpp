#include "censusMin65.h"

void mvcvCensusMin65(u8 *in, u8 *out, u32 width)
{
	u32 disparities = 65;
	for (u32 i = 0; i < width; i++)
    { // for all pixels
        
        u32 minCost = in[i * disparities];
        u8 minPos = 0;

        for (u32 disp = 1; disp < disparities; disp++)
        { // for all disparities

            if (in[i * disparities + disp] < minCost)
            { // new min
                minCost = in[i * disparities + disp];
                minPos = (u8)(disp);
            }
        }

        // Store the position of min
        out[i] = minPos;
    }
}
