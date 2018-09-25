///
/// @file combDecimDemosaic.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief    combined decimation and demosaic - Obtain a RGB image, downscaled with 0.5 from 
///           a bayer GRBG input image and apply white-balance gains
///

#ifndef __combDecimDemosaicAwbGains_MVCV_H__
#define __combDecimDemosaicAwbGains_MVCV_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@}

//!@{
/// combined decimation and demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image, and apply WB gains
/// @param[out] output   - 3 pointers to RGB lines channel
/// @param[in]  iline    - array of pointers to input line of the image 4 lines
/// @param[in] gains     - list 0f 3 white balance gains, in fixed point 8.8
/// @param[in] width     - width of the output lines
/// @return    Nothing

MVCV_FUNC(void, mvispCombDecimDemosaicAwbGains, unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)

	
#endif //__combDecimDemosaicAwbGains_MVCV_H__
