///
/// @file HglMutexDefines.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglMutex
/// @{
/// @brief     Defines to be used by the functionality implementation of the hardware
///
///            mutexes of the Myriad platform.
///


#ifndef HGL_MUTEX_DEFS_H
#define HGL_MUTEX_DEFS_H

#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <mv_types.h>
#include <DrvIcb.h>

#define HGLMUTEX_SUCCESS        ( 0)
#define HGLMUTEX_ERROR          (-1)
#define HGLMUTEX_OWNED          (-2)
#define HGLMUTEX_BUSY           (-3)

// Define specific mutex mask and bits
#define MTX_NUM_MASK            0x001F
#define MTX_STAT_MASK           0x01FF

#define MTX_NO_ACCES            0x0000
#define MTX_REQ_ON_RETRY        0x0100
#define MTX_RELEASE             0x0200
#define MTX_REQ_OFF_RETRY       0x0300
#define MTX_CLEAR_OWN_PENDING   0x0400
#define MTX_CLEAR_ALL_PENDING   0x0500

// status bits meaning
#define MTX_STAT_IN_USE         0x100
#define MTX_GRANTED_TO_LOS      CFG_NUM_SHAVES
#define MTX_GRANTED_TO_LRT      (CFG_NUM_SHAVES + 1)

#endif

///@}
