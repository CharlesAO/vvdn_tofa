///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Power manager implementation
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <assert.h>
#include <rtems/libio.h>
#include <rtems/status-checks.h>

#include "OsCommon.h"
#include "pwr_manager.h"
#include "pwr_manager_lp_api.h"
#include "swcWhoAmI.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define OS_DRV_PWRMANAGER_L1   //Errors
//#define OS_DRV_PWRMANAGER_L2   //Warnings
//#define OS_DRV_PWRMANAGER_L3   //Debug prints

#ifdef OS_DRV_PWRMANAGER_L1
#define OS_PWRMANAGER_DPRINTF1(...)            printf(__VA_ARGS__)
#else
#define OS_PWRMANAGER_DPRINTF1(...)
#endif

#ifdef OS_DRV_PWRMANAGER_L2
#define OS_PWRMANAGER_DPRINTF2(...)            printf(__VA_ARGS__)
#else
#define OS_PWRMANAGER_DPRINTF2(...)
#endif

#ifdef OS_DRV_PWRMANAGER_L3
#define OS_PWRMANAGER_DPRINTF3(...)            printf(__VA_ARGS__)
#else
#define OS_PWRMANAGER_DPRINTF3(...)
#endif

#if defined(OS_DRV_PWRMANAGER_L1) || defined(OS_DRV_PWRMANAGER_L2) || defined(OS_DRV_PWRMANAGER_L3)
#define OS_DRV_PWRMANAGER_DEBUG 1
#else
#define OS_DRV_PWRMANAGER_DEBUG 0
#endif

#define DEVICE_CLOSED           0
#define DEVICE_OPENED           1

#define CALLBACK_UNINITIALIZED  0
#define CALLBACK_INITIALIZED    1

typedef struct {
    PwrManagerPowerStates           pstates [POWER_MANAGER_MAX_STATES];
    volatile int8_t                 next_pstate;
    int8_t                          pstate_cnt;

    rtems_id                        mutex;              // Mutex between devices
    struct {
        PwrManagerCallbacksTable    callbacks;          // pointer to the list of callbacks for the current device
        void *                      callback_argument;  // argument the callbacks will be called with
        uint8_t                     callback_status;    // mark if cb entry is valid
        uint8_t                     dev_status;         // mark if device is opened
    } data[POWER_MANAGER_MAX_USERS];                    // Table of data for each device registered with Power Manager

} PwrManagerData;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern LowPowerInfo low_power_data;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static PwrManagerData * pwrManager=NULL;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

//power manager registration
static rtems_status_code registerDevice(rtems_device_major_number major,
                                        rtems_device_minor_number minor) {
    rtems_status_code status;
    OS_PWRMANAGER_DPRINTF3("%s() called with arguments: %lu %lu\n", __func__, major, minor);

    assert(pwrManager == NULL);

    if (swcWhoAmI() != PROCESS_LEON_OS) return RTEMS_INCORRECT_STATE;

    pwrManager = malloc(sizeof(PwrManagerData));
    if (pwrManager == NULL) {
        OS_PWRMANAGER_DPRINTF1("%s() unable to allocate data\n", __func__);
        return RTEMS_NO_MEMORY;
    }

    memset(pwrManager, 0, sizeof(PwrManagerData));

    // Create one global semaphore to be used by all tasks
    status = rtems_semaphore_create(rtems_build_name('P', 'W', 'R', 'M'),
                                    1,
                                    RTEMS_BINARY_SEMAPHORE,
                                    0,
                                    (rtems_id *) &pwrManager->mutex);
    if (status != RTEMS_SUCCESSFUL) {
        OS_PWRMANAGER_DPRINTF1("%s() unable to allocate semaphone\n", __func__);
        goto error_semaphore;
    }

    //Initialize pstates
    pwrManager->next_pstate = -1;

    //Initialize device data

    // foreach idx: pwrManager->data[idx].dev_status = DEVICE_CLOSED;
    // foreach idx: pwrManager->data[idx].callback_status = CALLBACK_UNINITIALIZED;
    // Initialization omitted because already done by memset.

    //register with the minor number from virtual channel
    status = rtems_io_register_name(OSDRVPWRMANAGER_DEVNAME, major, minor);
    if (status != RTEMS_SUCCESSFUL) {
        OS_PWRMANAGER_DPRINTF1("rtems_io_register_name failed with %d \n",status);
        goto error_io_name;
    }

    return RTEMS_SUCCESSFUL;

    error_io_name:
        rtems_semaphore_delete(pwrManager->mutex);
    error_semaphore:
        free(pwrManager);
        pwrManager=NULL;

    return status;

}

//implementation for rtems_io_initialize: power modes initialization for the component
static rtems_status_code initializeDevice(PwrManagerPowerStates *pstates) {
    int i;
    assert(pstates != NULL);
    assert(pwrManager != NULL);

    for (i=0;i<POWER_MANAGER_MAX_STATES;i++)
    {
        if (pstates[i].mode == POWER_MANAGER_MODE_NONE)
            break;
        if (pstates[i].mode >= POWER_MANAGER_MODE_MAX_IDX || pstates[i].mode == POWER_MANAGER_MODE_ACTIVE)
            return RTEMS_INCORRECT_STATE;
        pwrManager->pstates[i] = pstates[i];
    }
    if (i == POWER_MANAGER_MAX_STATES)
        return RTEMS_TOO_MANY;
    if (i == 0)
        return RTEMS_NOT_CONFIGURED;

    pwrManager->pstate_cnt = i;

    OS_PWRMANAGER_DPRINTF3("%s() called with %d power states\n", __func__, i);

    return RTEMS_SUCCESSFUL;
}

//add a list of callbacks for the current client
static rtems_status_code addCallbackEntries(PwrManagerIoctlCallback* cb_data, const uint32_t idx) {

    assert(cb_data != NULL);
    assert(pwrManager != NULL);
    assert(idx < POWER_MANAGER_MAX_USERS);

    OS_PWRMANAGER_DPRINTF3("%s() called for idx: %lu\n", __func__, idx);

    if (pwrManager->data[idx].dev_status != DEVICE_OPENED)
        return RTEMS_NOT_CONFIGURED;

    pwrManager->data[idx].callbacks = cb_data->callbacks;
    pwrManager->data[idx].callback_argument = cb_data->callback_argument;
    pwrManager->data[idx].callback_status = CALLBACK_INITIALIZED;

    return RTEMS_SUCCESSFUL;
}

//delete the list of callbacks for the current client
static rtems_status_code delCallbackEntries(PwrManagerIoctlCallback* cb_data, const uint32_t idx) {

    assert(cb_data != NULL);
    assert(pwrManager != NULL);
    assert(idx < POWER_MANAGER_MAX_USERS);

    OS_PWRMANAGER_DPRINTF3("%s() called for idx: %lu\n", __func__, idx);

    if (pwrManager->data[idx].dev_status != DEVICE_OPENED)
        return RTEMS_NOT_CONFIGURED;

    if (pwrManager->data[idx].callback_status != CALLBACK_INITIALIZED)
        return RTEMS_NOT_CONFIGURED;

    //not needed because we set the status bit to 0.
    memset(&pwrManager->data[idx].callbacks, 0 , sizeof(PwrManagerCallbacksTable));
    pwrManager->data[idx].callback_status = CALLBACK_UNINITIALIZED;

    return RTEMS_SUCCESSFUL;
}

//switch the current power state to the requested one, if valid
static rtems_status_code switchPowerState(PwrManagerIoctlPowerState* pstate) {

    assert(pstate != NULL);
    assert(pwrManager != NULL);
    if (pstate->mode_idx >=0 && pstate->mode_idx < pwrManager->pstate_cnt) {
        //sanity check: check is mode is configured from the user
        if (pwrManager->pstates[pstate->mode_idx].mode == POWER_MANAGER_MODE_NONE)
            return RTEMS_NOT_DEFINED;
        pwrManager->next_pstate = pstate->mode_idx;
        return RTEMS_SUCCESSFUL;
    } else if (pstate->mode_idx == -1) {
        // Allow the user to cancel a previously set power mode
        pwrManager->next_pstate = -1;
        return RTEMS_SUCCESSFUL;
    } else
        return RTEMS_INCORRECT_STATE;
}

//check the current power state
static rtems_status_code readPowerState(PwrManagerIoctlPowerState* pstate) {

    assert(pstate != NULL);
    assert(pwrManager != NULL);

    pstate->mode_idx = pwrManager->next_pstate;

    return RTEMS_SUCCESSFUL;
}

//rtems_io_control implementation
static rtems_status_code pwrManagerControl(rtems_device_major_number major,
                                           rtems_device_minor_number minor,
                                           void *e) {
    rtems_status_code ret = RTEMS_SUCCESSFUL;
    UNUSED(major);// hush the compiler warning.
    UNUSED(minor);// hush the compiler warning.

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) e;

    assert (pwrManager != NULL);

    if (!ctl || !ctl->buffer)
        return RTEMS_INVALID_ADDRESS;

    if (!ctl->iop)
        return RTEMS_INCORRECT_STATE;

    OS_PWRMANAGER_DPRINTF3("%s() called with arguments: %p %lu %p\n",
                           __func__, ctl->iop, ctl->command, ctl->buffer);

    const uint32_t idx = ctl->iop->data0;
    if (pwrManager->data[idx].dev_status != DEVICE_OPENED)
        return RTEMS_NOT_CONFIGURED;

    ret = rtems_semaphore_obtain(pwrManager->mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (ret != RTEMS_SUCCESSFUL)
        return ret;

    switch (ctl->command) {
        case PWR_MANAGER_ADD_NOTIFICATION_CALLBACKS:
            ret = addCallbackEntries(ctl->buffer, idx);
            break;
        case PWR_MANAGER_DEL_NOTIFICATION_CALLBACKS:
            ret = delCallbackEntries(ctl->buffer, idx);
            break;
        case PWR_MANAGER_SWITCH_POWER_STATE:
            ret = switchPowerState(ctl->buffer);
            break;
        case PWR_MANAGER_GET_CURRENT_POWER_STATE:
            ret = readPowerState(ctl->buffer);
            break;
        default:
            ret = RTEMS_UNSATISFIED;
    }

    rtems_semaphore_release(pwrManager->mutex);
    ctl->ioctl_return = ret;
    return ret;
}

//handles driver registration and initialization
static rtems_status_code pwrManagerInitialize(rtems_device_major_number major,
                                              rtems_device_minor_number minor,
                                              void *e) {
    if (e) { // rtems_io_initialize
        return initializeDevice((PwrManagerPowerStates *)e);
    } else { //rtems_io_register_driver
        if (pwrManager == NULL)
            return registerDevice(major, minor);
        else
            return RTEMS_INTERNAL_ERROR;
    }
}

//rtems_io_open implementation
//open one client storage area within power manager
static rtems_status_code pwrManagerOpen(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *e) {
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);

    rtems_libio_open_close_args_t* arg = (rtems_libio_open_close_args_t *) e;

    if (pwrManager == NULL)
        return RTEMS_NOT_CONFIGURED;

    if (!arg || !arg->iop)
        return RTEMS_INCORRECT_STATE;

    OS_PWRMANAGER_DPRINTF3("%s() called with arguments: %p %lu %lu\n", __func__, arg->iop, arg->flags, arg->mode);

    rtems_status_code ret = rtems_semaphore_obtain(pwrManager->mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (ret != RTEMS_SUCCESSFUL)
        return ret;

    uint32_t idx;
    for (idx=0;idx<POWER_MANAGER_MAX_USERS;idx++)
        if (pwrManager->data[idx].dev_status == DEVICE_CLOSED)
            break;
    if (idx == POWER_MANAGER_MAX_USERS)
        return RTEMS_TOO_MANY;

    assert(pwrManager->data[idx].callback_status == CALLBACK_UNINITIALIZED);

    pwrManager->data[idx].dev_status = DEVICE_OPENED;
    arg->iop->data0 = idx;

    rtems_semaphore_release(pwrManager->mutex);

    return RTEMS_SUCCESSFUL;
}

//rtems_io_close implementation
//close the current client storage area within power manager
static rtems_status_code pwrManagerClose(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *e) {
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    rtems_libio_open_close_args_t* arg = (rtems_libio_open_close_args_t *) e;

    if (pwrManager == NULL)
        return RTEMS_NOT_CONFIGURED;

    if (!arg || !arg->iop)
        return RTEMS_INCORRECT_STATE;

    const uint32_t idx = arg->iop->data0;

    OS_PWRMANAGER_DPRINTF3("%s() called with arguments: %p\n", __func__, arg->iop);

    if (pwrManager->data[idx].dev_status == DEVICE_CLOSED)
        return RTEMS_INCORRECT_STATE;

    rtems_status_code ret = rtems_semaphore_obtain(pwrManager->mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (ret != RTEMS_SUCCESSFUL)
        return ret;

    pwrManager->data[idx].dev_status = DEVICE_CLOSED;
    pwrManager->data[idx].callback_status = CALLBACK_UNINITIALIZED;

    rtems_semaphore_release(pwrManager->mutex);

    return RTEMS_SUCCESSFUL;
}

// 6: Functions Implementation
// ----------------------------------------------------------------------------

rtems_driver_address_table osDrvPwrManagerTbl = \
{                           \
    pwrManagerInitialize,   \
    pwrManagerOpen,         \
    pwrManagerClose,        \
    NULL,                   \
    NULL,                   \
    pwrManagerControl       \
};

// ------------------------------------------------------------------------------------
// -----------IDLE LOOP----------------------------------------------------------------
// ------------------------------------------------------------------------------------

#if OS_DRV_PWRMANAGER_DEBUG
static volatile unsigned long __debug_pwrManager_idle_loop_cnt=0;
static volatile unsigned long __debug_pwrManager_active_cnt=0;
static volatile unsigned long __debug_pwrManager_lp_executed_cnt=0;
static volatile unsigned long __debug_pwrManager_lp_cancelled_cnt=0;
static volatile unsigned long __debug_pwrManager_sleep_cnt=0;
static volatile unsigned long __debug_pwrManager_error_cnt=0;
#endif

void pwrManagerDebugValues(void) {
#if OS_DRV_PWRMANAGER_DEBUG
    printf("[T:%lx a:%lx lp: %lx/%lx s:%lx e: %lx]\n",  __debug_pwrManager_idle_loop_cnt,
                                                    __debug_pwrManager_active_cnt,
                                                    __debug_pwrManager_lp_executed_cnt,
                                                    __debug_pwrManager_lp_cancelled_cnt,
                                                    __debug_pwrManager_sleep_cnt,
                                                    __debug_pwrManager_error_cnt );
#endif
}

#define iterateOverCallbacks(MEMBER , MAX_IDX)  ({ \
        uint32_t _idx = 0; \
        for (_idx=0;_idx < MAX_IDX; _idx++) { \
            if ( (pwrManager->data[_idx].dev_status == DEVICE_OPENED) && \
                 (pwrManager->data[_idx].callback_status == CALLBACK_INITIALIZED)) { \
                if (pwrManager->data[_idx].callbacks. MEMBER != NULL) { \
                    if (RTEMS_SUCCESSFUL != pwrManager->data[_idx].callbacks. MEMBER (pwrManager->data[_idx].callback_argument)) \
                        break; \
                } } } \
        _idx; /* return value */ \
    })

static inline rtems_status_code pwrManagerModeActive(void) {
#if OS_DRV_PWRMANAGER_DEBUG
    __debug_pwrManager_active_cnt++;
#endif
    asm volatile("wr %g0, %asr19;");
    return RTEMS_SUCCESSFUL;
}

static inline rtems_status_code pwrManagerModeLowPower(int32_t pstate_idx) {
    uint32_t psr;
    rtems_interrupt_disable(psr);

    //try to get authorization to enter on LP mode
    const uint32_t idx = iterateOverCallbacks(powerChange_request, POWER_MANAGER_MAX_USERS);
    if (idx != POWER_MANAGER_MAX_USERS) {
#if OS_DRV_PWRMANAGER_DEBUG
        __debug_pwrManager_lp_cancelled_cnt++;
#endif
        iterateOverCallbacks(powerChange_cancel, idx);
        //As long as entering Low Power was not authorized, we enable ISR and execute Active mode before retrying
        rtems_interrupt_enable(psr);
        pwrManagerModeActive();
        return RTEMS_UNSATISFIED;
    }
#if OS_DRV_PWRMANAGER_DEBUG
    __debug_pwrManager_lp_executed_cnt++;
#endif
    iterateOverCallbacks(powerChange_approved, POWER_MANAGER_MAX_USERS);
    //write configuration
    low_power_data = pwrManager->pstates[pstate_idx].low_power_data;
    pwrManagerEnterLowPower();
    iterateOverCallbacks(powerChange_restore, POWER_MANAGER_MAX_USERS);

    rtems_interrupt_enable(psr);

    return RTEMS_SUCCESSFUL;
}

//switch to sleep mode (stub)
static inline rtems_status_code pwrManagerModeSleep(int32_t pstate_idx) {
    UNUSED(pstate_idx);
#if OS_DRV_PWRMANAGER_DEBUG
    __debug_pwrManager_sleep_cnt++;
#endif
    // unimplemented

    return RTEMS_SUCCESSFUL;
}

//exceptional error handling on the requested mode (stub)
static inline rtems_status_code pwrManagerModeError(void) {
#if OS_DRV_PWRMANAGER_DEBUG
    __debug_pwrManager_error_cnt++;
#endif
    //unimplemented

    return RTEMS_SUCCESSFUL;
}

void *pwrManagerIdleLoop(uintptr_t ignored) {
    UNUSED(ignored);

    while(1) {
        int rv;
        struct stat st;

#if OS_DRV_PWRMANAGER_DEBUG
        __debug_pwrManager_idle_loop_cnt++;
#endif
        rv = stat(OSDRVPWRMANAGER_DEVNAME, &st );

        if ( rv==0 && S_ISCHR( st.st_mode ) )
            break;
        else
            pwrManagerModeActive();
    }

    assert(pwrManager != NULL);

    while (1) {
#if OS_DRV_PWRMANAGER_DEBUG
        __debug_pwrManager_idle_loop_cnt++;
#endif
        int8_t curr_pstate_idx = pwrManager->next_pstate;
        assert(curr_pstate_idx < pwrManager->pstate_cnt);
        if (curr_pstate_idx == -1) {
            pwrManagerModeActive();
        } else {
            rtems_status_code ret;
            //TODO: we could improve semaphore management code here
            if (RTEMS_SUCCESSFUL != rtems_semaphore_obtain(pwrManager->mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT))
                continue;
            switch (pwrManager->pstates[curr_pstate_idx].mode) {
                case POWER_MANAGER_MODE_ACTIVE:
                    ret = pwrManagerModeActive();
                    break;
                case POWER_MANAGER_MODE_LOW_POWER:
                    ret = pwrManagerModeLowPower(curr_pstate_idx);
                    break;
                case POWER_MANAGER_MODE_SLEEP:
                    ret = pwrManagerModeSleep(curr_pstate_idx);
                    break;
                default:
                    ret = pwrManagerModeError();
            }

            if (ret == RTEMS_SUCCESSFUL)
                pwrManager->next_pstate = -1;
            rtems_semaphore_release(pwrManager->mutex);
        }
    }
    return 0;
}

