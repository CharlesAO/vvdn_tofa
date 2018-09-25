#ifndef __EQUALIZEHIST_H__
#define __EQUALIZEHIST_H__
#include <mvcv_macro.h>

#include <mv_types.h>

//!@{	
/// This kernel makes an equalization through an input line with a given histogram
/// @param[in] in         - Input line
/// @param[out] out       - Output line
/// @param[in] hist       - Pointer to an input array that indicates the cumulative histogram of the image
/// @param[in] width      - Width of input line

MVCV_FUNC(void, mvcvEqualizeHist,u8** in, u8** out, u32 *hist, u32 width)
//!@}

#endif //__EQUALIZEHIST_H__