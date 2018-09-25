//
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt   
///
/// @brief     Key application system settings
/// 
/// 
/// 

// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"
#include "OsDrvCpr.h"
#include "DrvDdr.h"
#include "assert.h"
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include "DrvGpioDefines.h"
#include "DrvGpio.h"
#include "registersMyriad.h"
#include "brdGpioCfgs/brdMv0182R2GpioDefaults.h"
#include "DrvShaveL2Cache.h"
#include "OsDrvTimer.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ 12000 // 20MHz       
      
#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)
#define CLOCKS_MIPICFG (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is require here for downstream tools
u32 __l2_config   __attribute__((section(".l2.mode")))  = L2CACHE_CFG;

// 4: Static Local Data 
// ----------------------------------------------------------------------------
static tyAuxClkDividerCfg auxClk[] =
{
        {
             .auxClockEnableMask     = AUX_CLK_MASK_MEDIA,         // SIPP Clock
             .auxClockSource         = CLK_SRC_SYS_CLK,            //
             .auxClockDivNumerator   = 1,                          //
             .auxClockDivDenominator = 1,                          //
        },
        {
             .auxClockEnableMask     = (AUX_CLK_MASK_LCD),         // LCD Clock
             .auxClockSource         = CLK_SRC_SYS_CLK,            //
             .auxClockDivNumerator   = 1,                          //
             .auxClockDivDenominator = 1,                          //
        },
        {
             .auxClockEnableMask     = CLOCKS_MIPICFG,             // MIPI CFG + ECFG Clock
             .auxClockSource         = CLK_SRC_SYS_CLK,            //
             .auxClockDivNumerator   = 1,                          //
             .auxClockDivDenominator = 18,                         //
        },
        {0, 0, 0, 0}, // Null Terminated List
};


static tySocClockConfig clocksConfig =
{
    .refClk0InputKhz = SYS_CLK_KHZ /* 12 MHz */,
    .refClk1InputKhz = 0 /* refClk1 not enabled for now */,
    .targetPll0FreqKhz = 580000, /* PLL0 target freq = 580 MHz */
    .targetPll1FreqKhz = 0, /* set in DDR driver */
    .clkSrcPll1 = CLK_SRC_REFCLK0, /* refClk1 is also not enabled for now */
    .masterClkDivNumerator = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .upaClockEnableMask = DEFAULT_UPA_CLOCKS,
    .mssClockEnableMask = MSS_CLOCKS, /* MSS clocks enabled */
    .sippClockEnableMask = OPIPE_CLOCKS,
    .pAuxClkCfg = auxClk,
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;

    OsDrvCprInit();
    OsDrvCprOpen();

    DrvCprSysDeviceAction(SIPP_DOMAIN, ASSERT_RESET, DEV_SIPP_MIPI | DEV_SIPP_APB_SLV | DEV_SIPP_MIPI_RX0 | DEV_SIPP_MIPI_RX1 );
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_SIPP | DEV_MSS_AMC | DEV_MSS_LCD);

    OsDrvCprSetupClocks(&clocksConfig);

    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(CSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(SIPP_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, -1);

    OsDrvTimerInit();
    DrvDdrInitialise(NULL);

    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    //Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();

    //Assign the one partition defined to all shaves
    for (i = 0; i < 12; i++)
    {
        DrvShaveL2CacheSetLSUPartId(i, 0);
    }

    // Configure GPIOs
    DrvGpioInitialiseRange(brdMV0182R2GpioCfgDefault);

    return 0;
}
