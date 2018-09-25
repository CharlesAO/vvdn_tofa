#include <convolution1x7_step.h>
#include <moviVectorUtils.h>


void Convolution1x7_step0(u8 **in, u8 **out, half conv[8], u32 inWidth)
{
	u32 i, y;
	float sum = 0.0;
	u8 *input = *in;
	u8 *output = *out;
	
	for (i = 0; i < inWidth; i++)
	{
		sum = 0.0;
		for (y = 0; y < 7; y++)
		{
			sum += ((float)input[y - 3]) * conv[y];
		}
		input++;
		if (sum >= 255.0f)
			sum = 255.0f;
		if (sum <= 0.0f)
			sum = 0.0f;
		output[i] = (u8)(sum);
	}
	return;
}
