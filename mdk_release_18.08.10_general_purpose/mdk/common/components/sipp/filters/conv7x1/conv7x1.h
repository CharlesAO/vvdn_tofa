///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv7x1 SIPP filter API.
///

#ifndef __SIPP_CONV7x1_H__
#define __SIPP_CONV7x1_H__ 

#include <sipp.h>

/// @sf_definition conv7x1 Convolution 7x1
/// @sf_description This filter performs a convolution on the input image using the given 7x1 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 7x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution7x1/arch/$(MV_SOC_REV_DIR)/shave/src/convolution7x1.asm
/// @{

/// @brief Parameter structure of the @ref conv7x1 filter.
typedef struct
{
    /// @sf_pfdesc 7 element array with fp16 values containing the 7x1 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 7
    UInt16* cMat; 
}
Conv7x1Param;

/// @brief Shave function of the @ref conv7x1 filter.
void SVU_SYM(svuConv7x1)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv7x1);

/// @}

#endif // __SIPP_CONV7x1_H__
