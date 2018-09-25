///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convSeparable9x9 SIPP filter API.
///

#ifndef __SIPP_CONV_SEPARABLE_9x9__
#define __SIPP_CONV_SEPARABLE_9x9__

#include <sipp.h>
/// @sf_definition convSeparable9x9 Convolution Separable 9x9
/// @sf_description This filter performs a separable convolution on the fp16 input image using the given 9x9 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 9x9
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convSeparable9x9/arch/$(MV_SOC_REV_DIR)/shave/src/convSeparable9x9.asm
/// @{

/// @brief Parameter structure of the @ref convSeparable9x9 filter.
typedef struct
{
	/// @sf_pfdesc Array with 5 fp32 values that represent the separable values for 9x9 matrix.
    float cMat[5]; //fp32 5 values represent the separable values for 9x9 matrix
}
ConvSeparable9x9Param;

/// @brief Shave function of the @ref convSeparable9x9 filter.
void SVU_SYM(svuConvSeparable9x9)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvSeparable9x9);

/// @}

#endif // __SIPP_CONV_SEPARABLE_9x9__

