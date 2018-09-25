///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Key application system settings
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "los_app_config.h"

#include "DrvDdr.h"
#include "assert.h"

#include "DrvShaveL2Cache.h"
#include "OsDrvTimer.h"
#include "OsDrvCpr.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
          
#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)
#define PARTITION_0                (0)
#define SHAVES_USED                (12)

#define AUX_CLOCKS_MIPICFG (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)
#define AUX_CLOCKS_VIDEO   (AUX_CLK_MASK_MEDIA | AUX_CLK_MASK_LCD)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

const tyAuxClkDividerCfg auxClk[] =
{
      {
          .auxClockEnableMask     = AUX_CLOCKS_VIDEO,        // SIPP + LCD Clock
          .auxClockSource         = CLK_SRC_SYS_CLK_DIV2,
          .auxClockDivNumerator   = 1,
          .auxClockDivDenominator = 1,
      },
      {
          .auxClockEnableMask     = AUX_CLK_MASK_CIF1,       // CIF1 Clock
          .auxClockSource         = CLK_SRC_SYS_CLK,
          .auxClockDivNumerator   = 1,
          .auxClockDivDenominator = 1,
      },
      {
          .auxClockEnableMask     = AUX_CLOCKS_MIPICFG,      // MIPI CFG + ECFG Clock
          .auxClockSource         = CLK_SRC_SYS_CLK,
          .auxClockDivNumerator   = 1,
          .auxClockDivDenominator = 24,                      // the MIPI cfg clock should be <= 20 Mhz !
      },
      {0, 0, 0, 0}, // Null Terminated List
    };

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;
    s32 sc;
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

    sc = OsDrvTimerInit();
    if(sc)
        return sc;
    DrvDdrInitialise(NULL);

    // Set the shave L2 Cache mode
    sc = DrvShaveL2CacheSetMode(L2CACHE_CFG);
    if(sc)
        return sc;

    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    //Allocate Shave L2 cache set partitions
    sc  = DrvShaveL2CacheAllocateSetPartitions();
    if(sc)
        return sc;

    //Assign the one partition defined to all shaves
    for (i = 0; i < SHAVES_USED; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, PARTITION_0);
    }

    DrvShaveL2CachePartitionFlushAndInvalidate(PARTITION_0);

    return 0;
}
