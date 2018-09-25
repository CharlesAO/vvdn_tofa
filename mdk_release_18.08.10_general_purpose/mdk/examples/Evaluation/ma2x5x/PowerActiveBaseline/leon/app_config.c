///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvDdr.h>
#include <DrvTimer.h>
#include "app_config.h"

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include <DrvTempSensor.h>
#endif
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)

#define MSS_CLOCKS_BASIC DEV_MSS_MXI_CTRL
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is required here for downstream tools

#define UPA_CLOCKS              (DEV_UPA_CTRL     )



// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static void turnOffShavePowerIslands(void)
{
    u32 i;

    for (i = 0; i < 12; i++)
    {
        DrvCprPowerTurnOffIsland(POWER_ISLAND_SHAVE_0 + i);
    }
}

static void turnOffMorePowerIslands(void)
{
    u32 mssClocksToTurnOff = ~MSS_CLOCKS_BASIC;

    DrvCprSysDeviceAction(MSS_DOMAIN, DISABLE_CLKS, mssClocksToTurnOff);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET, mssClocksToTurnOff);

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_AMC);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_SIPP);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_CPU);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_USB);
#else
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_DIGITAL);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_ANALOG);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS_DIGITAL);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS_ANALOG);
#endif
}

/* Reduce the baseline power to the absolute minimum achievable for a demo
 * (have UART, GPIO, JTAG on)
 */
static void reduceBaselinePower(void)
{
    turnOffShavePowerIslands();
    turnOffMorePowerIslands();
}

// 6: Functions Implementation
// ----------------------------------------------------------------------------
// Setup all the clock configurations needed by this application and also the ddr
int initClocksAndMemory(void)
{

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    DrvTempSensConfig tempParam = {1};
    tyAuxClkDividerCfg auxClk[] =
    {
        {
            .auxClockEnableMask = (u32)(1 << CSS_AUX_TSENS),
            .auxClockSource = CLK_SRC_REFCLK0,
            .auxClockDivNumerator = 1,
            .auxClockDivDenominator = 10,
        },
        {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
        {0, 0, 0, 0} // Null Terminated List
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
        .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
        .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
        .targetPll0FreqKhz       = 492000,          // should be multiple of 12000
        .targetPll1FreqKhz       = 0,               // will be set by DDR driver
        .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
        .masterClkDivNumerator   = 1,
        .masterClkDivDenominator = 1,
        .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
        .mssClockEnableMask      = MSS_CLOCKS_BASIC,      // Enabling minimal set of MSS clocks
        .upaClockEnableMask      = UPA_CLOCKS,
        .sippClockEnableMask     = 0,
        .pAuxClkCfg              = auxClk
    };

    DrvCprInit();
    DrvCprSetupClocks(&clockConfig);

    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, 0x0000ffff);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, 0x0001ffff);

    reduceBaselinePower();

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    DrvTempSensorInitialise(&tempParam);
#endif

    return 0;
}

void turnOnDDR(void)
{
    DrvDdrInitialise(NULL);
}

void turnOnDmaClocks(void)
{
    u32 mssClocksToTurnOn = ~(DEV_MSS_MXI_CTRL);
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_CPU);
    DrvCprSysDeviceAction(MSS_DOMAIN, DISABLE_CLKS, mssClocksToTurnOn);
    DrvCprSysDeviceAction(MSS_DOMAIN, ENABLE_CLKS, ~mssClocksToTurnOn);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET, mssClocksToTurnOn & ~7);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, ~mssClocksToTurnOn);
#else
    DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_DIGITAL);
    DrvCprSysDeviceAction(MSS_DOMAIN, DISABLE_CLKS, mssClocksToTurnOn);
    DrvCprSysDeviceAction(MSS_DOMAIN, ENABLE_CLKS, ~mssClocksToTurnOn);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET, mssClocksToTurnOn & ~7);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, ~mssClocksToTurnOn);
#endif
    DrvCprSysDeviceAction(UPA_DOMAIN, ENABLE_CLKS, ( DEV_UPA_SHAVE_L2 | DEV_UPA_CDMA) );

}
