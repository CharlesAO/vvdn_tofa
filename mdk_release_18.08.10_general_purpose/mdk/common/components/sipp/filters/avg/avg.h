///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref avg SIPP filter API.
///

#ifndef __SIPP_AVG_H__
#define __SIPP_AVG_H__ 

#include <sipp.h>

/// @sf_definition avg Average
/// @sf_description This filter outputs the average of two input images.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 1x1
/// 	- name: input2; datatypes: UInt8; kernels: 1x1
/// @sf_mandatoryinputs 2
/// @{

/// @brief Shave function of the @ref avg filter.
void SVU_SYM(svuAvg)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuAvg);

/// @}

#endif // __SIPP_AVG_H__ 
