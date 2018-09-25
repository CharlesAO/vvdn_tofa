///
/// @file OsDrvCdmaDefines.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvCdma
/// @{
/// @brief     CMX DMA RTEMS driver exported types and macros
///

#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OSDRVCDMADEFINES_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OSDRVCDMADEFINES_H_

#ifdef USE_CMX_DMA_NEW_DRIVER

// Common for all OS drivers
#include "OsCommon.h"
// HGL layer include
#include "HglCmxDma.h"
#include "DrvCmxDmaSharedDefines.h"

// 1: Defines
// ----------------------------------------------------------------------------
#ifndef OS_DRV_CMX_DMA_DEFAULT_IRQ_PRIO
#define OS_DRV_CMX_DMA_DEFAULT_IRQ_PRIO 7
#endif

#ifndef OS_DRV_CMX_DMA_DEFAULT_TRAN_PRIO
#define OS_DRV_CMX_DMA_DEFAULT_TRAN_PRIO 3
#endif

#ifndef OS_DRV_CMX_DMA_DEFAULT_BURST_SIZE
#define OS_DRV_CMX_DMA_DEFAULT_BURST_SIZE 16
#endif


// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef HglCmxDmaTransaction OsDrvCmxDmaTransaction;
typedef HglCmxDmaStatusInfo OsDrvCmxDmaStatusInfo;
typedef DrvCmxDmaAgent OsDrvCmxDmaAgent;
typedef DrvCmxDmaTransactionStatus OsDrvCmxDmaTransactionStatus;
typedef DrvCmxDmaTransactionType OsDrvCmxDmaTransactionType;

typedef struct {
  uint8_t irq_priority;
  OsDrvCmxDmaAgent agent;
} OsDrvCmxDmaSetupStruct;

typedef struct OsDrvCmxDmaTransactionHnd {
  OsDrvCmxDmaTransaction *head;
  OsDrvCmxDmaTransaction *tail;
  uint32_t list_size;
  uint32_t current_id;
  uint32_t task_id;
  uint8_t status;
} OsDrvCmxDmaTransactionHnd;

typedef struct {
  uint8_t priority;
  uint8_t burst_size;
} OsDrvCmxDmaConfParams;

#endif  // USE_CMX_DMA_NEW_DRIVER

#endif  // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OSDRVCDMADEFINES_H_


///@}
