///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv9x9Fp16ToFp16 SIPP filter API.
///

#ifndef __SIPP_CONV9x9FP16TOFP16_H__
#define __SIPP_CONV9x9FP16TOFP16_H__ 

#include <sipp.h>

/// @sf_definition conv9x9Fp16ToFp16 Convolution 9x9 Fp16ToFp16
/// @sf_description This filter performs a convolution on the fp16 input image using the given 9x9 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 9x9
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution9x9Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/convolution9x9Fp16ToFp16.asm
/// @{

/// @brief Parameter structure of the @ref conv9x9Fp16ToFp16 filter.
typedef struct
{
    /// @sf_pfdesc 81 element array with fp16 values containing the 9x9 convolution matrix.
	/// @sf_pfdefvalue
	/// @sf_pfarraysize 81
    UInt16* cMat; //fp16 9x9 matrix
}
Conv9x9Fp16ToFp16Param;

/// @brief Shave function of the @ref conv9x9Fp16ToFp16 filter.
void SVU_SYM(svuConv9x9Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv9x9Fp16ToFp16);

/// @}

#endif // __SIPP_CONV9x9FP16TOFP16_H__ 
