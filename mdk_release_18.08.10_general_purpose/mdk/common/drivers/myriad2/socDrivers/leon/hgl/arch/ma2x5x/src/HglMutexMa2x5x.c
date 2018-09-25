///
/// @file HglMutexMa2x5x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglMutexMa2x5x
/// @{
/// @brief     HGL C function implementation for accessing the hardware 
///
///            mutexes of the Myriad platform for MA2x5x architectures.
///


#include "HglMutexMa2x5x.h"

u32 HglMutexIsGranted( u32 statusAddr, u32 grantedFlags)
{
    if ((GET_REG_WORD_VAL(statusAddr) & MTX_STAT_MASK) == (MTX_STAT_IN_USE | grantedFlags))
        return HGLMUTEX_SUCCESS;
    else
        return HGLMUTEX_BUSY;
}

void HglMutexWait(u32 status_base, u32 get_base, u32 mutex, u32 granted_flag)
{
    UNUSED(get_base); // hush the compiler warning.
    UNUSED(mutex); // hush the compiler warning.

    while ((GET_REG_WORD_VAL(status_base) & MTX_STAT_MASK) != (MTX_STAT_IN_USE | granted_flag));
}

///@}
