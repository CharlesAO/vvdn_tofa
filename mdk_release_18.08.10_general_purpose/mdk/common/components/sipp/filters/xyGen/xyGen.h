///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref xyGen SIPP filter API.
///    

#ifndef __SIPP_XYGEN_H__
#define __SIPP_XYGEN_H__ 

#include <sipp.h>

/// @sf_definition xyGen  XY Generator
/// @sf_description  This filter performs xyGenerator for Bicubic.
/// @sf_group Arithmetic
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref xyGen filter.
typedef struct
{
	/// @sf_desc pointer to the warp matrix
	UInt32 *coefMat;
	/// @sf_desc first shave number
	UInt32 firstShave;
	/// @sf_desc last shave number
	UInt32 lastShave;
}
XYGenParam;

/// @brief Shave function of the @ref xyGen filter.
void SVU_SYM(svuXYgen)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuXYgen);

/// @}

#endif //__SIPP_XYGEN_H__
