///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv1x5 SIPP filter API.
///

#ifndef __SIPP_CONV1x5_H__
#define __SIPP_CONV1x5_H__ 

#include <sipp.h>

/// @sf_definition conv1x5 Convolution 1x5
/// @sf_description This filter performs a convolution on the input image using the given 1x5 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x5
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution1x5/arch/$(MV_SOC_REV_DIR)/shave/src/convolution1x5.asm
/// @{

/// @brief Parameter structure of the @ref conv1x5 filter.
typedef struct
{
    /// @sf_pfdesc 5 element array with fp16 values containing the 1x5 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 5
    UInt16* cMat; 
}
Conv1x5Param;

/// @brief Shave function of the @ref conv1x5 filter.
void SVU_SYM(svuConv1x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv1x5);

/// @}

#endif // __SIPP_CONV1x5_H__ 
