///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref whiteBalanceBayerGBRG SIPP filter API.
///    

#ifndef __SIPP_WHITEBALANCE_BAYER_GBRG_H__
#define __SIPP_WHITEBALANCE_BAYER_GBRG_H__ 

#include <sipp.h>

/// @sf_definition whiteBalanceBayerGBRG  White Balance Bayer GBRG
/// @sf_description  This filter applies white balance gains for BayerGBRG input.
/// @sf_group ISP
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/whiteBalanceBayerGBRG/arch/$(MV_SOC_REV_DIR)/shave/src/whiteBalanceBayerGBRG.asm
/// @{

/// @brief Parameter structure of the @ref whiteBalanceBayerGBRG filter.
typedef struct
{
	/// @sf_desc Auto White Balance gains coefficients
    UInt16* awbCoef; 
	/// @sf_desc Clamp value
	UInt16 clamp[1];
	
}
WhiteBalanceBayerGBRGParam;

/// @brief Shave function of the @ref whiteBalanceBayerGBRG filter.
void SVU_SYM(svuWhiteBalanceBayerGBRG)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuWhiteBalanceBayerGBRG);

/// @}

#endif //__SIPP_WHITEBALANCE_BAYER_GBRG_H__
