#include "thresholdBinaryU8.h"

void mvcvThresholdBinaryU8(u8** in, u8** out, u8 threshold, u32 width)
{

    int i;
    for (i = 0; i < (int)width; i++)
    {
        if (in[0][i] > threshold)
        {
            out[0][i] = 255;
        }
        else
        {
            out[0][i] = 0;
        }
     }
    return;
}
#ifdef MOVICOMPILE_OPTIMIZED
void mvcvThresholdBinaryU8_opt(u8** in, u8** out, u8 threshold, u32 width)
{
	uchar16 * __restrict inputPtr = (uchar16 *)in[0];
	uchar16 * __restrict outputPtr = (uchar16 *)out[0];

	uchar16 input1, input2, input3, input4, output;
	uchar16 temp1, temp2, temp3, temp4;
	UNUSED(output);
	// Constants to be used in the main loop
	uchar16 vThreshold = threshold;
	uchar16 vConst_255 = 255;

	int i;

	// Prefetch the input data for the main loop (4x16 pixels)
	input1 = inputPtr[0];
	input2 = inputPtr[1];
	input3 = inputPtr[2];
	input4 = inputPtr[3];

	// Execute the main loop
	for (i = 0; i < (int) width; i += 64)
	{
		temp1 = __builtin_shave_vau_isubs_u8_rr(input1, vThreshold); // all nums < threshold become 0
		temp2 = __builtin_shave_vau_isubs_u8_rr(input2, vThreshold); // all nums < threshold become 0
		temp3 = __builtin_shave_vau_isubs_u8_rr(input3, vThreshold); // all nums < threshold become 0
		temp4 = __builtin_shave_vau_isubs_u8_rr(input4, vThreshold); // all nums < threshold become 0

		input1 = inputPtr[4];
		input2 = inputPtr[5];
		input3 = inputPtr[6];
		input4 = inputPtr[7];

		temp1 = __builtin_shave_vau_imuls_u8_rr(temp1, vConst_255); // 0s remain 0, all others become 255
		temp2 = __builtin_shave_vau_imuls_u8_rr(temp2, vConst_255); // 0s remain 0, all others become 255
		temp3 = __builtin_shave_vau_imuls_u8_rr(temp3, vConst_255); // 0s remain 0, all others become 255
		temp4 = __builtin_shave_vau_imuls_u8_rr(temp4, vConst_255); // 0s remain 0, all others become 255

		outputPtr[0] = temp1;
		outputPtr[1] = temp2;
		outputPtr[2] = temp3;
		outputPtr[3] = temp4;

		inputPtr += 4;
		outputPtr += 4;
	}
	return;
}
#endif
