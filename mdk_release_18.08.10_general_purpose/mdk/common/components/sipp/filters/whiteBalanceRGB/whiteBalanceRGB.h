///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref whiteBalanceRGB SIPP filter API.
///

#ifndef __SIPP_WHITEBALANCE_RGB_H__
#define __SIPP_WHITEBALANCE_RGB_H__

#include <sipp.h>

/// @sf_definition whiteBalanceRGB  White Balance RGB
/// @sf_description  This filter applies white balance gains for RGB input.
/// @sf_group ISP
/// @sf_outdatatype UInt16
/// @sf_inputs
///     - datatypes: UInt16; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/whiteBalanceRGB/arch/$(MV_SOC_REV_DIR)/shave/src/whiteBalanceRGB.asm
/// @{

/// @brief Parameter structure of the @ref whiteBalanceRGB filter.
typedef struct
{
    /// @sf_desc Auto White Balance gains coefficients
    UInt16* awbCoef;
    /// @sf_desc Clamp value
    UInt16 clamp[1];

}
WhiteBalanceRGBParam;

/// @brief Shave function of the @ref whiteBalanceRGB filter.
void SVU_SYM(svuWhiteBalanceRGB)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuWhiteBalanceRGB);

/// @}

#endif //__SIPP_WHITEBALANCE_RGB_H__
