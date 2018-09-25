///
/// @file combDecimDemosaic.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief    combine decimation and demosaic to obtain an RGB image, downscaled with 0.5 from an bayer GRBG input image
///

#ifndef __combDecimDemosaic_MVCV_H__
#define __combDecimDemosaic_MVCV_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@}

//!@{
/// combine decimation demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image
/// @param[out] output   - 3 pointers to RGB lines channel
/// @param[in]  iline    - array of pointers to input line of the image 4 lines
/// @param[in]  width    - width of the output lines
/// @return     Nothing

MVCV_FUNC(void, mvispCombDecimDemosaic, unsigned short **output, unsigned short **iline, unsigned int width)

	
#endif //__MIXMEDIAN_MVCV_H__
