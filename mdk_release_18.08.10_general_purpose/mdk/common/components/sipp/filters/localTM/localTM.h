///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref localTM SIPP filter API.
///

#ifndef __SIPP_LOCALTM_H__
#define __SIPP_LOCALTM_H__

#include <sipp.h>

/// @sf_definition localTM localTM
/// @sf_description This filter applies a tone mapping function to the Luma channel.
/// @sf_group ISP
/// @sf_outdatatype UInt16
/// @sf_flags SIPP_RESIZE
/// @sf_inputs
///     - name: input1; datatypes: UInt16; kernels: 1x1
/// 	- name: input2; datatypes: UInt8; kernels: 2x2
/// @sf_mandatoryinputs 2	
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/localTM/arch/$(MV_SOC_REV_DIR)/shave/src/localTM.asm
/// @{

/// @brief Parameter structure of the @ref localTM filter.

typedef struct
{
	UInt16	*curves;
} LocalTMParam;

/// @brief Shave function of the @ref localTM filter.
void SVU_SYM(svuLocalTM)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuLocalTM);

/// @}

#if defined(__sparc) && defined(LTM_USE_LOCAL_DATA)
//Declare slice-local tables (1D.... addr)
extern UInt16 * SVU_SYM (slcLocalcurves);
extern UInt16 * SVU_SYM0(slcLocalcurves);
extern UInt16 * SVU_SYM1(slcLocalcurves);
extern UInt16 * SVU_SYM2(slcLocalcurves);
extern UInt16 * SVU_SYM3(slcLocalcurves);
extern UInt16 * SVU_SYM4(slcLocalcurves);
extern UInt16 * SVU_SYM5(slcLocalcurves);
extern UInt16 * SVU_SYM6(slcLocalcurves);
extern UInt16 * SVU_SYM7(slcLocalcurves);
#endif 

#endif //__SIPP_LOCALTM_H__
