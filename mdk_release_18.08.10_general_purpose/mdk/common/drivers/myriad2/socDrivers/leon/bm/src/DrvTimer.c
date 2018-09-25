///
/// @file DrvTimer.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvTimer
/// @{
/// @brief     Driver for Timer module
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include "DrvCommon.h"
#include "DrvRegUtils.h"
#include "DrvCpr.h" 
#include "DrvTimer.h"
#include "registersMyriad.h"
#include "DrvIcb.h"
#include "swcWhoAmI.h"
//remove deprecation warnings from the driver
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define LAST_AVAILABLE_TIMER (NUM_TIMERS)


//#define DRV_TIMER_DEBUG 

#ifdef DRV_TIMER_DEBUG
#define DPRINTF1(...) printf(__VA_ARGS__)
#else
#define DPRINTF1(...)
#endif

// Private Register Bitfield descriptions
#define D_TIMER_GENCFG_FREE_CNT_EN         (1 << 0)
#define D_TIMER_GENCFG_WATCHDOG_PRESCALLER (1 << 1)
#define D_TIMER_GENCFG_WATCHDOG_SYSTEM_CLK (0x0000)
#define D_TIMER_GENCFG_PRESCALER_ENABLE    (1 << 2)
#define D_TIMER_GENCFG_CLEAR_WATCHDOG_INT  (0x0000)

#define D_TIMER_CFG_ENABLE          (1 << 0)
#define D_TIMER_CFG_RESTART         (1 << 1)
#define D_TIMER_CFG_EN_INT          (1 << 2)
#define D_TIMER_CFG_CHAIN           (1 << 3)
#define D_TIMER_CFG_IRQ_PENDING     (1 << 4)
#define D_TIMER_CFG_FORCE_RELOAD    (1 << 5)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
tyTimerState timCfg = {.initialised = MAGIC_TIMER_NOT_INITIALISED};

// 4: Static Local Data
// ----------------------------------------------------------------------------
static struct {
    u32  status;
    u32  defaultSysRstTicksCount;
    DrvTimerWDCallback  callbackFunc;
} drvWatchdogControl = {0};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

static int timerCallAfterMicro(u32 timer,u32 numMicroSeconds, tyTimerCallback callback, int repeatCount, int priority);
static int getFreeTimer(u32 numMicroSeconds);
static u32 updateDataValue(u32 timer, u32 param0);
static inline int drvTimerEnable(int timer, u32 enable);
static inline void timerIrqHandler(u32 irq_source);
static void drvTimerWDIrqHandler(u32 source);
static void drvTimerWatchDogInterruptClear(void);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// API Function Implementations
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

int DrvTimerInit(void)
{
    int timer;

    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
        return ERR_TIM_ALREADY_INTIALISED; // The timer driver should only be initialized once at startup by CPR Init
    
    if (timCfg.initialised != MAGIC_TIMER_NOT_INITIALISED)
    {
        DPRINTF1("Unknown timer state when calling %s\n", __FUNCTION__);
        swcLeonHalt();
    }

    // Select between base address for LeonOS timer and LeonRT timer
    if (swcWhoAmI() == PROCESS_LEON_OS)
        timCfg.timerBaseAddress = TIM0_BASE_ADR;
    else
    {
        timCfg.timerBaseAddress = TIM1_BASE_ADR;
        // Deassert timer reset
        DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, DEV_MSS_TIM);
        // Check that the timer clock is enabled
        #ifndef NDEBUG
            u32 mssClockEnables;
            mssClockEnables = GET_REG_WORD_VAL(MSS_CLK_CTRL_ADR);
            assert(mssClockEnables & DEV_MSS_TIM);
        #endif
    }

    // Enable 64 bit mode of free running clock
    for (timer=0;timer<=LAST_AVAILABLE_TIMER;timer++)
    {
        timCfg.sysTimers[timer].type           = TIMER_FREE;
        timCfg.sysTimers[timer].callbackFunc   = NULL;
        timCfg.sysTimers[timer].pDataVal       = NULL;
        timCfg.sysTimers[timer].repeatCount    = 0;
    }

    // Enable the free running clock, and the scaler
    SET_REG_BITS_MASK(timCfg.timerBaseAddress + TIM_GEN_CONFIG_OFFSET ,   D_TIMER_GENCFG_FREE_CNT_EN | D_TIMER_GENCFG_PRESCALER_ENABLE); 

    // Set the prescaler to 0 so that we use the system clock direct for the timers
    // This gives us maximum resolution and we can chain registers to get larger counts
    // It is slightly more power hungry, but I think the tradeoff is worth it
    SET_REG_WORD(timCfg.timerBaseAddress + TIM_SCALER_RELOAD_OFFSET,0);

    
    timCfg.initialised = MAGIC_TIMER_INITIALISED; // This function should only be run once

    return 0;
}

u64 DrvTimerGetSysTicks64(void)
{
    u32 currentTicksL, currentTicksH;
    u32 base_address = timCfg.timerBaseAddress;
    
    if (base_address != TIM0_BASE_ADR && base_address != TIM1_BASE_ADR)
    {
        DPRINTF1("Programming error: DrvTimerInit() has not been called before %s\n", __FUNCTION__);
        swcLeonHalt();
    }

    // Note: The following 2 reads perform an atomic read of the internal 64 bit timer
#ifdef MA2X8X
    currentTicksL = GET_REG_WORD_VAL(base_address + TIM_FREE_CNT0_OFFSET);  // previously latched low value read here
    currentTicksH = GET_REG_WORD_VAL(base_address + TIM_FREE_CNT1_OFFSET);  // On this read the low value is latched
#else
    currentTicksH = GET_REG_WORD_VAL(base_address + TIM_FREE_CNT1_OFFSET);  // On this read the low value is latched
    currentTicksL = GET_REG_WORD_VAL(base_address + TIM_FREE_CNT0_OFFSET);  // previously latched low value read here
#endif

    return ((u64)currentTicksH << 32) | ((u64)currentTicksL);   
}

int DrvTimerGetSystemTicks64(u64* ticks)
{
    u32 currentTicksL, currentTicksH;
    u32 base_address = timCfg.timerBaseAddress;
    int status;

    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        // Note: The following 2 reads perform an atomic read of the internal 64 bit timer
#ifdef MA2X8X
        currentTicksL = GET_REG_WORD_VAL(base_address + TIM_FREE_CNT0_OFFSET);  // previously latched low value read here
        currentTicksH = GET_REG_WORD_VAL(base_address + TIM_FREE_CNT1_OFFSET);  // On this read the low value is latched
#else
        currentTicksH = GET_REG_WORD_VAL(base_address + TIM_FREE_CNT1_OFFSET);  // On this read the low value is latched
        currentTicksL = GET_REG_WORD_VAL(base_address + TIM_FREE_CNT0_OFFSET);  // previously latched low value read here
#endif

        *ticks = ((u64)currentTicksH << 32) | ((u64)currentTicksL);
        status = MYR_DRV_SUCCESS;
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvTimerDisable(int timer)
{
    // Disable the IRQ before we wipe out the callback
    DrvIcbDisableIrq(IRQ_TIMER + timer);

    timCfg.sysTimers[timer].callbackFunc   = NULL;
    timCfg.sysTimers[timer].pDataVal       = NULL;
    timCfg.sysTimers[timer].repeatCount    = 0;

    DPRINTF1("\nDisable Timer: %d\n",timer);

    // Disble the timer and IRQ source
    SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * timer), 0 );
    SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET      - 0x10) + (0x10 * timer), 0 );

    // Then clear any outstanding IRQ and disable
    DrvIcbIrqClear(IRQ_TIMER + timer);
    DrvIcbDisableIrq(IRQ_TIMER + timer);

    if (timCfg.sysTimers[timer].type == TIMER_CHAINED_HIGH)
    {
        // Disble the timer and IRQ source
        SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * (timer-1)), 0 );
        SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET      - 0x10) + (0x10 * (timer-1)), 0 );
        timCfg.sysTimers[timer-1].type     = TIMER_FREE;
        DPRINTF1("\nDisable Chained Timer: %d\n",timer-1);
    }

    // Last step, mark it free
    timCfg.sysTimers[timer].type           = TIMER_FREE;

    return timer;
}

int DrvTimerStopOperation(int timer)
{
    int status = MYR_DRV_ERROR;

    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        // Disable the IRQ before we wipe out the callback
        DrvIcbDisableIrq(IRQ_TIMER + timer);

        timCfg.sysTimers[timer].callbackFunc   = NULL;
        timCfg.sysTimers[timer].pDataVal       = NULL;
        timCfg.sysTimers[timer].repeatCount    = 0;

        DPRINTF1("\nDisable Timer: %d\n",timer);

        // Disble the timer and IRQ source
        SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * timer), 0 );
        SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET      - 0x10) + (0x10 * timer), 0 );

        // Then clear any outstanding IRQ and disable
        DrvIcbIrqClear(IRQ_TIMER + timer);
        DrvIcbDisableIrq(IRQ_TIMER + timer);

        if (timCfg.sysTimers[timer].type == TIMER_CHAINED_HIGH)
        {
            // Disble the timer and IRQ source
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * (timer-1)), 0 );
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET      - 0x10) + (0x10 * (timer-1)), 0 );
            timCfg.sysTimers[timer-1].type     = TIMER_FREE;
            DPRINTF1("\nDisable Chained Timer: %d\n",timer-1);
        }

        // Last step, mark it free
        timCfg.sysTimers[timer].type = TIMER_FREE;
        status = MYR_DRV_SUCCESS;
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }

    return status;
}

void DrvTimerAdjustTimerValue(u32 timer, u32 numMicroSeconds)
{
    u32 configAddr = (timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET  - 0x10) + (0x10 * (timer-0));
    SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * (timer-0)), DrvCprGetSysClocksPerUs()*numMicroSeconds);
    SET_REG_WORD (configAddr, GET_REG_WORD_VAL(configAddr)| D_TIMER_CFG_FORCE_RELOAD);

    return;
}

int DrvTimerSetTimeout(u32 timer, u32 numMicroSeconds)
{
    u32 configAddr;
    int status;

    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        configAddr = (timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET  - 0x10) + (0x10 * (timer-0));
        SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * (timer-0)), DrvCprGetSysClocksPerUs()*numMicroSeconds);
        SET_REG_WORD (configAddr, GET_REG_WORD_VAL(configAddr)| D_TIMER_CFG_FORCE_RELOAD);
        status = MYR_DRV_SUCCESS;
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }

    return status;
}

int DrvTimerIncrementAfterMicro(u32 numMicroSeconds,volatile u32 * pValueToIncrement, int repeatCount)
{
    int retVal;
    int timer = getFreeTimer(numMicroSeconds);
    timCfg.sysTimers[timer].pDataVal = pValueToIncrement;
    retVal = timerCallAfterMicro(timer,numMicroSeconds,&updateDataValue,repeatCount,DEFAULT_TIMER_PRIORITY);
    if (retVal == ERR_SINGLE_SHOT_ONLY_LARGE_TIMER)
    {
        DrvTimerDisable(timer);
    }    
    return retVal;
}

int DrvTimerStartCounter(u32 numMicroSeconds,volatile u32 * pValueToIncrement, int repeatCount, int* timerNo)
{
    int status = MYR_DRV_ERROR;

    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        *timerNo = getFreeTimer(numMicroSeconds);
        if (*timerNo < 0)
        {
            // no timers available
            status = MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            timCfg.sysTimers[*timerNo].pDataVal = pValueToIncrement;
            status = timerCallAfterMicro(*timerNo, numMicroSeconds,
                     &updateDataValue, repeatCount, DEFAULT_TIMER_PRIORITY);
            if (status == ERR_SINGLE_SHOT_ONLY_LARGE_TIMER)
            {
                DrvTimerStopOperation(*timerNo);
            }
        }
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvTimerCallAfterMicro(u32 numMicroSeconds, tyTimerCallback callback, int repeatCount, int priority) // 0 is forever
{
    int retVal;
    int timer = getFreeTimer(numMicroSeconds);
    if (timer<0)
    {
        assert(FALSE);
        return timer;
    }

    retVal = timerCallAfterMicro(timer,numMicroSeconds,callback,repeatCount,priority);
    if (retVal == ERR_SINGLE_SHOT_ONLY_LARGE_TIMER)
    {
        DrvTimerStopOperation(timer);
    }
    DPRINTF1("\nDrvTimerCallAfterMicro()-> returned (%d)\n",retVal);
    return retVal;
}

int DrvTimerStartOperation(u32 numMicroSeconds, tyTimerCallback callback, int repeatCount, int priority, int* timerNo)
{
    int status = MYR_DRV_ERROR;

    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        *timerNo = getFreeTimer(numMicroSeconds);
        if (*timerNo < 0)
        {
            // no timers available
            status = MYR_DRV_RESOURCE_BUSY;
        }
        else
        {
            status = timerCallAfterMicro(*timerNo, numMicroSeconds, callback, repeatCount, priority);
            if (status == ERR_SINGLE_SHOT_ONLY_LARGE_TIMER)
            {
                DrvTimerStopOperation(*timerNo);
            }
        }
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }
    DPRINTF1("\nDrvTimerCallAfterMicro()-> returned (%d)\n",status);
    return status;
}

void DrvTimerTimeoutInitUs(tyTimeStamp * pTimeStamp, u32 intervalUs)
{
    *pTimeStamp =  DrvTimerGetSysTicks64() + ((u64)intervalUs * (u64)DrvCprGetSysClocksPerUs() ); 
    return;
}

int DrvTimerInitTimeStamp(tyTimeStamp * pTimeStamp, u32 intervalUs)
{
    int status;
    u64 ticks;
    status = DrvTimerGetSystemTicks64(&ticks);
    if (status == MYR_DRV_SUCCESS)
    {
        *pTimeStamp =  ticks + ((u64)intervalUs * (u64)DrvCprGetSysClocksPerUs() );
    }
    return status;
}

int DrvTimerTimeout(tyTimeStamp * pTimeStamp)
{
    if ( DrvTimerGetSysTicks64() > (*pTimeStamp) )
        return 1;
    else
        return 0;
}

int DrvTimerCheckTimeStamp(tyTimeStamp * pTimeStamp, u32* timeoutReached)
{
    int status;
    u64 ticks;

    *timeoutReached = 0;
    status = DrvTimerGetSystemTicks64(&ticks);
    if (status == MYR_DRV_SUCCESS)
    {
        if (ticks > (*pTimeStamp))
        {
            *timeoutReached = 1;
        }
    }
    return status;
}

double DrvTimerUptimeMs(void)
{
    u64 upTimeTicks;
    upTimeTicks = DrvTimerGetSysTicks64();
    return DrvTimerTicksToMs(upTimeTicks);
}

void DrvTimerStart(tyTimeStamp * pTimeStamp)
{
   *pTimeStamp = DrvTimerGetSysTicks64();
   return;
}

int DrvTimerStartTicksCount(tyTimeStamp * pTimeStamp)
{
    return DrvTimerGetSystemTicks64(pTimeStamp);
}

u64 DrvTimerElapsedTicks(tyTimeStamp * pTimeStamp)
{
   return (DrvTimerGetSysTicks64() - (*pTimeStamp));
}

int DrvTimerGetElapsedTicks(tyTimeStamp * pTimeStamp, u64* elapsedTicks)
{
    int status;
    u64 ticks;

    status = DrvTimerGetSystemTicks64(&ticks);
    if (status == MYR_DRV_SUCCESS)
    {
        *elapsedTicks = ticks - (*pTimeStamp);
    }
    return status;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Static Function Implementations
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

static int getFreeTimer(u32 numMicroSeconds)
{
    int timer;
    static u64 clocksRequired;

    DPRINTF1("\nNum_microSeconds: %d\n",numMicroSeconds);
    // Detect if we need timer chaining
    clocksRequired = (u64)numMicroSeconds * (u64)DrvCprGetSysClocksPerUs();

    DPRINTF1("\nCLOCKS: %08X %08X",(u32)(clocksRequired >> 32),(u32)(clocksRequired & 0xFFFFFFFF));
    if (clocksRequired > (u64) 0xFFFFFFFF )
    {
        // Get 2 consecutive free timer resources
        for (timer=LAST_AVAILABLE_TIMER;timer>=2;timer--)
        {
            if ( (timCfg.sysTimers[timer  ].type == TIMER_FREE) && 
                 (timCfg.sysTimers[timer-1].type == TIMER_FREE) 
                 )
            {  
                timCfg.sysTimers[timer  ].type = TIMER_CHAINED_HIGH;
                timCfg.sysTimers[timer-1].type = TIMER_CHAINED_LOW;
                break;
            }
        }
        if (timer < 2 ) // No Free timer available
        {
            assert(FALSE); 
            return ERR_NO_FREE_TIMER;
        }
    }
    else
    {
        // Get a single free timer resource
        for (timer=LAST_AVAILABLE_TIMER;timer>=1;timer--)
        {
            if (timCfg.sysTimers[timer].type == TIMER_FREE)
            {  
                timCfg.sysTimers[timer  ].type = TIMER_NORMAL;
                break;
            }
        }
        if (timer < 1 ) // No Free timer available
        {
            assert(FALSE); 
            return ERR_NO_FREE_TIMER;
        }
    }


    return timer;
}

int DrvTimerSetupIrq(u32 timer, tyTimerCallback callback, u32 priority)
{
    int status;

    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        timCfg.sysTimers[timer].callbackFunc = callback;

        // Ensure there is no IRQ Pending before enabling Irqs
        CLR_REG_BITS_MASK((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET - 0x10) + (0x10 * timer), D_TIMER_CFG_IRQ_PENDING);
        // Read back the written value to ensure bus ordering
        GET_REG_WORD_VAL((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET - 0x10) + (0x10 * timer));

        DPRINTF1("timer_no %d, in irq no %d\n", timer, IRQ_TIMER + timer);

        DrvIcbSetupIrq(IRQ_TIMER + timer,priority, POS_LEVEL_INT, &timerIrqHandler );
        status = MYR_DRV_SUCCESS;
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvTimerConfig(u32 timer, u32 repeatCount, u64 clocksRequired)
{
    u32 timerConfig;
    int status;

    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        // Load the current reload value into the countdown timer, enable countdown and irq
        timerConfig = D_TIMER_CFG_FORCE_RELOAD    |
                D_TIMER_CFG_ENABLE          |
                D_TIMER_CFG_EN_INT;

        if (repeatCount != 1) // Unless oneshot, we auto-reload the timer
            timerConfig |= D_TIMER_CFG_RESTART;

        timCfg.sysTimers[timer].repeatCount = repeatCount;


        if (clocksRequired > (u64) 0xFFFFFFFF )
        {
            DPRINTF1("\nUsing Timers: %d,%d\n",timer,timer-1);

            if (repeatCount != 1)
            {
                // Hardware doesn't accurately support repeated timer for clock counts > 32 bits
                return ERR_SINGLE_SHOT_ONLY_LARGE_TIMER;
            }
            // Upper Timer
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * timer), (clocksRequired >> 32));
            // Chain the timer with the next lower timer
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET      - 0x10) + (0x10 * timer), timerConfig | D_TIMER_CFG_CHAIN );

            // Lower timer
            // First we load the timer with the remainder value
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * (timer-1)), (clocksRequired & 0xFFFFFFFF) );
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET      - 0x10) + (0x10 * (timer-1)), D_TIMER_CFG_FORCE_RELOAD);
            // Then we set the reload value to 0xFFFFFFFF so it continues to count down from there
            // and enable it to auto restart
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * (timer-1)), 0xFFFFFFFF);
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET      - 0x10) + (0x10 * (timer-1)), D_TIMER_CFG_RESTART    |
                    D_TIMER_CFG_ENABLE);
        }
        else
        {
            DPRINTF1("\nUsing Timer : %d \n",timer);
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_RELOAD_VAL_OFFSET  - 0x10) + (0x10 * timer), clocksRequired);
            // As last step we enable the timer
            SET_REG_WORD ((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET      - 0x10) + (0x10 * timer), timerConfig );
        }
        status = MYR_DRV_SUCCESS;
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }

    return status;
}

static int timerCallAfterMicro(u32 timer,u32 numMicroSeconds, tyTimerCallback callback, int repeatCount, int priority) 
{
    u64 clocksRequired;

    DrvTimerSetupIrq(timer, callback, priority);

    // Detect if we need timer chaining
    clocksRequired = (u64)numMicroSeconds * (u64)DrvCprGetSysClocksPerUs();;
    return DrvTimerConfig(timer, repeatCount, clocksRequired);
}

int DrvTimerPauseOperation(int timer)
{
    int status;
    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        drvTimerEnable(timer, 0);
        if (timCfg.sysTimers[timer].type == TIMER_CHAINED_HIGH)
        {
            timCfg.sysTimers[timer].type = TIMER_CHAINED_PAUSED;
        }
        else
        {
            timCfg.sysTimers[timer].type = TIMER_NORMAL_PAUSED;
        }
        status = MYR_DRV_SUCCESS;
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvTimerResumeOperation(int timer)
{
    int status;
    if (timCfg.initialised == MAGIC_TIMER_INITIALISED)
    {
        if (timCfg.sysTimers[timer].type == TIMER_NORMAL_PAUSED)
        {
            drvTimerEnable(timer, 1);
            timCfg.sysTimers[timer].type = TIMER_NORMAL;
            status = MYR_DRV_SUCCESS;
        }
        else if (timCfg.sysTimers[timer].type == TIMER_CHAINED_PAUSED)
        {
            drvTimerEnable(timer, 1);
            timCfg.sysTimers[timer].type = TIMER_CHAINED_HIGH;
            status = MYR_DRV_SUCCESS;
        }
        else
        {
            status = ERR_TIM_NOT_PAUSED;
        }
    }
    else
    {
        status = MYR_DRV_NOT_INITIALIZED;
    }
    return status;
}

void SleepMs(unsigned int numMs)
{
    SleepMicro(numMs * 1000);
    return;
}

int DrvTimerSleepMs(unsigned int numMs)
{
    return DrvTimerSleepMicro(numMs * 1000);
}

void SleepMicro(unsigned int numMicroSeconds)
{
    SleepTicks((u64)numMicroSeconds * (u64)DrvCprGetSysClocksPerUs());
    return;
}

int DrvTimerSleepMicro(unsigned int numMicroSeconds)
{
    return DrvTimerSleepTicks((u64)numMicroSeconds * (u64)DrvCprGetSysClocksPerUs());
}

static u32 updateDataValue(u32 timer, u32 __attribute__((unused)) param0)
{
    DPRINTF1("\nUpdate Data for timer %d\n",timer);

    // param0 is deliberately unused. Reserved for future use.
    // Simply increment the counter in the users application
    timCfg.sysTimers[timer].pDataVal[0]++;
    DPRINTF1("\nNew Val %d\n", timCfg.sysTimers[timer].pDataVal[0]);    
    return 0;
}


static inline void drvTimerSetEnableBit(int timer, u32 enable)
{
    u32 cfgRegVal;
    u32 cfgRegAddr;

    DPRINTF1("\nStop Timer: %d\n",timer);

    // Disable the timer
    cfgRegAddr = (timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET - 0x10) + (0x10 * timer);
    cfgRegVal = GET_REG_WORD_VAL(cfgRegAddr);
    if (enable)
    {
        SET_REG_WORD(cfgRegAddr, cfgRegVal | D_TIMER_CFG_ENABLE);
    }
    else
    {
        SET_REG_WORD(cfgRegAddr, cfgRegVal & (~D_TIMER_CFG_ENABLE));
    }
}

static inline int drvTimerEnable(int timer, u32 enable)
{
    drvTimerSetEnableBit(timer, enable);
    if (timCfg.sysTimers[timer].type == TIMER_CHAINED_HIGH)
    {
        drvTimerSetEnableBit(timer - 1, enable);
    }

    return timer;
}

static void timerIrqHandler(u32 irq_source)
{
    u32 timer = irq_source - IRQ_TIMER;

    // Call the user specified callback function
    timCfg.sysTimers[timer].callbackFunc(timer,0);

    // Clear interrupt source
    CLR_REG_BITS_MASK((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET - 0x10) + (0x10 * timer), D_TIMER_CFG_IRQ_PENDING);
    // Read back the written value to ensure bus ordering
    GET_REG_WORD_VAL((timCfg.timerBaseAddress + TIM0_CONFIG_OFFSET - 0x10) + (0x10 * timer));

    DrvIcbIrqClear(IRQ_TIMER + timer); // Clear the irq in ICB

    if (timCfg.sysTimers[timer].repeatCount > 0) // 0 means infinite
    {
        timCfg.sysTimers[timer].repeatCount--;
        if (timCfg.sysTimers[timer].repeatCount == 0) // If this brings us to zero
        {
            DrvTimerDisable(timer);
        }
    }

    return;
}

int DrvTimerWDConfigure(DrvTimerWDCfg *cfg)
{
    u32 base_address = timCfg.timerBaseAddress;
    
    if ((base_address != TIM0_BASE_ADR) && (base_address != TIM1_BASE_ADR))
    {
        DPRINTF1("Error: timer not initialized\n", __FUNCTION__);
        return MYR_DRV_NOT_INITIALIZED;
    }
    else if (base_address != TIM0_BASE_ADR)
        {
            DPRINTF1("Error: WDOG usage not allowed from LRT \n", __FUNCTION__);
            return MYR_DRV_ERROR;
        }

    // Check if WatchDog disabled (we should not configure it while enabled)
    if(GET_REG_WORD_VAL(TIM0_WDOG_EN_ADR) & TIM_WDOG_EN_ENABLE)
    {
        DPRINTF1("Programming error: Cannot configure the WatchDog if enabled! Please disable before config! %s\n", __FUNCTION__);
        return MYR_DRV_RESOURCE_BUSY;
    }

    if (cfg == NULL)
        return MYR_DRV_ERROR;

    if ((cfg->countdownThresholdMs > 0 && cfg->callbackFunc == NULL) ||
        (cfg->countdownThresholdMs == 0 && cfg->callbackFunc != NULL) ||  //TODO: even harder condition: if less then X msec (before WD reset) ?
         cfg->callbackPriority > 14 ||
         cfg->countdownThresholdMs > cfg->countdownSysRstMs)
    {
        return MYR_DRV_ERROR;
    }
    else
    {
        // Configure elapsed ms before system reset
        u64 cycles = (u64)cfg->countdownSysRstMs * DrvCprGetSysClockKhz();
        if (cycles <= 0xFFFFFFFF)
        {
            SET_REG_WORD(TIM0_WATCHDOG_ADR, (u32)cycles);
        }
        else
            return MYR_DRV_ERROR;

        if (cfg->callbackFunc != NULL)
        {
            // Store the callback
            drvWatchdogControl.callbackFunc = cfg->callbackFunc;

            // Configure threshold value in ms at which the timer will issue an interrupt;
            //this threshold limit is counted starting up to down
            SET_REG_WORD(TIM0_WATCHDOG_INT_THRES_ADR, DrvCprGetSysClockKhz() * cfg->countdownThresholdMs);
        }
        else
        {
            drvWatchdogControl.callbackFunc = NULL;
            SET_REG_WORD(TIM0_WATCHDOG_INT_THRES_ADR, 0);
        }

        // Configure ICB to accept interrupt from watchdog
        DrvIcbSetupIrq(IRQ_WATCHDOG, cfg->callbackPriority, POS_LEVEL_INT, drvTimerWDIrqHandler);

        drvWatchdogControl.status = TIM_WDOG_CONFIGURED;
        drvWatchdogControl.defaultSysRstTicksCount = cycles;
    }

    return MYR_DRV_SUCCESS;
}

int DrvTimerWDUpdate(u32 countdownSysRstMs, u32* remainingMs)
{
    u64 updTicks;

    if (drvWatchdogControl.status != TIM_WDOG_CONFIGURED)
    {
        DPRINTF1("Error: watchdog not initialized\n", __FUNCTION__);
        return MYR_DRV_NOT_INITIALIZED;
    }

    if (remainingMs != NULL)
        *remainingMs = GET_REG_WORD_VAL(TIM0_WATCHDOG_ADR) / (u32)DrvCprGetSysClockKhz();

    if (countdownSysRstMs > 0)
        updTicks = (u64)countdownSysRstMs * DrvCprGetSysClockKhz();
    else
        updTicks = (u64)drvWatchdogControl.defaultSysRstTicksCount;

    if (updTicks <= 0xFFFFFFFF)
        SET_REG_WORD(TIM0_WATCHDOG_ADR, (u32)updTicks);
    else
        return MYR_DRV_ERROR;

    return MYR_DRV_SUCCESS;
}

int DrvTimerWDEnable(tyWdogAction action)
{
    if (drvWatchdogControl.status != TIM_WDOG_CONFIGURED)
    {
        DPRINTF1("Error: watchdog not initialized\n", __FUNCTION__);
        return MYR_DRV_NOT_INITIALIZED;
    }

    if (action == WDOG_ENABLE)
    {
        // configure ICB to accept interrupt from watchdog
        drvTimerWatchDogInterruptClear();
        DrvIcbEnableIrq(IRQ_WATCHDOG);

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
        //disable the watchdog
        SET_REG_WORD(TIM0_WDOG_EN_ADR, TIM_WDOG_EN_DISABLE);

        drvWatchdogControl.status = TIM_WDOG_NOT_CONFIGURED;
    }

    return MYR_DRV_SUCCESS;
}

static void drvTimerWatchDogInterruptClear(void)
{
    CLR_REG_BIT_NUM(TIM0_GEN_CONFIG_ADR, TIM_WATCHDOG_CLR_IRQ_OFFS);

    DrvIcbIrqClear(IRQ_WATCHDOG);
}

static void drvTimerWDIrqHandler(u32 source)
{
    UNUSED(source);

    if (drvWatchdogControl.callbackFunc != NULL)
    {
        drvWatchdogControl.callbackFunc();
    }

    drvTimerWatchDogInterruptClear();
}

void DrvStartTimer(u32 timer, u32 numTicksToCount, u32 timerConfig)
{
    // reload
    SET_REG_WORD ((TIM0_BASE_ADR + TIM0_RELOAD_VAL_OFFSET - 0x10) + (0x10 * timer), numTicksToCount );

    // set the global timerConfig
    SET_REG_WORD ((TIM0_BASE_ADR + TIM0_CONFIG_OFFSET - 0x10) + (0x10 * timer), timerConfig);
}

// restore deprecation warnings
#pragma GCC diagnostic pop

///@}
