///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include "rtems_config.h"
#include "mv_types.h"
#include "spi_i2c_config.h"
#include "app_config.h"
#include "system.h"

#include "OsDrvUsbPhy.h"
#include "usbpumpdebug.h"
#include "usbpump_application_rtems_api.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#ifndef DISABLE_LEON_DCACHE
#define USBPUMP_MDK_CACHE_ENABLE        0
#else
#define USBPUMP_MDK_CACHE_ENABLE        1
#endif

//4: Static Local Data
// ----------------------------------------------------------------------------
// Variables to configure flash
struct tFlashParams flash_params;

static unsigned long __attribute__((section(".ddr_direct.bss"))) sg_DataPump_MemoryPool[(512 * 1024) / 4];

static USBPUMP_APPLICATION_RTEMS_CONFIGURATION sg_DataPump_AppConfig =
USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V1(
    /* nEventQueue */       64,
    /* pMemoryPool */       sg_DataPump_MemoryPool,
    /* nMemoryPool */       sizeof(sg_DataPump_MemoryPool),
    /* DataPumpTaskPriority */  100,
    /* DebugTaskPriority */     200,
    /* UsbInterruptPriority */  10,
    /* pDeviceSerialNumber */   NULL,
    /* pUseBusPowerFn */        NULL,
    /* fCacheEnabled */     USBPUMP_MDK_CACHE_ENABLE,
    /* DebugMask */         3 | UDMASK_PROTO | __TMS_UDMASK_ENTRY
    );

// Buffer stored in DDR memory for flash copy
extern u8 NDfuDemo_FlashRegion[];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------


static int i_SystemInit(void)
{
    int ret_val = 0;
    if((ret_val = i_initClocksAndMemory()) != 0)
    {
        printf("Init clock and memory failed with error code: %d", ret_val);
        goto exit_SystemInit;
    }

    v_myr2_spi_test_pin_config();

    if((ret_val = i_myr2_register_libi2c_spi_bus()) != 0)
    {
        printf("Failed to initialize bus or devices . Fatal ERROR: %d !!! \n", ret_val);
        goto exit_SystemInit;
    }

exit_SystemInit:
    return (ret_val);
}

rtems_task Init(rtems_task_argument argumentx)
{
     UNUSED(argumentx);

     int sc = 0;

     if ((sc = i_SystemInit()) != 0)
     {
         goto exit_Init;
     }

     printf("\n");
     printf("RTEMS OS Started\n"); /* initialize variables */

     osDrvUsbPhyParam_t initParam =
     {
         .enableOtgBlock    = USB_PHY_OTG_DISABLED,
         .useExternalClock  = USB_PHY_USE_EXT_CLK,
         .fSel              = USB_REFCLK_20MHZ,
         .refClkSel0        = USB_SUPER_SPEED_CLK_CONFIG,
         .forceHsOnly       = USB_PHY_HS_ONLY_OFF
     };

     flash_params.devName = SPI_BUS_NAME "." SPI_FLASH_NAME;
     flash_params.imgId = 0;
     flash_params.writeImg = FALSE;  // Set flag to skip flash writing as default.
     flash_params.inBuff = &NDfuDemo_FlashRegion;
     flash_params.outBuff = &NDfuDemo_FlashRegion;

     // try to execute an application from flash
     if (flash_execute(&flash_params))
     {
         // No application was found in flash - initialize usb and wait for a new one
         printf("No Application in flash! Waiting for update...\n");

         OsDrvUsbPhyInit(&initParam);

         if (!UsbPump_Rtems_DataPump_Startup(&sg_DataPump_AppConfig))
         {
               printf("\n\nUPF_DataPump_Startup() failed!\n\n\n");
         }
     }

 exit_Init:
    // delete Task
    rtems_task_delete(RTEMS_SELF);
}



