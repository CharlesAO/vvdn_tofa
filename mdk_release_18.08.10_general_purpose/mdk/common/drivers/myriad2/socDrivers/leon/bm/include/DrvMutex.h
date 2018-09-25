///
/// @file DrvMutex.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvMutex Mutex Driver
/// @{
/// @brief     API for accessing the hardware mutexes of the Myriad platform.
///


#ifndef DRV_MUTEX_H
#define DRV_MUTEX_H

#include <mv_types.h>
#include <DrvIcb.h>
#include <DrvRegUtils.h>
#include "HglMutex.h"

#define DRVMUTEX_SUCCESS 1
#define DRVMUTEX_ERROR   0

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Lock a mutex (blocking)
///
/// Lock the mutex, if not available block in loop until will be take
/// @param  mutex             mutex number, [0,31]
/// @return     1 if mutex was taken, 0 if mutex is already taken by LOS/LRT
///             and no lock operation was made
///
u32 DrvMutexLock(u32 mutex);

/// @brief Lock a mutex (non-blocking)
///
/// Lock the mutex if available else return 0
/// @param  mutex             mutex number, [0,31]
/// @return     1 if mutex was taken or is already taken by LOS/LRT,
///             0 if mutex is not available
///
u32 DrvMutexTryLock(u32 mutex);

/// @brief Request a lock for a mutex
///
/// Put a Lock request for mutex, the request answer can be taken
/// by using a separated define an interrupt service routine
/// @param  mutex             mutex number, [0,31]
/// @return     1 if the request was successfully set
///             0 if mutex is on current LOS/LRT unit and request was not set
///
u32 DrvMutexLockRequest(u32 mutex);

/// @brief Wait for the requested mutex
///
/// Wait until a specified mutex is taken by current units assuming that
/// prevision was put a lock request, otherwise will generate an infinite loop
/// @param  mutex             mutex number, [0,31]
/// @return void
///
void DrvMutexLockWait(u32 mutex);

/// @brief Unlock a specified mutex
///
/// @param  mutex             mutex number, [0,31]
/// @return void
///
void DrvMutexUnlock(u32 mutex);

/// @brief Clear own unit pending look request on a specified mutex
///
/// @param  mutex             mutex number, [0,31]
/// @return void
///
void DrvMutexClearOwnPendingRequest(u32 mutex);

/// @biref Clear all pending look request on a specified mutex
///
/// @param  mutex             mutex number, [0,31]
/// @return void
///
void DrvMutexClearAllPendingRequest(u32 mutex);

/// @brief Get status of a specified mutex
///
/// @param  mutex             mutex number, [0,31]
/// @return   bit 8: 1  - mutex in use / 0 - mutex free
///           bit [3:0] - [0x0 to 0xb] Mutex granted to SHAVE
///                     - 0xc          Mutex granted to LeonOS  
///                     - 0xd          Mutex granted to LeonRT  
/// 
u32 DrvMutexGetStatus(u32 mutex);

/// @brief Get status for all mutexes
///
/// @param  void
/// @return   bit [31:0]  - One bit-per-mutex summary of availability
///                         (0x1 - mutex in use, 0x0 - mutex not in use).
///
u32 DrvMutexGetAllStatus(void);

/// @brief Clear all interrupts that have 1 in the mask 
/// @param   intMaskToClear - 0-31 bits, 1 means interrupt will be clean
/// @return  void
///
void DrvMutexClearInterupts(u32 intMaskToClear);

/// @brief Enable interrupts for mutex that have corresponding bit 1
/// @param   intEnableMask - 0-31 bits, 1 means interrupt will be enabled
/// @return  void
///
void DrvMutexEnableInterupts(u32 intEnableMask);

/// @brief Get interrupt status for the mutexes
/// @param   void
/// @return  32 bits that correspond to all 32 mutexes. 
///           1 means interrupt was happen, 0 not was happen
///
u32 DrvMutexGetInteruptsStatus(void);

/// @brief Set interrupt - Not use it if other IRQ_CMX interrupts what to be set
/// For complex projects, please define in you project handler icb operations 
/// This is just for demo, for test propose and easy use
/// @param   mutexHandler - pointer to handler function
/// @param   intMask      - mask for the wich mutexs will generate interrupt
/// @return  void
///
void DrvMutexSetInterrupts(irq_handler mutexHandler, int intMask);


//#############################################################################################
//Fast mutex access routines that rely on static Leon identification
//#############################################################################################
#if defined(LEON_RT)
#define MTX_GET          CMX_MTX_LRT_GET
#define I_AM_THE_OWNER   MTX_GRANTED_TO_LRT
#else //Leon OS
#define MTX_GET          CMX_MTX_LOS_GET
#define I_AM_THE_OWNER   MTX_GRANTED_TO_LOS
#endif

//#############################################################################################
static inline void DrvFastMutexLock(u32 mutexNo)
{
    u32 mtxStatusRegAddr = CMX_MTX_BASE + CMX_MTX_0 + mutexNo * 4;
    
  //Put the request:
    SET_REG_HALF(CMX_MTX_BASE + MTX_GET, mutexNo | MTX_REQ_ON_RETRY);
  //Wait till current Leon gets the mutex
    while ( (GET_REG_WORD_VAL(mtxStatusRegAddr) & MTX_STAT_MASK) != (MTX_STAT_IN_USE | I_AM_THE_OWNER))
     ;
}

//#############################################################################################
static inline void DrvFastMutexUnlock(u32 mutexNo)
{
   //Release
    SET_REG_HALF(CMX_MTX_BASE + MTX_GET, mutexNo | MTX_RELEASE);
}

#ifdef __cplusplus
}
#endif

#endif

///@}
