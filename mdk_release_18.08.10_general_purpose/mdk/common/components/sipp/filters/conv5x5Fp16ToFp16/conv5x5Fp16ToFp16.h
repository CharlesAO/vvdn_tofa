///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv5x5Fp16ToFp16 SIPP filter API.
///

#ifndef __SIPP_CONV5x5FP16TOFP16_H__
#define __SIPP_CONV5x5FP16TOFP16_H__ 

#include <sipp.h>

/// @sf_definition conv5x5Fp16ToFp16 Convolution 5x5 Fp16ToFp16
/// @sf_description This filter performs a convolution on the fp16 input image using the given 5x5 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype half, fp16
/// @sf_inputs
/// 	- datatypes: half, fp16; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution5x5Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/convolution5x5Fp16ToFp16.asm
/// @{

/// @brief Parameter structure of the @ref conv5x5Fp16ToFp16 filter.
typedef struct
{
    /// @sf_pfdesc 25 element array with fp16 values containing the 5x5 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 25
    UInt16* cMat; 
}
Conv5x5Fp16ToFp16Param;

/// @brief Shave function of the @ref conv5x5Fp16ToFp16 filter.
void SVU_SYM(svuConv5x5Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv5x5Fp16ToFp16);
/// @}

#endif // __SIPP_CONV5x5FP16TOFP16_H__ 
