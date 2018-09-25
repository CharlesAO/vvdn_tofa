///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv1x15 SIPP filter API.
///

#ifndef __SIPP_CONV1x15_H__
#define __SIPP_CONV1x15_H__ 

#include <sipp.h>

/// @sf_definition conv1x15 Convolution 1x15
/// @sf_description This filter performs a convolution on the input image using the given 1x15 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x15
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution1x15/arch/$(MV_SOC_REV_DIR)/shave/src/convolution1x15.asm
/// @{

/// @brief Parameter structure of the @ref conv1x15 filter.
typedef struct
{
    /// @sf_pfdesc 15 element array with fp16 values containing the 1x15 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 15
    UInt16* cMat; //fp16 5x5 matrix
}
Conv1x15Param;

/// @brief Shave function of the @ref conv1x15 filter.
void SVU_SYM(svuConv1x15)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv1x15);

/// @}

#endif // __SIPP_CONV1x15_H__ 
