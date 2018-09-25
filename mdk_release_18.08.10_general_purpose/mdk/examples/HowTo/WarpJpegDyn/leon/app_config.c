///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Key application system settings
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "app_config.h"
#include "DrvDdr.h"
#include "swcLeonUtils.h"
#include "DrvShaveL2Cache.h"
#include "DrvTimer.h"
#include "DrvLeonL2C.h"
#include <DrvSipp.h>
#include <OsDrvCpr.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)
#define SHAVES_USED                (12)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is require here for downstream tools

// 4: Static Local Data
// ----------------------------------------------------------------------------

static tyAuxClkDividerCfg auxClk[] =
{
    {
        .auxClockEnableMask     = AUX_CLOCKS_VIDEO,           // SIPP + LCD Clocks
        .auxClockSource         = CLK_SRC_SYS_CLK_DIV2 ,      //
        .auxClockDivNumerator   = 1,                          //
        .auxClockDivDenominator = 1,                          //
    },
    {
        .auxClockEnableMask     = AUX_CLOCKS_MIPICFG,         // MIPI CFG + ECFG Clocks
        .auxClockSource         = CLK_SRC_SYS_CLK     ,       //
        .auxClockDivNumerator   = 1,                          //
        .auxClockDivDenominator = 18,                         //
    },
	{
		.auxClockEnableMask	 	= AUX_CLK_MASK_SDIO,		  // SDIO
		.auxClockSource			= CLK_SRC_PLL0,
		.auxClockDivNumerator	= 1,
		.auxClockDivDenominator	= 8,
	},
    {0, 0, 0, 0}, // Null Terminated List
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void blocksResetSiliconSpecific(void)
{
    DrvCprSysDeviceAction(SIPP_DOMAIN, ASSERT_RESET, DEV_SIPP_MIPI);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_SIPP | DEV_MSS_AMC | DEV_MSS_LCD );
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(CSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(SIPP_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, -1);
}

int initClocksAndMemory(void)
{
    int i;
    s32 sc;
    int partitionNumber[SHAVES_USED];

    // Configure the system
    sc = OsDrvCprInit();
    if(sc)
        return sc;
    sc = OsDrvCprOpen();
    if(sc)
    	return sc;
    sc = OsDrvCprAuxClockArrayConfig(auxClk);
    if(sc)
        return sc;
    blocksResetSiliconSpecific();
    // Enable clock MSS_SIPP_CLK_CTRL_ADR from media subsystem
    DrvSippEnableClk();

    DrvDdrInitialise(NULL);

    // Set the shave L2 Cache mode
    sc = DrvShaveL2CacheSetMode(L2CACHE_CFG);
    if(sc)
        return sc;

    // Invalidate entire L2Cache
    DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, /*disable cache?:*/ 0);

    //Set Shave L2 cache partitions
    // Initialize partition numbers allocated.
    partitionNumber[0] = DrvShaveL2CacheSetupPartition(SHAVEPART128KB);
    partitionNumber[1] = DrvShaveL2CacheSetupPartition(SHAVEPART64KB);
    partitionNumber[2] = DrvShaveL2CacheSetupPartition(SHAVEPART32KB);
    partitionNumber[3] = DrvShaveL2CacheSetupPartition(SHAVEPART32KB);

    //Allocate Shave L2 cache set partitions
    sc = DrvShaveL2CacheAllocateSetPartitions();
    if(sc)
        return sc;

    //Assign separate partition for data and instruction for shave's
    for(i = 0; i < SHAVES_USED; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, partitionNumber[0]);
        DrvShaveL2CacheSetInstrPartId(i, partitionNumber[3]);
        DrvShaveL2CacheSetWinPartId(i,SHAVEL2CACHEWIN_B,partitionNumber[1]); // win E used by SDL for dyn .textcrit data 0x1D...
        DrvShaveL2CacheSetWinPartId(i,SHAVEL2CACHEWIN_C,partitionNumber[2]); // win F used by SDL for common .data from DDR 0x1E...
    }

    DrvShaveL2CachePartitionFlushAndInvalidate(partitionNumber[0]);
    DrvShaveL2CachePartitionFlushAndInvalidate(partitionNumber[2]);

    return 0;
}
