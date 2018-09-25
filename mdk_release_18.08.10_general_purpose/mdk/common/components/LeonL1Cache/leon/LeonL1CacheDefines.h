///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved 
///            For License Warranty see: common/license.txt              
///
/// @defgroup LeonL1CacheDefines Leon L1 Cache Defines
/// @ingroup  LeonL1Cache
/// @{
/// @brief    Definitions and types needed by Leon L1Cache Component
/// 
/// This file contains all the definitions of constants, typedefs,
/// structures, enums and exported variables for the Leon L1 Cache component
/// 

#ifndef _LEON_L1CACHE_DEFINES_H
#define _LEON_L1CACHE_DEFINES_H 

#include "mv_types.h"
// 1: Defines
// ----------------------------------------------------------------------------
// Notes on different Leon L1 Cache Configurations for Myriad 
// ------------------------
// ------------------------
// Myriad 1  Leon3 L1 Cache
// ------------------------
// L1 ICache both have the same configuration
// Bits 01..00   (byte offset within word      2^2 ( 4 bytes per word)
// Bits 04..02   (word index within cache line 2^3 ( 8 words per line (32 bytes))
// Bits 11..05   (line index within cache way  2^7 ( 128 lines per way (4096 bytes per way))
// Bits 31..12   (Portion of address in tag memory (only one way)) [ 20 bit tag ]
// numWays = 1   (i.e. 4KB direct mapped write-through ICACHE and DCACHE with 32 bytes per line)
// Additional points:
// Individual valid bits per word
// Dcache has allocate on read miss with only 4 bytes being loaded into the cache line
// Dcache does not allocate on write miss
// ------------------------
// ------------------------
// Myriad 2  Leon4 OS L1 Cache
// ------------------------
// Bits 01..00   (byte offset within word      2^2 ( 4 bytes per word)
// Bits 04..02   (word index within cache line 2^3 ( 8 words per line (32 bytes))
// Bits 13..05   (line index within cache way  2^9 ( 512 lines per way (16KB bytes per way))
// Bits 31..14   (Portion of address in tag memory, Note: 2 ways so two different tags can be present)
// numWays = 2   (i.e. 32KB 2 way set associative write-through ICACHE and DCACHE with 32 bytes per line)
// ------------------------
// Myriad 2  Leon4 RT L1 Cache
// ------------------------
// Bits 01..00   (byte offset within word      2^2 ( 4 bytes per word)
// Bits 04..02   (word index within cache line 2^3 ( 8 words per line (32 bytes))
// Bits 10..05   (line index within cache way  2^6 ( 64 lines per way (2KB bytes per way))
// Bits 31..11   (Portion of address in tag memory, Note: 2 ways so two different tags can be present)
// numWays = 2   (i.e. 4KB 2 way set associative write-through ICACHE and DCACHE with 32 bytes per line)
// Additional points:
// Leon4 only has 1 valid bit per line (however diagnostic tag access replicates this bit x 8 for compatibility)
// Dcache has allocate on read miss with only 4 bytes being loaded into the cache line
// Dcache does not allocate on write miss

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

typedef enum
{
    L1_DCACHE,
    L1_ICACHE
} tyCacheType;

typedef enum 
{
    NONE = 0,   // Direct mapped cache
    LRU  = 1,   // Least Recently Used
    RAND = 3,   // Random
} tyReplicationPolicy;

typedef enum 
{
    WRITE_THROUGH = 0, 
    WRITE_BACK    = 1,  
} tyWritePolicy;

// This struction contains basic info about the cache as used by this module
typedef struct
{
    u32                 waySizeBits; 
    u32                 lineSizeBits;
    u32                 numWays;            // 1 means direct mapped, 2 (2 way set associative)
    u32                 tagOffsetMask;      // Mask address to index a tag during diag access
    u32                 dataOffsetMask;     // Mask data to index a dataword during diag access
    u32                 totalSizeBytes;     // Sum of all the ways
    u32                 waySizeBytes;
    u32                 lineSizeBytes;
    u32                 numLinesPerWay;
    u32                 mmuSupported;
    u32                 snoopSupported;
    u32                 lockingSupported;
    tyReplicationPolicy replicationPolicy;
    tyWritePolicy       writePolicy;
} tyCacheInfo;


// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

/// @}
#endif // _LEON_L1CACHE_DEFINES_H

