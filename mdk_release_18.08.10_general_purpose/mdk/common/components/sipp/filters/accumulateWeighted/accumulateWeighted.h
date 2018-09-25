///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref accumulateWeighted SIPP filter API.
///

#ifndef __SIPP_ACCUMULATEWEIGHTED_H__
#define __SIPP_ACCUMULATEWEIGHTED_H__ 

#include <sipp.h>


/// @sf_definition accumulateWeighted Accumulate Weighted
/// @sf_description This filter calculates the weighted sum of the input image and the accumulator so that accumulator becomes a running average of frame sequence.
/// @sf_group Arithmetic
/// @sf_outdatatype fp32, float
/// @sf_preserve numPlanes,imgSize
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// 	- datatypes: UInt8; kernels: 1x1
/// 	- datatypes: float; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/accumulateWeighted/arch/$(MV_SOC_REV_DIR)/shave/src/accumulateWeighted.asm
/// @{

/// @brief Parameter structure of the @ref accumulateWeighted filter.
typedef struct
{
	/// @sf_pfdesc Weight of the input image must be a fp32 between 0 and 1

		float alpha;
	
}
AccumulateWeightedParam;

/// @brief Shave function of the @ref accumulateWeighted filter.
void SVU_SYM(svuAccumulateWeighted)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuAccumulateWeighted);

/// @}

#endif // __SIPP_ACCUMULATEWEIGHTED_H__ 
