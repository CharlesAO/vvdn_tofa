///
/// @file DrvCdma.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvCdma
/// @{
/// @brief     CMX DMA driver implementation
///


// 1: Includes
// ----------------------------------------------------------------------------
#ifdef USE_CMX_DMA_NEW_DRIVER

#include <stdint.h>
#include <stdarg.h>
#include <limits.h>
#include "HglCmxDma.h"
#include "DrvCommon.h"
#include "DrvCdma.h"
#include "swcWhoAmI.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define DRV_CMX_DMA_MAX_QUEUED_TASKS 16
#define DRV_CMX_DMA_MAX_IRQ_PRIO 14
// irq priority value when interrupts are disabled
#define DRV_CMX_DMA_UNDEF_IRQ_PRIO -1

typedef enum {
  DRV_CMX_DMA_UNINITIALIZED = 0,
  DRV_CMX_DMA_INITIALIZED,
} DrvCmxDmaStatus;

typedef enum {
  DRV_CMX_DMA_STARTED = 0,
  DRV_CMX_DMA_NOT_STARTED,
  DRV_CMX_DMA_DONE,
} DrvCmxDmaHndStatus;

typedef struct {
  DrvCmxDmaStatus status;
  DrvCmxDmaAgent agent;
  uint32_t irq_mask;
  uint8_t proc;
  int8_t irq_priority;
  int8_t irq_ena;
  volatile int8_t can_start_new;
} DrvCmxDmaControl;

typedef struct {
  DrvCmxDmaTransactionHnd *queue[DRV_CMX_DMA_MAX_QUEUED_TASKS];
  uint16_t first;
  uint16_t last;
  int16_t count;
} TransactionQueue;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
static const uint32_t kDrvCmxDmaMutexId[DRV_CMX_DMA_LA_NUM] = {
    DRV_CMX_DMA_MUTEX_ID_LA_0, DRV_CMX_DMA_MUTEX_ID_LA_1,
    DRV_CMX_DMA_MUTEX_ID_LA_2, DRV_CMX_DMA_MUTEX_ID_LA_3};
static DrvCmxDmaControl cmx_dma_control;
static DrvCmxDmaStatusInfo *statusInfo[DRV_CMX_DMA_LA_NUM];
static TransactionQueue trQueue;
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static void drvCmxDmaTryStart(DrvCmxDmaTransactionHnd *handle,
                              DrvCmxDmaOnCompletionCallback callback,
                              void *context, uint32_t agent);
// 6: Functions Implementation

static void drvCmxDmaGlobalStatusInit(void) {
  uint32_t i;
  for (i = 0; i < DRV_CMX_DMA_LA_NUM; i++) {
    statusInfo[i] = &___globalTail[i];
  }
}

static inline void drvCmxDmaSaveTail(uint32_t agent,
                                     DrvCmxDmaTransaction *tail) {
  DrvCmxDmaTransaction __attribute__((unused)) *tr; // avoid 'unused variable' error from scan build
  UNUSED(tr);
  statusInfo[agent]->tail = tail;
  // read back to ensure that previous write transaction is finished
  tr = *((DrvCmxDmaTransaction**)&statusInfo[agent]->tail);
}

static inline DrvCmxDmaTransaction *drvCmxDmaGetTail(uint32_t agent) {
  return statusInfo[agent]->tail;
}

// sets the parameters for a list
static void drvCmxDmaListSetup(DrvCmxDmaTransactionHnd *handle) {
  DrvCmxDmaTransaction *list_ptr = handle->head;

  // irq trigger is needed only when interrupts are enabled
  while (list_ptr) {
    HglCmxDmaSetIrqTrigger(list_ptr, DRV_CMX_DMA_IRQ_NO(cmx_dma_control.proc));
    list_ptr->sw_id = statusInfo[cmx_dma_control.agent]->sw_id;
    list_ptr = list_ptr->link_address;
  }
  statusInfo[cmx_dma_control.agent]->sw_id++;
}

static inline void drvCmxDmaStartTransfer(DrvCmxDmaTransactionHnd *handle,
                                   DrvCmxDmaOnCompletionCallback callback,
                                   void *context) {
  cmx_dma_control.can_start_new = 0;
  drvCmxDmaSaveTail(cmx_dma_control.agent, handle->tail);
  HglCmxDmaSetIrqFrequency(DRV_CMX_DMA_IRQ_NO(cmx_dma_control.proc),
                           handle->list_size);
  handle->callback = callback;
  handle->context = context;
  HglCmxDmaStartTransfer(cmx_dma_control.agent, handle->head);
}

static inline void drvCmxDmaInitTransactionQueue(void) {
  trQueue.first = 0;
  trQueue.last = -1;
  trQueue.count = 0;
}

static inline int32_t drvCmxDmaSaveTransaction(
    DrvCmxDmaTransactionHnd *handle) {
  uint32_t pil;
  if (trQueue.count == DRV_CMX_DMA_MAX_QUEUED_TASKS) {
    return MYR_DRV_ERROR;
  }
  trQueue.last++;
  if (trQueue.last >= DRV_CMX_DMA_MAX_QUEUED_TASKS)
    trQueue.last = 0;
  trQueue.queue[trQueue.last] = handle;
  pil = swcLeonSetPIL(15);
  trQueue.count++;
  swcLeonSetPIL(pil);

  return MYR_DRV_SUCCESS;
}

static inline DrvCmxDmaTransactionHnd *drvCmxDmaGetCurrentTransaction(void) {
  DrvCmxDmaTransactionHnd *current;

  if (trQueue.count == 0)
    return NULL;
  current = trQueue.queue[trQueue.first];
  trQueue.queue[trQueue.first] = NULL;
  trQueue.first++;
  if (trQueue.first >= DRV_CMX_DMA_MAX_QUEUED_TASKS)
    trQueue.first = 0;
  trQueue.count--;

  return current;
}

// get the next transaction without removing it form the queue
static inline DrvCmxDmaTransactionHnd *drvCmxDmaGetNextTransaction(void) {
  return trQueue.queue[trQueue.first];
}

void DrvCmxDmaIrqHandler(uint32_t source) {
  UNUSED(source);
  DrvCmxDmaTransactionHnd *current;
  DrvCmxDmaTransactionHnd *next;
  uint32_t status;
  uint32_t mask;

  status = HglCmxDmaIrqGetStatus();
  mask = cmx_dma_control.irq_mask;
  if (status & mask) {
    HglCmxDmaIrqClearStatus(mask);
    DrvIcbIrqClear(IRQ_CMXDMA);
    // the previous transaction has finished, so a
    // as new one can be started if available
    cmx_dma_control.can_start_new = 1;
    current = drvCmxDmaGetCurrentTransaction();
    // execute the callback for the finished transaction, if available
    if (current) {
      current->status = DRV_CMX_DMA_DONE;
      if (current->callback)
        current->callback(current, current->context);
    }

    // execute the next available transaction list
    next = drvCmxDmaGetNextTransaction();
    if (next && (next->status == DRV_CMX_DMA_NOT_STARTED)) {
      drvCmxDmaTryStart(next, next->callback,
                        next->context, cmx_dma_control.agent);
    }
  }
}

// returns the sw id of the transaction at the top of the list
static int32_t drvCmxDmaGetTopSwId(uint32_t agent, uint32_t *id) {
  uint32_t topAddr = HglCmxDmaGetTopAddress(agent);
  if (topAddr) {
    *id = ((DrvCmxDmaTransaction *)topAddr)->sw_id;
    return 0;
  }
  return -1;
}

// links a new list to the end of the current list and updates tail pointer
static inline void drvCmxDmaLinkToTail(DrvCmxDmaTransactionHnd *handle,
                                      uint32_t agent) {
  DrvCmxDmaTransaction *tail;

  tail = drvCmxDmaGetTail(agent);
  // link at the end of the current list
  tail->link_address = handle->head;
  // read back to ensure that previous write transaction is finished
  GET_REG_WORD_VAL((DrvCmxDmaTransaction*)&tail->link_address);
  // for the case when head and tail point to the same descriptor
  handle->tail->link_address = NULL;
  drvCmxDmaSaveTail(agent, handle->tail);
}

static void drvCmxDmaTryStart(DrvCmxDmaTransactionHnd *handle,
                              DrvCmxDmaOnCompletionCallback callback,
                              void *context, uint32_t agent) {
  uint32_t top_id = 0;
  DrvCmxDmaTransaction *tail;

  HglCmxDmaDisableAllChannels();
  HglCmxDmaWaitUntilLinkAgentIsntFetchingDescriptors(agent);
  if (SC_CMX_DMA_SHADOW_REG_FREE(agent)) {
    handle->current_id = handle->head->sw_id;
    if(!cmx_dma_control.irq_ena || (trQueue.count <= 1) ||
        cmx_dma_control.can_start_new) {
      handle->status = DRV_CMX_DMA_STARTED;
      drvCmxDmaStartTransfer(handle, callback, context);
    } else {
      handle->callback = callback;
      handle->context = context;
    }
  } else {
    drvCmxDmaGetTopSwId(agent, &top_id);
    handle->current_id = top_id;
    handle->callback = callback;
    handle->context = context;
    tail = drvCmxDmaGetTail(agent);
    if((tail && (HglCmxDmaGetId(tail) != cmx_dma_control.proc)) ||
        !cmx_dma_control.irq_ena || (trQueue.count <= 1) ||
        cmx_dma_control.can_start_new) {
      HglCmxDmaSetIrqFrequency(DRV_CMX_DMA_IRQ_NO(cmx_dma_control.proc),
                               handle->list_size);
      handle->status = DRV_CMX_DMA_STARTED;
      drvCmxDmaLinkToTail(handle, agent);
    }
  }
  HglCmxDmaEnableAllChannels();
}

int32_t DrvCmxDmaInitialize(DrvCmxDmaSetupStruct *config) {
  uint32_t irq_priority = DRV_CMX_DMA_UNDEF_IRQ_PRIO;

  if (cmx_dma_control.status == DRV_CMX_DMA_INITIALIZED) {
    return MYR_DRV_ALREADY_INITIALIZED;
  }

  // interrupts are enabled by default
  cmx_dma_control.irq_ena = 1;
  cmx_dma_control.proc = swcWhoAmI();
  cmx_dma_control.can_start_new = 1;
  drvCmxDmaGlobalStatusInit();
  HglCmxDmaSetArbitrationMode(HGL_CMX_DMA_ARB_MODE_PRIO);
  if (config) {
    // user provided configuration
    if ((config->agent < 0) || (config->agent > DRV_CMX_DMA_LA_NUM) ||
        (config->irq_enable &&
         (config->irq_priority > DRV_CMX_DMA_MAX_IRQ_PRIO)))
      // invalid parameters;
      return MYR_DRV_ERROR;
    cmx_dma_control.agent = config->agent;
    cmx_dma_control.irq_ena = config->irq_enable;
    if (cmx_dma_control.irq_ena)
      irq_priority = config->irq_priority;
  } else {
    // default configuration
    irq_priority = DRV_CMX_DMA_DEFAULT_IRQ_PRIO;
    if (cmx_dma_control.proc == PROCESS_LEON_OS) {
      cmx_dma_control.agent = DRV_CMX_DMA_LOS_DEFAULT_LA;
    } else {
      cmx_dma_control.agent = DRV_CMX_DMA_LRT_DEFAULT_LA;
    }
  }
  cmx_dma_control.irq_priority = irq_priority;
  HglCmxDmaEnable();
  HglCmxDmaAssignLinkAgentToChannel(DRV_CMX_DMA_LA_TO_CHAN_DEFAULT_MAP);
  if (cmx_dma_control.irq_ena) {
    drvCmxDmaInitTransactionQueue();
    cmx_dma_control.irq_mask = DRV_CMX_DMA_IRQ_MASK(cmx_dma_control.proc);
    HglCmxDmaIrqEnable(cmx_dma_control.irq_mask, cmx_dma_control.proc);
    DrvIcbIrqClear(IRQ_CMXDMA);
    DrvIcbSetupIrq(IRQ_CMXDMA, irq_priority, POS_LEVEL_INT, DrvCmxDmaIrqHandler);
    DrvIcbEnableIrq(IRQ_CMXDMA);
  }
  HglCmxDmaChannelEnable(cmx_dma_control.agent);

  cmx_dma_control.status = DRV_CMX_DMA_INITIALIZED;

  return MYR_DRV_SUCCESS;
}

uint32_t DrvCmxDmaGetIrqMask ()
{
    return cmx_dma_control.irq_mask;
}

int32_t DrvCmxDmaTransactionConfig(DrvCmxDmaTransaction *transaction,
                                   DrvCmxDmaConfParams *params) {
  if (!transaction || !params) return MYR_DRV_ERROR;
  if (HglCmxDmaSetPriority(transaction, params->priority) != MYR_DRV_SUCCESS)
    return MYR_DRV_ERROR;
  if (HglCmxDmaSetBurstSize(transaction, params->burst_size) != MYR_DRV_SUCCESS)
    return MYR_DRV_ERROR;
  return MYR_DRV_SUCCESS;
}

int32_t DrvCmxDmaCreateTransaction(DrvCmxDmaTransactionHnd *handle,
                                 DrvCmxDmaTransaction *transaction,
                                 uint8_t *src, uint8_t *dst, uint32_t size) {
  int32_t status = MYR_DRV_ERROR;

  if (transaction && handle && src && dst) {
    HglCmxDmaCreateTransaction(transaction, DRV_CMX_DMA_1D_TRANSACTION, src,
                               dst, size, 0, 0, 0, 0,
                               HGL_CMX_DMA_DEFAULT_NUM_PLANE,
                               HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
                               HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
                               DRV_CMX_DMA_DEFAULT_BURST_SIZE);
    HglCmxDmaSetPriority(transaction, DRV_CMX_DMA_DEFAULT_TRAN_PRIO);
    handle->list_size = 1;
    handle->head = transaction;
    handle->tail = transaction;
    status = MYR_DRV_SUCCESS;
  }
  return status;
}

int32_t DrvCmxDmaAddTransaction(DrvCmxDmaTransactionHnd *handle,
                                DrvCmxDmaTransaction *transaction,
                                uint8_t *src, uint8_t *dst, uint32_t size) {
  return DrvCmxDmaAddStrideTransaction(handle, transaction, src, dst, 0, 0,
                                       0, 0, size);
}

int32_t DrvCmxDmaCreateStrideTransaction(DrvCmxDmaTransactionHnd *handle,
                                         DrvCmxDmaTransaction *transaction,
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
    HglCmxDmaCreateTransaction(transaction, type, src, dst, size, src_width,
                               dst_width, src_stride, dst_stride,
                               HGL_CMX_DMA_DEFAULT_NUM_PLANE,
                               HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
                               HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
                               DRV_CMX_DMA_DEFAULT_BURST_SIZE);
    if (HglCmxDmaSetPriority(transaction, DRV_CMX_DMA_DEFAULT_TRAN_PRIO) !=
        MYR_DRV_SUCCESS)
      return MYR_DRV_ERROR;
    handle->list_size = 1;
    handle->head = transaction;
    handle->tail = transaction;
    status = MYR_DRV_SUCCESS;
  }
  return status;
}

int32_t DrvCmxDmaAddStrideTransaction(DrvCmxDmaTransactionHnd *handle,
                                      DrvCmxDmaTransaction *transaction,
                                      uint8_t *src, uint8_t *dst,
                                      uint32_t src_width, uint32_t dst_width,
                                      uint32_t src_stride, uint32_t dst_stride,
                                      uint32_t size) {
  int32_t type = DRV_CMX_DMA_1D_TRANSACTION;

  if (!handle || !transaction || !src || !dst) return MYR_DRV_ERROR;
  if ((src_width != 0) || (dst_width != 0) || (src_stride != 0) ||
      (dst_stride != 0)) {
    type = DRV_CMX_DMA_2D_TRANSACTION;
  }
  HglCmxDmaCreateTransaction(transaction, type, src, dst, size, src_width,
                             dst_width, src_stride, dst_stride,
                             HGL_CMX_DMA_DEFAULT_NUM_PLANE,
                             HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
                             HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
                             DRV_CMX_DMA_DEFAULT_BURST_SIZE);
  if (HglCmxDmaSetPriority(transaction, DRV_CMX_DMA_DEFAULT_TRAN_PRIO) !=
      MYR_DRV_SUCCESS)
    return MYR_DRV_ERROR;
  if (HglCmxDmaSetBurstSize(transaction, DRV_CMX_DMA_DEFAULT_BURST_SIZE) !=
      MYR_DRV_SUCCESS)
    return MYR_DRV_ERROR;

  handle->tail->link_address = transaction;
  handle->tail = transaction;
  handle->list_size++;

  return MYR_DRV_SUCCESS;
}

int32_t DrvCmxDmaCreate3DTransaction(DrvCmxDmaTransactionHnd *handle,
                                     DrvCmxDmaTransaction *transaction,
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

      // All parameters are set correctly, create the 3D transaction.
      // This is done by setting up the transaction type to "2D" transaction
      // and by setting the number of planes greater than zero.
      HglCmxDmaCreateTransaction(transaction, DRV_CMX_DMA_2D_TRANSACTION,
                                 src, dst, size, src_width,
                                 dst_width, src_stride, dst_stride,
                                 num_planes,
                                 src_plane_stride,
                                 dst_plane_stride,
                                 DRV_CMX_DMA_DEFAULT_BURST_SIZE);

      if (MYR_DRV_SUCCESS == HglCmxDmaSetPriority(transaction, DRV_CMX_DMA_DEFAULT_TRAN_PRIO)) {
        handle->list_size = 1;
        handle->head = transaction;
        handle->tail = transaction;
        status = MYR_DRV_SUCCESS;
      }
    }
  }

  return status;
}

int32_t DrvCmxDmaAdd3DTransaction(DrvCmxDmaTransactionHnd *handle,
                                  DrvCmxDmaTransaction *transaction,
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

      // All parameters are set correctly, create the 3D transaction.
      // This is done by setting up the transaction type to "2D" transaction
      // and by setting the number of planes greater than zero.
      HglCmxDmaCreateTransaction(transaction, DRV_CMX_DMA_2D_TRANSACTION,
                                 src, dst, size, src_width,
                                 dst_width, src_stride, dst_stride,
                                 num_planes,
                                 src_plane_stride,
                                 dst_plane_stride,
                                 DRV_CMX_DMA_DEFAULT_BURST_SIZE);

      if (MYR_DRV_SUCCESS == HglCmxDmaSetPriority(transaction, DRV_CMX_DMA_DEFAULT_TRAN_PRIO)) {
        handle->tail->link_address = transaction;
        handle->tail = transaction;
        handle->list_size++;
        status = MYR_DRV_SUCCESS;
      }
    }
  }

  return status;
}

int32_t DrvCmxDmaLinkTransactions(DrvCmxDmaTransactionHnd *handle,
                                  uint32_t list_cnt, ...) {
  va_list a_list;
  uint32_t i;
  DrvCmxDmaTransactionHnd *list_hnd;

  if (list_cnt == 0) return MYR_DRV_ERROR;

  va_start(a_list, list_cnt);

  list_hnd = va_arg(a_list, DrvCmxDmaTransactionHnd *);
  handle->head = list_hnd->head;
  handle->tail = list_hnd->tail;
  handle->list_size = list_hnd->list_size;
  for (i = 1; i < list_cnt; i++) {
    list_hnd = va_arg(a_list, DrvCmxDmaTransactionHnd *);
    handle->tail->link_address = list_hnd->head;
    handle->tail = list_hnd->tail;
    handle->list_size += list_hnd->list_size;
  }

  va_end(a_list);

  return MYR_DRV_SUCCESS;
}

int32_t DrvCmxDmaStartTransfer(DrvCmxDmaTransactionHnd *handle,
                               DrvCmxDmaOnCompletionCallback callback,
                               void *context) {
  uint32_t agent = cmx_dma_control.agent;

  DrvIcbDisableIrq(IRQ_CMXDMA);
  DRV_CMX_DMA_MUTEX_LOCK(kDrvCmxDmaMutexId[agent]);
  handle->status = DRV_CMX_DMA_NOT_STARTED;
  if (cmx_dma_control.irq_priority != DRV_CMX_DMA_UNDEF_IRQ_PRIO) {
    // try to save the current transaction
    // this is only needed when interrupts are enabled
    if (drvCmxDmaSaveTransaction(handle) != MYR_DRV_SUCCESS) {
      DRV_CMX_DMA_MUTEX_UNLOCK(kDrvCmxDmaMutexId[agent]);
      // transaction queue overflow
      return MYR_DRV_ERROR;
    }
  }
  drvCmxDmaListSetup(handle);
  drvCmxDmaTryStart(handle, callback, context, agent);
  DRV_CMX_DMA_MUTEX_UNLOCK(kDrvCmxDmaMutexId[agent]);
  DrvIcbEnableIrq(IRQ_CMXDMA);
  return MYR_DRV_SUCCESS;
}

int32_t DrvCmxDmaWaitTransaction(DrvCmxDmaTransactionHnd *handle) {
  DrvCmxDmaTransactionStatus t_status;
  int32_t status;

  do {
    status = DrvCmxDmaGetTransactionStatus(handle, &t_status);
  } while ((status == MYR_DRV_SUCCESS) && (t_status != DRV_CMX_DMA_FINISHED));

  return status;
}

static int32_t drvCmxDmaGetTransactionStatusSwId(
                            DrvCmxDmaTransactionHnd *handle,
                            DrvCmxDmaTransactionStatus *t_status) {
  int32_t status;
  uint32_t id = 0;
  uint32_t sw_id;

  if (!handle) return MYR_DRV_ERROR;
  status = drvCmxDmaGetTopSwId(cmx_dma_control.agent, &id);
  sw_id = handle->head->sw_id;

  if (status != 0) {
    if (HGL_CMX_DMA_AGENT_IS_BUSY(cmx_dma_control.agent))
      *t_status = DRV_CMX_DMA_PENDING;
    else
      *t_status = DRV_CMX_DMA_FINISHED;
  } else if (handle->current_id <= sw_id) {
    if (id <= sw_id)
        *t_status = DRV_CMX_DMA_PENDING;
      else
        *t_status = DRV_CMX_DMA_FINISHED;
  } else {
    // overflow case
    if ((id > sw_id) && (id < handle->current_id))
      *t_status = DRV_CMX_DMA_FINISHED;
    else
      *t_status = DRV_CMX_DMA_PENDING;
  }

  return MYR_DRV_SUCCESS;
}

static inline int32_t drvCmxDmaGetTransactionStatusIrq(
                              DrvCmxDmaTransactionHnd *handle,
                              DrvCmxDmaTransactionStatus *t_status) {
  if (handle->status == DRV_CMX_DMA_DONE)
    *t_status = DRV_CMX_DMA_FINISHED;
  else
    *t_status = DRV_CMX_DMA_PENDING;
  return MYR_DRV_SUCCESS;
}

int32_t DrvCmxDmaGetTransactionStatus(DrvCmxDmaTransactionHnd *handle,
                                      DrvCmxDmaTransactionStatus *t_status) {
  int32_t status;

  if (cmx_dma_control.irq_ena)
    status = drvCmxDmaGetTransactionStatusIrq(handle, t_status);
  else
    status = drvCmxDmaGetTransactionStatusSwId(handle, t_status);
  return status;
}

#endif // USE_CMX_DMA_NEW_DRIVER


///@}
