///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref minTest3x3_fp16 SIPP filter API.
///

#ifndef __SIPP_MINTEST3X3FP16_H__
#define __SIPP_MINTEST3X3FP16_H__ 

#include <sipp.h>

/// @sf_definition minTest3x3_fp16 minTest3x3_fp16
/// @sf_description This kernel will compare the points from inBufferCandidates to the corresponding 3x3 zone of inBuffer.
/// @sf_group Arithmetic
/// @ingroup SIPP_Software_Filters
/// @sf_preserve imgSize
/// @sf_outdatatype UInt32
/// @sf_inputs
/// 	- datatypes: half; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/minTest3x3_fp16/arch/$(MV_SOC_REV_DIR)/shave/src/minTest3x3_fp16.asm
/// @{

/// @brief Parameter structure of the @ref minTest3x3_fp16 filter.
typedef struct
{
    half *inBufferCandidates;
    UInt32 *minLocationsIn;
    UInt32 minCountIn;
	
}MinTest3x3fp16Param;

/// @brief Shave function of the @ref minTest3x3_fp16 filter.
void SVU_SYM(svuMinTest3x3_fp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuMinTest3x3_fp16);

/// @}

#endif // __SIPP_MINTEST3X3FP16_H__ 
