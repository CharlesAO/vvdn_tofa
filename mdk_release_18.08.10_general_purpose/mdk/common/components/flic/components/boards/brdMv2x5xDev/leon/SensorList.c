///
/// @file      SensorList.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Default sensor list associated with this board
///            Platform(s) supported : ma2x8x
///

#include "DrvSensor.h"
#include "DrvSensorImx208.h"
#include "DrvSensorImx214.h"
#include "BoardConfig.h"
//#include "sensorApi.h"

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------
struct DrvSensorVideoDeviceSensor sensorDevices[] = {
    {
        .vd = {
            .name                   = "/dev/video_cam208B_left",
            .i2cBusControlId        = &i2cBus0, //i2cBus0,
            .i2cBusName             = I2C_BUS_NAME0,
            .i2cControlDeviceName   = "imx208B_left",
            .i2cAddress             = IMX208_I2C_ADDRESS_LEFT,
            .controlDevice          = "/dev/i2c0.imx208B_left",// Todo: this is generated from other 2
            .i2cProtocolAdrTbl      = &i2cImx208ProtocolDrvTbl,
            .sharedControlDevice    = "",//"/dev/i2c0.ov7750_tracking",
            .ctlFnc                 = &drvSensorImx208Control,
            .interruptLevel         = 10,
        },
        .ctrlNo = MIPI_CTRL_2,
        .ctrlRecNo = SIPP_DEVICE1,
        .gpioa = NULL,
        .spec = &drvSensorimx208_RAW10_camCfg,
    },
    {
        .vd = {
            .name                   = "/dev/video_cam208B_right",
            .i2cBusControlId        = &i2cBus0, //i2cBus0,
            .i2cBusName             = I2C_BUS_NAME0,
            .i2cControlDeviceName   = "imx208B_right",
            .i2cAddress             = IMX208_I2C_ADDRESS_RIGHT,
            .controlDevice          = "/dev/i2c0.imx208B_right",// Todo: this is generated from other 2
            .i2cProtocolAdrTbl      = &i2cImx208ProtocolDrvTbl,
            .sharedControlDevice    = "",//"/dev/i2c0.ov7750_tracking",
            .ctlFnc                 = &drvSensorImx208Control,
            .interruptLevel         = 10,
        },
        .ctrlNo = MIPI_CTRL_3,
        .ctrlRecNo = SIPP_DEVICE2,
        .gpioa = NULL,
        .spec = &drvSensorimx208_RAW10_camCfg,
    },
    {
        .vd = {
            .name                   = "/dev/video_cam214A",
            .i2cBusControlId        = &i2cBus1, //i2cBus0,
            .i2cBusName             = I2C_BUS_NAME1,
            .i2cControlDeviceName   = "imx214A",
            .i2cAddress             = IMX214_I2C_ADDRESS_DRV_SENSOR,
            .controlDevice          = "/dev/i2c1.imx214A",// Todo: this is generated from other 2
            .i2cProtocolAdrTbl      = &i2cImx214ProtocolDrvTbl,
            .sharedControlDevice    = "",
            .ctlFnc                 = &drvSensorImx214Control,
            .interruptLevel         = 10,
        },
        .ctrlNo = MIPI_CTRL_0,
        .ctrlRecNo = SIPP_DEVICE0,
        .gpioa = NULL,
        .spec = &drvSensorimx214_RAW10_binning_camCfg,
    }
};


rtems_status_code discoverAndRegisterCameras(void) {
    rtems_status_code sc;
    u32 drvSensorMajor;
    u32 i;
    sc = rtems_io_register_driver( 0, &DrvSensorDrvTblName, &drvSensorMajor );
    RTEMS_CHECK_SC(sc, "rtems_io_register_driver DrvSensor\n");
    if(RTEMS_SUCCESSFUL != sc) {
        return (sc);
    }
    for (i = 0; i < (sizeof(sensorDevices)/sizeof(struct DrvSensorVideoDeviceSensor)); i++ ) {
        sc = rtems_io_initialize(drvSensorMajor, 0, (void*)&sensorDevices[i]);
        if(sc) {
            printf("Error initializing %s video device\n", sensorDevices[i].vd.name);
        }
    }

    return RTEMS_SUCCESSFUL;
}
