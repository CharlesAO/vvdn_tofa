///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref convertFrom12BppTo8Bpp SIPP filter API.
///

#ifndef __SIPP_CONVERTFROM12BPPTO8BPP_H__
#define __SIPP_CONVERTFROM12BPPTO8BPP_H__ 

#include <sipp.h>

/// @sf_definition convertFrom12BppTo8Bpp 12Bpp to 8Bpp conversion
/// @sf_description This filter performs a conversion on the input image from 12bpp to 8bpp
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convertFrom12BppTo8Bpp/arch/$(MV_SOC_REV_DIR)/shave/src/convertFrom12BppTo8Bpp.asm
/// @{

/// @brief Shave function of the @ref convertFrom12BppTo8Bpp filter.
void SVU_SYM(svuConvertFrom12BppTo8Bpp)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConvertFrom12BppTo8Bpp);

/// @}

#endif // __SIPP_CONVERTFROM12BPPTO8BPP_H__
