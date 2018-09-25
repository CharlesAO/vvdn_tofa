///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref pixelUnpackerWB SIPP filter API.
///    

#ifndef __SIPP_PIXEL_UNPACKER_WB_H__
#define __SIPP_PIXEL_UNPACKER_WB_H__ 

#include <sipp.h>

/// @sf_definition pixelUnpackerWB Pixel Unpacker WB
/// @sf_description  The filter unpacks 10b -> 16b/15b/14b/13b/12b/11b/10b depending on the shift value; also applies White Balance. Input of this filter need to be the output of the pixelPacker10b filter.
/// @sf_group ISP
/// @sf_outdatatype UInt16
/// @sf_preserve numPLanes
/// @sf_inputs
/// 	- name: ms8b; datatypes: UInt32; kernels: 1x1
/// 	- name: ls2b; datatypes: UInt8;  kernels: 1x1
/// @sf_mandatoryinputs 2
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/pixelUnpackerWB/arch/$(MV_SOC_REV_DIR)/shave/src/pixelUnpackerWB.asm
/// @{

/// @brief Parameter structure of the @ref pixelUnpackerWB filter.
typedef struct
{
	/// sf_desc number of bits for shifting the result to left
    UInt8 shift;
	/// sf_desc coefficients needed for white balance
	/// @sf_pfarraysize 4
    UInt16 *awbCoef;
}
PixelUnpackerWBParam;

/// @brief Shave function of the @ref pixelUnpackerWB filter.
void SVU_SYM(svuPixelUnpackerWB)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuPixelUnpackerWB);

/// @}

#endif //__SIPP_PIXEL_UNPACKER_WB_H__
