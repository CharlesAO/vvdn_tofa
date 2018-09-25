///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convSeparable3x3 SIPP filter API.
///

#ifndef __SIPP_CONV_SEPARABLE_3X3__
#define __SIPP_CONV_SEPARABLE_3X3__

#include <sipp.h>
/// @sf_definition convSeparable3x3 Convolution Separable 3x3
/// @sf_description This filter performs a separable convolution on the fp16 input image using the given 3x3 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convSeparable3x3/arch/$(MV_SOC_REV_DIR)/shave/src/convSeparable3x3.asm
/// @{

/// @brief Parameter structure of the @ref convSeparable3x3 filter.
typedef struct
{
	/// @sf_pfdesc Array with 2 fp32 values that represent the separable values for 3x3 matrix.
    float cMat[2]; //fp32 2 values represent the separable values for 3x3 matrix
}
ConvSeparable3x3Param;

/// @brief Shave function of the @ref convSeparable3x3 filter.
void SVU_SYM(svuConvSeparable3x3)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvSeparable3x3);

/// @}

#endif // __SIPP_CONV_SEPARABLE_3X3__

