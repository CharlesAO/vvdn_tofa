///
/// @file      DrvSensorImx214.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Sensor driver implementation for Imx214
///            Platform(s) supported : ma2x8x
///

#include "DrvSensorImx214.h"

#include <DrvSensorDefines.h>
#include <DrvSensor.h>
#include <DrvSensorControl.h>
#include "mv_types.h"
#include "OsCommon.h"
#include <assert.h>

#ifndef IMX214_13MP_30FPS_LINES
#define IMX214_13MP_30FPS_LINES 0x0C58 //3160 = 3120 + 40 Blank-Lines
#endif

static const u16 imx214_I2Cregs_reset[][2] =
{
    //configuration step 0
    {0x0103,0x01}, // SW Reset
};

static const u16 imx214_binning_I2Cregs_config[][2] =
{
    //External Clock Setting
    { 0x0136, 0x18},
    { 0x0137, 0x00},

    //Global Setting - basic config
    { 0x0101, 0x00},
    { 0x0105, 0x01},
    { 0x0106, 0x01},
    { 0x4601, 0x00},
    { 0x4642, 0x05},
    { 0x6276, 0x00},
    { 0x900E, 0x06},
    { 0xA802, 0x90},
    { 0xA803, 0x11},
    { 0xA804, 0x62},
    { 0xA805, 0x77},
    { 0xA806, 0xAE},
    { 0xA807, 0x34},
    { 0xA808, 0xAE},
    { 0xA809, 0x35},
    { 0xA80A, 0x62},
    { 0xA80B, 0x83},
    { 0xAE33, 0x00},

    //Global Setting - analog settings
    { 0x4174, 0x00},
    { 0x4175, 0x11},
    { 0x4612, 0x29},
    { 0x461B, 0x12},
    { 0x461F, 0x06},
    { 0x4635, 0x07},
    { 0x4637, 0x30},
    { 0x463F, 0x18},
    { 0x4641, 0x0D},
    { 0x465B, 0x12},
    { 0x465F, 0x11},
    { 0x4663, 0x11},
    { 0x4667, 0x0F},
    { 0x466F, 0x0F},
    { 0x470E, 0x09},
    { 0x4909, 0xAB},
    { 0x490B, 0x95},
    { 0x4915, 0x5D},
    { 0x4A5F, 0xFF},
    { 0x4A61, 0xFF},
    { 0x4A73, 0x62},
    { 0x4A85, 0x00},
    { 0x4A87, 0xFF},
    { 0x583C, 0x04},
    { 0x620E, 0x04},
    { 0x6EB2, 0x01},
    { 0x6EB3, 0x00},
    { 0x9300, 0x02},
    // H : 2104
    // V : 1560
    // mode settings
    { 0x0114, 0x01},
    { 0x0220, 0x00},
    { 0x0221, 0x11},
    { 0x0222, 0x01},
    { 0x0340, 0x06},
    { 0x0341, 0x40},
    { 0x0342, 0x13},
    { 0x0343, 0x90},
    { 0x0344, 0x00},
    { 0x0345, 0x00},
    { 0x0346, 0x00},
    { 0x0347, 0x00},
    { 0x0348, 0x10},
    { 0x0349, 0x6F},
    { 0x034A, 0x0C},
    { 0x034B, 0x2F},
    { 0x0381, 0x01},
    { 0x0383, 0x01},
    { 0x0385, 0x01},
    { 0x0387, 0x01},
    { 0x0900, 0x01},
    { 0x0901, 0x22},
    { 0x0902, 0x02},
    { 0x3000, 0x35},
    { 0x3054, 0x01},
    { 0x305C, 0x11},

    //Output Size setting
    { 0x0112, 0x0A},
    { 0x0113, 0x0A},
    { 0x034C, 0x08},
    { 0x034D, 0x38},
    { 0x034E, 0x06},
    { 0x034F, 0x18},
    { 0x0401, 0x00},
    { 0x0404, 0x00},
    { 0x0405, 0x10},
    { 0x0408, 0x00},
    { 0x0409, 0x00},
    { 0x040A, 0x00},
    { 0x040B, 0x00},
    { 0x040C, 0x08},
    { 0x040D, 0x38},
    { 0x040E, 0x06},
    { 0x040F, 0x18},

    //Clock setting
    { 0x0301, 0x05},
    { 0x0303, 0x04},
    { 0x0305, 0x03},
    { 0x0306, 0x00},
    { 0x0307, 0x96},
    { 0x0309, 0x0A},
    { 0x030B, 0x01},
    { 0x0310, 0x00},

    //Data Rate setting
    { 0x0820, 0x09},
    { 0x0821, 0x60},
    { 0x0822, 0x00},
    { 0x0823, 0x00},

    //WaterMark setting
    { 0x3A03, 0x06},
    { 0x3A04, 0x68},
    { 0x3A05, 0x01},

    //Enable setting
    { 0x0B06, 0x01},
    { 0x30A2, 0x00},

    //Test setting},
    { 0x30B4, 0x00},

    //HDR setting},
    { 0x3A02, 0xFF},

    //STATS setting
    { 0x3013, 0x00},

    //Integration Time Setting
    { 0x0202, 0x06},
    { 0x0203, 0x36},
    { 0x0224, 0x01},
    { 0x0225, 0xF4},

    // Gain Setting
    { 0x0204, 0x01},
    { 0x0205, 0x90},
    { 0x020E, 0x01},
    { 0x020F, 0x00},
    { 0x0210, 0x01},
    { 0x0211, 0x00},
    { 0x0212, 0x01},
    { 0x0213, 0x00},
    { 0x0214, 0x01},
    { 0x0215, 0x00},
    { 0x0216, 0x00},
    { 0x0217, 0x00},

    { 0x0600, 0x00},  // test mode off
    //{ 0x0601, 0x02},

    //Analog Setting
    { 0x4170, 0x00},
    { 0x4171, 0x10},
    { 0x4176, 0x00},
    { 0x4177, 0x3C},
    { 0xAE20, 0x04},
    { 0xAE21, 0x5C},


    // Temperature setting -- 10ms after PON - main branch again
    //{ 0x0138, 0x01},
};

static const u16 imx214_I2Cregs_config[][2] = //imx214_4L_4208x3120_RAW10_30Hz_I2CregsCfg
{
    //Global Setting - basic config
    { 0x0100, 0x00},
    { 0x0101, 0x00},
    //Mode setting
    {0x0114, 0x01},
    {0x0220, 0x00},
    {0x0221, 0x11},
    {0x0222, 0x01},
    {0x0340, 0x0C},
    {0x0341, 0x58},
    {0x0342, 0x13},
    {0x0343, 0x90},
    {0x0344, 0x00},
    {0x0345, 0x00},
    {0x0346, 0x00},
    {0x0347, 0x00},
    {0x0348, 0x10},
    {0x0349, 0x6F},
    {0x034A, 0x0C},
    {0x034B, 0x2F},
    {0x0381, 0x01},
    {0x0383, 0x01},
    {0x0385, 0x01},
    {0x0387, 0x01},
    {0x0900, 0x00},
    {0x0901, 0x00},
    {0x0902, 0x00},
    {0x3000, 0x35},
    {0x3054, 0x01},
    {0x305C, 0x11},

    //Output Size setting
    {0x0112, 0x0A},
    {0x0113, 0x0A},
    {0x034C, 0x10},
    {0x034D, 0x70},
    {0x034E, 0x0C},
    {0x034F, 0x30},
    {0x0401, 0x00},
    {0x0404, 0x00},
    {0x0405, 0x10},
    {0x0408, 0x00},
    {0x0409, 0x00},
    {0x040A, 0x00},
    {0x040B, 0x00},
    {0x040C, 0x10},
    {0x040D, 0x70},
    {0x040E, 0x0C},
    {0x040F, 0x30},

    //Clock setting
    {0x0301, 0x05},
    {0x0303, 0x04},
    {0x0305, 0x03},
    {0x0306, 0x00},
    {0x0307, 0x96},
    {0x0309, 0x0A},
    {0x030B, 0x01},
    {0x0310, 0x00},

    //Data Rate setting
    {0x0820, 0x09},
    {0x0821, 0x60},
    {0x0822, 0x00},
    {0x0823, 0x00},

    //WaterMark setting
    {0x3A03, 0x08},
    {0x3A04, 0x70},
    {0x3A05, 0x02},

    //Enable setting
    {0x0B06, 0x01},
    {0x30A2, 0x00},

    //Test setting
    {0x30B4, 0x00},

    //HDR setting
    {0x3A02, 0xFF},

    //STATS setting
    {0x3013, 0x00},

    //Integration Time Setting
    {0x0202, 0x0C},
    {0x0203, 0x4E},
    {0x0224, 0x01},
    {0x0225, 0xF4},

    //Gain Setting
    {0x0204, 0x01},
    {0x0205, 0x90},
    {0x020E, 0x01},
    {0x020F, 0x00},
    {0x0210, 0x01},
    {0x0211, 0x00},
    {0x0212, 0x01},
    {0x0213, 0x00},
    {0x0214, 0x01},
    {0x0215, 0x00},
    {0x0216, 0x00},
    {0x0217, 0x00},

    //Analog Setting
    {0x4170, 0x00},
    {0x4171, 0x10},
    {0x4176, 0x00},
    {0x4177, 0x3C},
    {0xAE20, 0x04},
    {0xAE21, 0x5C},

};

static const u16 imx214_I2Cregs_testmode[][2] =
{
    {0x0600,0x01}, // test mode
};

static const u16 imx214_I2Cregs_enable[][2] =
{
    //configuration step 1 (individual step)
    {0x0100,0x01},  // Enable streaming
};

static const u16 imx214__I2Cregs_disable[][2] =
{
    //configuration step 2 (individual step)
    {0x0100,0x00},  // Disable streaming
};

static const DrvSensorI2CConfigDescriptor drvSensorImx214_binning_I2CConfigSteps[] =
{
    {
        .regValues = imx214_I2Cregs_reset,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_reset),
        .configType = DRV_SENSOR_FULL_CONFIG,
        .delayMs = 1
    },
	{
        .regValues = imx214_binning_I2Cregs_config,
        .numberOfRegs = COUNT_OF(imx214_binning_I2Cregs_config),
        .configType = DRV_SENSOR_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx214_I2Cregs_enable,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_enable),
        .configType = DRV_SENSOR_START_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx214__I2Cregs_disable,
        .numberOfRegs = COUNT_OF(imx214__I2Cregs_disable),
        .configType = DRV_SENSOR_STOP_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx214_I2Cregs_testmode,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_testmode),
        .configType = DRV_SENSOR_COLORBAR,
        .delayMs = 0
    },
};

static const DrvSensorI2CConfigDescriptor drvSensorImx214_I2CConfigSteps[] =
{
    {
        .regValues = imx214_I2Cregs_reset,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_reset),
        .configType = DRV_SENSOR_FULL_CONFIG,
        .delayMs = 1
    },
	{
        .regValues = imx214_I2Cregs_config,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_config),
        .configType = DRV_SENSOR_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx214_I2Cregs_enable,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_enable),
        .configType = DRV_SENSOR_START_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx214__I2Cregs_disable,
        .numberOfRegs = COUNT_OF(imx214__I2Cregs_disable),
        .configType = DRV_SENSOR_STOP_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx214_I2Cregs_testmode,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_testmode),
        .configType = DRV_SENSOR_COLORBAR,
        .delayMs = 0
    },
};

static const struct DrvSensorMipiSpec drvSensorImx214_binning_mipiCfg =
{
    .dataMode     = MIPI_D_MODE_0,
    .dataRateMbps = 1200,
    .nbOflanes    = 2,
    .pixelFormat  = CSI_RAW_10
};

static const struct DrvSensorMipiSpec drvSensorImx214_mipiCfg =
{
    .dataMode     = MIPI_D_MODE_0,
    .dataRateMbps = 1200,
    .nbOflanes    = 2,
    .pixelFormat  = CSI_RAW_10
};


const struct DrvSensorCamSpec drvSensorimx214_RAW10_binning_camCfg =
{
    .outFOV                = {8, 4, 2104, 1564},
    .outSize               = {2104-8, 1560},
    .hBackPorch            = 0,
    .hFrontPorch           = 0,
    .vBackPorch            = 0,
    .vFrontPorch           = 0,
    .bytesPerPixel         = 2,
    .bitsPerPixel          = 10,
    .internalPixelFormat   = RAW16,
    .mipiCfg               = &drvSensorImx214_binning_mipiCfg,
    .idealRefFreq          = 24,
    .nbOfI2CConfigSteps    = COUNT_OF(drvSensorImx214_binning_I2CConfigSteps),
    .i2cConfigSteps        = drvSensorImx214_binning_I2CConfigSteps,
    .regSize               = 1,
};


const struct DrvSensorCamSpec drvSensorimx214_RAW10_camCfg =
{
    .outFOV                = {16, 8, 4208, 3128},
    .outSize               = {4208-16, 3128-8},
    .hBackPorch            = 0,
    .hFrontPorch           = 0,
    .vBackPorch            = 0,
    .vFrontPorch           = 0,
    .bytesPerPixel         = 2,
    .bitsPerPixel          = 10,
    .internalPixelFormat   = RAW16,
    .mipiCfg               = &drvSensorImx214_mipiCfg,
    .idealRefFreq          = 24,
    .nbOfI2CConfigSteps    = COUNT_OF(drvSensorImx214_I2CConfigSteps),
    .i2cConfigSteps        = drvSensorImx214_I2CConfigSteps,
    .regSize               = 1,
};

int drvSensorSensorImx214Config (struct DrvSensorVideoContext *ctx,
                                 struct DrvSensorCotrolConfig *control)
{
//    int fd;
//
//    if(FP_IS_SHARED_CTL(control->cfgType))
//    {
//        fd = ctx->ctlFdShared;
//    }
//    else
//    {
//        fd = ctx->ctlFd;
//    }

    switch (FP_STRIP_CTL(control->cfgType))
    {
//    case DRV_SENSOR_CONFIG_SEND_EXPOSURE:
//        return drvSensorSensorOv7750SetExposureUs(ctx, control, fd, *(u32*)control->buffer);
//    case DRV_SENSOR_CONFIG_SEND_GAIN:
//        return drvSensorSensorOv7750SetGainFloat(ctx, control, fd, *(float*)control->buffer);
//    case DRV_SENSOR_CONFIG_GET_READOUT:
//        return drvSensorSensorOv7750GetReadoutUs(ctx, control, (u32*)control->buffer);
//    case DRV_SENSOR_GET_FRAME_RATE:
//        return drvSensorSensorOv7750GetFrameRate(ctx, control, fd, (u32*)control->buffer);
//    case DRV_SENSOR_SET_FRAME_RATE:
//        return drvSensorSensorOv7750SetFrameRate(ctx, control, fd, *(u32*)control->buffer);
//    case DRV_SENSOR_CONFIG_FLIP:
//        return drvSensorSensorOv7750Flip(ctx, control, *(u32*)control->buffer);
//    case DRV_SENSOR_CONFIG_MIRROR:
//        return drvSensorSensorOv7750Mirror(ctx, control, *(u32*)control->buffer);
//    case DRV_SENSOR_GET_TESTPATERN_CRC:
//        *(u32*)control->buffer = OV7750_TESTPATTERN_CRC_10BIT;
//        return 0;
    default:
        return drvSensorSensorConfig(ctx, control);
    }

    return 0;
}

static rtems_status_code dummySuccessDevice(rtems_device_major_number major,
                                            rtems_device_minor_number minor,
                                            void *arg)
{
  UNUSED(major);
  UNUSED(minor);
  UNUSED(arg);
    return RTEMS_SUCCESSFUL;
}


#define IMx214MODEL_IDENTIFICATION_ADRESS_H  0x0000
#define IMx214MODEL_IDENTIFICATION_ADRESS_L  0x0001


static rtems_status_code i2cImx214Init(rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg)
{
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    u8 modelIdGolden[2] = {0x00, 0x00};
    u8 modelId[2];
    struct i2cStruct_t i2cCtl;
    rtems_libio_ioctl_args_t ctl = {
            .buffer =&i2cCtl,
            .command = CTL_READ_REGISTER,
    };

    i2cCtl.regAddress = IMx214MODEL_IDENTIFICATION_ADRESS_H;
    i2cCtl.regValue = &modelId[0];
    i2cCtl.wordCount = 1;

    sc = i2cDeviceGenericControl16BitReg8BitVal(
            major,
            minor,
            &ctl);

    if (sc != RTEMS_SUCCESSFUL){
              return sc;
          }
    if(ctl.ioctl_return != i2cCtl.wordCount) {
        return -1;
    }

    i2cCtl.regAddress = IMx214MODEL_IDENTIFICATION_ADRESS_L;
    i2cCtl.regValue = &modelId[1];
    i2cCtl.wordCount = 1;
    sc = i2cDeviceGenericControl16BitReg8BitVal(
            major,
            minor,
            &ctl);

    if (sc != RTEMS_SUCCESSFUL){
              return sc;
          }
    if(ctl.ioctl_return != i2cCtl.wordCount) {
        return -1;
    }

    if(modelIdGolden[0] != modelId[0] ||
            modelIdGolden[1] != modelId[1])
    {
        printf("model id: %02x %02x\n", modelId[0], modelId[1]);
        return -RTEMS_UNSATISFIED;
    }
    printf("IMX214 id: %02x %02x found\n", modelId[0], modelId[1]);
    return RTEMS_SUCCESSFUL;
}

static rtems_driver_address_table i2cImx214ProtocolAdrTbl = {
    .initialization_entry = i2cImx214Init,
    .read_entry = dummySuccessDevice,
    .write_entry = dummySuccessDevice,
    .open_entry = dummySuccessDevice,
    .close_entry = dummySuccessDevice,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

rtems_libi2c_drv_t i2cImx214ProtocolDrvTbl = {
    .ops = &i2cImx214ProtocolAdrTbl,
    .size = sizeof(i2cImx214ProtocolDrvTbl),
};

struct DrvSensorControlFunctions drvSensorImx214Control =
{
    .open = drvSensorSensorOpen,
    .close = drvSensorSensorClose,
    .config = drvSensorSensorImx214Config,
    .power_up = drvSensorSensorPowerUp,
    .power_down = drvSensorSensorPowerDown,
};




