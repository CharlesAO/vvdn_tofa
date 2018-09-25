#include <conv3x3fp32Scharr.h>
#include <mvcv_types.h>
#include <stdio.h>
#include <stdlib.h>
/// conv3x3fp32Scharr - calcOpticalFlow component
///@param[in] src		- pointer tot the input line
///@param[in] smooth_k	-
///@param[in] width		- source width
///@param[out] dstX		- pointer to the output X line
///@param[out] dstY		- pointer to the output Y line


void mvcvConv3x3fp32Scharr( const u8* src, float* dstX, float* dstY, float* smooth_k, int height, int width)
{
	int x;
	int src_width = width;
	int h = height - 2;

	int src_step = width;
	int dst_step = width - 2;

	for (; h--; src += src_step, dstX += dst_step, dstY += dst_step)
	{
		const u8* src2 = src + src_step;
		const u8* src3 = src + src_step * 2;
		float b0_m2, b0_m1, b0_m0; // b0(x-2), b0(x-1), b0(x)
		float b1_m2, b1_m1, b1_m0; // b1(x-2), b1(x-1), b1(x)

		// calculate first couple of Y-axis scharr parts
		b0_m2 = (src3[0] + src[0]) * smooth_k[0] + src2[0] * smooth_k[1];
		b1_m2 = src3[0] - src[0];
		b0_m1 = (src3[1] + src[1]) * smooth_k[0] + src2[1] * smooth_k[1];
		b1_m1 = src3[1] - src[1];


		for (x = 2; x < src_width; x++)
		{
			b0_m0 = (src3[x] + src[x]) * smooth_k[0] + src2[x]	* smooth_k[1];
			b1_m0 = src3[x] - src[x];
			dstX[x - 2] = b0_m0 - b0_m2;
			dstY[x - 2] = (b1_m2 + b1_m0) * smooth_k[0] + b1_m1	* smooth_k[1];

			b0_m2 = b0_m1;
			b0_m1 = b0_m0;
			b1_m2 = b1_m1;
			b1_m1 = b1_m0;
		}
	}
}

