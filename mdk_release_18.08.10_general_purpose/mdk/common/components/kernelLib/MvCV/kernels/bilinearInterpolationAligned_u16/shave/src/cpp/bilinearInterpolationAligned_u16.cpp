#include <bilinearInterpolationAligned_u16.h>
#include <mvcv_types.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/// bilinearInterpolation16u - calcOpticalFlow component
///@param[in] src		- pointer tot the input line
///@param[in] src_step	- step used by input
///@param[out] dst		- pointer tot the output line
///@param[in] dst_step	- step used by output
///@param[in] win_size	- window size (width and height)
///@param[in] center	- center coordinates


static int myRound( float value )
{
   int a;
   a =   (int)(value + (value >= 0 ? 0.5 : -0.5));
   return a;
}

static int myFloor( float value )
{
    int i = myRound(value);
    mvCv32suf diff;
    diff.f = (float)(value - i);
    return i - (diff.i < 0);
}


void mvcvBilinearInterpolationAligned16u(const u8* src, s32 src_step, u8* dst, s32 dst_step, ClSize win_size, mvCvPoint2D32f center)
{
	mvCvPoint ip;
	float a, b;
	const u8* src0;
	const u8* src1;

	// Translate center to top-left corner of the patch
	center.x -= ((win_size.x - 1) * 0.5f);
	center.y -= ((win_size.y - 1) * 0.5f);

	ip.x = myFloor(center.x);
	ip.y = myFloor(center.y);

	// Compute bilinear weights
	a = center.x - ip.x;
	b = center.y - ip.y;

	/*float c00 = (1.f - a) * (1.f - b);
	float c01 = (1.f - b) * a;
	float c10 = (1.f - a) * b;
	float c11 = a * b;*/
	fp32 firstSum = (u16)((1.f - b) * 256);
	fp32 secondSum = 256 - firstSum;
	u16 c00 = (u16)((1.f - a) * firstSum + 0.5);
	u16 c01 = (u16)(firstSum - c00);
	u16 c10 = (u16)((1.f - a) * secondSum + 0.5);
	u16 c11 = (u16)(secondSum - c10);

//	printf ("%f %f\n", a, b);
//	printf ("c00: %d\n", c00);
//	printf ("c01: %d\n", c01);
//	printf ("c10: %d\n", c10);
//	printf ("c11: %d\n", c11);
//	printf ("sum: %d\n", c00 + c01 + c11 + c10);

	src0 = src;
	src1 = src0 + src_step;
	u32 j;
	for (; win_size.y--; dst += dst_step, src0 = src1, src1 += src_step)
	{
	    for ( j = 0; j < (u32)win_size.x; j++)
	    {
	        //float t = c00 * src0[j] + c01 * src0[j + 1] + c10 * src1[j] + c11 * src1[j + 1];
	        u8 temp = (u8)(((c00 * src0[j] + c01 * src0[j + 1] + c10 * src1[j] + c11 * src1[j + 1]) + 128) >> 8);
	        dst[j] = temp;
	    }
	}
}
