///
/// @file DrvTimerDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvTimer
/// @{
/// @brief     Definitions and types needed by Timer Driver
///

#ifndef DRV_TIMER_DEF_H
#define DRV_TIMER_DEF_H 

#include "mv_types.h"

// API ERRORS
#define     ERR_NO_FREE_TIMER                           (-1)
#define     ERR_SINGLE_SHOT_ONLY_LARGE_TIMER            (-2)
#define     ERR_TIM_ALREADY_INTIALISED                  (-3)
#define     ERR_TIM_NOT_PAUSED                          (-4)

#define REPEAT_FOREVER              (0)
#define US_IN_1_MS                  (1000)
#define US_IN_1_SECOND              (1000 * 1000)

#define DEFAULT_TIMER_PRIORITY      (3)

#define NUM_TIMERS           (8)

#define MAGIC_TIMER_INITIALISED     0xC0DEC0DE
#define MAGIC_TIMER_NOT_INITIALISED 0xDEADDEAD

#define TIM_WDOG_EN_DISABLE  0
#define TIM_WDOG_EN_ENABLE   1

#define TIM_WDOG_NOT_CONFIGURED  0
#define TIM_WDOG_CONFIGURED      1

#define TIM_WATCHDOG_CLR_IRQ_OFFS    0x03

// 1: Defines
// ----------------------------------------------------------------------------

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

typedef u32 (*tyTimerCallback)(u32 timerNumber, u32 param2); 
typedef u64 tyTimeStamp;

typedef struct 
{
    union 
    {
        u64 dword;
        u32 word[2];
    };
} tyDwordUnion;

typedef enum
{
    TIMER_FREE,
    TIMER_NORMAL,
    TIMER_NORMAL_PAUSED,
    TIMER_CHAINED_PAUSED,
    TIMER_CHAINED_HIGH,
    TIMER_CHAINED_LOW
} tyTimerType;

typedef enum
{
    WDOG_DISABLE,
    WDOG_ENABLE
}tyWdogAction;

typedef struct
{
  tyTimerType           type;
  tyTimerCallback       callbackFunc;
  u32                   repeatCount;
  volatile u32          *pDataVal;
}tyTimerConfig;

typedef void (*DrvTimerWDCallback)(void);

typedef struct
{
    DrvTimerWDCallback    callbackFunc;
    uint32_t              callbackPriority;
    uint32_t              countdownSysRstMs;          // ms elapsed until system reset
    uint32_t              countdownThresholdMs;       // ms elapsed until interrupt trigger
} DrvTimerWDCfg;

typedef struct
{
    u32 timerBaseAddress;
    tyTimerConfig sysTimers[NUM_TIMERS+1];  // configuration state for each timer
    u32  initialised;                        // So we don't re-init
} tyTimerState;

               
// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

#endif // DRV_TIMER_DEF_H


///@}
