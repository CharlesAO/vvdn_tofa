/*
 * Cdma.c
 */
#ifndef _PC_CDMA_H_
#define _PC_CDMA_H_

#include <swcCdmaCommonDefines.h>
#include <swcCdmaCommon.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MYRIAD1_MAX_NUMBER_OF_TASKS 4

/// Blocking DMA Copy
/// @param - ReqId A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param - NewTransaction Pointer to user-allocated space for a new task structure
/// @param - Dst - destination for copy
/// @param - Src - source for copy
/// @param - ByteLength - length in bytes for copy
/// @return - void
///
void dmaCopyBlocking(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength);

/// @brief Set-up CMXDMA to execute a list of tasks similar to  dmaStartListTask.
///
/// The main difference is that the user can also specify a callback function and a pointer to some specific data.
/// The callback function will be called once the given tasks will finish their execution, and will receive as parameters
/// the pointer to that user-specific data and another pointer to ListPtr parameter
/// @param[in] ListPtr     -  Pointer to the task or list of tasks to be executed
/// @param[in] function    - Callback function to be called when the tasks will finish the execution
/// @param[in] userContext - A pointer to some information the user wants to pass to callback function
/// @return
///  - 0 - CMXDMA waiting queue is full, no new tasks can be added now
///  - 1 - Tasks have been submitted directly to CMXDMA and are executing now
///  - 2 - Tasks have been added to a waiting queue and are pending execution
///
int dmaStartTaskAsync(dmaTransactionList* ListPtr, dmaIrqHandler function, void *userContext);

///Kept for initial model compatibility
void mmCreate(void);

///Kept for initial model compatibility
void mmDestroy();

#ifdef __cplusplus
}
#endif

#endif //_PC_CDMA_H_
