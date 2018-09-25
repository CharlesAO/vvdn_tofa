#include "aggregateCostSGBM32_clamp.h"

static inline u32 findMin32(u32 a, u32 b)
{
	if (a < b)
		return a;
	return b;
}
void mvcvAggregateCostSGBM32_clamp(u8* input1, u8* input2, u8* output, u16 penaltyP1, u16 penaltyP2, u8 min2, u8 activateClamp)
{
	u16 min_a, min_b, min1;
	
	// for 32 disparities there are 30 iterations needed because boundaries are handled separately
	u32 startDisp = 1;
	u32 endDisp = 31; 
	u16 result;
	
	// compute min for first disparity
	min_a = findMin32(input1[0], min2 + penaltyP2);
	min_b = input1[1] + penaltyP1;
	min1 = findMin32(min_a, min_b);
	result = input2[0] + min1 - min2;

	if ((activateClamp == 1) && (result >255))
		output[0] = 255;
	else
		output[0] = (u8)result;

	// compute min for 1...31 disparities
	for (u32 disp = startDisp; disp < endDisp; disp++)
	{
		min_a = findMin32(input1[disp], min2 + penaltyP2);
		min_b = findMin32(input1[disp-1], input1[disp+1]) + penaltyP1;
		min1 = findMin32(min_a, min_b);

		result = input2[disp] + min1 - min2;
		if ((activateClamp == 1) && (result >255))
			output[disp] = 255;
		else
			output[disp] = (u8)result;
	}

	// compute min for last disparity
	min_a = findMin32(input1[endDisp], (u32)min2 + penaltyP2);
	min_b = input1[endDisp-1] + penaltyP1;
	min1 = findMin32(min_a, min_b);
	result = input2[endDisp] + min1 - min2;
	if ((activateClamp == 1) && (result >255))
		output[endDisp] = 255;
	else
		output[endDisp] = (u8)result;
}
