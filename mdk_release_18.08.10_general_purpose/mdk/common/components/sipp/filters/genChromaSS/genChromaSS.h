//////
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref genChromaSS SIPP filter API.
///    

#ifndef __SIPP_GEN_CHROMASS_H__
#define __SIPP_GEN_CHROMASS_H__ 

#include <sipp.h>

/// @sf_definition genChromaSS Generate Chroma with subsampling
/// @sf_description This filter generates Chroma from an FP16 Luma plane(generated from RGB) and a U8 RGB plane. It also subsamples the data by half in each dimension.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_preserve 
/// @sf_flags SIPP_RESIZE
/// @sf_inputs
/// 	- name: RGB;	datatypes: UInt8; 	kernels: 2x2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/genChromaSS/arch/$(MV_SOC_REV_DIR)/shave/src/genChromaSS.asm
/// @{

/// @brief Parameter structure of the @ref genChromaSS filter.
typedef struct
{
	/// @sf_desc epsilon value. Range is [0, 255], but it's normally a very small non-zero
    int   epsilon;
	/// @sf_desc scaling factor. 3 element array: 0 for R channel, 1 for G channel, 2 for B channel
    float scale[3];
} ChrGenSSParam;

/// @brief Shave function of the @ref genChromaSS filter.
void SVU_SYM(svuGenChromaSS)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGenChromaSS);

/// @}
#endif //__SIPP_GEN_CHROMASS_H__
