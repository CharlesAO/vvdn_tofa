#ifndef __AGGREGATE_COST_SGBM_64_CLAMP__
#define __AGGREGATE_COST_SGBM_64_CLAMP__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvAggregateCostSGBM64_clamp 	- computes aggregated cost over a given path considering min2, penaltyP1 and penaltyP2 previously computed
/// @param[in] input1			- pointer to previous aggregated costs with SGBM
/// @param[in] input2			- pointer to current disparity costs
/// @param[out] output         	- pointer to aggregated cost computed from previous aggregated and current disparity costs
/// @param[in] min2          	- minimum cost from previous aggregated costs
/// @param[in] penaltyP1       	- penalty applied when minimum cost is located at +/-1 disparity
/// @param[in] penaltyP2       	- penalty applied when minimum cost is located at more than +/-1 disparity
/// @param[in] activateClamp    - clamp or not u32 values to u8 
/// @return    Nothing

MVCV_FUNC(void, mvcvAggregateCostSGBM64_clamp, u8* input1, u8* input2, u8* output, u16 penaltyP1, u16 penaltyP2, u8 min2, u8 activateClamp)
//!@}
	
#endif //__AGGREGATE_COST_SGBM_64_CLAMP__