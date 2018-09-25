///
/// @file    randNoiseFp16.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  generate a random noise on a fp16, [0,1] interval image, in interval +-0.5*strength parameter
///

#ifndef __randNoisefp16_H__
#define __randNoisefp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// Random noise apply on a fp16 frame (custom size)
/// @param[out] out             - Output line
/// @param[in] in               - Input lines
/// @param[in] noiseStrength    - Noise strength [0,1]
/// @param[in] width            - Width of input line

MVCV_FUNC(void, mvispRandNoiseFp16, half *output, half *input, float noiseStrength, unsigned int width)
//!@}

#endif //__randNoisefp16_H__
