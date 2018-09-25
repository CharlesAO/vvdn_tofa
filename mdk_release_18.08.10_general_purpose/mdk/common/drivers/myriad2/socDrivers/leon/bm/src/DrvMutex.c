///
/// @file DrvMutex.c
/// 
/// 
/// @ingroup DrvMutex
/// @{
///

//
// 
// 
// 
//
// @brief     c function implementation for accessing the hardware
//            mutexes of the Myriad platform.
//
#include "DrvMutex.h"
#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include "swcWhoAmI.h"
#include <assert.h>
#include <DrvIcbDefines.h>

#define DRV_MUTEX_RETURN(ret) if(!ret) return 1;\
                              else return 0;

u32 DrvMutexLock(u32 mutex)
{
    u8 ret = HglMutexObtain(mutex, swcWhoAmI());
    DRV_MUTEX_RETURN(ret);
}

u32 DrvMutexTryLock(u32 mutex)
{
    u8 ret = HglMutexTryObtain(mutex, swcWhoAmI());
    DRV_MUTEX_RETURN(ret);
}

u32 DrvMutexLockRequest(u32 mutex)
{
    u8 ret =  HglMutexSetRequest(mutex, swcWhoAmI());
    DRV_MUTEX_RETURN(ret);
}

void DrvMutexLockWait(u32 mutex)
{
    HglMutexWaitObtain(mutex, swcWhoAmI());
}

void DrvMutexUnlock(u32 mutex)
{
    HglMutexRelease(mutex, swcWhoAmI());
}

void DrvMutexClearOwnPendingRequest(u32 mutex)
{
    HglMutexClearRequest(mutex, swcWhoAmI(), 0);
}

void DrvMutexClearAllPendingRequest(u32 mutex)
{
    HglMutexClearRequest(mutex, swcWhoAmI(), 1);
}

u32 DrvMutexGetStatus(u32 mutex)
{
    return HglMutexGetStatus(mutex);
}

u32 DrvMutexGetAllStatus(void)
{
    return HglMutexGetStatusAll();
}

void DrvMutexClearInterupts(u32 intMaskToClear)
{
    HglMutexClearIrq(intMaskToClear, swcWhoAmI());
}

void DrvMutexEnableInterupts(u32 intEnableMask)
{
    HglMutexEnableIrq(intEnableMask, 1, swcWhoAmI());
}

u32 DrvMutexGetInteruptsStatus(void)
{
    return HglMutexGetIrqStatus(swcWhoAmI());
}

void DrvMutexSetInterrupts(irq_handler mutexHandler, int intMask)
{
    //Clear all interrupts so we can have the next interrupt happening
    DrvMutexClearInterupts(intMask);
    //Disable ICB (Interrupt Control Block) while setting new interrupt
    DrvIcbDisableIrq(IRQ_CMX);
    DrvIcbIrqClear(IRQ_CMX);
    //Configure interrupt handlers
    DrvIcbSetIrqHandler(IRQ_CMX, mutexHandler);
    //Enable interrupts on SHAVE done
    DrvIcbConfigureIrq(IRQ_CMX, 3, POS_EDGE_INT);
    DrvIcbEnableIrq(IRQ_CMX);

    //Enable
    DrvMutexEnableInterupts(intMask);

    //Can enable the interrupt now
    swcLeonEnableTraps();
}

///@}
