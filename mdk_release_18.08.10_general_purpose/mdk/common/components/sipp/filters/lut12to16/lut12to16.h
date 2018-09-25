///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref lut12to16 SIPP filter API.
///    

#ifndef __SIPP_LUT12TO16_H__
#define __SIPP_LUT12TO16_H__ 

#include <sipp.h>

/// @sf_definition lut12to16  LUT 12 to 16
/// @sf_description  This filter performs a look-up table transform of a line. Destination line is filled with values from the look-up table. Indices of the entries are taken from the source line.
/// @sf_group ISP
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/lookupTable12to16/arch/$(MV_SOC_REV_DIR)/shave/src/lookupTable12to16.asm
/// @{

/// @brief Parameter structure of the @ref lut12to16 filter.
typedef struct
{
    /// @sf_desc  Look-up table of 8193 elements; should have the same depth as the input line.
	UInt16 lutValue[8193];
}
Lut12to16Param;

/// @brief Shave function of the @ref lut12to16 filter.
void SVU_SYM(svuLut12to16)(SippFilter *fptr);
SHAVE_SYM_EXPORT (svuLut12to16);

/// @}

#endif //__SIPP_LUT12TO16_H__
