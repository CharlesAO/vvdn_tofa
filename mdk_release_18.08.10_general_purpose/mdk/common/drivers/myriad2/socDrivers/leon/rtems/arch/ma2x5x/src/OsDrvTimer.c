///
/// @file OsDrvTimer.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvTimer
/// @{
/// @brief     Timer API RTEMS Functions.
///


/// 1: Includes
/// ----------------------------------------------------------------------------
#include <stdio.h>
#include <bsp.h>
#include <bsp/irq-generic.h>
#include "assert.h"
#include "DrvIcb.h"
#include "DrvRegUtils.h"
#include "swcWhoAmI.h"
#include "OsDrvTimer.h"
#include "OsDrvCpr.h"
#include "OsCross.h"
#include <rtems.h>


extern __BSP_CLOCK_CONFIG__ BSP_CLOCK_CONFIG;

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// Lock utility by default wait forever to access the resource
#define OS_DRV_TIMER_MUTEX_LOCK()           osDrvTimerLockCriticalSection(osDrvTimerControl.controlMutex, true, RTEMS_NO_TIMEOUT);
// Unlock utility
#define OS_DRV_TIMER_MUTEX_UNLOCK()         osDrvTimerUnlockCriticalSection(osDrvTimerControl.controlMutex)

#define OS_DRV_TIMER_LOCK(x)                osDrvTimerHandler[x]?protectionLock(x, osDrvTimerHandler[x]->protection):OS_MYR_DRV_NOTOPENED
#define OS_DRV_TIMER_UNLOCK(x)              osDrvTimerHandler[x]?protectionUnlock(x, osDrvTimerHandler[x]->protection):OS_MYR_DRV_NOTOPENED


#ifdef OS_DRV_TIMER_L1
    #define OS_DPRINTF1(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF1(...)
#endif

#ifdef OS_DRV_TIMER_L2
    #define OS_DPRINTF2(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF2(...)
#endif

#ifdef OS_DRV_TIMER_L3
    #define OS_DPRINTF3(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF3(...)
#endif

#define D_TIMER_CFG_ENABLE          (1 << 0)
#define D_TIMER_CFG_RESTART         (1 << 1)
#define D_TIMER_CFG_EN_INT          (1 << 2)
#define D_TIMER_CFG_CHAIN           (1 << 3)
#define D_TIMER_CFG_IRQ_PENDING     (1 << 4)
#define D_TIMER_CFG_FORCE_RELOAD    (1 << 5)

#define LAST_AVAILABLE_TIMER (NUM_TIMERS - 1)
#define OS_DRV_TIMER_UNCHAINED  0
#define OS_DRV_TIMER_CHAINED_LO 1
#define OS_DRV_TIMER_CHAINED_HI 2

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static struct {
    u32 timerBaseAddress;
    rtems_id controlMutex;      //access control
    OS_MYRIAD_DRIVER_STATUS status;
    u8 openedTimersNo;          // the number of opened timers
} osDrvTimerControl;

static struct {
    OS_MYRIAD_DRIVER_STATUS status;
    rtems_id  taskId;
    u32       defaultSysRstTicksCount;
    OsDrvWatchDogCallback_t  callbackFunc;
} osDrvWatchdogControl;

static struct {
    rtems_id taskId;
    rtems_id timerMutex;
    u8 chained;
    osDrvTimerStatus_t status;
} osDrvTimer[NUM_TIMERS];

osDrvTimerHandler_t* osDrvTimerHandler[NUM_TIMERS];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static void osDrvTimerIrqHandler(u32 source);
static void osDrvTimerWDIrqHandler(u32 source);
static void osDrvTimerWatchDogInterruptClear(void);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

/* Enter Critical Section */
static int osDrvTimerLockCriticalSection(rtems_id sem, u32 wait, u32 wait_ticks)
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
    case RTEMS_INVALID_ID:
        // mutex id was not created, because the driver was not initalized
        res = OS_MYR_DRV_NOTOPENED;
        break;
    default:
        res = status;
        break;
    }
    OS_DPRINTF2("osDrvTimerLockCriticalSection res %d wait %d\n", res, wait);
    return res;
}

/* Abandon Critical Section */
static rtems_status_code osDrvTimerUnlockCriticalSection(rtems_id sem)
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
    OS_DPRINTF2("osDrvTimerUnlockCriticalSection res %d\n", res);
    return res;
}

static int protectionLock(u32 timer, OS_MYRIAD_DRIVER_PROTECTION protectionType)
{
    int res = OS_MYR_DRV_ERROR;

    switch (protectionType)
    {
    case OS_MYR_PROTECTION_SEM:
        res = osDrvTimerLockCriticalSection(osDrvTimerControl.controlMutex, true, RTEMS_NO_TIMEOUT);
        break;
    case OS_MYR_PROTECTION_SELF:
        if (osDrvTimerHandler[timer]->status == OS_DRV_TIMER_FREE)
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        else if (osDrvTimer[timer].taskId == rtems_task_self())
        {
            res = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        break;
    case OS_MYR_PROTECTION_CUSTOM_SEM:
        res = osDrvTimerLockCriticalSection(osDrvTimer[timer].timerMutex, true, RTEMS_NO_TIMEOUT);
        break;
    default:
        if (osDrvTimerHandler[timer]->status == OS_DRV_TIMER_FREE)
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

static int protectionUnlock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType)
{
    int res = OS_MYR_DRV_SUCCESS;

    switch (protectionType)
    {
    case OS_MYR_PROTECTION_SEM:
        res = osDrvTimerUnlockCriticalSection(osDrvTimerControl.controlMutex);
        break;
    case OS_MYR_PROTECTION_SELF:
        res = OS_MYR_DRV_SUCCESS;
        break;
    case OS_MYR_PROTECTION_CUSTOM_SEM:
        res = osDrvTimerUnlockCriticalSection(osDrvTimer[channel].timerMutex);
        break;
    default:
        res = OS_MYR_DRV_ERROR;
        break;
    }
    return res;
}

static int osDrvTimerDisable(int timer)
{
    //  clear IRQ source
    BSP_Clear_interrupt(IRQ_TIMER_1 + timer);

    // disable interrupts from this source
    BSP_Mask_interrupt(IRQ_TIMER_1 + timer);

    osDrvTimerHandler[timer]->timerCallback = NULL;
    osDrvTimerHandler[timer]->repeatCount = 0;

    // Disable the timer
    SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET) + (0x10 * timer), 0 );
    SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET    ) + (0x10 * timer), 0 );

    if ((osDrvTimerHandler[timer]->status == OS_DRV_TIMER_TAKEN) &&
        (osDrvTimer[timer].chained == OS_DRV_TIMER_CHAINED_HI))
    {
        // Disable the timer and IRQ source
        SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET) + (0x10 * (timer-1)), 0 );
        SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET    ) + (0x10 * (timer-1)), 0 );
        OS_DPRINTF1("\nDisable Chained Timer: %d\n",timer-1);
    }
    return timer;
}

static void osDrvTimerIrqHandler(u32 source)
{
    u32 timerNumber = source - IRQ_TIMER_1;

    if (osDrvTimerHandler[timerNumber]->timerCallback != NULL)
    {
        osDrvTimerHandler[timerNumber]->timerCallback(timerNumber, 0);
    }
    if (osDrvTimerHandler[timerNumber]->repeatCount > 0) // 0 means infinite
    {
        osDrvTimerHandler[timerNumber]->repeatCount--;
        if (osDrvTimerHandler[timerNumber]->repeatCount == 0) // If this brings us to zero
        {
            osDrvTimerDisable(timerNumber);
            osDrvTimerHandler[timerNumber]->status = OS_DRV_TIMER_TAKEN;
            if (osDrvTimer[timerNumber].chained)
            {
                osDrvTimerHandler[timerNumber - 1]->status = OS_DRV_TIMER_TAKEN;
            }
        }
    }
    // clear the timer interrupt
    CLR_REG_BITS_MASK((osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET) + (0x10 * timerNumber), D_TIMER_CFG_IRQ_PENDING);
    // Read back the written value to ensure bus ordering
    GET_REG_WORD_VAL((osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET) + (0x10 * timerNumber));

    BSP_Clear_interrupt(source);
}

static int osDrvGetFreeTimer(u32 numMicroSeconds, int* freeTimer, u32* chained)
{
    int timer;
    u64 clocksRequired;
    u32 clocksPerUs;
    int res;

    OS_DPRINTF1("\nNum_microSeconds: %d\n",numMicroSeconds);
    // Detect if we need timer chaining
    res = OsDrvCprGetSysClockPerUs(&clocksPerUs);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        clocksRequired = (u64)numMicroSeconds * (u64)clocksPerUs;

        OS_DPRINTF1("\nCLOCKS: %08X %08X",(u32)(clocksRequired >> 32),(u32)(clocksRequired & 0xFFFFFFFF));
        *freeTimer = -1;
        *chained = 0;
        res = OS_MYR_DRV_SUCCESS;
        if (clocksRequired > (u64) 0xFFFFFFFF )
        {
            // Get 2 consecutive free timer resources
            // search stops at timer 2 to avoid chaining timer 0
            for (timer = LAST_AVAILABLE_TIMER; timer >= 2; timer--)
            {
                if ((osDrvTimer[timer].status == OS_DRV_TIMER_FREE) &&
                    (osDrvTimer[timer].status == OS_DRV_TIMER_FREE))
                {
                    *freeTimer = timer;
                    *chained = 1;
                    break;
                }
            }
            if (timer < 2 ) // No Free timer available
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
        else
        {
            // Get a single free timer resource
            // search stops at timer 1 to avoid using channel 0
            for (timer = LAST_AVAILABLE_TIMER; timer >= 1; timer--)
            {
                if (osDrvTimer[timer].status == OS_DRV_TIMER_FREE)
                {
                    *freeTimer = timer;
                    break;
                }
            }
            if (timer < 1 ) // No Free timer available
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
    }
    return res;
}

int OsDrvTimerInit(void)
{
    int status;

    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    if (osDrvTimerControl.status == OS_DRV_STAT_UNINITIALIZED)
    {
        if (swcWhoAmI() == PROCESS_LEON_OS)
        {
            osDrvTimerControl.timerBaseAddress = TIM0_BASE_ADR;
            timCfg.timerBaseAddress = TIM0_BASE_ADR;
        }
        else
        {
            osDrvTimerControl.timerBaseAddress = TIM1_BASE_ADR;
            timCfg.timerBaseAddress = TIM1_BASE_ADR;
        }
        status = rtems_semaphore_create(rtems_build_name('S', 'T', 'I', 'M'), 1,
                RTEMS_BINARY_SEMAPHORE,0,(rtems_id *) &osDrvTimerControl.controlMutex);
        switch (status)
        {
        case RTEMS_SUCCESSFUL:
            //if the free running counter was not enabled, we may not be using clock device driver
            if (!GET_REG_WORD_VAL(timCfg.timerBaseAddress + TIM_GEN_CONFIG_OFFSET) & D_TIMER_GENCFG_FREE_CNT_EN)
                // Enable the free running clock
                SET_REG_BITS_MASK(timCfg.timerBaseAddress + TIM_GEN_CONFIG_OFFSET, D_TIMER_GENCFG_FREE_CNT_EN) ;
            // set the global timer status
            timCfg.initialised = MAGIC_TIMER_INITIALISED;
            osDrvTimerControl.status = OS_DRV_STAT_INITIALIZED;
            res = OS_MYR_DRV_SUCCESS;
            break;
        case RTEMS_TIMEOUT:
            res = OS_MYR_DRV_TIMEOUT;
            break;
        default:
            res = status;
            break;
        }
    }
    else if (osDrvTimerControl.status == OS_DRV_STAT_INITIALIZED)
    {
        res = OS_MYR_DRV_ALREADY_INITIALIZED;
    }

    OS_DPRINTF1("OsDrvTimerInit() res %d\n", res);
    return res;

}

int OsDrvTimerOpen(osDrvTimerHandler_t* handler, u32 maxWaitMicroSeconds, OS_MYRIAD_DRIVER_PROTECTION protection)
{
    int res, i;
    rtems_status_code status;
    int timer = 0;
    u32 chained = 0;

    res = OS_DRV_TIMER_MUTEX_LOCK();
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osDrvTimerControl.status == OS_DRV_STAT_INITIALIZED)
        {
            //check if the handler is already used
            for (i = 0; i < NUM_TIMERS; i++)
            {
                if (osDrvTimerHandler[i] == handler)
                {
                    OS_DRV_TIMER_MUTEX_UNLOCK();
                    return OS_MYR_DRV_RESOURCE_BUSY;
                }
            }
            res = osDrvGetFreeTimer(maxWaitMicroSeconds, &timer, &chained);
            if (res == OS_MYR_DRV_SUCCESS)
            {
                osDrvTimerHandler[timer] = handler;
                handler->status = OS_DRV_TIMER_TAKEN;
                osDrvTimer[timer].status = OS_DRV_TIMER_TAKEN;
                handler->timerNo = timer;
                handler->protection = protection;
                osDrvTimer[timer].taskId = rtems_task_self();

                res = BSP_interrupt_register(IRQ_TIMER_1 + handler->timerNo, NULL,
                    (rtems_interrupt_handler) osDrvTimerIrqHandler, (void *)(IRQ_TIMER_1 + handler->timerNo));

                if (res != RTEMS_SUCCESSFUL)
                {
                    OS_DPRINTF1(" OsDrvTimerOpen() error registering the ISR: %d\n\n", res);
                    OS_DRV_TIMER_MUTEX_UNLOCK();
                    return res;
                }

                if (chained)
                {
                    osDrvTimer[timer].chained = OS_DRV_TIMER_CHAINED_HI;
                    osDrvTimer[timer - 1].chained = OS_DRV_TIMER_CHAINED_LO;
                    osDrvTimer[timer - 1].status = OS_DRV_TIMER_TAKEN;
                    osDrvTimerControl.openedTimersNo += 2;
                }
                else
                {
                    osDrvTimer[timer].chained = OS_DRV_TIMER_UNCHAINED;
                    osDrvTimerControl.openedTimersNo++;
                }
                if (handler->protection == OS_MYR_PROTECTION_CUSTOM_SEM)
                {
                    status = rtems_semaphore_create(rtems_build_name('T', 'S', 'M', '0' + timer), 1,
                            RTEMS_SIMPLE_BINARY_SEMAPHORE, 0, (rtems_id *) &osDrvTimer[timer].timerMutex);
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
                }
            }
            else
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_TIMER_MUTEX_UNLOCK();
    }
    OS_DPRINTF1("OsDrvTimerOpen() timer %d number of us %d res %d\n", handler->timerNo, maxWaitMicroSeconds, res);
    return res;
}

int OsDrvTimerSetTimeout(osDrvTimerHandler_t* handler, u32 numMicroSeconds)
{
    int res;
    u32 configAddr;
    u32 clocksPerUs;

    res = OS_DRV_TIMER_LOCK(handler->timerNo);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if ((osDrvTimerHandler[handler->timerNo]->status == OS_DRV_TIMER_TAKEN)  ||
            (osDrvTimer[handler->timerNo].chained == OS_DRV_TIMER_CHAINED_HI))
        {
            res = OsDrvCprGetSysClockPerUs(&clocksPerUs);
            if (res == OS_MYR_DRV_SUCCESS)
            {
                configAddr = (osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET) + (0x10 * handler->timerNo);
                SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET) +
                              (0x10 * handler->timerNo), clocksPerUs * numMicroSeconds);
                SET_REG_WORD (configAddr, GET_REG_WORD_VAL(configAddr)| D_TIMER_CFG_FORCE_RELOAD);
            }

        }
        else if (osDrvTimerHandler[handler->timerNo]->status == OS_DRV_TIMER_RUNNING)
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_TIMER_UNLOCK(handler->timerNo);
    }
    OS_DPRINTF1("OsDrvTimerSetTimeout() timer %d number of us %d res %d\n", handler->timerNo, numMicroSeconds, res);
    return res;
}

static int osDrvTimerCallAfterMicro(osDrvTimerHandler_t* handler, u32 numMicroSeconds,
        tyTimerCallback callback, int repeatCount, int priority)
{
    u64 clocksRequired;
    u32 clocksPerUs;
    int res, status;

    res = OsDrvCprGetSysClockPerUs(&clocksPerUs);
    if (res == OS_MYR_DRV_SUCCESS)
    {

        clocksRequired = (u64)numMicroSeconds * (u64)clocksPerUs;
        if ((clocksRequired > 0xFFFFFFFF) &&
            (osDrvTimer[handler->timerNo].chained != OS_DRV_TIMER_CHAINED_HI))
        {
            // only one timer is opened and two timer are needed for clocksRequired value
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            // bare metal drivers uses 1 based timer index
            status = DrvTimerConfig(handler->timerNo + 1, repeatCount, clocksRequired);
            switch (status)
            {
            case MYR_DRV_SUCCESS:
                osDrvTimerHandler[handler->timerNo]->timerCallback = callback;
                osDrvTimerHandler[handler->timerNo]->repeatCount = repeatCount;
                handler->status = OS_DRV_TIMER_RUNNING;

                // Ensure there is no IRQ Pending before enabling Irqs
                CLR_REG_BITS_MASK((osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET) + (0x10 * handler->timerNo), D_TIMER_CFG_IRQ_PENDING);
                // Read back the written value to ensure bus ordering
                GET_REG_WORD_VAL((osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET) + (0x10 * handler->timerNo));
                // Clear interrupt if already triggered
                BSP_Clear_interrupt(IRQ_TIMER_1 + handler->timerNo);
                // Set Type and priority
                BSP_Set_interrupt_type_priority(IRQ_TIMER_1 + handler->timerNo, POS_LEVEL_INT, priority);

                res = OS_MYR_DRV_SUCCESS;
                break;
            case MYR_DRV_NOT_INITIALIZED:
                res = OS_MYR_DRV_NOTOPENED;
                break;
            default:
                res = status;
                break;
            }
        }
    }
    OS_DPRINTF1("osDrvTimerCallAfterMicro() timer %d number of us %d\n", handler->timerNo, numMicroSeconds);
    return res;
}

int OsDrvTimerStartTimer(osDrvTimerHandler_t* handler, u32 numMicroSeconds,
        tyTimerCallback callback, int repeatCount, int priority)
{
    int res;

    res = OS_DRV_TIMER_LOCK(handler->timerNo);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if ((osDrvTimerHandler[handler->timerNo]->status == OS_DRV_TIMER_TAKEN)  ||
            (osDrvTimer[handler->timerNo].chained == OS_DRV_TIMER_CHAINED_HI))
        {
            res = osDrvTimerCallAfterMicro(handler, numMicroSeconds, callback, repeatCount, priority);
        }
        else if ((osDrvTimerHandler[handler->timerNo]->status == OS_DRV_TIMER_RUNNING) ||
                 (osDrvTimerHandler[handler->timerNo]->status == OS_DRV_TIMER_PAUSED))
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_TIMER_UNLOCK(handler->timerNo);
    }
    OS_DPRINTF1("OsDrvTimerStartTimer() timer %d number of us %d res %d\n", handler->timerNo, numMicroSeconds, res);
    return res;
}

int OsDrvTimerPauseTimer(osDrvTimerHandler_t* handler)
{
    int res;
    int status;

    res = OS_DRV_TIMER_LOCK(handler->timerNo);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        // bare metal timers have 1 based index
        status = DrvTimerPauseOperation(handler->timerNo + 1);
        switch (status)
        {
        case MYR_DRV_SUCCESS:
            osDrvTimerHandler[handler->timerNo]->status = OS_DRV_TIMER_PAUSED;
            res = OS_MYR_DRV_SUCCESS;
            break;
        case MYR_DRV_NOT_INITIALIZED:
            res = OS_MYR_DRV_NOTOPENED;
            break;
        default:
            res = status;
            break;
        }
        OS_DRV_TIMER_UNLOCK(handler->timerNo);
    }
    return res;
}

int OsDrvTimerResumeTimer(osDrvTimerHandler_t* handler)
{
    int res;
    int status;

    res = OS_DRV_TIMER_LOCK(handler->timerNo);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        // bare metal timers have 1 based index
        status = DrvTimerResumeOperation(handler->timerNo + 1);
        switch (status)
        {
        case MYR_DRV_SUCCESS:
            osDrvTimerHandler[handler->timerNo]->status = OS_DRV_TIMER_TAKEN;
            res = OS_MYR_DRV_SUCCESS;
            break;
        case MYR_DRV_NOT_INITIALIZED:
            res = OS_MYR_DRV_NOTOPENED;
            break;
        default:
            res = status;
            break;
        }
        OS_DRV_TIMER_UNLOCK(handler->timerNo);
    }
    return res;
}

int OsDrvTimerStopTimer(osDrvTimerHandler_t* handler)
{
    int res;
    u32 timerNo;

    res = OS_DRV_TIMER_LOCK(handler->timerNo);
    timerNo = handler->timerNo;
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osDrvTimerHandler[handler->timerNo] != handler)
        {
            OS_DRV_TIMER_UNLOCK(handler->timerNo);
            return OS_MYR_DRV_NOTOPENED;
        }

        // Disable IRQ source
        SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET) + (0x10 * handler->timerNo), 0 );
        SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET    ) + (0x10 * handler->timerNo), 0 );
        // Then clear any outstanding IRQ and disable
        BSP_Clear_interrupt(IRQ_TIMER_1 + handler->timerNo);

        handler->timerCallback = NULL;
        handler->repeatCount = 0;

        if (osDrvTimer[handler->timerNo].chained == OS_DRV_TIMER_CHAINED_HI)
        {
            // Disble the timer and IRQ source
            SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET) + (0x10 * (handler->timerNo - 1)), 0 );
            SET_REG_WORD ((osDrvTimerControl.timerBaseAddress + TIM0_CONFIG_OFFSET    ) + (0x10 * (handler->timerNo - 1)), 0 );
            // clear and disable interrupts
            BSP_Clear_interrupt(IRQ_TIMER_1 + handler->timerNo - 1);

            osDrvTimerHandler[handler->timerNo - 1]->status = OS_DRV_TIMER_TAKEN;
            osDrvTimer[handler->timerNo].chained = OS_DRV_TIMER_UNCHAINED;
            osDrvTimer[handler->timerNo - 1].chained = OS_DRV_TIMER_UNCHAINED;

            OS_DPRINTF1("\nDisable Chained Timer: %d\n",handler->timerNo - 1);
        }

        handler->status = OS_DRV_TIMER_TAKEN;
        OS_DRV_TIMER_UNLOCK(timerNo);
    }
    OS_DPRINTF1("OsDrvTimerStopTimer() timer %d res %d\n", handler->timerNo, res);
    return res;
}

int OsDrvTimerCloseTimer(osDrvTimerHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    int rtRes;

    res = OS_DRV_TIMER_LOCK(handler->timerNo);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if ((handler->status == OS_DRV_TIMER_RUNNING) ||
            (handler->status == OS_DRV_TIMER_PAUSED))
        {
            OS_DRV_TIMER_UNLOCK(handler->timerNo);
            res =  OS_MYR_DRV_RESOURCE_BUSY;
        }
        else if (handler->status == OS_DRV_TIMER_FREE)
        {
            OS_DRV_TIMER_UNLOCK(handler->timerNo);
            res =  OS_MYR_DRV_NOTOPENED;
        }
        else
        {
            res = OS_DRV_TIMER_MUTEX_LOCK();
            if (res == OS_MYR_DRV_SUCCESS)
            {
                osDrvTimerControl.openedTimersNo--;
                osDrvTimer[handler->timerNo].status = OS_DRV_TIMER_FREE;

                rtRes = BSP_interrupt_unregister(IRQ_TIMER_1 + handler->timerNo, (rtems_interrupt_handler)osDrvTimerIrqHandler, (void *)(IRQ_TIMER_1 + handler->timerNo));
                if (rtRes != RTEMS_SUCCESSFUL)
                {
                    OS_DPRINTF1(" OsDrvTimerCloseTimer() error unregistering the ISR: %d\n\n", res);
                    OS_DRV_TIMER_MUTEX_UNLOCK();
                    OS_DRV_TIMER_UNLOCK(handler->timerNo);
                    return rtRes;
                }

                if (osDrvTimer[handler->timerNo].chained)
                {
                    osDrvTimerControl.openedTimersNo--;
                    osDrvTimer[handler->timerNo - 1].status = OS_DRV_TIMER_FREE;

                    rtRes = BSP_interrupt_unregister(IRQ_TIMER_1 + handler->timerNo - 1, (rtems_interrupt_handler)osDrvTimerIrqHandler, (void *)(IRQ_TIMER_1 + handler->timerNo - 1));
                    if (rtRes != RTEMS_SUCCESSFUL)
                    {
                        OS_DPRINTF1(" OsDrvTimerCloseTimer() error unregistering the ISR: %d\n\n", res);
                        OS_DRV_TIMER_MUTEX_UNLOCK();
                        OS_DRV_TIMER_UNLOCK(handler->timerNo);
                        return rtRes;
                    }
                }
                if (handler->protection == OS_MYR_PROTECTION_CUSTOM_SEM)
                {
                    rtems_semaphore_delete(osDrvTimer[handler->timerNo].timerMutex);
                }
                handler->status = OS_DRV_TIMER_FREE;                
                OS_DRV_TIMER_UNLOCK(handler->timerNo);
                osDrvTimerHandler[handler->timerNo] = NULL;
                OS_DRV_TIMER_MUTEX_UNLOCK();
            }
            else
            {
                OS_DRV_TIMER_UNLOCK(handler->timerNo);
            }
        }
    }

    OS_DPRINTF1("OsDrvSvuCloseShave() shave number res %d\n", handler->shaveNumber, res);
    return res;
}

int OsDrvTimerClose(void)
{
    int res = OS_MYR_DRV_ERROR;

    res = OS_DRV_TIMER_MUTEX_LOCK();
    if (res == RTEMS_SUCCESSFUL)
    {
        // check the number of opened timers. If all the timers are closed
        // then it is safe to release the driver mutex
        if (osDrvTimerControl.openedTimersNo == 0)
        {
            osDrvTimerControl.status = OS_DRV_STAT_UNINITIALIZED;
            // mutex can not be released while it is taken
            OS_DRV_TIMER_MUTEX_UNLOCK();
            rtems_semaphore_delete(osDrvTimerControl.controlMutex);
            res = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            // some timers are still opened
            res = OS_MYR_DRV_RESOURCE_BUSY;
            OS_DRV_TIMER_MUTEX_UNLOCK();
        }
    }
    OS_DPRINTF1("OsDrvTimerClose() res %d\n", res);
    return res;
}

int OsDrvTimerGetSystemTicks64(u64* ticks)
{
    int status;
    rtems_interrupt_level level;

    rtems_interrupt_disable(level);
    status = DrvTimerGetSystemTicks64(ticks);
    rtems_interrupt_enable(level);

    return status;
}

int OsDrvTimerGetSystemTimeNs(u64* uptimeNs)
{
    u32 clksPerUs;
    u64 ticks;
    int status;
    rtems_interrupt_level level;

    rtems_interrupt_disable(level);
    status = DrvTimerGetSystemTicks64(&ticks);
    clksPerUs = DrvCprGetSysClocksPerUs();
    rtems_interrupt_enable(level);
    if(status)
        return status;

    *uptimeNs = ticks * 1000 / clksPerUs;

    return status;
}

int OsDrvTimerInitTimeStamp(tyTimeStamp * pTimeStamp, u32 intervalUs)
{
    int status;
    rtems_interrupt_level level;

    rtems_interrupt_disable(level);
    status = DrvTimerInitTimeStamp(pTimeStamp, intervalUs);
    rtems_interrupt_enable(level);

    return status;
}

int OsDrvTimerCheckTimeStamp(tyTimeStamp * pTimeStamp, u32* timeoutReached)
{
    int status;
    rtems_interrupt_level level;

    rtems_interrupt_disable(level);
    status = DrvTimerCheckTimeStamp(pTimeStamp, timeoutReached);
    rtems_interrupt_enable(level);

    return status;
}

int OsDrvTimerStartTicksCount(tyTimeStamp * pTimeStamp)
{
    int status;
    rtems_interrupt_level level;

    rtems_interrupt_disable(level);
    status = DrvTimerStartTicksCount(pTimeStamp);
    rtems_interrupt_enable(level);

    return status;
}

int OsDrvTimerGetElapsedTicks(tyTimeStamp * pTimeStamp, u64* elapsedTicks)
{
    int status;
    rtems_interrupt_level level;

    rtems_interrupt_disable(level);
    status = DrvTimerGetElapsedTicks(pTimeStamp, elapsedTicks);
    rtems_interrupt_enable(level);

    return status;
}

int OsDrvTimerWatchdogConfigure(OsDrvWatchDogCfg_t *cfg)
{
    int retVal;

    if (cfg == NULL)
        return OS_MYR_DRV_ERROR;
    if ((cfg->countdownThresholdMs > 0 && cfg->callbackFunc == NULL) ||
        (cfg->countdownThresholdMs == 0 && cfg->callbackFunc != NULL) ||  //TODO: even harder condition: if less then X msec (before WD reset) ?
         cfg->callbackPriority > 14 || cfg->countdownThresholdMs > cfg->countdownSysRstMs)
        return OS_MYR_DRV_ERROR;

    retVal = OS_DRV_TIMER_MUTEX_LOCK();
    if (retVal == OS_MYR_DRV_SUCCESS)    //timer initialized
    {
        if (osDrvWatchdogControl.status != OS_DRV_STAT_UNINITIALIZED)
            retVal = OS_MYR_DRV_ALREADY_INITIALIZED;
        else   //watchdog configured
            if (GET_REG_WORD_VAL(TIM0_WDOG_EN_ADR) & TIM_WDOG_EN_ENABLE)
                retVal = OS_MYR_DRV_RESOURCE_BUSY;
            else     //watchdog not currently running
            {
                u64 cycles = (u64)cfg->countdownSysRstMs * BSP_CLOCK_CONFIG.currentSysClockKhz;

                if (cycles > 0xFFFFFFFF)
                    retVal = OS_MYR_DRV_ERROR;
                else //max timer value not wrapping around
                {
                    // Configure elapsed ms before system reset
                    SET_REG_WORD(TIM0_WATCHDOG_ADR, (u32)cycles);

                    if (cfg->callbackFunc != NULL)
                    {
                        // Store the callback
                        osDrvWatchdogControl.callbackFunc = cfg->callbackFunc;

                        // Configure threshold value in ms at which the timer will issue an interrupt;
                        //this threshold limit is counted starting the reset limit down
                        SET_REG_WORD(TIM0_WATCHDOG_INT_THRES_ADR, BSP_CLOCK_CONFIG.currentSysClockKhz * cfg->countdownThresholdMs);
                    }
                    else
                    {
                        osDrvWatchdogControl.callbackFunc = NULL;
                        SET_REG_WORD(TIM0_WATCHDOG_INT_THRES_ADR, 0);
                    }

                    BSP_Clear_interrupt(IRQ_WATCHDOG);
                    // Set WD ISR type and priority
                    BSP_Set_interrupt_type_priority(IRQ_WATCHDOG, POS_LEVEL_INT, cfg->callbackPriority);
                    // Install new interrupt handler
                    retVal = BSP_interrupt_register(IRQ_WATCHDOG, NULL, (rtems_interrupt_handler)osDrvTimerWDIrqHandler, NULL);
                    if (retVal != RTEMS_SUCCESSFUL)
                    {
                        OS_DPRINTF1(" OsDrvTimerWatchdogConfigure() error registering the ISR: %d\n\n", retVal);
                        OS_DRV_TIMER_MUTEX_UNLOCK();
                        return retVal;
                    }

                    osDrvWatchdogControl.status = OS_DRV_STAT_INITIALIZED;
                    osDrvWatchdogControl.taskId = rtems_task_self();
                    osDrvWatchdogControl.defaultSysRstTicksCount = cycles;

                    retVal = OS_MYR_DRV_SUCCESS;
                }
            }
     }

     OS_DRV_TIMER_MUTEX_UNLOCK();
     return retVal;
}


int OsDrvTimerWatchdogEnable(osDrvWdogAction_t action)
{
    if ((osDrvWatchdogControl.status != OS_DRV_STAT_INITIALIZED) || (osDrvWatchdogControl.taskId != rtems_task_self()))  //only the task which configured the WD is allowed to enable/disable it
        return OS_MYR_DRV_NOTOPENED;

    if (action == WDOG_ENABLE)
    {
        // configure ICB to accept interrupt from watchdog
        BSP_Unmask_interrupt(IRQ_WATCHDOG);

        // The purpose of TIM_SAFE is to protect registers for the next write access only.
        // Write to TIM_SAFE_ADR before enabling the watchdog
        SET_REG_WORD(TIM0_SAFE_ADR, TIM_SAFE_VALUE);
        //enable the watchdog
        SET_REG_WORD(TIM0_WDOG_EN_ADR, TIM_WDOG_EN_ENABLE);
    }
    else
    {
        // The purpose of TIM_SAFE is to protect registers for the next write access only.
        // Write to TIM_SAFE_ADR before enabling the watchdog
        SET_REG_WORD(TIM0_SAFE_ADR, TIM_SAFE_VALUE);
        //enable the watchdog
        SET_REG_WORD(TIM0_WDOG_EN_ADR, TIM_WDOG_EN_DISABLE);

        osDrvTimerWatchDogInterruptClear();
        BSP_Mask_interrupt(IRQ_WATCHDOG);

        //release the WD; further enable needs reconfiguration
        osDrvWatchdogControl.status = OS_DRV_STAT_UNINITIALIZED;
        osDrvWatchdogControl.taskId = 0;
    }

    return OS_MYR_DRV_SUCCESS;
}


int OsDrvTimerWatchdogUpdate(u32 sysRstMs, u32* remainingMs)
{
    int retVal = OS_MYR_DRV_SUCCESS;
    u32 status;
    u64 updTicks;

    if (osDrvWatchdogControl.status != OS_DRV_STAT_INITIALIZED)
        return OS_MYR_DRV_NOTOPENED;

    rtems_interrupt_disable(status);

    if (remainingMs != NULL)
        *remainingMs = GET_REG_WORD_VAL(TIM0_WATCHDOG_ADR) / BSP_CLOCK_CONFIG.currentSysClockKhz;

    if (sysRstMs > 0)
        updTicks = (u64)BSP_CLOCK_CONFIG.currentSysClockKhz * sysRstMs;
    else
        updTicks = (u64)osDrvWatchdogControl.defaultSysRstTicksCount;

    if (updTicks <= 0xFFFFFFFF)
    {
        // Configure elapsed ms before system reset
        SET_REG_WORD(TIM0_WATCHDOG_ADR, (u32)updTicks);
    }
    else
        retVal = OS_MYR_DRV_ERROR;

    rtems_interrupt_enable(status);

    return retVal;
}


static void osDrvTimerWatchDogInterruptClear(void)
{
    // Clear the interrupt from source
    CLR_REG_BIT_NUM(TIM0_GEN_CONFIG_ADR, TIM_WATCHDOG_CLR_IRQ_OFFS);
    // Read back the written value to ensure bus ordering
    GET_REG_WORD_VAL(TIM0_GEN_CONFIG_ADR);
    // Clear the interrupt from the ICB block
    BSP_Clear_interrupt(IRQ_WATCHDOG);
}

static void osDrvTimerWDIrqHandler(u32 source)
{
    UNUSED(source);

    if (osDrvWatchdogControl.callbackFunc != NULL)
    {
        osDrvWatchdogControl.callbackFunc();
    }

    osDrvTimerWatchDogInterruptClear();
}

///@}
