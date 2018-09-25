///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup LeonL1CacheApi Leon L1 Cache 
/// @ingroup  LeonL1CacheApi
/// @{
/// @brief     Utilities to work with Leon L1 Cache
///
/// Used for viewing and modifying Leon L1 Cache
///

#ifndef _LEON_L1CACHE_API_H
#define _LEON_L1CACHE_API_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "LeonL1CacheDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

///  @brief Initialise diagnostic access to the cache
/// @return void
///
void LeonL1CacheInitDiagAccess(void);


// Query Processor to report on size and configuration of target L1Cache
// @param[in] targetCache - (L1_ICACHE, L1_DCACHE)
// @param[in] cacheConfig - Pointer to cache config structure for storage of results
//void LeonL1CacheGetConfig(tyCacheType targetCache, tyCacheConfig * cacheConfig);

// Print the contents of a cacheConfig Structure
// @param[in] cacheConfig - Poitner to cache config structure for storage of results
//void LeonL1CacheDisplayConfig(tyCacheConfig * cacheConfig);

/// @{
/// @brief Raw access to Tag and Data Memories
/// @param[in] cache  - cache type(Data/Instructions)
/// @param[in] offset - the relative offset where the acces is requested
///
u32 LeonL1CacheReadCacheTagMem(tyCacheType cache,u32 offset);
u32 LeonL1CacheReadCacheDataMem(tyCacheType cache,u32 offset);
/// @}

/// @brief Display L1 cache contents
/// @param[in] cache - cache type(Data/Instructions)
/// @return void
///
void LeonL1CacheDiagDisplay(tyCacheType cache);

/// @brief Count the valid L1 cache lines
/// @param[in] cache - cache type(Data/Instructions)
/// @return number of valid lines
///
u32 LeonL1CacheDiagCountValidLines(tyCacheType cache);

/// @brief Display L1 cache general information
/// @param[in] cache - cache type(Data/Instructions)
///
void LeonL1CacheDisplayInfo(tyCacheType cache);

/// @{
/// @brief Read the L1 cache tag for a specific memory address
/// @param[in] address - the requested memory address
/// @param[in] way     - the cache way on which the request is made
/// @return The cache tag value
///
u32 LeonL1DCacheReadTagForAddr(u32 address, u32 way);//L1 DCache Tags
u32 LeonL1ICacheReadTagForAddr(u32 address, u32 way);//L1 ICache Tags
/// @}

/// @{
/// @brief Read the L1 cache content for a specific memory address
/// @param[in] address - the requested memory address
/// @param[in] way     - the cache way on which the request is made
/// @return The cached memory value
///
u32 LeonL1DCacheReadDataWordForAddr(u32 address, u32 way); //L1 DCache Tags
u32 LeonL1ICacheReadDataWordForAddr(u32 address, u32 way);//L1 ICache Data
/// @}

///@{
/// @brief Check whether a specific memory address is cached either in data or instructions cache
/// @param[in] address - address to be checked
/// @return 
///				- 1 - success; 
///				- 0 - failure
///
u32 LeonL1CacheIsAddressDCached(u32 address);
u32 LeonL1CacheIsAddressICached(u32 address);
/// @}

/// @}
#ifdef __cplusplus
}
#endif


#endif // _LEON_L1CACHE_API_H
