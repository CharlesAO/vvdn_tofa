#ifndef __BILINEAR_INTERPOLATION_ALIGNED_U16_H__
#define __BILINEAR_INTERPOLATION_ALIGNED_U16_H__
#include <mv_types.h>
#include <mvcv_types.h>
#include <mvcv_macro.h>

//!@{
/// bilinearInterpolationAligned16u - calcOpticalFlow component
///@param[in] src		- pointer to the input line
///@param[in] src_step	- step used by input. Must be bigger than 16 for win_size.x bigger than 8
///@param[out] dst		- pointer to the output line
///@param[in] dst_step	- step used by output
///@param[in] win_size	- window size (width and height). Width should be smaller than 16
///@param[in] center	- center coordinates

MVCV_FUNC(void, mvcvBilinearInterpolationAligned16u, const u8* src, s32 src_step, u8* dst, s32 dst_step, ClSize win_size, mvCvPoint2D32f center)


//!@}

#endif // __BILINEAR_INTERPOLATION_U16_H__
