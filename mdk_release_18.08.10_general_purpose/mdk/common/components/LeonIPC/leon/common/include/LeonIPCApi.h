///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup LeonIPCApi Leon IPC API
/// @ingroup LeonIPCApi
/// @{
/// @brief    Leon Inter Processor Communication Component Functions API.
///
/// This is the API for Leon to Leon message passing
///

#ifndef _LEONIPC_H_
#define _LEONIPC_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "LeonIPCApiDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------


/// @brief This function initializes a protocol given a message pool and a size for it
///
/// @note  The function should only be called on the Tx side of the communication
/// @param[in] msgChannel          - address of a leonIPCChannel_t variable to initialize protocol
/// @param[in] messagePool        - address of a uint32_t array for the message pool
/// @param[in] messagePoolSize - the maximum number of messages the queue can hold
/// @param[in] messageSize         - the size of a single message in words
/// @return
///                            - IPC_SUCCESS - initialization finished successfully
///                            - IPC_TX_ALREADY_INITIALIZED - Rx was already initialized
///
int LeonIPCTxInit(leonIPCChannel_t* msgChannel, uint32_t* messagePool, uint32_t messagePoolSize, uint32_t messageSize);


/// @brief This function initializes the Rx side of the communication
///
/// @note  The function should only be called on the Rx side of the communication
/// @param[in] msgChannel   - address of a leonIPCChannel_t variable used for communication
/// @param[in] msgCallback  - address of the callback to assign
/// @param[in] irqNo                - the number of the interrupt used to notify the receiver that a message is available
/// @param[in] irqPrio              - priority level of the notification interrupt
/// @return
///                         - IPC_SUCCESS - initialization finished successfully
///                         - IPC_RX_ALREADY_INITIALIZED - Rx was already initialized
///
int LeonIPCRxInit(leonIPCChannel_t* msgChannel, leonIPCCallback_t msgCallback, uint32_t irqNo, uint32_t irqPrio);

/// @brief This function resets the receiver thread to the current calling thread
///
/// @note  The function should only be called on the Rx side of the communication after LeonIPCRxReleaseSinkThread is called
/// @param[in] msgChannel   - address of a leonIPCChannel_t variable used for communication
/// @return
///                         - IPC_SUCCESS - initialization finished successfully
///                         - IPC_RX_ALREADY_INITIALIZED - Rx was already initialize
///                         - RTEMS_NOT_OWNER_OF_RESOURCE - LeonIPCRxReleaseSinkThread should be called before using this function
///
int LeonIPCRxReassignSinkThread(leonIPCChannel_t* channel);

/// @brief This function resets the receiver thread. This function must be used before using the LeonIPCRxReassignSinkThread function
///
/// @note  The function should only be called on the Rx side of the communication
/// @param[in] msgChannel   - address of a leonIPCChannel_t variable used for communication
/// @return
///                         - IPC_SUCCESS - initialization finished successfully
///                         - IPC_RX_ALREADY_INITIALIZED - Rx was already initialized
int LeonIPCRxReleaseSinkThread(leonIPCChannel_t* channel);

/// @brief This function sends a message to the consumer
/// @param[in] msgChannel - address of the communication channel
/// @param[in] message       - pointer to the message that needs to be sent
/// @return
///                       - IPC_SUCCESS - initialization finished successfully
///                       - IPC_QUEUE_OVERFLOW - the message queue if full
///                       - IPC_TX_ALREADY_INITIALIZED - Rx was already initialized
///
int LeonIPCSendMessage(leonIPCChannel_t* msgChannel, uint32_t* message);


/// @brief This function waits for a valid message to be present in the message queue
/// @param[in] msgChannel - address of the communication channel
/// @param[in] timeout           - the amount of time to wait for a message
/// @return
///                       - IPC_SUCCESS - initialization finished successfully
///                       - IPC_TIMEOUT - the timeout expired before a valid message arrived
///
int LeonIPCWaitMessage(leonIPCChannel_t* msgChannel, uint32_t timeout);


/// @brief This function waits for a valid message to be present in the message queue
/// @param[in]   msgChannel       - address of the communication channel
/// @param[out] noOfmessages  - the number of messages available in the message queue
/// @return
///                          - IPC_SUCCESS - the operation finished successfully
///                          - IPC_TX_NOTINITIALIZED - the transmitter was not initialized
///
int LeonIPCNumberOfPendingMessages(leonIPCChannel_t* msgChannel, uint32_t* noOfmessages);

/// @brief This function waits for a valid message to be present in the message queue
///
/// @note  This function must be called in the same thread that called LeonIPCRxInit
/// @param[in]  msgChannel - address of the message channel
/// @param[out] message      - pointer to the region of memory  where the read message will be placed
/// @return
///                        - IPC_SUCCESS - the operation finished successfully
///                        - IPC_TX_NOTINITIALIZED - the transmitter was not initialized
///
int LeonIPCReadMessage(leonIPCChannel_t* msgChannel, uint32_t* message);

/// @}
#ifdef __cplusplus
}
#endif


#endif /* _LEONIPC_H_ */
