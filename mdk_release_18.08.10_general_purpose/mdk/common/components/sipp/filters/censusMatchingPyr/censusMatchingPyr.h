///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref censusMatchingPyr SIPP filter API.
///

#ifndef __SIPP_CENSUSMATCHINGPYR_H__
#define __SIPP_CENSUSMATCHINGPYR_H__ 

#include <sipp.h>

/// @sf_definition censusMatchingPyr censusMatchingPyr
/// @sf_description  mvcvCensusMatchingPyr - performs an XOR operation between pixel one pixel in *in1 and 16 pixels from *in2 and counts up how many values of 1 are in the result
/// @sf_group Arithmetic
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt32; kernels: 1x1
/// 	- datatypes: UInt32; kernels: 1x15
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMatchingPyr/arch/$(MV_SOC_REV_DIR)/shave/src/censusMatchingPyr.asm
/// @{

/// @brief Parameter structure of the @ref censusMatchingPyr filter.
typedef struct 
{
	//pointer to predicted disparities 
	UInt8* predicted;
}
CensusMatchingPyrParam;

/// @brief Shave function of the @ref censusMatchingPyr filter.
void SVU_SYM(svuCensusMatchingPyr)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCensusMatchingPyr);

/// @}

#endif // __SIPP_CENSUSMATCHINGPYR_H__ 
