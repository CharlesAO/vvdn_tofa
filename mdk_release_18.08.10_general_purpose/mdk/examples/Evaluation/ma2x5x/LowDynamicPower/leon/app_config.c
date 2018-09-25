///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "app_config.h"
#include "registersMyriad.h"
#include "DrvCpr.h"
#include "DrvDdr.h"
#include "DrvTimer.h"
#include "DrvRegUtils.h"
#include "DrvShaveL2Cache.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ 12000 // 12MHz

#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is require here for downstream tools

// 4: Static Local Data
// ----------------------------------------------------------------------------

#define MSS_CLOCKS      (  DEV_MSS_MXI_CTRL  )

#define APP_UPA_CLOCKS  (   DEV_UPA_SHAVE_L2 | \
                            DEV_UPA_CDMA     | \
                            DEV_UPA_CTRL      )

#define CORE_CSS_DSS_CLOCKS   ( DEV_CSS_LOS             |    \
                                DEV_CSS_LAHB_CTRL       |    \
                                DEV_CSS_APB4_CTRL       |    \
                                DEV_CSS_CPR             |    \
                                DEV_CSS_LOS_L2C         |    \
                                DEV_CSS_MAHB_CTRL       |    \
                                DEV_CSS_LOS_DSU         |    \
                                DEV_CSS_LOS_TIM         |    \
                                DEV_CSS_GPIO            |    \
                                DEV_CSS_JTAG            |    \
                                DEV_CSS_APB1_CTRL       |    \
                                DEV_CSS_APB3_CTRL       |    \
                                DEV_CSS_UART            |    \
                                DEV_CSS_SAHB_CTRL       |    \
                                DEV_CSS_MSS_MAS         |    \
                                DEV_CSS_UPA_MAS         |    \
                                DEV_CSS_DSS_BUS_MAHB    |    \
                                DEV_CSS_LAHB2SHB        )
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
tyAuxClkDividerCfg auxClk[] =
{
    {
        .auxClockEnableMask = (u32)(1 << CSS_AUX_TSENS),
        .auxClockSource = CLK_SRC_REFCLK0,
        .auxClockDivNumerator = 1,
        .auxClockDivDenominator = 10,
    },
    {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
    {   0,0,0,0}, // Null Terminated List
};
#else
tyAuxClkDividerCfg auxClk[] =
{
    {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
    {0, 0, 0, 0} // Null Terminated List
};
#endif
tySocClockConfig clockConfig =
{
    .refClk0InputKhz = SYS_CLK_KHZ, // Default 12Mhz input clock
    .refClk1InputKhz = 0, // Assume no secondary oscillator for now
    .targetPll0FreqKhz = 600000,
    .targetPll1FreqKhz = 0, // will be set by DDR driver
    .clkSrcPll1 = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
    .masterClkDivNumerator = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .mssClockEnableMask = MSS_CLOCKS, // Must keep the MXI controller clocked
    .upaClockEnableMask = 0,
    .sippClockEnableMask = 0,
    .pAuxClkCfg = auxClk
};

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void turnOnShavePowerIslands(u32 shaveNumber)
{
    u32 i;

    for (i = POWER_ISLAND_SHAVE_0; i < shaveNumber; i++)
        DrvCprPowerTurnOnIsland(i);
}

void turnOffShavePowerIslands(void)
{
    u32 i;

    for (i = POWER_ISLAND_SHAVE_0; i <= POWER_ISLAND_SHAVE_11; i++)
        DrvCprPowerTurnOffIsland(i);
}

static void turnOffMorePowerIslands(void)
{
    u32 mssClocksToTurnOff = ~MSS_CLOCKS;
    u32 cssClocksToTurnOff = (u32) ~CORE_CSS_DSS_CLOCKS;

    DrvCprSysDeviceAction(MSS_DOMAIN, DISABLE_CLKS, mssClocksToTurnOff);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET, mssClocksToTurnOff);


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_CPU);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_AMC);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_SIPP);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_USB);
#else
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_DIGITAL);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_ANALOG);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS_DIGITAL);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS_ANALOG);
#endif

    DrvCprSysDeviceAction(CSS_DOMAIN, DISABLE_CLKS, cssClocksToTurnOff);
    DrvCprSysDeviceAction(CSS_DOMAIN, ASSERT_RESET, cssClocksToTurnOff);
}

static void reduceBaselinePower(void)
{
    turnOffMorePowerIslands();
}

// 6: Functions Implementation
// ----------------------------------------------------------------------------

/* Turn off extra clocks and start with the bare minimum for the Leon to run
 */
s32 initClocksAndMemory(void)
{
    DrvCprInit();
    DrvCprSetupClocks(&clockConfig);

    reduceBaselinePower();

    return 0;
}

/* Turn on the minimum necessary clocks for Shaves processing out of DDR
 */
void powerUpShavesAndDDR(void)
{
    s32 i;

    DrvDdrInitialise(NULL);
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_CPU);
#else
    DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_DIGITAL);
#endif
    DrvCprSysDeviceAction(UPA_DOMAIN, ENABLE_CLKS, APP_UPA_CLOCKS);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, APP_UPA_CLOCKS);

    DrvCprSysDeviceAction(MSS_DOMAIN, ENABLE_CLKS, MSS_CLOCKS);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, MSS_CLOCKS);

    // Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    // Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();

    // Assign the only partition defined to all Shaves
    for (i = 0; i < SHAVES_USED; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, PARTITION_0);
    }
}

