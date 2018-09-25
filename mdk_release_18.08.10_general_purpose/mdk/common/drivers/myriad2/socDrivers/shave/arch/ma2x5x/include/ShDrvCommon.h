///
/// @file ShDrvCommon.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup ShDrvCommon ShCommon Driver
/// @{
/// @brief     MA2x5x common utilities
///
/// Common utilities to be used in SHAVE bare-metal drivers
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVCOMMON_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVCOMMON_H_

#include "SglCommon.h"

// 1: Defines
// ----------------------------------------------------------------------------

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

/// @brief Common return values for all BM layer SHAVE drivers
typedef enum
{
    MYR_DRV_SUCCESS = SGL_SUCCESS,
    MYR_DRV_ERROR = SGL_RET_NB,
    MYR_DRV_NOT_INITIALIZED,
    MYR_DRV_ALREADY_INITIALIZED,
    MYR_DRV_RESOURCE_BUSY,
    MYR_DRV_INVALID_PARAM,
    MYR_DRV_RET_NB
} DRV_RETURN_TYPE;

#endif // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVCOMMON_H_

///@}
