///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convSeparable5x5Fp16ToFp16 SIPP filter API.
///

#ifndef __SIPP_CONV_SEPARABLE_5x5FP16TOFP16__
#define __SIPP_CONV_SEPARABLE_5x5FP16TOFP16__

#include <sipp.h>

/// @sf_definition convSeparable5x5Fp16ToFp16 Convolution Separable 5x5 Fp16ToFp16
/// @sf_description This filter performs a separable convolution on the fp16 input image using the given 5x5 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convSeparable5x5Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/convSeparable5x5Fp16ToFp16.asm
/// @{

/// @brief Parameter structure of the @ref convSeparable5x5Fp16ToFp16 filter.
typedef struct
{
	/// @sf_pfarraysize 3
	/// @sf_pfdesc Array with 3 fp32 values that represent the separable values for 5x5 matrix.
    UInt16* cMat; //fp32 3 values represent the separable values for 5x5 matrix
}
ConvSeparable5x5Fp16ToFp16Param;

/// @brief Shave function of the @ref convSeparable5x5Fp16ToFp16 filter.
void SVU_SYM(svuConvSeparable5x5Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvSeparable5x5Fp16ToFp16);

/// @}

#endif // __SIPP_CONV_SEPARABLE_5x5FP16TOFP16__
