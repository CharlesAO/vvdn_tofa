///
/// @file DrvCmxDma.h
/// 
/// 
/// @defgroup DrvCmxDma CmxDma Driver
/// @{
///

#ifndef _DRVCMXDMA_H_
#define _DRVCMXDMA_H_

#ifndef USE_CMX_DMA_NEW_DRIVER
// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvCmxDmaDefines.h"
#include <DrvRegUtilsDefines.h>
#include <DrvMutex.h>
#include <swcWhoAmI.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

// 2: Defines
// ----------------------------------------------------------------------------

//Pick a mutex implementation
#if defined(CDMA_USE_FAST_MTX)
#define DRV_MUTEX_LOCK    DrvFastMutexLock
#define DRV_MUTEX_UNLOCK  DrvFastMutexUnlock

#elif defined(CDMA_USE_EMPTY_MTX)
#define DRV_MUTEX_LOCK(x)
#define DRV_MUTEX_UNLOCK(x)

#else
#define DRV_MUTEX_LOCK    DrvMutexLock    //Default
#define DRV_MUTEX_UNLOCK  DrvMutexUnlock
#endif

//Faster alternative to: x = ++x % LIMIT
#define MODULO_INC(x, LIMIT) \
  x++;                       \
  if(x >= LIMIT)             \
     x = 0;

//Local macros
#define DMA_CFG_SET(ReqId)  DMA_PRIORITY(ReqId & 3)            |\
                            DMA_BURST_LENGTH(0xF)              |\
                            DMA_TRANSACTION_ID(ReqId >> 2)     |\
                            DMA_INTERRUPT_TRIGGER(0)           |\
                            DMA_ENABLE_INTERRUPTS              |\
                            DMA_SKIP_NR(TASK_SKIP_DESCRIPTOR)

// Default source/destination plane stride and number of plane
// value used when calling the drvCmxDmaNewTransaction function
// for 1D or 2D DMA transaction.
#define CMX_DMA_DEFAULT_NUM_PLANE (0)
#define CMX_DMA_DEFAULT_PLANE_STRIDE (0)

// Valid range for the number of planes.
#define MIN_NUM_PLANES (1)
#define MAX_NUM_PLANES (256)

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
extern int intOffset;
extern int usedAgentsNr;
extern int startFrom;
extern drvStat_t cmxDmaStatus;
extern u32 **tail[4];

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
///@brief Initialize a requester ID which will be used to properly initialize and distinguish single tasks or groups of tasks.
///@param priority The priority that will be assigned to all the tasks created using the returned ID
///@return a new requester ID
dmaRequesterId DrvCmxDmaInitRequester(int priority);

///@brief Initialize a requester ID which will be used to properly initialize and distinguish single tasks or groups of tasks.
///@param priority The priority that will be assigned to all the tasks created using the returned ID
///@param agent The link agent to be used by CMXDMA while performing transfers initiated with generated requester ID.
///@return a new requester ID
dmaRequesterId DrvCmxDmaInitRequesterOnAgent(int priority, int agent);

///Setup the LA configuration to be used by CMXDMA. Do not use this to initialize the driver. use #DrvCmxDmaInit instead
int DrvCmxDmaSetup(u32 interruptOffset, u32 nrOfAgents, u32 startingFrom);

///@brief Initialize the CMXDMA driver and perform initial setup
///@param intLevel The interrupt level to be set for CMXDMA IRQ signal
///@param mutexUsed The hardware mutex to be used in order to have synchronized access to DMA with other processors. Shaves also should share the same mutex with Leons.
///@param interruptOffset The offset from where to start mapping interrupt bits to link agents
///@param nrOfAgents Number of Link Agents to use
///@param startingFrom The number of first Link Agent to start using
///@return
/// - CMX_DMA_DRV_SUCCESS - Driver successuflly initialized
/// - CMX_DMA_DRV_ALREADY_INITIALIZED - Driver already initialized
int DrvCmxDmaInit(u32 intLevel, u32 interruptOffset, u32 nrOfAgents, u32 startingFrom);

///@brief Initialize the CMXDMA driver and perform initial setup with default configuration parameters
///@return
/// - CMX_DMA_DRV_SUCCESS - Driver successuflly initialized
/// - CMX_DMA_DRV_ALREADY_INITIALIZED - Driver already initialized
int DrvCmxDmaInitDefault( void );

///@brief Descriptor initialization; user is to set addresses/params at application level
///@param ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
///@param Type : DMA_1D_TRANSACTION or DMA_2D_TRANSACTION
///@param NewTransaction Pointer to user-allocated space for a new task structure
void DrvCmxDmaTransactionBriefInit(dmaRequesterId ReqId, u32 Type, dmaTransactionList *NewTransaction);

///@brief Initialize a new CMXDMA task structure which can be used to realize a DMA data transfer using source and destination strides
///@brief The transaction type is a 2D transaction
///@brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
///@param ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
///@param NewTransaction Pointer to user-allocated space for a new task structure
///@param Src Source address of data transfer
///@param Dst Destination address of data transfer
///@param ByteLength Size(in bytes) of the transfer
///@param SrcLineWidth Source line width
///@param DstLineWidth Destination line width
///@param SrcStride Source stride
///@param DstStride Destination stride
///@return Pointer to initialized CMXDMA structure
dmaTransactionList* DrvCmxDmaCreateTransactionFullOptions(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 SrcLineWidth, u32 DstLineWidth, s32 SrcStride, s32 DstStride);

///@brief Initialize a new CMXDMA task structure which can be used to realize a simple DMA data transfer
///@brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
///@param ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
///@param NewTransaction Pointer to user-allocated space for a new task structure
///@param Src Source address of data transfer
///@param Dst Destination address of data transfer
///@param ByteLength Size(in bytes) of the transfer
///@return Pointer to initialized CMXDMA structure
dmaTransactionList* DrvCmxDmaCreateTransaction(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength);

///@brief Initialize a new CMXDMA task structure which can be used to realize a DMA data transfer using source stride only
///@brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
///@param ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
///@param NewTransaction Pointer to user-allocated space for a new task structure
///@param Src Source address of data transfer
///@param Dst Destination address of data transfer
///@param ByteLength Size(in bytes) of the transfer
///@param LineWidth Source line width
///@param SrcStride Source stride
///@return Pointer to initialized CMXDMA structure
dmaTransactionList* DrvCmxDmaCreateTransactionSrcStride(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 LineWidth, s32 SrcStride);

///@brief Initialize a new CMXDMA task structure which can be used to realize a DMA data transfer using destination stride only
///@brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
///@param ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
///@param NewTransaction Pointer to user-allocated space for a new task structure
///@param Src Source address of data transfer
///@param Dst Destination address of data transfer
///@param ByteLength Size(in bytes) of the transfer
///@param LineWidth Destination line width
///@param DstStride Destination stride
///@return Pointer to initialized CMXDMA structure
dmaTransactionList* DrvCmxDmaCreateTransactionDstStride(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 LineWidth, s32 DstStride);

///@brief Initialize a new CMXDMA task structure which can be used to realize a 3D DMA data transfer
///@brief The function returns a handle to the new transaction.
///@param transaction the new 3D transaction to be created.
///@param src source address for the transaction.
///@param dst destination address for the transaction.
///@param src_width line width for source in bytes.
///@param dst_width line width for destination in bytes.
///@param src_stride stride size for source, defined as the size in bytes
///       from the start of a line to the start of the following line.
///@param dst_stride stride size for destination, defined as the size in
///       bytes from the start of a line to the start of the following line.
///@param num_planes number of planes of data to be transfered. The value
///       needs to be greater than zero for 3D transaction.
///@param src_plane_stride plane stride size for source, defined as the
///       size in bytes from the start of a plane to the start of the
///       following plane.
///@param dst_plane_stride plane stride size for destination, defined as
///       the size in bytes from the start of a plane to the start of the
///       following plane.
///@param size transaction size in bytes.
///
///@return Pointer to initialized CMXDMA structure
///
dmaTransactionList* DrvCmxDmaCreate3DTransaction(dmaRequesterId ReqId,
                                                 dmaTransactionList *NewTransaction,
                                                 u8 *Src, u8 *Dst, u32 ByteLength,
                                                 u32 SrcWidth, u32 DstWidth,
                                                 u32 SrcStride, u32 DstStride,
                                                 u32 NumPlanes,
                                                 u32 SrcPlaneStride,
                                                 u32 DstPlaneStride);

///@brief Link multiple tasks in a single linked list. Please note that this function allows linking just for single tasks.
///@brief One can not link together in this way two or more linked lists of tasks in order to form a single list.
///@param listHead Pointer to the structure which will represent the start of linked task list.
///@param nmbTasks Number of tasks to be linked to list head
///@param ... Pointers to the tasks to be linked. The structures passed here will be linked to listHead from left to right, in order of their placement on function call.
///@return
/// - CMX_DMA_DRV_SUCCESS
/// - CMX_DMA_DRV_NOT_INITIALIZED
int DrvCmxDmaLinkTasks(dmaTransactionList* listHead, u32 nmbTasks, ...);

void DrvCmxDmaIrqHandlerFunc(u32 source);
///@brief Set-up CMXDMA to execute the given list of tasks.
///@brief Please note if there is heavy use of CMXDMA, the task list won't start immediately,
///@brief it will be put in a waiting queue until CMXDMA will become available to execute the current task.
///@param ListPtr Pointer to the task or list of tasks to be executed
///@return
/// - CDMA_TASK_STARTED - Tasks have been submitted directly to CMXDMA and are executing now
/// - CDMA_TASK_LINKED  - Tasks are linked to an executing list of tasks
/// - CDMA_TASK_QUEUED - Tasks have been added to a waiting queue and are pending execution
/// - CDMA_TASK_QUEUE_FULL - Waiting queue is full. Increaze the value of WAITING_QUEUE_SIZE define in order to have a bigger queue
int DrvCmxDmaStartListTask(dmaTransactionList* ListPtr);

///@brief Set-up CMXDMA to execute a list of tasks similar to  dmaStartListTask.
///@brief The main difference is that the user can also specify a callback function and a pointer to some specific data.
///@brief The callback function will be called once the given tasks will finish their execution, and will receive as parameters
///@brief the pointer to that user-specific data and another pointer to ListPtr parameter
///@param ListPtr Pointer to the task or list of tasks to be executed
///@param function Callback function to be called when the tasks will finish the execution
///@param userContext A pointer to some information the user wants to pass to callback function
///@return
/// - CDMA_TASK_STARTED - Tasks have been submitted directly to CMXDMA and are executing now
/// - CDMA_TASK_LINKED  - Tasks are linked to an executing list of tasks
/// - CDMA_TASK_QUEUED - Tasks have been added to a waiting queue and are pending execution
/// - CDMA_TASK_QUEUE_FULL - Waiting queue is full. Increaze the value of WAITING_QUEUE_SIZE define in order to have a bigger queue
int DrvCmxDmaStartTaskAsync(dmaTransactionList* ListPtr, dmaIrqHandler function, void *userContext);

///@brief Wait in a blocking way for a given task to finish.
///@param ListPtr Pointer to the task to be waited
///@return
/// - CMX_DMA_DRV_SUCCESS
/// - CMX_DMA_DRV_NOT_INITIALIZED
int DrvCmxDmaWaitTask(dmaTransactionList* ListPtr);

///@brief Check whether a task finished it's execution or is still running/pending
///@param ListPtr Pointer to the task to be checked
///@return
/// - CDMA_TASK_STARTED - Task is still executed/pending
/// - CDMA_TASK_FINISHED - Task finished it's execution
/// - CMX_DMA_DRV_NOT_INITIALIZED
int DrvCmxDmaIsTaskFinished(dmaTransactionList* ListPtr);


/// @brief CMXDMA task structure initialization extension, allowing the user to set a custom burst length
/// @brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
/// @param[in] Type           Transaction type
/// @param[in] ReqId          A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param[in] NewTransaction Pointer to user-allocated space for a new task structure
/// @param[in] Src            Source address of data transfer
/// @param[in] Dst            Destination address of data transfer
/// @param[in] ByteLength     Size(in bytes) of the transfer
/// @param[in] SrcLineWidth   Source line width
/// @param[in] DstLineWidth   Destination line width
/// @param[in] SrcStride      Source stride
/// @param[in] DstStride      Destination stride
/// @param[in] BurstLength    Number of transactions in a burst (1 - 16)
/// @return Pointer to initialized CMXDMA structure
///
dmaTransactionList* DrvCmxDmaCreateTransactionExt(u32 Type, dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction, u8* Src, u8* Dst,
                                                  u32 ByteLength, u32 SrcLineWidth, u32 DstLineWidth,
                                                  s32 SrcStride, s32 DstStride, u8 BurstLength);

#ifdef __cplusplus
}
#endif

#endif // USE_CMX_DMA_NEW_DRIVER

#endif // _DRVCMXDMA_H_

///@}
