#include "absoluteDiff_intr.h"
#include <stdlib.h>

void AbsoluteDiff_intr(u8** in1, u8** in2, u8** out, u32 width)

{
	u32 i;
	uchar16 *input1 = *(uchar16 **)in1;
	uchar16 *input2 = *(uchar16 **)in2;
	uchar16 *output = *(uchar16 **)out;
	
	width = width >> 4;
    

	for(i=0;i<width;i++)
	{
		output[i] = __builtin_shave_vau_adiff_u8_rr(input1[i], input2[i]);
	}
	
    return;
}