///   
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt   
///
/// @brief     Key application system settings
/// 
/// 
/// 

// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"
#include "DrvCpr.h"
#include "DrvDdr.h"
#include "swcLeonUtils.h"
#include "DrvShaveL2Cache.h"
#include "DrvTimer.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ 12000 // 12MHz

#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)
#define PARTITION_0                (0)
#define SHAVES_USED                (12)

//clock defines
#define AUX_CLOCKS_MIPICFG  (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)
#define AUX_CLOCKS_VIDEO    (AUX_CLK_MASK_MEDIA | AUX_CLK_MASK_LCD)

#define MSS_CLOCKS         (  DEV_MSS_APB_SLV     |     \
                              DEV_MSS_APB2_CTRL   |     \
                              DEV_MSS_RTBRIDGE    |     \
                              DEV_MSS_RTAHB_CTRL  |     \
                              DEV_MSS_LRT         |     \
                              DEV_MSS_LRT_DSU     |     \
                              DEV_MSS_LRT_L2C     |     \
                              DEV_MSS_LRT_ICB     |     \
                              DEV_MSS_AXI_BRIDGE  |     \
                              DEV_MSS_MXI_CTRL    |     \
                              DEV_MSS_MXI_DEFSLV  |     \
                              DEV_MSS_AXI_MON     |     \
                              DEV_MSS_LCD         |     \
                              DEV_MSS_AMC         |     \
                              DEV_MSS_SIPP        |     \
                              DEV_MSS_TIM         )

#define SIPP_CLOCKS         ( DEV_SIPP_SIPP_ABPSLV  | \
                              DEV_SIPP_MIPI         | \
                              DEV_SIPP_MIPI_RX1     )

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
      {0, 0, 0, 0}, // Null Terminated List
    };




static tySocClockConfig clocksConfig =
{
    .refClk0InputKhz         = SYS_CLK_KHZ      /* input oscillator source at 12 MHz */,
    .refClk1InputKhz         = 0,
    .targetPll0FreqKhz       = 360000,          /* system clock at 360 Mhz */
    .targetPll1FreqKhz       = 0,               /* will be set by DDR driver */
    .clkSrcPll1              = CLK_SRC_REFCLK0, /* refClk1 is also not enabled for now */
    .masterClkDivNumerator   = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .mssClockEnableMask      = MSS_CLOCKS,      /* MSS clocks enabled */
    .sippClockEnableMask     = SIPP_CLOCKS,     /* SIPP clocks enabled */
    .upaClockEnableMask      = 0,               /* UPA clocks disabled */
    .pAuxClkCfg              = auxClk,
};


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{
    int i;
    s32 sc;

    // Configure the system
    sc = DrvCprInit();
    if(sc)
        return sc;

    DrvCprSysDeviceAction(SIPP_DOMAIN, ASSERT_RESET, DEV_SIPP_MIPI);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_SIPP | DEV_MSS_AMC | DEV_MSS_LCD );
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET,  DEV_MSS_SIPP );

    sc = DrvCprSetupClocks(&clocksConfig);
    if(sc)
        return sc;

    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(CSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(SIPP_DOMAIN, DEASSERT_RESET, -1);

    swcLeonSetPIL(0);

    DrvDdrInitialise(NULL);

    // Set the shave L2 Cache mode
    sc = DrvShaveL2CacheSetMode(L2CACHE_CFG);
    if(sc)
        return sc;

    //Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);

    //Allocate Shave L2 cache set partitions
    sc = DrvShaveL2CacheAllocateSetPartitions();
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
