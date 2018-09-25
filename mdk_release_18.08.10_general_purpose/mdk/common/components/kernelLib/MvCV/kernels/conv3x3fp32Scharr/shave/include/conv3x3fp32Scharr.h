#ifndef __CONV3x3FP32_SCHARR_H__
#define __CONV3x3FP32_SCHARR_H__
#include <mv_types.h>
#include <mvcv_types.h>
#include <mvcv_macro.h>

//!@{
/// conv3x3fp32Scharr - calcOpticalFlow component
///@param[in] src       - pointer to the input line
///@param[in] smooth_k  - should be set to smooth_k[0] = 0.09375 and smooth_k[1] =  0.3125
///                       to do the exact output as the asm code
///@param[in] width     - source width
///                       the assembly code was optimized for width = 13 and width = 21
///
///@param[out] dstX     - pointer to the output X line
///@param[out] dstY     - pointer to the output Y line

MVCV_FUNC(void, mvcvConv3x3fp32Scharr, const u8* src, float* dstX, float* dstY, float* smooth_k, int y, int width)
//!@}

#endif // __CONV3x3FP32_SCHARR_H__
