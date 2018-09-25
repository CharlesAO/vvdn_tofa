///
/// @file OsDrvCpr.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvCpr
/// @{
/// @brief     RTEMS Driver to control Clocks, Power and Reset
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include <rtems/clockdrv.h>
#include "OsCross.h"
#include "OsDrvCpr.h"
#include <stdlib.h>
#include <stdio.h>
#include "HglCpr.h"
/* Startup info */
extern __BSP_CLOCK_CONFIG__ BSP_CLOCK_CONFIG;
/* System Ticks */
extern volatile u32 Clock_driver_ticks;
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define OS_DRVCPR_RUN_EVENT					0x0001
#define OS_DRVCPR_EXIT_EVENT				0x0002

//#define OS_DRV_CPR_L1
//#define OS_DRV_CPR_L2
//#define OS_DRV_CPR_L3

#ifdef OS_DRV_CPR_L1
	#define OS_DPRINTF1(...)			printf(__VA_ARGS__)
#else
	#define OS_DPRINTF1(...)
#endif

#ifdef OS_DRV_CPR_L2
	#define OS_DPRINTF2(...)			printf(__VA_ARGS__)
#else
	#define OS_DPRINTF2(...)
#endif

#ifdef OS_DRV_CPR_L3
	#define OS_DPRINTF3(...)			printf(__VA_ARGS__)
#else
	#define OS_DPRINTF3(...)
#endif


// Lock utility by default wait forever to access the resource
#define OS_DRV_CPR_LOCK()			osDrvCprLockCriticalSection(true, RTEMS_NO_TIMEOUT);
// Unlock utility
#define OS_DRV_CPR_UNLOCK()			osDrvCprUnlockCriticalSection()

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
typedef u32 (*osDrvCprThreadNotifier)(u32 sysClock);

typedef struct
{
	void *extraInfo; /*< Extra information for the notifications */
	osDrvCprNotification notification; /*< callback to execute on clock change */
}osDrvCprNotificationSlot;

static struct
{
	OS_MYRIAD_DRIVER_STATUS status; /*< Driver Status */
	rtems_id access;	/*< Access control */
	rtems_id callBackThreadId; /*< Thread in charge of the callbacks */
	osDrvCprThreadNotifier systemNotifier; /*< Callback to notify rest of the threads */
	osDrvCprNotificationSlot notificationArray[OS_MYR_MAX_DRVCPR_NOTIFICATION_LIST]; /*< Notification list  */
}osCprControl;


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
// Modify System Tick after modifying the System Clock
// TBD Find a cleaner and safer way of initializing system ticks
// This is done to prevent os directives depending on Clock_driver_ticks as a reference to work properly after changing the system clock
static u32 osDrvCprOnOsSystemTickChange(u32 sysClock)
{
	u32 status = 0, ticks = 0;
	if (sysClock) {
		// Disable all interrupts
		rtems_interrupt_disable(status);
		// Take advantage of the start-up shared variable
		BSP_CLOCK_CONFIG.currentSysClockKhz = sysClock;
		// Shut down Clock
		Clock_exit();
		// Backup current ticks
		ticks = Clock_driver_ticks;
		// Reinitialize Clock
		Clock_initialize(0,0,NULL);
		// Restore Ticks
		Clock_driver_ticks = ticks;
		// Reenable interrupts
		rtems_interrupt_enable(status);
	}
	if (*osCprControl.systemNotifier)
		(*osCprControl.systemNotifier)(sysClock);
	return sysClock;
}

/* Add to notification list */
static u32 osDrvSysClockChangeAdd2List(osDrvCprNotification notification, void *extraInfo)
{
	u32 i = 0;
	while (i < OS_MYR_MAX_DRVCPR_NOTIFICATION_LIST) {
		if (osCprControl.notificationArray[i].notification == NULL) {
			osCprControl.notificationArray[i].notification = notification;
			osCprControl.notificationArray[i].extraInfo = extraInfo;
			break;
		}
		i ++;
	}
	OS_DPRINTF3("osDrvSysClockChangeAdd2List Notification %p ExtraInfo %p res %d\n", notification, extraInfo, i);
	return i;
}
/* Remove from notification list */
static u32 osDrvSysClockChangeRemoveFromList(osDrvCprNotification notification, u32 *active)
{
	u32 i = 0,j = 0, k = OS_MYR_MAX_DRVCPR_NOTIFICATION_LIST;
	for (i = 0; i < OS_MYR_MAX_DRVCPR_NOTIFICATION_LIST; i ++) {
		if (osCprControl.notificationArray[i].notification) {
			if (osCprControl.notificationArray[i].notification == notification) {
				osCprControl.notificationArray[i].notification = NULL;
				osCprControl.notificationArray[i].extraInfo = NULL;
				k = i;
			}
			else
				j ++;
		}
	}
	*active = j;
	OS_DPRINTF3("osDrvSysClockChangeRemoveFromList Notification %p Active %d res %d\n", notification, j, k);
	return k;

}
/* Enter Critical Section */
static rtems_status_code osDrvCprLockCriticalSection(u32 wait, u32 wait_ticks)
{
	rtems_status_code res;
	if (wait)
		res = rtems_semaphore_obtain(osCprControl.access, RTEMS_WAIT, wait_ticks);
	else
		res = rtems_semaphore_obtain(osCprControl.access, RTEMS_NO_WAIT, 0);
	OS_DPRINTF2("osDrvCprLockCriticalSection res %d wait %d\n", res, wait);
	return res;
}

/* Abandon Critical Section */
static rtems_status_code osDrvCprUnlockCriticalSection(void)
{
	rtems_status_code res = rtems_semaphore_release(osCprControl.access);
	OS_DPRINTF2("osDrvCprUnlockCriticalSection res %d\n", res);
	return res;
}

/*
* This is a task in charge of executing all the callbacks registered so far
*/
rtems_task OsDrvCprExecuteNotifications(rtems_task_argument arg)
{
	u32 i, sysclock, execute = 1;
	osDrvCprNotificationSlot *notificationList = (osDrvCprNotificationSlot *) arg;
	rtems_event_set events;
	while(execute) {
		// Wait for events
		rtems_event_receive(OS_DRVCPR_RUN_EVENT | OS_DRVCPR_EXIT_EVENT, RTEMS_EVENT_ANY | RTEMS_WAIT, RTEMS_NO_TIMEOUT, &events);
		if (events & OS_DRVCPR_RUN_EVENT) {
			// Cannot allow the list to be modified while issuing the notifications
			if (osDrvCprLockCriticalSection(true, RTEMS_NO_TIMEOUT) == RTEMS_SUCCESSFUL) {
			    sysclock = (u32) DrvCprGetClockFreqKhz(SYS_CLK, NULL);
				for (i = 0; i < OS_MYR_MAX_DRVCPR_NOTIFICATION_LIST; i ++) {
					if (notificationList[i].notification != NULL)
						(*notificationList[i].notification)(sysclock, notificationList[i].extraInfo);
				}
				osDrvCprUnlockCriticalSection();
			}
	    		OS_DPRINTF3("OsDrvCprExecuteNotifications Arg %p\n", arg);
		}
		// End Thread
		if (events & OS_DRVCPR_EXIT_EVENT)
			execute = 0;
	}
	OS_DPRINTF3("OsDrvCprExecuteNotifications Delete\n");
	// Self deletion
	rtems_task_delete(RTEMS_SELF);
}

/* Allows to prepare all the infraestructure to handle callbacks when the clock is changed
   By default this thread is a floating point thread */
static u32 osDrvCreateCallBackThread(rtems_task_argument arg)
{
	u32 res = RTEMS_SUCCESSFUL;
	if (osCprControl.callBackThreadId == 0) {
		res = rtems_task_create(rtems_build_name( 'C', 'P', 'R', 'C' ),
    					1,
    					RTEMS_MINIMUM_STACK_SIZE,
    					RTEMS_PREEMPT | RTEMS_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0),
    					RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT,
    					&osCprControl.callBackThreadId);
		if (res == RTEMS_SUCCESSFUL) {
			res = rtems_task_start(osCprControl.callBackThreadId, OsDrvCprExecuteNotifications, arg);
			if (res != RTEMS_SUCCESSFUL) {
				rtems_task_delete(osCprControl.callBackThreadId);
				osCprControl.callBackThreadId = 0;
			}
		}
	}
	OS_DPRINTF3("osDrvCreateCallBackThread Arg %p res %d\n", arg, res);
	return res;
}

/* System Notifier tweaked */
static u32 osDrvCprSysClockChangeNotifier(u32 newsysclock)
{
	s32 res;
	// There is a new system clock
	// When we call this function we are protected from concurrent accesses
	res = osDrvCreateCallBackThread((rtems_task_argument) &osCprControl.notificationArray);
	// Notify change
	if (res == RTEMS_SUCCESSFUL)
		rtems_event_send(osCprControl.callBackThreadId, OS_DRVCPR_RUN_EVENT);
	// Debug
	OS_DPRINTF3("osDrvCprSysClockChangeNotifier SysClock %d res %d\n", newsysclock, res);
	return newsysclock;
}

/* Init function that must be called prior to any other call to this driver */
int OsDrvCprInit(void)
{
    rtems_status_code status;
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;;
    if (osCprControl.status == OS_DRV_STAT_UNINITIALIZED)
    {
        status = rtems_semaphore_create(rtems_build_name('C', 'P', 'R', '0'),
                                        1, RTEMS_SIMPLE_BINARY_SEMAPHORE, 0,
                                        (rtems_id *) &osCprControl.access);
        if (status == RTEMS_SUCCESSFUL)
        {
            osCprControl.status = OS_DRV_STAT_INITIALIZED;
            res = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            res  = status;
        }
    }
    else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
    {
        res = OS_MYR_DRV_ALREADY_INITIALIZED;
    }
    else if (osCprControl.status == OS_DRV_STAT_OPENED)
    {
        res = OS_MYR_DRV_ALREADY_OPENED;
    }
    OS_DPRINTF1("OsDrvCprInit() res %d\n", res);
    return res;
}

/* Open Driver */
int OsDrvCprOpen(void)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_INITIALIZED) {
				if (DrvCprInitClocks(BSP_CLOCK_CONFIG.ref0ClockKhz, 0, BSP_CLOCK_CONFIG.pll0TargetFrequencyKhz, 0, BSP_CLOCK_CONFIG.currentSysClockKhz) == CPR_SUCCESS) {
					osCprControl.status = OS_DRV_STAT_OPENED;
					// Register internal callback for changes
					DrvCprRequestSysClkChangeNotification((tySysClkFreqChangeCb) osDrvCprOnOsSystemTickChange);
					res = OS_MYR_DRV_SUCCESS;
				}
			}
			else if (osCprControl.status == OS_DRV_STAT_OPENED) {
				res = OS_MYR_DRV_ALREADY_OPENED;
			}
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprOpen() res %d\n", res);
	return res;
}

/* Register callbacks on System clock change */
int OsDrvCprRequestSysClockChangeNotificationRegister(osDrvCprNotification notification, void *extraInfo)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				if ((notification != NULL) && (osDrvSysClockChangeAdd2List(notification, extraInfo) < OS_MYR_MAX_DRVCPR_NOTIFICATION_LIST)){
					osCprControl.systemNotifier = osDrvCprSysClockChangeNotifier;
					res = OS_MYR_DRV_SUCCESS;
				}
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprRequestSysClockchangeNotificationRegister() notification %p res %d\n", notification, res);
	return res;
}

/* Register callbacks on System clock change */
int OsDrvCprRequestSysClockChangeNotificationUnregister(osDrvCprNotification notification)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	u32 active = 0;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				if ((notification != NULL) && (osDrvSysClockChangeRemoveFromList(notification, &active) < OS_MYR_MAX_DRVCPR_NOTIFICATION_LIST)){
					// If there are no more active callbacks
					if ((active == 0) && (osCprControl.callBackThreadId)){
						rtems_event_send(osCprControl.callBackThreadId, OS_DRVCPR_EXIT_EVENT);
						osCprControl.callBackThreadId = 0;
						osCprControl.systemNotifier = NULL;
					}
					res = OS_MYR_DRV_SUCCESS;
				}
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprRequestSysClockchangeNotificationUnregister() notification %p res %d\n", notification, res);
	return res;
}

/* Apply clocks */
int OsDrvCprSetupClocks(const tySocClockConfig * pSocClockConfig)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				res = OSDRV_HW_DRV_CHECK_SUCCESS(DrvCprSetupClocks(pSocClockConfig));
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprSetupClocks res %d\n", res);
	return res;
}

/* Configure system clock */
int OsDrvCprConfigureSystemClock(u32 plltargetfreqkhz, u32 clockdivnumerator, u32 clockdivdenominator)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				int retVal = DrvCprConfigureSystemClk((int) plltargetfreqkhz, (int) clockdivnumerator, (int) clockdivdenominator);
				if (retVal < 0)
					res = OSDRV_HW_DRV_CHECK_SUCCESS(retVal);
				else
					res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprConfigureSystemClock target %d num %d den %d res %d\n", plltargetfreqkhz, clockdivnumerator, clockdivdenominator, res);
	return res;
}

int OsDrvCprAuxClockEnable(u32 auxClksMask,tyCprClockSrc clkSource,u32 numerator, u32 denominator)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				res = OSDRV_HW_DRV_CHECK_SUCCESS(DrvCprAuxClocksEnable(auxClksMask,clkSource, numerator, denominator));
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprAuxClockEnable mask %d source %d num %d den %d res %d\n",auxClksMask, clkSource, numerator, denominator, res);
	return res;
}

int OsDrvCprAuxClockArrayConfig(const tyAuxClkDividerCfg  pAuxClkCfg[])
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				res = OSDRV_HW_DRV_CHECK_SUCCESS(DrvCprAuxClockArrayConfig(pAuxClkCfg));
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprAuxClockArrayConfig res %d\n", res);
	return res;
}

int OsDrvCprGetClockFreqKhz(tyClockType clockType,tyClockConfig * clkCfg, s32 *result)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				*result = DrvCprGetClockFreqKhz(clockType, clkCfg);
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprGetClockFreqKhz Type %d result %d, res %d\n",clockType, *result, res);
	return res;
}

int OsDrvCprPllBypass(tyPllBypassOption pll0Bypass,tyPllBypassOption pll1Bypass)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				DrvCprPllBypass(pll0Bypass, pll1Bypass);
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprPllBypass PLL0 %d PLL1 %d res %d\n",pll0Bypass, pll1Bypass, res);
	return res;
}

int OsDrvCprSysDeviceAction(tyCprClockDomain clkDomain,tyCprDeviceAction action,uint64_t devices)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				DrvCprSysDeviceAction(clkDomain, action, devices);
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprSysDeviceAction domain %d action %d devicesA %d devicesB %d res %d\n",clkDomain, action, (devices >> 32), (devices & 0xFFFFFFFF), res);
	return res;
}

int OsDrvCprGetSysClockKhz(u32 *result)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				*result = DrvCprGetSysClockKhz();
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprGetSysClockKhz res %d result %d\n", res, *result);
	return res;
}

int OsDrvCprGetSysClockPerUs(u32 *result)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				*result = DrvCprGetSysClocksPerUs();
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprGetSysClockPerUs res %d result %d\n", res, *result);
	return res;
}

int OsDrvCprTurnOnShaveMask(u32 shave_mask)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    rtems_status_code status = OS_DRV_CPR_LOCK();
    switch(status) {
        case RTEMS_SUCCESSFUL:
            if (osCprControl.status == OS_DRV_STAT_OPENED) {
                HglCprTurnOnShaveMask(shave_mask);
                res = OS_MYR_DRV_SUCCESS;
            }
            else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
                res = OS_MYR_DRV_NOTOPENED;
            OS_DRV_CPR_UNLOCK();
            break;
        case RTEMS_TIMEOUT:
            res = OS_MYR_DRV_TIMEOUT;
            break;
        default:
            res = status;
            break;
    }
    OS_DPRINTF1("OsDrvCprStartAllClocks mask 0x%x result %d\n", shave_mask, res);
    return res;
}

int OsDrvCprTurnOffShaveMask(u32 shave_mask)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    rtems_status_code status = OS_DRV_CPR_LOCK();
    switch(status) {
        case RTEMS_SUCCESSFUL:
            if (osCprControl.status == OS_DRV_STAT_OPENED) {
                HglCprTurnOffShaveMask(shave_mask);
                res = OS_MYR_DRV_SUCCESS;
            }
            else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
                res = OS_MYR_DRV_NOTOPENED;
            OS_DRV_CPR_UNLOCK();
            break;
        case RTEMS_TIMEOUT:
            res = OS_MYR_DRV_TIMEOUT;
            break;
        default:
            res = status;
            break;
    }
    OS_DPRINTF1("OsDrvCprStartAllClocks mask 0x%x result %d\n", shave_mask, res);
    return res;
}

int OsDrvCprStartAllClocks(void)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				DrvCprStartAllClocks();
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprStartAllClocks res %d result %d\n", res);
	return res;
}

int OsDrvCprPowerTurnOnIslandRaw(u32 islands_mask, u32 trickle_ticks, u32 enable_ticks)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				DrvCprPowerTurnOnIslandsRaw(islands_mask, trickle_ticks, enable_ticks);
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprPowerTurnOnIslandRaw Mask %d trickle ticks %d enable ticks %d res %d\n", islands_mask, trickle_ticks, enable_ticks, res);
	return res;
}

int OsDrvCprPowerTurnOffIslandRaw(u32 islands_mask, u32 iso_ticks, u32 disable_ticks)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				DrvCprPowerTurnOffIslandsRaw(islands_mask, iso_ticks, disable_ticks);
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprPowerTurnOffIslandRaw Mask %d iso ticks %d disable ticks %d res %d\n", islands_mask, iso_ticks, disable_ticks, res);
	return res;
}

int OsDrvCprPowerTurnOnIsland(enum PowerIslandIndex island_index)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				DrvCprPowerTurnOnIsland(island_index);
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprPowerTurnOnIsland Index %d res %d\n", island_index, res);
	return res;
}

int OsDrvCprPowerTurnOffIsland(enum PowerIslandIndex island_index)
{
	OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
	rtems_status_code status = OS_DRV_CPR_LOCK();
	switch(status) {
		case RTEMS_SUCCESSFUL:
			if (osCprControl.status == OS_DRV_STAT_OPENED) {
				DrvCprPowerTurnOffIsland(island_index);
				res = OS_MYR_DRV_SUCCESS;
			}
			else if (osCprControl.status == OS_DRV_STAT_INITIALIZED)
				res = OS_MYR_DRV_NOTOPENED;
			OS_DRV_CPR_UNLOCK();
			break;
		case RTEMS_TIMEOUT:
			res = OS_MYR_DRV_TIMEOUT;
			break;
		default:
			res = status;
			break;
	}
	OS_DPRINTF1("OsDrvCprPowerTurnOffIsland Index %d res %d\n", island_index, res);
	return res;
}

int OsDrvCprGetCprVersionId(u32* cprVersionId)
{
    *cprVersionId = HGL_CPR_GET_CPR_VERSION_ID();
    OS_DPRINTF1("OsDrvCprGetCprVersionId id 0x%x\n", *cprVersionId);
    return OS_MYR_DRV_SUCCESS;
}

///@}
