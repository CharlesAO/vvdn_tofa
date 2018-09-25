#include <sipp.h>
///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref histogram SIPP filter API.
///    

#ifndef __SIPP_HISTOGRAM_H__
#define __SIPP_HISTOGRAM_H__ 

#include <sipp.h>

/// @sf_definition histogram Histogram
/// @sf_description  This filter computes a histogram on a given line to be applied to all lines of an image.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/histogram/arch/$(MV_SOC_REV_DIR)/shave/src/histogram.asm

/// @{

/// @brief Parameter structure of the @ref histogram filter.
typedef struct
{
	/// @sf_desc array of values from histogram
    UInt32* hist; 
}
HistogramParam;

/// @brief Shave function of the @ref histogram filter.
void SVU_SYM(svuHistogram)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuHistogram);

/// @}

#endif //__SIPP_HISTOGRAM_H__
