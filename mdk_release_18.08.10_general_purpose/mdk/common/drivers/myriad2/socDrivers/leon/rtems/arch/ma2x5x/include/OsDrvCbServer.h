///
/// @file OsDrvCbServer.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvCbServer OsCbServer Driver
/// @{
/// @brief     Callback Server API definitions
///
///            This devide is supposed to be used for internal use only.
///            A user application should not use this device, and thus to include this file.
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OS_DRV_CBSERVER_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OS_DRV_CBSERVER_H_

// 1: Includes
// ----------------------------------------------------------------------------

#include <OsCommon.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_DRV_CB_SERVER_DEV_NAME   "/dev/cb_server"
#define CB_SERVER_MAX_QUEUE_SIZE    16

typedef enum {
    CB_SERVER_CALLBACK_REQUEST,
    CB_SERVER_BARRIER,
    CB_SERVER_SUSPEND,
    CB_SERVER_RESUME,
    CB_SERVER_GET_STATE,
} CbServerIoctl;

typedef enum {
    CB_SERVER_UNINITIALIZED = 0,
    CB_SERVER_INITIALIZED,
    CB_SERVER_STARTED,
    CB_SERVER_SUSPEND_REQUESTED,
    CB_SERVER_SUSPENDED,
    CB_SERVER_EXITED_ON_ERROR,
} CbServerStates;

typedef struct {
    void (*callback)(void *);
    void * argument;
} CbServerCbRequest;

typedef struct {
    uint32_t state;
} CbServerState;

typedef union {
    CbServerCbRequest cb;
    CbServerState     st;
} CbServerIoctlData;

// 2:  Exported Global Data
// ----------------------------------------------------------------------------

// 3:  Exported Functions
// ----------------------------------------------------------------------------

extern rtems_driver_address_table osDrvCbServerTbl;

#ifdef __cplusplus
}
#endif

#endif //COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_RTEMS_INCLUDE_OS_DRV_CBSERVER_H_

///@}
