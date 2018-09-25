///
/// @file OsDrvMutex.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvMutex OsMutex Driver
/// @{
/// @brief     Hardware mutex API for RTEMS
///
/// --------------------------------------------------------------------------------
/// Usage example
///
/// #include "OsDrvMutex.h"
/// ...
/// osDrvMutexHandle_t handle;
///
/// OsDrvMutexInit(INTERRUPT_PRIORITY); // this must be the first called function
///                                     // in order to initialize the mutex API
/// OsDrvMutexOpen(&handle);    // will return a handle to be used for further mutex operations
/// status = OsDrvMutexObtain(&handle); // will obtain the mutex immediately or block until
///                                     // it becomes available
/// if (status == OS_MYR_DRV_SUCCESS)
///    // perform whatever operations need mutex protection
///    ...
/// OsDrvMutexRelease(&handle); // make the mutex available for others to use
/// OsDrvMutexClose(&handle);   // release the resources associated with the mutex
///


#ifndef _OS_DRV_MUTEX_H_
#define _OS_DRV_MUTEX_H_

// System Includes
// ----------------------------------------------------------------------------

// Application Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include "OsDrvMutexDefines.h"
#include "mv_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Global function prototypes
// ----------------------------------------------------------------------------

/// Hardware mutex driver API initialization.
///
/// @param[in] interruptLevel the level of the mutex interrupt
///@return
///     OS_MYR_DRV_SUCCESS - initialization finished successfully
///     OS_MYR_DRV_ALREADY_INITIALIZED - driver was already initialized by a
///     previous call to OsDrvMutexInit
///
s32 OsDrvMutexInit(u32 interruptLevel);

/// Lock a mutex specified by handle
///
/// @param[in] handle handle for the mutex to be locked
///
/// This return imediately if the requested mutex is available, or will
/// block until the mutex becomes available.
///
///@return
///     OS_MYR_DRV_SUCCESS - the mutex was successfully locked
///     OS_MYR_DRV_RESOURCE_BUSY - the mutex is taken by the same processor that
//      called the function, but
///     OS_MYR_DRV_ERROR - the handle is invalid or an error has occurred trying
///     to obtain the mutex
///
s32 OsDrvMutexObtain(osDrvMutexHandle_t * handle);

/// Releases a mutex specified by handle
///
/// @param[in] handle handle for the mutex to be released
///
///@return
///     OS_MYR_DRV_SUCCESS - the mutex was successfully released
///     OS_MYR_DRV_RESOURCE_BUSY - the mutex is owned by other processor
///     OS_MYR_DRV_ERROR - the handle is invalid or an error has occurred trying
///     to release the mutex
///
s32 OsDrvMutexRelease(osDrvMutexHandle_t * handle);

/// Initialize resources and returns a handle for the mutex specified by id
///
/// @param[out] handle handle that can be used to perform mutex operations
/// @param[in] id id of the mutex than need to be used
///
/// If there are another handles using the same mutex, the resources will not be
/// allocated another time. All the handles that are using the same mutex are
/// sharing the common resource allocated by this function.
///
///@return
///     OS_MYR_DRV_SUCCESS - a mutex handle was successfully created
///     OS_MYR_DRV_ERROR - an error has occurred when trying to create the
///     mutex handle
///
s32 OsDrvMutexOpen(osDrvMutexHandle_t * handle, u32 id);

/// Release all the resources associated to a mutex
///
/// @param[in] handle handle for the mutex to be closed
///
/// The driver keeps a count of how many handles are using the same mutex. The
/// resources associated with a mutex are only released when there is no other
/// handle using that mutex.
///
///@return
///     OS_MYR_DRV_SUCCESS - a mutex handle was successfuly closed
///     OS_MYR_DRV_ERROR - the handle is invalid or an error has occurred when
///     trying to release the mutex resources.
///
s32 OsDrvMutexClose(osDrvMutexHandle_t * handle);

/// }@
#ifdef __cplusplus
}
#endif


#endif

///@}
