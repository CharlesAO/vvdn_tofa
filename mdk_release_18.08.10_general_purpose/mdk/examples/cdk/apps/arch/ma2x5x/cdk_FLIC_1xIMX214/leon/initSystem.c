/******************************************************************************

 @File         : initSystem.c
 @Author       : MT
 @Brief        : IPIPE Hw configuration on Los side
 Date          : 02 - Sep - 2014
 E-mail        : xxx.xx@movidius.com
 Copyright     : � Movidius Srl 2013, � Movidius Ltd 2014

 Description :


******************************************************************************/


/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <OsDrvCpr.h>
#include <DrvGpio.h>
#include <OsDrvTimer.h>
#include <DrvShaveL2Cache.h>
#include "initSystem.h"
#include <brdDefines.h>
#include "brdGpioCfgs/brdMv0182GpioDefaults.h"


/**************************************************************************************************
 ~~~  Specific #defines and types (typedef,enum,struct)
**************************************************************************************************/
#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)

#define CLOCKS_MIPICFG (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)
#define REM_CLOCKS        (    AUX_CLK_MASK_I2S0  |     \
                                AUX_CLK_MASK_I2S1  |     \
                                AUX_CLK_MASK_I2S2  )

#define DESIRED_USB_FREQ_KHZ        (20000)
#if (DEFAULT_APP_CLOCK_KHZ%DESIRED_USB_FREQ_KHZ)
#error "Can not achieve USB Reference frequency. Aborting."
#endif

/**************************************************************************************************
 ~~~  Global Data (Only if absolutely necessary)
**************************************************************************************************/

/**************************************************************************************************
 ~~~  Static Local Data
 **************************************************************************************************/
tyAuxClkDividerCfg auxClkAllOn[] =
{
    {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
    {
        .auxClockEnableMask     = REM_CLOCKS,                // all rest of the necessary clocks
        .auxClockSource         = CLK_SRC_SYS_CLK,            //
        .auxClockDivNumerator   = 1,                          //
        .auxClockDivDenominator = 1,                          //
    },
    {
        .auxClockEnableMask     = AUX_CLK_MASK_LCD ,          // LCD clock
        .auxClockSource         = CLK_SRC_SYS_CLK_DIV2,
        .auxClockDivNumerator   = 1,
        .auxClockDivDenominator = 1
    },
    {
        .auxClockEnableMask     = AUX_CLK_MASK_MEDIA,         // SIPP Clock
        .auxClockSource         = CLK_SRC_SYS_CLK ,      //
        .auxClockDivNumerator   = 1,                          //
        .auxClockDivDenominator = 1,                          //
    },
    //TODO: Disable CIF for release application?
    {
        .auxClockEnableMask     = (AUX_CLK_MASK_CIF0 | AUX_CLK_MASK_CIF1),  // CIFs Clock
        .auxClockSource         = CLK_SRC_SYS_CLK,            //
        .auxClockDivNumerator   = 1,                          //
        .auxClockDivDenominator = 1,                          //
    },
    {
        .auxClockEnableMask     = CLOCKS_MIPICFG,             // MIPI CFG + ECFG Clock
        .auxClockSource         = CLK_SRC_SYS_CLK     ,       //
        .auxClockDivNumerator   = 1,                          //
        .auxClockDivDenominator = (uint32_t)(DEFAULT_APP_CLOCK_KHZ/24000),                         //
    },
    {
        .auxClockEnableMask = AUX_CLK_MASK_MIPI_TX0 | AUX_CLK_MASK_MIPI_TX1,
        .auxClockSource = CLK_SRC_REFCLK0,
        .auxClockDivNumerator = 1,
        .auxClockDivDenominator = 1
    }, // ref clocks for MIPI PLL,
    {
        .auxClockEnableMask = (u32)(1 << CSS_AUX_TSENS),
        .auxClockSource = CLK_SRC_REFCLK0,
        .auxClockDivNumerator = 1,
        .auxClockDivDenominator = 10,
    },
    {
      .auxClockEnableMask     = AUX_CLK_MASK_USB_PHY_EXTREFCLK,
      .auxClockSource         = CLK_SRC_PLL0,
      .auxClockDivNumerator   = 1,
      .auxClockDivDenominator = (uint32_t)(DEFAULT_APP_CLOCK_KHZ/DESIRED_USB_FREQ_KHZ)
    },
    {
      .auxClockEnableMask     = AUX_CLK_MASK_USB_PHY_REF_ALT_CLK,
      .auxClockSource         = CLK_SRC_PLL0,
      .auxClockDivNumerator   = 1,
      .auxClockDivDenominator = (uint32_t)(DEFAULT_APP_CLOCK_KHZ/DESIRED_USB_FREQ_KHZ)
    },
    {
      .auxClockEnableMask     = AUX_CLK_MASK_USB_CTRL_REF_CLK,
      .auxClockSource         = CLK_SRC_PLL0,
      .auxClockDivNumerator   = 1,
      .auxClockDivDenominator = (uint32_t)(DEFAULT_APP_CLOCK_KHZ/DESIRED_USB_FREQ_KHZ)
    },
    {
      .auxClockEnableMask     = AUX_CLK_MASK_USB_CTRL_SUSPEND_CLK,
      .auxClockSource         = CLK_SRC_PLL0,
      .auxClockDivNumerator   = 1,
      .auxClockDivDenominator = (uint32_t)(DEFAULT_APP_CLOCK_KHZ/DESIRED_USB_FREQ_KHZ)
    },

    {0,0,0,0}, // Null Terminated List
};

//#############################################################################
#include <DrvLeonL2C.h>
void leonL2CacheInitWrThrough()
{
    LL2CConfig_t ll2Config;

    // Invalidate entire L2Cache before enabling it
    DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, /*disable cache?:*/ 0);

    ll2Config.LL2CEnable = 1;
    ll2Config.LL2CLockedWaysNo = 0;
    ll2Config.LL2CWayToReplace = 0;
    ll2Config.busUsage = BUS_WRAPPING_MODE;
    ll2Config.hitRate = HIT_WRAPPING_MODE;
    ll2Config.replacePolicy = LRU;
    ll2Config.writePolicy = WRITE_THROUGH;

    DrvLL2CInitialize(&ll2Config);
}


/**************************************************************************************************
 ~~~ Functions Implementation
**************************************************************************************************/
int initClocksAndMemory(void)
{
//    int retVal;


    // Configure the system
    OsDrvCprInit();
    OsDrvCprOpen();
    OsDrvTimerInit();
    OsDrvCprAuxClockArrayConfig(auxClkAllOn);
    leonL2CacheInitWrThrough();
    // Set the shave L2 Cache mode
    DrvShaveL2CacheSetMode(L2CACHE_CFG);

    // Enable PMB subsystem gated clocks and resets
    SET_REG_WORD   (CMX_RSTN_CTRL,0x00000000);               // engage reset
    SET_REG_WORD   (CMX_CLK_CTRL, 0xffffffff);               // turn on clocks
    SET_REG_WORD   (CMX_RSTN_CTRL,0xffffffff);               // dis-engage reset
    // Enable media subsystem gated clocks and resets
    SET_REG_WORD(MSS_CLK_CTRL_ADR,      0xffffffff);
    SET_REG_WORD(MSS_RSTN_CTRL_ADR,     0xffffffff);

    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD |  DEV_MSS_CIF0 | DEV_MSS_CIF1 | DEV_MSS_SIPP);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(CSS_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, -1);

    DrvCprStartAllClocks();
    SET_REG_WORD(MSS_SIPP_RSTN_CTRL_ADR, 0);
    SET_REG_WORD(MSS_SIPP_RSTN_CTRL_ADR, 0x3ffffff);
    SET_REG_WORD(MSS_SIPP_CLK_SET_ADR,   0x3ffffff);

    return 0;
}


// assumed host will update all this parameters
void initSystem(void)
{
    #define IRQ_SRC_0   0
    #define IRQ_SRC_1   1
    #define IRQ_SRC_2   2
    #define IRQ_SRC_3   3

    initClocksAndMemory();
    DrvGpioIrqSrcDisable(IRQ_SRC_0);
    DrvGpioIrqSrcDisable(IRQ_SRC_1);
    DrvGpioIrqSrcDisable(IRQ_SRC_2);
    DrvGpioIrqSrcDisable(IRQ_SRC_3);
    DrvGpioInitialiseRange(brdMV0182GpioCfgDefault);
}

