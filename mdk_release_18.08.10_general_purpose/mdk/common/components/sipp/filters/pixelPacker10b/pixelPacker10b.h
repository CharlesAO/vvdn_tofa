///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref pixelPacker10b SIPP filter API.
///    

#ifndef __SIPP_PIXEL_PACKER_H__
#define __SIPP_PIXEL_PACKER_H__ 

#include <sipp.h>

/// @sf_definition pixelPacker10b Pixel packer
/// @sf_description  The filter takes 16bits(10bpp) as input and, in order to compress the data, split a pixel in two components.
/// @sf_group ISP
/// @sf_outdatatype UInt8, UInt32
/// @sf_preserve numPLanes
/// @sf_inputs
/// 	-  datatypes: UInt16; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/pixelPacker10b/arch/$(MV_SOC_REV_DIR)/shave/src/pixelPacker10b.asm
/// @{

/// @brief Shave function of the @ref pixelPacker10b filter.
void SVU_SYM(svuPixelPacker10b)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuPixelPacker10b);

/// @}

#endif //__SIPP_PIXEL_PACKER_H__
