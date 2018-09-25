///
/// @file OsDrvMutexDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvMutex
/// @{
/// @brief     Definitions and types needed by RTEMS mutex driver
///


#ifndef _OS_DRV_MUTEX_DEFINES_H_
#define _OS_DRV_MUTEX_DEFINES_H_

// System Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"

// Application Includes
// ----------------------------------------------------------------------------

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define OS_DRV_MUTEX_DEFAULT_INT_LEVEL   5
#define OS_DRV_MUTEX_WAIT_FOREVER        0xFFFFFFFF

#define OS_DRV_MUTEX_EVENT(e)            (0x1 << (e))

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
// manages the resources that need to be
// shared between handles with the same id
typedef struct {
    u32 eventId;
    u32 lockId;
    u8 count;
} osDrvMutexShared_t;

typedef struct {
    osDrvMutexShared_t * mutexInfo;
    s8 mutexId;
    u8 mutexOwner;
} osDrvMutexHandle_t;


#endif

///@}
