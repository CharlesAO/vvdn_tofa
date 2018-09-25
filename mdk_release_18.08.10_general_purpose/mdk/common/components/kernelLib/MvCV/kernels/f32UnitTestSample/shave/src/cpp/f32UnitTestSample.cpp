#include "f32UnitTestSample.h"

void dotProductStreaming(fp32 *a, fp32 **b, fp32 *out, u32 num_vectors)
{
	for(u32 i=0; i < num_vectors; i++)
	{
		fp32 sum = 0;
		for(u8 j=0; j < 4; j++)
			sum += a[j] * b[i][j];
		out[i] = sum;
	}
}
