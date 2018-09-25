///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref minMaxValue SIPP filter API.
///
#ifndef __SIPP_MINMAXVALUE_H__
#define __SIPP_MINMAXVALUE_H__ 

#include <sipp.h>

/// @sf_definition minMaxValue Min/Max Value
/// @sf_description This filter computes the minimum and the maximum value of a given input image.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/minMaxKernel/arch/$(MV_SOC_REV_DIR)/shave/src/minMaxKernel.asm
/// @{

/// @brief Parameter structure of the @ref minMaxValue filter.
typedef struct
{
	/// @sf_desc stores the minimum value on the line
    UInt8 minVal;
	/// @sf_desc stores the maximum value on the line
    UInt8 maxVal;
	/// @sf_desc mask filled with 1s and 0s which determines the image area to compute minimum and maximum
    UInt8 *maskAddr;
}
minMaxValParam;


/// @brief Shave function of the @ref minMaxValue filter.
void SVU_SYM(svuMinMaxValue)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuMinMaxValue);

/// @}

#endif // __SIPP_MINMAXVALUE_H__ 
