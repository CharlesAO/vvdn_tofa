#include <sipp.h>
///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref mixMedian SIPP filter API.
///    

#ifndef __SIPP_MIXMEDIAN_H__
#define __SIPP_MIXMEDIAN_H__ 

#include <sipp.h>

/// @sf_definition mixMedian Mix Median
/// @sf_description  This filter performs mix two 8-bit planes according to a third 8-bit reference plane.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: in1; datatypes: UInt8; kernels: 1x1
/// 	- name: in2; datatypes: UInt8; kernels: 1x1
/// 	- name: ref; datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/mixMedian/arch/$(MV_SOC_REV_DIR)/shave/src/mixMedian.asm
/// @{

/// @brief Parameter structure of the @ref mixMedian filter.
typedef struct
{
	/// @sf_pfdesc offset
	float	offset;
	/// @sf_pfdesc slope
	float	slope;
}MixMedianParam;

/// @brief Shave function of the @ref mixMedian filter.
void SVU_SYM(svuMixMedian)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuMixMedian); 

/// @}

#endif //__SIPP_MIXMEDIAN_H__
