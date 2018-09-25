///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref minMaxPos SIPP filter API.
///
#ifndef __SIPP_MINMAXPOS_H__
#define __SIPP_MINMAXPOS_H__ 

#include <sipp.h>

/// @sf_definition minMaxPos Min/Max Value Position
/// @sf_description This filter computes the minimum and the maximum value of a given input line and their position.
/// @sf_group CV
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/minMaxPos/arch/$(MV_SOC_REV_DIR)/shave/src/minMaxPos.asm
/// @{

/// @brief Parameter structure of the @ref minMaxPos filter.
typedef struct
{
	/// @sf_desc mask filled with 1s and 0s which determines the image area to compute minimum and maximum
    UInt8* Mask;
}
MinMaxPosParam;

/// @brief Shave function of the @ref minMaxPos filter.
void SVU_SYM(svuMinMaxPos)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuMinMaxPos);

/// @}

#endif // __SIPP_MINMAXPOS_H__ 
