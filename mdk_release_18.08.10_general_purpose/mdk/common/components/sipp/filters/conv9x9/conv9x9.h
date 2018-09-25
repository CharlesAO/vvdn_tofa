///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv9x9 SIPP filter API.
///

#ifndef __SIPP_CONV9x9_H__
#define __SIPP_CONV9x9_H__ 

#include <sipp.h>

/// @sf_definition conv9x9 Convolution 9x9
/// @sf_description This filter performs a convolution on the input image using the given 9x9 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 9x9
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution9x9/arch/$(MV_SOC_REV_DIR)/shave/src/convolution9x9.asm
/// @{

/// @brief Parameter structure of the @ref conv9x9 filter.
typedef struct
{
    /// @sf_pfdesc 81 element array with fp16 values containing the 9x9 convolution matrix.
	/// @sf_pfdefvalue 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252, 0x2252
	/// @sf_pfarraysize 81
    UInt16* cMat; //fp16 9x9 matrix
}
Conv9x9Param;

/// @brief Shave function of the @ref conv9x9 filter.
void SVU_SYM(svuConv9x9)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv9x9);

/// @}

#endif // __SIPP_CONV9x9_H__
