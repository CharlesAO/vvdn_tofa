///
/// @file DrvResMgr.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvResMgr
/// @{
/// @brief     Resource Manager baremetal Leon implementation
///

#include "DrvResMgr.h"
#include "DrvResMgrInternal.h"

#include <stdint.h>

#include "SglResMgr.h"
#include "SglResMgrDefines.h"
#include "DrvCommon.h"
#include "DrvMutex.h"

int32_t DrvResMgrGetClockEntry(DrvResMgrInfoClockEntry *infoclk) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrGetClockEntry(infoclk);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrGetClockEntryField(DrvResMgrClk clock_entry, uint32_t *value) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrGetClockEntryField(clock_entry, value);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrSetClockEntry(DrvResMgrInfoClockEntry *infoclk) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrSetClockEntry(infoclk);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrSetClockEntryField(DrvResMgrClk clock_entry, uint32_t value) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrSetClockEntryField(clock_entry, value);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrGetShaveL2cPart(DrvResMgrShaveL2cPartEntry *part_entry,
                                 uint8_t id) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrGetShaveL2cPart(part_entry, id);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrGetShaveL2cPartMap(DrvResMgrShaveL2cPartEntry part_entry[]) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrGetShaveL2cPartMap(part_entry);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrSetShaveL2cPart(DrvResMgrShaveL2cPartEntry *part_entry,
                                 uint8_t id) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrSetShaveL2cPart(part_entry, id);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrSetShaveL2cPartMap(DrvResMgrShaveL2cPartEntry part_entry[]) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrSetShaveL2cPartMap(part_entry);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrAllocate(DrvResMgrRequest *request) {
    uint32_t owner = SGL_RESMGR_OWNER;
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrAllocate(request, owner);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

static int32_t AllocateGroupNonAtomic(DrvResMgrRequest request[],
                                      uint8_t req_nb) {
    if (request == NULL)
        return MYR_DRV_INVALID_PARAM;

    uint32_t owner = SGL_RESMGR_OWNER;
    for (int i = 0; i < req_nb; i++) {
        DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
        SglResMgrAllocate(&request[i], owner);
        DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    }
    return MYR_DRV_SUCCESS;
}

static int32_t AllocateGroupAtomic(DrvResMgrRequest request[], uint8_t req_nb) {
    if (request == NULL)
        return MYR_DRV_INVALID_PARAM;

    uint32_t owner = SGL_RESMGR_OWNER;
    int32_t ret_val = MYR_DRV_SUCCESS;
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
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
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrAllocateGroup(DrvResMgrRequest request[], uint8_t req_nb,
                               uint8_t atomic) {
    int32_t ret_val;
    if (atomic)
        ret_val = AllocateGroupAtomic(request, req_nb);
    else
        ret_val = AllocateGroupNonAtomic(request, req_nb);
    return ret_val;
}

int32_t DrvResMgrRelease(DrvResMgrHandler *handler) {
    uint32_t owner = SGL_RESMGR_OWNER;
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int32_t ret_val = SglResMgrRelease(handler, owner);
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    return ret_val;
}

int32_t DrvResMgrReleaseGroup(DrvResMgrHandler handler[], uint8_t res_nb,
                              uint8_t *release_nb) {
    if ((handler == NULL) || (release_nb == NULL))
        return MYR_DRV_INVALID_PARAM;

    int32_t ret_val = MYR_DRV_SUCCESS;
    uint32_t owner = SGL_RESMGR_OWNER;
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    int i;
    for (i = 0; i < res_nb; i++) {
        ret_val = SglResMgrRelease(&handler[i], owner);
        if (ret_val != SGL_SUCCESS)
            break;
    }
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
    *release_nb = i;
    return ret_val;
}

int32_t DrvResMgrExecuteProtected(uint8_t level, int32_t protected_func(void *),
                                  void *arg, int32_t *res) {
    if (!protected_func || !res)
        return MYR_DRV_INVALID_PARAM;

    uint32_t mutex_id = ((level == 0) ? SGL_RESMGR_SAFE_EXEC_L0_MUTEX_ID
                                      : SGL_RESMGR_SAFE_EXEC_L1_MUTEX_ID);
    DrvMutexLock(mutex_id);
    *res = protected_func(arg);
    DrvMutexUnlock(mutex_id);
    return MYR_DRV_SUCCESS;
}

int32_t DrvResMgrCpuLock(uint8_t sys_wide, DrvResMgrHandler *handler) {
    if (handler == NULL)
        return MYR_DRV_INVALID_PARAM;

    uint32_t mutex_id = (sys_wide ? SGL_RESMGR_CPU_LOCK_SYS_MUTEX_ID
                                  : SGL_RESMGR_CPU_LOCK_LEON_MUTEX_ID);
    handler->type = SGL_RESMGR_MUTEX;
    handler->id = mutex_id;
    DrvMutexLock(mutex_id);
    return MYR_DRV_SUCCESS;
}

int32_t DrvResMgrCpuUnlock(DrvResMgrHandler *handler) {
    if ((handler == NULL)
        || (handler->type != SGL_RESMGR_MUTEX)
        || ((handler->id != SGL_RESMGR_CPU_LOCK_SYS_MUTEX_ID)
            && (handler->id != SGL_RESMGR_CPU_LOCK_LEON_MUTEX_ID)))
        return MYR_DRV_INVALID_PARAM;

    DrvMutexUnlock(handler->id);
    return MYR_DRV_SUCCESS;
}

void DrvResMgrInit(void) {
    DrvMutexLock(SGL_RESMGR_OWN_MUTEX_ID);
    SglResMgrInit();
    DrvMutexUnlock(SGL_RESMGR_OWN_MUTEX_ID);
}

///@}
