///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref randNoise SIPP filter API.
///
#ifndef __SIPP_RANDNOISE_H__
#define __SIPP_RANDNOISE_H__ 

#include <sipp.h>

/// @sf_definition randNoise Random Noise
/// @sf_description This filter generates random noise.
/// @sf_group Arithmetic
/// @sf_outdatatype half
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref randNoise filter.
typedef struct
{
	/// @sf_desc noise stregth [0,1]
    float strength;
}
RandNoiseParam;

/// @brief Shave function of the @ref randNoise filter.
void SVU_SYM(svuGenNoise)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGenNoise); 

/// @}

#endif //__SIPP_RANDNOISE_H__
