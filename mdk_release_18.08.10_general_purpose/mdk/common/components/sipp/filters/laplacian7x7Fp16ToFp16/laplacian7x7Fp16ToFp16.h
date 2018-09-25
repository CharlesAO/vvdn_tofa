///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref laplacian7x7Fp16ToFp16 SIPP filter API.
///    

#ifndef __SIPP_LAPLACIAN7x7_FP16TOFP16_H__
#define __SIPP_LAPLACIAN7x7_FP16TOFP16_H__ 

#include <sipp.h>

/// @sf_definition laplacian7x7Fp16ToFp16 Laplacian 7x7 Fp16 To Fp16
/// @sf_description  The filter applies a Laplacian filter with custom size.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sLaplacian7x7Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/sLaplacian7x7Fp16ToFp16.asm
/// @{

/// @brief Shave function of the @ref laplacian7x7Fp16ToFp16 filter.
void SVU_SYM(svuLaplacian7x7Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLaplacian7x7Fp16ToFp16);

/// @}

#endif //__SIPP_LAPLACIAN7x7_FP16TOFP16_H__
