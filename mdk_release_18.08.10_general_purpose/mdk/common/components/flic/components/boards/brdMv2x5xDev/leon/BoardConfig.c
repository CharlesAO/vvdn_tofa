///
/// @file      BoardConfig.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Board configuration
///            Platform(s) supported : ma2x8x
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include <assert.h>
#include <stdlib.h>
#include <rtems.h>
#include <rtems/status-checks.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include <rtems/shell.h>
#include <sys/ioctl.h>
#include <fcntl.h> // open, close ...

#include <OsDrvI2cMyr2.h>
#include <OsDrvI2cBus.h>
#include <OsDrvGpio.h>
#include <OsDrvUsbPhy.h>
#include <OsDrvCpr.h>
#include <OsDrvTimer.h>
#include <DrvDdr.h>

#include <BoardConfigGpio.h>
#include <OsADV7513.h>
#include <OsCDCEL.h>
/// Application Includes
/// -------------------------------------------------------------------------------------
#include "BoardConfig.h"
#include "DrvSensor.h"


/// Source Specific #defines and types (typedef,enum,struct)
/// -----------------------------------------------------------------------------------------------

/// CPR
#define AUX_CLOCKS_MIPICFG  (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)
#define AUX_CLOCKS_VIDEO    (AUX_CLK_MASK_MEDIA | AUX_CLK_MASK_LCD)

/// 3: Global Data (Only if absolutely necessary)
/// ----------------------------------------------------------------------------
extern rtems_status_code discoverAndRegisterCameras(void);

/// 4: Static Local Data
/// ----------------------------------------------------------------------------
DECLARE_I2C_BUS(brdI2CBus0, 1, I2C_SPEED_SS, 0, 8);
DECLARE_I2C_BUS(brdI2CBus1, 2, I2C_SPEED_SS, 0, 8);
DECLARE_I2C_BUS(brdI2CBus2, 3, I2C_SPEED_SS, 0, 8);

int i2cBus0;
int i2cBus1;
int i2cBus2;

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------
static tyAuxClkDividerCfg auxClk[] = {
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
      {
          .auxClockEnableMask     = AUX_CLOCKS_VIDEO,           // SIPP + LCD Clocks
          .auxClockSource         = CLK_SRC_SYS_CLK ,           //
          .auxClockDivNumerator   = 1,                          //
          .auxClockDivDenominator = 1,                          //
      },
      {
          .auxClockEnableMask     = AUX_CLOCKS_MIPICFG,         // MIPI CFG + ECFG Clocks
          .auxClockSource         = CLK_SRC_SYS_CLK     ,       //
          .auxClockDivNumerator   = 1,                          //
          .auxClockDivDenominator = 24,                         //
      },
      {0, 0, 0, 0}, // Null Terminated List
};

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

static rtems_status_code brdInitI2c(void) {
    rtems_status_code sc;
    int fd;

    sc = rtems_libi2c_initialize();
    RTEMS_CHECK_SC(sc, "rtems_libi2c_initialize FAILED %d \n");

    //// I2C BUS 0
    i2cBus0 = rtems_libi2c_register_bus(I2C_BUS_NAME0, (rtems_libi2c_bus_t *)&brdI2CBus0);
    RTEMS_CHECK_RV(i2cBus0, "register the i2cBus0 bus\n");
    /// I2C BUS 1
    i2cBus1 = rtems_libi2c_register_bus(I2C_BUS_NAME1, (rtems_libi2c_bus_t *)&brdI2CBus1);
    RTEMS_CHECK_RV(i2cBus1, "register the i2cBus1 bus\n");
    /// I2C BUS 2
    i2cBus2 = rtems_libi2c_register_bus(I2C_BUS_NAME2, (rtems_libi2c_bus_t *)&brdI2CBus2);
    RTEMS_CHECK_RV(i2cBus2, "register the i2cBus2 bus\n");

    // Install board specific devices
    // HDMI ADV7513 chip.
    sc = rtems_libi2c_register_drv(ADV7513_NAME,
                    (rtems_libi2c_drv_t*) &adv7513ProtocolDrvTbl, i2cBus2, ADV7513_SLAVE_ADDRESS);
    if(sc < 0) { printf("Could not register %s peripheral !!\n", "ADV7513");}
    //Clock Generator CDCEL
    sc = rtems_libi2c_register_drv(CDCEL_NAME, (rtems_libi2c_drv_t*) &cdcelProtocolDrvTbl,
                    i2cBus2, CDCEL925_SLAVE_ADDR);//CDCEL913_SLAVE_ADDR CDCEL925_SLAVE_ADDR
    if(sc < 0) { printf("Could not register %s peripheral !!\n", "cdcel");}

    // ************ Sensor search and register ***********
    discoverAndRegisterCameras();

    // ********* Configure CDCEL clock generator for HDMI usage *******************
    fd = open("/dev/i2c2." CDCEL_NAME, O_RDWR);
    if (fd < 0) {
        perror("CDCEL open failed ");
        exit(-1);
    }
    // !!! hardcoded for 1080p resolution, nv12 video format, used just for demo
    sc = ioctl(fd, EXT_PLL_CFG_148_24_24MHZ, NULL);
    if (sc < 0) {
        perror("CDCEL control failed !");
        exit(-1);
    }
    return RTEMS_SUCCESSFUL;
}

static rtems_status_code brdInitGpios(void)
{
    //OS_MYRIAD_DRIVER_STATUS_CODE status;
    rtems_status_code sc;
    uint32_t gpMajor;

    sc = rtems_io_register_driver( 0, &osDrvGpioTblName, &gpMajor);
    RTEMS_CHECK_SC(sc, "rtems_io_register_driver\n");

    sc = rtems_io_initialize(gpMajor, 0, (void*)brdMVGpioCfgDefault);
    RTEMS_CHECK_SC(sc, "rtems_io_initialize\n");
    /// -----------------------------------------------------------
    /// Sensor Reset
    /// -----------------------------------------------------------
    gpioVal_t gpioHiVal = GPIO_HIGH;
    gpioVal_t gpioLoVal = GPIO_LOW;
    rtems_libio_rw_args_t gpioHi = { .buffer = (char*)&gpioHiVal };
    rtems_libio_rw_args_t gpioLo = { .buffer = (char*)&gpioLoVal };
    /// CAM A
    rtems_io_open(gpMajor,    MVDEV_CAMA_RST_GPIO, NULL);
    rtems_io_write(gpMajor,   MVDEV_CAMA_RST_GPIO, &gpioLo);
    usleep(1 * 1000);
    rtems_io_write(gpMajor,   MVDEV_CAMA_RST_GPIO, &gpioHi);
    rtems_io_close(gpMajor,   MVDEV_CAMA_RST_GPIO, NULL);
    /// CAM B
    rtems_io_open(gpMajor,    MVDEV_CAMB_RST_GPIO, NULL);
    rtems_io_write(gpMajor,   MVDEV_CAMB_RST_GPIO, &gpioLo);
    usleep(1 * 1000);
    rtems_io_write(gpMajor,   MVDEV_CAMB_RST_GPIO, &gpioHi);
    rtems_io_close(gpMajor,   MVDEV_CAMB_RST_GPIO, NULL);
    /// CAM C
    rtems_io_open(gpMajor,    MVDEV_CAMC_RST_GPIO, NULL);
    rtems_io_write(gpMajor,   MVDEV_CAMC_RST_GPIO, &gpioLo);
    usleep(1 * 1000);
    rtems_io_write(gpMajor,   MVDEV_CAMC_RST_GPIO, &gpioHi);
    rtems_io_close(gpMajor,   MVDEV_CAMC_RST_GPIO, NULL);
    return sc;
}

static rtems_status_code brdInitCpr(void) {
    assert(OS_MYR_DRV_SUCCESS == OsDrvCprInit());
    assert(OS_MYR_DRV_SUCCESS == OsDrvCprOpen());
    assert(OS_MYR_DRV_SUCCESS == OsDrvCprAuxClockArrayConfig(auxClk));
    return RTEMS_SUCCESSFUL;
}


/// ===  FUNCTION  ======================================================================
///  Name:  brdInit
///  Description: board init top level function
/// =====================================================================================
rtems_status_code brdInit(void)
{
    /// Init CPR
    brdInitCpr();

    /// Init Gpios
    brdInitGpios();

    /// Init I2C
    brdInitI2c();

    return RTEMS_SUCCESSFUL;
}
