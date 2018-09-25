///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref absdiff SIPP filter API.
///

#ifndef __SIPP_ABSDIFF_H__
#define __SIPP_ABSDIFF_H__

#include <sipp.h>

/// @sf_definition absdiff Absolute difference
/// @sf_description This filter computes the absolute difference of two images.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 1x1
/// 	- name: input2; datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/absoluteDiff/arch/$(MV_SOC_REV_DIR)/shave/src/absoluteDiff.asm
/// @{

/// @brief Shave function of the @ref absdiff filter.
void SVU_SYM(svuAbsdiff)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuAbsdiff);

/// @}

#endif // __SIPP_ABSDIFF_H__
