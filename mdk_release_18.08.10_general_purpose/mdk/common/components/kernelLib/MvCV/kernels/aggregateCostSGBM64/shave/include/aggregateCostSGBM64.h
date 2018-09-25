#ifndef __AGGREGATE_COST_SGBM_64__
#define __AGGREGATE_COST_SGBM_64__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvAggregateCostSGBM64 	- computes aggregated cost over a given path considering min2 and penaltyP2 previously computed
/// @param[in] input1			- pointer to previous aggregated costs
/// @param[in] input2			- pointer to current matching costs
/// @param[out] output         	- pointer to output aggregated cost
/// @param[in] min2          	- minimum value from the previous aggregated costs
/// @param[in] penaltyP2       	- penalty value for the case where minimum cost is found at more than +/-1 disparity
/// @return    Nothing

MVCV_FUNC(void, mvcvAggregateCostSGBM64, u8* input1, u8* input2, u8* output, u8 min2, u8 penaltyP2)
//!@}
	
#endif //__AGGREGATE_COST_SGBM_62__