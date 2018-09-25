///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv3x3 SIPP filter API.
///

#ifndef __SIPP_CONV3x3_H__
#define __SIPP_CONV3x3_H__ 

#include <sipp.h>

/// @sf_definition conv3x3 Convolution 3x3
/// @sf_description This filter performs a convolution on the input image using the given 3x3 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution3x3/arch/$(MV_SOC_REV_DIR)/shave/src/convolution3x3.asm
/// @{

/// @brief Parameter structure of the @ref conv3x3 filter.
typedef struct
{
    /// @sf_pfdesc 9 element array with fp16 values containing the 3x3 convolution matrix.
	/// @sf_pfdefvalue 0x2C00,0x3000,0x2C00,0x3000,0x3400,0x3000,0x2C00,0x3000,0x2C00
	/// @sf_pfarraysize 9
	UInt16* cMat;
}
Conv3x3Param;

/// @brief Shave function of the @ref conv3x3 filter.
void SVU_SYM(svuConv3x3)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv3x3);

/// @}

#endif // __SIPP_CONV3x3_H__ 
