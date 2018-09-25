///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup MessageQueue Messsage Queue
/// @ingroup  LeonIPC
/// @{
/// @brief    Circular message queue.
///
/// API for handling a circular queue
///

#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "MessageQueueDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// @brief Initializes a queue given a message pool and a size for it
///
/// @param[out] handler        - pointer to a queue handler
/// @param[in] messagePool     - address of a uint32_t array for the message pool
/// @param[in] messagePoolSize - the maximum number of messages the queue can hold
/// @param[in] messageSize     - the size of a single message in words
/// @return 
///     - MSG_QUEUE_SUCCESS - initialization completed successfully
///     - MSG_QUEUE_ALREADY_INITIALIZED - the queue is already initialized
///
int MessageQueueInit(circularMsgQueue_t* handler, uint32_t* messagePool, uint32_t messagePoolSize, uint32_t messageSize);


/// @brief Adds a new element to the queue
///
/// @param[in] handler        - pointer to a queue handler
/// @param[in] message        - pointer to the message that needs to be added to the queue
/// @return 
///     - MSG_QUEUE_SUCCESS - the new element was added successfully
///     - MSG_QUEUE_OVERFLOW - there is no space in the queue for a new element
///
int MessageQueuePush(circularMsgQueue_t* handler, const uint32_t* const message);

/// @brief Removes an element from the queue
///
/// @param[in] handler        - pointer to a queue handler
/// @param[out] message       - pointer to the message that needs to be added to the queue
/// @return 
///     - MSG_QUEUE_SUCCESS - the element was removed successfully
///     - MSG_QUEUE_EMPTY - no element is available in the queue
///
int MessageQueuePop(circularMsgQueue_t* handler, uint32_t* const message);


/// @brief Checks if the queue is empty or not
///
/// @param[in] handler      - pointer to a queue handler
/// @param[out] empty       - if 0 the queue is not empty otherwise there is at least on element in the queue
/// @return 
///     - MSG_QUEUE_SUCCESS - the operation completed successfully
///     - MSG_QUEUE_NOTINITIALIZED - the queue was not initialized
///
int MessageQueueIsEmpty(circularMsgQueue_t* handler, uint32_t* empty);


/// @brief Checks if the queue is full or not
///
/// @param[in] handler      - pointer to a queue handler
/// @param[out] full       - if 0 the queue is not full, non zero if there is space for at least one element in the queue
/// @return 
///     - MSG_QUEUE_SUCCESS - the operation completed successfully
///     - MSG_QUEUE_NOTINITIALIZED - the queue was not initialized
///
int MessageQueueIsFull(circularMsgQueue_t* handler, uint32_t* full);


/// @brief Returns the number of elements available in the queue
///
/// @param[in] handler     - pointer to a queue handler
/// @param[out] size       - the number of elements available in the queue
/// @return 
///     - MSG_QUEUE_SUCCESS - the operation completed successfully
///     - MSG_QUEUE_NOTINITIALIZED - the queue was not initialized
///
int MessageQueueGetNoOfElements(circularMsgQueue_t* handler, uint32_t* size);


/// @}
#ifdef __cplusplus
}
#endif


#endif /* _MESSAGE_QUEUE_H_ */
