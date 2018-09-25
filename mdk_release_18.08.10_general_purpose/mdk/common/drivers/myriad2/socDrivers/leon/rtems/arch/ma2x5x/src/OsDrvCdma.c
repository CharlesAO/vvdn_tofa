///
/// @file OsDrvCdma.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvCdma
/// @{
/// @brief     CMX DMA API RTEMS Functions.
///


/// 1: Includes
/// ----------------------------------------------------------------------------

#include "OsDrvCdma.h"

#ifdef USE_CMX_DMA_NEW_DRIVER

#include <stdarg.h>
#include <assert.h>
#include <bsp.h>
#include <rtems.h>
#include "DrvRegUtils.h"
#include "swcWhoAmI.h"
#include "HglMutex.h"
#include "OsCommon.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define OS_DRV_CMX_DMA_MAX_QUEUED_TASKS 32
#define OS_DRV_CMX_DMA_MAX_IRQ_PRIO 14
#define OS_DRV_CMX_DMA_MAX_SW_ID UINT_MAX
// irq priority value when interrupts are disabled
#define OS_DRV_CMX_DMA_UNDEF_IRQ_PRIO -1
#define OS_DRV_CMX_DMA_TRANSFER_EVENT OSCOMMON_SYS_EVENT_CMX_DMA
#define OS_DRV_CMX_DMA_MTX_IRQ_LEVEL 8

// Lock utility by default wait forever to access the resource
#define OS_DRV_CMX_DMA_LOCK() \
  osDrvCmxDmaLockCriticalSection(true, RTEMS_NO_TIMEOUT);
// Unlock utility
#define OS_DRV_CMX_DMA_UNLOCK() osDrvCmxDmaUnlockCriticalSection()

typedef enum {
  OS_DRV_CMX_DMA_UNINITIALIZED = 0,
  OS_DRV_CMX_DMA_INITIALIZED,
} OsDrvCmxDmaStatus;

typedef enum {
  OS_DRV_CMX_DMA_STARTED = 0,
  OS_DRV_CMX_DMA_NOT_STARTED,
} OsDrvCmxDmaHndStatus;

typedef struct {
  OsDrvCmxDmaStatus status;
  OsDrvCmxDmaAgent agent;
  rtems_id access;
  uint8_t proc;
  int8_t irq_priority;
  volatile int8_t can_start_new;
} OsDrvCmxDmaControl;

typedef struct {
  OsDrvCmxDmaTransactionHnd *transaction[OS_DRV_CMX_DMA_MAX_QUEUED_TASKS];
  int16_t first;
  int16_t last;
  uint16_t count;
} TransactionQueue;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
static const uint32_t kDrvCmxDmaMutexId[DRV_CMX_DMA_LA_NUM] = {
    DRV_CMX_DMA_MUTEX_ID_LA_0, DRV_CMX_DMA_MUTEX_ID_LA_1,
    DRV_CMX_DMA_MUTEX_ID_LA_2, DRV_CMX_DMA_MUTEX_ID_LA_3};
static OsDrvCmxDmaControl cmx_dma_control;
static OsDrvCmxDmaStatusInfo *statusInfo[DRV_CMX_DMA_LA_NUM];
static TransactionQueue trQueue;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static void osDrvCmxDmaStartTransfer(OsDrvCmxDmaTransactionHnd *handle);
static void osDrvCmxDmaTryStart(OsDrvCmxDmaTransactionHnd *handle,
                                uint32_t agent);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
// Enter Critical Section
static rtems_status_code osDrvCmxDmaLockCriticalSection(uint32_t wait,
                                                        uint32_t wait_ticks) {
  rtems_status_code res;
  if (wait)
    res =
        rtems_semaphore_obtain(cmx_dma_control.access, RTEMS_WAIT, wait_ticks);
  else
    res = rtems_semaphore_obtain(cmx_dma_control.access, RTEMS_NO_WAIT, 0);
  return res;
}

// Abandon Critical Section
static rtems_status_code osDrvCmxDmaUnlockCriticalSection(void) {
  rtems_status_code res = rtems_semaphore_release(cmx_dma_control.access);
  return res;
}

/// @todo Replace this with a proper call once SysPage is released
static inline int32_t osDrvCmxDmaHwMutexObtain(uint32_t agent) {
  return HglMutexObtain(kDrvCmxDmaMutexId[agent], cmx_dma_control.proc);
}

/// @todo Replace this with a proper call once SysPage is released
static inline void osDrvCmxDmaHwMutexRelease(uint32_t agent) {
  HglMutexRelease(kDrvCmxDmaMutexId[agent], cmx_dma_control.proc);
}

static void osDrvCmxDmaGlobalStatusInit(void) {
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

static inline void osDrvCmxDmaInitTransactionQueue(TransactionQueue *queue) {
  queue->first = 0;
  queue->last = -1;
  queue->count = 0;
}

static inline int32_t osDrvCmxDmaSaveTransaction(
    TransactionQueue *queue,
    OsDrvCmxDmaTransactionHnd *handle) {
  uint32_t level;
  if (queue->count == OS_DRV_CMX_DMA_MAX_QUEUED_TASKS) {
    return OS_MYR_DRV_ERROR;
  }
  queue->last++;
  if (queue->last >= OS_DRV_CMX_DMA_MAX_QUEUED_TASKS)
    queue->last = 0;
  queue->transaction[queue->last] = handle;
  rtems_interrupt_disable(level);
  queue->count++;
  rtems_interrupt_enable(level);

  return OS_MYR_DRV_SUCCESS;
}

static inline OsDrvCmxDmaTransactionHnd *osDrvCmxDmaGetCurrentTransaction(
    TransactionQueue *queue) {
  OsDrvCmxDmaTransactionHnd *current;

  if (queue->count == 0)
    return NULL;
  current = queue->transaction[queue->first];
  queue->transaction[queue->first] = NULL;
  queue->first++;
  if (queue->first >= OS_DRV_CMX_DMA_MAX_QUEUED_TASKS)
    queue->first = 0;
  queue->count--;

  return current;
}

// get the next transaction without removing it form the queue
static inline OsDrvCmxDmaTransactionHnd *osDrvCmxDmaGetNextTransaction(
    TransactionQueue *queue) {
  return queue->transaction[queue->first];
}

static void osDrvCmxDmaIrqHandler(void *source) {
  UNUSED(source);
  uint32_t status;
  uint32_t mask;
  OsDrvCmxDmaTransactionHnd *next;
  OsDrvCmxDmaTransactionHnd *current;

  status = HglCmxDmaIrqGetStatus();
  mask = DRV_CMX_DMA_IRQ_MASK(cmx_dma_control.proc);
  if (status & mask) {
    HglCmxDmaIrqClearStatus(mask);
    cmx_dma_control.can_start_new = 1;
    current = osDrvCmxDmaGetCurrentTransaction(&trQueue);
    if (current)
      rtems_event_system_send(current->task_id, OS_DRV_CMX_DMA_TRANSFER_EVENT);

    // execute the next available transaction list
    next = osDrvCmxDmaGetNextTransaction(&trQueue);
    if (next && (next->status == OS_DRV_CMX_DMA_NOT_STARTED))
          osDrvCmxDmaTryStart(next, cmx_dma_control.agent);
  }
}

// sets the parameters for a list
static void osDrvCmxDmaListSetup(OsDrvCmxDmaTransactionHnd *handle) {
  OsDrvCmxDmaTransaction *list_ptr = handle->head;

  // irq trigger is needed only when interrupts are enabled
  while (list_ptr) {
    HglCmxDmaSetIrqTrigger(list_ptr,
                           DRV_CMX_DMA_IRQ_NO(cmx_dma_control.proc));
    list_ptr->sw_id = statusInfo[cmx_dma_control.agent]->sw_id;
    list_ptr = list_ptr->link_address;
  }
  statusInfo[cmx_dma_control.agent]->sw_id++;
  handle->task_id = rtems_task_self();
}

// returns the sw id of the transaction at the top of the list
static int32_t osDrvCmxDmaGetTopSwId(uint32_t agent, uint32_t *id) {
  uint32_t topAddr = HglCmxDmaGetTopAddress(agent);
  if (topAddr) {
    *id = ((OsDrvCmxDmaTransaction *)topAddr)->sw_id;
    return 0;
  }
  return -1;
}

static inline void osDrvCmxDmaSaveTail(uint32_t agent,
                                       OsDrvCmxDmaTransaction *tail) {
  OsDrvCmxDmaTransaction *tr;
  UNUSED(tr);
  statusInfo[agent]->tail = tail;
  // read back to ensure that previous write transaction is finished
  tr = *((OsDrvCmxDmaTransaction**)&statusInfo[agent]->tail);
}

static inline OsDrvCmxDmaTransaction *osDrvCmxDmaGetTail(uint32_t agent) {
  return statusInfo[agent]->tail;
}

static void osDrvCmxDmaStartTransfer(OsDrvCmxDmaTransactionHnd *handle) {
  cmx_dma_control.can_start_new = 0;
  osDrvCmxDmaSaveTail(cmx_dma_control.agent, handle->tail);
  HglCmxDmaSetIrqFrequency(DRV_CMX_DMA_IRQ_NO(cmx_dma_control.proc),
                           handle->list_size);
  HglCmxDmaStartTransfer(cmx_dma_control.agent, handle->head);
}

// links a new list to the end of the current list and updates tail pointer
static inline void osDrvCmxDmaLinkToTail(OsDrvCmxDmaTransactionHnd *handle,
                                         uint32_t agent) {
  OsDrvCmxDmaTransaction *tail;

  tail = osDrvCmxDmaGetTail(agent);
  // link at the end of the current list
  tail->link_address = handle->head;
  // read back to ensure that previous write transaction is finished
  GET_REG_WORD_VAL((OsDrvCmxDmaTransaction*)&tail->link_address);
  // for the case when head and tail point to the same descriptor
  handle->tail->link_address = NULL;
  osDrvCmxDmaSaveTail(agent, handle->tail);
}

// starts a new transfer o queue it for a later start
static void osDrvCmxDmaTryStart(OsDrvCmxDmaTransactionHnd *handle,
                                uint32_t agent) {
  uint32_t top_id = 0;

  // as a resolution of Bug 26749, we need to stop all channels while adding a task
  HglCmxDmaDisableAllChannels();
  HglCmxDmaWaitUntilLinkAgentIsntFetchingDescriptors(agent);
  if (SC_CMX_DMA_SHADOW_REG_FREE(agent)) {
    handle->current_id = handle->head->sw_id;
    handle->status = OS_DRV_CMX_DMA_STARTED;
    if (cmx_dma_control.can_start_new)
      osDrvCmxDmaStartTransfer(handle);
  } else {
    osDrvCmxDmaGetTopSwId(agent, &top_id);
    handle->current_id = top_id;
    if (HglCmxDmaGetId(osDrvCmxDmaGetTail(agent)) != cmx_dma_control.proc) {
      HglCmxDmaSetIrqFrequency(DRV_CMX_DMA_IRQ_NO(cmx_dma_control.proc),
                               handle->list_size);
      handle->status = OS_DRV_CMX_DMA_STARTED;
      osDrvCmxDmaLinkToTail(handle, agent);
    }
  }
  HglCmxDmaEnableAllChannels();
}

int32_t OsDrvCmxDmaInitialize(OsDrvCmxDmaSetupStruct *config) {
  int32_t status;

  if (cmx_dma_control.status == OS_DRV_CMX_DMA_INITIALIZED)
    return OS_MYR_DRV_ALREADY_INITIALIZED;

  // create the semaphore used for concurrent accesses protection
  status = rtems_semaphore_create(rtems_build_name('C', 'M', 'X', '0'), 1,
                                  RTEMS_SIMPLE_BINARY_SEMAPHORE, 0,
                                  &cmx_dma_control.access);
  if (status != RTEMS_SUCCESSFUL) return status;

  HglCmxDmaSetArbitrationMode(HGL_CMX_DMA_ARB_MODE_PRIO);
  cmx_dma_control.proc = swcWhoAmI();
  cmx_dma_control.can_start_new = 1;
  osDrvCmxDmaGlobalStatusInit();
  if (config) {
    // user provided configuration
    if ((config->agent < 0) || (config->agent > DRV_CMX_DMA_AGENT3) ||
        (config->irq_priority > OS_DRV_CMX_DMA_MAX_IRQ_PRIO))
      // invalid parameters;
      return OS_MYR_DRV_ERROR;
    cmx_dma_control.agent = config->agent;
    cmx_dma_control.irq_priority = config->irq_priority;
  } else {
    // default configuration
    cmx_dma_control.irq_priority = OS_DRV_CMX_DMA_DEFAULT_IRQ_PRIO;
    if (cmx_dma_control.proc == PROCESS_LEON_OS)
      cmx_dma_control.agent = DRV_CMX_DMA_LOS_DEFAULT_LA;
    else
      cmx_dma_control.agent = DRV_CMX_DMA_LRT_DEFAULT_LA;
  }
  osDrvCmxDmaInitTransactionQueue(&trQueue);
  HglCmxDmaEnable();
  HglCmxDmaAssignLinkAgentToChannel(DRV_CMX_DMA_LA_TO_CHAN_DEFAULT_MAP);
  HglCmxDmaIrqEnable(DRV_CMX_DMA_IRQ_MASK(cmx_dma_control.proc),
                     cmx_dma_control.proc);
  BSP_Clear_interrupt(IRQ_CMXDMA);
  BSP_Set_interrupt_type_priority(IRQ_CMXDMA, POS_EDGE_INT,
                                  cmx_dma_control.irq_priority);
  BSP_interrupt_register(
      IRQ_CMXDMA, NULL, osDrvCmxDmaIrqHandler, NULL);
  HglCmxDmaChannelEnable(cmx_dma_control.agent);

  cmx_dma_control.status = OS_DRV_CMX_DMA_INITIALIZED;

  return status;
}

int32_t OsDrvCmxDmaTransactionConfig(OsDrvCmxDmaTransaction *transaction,
                                     OsDrvCmxDmaConfParams *params) {
  if (!transaction || !params)
    return OS_MYR_DRV_ERROR;
  if (HglCmxDmaSetPriority(transaction, params->priority) !=
      OS_MYR_DRV_SUCCESS)
    return OS_MYR_DRV_ERROR;
  if (HglCmxDmaSetBurstSize(transaction, params->burst_size) !=
      OS_MYR_DRV_SUCCESS)
    return OS_MYR_DRV_ERROR;

  return OS_MYR_DRV_SUCCESS;
}

int32_t OsDrvCmxDmaCreateTransaction(OsDrvCmxDmaTransactionHnd *handle,
                                   OsDrvCmxDmaTransaction *transaction,
                                   uint8_t *src, uint8_t *dst, uint32_t size) {
  if (transaction && handle && src && dst) {
    HglCmxDmaCreateTransaction(transaction, DRV_CMX_DMA_1D_TRANSACTION, src,
        dst, size, 0, 0, 0, 0,
        HGL_CMX_DMA_DEFAULT_NUM_PLANE,
        HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
        HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
        OS_DRV_CMX_DMA_DEFAULT_BURST_SIZE);
    HglCmxDmaSetPriority(transaction, OS_DRV_CMX_DMA_DEFAULT_TRAN_PRIO);

    handle->list_size = 1;
    handle->head = transaction;
    handle->tail = transaction;
  } else {
    return OS_MYR_DRV_ERROR;
  }

  return OS_MYR_DRV_SUCCESS;
}

int32_t OsDrvCmxDmaAddTransaction(OsDrvCmxDmaTransactionHnd *handle,
                                  OsDrvCmxDmaTransaction *transaction,
                                  uint8_t *src, uint8_t *dst, uint32_t size) {
  return OsDrvCmxDmaAddStrideTransaction(handle, transaction, src, dst, 0,
                                         0, 0, 0, size);
}

int32_t OsDrvCmxDmaCreateStrideTransaction(
    OsDrvCmxDmaTransactionHnd *handle,
    OsDrvCmxDmaTransaction *transaction,
    uint8_t *src, uint8_t *dst,
    uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride,
    uint32_t size) {
  int32_t status = OS_MYR_DRV_ERROR;
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
        OS_DRV_CMX_DMA_DEFAULT_BURST_SIZE);
    if (HglCmxDmaSetPriority(transaction, OS_DRV_CMX_DMA_DEFAULT_TRAN_PRIO) !=
        OS_MYR_DRV_SUCCESS)
      return OS_MYR_DRV_ERROR;
    status = OS_DRV_CMX_DMA_LOCK();
    if (status == RTEMS_SUCCESSFUL) {
      handle->list_size = 1;
      handle->head = transaction;
      handle->tail = transaction;
      OS_DRV_CMX_DMA_UNLOCK();
    }
  }
  return status;
}

int32_t OsDrvCmxDmaAddStrideTransaction(OsDrvCmxDmaTransactionHnd *handle,
                                        OsDrvCmxDmaTransaction *transaction,
                                        uint8_t *src, uint8_t *dst,
                                        uint32_t src_width, uint32_t dst_width,
                                        uint32_t src_stride,
                                        uint32_t dst_stride, uint32_t size) {
  int32_t type = DRV_CMX_DMA_1D_TRANSACTION;
  int32_t status;

  if (!handle || !transaction || !src || !dst) return OS_MYR_DRV_ERROR;
  if ((src_width != 0) || (dst_width != 0) || (src_stride != 0) ||
      (dst_stride != 0)) {
    type = DRV_CMX_DMA_2D_TRANSACTION;
  }
  HglCmxDmaCreateTransaction(transaction, type, src, dst, size, src_width,
                             dst_width, src_stride, dst_stride,
                             HGL_CMX_DMA_DEFAULT_NUM_PLANE,
                             HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
                             HGL_CMX_DMA_DEFAULT_PLANE_STRIDE,
                             OS_DRV_CMX_DMA_DEFAULT_BURST_SIZE);
  if (HglCmxDmaSetPriority(transaction, OS_DRV_CMX_DMA_DEFAULT_TRAN_PRIO) !=
      OS_MYR_DRV_SUCCESS)
    return OS_MYR_DRV_ERROR;

  status = OS_DRV_CMX_DMA_LOCK();
  if (status == RTEMS_SUCCESSFUL) {
    handle->tail->link_address = transaction;
    handle->tail = transaction;
    handle->list_size++;
    OS_DRV_CMX_DMA_UNLOCK();
  }

  return OS_MYR_DRV_SUCCESS;
}

int32_t OsDrvCmxDmaCreate3DTransaction(
    OsDrvCmxDmaTransactionHnd *handle,
    OsDrvCmxDmaTransaction *transaction,
    uint8_t *src, uint8_t *dst,
    uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride,
    uint32_t num_planes, uint32_t src_plane_stride,
    uint32_t dst_plane_stride, uint32_t size) {
  int32_t status = OS_MYR_DRV_ERROR;

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
                                   OS_DRV_CMX_DMA_DEFAULT_BURST_SIZE);

      if (OS_MYR_DRV_SUCCESS == HglCmxDmaSetPriority(transaction,
                                  OS_DRV_CMX_DMA_DEFAULT_TRAN_PRIO)) {
        status = OS_DRV_CMX_DMA_LOCK();
        if (status == RTEMS_SUCCESSFUL) {
          handle->list_size = 1;
          handle->head = transaction;
          handle->tail = transaction;
          OS_DRV_CMX_DMA_UNLOCK();
          status = OS_MYR_DRV_SUCCESS;
        }
      }
    }
  }

  return status;
}

int32_t OsDrvCmxDmaAdd3DTransaction(
    OsDrvCmxDmaTransactionHnd *handle,
    OsDrvCmxDmaTransaction *newTransaction,
    uint8_t *src, uint8_t *dst,
    uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride,
    uint32_t num_planes, uint32_t src_plane_stride,
    uint32_t dst_plane_stride, uint32_t size) {
  int32_t status = OS_MYR_DRV_ERROR;

  if (newTransaction && handle && src && dst &&
     (num_planes >= MIN_NUM_PLANES) && (num_planes <= MAX_NUM_PLANES)) {
    if ((src_width != 0) || (dst_width != 0) ||
        (src_stride != 0) || (dst_stride != 0) ||
        (src_plane_stride != 0) || (dst_plane_stride != 0)) {
        // In 3D mode, the "num_planes" field holds the number of planes minus one.
        num_planes--;

        // All parameters are set correctly, create the 3D transaction.
        // This is done by setting up the transaction type to "2D" transaction
        // and by setting the number of planes greater than zero.
        HglCmxDmaCreateTransaction(newTransaction, DRV_CMX_DMA_2D_TRANSACTION,
                                   src, dst, size, src_width,
                                   dst_width, src_stride, dst_stride,
                                   num_planes,
                                   src_plane_stride,
                                   dst_plane_stride,
                                   OS_DRV_CMX_DMA_DEFAULT_BURST_SIZE);

      if (OS_MYR_DRV_SUCCESS == HglCmxDmaSetPriority(newTransaction,
                                  OS_DRV_CMX_DMA_DEFAULT_TRAN_PRIO)) {
        status = OS_DRV_CMX_DMA_LOCK();
        if (status == RTEMS_SUCCESSFUL) {
          handle->tail->link_address = newTransaction;
          handle->tail = newTransaction;
          handle->list_size++;
          OS_DRV_CMX_DMA_UNLOCK();
          status = OS_MYR_DRV_SUCCESS;
        }
      }
    }
  }

  return status;
}

int32_t OsDrvCmxDmaLinkTransactions(OsDrvCmxDmaTransactionHnd *handle,
                                    uint32_t list_cnt, ...) {
  va_list a_list;
  uint32_t i;
  int32_t status;
  OsDrvCmxDmaTransactionHnd *list_hnd;

  if (list_cnt == 0) return OS_MYR_DRV_ERROR;

  va_start(a_list, list_cnt);

  list_hnd = va_arg(a_list, OsDrvCmxDmaTransactionHnd *);
  status = OS_DRV_CMX_DMA_LOCK();
  if (status == RTEMS_SUCCESSFUL) {
    handle->head = list_hnd->head;
    handle->tail = list_hnd->tail;
    handle->list_size = list_hnd->list_size;
    for (i = 1; i < list_cnt; i++) {
      list_hnd = va_arg(a_list, OsDrvCmxDmaTransactionHnd *);
      handle->tail->link_address = list_hnd->head;
      handle->tail = list_hnd->tail;
      handle->list_size += list_hnd->list_size;
    }
    OS_DRV_CMX_DMA_UNLOCK();
  }

  va_end(a_list);

  return status;
}

int32_t OsDrvCmxDmaStartTransfer(OsDrvCmxDmaTransactionHnd *handle,
                                 uint32_t wait) {
  rtems_event_set evt_out;
  int32_t status = OS_MYR_DRV_ERROR;

  status = OS_DRV_CMX_DMA_LOCK();
  if (status != OS_MYR_DRV_SUCCESS) return status;
  BSP_Mask_interrupt(IRQ_CMXDMA);
  status = osDrvCmxDmaHwMutexObtain(cmx_dma_control.agent);
  if (status != OS_MYR_DRV_SUCCESS) return status;

  if (cmx_dma_control.status == OS_DRV_CMX_DMA_INITIALIZED) {
    handle->status = OS_DRV_CMX_DMA_NOT_STARTED;
    if (osDrvCmxDmaSaveTransaction(&trQueue, handle) != 0) {
      // transaction queue overflow
      osDrvCmxDmaHwMutexRelease(cmx_dma_control.agent);
      return OS_MYR_DRV_ERROR;
    }
    osDrvCmxDmaListSetup(handle);
    osDrvCmxDmaTryStart(handle, cmx_dma_control.agent);
  } else {
    status = OS_MYR_DRV_NOTOPENED;
  }
  osDrvCmxDmaHwMutexRelease(cmx_dma_control.agent);
  BSP_Unmask_interrupt(IRQ_CMXDMA);
  OS_DRV_CMX_DMA_UNLOCK();
  if (wait) {
    status = rtems_event_system_receive(OS_DRV_CMX_DMA_TRANSFER_EVENT,
                                        RTEMS_WAIT | RTEMS_EVENT_ALL,
                                        RTEMS_NO_TIMEOUT, &evt_out);
  }
  return status;
}

int32_t OsDrvCmxDmaGetTransactionStatus(
    OsDrvCmxDmaTransactionHnd *handle, OsDrvCmxDmaTransactionStatus *t_status) {
  int32_t status;
  uint32_t id = 0;
  uint32_t sw_id;

  if (!handle) return OS_MYR_DRV_ERROR;
  status = osDrvCmxDmaGetTopSwId(cmx_dma_control.agent, &id);
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

  return OS_MYR_DRV_SUCCESS;
}

#endif // USE_CMX_DMA_NEW_DRIVER


///@}

