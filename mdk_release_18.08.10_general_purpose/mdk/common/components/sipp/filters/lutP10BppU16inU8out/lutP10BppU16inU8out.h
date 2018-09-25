///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref lutP10BppU16inU8out SIPP filter API.
///    

#ifndef __SIPP_LUT10BPP_U16INU8OUT_H__
#define __SIPP_LUT10BPP_U16INU8OUT_H__ 

#include <sipp.h>

/// @sf_definition lutP10BppU16inU8out  LUT 16 to 8
/// @sf_description  This filter performs a look-up table transform of a line. Destination line is filled with values from the look-up table. Indices of the entries are taken from the source line.
/// @sf_group ISP
/// @sf_preserve numPlanes, imgSize
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref lutP10BppU16inU8out filter.
typedef struct
{
	/// @sf_desc pointer to the look-up table
    UInt8 *lut;
}
YDnsRefLut10bppParam;

/// @brief Shave function of the @ref lutP10BppU16inU8out filter
void SVU_SYM(svuLutP10BppU16inU8out)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLutP10BppU16inU8out);

/// @}

#endif //__SIPP_LUT10BPP_U16INU8OUT_H__
