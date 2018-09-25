///
/// @file scale2xBilinHV_025_075_U16ToU16.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref scale2xBilinHV_025_075_U16ToU16 SIPP filter API.
///

#ifndef __SIPP_SCALE2XBILINHV_025_075_U16TOU16_H__
#define __SIPP_SCALE2XBILINHV_025_075_U16TOU16_H__ 

#include <sipp.h>

/// @sf_definition scale2xBilinHV_025_075_U16ToU16 Upscale by 2 with phases 0.25 and 0.75 u16 to u16
/// @sf_description This filter performs bilinear upscale with 2 factor; Horizontal and Vertical directions; phases 0.25 and 0.75 
/// @sf_group ISP
/// @sf_flags SIPP_RESIZE
/// @sf_outdatatype UInt16
/// @sf_preserve numPlanes, dataType
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 2x2
/// @sf_mandatoryinputs 1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale2xBilinHV025_U16ToU16/arch/$(MV_SOC_REV_DIR)/shave/src/scale2xBilinHV025_U16ToU16.asm,
///            $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale2xBilinHV075_U16ToU16/arch/$(MV_SOC_REV_DIR)/shave/src/scale2xBilinHV075_U16ToU16.asm
/// @{


/// @brief Shave function of the @ref scale2xBilinHV_025_075_U16ToU16 filter.
void SVU_SYM(svuScale2xBilinHV_025_075_U16ToU16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuScale2xBilinHV_025_075_U16ToU16);

/// @}

#endif // __SIPP_SCALE2XBILINHV_025_075_U16TOU16_H__ 
