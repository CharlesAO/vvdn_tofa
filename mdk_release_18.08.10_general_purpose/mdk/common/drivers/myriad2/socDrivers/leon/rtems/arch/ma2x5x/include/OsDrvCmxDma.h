///
/// @file OsDrvCmxDma.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvCmxDma OsCmxDma Driver
/// @{
/// @brief Functions and macros for the RTEMS CmxDma Driver
///

#ifndef OSDRVCMXDMA_H_
#define OSDRVCMXDMA_H_

#ifndef USE_CMX_DMA_NEW_DRIVER

#include <DrvCmxDma.h>
#include <OsCommon.h>

#ifdef __cplusplus
extern "C" {
#endif

// Default source/destination plane stride and number of plane
// value used when calling the drvCmxDmaNewTransaction function
// for 1D or 2D DMA transaction.
#define CMX_DMA_DEFAULT_NUM_PLANE (0)
#define CMX_DMA_DEFAULT_PLANE_STRIDE (0)

// Valid range for the number of planes.
#define MIN_NUM_PLANES (1)
#define MAX_NUM_PLANES (256)


///@brief Initialize the CMXDMA driver and perform initial setup with default configuration parameters
///@return
/// - OS_MYR_DRV_ERROR
/// - OS_MYR_DRV_SUCCESS
/// - OS_MYR_DRV_ALREADY_INITIALIZED
int OsDrvCmxDmaInitDefault(void);
///@param interruptLevel The interrupt level to be set for CMXDMA IRQ signal
///@param mutexUsed The hardware mutex to be used in order to have synchronized access to DMA with other processors. Shaves also should share the same mutex with Leons.
///@param interruptOffset The offset from where to start mapping interrupt bits to link agents
///@param nrOfAgents Number of Link Agents to use
///@param startingFrom The number of first Link Agent to start using
///@return
/// - OS_MYR_DRV_ERROR
/// - OS_MYR_DRV_SUCCESS
/// - OS_MYR_DRV_ALREADY_INITIALIZED
int OsDrvCmxDmaInit(u32 interruptLevel, u32 interruptOffset, u32 nrOfAgents, u32 startingFrom);

///@brief Initialize a requester ID which will be used to properly initialize and distinguish single tasks or groups of tasks for different threads.
///@param priority The priority that will be assigned to all the tasks created using the returned ID
///@param reqId A new requester ID
///@return
/// - OS_MYR_DRV_ERROR
/// - OS_MYR_DRV_SUCCESS
/// - OS_MYR_DRV_NOTOPENED
int OsDrvCmxDmaInitRequester(int priority, dmaRequesterId *reqId);

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
///@return
/// - OS_MYR_DRV_SUCCESS
///
int OsDrvCmxDmaCreateTransactionFullOptions(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 SrcLineWidth, u32 DstLineWidth, s32 SrcStride, s32 DstStride);

///@brief Initialize a new CMXDMA task structure which can be used to realize a simple DMA data transfer
///@brief The transaction type is a 1D transaction
///@brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
///@param ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
///@param NewTransaction Pointer to user-allocated space for a new task structure
///@param Src Source address of data transfer
///@param Dst Destination address of data transfer
///@param ByteLength Size(in bytes) of the transfer
///@return
/// - OS_MYR_DRV_SUCCESS
///
int OsDrvCmxDmaCreateTransaction(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength);

///@brief Initialize a new CMXDMA task structure which can be used to realize a DMA data transfer using source stride only
///@brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
///@param ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
///@param NewTransaction Pointer to user-allocated space for a new task structure
///@param Src Source address of data transfer
///@param Dst Destination address of data transfer
///@param ByteLength Size(in bytes) of the transfer
///@param LineWidth Source line width
///@param SrcStride Source stride
///@return
/// - OS_MYR_DRV_SUCCESS
///
int OsDrvCmxDmaCreateTransactionSrcStride(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 LineWidth, s32 SrcStride);

///@brief Initialize a new CMXDMA task structure which can be used to realize a DMA data transfer using destination stride only
///@brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
///@param ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
///@param NewTransaction Pointer to user-allocated space for a new task structure
///@param Src Source address of data transfer
///@param Dst Destination address of data transfer
///@param ByteLength Size(in bytes) of the transfer
///@param LineWidth Destination line width
///@param DstStride Destination stride
///@return
/// - OS_MYR_DRV_SUCCESS
///
int OsDrvCmxDmaCreateTransactionDstStride(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 LineWidth, s32 DstStride);

///@brief Initialize a new CMXDMA task structure which can be used to realize a 3D DMA data transfer
///@brief The function returns a handle to the new transaction.
///@param transaction the new 3D transaction to be created.
///@param src source address for the transaction.
///@param dst destination address for the transaction.
///@param SrcWidth line width for source in bytes.
///@param DstWidth line width for destination in bytes.
///@param SrcStride stride size for source, defined as the size in bytes
///       from the start of a line to the start of the following line.
///@param DstStride stride size for destination, defined as the size in
///       bytes from the start of a line to the start of the following line.
///@param NumPlanes number of planes of data to be transfered. The value
///       needs to be greater than zero for 3D transaction.
///@param SrcPlaneStride plane stride size for source, defined as the
///       size in bytes from the start of a plane to the start of the
///       following plane.
///@param DstPlaneStride plane stride size for destination, defined as
///       the size in bytes from the start of a plane to the start of the
///       following plane.
///@param size transaction size in bytes.
///@return
///       OS_MYR_DRV_SUCCESS the new transaction was successfully created
///       OS_MYR_DRV_ERROR the new transaction could not be created
///
int OsDrvCmxDmaCreate3DTransaction(dmaRequesterId ReqId,
                                   dmaTransactionList *NewTransaction,
                                   u8 *Src,
                                   u8 *Dst,
                                   u32 ByteLength,
                                   u32 SrcWidth,
                                   u32 DstWidth,
                                   u32 SrcStride,
                                   u32 DstStride,
                                   u32 NumPlanes,
                                   u32 SrcPlaneStride,
                                   u32 DstPlaneStride);

///@brief Link multiple tasks in a single linked list. Please note that this function allows linking just for single tasks.
///@brief One can not link together in this way two or more linked lists of tasks in order to form a single list.
///@param listHead Pointer to the structure which will represent the start of linked task list.
///@param nmbTasks Number of tasks to be linked to list head
///@param ... Pointers to the tasks to be linked. The structures passed here will be linked to listHead from left to right, in order of their placement on function call.
///@return
/// - OS_MYR_DRV_ERROR
/// - OS_MYR_DRV_SUCCESS
/// - OS_MYR_DRV_NOTOPENED
int OsDrvCmxDmaLinkTasks(dmaTransactionList* listHead, u32 nmbTasks, ...);

///@brief Set-up CMXDMA to execute the given list of tasks.
///@brief Please note if there is heavy use of CMXDMA, the task list won't start immediately,
///@brief it will be put in a waiting queue until CMXDMA will become available to execute the current task.
///@param ListPtr Pointer to the task or list of tasks to be executed
///@param status The way the task was started:
/// - CDMA_TASK_STARTED - Tasks have been submitted directly to CMXDMA and are executing now
/// - CDMA_TASK_LINKED - Tasks are linked to an executing list of tasks
/// - CDMA_TASK_QUEUED - Tasks have been added to a waiting queue and are pending execution
/// - CDMA_TASK_QUEUE_FULL - Waiting queue is full. Increaze the value of WAITING_QUEUE_SIZE define in order to have a bigger queue
///@return
/// - OS_MYR_DRV_ERROR
/// - OS_MYR_DRV_SUCCESS
/// - OS_MYR_DRV_NOTOPENED
int OsDrvCmxDmaStartListTask(dmaTransactionList* ListPtr, int *status);

///@brief Wait in a blocking way for a given task to finish.
///@param ListPtr Pointer to the task to be waited
///@return
/// - OS_MYR_DRV_ERROR
/// - OS_MYR_DRV_SUCCESS
/// - OS_MYR_DRV_NOTOPENED
int OsDrvCmxDmaWaitTask(dmaTransactionList* ListPtr);

///Check whether a task finished it's execution or is still running/pending
///@param ListPtr Pointer to the task to be checked
///@return
/// - OS_MYR_DRV_ERROR
/// - OS_MYR_DRV_SUCCESS
/// - OS_MYR_DRV_NOTOPENED
int OsDrvCmxDmaIsTaskFinished(dmaTransactionList* ListPtr);

/// @brief Releases the driver resources
int OsDrvCmxDmaClose(void);

/// @brief CMXDMA task structure initialization extension, allowing the user to set a custom burst length
/// @param[in]     Type           Transaction type (DMA_1D_TRANSACTION or DMA_2D_TRANSACTION)
/// @param[in]     ReqId          A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param[in,out] NewTransaction Pointer to user-allocated space for a new task structure
/// @param[in]     Src            Source address of data transfer
/// @param[in]     Dst            Destination address of data transfer
/// @param[in]     ByteLength     Size(in bytes) of the transfer
/// @param[in]     SrcLineWidth   Source line width
/// @param[in]     DstLineWidth   Destination line width
/// @param[in]     SrcStride      Source stride
/// @param[in]     DstStride      Destination stride
/// @param[in]     BurstLength    Number of transactions in a burst (1 - 16)
/// @return
/// - OS_MYR_DRV_ERROR
/// - OS_MYR_DRV_SUCCESS
///
int OsDrvCmxDmaCreateTransactionExt(u32 Type, dmaRequesterId ReqId, dmaTransactionList *NewTransaction,
                                    u8* Src, u8* Dst, u32 ByteLength, u32 SrcLineWidth, u32 DstLineWidth,
                                    s32 SrcStride, s32 DstStride, u8 BurstLength);

#ifdef __cplusplus
}
#endif

#endif // USE_CMX_DMA_NEW_DRIVER

#endif /* OSDRVCMXDMA_H_ */

