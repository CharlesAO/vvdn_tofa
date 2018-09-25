///
/// @file DrvSvu.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvSvu
/// @{
/// @brief     Shave low level functionality
///
/// Driver for Myriad2 Shave low level control.
///

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvSvu.h>

//#define DRV_SVU_DEBUG

#ifdef DRV_SVU_DEBUG
#define DPRINTF1(...) printf(__VA_ARGS__)
#else
#define DPRINTF1(...)
#endif

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

/// Base addresses of the first 12 slices in CMX memory (normal view, cacheable)
u32 SLICE_BASE_ADDR[TOTAL_NUM_SHAVES] =
{
    CMX_SLICE_0_BASE_ADR,
    CMX_SLICE_1_BASE_ADR,
    CMX_SLICE_2_BASE_ADR,
    CMX_SLICE_3_BASE_ADR,
    CMX_SLICE_4_BASE_ADR,
    CMX_SLICE_5_BASE_ADR,
    CMX_SLICE_6_BASE_ADR,
    CMX_SLICE_7_BASE_ADR,
    CMX_SLICE_8_BASE_ADR,
    CMX_SLICE_9_BASE_ADR,
    CMX_SLICE_10_BASE_ADR,
    CMX_SLICE_11_BASE_ADR
};

/// Base addresses of the 12 Shaves in Myriad2
u32 SVU_CTRL_ADDR[TOTAL_NUM_SHAVES] =
{
    SHAVE_0_BASE_ADR,
    SHAVE_1_BASE_ADR,
    SHAVE_2_BASE_ADR,
    SHAVE_3_BASE_ADR,
    SHAVE_4_BASE_ADR,
    SHAVE_5_BASE_ADR,
    SHAVE_6_BASE_ADR,
    SHAVE_7_BASE_ADR,
    SHAVE_8_BASE_ADR,
    SHAVE_9_BASE_ADR,
    SHAVE_10_BASE_ADR,
    SHAVE_11_BASE_ADR
};

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

// Invalidate L1 Cache
void DrvSvuInvalidateL1InstrCache(u32 firstShave, u32 lastShave)
{
    u32 svu;

    for(svu = firstShave; svu <= lastShave; svu++)
    {
        //wait for busy to be cleared as there might be pending reads
        while(GET_REG_WORD_VAL(SHAVE_CACHE_STATUS(svu)) & CACHE_STATUS_BUSY_FLUSH_INVAL);

        //Invalidate (invalidate takes 1 cc so no need to wait)
        SET_REG_WORD(SHAVE_L1C_I_CTRL(svu), SVUL1INSTRCACHE_INVALIDATE);
    }
}

void DrvSvuFlushInvalidateL1DataCache(u32 firstShave, u32 lastShave, DrvSvuL1DCacheAction_t action)
{
    u32 svu, regVal;

    // we don't allow all actions
    assert(action == SVUL1DATACACHE_INVALIDATE_ALL ||
           action == SVUL1DATACACHE_FLUSH_ALL      ||
           action == SVUL1DATACACHE_FLUSH_INVALIDATE_ALL);
    switch (action)
    {
    case SVUL1DATACACHE_INVALIDATE_ALL:
        action = CACHE_TXN_INV_ALL;
        break;
    case SVUL1DATACACHE_FLUSH_ALL:
        action = CACHE_TXN_FLUSH_ALL;
        break;
    case SVUL1DATACACHE_FLUSH_INVALIDATE_ALL:
        action = CACHE_TXN_WB_INV_ALL;
        break;
    default:
        break;
    }
    // Stop all L1 caches
    for(svu = firstShave; svu <= lastShave; svu++)
    { //Set stop bit
        SET_REG_WORD(SHAVE_CACHE_CTRL(svu), CACHE_CTRL_STOP | CACHE_CTRL_ENABLE);
    }

    // Wait for all L1 caches to stop :either due to FlushInv(bit1), or data transfers(bit0)
    for(svu = firstShave; svu <= lastShave; svu++)
    {
        //Wait while busy
        while(GET_REG_WORD_VAL(SHAVE_CACHE_STATUS(svu)) &
                (CACHE_STATUS_BUSY_FLUSH_INVAL | CACHE_STATUS_BUSY_DATA));
        // Perform cache operation
        SET_REG_WORD(SHAVE_CACHE_DBG_TXN_TYPE(svu), action);
    }

    // Wait for all invalidation to finish:
    for(svu = firstShave; svu <= lastShave; svu++)
    {
        //Wait while busy
        while(GET_REG_WORD_VAL(SHAVE_CACHE_STATUS(svu)) &
                (CACHE_STATUS_BUSY_FLUSH_INVAL | CACHE_STATUS_BUSY_DATA));
    }

    // Start all L1 caches
    for(svu = firstShave; svu <= lastShave; svu++)
    {
        regVal = GET_REG_WORD_VAL(SLICE_NWN_CPC(svu));
        regVal |= SVUL1DATACACHE_CACHE_L2_CACHED;   //LSU enable DDRL2C L1 caching
        regVal |= SVUL1DATACACHE_CACHE_L2_BYPASSED; //LSU enable DDRL2B L1 caching
        SET_REG_WORD(SLICE_NWN_CPC(svu), regVal);
        SET_REG_WORD(SHAVE_CACHE_CTRL(svu), CACHE_CTRL_ENABLE);
    }
}

// Invalidate L1 Cache
void DrvSvuInvalidateL1Cache(u32 shvNumber)
{
    DrvSvuL1DataCacheCtrl(shvNumber, SVUL1DATACACHE_INVALIDATE_ALL);
}

void DrvSvuL1DataCacheCtrl(u32 shvNumber, DrvSvuL1DCacheAction_t action)
{
    u32 regVal, cacheOperation = 0;

    switch(action)
    {
        case SVUL1DATACACHE_ENABLE:
            /// Set non-windowed (L1) cache policy configuration for
            /// LSU DDRL2Cache/Bypass accesses

            /// LSU ENABLE DDRL2C/B L1 CACHING & L1 CMX EN
            SET_REG_WORD(SLICE_NWN_CPC(shvNumber),  0x700);

            /// Invalidate and enable L1 Cache
            /// ENABLE (STOPPED)
            SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), CACHE_CTRL_ENABLE | CACHE_CTRL_STOP);
            /// INVALIDATE
            SET_REG_WORD(SHAVE_CACHE_DBG_TXN_TYPE(shvNumber), CACHE_TXN_INV_ALL);

            do
            { NOP; }
            while (GET_REG_WORD_VAL(SHAVE_CACHE_STATUS(shvNumber)) != 0x0);     /// BUSY
            SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), CACHE_CTRL_ENABLE);       /// ENABLE
            break;

        case SVUL1DATACACHE_DISABLE:
            SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), 0);       /// Disable
            break;

        case SVUL1DATACACHE_STOP:
            /// ENABLE (STOPPED)
            SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), CACHE_CTRL_ENABLE | CACHE_CTRL_STOP);
            break;

        /// Not sure if this functionality is supported by Leon.
        /// CACHE_SVU_CTRL register seems to be controlled by Shave only. To be tested.
        // TBD
        //case SVUL1DATACACHE_BYPASS:
            //SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), CACHE_CTRL_ENABLE | CACHE_CTRL_STOP); // ENABLE (STOPPED)
            //SET_REG_WORD(SHAVE_CACHE_SVU_CTRL(shvNumber), CACHE_SVU_CTRL_BYPASS);
            //break;

        case SVUL1DATACACHE_INVALIDATE_ALL:
            if (action == SVUL1DATACACHE_INVALIDATE_ALL)
                cacheOperation = CACHE_TXN_INV_ALL;

        case SVUL1DATACACHE_FLUSH_ALL:
            if (action == SVUL1DATACACHE_FLUSH_ALL)
                cacheOperation = CACHE_TXN_FLUSH_ALL;

        case SVUL1DATACACHE_FLUSH_INVALIDATE_ALL:
            if (action == SVUL1DATACACHE_FLUSH_INVALIDATE_ALL)
                cacheOperation = CACHE_TXN_WB_INV_ALL;

            /// Set STOP bit in SHAVE_CACHE_CTRL register
            regVal = GET_REG_WORD_VAL(SHAVE_CACHE_CTRL(shvNumber));
            SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), (regVal | CACHE_CTRL_STOP));

            /// Invalidate / Flush / Flush & Invalidate all the cache
            SET_REG_WORD(SHAVE_CACHE_DBG_TXN_TYPE(shvNumber), cacheOperation);

            /// Wait for L1 cache operation to finish
            do
            { NOP; }
            while (GET_REG_WORD_VAL(SHAVE_CACHE_STATUS(shvNumber)) != 0x0);     /// BUSY

            /// Clear STOP bit in SHAVE_CACHE_CTRL register
            regVal = GET_REG_WORD_VAL(SHAVE_CACHE_CTRL(shvNumber));
            SET_REG_WORD(SHAVE_CACHE_CTRL(shvNumber), (regVal & ~CACHE_CTRL_STOP));

            break;

        default:
            break;
    }
}

void DrvSvuL1InstrCacheCtrl(u32 shvNumber, u32 action)
{
    SET_REG_WORD(SHAVE_L1C_I_CTRL(shvNumber), action);
}

void DrvSvutZeroRfs(u32 svu)
{
    u32 i, k;

    for (i = 0; i < 32; i++)
    {
        DrvSvutIrfWrite(svu, i, 0x00000000);
        for (k = 0; k < 4; k++)
        {
            DrvSvutVrfWrite(svu, i, k, 0x00000000);
        }
    }
}

#include <stdio.h>

void DrvSvuL1InstrCacheDump(u32 shv)
{
    u32 regVal;
    u32 set, way;
    u32 __attribute__((unused)) tag; // avoid 'unused variable' error from scan build
    u32 dat[4]; UNUSED(dat);

    /// Shave L1 Instruction cache cannot enter debug mode while busy
    /// Wait cache for pending requests to memory
    do
    { NOP; }
    while (GET_REG_WORD_VAL(SHAVE_L1C_I_STATUS(shv)) != 0x0);

    /// Set bit CACHE_DBG in SHAVE_DBG register
    /// This will allow Leon OS to read/write L1 cache tag/cache memory
    regVal = GET_REG_WORD_VAL(DCU_SVU_SLICE_DBG(shv));
    SET_REG_WORD(DCU_SVU_SLICE_DBG(shv), regVal | L1C_CACHE_DBG);

    /// Dump Shave L1 Instruction cache contents
    DPRINTF1("\n --- Dump Shave #%d L1 Instruction cache contents:\n", shv);
    DPRINTF1("-----------------------------------------------------------------------\n");
    DPRINTF1("|     Address    | {MRU,Val}   Dat0       Dat1       Dat2       Dat3   \n");
    DPRINTF1("-----------------------------------------------------------------------\n");

    for (set = 0; set < 64; set++)
    {
        for (way = 0; way < 2; way++)
        {
            u32 w;

            tag = GET_REG_WORD_VAL(SHAVE_L1C_I_TAG(shv, set, way));
            for (w = 0; w < 4; w++)
                dat[w] = GET_REG_WORD_VAL(SHAVE_L1C_I_DAT(shv, set, way, w));
            DPRINTF1("| addr 0x%08X: { %c, %c } 0x%08X 0x%08X 0x%08X 0x%08X\n",
                   tag << 10 | set << 4,                // Assemble the address
                   (tag & (1 << 23)) ? 'M' : 'L',       // Tag most recently used?
                   (tag & (1 << 22)) ? 'V' : '-',       // Tag valid?
                   dat[0], dat[1], dat[2], dat[3]);     // L1 Instruction cache Data
        }
    }
    DPRINTF1("-----------------------------------------------------------------------\n");

    /// Reset bit CACHE_DBG in SHAVE_DBG register
    regVal = GET_REG_WORD_VAL(DCU_SVU_SLICE_DBG(shv));
    SET_REG_WORD(DCU_SVU_SLICE_DBG(shv), regVal & ~L1C_CACHE_DBG);
}

#pragma GCC diagnostic pop

///@}
