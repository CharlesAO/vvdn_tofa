///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convGeneric SIPP filter API.
///

#ifndef __SIPP_CONVGENERIC_H__
#define __SIPP_CONVGENERIC_H__ 

#include <sipp.h>

/// @sf_definition convGeneric Generic Convolution
/// @sf_description This filter performs a generic convolution on the input image using the kernel size given by the user.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels:
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution/arch/$(MV_SOC_REV_DIR)/shave/src/convolution.asm
/// @{

/// @brief Parameter structure of the @ref convGeneric filter.
typedef struct
{
	/// @sf_pfdesc Array with fp16 values containing the convolution matrix.
    UInt16* cMat; //fp16 matrix
	/// @sf_pfdesc u32 Kernel size.
	UInt32 filterSize; //u32 kernel size
}
ConvGenericParam;

/// @brief Shave function of the @ref convGeneric filter.
void SVU_SYM(svuConvGeneric)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvGeneric);

/// @}

#endif // __SIPP_CONVGENERIC_H__
