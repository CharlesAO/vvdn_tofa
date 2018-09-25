#ifndef __ACCUMULATESQUARE_H__
#define __ACCUMULATESQUARE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
///Adds the square of the source image to the accumulator.
///@param srcAddr  The input image, 8-bit unsigned integer
///@param destAddr The accumulator image, 32-bit floating-point
///@param maskAddr Operation mask
///@param width    Width of input image
///@param height   Number of lines of input image

MVCV_FUNC(void, mvcvAccumulateSquare, u8** srcAddr, u8** maskAddr, fp32** destAddr,u32 width, u32 height)
//!@}

#endif //__ACCUMULATESQUARE_H__