/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref thresholdFilter SIPP filter API.
///

#ifndef __SIPP_THRESHOLD_FILTER_H__
#define __SIPP_THRESHOLD_FILTER_H__

#include <sipp.h>

/// @sf_definition thresholdFilter ThresholdFilter
/// @sf_description This filter computes the output image based on a thresholdFilter value 
/// @sf_group CV
/// @sf_outdatatype UInt16
/// @sf_preserve numPlanes
/// @sf_inputs
/// 	- datatypes: float; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/thresholdFilter/arch/$(MV_SOC_REV_DIR)/shave/src/thresholdFilter.asm
/// @{

/// @brief Parameter structure of the @ref thresholdFilter filter.
typedef struct
{
	float threshold;
	UInt32 posOffset;
}
ThresholdFilterParam;

/// @brief Shave function of the @ref thresholdFilter filter
void SVU_SYM(svuThresholdFilter)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuThresholdFilter);

/// @}

#endif //__SIPP_THRESHOLD_FILTER_H__
