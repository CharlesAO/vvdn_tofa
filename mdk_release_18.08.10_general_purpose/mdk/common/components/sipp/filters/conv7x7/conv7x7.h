///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv7x7 SIPP filter API.
///

#ifndef __SIPP_CONV7x7_H__
#define __SIPP_CONV7x7_H__ 

#include <sipp.h>

/// @sf_definition conv7x7 Convolution 7x7
/// @sf_description This filter performs a convolution on the input image using the given 7x7 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution7x7/arch/$(MV_SOC_REV_DIR)/shave/src/convolution7x7.asm
/// @{

/// @brief Parameter structure of the @ref conv7x7 filter.
typedef struct
{
    /// @sf_pfdesc 49 element array with fp16 values containing the 7x7 convolution matrix.
	/// @sf_pfdefvalue 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532
	/// @sf_pfarraysize 49
    UInt16* cMat; //fp16 7x7 matrix
}
Conv7x7Param;

/// @brief Shave function of the @ref conv7x7 filter.
void SVU_SYM(svuConv7x7)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv7x7);

/// @}

#endif // __SIPP_CONV7x7_H__
