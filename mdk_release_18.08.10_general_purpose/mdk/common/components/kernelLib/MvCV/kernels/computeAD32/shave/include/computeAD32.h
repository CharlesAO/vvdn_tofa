#ifndef __COMPUTEAD32__
#define __COMPUTEAD32__

#include <mv_types.h>
#include <mvcv_macro.h>
    
//!@{
/// mvcvComputeAD32             - compute absolute difference between each pixel from left image and 32 left pixels from right image
/// @param[in] inputLeft        - pointer to left image
/// @param[in] inputRight       - pointer to right image
/// @param[out] output          - pointer to output - absolute difference
/// @param[in] width            - image width
/// @param[in] flag             - enable right crossing (by default left crossing)
/// @return    Nothing

MVCV_FUNC(void, mvcvComputeAD32, u8* inputLeft, u8* inputRight, u8* output, u32 width, u32 flag)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvComputeAD32_opt( u8* inputLeft, u8* inputRight, u8* output, u32 width, u32 flag);
}
#endif //MOVICOMPILE_OPTIMIZED
    
#endif //__COMPUTEAD32__

