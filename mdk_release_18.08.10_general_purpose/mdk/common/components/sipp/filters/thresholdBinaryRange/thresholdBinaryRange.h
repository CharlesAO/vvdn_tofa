/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref thresholdBinaryRange SIPP filter API.
///

#ifndef __SIPP_THRESHOLDBINARYRANGE_H__
#define __SIPP_THRESHOLDBINARYRANGE_H__

#include <sipp.h>

/// @sf_definition thresholdBinaryRange Threshold Binary Range
/// @sf_description This filter sets output to 0xFF if pixel value is in specified range, otherwise output is 0. 
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/thresholdBinaryRange/arch/$(MV_SOC_REV_DIR)/shave/src/thresholdBinaryRange.asm
/// @{

/// @brief Parameter structure of the @ref thresholdBinaryRange filter.
typedef struct
{
	/// @sf_desc lower value
    UInt8 lowerValue;
	/// @sf_desc upper value
    UInt8 upperValue;
	
}ThresholdBinaryRangeParam;

/// @brief Shave function of the @ref thresholdBinaryRange filter
void SVU_SYM(svuThresholdBinaryRange)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuThresholdBinaryRange);

/// @}

#endif //__SIPP_THRESHOLDBINARYRANGE_H__
