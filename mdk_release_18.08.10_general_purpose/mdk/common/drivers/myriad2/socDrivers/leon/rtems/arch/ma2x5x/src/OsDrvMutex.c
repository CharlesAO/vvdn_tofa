///
/// @file OsDrvMutex.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvMutex
/// @{
/// @brief     Hardware mutex API for RTEMS
///


/// 1: Includes
/// ----------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include <rtems.h>
#include <semaphore.h>

#include "OsDrvMutex.h"
#include "OsCommon.h"
#include "HglMutex.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
typedef struct {
    OS_MYRIAD_DRIVER_STATUS status;
    rtems_id controlMutex;
} osDrvMutexControl_t;

#define OS_DRV_MUTEX_COUNT             32
#define OS_DRV_MUTEX_INVALID_ID        -1

#ifdef OS_DRV_MUTEX_L1
    #define OS_DPRINTF1(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF1(...)
#endif

#ifdef OS_DRV_MUTEX_L2
    #define OS_DPRINTF2(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF2(...)
#endif

#ifdef OS_DRV_MUTEX_L3
    #define OS_DPRINTF3(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF3(...)
#endif

// Lock utility by default wait forever to access the resource
#define OS_DRV_MUTEX_LOCK()            osDrvMutexLockCriticalSection(osDrvMutexControl.controlMutex, true, RTEMS_NO_TIMEOUT)
#define OS_DRV_MUTEX_N_LOCK(n)         osDrvMutexLockCriticalSection(sharedInfo[n].lockId, true, RTEMS_NO_TIMEOUT)
// Unlock utility
#define OS_DRV_MUTEX_UNLOCK()          osDrvMutexUnlockCriticalSection(osDrvMutexControl.controlMutex)
#define OS_DRV_MUTEX_N_UNLOCK(n)       osDrvMutexUnlockCriticalSection(sharedInfo[n].lockId)

#define OS_DRV_MUTEX_STATUS_BIT    (1 << 8)
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
//extern osDrvMutexShared_t __mutexShared;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static osDrvMutexControl_t osDrvMutexControl;
static osDrvMutexShared_t sharedInfo[OS_DRV_MUTEX_COUNT];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

/* Enter Critical Section */
static rtems_status_code osDrvMutexLockCriticalSection(rtems_id sem, u32 wait, u32 wait_ticks)
{
    rtems_status_code res;
    if (wait)
        res = rtems_semaphore_obtain(sem, RTEMS_WAIT, wait_ticks);
    else
        res = rtems_semaphore_obtain(sem, RTEMS_NO_WAIT, 0);
    OS_DPRINTF2("osDrvMutexLockCriticalSection res %d wait %d\n", res, wait);
    return res;
}

/* Abandon Critical Section */
static rtems_status_code osDrvMutexUnlockCriticalSection(rtems_id sem)
{
    rtems_status_code res = rtems_semaphore_release(sem);
    OS_DPRINTF2("osDrvMutexUnlockCriticalSection res %d\n", res);
    return res;
}

static void osDrvMutexInterruptHandler(void * unused)
{
    UNUSED(unused);

    u32 mtxCount = 0;
    u32 intStatus = HglMutexGetIrqStatus(swcWhoAmI());

    while (intStatus)
    {
        if (intStatus & 1)
        {
            rtems_semaphore_release(sharedInfo[mtxCount].eventId);
            HglMutexClearIrq(1 << mtxCount, swcWhoAmI());
        }
        mtxCount++;
        intStatus >>= 1;
    }
}

static void osDrvMutexInterruptSetup(u32 interruptLevel)
{
    BSP_Clear_interrupt(IRQ_CMX);
    BSP_Set_interrupt_type_priority(IRQ_CMX, POS_EDGE_INT, interruptLevel);
    BSP_interrupt_register(IRQ_CMX, NULL, (rtems_interrupt_handler)osDrvMutexInterruptHandler, NULL);
}

static s32 osDrvMutexValidHandle(osDrvMutexHandle_t * handle)
{
    return ((handle->mutexId >= 0)                         &&
            (handle->mutexId < OS_DRV_MUTEX_COUNT)         &&
            (handle->mutexInfo != NULL));
}

s32 OsDrvMutexInit(u32 interruptLevel)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    s32 status;

    assert((interruptLevel > 0) && (interruptLevel <= 14));

    if (osDrvMutexControl.status == OS_DRV_STAT_UNINITIALIZED)
    {
        status = rtems_semaphore_create(rtems_build_name('M', 'S', 'E', 'M'), 1,
                    RTEMS_BINARY_SEMAPHORE, 0, &osDrvMutexControl.controlMutex);
        if (status == RTEMS_SUCCESSFUL)
        {
            osDrvMutexControl.status = OS_DRV_STAT_INITIALIZED;
            res = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            res = status;
        }
        osDrvMutexInterruptSetup(interruptLevel);
    }
    else if (osDrvMutexControl.status == OS_DRV_STAT_INITIALIZED)
    {
        res = OS_MYR_DRV_ALREADY_INITIALIZED;
    }

    OS_DPRINTF1("OsDrvMutexInit() res %d\n", res);
    return res;
}

s32 osDrvMutexWait(osDrvMutexHandle_t * handle)
{
    s32 res = OS_MYR_DRV_ERROR;
    rtems_status_code status;

    status = rtems_semaphore_obtain(sharedInfo[handle->mutexId].eventId, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    if (status == RTEMS_SUCCESSFUL)
    {
        res = OS_MYR_DRV_SUCCESS;
    }
    else
    {
        res = status;
    }

    return res;
}

s32 OsDrvMutexOpen(osDrvMutexHandle_t * handle, u32 id)
{
    s32 status;
    s32 res = OS_MYR_DRV_ERROR;

    status = OS_DRV_MUTEX_LOCK();
    if (status == OS_MYR_DRV_SUCCESS)
    {
        if (id >= OS_DRV_MUTEX_COUNT)
        {
            //invalid mutex id
            res = OS_MYR_DRV_ERROR;
        }
        else
        {
            if (sharedInfo[id].count == 0)
            {
                //only allocate resources for the mutex if they
                //were not already allocated

                //semaphore used to send the mutex available
                //event from the interrupt handler
                status = rtems_semaphore_create(
                        rtems_build_name('E', 'V', 'T', '0' + id),
                        0, RTEMS_SIMPLE_BINARY_SEMAPHORE, 0, &sharedInfo[id].eventId);
                if (status != RTEMS_SUCCESSFUL)
                {
                    OS_DRV_MUTEX_UNLOCK();
                    return status;
                }
                //semaphore used to protect critical sections
                status = rtems_semaphore_create(
                        rtems_build_name('L', 'C', 'K', '0' + id),
                        1, RTEMS_SIMPLE_BINARY_SEMAPHORE, 1, &sharedInfo[id].lockId);
                if (status != RTEMS_SUCCESSFUL)
                {
                    OS_DRV_MUTEX_UNLOCK();
                    return status;
                }
                HglMutexEnableIrq(1 << id, 1, swcWhoAmI());
            }
            handle->mutexId = id;
            handle->mutexOwner = 0;
            handle->mutexInfo = &sharedInfo[id];
            sharedInfo[id].count++;
            res = OS_MYR_DRV_SUCCESS;
        }
        OS_DRV_MUTEX_UNLOCK();
    }
    else
    {
        res = status;
    }
    OS_DPRINTF1("OsDrvMutexOpen() res %d\n", res);
    return res;
}

s32 OsDrvMutexObtain(osDrvMutexHandle_t * handle)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    s32 status;

    status = OS_DRV_MUTEX_N_LOCK(handle->mutexId);
    if (status == RTEMS_SUCCESSFUL)
    {
        if (!osDrvMutexValidHandle(handle))
        {
            OS_DRV_MUTEX_N_UNLOCK(handle->mutexId);
            return OS_MYR_DRV_ERROR;
        }

        status = HglMutexSetRequest(handle->mutexId, swcWhoAmI());
        if (status == HGLMUTEX_OWNED)
        {
            //current processor already owns the mutex; no need to take it again
            if (handle->mutexOwner == 1)
            {
                res = OS_MYR_DRV_SUCCESS;
            }
            else
            {
                // another handle owns the mutex
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
            OS_DRV_MUTEX_N_UNLOCK(handle->mutexId);
        }
        else if (status == HGLMUTEX_SUCCESS)
        {
            //wait for the mutex to become available
            status = osDrvMutexWait(handle);
            if (status == OS_MYR_DRV_SUCCESS)
            {
                handle->mutexOwner = 1;
                OS_DRV_MUTEX_N_UNLOCK(handle->mutexId);
                res = OS_MYR_DRV_SUCCESS;
            }
            else
            {
                status = OS_MYR_DRV_ERROR;
            }
        }
    }
    else
    {
        res = status;
    }

    OS_DPRINTF1("OsDrvMutexObtain() res %d\n", res);
    return res;
}

s32 OsDrvMutexRelease(osDrvMutexHandle_t * handle)
{
    s32 status;
    s32 res = OS_MYR_DRV_ERROR;

    status = OS_DRV_MUTEX_N_LOCK(handle->mutexId);
    if (status == RTEMS_SUCCESSFUL)
    {
        if (!osDrvMutexValidHandle(handle) || !handle->mutexOwner)
        {
            OS_DRV_MUTEX_N_UNLOCK(handle->mutexId);
            return OS_MYR_DRV_ERROR;
        }

        if (handle->mutexOwner)
        {
            //only release the mutex if we own it
            HglMutexRelease(handle->mutexId, swcWhoAmI());
            handle->mutexOwner = 0;
            res = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            //we don't own the mutex so we cannot release it
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        OS_DRV_MUTEX_N_UNLOCK(handle->mutexId);
    }
    else
    {
        res = status;
        OS_DRV_MUTEX_N_UNLOCK(handle->mutexId);
    }

    OS_DPRINTF1("OsDrvMutexRelease() res %d\n", res);
    return res;
}

s32 OsDrvMutexClose(osDrvMutexHandle_t * handle)
{
    s32 status;
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    status = OS_DRV_MUTEX_LOCK();
    if (status == RTEMS_SUCCESSFUL)
    {
        if (!osDrvMutexValidHandle(handle))
        {
            OS_DRV_MUTEX_UNLOCK();
            return OS_MYR_DRV_ERROR;
        }

        sharedInfo[handle->mutexId].count--;
        if (sharedInfo[handle->mutexId].count <= 0)
        {
            //make sure the mutex is not taken before closing
            if (handle->mutexOwner)
            {
                //restore the count since we didn't close the mutex
                sharedInfo[handle->mutexId].count++;
                OS_DRV_MUTEX_UNLOCK();
                return OS_MYR_DRV_RESOURCE_BUSY;
            }

            status = OS_DRV_MUTEX_N_LOCK(handle->mutexId);
            if (status == RTEMS_SUCCESSFUL)
            {
                //no other handle is using this mutex
                //so is safe to release allocated resources
                status = rtems_semaphore_delete(sharedInfo[handle->mutexId].eventId);
                if (status == RTEMS_SUCCESSFUL)
                {
                    status = rtems_semaphore_delete(sharedInfo[handle->mutexId].lockId);
                    if (status == RTEMS_SUCCESSFUL)
                    {
                        sharedInfo[handle->mutexId].eventId = 0;
                        sharedInfo[handle->mutexId].lockId = 0;
                        sharedInfo[handle->mutexId].count = 0;
                        res = OS_MYR_DRV_SUCCESS;
                    }
                    else
                    {
                        //release semaphore only if it was not successfully deleted
                        OS_DRV_MUTEX_N_UNLOCK(handle->mutexId);
                    }
                }
            }
        }
        if (status == RTEMS_SUCCESSFUL)
        {
            handle->mutexId = OS_DRV_MUTEX_INVALID_ID;
            handle->mutexInfo = NULL;
            OS_DRV_MUTEX_UNLOCK();
        }
        res = status;
    }
    else
    {
        res = status;
    }
    OS_DPRINTF1("OsDrvMutexClose() res %d\n", res);
    return res;
}


///@}
