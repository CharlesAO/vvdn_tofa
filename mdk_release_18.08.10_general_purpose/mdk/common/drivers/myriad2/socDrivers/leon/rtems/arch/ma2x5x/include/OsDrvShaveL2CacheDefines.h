///
/// @file OsDrvShaveL2CacheDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvShaveL2Cache
/// @{
/// @brief     Definitions and types needed by shave low level functionality
///



#ifndef _OS_DRV_SHAVE_L2CACHE_DEFINES_H
#define _OS_DRV_SHAVE_L2CCAHE_DEFINES_H

//#endif // OS_DRV_SHAVE_L2CACHE_DEFINES_H

// System Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "OsCommon.h"
#include "DrvShaveL2CDefines.h"

// Application Includes
// ----------------------------------------------------------------------------

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#ifdef __RTEMS__
    #ifdef POWER_AWARE
        #undef POWER_AWARE
    #endif
#endif

#define OS_DRV_SHAVE_L2CACHE_WAIT_FOREVER              (0xFFFFFFFF)


// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef drvShaveL2CacheWindowType_t           osDrvShaveL2CacheWindowType_t;
typedef drvShaveL2CachePartitionSizes_enum_t  osDrvShaveL2CachePartitionSizes_enum_t;
typedef drvShaveL2CacheModes_t                osDrvShaveL2CacheModes_t;

typedef enum
{
    NON_WINDOWED_DATA_PARTITION = 0,
    NON_WINDOWED_INSTRUCTIONS_PARTITION
} osDrvShaveL2CachePartitionType_t;

typedef enum
{
    DO_NOT_PERFORM_INVALIDATION = 0,
    PERFORM_INVALIDATION,
} osDrvShaveL2CacheInvalidate_t;

typedef enum
{
    SHAVE0 = 0,
    SHAVE1,
    SHAVE2,
    SHAVE3,
    SHAVE4,
    SHAVE5,
    SHAVE6,
    SHAVE7,
    SHAVE8,
    SHAVE9,
    SHAVE10,
    SHAVE11,
    SHAVE12,
    SHAVE13,
    SHAVE14,
    SHAVE15
} shaveId_t;

#endif

///@}
