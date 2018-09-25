///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref laplacian5x5Fp16ToFp16 SIPP filter API.
///    

#ifndef __SIPP_LAPLACIAN5x5_FP16TOFP16_H__
#define __SIPP_LAPLACIAN5x5_FP16TOFP16_H__ 

#include <sipp.h>

/// @sf_definition laplacian5x5Fp16ToFp16 Laplacian 5x5 Fp16 To Fp16
/// @sf_description  The filter applies a Laplacian filter with custom size.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sLaplacian5x5Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/sLaplacian5x5Fp16ToFp16.asm
/// @{


/// @brief Shave function of the @ref laplacian5x5Fp16ToFp16 filter.
void SVU_SYM(svuLaplacian5x5Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLaplacian5x5Fp16ToFp16);

/// @}

#endif //__SIPP_LAPLACIAN5x5_FP16TOFP16_H__
