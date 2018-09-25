///
/// @file DrvCmxDmaSharedDefines.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvCmxDmaShared
/// @{
/// @brief     CMX DMA shared defines and types
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHARED_INCLUDE_DRVCMXDMASHAREDDEFINES_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHARED_INCLUDE_DRVCMXDMASHAREDDEFINES_H_

// 0: Includes
// ----------------------------------------------------------------------------
#include "swcMutexUsage.h"

// 1: Defines
// ----------------------------------------------------------------------------
#define DRV_CMX_DMA_LA_NUM 4
#define DRV_CMX_DMA_LA_IRQ_OFFSET 16

#ifndef DRV_CMX_DMA_LOS_DEFAULT_LA
#define DRV_CMX_DMA_LOS_DEFAULT_LA 1
#endif

#ifndef DRV_CMX_DMA_LRT_DEFAULT_LA
#define DRV_CMX_DMA_LRT_DEFAULT_LA 2
#endif

#ifndef DRV_CMX_DMA_SHV_DEFAULT_LA
#define DRV_CMX_DMA_SHV_DEFAULT_LA 3
#endif

#ifndef DRV_CMX_DMA_MUTEX_ID_LA_0
#define DRV_CMX_DMA_MUTEX_ID_LA_0 CMXDMA_M0_MUTEX_24
#endif

#ifndef DRV_CMX_DMA_MUTEX_ID_LA_1
#define DRV_CMX_DMA_MUTEX_ID_LA_1 CMXDMA_M1_MUTEX_25
#endif

#ifndef DRV_CMX_DMA_MUTEX_ID_LA_2
#define DRV_CMX_DMA_MUTEX_ID_LA_2 CMXDMA_M2_MUTEX_26
#endif

#ifndef DRV_CMX_DMA_MUTEX_ID_LA_3
#define DRV_CMX_DMA_MUTEX_ID_LA_3 CMXDMA_M3_MUTEX_27
#endif

#define DRV_CMX_DMA_IRQ_NO(p) \
  (((p) == PROCESS_LEON_OS) ? 1 : (((p) == PROCESS_LEON_RT) ? 2 : 1))
#define SC_CMX_DMA_SHADOW_REG_FREE(agent) \
    ((GET_REG_WORD_VAL(CDMA_LINK_CFG0_ADR + 8 * (agent) + 4) & 1) == 0)
#define DRV_CMX_DMA_IRQ_MASK(p) (1 << DRV_CMX_DMA_IRQ_NO(p))
#define DRV_CMX_DMA_LA_MASK(agent) ((1 << (agent)) << DRV_CMX_DMA_LA_IRQ_OFFSET)
#define DRV_CMX_DMA_LA_TO_CHAN_DEFAULT_MAP \
  0x3210  // Map each channel to a single Link Agent.

#define SVU_SLICE_OFFSET  0x10000

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef enum {
  DRV_CMX_DMA_AGENT0 = 0,
  DRV_CMX_DMA_AGENT1,
  DRV_CMX_DMA_AGENT2,
  DRV_CMX_DMA_AGENT3,
  DRV_CMX_DMA_AGENTAUTO
} DrvCmxDmaAgent;

typedef enum {
  DRV_CMX_DMA_PENDING = 0,
  DRV_CMX_DMA_FINISHED
} DrvCmxDmaTransactionStatus;

typedef enum {
  DRV_CMX_DMA_1D_TRANSACTION = 0,
  DRV_CMX_DMA_2D_TRANSACTION
} DrvCmxDmaTransactionType;

#endif  // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHARED_INCLUDE_DRVCMXDMASHAREDDEFINES_H_

///@}
