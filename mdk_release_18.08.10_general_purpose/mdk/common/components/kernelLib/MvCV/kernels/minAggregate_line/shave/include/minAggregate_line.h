#ifndef __MINAGGREGATE_LINE__
#define __MINAGGREGATE_LINE__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvMinAggregate_line	          - computes aggregated cost over a given path considering min2, penaltyP1 and penaltyP2 previously computed
/// @param[in] input1		- first input line
/// @param[in] input2		- second input line
/// @param[out] output         	- pointer to aggregated cost computed from previous aggregated and current disparity costs
/// @param[in] penaltyP1       	- pointer to penalty applied when minimum cost is located at +/-1 disparity
/// @param[in] penaltyP2       	- pointer to penalty applied when minimum cost is located at more than +/-1 disparity
/// @param[in] min2            	- minimum minimum value from the input line 
/// @param[in] activateClamp            - clamp or not u32 values to u8 
/// @param[in] disparities 		- used to choose witch one of the functions must be called  

MVCV_FUNC(void, mvcvMinAggregate_line, u8* input1, u8* input2, u8* output, u32* penaltyP1, u32* penaltyP2, u32 width, u8 activateClamp, u8 direction, u32 disparities)

//!@}
	
#endif //__MINAGGREGATE_LINE__

