///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv5x1 SIPP filter API.
///

#ifndef __SIPP_CONV5x1_H__
#define __SIPP_CONV5x1_H__ 

#include <sipp.h>

/// @sf_definition conv5x1 Convolution 5x1
/// @sf_description This filter performs a convolution on the input image using the given 5x1 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 5x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution5x1/arch/$(MV_SOC_REV_DIR)/shave/src/convolution5x1.asm
/// @{

/// @brief Parameter structure of the @ref conv5x1 filter.
typedef struct
{
    /// @sf_pfdesc 5 element array with fp16 values containing the 5x1 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 5
    UInt16* cMat;  //fp16 5x1 matrix
}
Conv5x1Param;

/// @brief Shave function of the @ref conv5x1 filter.
void SVU_SYM(svuConv5x1)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv5x1);

/// @}

#endif // __SIPP_CONV5x1_H__
