///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convSeparable11x11 SIPP filter API.
///

#ifndef __SIPP_CONV_SEPARABLE_11x11__
#define __SIPP_CONV_SEPARABLE_11x11__
#include <sipp.h>

/// @sf_definition convSeparable11x11 Convolution Separable 11x11
/// @sf_description This filter performs a separable convolution on the input image using the given 11x11 matrix.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 11x11
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convSeparable11x11/arch/$(MV_SOC_REV_DIR)/shave/src/convSeparable11x11.asm
/// @{

/// @brief Parameter structure of the @ref convSeparable11x11 filter.
typedef struct
{
	/// @sf_pfdesc Array with 6 fp32 values that represent the separable values for 11x11 matrix.
	/// @sf_pfarraysize 6
    UInt32* cMat; //fp32 6 values represent the separable values for 11x11 matrix
}
ConvSeparable11x11Param;

/// @brief Shave function of the @ref convSeparable11x11 filter.
void SVU_SYM(svuConvSeparable11x11)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvSeparable11x11);

/// @}

#endif // __SIPP_CONV_SEPARABLE_11x11__
