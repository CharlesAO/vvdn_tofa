///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main function that starts USB DataPump
///

#include <stdio.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <bsp.h>

#include "app_config.h"
#include "OsDrvUsbPhy.h"
#include "usbpumpdebug.h"

#include "usbpump_application_rtems_api.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// Default clock used by the App
// System Clock configuration on start-up
BSP_SET_CLOCK(DEFAULT_OSC0_KHZ, PLL_DESIRED_FREQ_KHZ, 1, 1, DEFAULT_RTEMS_CSS_LOS_CLOCKS, DEFAULT_RTEMS_MSS_LRT_CLOCKS,0,0,0);
// Program L2 cache behaviour
BSP_SET_L2C_CONFIG(0 ,DEFAULT_RTEMS_L2C_REPLACEMENT_POLICY, DEFAULT_RTEMS_L2C_LOCKED_WAYS,DEFAULT_RTEMS_L2C_MODE, 0, 0);

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
    /* fCacheEnabled */           0,
    /* DebugMask */               UDMASK_ERRORS | UDMASK_ANY
    );

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init (void *args)
{
    UNUSED(args);

    osDrvUsbPhyParam_t initParam =
    {
        .enableOtgBlock    = USB_PHY_OTG_DISABLED,
        .useExternalClock  = USB_PHY_USE_EXT_CLK,
        .fSel              = USB_REFCLK_20MHZ,
        .refClkSel0        = USB_SUPER_SPEED_CLK_CONFIG,
        .forceHsOnly       = USB_PHY_HS_ONLY_OFF
    };

    initClocksAndMemory();
    OsDrvUsbPhyInit(&initParam);

    if (UsbPump_Rtems_DataPump_Startup(&sg_DataPump_AppConfig))
    {
        printf("\n\nUsbPump_Rtems_DataPump_Startup()!\n\n\n");
    }
    else
    {
        printf("\n\nUsbPump_Rtems_DataPump_Startup() failed!\n\n\n");
    }
    return NULL;
}


