///
/// @file DrvShaveL2CDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvShaveL2C
/// @{
/// @brief     Shave L2 cache functionality
///
/// Driver for Myriad2 Shave L2 cache management.
///


#ifndef _DRV_SHAVE_L2C_DEFINES_H_
#define _DRV_SHAVE_L2C_DEFINES_H_

// 1: Defines

/// Maximum number of Shave L2 cache partitions supported
#define MAX_SHAVE_L2C_PARTITIONS 8

// ----------------------------------------------------------------------------
/// In this mode the L2Cache acts as a 128KB SRAM at address 0x40000000
/// This mode is typically used for unstacked Myriad parts that do not have DDR
#define SHAVE_L2CACHE_DIRECT_MODE                           (0x7)
/// In this mode the L2Cache acts as a cache only for the 0x80000000-0xbfffffff address space of DRAM
/// If in doubt the cache should typically be configured as L2CACHE_NORMAL_MODE
#define SHAVE_L2CACHE_NORMAL_MODE                           (0x16)
/// In this mode the L2Cache is bypassed completely
#define SHAVE_L2CACHE_BYPASS_MODE                           (0xE)
/// In this mode the L2Cache acts as a cache for the full DRAM address space.
#define SHAVE_L2CACHE_ALL_DDR_ADDRESS_SPACE_CACHED_MODE     (0x6)

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
/// Supported L2 cache partition sizes (in multiples of 16kB)
/// Note: partition alignment calculation requires these values to be power of 2
typedef enum
{
    SHAVEPART256KB = 16,
    SHAVEPART128KB = 8,
    SHAVEPART64KB  = 4,
    SHAVEPART32KB  = 2,
    SHAVEPART16KB  = 1
} drvShaveL2CachePartitionSizes_enum_t;

/// Bit offsets of L2 cache Partition ID fields for WIN_n in WIN_CPC register
typedef enum
{
    SHAVEL2CACHEWIN_A = 0,
    SHAVEL2CACHEWIN_B = 5,
    SHAVEL2CACHEWIN_C = 10,
    SHAVEL2CACHEWIN_D = 15
} drvShaveL2CacheWindowType_t;

/// Supported L2 cache modes (L2C_MODE register)
typedef enum
{
    SHAVEL2C_MODE_DIRECT     = SHAVE_L2CACHE_DIRECT_MODE,
    SHAVEL2C_MODE_NORMAL     = SHAVE_L2CACHE_NORMAL_MODE,
    SHAVEL2C_MODE_BYPASS     = SHAVE_L2CACHE_BYPASS_MODE,
    SHAVEL2C_MODE_CACHED_ALL = SHAVE_L2CACHE_ALL_DDR_ADDRESS_SPACE_CACHED_MODE
} drvShaveL2CacheModes_t;

#endif //_DRV_SHAVE_L2C_DEFINES_H_

///@}
