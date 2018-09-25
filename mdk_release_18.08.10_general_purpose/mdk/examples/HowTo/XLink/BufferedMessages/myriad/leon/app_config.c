///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "stdio.h"
#include "app_config.h"
#include <DrvLeonL2C.h>
#include <OsDrvCpr.h>
#include <stdlib.h>
#ifdef MA2X8X
#include "OsDrvInit.h"
#endif

// 2:  Source Specific #defines and types  (typedef,enum,struct)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
#ifdef MA2X8X
// parameters to be passed to CPR platform init
static OsDrvCprAuxClockConfig auxClkConfig[] =
{
    {OS_DRV_CPR_DEV_CSS_AUX_USB_PHY_REF_ALT, OS_DRV_CPR_CLK_PLL0, 1, 25},
    {OS_DRV_CPR_DEV_CSS_AUX_USB_CTRL_SUSPEND, OS_DRV_CPR_CLK_PLL0, 1, 10},
    {OS_DRV_CPR_DEV_CSS_AUX_USB20_REF, OS_DRV_CPR_CLK_PLL0, 1, 50},
    {OS_DRV_CPR_DEV_CSS_AUX_USB_CTRL_REF, OS_DRV_CPR_CLK_PLL0, 1, 600},
    {OS_DRV_CPR_DEV_CSS_AUX_USB_CTRL, OS_DRV_CPR_CLK_PLL0, 1, 2},
    OS_DRV_CPR_AUX_ARRAY_TERMINATOR
};

static OsDrvCprConfig config = {
    .ref0InClk = DEFAULT_OSC_CLOCK_KHZ,
    .auxCfg = auxClkConfig
};

OS_DRV_INIT_CPR_CFG_DEFINE(&config);
#endif

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

// Setup all the clock configurations needed by this application and also the ddr
int initClocksAndMemory(void)
{
    s32 sc = 0;

#ifndef MA2X8X
    tyAuxClkDividerCfg auxClk[] =
    {
        {AUX_CLK_MASK_USB_CTRL_SUSPEND_CLK, CLK_SRC_PLL0, 1, 30}, // 20 MHz
        {0, 0, 0, 0}, // Null Terminated List
    };

    sc = OsDrvCprInit();
    if (sc != RTEMS_SUCCESSFUL)
        return sc;
    sc = OsDrvCprOpen();
    if (sc != RTEMS_SUCCESSFUL)
        return sc;
    sc = OsDrvCprAuxClockArrayConfig(auxClk);
#else
    rtems_status_code status = OsDrvCprTurnOnIsland(OS_DRV_CPR_PWR_ISLAND_USB);
    if (status){
        printf("exiting\n");
        exit(status);
    }
#endif

    return sc;
}

