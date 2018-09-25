///
/// @file ShDrvCmxDma.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup ShDrvCmxDma ShCmxDma Driver
/// @{
/// @brief     CMX DMA driver API
///
/// Driver usage example:
///
///  #include "ShDrvCmxDma.h"
///
///  static ShDrvCmxDmaTransaction list[LIST_SIZE];
///  static ShDrvCmxDmaTransactionHnd handle;
///
///  // use NULL as parameter instead of config for default configuration
///  int32_t status = ShDrvCmxDmaInitialize(NULL);
///
///  if (status == MYR_DRV_SUCCESS) {
///    ScCdmaCreateTransaction(&handle, &list[0], src_buff, dst_buff, size);
///    for (i = 1; i < LIST_SIZE; i++)
///      ShDrvCmxDmaAddTransaction(&handle, &list[i], src_buff, dst_buff, size);
///    ShDrvCmxDmaStartTransfer(&handle);
///    ShDrvCmxDmaWaitTransaction(&handle);
///  }
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X8X_INCLUDE_SH_DRV_SHAVEDATA_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X8X_INCLUDE_SH_DRV_SHAVEDATA_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "ShaveDataDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2: Defines
// ----------------------------------------------------------------------------

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

extern ShDrvShaveDataStruct* __globalShaveData;

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif //COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X8X_INCLUDE_SH_DRV_SHAVEDATA_H_


///@}
