#include "computeCombinedCost32.h"

void mvcvComputeCombinedCost32(u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor)
{
	u32 disparities = 32;
	u32 result = 0;
	u32 col, disp;

	// compute combined cost for 32 disparities
	for (col = 0; col < width; col++)
	{
		for (disp = 0; disp < disparities; disp++)
		{
			result = (alpha * disparityCost[col * disparities + disp] + beta * adCost[col * disparities + disp]) / normFactor;
			if (result > 255) result = 255;
			disparityCost[col * disparities + disp] = result;
			
		}
	}
}