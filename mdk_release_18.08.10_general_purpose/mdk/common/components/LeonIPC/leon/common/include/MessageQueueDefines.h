///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup MessageQueueDefines Message Queue Defines
/// @ingroup MessageQueue
/// @{
/// @brief    Definitions and types needed by MessageQueue
///
/// This is the file that contains all the Sensor definitions of constants, typedefs,
/// structures or enums private of this module
///

#ifndef _MESSAGE_QUEUE_DEFINES_H_
#define _MESSAGE_QUEUE_DEFINES_H_

// 0: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>

// 1: Defines
// ----------------------------------------------------------------------------


// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef enum {
    MSG_QUEUE_SUCCESS = 0,
    MSG_QUEUE_OVERFLOW = 100,
    MSG_QUEUE_EMPTY,
    MSG_QUEUE_NOTINITIALIZED,
    MSG_QUEUE_ALREADY_INITIALIZED,
    MSG_QUEUE_SENT_TOO_MANY
} msgQueueStatus_t;

typedef struct{
    uint32_t* messagePool;      ///< Keeps a pointer to the message pool
    uint32_t messageSize;       ///< Size of a message in words
    uint32_t poolSize;          ///< Number of messages that fit in the pool
    uint32_t* writeIdx;         ///< Points to the location where the next message will be placed
    uint32_t* readIdx;          ///< Pointer to the current message available to be read
    uint32_t readCount;         ///< Keeps the number of performed read operations
    uint32_t writeCount;        ///< Keeps the number of performed write operations
    uint32_t queueInitialized;  ///< Queue initialization status (0 - not initialized, 1 - initialized)
    uint32_t mutexIdProducer;           ///< Id for the mutex used for synchronization
    uint32_t mutexIdConsumer;           ///< Id for the mutex used for synchronization
} circularMsgQueue_t;

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

/// @}
#endif /* _MESSAGE_QUEUE_DEFINES_H_ */
