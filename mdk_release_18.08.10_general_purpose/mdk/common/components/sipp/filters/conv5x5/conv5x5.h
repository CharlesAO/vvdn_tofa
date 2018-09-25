///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv5x5 SIPP filter API.
///

#ifndef __SIPP_CONV5x5_H__
#define __SIPP_CONV5x5_H__ 

#include <sipp.h>

/// @sf_definition conv5x5 Convolution 5x5
/// @sf_description This filter performs a convolution on the input image using the given 5x5 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution5x5/arch/$(MV_SOC_REV_DIR)/shave/src/convolution5x5.asm
/// @{

/// @brief Parameter structure of the @ref conv5x5 filter.
typedef struct
{
    /// @sf_pfdesc 25 element array with fp16 values containing the 5x5 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 25
    UInt16* cMat; 
}
Conv5x5Param;

/// @brief Shave function of the @ref conv5x5 filter.
void SVU_SYM(svuConv5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv5x5);

/// @}

#endif // __SIPP_CONV5x5_H__

