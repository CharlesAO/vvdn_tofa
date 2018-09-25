///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved 
///            For License Warranty see: common/license.txt              
///
/// @defgroup VcsHooksApiDefines VCS Hooks API Defines
/// @ingroup  VcsHooks
/// @{
/// @brief    Definitions and types needed by the vcsHooks component
/// 
/// This file contains all the definitions of constants, typedefs,
/// structures, enums and exported variables for the VCS Hooks component
///

#ifndef _VCS_HOOKS_DEF_H
#define _VCS_HOOKS_DEF_H 

#include "mv_types.h"
#include "registersMyriad.h"
#include <DrvRegUtilsDefines.h>
#include <DrvMutex.h>

// 1: Defines
// ----------------------------------------------------------------------------
#define PLAT_SILICON                    (0)
#define PLAT_VCS_SIM                    (1)
#define PLAT_MOVI_SIM                   (2)
#define PLAT_FPGA                       (3)

#define PLATFORM_DETECT_REG_ADR         (CPR_GEN_CTRL_ADR)
#define GET_CURRENT_PLATFORM(null)      ((GET_REG_WORD_VAL(PLATFORM_DETECT_REG_ADR)>>29)&0x7)
// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

#ifndef VCSHOOKS_USE_SOFTWARE
    #ifdef VCSHOOKS_FORCE_USE_SOFTWARE
        #define VCSHOOKS_USE_SOFTWARE (1)
    #else
        #ifdef VCSHOOKS_USE_SOFTWARE_ON_SILICON_OR_FPGA
            #define VCSHOOKS_USE_SOFTWARE (IS_PLATFORM_SILICON || IS_PLATFORM_FPGA)
        #else
            #define VCSHOOKS_USE_SOFTWARE (0)
        #endif
    #endif
#endif

// This is a temporary experimental feature, which is needed in the short term
// for brinugp testing.
// It uses a hardcoded MUTEX0 which is very dangerous and hence this feature is off 
// by default, except in very specific test cases where it is needed
#ifdef USE_MUTEX0_PRINTF
#define MUTEX_LOCK(x)  DrvMutexLock(x)
#define MUTEX_UNLOCK(x)  DrvMutexUnlock(x)
#else
#define MUTEX_LOCK(x)  (void)(x)
#define MUTEX_UNLOCK(x)  (void)(x)
#endif

#ifdef MA2X9X
#define prefixed_printf(...) do { \
    if (swcWhoAmI()==PROCESS_LEON_RT0) \
    { \
        MUTEX_LOCK(0);printf("LR0:: " __VA_ARGS__);MUTEX_UNLOCK(0);  \
    } \
    else if(swcWhoAmI()==PROCESS_LEON_RT1) \
    { \
        MUTEX_LOCK(0);printf("LR1:: " __VA_ARGS__);MUTEX_UNLOCK(0);  \
    } \
    else \
    { \
        MUTEX_LOCK(0);printf("LNN:: " __VA_ARGS__);MUTEX_UNLOCK(0);  \
    } \
} while (0)
#else
#define prefixed_printf(...) do { \
    if (swcWhoAmI()==PROCESS_LEON_RT) \
    { \
        MUTEX_LOCK(0);printf("LRT:: " __VA_ARGS__);MUTEX_UNLOCK(0);  \
    } \
    else \
    { \
        MUTEX_LOCK(0);printf("LOS:: " __VA_ARGS__);MUTEX_UNLOCK(0);  \
    } \
} while (0)
#endif
/// @}
#endif // _VCS_HOOKS_DEF_H

