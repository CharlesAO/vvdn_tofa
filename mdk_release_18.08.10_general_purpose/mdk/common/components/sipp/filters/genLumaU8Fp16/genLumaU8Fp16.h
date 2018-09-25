///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref genLumaU8Fp16 SIPP filter API.
///

#ifndef __SIPP_GEN_LUMA_U8FP16_H__
#define __SIPP_GEN_LUMA_U8FP16_H__ 

#include <sipp.h>

/// @sf_definition genLumaU8Fp16 Generate Luma U8 to Fp16
/// @sf_description This filter generates Luminance from UInt8 RGB input to fp16 output.
/// @sf_group ISP
/// @sf_outdatatype half
/// @sf_preserve imgSize
/// @sf_inputs
///		- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/genLumaU8Fp16/arch/$(MV_SOC_REV_DIR)/shave/src/genLumaU8Fp16.asm
/// @{

/// @brief Shave function of the @ref genLumaU8Fp16 filter.
typedef struct
{
    /// @sf_pfdesc 3 element array with fp16 values containing the 3 coeficients for genluma.
	/// @sf_pfarraysize 3
	UInt16* coefs;
}
GenLumaU8Fp16Param;

void SVU_SYM(svuGenLumaU8Fp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGenLumaU8Fp16);

/// @}

#endif //__SIPP_GEN_LUMA_U8FP16_H__
