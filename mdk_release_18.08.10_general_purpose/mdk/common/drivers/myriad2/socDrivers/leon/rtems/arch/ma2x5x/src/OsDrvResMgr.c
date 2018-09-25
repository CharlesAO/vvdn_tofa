///
/// @file OsDrvResMgr.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvResMgr
/// @{
/// @brief     ResMgr RTEMS API Functions.
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "OsDrvResMgr.h"
#include "OsDrvResMgrInternal.h"

#include <stdint.h>
#include <stddef.h>

#include <rtems.h>
#include <rtems/libio.h>

#include "SglResMgr.h"
#include "SglCommon.h"
#include "OsDrvMutex.h"
#include "OsDrvMutexDefines.h"
#include "OsCommon.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

static rtems_id OsDrvResMgrSem;
static OS_MYRIAD_DRIVER_STATUS OsDrvResMgrControlStatus;

static osDrvMutexHandle_t OsDrvResMgrMutex;
static osDrvMutexHandle_t OsDrvResMgrSafeExecL0Mutex;
static osDrvMutexHandle_t OsDrvResMgrSafeExecL1Mutex;
static osDrvMutexHandle_t OsDrvResMgrCpuLockSysMutex;
static osDrvMutexHandle_t OsDrvResMgrCpuLockLeonMutex;

// Used in CPU lock/unlock feature to enable/disable preemption.
static rtems_mode OsDrvResMgrTaskPrevMode;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

/// Lock the semaphore and ResMgr's mutex so that ResMgr core can be accessed
/// safely.
/// Returns SGL_ERR_UNSATISFIED if any error, SGL_SUCCESS otherwise.
static int32_t OsDrvResMgrLock(void) {
    if (rtems_semaphore_obtain(OsDrvResMgrSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT)
        != RTEMS_SUCCESSFUL)
        return SGL_ERR_UNSATISFIED;
    if (OsDrvMutexObtain(&OsDrvResMgrMutex) != OS_MYR_DRV_SUCCESS) {
        rtems_semaphore_release(OsDrvResMgrSem);
        return SGL_ERR_UNSATISFIED;
    }

    return SGL_SUCCESS;
}

/// Release the semaphore and ResMgr's mutex.
/// Returns SGL_ERR_UNSATISFIED if any error, SGL_SUCCESS otherwise.
static int32_t OsDrvResMgrUnlock(void) {
    int32_t ret = SGL_SUCCESS;
    if (OsDrvMutexRelease(&OsDrvResMgrMutex) != OS_MYR_DRV_SUCCESS)
        ret = SGL_ERR_UNSATISFIED;
    if (rtems_semaphore_release(OsDrvResMgrSem) != RTEMS_SUCCESSFUL)
        ret = SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrGetClockEntry(void *arg) {
    OsDrvResMgrInfoClockEntry *infoclk = arg;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrGetClockEntry(infoclk);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrGetClockEntryField(void *arg) {
    OsDrvResMgrClockEntryFieldParam *param = arg;
    if (param == NULL)
        return SGL_ERR_PARAM;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrGetClockEntryField(param->clock_entry, &param->value);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrSetClockEntry(void *arg) {
    OsDrvResMgrInfoClockEntry *infoclk = arg;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrSetClockEntry(infoclk);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrSetClockEntryField(void *arg) {
    OsDrvResMgrClockEntryFieldParam *param = arg;
    if (param == NULL)
        return SGL_ERR_PARAM;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrSetClockEntryField(param->clock_entry, param->value);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrGetShaveL2cPart(void *arg) {
    OsDrvResMgrShaveL2cPartParam *param = arg;
    if (param == NULL)
        return SGL_ERR_PARAM;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrGetShaveL2cPart(&param->part_entry, param->id);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrGetShaveL2cPartMap(void *arg) {
    OsDrvResMgrShaveL2cPartEntry *part_entry = arg;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrGetShaveL2cPartMap(part_entry);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrSetShaveL2cPart(void *arg) {
    OsDrvResMgrShaveL2cPartParam *param = arg;
    if (param == NULL)
        return SGL_ERR_PARAM;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrSetShaveL2cPart(&param->part_entry, param->id);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrSetShaveL2cPartMap(void *arg) {
    OsDrvResMgrShaveL2cPartEntry *part_entry = arg;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrSetShaveL2cPartMap(part_entry);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrAllocate(void *arg) {
    OsDrvResMgrRequest *request = arg;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrAllocate(request, SGL_RESMGR_OWNER);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t AllocateGroupNonAtomic(OsDrvResMgrRequest *request,
                                             uint8_t req_nb) {
    if (request == NULL)
        return SGL_ERR_PARAM;

    uint32_t owner = SGL_RESMGR_OWNER;
    int32_t ret = SGL_SUCCESS;
    for (int i = 0; i < req_nb; i++) {
        // Mutex is released and re-acquired between each allocation.
        if (OsDrvResMgrLock() != SGL_SUCCESS)
            return SGL_ERR_UNSATISFIED;
        SglResMgrAllocate(&request[i], owner);
        if (OsDrvResMgrUnlock() != SGL_SUCCESS)
            return SGL_ERR_UNSATISFIED;
    }

    return ret;
}

static inline int32_t AllocateGroupAtomic(OsDrvResMgrRequest *request,
                                          uint8_t req_nb) {
    if (request == NULL)
        return SGL_ERR_PARAM;

    uint32_t owner = SGL_RESMGR_OWNER;
    int32_t ret_val = SGL_SUCCESS;
    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int i;
    for (i = 0; i < req_nb; i++) {
        SglResMgrAllocate(&request[i], owner);
        if (request[i].req_status != SGL_SUCCESS) {
            ret_val = request[i].req_status;
            break;
        }
    }
    // If one allocation fails, release resources allocated before.
    if (ret_val != SGL_SUCCESS) {
        for (int j = 0; j < i; j++)
            SglResMgrRelease(request[j].handler, owner);
    }
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    return ret_val;
}

static inline int32_t OsDrvResMgrAllocateGroup(void *arg) {
    OsDrvResMgrAllocateGroupParam *param = arg;
    if (param == NULL)
        return SGL_ERR_PARAM;

    int32_t ret_val;
    if (param->atomic)
        ret_val = AllocateGroupAtomic(param->request, param->req_nb);
    else
        ret_val = AllocateGroupNonAtomic(param->request, param->req_nb);
    return ret_val;
}

static inline int32_t OsDrvResMgrRelease(void *arg) {
    OsDrvResMgrHandler *handler = arg;

    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int32_t ret = SglResMgrRelease(handler, SGL_RESMGR_OWNER);
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrReleaseGroup(void *arg) {
    OsDrvResMgrReleaseGroupParam *param = arg;
    if ((param == NULL) || (param->handler == NULL))
        return SGL_ERR_PARAM;

    uint32_t owner = SGL_RESMGR_OWNER;
    int32_t ret_val = SGL_SUCCESS;
    if (OsDrvResMgrLock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    int i;
    for (i = 0; i < param->res_nb; i++) {
        ret_val = SglResMgrRelease(&param->handler[i], owner);
        if (ret_val != SGL_SUCCESS)
            break;
    }
    if (OsDrvResMgrUnlock() != SGL_SUCCESS)
        return SGL_ERR_UNSATISFIED;
    param->release_nb = i;

    return ret_val;
}

static inline int32_t OsDrvResMgrExecuteProtected(void *arg) {
    OsDrvResMgrExecuteProtectedParam *param = arg;
    if ((param == NULL) || (param->protected_func == NULL))
        return SGL_ERR_PARAM;

    int32_t ret = SGL_SUCCESS;
    osDrvMutexHandle_t *mutex_handler =
        ((param->level == 0) ? &OsDrvResMgrSafeExecL0Mutex
                             : &OsDrvResMgrSafeExecL1Mutex);
    // Get the RTEMS semaphore. Note that, because of this, two RTEMS threads
    // will not be able to execute a protected function at the same time EVEN IF
    // they do not use the same protection level.
    if (rtems_semaphore_obtain(OsDrvResMgrSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT)
        != RTEMS_SUCCESSFUL)
        return SGL_ERR_UNSATISFIED;
    // Get the ResMgr mutex for the requested level of protection.
    if (OsDrvMutexObtain(mutex_handler) != OS_MYR_DRV_SUCCESS) {
        rtems_semaphore_release(OsDrvResMgrSem);
        return SGL_ERR_UNSATISFIED;
    }
    // Call the protected function.
    param->res = param->protected_func(param->arg);
    // Release mutex and semaphore.
    if (OsDrvMutexRelease(mutex_handler) != OS_MYR_DRV_SUCCESS)
        ret = SGL_ERR_UNSATISFIED;
    if (rtems_semaphore_release(OsDrvResMgrSem) != RTEMS_SUCCESSFUL)
        ret = SGL_ERR_UNSATISFIED;

    return ret;
}

static inline int32_t OsDrvResMgrCpuLock(void *arg) {
    OsDrvResMgrCpuLockParam *param = arg;
    if ((param == NULL) || (param->handler == NULL))
        return SGL_ERR_PARAM;

    osDrvMutexHandle_t *mutex_handler =
        (param->sys_wide ? &OsDrvResMgrCpuLockSysMutex
                         : &OsDrvResMgrCpuLockLeonMutex);
    param->handler->type = SGL_RESMGR_MUTEX;
    param->handler->id = mutex_handler->mutexId;
    // Disable preemption for the current task during the time we are under CPU
    // lock so that we are not interrupted by other tasks and that the execution
    // of the protected piece of code is as fast as possible. Interrupts will
    // still be enabled and task switch will still be possible if the task
    // explicitly suspends itself (typically if we have to wait to get the HW
    // mutex). There is also no need to get the ResMgr semaphore now as
    // preemption will prevent any other task to run at all.
    if (rtems_task_mode(RTEMS_NO_PREEMPT, RTEMS_PREEMPT_MASK,
                        &OsDrvResMgrTaskPrevMode)
        != RTEMS_SUCCESSFUL)
        return SGL_ERR_UNSATISFIED;
    // Get the ResMgr mutex for the requested level of protection.
    if (OsDrvMutexObtain(mutex_handler) != OS_MYR_DRV_SUCCESS) {
        rtems_semaphore_release(OsDrvResMgrSem);
        return SGL_ERR_UNSATISFIED;
    }
    return SGL_SUCCESS;
}

static inline int32_t OsDrvResMgrCpuUnlock(void *arg) {
    OsDrvResMgrHandler *handler = arg;
    if ((handler == NULL) || (handler->type != SGL_RESMGR_MUTEX))
        return SGL_ERR_PARAM;

    // Release mutex and put the task back to its former mode.
    int32_t ret = SGL_SUCCESS;
    osDrvMutexHandle_t *mutex_handler;
    switch (handler->id) {
        case SGL_RESMGR_CPU_LOCK_SYS_MUTEX_ID:
            mutex_handler = &OsDrvResMgrCpuLockSysMutex;
            break;
        case SGL_RESMGR_CPU_LOCK_LEON_MUTEX_ID:
            mutex_handler = &OsDrvResMgrCpuLockLeonMutex;
            break;
        default:
            return SGL_ERR_PARAM;
    }
    if (OsDrvMutexRelease(mutex_handler) != OS_MYR_DRV_SUCCESS)
        ret = SGL_ERR_UNSATISFIED;
    if (rtems_task_mode(OsDrvResMgrTaskPrevMode, RTEMS_PREEMPT_MASK,
                        &OsDrvResMgrTaskPrevMode)
        != RTEMS_SUCCESSFUL)
        ret = SGL_ERR_UNSATISFIED;
    return ret;
}

// /!\ This function can only be called once OsDrvMutexInit has been performed!!
static OS_MYRIAD_DRIVER_STATUS_CODE OsDrvResMgrInitialize(
    rtems_device_major_number major, rtems_device_minor_number minor) {
    OS_MYRIAD_DRIVER_STATUS_CODE ret_val = OS_MYR_DRV_ERROR;
    if (OsDrvResMgrControlStatus == OS_DRV_STAT_UNINITIALIZED) {
        // Create one global semaphore to be used by any tasks.
        if (rtems_semaphore_create(
                rtems_build_name('R', 'E', 'S', 'M'),
                1, // Initial count to 1 as it is a binary semaphore.
                RTEMS_BINARY_SEMAPHORE | RTEMS_PRIORITY | RTEMS_INHERIT_PRIORITY,
                0, // Priority ceiling value set to 0 as we do not use it.
                (rtems_id *)&OsDrvResMgrSem)
            != RTEMS_SUCCESSFUL) {
            return OS_MYR_DRV_ERROR;
        }
        // Get a handler on ResMgr mutex. This handler will never be closed.
        if (OsDrvMutexOpen(&OsDrvResMgrMutex, SGL_RESMGR_OWN_MUTEX_ID)
            != OS_MYR_DRV_SUCCESS)
            return OS_MYR_DRV_ERROR;
        // Initialise ResMgr core.
        if (OsDrvMutexObtain(&OsDrvResMgrMutex) != OS_MYR_DRV_SUCCESS)
            return OS_MYR_DRV_ERROR;
        SglResMgrInit();
        if (OsDrvMutexRelease(&OsDrvResMgrMutex) != OS_MYR_DRV_SUCCESS)
            return OS_MYR_DRV_ERROR;
        // Get handlers on ResMgr execute protected L0 and L1 mutexes. These
        // handlers will never be closed.
        if (OsDrvMutexOpen(&OsDrvResMgrSafeExecL0Mutex,
                           SGL_RESMGR_SAFE_EXEC_L0_MUTEX_ID)
            != OS_MYR_DRV_SUCCESS)
            return OS_MYR_DRV_ERROR;
        if (OsDrvMutexOpen(&OsDrvResMgrSafeExecL1Mutex,
                           SGL_RESMGR_SAFE_EXEC_L1_MUTEX_ID)
            != OS_MYR_DRV_SUCCESS)
            return OS_MYR_DRV_ERROR;
        // Get handlers on ResMgr CPU lock mutexes. These handlers will never be
        // closed.
        if (OsDrvMutexOpen(&OsDrvResMgrCpuLockSysMutex,
                           SGL_RESMGR_CPU_LOCK_SYS_MUTEX_ID)
            != OS_MYR_DRV_SUCCESS)
            return OS_MYR_DRV_ERROR;
        if (OsDrvMutexOpen(&OsDrvResMgrCpuLockLeonMutex,
                           SGL_RESMGR_CPU_LOCK_LEON_MUTEX_ID)
            != OS_MYR_DRV_SUCCESS)
            return OS_MYR_DRV_ERROR;
        // Register with the minor number from virtual channel.
        rtems_device_driver status = rtems_io_register_name(
            OS_DRV_RESMGR_DEVNAME, major, minor);
        if (status) {
            ret_val = OS_MYR_DRV_ERROR;
        } else {
            OsDrvResMgrControlStatus = OS_DRV_STAT_INITIALIZED;
            ret_val = OS_MYR_DRV_SUCCESS;
        }
    } else if (OsDrvResMgrControlStatus == OS_DRV_STAT_INITIALIZED) {
        ret_val = OS_MYR_DRV_ALREADY_INITIALIZED;
    } else {
        ret_val = OS_MYR_DRV_ERROR;
    }
    return ret_val;
}

static rtems_status_code resmgr_initialize(rtems_device_major_number major,
                                           rtems_device_minor_number minor,
                                           void *args) {
    UNUSED(args); // Hush the compiler warning.

    OS_MYRIAD_DRIVER_STATUS_CODE ret_val = OsDrvResMgrInitialize(major, minor);
    if (ret_val == OS_MYR_DRV_ERROR)
        return RTEMS_INTERNAL_ERROR;
    else
        return RTEMS_SUCCESSFUL;
}

static rtems_device_driver resmgr_open(rtems_device_major_number major,
                                       rtems_device_minor_number minor,
                                       void *args) {
    // Hush compiler warnings.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(args);
    // Nothing to do.
    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver resmgr_close(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *args) {
    // Hush compiler warnings.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(args);
    // Nothing to do.
    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver resmgr_read(rtems_device_major_number major,
                                       rtems_device_minor_number minor,
                                       void *args) {
    // Hush compiler warnings.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(args);
    // Nothing to do.
    return RTEMS_NOT_IMPLEMENTED;
}

static rtems_device_driver resmgr_write(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *args) {
    // Hush compiler warnings.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(args);
    // Nothing to do.
    return RTEMS_NOT_IMPLEMENTED;
}

static rtems_device_driver resmgr_control(rtems_device_major_number major,
                                          rtems_device_minor_number minor,
                                          void *args) {
    // Hush compiler warnings.
    UNUSED(major);
    UNUSED(minor);

    rtems_libio_ioctl_args_t *ctl = args;

    if (ctl == NULL)
        return RTEMS_INVALID_ADDRESS;

    int32_t ret_val;
    switch (ctl->command) {
        case OS_DRV_RESMGR_GET_CLOCK_ENTRY:
            ret_val = OsDrvResMgrGetClockEntry(ctl->buffer);
            break;

        case OS_DRV_RESMGR_GET_CLOCK_ENTRY_FIELD:
            ret_val = OsDrvResMgrGetClockEntryField(ctl->buffer);
            break;

        case OS_DRV_RESMGR_SET_CLOCK_ENTRY:
            ret_val = OsDrvResMgrSetClockEntry(ctl->buffer);
            break;

        case OS_DRV_RESMGR_SET_CLOCK_ENTRY_FIELD:
            ret_val = OsDrvResMgrSetClockEntryField(ctl->buffer);
            break;

        case OS_DRV_RESMGR_GET_SHAVE_L2C_PART:
            ret_val = OsDrvResMgrGetShaveL2cPart(ctl->buffer);
            break;

        case OS_DRV_RESMGR_GET_SHAVE_L2C_PART_MAP:
            ret_val = OsDrvResMgrGetShaveL2cPartMap(ctl->buffer);
            break;

        case OS_DRV_RESMGR_SET_SHAVE_L2C_PART:
            ret_val = OsDrvResMgrSetShaveL2cPart(ctl->buffer);
            break;

        case OS_DRV_RESMGR_SET_SHAVE_L2C_PART_MAP:
            ret_val = OsDrvResMgrSetShaveL2cPartMap(ctl->buffer);
            break;

        case OS_DRV_RESMGR_ALLOCATE:
            ret_val = OsDrvResMgrAllocate(ctl->buffer);
            break;

        case OS_DRV_RESMGR_ALLOCATE_GROUP:
            ret_val = OsDrvResMgrAllocateGroup(ctl->buffer);
            break;

        case OS_DRV_RESMGR_RELEASE:
            ret_val = OsDrvResMgrRelease(ctl->buffer);
            break;

        case OS_DRV_RESMGR_RELEASE_GROUP:
            ret_val = OsDrvResMgrReleaseGroup(ctl->buffer);
            break;

        case OS_DRV_RESMGR_EXECUTE_PROTECTED:
            ret_val = OsDrvResMgrExecuteProtected(ctl->buffer);
            break;

        case OS_DRV_RESMGR_CPU_LOCK:
            ret_val = OsDrvResMgrCpuLock(ctl->buffer);
            break;

        case OS_DRV_RESMGR_CPU_UNLOCK:
            ret_val = OsDrvResMgrCpuUnlock(ctl->buffer);
            break;

        default:
            ret_val = SGL_ERR_PARAM;
    }

    // Translate SGL return codes into RTEMS ones.
    switch (ret_val) {
        case SGL_SUCCESS:
            ctl->ioctl_return = RTEMS_SUCCESSFUL;
            break;

        case SGL_ERR_PARAM:
            ctl->ioctl_return = RTEMS_INVALID_ADDRESS;
            break;

        case SGL_ERR_RES_BUSY:
            ctl->ioctl_return = RTEMS_RESOURCE_IN_USE;
            break;

        case SGL_ERR_OWNER:
            // Actually SGL_ERR_OWNER is sometimes used to indicate that the
            // user is already the owner of the resource it requests. But there
            // is no such error code in RTEMS so we will stick to this one as it
            // still eases debug, even if it can be a bit confusing in some
            // cases.
            ctl->ioctl_return = RTEMS_NOT_OWNER_OF_RESOURCE;
            break;

        case SGL_ERR_UNSATISFIED:
            ctl->ioctl_return = RTEMS_UNSATISFIED;
            break;

        default:
            ctl->ioctl_return = RTEMS_IO_ERROR;
    }

    return ctl->ioctl_return;
}

rtems_driver_address_table OsDrvResMgrTbl = {
    resmgr_initialize,
    resmgr_open,
    resmgr_close,
    resmgr_read,
    resmgr_write,
    resmgr_control
};


///@}
