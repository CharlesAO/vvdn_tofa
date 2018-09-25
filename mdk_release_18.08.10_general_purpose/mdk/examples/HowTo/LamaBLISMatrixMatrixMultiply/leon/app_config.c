///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"
#include "DrvCpr.h"
#include "DrvDdr.h"
#include "assert.h"
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include "DrvGpioDefines.h"
#include "DrvGpio.h"
#include "registersMyriad.h"
#include "brdGpioCfgs/brdMv0182GpioDefaults.h"
#include "DrvShaveL2Cache.h"
#include "DrvTimer.h"
#include "DrvMipi.h"
#include "DrvMipiDefines.h"
#include "DrvMssDefines.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ 12000 // 20MHz
#define PLL0_FREQ 300000
#define APP_UPA_CLOCKS (DEV_UPA_SH0      | \
                         DEV_UPA_SH1      | \
                         DEV_UPA_SH2      | \
                         DEV_UPA_SH3      | \
                         DEV_UPA_SH4      | \
                         DEV_UPA_SH5      | \
                         DEV_UPA_SH6      | \
                         DEV_UPA_SH7      | \
                         DEV_UPA_SH8      | \
                         DEV_UPA_SH9      | \
                         DEV_UPA_SH10     | \
                         DEV_UPA_SH11     | \
                         DEV_UPA_SHAVE_L2 | \
                         DEV_UPA_CDMA     | \
                         DEV_UPA_CTRL         )
#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)
                         
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is require here for downstream tools
u32 __l2_config   __attribute__((section(".l2.mode")))  = L2CACHE_CFG;

// 4: Static Local Data 
// ----------------------------------------------------------------------------

#if (defined MYRIAD2)
static tyAuxClkDividerCfg auxClk[] =
{
        {
            .auxClockEnableMask = AUX_CLK_MASK_LCD | CSS_AUX_MEDIA,
            .auxClockSource = CLK_SRC_SYS_CLK,
            .auxClockDivNumerator = 1,
            .auxClockDivDenominator = 1
        },    // media clock and LCD
        {0, 0, 0, 0}, // Null Terminated List
    };


static tySocClockConfig clocksConfig =
{
    .refClk0InputKhz         = SYS_CLK_KHZ      /* 12 MHz */,
    .refClk1InputKhz         = 0                /* refClk1 not enabled for now */,
    .targetPll0FreqKhz       = PLL0_FREQ,   /* PLL0 target freq = 100 MHz */
    .targetPll1FreqKhz       = 0,               /* set in DDR driver */
    .clkSrcPll1              = CLK_SRC_REFCLK0, /* refClk1 is also not enabled for now */
    .masterClkDivNumerator   = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .upaClockEnableMask      = DEFAULT_UPA_CLOCKS, /* no UPA clocks enabled */
    .mssClockEnableMask      = 0xFFFFFFFF, /* MSS clocks enabled */
    .sippClockEnableMask     = 0,
    .pAuxClkCfg              = auxClk,
};
#endif

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;

    // Configure the system
    DrvCprInit();
    DrvCprSetupClocks(&clocksConfig);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, 0xFFFFFFFF);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, APP_UPA_CLOCKS);

    DrvResetMssGpioLcdMipiLoopbackCfgRegisterValues();

    DrvTimerInit(); //needed to initialize LeonOS timer
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
    DrvGpioInitialiseRange(brdMV0182GpioCfgDefault);

    return 0;
}

