///
/// @file HglCmxDma.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglCmxDma
/// @{
/// @brief     HGL common source file for CMX DMA
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "HglCmxDma.h"

#ifdef USE_CMX_DMA_NEW_DRIVER

#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include "swcWhoAmI.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define HGL_CMX_DMA_SKIP_ID 31
// macro to check if the given address is a CMX address
#define HGL_CMX_DMA_VALID_DESCRIPTOR_ADDRESS(a) \
  (((uint32_t)(a)>>24==0x70) || ((uint32_t)(a)>>24)==0x78)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
HglCmxDmaStatusInfo CMXDMA_SECTION ___globalTail[HGL_CMX_DMA_LA_NUM];
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation

void HglCmxDmaIrqEnable(uint32_t mask, uint32_t processor) {
  // route interrupts to LOS or LRT
  if (processor == PROCESS_LEON_OS) {
    SET_REG_WORD(CDMA_CLR_IMASK_ADR, mask);
  } else if (processor == PROCESS_LEON_RT) {
    SET_REG_WORD(CDMA_SET_IMASK_ADR, mask);
  }
  SET_REG_WORD(CDMA_SET_INTEN_ADR, mask);
}

HglCmxDmaLinkAgentStatus HglCmxDmaLinkAgentGetStatus(uint32_t agent) {
  HglCmxDmaLinkAgentStatus status = HGL_CMX_DMA_LA_NOT_BUSY;

  HglCmxDmaWaitUntilLinkAgentIsntFetchingDescriptors(agent);
  if (HGL_CMX_DMA_AGENT_IS_BUSY(agent)) {
    status = HGL_CMX_DMA_LA_BUSY;
  }
  return status;
}

#endif  // USE_CMX_DMA_NEW_DRIVER

///@}
