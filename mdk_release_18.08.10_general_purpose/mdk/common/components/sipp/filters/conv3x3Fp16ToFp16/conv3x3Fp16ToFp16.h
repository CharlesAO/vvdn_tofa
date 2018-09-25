///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv3x3Fp16ToFp16 SIPP filter API.
///

#ifndef __SIPP_CONV3x3FP16TOFP16_H__
#define __SIPP_CONV3x3FP16TOFP16_H__ 

#include <sipp.h>

/// @sf_definition conv3x3Fp16ToFp16 Convolution 3x3 Fp16ToFp16
/// @sf_description This filter performs a convolution on the fp16 input image using the given 3x3 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution3x3Fp16ToFp16/arch/$(MV_SOC_REV_DIR)/shave/src/convolution3x3Fp16ToFp16.asm
/// @{

/// @brief Parameter structure of the @ref conv3x3Fp16ToFp16 filter.
typedef struct
{
    /// @sf_pfdesc 9 element array with fp16 values containing the 3x3 convolution matrix.
	/// @sf_pfdefvalue 0x2C00,0x3000,0x2C00,0x3000,0x3400,0x3000,0x2C00,0x3000,0x2C00
	/// @sf_pfarraysize 9
    UInt16 *cMat; //fp16 3x3 matrix
}
Conv3x3Fp16ToFp16Param;

/// @brief Shave function of the @ref conv3x3Fp16ToFp16 filter.
void SVU_SYM(svuConv3x3Fp16ToFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv3x3Fp16ToFp16);

/// @}

#endif // __SIPP_CONV3x3FP16TOFP16_H__ 
