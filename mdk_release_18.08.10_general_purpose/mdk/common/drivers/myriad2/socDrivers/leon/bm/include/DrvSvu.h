///
/// @file DrvSvu.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvSvu Svu Driver
/// @{
/// @brief     Shave low level definitions and types
///
/// Definitions and types dealing with Shave low level functionality.
///

#ifndef _DRVSVU_H_
#define _DRVSVU_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <DrvRegUtils.h>
#include "DrvSvuDefines.h"
#include "registersMyriad.h"
#include "swcShaveLoader.h"
#include <assert.h>
#include "DrvCommon.h"
#include <dbgTracerApi.h>

// 2:  Source Specific #defines
// ----------------------------------------------------------------------------
/// Special value indicating that a port clash occurred and the register couldn't be read
#define STATUS_RD_PORT_CLASH    (0xDEAD1501)

/// Useful macros missing in DrvSvuDefines.h (generated automatically)
/// Shave L1 Instruction cache register access macros
#define SHAVE_L1C_I_CTRL(shv)       (SVU_CTRL_ADDR[shv] + SLC_OFFSET_IL1 + SHV_IL1_CTRL)
#define SHAVE_L1C_I_HIT_CNT(shv)    (SVU_CTRL_ADDR[shv] + SLC_OFFSET_IL1 + SHV_HIT_CNT)
#define SHAVE_L1C_I_MISS_CNT(shv)   (SVU_CTRL_ADDR[shv] + SLC_OFFSET_IL1 + SHV_MISS_CNT)
#define SHAVE_L1C_I_RAM_CFG(shv)    (SVU_CTRL_ADDR[shv] + SLC_OFFSET_IL1 + SHV_IL1_RAM_CFG)
#define SHAVE_L1C_I_STATUS(shv)     (SVU_CTRL_ADDR[shv] + SLC_OFFSET_IL1 + SHV_IL1_STATUS)
/// Shave L1 Instruction cache debug TAG & DATA access
#define SHAVE_L1C_I_TAG_BASE(shv)       (SVU_CTRL_ADDR[shv] + SLC_OFFSET_IL1 + SHV_IL1_TAG_ACCESS)
#define SHAVE_L1C_I_TAG_IDX(set, way)   (((set) << 3) + ((way) << 2))
#define SHAVE_L1C_I_TAG(shv, set, way)  (SHAVE_L1C_I_TAG_BASE(shv) + SHAVE_L1C_I_TAG_IDX(set, way))
#define SHAVE_L1C_I_DAT_BASE(shv)           (SVU_CTRL_ADDR[shv] + SLC_OFFSET_IL1 + SHV_IL1_DATA_ACCESS)
#define SHAVE_L1C_I_DAT_IDX(set, way, w)    (((set) << 5) + ((way) << 4) + ((w) << 2))
#define SHAVE_L1C_I_DAT(shv, set, way, w)   (SHAVE_L1C_I_DAT_BASE(shv) + SHAVE_L1C_I_DAT_IDX(set, way, w))

#ifdef __cplusplus
extern "C" {
#endif

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// Do this so we can write to SVUs using "for" loops
extern u32 SLICE_BASE_ADDR[TOTAL_NUM_SHAVES];
extern u32 SVU_CTRL_ADDR[TOTAL_NUM_SHAVES];

// 4:  Exported Functions (non-static inline)
// ----------------------------------------------------------------------------
/// Zero all register files
/// @param[in] - svu -> Shave number to zero register files for
/// @return    - void
///
void DrvSvutZeroRfs(u32 svu);

//void svut_pc_write(u32 svu, u32 pc_val)
//{
//    SET_REG_WORD(DCU_SVU_PTR(svu), pc_val);
//}
//
//void svut_start(u32 svu)
//{
//    /// Without affecting other bits in OCR
//    u32 OCR_val = GET_REG_WORD_VAL(DCU_OCR(svu));
//    SET_REG_WORD(DCU_OCR(svu), (OCR_val & 0xFFFFFFFB));
//}

/// Invalidate L1 cache for a specified Shave
/// @param[in] - shvNumber -> Shave number
/// @return    - void
/// @deprecated Use DrvSvuL1DataCacheCtrl(u32 shvNumber, SVUL1DATACACHE_INVALIDATE_ALL) instead.
///
__Deprecated__("Use DrvSvuL1DataCacheCtrl(u32 shvNumber, SVUL1DATACACHE_INVALIDATE_ALL) instead")
void DrvSvuInvalidateL1Cache(u32 shvNumber);

/// Performs an L1 Data Cache operation on the given Shave
/// @param[in] - shvNumber -> number of the Shave processor used
/// @param[in] - action -> supported cache operations are:
///                 - SVUL1DATACACHE_ENABLE: enables Shave L1 Data Cache
///                 - SVUL1DATACACHE_DISABLE: disables Shave L1 Data Cache
///                 - SVUL1DATACACHE_STOP: stops Shave L1 Data Cache
///                 - SVUL1DATACACHE_INVALIDATE_ALL: invalidates entire L1 Data Cache
///                 - SVUL1DATACACHE_FLUSH_ALL: flushes entire L1 Data Cache
///                 - SVUL1DATACACHE_FLUSH_INVALIDATE_ALL: flushes and invalidates entire L1 Data Cache
/// @return void
///
void DrvSvuL1DataCacheCtrl(u32 shvNumber, DrvSvuL1DCacheAction_t action);

/// Performs an L1 Instruction Cache operation on the given Shave
/// @param[in] - shvNumber -> number of the Shave processor used
/// @param[in] - action -> supported cache operations are:
///                 - SVUL1INSTRCACHE_DEFAULT: enables Shave L1 Instruction Cache
///                 - SVUL1INSTRCACHE_BYPASS: bypasses Shave L1 Instruction Cache
///                 - SVUL1INSTRCACHE_LOCK: locks Shave L1 Instruction Cache
///                 - SVUL1INSTRCACHE_INVALIDATE: invalidates L1 Instruction Cache
/// @return void
///
void DrvSvuL1InstrCacheCtrl(u32 shvNumber, u32 action);

// 5:  Exported Functions (static inline)
// ----------------------------------------------------------------------------
/// Read the value from an I-Register of a specific Shave
/// @param[in] - svu -> Shave number to read I-Register value from
/// @param[in] - irfLoc -> I-Register position to read
/// @return    - u32 value of the read register
///
static inline u32 DrvSvutIrfRead(u32 svu, u32 irfLoc)
{
    u32 retValue;

    do
    {
        retValue = GET_REG_WORD_VAL(DCU_SVU_IRF(svu, irfLoc));
    }
    while (retValue == STATUS_RD_PORT_CLASH);   /// Port clash occurred => could not read the register

    return retValue;

    //SET_REG_WORD(SVU_CTRL_ADDR[svu] + SVU_RFC_ADDR, 0x00000140 + irfLoc);
    //u32 irfVal = GET_REG_WORD_VAL(SVU_CTRL_ADDR[svu] + SVU_RF0_ADDR);
    //return irfVal;
}

/// Read the value of an element from a V-Register of a specific Shave
/// @param[in] - svu -> Shave number to read V-Register value from
/// @param[in] - vrfLoc -> V-Register position to read
/// @param[in] - vrfEle -> V-Register location to read
/// @return    - u32 value of the read register
///
static inline u32 DrvSvutVrfRead(u32 svu, u32 vrfLoc, u32 vrfEle)
{
    return GET_REG_WORD_VAL(SVU_CTRL_ADDR[svu] + SLC_OFFSET_SVU +
                            VRF_BASE + (vrfLoc << 4) + (vrfEle << 2));
}

/// Read the value from a T-Register of a specific Shave
/// @param[in] - svu -> Shave number to read T-Register value from
/// @param[in] - trfLoc -> T-Register position to read
/// @return    - u32 value of the read register
///
static inline u32 DrvSvutTrfRead(u32 svu, u32 trfLoc)
{
    u32 retValue;

    retValue = GET_REG_WORD_VAL(DCU_SVU_TRF(svu, trfLoc));

    return retValue;
}

/// Write the value to an I-Register of a specific Shave
/// @param[in] - svu -> Shave number to write I-Register value to
/// @param[in] - irfLoc -> I-Register position to write to
/// @param[in] - irfVal -> value to write
/// @return    - void
///
static inline void DrvSvutIrfWrite(u32 svu, u32 irfLoc, u32 irfVal)
{
    SET_REG_WORD(DCU_SVU_IRF(svu, irfLoc), irfVal);
}

/// Write the value to a V-Register element of a specific Shave
/// @param[in] - svu -> Shave number to write V-Register value to
/// @param[in] - vrfLoc -> V-Register position to write to
/// @param[in] - vrfEle -> V-Register element to write to
/// @param[in] - vrfVal -> value to write
/// @return    - void
///
static inline void DrvSvutVrfWrite(u32 svu, u32 vrfLoc, u32 vrfEle, u32 vrfVal)
{
    SET_REG_WORD(SVU_CTRL_ADDR[svu] + SLC_OFFSET_SVU + VRF_BASE +
                 (vrfLoc << 4) + (vrfEle << 2), vrfVal);
}

/// Write the value to a T-Register of a specific Shave
/// @param[in] - svu -> Shave number to write T-Register value to
/// @param[in] - trfLoc -> T-Register position to write to
/// @param[in] - trfVal -> value to write
/// @return    - void
///
static inline void DrvSvutTrfWrite(u32 svu, u32 trfLoc, u32 trfVal)
{
    u32 TRFAddress = DCU_SVU_TRF(svu, trfLoc);

    SET_REG_WORD(TRFAddress, trfVal);
}

/// Check if a Shave is halted or not
/// @param[in] - svu -> Shave number to check if halted or not
/// @return    - u32 -> 0 - is not halted, 1 - is halted
///
static inline u32 DrvSvuSwiHalted(u32 svu)
{
    u32 status = GET_REG_WORD_VAL(DCU_OSR(svu));

    return !!(status & 0x00000008);
}

/// Check if a Shave is halted or not
/// @param[in] - svu -> Shave number to check if halted or not
/// @return    - u32 -> 0 - is not halted, 1 - is halted
/// @deprecated Please use DrvSvuSwiHalted(u32 svu) instead
///
__Deprecated__("Please use DrvSvuSwiHalted(u32 svu) instead.")
static inline u32 DrvSvuIsStopped(u32 svu)
{
    return DrvSvuSwiHalted(svu);
}

/// Check Shave tag
/// @param[in] - svu -> Shave number to check tag for
/// @return    - u32 tag
///
static inline u32 DrvSvutSwiTag(u32 svu)
{
    u32 status = GET_REG_WORD_VAL(DCU_ISR(svu));

    return ((status >> 13) & 0x0000001F);
}

/// Function used to start a Shave from a specific address
/// @param[in] - svu -> Shave number to start
/// @param[in] - pcVal -> program counter value
/// @return    - void
///
static inline void DrvSvuStart(u32 svu, u32 pcVal)
{
    u32 OCR_val;

    if ((pcVal & 0xFF000000) == 0x1D000000)     /// Optimised away when assert() not present
    {
        /// If we are attempting to start a shave in windowed space,
        /// make sure window pointer is set
        assert(GET_REG_WORD_VAL((SHAVE_0_BASE_ADR + (SVU_SLICE_OFFSET * svu) +
                                 SLC_TOP_OFFSET_WIN_B )) != 0);
    }

    OCR_val = GET_REG_WORD_VAL(DCU_OCR(svu));
    SET_REG_WORD(DCU_SVU_PTR(svu), pcVal);
    /// Without affecting other bits in OCR
    SET_REG_WORD(DCU_OCR(svu), OCR_val & 0xFFFFFFFB);

    // Add the tracer functionality to visualize the shave run:start
    dbgLogEvent((Event_t)((u32)LOG_EVENT_SHAVE_0_RUN +  svu), pcVal, DEBUG_LOG_LEVEL_HIGH);
    return;
}

/// Stop a specific Shave
/// @param[in] - svu -> Shave number to stop
/// @return    - void
///
static inline void DrvSvutStop(u32 svu)
{
    /// Without affecting other bits in OCR
    u32 OCR_val = GET_REG_WORD_VAL(DCU_OCR(svu));

    SET_REG_WORD(DCU_OCR(svu), OCR_val | 0x4);

    // Add the tracer functionality to visualize the shave run:complete
    dbgLogEvent((Event_t)((u32)LOG_EVENT_SHAVE_0_RUN + svu), 0, DEBUG_LOG_LEVEL_HIGH);
}

/// Enable performance counter for a specific Shave
/// @param[in] - shaveNumber -> Shave number to enable perf. counter for
/// @param[in] - pcNb -> Performance counter number. Valid values [0, 3].
/// @param[in] - countType -> Count type. Supported types are:
///                 - PC_IBP0_EN (instructions on performance counter 0)
///                 - PC_IBP1_EN (instructions on performance counter 1)
///                 - PC_DBP0_EN (data counter on performance counter 0)
///                 - PC_DBP1_EN (data counter on performance counter 1)
///                 - any other value: just enable history
/// @return    - void
///
static inline void DrvSvuEnablePerformanceCounter(u32 shaveNumber, u32 pcNb,
                                                  u32 countType)
{
    u32 bpCtrl;

    switch (countType)
    {
        case PC_IBP0_EN:
            bpCtrl = GET_REG_WORD_VAL(DCU_IBC0(shaveNumber));
            SET_REG_WORD(DCU_IBC0(shaveNumber), (bpCtrl | 0x00002000));
            break;
        case PC_IBP1_EN:
            bpCtrl = GET_REG_WORD_VAL(DCU_IBC1(shaveNumber));
            SET_REG_WORD(DCU_IBC1(shaveNumber), (bpCtrl | 0x00002000));
            break;
        case PC_DBP0_EN:
            bpCtrl = GET_REG_WORD_VAL(DCU_DBC0(shaveNumber));
            SET_REG_WORD(DCU_DBC0(shaveNumber), (bpCtrl | 0x20000000));
            break;
        case PC_DBP1_EN:
            bpCtrl = GET_REG_WORD_VAL(DCU_DBC1(shaveNumber));
            SET_REG_WORD(DCU_DBC1(shaveNumber), (bpCtrl | 0x20000000));
            break;
        default:
            /// Enable history
            bpCtrl = GET_REG_WORD_VAL(DCU_OCR(shaveNumber));
            SET_REG_WORD(DCU_OCR(shaveNumber), (bpCtrl | 0x00000800));

            /// Enable debug
            u32 dcrValue = GET_REG_WORD_VAL(DCU_DCR(shaveNumber));
            SET_BITS(DCU_DCR(shaveNumber), dcrValue, DCR_DBGE);
            break;
    }

    switch (pcNb)
    {
        case 0:
            SET_REG_WORD(DCU_PCC0(shaveNumber), countType);
            break;
        case 1:
            SET_REG_WORD(DCU_PCC1(shaveNumber), countType);
            break;
        case 2:
            SET_REG_WORD(DCU_PCC2(shaveNumber), countType);
            break;
        case 3:
            SET_REG_WORD(DCU_PCC3(shaveNumber), countType);
            break;
        default:
            assert("Invalid Performance Counter number selected!");
            break;
    }
}

/// Read executed instructions counter of a given Shave
/// @param[in] - shaveNumber -> Shave number
/// @return    - u32 get executed instructions count
///
static inline u32 DrvSvuGetPerformanceCounter0(u32 shaveNumber)
{
    return GET_REG_WORD_VAL(DCU_PC0(shaveNumber));
}

/// Read stall cycles counter of a given Shave
/// @param[in] - shaveNumber -> Shave number
/// @return    - u32 get stall cycles count
///
static inline u32 DrvSvuGetPerformanceCounter1(u32 shaveNumber)
{
    return GET_REG_WORD_VAL(DCU_PC1(shaveNumber));
}

/// Read branches taken counter of a given Shave
/// @param[in] - shaveNumber -> Shave number
/// @return    - u32 get stall cycles count
///
static inline u32 DrvSvuGetPerformanceCounter2(u32 shaveNumber)
{
    return GET_REG_WORD_VAL(DCU_PC2(shaveNumber));
}

/// Read clock cycles counter of a given Shave
/// @param[in] - shaveNumber -> Shave number
/// @return    - u32 get stall cycles count
///
static inline u32 DrvSvuGetPerformanceCounter3(u32 shaveNumber)
{
    return GET_REG_WORD_VAL(DCU_PC3(shaveNumber));
}

/// Enable L1 cache for a specified Shave
/// @param[in] - shaveNumber -> Shave number for which to enable L1 cache
/// @return    - void
/// @deprecated Use DrvSvuL1DataCacheCtrl(u32 shvNumber, SVUL1DATACACHE_ENABLE) instead.
///
__Deprecated__("Use DrvSvuL1DataCacheCtrl(u32 shvNumber, SVUL1DATACACHE_ENABLE) instead")
static inline void DrvSvuEnableL1Cache(u32 shaveNumber)
{
    /// Set non-windowed L1 cache policy configuration for LSU DDRL2Cache/Bypass accesses
    SET_REG_WORD(SLICE_NWN_CPC(shaveNumber), 0x180);    /// LSU ENABLE DDRL2C/B L1 CACHING

    /// Invalidate and enable L1Cache
    SET_REG_WORD(SHAVE_CACHE_DBG_TXN_TYPE(shaveNumber), 0x9);   /// ENABLE (STOPPED)
    SET_REG_WORD(SHAVE_CACHE_CTRL(shaveNumber), 0x5);           /// INVALIDATE

    do
    { NOP; }
    while (GET_REG_WORD_VAL(SHAVE_CACHE_STATUS(shaveNumber)) != 0x0);   /// BUSY
    SET_REG_WORD(SHAVE_CACHE_DBG_TXN_TYPE(shaveNumber), 0x1);           /// ENABLE

    return;
}

/// Data L1 cache control register setting for a specified Shave
/// @param[in] - shvNumber -> Shave number
/// @param[in] - ctrlType -> bit field options. Supported values are:
///                 - L1C_INVAL_ALL
///                 - L1C_FLUSH_ALL
///                 - L1C_FLUSHINV_ALL
/// @return    - void
/// @deprecated Use DrvSvuL1DataCacheCtrl(u32 shvNumber, u32 action) instead.
///
__Deprecated__("Use DrvSvuL1DataCacheCtrl(u32 shvNumber, u32 action) instead")
static inline void DrvSvuL1CacheCtrl(u32 shvNumber, u32 ctrlType)
{
    u32 lVal;
    int stopBit = 0x8;

    /// Set Stop
    lVal = GET_REG_WORD_VAL(SHAVE_CACHE_CTRL(shvNumber));
//    DEBUG_INFO ("SHAVE_CACHE_CTRL = ");
//    ACTUAL_DATA (lVal);
    SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), (lVal | stopBit));

    asm("nop"); asm("nop"); asm("nop");

    while(GET_REG_WORD_VAL(SHAVE_CACHE_CTRL(shvNumber) + 0x18) & 0x01);

    /// Select Invalidate Operation
    SET_REG_WORD(SHAVE_CACHE_DBG_TXN_TYPE(shvNumber), ctrlType);

    asm("nop"); asm("nop"); asm("nop");

    while(GET_REG_WORD_VAL(SHAVE_CACHE_CTRL(shvNumber) + 0x18) & 0x03);

    /// Clear Stop
    lVal = GET_REG_WORD_VAL(SHAVE_CACHE_CTRL(shvNumber));

//    DEBUG_INFO ("SHAVE_CACHE_CTRL = ");
//    ACTUAL_DATA (lVal);
    SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), (lVal & ~stopBit));
}

/// L1 cache line control register setting for a specified Shave
/// @param[in] - shvNumber -> Shave number
/// @param[in] - ctrlType -> bit field options. Supported values are:
///                 - L1C_PFL1
///                 - L1C_PFL1_LOCK
///                 - L1C_PFL2
///                 - L1C_FLUSH_LINE
///                 - L1C_INVAL_LINE
/// @param[in] - address -> address
/// @return    - void
///
static inline void DrvSvuL1CacheCtrlLine(u32 shvNumber, u32 ctrlType, u32 address)
{
    u32 lVal;
    int stopBit = 0x8;

    /// Set Stop
    lVal = GET_REG_WORD_VAL(SHAVE_CACHE_CTRL(shvNumber));
//    DEBUG_INFO ("SHAVE_CACHE_CTRL = ");
//    ACTUAL_DATA (lVal);
    SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), (lVal | stopBit));

    /// Set address and then operations
    SET_REG_WORD(SHAVE_CACHE_DBG_TXN_ADDR(shvNumber), address);
    SET_REG_WORD(SHAVE_CACHE_DBG_TXN_TYPE(shvNumber), ctrlType);

    /// Clear Stop
    lVal = GET_REG_WORD_VAL(SHAVE_CACHE_CTRL(shvNumber));
//    DEBUG_INFO ("SHAVE_CACHE_CTRL = ");
//    ACTUAL_DATA (lVal);
    SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), (lVal & ~stopBit));

    /// Wait for flush/invalidate busy to clear
    do
    {
        NOP; NOP; NOP; NOP; NOP;
        NOP; NOP; NOP; NOP; NOP;

        lVal = GET_REG_WORD_VAL (SHAVE_CACHE_STATUS(shvNumber));
    }
    while ((lVal & 0x2) != 0);
}

/// Configures L1 instruction cache for a range of shaves
///
/// This function assumes that the shaves are stopped. There is no
/// bit available to stop L1 instruction cache
///
///@param [in] firstShave the first shave in the range
///@param [in] lastShave the last shave in the range
///
///@return
///
void DrvSvuInvalidateL1InstrCache(u32 firstShave, u32 lastShave);

/// Performs the required L1 data cache action on a range of shaves
///
///@param [in] firstShave the first shave in the range
///@param [in] lastShave the last shave in the range
///@param [in] action the action to be performed
///  action can have one of the following values:
///    - SVUL1DATACACHE_INVALIDATE_ALL - invalidates the entire cache
///    - SVUL1DATACACHE_FLUSH_ALL - flushes the entire cache
///    - SVUL1DATACACHE_FLUSH_INVALIDATE_ALL - flushes and invalidate the entire cache
///Calling this function will enable the cache if it was not enabled before
///
///@return
///
void DrvSvuFlushInvalidateL1DataCache(u32 firstShave, u32 lastShave, DrvSvuL1DCacheAction_t action);

static inline void DrvSvuSliceResetAll(u32 shaveNumber)
{
    /* Wait until L1 instruction cache is not busy before resetting shave. */
    while(GET_REG_WORD_VAL(SHAVE_L1C_I_STATUS(shaveNumber)) & SVUL1INSTRCACHE_BUSY) {
    }

    SET_REG_WORD(DCU_SVU_SLICE_RST(shaveNumber), ~(SVU_RESET /*| DCU_RESET*/ | SLICE_RESET | /*TLB_RESET |*/
            SLICE_FIFO_RESET /* |L1_CACHE_RESET */)); // Clear all of the slice bits

    // Add the tracer functionality to visualize the shave reset
    dbgLogEvent((Event_t)((u32)LOG_EVENT_SHAVE_0_RESET +  shaveNumber) , 0, DEBUG_LOG_LEVEL_HIGH);
    return;
}

/// Resets the specified SHAVE
/// @param[in] shvNumber - shave number
/// @return    - void
///
static inline void DrvSvuSliceResetOnlySvu(u32 shaveNumber)
{
    /* Wait until L1 instruction cache is not busy before resetting shave. */
    while(GET_REG_WORD_VAL(SHAVE_L1C_I_STATUS(shaveNumber)) & SVUL1INSTRCACHE_BUSY) {
    }

    SET_REG_WORD(DCU_SVU_SLICE_RST(shaveNumber), ~(SLICE_RESET | SVU_RESET /*| DCU_RESET*/)); // Clear only the bits we want to reset

    // Add the tracer functionality to visualize the shave reset
    dbgLogEvent((Event_t)((u32)LOG_EVENT_SHAVE_0_RESET + shaveNumber) , 0, DEBUG_LOG_LEVEL_HIGH);

    return;
}

/// Disable the L1 Instruction Cache in the specified Shave
/// By default the Shave L1 ICache is enabled unless this function
/// is called
/// @param[in] shvNumber - shave number
/// @return    - void
/// @deprecated Use DrvSvuL1InstrCacheCtrl(u32 shvNumber, u32 SVUL1INSTRCACHE_BYPASS) instead.
///
__Deprecated__("Use DrvSvuL1InstrCacheCtrl(u32 shvNumber, u32 SVUL1INSTRCACHE_BYPASS) instead")
static inline void DrvSvuL1IcacheDisable(u32 shaveNumber)
{
    /// Bypass Shave L1 Instruction cache
    SET_REG_WORD(SHAVE_L1C_I_CTRL(shaveNumber), 0x01);
}

#ifdef __cplusplus
}
#endif

#endif //_DRVSVU_H_

///@}
