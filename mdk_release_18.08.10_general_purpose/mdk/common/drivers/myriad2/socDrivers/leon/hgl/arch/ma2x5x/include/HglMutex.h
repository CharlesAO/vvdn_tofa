///
/// @file HglMutex.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup HglMutex HglMutex Driver
/// @{
/// @brief     HGL function API for accessing the hardware
///
///            mutexes of the Myriad platform.
///


#ifndef HGL_MUTEX_H
#define HGL_MUTEX_H

#include "HglMutexDefines.h"
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "HglMutexMa2x5x.h"
#endif
#include <swcWhoAmI.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Obtain a mutex (blocking)
///
/// Obtain the mutex, if not available block in loop until will be taken
///
/// @param[in]  mutex - mutex number, [0,31]
/// @param[in]  proc  - processor that calls the function
/// @return
///             - HGLMUTEX_SUCCESS - if mutex was taken
///             - HGLMUTEX_OWNED   - if mutex is already taken by LOS/LRT
///                                  and no lock operation was made
///
s32 HglMutexObtain(u8 mutex, swcProcessorType proc);

/// @brief Obtain a mutex (non-blocking)
///
/// Obtain the mutex if available else return 0
///
/// @param[in]  mutex  -  mutex number, [0,31]
/// @param[in]  proc   -  processor that calls the function
/// @return     1 if mutex was taken or is already taken by LOS/LRT,
///             0 if mutex is not available
///
s32 HglMutexTryObtain(u8 mutex, swcProcessorType proc);

/// @brief Request a lock for a mutex
///
/// Put a Lock request for mutex, the request answer can be taken
/// by using a separated define an interrupt service routine
/// @param[in]  mutex   -     mutex number, [0,31]
/// @param[in]  proc    -     processor that calls the function
/// @return     1 if the request was successfully set
///             0 if mutex is on current LOS/LRT unit and request was not set
///
s32 HglMutexSetRequest(u8 mutex,swcProcessorType proc);

/// @brief Wait for the requested mutex
///
/// Wait until a specified mutex is taken by current units assuming that
/// prevision was put a lock request, otherwise will generate an infinite loop
/// @param[in]  mutex   -   mutex number, [0,31]
/// @param[in]  proc    -   processor that calls the function
/// @return void
///
void HglMutexWaitObtain(u8 mutex, swcProcessorType proc);

/// @brief Release a specified mutex
///
/// @param[in]  mutex     -  mutex number, [0,31]
/// @param[in]  proc      -  processor that calls the function
/// @return void
///
void HglMutexRelease (u8 mutex, swcProcessorType proc);

/// @brief Clear pending look request on a specified mutex
///
/// @param[in]  mutex    -  mutex number, [0,31]
/// @param[in]  proc     -  processor that calls the function
/// @param[in]  flush
///                           - !=0 - clear all requests
///                           - 0   - clear own request
/// @return void
///
void HglMutexClearRequest(u8 mutex ,swcProcessorType proc, u32 flush);

/// @brief Get status of a specified mutex
///
/// @param[in]  mutex       -    mutex number, [0,31]
/// @return   bit 8     - 1 Mutex in use / 0 Mutex free
///           bit [3:0] - [0x0 to 0xb] Mutex granted to SHAVE
///                     - 0xc          Mutex granted to LeonOS
///                     - 0xd          Mutex granted to LeonRT
///
u32 HglMutexGetStatus(u8 mutex);

/// @brief Get status for all mutexes
///
/// @param[in]  void
/// @return   bit [31:0]  - One bit-per-mutex summary of availability
///                         (0x1 - mutex in use, 0x0 - mutex not in use).
///
u32 HglMutexGetStatusAll(void);

/// @brief Clear all interrupts that have 1 in the mask
/// @param[in]  mask       - 0-31 bits, 1 means interrupt will be clean
/// @param[in]  proc       -       processor that calls the function
/// @return  void
///
void HglMutexClearIrq(u32 mask, swcProcessorType proc);

/// @brief Set interrupt - Not use it if other IRQ_CMX interrupts what to be set
/// For complex projects, please define in you project handler icb operations
/// This is just for demo, for test propose and easy use
/// @param[in]   mask         - mask for the mutex interrupts to be enabled/disabled
/// @param[in]   enable       - enable/disable flag for the mask
/// @param[in]   proc         - processor that calls the function
/// @return  void
///
void HglMutexEnableIrq(u32 mask, u32 enable, swcProcessorType proc);

/// @brief Get interrupt status for the mutexes
/// @param[in]  proc              processor that calls the function
/// @return  32 bits that correspond to all 32 mutexes.
///           1 means interrupt was happen, 0 not was happen
///
u32 HglMutexGetIrqStatus(swcProcessorType proc);


#ifdef __cplusplus
}
#endif

#endif

///@}
