///
/// @file scCmxDma.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup scCmxDma
/// @{
/// @brief     CMX DMA driver implementation for SHAVE
///


#include "scCmxDma.h"

#ifdef USE_CMX_DMA_NEW_DRIVER

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include "ShDrvCommon.h"
#include "svuCommonShave.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#ifndef SC_CMX_DMA_DEFAULT_AGENT
#define SC_CMX_DMA_DEFAULT_AGENT 3
#endif

#define SC_CMX_DMA_SKIP_ID 31

#define SC_CMX_DMA_AGENT_BUSY_OFFSET 2
#define SC_CMX_DMA_LANE_BUSY_OFFSET 6
#define SC_CMX_DMA_LA_BUSY_MASK \
  0xF  // Bit mask used to check which link agent is busy.
#define SC_CMX_DMA_START_LCA_BIT (1LL << 32)

#define SC_CMX_DMA_IRQ_OFFSET 3
#define SC_CMX_DMA_IRQ_TRIG(shvnum) \
  (SC_CMX_DMA_IRQ_OFFSET + (shvnum))

#define SC_CMX_DMA_LANE_IS_BUSY(agent)                      \
  (GET_REG_WORD_VAL(CDMA_STATUS_ADR) &                      \
       ((1 << SC_CMX_DMA_LANE_BUSY_OFFSET) << (agent)))     \

#define SC_CMX_DMA_AGENT_IS_BUSY(agent)                     \
  (GET_REG_WORD_VAL(CDMA_STATUS_ADR) &                      \
         ((1 << SC_CMX_DMA_AGENT_BUSY_OFFSET) << (agent)))  \

// macro to check if the given address is a CMX address
#define SC_CMX_DMA_VALID_DESCRIPTOR_ADDRESS(a) \
  (((uint32_t)(a)>>24==0x70) || ((uint32_t)(a)>>24)==0x78)


typedef enum {
  SC_CMX_DMA_ARB_MODE_RR = 0,
  SC_CMX_DMA_ARB_MODE_PRIO
} ScCmxDmaArbMode;

typedef enum {
  SC_CMX_DMA_UNINITIALIZED = 0,
  SC_CMX_DMA_INITIALIZED,
} ScCmxDmaStatus;

typedef struct {
  ScCmxDmaStatus status;
  ScCmxDmaAgent agent;
  uint8_t proc;
} ScCmxDmaControl;

static uint32_t kScCmxDmaMutexId[DRV_CMX_DMA_LA_NUM] = {
    DRV_CMX_DMA_MUTEX_ID_LA_0, DRV_CMX_DMA_MUTEX_ID_LA_1,
    DRV_CMX_DMA_MUTEX_ID_LA_2, DRV_CMX_DMA_MUTEX_ID_LA_3};

typedef enum {
  S_POP_REQ_IDLE = 0,
  S_POP_REQ_ISSUE = 1,
  S_POP_WAIT_RSP = 2,
  S_POP_WAIT_TAKEN = 3,
  S_POP_STALL = 4,
} ScCmxDmaReqSmState;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
ScCmxDmaStatusInfo ___globalTail[DRV_CMX_DMA_LA_NUM];

// 4: Static Local Data
// ----------------------------------------------------------------------------
static ScCmxDmaControl cmx_dma_control;
static ScCmxDmaStatusInfo *statusInfo[DRV_CMX_DMA_LA_NUM];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
static void scCmxDmaGlobalStatusInit(void) {
  uint32_t i;
  for (i = 0; i < DRV_CMX_DMA_LA_NUM; i++) {
    statusInfo[i] = &___globalTail[i];
    if (!statusInfo[i]->initialized) {
      statusInfo[i]->tail = NULL;
      statusInfo[i]->sw_id = 0;
      statusInfo[i]->initialized = 1;
    }
  }
}

static inline void ScCmxDmaEnable(void) {
  SET_REG_WORD(CDMA_CTRL_ADR, SC_CMX_DMA_ENABLE_BIT);
}

static inline void ScCmxDmaAssignLinkAgentToChannel(uint32_t la_to_channel) {
  SET_REG_WORD(CDMA_CHTOAGN_ADR, la_to_channel);
}

static inline void ScCmxDmaChannelEnable(uint32_t channel) {
  SET_REG_WORD(CDMA_CLR_CHGATE_ADR, (1 << channel));
}

__attribute__((unused))
static inline void ScCmxDmaChannelDisable(uint32_t channel) {
  SET_REG_WORD(CDMA_SET_CHGATE_ADR, (1 << channel));
}

static inline void ScCmxDmaEnableAllChannels(void) {
  SET_REG_WORD(CDMA_CLR_CHGATE_ADR, 0xf);
}

static inline void ScCmxDmaDisableAllChannels(void) {
  SET_REG_WORD(CDMA_SET_CHGATE_ADR, 0xf);
}

static inline int32_t ScCmxDmaSetPriority(ScCmxDmaTransaction *transaction,
                                          uint32_t priority) {
  if (priority > SC_CMX_DMA_MAX_PRIO) return -1;
  transaction->cfg_link.cfg_bits.priority = priority;
  return 0;
}

static inline int32_t ScCmxDmaSetBurstSize(ScCmxDmaTransaction *transaction,
                                           uint32_t burst_length) {
  if ((burst_length < SC_CMX_DMA_MIN_BURST_LEN) ||
      (burst_length > SC_CMX_DMA_MAX_BURST_LEN))
    return -1;
  transaction->cfg_link.cfg_bits.burst_length = burst_length - 1;
  return 0;
}

static inline uint32_t scCmxDmaResolveRelAddr(uint32_t in_addr,
                                              uint32_t shave_number) {
  uint32_t window = 0;
  uint32_t window_base;
  uint32_t *win_reg_ptr =
      (uint32_t *)(SHAVE_0_BASE_ADR + (SVU_SLICE_OFFSET * shave_number) +
                   SLC_TOP_OFFSET_WIN_A);

  uint32_t resolved;
  switch (in_addr >> 24) {
    case 0x1C:
      window = 0;
      break;
    case 0x1D:
      window = 1;
      break;
    case 0x1E:
      window = 2;
      break;
    case 0x1F:
      window = 3;
      break;
    default:
      return (in_addr);
      break;  // absolute address, no translation is to be done
  }
  window_base = win_reg_ptr[window];
  resolved = ((in_addr & 0x00FFFFFF) + window_base);

  return resolved;
}

static inline ScCmxDmaTransaction *scCmxDmaCreateTransaction(
    ScCmxDmaTransaction *transaction, uint32_t type, uint8_t *src,
    uint8_t *dst, uint32_t length, uint32_t src_line_width,
    uint32_t dst_line_width, int32_t src_stride, int32_t dst_stride,
    uint32_t num_planes, int32_t src_plane_stride,
    int32_t dst_plane_stride, uint8_t burst_length) {
#ifdef SC_CMX_DMA_CHECK_DESCR_ADDR
  assert(SC_CMX_DMA_VALID_DESCRIPTOR_ADDRESS(
                    scCmxDmaResolveRelAddr((uint32_t)transaction,
                                           cmx_dma_control.proc)
                    ));
#endif
  transaction->link_address = 0;
  transaction->cfg_link.cfg_bits.type = type;
  transaction->cfg_link.cfg_bits.burst_length = burst_length - 1;
  transaction->src =
    (uint8_t*)scCmxDmaResolveRelAddr((uint32_t)src, cmx_dma_control.proc);
  transaction->dst =
    (uint8_t*)scCmxDmaResolveRelAddr((uint32_t)dst, cmx_dma_control.proc);
  transaction->length = length;
  transaction->planes_no = num_planes;
  transaction->src_width = src_line_width;
  transaction->src_stride = src_stride;
  transaction->dst_width = dst_line_width;
  transaction->dst_stride = dst_stride;
  transaction->src_plane_stride = src_plane_stride;
  transaction->dst_plane_stride = dst_plane_stride;
  // just use the same skip id for all transactions
  transaction->cfg_link.cfg_bits.skip_nr = SC_CMX_DMA_SKIP_ID;
  transaction->cfg_link.cfg_bits.disable_int = 1;
  return transaction;
}

static inline void scCmxDmaWaitUntilLinkAgentIsntFetchingDescriptors(
    uint32_t link_agent) {
  int32_t isFetching, reqSm;
  do {
    reqSm =
        0x7 & (GET_REG_WORD_VAL(CDMA_CHANNEL_DBG_ADR + 4) >> (link_agent * 4));
    isFetching = (reqSm != S_POP_REQ_ISSUE) && (reqSm != S_POP_WAIT_RSP);
  } while (!isFetching);
}

static inline uint32_t ScCmxDmaGetTopAddress(uint32_t agent) {
  return GET_REG_WORD_VAL(CDMA_TOP0_ADR + 8 * agent);
}

// returns the sw id of the transaction at the top of the list
static inline int32_t scCmxDmaGetTopSwId(uint32_t agent, uint32_t *id) {
  uint32_t top_addr = ScCmxDmaGetTopAddress(agent);
  if (top_addr) {
    *id = ((ScCmxDmaTransaction *)top_addr)->sw_id;
    return 0;
  }
  return -1;
}

static inline void scCmxDmaSaveTail(uint32_t agent, ScCmxDmaTransaction *tail) {
  ScCmxDmaTransaction __attribute__((unused)) *tr;  // avoid 'unused variable' error from scan build
  UNUSED(tr);
  statusInfo[agent]->tail = tail;
  // read back to ensure that previous write transaction is finished
  tr = *((ScCmxDmaTransaction**)&statusInfo[agent]->tail);
}

static inline void scCmxDmaStartTransfer(ScCmxDmaTransactionHnd *handle) {
  scCmxDmaSaveTail(cmx_dma_control.agent, handle->tail);
  SET_REG_DWORD(CDMA_LINK_CFG0_ADR + 8 * cmx_dma_control.agent,
                (uint64_t)((uint32_t)handle->head) | SC_CMX_DMA_START_LCA_BIT);
}

static inline ScCmxDmaTransaction *scCmxDmaGetTail(uint32_t agent) {
  return statusInfo[agent]->tail;
}

// sets the parameters for a list
static inline void scCmxDmaListSetup(ScCmxDmaTransactionHnd *handle) {
  ScCmxDmaTransaction *list_ptr = handle->head;

  while (list_ptr) {
    list_ptr->sw_id = statusInfo[cmx_dma_control.agent]->sw_id;
    list_ptr = list_ptr->link_address;
  }
  statusInfo[cmx_dma_control.agent]->sw_id++;
}

static inline void ScCmxDmaSetArbitrationMode(uint32_t mode) {
  uint32_t reg_val;
  reg_val = GET_REG_WORD_VAL(CDMA_CTRL_ADR);
  if (mode == SC_CMX_DMA_ARB_MODE_PRIO)
    reg_val |= SC_CMX_DMA_ARB_MODE_BIT;
  else
    reg_val &= ~SC_CMX_DMA_ARB_MODE_BIT;
  SET_REG_WORD(CDMA_CTRL_ADR, reg_val);
}

// links a new list to the end of the current list and updates tail pointer
static inline void scCmxDmaLinkToTail(ScCmxDmaTransactionHnd *handle,
                                      uint32_t agent) {
  ScCmxDmaTransaction *tail;

  tail = scCmxDmaGetTail(agent);
  // link at the end of the current list
  tail->link_address = handle->head;
  // read back to ensure that previous write transaction is finished
  GET_REG_WORD_VAL((ScCmxDmaTransaction*)&tail->link_address);
  // for the case when head and tail point to the same descriptor
  handle->tail->link_address = NULL;
  scCmxDmaSaveTail(agent, handle->tail);
}

ScCmxDmaLinkAgentStatus ScCmxDmaLinkAgentGetStatus(uint32_t link_agent) {
  ScCmxDmaLinkAgentStatus status = SC_CMX_DMA_LA_NOT_BUSY;

  scCmxDmaWaitUntilLinkAgentIsntFetchingDescriptors(link_agent);
  if (SC_CMX_DMA_AGENT_IS_BUSY(link_agent)) {
    status = SC_CMX_DMA_LA_BUSY;
  }
  return status;
}

int32_t ScCmxDmaInitialize(ScCmxDmaSetupStruct *config) {
  if (cmx_dma_control.status == SC_CMX_DMA_INITIALIZED) {
    return MYR_DRV_ALREADY_INITIALIZED;
  }

  cmx_dma_control.proc = scGetShaveNumber();
  scCmxDmaGlobalStatusInit();
  ScCmxDmaSetArbitrationMode(SC_CMX_DMA_ARB_MODE_PRIO);
  if (config) {
    // user provided configuration
    if (config->agent > DRV_CMX_DMA_LA_NUM)
      // invalid parameters;
      return MYR_DRV_ERROR;
    cmx_dma_control.agent = config->agent;
  } else {
    // default configuration
    cmx_dma_control.agent = SC_CMX_DMA_DEFAULT_AGENT;
  }
  ScCmxDmaEnable();
  ScCmxDmaAssignLinkAgentToChannel(DRV_CMX_DMA_LA_TO_CHAN_DEFAULT_MAP);
  ScCmxDmaChannelEnable(cmx_dma_control.agent);

  cmx_dma_control.status = SC_CMX_DMA_INITIALIZED;

  return MYR_DRV_SUCCESS;
}

int32_t ScCmxDmaTransactionConfig(ScCmxDmaTransaction *transaction,
                                  ScCmxDmaConfParams *params) {
  if (!transaction || !params) return MYR_DRV_ERROR;
  if (ScCmxDmaSetPriority(transaction, params->priority) != MYR_DRV_SUCCESS)
    return MYR_DRV_ERROR;
  if (ScCmxDmaSetBurstSize(transaction, params->burst_size) != MYR_DRV_SUCCESS)
    return MYR_DRV_ERROR;
  return MYR_DRV_SUCCESS;
}

int32_t ScCmxDmaCreateTransaction(ScCmxDmaTransactionHnd *handle,
                                  ScCmxDmaTransaction *transaction,
                                  uint8_t *src, uint8_t *dst, uint32_t size) {
  int32_t status = MYR_DRV_ERROR;

  //if (transaction && handle && src && dst) {
    scCmxDmaCreateTransaction(transaction, DRV_CMX_DMA_1D_TRANSACTION, src,
                              dst, size, 0, 0, 0, 0,
                              SC_CMX_DMA_DEFAULT_NUM_PLANE,
                              SC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                              SC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                              SC_CMX_DMA_DEFAULT_BURST_SIZE);
    ScCmxDmaSetPriority(transaction, SC_CMX_DMA_DEFAULT_TRAN_PRIO);
    handle->head = handle->tail =
      (ScCmxDmaTransaction*)scCmxDmaResolveRelAddr((uint32_t)transaction,
                                                   cmx_dma_control.proc);
    status = MYR_DRV_SUCCESS;
  //}
  return status;
}

int32_t ScCmxDmaCreateStrideTransaction(ScCmxDmaTransactionHnd *handle,
                                        ScCmxDmaTransaction *transaction,
                                        uint8_t *src, uint8_t *dst,
                                        uint32_t src_width, uint32_t dst_width,
                                        uint32_t src_stride,
                                        uint32_t dst_stride, uint32_t size) {
  int32_t status = MYR_DRV_ERROR;
  int32_t type = DRV_CMX_DMA_1D_TRANSACTION;

  if (transaction && handle && src && dst) {
    if ((src_width != 0) || (dst_width != 0) || (src_stride != 0) ||
        (dst_stride != 0)) {
      type = DRV_CMX_DMA_2D_TRANSACTION;
    }
    scCmxDmaCreateTransaction(transaction, type, src, dst, size, src_width,
                              dst_width, src_stride, dst_stride,
                              SC_CMX_DMA_DEFAULT_NUM_PLANE,
                              SC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                              SC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                              SC_CMX_DMA_DEFAULT_BURST_SIZE);
    if (ScCmxDmaSetPriority(transaction, SC_CMX_DMA_DEFAULT_TRAN_PRIO) !=
        MYR_DRV_SUCCESS)
      return MYR_DRV_ERROR;
    handle->head = handle->tail =
      (ScCmxDmaTransaction*)scCmxDmaResolveRelAddr((uint32_t)transaction,
                                                   cmx_dma_control.proc);
    status = MYR_DRV_SUCCESS;
  }
  return status;
}

int32_t ScCmxDmaCreate3DTransaction(ScCmxDmaTransactionHnd *handle,
                                    ScCmxDmaTransaction *transaction,
                                    uint8_t *src, uint8_t *dst,
                                    uint32_t src_width, uint32_t dst_width,
                                    uint32_t src_stride, uint32_t dst_stride,
                                    uint32_t num_planes, uint32_t src_plane_stride,
                                    uint32_t dst_plane_stride, uint32_t size) {
  int32_t status = MYR_DRV_ERROR;

  if (transaction && handle && src && dst &&
     (num_planes >= MIN_NUM_PLANES) && (num_planes <= MAX_NUM_PLANES)) {
    if ((src_width != 0) || (dst_width != 0) ||
        (src_stride != 0) || (dst_stride != 0) ||
        (src_plane_stride != 0) || (dst_plane_stride != 0)) {
      // In 3D mode, the "num_planes" field holds the number of planes minus one.
      num_planes--;

      scCmxDmaCreateTransaction(transaction, DRV_CMX_DMA_2D_TRANSACTION,
                                src, dst, size,
                                src_width, dst_width,
                                src_stride, dst_stride,
                                num_planes,
                                src_plane_stride, dst_plane_stride,
                                SC_CMX_DMA_DEFAULT_BURST_SIZE);

      if (MYR_DRV_SUCCESS == ScCmxDmaSetPriority(transaction,
                                          SC_CMX_DMA_DEFAULT_TRAN_PRIO)) {
        handle->head = handle->tail =
          (ScCmxDmaTransaction*)scCmxDmaResolveRelAddr((uint32_t)transaction,
                                                       cmx_dma_control.proc);
        status = MYR_DRV_SUCCESS;
      }
    }
  }

  return status;
}

int32_t ScCmxDmaLinkTransactions(ScCmxDmaTransactionHnd *list_head,
                                 uint32_t list_cnt, ...) {
  va_list a_list;
  uint32_t i;
  ScCmxDmaTransactionHnd *list_hnd;

  if (list_cnt == 0) return MYR_DRV_ERROR;

  va_start(a_list, list_cnt);

  list_hnd = va_arg(a_list, ScCmxDmaTransactionHnd *);
  list_head->head = list_hnd->head;
  list_head->tail = list_hnd->tail;
  for (i = 1; i < list_cnt; i++) {
    list_hnd = va_arg(a_list, ScCmxDmaTransactionHnd *);
    list_head->tail->link_address = list_hnd->head;
    list_head->tail = list_hnd->tail;
  }

  va_end(a_list);

  return MYR_DRV_SUCCESS;
}

int32_t ScCmxDmaAddTransaction(ScCmxDmaTransactionHnd *handle,
                               ScCmxDmaTransaction *transaction,
                               uint8_t *src, uint8_t *dst, uint32_t size) {
  return ScCmxDmaAddStrideTransaction(handle, transaction, src, dst,
                                      0, 0, 0, 0, size);
}

int32_t ScCmxDmaAddStrideTransaction(ScCmxDmaTransactionHnd *handle,
                                     ScCmxDmaTransaction *transaction,
                                     uint8_t *src, uint8_t *dst,
                                     uint32_t src_width, uint32_t dst_width,
                                     uint32_t src_stride, uint32_t dst_stride,
                                     uint32_t size) {
  int32_t type = DRV_CMX_DMA_1D_TRANSACTION;

  if (!handle || !transaction || !src || !dst) return MYR_DRV_ERROR;
  if ((src_width != 0) || (dst_width != 0) ||
      (src_stride != 0) || (dst_stride != 0)) {
    type = DRV_CMX_DMA_2D_TRANSACTION;
  }
  scCmxDmaCreateTransaction(transaction, type, src, dst, size, src_width,
                            dst_width, src_stride, dst_stride,
                            SC_CMX_DMA_DEFAULT_NUM_PLANE,
                            SC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                            SC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                            SC_CMX_DMA_DEFAULT_BURST_SIZE);
  if (ScCmxDmaSetPriority(transaction, SC_CMX_DMA_DEFAULT_TRAN_PRIO) !=
      MYR_DRV_SUCCESS)
    return MYR_DRV_ERROR;

  handle->tail = handle->tail->link_address =
    (ScCmxDmaTransaction*)scCmxDmaResolveRelAddr((uint32_t)transaction,
                                                 cmx_dma_control.proc);
  return MYR_DRV_SUCCESS;
}

int32_t ScCmxDmaAdd3DTransaction(ScCmxDmaTransactionHnd *handle,
                                 ScCmxDmaTransaction *transaction,
                                 uint8_t *src, uint8_t *dst,
                                 uint32_t src_width, uint32_t dst_width,
                                 uint32_t src_stride, uint32_t dst_stride,
                                 uint32_t num_planes, uint32_t src_plane_stride,
                                 uint32_t dst_plane_stride, uint32_t size) {
  int32_t status = MYR_DRV_ERROR;

  if (transaction && handle && src && dst &&
     (num_planes >= MIN_NUM_PLANES) && (num_planes <= MAX_NUM_PLANES)) {
    if ((src_width != 0) || (dst_width != 0) ||
        (src_stride != 0) || (dst_stride != 0) ||
        (src_plane_stride != 0) || (dst_plane_stride != 0)) {
      // In 3D mode, the "num_planes" field holds the number of planes minus one.
      num_planes--;

      scCmxDmaCreateTransaction(transaction, DRV_CMX_DMA_2D_TRANSACTION,
                                src, dst, size,
                                src_width, dst_width,
                                src_stride, dst_stride,
                                num_planes,
                                src_plane_stride, dst_plane_stride,
                                SC_CMX_DMA_DEFAULT_BURST_SIZE);

      if (MYR_DRV_SUCCESS == ScCmxDmaSetPriority(transaction,
                                          SC_CMX_DMA_DEFAULT_TRAN_PRIO)) {
        handle->tail = handle->tail->link_address =
          (ScCmxDmaTransaction*)scCmxDmaResolveRelAddr((uint32_t)transaction,
                                                       cmx_dma_control.proc);
        status = MYR_DRV_SUCCESS;
      }
    }
  }

  return status;
}

int32_t ScCmxDmaStartTransfer(ScCmxDmaTransactionHnd *handle) {
  uint32_t agent = cmx_dma_control.agent;
  uint32_t top_id = 0;
  uint32_t shadowRegFree;

  scMutexRequest(kScCmxDmaMutexId[agent]);
  scCmxDmaListSetup(handle);

  ScCmxDmaDisableAllChannels();
  scCmxDmaWaitUntilLinkAgentIsntFetchingDescriptors(agent);

  shadowRegFree = SC_CMX_DMA_SHADOW_REG_FREE(agent);
  if (shadowRegFree) {
    handle->head->current_id = handle->head->sw_id;
    scCmxDmaStartTransfer(handle);
  } else {
    if (scCmxDmaGetTopSwId(agent, &top_id) != 0)
      handle->head->current_id = top_id;
    else
      handle->head->current_id = handle->head->sw_id;
    // a transfer is still ongoing
    scCmxDmaLinkToTail(handle, agent);
  }

  ScCmxDmaEnableAllChannels();
  scMutexRelease(kScCmxDmaMutexId[agent]);

  return MYR_DRV_SUCCESS;
}

int32_t ScCmxDmaWaitTransaction(ScCmxDmaTransactionHnd *handle) {
  ScCmxDmaTransactionStatus t_status;
  int32_t status;

  do {
    status = ScCmxDmaGetTransactionStatus(handle, &t_status);
  } while ((status == MYR_DRV_SUCCESS) && (t_status != DRV_CMX_DMA_FINISHED));

  return status;
}

int32_t ScCmxDmaGetTransactionStatus(ScCmxDmaTransactionHnd *handle,
                                     ScCmxDmaTransactionStatus *t_status) {
  int32_t status;
  uint32_t id = 0;
  uint32_t sw_id;

  status = scCmxDmaGetTopSwId(cmx_dma_control.agent, &id);
  sw_id = handle->head->sw_id;

  if (status != 0) {
    if (SC_CMX_DMA_AGENT_IS_BUSY(cmx_dma_control.agent) ||
        SC_CMX_DMA_LANE_IS_BUSY(cmx_dma_control.agent)) {
      *t_status = DRV_CMX_DMA_PENDING;
    }
    else
      *t_status = DRV_CMX_DMA_FINISHED;
  } else if (handle->head->current_id <= sw_id) {
    if (id <= sw_id + 2)
      *t_status = DRV_CMX_DMA_PENDING;
    else
      *t_status = DRV_CMX_DMA_FINISHED;
  } else {
    // overflow case
    if ((id > sw_id) && (id < handle->head->current_id))
      *t_status = DRV_CMX_DMA_FINISHED;
    else
      *t_status = DRV_CMX_DMA_PENDING;
  }
  return MYR_DRV_SUCCESS;
}

#endif // USE_CMX_DMA_NEW_DRIVER


///@}
