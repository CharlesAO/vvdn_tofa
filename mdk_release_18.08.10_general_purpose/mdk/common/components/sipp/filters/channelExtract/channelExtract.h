///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref channelExtract SIPP filter API.
///

#ifndef __SIPP_CHANNELEXTRACT_H__
#define __SIPP_CHANNELEXTRACT_H__

#include <sipp.h>

/// @sf_definition channelExtract channelExtract
/// @sf_description  This kernel extracts one of the R, G, B, plane from an interleaved RGB line
/// @sf_group Arithmetic
/// @sf_preserve dataType
/// @sf_outdatatype UInt8
/// @sf_inputs
///     - datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/channelExtract/arch/$(MV_SOC_REV_DIR)/shave/src/channelExtract.asm
/// @{

/// @brief Parameter structure of the @ref channelExtract filter.
typedef struct
{
    /// @sf_pfdesc number 0 to extract plane R, 1 for extracting G, 2 for extracting B
    UInt32 plane;
}
ChannelExtractParam;

/// @brief Shave function of the @ref channelExtract filter.
void SVU_SYM(svuChannelExtract)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuChannelExtract);

/// @}

#endif // __SIPP_CHANNELEXTRACT_H__
