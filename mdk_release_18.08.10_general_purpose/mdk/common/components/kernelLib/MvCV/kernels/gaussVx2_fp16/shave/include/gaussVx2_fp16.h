#ifndef __GAUSSVX2_FP16_H__
#define __GAUSSVX2_FP16_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel apply downscale 2x vertical with gaussian filters with kernel 5x5. 
/// Have to be used in combination with GaussHx2 to obtain correct output.
/// @param[in]  inLine   - Input line
/// @param[out] outLine  - Output resized line
/// @param[in]  width    - Input line width

MVCV_FUNC(void, mvcvGaussVx2_fp16, half **inLine,half **outLine,int width)
//!@}

#endif //__GAUSSVX2_FP16_H__