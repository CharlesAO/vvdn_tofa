///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref fast9M2 SIPP filter API.
///    

#ifndef __SIPP_FAST9M2_H__
#define __SIPP_FAST9M2_H__ 

#include <sipp.h>

/// @sf_definition fast9M2 Fast9M2
/// @sf_description  The filter performs the Fast9 for Myriad 2 corner detection.
/// @sf_group CV
/// @sf_outdatatype float
/// @sf_preserve numPlanes, dataType
/// @sf_inputs
/// 	- datatypes: float; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/fast9M2/arch/$(MV_SOC_REV_DIR)/shave/src/fast9M2.asm

/// @{

/// @brief Parameter structure of the @ref fast9M2 filter.
typedef struct
{
	/// @sf_desc threshold
    UInt8   threshold;
	/// @sf_desc image width per slices used
    UInt16  frameSliceWidth;
}
Fast9M2Param;

/// @brief Shave function of the @ref fast9M2 filter.
void SVU_SYM(svuFast9M2)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuFast9M2);

/// @}

#endif //__SIPP_FAST9M2_H__
