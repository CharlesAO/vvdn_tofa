/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref purpleFlare SIPP filter API.
///

#ifndef __SIPP_PURPLE_FLARE_H__
#define __SIPP_PURPLE_FLARE_H__

#include <sipp.h>

/// @sf_definition purpleFlare purpleFlare
/// @sf_description This filter applies a modified unsharp mask on the blue channel only to reduce the "purple flare" effect.
/// @sf_group ISP
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/purpleFlare/arch/$(MV_SOC_REV_DIR)/shave/src/purpleFlare.asm
/// @{

/// @brief Parameter structure of the @ref purpleFlare filter.
typedef struct
{
    UInt8 strength;
} PurpleFlareParam;

/// @brief Shave function of the @ref purpleFlare filter
void SVU_SYM(svuPurpleFlare)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuPurpleFlare); 

/// @}

#endif //__SIPP_PURPLE_FLARE_H__
