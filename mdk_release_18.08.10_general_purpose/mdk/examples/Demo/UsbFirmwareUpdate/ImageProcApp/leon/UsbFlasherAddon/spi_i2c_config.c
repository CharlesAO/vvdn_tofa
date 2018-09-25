/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Key application system settings
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "OsDrvCpr.h"
#include "OsDrvGpio.h"
#include "OsDrvSpiBus.h"
#include "OsDrvI2cBus.h"
#include "spi_i2c_config.h"
#include <SpiFlashN25QDevice.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SPI_BUS_NAME   "/dev/spi"
#define SPI_FLASH_NAME "flash"

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
// Declare the SPI bus
DECLARE_SPI_BUS_GPIO_SS(myr2_spi_0, 1, 0, 0, 1, 1, 10*1000*1000, 8);

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static void v_myr2_spi_test_pin_config(void);
static void v_myr2_i2c2_pin_config(void);


// 6: Functions Implementation
// ----------------------------------------------------------------------------


/**********************************************************************
 * Function: void myr2_spi_test_pin_config(void)
 * Description: function called at init to configure SPI port
 ***********************************************************************/
static void v_myr2_spi_test_pin_config(void)
{
    DrvGpioModeRange(74, 77, 0 |  D_GPIO_DIR_OUT);
}



/**********************************************************************
 * Function: void myr2_i2c2_pin_config(void)
 * Description: function called at init to configure i2c port
 ***********************************************************************/
static void v_myr2_i2c2_pin_config(void)
{
    DrvGpioSetMode(79, 2);
    DrvGpioSetMode(80, 2);
}



/**********************************************************************
 * Function: int myr2_register_libi2c_spi_bus(void)
 * Return Val:  - OS_MYR_DRV_SUCCESS (0) if all initialization functions executed correctly,
 *              - different then 0 if any init function failed.
 ***********************************************************************/
int i_myr2_register_libi2c_spi_bus(void)
{
    int ret_code = (int)RTEMS_SUCCESSFUL;
    int spi0_busno;
    u32 spi_flash_minor;

    v_myr2_spi_test_pin_config();
    v_myr2_i2c2_pin_config();

    /*
     * init I2C library (if not already done)
     */
    if ((ret_code = (int)OsDrvLibI2CInitialize()) != (int)RTEMS_SUCCESSFUL)
    {
        printf("rtems_libi2c_initialize FAILED %d \n", ret_code);
        goto exit_myr2_register_libi2c_spi_bus;
    }

    /*
     * register first I2C bus
     */
    ret_code = (int)OsDrvLibI2CRegisterBus(SPI_BUS_NAME,
                                      (rtems_libi2c_bus_t *)&myr2_spi_0);
    if (ret_code < 0)
    {
        printf("Could not register the bus\n");
        goto exit_myr2_register_libi2c_spi_bus;
    }
    spi0_busno = ret_code;

    // Returns minor
    ret_code = (int)OsDrvLibI2CRegisterDevice(SPI_FLASH_NAME,
                                         spi_flash_N25Q_driver_descriptor,
                                         spi0_busno, 77);
    if (ret_code < 0)
    {
        printf("Could not register the spi device\n");
        goto exit_myr2_register_libi2c_spi_bus;
    }

    spi_flash_minor = ret_code;

    if((ret_code = (int)OsDrvIOInitialize(rtems_libi2c_major, spi_flash_minor, NULL)) != (int)RTEMS_SUCCESSFUL)
    {
        printf("rtems_io_initialize failed with sc %d\n", ret_code);
        goto exit_myr2_register_libi2c_spi_bus;
    }

exit_myr2_register_libi2c_spi_bus:
    return (ret_code);
}

/* End of File */
