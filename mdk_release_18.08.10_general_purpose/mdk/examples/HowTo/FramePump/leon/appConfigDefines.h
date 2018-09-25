///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Definitions and types needed by the board driver
///
/// This header contains all necessary hardware defined constants for this board
///

#ifndef APP_CONFIG_DEF_H
#define APP_CONFIG_DEF_H

#include <DrvCpr.h>

// 1: Defines
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// I2C
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// General CLOCKS definition
// ----------------------------------------------------------------------------

// #define DESPERATE_CLOCKS

#ifndef DESPERATE_CLOCKS

#define CSS_LOS_CLOCKS     (                                    \
                            DEFAULT_CORE_CSS_DSS_CLOCKS  |      \
                            DEV_CSS_LAHB_CTRL            |      \
                            DEV_CSS_APB4_CTRL            |      \
                            DEV_CSS_CPR                  |      \
                            DEV_CSS_ROM                  |      \
                            DEV_CSS_LOS_L2C              |      \
                            DEV_CSS_MAHB_CTRL            |      \
                            DEV_CSS_LOS_ICB              |      \
                            DEV_CSS_LOS_DSU              |      \
                            DEV_CSS_LOS_TIM              |      \
                            DEV_CSS_APB1_CTRL            |      \
                            DEV_CSS_APB3_CTRL            |      \
                            DEV_CSS_SAHB_CTRL            |      \
                            DEV_CSS_MSS_MAS              |      \
                            DEV_CSS_UPA_MAS              |      \
                            DEV_CSS_LAHB2SHB             |      \
                            DEV_CSS_SAHB2MAHB            |      \
                            DEV_CSS_I2C0                 |      \
                            DEV_CSS_I2C1                 |      \
                            DEV_CSS_I2C2                 )

#define MSS_CLOCKS         (                                    \
                            DEV_MSS_APB_SLV              |      \
                            DEV_MSS_APB2_CTRL            |      \
                            DEV_MSS_RTBRIDGE             |      \
                            DEV_MSS_RTAHB_CTRL           |      \
                            DEV_MSS_LRT                  |      \
                            DEV_MSS_LRT_DSU              |      \
                            DEV_MSS_LRT_L2C              |      \
                            DEV_MSS_LRT_ICB              |      \
                            DEV_MSS_AXI_BRIDGE           |      \
                            DEV_MSS_MXI_CTRL             |      \
                            DEV_MSS_MXI_DEFSLV           |      \
                            DEV_MSS_AXI_MON              |      \
                            DEV_MSS_MIPI                 |      \
                            DEV_MSS_CIF0                 |      \
                            DEV_MSS_CIF1                 |      \
                            DEV_MSS_AMC                  |      \
                            DEV_MSS_SIPP                 |      \
                            DEV_MSS_TIM)

#define SIPP_CLOCKS        (                                    \
                            DEV_SIPP_MIPI_RX0            |      \
                            DEV_SIPP_MIPI_RX1            |      \
                            DEV_SIPP_MIPI_RX2            |      \
                            DEV_SIPP_MIPI_RX3            |      \
                            DEV_SIPP_MIPI                |      \
                            DEV_SIPP_SIPP_ABPSLV         |      \
                            DEV_SIPP_APB_SLV             )

#define UPA_CLOCKS         (                                    \
                            DEV_UPA_SHAVE_L2             |      \
                            DEV_UPA_CDMA                 |      \
                            DEV_UPA_CTRL                 )
#else
#    define CSS_LOS_CLOCKS  (0xFFFFFFFFFFFFFFFFULL)
#    define MSS_CLOCKS      (0xFFFFFFFFUL)
#    define SIPP_CLOCKS     (0xFFFFFFFFUL)
#    define UPA_CLOCKS      (0xFFFFFFFFUL)
#endif

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
#define APP_CLOCK_KHZ       (600000)

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------
static const tyAuxClkDividerCfg auxClkConfig[] = {
    {
        .auxClockEnableMask     = AUX_CLK_MASK_MEDIA | AUX_CLK_MASK_CIF0 | AUX_CLK_MASK_CIF1,
        .auxClockSource         = CLK_SRC_SYS_CLK_DIV2,
        .auxClockDivNumerator   = 1,
        .auxClockDivDenominator = 1,
    },
    {
        .auxClockEnableMask     = AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG,
        .auxClockSource         = CLK_SRC_SYS_CLK_DIV2,
        .auxClockDivNumerator   = 1,
        .auxClockDivDenominator = 14,           // Gives 20MHz clock
    },
    {0,0,0,0}, // Null Terminated List
};

static const tySocClockConfig appClockConfig = {
    .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
    .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
    .targetPll0FreqKhz       = APP_CLOCK_KHZ,
    .targetPll1FreqKhz       = 0,               // set in DDR driver
    .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
    .masterClkDivNumerator   = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask   = CSS_LOS_CLOCKS,
    .mssClockEnableMask      = MSS_CLOCKS,
    .sippClockEnableMask     = SIPP_CLOCKS,
    .upaClockEnableMask      = UPA_CLOCKS,
    .pAuxClkCfg              = auxClkConfig,
};

#endif // APP_CONFIG_DEF_H
