#include "aggregateCostSGBM64.h"

static inline u8 findMin(u8 a, u8 b)
{
	if (a < b)
		return a;
	return b;
}

void mvcvAggregateCostSGBM64(u8* input1, u8* input2, u8* output, u8 min2, u8 penaltyP2)
{
	u8 min_a, min_b, min1;
	
	// for 64 disparities there are 62 iterations needed because boundaries are handled separately
	u32 startDisp = 1;
	u32 endDisp = 63; 
	
	// penaltyP1 is fixed, penaltyP2 varies depending on pixel's intensity difference 
	u8 penaltyP1 = 10;
	
	// compute min for first disparity
	min_a = findMin(input1[0], min2 + penaltyP2);
	min_b = input1[1] + penaltyP1;
	min1 = findMin(min_a, min_b);
	output[0] = input2[0] + min1 - min2;

	// compute min for 1...63 disparities
	for (u32 disp = startDisp; disp < endDisp; disp++)
	{
		min_a = findMin(input1[disp], min2 + penaltyP2);
		min_b = findMin(input1[disp-1], input1[disp+1]) + penaltyP1;
		min1 = findMin(min_a, min_b);

		output[disp] = input2[disp] + min1 - min2;
	}

	// compute min for last disparity
	min_a = findMin(input1[endDisp], min2 + penaltyP2);
	min_b = input1[endDisp-1] + penaltyP1;
	min1 = findMin(min_a, min_b);
	output[endDisp] = input2[endDisp] + min1 - min2;
}