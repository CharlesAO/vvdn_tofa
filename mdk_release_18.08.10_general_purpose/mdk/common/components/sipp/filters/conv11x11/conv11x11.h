///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv11x11 SIPP filter API.
///

#ifndef __SIPP_CONV11x11_H__
#define __SIPP_CONV11x11_H__ 

#include <sipp.h>

/// @sf_definition conv11x11 Convolution 11x11
/// @sf_description This filter performs a convolution on the input image using the given 11x11 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 11x11
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution11x11/arch/$(MV_SOC_REV_DIR)/shave/src/convolution11x11.asm
/// @{

/// @brief Parameter structure of the @ref conv11x11 filter.
typedef struct
{
	/// @sf_pfdesc 121 element array with fp16 values containing the 11x11 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 121
    UInt16* cMat;
}
Conv11x11Param;

/// @brief Shave function of the @ref conv11x11 filter.
void SVU_SYM(svuConv11x11)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv11x11);

/// @}

#endif // __SIPP_CONV11x11_H__
