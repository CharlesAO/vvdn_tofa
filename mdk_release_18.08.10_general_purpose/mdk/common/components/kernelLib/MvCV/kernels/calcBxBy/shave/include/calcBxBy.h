#ifndef __CALC_BX_BY_H__
#define __CALC_BX_BY_H__
#include <mv_types.h>
#include <mvcv_types.h>
#include <mvcv_macro.h>
    
//!@{
/// conv3x3fp32_13x13 - calcOpticalFlow component 
///@param[in] src          - pointer tot the input line
///@param[in] smooth_k     -  
///@param[out] dstX        - pointer to the output X line
///@param[out] dstY        - pointer to the output Y line

MVCV_FUNC(mvCvPoint2D32f, mvcvCalcBxBy, const u8* patchI, ClSizeW isz, const float* Ix, const float* Iy, mvCvPointW minI,
    const u8* patchJ, ClSizeW jsz, mvCvPointW minJ, float* error)

//!@}

#endif // __CALC_BX_BY_H__
