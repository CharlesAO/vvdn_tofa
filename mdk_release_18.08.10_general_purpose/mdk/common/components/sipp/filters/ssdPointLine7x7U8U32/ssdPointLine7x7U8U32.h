/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref SsdPointLine7x7U8U32 SIPP filter API.
///

#ifndef __SIPP_SSD7x7POINTLINE_H__
#define __SIPP_SSD7x7POINTLINE_H__ 

#include <sipp.h>

/// @sf_definition SsdPointLine7x7U8U32 Sum of Squared Differences 7x7
/// @sf_description This filter performs sum of squared differences (SSD), the differences are squared and aggregated within a square window (7x7 in this case).
/// @sf_group Arithmetic
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt32
/// @sf_inputs
/// 	- name: input1; datatypes: UInt8; kernels: 7x7
/// 	- name: input2; datatypes: UInt8; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/ssdPointLine7x7U8U32/arch/$(MV_SOC_REV_DIR)/shave/src/ssdPointLine7x7U8U32.asm
/// @{

/// @brief Shave function of the @ref SsdPointLine7x7U8U32 filter.
void SVU_SYM(svuSsdPointLine7x7U8U32)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuSsdPointLine7x7U8U32);

/// @}

#endif // __SIPP_SSD7x7POINTLINE_H__ 
