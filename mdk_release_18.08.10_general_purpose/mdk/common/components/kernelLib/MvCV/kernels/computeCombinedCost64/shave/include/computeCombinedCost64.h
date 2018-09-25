#ifndef __COMPUTE_COMBINED_COST_64__
#define __COMPUTE_COMBINED_COST_64__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvComputeCombinedCost6464 	- computes disparity cost of AD-Census 
/// @param[in] disparityCost	- matching cost
/// @param[in] adCost			- absolute difference cost
/// @param[in] alpha			- weight factor for disparityCost
/// @param[in] beta			    - weight factor for absolute difference cost
/// @param[in] width         	- image width
/// @param[in] normFactor       - normalization factor to fit in u8
/// @return    Nothing

MVCV_FUNC(void, mvcvComputeCombinedCost64, u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvComputeCombinedCost64_opt(u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor);
}
#endif //MOVICOMPILE_OPTIMIZED
	
#endif //__COMPUTE_COMBINED_COST_64__

