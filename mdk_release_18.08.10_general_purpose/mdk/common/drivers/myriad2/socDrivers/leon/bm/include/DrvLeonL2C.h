///
/// @file DrvLeonL2C.h
/// @copyright All code copyright Movidius Srl 2014, Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvLeonL2C LeonL2C Driver
/// @{
/// @brief Leon L2 Cache Driver.
///
/// This module contains the helper functions necessary to control the
/// Leon L2 Cache in the Myriad Soc
///

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifndef DRV_LEON_L2_CACHE_H
#define DRV_LEON_L2_CACHE_H

// 1: Includes
// ----------------------------------------------------------------------------

#include "DrvLeonL2CDefines.h"
#include "DrvCommon.h"
#include "registersMyriad.h"
#include "DrvRegUtilsDefines.h"
#include "swcWhoAmI.h"
#include "assert.h"

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
typedef struct LL2CMemoryRange
{
    u32 address;
    u32 mask;
    enum LL2CMTRRAccessMode accessMode;
    int writeProtected;
} LL2CMemoryRange_t;

// 3:  Exported Functions
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

/// Function to get the base address of L2 cache
///
/// This function is used to get the base address of the L2 Cache depending on the Leon it's running
/// on. The caller should test the return value to be different than 0.
/// @param[in] void
/// @return    the base address of L2 cache or 0 if it fails

static inline u32 DrvLL2CGetMyBase(void)
{
    switch (swcWhoAmI())
    {
    case PROCESS_LEON_OS:
        return L2C_LEON_OS_BASE_ADR;
    case PROCESS_LEON_RT:
        return L2C_LEON_RT_BASE_ADR;
    default:
        return 0;
    }
}

/// Function to initialize Leon L2 Cache
///
/// This function uses a config to initialize the L2 Cache. Check LL2CConfig_t type for more information
/// @param[in] *config check LL2CConfig_t type for more information
/// @return    void

void DrvLL2CInitialize(LL2CConfig_t *config);

/// Function to initialize Leon L2 Cache in Write-Trhough mode.
/// @return    void
void DrvLL2CInitWriteThrough( void );

/// Function to read a given register
///
/// This function is used read the value of a given Leon L2C register.
/// @param[in] reg one of LL2CRegister_t registers
/// @param[in] *regVal pointer to the returned value of the register
/// @return    DRV_RETURN_TYPE

DRV_RETURN_TYPE DrvLL2CGetRegister(LL2CRegister_t reg, u32 *regVal);

/// Function to read a given register
///
/// This function is used read the value of a given Leon L2C register.
/// @param[in] one of LL2CRegister_t registers
/// @param[in] value to be written
/// @return    DRV_RETURN_TYPE

DRV_RETURN_TYPE DrvLL2CSetRegister(LL2CRegister_t reg, u32 value);

/// Function to read Cache error types
///
/// This function returns the type of cache error, if any
/// @param[in] void
/// @return    one of LL2CErrorType values

static inline enum LL2CErrorType DrvLL2CGetAccessErrorType(void)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    u32 reg = (base) + (L2C_LEON_OS_ERR_STAT_CTL - L2C_LEON_OS_BASE_ADR);
    u32 val = GET_REG_WORD_VAL(reg);
    if (!(val & (1 << LL2C_ERR_STAT_VALID_POS)))
    {
        return LL2C_ERROR_NO_ERROR;
    }
    return (enum LL2CErrorType) ((val & LL2C_ERR_STAT_ERRTYPE_MASK)
                                 >> LL2C_ERR_STAT_ERRTYPE_POS);
}

/// Function to disable L2C cache
///
/// This function is used disable the L2C cache. It performs also a flush
/// @param[in] operation This enum specifies the flush mode to be performed (invalidate, write-back or both)
/// @return    void

void DrvLL2CDisable(enum DevLL2COperation operation);

/// Function to get line size
///
/// This function returns L2 cache line size in bytes
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @return    line size, 64 or 32

static inline u32 DrvLL2CGetLineSizeB( void )
{
    u32 val;

    DrvLL2CGetRegister(LL2C_STATUS_REGISTER, &val);

    if (((val & LL2C_STATUS_LINE_SIZE_MAKS) >> LL2C_STATUS_LINE_SIZE_POS) & 1)
        return 64;
    else
        return 32;
}

/// Function to get backend bus width
///
/// This function returns the backend bus width, one of 128, 64 or 32 bits
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @return    backend bus width in bits

static inline u32 DrvLL2CGetBbusWidthBits(void)
{
    u32 val;

    DrvLL2CGetRegister(LL2C_STATUS_REGISTER, &val);

    return (LL2C_STATUS_BBUS_MAX_WIDTH_BITS /
            ((val & LL2C_STATUS_BBUS_WIDTH_MAKS) >> LL2C_STATUS_BBUS_WIDTH_POS));
}

/// Function to get cache set size
///
/// This function returns L2 cache set size in Kilobytes
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @return    set size

static inline u32 DrvLL2CGetSetSizeKB(void)
{
    u32 val;

    DrvLL2CGetRegister(LL2C_STATUS_REGISTER, &val);

    return ((val & LL2C_STATUS_SET_SIZE_MAKS) >> LL2C_STATUS_SET_SIZE_POS);
}

/// Function to flush a cache way(update the valid dirty bits and write the data to memory)
///
/// This function flushes a cache way(update the valid dirty bits and write the data to memory)
/// @param[in] wayNr The cache way to be flush
/// @param[in] flushType Type of cache to be flushed. There are three types of cache flush:
///             - #LL2C_OPERATION_INVALIDATE - mark the specified line as invalid. It's content isn't stored back to memory and it can be overwritten with new data.
///             - #LL2C_OPERATION_WRITE_BACK - write back the dirty data to memory(no invalidation performed)
///             - #LL2C_OPERATION_INVALIDATE_AND_WRITE_BACK - write back dirty data to memory and invalidate the cache line
/// @return    void

void DrvLL2CFlushWay(u32 wayNr, enum DevLL2CWayAndTagOperation flushType);

/// Function to perform flush on address
///
/// This function is perform flush operation on specific address residing in cache
/// @param[in] operation This enum specifies the flush mode to be performed (invalidate, write-back or both)
/// @param[in] disableCache Enable/Disable L2 cache
/// @param[in] address the memory address on which the flush operation will be performed
/// @return    void

static inline void DrvLL2CFlushOpOnAddr(enum DevLL2COperation operation,
                                        u32 disableCache,
                                        u32 address)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    u32 reg = (base) + (L2C_LEON_OS_FLUSH_ADR - L2C_LEON_OS_BASE_ADR);
    u32 val = ((address) & LL2C_FLUSHM_ADDRESS_MASK) |
              ((disableCache << LL2C_FLUSHM_DISABLE_POS) & LL2C_FLUSHM_DISABLE_MASK)
              |
              ((operation << LL2C_FLUSHM_FLUSH_MODE_POS) & LL2C_FLUSHM_FLUSH_MODE_MASK);

    SET_REG_WORD(reg, val);
}

/// Function to perform flush on a range of addresses
///
/// This function is perform flush operation on specific range of addresses residing in cache
/// @param[in] operation This enum specifies the flush mode to be performed (invalidate, write-back or both)
/// @param[in] disableCache Enable/Disable L2 cache
/// @param[in] startAddress start address from memory address on which the flush operation will start
/// @param[in] endAddress address the memory address on which the flush operation will be performed
/// @return    void

static inline void DrvLL2CFlushOpOnAddrRange(enum DevLL2COperation operation,
                                             u32 disableCache,
                                             u32 startAddress, u32 endAddress)
{
    u32 tmp = startAddress, lineSize = DrvLL2CGetLineSizeB();
    do
    {
        DrvLL2CFlushOpOnAddr(operation, disableCache, tmp);
        tmp += lineSize;
    } while (tmp < endAddress);
}

/// Function to perform flush on all cache lines
///
/// This function is perform flush on all cache lines with the option to disable the cache
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @param[in] operation This enum specifies the flush mode to be performed (invalidate, write-back or both)
/// @param[in] disableCache Enable/Disable L2 cache
/// @return    void

static inline void DrvLL2CFlushOpOnAllLines(enum DevLL2COperation operation,
                                            u32 disableCache)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    u32 regAddr = (base) + (L2C_LEON_OS_FLUSH_ADR - L2C_LEON_OS_BASE_ADR);

    u32 val = ((disableCache << LL2C_FLUSHM_DISABLE_POS) & LL2C_FLUSHM_DISABLE_MASK)
              | ((operation << LL2C_FLUSHM_FLUSH_MODE_POS) & LL2C_FLUSHM_FLUSH_MODE_MASK)
              | LL2C_FLUSHM_FLUSH_MODE_OP_ON_ALL;

    SET_REG_WORD(regAddr, val);
}

/// Function to perform flush on way and line
///
/// This function is perform L2 cache flush operation on specific way and line
/// @param[in] operation This enum specifies the flush mode to be performed (invalidate, write-back or both)
/// @param[in] disableCache Enable/Disable L2 cache
/// @param[in] way The way on which the flush will be performed
/// @param[in] lineIndex the line index on which the flush will be performed
/// @return    void

static inline void DrvLL2CFlushOpOnWayAndLine(enum DevLL2COperation operation,
                                              u32 disableCache,
                                              u32 way, u32 lineIndex)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    u32 regAddr = (base) + (L2C_LEON_OS_FLUSH_INDEX - L2C_LEON_OS_BASE_ADR);

    u32 val = ((disableCache << LL2C_FLUSHSI_DISABLE_POS) & LL2C_FLUSHSI_DISABLE_MASK)
              | ((operation << LL2C_FLUSHSI_FLUSH_MODE_POS) & LL2C_FLUSHSI_FLUSH_MODE_MASK)
              | ((way << LL2C_FLUSHSI_WAY_POS) & LL2C_FLUSHSI_WAY_MASK)
              | ((lineIndex << LL2C_FLUSHSI_LINE_INDEX_POS) & LL2C_FLUSHSI_LINE_INDEX_MASK);

    SET_REG_WORD(regAddr, val);
}

/// Function to perform flush on way
///
/// This function is perform L2 cache flush operation on specific way
/// @param[in] operation This enum specifies the flush mode to be performed (invalidate, write-back or both)
/// @param[in] disableCache Enable/Disable L2 cache
/// @param[in] way The way on which the flush will be performed
/// @param[in] tag tag used when way flush is issued
/// @param[in] fetch if set, fetch data from memory when way flush is issued
/// @param[in] valid use valid bit when way flush is issued
/// @param[in] dirty use dirty bit when way flush is issued
/// @return    void

static inline void DrvLL2CFlushOpOnWayAndTag(
                enum DevLL2CWayAndTagOperation operation,
                u32 disableCache,
                u32 way, u32 tag, u32 fetch, u32 valid, u32 dirty)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    u32 reg = (base) + (L2C_LEON_OS_FLUSH_INDEX - L2C_LEON_OS_BASE_ADR);
    u32 val = ((disableCache << LL2C_FLUSHSI_DISABLE_POS) & LL2C_FLUSHSI_DISABLE_MASK)
              | ((operation << LL2C_FLUSHSI_FLUSH_MODE_POS) & LL2C_FLUSHSI_FLUSH_MODE_MASK)
              | ((way << LL2C_FLUSHSI_WAY_POS) & LL2C_FLUSHSI_WAY_MASK)
              | (tag & LL2C_FLUSHSI_TAG_MASK)
              | ((fetch << LL2C_FLUSHSI_FETCH_POS) & LL2C_FLUSHSI_FETCH_MASK)
              | ((valid << LL2C_FLUSHSI_VALID_POS) & LL2C_FLUSHSI_VALID_MASK)
              | ((dirty << LL2C_FLUSHSI_DIRTY_POS) & LL2C_FLUSHSI_DIRTY_MASK)
              | LL2C_FLUSHSI_ISSUE_WAY_FLUSH;

    SET_REG_WORD(reg, val);
}

/// Function to get the number of MTRR registers
///
/// This function returns the number of MTRR registers implemented
/// @param[in] void
/// @return    number of MTRR registers

static inline u32 DrvLL2CGetNumOfMTRRRegs(void)
{
    u32 val;

    DrvLL2CGetRegister(LL2C_STATUS_REGISTER, &val);

    return (val & LL2C_STATUS_NUM_MTRR_MAKS) >> LL2C_STATUS_NUM_MTRR_POS;
}

/// Function to set the MTRR registers
///
/// This function configures a number of ranges to the MTRR registes.
/// Warning: when changing MTRR registers, the cache must be disabled and flushed
/// @param[in] ranges array of MTRR configurations
/// @param[in] nrRanges number of ranges to configure
/// @return    void

void DrvLL2CSetMTRRs(const struct LL2CMemoryRange *ranges, u32 nrRanges);

/// Function to add a function to MTRR
///
/// This function add the given function address to the next available MTRR register.
/// Warning: when changing MTRR registers, the cache must be disabled and flushed
/// @param[in] *addr address of the function to add to MTRR
/// @param[in] size size of function
/// @param[in] mode access mode
/// @param[in] wp write protection
/// @return    returns 0 on success; negative value if it fails

s32 DrvLL2CSetMTRRfunc(void *funcAddr, u32 size, u32 mode, u32 wp);

/// Function to remove a function from MTRR
///
/// This function removes the given function address from the MTRR register (resets register to default).
/// Warning: when changing MTRR registers, the cache must be disabled and flushed
/// @param[in] *addr address of the function to add to MTRR
/// @return    returns 0 on success
/// @deprecated use DrvLL2CRemoveMTRRfunc instead.

s32 DrvLL2CRemoveMTRRfunc(void *addr);

/// Function to get a cache tag
///
/// This function is used to get a cache tag value from a specified location
/// @param[in] way - the way number between 0..3
/// @param[in] tagEntryIndex - when the l2 cache controller is configured for 32 byte lines,
///               this is the same as the line number. When the controller is configured for
///               64-byte lines, then each line occupies two tag entries, so the tagEntryIndex
///               parameter corresponds to line_index*2, and line_index*2+1
/// @return    tag

static inline u32 DrvLL2CDiagGetTag(u32 way, u32 tagEntryIndex)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    return GET_REG_WORD_VAL(
                    base + L2C_LEON_OS_DIAG_TAG_OFFSET + tagEntryIndex * 0x20 + way * 4);
}

/// Function to manually set a cache tag to a specified location
///
/// @param[in] way - the way number between 0..3
/// @param[in] tagEntryIndex - when the l2 cache controller is configured for 32 byte lines,
///               this is the same as the line number. When the controller is configured for
///               64-byte lines, then each line occupies two tag entries, so the tagentry_index
///               parameter corresponds to line_index*2, and line_index*2+1
/// @param[in] tagEntryValue - the entry value to set. This includes that tag field, the valid, dirty bits,
///               and the LRU field. When the L2 cache controller in configured for 64 bit line size,
///               the tag field of the tag entry is common for both tag entries that refer to a single line.
/// @return    void

static inline void DrvLL2CDiagSetTag(u32 way, u32 tagEntryIndex,
                                     u32 tagEntryValue)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    SET_REG_WORD(base + L2C_LEON_OS_DIAG_TAG_OFFSET + tagEntryIndex * 0x20 + way * 4,
                 tagEntryValue);
}

/// Function to get the pointer to a specific cache line
///
/// @param[in] base base address of the L2 cache controller you may want to use DrvLL2CGetMyBase()
///                    to determine this on the fly, or you can use a constant such as L2C_LEON_OS_BASE_ADR
/// @param[in] way the way number between 0..3
/// @param[in] line the index of needed line
/// @return    *void pointer to cache line

static inline void *DrvLL2CDiagGetLinePointer(u32 way, u32 line)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    return (void *) (base + L2C_LEON_OS_DIAG_DATA_OFFSET
                     + way * L2C_LEON_OS_DIAG_TAG_OFFSET
                     + line * DrvLL2CGetLineSizeB());
}

/// Function to check whether memory protection is implemented
///
/// This function checks whether memory protection is implemented
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @return    1 if implemented, 0 if not
/// @deprecated use DrvLL2CGetMemProtectionImplemented instead.

static inline u32 DrvLL2CGetProtectionTimingSimulated(u32 base)
{
    UNUSED(base); // hush the compiler warning.
    u32 val;

    DrvLL2CGetRegister(LL2C_STATUS_REGISTER, &val);

    return ((val & LL2C_STATUS_PROTECTION_TIMING_MASK) >> LL2C_STATUS_PROTECTION_TIMING_POS);
}

/// Function to check whether memory protection is implemented
///
/// This function checks whether memory protection is implemented
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @return    1 if implemented, 0 if not

static inline u8 DrvLL2CGetMemProtectionImplemented(void)
{
    u32 val;

    DrvLL2CGetRegister(LL2C_STATUS_REGISTER, &val);

    return ((val & LL2C_STATUS_NUM_MEM_PROT_MAKS) >> LL2C_STATUS_NUM_MEM_PROT_POS);
}

/// Function to get the multi-way configuration
///
/// This function returns the multi way configuration, one of: direct mapping, 2, 3 or 4 way
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @return    cache way configuration

static inline u32 DrvLL2CGetMultiWayConfig(void)
{
    u32 val;

    DrvLL2CGetRegister(LL2C_STATUS_REGISTER, &val);

    return (val & LL2C_STATUS_MWAY_CONFIG_MAKS) >> LL2C_STATUS_MWAY_CONFIG_POS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////DEPRECATED FUNCTIONS HERE//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

/// Function to set LL2C Control Register
///
/// This function is used to set the Control Register
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @param[in] value The value to be set in control register.
/// @return    void
/// @deprecated use DrvLL2CInit instead.

static inline void __Deprecated__("Use DrvLL2CInit instead") DrvLL2CSetControl(
                u32 base, u32 value)
{
    SET_REG_WORD((base) + (L2C_LEON_OS_CONTROL - L2C_LEON_OS_BASE_ADR),
                 (value));
}

/// Function to get Control Register
///
/// This function is used to get the Leon L2 Cache Control Register
/// @param[in] void
/// @return    The value of control register.
/// @deprecated use DrvLL2CGetConfig instead.

u32 DrvLL2CGetControlRegister(void)
                __Deprecated__("Use DrvLL2CGetConfig instead");

/// Function to initialize Leon L2 Cache
///
/// This function uses a default config to init the L2C:
///     - Replacement policy: LRU;
///     - Locked ways: 0
///     - Write policy: Copy-Back
/// @param[in] void
/// @return    void
/// @deprecated use DrvLL2CGetAccessErrorType instead.

static inline void __Deprecated__("Use DrvLL2CInitialize instead") DrvLL2CacheInit(
                void)
{
    u32 base = DrvLL2CGetMyBase();
    // Invalidate entire L2Cache before enabling it
    DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, /*disable cache?:*/0);

    DrvLL2CSetControl(
                    base,
                    ( LL2C_CTRL_EN | LL2C_CTRL_REPL__LRU
                      | LL2C_CTRL_NR_LOCKED_WAYS(0)
                      | LL2C_CTRL_WRITE_POLICY_COPY_BACK));
}

/// Function to get Status Register
///
/// This function is used to get the Leon L2 Cache Status Register
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @return    The value of status register.
/// @deprecated use DrvLL2CGetRegister instead.

static inline u32 __Deprecated__("Use DrvLL2CGetRegister instead") DrvLL2CGetStatus(
                u32 base)
{
    u32 reg = (base) + (L2C_LEON_OS_STATUS - L2C_LEON_OS_BASE_ADR);
    return GET_REG_WORD_VAL(reg);
}

/// Function to get line size
///
/// This function returns L2 cache line size in bytes
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @return    line size, 64 or 32
/// @deprecated use DrvLL2CGetLineSizeB instead.

static inline u32 __Deprecated__("Use DrvLL2CGetLineSizeB instead") DrvLL2CGetLineSizeBytes(
                u32 base)
{
    if ((((DrvLL2CGetStatus(base)) & LL2C_STATUS_LINE_SIZE_MAKS) >> LL2C_STATUS_LINE_SIZE_POS) & 1)
        return 64;
    else
        return 32;
}

/// Function to perform flush on address
///
/// This function is perform flush operation on specific address residing in cache
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @param[in] operation This enum specifies the flush mode to be performed (invalidate, write-back or both)
/// @param[in] disableCache Enable/Disable L2 cache
/// @param[in] address the memory address on which the flush operation will be performed
/// @return    void
/// @deprecated use DrvLL2CFlushOpOnAddr instead.

static inline void __Deprecated__("Use DrvLL2CFlushOpOnAddr instead") DrvLL2COperationOnAddress(
                u32 base,
                enum DevLL2COperation operation,
                u32 disableCache, u32 address)
{
    u32 reg = (base) + (L2C_LEON_OS_FLUSH_ADR - L2C_LEON_OS_BASE_ADR);
    u32 val = ((address) & LL2C_FLUSHM_ADDRESS_MASK) |
              ((disableCache << LL2C_FLUSHM_DISABLE_POS) & LL2C_FLUSHM_DISABLE_MASK)
              |
              ((operation << LL2C_FLUSHM_FLUSH_MODE_POS) & LL2C_FLUSHM_FLUSH_MODE_MASK);

    SET_REG_WORD(reg, val);
}

/// Function to perform flush on all cache lines
///
/// This function is perform flush on all cache lines with the option to disable the cache
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @param[in] operation This enum specifies the flush mode to be performed (invalidate, write-back or both)
/// @param[in] disableCache Enable/Disable L2 cache
/// @return    void
/// @deprecated use DrvLL2CFlushOpOnAllLines instead.

static inline void __Deprecated__("Use DrvLL2CFlushOpOnAllLines instead") DrvLL2COperationOnAll(
                u32 base, enum DevLL2COperation operation,
                u32 disableCache)
{
    u32 regAddr = (base) + (L2C_LEON_OS_FLUSH_ADR - L2C_LEON_OS_BASE_ADR);

    u32 val = ((disableCache << LL2C_FLUSHM_DISABLE_POS) & LL2C_FLUSHM_DISABLE_MASK)
              | ((operation << LL2C_FLUSHM_FLUSH_MODE_POS) & LL2C_FLUSHM_FLUSH_MODE_MASK)
              | LL2C_FLUSHM_FLUSH_MODE_OP_ON_ALL;

    SET_REG_WORD(regAddr, val);
}

/// Function to set the MTRR registers
///
/// This function configures a number of ranges to the MTRR registes
/// @param[in] base LeonRT/OS base address from which the L2 cache register reside. Used to make the difference between LeonOS and LeonRT
/// @param[in] ranges array of MTRR configurations
/// @param[in] nrRanges number of ranges to configure
/// @return    void
/// @deprecated use DrvLL2CSetMTRRs instead.

void DrvLL2CSetMTRR(u32 base, const struct LL2CMemoryRange *ranges,
                    u32 nrRanges) __Deprecated__("Use DrvLL2CSetMTRRs instead");

/// Function to add a function to MTRR
///
/// This function add the given function address to the next available MTRR register.
/// @param[in] *addr address of the function to add to MTRR
/// @param[in] size size of function
/// @param[in] mode access mode
/// @param[in] wp write protection
/// @return    returns 0 on success
/// @deprecated use DrvLL2CSetMTRRfunc instead.

s32 SetMTRRfunc(void *addr, u32 size, u32 mode, u32 wp)
                __Deprecated__("Use DrvLL2CSetMTRRfunc instead");

/// Function to get a cache tag
///
/// This function is used to get a cache tag value from a specified location
/// @param[in] way - the way number between 0..3
/// @param[in] tagEntryIndex - when the l2 cache controller is configured for 32 byte lines,
///               this is the same as the line number. When the controller is configured for
///               64-byte lines, then each line occupies two tag entries, so the tagEntryIndex
///               parameter corresponds to line_index*2, and line_index*2+1
/// @return    tag
/// @deprecated use DrvLL2CDiagGetTag instead.

static inline u32 __Deprecated__("Use DrvLL2CDiagGetTag instead") DrvLL2CDiagnosticGetTag(
                u32 base, u32 way, u32 tagEntryIndex)
{
    return GET_REG_WORD_VAL(base + 0x80000 + tagEntryIndex * 0x20 + way * 4);
}

/// Function to manually set a cache tag to a specified location
///
/// @param[in] way - the way number between 0..3
/// @param[in] tagEntryIndex - when the l2 cache controller is configured for 32 byte lines,
///               this is the same as the line number. When the controller is configured for
///               64-byte lines, then each line occupies two tag entries, so the tagentry_index
///               parameter corresponds to line_index*2, and line_index*2+1
/// @param[in] tagEntryValue - the entry value to set. This includes that tag field, the valid, dirty bits,
///               and the LRU field. When the L2 cache controller in configured for 64 bit line size,
///               the tag field of the tag entry is common for both tag entries that refer to a single line.
/// @return    void
/// @deprecated use DrvLL2CDiagSetTag instead.

static inline void __Deprecated__("Use DrvLL2CDiagSetTag instead") DrvLL2CDiagnosticSetTag(
                u32 base, u32 way, u32 tagEntryIndex, u32 tagEntryValue)
{
    SET_REG_WORD(base + 0x80000 + tagEntryIndex * 0x20 + way * 4,
                 tagEntryValue);
}

/// Function to get the pointer to a specific cache line
///
/// @param[in] base base address of the L2 cache controller you may want to use DrvLL2CGetMyBase()
///                    to determine this on the fly, or you can use a constant such as L2C_LEON_OS_BASE_ADR
/// @param[in] way the way number between 0..3
/// @param[in] line the index of needed line
/// @return    *void pointer to cache line
/// @deprecated use DrvLL2CDiagGetLinePointer instead.

static inline void __Deprecated__("Use DrvLL2CDiagGetLinePointer instead") *DrvLL2CDiagnosticGetLinePointer(
                u32 base, u32 way, u32 line)
{
    return (void *) (base + 0x200000 + way * 0x80000
                     + line * DrvLL2CGetLineSizeB());
}

/// Function to flush all cache lines
///
/// This function flushes cache lines
/// @param[in] flushType Type of cache to be flushed. There are three types of cache flush:
///             - #LL2C_OPERATION_INVALIDATE - mark the specified line as invalid. It's content isn't stored back to memory and it can be overwritten with new data.
///             - #LL2C_OPERATION_WRITE_BACK - write back the dirty data to memory(no invalidation performed)
///             - #LL2C_OPERATION_INVALIDATE_AND_WRITE_BACK - write back dirty data to memory and invalidate the cache line
/// @return    void
/// @deprecated Use DrvLL2CFlushOpOnAllLines instead

void DrvLL2CFlushAll(enum DevLL2COperation flushType)
                __Deprecated__("Use DrvLL2CFlushOpOnAllLines instead");

#ifdef __cplusplus
}
#endif

#endif ///DRV_LEON_L2_CACHE_H

#pragma GCC diagnostic pop

/// @}


