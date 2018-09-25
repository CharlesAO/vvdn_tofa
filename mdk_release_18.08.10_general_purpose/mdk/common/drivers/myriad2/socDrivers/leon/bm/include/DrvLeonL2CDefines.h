///
/// @file DrvLeonL2CDefines.h
/// @copyright All code copyright Movidius Srl 2014, Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvLeonL2C
/// @{
/// @brief Leon L2 Cache Driver.
///
/// This module contains the helper functions necessary to control the
/// Leon L2 Cache in the Myriad Soc
///

#ifndef DRV_LEON_L2_CACHE_DEFINES_H
#define DRV_LEON_L2_CACHE_DEFINES_H

#include "mv_types.h"

// 1: Defines
// ----------------------------------------------------------------------------

// Control register
#define LL2C_CTRL_EN (1 << 31)
/// 31: enable bit
#define LL2C_CTRL_EN_POS 31
#define LL2C_CTRL_EN_MASK 0x80000000
/// 29:28: replacement policy
#define LL2C_CTRL_REPL_POS 28
#define LL2C_CTRL_REPL_MASK 0x30000000
#define LL2C_CTRL_REPL__LRU                  (0 << 28)
#define LL2C_CTRL_REPL__PSEUDO_RANDOM        (1 << 28)
#define LL2C_CTRL_REPL__MASTER_INDEX_REPLACE (2 << 28)
#define LL2C_CTRL_REPL__MASTER_INDEX_MODULUS (3 << 28)
//27:16 = reserved
#define LL2C_CTRL_WAY_TO_REPLACE_MASK 0x0000f000
#define LL2C_CTRL_WAY_TO_REPLACE(way) (((way) & 0xf) << 12)
#define LL2C_CTRL_NR_LOCKED_WAYS_MASK 0x00000f00
#define LL2C_CTRL_NR_LOCKED_WAYS(number) (((number) & 0xf) << 8)
// 7:6 = reserved
#define LL2C_CTRL_BUS_USAGE_STATUS_MASK 0x00000008
#define LL2C_CTRL_BUS_USAGE_STATUS_POS 3
#define LL2C_CTRL_BUS_USAGE_STATUS_SHIFTING (1 << 3)
#define LL2C_CTRL_BUS_USAGE_STATUS_WRAPPING (0 << 3
#define LL2C_CTRL_HIT_RATE_STATUS_MASK 0x00000004
#define LL2C_CTRL_HIT_RATE_STATUS_POS 2
#define LL2C_CTRL_HIT_RATE_STATUS_SHIFTING (1 << 2)
#define LL2C_CTRL_HIT_RATE_STATUS_WRAPPING (0 << 2)
#define LL2C_CTRL_WRITE_POLICY_MASK 0x00000002
#define LL2C_CTRL_WRITE_POLICY_POS 1
#define LL2C_CTRL_WRITE_POLICY_WRITE_THROUGH (1 << 1)
#define LL2C_CTRL_WRITE_POLICY_COPY_BACK (0 << 1)
// the following two lines are intentionally the same as *_COPY_BACK, these are alternative names
#define LL2C_CTRL_WRITE_POLICY_WRITE_BACK (0 << 1)
#define LL2C_CTRL_WRITE_POLICY_WRITE_BEHIND (0 << 1)

// Flush Memory register
#define LL2C_FLUSHM_ADDRESS_MASK 0xFFFFFFE0
#define LL2C_FLUSHM_DISABLE_MASK 0x00000008
#define LL2C_FLUSHM_DISABLE_POS 3
#define LL2C_FLUSHM_FLUSH_MODE_MASK 0x00000007
#define LL2C_FLUSHM_FLUSH_MODE_POS 0
// If this bit is on, the operation will apply to all cache lines
#define LL2C_FLUSHM_FLUSH_MODE_OP_ON_ALL 1<<2


// Error/Status register
#define LL2C_ERR_STAT_VALID_POS 20
#define LL2C_ERR_STAT_ERRTYPE_MASK 0x07000000
#define LL2C_ERR_STAT_ERRTYPE_POS 24

// FLush set and index register
#define LL2C_FLUSHSI_TAG_MASK 0xFFFFFC00
#define LL2C_FLUSHSI_FLUSH_MODE_MASK 0x00000003
#define LL2C_FLUSHSI_FLUSH_MODE_POS 0
#define LL2C_FLUSHSI_DISABLE_MASK 0x00000008
#define LL2C_FLUSHSI_DISABLE_POS 3
#define LL2C_FLUSHSI_WAY_MASK 0x00000030
#define LL2C_FLUSHSI_WAY_POS 4
#define LL2C_FLUSHSI_DIRTY_MASK 0x00000080
#define LL2C_FLUSHSI_DIRTY_POS 7
#define LL2C_FLUSHSI_VALID_MASK 0x00000100
#define LL2C_FLUSHSI_VALID_POS 8
#define LL2C_FLUSHSI_FETCH_MASK 0x00000200
#define LL2C_FLUSHSI_FETCH_POS 9
#define LL2C_FLUSHSI_LINE_INDEX_MASK 0xffff0000
#define LL2C_FLUSHSI_LINE_INDEX_POS 16
#define LL2C_FLUSHSI_ISSUE_WAY_FLUSH 1<<2

// 31:25 = reserved
#define LL2C_STATUS_LINE_SIZE_BYTES(bits) ((((bits)==64)?1:0)<<24)
#define LL2C_STATUS_PROTECTION_TIMING_SIMULATED (1 << 23)
#define LL2C_STATUS_MEMORY_PROTECTION_IMPLEMENTED (1 << 22)
#define LL2C_STATUS_NUMBER_OF_MTRR_REGS(number) ((number) << 16)
#define LL2C_STATUS_BACKEND_BUS_WIDTH_BITS(bits) (((128/(bits)) & 7) << 13)
#define LL2C_STATUS_SET_SIZE_KiB(kilobytes) (((kilobytes)&0x7ff) << 2)
#define LL2C_STATUS_MULTI_WAY_CONFIGURATION(conf) (((conf) & 3) << 0)
#define LL2C_STATUS_NUMBER_OF_WAYS(ways) LL2C_STATUS_MULTI_WAY_CONFIGURATION((ways) - 1)

#define LL2C_STATUS_MWAY_CONFIG_MAKS 0x00000003
#define LL2C_STATUS_MWAY_CONFIG_POS 0
#define LL2C_STATUS_SET_SIZE_MAKS 0x00001FFC
#define LL2C_STATUS_SET_SIZE_POS 2
#define LL2C_STATUS_BBUS_MAX_WIDTH_BITS 128
#define LL2C_STATUS_BBUS_WIDTH_MAKS 0x0000E000
#define LL2C_STATUS_BBUS_WIDTH_POS 13
#define LL2C_STATUS_NUM_MTRR_MAKS 0x003F0000
#define LL2C_STATUS_NUM_MTRR_POS 16
#define LL2C_STATUS_NUM_MEM_PROT_MAKS 0x00400000
#define LL2C_STATUS_NUM_MEM_PROT_POS 22
#define LL2C_STATUS_PROTECTION_TIMING_MASK 0x00800000
#define LL2C_STATUS_PROTECTION_TIMING_POS 23
#define LL2C_STATUS_LINE_SIZE_MAKS 0x01000000
#define LL2C_STATUS_LINE_SIZE_POS 24

#define LL2C_MTRR_MSB_ADDRESS_BITS_MATCHED 14
#define LL2C_MTRR_MINIMUM_SIZE (1 << (32 - LL2C_MTRR_MSB_ADDRESS_BITS_MATCHED))
#define LL2C_MTRR_UNMATCHED_MASK (LL2C_MTRR_MINIMUM_SIZE - 1)
#define LL2C_MTRR_MATCHED_MASK (0xFFFFFFFF ^ LL2C_MTRR_UNMATCHED_MASK)
#define LL2C_MTRR_HIGHEST_RESOLUTION_MASK LL2C_MTRR_MATCHED_MASK
// MTRRs masks
#define LL2C_MTRR_ADDRESS_MASK 0xFFFC0000
#define LL2C_MTRR_ADDRESS_POS 18
#define LL2C_MTRR_ACCESS_FIELD_MASK 0x00030000
#define LL2C_MTRR_ACCESS_FIELD_POS 16
#define LL2C_MTRR_ADDRESS_MASK_MASK 0x0000FFFC
#define LL2C_MTRR_ADDRESS_MASK_POS 2
#define LL2C_MTRR_WRITE_PROT_MASK 0x00000002
#define LL2C_MTRR_WRITE_PROT_POS 1
#define LL2C_MTRR_ENABLE_MASK 0x00000001

// Offset of Diag tag register
#define L2C_LEON_OS_DIAG_TAG_OFFSET 0x80000

// Offset of Diag data register
#define L2C_LEON_OS_DIAG_DATA_OFFSET 0x200000

#define LL2C_STATUS_LEON_OS ( LL2C_STATUS_LINE_SIZE_BYTES(64) \
                            | LL2C_STATUS_NUMBER_OF_MTRR_REGS(8) \
                            | LL2C_STATUS_BACKEND_BUS_WIDTH_BITS(128) \
                            | LL2C_STATUS_SET_SIZE_KiB(256/4) \
                            | LL2C_STATUS_NUMBER_OF_WAYS(4))

#define LL2C_STATUS_LEON_RT ( LL2C_STATUS_LINE_SIZE_BYTES(64) \
                            | LL2C_STATUS_NUMBER_OF_MTRR_REGS(8) \
                            | LL2C_STATUS_BACKEND_BUS_WIDTH_BITS(128) \
                            | LL2C_STATUS_SET_SIZE_KiB(32/4) \
                            | LL2C_STATUS_NUMBER_OF_WAYS(4))

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

enum DevLL2COperation {
    LL2C_OPERATION_INVALIDATE = 1,
    LL2C_OPERATION_WRITE_BACK = 2,
    LL2C_OPERATION_INVALIDATE_AND_WRITE_BACK = 3,
};

enum DevLL2CWayAndTagOperation {
    LL2C_OPERATION_WAY_AND_TAG_UPDATE_VALID_DIRTY_BITS = 1,
    LL2C_OPERATION_WAY_AND_TAG_WRITE_BACK = 2,
    LL2C_OPERATION_WAY_AND_TAG_UPDATE_VALID_DIRTY_BITS_AND_WRITE_BACK = 3,
};

enum LL2CMTRRAccessMode {
    LL2C_UNCACHED = 0,
    LL2C_DEMOTE_TO_UNCACHED = 0,
    LL2C_WRITE_THROUGH = 1,
    LL2C_DEMOTE_TO_WRITE_THROUGH = 1,
    LL2C_ACCESS_RESERVED_2,
    LL2C_ACCESS_RESERVED_3,
};

enum LL2CBusUsage
{
    BUS_WRAPPING_MODE = 0,
    BUS_SHIFTING_MODE
};


enum LL2CHitRate
{
    HIT_WRAPPING_MODE = 0,
    HIT_SHIFTING_MODE
};

enum LL2CWritePolicy
{
    COPY_BACK = 0,
    WRITE_THROUGH
};

enum LL2CReplacementPolicy
{
    LRU = 0,
    RANDOM,
    MASTER_INDEX_INDEX_REPLACE,
    MASTER_INDEX_MODULUS_FUNCTION
};

typedef struct LL2CConfig
{
    u8 LL2CEnable;
    // Way to replace when Master-index replacement policy and master index is larger then number of ways in the cache
    u8 LL2CWayToReplace;
    // Number of locked ways
    u8 LL2CLockedWaysNo;
    enum LL2CReplacementPolicy replacePolicy;
    enum LL2CBusUsage busUsage;
    enum LL2CHitRate hitRate;
    enum LL2CWritePolicy writePolicy;
} LL2CConfig_t;

typedef enum {
    LL2C_STATUS_REGISTER=0,
    LL2C_USAGE_COUNTER,
    LL2C_CYCLE_COUNTER,
    LL2C_HIT_COUNTER,
    LL2C_ACCESS_COUNTER,
    LL2C_ERROR_ADDRESS
} LL2CRegister_t;

enum LL2CErrorType {
    LL2C_ERROR_NO_ERROR = -1,
    LL2C_ERROR_CACHE_READ = 0,
    LL2C_ERROR_CACHE_WRITE = 1,
    LL2C_ERROR_MEMORY_FETCH = 2,
    LL2C_ERROR_MEMORY_WRITE = 3,
    LL2C_ERROR_WRITE_PROTECT_HIT = 4,
    LL2C_ERROR_BACKEND_AHB_READ = 5,
    LL2C_ERROR_BACKEND_AHB_WRITE = 6,
};

#endif ///DRV_LEON_L2_CACHE_DEFINES_H

/// @}


