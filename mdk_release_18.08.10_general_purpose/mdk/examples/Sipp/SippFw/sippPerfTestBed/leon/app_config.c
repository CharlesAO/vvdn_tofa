///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <sipp.h>
#include "app_config.h"
#include <stdio.h>
#include <string.h> //memcpy
#include <DrvCpr.h>
#include <DrvDdr.h>
#include <DrvLeonL2C.h>
#include "swcLeonUtils.h"
#include "UnitTestApi.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MSS_CLOCKS_APP   (DEV_MSS_APB_SLV     |     \
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
                          DEV_MSS_AMC         |     \
                          DEV_MSS_SIPP        |     \
                          DEV_MSS_TIM         )

#define OPIPE_CLOCKS (DEV_SIPP_SIGMA          |   \
                      DEV_SIPP_LSC            |   \
                      DEV_SIPP_RAW            |   \
                      DEV_SIPP_DBYR           |   \
                      DEV_SIPP_DOGL           |   \
                      DEV_SIPP_LUMA           |   \
                      DEV_SIPP_SHARPEN        |   \
                      DEV_SIPP_CGEN           |   \
                      DEV_SIPP_MED            |   \
                      DEV_SIPP_CHROMA         |   \
                      DEV_SIPP_CC             |   \
                      DEV_SIPP_LUT            |   \
                      DEV_SIPP_EDGE_OP        |   \
                      DEV_SIPP_CONV           |   \
                      DEV_SIPP_HARRIS         |   \
                      DEV_SIPP_UPFIRDN0       |   \
                      DEV_SIPP_UPFIRDN1       |   \
                      DEV_SIPP_UPFIRDN2       |   \
                      DEV_SIPP_MIPI_RX0       |   \
                      DEV_MSS_SIPP_CLK_GATES  |   \
                      DEV_SIPP_MIPI           |   \
                      DEV_SIPP_SIPP_ABPSLV    |   \
                      DEV_SIPP_APB_SLV)

// 3: Global Data (Only if absolutely necessary)

void reduceBaselinePower()
{
    DrvCprPowerTurnOffIsland(POWER_ISLAND_USB);

    u32 upaClocksToTurnOff = ~(0
            | DEV_UPA_SH0
            | DEV_UPA_SH1
            | DEV_UPA_SH2
            | DEV_UPA_SH3
            | DEV_UPA_SH4
            | DEV_UPA_SH5
            | DEV_UPA_SH6
            | DEV_UPA_SH7
            | DEV_UPA_SH8
            | DEV_UPA_SH9
            | DEV_UPA_SH10
            | DEV_UPA_SH11
            | DEV_UPA_SHAVE_L2
            | DEV_UPA_CDMA
            | DEV_UPA_CTRL
            | (1 << UPA_MTX)
            );
    u32 auxClocksToTurnOff = ~(0
            | AUX_CLK_MASK_DDR_REF
            | AUX_CLK_MASK_DDR_CORE_CTRL
            | AUX_CLK_MASK_DDR_CORE_PHY
            | AUX_CLK_MASK_UART
            | (1<< CSS_AUX_TSENS)
            );
    u32 mssClocksToTurnOff = 0
            | DEV_MSS_AXI_BRIDGE
            | DEV_MSS_MXI_DEFSLV
            | DEV_MSS_AXI_MON
            | DEV_MSS_NAL
            | DEV_MSS_CIF0
            | DEV_MSS_CIF1
            | DEV_MSS_LCD
            ;
    u64 cssClocksToTurnOff = ~(0
          | DEV_CSS_AON
          | DEV_CSS_DSS_BUS
          | DEV_CSS_DSS_BUS_AAXI
          | DEV_CSS_LAHB2SHB
          | DEV_CSS_SAHB2MAHB
          | DEV_CSS_LAHB_CTRL
          | DEV_CSS_APB4_CTRL
          | DEV_CSS_MAHB_CTRL
          | DEV_CSS_APB1_CTRL
          | DEV_CSS_APB3_CTRL
          | DEV_CSS_SAHB_CTRL
          | DEV_CSS_UART
          | DEV_CSS_JTAG
          | DEV_CSS_LOS_L2C
          | DEV_CSS_MSS_MAS
          | DEV_CSS_UPA_MAS
          | DEV_CSS_I2C2
          | DEV_CSS_GPIO
          );

    // Turn off unneeded clocks
    DrvCprSysDeviceAction(UPA_DOMAIN,     DISABLE_CLKS, upaClocksToTurnOff);
    DrvCprSysDeviceAction(UPA_DOMAIN,     ASSERT_RESET, upaClocksToTurnOff);
    DrvCprSysDeviceAction(MSS_DOMAIN,     DISABLE_CLKS, mssClocksToTurnOff);
    DrvCprSysDeviceAction(MSS_DOMAIN,     ASSERT_RESET, mssClocksToTurnOff);
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, DISABLE_CLKS, auxClocksToTurnOff);
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, ASSERT_RESET, auxClocksToTurnOff);
    DrvCprSysDeviceAction(CSS_DOMAIN,     DISABLE_CLKS, cssClocksToTurnOff);

    // Hardcoded because the mask is not yet computed here
    //SET_REG_WORD(MSS_SIPP_CLK_CTRL_ADR, 0x18FFF | (1<<MSS_SP_SIPP_ABPSLV));
}


s32 initClocksAndMemory(void)
{
    s32 sc;

    tyAuxClkDividerCfg auxClkCfg[] =
    {    {
        .auxClockEnableMask = (u32)(1 << CSS_AUX_TSENS),
        .auxClockSource = CLK_SRC_REFCLK0,
        .auxClockDivNumerator = 1,
        .auxClockDivDenominator = 10,
    },
        {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
        {0,0,0,0}, // Null Terminated List
    };

    tySocClockConfig clockConfig =
    {
        .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
        .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
        .targetPll0FreqKhz       = 600000,
        .targetPll1FreqKhz       = 0,
        .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
        .masterClkDivNumerator   = 1,
        .masterClkDivDenominator = 1,
        .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
        .mssClockEnableMask      = MSS_CLOCKS_APP,             // Basic clocks to allow LRT and busses to function
        .sippClockEnableMask     = OPIPE_CLOCKS,               // Opipe clocks
        .upaClockEnableMask      = DEFAULT_UPA_CLOCKS,         // Enable all Shave clocks, CMXDMA, Shave L2Cache and UPA Control interfaces by default
        .pAuxClkCfg              = auxClkCfg,
    };

    DrvCprStartAllClocks();
    DrvLL2CInitWriteThrough();
    swcLeonSetPIL(0);        // Enable all interrupts

    sc = DrvCprInit();
    if(sc)
        return sc;
    sc = DrvCprSetupClocks(&clockConfig);
    if(sc)
        return sc;

    DrvDdrInitialise(NULL);
    //reduceBaselinePower();   // Disable unneeded clocks and power islands

    return 0;
}
