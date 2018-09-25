#ifndef __COMPUTEAD64__
#define __COMPUTEAD64__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvComputeAD64             - compute absolute difference between each pixel from left image and 64 left pixels from right image
/// @param[in] inputLeft        - pointer to left image
/// @param[in] inputRight       - pointer to right image
/// @param[in] flag             - enable right crossing (by default left crossing)
/// @param[out] output          - pointer to output - absolute difference
/// @param[in] width            - image width
/// @return    Nothing

MVCV_FUNC(void, mvcvComputeAD64, u8* inputLeft, u8* inputRight, u8* output, u32 width, u32 flag)
//!@}
    
#endif //__COMPUTEAD64__