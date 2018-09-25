#include "brdRtems.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <rtems.h>
#include <rtems/status-checks.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>

#include <OsDrvI2cBus.h>
#include <OsDrvCpr.h>
#include <OsDrvTimer.h>
#include <DrvDdr.h>
#include <appConfigDefines.h>
#include "OsDrvI2cMyr2.h"
#if defined(MA2150)
#include <OsBrdMv0182.h>
#elif defined(MA2450)
#include <OsMV0212.h>
#else
#error "This app does not support your chip version"
#endif
#include <OsCDCEL.h>
#include <DrvShaveL2Cache.h>
#include <FramePump.h>
#include <FramePumpCif.h>
#include <FramePumpSipp.h>
#include <FramePumpOv7750.h>
#include <FramePumpOv9282.h>
#include <mvLog.h>

// Maximum number of buses to describe devices
#define MAX_I2C_BUSES_PER_DEV                   (3)
#define MAX_I2C_ADDRESSES_PER_DEV               (8)

extern int32_t i2cbusHandlers[3];

typedef struct
{
    const char* name;
    const rtems_libi2c_drv_t* protocolDesc;
    int32_t busIds[MAX_I2C_BUSES_PER_DEV];
    uint16_t addresses[MAX_I2C_ADDRESSES_PER_DEV];
}BoardDevList;

static BoardDevList i2cDeviceListOv9282CamPair[] =
{
    {
        .name = "ov9282_tracking_left",
        .protocolDesc = &libi2cOv9282ProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {OV9282_I2C_ADDRESS_LEFT, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = "ov9282_tracking_right",
        .protocolDesc = &libi2cOv9282ProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {OV9282_I2C_ADDRESS_RIGHT, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = "ov9282_tracking",
        .protocolDesc = &libi2cOv9282BroadcastProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {OV9282_I2C_ADDRESS_BROADCAST, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};

rtems_status_code brdInitializeCpr(void)
{
    rtems_status_code sc;
    sc = OsDrvCprInit();
    if(sc)
        return -sc;
    sc = OsDrvCprOpen();
    if(sc)
        return -sc;

    sc = OsDrvCprSetupClocks(&appClockConfig);
    if(sc)
        return -sc;
    sc = OsDrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, -1);
    if(sc)
        return -sc;
    sc = OsDrvCprSysDeviceAction(CSS_DOMAIN, DEASSERT_RESET, -1);
    if(sc)
        return -sc;
    sc = OsDrvCprSysDeviceAction(SIPP_DOMAIN, DEASSERT_RESET, -1);
    if(sc)
        return -sc;
    sc = OsDrvCprSysDeviceAction(UPA_DOMAIN, DEASSERT_RESET,
                                 -1);
    if(sc)
        return -sc;
    return RTEMS_SUCCESSFUL;
}

rtems_status_code brdInitializeSensors(int bpp, int binning)
{
    rtems_status_code sc;
    u32 framePumpMajor;
    sc = rtems_io_register_driver( 0, &FramePumpDrvTblName, &framePumpMajor );
    RTEMS_CHECK_SC(sc, "rtems_io_register_driver FramePump\n");

    struct FramePumpVideoDeviceSensor sensorDevice[] =
        {
            {
                .vd = {
                    .name = "/dev/video_tracking_right",
                    .controlDevice = "/dev/i2c.ov7750_tracking_right",
                    .sharedControlDevice = "/dev/i2c.ov7750_tracking",
                    .hwFnc = &sippFunc,
                    .interruptLevel = 10,
                },
                .ctrlNo = MIPI_CTRL_2,
                .gpioa = NULL,
#ifdef CAMERA_8BIT_CONFIG
                .spec = &framePumpOv7750Raw8_camCfg,
#else
                .spec = &framePumpOv7750Raw10_camCfg,
#endif
            },
            {
                .vd = {
                    .name = "/dev/video_tracking_left",
                    .controlDevice = "/dev/i2c.ov7750_tracking_left",
                    .sharedControlDevice = "/dev/i2c.ov7750_tracking",
                    .hwFnc = &cifFunc,
                    .interruptLevel = 10,
                },
                .ctrlNo = MIPI_CTRL_3,
                .gpioa = NULL,
#ifdef CAMERA_8BIT_CONFIG
                .spec = &framePumpOv7750Raw8_camCfg,
#else
                .spec = &framePumpOv7750Raw10_camCfg,
#endif
            },
            {
                .vd = {
                    .name = "/dev/video_tracking_right",
                    .controlDevice = "/dev/i2c.ov9282_tracking_right",
                    .sharedControlDevice = "/dev/i2c.ov9282_tracking",
                    .hwFnc = &sippFunc,
                    .interruptLevel = 10,
                },
                .ctrlNo = MIPI_CTRL_2,
                .gpioa = NULL,
#ifdef CAMERA_8BIT_CONFIG
                .spec = &framePumpOv9282Raw8_camCfg,
#else
                .spec = &framePumpOv9282Raw10_camCfg,
#endif
            },
            {
                .vd = {
                    .name = "/dev/video_tracking_left",
                    .controlDevice = "/dev/i2c.ov9282_tracking_left",
                    .sharedControlDevice = "/dev/i2c.ov9282_tracking",
                    .hwFnc = &cifFunc,
                    .interruptLevel = 10,
                },
                .ctrlNo = MIPI_CTRL_3,
                .gpioa = NULL,
#ifdef CAMERA_8BIT_CONFIG
                .spec = &framePumpOv9282Raw8_camCfg,
#else
                .spec = &framePumpOv9282Raw10_camCfg,
#endif
            },
        };
    if(bpp == 2){
        sensorDevice[0].spec = &framePumpOv7750Raw10_camCfg;
        sensorDevice[1].spec = &framePumpOv7750Raw10_camCfg;
        sensorDevice[2].spec = &framePumpOv9282Raw10_camCfg;
        sensorDevice[3].spec = &framePumpOv9282Raw10_camCfg;
    }else{
        sensorDevice[0].spec = &framePumpOv7750Raw8_camCfg;
        sensorDevice[1].spec = &framePumpOv7750Raw8_camCfg;
        sensorDevice[2].spec = &framePumpOv9282Raw8_camCfg;
        sensorDevice[3].spec = &framePumpOv9282Raw8_camCfg;
    }
    if(bpp == 2){
        if (binning){
            sensorDevice[0].spec = &framePumpOv7750Raw10_camCfg;
            sensorDevice[1].spec = &framePumpOv7750Raw10_camCfg;
            sensorDevice[2].spec = &framePumpOv9282Raw10_camCfg;
            sensorDevice[3].spec = &framePumpOv9282Raw10_camCfg;
        }else{
            sensorDevice[0].spec = &framePumpOv7750Raw10_camCfg;
            sensorDevice[1].spec = &framePumpOv7750Raw10_camCfg;
            sensorDevice[2].spec = &framePumpOv9282Raw10_720P_camCfg;
            sensorDevice[3].spec = &framePumpOv9282Raw10_720P_camCfg;
        }
    }else{
        if (binning){
            sensorDevice[0].spec = &framePumpOv7750Raw8_camCfg;
            sensorDevice[1].spec = &framePumpOv7750Raw8_camCfg;
            sensorDevice[2].spec = &framePumpOv9282Raw8_camCfg;
            sensorDevice[3].spec = &framePumpOv9282Raw8_camCfg;
        }else{
            sensorDevice[0].spec = &framePumpOv7750Raw8_camCfg;
            sensorDevice[1].spec = &framePumpOv7750Raw8_camCfg;
            sensorDevice[2].spec = &framePumpOv9282Raw8_720P_camCfg;
            sensorDevice[3].spec = &framePumpOv9282Raw8_720P_camCfg;
        }
    }
    for (unsigned i = 0 ; i < COUNT_OF(sensorDevice); i++)
    {
        sc = rtems_io_initialize(framePumpMajor, 0, (void*)&sensorDevice[i]);
        if(sc)
        {
            printf("Error initializing %s video device with %s error %s\n",
                   sensorDevice[i].vd.name, sensorDevice[i].vd.controlDevice,
                   strerror(errno));
        }
        else
        {
            printf("Initialized device %s \n", sensorDevice[i].vd.name);
        }
    }

    return sc;
}

static int boardRegisterI2CDevices(BoardDevList* i2c_device_list)
{
    mvLog(MVLOG_INFO, "\nRegistering a list of i2c devices:");
    uint32_t dev_ind = 0;
    int32_t i2c_drv_minor;
    BoardDevList* device = &i2c_device_list[dev_ind];
    int rc = 0;

    while(device->name != NULL)
    {
        mvLog(MVLOG_INFO, "Registering i2c device %s", device->name);
        int32_t i = 0, j = 0;
        while (i < MAX_I2C_BUSES_PER_DEV && device->busIds[i] != -1)
        {
            j = 0;
            while (j < MAX_I2C_ADDRESSES_PER_DEV && device->addresses[j] != 0)
            {
                mvLog(MVLOG_INFO, "Polling address 0x%x on bus %ld", device->addresses[j], device->busIds[i]);
                i2c_drv_minor = rtems_libi2c_register_drv(device->name,
                                                        (rtems_libi2c_drv_t*) device->protocolDesc,
                                                        i2cbusHandlers[device->busIds[i]],
                                                        device->addresses[j]);
                if (i2c_drv_minor >= 0)
                {
                    mvLog(MVLOG_INFO, "i2c device %s registered at bus %ld with address 0x%x",
                           device->name, device->busIds[i], device->addresses[j]);
                    break;
                }
                j++;
            }

            if (j < MAX_I2C_ADDRESSES_PER_DEV && device->addresses[j] != 0)
            {
                // The device was found. No need to poll it on the other buses
                break;
            }
            i++;
        }
        if (i >= MAX_I2C_BUSES_PER_DEV || device->busIds[i] == -1 )
        {
            // This isn't an error, we can use this feature to autodetect devices (i.e  what camera is connected to the board)
            mvLog(MVLOG_INFO, "Device not found");
            rc = -1;
        }
        mvLog(MVLOG_INFO, "----------------------------------------");
        device = &i2c_device_list[++dev_ind];
    }

    return rc;
}

rtems_status_code  brdInitializeBoard(void)
{
    rtems_status_code sc;
    struct stat st;
    OsDrvTimerInit();
    sc = brdInitializeCpr();
#if defined(MA2150)
    tyOsBoard0182Configuration brdConfig[] =
    {
        {
            .type = MV182_CAM_B_I2C_BUS,
            .value = MV182_I2C1
        },
//        {
//            .type = MV182_I2C0_STATE,
//            .value = MV182_I2C_SLAVE,
//        },
        {
            .type = MV182_END,
            .value = 0
        },
    };
    tyOsBrd182ErrorCode rc;
    rc = osBoard0182Initialise((tyOsBoard0182Configuration*)brdConfig);
    if (rc != DRV_OS_BRD_182_DRV_SUCCESS)
    {
        mvLog(MVLOG_FATAL, "Board initialize problem");
        exit(1);
    }
#elif defined(MA2450)
    BoardI2CDevConfiguration i2cConfig[] = {
        {MV212_CAM_B_I2C_BUS, MV212_I2C1},
        { MV212_END, 0 }}; // default config
    BoardConfigDesc brdConfig[] =
    {
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
    mvLog(MVLOG_INFO, "Board 212 started ");
#else
#error Not supported
#endif
    // Note that Cpr was opened by osBoard0182Initialise()

    if(sc)
        return -sc;

    if(stat("/dev/i2c.ov7750_tracking", NULL) != 0)
        boardRegisterI2CDevices(i2cDeviceListOv9282CamPair);

    const char *cdcel_name = "/dev/i2c.cdcel";
    rtems_device_major_number cdcel_major;
    rtems_device_minor_number cdcel_minor;
    rtems_libio_ioctl_args_t cdcelControl;
    cdcelControl.command = EXT_PLL_CFG_148_24_24MHZ;
    if ((sc = stat(cdcel_name, &st)))
    {
        mvLog(MVLOG_ERROR, "CDCEL error");
        exit(-sc);
    }
    cdcel_major = rtems_filesystem_dev_major_t(st.st_rdev);
    cdcel_minor = rtems_filesystem_dev_minor_t(st.st_rdev);

    sc = rtems_io_open(cdcel_major, cdcel_minor, NULL);
    if (sc)
        exit(-sc);
    sc = rtems_io_control(cdcel_major, cdcel_minor, &cdcelControl);
    if (sc)
        exit(-sc);

    DrvDdrInitialise(NULL);
    return RTEMS_SUCCESSFUL;
}
