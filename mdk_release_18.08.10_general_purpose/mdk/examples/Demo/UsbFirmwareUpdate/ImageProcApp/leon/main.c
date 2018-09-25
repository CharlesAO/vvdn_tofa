#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <bsp.h>

#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include <fcntl.h>
#include <mv_types.h>
#include <DrvLeon.h>
#include <DrvDdr.h>
#include <DrvCpr.h>
#include <DrvShaveL2Cache.h>
#include <swcLeonUtils.h>
#include <DrvLeonL2C.h>
#include <DrvTimer.h>

#include <rtems/blkdev.h>
#include <rtems/ide_part_table.h>
#include <rtems/ramdisk.h>
#include <rtems/libcsupport.h>
#include <OsDrvCpr.h>
#include <rtems/io.h>
#include <rtems/libio.h>

#include <unistd.h>
#include <sys/dirent.h>
#include <ctype.h>
#include "app_config.h"
#include "rtems_config.h"
#include "UsbFlasherAddon/If_usbFlasher.h"

#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include "DrvGpio.h"
#include "OsDrvSpiBus.h"
#include "OsDrvI2cBus.h"
#include "OsDrvI2cMyr2.h"
#include "system.h"
#include "OsWm8325.h"

// declare I2C bus
#define INTERRUPTPRIORITY   8   //priority of the I2C bus
#define ADDR10BIT           0   //addr10bit_ can be 0 for 7 bits address or 1 for 10 bits address
#define PMIC_I2C_ADDRESS        (0x6C>>1)

#if(CONFIGURE_MAXIMUM_TASKS < 15)
#warning "CONFIGURE_MAXIMUM_TASKS is less then min 15 needed for USB Flasher"
#endif

#if(CONFIGURE_MAXIMUM_TIMERS < 4)
#warning "CONFIGURE_MAXIMUM_TIMERS is less then min 4 needed for USB Flasher"
#endif

// Declare the I2C bus
DECLARE_I2C_BUS(myr2_i2c, 3, I2C_SPEED_SS, ADDR10BIT, INTERRUPTPRIORITY);

static int i_myr2_register_libi2c_spi_bus(void)
{
    int ret_code = RTEMS_SUCCESSFUL;
    int i2c2_busno;
    u32 drvMinorWM8325;

     /*
     * init I2C library (if not already done)
     */
    if ((ret_code = OsDrvLibI2CInitialize()) != RTEMS_SUCCESSFUL) {
        printf("rtems_libi2c_initialize FAILED %d \n", ret_code);
        goto exit_i_myr2_register_libi2c_spi_bus;
    }

    ret_code = OsDrvLibI2CRegisterBus("/dev/i2c2",
                                      (rtems_libi2c_bus_t *)&myr2_i2c);
    if (ret_code < 0) {
          printf("Could not register the I2C bus, err %d\n", ret_code);
        goto exit_i_myr2_register_libi2c_spi_bus;
    }
    i2c2_busno = ret_code;

   // WM8325
    ret_code = OsDrvLibI2CRegisterDevice(   "wm8325", (rtems_libi2c_drv_t *)&wm8325ProtocolDrvTbl, i2c2_busno, PMIC_I2C_ADDRESS);
    if (ret_code < 0) {
        printf("\n WM8325 minor version < 0 \n");
        goto exit_i_myr2_register_libi2c_spi_bus;
    }
    drvMinorWM8325 = ret_code;

    if ((ret_code = OsDrvIOInitialize(rtems_libi2c_major, drvMinorWM8325, NULL)) != RTEMS_SUCCESSFUL) {
        printf("rtems_io_initialize failed with sc %d\n", ret_code);
        goto exit_i_myr2_register_libi2c_spi_bus;
    }

    exit_i_myr2_register_libi2c_spi_bus:

    return ret_code;
}


void *POSIX_Init (void *args)
{
    UNUSED(args);
    uint32_t floating_atribute = RTEMS_LOCAL;
    rtems_name task_name;
    rtems_status_code status;
    rtems_id task_id;

    if (i_initClocksAndMemory()) {
        printf("Failed to initialize clocks.\n");
    }

    // Call USB Flahser init
    if(i_UsbFlasherInit() != 0)
    {
        printf("Failed to initialize UsbFlashLoader system configuration.\n");
    }
    else
    {
        if (i_UsbFlasherStart() != 0) // start usb flasher specific tasks
        {
            printf("\n\nUSB Flasher Start failed!\n\n\n");
        }
    }

    if (i_myr2_register_libi2c_spi_bus() != RTEMS_SUCCESSFUL)
    {
        printf("Failed to initialize i2c registers.\n");
    }

    // the user task
    task_name = rtems_build_name('B', 'L', 'N', 'K');
    // Task creation
    status = rtems_task_create(task_name, 2, RTEMS_MINIMUM_STACK_SIZE,
                   RTEMS_INTERRUPT_LEVEL(0),
                   floating_atribute,
                   &task_id);

    if (status != RTEMS_SUCCESSFUL) {
        printf("rtems_task_create failed with %d\n", status);
    }

    // Start Task
    rtems_task_start(task_id, blink_task, (uint32_t)NULL);

    while (1) {
        rtems_task_wake_after(1000);
    }

    exit(0);
}



/* End of File */
