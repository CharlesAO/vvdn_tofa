///
/// @file OsDrvResMgrInternal.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvResMgr
/// @{
/// @brief     ResMgr RTEMS internal API.
///
/// --------------------------------------------------------------------------------
///


#ifndef _OS_DRV_RESMGR_INTERNAL_H_
#define _OS_DRV_RESMGR_INTERNAL_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "OsDrvResMgr.h"

#include <stdint.h>

#include "SglResMgrDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data
// ----------------------------------------------------------------------------

// SGL exported objects re-mapping.
typedef SglResMgrShaveL2cPartEntry OsDrvResMgrShaveL2cPartEntry;

// CPU lock scope.
#define OS_DRV_RESMGR_CPU_LOCK_LEON 0
#define OS_DRV_RESMGR_CPU_LOCK_SYSTEM 1

////////////////////////////////////////////////////////////////////////////////
//                                   / \                                      //
//                                  / ! \                                     //
//                                 /_____\                                    //
//                                                                            //
// All the IOCTL commands declared in this file are supposed to be for        //
// drivers' internal use only! A user application should not need to call     //
// them and, thus, should not need to include this file.                      //
////////////////////////////////////////////////////////////////////////////////


// Internal IOCTL commands.
typedef enum
{
    // Set the whole clock entry structure.
    // Param: OsDrvResMgrInfoClockEntry *
    OS_DRV_RESMGR_SET_CLOCK_ENTRY = OS_DRV_RESMGR_IOCTL_CMD_NB,
    // Set a clock entry field value.
    // Param: OsDrvResMgrClockEntryFieldParam *
    OS_DRV_RESMGR_SET_CLOCK_ENTRY_FIELD,
    // Get information for a specific SHAVE L2 cache partition.
    // Param: OsDrvResMgrShaveL2cPartParam *
    OS_DRV_RESMGR_GET_SHAVE_L2C_PART,
    // Get the whole SHAVE L2 cache partition map.
    // Param: OsDrvResMgrShaveL2cPartEntry *
    // /!\ Partition entries must be allocated as one array of
    // SGL_SHAVE_L2C_PART_NB elements.
    OS_DRV_RESMGR_GET_SHAVE_L2C_PART_MAP,
    // Set information for a specific SHAVE L2 cache partition.
    // Param: OsDrvResMgrShaveL2cPartParam *
    OS_DRV_RESMGR_SET_SHAVE_L2C_PART,
    // Set the whole SHAVE L2 cache partition map.
    // Param: OsDrvResMgrShaveL2cPartEntry *
    // /!\ Partition entries must be allocated as one array of
    // SGL_SHAVE_L2C_PART_NB elements.
    OS_DRV_RESMGR_SET_SHAVE_L2C_PART_MAP,
    // Enable CPU lock.
    // Param: OsDrvResMgrCpuLockParam *
    // /!\ 1: When under lock, the user should not modify the current task's
    // preemption mode as it will be overwritten on unlocking!
    // /!\ 2: The user should not get both locks (system-wide and LEON-only) at
    // the same time!
    OS_DRV_RESMGR_CPU_LOCK,
    // Disable CPU lock.
    // Param: OsDrvResMgrHandler *
    // /!\ This function writes the current task's preemption mode to what it
    // was before locking!
    OS_DRV_RESMGR_CPU_UNLOCK,
} OsDrvResMgrInternalControl;

// Parameters for OS_DRV_RESMGR_GET_SHAVE_L2C_PART and
// OS_DRV_RESMGR_SET_SHAVE_L2C_PART commands.
typedef struct
{
    OsDrvResMgrShaveL2cPartEntry part_entry;
    uint8_t id;
} OsDrvResMgrShaveL2cPartParam;

// Parameters for OS_DRV_RESMGR_CPU_LOCK command.
typedef struct
{
    OsDrvResMgrHandler *handler;
    uint8_t sys_wide;
} OsDrvResMgrCpuLockParam;

// 3:  Exported Functions
// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif //_OS_DRV_RESMGR_INTERNAL_H_

///@}
