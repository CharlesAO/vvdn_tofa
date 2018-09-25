/* Test_task to be run on FPGA
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <bsp.h>
#include <icb_defines.h>
#include "mv_types.h"
#include <fcntl.h>

#include "OsDrvSpiBus.h"
#include "DrvTimer.h"
#include "DrvGpio.h"

#include <bsp/irq.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include "OsDrvCpr.h"
#include "OsDrvUsbPhy.h"

#include <SpiFlashN25QDevice.h>

#include "swcShaveLoaderPrivate.h"
#include "swcMemoryTransfer.h"
#include <swcLeonUtils.h>
#include <DrvLeonL2C.h>
#include <DrvLeonL2CDefines.h>
#include "spi_i2c_config.h"

#include "If_usbFlasher.h"
#include "usbFlasher.h"

#include "usbpump_application_rtems_api.h"

// Temporary storage for one flash subsector
static u8 flash_buff[SUBSECTOR_SIZE] = {0};

#ifndef DISABLE_LEON_DCACHE
#define USBPUMP_MDK_CACHE_ENABLE        0
#else
#define USBPUMP_MDK_CACHE_ENABLE        1
#endif


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
    /* DebugMask */         3
    );


static int i_UsbFlasherSystemCfg(void);


static int i_UsbFlasherSystemCfg ( void )
{
    // Array to hold USB clock configurations.
    const tyAuxClkDividerCfg auxClk[] =
    {
        {
            .auxClockEnableMask     = AUX_CLK_MASK_USB_PHY_EXTREFCLK,
            .auxClockSource         = CLK_SRC_PLL0,
            .auxClockDivNumerator   = 1,
            .auxClockDivDenominator = 24
        },
        {
            .auxClockEnableMask     = AUX_CLK_MASK_USB_PHY_REF_ALT_CLK,
            .auxClockSource         = CLK_SRC_PLL0,
            .auxClockDivNumerator   = 1,
            .auxClockDivDenominator = 24
        },
        {
            .auxClockEnableMask     = AUX_CLK_MASK_USB_CTRL_REF_CLK,
            .auxClockSource         = CLK_SRC_PLL0,
            .auxClockDivNumerator   = 1,
            .auxClockDivDenominator = 24
        },
        {
            .auxClockEnableMask     = AUX_CLK_MASK_USB_CTRL_SUSPEND_CLK,
            .auxClockSource         = CLK_SRC_PLL0,
            .auxClockDivNumerator   = 1,
            .auxClockDivDenominator = 24
        },
        {0,0,0,0}, // Null Terminated List
    };

    // Setup clocks for the USB subsystem
    return ((int)OsDrvCprAuxClockArrayConfig(auxClk) != OS_MYR_DRV_SUCCESS);
}


int i_UsbFlasherInit(void)
{
    int retVal = 0;
    if (i_UsbFlasherSystemCfg())
    {
        printf("Error: OsDrvCprAuxClockArrayConfig() FAILED, with error code: %d\n", retVal);
        retVal = -1;
    }
    else
    {
        printf("\nVSC App...\n\n");

        if (i_myr2_register_libi2c_spi_bus()) {
            printf("Failed to initialize bus or devices . Fatal ERROR !!! \n");
            retVal = -1;
        }
    }

    return retVal;
}


int i_UsbFlasherStart(void)
{
    osDrvUsbPhyParam_t initParam =
    {
        .enableOtgBlock    = USB_PHY_OTG_DISABLED,
        .useExternalClock  = USB_PHY_USE_EXT_CLK,
        .fSel              = USB_REFCLK_20MHZ,
        .refClkSel0        = USB_SUPER_SPEED_CLK_CONFIG,
        .forceHsOnly       = USB_PHY_HS_ONLY_OFF
    };

    OsDrvUsbPhyInit(&initParam);

    return ((int)UsbPump_Rtems_DataPump_Startup(&sg_DataPump_AppConfig));
}

static int erase_spi_flash(int fd, struct tFlashParams *p)
{
    int status;

    struct spiFlashN25QEraseArgs_t eraseArgs =
            {
                .offset = p->offset,
                .size   = p->size,
            };

    status = ioctl(fd, FLASH_CMD_ERASE, &eraseArgs);

    if (status) {
        printf("Unable to erase device, err = %d\n", (int)status);
    }

    return status;
}


static int write_spi_flash(int fd, struct tFlashParams *p)
{
    int status;
    int retVal = 0;

    status =  lseek(fd, p->offset, SEEK_SET);
    if (status != (int)p->offset)
    {
        perror("seek error: ");
        retVal = -1;
    }
    else
    {
        status = write(fd, p->inBuff, p->size);
        if (status != (int)p->size)
        {
            printf("Unable to write all %u bytes to device. %d sent \n", (unsigned int)p->size, (int)status);
            perror("");
            retVal = -1;
        }
    }
    return retVal;
}


static int read_spi_flash(int fd, struct tFlashParams *p)
{
    int status = 0;
    int retVal = 0;

    status =  lseek(fd, p->offset, SEEK_SET);
    if (status != (int)p->offset) {
        perror("seek error: ");
        retVal = -1;
    }
    else
    {
        status = read(fd, p->outBuff, p->size);
        if (status != (int)p->size) {
            printf("Unable to read all %u bytes from device. %d sent \n", (unsigned int)p->size, (int)status);
            perror("");
            retVal = -1;
        }
    }

    return retVal;
}


static int32_t write_flash_flag(struct tFlashParams *p)
{
    rtems_status_code status;
    void (*app_entry)(void) = NULL;
    tFlashHeader *flash_header = (tFlashHeader *)&flash_buff;
    uint32_t app_idx;
    int32_t ret = ERR_NONE;
    int fd;

    printf("Opening %s ...\n", p->devName);
    fd = open(p->devName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
        printf("spi open failed\n");
        ret = ERR_IO_DRV;
        goto exit_func;
    }

    printf("\nVSC Demo App...\n\n");

    // get the flash header
    p->offset = ALIGN_TO_SUBSECTOR(FLAGS_SECTION_OFFSET);
    p->size = sizeof(tFlashHeader);
    p->outBuff = flash_header;

    printf("Reading Header: %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);

    if (RTEMS_SUCCESSFUL != read_spi_flash(fd, p)) {
        printf("Error reading flash !!!\n");
        ret = ERR_RW;
        goto exit_deinit;
    }

    // Erase the header
    p->offset = ALIGN_TO_SUBSECTOR(FLAGS_SECTION_OFFSET);
    p->size = ALIGN_TO_SUBSECTOR(sizeof(tFlashHeader));

    printf("Erasing Header: %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);
    if (RTEMS_SUCCESSFUL != erase_spi_flash(fd, p)) {
        ret = ERR_RW;
        goto exit_deinit;
    }

    // update the global flag
    flash_header->execute_app_flag = FLAG_APP_UPDATE;

#if 1
    printf("\n");
    printf("global app flag  %d\n", (int)flash_header->execute_app_flag);
    printf("bootloader entry 0x%08X\n", (unsigned int)flash_header->loader_entry);
    for (app_idx = 0; app_idx < MAX_APPLIACIONS_IN_FLASH; app_idx++) {
        printf("header%u_magic   %08X\n", (unsigned int)app_idx, (unsigned int)flash_header->app_info[app_idx].header_magic);
        printf("header%u_crc     %08X\n", (unsigned int)app_idx, (unsigned int)flash_header->app_info[app_idx].app_crc);
        printf("bootImg%u_offset %08X\n", (unsigned int)app_idx, (unsigned int)flash_header->app_info[app_idx].bootImg_offset);
        printf("bootImg%u_size   %u\n",   (unsigned int)app_idx, (unsigned int)flash_header->app_info[app_idx].bootImg_size);
    }
#endif

    // Save the header
    p->inBuff = flash_header;
    p->size = sizeof(tFlashHeader);
    p->offset = ALIGN_TO_SUBSECTOR(FLAGS_SECTION_OFFSET);

    printf("Writing Header: %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);
    if (RTEMS_SUCCESSFUL != write_spi_flash(fd, p)) {
        ret = ERR_RW;
        goto exit_deinit;
    }

    printf("Flash OK\n");

    // Handle the data as an elf and execute it
    app_entry = (void (*)(void))(flash_header->loader_entry);
    printf("loader_entry 0x%x\n", (unsigned int)app_entry);

    if ((NULL != app_entry) && (0xFFFFFFFF != (u32)app_entry)) {
        // Reset the chip
        DrvSocReset();
    } else {
        // Invalid Entry point
        printf("Invalid Entry point!\n");
        ret = ERR_ELF_ENTRY;
        goto exit_deinit;
    }

exit_deinit:
    // Jump here to close the spi driver before exiting
    printf("Close and exit!\n");
    if ((status = close(fd)) != 0) {
        printf("spi close failed with sc %d\n", status);
        ret = ERR_IO_DRV;
    }

exit_func:
    // Just exit
    return ret;
}


rtems_status_code set_flash_update_flag()
{
    struct tFlashParams flash_params;
    rtems_status_code status = RTEMS_SUCCESSFUL;

    flash_params.devName = SPI_BUS_NAME "." SPI_FLASH_NAME;
    flash_params.imgId = 0;
    flash_params.writeImg = 1;
    flash_params.inBuff = &flash_buff[0];
    flash_params.outBuff = &flash_buff[0];

    if (ERR_NONE != write_flash_flag(&flash_params)) {
        printf("Failed to update flash!\n");
        status = RTEMS_IO_ERROR;
    }

    return status;
}
