/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref contrast SIPP filter API.
///

#ifndef __SIPP_CONTRAST_H__
#define __SIPP_CONTRAST_H__

#include <sipp.h>

/// @sf_definition contrast Contrast
/// @sf_description This filter applies contrast on pixel element. 
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref contrast filter.
typedef struct
{
  //Params derived by Leon (using float here as Leon doesn't know half)
	/// @sf_desc index that sets the low end value for grey scale clipping 
    float idxLow;
	/// @sf_desc scaling factor
    float scale;
}
ContrastParam;

/// @brief Shave function of the @ref contrast filter
void SVU_SYM(svuContrast)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuContrast);

/// @}

#endif //__SIPP_CONTRAST_H__
