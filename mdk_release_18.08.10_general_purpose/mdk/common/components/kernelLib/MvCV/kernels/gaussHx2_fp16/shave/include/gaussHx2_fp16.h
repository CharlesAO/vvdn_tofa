
#ifndef __GAUSSHX2_FP16_H__
#define __GAUSSHX2_FP16_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel apply downscale 2x horizontal with a gaussian filters with kernel 5x5. 
/// Have to be used in combination with GaussVx2 to obtain correct output.
/// @param[in]  inLine   -  Input line
/// @param[out] outLine  -  Output resized line
/// @param[in]  width    -  Input line width (must be multiple of 16)

MVCV_FUNC(void, mvcvGaussHx2_fp16, half **inLine,half **outLine,int width)
//!@}

#endif //__GAUSSHX2_FP16_H__