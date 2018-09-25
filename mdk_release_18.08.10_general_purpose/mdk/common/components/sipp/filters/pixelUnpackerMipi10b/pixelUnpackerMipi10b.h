///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref pixelUnpackerMipi10b SIPP filter API.
///    

#ifndef __SIPP_PIXEL_UNPACKER_MIPI10B_H__
#define __SIPP_PIXEL_UNPACKER_MIPI10B_H__ 

#include <sipp.h>

/// @sf_definition pixelUnpackerMipi10b Pixel Unpacker Mipi 10b
/// @sf_description  The filter unpacks 2 px/cc with 10bpp to 16b with Mipi.
/// @sf_group ISP
/// @sf_outdatatype UInt16
/// @sf_preserve numPLanes
/// @sf_inputs
/// 	- datatypes: UInt32; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/pixelUnpackerMipi10b/arch/$(MV_SOC_REV_DIR)/shave/src/pixelUnpackerMipi10b.asm
/// @{

/// @brief Parameter structure of the @ref pixelUnpackerMipi10b filter.
typedef struct
{
	///@sf_desc Line number
    UInt32 lineNo;
	///@sf_desc coefficients for black correction
    UInt32 coefs[4];
    ///@sf_desc If image is from MA2100, need software workaround to unpack 10-bit data into 16-bit words. If not set, do black-level subtraction only.
    UInt32 noMipiRxWorkaround;
}
PixelUnpackerMipi10bParam;

/// @brief Shave function of the @ref pixelUnpackerMipi10b filter.
void SVU_SYM(svuPixelUnpackerMipi10b)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuPixelUnpackerMipi10b);

/// @}

#endif //__SIPP_PIXEL_UNPACKER_MIPI10B_H__
