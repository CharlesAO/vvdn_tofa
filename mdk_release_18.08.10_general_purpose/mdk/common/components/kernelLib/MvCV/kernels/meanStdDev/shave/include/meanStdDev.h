#ifndef __MEANSTDDEV_H__
#define __MEANSTDDEV_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
///This kernel calculates mean and standard deviation of an array of elements
///@param[in]  in     - Input line
///@param[out] mean   - Computed mean value
///@param[out] stddev - Computed standard deviation
///@param[out] width  - Width of line

MVCV_FUNC(void, mvcvMeanstddev, u8** in, float *mean, float *stddev, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvMeanstddev_opt(u8** in, float *mean, float *stddev, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__MEANSTDDEV_H__
