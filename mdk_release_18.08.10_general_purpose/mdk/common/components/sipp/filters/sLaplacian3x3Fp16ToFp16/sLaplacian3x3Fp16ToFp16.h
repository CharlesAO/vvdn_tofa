///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref laplacian3x3Fp16ToFp16 SIPP filter API.
///    

#ifndef __SIPP_LAPLACIAN3x3_FP16TOFP16_H__
#define __SIPP_LAPLACIAN3x3_FP16TOFP16_H__ 

#include <sipp.h>

/// @sf_definition laplacian3x3Fp16ToFp16 Laplacian 3x3 Fp16 To Fp16
/// @sf_description  The filter applies a Laplacian filter with custom size.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sLaplacian3x3Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/sLaplacian3x3Fp16ToFp16.asm
/// @{

/// @brief Shave function of the @ref laplacian3x3Fp16ToFp16 filter.
void SVU_SYM(svusLaplacian3x3Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svusLaplacian3x3Fp16ToFp16);

/// @}


#endif //__SIPP_LAPLACIAN3x3_FP16TOFP16_H__
