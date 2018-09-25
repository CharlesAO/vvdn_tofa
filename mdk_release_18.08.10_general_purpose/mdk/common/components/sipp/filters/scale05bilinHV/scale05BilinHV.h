///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale05bilinHV SIPP filter API.
///

#ifndef __SIPP_SCALE05BILINHV_H__
#define __SIPP_SCALE05BILINHV_H__ 

#include <sipp.h>

/// @sf_definition scale05BilinHV Downscale by 2
/// @sf_description This filter performs bilinear downscale with 0.5 factor – Horizontal and Vertical directions.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes, dataType
/// @sf_outdatatype UInt8, UInt16
/// @sf_flags SIPP_RESIZE
/// @sf_inputs
/// 	- datatypes: UInt8, UInt16; kernels: 2x2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale05BilinHV_U8ToU8/arch/$(MV_SOC_REV_DIR)/shave/src/scale05BilinHV_U8ToU8.asm,
/// $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale05BilinHV_U16ToU16/arch/$(MV_SOC_REV_DIR)/shave/src/scale05BilinHV_U16ToU16.asm
/// @{

/// @brief Shave function of the @ref scale05BilinHV filter.
void SVU_SYM(svuScl05BilinHV)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScl05BilinHV); 

/// @}

#endif // __SIPP_SCALE05BILINHV_H__ 
