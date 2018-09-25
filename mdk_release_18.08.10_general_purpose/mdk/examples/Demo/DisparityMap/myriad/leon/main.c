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
#include <DrvRegUtilsDefines.h>
#include <DrvLeon.h>
#include <DrvCpr.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#include <swcLeonUtils.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include <fcntl.h>
#include <mv_types.h>
#include <rtems/cpuuse.h>

#include <DrvShaveL2Cache.h>
#include <bsp.h>
#include "rtems_config.h"
#include "OsDrvUsbPhy.h"
#include "usbpump_application_rtems_api.h"
#include "DrvCmxDma.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#ifndef DISABLE_LEON_DCACHE
# define USBPUMP_MDK_CACHE_ENABLE       1
#else
# define USBPUMP_MDK_CACHE_ENABLE       0
#endif

#define INTERRUPT_LEVEL     14
#define INTERRUPT_OFFSET    13
#define NUMBER_OF_AGENTS    0
#define STARTING_FROM       1
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
//Entry point of the leon_rt processor
volatile u32 var_debug = 0x33333333;
extern u32      *lrt_start;


// 4: Static Local Data
// ----------------------------------------------------------------------------

static USBPUMP_APPLICATION_RTEMS_CONFIGURATION sg_DataPump_AppConfig =
USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V1(
    /* nEventQueue */       64,
    /* pMemoryPool */       NULL,
    /* nMemoryPool */       0,
    /* DataPumpTaskPriority */  100,
    /* DebugTaskPriority */     200,
    /* UsbInterruptPriority */  10,
    /* pDeviceSerialNumber */   NULL,
    /* pUseBusPowerFn */        NULL,
    /* fCacheEnabled */     USBPUMP_MDK_CACHE_ENABLE,
    /* DebugMask */         3
);

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

    sc = initClocksAndMemory();
    if(sc)
        exit(sc);


    DrvLeonRTStartup((u32)&lrt_start);
    DrvLeonRTWaitForBoot();

    OsDrvUsbPhyInit(&initParam);

    if(UsbPump_Rtems_DataPump_Startup(&sg_DataPump_AppConfig) != NULL)
    {
        printf("\n\nUsbPump_Rtems_DataPump_Startup()!\n\n\n");
    }
    else
    {
        printf("\n\nUsbPump_Rtems_DataPump_Startup() failed!\n\n\n");
        exit(1);
    }

    DrvLeonRTWaitExecution();

    exit(0);
}
