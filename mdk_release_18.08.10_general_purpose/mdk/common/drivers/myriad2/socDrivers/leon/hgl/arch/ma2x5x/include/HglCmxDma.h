///
/// @file HglCmxDma.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup HglCmxDma HglCmxDma Driver
/// @ingroup  HglCmxDma
/// @{
/// @brief     Header of HGL for CMX DMA
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_INCLUDE_HGLCMXDMA_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_INCLUDE_HGLCMXDMA_H_


#ifdef USE_CMX_DMA_NEW_DRIVER

#include "HglCmxDmaDefines.h"
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include "swcWhoAmI.h"
#include <assert.h>


#define HGL_CMX_DMA_SKIP_ID 31
#define HGL_CMX_DMA_LOS_ID PROCESS_LEON_OS
#define HGL_CMX_DMA_LRT_ID PROCESS_LEON_RT
// macro to check if the given address is a CMX address
#define HGL_CMX_DMA_VALID_DESCRIPTOR_ADDRESS(a) \
  (((uint32_t)(a)>>24==0x70) || ((uint32_t)(a)>>24)==0x78)

// Default source/destination plane stride and number of plane
// value used when calling the HglCmxDmaCreateTransaction function
// for 1D or 2D DMA transaction.
#define HGL_CMX_DMA_DEFAULT_NUM_PLANE (0)
#define HGL_CMX_DMA_DEFAULT_PLANE_STRIDE (0)

// Valid range for the number of planes.
#define MIN_NUM_PLANES (1)
#define MAX_NUM_PLANES (256)

extern HglCmxDmaStatusInfo ___globalTail[HGL_CMX_DMA_LA_NUM];
//------------------------------
// API functions
//------------------------------
static inline
HglCmxDmaTransaction* HglCmxDmaCreateTransaction(
    HglCmxDmaTransaction* newTransaction, uint32_t type, uint8_t* src,
    uint8_t* dst, uint32_t length, uint32_t src_line_width,
    uint32_t dst_line_width, int32_t src_stride, int32_t dst_stride,
    uint8_t num_planes, int32_t src_plane_stride,
    int32_t dst_plane_stride, uint8_t burst_length) {
#ifdef HGL_CMX_DMA_CHECK_DESCR_ADDR
  assert(HGL_CMX_DMA_VALID_DESCRIPTOR_ADDRESS(newTransaction) &&
         "All CMX DMA descriptors must be placed in CMX");
#endif
  newTransaction->link_address = 0;
  newTransaction->cfg_link.cfg_bits.type = type;
  newTransaction->cfg_link.cfg_bits.burst_length = burst_length - 1;
  newTransaction->cfg_link.cfg_bits.reserved1 = 0;
  newTransaction->src = src;
  newTransaction->dst = dst;
  newTransaction->length = length;
  newTransaction->planes_no = num_planes;
  newTransaction->src_width = src_line_width;
  newTransaction->src_stride = src_stride;
  newTransaction->dst_width = dst_line_width;
  newTransaction->dst_stride = dst_stride;
  newTransaction->src_plane_stride = src_plane_stride;
  newTransaction->dst_plane_stride = dst_plane_stride;
  // just use the same skip id for all transactions
  newTransaction->cfg_link.cfg_bits.skip_nr = HGL_CMX_DMA_SKIP_ID;
  newTransaction->cfg_link.cfg_bits.disable_int = 0;
  if (swcWhoAmI() == PROCESS_LEON_OS)
    newTransaction->cfg_link.cfg_bits.id = HGL_CMX_DMA_LOS_ID;
  else
    newTransaction->cfg_link.cfg_bits.id = HGL_CMX_DMA_LRT_ID;

  return newTransaction;
}

static inline void HglCmxDmaEnable(void) {
  SET_REG_WORD(CDMA_CTRL_ADR, HGL_CMX_DMA_ENABLE_BIT);
}

static inline void HglCmxDmaDisable(void) {
  SET_REG_WORD(CDMA_CTRL_ADR, HGL_CMX_DMA_DISABLE_BIT);
}

static inline void HglCmxDmaSetArbitrationMode(uint32_t mode) {
  uint32_t reg_val;
  reg_val = GET_REG_WORD_VAL(CDMA_CTRL_ADR);
  if (mode == HGL_CMX_DMA_ARB_MODE_PRIO)
    reg_val |= HGL_CMX_DMA_ARB_MODE_BIT;
  else
    reg_val &= ~HGL_CMX_DMA_ARB_MODE_BIT;
  SET_REG_WORD(CDMA_CTRL_ADR, reg_val);
}

static inline void HglCmxDmaDisableAllChannels(void) {
  SET_REG_WORD(CDMA_SET_CHGATE_ADR, 0xf);
}

static inline void HglCmxDmaEnableAllChannels(void) {
  SET_REG_WORD(CDMA_CLR_CHGATE_ADR, 0xf);
}

static inline void HglCmxDmaChannelDisable(uint32_t channelNo) {
  SET_REG_WORD(CDMA_SET_CHGATE_ADR, (1 << channelNo));
}

static inline void HglCmxDmaChannelEnable(uint32_t channelNo) {
  SET_REG_WORD(CDMA_CLR_CHGATE_ADR, (1 << channelNo));
}

static inline void HglCmxDmaIrqDisable(uint32_t mask) {
  SET_REG_WORD(CDMA_CLR_INTEN_ADR, mask);
}

static inline void HglCmxDmaIrqClearStatus(uint32_t mask) {
  SET_REG_WORD(CDMA_INT_RESET_ADR, mask);
}

static inline uint32_t HglCmxDmaIrqGetStatus(void) {
  return GET_REG_WORD_VAL(CDMA_INT_STATUS_ADR);
}

static inline int32_t HglCmxDmaSetIrqTrigger(HglCmxDmaTransaction *transaction,
                                             uint32_t irq_trigger) {
  if (irq_trigger > HGL_CMX_DMA_MAX_IRQ_TRIG) return -1;
  transaction->cfg_link.cfg_bits.interrupt_trigger = irq_trigger;
  return 0;
}

// configures the number of transactions that need to be
// completed before rising an interrupt
static inline void HglCmxDmaSetIrqFrequency(uint32_t irq_no,
                                            uint32_t task_count) {
  SET_REG_WORD(CDMA_INT_FREQ0_ADR + 8 * irq_no, task_count - 1);
}

static inline int32_t HglCmxDmaSetId(HglCmxDmaTransaction *transaction,
                                     uint32_t id) {
  if (id > HGL_CMX_DMA_MAX_ID) return -1;
  transaction->cfg_link.cfg_bits.id = id;
  return 0;
}

static inline uint32_t HglCmxDmaGetId(HglCmxDmaTransaction *transaction) {
  return transaction->cfg_link.cfg_bits.id;
}

static inline int32_t HglCmxDmaSetBurstSize(HglCmxDmaTransaction *transaction,
                                            uint32_t burst_length) {
  if ((burst_length < HGL_CMX_DMA_MIN_BURST_LEN) ||
      (burst_length > HGL_CMX_DMA_MAX_BURST_LEN))
    return -1;
  transaction->cfg_link.cfg_bits.burst_length = burst_length - 1;
  return 0;
}

static inline int32_t HglCmxDmaSetPriority(HglCmxDmaTransaction *transaction,
                                           uint32_t priority) {
  if (priority > HGL_CMX_DMA_MAX_PRIO) return -1;
  transaction->cfg_link.cfg_bits.priority = priority;
  return 0;
}

static inline void HglCmxDmaWaitLaNotBusy(uint32_t agent) {
  while (HGL_CMX_DMA_AGENT_IS_BUSY(agent))
    ;
}

static inline uint32_t HglCmxDmaGetTopAddress(uint32_t agent) {
  return GET_REG_WORD_VAL(CDMA_TOP0_ADR + 8 * agent);
}

static inline void HglCmxDmaAssignLinkAgentToChannel(uint32_t laToChannel) {
  SET_REG_WORD(CDMA_CHTOAGN_ADR, laToChannel);
}

static inline void HglCmxDmaStartTransfer(uint32_t linkAgentNo,
                                          HglCmxDmaTransaction *transaction) {
  SET_REG_DWORD(CDMA_LINK_CFG0_ADR + 8 * linkAgentNo,
                1LL << 32 | (uint32_t)transaction);
}

static inline void HglCmxDmaWaitUntilLinkAgentIsntFetchingDescriptors(
    uint32_t linkAgentNo) {
  int32_t isFetching, reqSm;
  do {
    reqSm =
        0x7 & (GET_REG_WORD_VAL(CDMA_CHANNEL_DBG_ADR + 4) >> (linkAgentNo * 4));
    isFetching = (reqSm != S_POP_REQ_ISSUE) && (reqSm != S_POP_WAIT_RSP);
  } while (!isFetching);
}

// enable/disable interrupts and route the interrupt to
// LOS or LRT, depending on where it is called
void HglCmxDmaIrqEnable(uint32_t mask, uint32_t processor);
HglCmxDmaLinkAgentStatus HglCmxDmaLinkAgentGetStatus(uint32_t linkAgentNo);

#endif  // USE_CMX_DMA_NEW_DRIVER

#endif  // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_INCLUDE_HGLCMXDMA_H_

///@}
