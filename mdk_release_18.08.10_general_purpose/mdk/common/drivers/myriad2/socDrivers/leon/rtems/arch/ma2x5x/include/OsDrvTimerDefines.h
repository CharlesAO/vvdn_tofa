///
/// @file OsDrvTimerDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvTimer
/// @{
/// @brief     Definitions and types needed by the Timer API for RTEMS
///


#ifndef _OS_DRV_TIMER_DEFINES_H
#define _OS_DRV_TIMER_DEFINES_H

// System Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "DrvTimer.h"
#include "OsCommon.h"


// Application Includes
// ----------------------------------------------------------------------------

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define D_TIMER_GENCFG_FREE_CNT_EN          (1 << 0)


// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef void (*OsDrvWatchDogCallback_t)(void);

typedef enum {
    OS_DRV_TIMER_FREE = 0,
    OS_DRV_TIMER_TAKEN,
    OS_DRV_TIMER_RUNNING,
    OS_DRV_TIMER_PAUSED,
    OS_DRV_TIMER_ERROR
} osDrvTimerStatus_t;

typedef tyWdogAction osDrvWdogAction_t;

typedef struct {
    u8 timerNo;
    u32 repeatCount;
    osDrvTimerStatus_t status;
    tyTimerCallback timerCallback;
    OS_MYRIAD_DRIVER_PROTECTION protection;
} osDrvTimerHandler_t;

typedef struct
{
   OsDrvWatchDogCallback_t  callbackFunc;
   uint32_t                 callbackPriority;
   uint32_t                 countdownSysRstMs;          // ms elapsed until system reset
   uint32_t                 countdownThresholdMs;       // ms elapsed until interrupt trigger
} OsDrvWatchDogCfg_t;

#endif

///@}
