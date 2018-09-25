///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv1x5Fp16ToFp16 SIPP filter API.
///

#ifndef __SIPP_CONV1x5FP16TOFP16_H__
#define __SIPP_CONV1x5FP16TOFP16_H__ 

#include <sipp.h>

/// @sf_definition conv1x5Fp16ToFp16 Convolution 1x5 Fp16ToFp16
/// @sf_description This filter performs a convolution on the fp16 input image using the given 1x5 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution1x5Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/convolution1x5Fp16ToFp16.asm
/// @{

/// @brief Parameter structure of the @ref conv1x5Fp16ToFp16 filter.
typedef struct
{
    /// @sf_pfdesc 5 element array with fp16 values containing the 1x5 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 5
    UInt16* cMat; //fp16 1x5 matrix
}
Conv1x5Fp16ToFp16Param;

/// @brief Shave function of the @ref conv1x5Fp16ToFp16 filter.
void SVU_SYM(svuConv1x5Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv1x5Fp16ToFp16);

/// @}

#endif // __SIPP_CONV1x5FP16TOFP16_H__ 
