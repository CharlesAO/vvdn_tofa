///
/// @file HglMutexMa2x5x.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup HglMutexMa2x5x HglMutexMa2x5x Driver
/// @{
/// @brief     HGL function API for accessing the hardware 
///
///            mutexes of the Myriad platform for MA2x5x architectures.
///


#ifndef HGL_MUTEX_MA2X5X_H
#define HGL_MUTEX_MA2X5X_H

#include "HglMutexDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Wait for the requested mutex
///
/// Wait until a specified mutex is taken, otherwise generate an infinite loop
///
/// @param[in] status_base  - Status register address
/// @param[in] get_base     - Base address for the specific processor
/// @param[in] mutex        - mutex number, [0,31]
/// @param[in] granted_flag - flag for the specific processor
/// @return void
///
void HglMutexWait(u32 status_base, u32 get_base, u32 mutex, u32 granted_flag);

/// @brief Wait for the requested mutex
///
/// Wait until a specified mutex is taken by current units assuming that
/// prevision was put a lock request, otherwise will generate an infinite loop
///
/// @param[in] status_base  - Status register address
/// @param[in] granted_flag - flag for the specific processor
/// @return 
///             - 1 - if mutex was taken or is already taken by LOS/LRT
///             - 0 - if mutex is not available
///
u32 HglMutexIsGranted( u32 statusAddr, u32 grantedFlags);

#ifdef __cplusplus
}
#endif

#endif

///@}
