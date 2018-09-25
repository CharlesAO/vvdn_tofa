///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv15x1 SIPP filter API.
///

#ifndef __SIPP_CONV15x1_H__
#define __SIPP_CONV15x1_H__ 

#include <sipp.h>

/// @sf_definition conv15x1 Convolution 15x1
/// @sf_description This filter performs a convolution on the input image using the given 15x1 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 15x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution15x1/arch/$(MV_SOC_REV_DIR)/shave/src/convolution15x1.asm
/// @{

/// @brief Parameter structure of the @ref conv15x1 filter.
typedef struct
{
    /// @sf_pfdesc 15 element array with fp16 values containing the 15x1 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 15
    UInt16* cMat;
}
Conv15x1Param;

/// @brief Shave function of the @ref conv15x1 filter.
void SVU_SYM(svuConv15x1)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv15x1);

/// @}

#endif // __SIPP_CONV15x1_H__
