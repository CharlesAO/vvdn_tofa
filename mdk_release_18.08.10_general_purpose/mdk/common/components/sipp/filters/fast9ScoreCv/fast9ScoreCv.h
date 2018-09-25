///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref fast9ScoreCv SIPP filter API.
///    

#ifndef __SIPP_FAST9SCORECV_H__
#define __SIPP_FAST9SCORECV_H__ 

#include <sipp.h>

/// @sf_definition fast9ScoreCv Fast9ScoreCv
/// @sf_description  The filter performs the Fast9 for Myriad 2 corner detection.
/// @sf_group CV
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/fast9ScoreCv/arch/$(MV_SOC_REV_DIR)/shave/src/fast9ScoreCv.asm

/// @{

/// @brief Parameter structure of the @ref fast9ScoreCv filter.
typedef struct
{
    UInt8   threshold;
    UInt16  frameSliceWidth;
}
Fast9ScoreCvParam;

/// @brief Shave function of the @ref fast9ScoreCv filter.
void SVU_SYM(svuFast9ScoreCv)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuFast9ScoreCv);

/// @}

#endif //__SIPP_FAST9SCORECV_H__
