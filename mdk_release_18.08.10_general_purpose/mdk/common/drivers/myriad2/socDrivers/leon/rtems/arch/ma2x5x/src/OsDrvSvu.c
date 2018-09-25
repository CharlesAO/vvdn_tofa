///
/// @file OsDrvSvu.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvSvu
/// @{
/// @brief     Shave low level functionality for RTEMS.
///



/// 1: Includes
/// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <bsp.h>
#include <bsp/irq-generic.h>
#include "theDynContext.h"
#include "OsDrvSvu.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Lock utility by default wait forever to access the resource
#define OS_DRV_SVU_MUTEX_LOCK()           osDrvSvuLockCriticalSection(osDrvSvuControl.controlMutex, true, RTEMS_NO_TIMEOUT);
// Unlock utility
#define OS_DRV_SVU_MUTEX_UNLOCK()         osDrvSvuUnlockCriticalSection(osDrvSvuControl.controlMutex)

#define OS_DRV_SVU_MUTEX_LOCK_NBLK()      osDrvSvuLockCriticalSection(osDrvSvuControl.controlMutex, false, 0);

#define OS_DRV_SVU_LOCK(x)                osDrvSvuHandler[x]?protectionLock(x, osDrvSvuHandler[x]->protection):OS_MYR_DRV_NOTOPENED
#define OS_DRV_SVU_UNLOCK(x)              osDrvSvuHandler[x]?protectionUnlock(x, osDrvSvuHandler[x]->protection):OS_MYR_DRV_NOTOPENED


// Lock utility by default wait forever to access the resource
#ifdef OS_DRV_SVU_L1
    #define OS_DPRINTF1(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF1(...)
#endif

#ifdef OS_DRV_SVU_L2
    #define OS_DPRINTF2(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF2(...)
#endif

#ifdef OS_DRV_SVU_L3
    #define OS_DPRINTF3(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF3(...)
#endif

#define OS_DRV_SVU_NO_ID  0

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static struct {
    OS_MYRIAD_DRIVER_STATUS status;
    rtems_id controlMutex;
    u8 openedShavesNo;
} osDrvSvuControl;

osDrvSvuHandler_t* osDrvSvuHandler[TOTAL_NUM_SHAVES];

static osCommonIrqShareInfo_t osCommonIrqSharedInfo;

osDrvSvuHandler_t osDynHandler[TOTAL_NUM_SHAVES];

static int osDrvSvuExitCode;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static int OsDrvDynFindFreeShave(DynamicContext_t const *moduleContext);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
/* Enter Critical Section */
static int osDrvSvuLockCriticalSection(rtems_id sem, u32 wait, u32 wait_ticks)
{
    rtems_status_code status;
    int res;

    if (wait)
        status = rtems_semaphore_obtain(sem, RTEMS_WAIT, wait_ticks);
    else
        status = rtems_semaphore_obtain(sem, RTEMS_NO_WAIT, 0);

    switch (status)
    {
    case RTEMS_SUCCESSFUL:
        res = OS_MYR_DRV_SUCCESS;
        break;
    case RTEMS_TIMEOUT:
        res = OS_MYR_DRV_TIMEOUT;
        break;
    default:
        res = status;
        break;
    }
    OS_DPRINTF2("osDrvSvuLockCriticalSection res %d task %x wait %d\n", res,  rtems_task_self(), wait);
    return res;
}

/* Abandon Critical Section */
static rtems_status_code osDrvSvuUnlockCriticalSection(rtems_id sem)
{
    rtems_status_code status = rtems_semaphore_release(sem);
    int res;

    switch (status)
    {
    case RTEMS_SUCCESSFUL:
        res = OS_MYR_DRV_SUCCESS;
        break;
    case RTEMS_TIMEOUT:
        res = OS_MYR_DRV_TIMEOUT;
        break;
    default:
        res = status;
        break;
    }
    OS_DPRINTF2("osDrvSvuUnlockCriticalSection res %d task %x\n", res, rtems_task_self());
    return res;
}

int protectionLock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType)
{
    int res = OS_MYR_DRV_ERROR;

    switch (protectionType)
    {
    case OS_MYR_PROTECTION_SEM:
        res = osDrvSvuLockCriticalSection(osDrvSvuControl.controlMutex, true, RTEMS_NO_TIMEOUT);
        break;
    case OS_MYR_PROTECTION_SELF:
        if (osDrvSvuHandler[channel]->taskId == rtems_task_self())
        {
            res = OS_MYR_DRV_SUCCESS;
        }
        else if (osDrvSvuHandler[channel]->taskId == OS_DRV_SVU_NO_ID)
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        else
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        break;
    case OS_MYR_PROTECTION_CUSTOM_SEM:
        res = osDrvSvuLockCriticalSection(osDrvSvuHandler[channel]->shaveMutex, true, RTEMS_NO_TIMEOUT);
        break;
    default:
        if (osDrvSvuHandler[channel]->shaveState == OS_DRV_SVU_FREE)
        {
            // trying to lock a channel that was not opened
            res = OS_MYR_DRV_NOTOPENED;
        }
        else
        {
            // the channel was initialized with an unknown protection parameter
            res = OS_MYR_DRV_ERROR;
        }
        break;
    }
    return res;
}

int protectionUnlock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType)
{
    int res = OS_MYR_DRV_SUCCESS;

    switch (protectionType)
    {
    case OS_MYR_PROTECTION_SEM:
        res = osDrvSvuUnlockCriticalSection(osDrvSvuControl.controlMutex);
        break;
    case OS_MYR_PROTECTION_SELF:
        res = OS_MYR_DRV_SUCCESS;
        break;
    case OS_MYR_PROTECTION_CUSTOM_SEM:
        res = osDrvSvuUnlockCriticalSection(osDrvSvuHandler[channel]->shaveMutex);
        break;
    default:
        res = OS_MYR_DRV_ERROR;
        break;
    }
    return res;
}

void osDrvSvuIrqHandler(u32 source)
{
    u32 shaveNumber = source - IRQ_SVE_0;

    osDrvSvuExitCode = DrvSvutSwiTag(shaveNumber);

    OsDrvCommonInterruptSendEvent(osDrvSvuHandler[shaveNumber]->taskId, SHAVE_FINISH_EVENT(shaveNumber), &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_SHAVE, TRUE);
    //rtems_event_send(osDrvSvuHandler[shaveNumber]->taskId, SHAVE_FINISH_EVENT(shaveNumber));

    // Add the tracer functionality to visualize the shave run:complete
    dbgLogEvent((Event_t)((u32)LOG_EVENT_SHAVE_0_RUN + shaveNumber), 0, DEBUG_LOG_LEVEL_HIGH);
}

void updateTaskId(osDrvSvuHandler_t* handler)
{
    if (handler == NULL) return;
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_DRV_SVU_MUTEX_LOCK();
	if (res != OS_MYR_DRV_SUCCESS) return;

    if ((handler->protection == OS_MYR_PROTECTION_SEM) ||
        (handler->protection == OS_MYR_PROTECTION_CUSTOM_SEM))
    {
        // only needed when mutex protection is used; in this case functions can
        // be called from different threads, so taskId needs to be updated
        handler->taskId = rtems_task_self();
    }
    OS_DRV_SVU_MUTEX_UNLOCK();
}

int OsDrvSvuInit(void)
{
    int status;

    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    if (osDrvSvuControl.status == OS_DRV_STAT_UNINITIALIZED)
    {
        status = rtems_semaphore_create(rtems_build_name('_', 'S', 'E', 'M'), 1,
                RTEMS_BINARY_SEMAPHORE,0,(rtems_id *) &osDrvSvuControl.controlMutex);
        if (status == RTEMS_SUCCESSFUL)
        {
            osDrvSvuControl.status = OS_DRV_STAT_INITIALIZED;
            res = OS_MYR_DRV_SUCCESS;
        }
    }
    else if (osDrvSvuControl.status == OS_DRV_STAT_INITIALIZED)
    {
        res = OS_MYR_DRV_ALREADY_INITIALIZED;
    }

    OS_DPRINTF1("OsDrvSvuInit() res %d task %x\n", res, rtems_task_self());
    return res;
}

int OsDrvSvuOpenShave(osDrvSvuHandler_t* handler, u32 shaveNumber, OS_MYRIAD_DRIVER_PROTECTION protection)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    int status;

    res = OS_DRV_SVU_MUTEX_LOCK();
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if ( (osDrvSvuHandler[shaveNumber] != NULL) &&
                (osDrvSvuHandler[shaveNumber]->shaveState != OS_DRV_SVU_FREE))
        {
            res = OS_MYR_DRV_ALREADY_OPENED;
        }
        else
        {
            osDrvSvuHandler[shaveNumber] = handler;
            handler->shaveNumber = shaveNumber;
            handler->protection = protection;

            res = OS_MYR_DRV_SUCCESS;
            if (protection == OS_MYR_PROTECTION_CUSTOM_SEM)
            {
                status = rtems_semaphore_create( rtems_build_name('S', 'V', 'U', '0' + shaveNumber), 1,
                        RTEMS_SIMPLE_BINARY_SEMAPHORE, 1, &handler->shaveMutex);
                switch (status)
                {
                    case RTEMS_SUCCESSFUL:
                        res = OS_MYR_DRV_SUCCESS;
                        break;
                    case RTEMS_TIMEOUT:
                        res = OS_MYR_DRV_TIMEOUT;
                        break;
                    default:
                        res = OS_MYR_DRV_ERROR;
                        break;
                }
            }

            if (res == OS_MYR_DRV_SUCCESS)
            {
                handler->taskId = rtems_task_self();
                handler->shaveState = OS_DRV_SVU_TAKEN;
                osDrvSvuControl.openedShavesNo++;
            }
        }
        OS_DRV_SVU_MUTEX_UNLOCK();
    }

    OS_DPRINTF1("OsDrvSvuOpen() shave number %d res %d task %x\n", shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuOpenShaves(const swcShaveUnit_t *svuList, const uint32_t shavesInList, OS_MYRIAD_DRIVER_PROTECTION protection)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DYN_INFR_ERROR;
    uint32_t i, handlerIndex;

    for(i=0; i < shavesInList; i++)
    {
        handlerIndex = svuList[i];
        if (handlerIndex < TOTAL_NUM_SHAVES)
        {
            res = OsDrvSvuOpenShave(&osDynHandler[handlerIndex], svuList[i], protection);
            if (res != OS_MYR_DRV_SUCCESS)
                return res;
        } else
            res = OS_MYR_DYN_INFR_INVALID_PARAMETERS;
    }
    return res;
}

int OsDrvSvuGetShaveWindowRegs(osDrvSvuHandler_t* handler, u32* windows)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if ((handler->shaveState == OS_DRV_SVU_TAKEN) ||
            (handler->shaveState == OS_DRV_SVU_RUNNING))
    {
        swcGetShaveWindowRegs(handler->shaveNumber, windows);
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }

    OS_DPRINTF1("OsDrvSvuGetWinRegsShave() shave number %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuSetShaveWindowsToDefault(osDrvSvuHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (handler->shaveState == OS_DRV_SVU_TAKEN)
    {
        swcSetShaveWindowsToDefault(handler->shaveNumber);
    }
    else if (handler->shaveState == OS_DRV_SVU_RUNNING)
    {
        res = OS_MYR_DRV_RESOURCE_BUSY;
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }

    OS_DPRINTF1("OsDrvSvuSetShaveWindowsToDefault() shave number %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuSetShaveWindowsToAppSettings(osDrvSvuHandler_t* handler, DynamicContext_t* moduleStData)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (handler->shaveState == OS_DRV_SVU_TAKEN)
    {
        swcSetShaveWindowsToDefault(handler->shaveNumber);
        //Overwrite code and data windows
        swcSetShaveWindow(handler->shaveNumber,WINDOW_A_NO,CMX_BASE_ADR+handler->shaveNumber*CMX_SLICE_SIZE+moduleStData->cmxCriticalCodeSize);
        swcSetShaveWindow(handler->shaveNumber,WINDOW_B_NO,CMX_BASE_ADR+handler->shaveNumber*CMX_SLICE_SIZE);
    }
    else if (handler->shaveState == OS_DRV_SVU_RUNNING)
    {
        res = OS_MYR_DRV_RESOURCE_BUSY;
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }

    OS_DPRINTF1("OsDrvSvuSetShaveWindowsToDefault() shave number %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuSetShaveWindows(osDrvSvuHandler_t* handler, u32 windowA, u32 windowB, u32 windowC, u32 windowD)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (handler->shaveState == OS_DRV_SVU_TAKEN)
    {
        swcSetShaveWindows(handler->shaveNumber, windowA, windowB, windowC, windowD);
    }
    else if (handler->shaveState == OS_DRV_SVU_RUNNING)
    {
        res = OS_MYR_DRV_RESOURCE_BUSY;
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }

    OS_DPRINTF1("OsDrvSvuSetShaveWindows() shave number %d winA 0x%x winB"
            " 0x%x winC 0x%x winD 0x%x res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuSetWindowedDefaultStack(osDrvSvuHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (handler->shaveState == OS_DRV_SVU_TAKEN)
    {
        swcSetWindowedDefaultStack(handler->shaveNumber);
    }
    else if (handler->shaveState == OS_DRV_SVU_RUNNING)
    {
        res = OS_MYR_DRV_RESOURCE_BUSY;
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }

    OS_DPRINTF1("OsDrvSvuSetWindowedDefaultStack() shave number %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuSetAbsoluteDefaultStack(osDrvSvuHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (handler->shaveState == OS_DRV_SVU_TAKEN)
    {
        swcSetAbsoluteDefaultStack(handler->shaveNumber);
    }
    else if (handler->shaveState == OS_DRV_SVU_RUNNING)
    {
        res = OS_MYR_DRV_RESOURCE_BUSY;
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }

    OS_DPRINTF1("OsDrvSvuSetAbsoluteDefaultStack() shave number %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuRunShave(osDrvSvuHandler_t* handler, u32 entryPoint)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;
    u32 running, waitShaveRet = 0;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_TAKEN)
        {
            updateTaskId(handler);
            handler->shaveState = OS_DRV_SVU_RUNNING;
            swcShaveStartAsync(handler->shaveNumber, entryPoint, osDrvSvuIrqHandler);
            waitShaveRet = OsDrvSvuWaitShaves(1, handler, OS_DRV_SVU_WAIT_FOREVER, &running);
        }
        else if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvuRunShave() shave number %d entry point 0x%x res %d task %x\n", handler->shaveNumber, entryPoint, res, rtems_task_self());

    if (waitShaveRet == OS_MYR_DYN_INFR_ERROR_STACK_OVERFLOW)
    {
    	return OS_MYR_DYN_INFR_ERROR_STACK_OVERFLOW;
    }

    return res;
}

int OsDrvSvuStartShave(osDrvSvuHandler_t* handler, u32 entryPoint)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_TAKEN)
        {
            updateTaskId(handler);
            handler->shaveState = OS_DRV_SVU_RUNNING;
            swcShaveStartAsync(handler->shaveNumber, entryPoint, osDrvSvuIrqHandler);
        }
        else if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvuStartShave() shave number %d entry point 0x%x res %d task %x\n", handler->shaveNumber, entryPoint, res, rtems_task_self());
    return res;
}

static int OsDrvSvuDynStartShave(osDrvSvuHandler_t* handler, u32 Context)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_TAKEN)
        {
            updateTaskId(handler);
            handler->shaveState = OS_DRV_SVU_RUNNING;
            swcDynStartShave(handler->shaveNumber, Context);
        }
        else if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvuDynStartShave() shave number %d module data 0x%x res %d task %x\n", handler->shaveNumber, Context, res, rtems_task_self());
    return res;
}

int OsDrvSvuStartShaveCC(osDrvSvuHandler_t* handler, u32 entryPoint, const char *fmt, ...)
{
    va_list a_list;
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_TAKEN)
        {
            va_start(a_list, fmt);
            swcSetRegsCC(handler->shaveNumber, fmt, a_list);
            va_end(a_list);

            updateTaskId(handler);
            handler->shaveState = OS_DRV_SVU_RUNNING;
            swcShaveStartAsync(handler->shaveNumber, (u32)entryPoint, osDrvSvuIrqHandler);
        }
        else if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvuStartShaveCC() shave number %d entry point 0x%x res %d task %x\n", handler->shaveNumber, entryPoint, res, rtems_task_self());
    return res;
}

int OsDrvSvuSetupShaveCC(osDrvSvuHandler_t* handler, const char *fmt, ...)
{
    va_list a_list;
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_TAKEN)
        {
            va_start(a_list, fmt);
            swcSetRegsCC(handler->shaveNumber, fmt, a_list);
            va_end(a_list);
        }
        else if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvuSetupShaveCC() shave number %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

//XXX there is another function ResetShaveLite
int OsDrvSvuResetShave(osDrvSvuHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (handler->shaveState == OS_DRV_SVU_TAKEN)
    {
        swcResetShave(handler->shaveNumber);
    }
    else if (handler->shaveState == OS_DRV_SVU_RUNNING)
    {
        res = OS_MYR_DRV_RESOURCE_BUSY;
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }

    OS_DPRINTF1("OsDrvSvuResetShave() shave number %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

static inline int processEvents(u32 allEvents, u32 receivedEvents, u32* remainingEvents)
{
    u32 shaveNumber = 0;
    u32 res = OS_MYR_DRV_SUCCESS;
    u32 status = OS_MYR_DRV_SUCCESS;

    *remainingEvents = allEvents;
    while (receivedEvents > 0)
    {
        if (receivedEvents & 0x1)
        {
            status = OS_DRV_SVU_LOCK(shaveNumber);
            if (status == OS_MYR_DRV_SUCCESS)
            {
                osDrvSvuHandler[shaveNumber]->shaveState = OS_DRV_SVU_TAKEN;
                OS_DRV_SVU_UNLOCK(shaveNumber);
            }
            else
            {
                res = status;
            }
            *remainingEvents &= ~(1 << shaveNumber);
        }
        receivedEvents >>= 1;
        shaveNumber++;
    }
    return res;
}

int OsDrvSvuWaitShaves(u32 noOfShaves, osDrvSvuHandler_t* shaveList, u32 timeout, u32 *runningShaves)
{
    rtems_status_code sc;
    rtems_event_set eventsOut = 0;
    u32 i;
    u32 shaves = 0;
    u32 procShaves = 0;
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    if(timeout != OS_DRV_SVU_WAIT_FOREVER && runningShaves == NULL)
        return OS_MYR_DRV_ERROR;

    if (osDrvSvuControl.status == OS_DRV_STAT_UNINITIALIZED)
    {
        return OS_MYR_DRV_NOTOPENED;
    }

    for (i = 0; i < noOfShaves; i++)
    {
        if (shaveList[i].shaveState == OS_DRV_SVU_RUNNING)
        {
            if(shaveList[i].taskId == rtems_task_self())
            {
                shaves |= (1 << shaveList[i].shaveNumber);
            }
            else
            {
                if (runningShaves)
                    // return the first shave for which the wait fails
                    *runningShaves = (1 << shaveList[i].shaveNumber);

                return OS_MYR_RESOURCE_NOT_OWNED;
            }
        }
    }

    // Add the tracer functionality to visualize the shave wait:complete
    for (i = 0; i < noOfShaves; i++)
        dbgLogEvent((Event_t)((u32)LOG_EVENT_WAIT_FOR_SHAVE_0 +  shaveList[i].shaveNumber), 1, DEBUG_LOG_LEVEL_HIGH);

    if (shaves == 0)
    {
        res = OS_MYR_DRV_SUCCESS;
    }
    else
    {
        switch(timeout)
        {
        case OS_DRV_SVU_WAIT_FOREVER:
            sc = OsCommonEventReceive(shaves, FALSE, TRUE, RTEMS_NO_TIMEOUT, &eventsOut, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_SHAVE);
            break;
        case 0:
            sc = OsCommonEventReceive(shaves, FALSE, FALSE, 0, &eventsOut, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_SHAVE);
            break;
        default:
            sc = OsCommonEventReceive(shaves, FALSE, TRUE, timeout, &eventsOut, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_SHAVE);
            break;
        }

        procShaves = shaves;

        if (sc == RTEMS_SUCCESSFUL)
        {
            res = processEvents(shaves, eventsOut, &procShaves);
        }
        else if (sc == RTEMS_TIMEOUT)
        {
            OsCommonEventReceive(shaves, FALSE, FALSE, 0, &eventsOut, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_SHAVE);
            // only process the finished events
            processEvents(shaves, shaves & eventsOut, &procShaves);
            res = OS_MYR_DRV_TIMEOUT;
        }
        else
        {
            res = sc;
        }

        if(osDrvSvuExitCode == SHAVEExitStackOverflow)
        {
        	res = OS_MYR_DYN_INFR_ERROR_STACK_OVERFLOW;
        }
    }

    if (runningShaves)
        *runningShaves = procShaves;

    // Add the tracer functionality to visualize the shave wait:complete
    for (i = 0; i < noOfShaves; i++)
        dbgLogEvent((Event_t)((u32)LOG_EVENT_WAIT_FOR_SHAVE_0 +  shaveList[i].shaveNumber), 0, DEBUG_LOG_LEVEL_HIGH);

    return res;
}

int OsDrvSvuDynWaitShaves(const swcShaveUnit_t *svuList, const uint32_t shavesInList, u32 timeout, u32 *runningShaves)
{
    uint32_t i;
    OS_MYRIAD_DRIVER_STATUS_CODE res;
    for (i=0; i<shavesInList; i++)
    {
        res = OsDrvSvuWaitShaves(1, &osDynHandler[svuList[i]], timeout, runningShaves);
        if (res != OS_MYR_DRV_SUCCESS)
            return res;
    }
    return res;
}

int OsDrvSvuSetRounding(osDrvSvuHandler_t* handler, u32 roundingBits)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_TAKEN)
        {
            swcSetRounding(handler->shaveNumber, roundingBits);
        }
        else if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }

    OS_DPRINTF1("OsDrvSvuSetRounding() shave number %d rounding bits 0x%x res %d task %x\n", handler->shaveNumber, roundingBits, res, rtems_task_self());
    return res;
}

int OsDrvSvuSolveShaveRelAddr(u32 inAddr, u32 shaveNumber, u32* outAddr)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_SVU_LOCK(shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        *outAddr = swcSolveShaveRelAddr(inAddr, shaveNumber);
        OS_DRV_SVU_UNLOCK(shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvuSolveShaveRelAddr() shave number %d in addr 0x%x out addr 0x%x res %d task %x\n",
            shaveNumber, inAddr, *outAddr, res, rtems_task_self());
    return res;
}

int OsDrvSvuL1DataCacheCtrl(osDrvSvuHandler_t* handler, u32 action)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_TAKEN)
        {
            DrvSvuL1DataCacheCtrl(handler->shaveNumber, action);
        }
        else if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvuL1DataCacheCtrl() shave number %d action %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuL1InstrCacheCtrl(osDrvSvuHandler_t* handler, u32 action)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_TAKEN)
        {
            DrvSvuL1InstrCacheCtrl(handler->shaveNumber, action);
        }
        else if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvuL1InstrCacheCtrl() shave number %d action %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuStopShaves(const swcShaveUnit_t *svuList, const uint32_t shavesInList)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DYN_INFR_ERROR;
    uint32_t i, handlerIndex;

    for(i=0; i<shavesInList; i++)
    {
        handlerIndex = svuList[i];
        if (handlerIndex < TOTAL_NUM_SHAVES)
        {
            res = OsDrvSvuStop(&osDynHandler[handlerIndex]);
            if (res != OS_MYR_DRV_SUCCESS)
                return res;
        } else
            res = OS_MYR_DYN_INFR_INVALID_PARAMETERS;
    }
    return res;
}

int OsDrvSvuStop(osDrvSvuHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            DrvSvutStop(handler->shaveNumber);
            handler->shaveState = OS_DRV_SVU_TAKEN;
        }
        else if (handler->shaveState == OS_DRV_SVU_FREE)
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }
    OS_DPRINTF1("OsDrvSvutStop() shave number %d res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuCloseShave(osDrvSvuHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    res = OS_DRV_SVU_LOCK(handler->shaveNumber);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (handler->shaveState == OS_DRV_SVU_RUNNING)
        {
            res =  OS_MYR_DRV_RESOURCE_BUSY;
        }
        else if (handler->shaveState == OS_DRV_SVU_FREE)
        {
            res =  OS_MYR_DRV_NOTOPENED;
        }
        else
        {
            res = OS_DRV_SVU_MUTEX_LOCK();
            if (res == OS_MYR_DRV_SUCCESS)
            {
                handler->shaveState = OS_DRV_SVU_FREE;
                osDrvSvuControl.openedShavesNo--;
                if (handler->protection == OS_MYR_PROTECTION_CUSTOM_SEM)
                {
                    rtems_semaphore_delete(handler->shaveMutex);
                }
                OS_DRV_SVU_MUTEX_UNLOCK();
            }
        }
        OS_DRV_SVU_UNLOCK(handler->shaveNumber);
    }

    OS_DPRINTF1("OsDrvSvuCloseShave() shave number res %d task %x\n", handler->shaveNumber, res, rtems_task_self());
    return res;
}

int OsDrvSvuCloseShaves(const swcShaveUnit_t *svuList, const uint32_t shavesInList)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DYN_INFR_ERROR;
    uint32_t i, handlerIndex;

    for(i=0; i<shavesInList; i++)
    {
        handlerIndex = svuList[i];
        if (handlerIndex < TOTAL_NUM_SHAVES)
        {
            res = OsDrvSvuCloseShave(&osDynHandler[handlerIndex]);
            if (res != OS_MYR_DRV_SUCCESS)
                return res;
        } else
            res = OS_MYR_DYN_INFR_INVALID_PARAMETERS;
    }
    return res;
}

int OsDrvSvuClose(void)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    u32 i;

    res = OS_DRV_SVU_MUTEX_LOCK();
    if (res == OS_MYR_DRV_SUCCESS)
    {
        // check the number of opened shaves. If all the shaves are closed
        // then it is safe to release the driver mutex
        if (osDrvSvuControl.openedShavesNo == 0)
        {
            osDrvSvuControl.status = OS_DRV_STAT_UNINITIALIZED;
            for (i = 0; i < TOTAL_NUM_SHAVES; i++)
            {
                osDrvSvuHandler[i] = NULL;
            }
            // mutex can not be released while it is taken
            OS_DRV_SVU_MUTEX_UNLOCK();
            rtems_semaphore_delete(osDrvSvuControl.controlMutex);
            res = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            OS_DRV_SVU_MUTEX_UNLOCK();
            // some shaves are still opened
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
    }
    OS_DPRINTF1("OsDrvSvuClose() res %d task %x\n", res, rtems_task_self());
    return res;
}

int OsDrvSvuLoadShvdlib(u8 *startAddr, osDrvSvuHandler_t* handler)
{
    updateTaskId(handler);
    swcLoadshvdlib(startAddr, handler->shaveNumber);
    OS_DPRINTF1("OsDrvSvuLoadShvdlib() shave number %d task %x\n", handler->shaveNumber, rtems_task_self());
    return OS_MYR_DRV_SUCCESS;
}

int OsDrvSvuLoadDynLibrary(u8 *startAddr, osDrvSvuHandler_t* handler, context_t contextType)
{
	uint32_t uiStartMemAddress = 0;
	uint32_t uiFlushLength = 0;
	updateTaskId(handler);
	swcLoadDynLibraryCacheAware(startAddr, handler->shaveNumber, contextType, &uiStartMemAddress, &uiFlushLength);
	//Flush the L1 and L2 leon caches in order to have the data available for the shave when it starts
	rtems_cache_flush_multiple_data_lines((const void *)uiStartMemAddress,(size_t) uiFlushLength);
	OS_DPRINTF1("OsDrvSvuLoadShvdlib() shave number %d task %x\n", handler->shaveNumber, rtems_task_self());
	return OS_MYR_DRV_SUCCESS;
}

int OsDrvSvuSetupDynShaveApps(DynamicContext_t *mData, const swcShaveUnit_t *svuList, const uint32_t instances)
{
    s32 status;

    OS_DRV_SVU_MUTEX_LOCK();

    for(uint32_t i=0; i < instances; i++){
        //Clear all interrupts so we can have the next interrupt happening
        SET_REG_WORD(DCU_IRR((u32)svuList[i]), (u32)(DCU_IRR_FPEIC_MASK | DCU_IRR_IEIC_MASK | DCU_IRR_IOIC_MASK | DCU_IRR_RMOIC_MASK | DCU_IRR_DWIIC_MASK));

        //Disable ICB (Interrupt Control Block) while setting new interrupt
        DrvIcbDisableIrq(IRQ_SVE_0 + svuList[i]);
        DrvIcbIrqClear(IRQ_SVE_0 + svuList[i]);
        DrvIcbIrqUnsetup(IRQ_SVE_0 + svuList[i]);
        //Configure interrupt handlers
        DrvIcbSetIrqHandler(IRQ_SVE_0 + svuList[i], osDrvSvuIrqHandler);
        //Enable interrupts on SHAVE done
        DrvIcbConfigureIrq(IRQ_SVE_0 + svuList[i], SHAVE_INTERRUPT_LEVEL, POS_EDGE_INT);
        DrvIcbEnableIrq(IRQ_SVE_0 + svuList[i]);

        //Enable SWIH IRQ sources
        SET_REG_WORD(DCU_ICR((u32)svuList[i]), ICR_SWI_ENABLE);
    }

    status = swcSetupDynShaveApps(mData,svuList,instances);
    OS_DPRINTF1("OsDrvSvuSetupDynShaveApps() status of function call swcSetupDynShaveApps(mData,svuList,instances) = %d task %x\n", (int)status, (unsigned int)rtems_task_self());
    OS_DRV_SVU_MUTEX_UNLOCK();

    return status;
}

s32 OsDrvDynShaveAppSetWindows(DynamicContext_t *moduleStData, u32 cmxCriticalCodeSize)
{
    s32 status;
    status=swcDynShaveAppSetWindows(moduleStData,cmxCriticalCodeSize);
    return status;
}

int OsDrvSvuCleanupDynShaveApps(DynamicContext_t *mData)
{
    s32 status;
    status = swcCleanupDynShaveApps(mData);
    return status;
}

static int OsDrvSvuSetupAndStartDynApp(osDrvSvuHandler_t* handler, DynamicContext_t *moduleStData)
{
    int osRetCod;

    osRetCod = OsDrvSvuSetShaveWindowsToAppSettings(handler,moduleStData);
    if (osRetCod != OS_MYR_DRV_SUCCESS)
        return osRetCod;

    /*0x1Exxxxxx window needs to be set to the module group address before we load data*/
    swcSetShaveWindow(handler->shaveNumber, WINDOW_C_NO, (u32)moduleStData->instancesData->GrpDataPools[handler->shaveNumber]);
    //And set data and code windows according to code size needs
    swcSetShaveWindow(handler->shaveNumber,WINDOW_A_NO,(u32)moduleStData->appdyndataAllocAddr[handler->shaveNumber]);
    swcSetShaveWindow(handler->shaveNumber,WINDOW_B_NO,(u32)moduleStData->cmxCriticalCodeAllocAddr[handler->shaveNumber]);

    //Load the data side
    osRetCod = OsDrvSvuLoadDynLibrary((u8*)moduleStData->appdyndata, handler, SHVXDATA);
    if (osRetCod != OS_MYR_DRV_SUCCESS)
        return osRetCod;
    //Load the library group data
    osRetCod = OsDrvSvuLoadDynLibrary((u8*)moduleStData->groupappdyndata, handler, SHVZDATA);
    if (osRetCod != OS_MYR_DRV_SUCCESS)
        return osRetCod;

    osRetCod = OsDrvSvuSetWindowedDefaultStack(handler);
    if (osRetCod != OS_MYR_DRV_SUCCESS)
        return osRetCod;

    osRetCod = OsDrvSvuDynStartShave(handler, (u32)moduleStData);
    return osRetCod;
}

static int OsDrvDynFindFreeShave(DynamicContext_t const *moduleContext)
{
    u32 i=0;
    int shaveNumber = -1;

    if (moduleContext != NULL) {
        while (i < moduleContext->instancesData->appInstances)
        {
            /* search configured shaves to be used */
            if (osDynHandler[moduleContext->instancesData->shaveList[i]].shaveState == OS_DRV_SVU_TAKEN){
                shaveNumber = moduleContext->instancesData->shaveList[i];
                return shaveNumber;
            } /* else continue searching */
            i++;
        }
    }
    return shaveNumber;
}

int OsDrvSvuRunShaveAlgo(DynamicContext_t *moduleStData, int * const shvNumber)
{
    int osRetCod = OS_MYR_DYN_INFR_ERROR;

    if (shvNumber != NULL)
    {
        *shvNumber = OsDrvDynFindFreeShave(moduleStData);
        if ( (*shvNumber >= 0) && (*shvNumber < TOTAL_NUM_SHAVES)) {
            osRetCod = OsDrvSvuResetShave(&osDynHandler[*shvNumber]);
            if (osRetCod == OS_MYR_DRV_SUCCESS)
                osRetCod = OsDrvSvuSetupAndStartDynApp(&osDynHandler[*shvNumber], moduleStData);
        } else
            osRetCod = OS_MYR_DYN_INFR_RUNALGO_SHAVES_BUSY;

    } else
        osRetCod = OS_MYR_DYN_INFR_INVALID_PARAMETERS;
    return osRetCod;
}

int OsDrvSvuRunShaveAlgoCC(DynamicContext_t *moduleStData, int * const shvNumber, const char *fmt, ...)
{
    int osRetCod = OS_MYR_DYN_INFR_ERROR;
    va_list a_list;

    if (shvNumber != NULL)
    {
        *shvNumber = OsDrvDynFindFreeShave(moduleStData);
        if ( (*shvNumber >= 0) && (*shvNumber < TOTAL_NUM_SHAVES)) {
            osRetCod = OsDrvSvuResetShave(&osDynHandler[*shvNumber]);
            va_start(a_list, fmt);
            swcSetRegsCC(osDynHandler[*shvNumber].shaveNumber, fmt, a_list);
            va_end(a_list);
            if (osRetCod == OS_MYR_DRV_SUCCESS)
                osRetCod = OsDrvSvuSetupAndStartDynApp(&osDynHandler[*shvNumber], moduleStData);
        } else
            osRetCod = OS_MYR_DYN_INFR_RUNALGO_SHAVES_BUSY;
    } else
        osRetCod = OS_MYR_DYN_INFR_INVALID_PARAMETERS;

    return osRetCod;
}

int OsDrvSvuRunShaveAlgoOnAssignedShave(DynamicContext_t *moduleStData, u32 shvNumber)
{
    int osRetCod;

    if (swcCheckFreeAndValidShave(moduleStData, shvNumber) == 0)
        osRetCod = OS_MYR_DYN_INFR_SHAVE_BUSY_OR_INVALID;
    else
    {
        osRetCod = OsDrvSvuResetShave(&osDynHandler[shvNumber]);
        if (osRetCod == OS_MYR_DRV_SUCCESS)
            osRetCod = OsDrvSvuSetupAndStartDynApp(&osDynHandler[shvNumber], moduleStData);
    }
    return osRetCod;
}

int OsDrvSvuRunShaveAlgoOnAssignedShaveCC(DynamicContext_t *moduleStData, u32 shvNumber, const char *fmt, ...)
{
    int osRetCod;
    va_list a_list;

    if (swcCheckFreeAndValidShave(moduleStData, shvNumber) == 0)
        osRetCod = OS_MYR_DYN_INFR_SHAVE_BUSY_OR_INVALID;
    else
    {
        osRetCod = OsDrvSvuResetShave(&osDynHandler[shvNumber]);
        if (osRetCod == OS_MYR_DRV_SUCCESS)
        {
            va_start(a_list, fmt);
            swcSetRegsCC(osDynHandler[shvNumber].shaveNumber, fmt, a_list);
            va_end(a_list);
            osRetCod = OsDrvSvuSetupAndStartDynApp(&osDynHandler[shvNumber], moduleStData);
        }
    }
    return osRetCod;
}

int OsDrvRequestUnallocatedShaves(swcShaveUnit_t *svuList, u32 shvNumber)
{
    int status;

    OS_DRV_SVU_MUTEX_LOCK();
    status = swcRequestUnallocatedShaves(svuList, shvNumber);
    OS_DRV_SVU_MUTEX_UNLOCK();

    return status;
}

int OsDrvGetUnallocatedShavesNumber(void)
{
    int shavesNumber;
    shavesNumber = swcGetUnallocatedShavesNumber();
    return shavesNumber;
}

int OsDrvSvuCleanupDynShaveListApps(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber)
{
    int status;
    status = swcCleanupDynShaveListApps(moduleStData, svuList, shavesNumber);
    return status;
}

// 7: Shave Paradigm Functions Implementation
// ----------------------------------------------------------------------------

int OsDrvIsoSetupShaveApplication(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber, MISA_PARADIGM_TYPE paradigmType)
{
    int osRetCod = 0;
    uint32_t breakPosition = shavesNumber;

    for (uint32_t i = 0; i < shavesNumber; i++) {
        osRetCod = OS_DRV_SVU_LOCK((&osDynHandler[svuList[i]])->shaveNumber);

        if (osRetCod == OS_MYR_DRV_SUCCESS) {
            if ((&osDynHandler[svuList[i]])->shaveState == OS_DRV_SVU_TAKEN) {
                updateTaskId(&osDynHandler[svuList[i]]);
                (&osDynHandler[svuList[i]])->shaveState = OS_DRV_SVU_RUNNING;
            } else if ((&osDynHandler[svuList[i]])->shaveState == OS_DRV_SVU_RUNNING) {
                OS_DRV_SVU_UNLOCK((&osDynHandler[svuList[i]])->shaveNumber);
                osRetCod = OS_MYR_DRV_RESOURCE_BUSY;
            } else {
                OS_DRV_SVU_UNLOCK((&osDynHandler[svuList[i]])->shaveNumber);
                osRetCod = OS_MYR_DRV_NOTOPENED;
            }
        } else {
            breakPosition = i;
            break;
        }
    }

    if(osRetCod == OS_MYR_DRV_SUCCESS) {
        osRetCod = swcIsoSetupShaveApplication(moduleStData, svuList, shavesNumber, paradigmType);

        for (uint32_t i = 0; i < shavesNumber; i++) {
            //Clear all interrupts so we can have the next interrupt happening
            SET_REG_WORD(DCU_IRR((u32 )svuList[i]),
                    (u32 )(DCU_IRR_FPEIC_MASK | DCU_IRR_IEIC_MASK | DCU_IRR_IOIC_MASK | DCU_IRR_RMOIC_MASK | DCU_IRR_DWIIC_MASK));

            //Disable ICB (Interrupt Control Block) while setting new interrupt
            DrvIcbDisableIrq(IRQ_SVE_0 + svuList[i]);
            DrvIcbIrqClear(IRQ_SVE_0 + svuList[i]);
            DrvIcbIrqUnsetup(IRQ_SVE_0 + svuList[i]);
            //Configure interrupt handlers
            DrvIcbSetIrqHandler(IRQ_SVE_0 + svuList[i], osDrvSvuIrqHandler);
            //Enable interrupts on SHAVE done
            DrvIcbConfigureIrq(IRQ_SVE_0 + svuList[i], SHAVE_INTERRUPT_LEVEL, POS_EDGE_INT);
            DrvIcbEnableIrq(IRQ_SVE_0 + svuList[i]);

            //Enable SWIH IRQ sources
            SET_REG_WORD(DCU_ICR((u32 )svuList[i]), ICR_SWI_ENABLE);
        }
    }

    for (uint32_t i = 0; i < breakPosition; i++) {
        (&osDynHandler[svuList[i]])->shaveState = OS_DRV_SVU_TAKEN;
        OS_DRV_SVU_UNLOCK((&osDynHandler[svuList[i]])->shaveNumber);
    }

    OS_DPRINTF1("OsDrvSvuDynStartShave() shave number %d module data 0x%x res %d task %x\n", handler->shaveNumber, Context, res, rtems_task_self());
    return osRetCod;
}

int OsDrvStartEntryPointDC(DynamicContext_t *moduleStData, uint32_t shaveNumber, const char* fName)
{
    int osRetCod;

    osRetCod = OS_DRV_SVU_LOCK((&osDynHandler[shaveNumber])->shaveNumber);

    if (osRetCod == OS_MYR_DRV_SUCCESS) {
        if ((&osDynHandler[shaveNumber])->shaveState == OS_DRV_SVU_TAKEN) {
            updateTaskId(&osDynHandler[shaveNumber]);
            (&osDynHandler[shaveNumber])->shaveState = OS_DRV_SVU_RUNNING;

            osRetCod = swcStartEntryPointDC(moduleStData, shaveNumber, fName);
            if (osRetCod != OS_MYR_DYN_INFR_SUCCESS) {
                (&osDynHandler[shaveNumber])->shaveState = OS_DRV_SVU_TAKEN;
            }

        } else if ((&osDynHandler[shaveNumber])->shaveState == OS_DRV_SVU_RUNNING) {
            osRetCod = OS_MYR_DRV_RESOURCE_BUSY;
        } else {
            osRetCod = OS_MYR_DRV_NOTOPENED;
        }
    }

    OS_DRV_SVU_UNLOCK((&osDynHandler[shaveNumber])->shaveNumber);

    return osRetCod;
}

int OsDrvStartEntryPointDCCC(DynamicContext_t *moduleStData, uint32_t shaveNumber, const char* fName, const char *fmt, ...)
{
    int osRetCod;
    va_list a_list;

    osRetCod = OS_DRV_SVU_LOCK((&osDynHandler[shaveNumber])->shaveNumber);

    if (osRetCod == OS_MYR_DRV_SUCCESS) {
        if ((&osDynHandler[shaveNumber])->shaveState == OS_DRV_SVU_TAKEN) {
            updateTaskId(&osDynHandler[shaveNumber]);
            (&osDynHandler[shaveNumber])->shaveState = OS_DRV_SVU_RUNNING;

            va_start(a_list, fmt);
            swcSetRegsCC(osDynHandler[shaveNumber].shaveNumber, fmt, a_list);
            va_end(a_list);

            osRetCod = swcStartEntryPointDC(moduleStData, shaveNumber, fName);
            if (osRetCod != OS_MYR_DYN_INFR_SUCCESS) {
                (&osDynHandler[shaveNumber])->shaveState = OS_DRV_SVU_TAKEN;
            }

        } else if ((&osDynHandler[shaveNumber])->shaveState == OS_DRV_SVU_RUNNING) {
            osRetCod = OS_MYR_DRV_RESOURCE_BUSY;
        } else
            osRetCod = OS_MYR_DRV_NOTOPENED;
    }

    OS_DRV_SVU_UNLOCK((&osDynHandler[shaveNumber])->shaveNumber);

    return osRetCod;
}

int OsDrvStartFC(DynamicContext_t *moduleStData, uint32_t shaveNumber)
{
    int osRetCod;

    osRetCod = OS_DRV_SVU_LOCK((&osDynHandler[shaveNumber])->shaveNumber);

    if (osRetCod == OS_MYR_DRV_SUCCESS) {
        if ((&osDynHandler[shaveNumber])->shaveState == OS_DRV_SVU_TAKEN) {
            updateTaskId(&osDynHandler[shaveNumber]);
            (&osDynHandler[shaveNumber])->shaveState = OS_DRV_SVU_RUNNING;

            osRetCod = swcStartFC(moduleStData, shaveNumber);
            if (osRetCod != OS_MYR_DYN_INFR_SUCCESS) {
                (&osDynHandler[shaveNumber])->shaveState = OS_DRV_SVU_TAKEN;
            }

        } else if ((&osDynHandler[shaveNumber])->shaveState == OS_DRV_SVU_RUNNING) {
            osRetCod = OS_MYR_DRV_RESOURCE_BUSY;
        } else {
            osRetCod = OS_MYR_DRV_NOTOPENED;
        }
    }

    OS_DRV_SVU_UNLOCK((&osDynHandler[shaveNumber])->shaveNumber);

    return osRetCod;
}

int OsDrvIsoCleanShaveApplication(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber, MISA_PARADIGM_TYPE paradigmType)
{
    int osRetCod = 0;
    uint32_t breakPosition = shavesNumber;

    for (uint32_t i = 0; i < shavesNumber; i++) {
        osRetCod = OS_DRV_SVU_LOCK((&osDynHandler[svuList[i]])->shaveNumber);

        if (osRetCod == OS_MYR_DRV_SUCCESS) {
            if ((&osDynHandler[svuList[i]])->shaveState == OS_DRV_SVU_TAKEN) {
                updateTaskId(&osDynHandler[svuList[i]]);
                (&osDynHandler[svuList[i]])->shaveState = OS_DRV_SVU_RUNNING;
            } else if ((&osDynHandler[svuList[i]])->shaveState == OS_DRV_SVU_RUNNING) {
                osRetCod = OS_MYR_DRV_RESOURCE_BUSY;
            } else {
                osRetCod =  OS_MYR_DRV_NOTOPENED;
            }
        } else {
            breakPosition = i;
            break;
        }
    }

    if(osRetCod == OS_MYR_DRV_SUCCESS)
        osRetCod = swcIsoCleanShaveApplication(moduleStData, svuList, shavesNumber, paradigmType);

    for (uint32_t i = 0; i < breakPosition; i++) {
        (&osDynHandler[svuList[i]])->shaveState = OS_DRV_SVU_TAKEN;
        OS_DRV_SVU_UNLOCK((&osDynHandler[svuList[i]])->shaveNumber);
    }

    return osRetCod;
}

//Temporary workaround
int OsDrvprotectionLock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType)
{
    return protectionLock(channel,protectionType);
}

int OsDrvprotectionUnlock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType)
{
   return protectionUnlock(channel,protectionType);
}

void OsDrvSvuIrqHandler(u32 source)
{
    osDrvSvuIrqHandler(source);
}

void OsDrvupdateTaskId(osDrvSvuHandler_t* handler)
{
    updateTaskId(handler);
}
