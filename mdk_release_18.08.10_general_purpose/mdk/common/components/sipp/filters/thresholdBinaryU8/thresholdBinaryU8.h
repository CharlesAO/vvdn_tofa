/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref thresholdBinaryU8 SIPP filter API.
///

#ifndef __SIPP_THRESHOLDBINARYU8_H__
#define __SIPP_THRESHOLDBINARYU8_H__

#include <sipp.h>

/// @sf_definition thresholdBinaryU8 Threshold Binary U8
/// @sf_description This filter sets output to 0 if threshold value is less then input value and to 0xFF if threshold value is greater then input value. 
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/thresholdBinaryU8/arch/$(MV_SOC_REV_DIR)/shave/src/thresholdBinaryU8.asm
/// @{

/// @brief Parameter structure of the @ref thresholdBinaryU8 filter.
typedef struct
{
	/// @sf_desc threshold value
    UInt8 threshold;
	
}ThresholdBinaryU8Param;

/// @brief Shave function of the @ref thresholdBinaryU8 filter
void SVU_SYM(svuThresholdBinaryU8)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuThresholdBinaryU8);

/// @}

#endif //__SIPP_THRESHOLDBINARYU8_H__
