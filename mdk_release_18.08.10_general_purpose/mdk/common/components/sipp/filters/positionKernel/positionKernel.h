///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref positionKernel SIPP filter API.
///
#ifndef __SIPP_POSITIONKERNEL_H__
#define __SIPP_POSITIONKERNEL_H__ 

#include <sipp.h>

/// @sf_definition positionKernel Pixel Position
/// @sf_description This filter returns the position of a given pixel value.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/pixelPos/arch/$(MV_SOC_REV_DIR)/shave/src/pixelPos.asm
/// @{

/// @brief Parameter structure of the @ref positionKernel filter.
typedef struct
{
	/// @sf_desc mask filled with 1s and 0s which determines the image area to find position
	UInt8 *maskAddr;
	/// @sf_desc stores the pixel value to be searched
	UInt8 pixelValue;
	/// @sf_desc stores the position occupied by the searched value within line
	UInt32 pixelPosition;
	/// @sf_desc stores 0x11 if pixel value found, else 0x00
	UInt8 status;
	
}
positionKernelParam;


/// @brief Shave function of the @ref positionKernel filter.
void SVU_SYM(svuPositionKernel)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuPositionKernel);

/// @}

#endif // __SIPP_POSITIONKERNEL_H__ 
