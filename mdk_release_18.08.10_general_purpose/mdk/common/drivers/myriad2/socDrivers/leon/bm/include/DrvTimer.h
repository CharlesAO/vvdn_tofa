///
/// @file DrvTimer.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvTimer Timer Driver
/// @{
/// @brief     API for the timer module
///
/// The Timer Driver provides the following key functionality       
/// - Full 64 bit system tick counter which is never reset.                                                                   
/// - Timer Async callback functionality to allow the user to request a callback after specified num microseconds             
///   The callback can be requested; once, x times or infinitely.                                                             
///   There is a pool of 7 hardware timers available.                                                                         
/// - Timer Async variable update. (Same as callback only simpler case where the users variable gets incremented after a time)
/// - New Sleep API with units of microseconds, ms, or ticks. Is aware of current CPU frequency.                              
///   It has no dependence of RTC and it does not reset the the system tick counter                                           
/// - Timeout API to support a simple design pattern for timeout on register access etc.                                      
/// 


#ifndef DRV_TIMER_H
#define DRV_TIMER_H 

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvCpr.h"
#include "DrvTimerDefines.h"
#include "swcLeonMath.h"
#include "DrvCommon.h"
//remove deprecation warnings from the driver
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
extern tyTimerState timCfg;

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Initialise the timer driver
/// 
/// Initialise config structures, enable 64 bit free running clock
/// This function should only be called once from DrvCprInit so users generally
/// should not have to call this function assuming CPR has been properly intialised
/// Mark all timers as available
/// @return    0 on success, non-zero otherwise  
int DrvTimerInit(void);

/// Returns the number of clock cycles since DrvTimerInit() called
/// 
/// This is a full 64 bit free running counter so it only overflows 
/// every 3249 years @ 180Mhz
/// @param[out] ticks number of clock cycles since DrvCprInit() [64 bit value]
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerGetSystemTicks64(u64* ticks);
__Deprecated__("Use DrvTimerGetSystemTicks64 instead")
u64 DrvTimerGetSysTicks64(void);

/// Converts a number of ticks into a number of ms in FP64 representation
///
/// Floating point representation is used to give maximum useful range for this function
/// @return float containing Number of milliseconds corresponding to the specified tick count
static inline double DrvTimerTicksToMs(u64 ticks)
{
    double timeMs;
    timeMs = swcLongLongToDouble(ticks);
    timeMs = timeMs / DrvCprGetSysClocksPerUs() / 1000;
    return timeMs;
}

/// Configures timer interrupt
///
/// @param[in] timer timer number
/// @param[in] callback function callback that will be called when timeout expires
/// @param[in] priority level of the interrupt
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY no available timer was found
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerSetupIrq(u32 timer, tyTimerCallback callback, u32 priority);

/// Configures timer given the number of clocks that it needs to run
///
/// @param[in] timer timer number
/// @param[in] repeatCount times to callback (0-> forever until DrvTimerStop), 1 once, x -> x times)
/// @param[in] clocksRequired the number of clock ticks to initialize the timer
///     This value is also used to decide whether one or two chained timers are needed
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY no available timer was found
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerConfig(u32 timer, u32 repeatCount, u64 clocksRequired);

/// Request a callback after numMicroSeconds
/// 
/// Optionally have the callback for repeatCount times
/// @param[in] number of micro-seconds before the callback is called
/// @param[in] pointer to function to callback
/// @param[in] Times to callback (0-> forever until DrvTimerStop), 1 once, x -> x times)
/// @param[in] priority level of the IRQ
/// @param[out] timerNo the number of the timer used on success <0 on fail.
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY no available timer was found
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerStartOperation(u32 numMicroSeconds, tyTimerCallback callback, int repeatCount, int priority, int* timerNo);
__Deprecated__("Use DrvTimerStartOperation instead")
int DrvTimerCallAfterMicro(u32 numMicroSeconds, tyTimerCallback callback, int repeatCount, int irqPriority);

/// Trigger timer even to increment a static 32 bit word
///
/// Optionally do this for repeatCount times
/// @param[in] number of micro-seconds before the timer increments the variable
/// @param[in] pointer to 32 bit word to be incremented
/// @param[in] Times to callback (0-> forever until DrvTimerStop), 1 once, x -> x times)
/// @param[out] timerNo the number of the timer used on success <0 on fail.
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY no available timer was found
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerStartCounter(u32 numMicroSeconds,volatile u32 * pValueToIncrement, int repeatCount, int* timerNo);
__Deprecated__("Use DrvTimerStartCounter instead")
int DrvTimerIncrementAfterMicro(u32 numMicroSeconds, volatile u32 * pValueToIncrement, int repeatCount);

/// Pause the timer keeping it's state
///
/// The timer can be resumed by calling DrvTimerResume
/// @param[in] timer the number of the timer that needs to be paused
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerPauseOperation(int timer);

/// Resumes a timer paused by a call to DrvTimerPause
///
/// @param[in] timer the number of the timer that needs to be resumed
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///     ERR_TIM_NOT_PAUSED the timer can not be resumed becaused it is not paused
///
int DrvTimerResumeOperation(int timer);

/// Adjust a currently running timer to a new time value
///
/// This function may be used in the callback routine of an infinite count timer
/// @param[in] timer handle returned from timer setup function
/// @param[in] set adjusted number of micro-seconds until the next timer callback
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerSetTimeout(u32 timer, u32 numMicroSeconds);
__Deprecated__("Use DrvTimerSetTimeout instead")
void DrvTimerAdjustTimerValue(u32 timer, u32 numMicroSeconds);

/// Disable the specified timer using the handle returned by other timer functions
/// 
/// This is necesary if timer was configured as REPEAT_FOREVER or if it needs to be stopped early
/// @param[in] timer handle returned from timer setup function
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerStopOperation(int timer);
__Deprecated__("Use DrvTimerStopOperation instead")
int DrvTimerDisable(int timer);

/// Sleep for numMs milli-seconds
/// 
/// Uses knowledge of the system clock to calculate the time
/// Doesn't rely on RTC
/// @param[in] number of milliseconds to stall for
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerSleepMs(unsigned int numMs);
__Deprecated__("Use DrvTimerSleepMs instead")
void SleepMs(unsigned int numMs);

/// Sleep for numMicroSeconds micro-seconds
/// 
/// Uses knowledge of the system clock to calculate the time
/// Doesn't rely on RTC
/// When running with 180Mhz system clock and cache enabled, this function can 
/// achieve accurate delays down to 1 micro-second
/// Note: Accurate => error < 1uS
/// @param[in] number of microseconds to stall for
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerSleepMicro(unsigned int numMicroSeconds);
__Deprecated__("Use DrvTimerSleepMicro instead")
void SleepMicro(unsigned int numMicroSeconds);

/// Return system uptime in milli-seconds
///
/// @return    number of ms since DrvTimerInit as FP64
double DrvTimerUptimeMs(void);

/// Sleep for numSystemTicks system clock cycles
/// 
/// Attempts to define a delay based on system clock cycles.
/// For small values this function will not be accurate due to 
/// function overhead.
/// Function is inline for performance
/// This function is not advised in most applications as delays should normally be in ms or us.
/// @param[in] number of system clocks to stall for
/// @return    void
__Deprecated__("Use DrvTimerSleepTicks instead")
static inline void SleepTicks(u64 numSystemTicks)
{
    u64 startTime = DrvTimerGetSysTicks64();
    u64 endTime   = startTime + numSystemTicks;
    while (DrvTimerGetSysTicks64() < endTime)
        ; // Deliberatly empty
    return;
}

static inline int DrvTimerSleepTicks(u64 numSystemTicks)
{
    int status;
    u64 startTime, endTime;
    u64 ticks;

    status = DrvTimerGetSystemTicks64(&startTime);
    if (status == MYR_DRV_SUCCESS)
    {
        endTime = startTime + numSystemTicks;
        do
        {
            DrvTimerGetSystemTicks64(&ticks);
        }
        while (ticks < endTime);
    }

    return status;
}

/// Initialise a timeout 
///
/// Creates a timestamp which is used to check if a timeout has occurred
/// Uses storage passed by the user to make the function re-entrant
/// @param[in] pointer to storage for calculated timestamp
/// @param[in] Interval for the timestamp in microseconds
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerInitTimeStamp(tyTimeStamp * pTimeStamp,u32 intervalUs);
__Deprecated__("Use DrvTimerInitTimeStamp instead")
void DrvTimerTimeoutInitUs(tyTimeStamp * pTimeStamp,u32 intervalUs);

/// Reports if a timeout has occurred based on previously initialised timestamp
/// 
/// @param[in] pointer to timestamp which after which the function will return TRUE
/// @param[out] timeoutReached  FALSE if current time is < timestamp; TRUE if timeout has occurred
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerCheckTimeStamp(tyTimeStamp * pTimeStamp, u32* timeoutReached);
__Deprecated__("Use DrvTimerCheckTimeStamp instead")
int DrvTimerTimeout(tyTimeStamp * pTimeStamp);

/// Update pTimestamp with the current tickcount
///
/// This function is part of a pair of functions used for event timing
/// Call this function with a pointer to a tyTimeStamp which is used to 
/// record the current tick count.Later this timestamp will be used to 
/// report the elapsed ticks since the original timestamp
/// @param[in] pointer to storage for calculated timestamp
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerStartTicksCount(tyTimeStamp * pTimeStamp);
__Deprecated__("Use DrvTimerStartTicksCount instead")
void DrvTimerStart(tyTimeStamp * pTimeStamp);

/// Report the ticks elapsed since DrvTimerStart was called on this timestamp
///
/// @param[in] pointer to the previsouly initialised timeStamp (using DrvTimerStart)
/// @param[out] elapsedTicks 64 bit tick count of elapsed time since start
///
/// @return
///     MYR_DRV_SUCCESS the operation completed successfully
///     MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerGetElapsedTicks(tyTimeStamp * pTimeStamp, u64* elapsedTicks);
__Deprecated__("Use DrvTimerGetElapsedTicks instead")
u64 DrvTimerElapsedTicks(tyTimeStamp * pTimeStamp);

/// @brief Configure the watchdog timer
///
/// The watchdog configuration (DrvTimerWDCfg) is a structure that is comprised of a callback
/// function, callback priority, system reset counter in ms and a threshold at which the watchdog issues
/// an interrupt, in ms. The count down starts from the upper value of the system reset value until reach the
/// threshold value; if a WD callback was provided, this will be called from ISR. The counting continues towards
/// zero, when will trigger a system reset, if not stopped/updated in the meantime.
/// The maximum value for the two counters is  0xFFFFFFFF / system clock in khz (7158 ms for a 600 Mhz system clock)
///
/// @note Writing a 0(zero) to the system reset parameter will force an immediate system reset, without waiting for
/// watchdog enable
///
/// @param[in] cfg watchdog configuration structure
///
/// @return
///     - MYR_DRV_SUCCESS the operation completed successfully
///     - MYR_DRV_RESOURCE_BUSY the watchdog is enabled and cannot be configured
///     - MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerWDConfigure(DrvTimerWDCfg *cfg);

/// @brief Update the watchdog timer
///
/// This function updates the watchdog timer in order to avoid both interrupt (if configured) and system reset.
///
/// @param[in]  countdownSysRstMs: time in ms until the system reset is triggered; if 0, the default value set at configuration is used to set the WD counter;
///               maximum value dependent on frequency (7158 ms for a 600 Mhz system clock); if value bigger than max value (say 0xFFFFFFFF),
///               the counter is not updated (and an error is returned) but the remainingTicks can be used to read the current ctr value
/// @param[out] remainingMs: the remaining ms until reset, read right before updating the WD counter
///
/// @return
///     - MYR_DRV_SUCCESS the operation completed successfully
///     - MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerWDUpdate(u32 countdownSysRstMs, u32* remainingMs);

/// @brief Enable or disable the watchdog timer
/// Disabling the WD releases the current configuration. The WD needs reconfiguring again
/// @param[in] action 
///                 - WDOG_DISABLE: disable
///                 - WDOG_ENABLE: enable
/// @return
///     - MYR_DRV_SUCCESS the operation completed successfully
///     - MYR_DRV_NOT_INITIALIZED the driver was not initialized
///
int DrvTimerWDEnable(tyWdogAction action);

/// @brief Start profiler timer
/// @param[in] action
///                 - timer
///                 - numTickToCount
/// @return
//
void DrvStartTimer(u32 timer, u32 numTicksToCount, u32 timerConfig);

//Clears Timer Register Config bits
inline u32 DrvClearTimConfigRegBit(u32 regAddr, u32 bitMask){

	CLR_REG_BITS_MASK(regAddr, bitMask);

    // With interrupt configured on level, reading back the register is needed to ensure that the write operation was performed
    // before continuing with interrupt clearing from ICB. Otherwise, there is a risk for the interrupt to not get cleared
	return GET_REG_WORD_VAL(regAddr);
}


#ifdef __cplusplus
}
#endif
// restore deprecation warnings
#pragma GCC diagnostic pop

#endif // DRV_TIMER_H  


///@}
