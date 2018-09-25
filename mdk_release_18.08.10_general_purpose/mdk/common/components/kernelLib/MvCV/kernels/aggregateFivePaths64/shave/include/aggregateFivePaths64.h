#ifndef __AGGREGATE_FIVE_PATHS_64__
#define __AGGREGATE_FIVE_PATHS_64__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvAggregateFivePaths64 	- computes average sum from 5 paths (width x 64 arrays)
/// @param[in] path0			- pointer to a width x 64 array from path0
/// @param[in] path1			- pointer to a width x 64 array from path1
/// @param[in] path2			- pointer to a width x 64 array from path2
/// @param[in] path3			- pointer to a width x 64 array from path3
/// @param[in] path4			- pointer to a width x 64 array from path4
/// @param[out] output         	- pointer to output result
/// @param[in] width          	- line width
/// @return    Nothing

MVCV_FUNC(void, mvcvAggregateFivePaths64, u8* path0, u8* path1, u8* path2, u8* path3, u8* path4, u8* out, u32 width)
//!@}
	
#endif //__AGGREGATE_FIVE_PATHS_64__
