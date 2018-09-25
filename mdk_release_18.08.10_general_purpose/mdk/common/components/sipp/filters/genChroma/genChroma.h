//////
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref genChroma SIPP filter API.
///    

#ifndef __SIPP_GEN_CHROMA_H__
#define __SIPP_GEN_CHROMA_H__ 

#include <sipp.h>

/// @sf_definition genChroma Generate Chroma
/// @sf_description This filter generates Chroma planes from RGB and Luma planes.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_preserve imgSize, dataType
/// @sf_mandatoryinputs 2
/// @sf_inputs
/// 	- name: RGB;	datatypes: UInt8; kernels: 1x1
/// 	- name: Luma;	datatypes: UInt8; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref  filter.
typedef struct
{
	/// @sf_desc epsilon value. Range is [0, 255], but it's normally a very small non-zero
    int epsilon;
}ChrGenParam;

/// @brief Shave function of the @ref genChroma filter.
void SVU_SYM(svuGenChroma)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGenChroma);

/// @}

#endif //__SIPP_GEN_CHROMA_H__
