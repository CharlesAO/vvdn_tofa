///
/// @file      DrvSensorImx208.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Sensor driver implementation for Imx208
///            Platform(s) supported : ma2x8x
///

#include "DrvSensorImx208.h"

#include <DrvSensorDefines.h>
#include <DrvSensor.h>
#include <DrvSensorControl.h>
#include "mv_types.h"
#include "OsCommon.h"
#include <assert.h>

static const u16 imx208_I2Cregs_reset[][2] =
{
    //configuration step 0
    {0x0103,0x01}, // SW Reset
};

static const u16 imx208_binning_I2Cregs_config[][2] =
{
    // -----------start of official config from spreadsheet ------------
    //PLL Setting
    {0x0305, 0x04},
    {0x0307, 0x87},
    {0x303C, 0x4B},
    {0x30A4, 0x01},
    //Mode setting
    {0x0112, 0x0A},
    {0x0113, 0x0A},
    {0x0340, 0x02},
    {0x0341, 0x58},
    {0x0342, 0x04},
    {0x0343, 0x64},
    {0x0344, 0x00},
    {0x0345, 0x00},
    {0x0346, 0x00},
    {0x0347, 0x00},
    {0x0348, 0x07},
    {0x0349, 0x8F},
    {0x034A, 0x04},
    {0x034B, 0x47},
    {0x034C, 0x03},
    {0x034D, 0xC8},
    {0x034E, 0x02},
    {0x034F, 0x24},
    {0x0381, 0x01},
    {0x0383, 0x03},
    {0x0385, 0x01},
    {0x0387, 0x03},
    {0x3048, 0x01},
    {0x304E, 0x0A},
    {0x3050, 0x01},
    {0x309B, 0x08},
    {0x30D5, 0x03},
    {0x3301, 0x00},
    {0x3318, 0x66},
    //Shutter Gain setting
    {0x0202, 0x01}, //(exposure time)
    {0x0203, 0x3b},
    {0x0205, 0x00}, //analog gain
    {0x020E, 0x00}, //digital gain
    {0x0210, 0x00}, //
    {0x0212, 0x00}, //
    {0x0214, 0x00}, //
    {0x3343,0x03}, // Manufacturer prohibited !?!
    {0x30f6,0x00}, // Manufacturer prohibited !?!
};

static const u16 imx208_I2Cregs_config[][2] =
{
    // -----------start of official config from spreadsheet ------------
    //PLL Setting
    {0x0305, 0x04},
    {0x0307, 0x87},
    {0x303C, 0x4B},
    {0x30A4, 0x00},  // 0x02->60fps, 0x00->30fps, 0x01->15fps
    //Mode setting
    {0x0112, 0x0A},
    {0x0113, 0x0A},
    {0x0340, 0x04},
    {0x0341, 0xB0},
    {0x0342, 0x04},
    {0x0343, 0x64},
    {0x0344, 0x00},
    {0x0345, 0x00},
    {0x0346, 0x00},
    {0x0347, 0x00},
    {0x0348, 0x07},
    {0x0349, 0x8F},
    {0x034A, 0x04},
    {0x034B, 0x47},
    {0x034C, 0x07},
    {0x034D, 0x90},
    {0x034E, 0x04},
    {0x034F, 0x48},
    {0x0381, 0x01},
    {0x0383, 0x01},
    {0x0385, 0x01},
    {0x0387, 0x01},
    {0x3048, 0x00},
    {0x304E, 0x0A},
    {0x3050, 0x01},
    {0x309B, 0x00},
    {0x30D5, 0x00},
    {0x3301, 0x00},
    {0x3318, 0x61},
    //Shutter Gain setting
    {0x0202, 0x02}, //(exposure time)
    {0x0203, 0xcb},
    {0x0205, 0xE0}, //analog gain
    {0x020E, 0x00}, //digital gain
    {0x0210, 0x00}, //
    {0x0212, 0x00}, //
    {0x0214, 0x00}, //
    // -----------End of official config from spreadsheet ------------
    // registers obtained out of scoping the dev board
    {0x3343,0x03}, // Manufacturer prohibited !?!
    {0x30f6,0x00}, // Manufacturer prohibited !?!
};

static const u16 imx208_I2Cregs_testmode[][2] =
{
    {0x0600,0x01}, // test mode
};

static const u16 imx208_I2Cregs_enable[][2] =
{
    //configuration step 1 (individual step)
    {0x0100,0x01},  // Enable streaming
};

static const u16 imx208__I2Cregs_disable[][2] =
{
    //configuration step 2 (individual step)
    {0x0100,0x00},  // Disable streaming
};

static const DrvSensorI2CConfigDescriptor drvSensorImx208_binning_I2CConfigSteps[] =
{
    {
        .regValues = imx208_I2Cregs_reset,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_reset),
        .configType = DRV_SENSOR_FULL_CONFIG,
        .delayMs = 1
    },
	{
        .regValues = imx208_binning_I2Cregs_config,
        .numberOfRegs = COUNT_OF(imx208_binning_I2Cregs_config),
        .configType = DRV_SENSOR_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx208_I2Cregs_enable,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_enable),
        .configType = DRV_SENSOR_START_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx208__I2Cregs_disable,
        .numberOfRegs = COUNT_OF(imx208__I2Cregs_disable),
        .configType = DRV_SENSOR_STOP_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx208_I2Cregs_testmode,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_testmode),
        .configType = DRV_SENSOR_COLORBAR,
        .delayMs = 0
    },
};

static const DrvSensorI2CConfigDescriptor drvSensorImx208_I2CConfigSteps[] =
{
    {
        .regValues = imx208_I2Cregs_reset,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_reset),
        .configType = DRV_SENSOR_FULL_CONFIG,
        .delayMs = 1
    },
	{
        .regValues = imx208_I2Cregs_config,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_config),
        .configType = DRV_SENSOR_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx208_I2Cregs_enable,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_enable),
        .configType = DRV_SENSOR_START_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx208__I2Cregs_disable,
        .numberOfRegs = COUNT_OF(imx208__I2Cregs_disable),
        .configType = DRV_SENSOR_STOP_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx208_I2Cregs_testmode,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_testmode),
        .configType = DRV_SENSOR_COLORBAR,
        .delayMs = 0
    },
};

static const struct DrvSensorMipiSpec drvSensorImx208_binning_mipiCfg =
{
    .dataMode     = MIPI_D_MODE_0,
    .dataRateMbps = 405,
    .nbOflanes    = 2,
    .pixelFormat  = CSI_RAW_10
};

static const struct DrvSensorMipiSpec drvSensorImx208_mipiCfg =
{
    .dataMode     = MIPI_D_MODE_0,
    .dataRateMbps = 811,
    .nbOflanes    = 2,
    .pixelFormat  = CSI_RAW_10
};

//RectT            outFOV;
//SizeT            outSize;

const struct DrvSensorCamSpec drvSensorImx208_RAW8_binning_camCfg =
{
    .outFOV                = {0, 0, 968, 548},
    .outSize               = {968, 548},
    .hBackPorch            = 0,
    .hFrontPorch           = 0,
    .vBackPorch            = 0,
    .vFrontPorch           = 0,
    .bytesPerPixel         = 1,     //2 bytes on MIPI interface, stored on 1 byte after SIPP (ONLY!) conversion 10bits -> 8 bits
    .bitsPerPixel          = 10,
    .internalPixelFormat   = RAW8,
    .mipiCfg               = &drvSensorImx208_binning_mipiCfg,
    .idealRefFreq          = 24,
    .nbOfI2CConfigSteps    = COUNT_OF(drvSensorImx208_binning_I2CConfigSteps),
    .i2cConfigSteps        = drvSensorImx208_binning_I2CConfigSteps,
    .regSize               = 1,
};

const struct DrvSensorCamSpec drvSensorimx208_RAW10_binning_camCfg =
{
    .outFOV                = {0, 0, 968, 548},
    .outSize               = {968, 548},
    .hBackPorch            = 0,
    .hFrontPorch           = 0,
    .vBackPorch            = 0,
    .vFrontPorch           = 0,
    .bytesPerPixel         = 2,
    .bitsPerPixel          = 10,
    .internalPixelFormat   = RAW16,
    .mipiCfg               = &drvSensorImx208_binning_mipiCfg,
    .idealRefFreq          = 24,
    .nbOfI2CConfigSteps    = COUNT_OF(drvSensorImx208_binning_I2CConfigSteps),
    .i2cConfigSteps        = drvSensorImx208_binning_I2CConfigSteps,
    .regSize               = 1,
};

const struct DrvSensorCamSpec drvSensorImx208_RAW8_camCfg =
{
    .outFOV                = {0, 0, 1936, 1096},
    .outSize               = {1936, 1096},
    .hBackPorch            = 0,
    .hFrontPorch           = 0,
    .vBackPorch            = 0,
    .vFrontPorch           = 0,
    .bytesPerPixel         = 1,     //2 bytes on MIPI interface, stored on 1 byte after SIPP (ONLY!) conversion 10bits -> 8 bits
    .bitsPerPixel          = 10,
    .internalPixelFormat   = RAW8,
    .mipiCfg               = &drvSensorImx208_mipiCfg,
    .idealRefFreq          = 24,
    .nbOfI2CConfigSteps    = COUNT_OF(drvSensorImx208_I2CConfigSteps),
    .i2cConfigSteps        = drvSensorImx208_I2CConfigSteps,
    .regSize               = 1,
};

const struct DrvSensorCamSpec drvSensorimx208_RAW10_camCfg =
{
    .outFOV                = {0, 12, 1936, 1100},
    .outSize               = {1936, 1088},
    .hBackPorch            = 0,
    .hFrontPorch           = 0,
    .vBackPorch            = 0,
    .vFrontPorch           = 0,
    .bytesPerPixel         = 2,
    .bitsPerPixel          = 10,
    .internalPixelFormat   = RAW16,
    .mipiCfg               = &drvSensorImx208_mipiCfg,
    .idealRefFreq          = 24,
    .nbOfI2CConfigSteps    = COUNT_OF(drvSensorImx208_I2CConfigSteps),
    .i2cConfigSteps        = drvSensorImx208_I2CConfigSteps,
    .regSize               = 1,
};

int drvSensorSensorImx208Config (struct DrvSensorVideoContext *ctx,
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


#define IMX208MODEL_IDENTIFICATION_ADRESS_H  0x0000
#define IMX208MODEL_IDENTIFICATION_ADRESS_L  0x0001


static rtems_status_code i2cImx208Init(rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg)
{
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    u8 modelIdGolden[2] = {0x02, 0x08};
    u8 modelId[2];
    struct i2cStruct_t i2cCtl;
    rtems_libio_ioctl_args_t ctl = {
            .buffer =&i2cCtl,
            .command = CTL_READ_REGISTER,
    };

    i2cCtl.regAddress = IMX208MODEL_IDENTIFICATION_ADRESS_H;
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

    i2cCtl.regAddress = IMX208MODEL_IDENTIFICATION_ADRESS_L;
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
    printf("IMX208 id: %02x %02x found\n", modelId[0], modelId[1]);
    return RTEMS_SUCCESSFUL;
}

static rtems_driver_address_table i2cImx208ProtocolAdrTbl = {
    .initialization_entry = i2cImx208Init,
    .read_entry = dummySuccessDevice,
    .write_entry = dummySuccessDevice,
    .open_entry = dummySuccessDevice,
    .close_entry = dummySuccessDevice,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

rtems_libi2c_drv_t i2cImx208ProtocolDrvTbl = {
    .ops = &i2cImx208ProtocolAdrTbl,
    .size = sizeof(i2cImx208ProtocolDrvTbl),
};

struct DrvSensorControlFunctions drvSensorImx208Control =
{
    .open = drvSensorSensorOpen,
    .close = drvSensorSensorClose,
    .config = drvSensorSensorImx208Config,
    .power_up = drvSensorSensorPowerUp,
    .power_down = drvSensorSensorPowerDown,
};




