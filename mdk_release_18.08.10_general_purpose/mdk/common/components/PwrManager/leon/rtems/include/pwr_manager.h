///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     power manager API definitions
///

#ifndef _OS_DRV_PWRMANAGER_H_
#define _OS_DRV_PWRMANAGER_H_

// 1: Includes
// ----------------------------------------------------------------------------

#include <OsCommon.h>
#include "pwr_manager_lp_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OSDRVPWRMANAGER_DEVNAME     "/dev/pwr_manager"
#define POWER_MANAGER_MAX_USERS     16
#define POWER_MANAGER_MAX_STATES    16

typedef enum {
    POWER_MANAGER_MODE_NONE = 0,
    POWER_MANAGER_MODE_ACTIVE,
    POWER_MANAGER_MODE_LOW_POWER,
    POWER_MANAGER_MODE_SLEEP,
    POWER_MANAGER_MODE_MAX_IDX
} PwrManagerPowerMode;

typedef enum {
    PWR_MANAGER_ADD_NOTIFICATION_CALLBACKS,
    PWR_MANAGER_DEL_NOTIFICATION_CALLBACKS,
    PWR_MANAGER_SWITCH_POWER_STATE,
    PWR_MANAGER_GET_CURRENT_POWER_STATE,
} PwrManagerIoctl;

typedef struct {
    PwrManagerPowerMode mode;
    LowPowerInfo low_power_data;
} PwrManagerPowerStates;

typedef rtems_status_code (*PwrManagerCallback)(void *);

typedef struct {
  PwrManagerCallback powerChange_request;
  PwrManagerCallback powerChange_cancel;
  PwrManagerCallback powerChange_approved;
  PwrManagerCallback powerChange_restore;
} PwrManagerCallbacksTable;

typedef struct {
    PwrManagerCallbacksTable callbacks;
    void * callback_argument;
} PwrManagerIoctlCallback;

typedef struct {
    int32_t mode_idx;
} PwrManagerIoctlPowerState;

typedef union {
    PwrManagerIoctlCallback cb_data;
    PwrManagerIoctlPowerState pstate_data;
} PwrManagerIoctlData;

// 2:  Exported Global Data
// ----------------------------------------------------------------------------

// 3:  Exported Functions
// ----------------------------------------------------------------------------

extern rtems_driver_address_table osDrvPwrManagerTbl;
extern void *pwrManagerIdleLoop(uintptr_t ignored);
extern void pwrManagerDebugValues(void);

#define CONFIGURE_IDLE_TASK_BODY pwrManagerIdleLoop

#ifdef __cplusplus
}
#endif

#endif //_OS_DRV_PWRMANAGER_H_
