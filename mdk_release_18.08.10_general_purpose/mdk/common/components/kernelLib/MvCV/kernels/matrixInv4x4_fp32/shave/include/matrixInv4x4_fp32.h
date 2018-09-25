#ifndef __MATRIX_INV_4x4_FP32_H__
#define __MATRIX_INV_4x4_FP32_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel inverse a 4x4 matrix
/// @param[in] in          - input matrix
/// @param[out] out        - output matrix

	
MVCV_FUNC(void, mvcvMatrixInv4x4_fp32, fp32 *in, fp32 *out)
//!@}

#endif //__MATRIX_INV_4x4_FP32_H__