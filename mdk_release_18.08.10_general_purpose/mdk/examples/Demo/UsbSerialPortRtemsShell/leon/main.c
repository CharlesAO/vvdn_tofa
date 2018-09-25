///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main function that starts USB DataPump
///

#include <stdio.h>
#include <stdlib.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <bsp.h>

#include "app_config.h"
#include "OsDrvUsbPhy.h"
#include "usbpumpdebug.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "serialPort.h"
#include "OsDrvCpr.h"

#include "usbpump_application_rtems_api.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// Default clock used by the App
// System Clock configuration on start-up
BSP_SET_CLOCK(DEFAULT_OSC0_KHZ, PLL_DESIRED_FREQ_KHZ, 1, 1, DEFAULT_RTEMS_CSS_LOS_CLOCKS, DEFAULT_RTEMS_MSS_LRT_CLOCKS,0,0,0);
// Program L2 cache behaviour
BSP_SET_L2C_CONFIG(1,DEFAULT_RTEMS_L2C_REPLACEMENT_POLICY, DEFAULT_RTEMS_L2C_LOCKED_WAYS,DEFAULT_RTEMS_L2C_MODE, 0, 0);

// 4: Static Local Data
// ----------------------------------------------------------------------------
/* Sections decoration is require here for downstream tools */

#ifndef DISABLE_LEON_DCACHE
# define USBPUMP_MDK_CACHE_ENABLE        1
#else
# define USBPUMP_MDK_CACHE_ENABLE        0
#endif

static USBPUMP_APPLICATION_RTEMS_CONFIGURATION sg_DataPump_AppConfig =
USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V1(
    /* nEventQueue */             64,
    /* pMemoryPool */             NULL,
    /* nMemoryPool */             0,
    /* DataPumpTaskPriority */    100,
    /* DebugTaskPriority */       200,
    /* UsbInterruptPriority */    10,
    /* pDeviceSerialNumber */     NULL,
    /* pUseBusPowerFn */          NULL,
    /* fCacheEnabled */           USBPUMP_MDK_CACHE_ENABLE,
    /* DebugMask */               UDMASK_ERRORS | UDMASK_ANY
    );

static tyAuxClkDividerCfg auxClk[] =
{
    {
        .auxClockEnableMask     = AUX_CLK_MASK_USB_CTRL_SUSPEND_CLK,
        .auxClockSource         = CLK_SRC_PLL0,
        .auxClockDivNumerator   = 1,
        .auxClockDivDenominator = 24
    },
    {0, 0, 0, 0}, // Null Terminated List
};


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init (void *args)
{
    UNUSED(args);

    s32 sc;
    osDrvUsbPhyParam_t initParam =
    {
        .enableOtgBlock    = USB_PHY_OTG_DISABLED,
        .useExternalClock  = USB_PHY_USE_EXT_CLK,
        .fSel              = USB_REFCLK_20MHZ,
        .refClkSel0        = USB_SUPER_SPEED_CLK_CONFIG,
        .forceHsOnly       = USB_PHY_HS_ONLY_OFF
    };

    OsDrvCprInit();
    OsDrvCprOpen();
    sc = OsDrvCprAuxClockArrayConfig(auxClk);
    if(sc)
        exit(sc);

    OsDrvUsbPhyInit(&initParam);
    if (UsbPump_Rtems_DataPump_Startup(&sg_DataPump_AppConfig) != NULL)
    {
        // AcmDemo_ClientCreate called at the end of USB initialization
        // redirects stdin, stdout and stderr to USB. Starting from this point
        // printk will be used where printf should normally be used, to avoid
        // redirection
        printk("USB serial port demo\n");
        printk("Please connect with a terminal emulator to access"
               "RTEMS shell (see Readme for details)\n\n");
    }
    else
    {
        printk("\n\nUsbPump_Rtems_DataPump_Startup() failed!\n\n\n");
        exit(1);
    }
    return NULL;
}


