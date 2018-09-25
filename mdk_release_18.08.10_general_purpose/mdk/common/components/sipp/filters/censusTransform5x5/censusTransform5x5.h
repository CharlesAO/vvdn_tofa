///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref censusTransform5x5 SIPP filter API.
///

#ifndef __SIPP_CENSUSTRANSFORM5x5_H__
#define __SIPP_CENSUSTRANSFORM5x5_H__ 

#include <sipp.h>

/// @sf_definition censusTransform5x5 CensusTransform5x5
/// @sf_description  
/// @sf_group Arithmetic
/// @sf_preserve  imgSize
/// @sf_outdatatype UInt32
/// @sf_inputs
/// 	- datatypes: UInt8; kernels 5x5:
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusTransform5x5/arch/$(MV_SOC_REV_DIR)/shave/src/censusTransform5x5.asm
/// @{

/// @brief Parameter structure of the @ref censusTransform5x5 filter.


/// @brief Shave function of the @ref censusTransform5x5 filter.
void SVU_SYM(svuCensusTransform5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCensusTransform5x5);

/// @}

#endif // __SIPP_CENSUSTRANSFORM5x5_H__ 
