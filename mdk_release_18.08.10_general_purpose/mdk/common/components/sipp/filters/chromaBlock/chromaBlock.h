///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref chromaBlock SIPP filter API.
///
#ifndef __SIPP_CHROMABLOCK_H__
#define __SIPP_CHROMABLOCK_H__ 

#include <sipp.h>

/// @sf_definition chromaBlock Chroma Block
/// @sf_description This filter applies chroma desaturation and 3x3 color correction matrix.
/// @sf_group ISP
/// @sf_outdatatype UInt8, half
/// @sf_preserve imgSize
/// @sf_inputs
/// 	- name:rgb;  datatypes: UInt8; kernels: 1x1
///		- name:luma; datatypes: UInt8, half; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref chromaBlock filter.
typedef struct 
{
	/// @sf_desc pointer to 3x3 color correction matrix
    float *ccm;      //ptr to 3x3 Color correction Matrix
	/// @sf_desc range look-up table
    UInt8 *rangeLut; //range lut
}ChromaBlkParam;

/// @brief Shave function of the @ref chromaBlock filter.
void SVU_SYM(svuChromaBlock)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuChromaBlock);

/// @}

#endif //__SIPP_CHROMABLOCK_H__
