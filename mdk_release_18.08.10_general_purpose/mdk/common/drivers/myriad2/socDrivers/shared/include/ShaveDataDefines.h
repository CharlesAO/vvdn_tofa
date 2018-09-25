///
/// @file ShDrvCmxDmaDefines.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup ShDrvCmxDma
/// @{
/// @brief     Definitions header of CMX DMA driver for SHAVE
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X8X_INCLUDE_SH_DRV_SHAVEDATA_DEFINES_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X8X_INCLUDE_SH_DRV_SHAVEDATA_DEFINES_H_

#ifdef __cplusplus
extern "C" {
#endif

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvSvuDefines.h>

// 2: Defines
// ----------------------------------------------------------------------------

typedef struct {
    char** Xdata;
    char** Zdata;
    char** dyndata;
    void* profileData;
} ShDrvShaveDataStruct;

#ifdef __cplusplus
}
#endif

#endif //COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X8X_INCLUDE_SH_DRV_SHAVEDATA_DEFINES_H_

///@}
