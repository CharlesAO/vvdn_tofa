///
/// @file OsDrvSvuDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvSvu
/// @{
/// @brief     Definitions and types needed by shave low level functionality
///


#ifndef _OS_DRV_SVU_DEFINES_H
#define _OS_DRV_SVU_DEFINES_H

// System Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "OsCommon.h"

// Application Includes
// ----------------------------------------------------------------------------

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#ifdef __RTEMS__
    #ifdef POWER_AWARE
        #undef POWER_AWARE
    #endif
#endif

#define SHAVE_FINISH_EVENT(i) (1 << i)
#define OS_DRV_SVU_WAIT_FOREVER              (0xFFFFFFFF)

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef enum {
    OS_DRV_SVU_FREE = 0,
    OS_DRV_SVU_TAKEN,
    OS_DRV_SVU_RUNNING,
    OS_DRV_SVU_ERROR
} drvSvuShaveState_t;

typedef enum {
    OS_DRV_SVU_NO_WAIT = 0,
    OS_DRV_SVU_TIMEOUT,
    OS_DRV_SVU_WAIT_ALL
} drvSvuWaitOption_t;

typedef struct {
    u8 shaveNumber;
    rtems_id taskId;
    rtems_id shaveMutex;
    OS_MYRIAD_DRIVER_PROTECTION protection;
    drvSvuShaveState_t shaveState;
} osDrvSvuHandler_t;

#endif

///@}
