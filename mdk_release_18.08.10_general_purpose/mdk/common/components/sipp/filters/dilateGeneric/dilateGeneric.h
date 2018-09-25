///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref dilateGeneric SIPP filter API.
///

#ifndef __SIPP_DILATE_GENERIC_H__
#define __SIPP_DILATE_GENERIC_H__ 

#include <sipp.h>

/// @sf_definition dilateGeneric Generic Dilate
/// @sf_description This filter performs a generic dilate on the input image using the kernel size given by the user.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 3x3, 5x5, 7x7, 9x9, 11x11, 13x13, 15x15
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dilate/arch/$(MV_SOC_REV_DIR)/shave/src/dilate.asm
/// @{

/// @brief Parameter structure of the @ref dilateGeneric filter.
typedef struct
{
	/// @sf_pfdesc Array with fp16 values containing the dilate matrix.
    UInt32 *dMat;
	/// @sf_pfdesc u32 Kernel size.
	UInt32 kernelSize;
}
DilateGenericParam;

/// @brief Shave function of the @ref dilateGeneric filter.
void SVU_SYM(svuDilateGeneric)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuDilateGeneric);

/// @}

#endif // __SIPP_DILATE_GENERIC_H__ 
