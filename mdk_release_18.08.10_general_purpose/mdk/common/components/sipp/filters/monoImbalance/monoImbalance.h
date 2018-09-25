///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref MonoImbalance SIPP filter API.
///

#ifndef __SIPP_MONO_IMB_H__
#define __SIPP_MONO_IMB_H__ 

#include <sipp.h>

/// @sf_definition MonoImbalance MonoImbalance
/// @sf_group Arithmetic
/// @sf_outdatatype half 
/// @sf_preserve numPlanes
/// @sf_inputs
/// 	- datatypes: UInt16; kernels: 3x3
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/monoImbalance/arch/$(MV_SOC_REV_DIR)/shave/src/monoImbalance.asm 
/// @

/// @brief Parameter structure of the @ref monoImbalance filter.
typedef struct
{
int thrDark;
int thrBright;
int inputBits;

}
MonoImbalanceParam;

/// @brief Shave function of the @ref MonoImbalance filter.
void SVU_SYM(svuMonoImbalance)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuMonoImbalance);

/// @}

#endif // __SIPP_MONO_IMB_H__ 
