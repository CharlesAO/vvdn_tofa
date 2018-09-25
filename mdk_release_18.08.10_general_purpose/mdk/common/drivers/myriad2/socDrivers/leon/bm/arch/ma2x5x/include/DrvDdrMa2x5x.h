///
/// @file DrvDdrMa2x5x.h
/// 
/// 
/// @defgroup DrvDdrMa2x5x DdrMa2x5x Driver
/// @{
/// @brief DdrMa2x5x Driver API. 
///


///
/// 
/// 
/// 
///
/// This module contains the helper functions necessary to control the
/// DDR in the MA2x5x Myriad
///

#ifndef _DRV_DDR_MA2x5x_H_
#define _DRV_DDR_MA2x5x_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "DrvCommon.h"
#include "DrvRegUtils.h"
#include "registersMyriad.h"

#include "DrvDdrAddrMapMa2x5x.h"

// 2: Defines
// ----------------------------------------------------------------------------
// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Exported Inlined Functions
// ----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

/// Read an LPDDR2 or LPDDR3 mode register. Please see the die datasheet, and/or
/// the JEDEC JESD209-2/JESD209-3 specs for a list of commonly supported registers
/// and their fields.
///
/// @param[in] reg - register index, maximum register index that can be accessed is 255.
/// @return data - the 8 bits of data read back from the MR register
u32 DrvDdrMrRead(u32 reg);

/// Determine the memory chip density, using an MR register read.
u32 DrvDdrGetMr8DensityGigaBit( void );

#ifdef __cplusplus
}
#endif

#endif // _DRV_DDR_MA2x5x_H_

///@}
