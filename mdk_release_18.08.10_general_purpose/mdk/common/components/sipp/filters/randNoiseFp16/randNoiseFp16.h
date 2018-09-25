///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref randNoiseFp16 SIPP filter API.
///

#ifndef __RANDNOISEFP16_H__
#define __RANDNOISEFP16_H__

#include <sipp.h>

/// @sf_definition randNoiseFp16 Random Noise (high speed)
/// @sf_description This filter generates random noise using high speed algorithm.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/randNoiseFp16/arch/$(MV_SOC_REV_DIR)/shave/src/randNoiseFp16.asm
/// @{

/// @brief Parameter structure of the @ref randNoiseFp16 filter.
typedef struct
{
	/// @sf_desc noise stregth [0,1]
    float strength;
}
RandNoiseFp16Param;

/// @brief Shave function of the @ref randNoiseFp16 filter.
void SVU_SYM(svuGenNoiseFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGenNoiseFp16); 

/// @}

#endif //__RANDNOISEFP16_H__

