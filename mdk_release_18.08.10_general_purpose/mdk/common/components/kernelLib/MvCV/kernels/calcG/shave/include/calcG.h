#ifndef __CALC_G_H__
#define __CALC_G_H__
#include <mv_types.h>
#include <mvcv_types.h>
#include <mvcv_macro.h>

//!@{
/// calcG - calcOpticalFlow component 
///@param[in] Ix		- pointer tot the X line 
///@param[in] Iy		- pointer tot the Y line 
///@param[in] isz		- ClSize
///@param[in] jsz		- ClSize
///@param[in] minI		- mvCvPoint
///@param[in] minJ		- mvCvPoint
///@param[out] G		- output scalar

MVCV_FUNC(void, mvcvCalcG, const float* Ix, ClSizeW isz, mvCvPointW minI, const float* Iy, ClSizeW jsz, mvCvPointW minJ,  mvScalar& G)

//!@}

#endif // __CALC_G_H__
