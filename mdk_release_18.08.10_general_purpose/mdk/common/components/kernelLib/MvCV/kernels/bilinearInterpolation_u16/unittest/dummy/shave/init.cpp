///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    bilinearInterpolation_u16 kernel function call for unitary test
///

#include <bilinearInterpolation_u16.h>
#include <mv_types.h>
#include <svuCommonShave.h>
#include <stdio.h>

#define MAX_FRAME_SIZE 5000

u8 src[MAX_FRAME_SIZE];
u8 dst[MAX_FRAME_SIZE];
s32 src_step;
s32 dst_step;

s32 win_size_s32[2];
fp32 center_fp32[2]; 
ClSize win_size;
mvCvPoint2D32f center;



int my2Round( float value )
{
   int a;
   a =   (int)(value + (value >= 0 ? 0.5 : -0.5));
   return a;
}

int my2Floor( float value )
{
    int i = my2Round(value);
    mvCv32suf diff;
    diff.f = (float)(value - i);
    return i - (diff.i < 0);
}

int main( void )
{
//	void mvcvBilinearInterpolation16u(const u8* src, s32 src_step, u8* dst, s32 dst_step, ClSize win_size, mvCvPoint2D32f center)

	win_size.x = win_size_s32[0];
	win_size.y = win_size_s32[1];
	center.x = center_fp32[0];
	center.y = center_fp32[1];
	
	
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvBilinearInterpolation16u(src, src_step, dst, dst_step, win_size, center);
	#else
		mvcvBilinearInterpolation16u_asm(src, src_step, dst, dst_step, win_size, center);
	#endif
	
	
	SHAVE_HALT; 
	return 0;
}
