///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref cannyEdgeDetection SIPP filter API.
///    

#ifndef __SIPP_CANNY_H__
#define __SIPP_CANNY_H__ 

#include <sipp.h>

/// @sf_definition cannyEdgeDetection Canny Edge Detection
/// @sf_description  The filter finds edges in the input image and marks them in the output map edges using the Canny algorithm.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 9x9
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/canny/arch/$(MV_SOC_REV_DIR)/shave/src/canny.asm

/// @{

/// @brief Parameter structure of the @ref cannyEdgeDetection filter.
typedef struct
{
	/// @sf_desc lower threshold - value between 0-255
	UInt32 threshold1;
	/// @sf_desc upper threshold - value between 0-255
	UInt32 threshold2;


}
cannyEdgeDetectionParam;


/// @brief Shave function of the @ref cannyEdgeDetection filter.
void SVU_SYM(svuCannyEdgeDetection)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCannyEdgeDetection);

/// @}

#endif //__SIPP_CANNY_H__
