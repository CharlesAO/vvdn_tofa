///
/// @file scResMgr.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup scResMgr
/// @{
/// @brief     Resource Manager baremetal Shave implementation
///

#include "scResMgr.h"
#include "scResMgrInternal.h"

#include <stdint.h>

#include "SglResMgr.h"
#include "SglResMgrDefines.h"
#include "SglCommon.h"
#include "svuCommonShave.h"

int32_t ScResMgrGetClockEntry(ScResMgrInfoClockEntry *infoclk) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrGetClockEntry(infoclk);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrGetClockEntryField(ScResMgrClk clock_entry, uint32_t *value) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrGetClockEntryField(clock_entry, value);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrSetClockEntry(ScResMgrInfoClockEntry *infoclk) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrSetClockEntry(infoclk);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrSetClockEntryField(ScResMgrClk clock_entry, uint32_t value) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrSetClockEntryField(clock_entry, value);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrGetShaveL2cPart(ScResMgrShaveL2cPartEntry *part_entry,
                                uint8_t id) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrGetShaveL2cPart(part_entry, id);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrGetShaveL2cPartMap(ScResMgrShaveL2cPartEntry part_entry[]) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrGetShaveL2cPartMap(part_entry);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrSetShaveL2cPart(ScResMgrShaveL2cPartEntry *part_entry,
                                uint8_t id) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrSetShaveL2cPart(part_entry, id);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrSetShaveL2cPartMap(ScResMgrShaveL2cPartEntry part_entry[]) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrSetShaveL2cPartMap(part_entry);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrAllocate(ScResMgrRequest *request) {
    uint32_t owner = SGL_RESMGR_OWNER;
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrAllocate(request, owner);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

static int32_t AllocateGroupNonAtomic(ScResMgrRequest request[],
                                      uint8_t req_nb) {
    if (request == NULL)
        return SGL_ERR_PARAM;

    uint32_t owner = SGL_RESMGR_OWNER;
    for (int i = 0; i < req_nb; i++) {
        scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
        SglResMgrAllocate(&request[i], owner);
        scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    }
    return SGL_SUCCESS;
}

static int32_t AllocateGroupAtomic(ScResMgrRequest request[], uint8_t req_nb) {
    if (request == NULL)
        return SGL_ERR_PARAM;

    uint32_t owner = SGL_RESMGR_OWNER;
    int32_t ret_val = SGL_SUCCESS;
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int i;
    for (i = 0; i < req_nb; i++) {
        ret_val = SglResMgrAllocate(&request[i], owner);
        if (ret_val != SGL_SUCCESS)
            break;
    }
    // If one allocation fails, release resources allocated before.
    if (ret_val != SGL_SUCCESS) {
        for (int j = 0; j < i; j++)
            SglResMgrRelease(request[j].handler, owner);
    }
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrAllocateGroup(ScResMgrRequest request[], uint8_t req_nb,
                              uint8_t atomic) {
    int32_t ret_val;
    if (atomic)
        ret_val = AllocateGroupAtomic(request, req_nb);
    else
        ret_val = AllocateGroupNonAtomic(request, req_nb);
    return ret_val;
}

int32_t ScResMgrRelease(ScResMgrHandler *handler) {
    uint32_t owner = SGL_RESMGR_OWNER;
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrRelease(handler, owner);
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t ScResMgrReleaseGroup(ScResMgrHandler handler[], uint8_t res_nb,
                             uint8_t *release_nb) {
    if ((handler == NULL) || (release_nb == NULL))
        return SGL_ERR_PARAM;

    int32_t ret_val = SGL_SUCCESS;
    uint32_t owner = SGL_RESMGR_OWNER;
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    int i;
    for (i = 0; i < res_nb; i++) {
        ret_val = SglResMgrRelease(&handler[i], owner);
        if (ret_val != SGL_SUCCESS)
            break;
    }
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
    *release_nb = i;
    return ret_val;
}

int32_t ScResMgrExecuteProtected(uint8_t level, int32_t protected_func(void *),
                                 void *arg, int32_t *res) {
    if (!protected_func || !res)
        return SGL_ERR_PARAM;

    uint32_t mutex_id = ((level == 0) ? SGL_RESMGR_SAFE_EXEC_L0_MUTEX_ID
                                      : SGL_RESMGR_SAFE_EXEC_L1_MUTEX_ID);
    scMutexRequest(mutex_id);
    *res = protected_func(arg);
    scMutexRelease(mutex_id);
    return SGL_SUCCESS;
}

int32_t ScResMgrCpuLock(uint8_t sys_wide, ScResMgrHandler *handler) {
    if (handler == NULL)
        return SGL_ERR_PARAM;

    uint32_t mutex_id = (sys_wide ? SGL_RESMGR_CPU_LOCK_SYS_MUTEX_ID
                                  : SGL_RESMGR_CPU_LOCK_SHAVE_MUTEX_ID);
    handler->type = SGL_RESMGR_MUTEX;
    handler->id = mutex_id;
    scMutexRequest(mutex_id);
    return SGL_SUCCESS;
}

int32_t ScResMgrCpuUnlock(ScResMgrHandler *handler) {
    if ((handler == NULL)
        || (handler->type != SGL_RESMGR_MUTEX)
        || ((handler->id != SGL_RESMGR_CPU_LOCK_SYS_MUTEX_ID)
            && (handler->id != SGL_RESMGR_CPU_LOCK_SHAVE_MUTEX_ID)))
        return SGL_ERR_PARAM;

    scMutexRelease(handler->id);
    return SGL_SUCCESS;
}

void ScResMgrInit(void) {
    scMutexRequest(SGL_RESMGR_OWN_MUTEX_ID);
    SglResMgrInit();
    scMutexRelease(SGL_RESMGR_OWN_MUTEX_ID);
}

///@}
