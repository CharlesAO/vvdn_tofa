#ifndef __AGGREGATE_ALL_PATHS_64__
#define __AGGREGATE_ALL_PATHS_64__

#include <mv_types.h>
#include <mvcv_macro.h>

#define DISPARITIES (64)

//!@{
/// mvcvAggregateAllPaths64 	- computes average sum from 4 paths (width x 64 arrays)
/// @param[in] path0			- pointer to a width x 64 array from path0
/// @param[in] path1			- pointer to a width x 64 array from path1
/// @param[in] path2			- pointer to a width x 64 array from path2
/// @param[in] path3			- pointer to a width x 64 array from path3
/// @param[out] output         	- pointer to output result
/// @param[in] width          	- line width
/// @return    Nothing

MVCV_FUNC(void, mvcvAggregateAllPaths64, u8* path0, u8* path1, u8* path2, u8* path3, u8* out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
    void mvcvAggregateAllPaths64_opt(u8* path0, u8* path1, u8* path2, u8* path3, u8* out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__AGGREGATE_ALL_PATHS_64__
