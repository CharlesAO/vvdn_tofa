///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup swcCdmaApi CMXDMA API
/// @ingroup  swcCdmaApi
/// @{
/// @brief CMXDMA driver common API
///
/// This driver lets you perform fast data transfers using CMXDMA hardware
///


#ifndef _SWC_CDMA_COMMON_H_
#define _SWC_CDMA_COMMON_H_


#include "swcCdmaCommonDefines.h"

#ifdef __cplusplus
extern "C" {
#endif


/// @brief Initialize a requester ID which will be used to properly initialize and distinguish single tasks or groups of tasks.
/// @param[in] priority - The priority that will be assigned to all the tasks created using the returned ID
/// @return a new requester ID
///
dmaRequesterId dmaInitRequester(int priority);

/// @brief Initialize a new CMXDMA task structure which can be used to realize a DMA data transfer using source and destination strides.
/// @brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
/// @param[in] ReqId          - A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param[in] NewTransaction - Pointer to user-allocated space for a new task structure
/// @param[in] Src            - Source address of data transfer
/// @param[in] Dst            - Destination address of data transfer
/// @param[in] ByteLength     - Size(in bytes) of the transfer
/// @param[in] SrcLineWidth   - Source line width
/// @param[in] DstLineWidth   - Destination line width
/// @param[in] SrcStride      - Source stride
/// @param[in] DstStride      - Destination stride
/// @return Pointer to initialized CMXDMA structure
///
dmaTransactionList* dmaCreateTransactionFullOptions(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 SrcLineWidth, u32 DstLineWidth, s32 SrcStride, s32 DstStride);

/// @brief Initialize a new CMXDMA task structure which can be used to realize a simple DMA data transfer
/// @brief The transaction type is a 2D transaction
/// @brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
/// @param[in] ReqId          -  A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param[in] NewTransaction - Pointer to user-allocated space for a new task structure
/// @param[in] Src            - Source address of data transfer
/// @param[in] Dst            - Destination address of data transfer
/// @param[in] ByteLength     - Size(in bytes) of the transfer
/// @return Pointer to initialized CMXDMA structure
///
dmaTransactionList* dmaCreateTransaction(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength);

/// @brief Initialize a new CMXDMA task structure which can be used to realize a DMA data transfer using source stride only
/// @brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
/// @param[in] ReqId          - A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param[in] NewTransaction - Pointer to user-allocated space for a new task structure
/// @param[in] Src            - Source address of data transfer
/// @param[in] Dst            - Destination address of data transfer
/// @param[in] ByteLength     - Size(in bytes) of the transfer
/// @param[in] LineWidth      - Source line width
/// @param[in] SrcStride      - Source stride
/// @return Pointer to initialized CMXDMA structure
///
dmaTransactionList* dmaCreateTransactionSrcStride(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 LineWidth, s32 SrcStride);

/// @brief Initialize a new CMXDMA task structure which can be used to realize a DMA data transfer using destination stride only
/// @brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
/// @param[in] ReqId          - A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param[in] NewTransaction - Pointer to user-allocated space for a new task structure
/// @param[in] Src            - Source address of data transfer
/// @param[in] Dst            - Destination address of data transfer
/// @param[in] ByteLength     - Size(in bytes) of the transfer
/// @param[in] LineWidth      - Destination line width
/// @param[in] DstStride      - Destination stride
/// @return Pointer to initialized CMXDMA structure
///
dmaTransactionList* dmaCreateTransactionDstStride(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 LineWidth, s32 DstStride);

/// @brief Creates a new 3D transaction.
/// @brief The function returns a handle to the new transaction.
/// @param[in] ReqId          - A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param[in] NewTransaction - Pointer to user-allocated space for a new task structure
/// @param[in] Src            - source address for the transaction.
/// @param[in] Dst            - destination address for the transaction.
/// @param[in] ByteLength     - Size(in bytes) of the transfer
/// @param[in] SrcLineWidth   - line width for source in bytes.
/// @param[in] DstLineWidth   - line width for destination in bytes.
/// @param[in] SrcStride      - stride size for source, defined as the size in bytes
///                             from the start of a line to the start of the following line.
/// @param[in] DstStride      - stride size for destination, defined as the size in
///                             bytes from the start of a line to the start of the following line.
/// @param[in] NumPlanes      - number of planes of data to be transfered. The value
///                             needs to be greater than zero for 3D transaction.
/// @param[in] SrcPlaneStride - plane stride size for source, defined as the size in bytes from the
///					  start of a plane to the start of the following plane.
/// @param[in] DstPlaneStride - plane stride size for destination, defined as the size in bytes from
/// 					  the start of a plane to the start of the following plane.
///
/// @return Pointer to initialized CMXDMA structure
///
dmaTransactionList* dmaCreate3DTransaction(dmaRequesterId ReqId,
                                           dmaTransactionList *NewTransaction,
                                           u8* Src,
                                           u8* Dst,
                                           u32 ByteLength,
                                           u32 SrcLineWidth,
                                           u32 DstLineWidth,
                                           s32 SrcStride,
                                           s32 DstStride,
                                           u32 NumPlanes,
                                           s32 SrcPlaneStride,
                                           s32 DstPlaneStride);

/// @brief Link multiple tasks in a single linked list. Please note that this function allows linking just for single tasks.
/// @note One can not link together in this way two or more linked lists of tasks in order to form a single list.
/// @param[in] listHead - Pointer to the structure which will represent the start of linked task list.
/// @param[in] nmbTasks - Number of tasks to be linked to list head
/// @param[in] ...      - Pointers to the tasks to be linked. The structures passed here will be linked to listHead from left to right, in order of their placement on function call.
/// @return void 
///
void dmaLinkTasks(dmaTransactionList* listHead, u32 nmbTasks, ...);

/// @brief Set-up CMXDMA to execute the given list of tasks.
/// @note Please note if there is heavy use of CMXDMA, the task list won't start immediately,
/// it will be put in a waiting queue until CMXDMA will become available to execute the current task.
/// @param[in] ListPtr - Pointer to the task or list of tasks to be executed
/// @return
///  - 0 - CMXDMA waiting queue is full, no new tasks can be added now
///  - 1 - Tasks have been submitted directly to CMXDMA and are executing now
///  - 2 - Tasks have been added to a waiting queue and are pending execution
///
int dmaStartListTask(dmaTransactionList* ListPtr);

/// @brief Wait in a blocking way for a given task to finish.
/// @param[in] ListPtr - Pointer to the task to be waited
/// @return void 
///
void dmaWaitTask(dmaTransactionList* ListPtr);

/// @brief Check whether a task finished it's execution or is still running/pending
/// @param[in] ListPtr - Pointer to the task to be checked
/// @return
/// - 0 - Task is still executed/pending
/// - 1 - Task finished it's execution
///
int dmaIsTaskFinished(dmaTransactionList* ListPtr);
/// @}

#ifdef __cplusplus
}
#endif

#endif


