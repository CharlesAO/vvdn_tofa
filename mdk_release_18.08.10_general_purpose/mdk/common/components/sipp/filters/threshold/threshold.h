/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref threshold SIPP filter API.
///

#ifndef __SIPP_THRESHOLD_H__
#define __SIPP_THRESHOLD_H__

#include <sipp.h>

/// @sf_definition threshold Threshold
/// @sf_description This filter computes the output image based on a threshold value and a threshold type. 
/// @sf_group CV
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/thresholdKernel/arch/$(MV_SOC_REV_DIR)/shave/src/thresholdKernel.asm
/// @{

/// @brief Parameter structure of the @ref threshold filter.
typedef struct
{
    /// @sf_desc threshold value
	UInt8 thresholdValue;
	/// @sf_desc one of the 5 available threshold types:
	///   		  - Thresh_To_Zero: values below threshold are zeroed
	///   		  - Thresh_To_Zero_Inv: opposite of Thresh_To_Zero
	///   		  - Thresh_To_Binary: values below threshold are zeroed and all others are saturated to pixel max value
	///   		  - Thresh_To_Binary_Inv: opposite of Thresh_To_Binary
	///   		  - Thresh_Trunc: values above threshold are given threshold value
	///   		  - default mode: Thresh_Trunc

	UInt32 threshType;
	
}
ThresholdParam;
enum
{
    Thresh_To_Zero       = 0,
    Thresh_To_Zero_Inv   = 1,
    Thresh_To_Binary     = 2,
    Thresh_To_Binary_Inv = 3,
    Thresh_Trunc         = 4
    //THRESH_MASK        = CV_THRESH_MASK,
    //THRESH_OTSU        = CV_THRESH_OTSU
};

/// @brief Shave function of the @ref threshold filter
void SVU_SYM(svuThreshold)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuThreshold);

/// @}

#endif //__SIPP_THRESHOLD_H__
