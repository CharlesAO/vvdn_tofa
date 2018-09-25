///
/// @file OsDrvShaveL2Cache.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvShaveL2Cache OsShaveL2Cache Driver
/// @{
/// @brief     Shave low level functionality for RTEMS.
// --------------------------------------------------------------------------------
///


#ifndef _OS_DRV_SHAVE_L2CACHE_H
#define _OS_DRV_SHAVE_L2CACHE_H

// System Includes
// ----------------------------------------------------------------------------

// Application Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include "OsDrvShaveL2CacheDefines.h"
// Common for all OS drivers
#include <OsCommon.h>
// Bare-metal layer include
#include "DrvShaveL2Cache.h"

#ifdef __cplusplus
extern "C" {
#endif

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Global function prototypes
// ----------------------------------------------------------------------------
/// Create the shave L2C mutex and set the shave usage mode
/// @param[in] - l2cMode -the mode to set the L2C to
/// @return    - OS_MYR_DRV_SUCCESS             - if the initialization performed ok
///            - an RTEMS return code           - if an error occur while creating the mutex
///            - OS_MYR_DRV_ALREADY_INITIALIZED - if the driver was already initialized
OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CacheInit(osDrvShaveL2CacheModes_t l2cMode);

/// Reset the L2 cache partition configuration internal structure
/// (The L2 cache configuration registers are left unmodified.)
/// @return    - OS_MYR_DRV_SUCCESS
OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CResetPartitions(void);

/// Shave Non-windowed Cache policy: set the L2 cache partition ID for a shave LSU port or shave instruction port
/// @param[in] - shaveNum - Shave to allocate partition to
/// @param[in] - partNum - L2 cache partition ID to allocate to the indicated shave (values 0 - 15)
/// @param[in] - partitionType - data/instruction partition to set
/// @return    - OS_MYR_DRV_SUCCESS   - if function performed ok
///            - OS_MYR_DRV_ERROR     - if the port was not correct
///            - an RTEMS return code - if an error occur while obtaining/releasing the mutex
///            - OS_MYR_DRV_NOTOPENED - if the driver was not previously initialized
OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CSetNonWindowedPartition(shaveId_t shaveNum, u32 partNum, osDrvShaveL2CachePartitionType_t partitionType);

/// Shave Windowed Cache policy: set the L2 cache partitions for every shave window
/// @param[in] - shaveNum - Shave to allocate partition to
/// @param[in] - win - Window to set partition ID to ([WIN_A, WIN_D])
/// @param[in] - partNum - L2 cache partition ID to allocate to the indicated shave (values 0 - 15)
/// @return    - OS_MYR_DRV_SUCCESS   - if function performed ok
///            - an RTEMS return code - if an error occur while obtaining/releasing the mutex
///            - OS_MYR_DRV_NOTOPENED - if the driver was not previously initialized
OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CSetWindowPartition(shaveId_t shaveNum, drvShaveL2CacheWindowType_t winId, u32 partNum);

/// Perform flush, then optionally invalidate the selected L2 cache partition
/// @param[in] - partNum - L2 cache partition ID to be flushed/invalidated
/// @param[in] - invalidate - perform invalidation
/// @return    - OS_MYR_DRV_SUCCESS   - if function performed ok
///            - an RTEMS return code - if an error occur while obtaining/releasing the mutex
///            - OS_MYR_DRV_NOTOPENED - if the driver was not previously initialized
OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CachePartitionFlush(u32 partNumber, osDrvShaveL2CacheInvalidate_t invalidate);

/// Perform invalidation of the selected L2 cache partition
/// @param[in] - partNum - L2 cache partition ID to be invalidated
/// @return    - OS_MYR_DRV_SUCCESS   - if function performed ok
///            - an RTEMS return code - if an error occur while obtaining/releasing the mutex
///            - OS_MYR_DRV_NOTOPENED - if the driver was not previously initialized
OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CachePartitionInvalidate(u32 partNumber);

/// Declare a new partition (in the internal configuration structure)
/// Make sure you declare your partitions in descending order, so to avoid padding
/// For example if you declare a partition of 16kB followed by one of 32kB, owing to
/// partition alignment, a 16kB padding will be added; avoid by declaring in descending order
/// @param[in]  - partitionSize - Partition size requested for this particular partition
/// @param[out] - returned id of the current partition or MAX partitions if, there are no more available partitions
/// @return     - OS_MYR_DRV_SUCCESS   - if function performed ok
///             - an RTEMS return code - if an error occur while obtaining/releasing the mutex
///             - OS_MYR_DRV_NOTOPENED - if the driver was not previously initialized
OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CGetPartition(osDrvShaveL2CachePartitionSizes_enum_t partitionSize, int* partitionId);

/// Allocate all the previously declared L2 cache partitions
/// @return     - OS_MYR_DRV_SUCCESS   - if function performed ok
///             - OS_MYR_DRV_ERROR     - if an error occur while allocating
///             - an RTEMS return code - if an error occur while obtaining/releasing the mutex
///             - OS_MYR_DRV_NOTOPENED - if the driver was not previously initialized
OS_MYRIAD_DRIVER_STATUS_CODE  OsDrvShaveL2CacheAllocateSetPartitions(void);



#ifdef __cplusplus
}
#endif
#endif

