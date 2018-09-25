#include <moviVectorUtils.h>
#include <f32UnitTestSample.h>
#include <stdio.h>

void mvuDotProductStreaming(fp32 *a, fp32 **b, fp32 *out, u32 num_vectors)
{
	u32 i;
	float4 vec_a = {a[0], a[1], a[2], a[3]};
	
	for(i = 0; i < num_vectors; i++)
	{
		float4 vec_b = { b[i][0], b[i][1], b[i][2], b[i][3] }; 
		out[i] = mvuDot(vec_a, vec_b);
	}
}
