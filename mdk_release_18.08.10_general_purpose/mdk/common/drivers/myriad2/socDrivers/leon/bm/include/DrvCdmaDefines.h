///
/// @file DrvCdmaDefines.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvCdma
/// @{
/// @brief     Definitions header of CMX DMA driver
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_BM_INCLUDE_DRVCDMADEFINES_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_BM_INCLUDE_DRVCDMADEFINES_H_

#include "HglCmxDma.h"
#include "DrvCmxDmaSharedDefines.h"

#ifndef DRV_CMX_DMA_DEFAULT_IRQ_PRIO
#define DRV_CMX_DMA_DEFAULT_IRQ_PRIO 7
#endif

#ifndef DRV_CMX_DMA_DEFAULT_TRAN_PRIO
#define DRV_CMX_DMA_DEFAULT_TRAN_PRIO 3
#endif

#ifndef DRV_CMX_DMA_DEFAULT_BURST_SIZE
#define DRV_CMX_DMA_DEFAULT_BURST_SIZE 16
#endif

#ifdef CDMA_USE_FAST_MTX
#define DRV_CMX_DMA_MUTEX_LOCK DrvFastMutexLock
#define DRV_CMX_DMA_MUTEX_UNLOCK DrvFastMutexUnlock
#elif defined(CDMA_USE_EMPTY_MTX)
#define DRV_CMX_DMA_MUTEX_LOCK(x)
#define DRV_CMX_DMA_MUTEX_UNLOCK(x)
#else
#define DRV_CMX_DMA_MUTEX_LOCK DrvMutexLock  // Default
#define DRV_CMX_DMA_MUTEX_UNLOCK DrvMutexUnlock
#endif

struct DrvCmxDmaTransactionHnd;
typedef void (*DrvCmxDmaOnCompletionCallback)(
    struct DrvCmxDmaTransactionHnd *handle, void *user_context);

typedef HglCmxDmaTransaction DrvCmxDmaTransaction;
typedef HglCmxDmaStatusInfo  DrvCmxDmaStatusInfo;

typedef struct {
  uint8_t irq_priority;
  uint8_t irq_enable;
  DrvCmxDmaAgent agent;
} DrvCmxDmaSetupStruct;

typedef struct DrvCmxDmaTransactionHnd {
  DrvCmxDmaTransaction *head;
  DrvCmxDmaTransaction *tail;
  DrvCmxDmaOnCompletionCallback callback;
  void *context;
  uint32_t list_size;
  uint32_t current_id;
  volatile uint8_t status;
} DrvCmxDmaTransactionHnd;

typedef struct {
  uint8_t priority;
  uint8_t burst_size;
} DrvCmxDmaConfParams;

#endif  // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_BM_INCLUDE_DRVCDMADEFINES_H_


///@}
