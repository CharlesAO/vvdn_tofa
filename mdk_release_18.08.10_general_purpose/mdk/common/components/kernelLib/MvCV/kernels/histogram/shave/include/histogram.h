#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel computes a histogram on a given line (has to be applied to all lines of an image)
/// @param[in] in     - Input line      
/// @param[out] hist  - Array of values from histogram
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvHistogram, u8** in, u32 *hist, u32 width)
//!@}

#endif //__HISTOGRAM_H__