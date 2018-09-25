///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref conv7x7Fp16ToU8 SIPP filter API.
///

#ifndef __SIPP_CONV7x7FP16TOU8_H__
#define __SIPP_CONV7x7FP16TOU8_H__ 

#include <sipp.h>

/// @sf_definition conv7x7Fp16ToU8 Convolution 7x7 Fp16ToU8
/// @sf_description This filter performs a convolution on the fp16 input image using the given 7x7 matrix and stores the result in the U8 output image.
/// @sf_group Arithmetic
/// @sf_preserve imgSize
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: half; kernels: 7x7
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution7x7Fp16ToU8/arch/$(MV_SOC_REV_DIR)/shave/src/convolution7x7Fp16ToU8.asm
/// @{

/// @brief Parameter structure of the @ref conv7x7Fp16ToU8 filter.
typedef struct
{
    /// @sf_pfdesc 49 element array with fp16 values containing the 7x7 convolution matrix.
	/// @sf_pfdefvalue 
	/// @sf_pfarraysize 49
    UInt16* cMat; //fp16 7x7 matrix
}
Conv7x7ParamFp16ToU8;

/// @brief Shave function of the @ref conv7x7Fp16ToU8 filter.
void SVU_SYM(svuConv7x7Fp16ToU8)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuConv7x7Fp16ToU8);

/// @}

#endif // __SIPP_CONV7x7FP16TOU8_H__
