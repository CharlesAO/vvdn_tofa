///
/// @file OsCommon.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsCommon OsCommon Driver
/// @{
/// @brief     Common header file for all drivers
///

#ifndef OS_COMMON_H
#define OS_COMMON_H

// Common OS header files
#include <stdio.h>
#include <rtems.h>
#include <pthread.h>
#include <bsp.h>
#include <mv_types.h>

#ifdef __cplusplus
extern "C" {
#endif

// 1: Defines
// ----------------------------------------------------------------------------

/// OS Start code
#define MYR_OS_START_CODE               0
/// OS Last code
#define MYR_OS_LAST_CODE                999
/// Driver first code
#define MYR_DRV_START_CODE              1000
/// Driver last code
#define MYR_DRV_LAST_CODE               1499
/// Driver custom return code start
#define MYR_DRV_CUSTOM_START_CODE       1500
/// Driver custom last code
#define MYR_DRV_CUSTOM_LAST_CODE        1999
/// Driver HW start code
#define MYR_DRV_HW_START_CODE           2000
/// Driver HW standard base code
#define MYR_DRV_HW_BASE_START_CODE      3000
/// Driver HW last code
#define MYR_DRV_HW_LAST_CODE            3999
/// Driver Dynamic Infrastructure base code
#define MYR_DYN_INFR_START_CODE         7000
/// Default success return value
#define MYR_DRV_OS_SUCCESS_CODE         0

/// Macro to obatin return code type
#define OSDRV_RETRIEVE_CODE_TYPE(x)     OsCommonRetrieveReturnType(code)
/// Macro to map HW errors
#define OSDRV_MAP_HW_ERROR(code)        (code + MYR_DRV_HW_BASE_START_CODE)
/// Macro to check HW driver return value extended
#define OSDRV_HW_DRV_CHECK_SUCCESS_EXT(code, success) OsCommonCheckSuccessExt(code, success)
/// Macro to check HW driver return value
#define OSDRV_HW_DRV_CHECK_SUCCESS(result) OSDRV_HW_DRV_CHECK_SUCCESS_EXT(result, 0)
/// Macro to convert dynamic infrastructure driver (bare metal) error codes to OS codes
#define OSDRV_DYN_INFR_CHECK_CODE(code) OsDrvCheckDynaInfrStatusCode(code);

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
/// @brief Enum containing types of return codes
typedef enum
{
    OSDRV_OS_CODE,          /*< OS driver return code */
    OSDRV_DRV_CODE,         /*< Driver return code */
    OSDRV_CUSTOM_DRV_CODE,  /*< Custom driver return code */
    OSDRV_HW_DRV_CODE,      /*< Driver HW return code */
    OSDRV_OUT_OF_RANGE_CODE /*< Drive code out of range */
}OSDRV_RETURN_TYPE;


/// @brief Enum containing driver status codes
typedef enum
{
    OS_MYR_DRV_SUCCESS = MYR_DRV_OS_SUCCESS_CODE, /*< Successful operation */
    OS_MYR_DRV_ERROR = MYR_DRV_START_CODE,        /*< Error in driver */
    OS_MYR_DRV_NOTOPENED,                         /*< Driver still not opened */
    OS_MYR_DRV_ALREADY_INITIALIZED,               /*< Driver already initialized */
    OS_MYR_DRV_ALREADY_OPENED,                    /*< Driver already opened */
    OS_MYR_DRV_TIMEOUT,                           /*< Timeout while accessing the resource */
    OS_MYR_DRV_RESOURCE_BUSY,                     /*< Cannot access the resource because it is busy */
    OS_MYR_RESOURCE_NOT_OWNED,                    /*< Trying to access a resource that is own by other thread */
    OS_MYR_DRV_CLOCK_ERROR                        /*< Error related to clock */
} OS_MYRIAD_DRIVER_STATUS_CODE;

/// @brief Enum containing driver statuses
typedef enum
{
    OS_DRV_STAT_UNINITIALIZED = 0,  /*< Driver status not initialized */
    OS_DRV_STAT_INITIALIZED,        /*< Driver initialized */
    OS_DRV_STAT_OPENED              /*< Driver opened, ready to be used */
} OS_MYRIAD_DRIVER_STATUS;

/// @brief Enum containing types of protection for driver handlers
typedef enum {
    OS_MYR_PROTECTION_SELF = 0,     /*< Protect handler from being used in other threads other than the one where it was created */
    OS_MYR_PROTECTION_SEM,          /*< Protection provided by a single semaphore for all resources */
    OS_MYR_PROTECTION_CUSTOM_SEM    /*< Protection provided by creating a single semaphore per resource */
} OS_MYRIAD_DRIVER_PROTECTION;

typedef enum
{
    OS_MYR_DYN_INFR_SUCCESS = OS_MYR_DRV_SUCCESS,               /* Successful operation */
    OS_MYR_DYN_INFR_ERROR = MYR_DYN_INFR_START_CODE,            /* Generic error in dynamic infrastructure */
    OS_MYR_DYN_INFR_MULTIPLE_SHAVE_ALLOCATION,                  /* Shave allocated to multiple applications */
    OS_MYR_DYN_INFR_INVALID_PARAMETERS,                         /* Invalid function parameters */
    OS_MYR_DYN_INFR_CANNOT_ALLOC_HEAP,                          /* Cannot allocate heap memory */
    OS_MYR_DYN_INFR_CANNOT_ALLOC_GROUPDATA,                     /* Cannot allocate group data memory */
    OS_MYR_DYN_INFR_RUNALGO_SHAVES_BUSY,                        /* Error running shave algo. Target shaves are busy or invalid function parameter */
    OS_MYR_DYN_INFR_SHAVE_BUSY_OR_INVALID,                      /* Error running shave algo on assigned shave. Target shave busy or not assigned */
    OS_MYR_DYN_INFR_INVALID_SAVED_CONFIG,                       /* Error at cleanup. Invalid saved configuration */
    OS_MYR_DYN_INFR_ERROR_STACK_OVERFLOW,						/* Error running shave algo. Stack overflow occurred */
    OS_MYR_DYN_INFR_NUMBER_OF_FREE_SHAVE_NOT_AVAILABLE,         /* Error running shave algo. Free shaves number requested not available */
    OS_MYR_DYN_INFR_SHAVE_NOT_FOUND_IN_CONTEXT_LIST,            /* Cannot find the shave in the Context list*/
    OS_MYR_DYN_INFR_HEAP_NOT_SELECTED,                          /* Heap is not selected but heap size in set*/
    OS_MYR_DYN_INFR_HEAP_SIZE_NOT_SET,                          /* Heap is selected but heap size is not set*/
    OS_MYR_DYN_INFR_FUNCTION_NOT_FOUND,                         /* The function name is not found into the application IAT*/
    OS_MYR_DYN_INFR_INVALID_LAST_CODE

} OS_MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE;

/// @brief Type to be used for sharing flags with interrupts
typedef struct
{
    volatile u32 irqShared; /*< shared interrupt request flag */
}osCommonIrqShareInfo_t;

// TODO: Find a better way of isolating the user from the OS
// IO Driver Manager Abstraction
///  Initialize a device driver
#define OsDrvIOInitialize       rtems_io_initialize
/// Open a device driver
#define OsDrvIOOpen             rtems_io_open
/// Close a device driver
#define OsDrvIOClose            rtems_io_close
/// Write to a device
#define OsDrvIOWrite            rtems_io_write
/// Read from a device
#define OsDrvIORead             rtems_io_read
/// Control device's services
#define OsDrvIOControl          rtems_io_control

/// Register a device driver
#define OsDrvIORegister         rtems_io_register_driver
/// Unregister a driver from the device driver table
#define osDrvIOUnregister       rtems_io_unregister_driver
/// Register a device name in the file system
#define OsDrvIORegisterName     rtems_io_register_name
/// Determine the major/minor number pair associated with the specified device name
#define OsDrvIOLookupName       rtems_io_lookup_name

// LIBI2C Driver abstraction
/// Initialize the I2C library
#define OsDrvLibI2CInitialize       rtems_libi2c_initialize
/// Register a low level libi2c bus driver
#define OsDrvLibI2CRegisterBus      rtems_libi2c_register_bus
/// Register a high level driver that provides abstraction for specific i2c devices
#define OsDrvLibI2CRegisterDevice   rtems_libi2c_register_drv

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------
//Non overlapping events list
//The events 0  - 11 are available to use by the application
//The events 12 - 22 listed below are used by the MDK drivers/components
//The events 23 - 31 are available to use by the application

// List of events currently being used by MDK drivers

// The following 2 USB events are directed to the USB event processing thread
// and not to the thread from which USB API functions are called. The callbacks
// for USB API function are called in USB event thread, so event collision
// should be avoided in this callbacks
#define OSCOMMON_EVENT_USB_1                         RTEMS_EVENT_12  /*< USB event */
#define OSCOMMON_EVENT_USB_2                         RTEMS_EVENT_13  /*< USB event */
#define OSCOMMON_EVENT_AHBDMA                        RTEMS_EVENT_14  /*< AHBDMA RTEMS event */
#define OSCOMMON_EVENT_SHAVE                         RTEMS_EVENT_15  /*< SHAVE RTEMS event */
#define OSCOMMON_EVENT_CMXDMA                        RTEMS_EVENT_16  /*< CMXDMA RTEMS event */
#define OSCOMMON_EVENT_SDIO                          RTEMS_EVENT_17  /*< SDIO RTEMS event */
#define OSCOMMON_EVENT_UART                          RTEMS_EVENT_18  /*< UART RTEMS event */
#define OSCOMMON_EVENT_I2C_MTX_END                   RTEMS_EVENT_19   /*< Transfer in progress */
#define OSCOMMON_EVENT_I2C_MTX_NACK                  RTEMS_EVENT_20   /*< Transfer in progress */
#define OSCOMMON_EVENT_I2C_MRX_END                   RTEMS_EVENT_21   /*< Receive in progress */
#define OSCOMMON_EVENT_I2C_MRX_NACK                  RTEMS_EVENT_22   /*< Receive in progress */

//Non overlapping system events list
//The events  0 - 23 listed below are used by the MDK drivers/components
//The events 24 - 31 are reserved for RTEMS use

// List of system events currently being used by MDK drivers

// List of system events currently being used by MDK components
#define OSCOMMON_EVENT_MPROTOCOL_VC_READ_DATA        RTEMS_EVENT_23 /* MessageProtocol */
#define OSCOMMON_SYS_EVENT_CMX_DMA                   RTEMS_EVENT_22 /*< CMXDMA RTEMS event */

// 4: Export Functions
// ----------------------------------------------------------------------------
/// @brief Replacement for standard rtems_event_receive
///
/// Instead of using just events we needed another way of sharing information between the ISR and the threads
/// We are allowed to take one event per driver and this way we will not overlap events from our drivers.
/// This is meant to separate notification from data and thus allow to consecutive non-blocking calls from
/// different drivers
/// @param [in] event_in Pattern of bits expected in shared vatiable
/// @param [in] waitforany TRUE if we are to wait for any of the flags specified, FALSE if we have to wait for all
/// @param [in]  wait TRUE if we are to wait, FALSE otherwise
/// @param [in]  ticks Ticks to wait. RTEMS_NO_TIMEOUT to wait foreever, 0 not to wait a single tick
/// @param [out] event_out Events captured (out of event_in only)
/// @param [inout] shared Pointer to the shared variable
/// @param [in]  driver_event Event corresponding to the driver
/// @remarks TODO increase counter resolution
/// @return
///     RTEMS_SUCCESSFUL
///     RTEMS_UNSATISFIED
///     RTEMS_TIMEOUT
///
rtems_status_code OsCommonEventReceive(rtems_event_set event_in, int waitforany, int wait, rtems_interval ticks, rtems_event_set *event_out, volatile u32 *shared, rtems_event_set driver_event);

/// @brief Replacement for standard rtems_event_send
///
/// Instead of using just events we needed another way of sharing information between the ISR and the threads
/// We are allowed to take one event per driver and this way we will not overlap events from our drivers.
/// This is meant to separate notification from data and thus allow to consecutive non-blocking calls from
/// different drivers
/// @param [in] id Thread Id to be notified
/// @param [in] event_in events to be notified in shared variable
/// @param [inout] sharedInfo Pointer to the variable shared with ISR
/// @param [in] driver_event event corresponding to the driver
/// @param [in] protect TRUE if variable assignment must be protected, FALSE otherwise. To be used for shared ISR
/// @remarks TODO Find a better way of protecting the variable
/// @return
///     RTEMS_SUCCESSFUL
///     RTEMS_INVALID_ID
///
rtems_status_code OsDrvCommonInterruptSendEvent(rtems_id id, rtems_event_set event_in, volatile u32 *sharedInfo, rtems_event_set driver_event, int protect);

/// @brief Utility to print more information about status codes
///
/// @param[in] code     Status code to be checked
/// @return
///     none
///
void OsDrvCheckDynaInfrStatusCode(int code);

/// @brief Function to get the type of code returned
///
/// @param[in] code Status code to be evaluated
/// @return @ref OSDRV_RETURN_TYPE
///
static inline OSDRV_RETURN_TYPE OsCommonRetrieveReturnType(int code)
{
    OSDRV_RETURN_TYPE ret;
    if ((code >= MYR_OS_START_CODE) && (code <= MYR_OS_LAST_CODE))
        ret = OSDRV_OS_CODE;
    else if ((code >= MYR_DRV_START_CODE) && (code <= MYR_OS_LAST_CODE))
        ret = OSDRV_DRV_CODE;
    else if ((code >= MYR_DRV_CUSTOM_START_CODE) && (code <= MYR_DRV_CUSTOM_LAST_CODE))
        ret = OSDRV_CUSTOM_DRV_CODE;
    else if ((code >= MYR_DRV_HW_START_CODE) && (code <= MYR_DRV_HW_LAST_CODE))
        ret = OSDRV_HW_DRV_CODE;
    else
        ret = OSDRV_OUT_OF_RANGE_CODE;
    return ret;
}

/// @brief Utility to check if code is successful and map it
///
/// @param[in] code     Status code to be checked
/// @param[in] success  Model of a success status code
/// @return
///     MYR_DRV_OS_SUCCESS_CODE
///     Driver HW error code
///
static inline int32_t OsCommonCheckSuccessExt(int32_t code, int32_t success)
{
    return code == success ?  MYR_DRV_OS_SUCCESS_CODE : OSDRV_MAP_HW_ERROR(code);
}

#ifdef __cplusplus
}
#endif


#endif // OS_COMMON_H


///@}
