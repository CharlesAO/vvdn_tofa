///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup LeonIPCApiDefines Leon IPC API Defines
/// @ingroup  LeonIPC
/// @{
/// @brief    Definitions and types needed by the Math operations library
///
/// This is the file that contains all the Sensor definitions of constants, typedefs,
/// structures or enums private of this module
///

#ifndef _LEONIPCDEFINES_H_
#define _LEONIPCDEFINES_H_

// 0: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include "MessageQueue.h"

// 1: Defines
// ----------------------------------------------------------------------------
#define IPC_WAIT_FOREVER 0xFFFFFFFF

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

typedef enum leonIPCType{
    IPC_POSTING_OFF,
    IPC_POSTING_IRQ
} leonIPCMessagingType_t;

typedef enum
{
    IPC_SUCCESS = 0,
    IPC_TIMEOUT = 200,
    IPC_QUEUE_EMPTY,
    IPC_QUEUE_OVERFLOW,
    IPC_TX_ALREADY_INITIALIZED,
    IPC_RX_ALREADY_INITIALIZED,
    IPC_TX_NOTINITIALIZED,
    IPC_RX_NOTINITIALIZED
} ipcStatus_t;

struct leonIPCChannel_t;

/// Messaging callback function
typedef void (*leonIPCCallback_t) (struct leonIPCChannel_t *);

typedef struct leonIPCChannel_t {
    circularMsgQueue_t msgQueue;
    leonIPCCallback_t protoCallBack;    ///< Assigns a callback for the consumer to use
    uint32_t irqNo;                     ///< Number of interrupt to be used in case of ASYNC messaging
    uint32_t irqPrio;                   ///< Priority to be used in the case of ASYNC messaging
    uint32_t txInitialized;             ///< Keeps the initialization status for Tx
    uint32_t rxInitialized;             ///< Keeps the initialization status for Rx
    uint32_t ipcTaskId;                 ///< The id of the task that waits for a new message
    leonIPCMessagingType_t msgType;
} leonIPCChannel_t;

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

// Constant value description

/// @}
#endif /* _LEONIPCDEFINES_H_ */
