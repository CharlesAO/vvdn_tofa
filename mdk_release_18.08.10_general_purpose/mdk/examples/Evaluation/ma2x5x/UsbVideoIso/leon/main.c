///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Variation of "IpipePPTest09, IpipePPTest09_a
///            PP inputs 960x128 and outputs 480x64 RGB data for VideoSipp
///            LOS starts LRT which executes the PP test.
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <DrvCpr.h>
#include <errno.h>
#include "rtems_config.h"
#include "OsDrvUsbPhy.h"
#include "usbpumpdebug.h"
#include "usbpump_application_rtems_api.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#ifndef DISABLE_LEON_DCACHE
# define USBPUMP_MDK_CACHE_ENABLE        1
#else
# define USBPUMP_MDK_CACHE_ENABLE        0
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
static USBPUMP_APPLICATION_RTEMS_CONFIGURATION sg_DataPump_AppConfig =
USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V1(
    /* nEventQueue */            64,
    /* pMemoryPool */            NULL,
    /* nMemoryPool */            0,
    /* DataPumpTaskPriority */   100,
    /* DebugTaskPriority */      200,
    /* UsbInterruptPriority */   10,
    /* pDeviceSerialNumber */    NULL,
    /* pUseBusPowerFn */         NULL,
    /* fCacheEnabled */          USBPUMP_MDK_CACHE_ENABLE,
    /* DebugMask */              UDMASK_ANY | UDMASK_ERRORS
    );

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void *POSIX_Init (void *args)
{
    UNUSED(args);

    int32_t sc;
    osDrvUsbPhyParam_t initParam =
    {
        .enableOtgBlock    = USB_PHY_OTG_DISABLED,
        .useExternalClock  = USB_PHY_USE_EXT_CLK,
        .fSel              = USB_REFCLK_20MHZ,
        .refClkSel0        = USB_SUPER_SPEED_CLK_CONFIG,
        .forceHsOnly       = USB_PHY_HS_ONLY_OFF
    };

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);

    OsDrvUsbPhyInit(&initParam);
    if (UsbPump_Rtems_DataPump_Startup(&sg_DataPump_AppConfig) != NULL)
    {
        printf("USB started\n\n");
    }
    else
    {
        printf("\n\nUsbPump_Rtems_DataPump_Startup() failed!\n\n\n");
        exit(1);
    }

    while (1) {
        rtems_task_wake_after(100);
    }
    exit(0);
}
