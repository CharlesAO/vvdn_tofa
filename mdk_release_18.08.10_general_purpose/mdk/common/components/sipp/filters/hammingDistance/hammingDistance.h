///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref censusTransform7x7 SIPP filter API.
///

#ifndef __SIPP_HAMMINGDISTANCE_H__
#define __SIPP_HAMMINGDISTANCE_H__ 

#include <sipp.h>

/// @sf_definition hammingDistance hammingDistance
/// @sf_description  hammingDistance kernel finds matches between two descriptors
/// @sf_group Arithmetic
/// @sf_outdatatype UInt16
/// @sf_preserve numPlanes
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 1x1
/// 	- name: input2; datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/hammingDistance/arch/$(MV_SOC_REV_DIR)/shave/src/hammingDistance.asm
/// @{

/// @brief Parameter structure of the @ref hammingDistance filter.
typedef struct
{
	int descriptor_size;
}
HammingDistanceParam;

/// @brief Shave function of the @ref hammingDistance filter.
void SVU_SYM(svuHammingDistance)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuHammingDistance);

/// @}

#endif // __SIPP_HAMMINGDISTANCE_H__ 
