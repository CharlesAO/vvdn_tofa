///
/// @file HglMutex.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglMutex
/// @{
/// @brief     c function implementation for accessing the hardware
///
///            mutexes of the Myriad platform.
///


#include "HglMutex.h"

// Utility get parameters used very frequently
static inline u32 hglMutexGetReqLockParams(swcProcessorType proc, u32 *cmxMtxGranted)
{
    u32 cmxMtxGet;

    if (proc == PROCESS_LEON_OS)
    {
        cmxMtxGet = CMX_MTX_LOS_GET;
        *cmxMtxGranted = MTX_GRANTED_TO_LOS;
    }
    else
    {
        cmxMtxGet = CMX_MTX_LRT_GET;
        *cmxMtxGranted = MTX_GRANTED_TO_LRT;
    }
    return cmxMtxGet;
}

s32 HglMutexSetRequest(u8 mutex,swcProcessorType proc)
{
    u32 cmxMtxGet, cmxMtxGranted;
    // Get Params
    cmxMtxGet = hglMutexGetReqLockParams(proc, &cmxMtxGranted);
    // If Mutex is already owned ...
    if (((GET_REG_WORD_VAL(CMX_MTX_BASE + CMX_MTX_0 + (mutex & MTX_NUM_MASK)* 4)
                            & MTX_STAT_MASK) == (MTX_STAT_IN_USE | cmxMtxGranted)))
        return HGLMUTEX_OWNED;
    else
    {
        // Otherwise request properly placed
        SET_REG_HALF(CMX_MTX_BASE + cmxMtxGet, (mutex & MTX_NUM_MASK) |
                    MTX_REQ_ON_RETRY);
        return HGLMUTEX_SUCCESS;
    }
}

s32 HglMutexObtain(u8 mutex, swcProcessorType proc)
{
    s32 sRet = HglMutexSetRequest(mutex, proc);
    if (sRet == HGLMUTEX_SUCCESS)
        HglMutexWaitObtain(mutex, proc);
    return sRet;
}

s32 HglMutexTryObtain(u8 mutex, swcProcessorType proc)
{
    u32 cmxMtxGet, cmxMtxGranted, cmxMtxStatusAddr;
    // Get Params
    cmxMtxGet = hglMutexGetReqLockParams(proc, &cmxMtxGranted);

    SET_REG_HALF(CMX_MTX_BASE + cmxMtxGet, (mutex & MTX_NUM_MASK) |
                MTX_REQ_OFF_RETRY);

    cmxMtxStatusAddr = CMX_MTX_BASE + CMX_MTX_0 + (mutex & MTX_NUM_MASK) * 4;
    // Check if the Mutex was granted
    if (!HglMutexIsGranted(cmxMtxStatusAddr, cmxMtxGranted))
        return HGLMUTEX_SUCCESS;
    else
        return HGLMUTEX_BUSY;
}

void HglMutexWaitObtain(u8 mutex, swcProcessorType proc)
{
    u32 cmxMtxGet, cmxMtxGranted, cmxMtxStatus;
    // Get Params
    cmxMtxGet = hglMutexGetReqLockParams(proc, &cmxMtxGranted);
    cmxMtxStatus  = CMX_MTX_BASE + CMX_MTX_0 + (mutex & MTX_NUM_MASK) * 4;
    // Wait to adquire the Mutex
    HglMutexWait(cmxMtxStatus, cmxMtxGet, mutex, cmxMtxGranted);
}

void HglMutexClearRequest(u8 mutex ,swcProcessorType proc, u32 flush)
{
    u16 flag;
    // Either our request or any (flush > 0)
    if (flush)
        flag = MTX_CLEAR_ALL_PENDING;
    else
        flag = MTX_CLEAR_OWN_PENDING;

    if (proc == PROCESS_LEON_OS)
        SET_REG_HALF(CMX_MTX_BASE + CMX_MTX_LOS_GET, (mutex & MTX_NUM_MASK) | flag);
    else
        SET_REG_HALF(CMX_MTX_BASE + CMX_MTX_LRT_GET, (mutex & MTX_NUM_MASK) | flag);
}

void HglMutexRelease(u8 mutex, swcProcessorType proc)
{
    u32 cmxMtxGet, cmxMtxGranted;
    // Get Params
    cmxMtxGet = hglMutexGetReqLockParams(proc, &cmxMtxGranted);

    SET_REG_HALF(CMX_MTX_BASE + cmxMtxGet, (mutex & MTX_NUM_MASK) |
                MTX_RELEASE);
    // Wait for release
    while ((GET_REG_WORD_VAL(CMX_MTX_BASE + CMX_MTX_0 + (mutex & MTX_NUM_MASK)
        * 4) & MTX_STAT_MASK) == (MTX_STAT_IN_USE | cmxMtxGranted));
}

u32 HglMutexGetStatus(u8 mutex)
{
    return (GET_REG_WORD_VAL(CMX_MTX_BASE + CMX_MTX_0 +
            (mutex & MTX_NUM_MASK) * 4) & MTX_STAT_MASK);
}

u32 HglMutexGetStatusAll(void)
{
   return (GET_REG_WORD_VAL(CMX_MTX_BASE + CMX_MTX_STATUS));
}

void HglMutexClearIrq(u32 mask, swcProcessorType proc)
{
    if (proc == PROCESS_LEON_OS)
        SET_REG_WORD(CMX_MTX_BASE + CMX_MTX_LOS_INT_CLR, mask);
    else
        SET_REG_WORD(CMX_MTX_BASE + CMX_MTX_LRT_INT_CLR, mask);
}

void HglMutexEnableIrq(u32 mask, u32 enable, swcProcessorType proc)
{
    u32 base_addr;
    if (proc == PROCESS_LEON_OS)
        base_addr = CMX_MTX_BASE + CMX_MTX_LOS_INT_EN;
    else
        base_addr = CMX_MTX_BASE + CMX_MTX_LRT_INT_EN;
    SET_REG_WORD(base_addr, enable > 0 ? GET_REG_WORD_VAL(base_addr) | mask
                                        :GET_REG_WORD_VAL(base_addr) & ~mask);
}

u32 HglMutexGetIrqStatus(swcProcessorType proc)
{
    if (proc == PROCESS_LEON_OS)
        return (GET_REG_WORD_VAL(CMX_MTX_BASE + CMX_MTX_LOS_INT_STAT));
    else
        return (GET_REG_WORD_VAL(CMX_MTX_BASE + CMX_MTX_LRT_INT_STAT));
}


///@}
