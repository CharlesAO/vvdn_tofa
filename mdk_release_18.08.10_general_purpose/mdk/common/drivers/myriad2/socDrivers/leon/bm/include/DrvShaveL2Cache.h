///
/// @file DrvShaveL2Cache.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvShaveL2Cache ShaveL2Cache Driver
/// @{
/// @brief     Shave L2 cache functionality
///
/// Driver for Myriad2 Shave L2 cache management.
///


#ifndef _DRV_SHAVE_L2_CACHE_H_
#define _DRV_SHAVE_L2_CACHE_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvCommon.h"
#include "DrvShaveL2CDefines.h"
#include "mv_types.h"

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
/// Deprecate __l2_config variable used in applications to set L2 cache mode
/// (in section ".l2.mode"). That was needed by moviDebug & moviConvert for Myriad1
/// and is obsolete now. Use the new API DrvShaveL2CacheSetMode() instead.
extern u32 __l2_config __Deprecated__("Use DrvShaveL2CacheSetMode(drvShaveL2CacheModes_t l2cMode) instead");

#ifdef __cplusplus
extern "C" {
#endif

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Set the desired Shave L2 cache mode
/// @param[in] - l2cMode -> Requested Shave L2 cache mode
///              Supported modes are Normal (addresses > 0xC0000000 not cached),
///              Direct (L2C mem as SRAM), Bypass (L2C bypassed), All Cached
///              (whole address space is cached)
/// @return    - 1/0 - ERROR/SUCCESS
///
u32 DrvShaveL2CacheSetMode(drvShaveL2CacheModes_t l2cMode);

/// Shave Non-windowed Cache policy: set LSU port L2 cache partition ID (NW_CPC reg)
/// @param[in] - shaveNum -> Shave to allocate partition to
///              Valid shaveNum values are [0x0, 0xB] ([SVU_0, SVU_11]).
/// @param[in] - partNum -> L2 cache partition ID to allocate to the said Shave LSU-s
/// @return    - void
///
void DrvShaveL2CacheSetLSUPartId(u32 shaveNum, u32 partNum);

/// Shave Non-windowed Cache policy: set Instruction port L2 cache partition ID (NW_CPC reg)
/// @param[in] - shaveNum -> Shave to allocate partition to
///              Valid shaveNum values are [0x0, 0xB] ([SVU_0, SVU_11]).
/// @param[in] - partNum -> L2 cache partition ID to allocate to the said Shave Instruction port
/// @return    - void
///
void DrvShaveL2CacheSetInstrPartId(u32 shaveNum, u32 partNum);

/// Shave Windowed Cache policy: set Shave windows L2 cache partition ID (WIN_CPC reg)
/// @param[in] - shaveNum -> Shave to allocate partition to
///              Valid shaveNum values are [0x0, 0xB] ([SVU_0, SVU_11]).
/// @param[in] - win -> Window to set partition ID to ([WIN_A, WIN_D])
/// @param[in] - partNum -> L2 cache partition ID to allocate to the said Shave
/// @return    - void
///
void DrvShaveL2CacheSetWinPartId(u32 shaveNum, drvShaveL2CacheWindowType_t win, u32 partNum);

/// Perform flush operation on selected L2 cache partition
/// !Note: this uses HW mutex 28. Configurable through redefining #define SHAVE_L2C_HW_MUTEX_USED
/// @param[in] - partNum -> L2 cache partition ID to be flushed
/// @return    - void
///
void DrvShaveL2CachePartitionFlush(u32 partNo);

/// Perform invalidate operation on selected L2 cache partition
/// !Note: this uses HW mutex 28. Configurable through redefining #define SHAVE_L2C_HW_MUTEX_USED
/// @param[in] - partNum -> L2 cache partition ID to be invalidated
/// @return    - void
///
void DrvShaveL2CachePartitionInvalidate(u32 partNo);

/// Perform flush and invalidate operation on selected L2 cache partition
/// !Note: this uses HW mutex 28. Configurable through redefining #define SHAVE_L2C_HW_MUTEX_USED
/// @param[in] - partNum -> L2 cache partition ID to be flushed and invalidated
/// @return    - void
///
void DrvShaveL2CachePartitionFlushAndInvalidate(u32 partNo);

/// Reset the L2 cache partition configuration internal structure
/// !Note: this uses HW mutex 28. Configurable through redefining #define SHAVE_L2C_HW_MUTEX_USED
/// (The L2 cache configuration registers are left unmodified.)
/// @return    - void
///
void DrvShaveL2CacheClearPartitions(void);

/// Set up a new partition (in internal configuration structure)
/// Make sure you declare your partitions in descending order, so to avoid padding
/// For example if you declare a partition of 16kB followed by one of 32kB, owing to
/// partition alignment, a 16kB padding will be added; avoid by declaring in descending order
/// @param[in] - partitionSize -> Partition size requested for this particular partition
/// @return    - id of the last declared partition
///
int DrvShaveL2CacheSetupPartition(drvShaveL2CachePartitionSizes_enum_t partitionSize);

/// Allocate all requested L2 cache partitions
/// !Note: this uses HW mutex 28. Configurable through redefining #define SHAVE_L2C_HW_MUTEX_USED
/// @return    - 1/0 - ERROR/SUCCESS
///
u32 DrvShaveL2CacheAllocateSetPartitions(void);

#ifdef __cplusplus
}
#endif

#endif //_DRV_SHAVE_L2_CACHE_H_

///@}
