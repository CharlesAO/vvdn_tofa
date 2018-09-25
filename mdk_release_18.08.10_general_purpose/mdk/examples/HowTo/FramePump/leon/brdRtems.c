///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     board initialization file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "brdRtems.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems.h>
#include <rtems/shell.h>
#include <rtems/status-checks.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include <errno.h>
#include <OsDrvI2cBus.h>
#include <OsDrvCpr.h>
#include <OsDrvTimer.h>
#include <DrvDdr.h>
#include "appConfigDefines.h"
#include "OsDrvI2cMyr2.h"
#if defined(MA2150)
 #include <OsBrdMv0182.h>
#elif defined(MA2450)
 #include <OsMV0212.h>
#else
 #error "Unsupported chip version"
#endif
#include <OsCDCEL.h>
#include <DrvShaveL2Cache.h>
#include <FramePump.h>
#include <FramePumpCif.h>
#include <FramePumpSipp.h>
#include <FramePumpControl.h>
#include "commonTests.h"
#include "DrvLeonL2C.h"

#include <OsImx208.h>
#include "FramePumpOv9282.h"
#include "FramePumpImx378.h"

#include <mvLog.h>

// Maximum number of buses to describe devices
#define MAX_I2C_BUSES_PER_DEV                   (3)
#define MAX_I2C_ADDRESSES_PER_DEV               (8)

typedef struct
{
    const char* name;
    const rtems_libi2c_drv_t* protocolDesc;
    int32_t busIds[MAX_I2C_BUSES_PER_DEV];
    uint16_t addresses[MAX_I2C_ADDRESSES_PER_DEV];
}BoardDevList;

static BoardDevList i2cDeviceListOv9282CamPair[] = {
    {
        .name = "ov9282_tracking_left",
        .protocolDesc = &libi2cOv9282ProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {0x61, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = "ov9282_tracking_right",
        .protocolDesc = &libi2cOv9282ProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {0x62, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = "ov9282_tracking",
        .protocolDesc = &libi2cOv9282BroadcastProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {0x71, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};

static BoardDevList i2cDeviceListImx208CamPair[] =
{
    {
        .name = "imx208_left",
        .protocolDesc = &imx208ProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {(0x6c >> 1), 0, 0, 0, 0, 0, 0},
    },
    {
        .name = "imx208_right",
        .protocolDesc = &imx208ProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {(0x6e >> 1), 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};

static BoardDevList i2cDeviceListCamA[] = {
    {
        .name = "imx378",
        .protocolDesc = &imx378ProtocolDrvTbl,
        .busIds = {1, -1},
        .addresses = {IMX378_I2C_ADDRESS, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};


void initCachesAndOtherStuff(void)
{
    // DrvCprSysDeviceAction(UPA_DOMAIN, ASSERT_RESET, -1);
    DrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, -1);

    DrvShaveL2CacheSetMode(SHAVEL2C_MODE_NORMAL);

    // Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART64KB);
    DrvShaveL2CacheSetupPartition(SHAVEPART64KB);
    DrvShaveL2CacheSetupPartition(SHAVEPART64KB);
    DrvShaveL2CacheSetupPartition(SHAVEPART64KB);

    // Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();

    DrvShaveL2CacheSetLSUPartId(0, 0);
    DrvShaveL2CacheSetLSUPartId(1, 1);
    DrvShaveL2CacheSetLSUPartId(5, 2);
    DrvShaveL2CacheSetLSUPartId(6, 3);
}


OS_MYRIAD_DRIVER_STATUS_CODE brdInitializeCpr(void)
{
    OS_MYRIAD_DRIVER_STATUS_CODE mdsc = OS_MYR_DRV_SUCCESS;

    mdsc = OsDrvCprInit();
    if (mdsc != OS_MYR_DRV_SUCCESS) {
        return mdsc;
    }

    mdsc = OsDrvCprOpen();
    if (mdsc != OS_MYR_DRV_SUCCESS) {
        return mdsc;
    }

    mdsc = OsDrvCprSetupClocks(&appClockConfig);
    if (mdsc != OS_MYR_DRV_SUCCESS) {
        return mdsc;
    }

    mdsc = OsDrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, -1);
    if (mdsc != OS_MYR_DRV_SUCCESS) {
        return mdsc;
    }

    mdsc = OsDrvCprSysDeviceAction(CSS_DOMAIN, DEASSERT_RESET, -1);
    if (mdsc != OS_MYR_DRV_SUCCESS) {
        return mdsc;
    }

    mdsc = OsDrvCprSysDeviceAction(SIPP_DOMAIN, DEASSERT_RESET, -1);
    if (mdsc != OS_MYR_DRV_SUCCESS) {
        return mdsc;
    }

    mdsc = OsDrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET, -1);
    if (mdsc != OS_MYR_DRV_SUCCESS) {
        return mdsc;
    }

    return OS_MYR_DRV_SUCCESS;
}

static bool mipiCtrl[6] = {false};

rtems_status_code brdInitializeSensors(void)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    uint32_t framePumpMajor = 0;
    uint32_t i = 0;
    sc = rtems_io_register_driver( 0, &FramePumpDrvTblName, &framePumpMajor );
    RTEMS_CHECK_SC(sc, "rtems_io_register_driver FramePump");

    struct FramePumpVideoDeviceSensor sensorDevice[] = {
        {
            .vd = {
                .name = "/dev/video_tracking_left",
                .controlDevice = "/dev/i2c.imx208_left",
#ifdef USE_SIPP_CMX
                .hwFnc = &sippCmxFunc,
#else
                .hwFnc = &sippFunc,
#endif
                .interruptLevel = 10,
            },
            .ctrlNo = MIPI_CTRL_2,
            .gpioa = NULL,
            .spec = &framePumpimx208_camCfg,
        },
        {

            .vd = {
                .name = "/dev/video_tracking_right",
                .controlDevice = "/dev/i2c.imx208_right",
                .hwFnc = &cifFunc,
                .interruptLevel = 10,
            },
            .ctrlNo = MIPI_CTRL_3,
            .gpioa = NULL,
            .spec = &framePumpimx208_camCfg,
        },
        {
            .vd = {
                .name = "/dev/video_tracking_left",
                .controlDevice = "/dev/i2c.ov7750_tracking_left",
                .sharedControlDevice = "/dev/i2c.ov7750_tracking",
#ifdef USE_SIPP_CMX
                .hwFnc = &sippCmxFunc,
#else
                .hwFnc = &sippFunc,
#endif
                .interruptLevel = 10,
            },
            .ctrlNo = MIPI_CTRL_2,
            .gpioa = NULL,
            .spec = &framePumpOv7750Raw10_camCfg,
        },
        {
            .vd = {
                .name = "/dev/video_tracking_right",
                .controlDevice = "/dev/i2c.ov7750_tracking_right",
                .sharedControlDevice = "/dev/i2c.ov7750_tracking",
                .hwFnc = &cifFunc,
                .interruptLevel = 10,
            },
            .ctrlNo = MIPI_CTRL_3,
            .gpioa = NULL,
            .spec = &framePumpOv7750Raw10_camCfg,
        },
        {
            .vd = {
                .name = "/dev/video_tracking_left",
                .controlDevice = "/dev/i2c.ov9282_tracking_left",
                .sharedControlDevice = "/dev/i2c.ov9282_tracking",
#ifdef USE_SIPP_CMX
                .hwFnc = &sippCmxFunc,
#else
                .hwFnc = &sippFunc,
#endif
                .interruptLevel = 10,
            },
            .ctrlNo = MIPI_CTRL_2,
            .gpioa = NULL,
            .spec = &framePumpOv9282Raw8_camCfg,
        },
        {
            .vd = {
                .name = "/dev/video_tracking_right",
                .controlDevice = "/dev/i2c.ov9282_tracking_right",
                .sharedControlDevice = "/dev/i2c.ov9282_tracking",
                .hwFnc = &cifFunc,
                    .interruptLevel = 10,
                },
                .ctrlNo = MIPI_CTRL_3,
                .gpioa = NULL,
                .spec = &framePumpOv9282Raw8_camCfg,
            },
            {
            .vd = {
                .name = "/dev/video_streaming",
                .controlDevice = "/dev/i2c.imx214_video",
#ifdef USE_SIPP_CMX
                .hwFnc = &sippCmxFunc,
#else
                .hwFnc = &sippFunc,
#endif
                .interruptLevel = 10,
            },
            .ctrlNo = MIPI_CTRL_0,
            .gpioa = NULL,
            .spec = &framePumpimx214_binning_camCfg,
        },
        {
            .vd = {
                .name = "/dev/video_streaming",
                .controlDevice = "/dev/i2c.imx378",
                .hwFnc = &cifFunc,
                .interruptLevel = 10,
            },
            .ctrlNo = MIPI_CTRL_0,
            .gpioa = NULL,
            .spec = &framePumpimx378_camCfg,
        },
    };

    for (i = 0 ; i < COUNT_OF(sensorDevice); i++) {

        // Check if the MIPI control has not yet been initialized.
        if (mipiCtrl[sensorDevice[i].ctrlNo] == false) {
            sc = rtems_io_initialize(framePumpMajor, 0, (void*)&sensorDevice[i]);
            if (sc != RTEMS_SUCCESSFUL) {
                mvLog(MVLOG_WARN, "Error initializing %s video device with %s error %s",
                       sensorDevice[i].vd.name, sensorDevice[i].vd.controlDevice,
                       strerror(errno));
            } else {
                mvLog(MVLOG_INFO, "Initialized %s video device with %s",
                        sensorDevice[i].vd.name, sensorDevice[i].vd.controlDevice);
                mipiCtrl[sensorDevice[i].ctrlNo] = true;
            }
        }
    }

    return sc;
}

extern int32_t i2cbusHandlers[MAX_I2C_BUSES_PER_DEV];

static rtems_status_code boardRegisterI2CDevices (BoardDevList* i2c_device_list)
{
    mvLog(MVLOG_INFO, "Registering a list of I2C devices:");
    uint32_t dev_ind = 0;
    int32_t i = 0;
    int32_t j = 0;
    int32_t i2c_drv_minor = 0;
    BoardDevList* device = &i2c_device_list[dev_ind];
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    while (device->name != NULL) {
        mvLog(MVLOG_INFO, "Registering I2C device %s", device->name);
        i = 0;
        j = 0;
        while (i < MAX_I2C_BUSES_PER_DEV && device->busIds[i] != -1) {
            j = 0;
            while (j < MAX_I2C_ADDRESSES_PER_DEV && device->addresses[j] != 0) {
                mvLog(MVLOG_INFO, "Polling address 0x%x on bus %ld",
                      device->addresses[j], device->busIds[i]);
                i2c_drv_minor = rtems_libi2c_register_drv(device->name,
                                                        (rtems_libi2c_drv_t*) device->protocolDesc,
                                                        i2cbusHandlers[device->busIds[i]],
                                                        device->addresses[j]);
                if (i2c_drv_minor >= 0) {
                    mvLog(MVLOG_INFO, "I2C device %s registered at bus %ld with"
                                      "address 0x%x",
                           device->name, device->busIds[i],
                           device->addresses[j]);
                    break;
                }
                j++;
            }

            if (j < MAX_I2C_ADDRESSES_PER_DEV && device->addresses[j] != 0) {
                // The device was found. No need to poll it on the other buses
                break;
            }
            i++;
        }
        if (i >= MAX_I2C_BUSES_PER_DEV || device->busIds[i] == -1 ) {
            // This isn't an error, we can use this feature to autodetect
            // devices (i.e  what camera is connected to the board)
            mvLog(MVLOG_WARN, "Device not found");
            sc = -1;
        }
        mvLog(MVLOG_INFO, "----------------------------------------");
        device = &i2c_device_list[++dev_ind];
    }

    return sc;
}

rtems_status_code brdInitializeBoard (void)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    OS_MYRIAD_DRIVER_STATUS_CODE mdsc = OS_MYR_DRV_SUCCESS;
    struct stat st = {0};
    const char *cdcel_name = "/dev/i2c.cdcel";
    rtems_device_major_number cdcel_major = 0;
    rtems_device_minor_number cdcel_minor = 0;
    rtems_libio_ioctl_args_t cdcelControl = {0};

    // Initialize clocks
    mdsc = brdInitializeCpr();
    // TODO - JHD - Handle error code.
    (void)mdsc;

#ifdef DDISABLE_LEON_DCACHE
    DrvLL2CDisable(LL2C_OPERATION_INVALIDATE);
#endif // DDISABLE_LEON_DCACHE

    OsDrvTimerInit();

#if defined(MA2150)
    tyOsBoard0182Configuration brdConfig[] = {
        {
            .type = MV182_CAM_B_I2C_BUS,
            // Always use I2C0 for CAMB (and normal I2C1 for CAMA) when both the
            // IMX208 and IMX214 sensors are present, in order to avoid conflict
            // on the 0x10 I2C address.
            .value = MV182_I2C0
        },
        {
            .type = MV182_END,
            .value = 0
        },
    };
    tyOsBrd182ErrorCode errCode;
    errCode = osBoard0182Initialise((tyOsBoard0182Configuration*)brdConfig);
    if (errCode != DRV_OS_BRD_182_DRV_SUCCESS) {
        mvLog(MVLOG_WARN, "Board initialization problem.");
        failTest( __func__, __LINE__ );
    }
#elif defined(MA2450)
    BoardI2CDevConfiguration i2cConfig[] = {
        {MV212_CAM_B_I2C_BUS, MV212_I2C0},
        { MV212_END, 0 }}; // Default config.
    BoardConfigDesc brdConfig[] = {
            {
                .type = BRDCONFIG_I2CDEVLIST,
                .config = &i2cConfig
            },
            {
                .type = BRDCONFIG_END,
                .config = 0
            },
        };
    BoardInit(brdConfig);
    mvLog(MVLOG_INFO, "Board 212 started");
#endif // defined(MA2150)

    if (stat("/dev/i2c.ov7750_tracking", NULL) != 0) {
        boardRegisterI2CDevices(i2cDeviceListOv9282CamPair);
    }

    boardRegisterI2CDevices(i2cDeviceListCamA);

    // This is done here again because the first time the sensor reset GPIOs were not LOW
    // Here the OV9282 or OV7750 driver probe already set the reset GPIOs HIGH
    boardRegisterI2CDevices(i2cDeviceListImx208CamPair);

    initCachesAndOtherStuff();

    // Note that Cpr was opened by osBoard0182Initialise()

    cdcelControl.command = EXT_PLL_CFG_148_24_24MHZ;
    if ((sc = stat(cdcel_name, &st))) {
        mvLog(MVLOG_WARN, "CDCEL error");
        failTest( __func__, __LINE__ );
    }
    cdcel_major = rtems_filesystem_dev_major_t(st.st_rdev);
    cdcel_minor = rtems_filesystem_dev_minor_t(st.st_rdev);

    sc = rtems_io_open(cdcel_major, cdcel_minor, NULL);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_WARN, "CDCEL open error: %d", sc);
        failTest( __func__, __LINE__ );
    }

    sc = rtems_io_control(cdcel_major, cdcel_minor, &cdcelControl);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_WARN, "CDCEL control error: %d", sc);
        failTest( __func__, __LINE__ );
    }

    DrvDdrInitialise(NULL);
    brdInitializeSensors();

    return RTEMS_SUCCESSFUL;
}
