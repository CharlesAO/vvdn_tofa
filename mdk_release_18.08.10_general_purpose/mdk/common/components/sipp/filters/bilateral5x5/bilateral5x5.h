///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref bilateral5x5 SIPP filter API.
///

#ifndef __SIPP_BILATERAL5x5_H__
#define __SIPP_BILATERAL5x5_H__ 

#include <sipp.h>

/// @sf_definition bilateral5x5 bilateral5x5
/// @sf_description This filter performs per-element bit-wise logical conjunction(AND) for two input images.
/// @sf_group Arithmetic
/// @sf_preserve numPlanes
/// @sf_outdatatype UInt16
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 5x5
/// @sf_mandatoryinputs 1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/bilateral5x5/shave/myriad2/bilateral5x5.asm
/// @{

typedef struct
{
    UInt16* sigma;
}
Bilateral5x5Param;

/// @brief Shave function of the @ref bilateral5x5 filter.
void SVU_SYM(svuBilateral5x5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBilateral5x5);

/// @}

#endif // __SIPP_BILATERAL5x5_H__ 
