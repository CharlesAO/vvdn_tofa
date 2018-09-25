///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref accumulateSquare SIPP filter API.
///

#ifndef __SIPP_ACCUMULATESQUARE_H__
#define __SIPP_ACCUMULATESQUARE_H__ 

#include <sipp.h>

/// @sf_definition accumulateSquare Accumulate Square
/// @sf_description This filter adds the square of the source image to the accumulator.
/// @sf_group Arithmetic
/// @sf_preserve imgSize, numPlanes
/// @sf_outdatatype float
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// 	- datatypes: UInt8; kernels: 1x1
/// 	- datatypes: float; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/accumulateSquare/arch/$(MV_SOC_REV_DIR)/shave/src/accumulateSquare.asm
/// @{

//Shave symbols that need to be understood by leon need to be declared through "SVU_SYM" MACRO,
//as moviCompile adds a leading _ to symbol exported

/// @brief Shave function of the @ref accumulateSquare filter.
void SVU_SYM(svuAccumulateSquare)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuAccumulateSquare);

/// @}



#endif // __SIPP_ACCUMULATESQUARE_H__ 
