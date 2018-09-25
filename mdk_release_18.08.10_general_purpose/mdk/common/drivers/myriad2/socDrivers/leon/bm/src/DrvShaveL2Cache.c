///
/// @file DrvShaveL2Cache.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvShaveL2Cache
/// @{
/// @brief     Shave L2 cache functionality
///
/// Driver for Myriad2 Shave L2 cache management.
///


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include <DrvShaveL2Cache.h>
#include <assert.h>
#include <DrvSvu.h>
#include <DrvMutex.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
/// Flush and/or Invalidate operations on a certain L2 partition (L2C_FLUSHINV register)
#define SHAVE_L2_OP_FLUSH_INVLD (1 << 0)
#define SHAVE_L2_OP_INVLD       (1 << 1)
#define SHAVE_L2_OP_FLUSH       (1 << 2)
#define SHAVE_L2_OP_MAX         SHAVE_L2_OP_FLUSH


//TODO: request free mutex from ResMgr
#ifndef SHAVE_L2C_HW_MUTEX_USED
#define SHAVE_L2C_HW_MUTEX_USED 28
#endif

/// Structure to be initialized and used to generate each buffer address for each user
typedef struct partitions_users
{
    /// Number of partitions asked so far
    u32 partitions_asked;
    /// Size for each of the asked partitions
    drvShaveL2CachePartitionSizes_enum_t size[MAX_SHAVE_L2C_PARTITIONS];
} partitions_cfg_t;

/// Shave L2 cache partition size codes (L2C_PTRANSn registers)
typedef enum
{
    SHAVEPART256KBCODE  = 0x0,
    SHAVEPART128KBCODE  = 0x4,
    SHAVEPART64KBCODE   = 0x1,
    SHAVEPART32KBCODE   = 0x2,
    SHAVEPART16KBCODE   = 0x3
} drvShaveL2CachePartitionSizeCodes_enum_t;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
/// Variable to hold the L2C Partition configuration information
static partitions_cfg_t partitions_configuration =
{
    0,
    {
        SHAVEPART16KB, SHAVEPART16KB, SHAVEPART16KB, SHAVEPART16KB,
        SHAVEPART16KB, SHAVEPART16KB, SHAVEPART16KB, SHAVEPART16KB
    }
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static void PartitionUtil(u32 partNo, u32 operation);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

u32 DrvShaveL2CacheSetMode(drvShaveL2CacheModes_t l2cMode)
{
    u32 status = 0;

    switch(l2cMode)
    {
        case SHAVEL2C_MODE_DIRECT:
        case SHAVEL2C_MODE_NORMAL:
        case SHAVEL2C_MODE_BYPASS:
        case SHAVEL2C_MODE_CACHED_ALL:
            SET_REG_WORD(L2C_MODE_ADR, l2cMode);
            break;
        default:
            status = 1;
            // Set a valid mode for L2 cache
            assert(0);
            break;
    }

    return status;
}

void DrvShaveL2CacheSetLSUPartId(u32 shaveNum, u32 partNum)
{
    u32 regVal;

    assert(shaveNum < 12);
    assert(partNum < MAX_SHAVE_L2C_PARTITIONS);

    /// Get the Non-windowed Cache Policy register for the requested Shave
    /// Mask the L2 cache Partition ID bit-field {0, 3} for LSU port
    regVal = GET_REG_WORD_VAL(SLICE_NWN_CPC(shaveNum)) & (~0xF);
    /// Set the new Partition ID and write the register back
    SET_REG_WORD(SLICE_NWN_CPC(shaveNum), regVal | (partNum & 0xF));
}

void DrvShaveL2CacheSetInstrPartId(u32 shaveNum, u32 partNum)
{
    u32 regVal;

    assert(shaveNum < 12);
    assert(partNum < MAX_SHAVE_L2C_PARTITIONS);

    /// Get the Non-windowed Cache Policy register for the requested Shave
    /// Mask the L2 cache Partition ID bit-field {12, 15} for Instruction port
    regVal = GET_REG_WORD_VAL(SLICE_NWN_CPC(shaveNum)) & ~(0xF << 12);
    /// Set the new Partition ID and write the register back
    SET_REG_WORD(SLICE_NWN_CPC(shaveNum), regVal | ((partNum & 0xF) << 12));
}

void DrvShaveL2CacheSetWinPartId(u32 shaveNum, drvShaveL2CacheWindowType_t win, u32 partNum)
{
    u32 partitionWord;

    assert(shaveNum < 12);
    assert(partNum < MAX_SHAVE_L2C_PARTITIONS);

    /// Get the Window Cache Policy register for the requested Shave
    partitionWord = GET_REG_WORD_VAL(SLICE_WIN_CPC(shaveNum));
    /// Associate the requested window
    partitionWord = (partitionWord & ~(0xF << win)) | ((partNum & 0xF) << win);
    SET_REG_WORD(SLICE_WIN_CPC(shaveNum), partitionWord);
}

/// Perform selected flush, invalidate or flush and invalidate operation
/// on selected L2 Cache partition (L2C_FLUSHINV register)
/// !Note: this uses HW mutex 28. Configurable through redefining #define SHAVE_L2C_HW_MUTEX_USED
/// @param[in] - partNum -> L2Cache partition ID to be flushed/invalidated
/// @return    - void
///
static void PartitionUtil(u32 partNo, u32 operation)
{
    assert(partNo < MAX_SHAVE_L2C_PARTITIONS);
    assert(operation <= SHAVE_L2_OP_MAX);

    DrvMutexLock(SHAVE_L2C_HW_MUTEX_USED);

    /// Request to stop L2 cache (allows ongoing transactions to complete first)
    SET_REG_WORD(L2C_STOP_ADR, 0x1);
    /// Wait for pending transaction or outstanding DDR read
    while ((GET_REG_WORD_VAL(L2C_BUSY_ADR) & 0x1));
    /// Flush & | Invalidate partition
    SET_REG_WORD(L2C_FLUSHINV_ADR, (partNo << 3) | operation);
    /// Wait for Flush & | Invalidate DONE
    while ((GET_REG_WORD_VAL(L2C_BUSY_ADR) & 0x2));
    /// Enable L2 cache back
    SET_REG_WORD(L2C_STOP_ADR, 0x0);

    DrvMutexUnlock(SHAVE_L2C_HW_MUTEX_USED);
}

void DrvShaveL2CachePartitionFlush(u32 partNo)
{
    PartitionUtil(partNo, SHAVE_L2_OP_FLUSH);
}

void DrvShaveL2CachePartitionInvalidate(u32 partNo)
{
    PartitionUtil(partNo, SHAVE_L2_OP_INVLD);
}

void DrvShaveL2CachePartitionFlushAndInvalidate(u32 partNo)
{
    PartitionUtil(partNo, SHAVE_L2_OP_FLUSH_INVLD);
}

void DrvShaveL2CacheClearPartitions(void)
{
    partitions_configuration.partitions_asked = 0;
}

int DrvShaveL2CacheSetupPartition(drvShaveL2CachePartitionSizes_enum_t partitionSize)
{
    u32 partitionsTotalSize = 0;
    u32 i;

    assert(partitionSize <= SHAVEPART256KB);
    /// Check that there are still available L2 cache partitions
    assert(partitions_configuration.partitions_asked < MAX_SHAVE_L2C_PARTITIONS);

    /// Check that there is still available L2 cache space (total 256kB)
    /// TODO Partition start address alignment (multiple of partition size)
    for (i = 0; i < partitions_configuration.partitions_asked; i++)
    {
        partitionsTotalSize += partitions_configuration.size[i];
        assert(partitionsTotalSize <= SHAVEPART256KB);
    }

    assert(partitionsTotalSize + partitionSize <= SHAVEPART256KB);
    partitions_configuration.size[partitions_configuration.partitions_asked] = partitionSize;

    return (partitions_configuration.partitions_asked++);
}

u32 DrvShaveL2CacheAllocateSetPartitions(void)
{
    u32 i;
    u32 __attribute__((unused)) result = 0; // avoid 'unused variable' error from scan build
    s32 size16KBsectionsLeft;
    u32 curPartitionStartPos;

    /// Check the L2 cache partition number in the configuration
    assert(partitions_configuration.partitions_asked <= MAX_SHAVE_L2C_PARTITIONS);

    /// We have 16 x 16KB sections assignable as partitions
    size16KBsectionsLeft = 16;
    /// We need to keep track of the partition start address inside L2 cache
    /// (in multiples of 16kB)
    curPartitionStartPos = 0x0;

    for (i = 0; i < partitions_configuration.partitions_asked; i++)
    {
        u32 partitionWord = 0x0;
        u32 partitionSize = partitions_configuration.size[i];
        u32 partitionAlignPadding = 0;

        /// Build up the L2 Cache partition word requested for the current partition
        /// This is built using data from the 56_L2_cache.doc Myriad2 document
        switch (partitionSize)
        {
            case SHAVEPART256KB:
                partitionWord = SHAVEPART256KBCODE;
                break;
            case SHAVEPART128KB:
                partitionWord = SHAVEPART128KBCODE;
                break;
            case SHAVEPART64KB:
                partitionWord = SHAVEPART64KBCODE;
                break;
            case SHAVEPART32KB:
                partitionWord = SHAVEPART32KBCODE;
                break;
            case SHAVEPART16KB:
                partitionWord = SHAVEPART16KBCODE;
                break;
            default:
                result = 1;
              // Invalid Shave L2 cache partition size
                assert(0);
                break;
        }

        /// Provide Partition start address alignment (multiple of partition size)
        /// alignment_padding = aligned_start_position - unaligned_one
        partitionAlignPadding =
            ((curPartitionStartPos + (partitionSize - 1)) & ~(partitionSize - 1)) -
            curPartitionStartPos;

        /// Decrease the number of sections left with the number of sections requested
        size16KBsectionsLeft -= partitionAlignPadding + partitionSize;

        /// Check that there is sufficient L2 cache space available
        if (size16KBsectionsLeft < 0)
        {
            /// Fail: we are being requested to allocate a partition
            /// but cannot do so anymore because of lack of 16 KB sections
            result = 1;
           // Failed to allocate some L2 cache partition
            assert(size16KBsectionsLeft >= 0);
            break;
        }

        /// Prepare L2C_PTRANSn register value:
        /// {7} Bypass override = 0, {6:3} Partition start, {2:0} Partition size
        partitionWord = ((curPartitionStartPos + partitionAlignPadding) << 3) | partitionWord;

        /// Fill in the partition word for the requested partition
        /// Register group L2C_PTRANS[8-15] is separated in the memory map
        if (i < 8)
            SET_REG_WORD(L2C_PTRANS0_ADR + (i * 0x4), partitionWord);
        else if (i < 16)
            SET_REG_WORD(L2C_PTRANS8_ADR + ((i - 8) * 0x4), partitionWord);

        /// Advance the start position for the next partition
        curPartitionStartPos += partitionAlignPadding + partitionSize;

        /// Invalidate the allocated L2 cache partition
        DrvShaveL2CachePartitionInvalidate(i);
    }

    return result;
}

#pragma GCC diagnostic pop

///@}

