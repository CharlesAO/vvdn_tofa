///
/// @file      DrvSensorOv7251.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     DrvSensor main API implementation
///            Platform(s) supported : ma2x8x
///            640x480p @ 30 fps RAW10
///

#include "DrvSensorOv7251.h"

#include <DrvSensorDefines.h>
#include <mv_types.h>
#include <mvMacros.h>
#include "swcFrameTypes.h"

#include <DrvSensor.h>
#include <DrvSensorControl.h>

#include <OsDrvI2cMyr2.h>
#include <sys/ioctl.h>

#include <stdio.h>

#define OV7750_TESTPATTERN_CRC_8BIT  (0x2C83B24E)
#define OV7750_TESTPATTERN_CRC_10BIT (0x95873BC3)

#define OV7750_SC_MODE_SELECT           0x0100
#define OV7750_SC_SOFTWARE_RESET        0x0103
#define OV7750_SC_FAST_STANDBY_CTRL     0x0106
#define OV7750_SC_SCCB_ID1              0x0109
#define OV7750_SC_CHIP_ID_HIGH          0x300A
#define OV7750_SC_CHIP_ID_LOW           0x300B
#define OV7750_SC_SCCB_ID2              0x302B

#define OV7750_X_OUTPUT_SIZE_H          0x3808
#define OV7750_X_OUTPUT_SIZE_L          0x3809
#define OV7750_Y_OUTPUT_SIZE_H          0x380A
#define OV7750_Y_OUTPUT_SIZE_L          0x380B
#define OV7750_HTS_H                    0x380C
#define OV7750_HTS_L                    0x380D
#define OV7750_VTS_H                    0x380E
#define OV7750_VTS_L                    0x380F

#define OV7750_EXPOSURE_19_16           0x3502
#define OV7750_EXPOSURE_15_8            0x3501
#define OV7750_EXPOSURE_7_0             0x3500
#define OV7750_GAIN_AGC_ADJ_1			0x350A
#define OV7750_GAIN_AGC_ADJ_2			0x350B
#define OV7750_MAN_SNR_GAIN_1			0x3504
#define OV7750_MAN_SNR_GAIN_2			0x3505

#define OV7750_AWB_RED_GAIN_H           0x3400
#define OV7750_AWB_RED_GAIN_L           0x3401
#define OV7750_AWB_GRN_GAIN_H           0x3402
#define OV7750_AWB_GRN_GAIN_L           0x3403
#define OV7750_AWB_BLU_GAIN_H           0x3404
#define OV7750_AWB_BLU_GAIN_L           0x3405

/// PLL Registers
#define OV7750_PLL1_PRE                 0x30B4
#define OV7750_PLL1_MULTI               0x30B3
#define OV7750_PLL1_DIV                 0x30B1
#define OV7750_PLL1_PIX_DIV             0x30B0
#define OV7750_PLL1_MIPI_DIV            0x30B5
#define OV7750_PLL2_PRE                 0x3098
#define OV7750_PLL2_MULTI               0x3099
#define OV7750_PLL2_DIV                 0x309D
#define OV7750_PLL2_SYS_DIV             0x309A
#define OV7750_PLL2_ADC                 0x309B

/// Group Hold
#define OV7750_GROUP_ACCESS 0x3208
#define OV7750_GHP_G0_START 0x0     /// Group 0 - Start hold
#define OV7750_GPH_G0_END   0x10    /// Group 0 - End hold
#define OV7750_GPH_G1_START 0x1     /// Group 1 - Start hold
#define OV7750_GPH_G1_END   0x11    /// Group 1 - End hold

#define OV7750_GROUP_ACTIVE 0x320D

static const u16 cfgLeft[][2] =
{
                {0x3005, 0x08}, // STROBE_out // FSIN_in
                {0x3b81, 0xFF}, // STROBE PATTERN
};

static const u16 cfgRight[][2] =
{
                {0x3005, 0x08}, // STROBE_out  // FSIN in
                {0x3027, 0x00},
                {0x3b81, 0xFF}, // STROBE PATTERN
};


static const u16 fullConfig[][2] =
{
                //    {0x0103, 0x01},
                {0x0100, 0x00},

                {0x3666, 0x00},
                {0x3823, 0x00},
                {0x3824, 0x01},
                {0x3825, 0x07},
                {0x3826, 0x00},
                {0x3827, 0x00},

                {0x3013, 0x12},
                {0x3014, 0x04},
                {0x3016, 0xf0},
                {0x3017, 0xf0},
                {0x3018, 0xf0},
                {0x301a, 0xf0},
                {0x301b, 0xf0},
                {0x301c, 0xf0},
                {0x3023, 0x07},
                {0x3037, 0xf0},
                {0x3098, 0x04},
                {0x3099, 0x28},
                {0x309a, 0x05},
                {0x309b, 0x04},
                {0x30b0, 0x0a},
                {0x30b1, 0x01},
                {0x30b3, 0x64},
                {0x30b4, 0x03},
                {0x30b5, 0x05},
                {0x3106, 0x12},
                {0x3500, 0x00}, // integration time
                {0x3501, 0x2f}, // integration time
                {0x3502, 0x80}, // integration time
                {0x3503, 0x07},
                {0x3509, 0x12},
                {0x350b, 0x10},
                {0x3600, 0x1c},
                {0x3602, 0x62},
                {0x3620, 0xb7},
                {0x3622, 0x04},
                {0x3626, 0x21},
                {0x3627, 0x30},
                {0x3634, 0x41},
                {0x3636, 0x00},
                {0x3662, 0x01},
                {0x3664, 0xf0},
                {0x366a, 0x00},
                {0x366b, 0x50},
                {0x3705, 0xc1},
                {0x3709, 0x40},
                {0x373c, 0x08},
                {0x3742, 0x00},
                {0x3788, 0x00},
                {0x37a8, 0x01},
                {0x37a9, 0xc0},
                {0x3800, 0x00},
                {0x3801, 0x04},
                {0x3802, 0x00},
                {0x3803, 0x04},
                {0x3804, 0x02},
                {0x3805, 0x8b},
                {0x3806, 0x01},
                {0x3807, 0xeb},
                {0x3808, 0x02},
                {0x3809, 0x80},
                {0x380a, 0x01},
                {0x380b, 0xe0},
                {0x380c, 0x03},
                {0x380d, 0xa0},
                {0x380e, 0x06},  // VBLANKING HIGH
                {0x380f, 0xBC},  // VBLANKING LOW
                {0x3810, 0x00},
                {0x3811, 0x04},
                {0x3812, 0x00},
                {0x3813, 0x05},
                {0x3814, 0x11},
                {0x3815, 0x11},
                {0x3820, 0x00}, // vertical flip bit 4
                {0x3821, 0x00}, // horizontal mirror bit 4
                {0x382f, 0xc4},
                {0x3832, 0xff},
                {0x3833, 0xff},
                {0x3834, 0x00},
                {0x3835, 0x05},
                {0x3837, 0x00},
                {0x3b80, 0x00},
                {0x3b82, 0x10},
                {0x3b83, 0x00},
                {0x3b84, 0x08},
                {0x3b85, 0x00},
                {0x3b86, 0x01},
                {0x3b87, 0x00},

                {0x3b88, 0x00},
                {0x3b89, 0x00},
                {0x3b8a, 0x02},
                {0x3b8b, 0x00},

                {0x3b8c, 0x00}, // strobe span
                {0x3b8d, 0x00}, // strobe span
                {0x3b8e, 0x00}, // strobe span
                {0x3b8f, 0x40}, // strobe span

                {0x3b94, 0x05},
                {0x3b95, 0xf2},
                {0x3b96, 0x40},
                {0x3c00, 0x89},
                {0x3c01, 0xab},
                {0x3c02, 0x01},
                {0x3c03, 0x00},
                {0x3c04, 0x00},
                {0x3c05, 0x03},
                {0x3c06, 0x00},
                {0x3c07, 0x05},
                {0x3c0c, 0x00},
                {0x3c0d, 0x00},
                {0x3c0e, 0x00},
                {0x3c0f, 0x00},
                {0x4001, 0x42},
                {0x4004, 0x04},
                {0x404e, 0x01},
                {0x4300, 0xff},
                {0x4301, 0x00},
                {0x4600, 0x00},
                {0x4601, 0x4e},
                {0x4801, 0x0f},
                {0x4806, 0x0f},
                {0x4837, 0x19},
                {0x4a0d, 0x00},
                {0x5000, 0x85},
};

static const u16 drvSensorOv7251Raw10SensorColorbarCfg[][2] =
{
                {0x5e00, 0x8c},
};

static const u16 drvSensorOv7251Raw10SensorStartStreaming[][2] =
{
                {0x0100, 0x01},
};

static const u16 drvSensorOv7251Raw10SensorStopStreaming[][2] =
{
                {0x0100, 0x00},
};

static const u16 drvSensorOv7251Raw10SensorIntegrationRegistere[][2] =
{
                {0x3502, 0x00},
                {0x3501, 0x00},
                {0x3500, 0x00},
};

static const u16 drvSensorOv7251Raw10SensorGainRegisters[][2] =
{
                {0x350B, 0x00},
                {0x350A, 0x00},
};

static const DrvSensorI2CConfigDescriptor drvSensorOv7251Raw10_I2CConfigSteps[] =
{
                {
                                .regValues = fullConfig,
                                .numberOfRegs = COUNT_OF(fullConfig),
                                .configType = DRV_SENSOR_FULL_CONFIG,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorColorbarCfg,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorColorbarCfg),
                                .configType = DRV_SENSOR_COLORBAR,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorStartStreaming,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorStartStreaming),
                                .configType = DRV_SENSOR_START_STREAMING,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorStopStreaming,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorStopStreaming),
                                .configType = DRV_SENSOR_STOP_STREAMING,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorIntegrationRegistere,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorIntegrationRegistere),
                                .configType = DRV_SENSOR_INTEGRATION_TIME,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorGainRegisters,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorGainRegisters),
                                .configType = DRV_SENSOR_GAIN,
                                .delayMs = 0
                },
};

static const DrvSensorI2CConfigDescriptor drvSensorOv7251Raw10_right_I2CConfigSteps[] =
{
                {
                                .regValues = fullConfig,
                                .numberOfRegs = COUNT_OF(fullConfig),
                                .configType = DRV_SENSOR_FULL_CONFIG,
                                .delayMs = 0
                },
                {
                                .regValues = cfgRight,
                                .numberOfRegs = COUNT_OF(cfgRight),
                                .configType = DRV_SENSOR_FULL_CONFIG,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorColorbarCfg,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorColorbarCfg),
                                .configType = DRV_SENSOR_COLORBAR,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorStartStreaming,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorStartStreaming),
                                .configType = DRV_SENSOR_START_STREAMING,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorStopStreaming,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorStopStreaming),
                                .configType = DRV_SENSOR_STOP_STREAMING,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorIntegrationRegistere,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorIntegrationRegistere),
                                .configType = DRV_SENSOR_INTEGRATION_TIME,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorGainRegisters,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorGainRegisters),
                                .configType = DRV_SENSOR_GAIN,
                                .delayMs = 0
                },
};

static const DrvSensorI2CConfigDescriptor drvSensorOv7251Raw10_left_I2CConfigSteps[] =
{
                {
                                .regValues = fullConfig,
                                .numberOfRegs = COUNT_OF(fullConfig),
                                .configType = DRV_SENSOR_FULL_CONFIG,
                                .delayMs = 0
                },
                {
                                .regValues = cfgLeft,
                                .numberOfRegs = COUNT_OF(cfgLeft),
                                .configType = DRV_SENSOR_FULL_CONFIG,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorColorbarCfg,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorColorbarCfg),
                                .configType = DRV_SENSOR_COLORBAR,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorStartStreaming,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorStartStreaming),
                                .configType = DRV_SENSOR_START_STREAMING,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorStopStreaming,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorStopStreaming),
                                .configType = DRV_SENSOR_STOP_STREAMING,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorIntegrationRegistere,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorIntegrationRegistere),
                                .configType = DRV_SENSOR_INTEGRATION_TIME,
                                .delayMs = 0
                },
                {
                                .regValues = drvSensorOv7251Raw10SensorGainRegisters,
                                .numberOfRegs = COUNT_OF(drvSensorOv7251Raw10SensorGainRegisters),
                                .configType = DRV_SENSOR_GAIN,
                                .delayMs = 0
                },
};


static const struct DrvSensorMipiSpec drvSensorOv7251Raw10_mipiCfg =
{
                .dataMode     = MIPI_D_MODE_0,
                .dataRateMbps = 600,
                .nbOflanes    = 1,
                .pixelFormat  = CSI_RAW_10
};

const struct DrvSensorCamSpec drvSensorOv7251Raw8_camCfg =
{
                .outFOV                = {0, 0, OV7750_WIDTH, OV7750_HEIGHT},
                .outSize               = {OV7750_WIDTH, OV7750_HEIGHT},
                .hBackPorch            = 0,
                .hFrontPorch           = 0,
                .vBackPorch            = 0,
                .vFrontPorch           = 0,
                .bytesPerPixel         = 1,     //2 bytes on MIPI interface, stored on 1 byte after SIPP (ONLY!) conversion 10bits -> 8 bits
                .bitsPerPixel          = 10,
                .internalPixelFormat   = RAW8,
                .mipiCfg               = &drvSensorOv7251Raw10_mipiCfg,
                .idealRefFreq          = 24,
                .nbOfI2CConfigSteps    = COUNT_OF(drvSensorOv7251Raw10_I2CConfigSteps),
                .i2cConfigSteps        = drvSensorOv7251Raw10_I2CConfigSteps,
                .regSize               = 1,
};

const struct DrvSensorCamSpec drvSensorOv7251Raw10_camCfg =
{
                .outFOV                = {0, 0, OV7750_WIDTH, OV7750_HEIGHT},
                .outSize               = {OV7750_WIDTH, OV7750_HEIGHT},
                .hBackPorch            = 0,
                .hFrontPorch           = 0,
                .vBackPorch            = 0,
                .vFrontPorch           = 0,
                .bytesPerPixel         = 2,     //2 bytes on MIPI interface, stored on 1 byte after SIPP (ONLY!) conversion 10bits -> 8 bits
                .bitsPerPixel          = 10,
                .internalPixelFormat   = RAW16,
                .mipiCfg               = &drvSensorOv7251Raw10_mipiCfg,
                .idealRefFreq          = 24,
                .nbOfI2CConfigSteps    = COUNT_OF(drvSensorOv7251Raw10_I2CConfigSteps),
                .i2cConfigSteps        = drvSensorOv7251Raw10_I2CConfigSteps,
                .regSize               = 1,
};

const struct DrvSensorCamSpec drvSensorOv7251Raw10_right_camCfg =
{
                .outFOV                = {0, 0, OV7750_WIDTH, OV7750_HEIGHT},
                .outSize               = {OV7750_WIDTH, OV7750_HEIGHT},
                .hBackPorch            = 0,
                .hFrontPorch           = 0,
                .vBackPorch            = 0,
                .vFrontPorch           = 0,
                .bytesPerPixel         = 2,     //2 bytes on MIPI interface, stored on 1 byte after SIPP (ONLY!) conversion 10bits -> 8 bits
                .bitsPerPixel          = 10,
                .internalPixelFormat   = RAW16,
                .mipiCfg               = &drvSensorOv7251Raw10_mipiCfg,
                .idealRefFreq          = 24,
                .nbOfI2CConfigSteps    = COUNT_OF(drvSensorOv7251Raw10_right_I2CConfigSteps),
                .i2cConfigSteps        = drvSensorOv7251Raw10_right_I2CConfigSteps,
                .regSize               = 1,
};

const struct DrvSensorCamSpec drvSensorOv7251Raw10_left_camCfg =
{
                .outFOV                = {0, 0, OV7750_WIDTH, OV7750_HEIGHT},
                .outSize               = {OV7750_WIDTH, OV7750_HEIGHT},
                .hBackPorch            = 0,
                .hFrontPorch           = 0,
                .vBackPorch            = 0,
                .vFrontPorch           = 0,
                .bytesPerPixel         = 2,     //2 bytes on MIPI interface, stored on 1 byte after SIPP (ONLY!) conversion 10bits -> 8 bits
                .bitsPerPixel          = 10,
                .internalPixelFormat   = RAW16,
                .mipiCfg               = &drvSensorOv7251Raw10_mipiCfg,
                .idealRefFreq          = 24,
                .nbOfI2CConfigSteps    = COUNT_OF(drvSensorOv7251Raw10_left_I2CConfigSteps),
                .i2cConfigSteps        = drvSensorOv7251Raw10_left_I2CConfigSteps,
                .regSize               = 1,
};

const struct DrvSensorCamSpec drvSensorOv7251Raw8_right_camCfg =
{
                .outFOV                = {0, 0, OV7750_WIDTH, OV7750_HEIGHT},
                .outSize               = {OV7750_WIDTH, OV7750_HEIGHT},
                .hBackPorch            = 0,
                .hFrontPorch           = 0,
                .vBackPorch            = 0,
                .vFrontPorch           = 0,
                .bytesPerPixel         = 1,     //2 bytes on MIPI interface, stored on 1 byte after SIPP (ONLY!) conversion 10bits -> 8 bits
                .bitsPerPixel          = 10,
                .internalPixelFormat   = RAW16,
                .mipiCfg               = &drvSensorOv7251Raw10_mipiCfg,
                .idealRefFreq          = 24,
                .nbOfI2CConfigSteps    = COUNT_OF(drvSensorOv7251Raw10_right_I2CConfigSteps),
                .i2cConfigSteps        = drvSensorOv7251Raw10_right_I2CConfigSteps,
                .regSize               = 1,
};

const struct DrvSensorCamSpec drvSensorOv7251Raw8_left_camCfg =
{
                .outFOV                = {0, 0, OV7750_WIDTH, OV7750_HEIGHT},
                .outSize               = {OV7750_WIDTH, OV7750_HEIGHT},
                .hBackPorch            = 0,
                .hFrontPorch           = 0,
                .vBackPorch            = 0,
                .vFrontPorch           = 0,
                .bytesPerPixel         = 1,     //2 bytes on MIPI interface, stored on 1 byte after SIPP (ONLY!) conversion 10bits -> 8 bits
                .bitsPerPixel          = 10,
                .internalPixelFormat   = RAW16,
                .mipiCfg               = &drvSensorOv7251Raw10_mipiCfg,
                .idealRefFreq          = 24,
                .nbOfI2CConfigSteps    = COUNT_OF(drvSensorOv7251Raw10_left_I2CConfigSteps),
                .i2cConfigSteps        = drvSensorOv7251Raw10_left_I2CConfigSteps,
                .regSize               = 1,
};

static int ov7500SendArrayToRegister(int fd, u16 reg, u8 * array, int count)
{
    struct i2cStruct_t i2cCtl;
    int i, rval;

    if(array == NULL)
        return RTEMS_INVALID_ADDRESS;

    for(i = 0; i < count ; i++)
    {
        i2cCtl.regAddress = reg - i;
        i2cCtl.regValue = &array[i];
        i2cCtl.wordCount = 1;

        rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
        if(rval != i2cCtl.wordCount)
        {
            printf("I2C sensor configuration failed at address 0x%x \n"
                            "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
            i-=1;
            continue;
        }
    }
    return 0;
}

static int drvSensorSensorOv7750Flip( struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                int flip)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl;
    int rval;
    u8 regVal = 0;

    (void)control;

    i2cCtl.regAddress = 0x3820;
    i2cCtl.regValue = &regVal;
    i2cCtl.wordCount = 1;

    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        printf("I2C sensor read failed at address 0x%x \n"
                        "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return -1;
    }

    if(flip) {
        *(u8*)i2cCtl.regValue |= 0x04;
    }
    else {
        *(u8*)i2cCtl.regValue &= ~0x04;
    }

    rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        printf("I2C sensor configuration failed at address 0x%x \n"
                        "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return -1;
    }

    return 0;
}

static int drvSensorSensorOv7750Mirror( struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                int mirror)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl;
    int rval;
    u8 regVal = 0;

    (void)control;

    i2cCtl.regAddress = 0x3821;
    i2cCtl.regValue = &regVal;
    i2cCtl.wordCount = 1;

    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        printf("I2C sensor read failed at address 0x%x \n"
                        "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return -1;
    }

    if(mirror)
    {
        *(u8*)i2cCtl.regValue |= 0x04;
    }
    else
    {
        *(u8*)i2cCtl.regValue &= ~0x04;
    }

    rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        printf("I2C sensor configuration failed at address 0x%x \n"
                        "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return -1;
    }

    return 0;
}

static int drvSensorSensorOv7750GetHTS ( struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                s32 *hts)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl;
    u8 readbyte[2];
    s32 rval;

    (void)control;

    i2cCtl.regAddress = OV7750_HTS_H;
    i2cCtl.regValue = &readbyte[0];
    i2cCtl.wordCount = 1;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }

    i2cCtl.regAddress = OV7750_HTS_L;
    i2cCtl.regValue = &readbyte[1];
    i2cCtl.wordCount = 1;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }

    *hts = (readbyte[0]<<8) | readbyte[1];
    return 0;
}

static float pll1PreDivLT[16] =
{
                1.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.5f, 6.0f, 2.5f, 8.0f,
                -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f
};

static int __attribute__((unused)) drvSensorSensorOv7750GetPclk(
                struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                float *pclk)
                {
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl;
    u8 readbyte[2];
    fp32 refClk = (fp32)ctx->spec->idealRefFreq;
    fp32 pll1Clk, pll1Multi, pll1PreDiv, pll1Div, pll1Pclk;
    s32 rval;

    (void)control;

    i2cCtl.regValue = &readbyte[0];
    i2cCtl.wordCount = 1;

    /// Get PLL1_pre_divider (0x30B4[3:0])
    i2cCtl.regAddress = OV7750_PLL1_PRE;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    else
    {
        pll1PreDiv = pll1PreDivLT[readbyte[0] & 0x0F];
    }
    /// Get PLL1_multiplier (0x30B3)
    i2cCtl.regAddress = OV7750_PLL1_MULTI;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    pll1Multi = (fp32)(readbyte[0] & 0xFF);

    /// Get PLL1_divider (0x30b1[4:0])
    i2cCtl.regAddress = OV7750_PLL1_DIV;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    pll1Div = (fp32)(readbyte[0] & 0x1F); /// Should be >=1 and <= 16

    /// Get PLL1_pix_divider (0x30B0[3:0])
    i2cCtl.regAddress = OV7750_PLL1_PIX_DIV;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    pll1Pclk = (fp32) (readbyte[0] & 0x0F); /// Should be 0x08 or 0x0A

    /// Compute the pixel clock
    pll1Clk = ((refClk / pll1PreDiv) * pll1Multi) / pll1Div;

    *pclk = pll1Clk / pll1Pclk;

    return 0;
                }
static float pll2PreDivLT[32] =
{
                1.0f, // 0x00
                1.0f, // 0x01
                1.0f, // 0x02
                1.5f, // 0x03
                2.0f, // 0x04
                2.5f, // 0x05
                3.0f, // 0x06
                1.0f, // 0x07
                4.0f, // 0x08
                1.0f, // 0x09
                1.0f, // 0x0A
                1.0f, // 0x0B
                6.0f, // 0x0C
                1.0f, // 0x0D
                1.0f, // 0x0E
                1.0f, // 0x0F
                8.0f, // 0x10
                1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
};

static float pll2SysDivLT[16] =
{
                -1.0f, // 0x00
                -1.0f, // 0x01
                4.0f, // 0x02
                6.0f, // 0x03
                8.0f, // 0x04
                10.0f, // 0x05
                12.0f, // 0x06
                14.0f, // 0x07
                16.0f, // 0x08
                18.0f, // 0x09
                -1.0f, // 0x0A
                -1.0f, // 0x0B
                -1.0f, // 0x0C
                -1.0f, // 0x0D
                -1.0f, // 0x0E
                -1.0f, // 0x0F
};

static int drvSensorSensorOv7750GetSclk ( struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                float *sclkMhz)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl;
    u8 readbyte[2];
    fp32 refClk = (fp32)ctx->spec->idealRefFreq;
    fp32 pll2Clk, pll2Multi, pll2PreDiv, pll2Div, PLL2_sys_divider;
    s32 rval;

    (void)control;

    i2cCtl.regValue = &readbyte[0];
    i2cCtl.wordCount = 1;

    /// Get PLL2_pre_divider (0x30B4[3:0])
    i2cCtl.regAddress = OV7750_PLL2_PRE;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    else
    {
        pll2PreDiv = pll2PreDivLT[readbyte[0] & 0x1F];
    }
    /// Get PLL2_multiplier (0x30B3)
    i2cCtl.regAddress = OV7750_PLL2_MULTI;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    pll2Multi = (fp32)(readbyte[0] & 0xFF);

    /// Get PLL2_divider (0x30b1[4:0])
    i2cCtl.regAddress = OV7750_PLL2_DIV;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    if(readbyte[0] & (1<<2))
    {
        pll2Div = 1.5f;
    }
    else
    {
        pll2Div = 1.0f;
    }

    /// Get PLL2_sys_divider (0x30B0[3:0])
    i2cCtl.regAddress = OV7750_PLL2_SYS_DIV;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    PLL2_sys_divider = pll2SysDivLT[readbyte[0] & 0x0F];

    /// Compute the pixel clock
    pll2Clk = ((refClk / pll2PreDiv) * pll2Multi) / pll2Div;


    *sclkMhz = pll2Clk / PLL2_sys_divider;

    return 0;
}
static int drvSensorSensorOv7750GetReadoutUs ( struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                u32 *readoutUs)
{
    u32 camHeight = ctx->spec->outSize.h;
    float sclkMhz;
    s32 hts;
    s32 sc;

    sc = drvSensorSensorOv7750GetHTS(ctx, control, &hts);
    if(sc) return sc;

    sc = drvSensorSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if(sc) return sc;

    *readoutUs = (u32)((float)(camHeight * hts) / sclkMhz);

    return 0;
}

// exposuretime = (Exposure[19:0]/16 + Exposure[3:0]*1/16) * tROW
// tROW = 1/PCLK * TIMING_HTS
static int drvSensorSensorOv7750SetExposureUs ( struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                u32 fd,
                u32 exposureUs)
{
    float exposureVal;
    s32 exposureReg;
    s32 hts;
    float sclkMhz;
    float tROW;
    s32 sc;
    u32 maxExposure;

    sc = drvSensorSensorOv7750GetHTS(ctx, control, &hts);
    if(sc) return sc;

    sc = drvSensorSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if(sc) return sc;


    tROW = (1 / sclkMhz) * hts;

    maxExposure = (u32)(502 * tROW);

    exposureUs = MIN(exposureUs, maxExposure);

    exposureVal = exposureUs / tROW;

    // LOW 4 bits are fraction bits
    exposureVal = exposureVal * 16;

    exposureReg = (u32)exposureVal;

    return ov7500SendArrayToRegister(fd, OV7750_EXPOSURE_19_16, (u8*)&exposureReg, 3);
}

static int drvSensorSensorOv7750SetFrameRate ( struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                u32 fd,
                u32 frameRate)
{
    float sclkMhz;
    float tROW;

    s32 hts;
    s16 regVal;
    int sc = drvSensorSensorOv7750GetHTS(ctx, control, &hts);
    if(sc) return sc;

    sc = drvSensorSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if(sc) return sc;

    tROW = (1.0 / sclkMhz) * hts;

    regVal = (u32)(1000.0 / frameRate / tROW *1000.0);
    return ov7500SendArrayToRegister(fd, OV7750_VTS_L, (u8*)&regVal, 2);

}


static int drvSensorSensorOv7750GetFrameRate ( struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                u32 fd,
                u32* frameRate)
{
    float sclkMhz;
    float tROW;

    s32 hts;
    struct i2cStruct_t i2cCtl;
    u8 readbyte[2];
    s32 rval;
    (void)control;

    i2cCtl.regValue = &readbyte[0];
    i2cCtl.wordCount = 1;

    /// Get PLL1_pre_divider (0x30B4[3:0])
    i2cCtl.regAddress = OV7750_VTS_H;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    else
    {
        *frameRate = (readbyte[0] << 8);
    }
    i2cCtl.regAddress = OV7750_VTS_L;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if(rval != i2cCtl.wordCount)
    {
        return -1;
    }
    else
    {
        *frameRate |= (readbyte[0]);
    }
    int sc = drvSensorSensorOv7750GetHTS(ctx, control, &hts);
    if(sc) return sc;

    sc = drvSensorSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if(sc) return sc;
    tROW = (1.0 / sclkMhz) * hts;

    *frameRate = 1000 / (tROW * (*frameRate) / 1000);
    return 0;
}

static int drvSensorSensorOv7750SetGainFloat(struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control,
                u32 fd,
                float gain)
{
    u8 regVal[2];

    (void)ctx;
    (void)control;

    regVal[0] = (u8)(gain * 16);

    return ov7500SendArrayToRegister(fd, OV7750_GAIN_AGC_ADJ_2, (u8*)regVal, 1);
}

int drvSensorSensorOv7750Config (struct DrvSensorVideoContext *ctx,
                struct DrvSensorCotrolConfig *control)
{
    int fd;

    if(FP_IS_SHARED_CTL(control->cfgType))
    {
        fd = ctx->ctlFdShared;
    }
    else
    {
        fd = ctx->ctlFd;
    }

    switch (FP_STRIP_CTL(control->cfgType))
    {
    case DRV_SENSOR_CONFIG_SEND_EXPOSURE:
        return drvSensorSensorOv7750SetExposureUs(ctx, control, fd, *(u32*)control->buffer);
    case DRV_SENSOR_CONFIG_SEND_GAIN:
        return drvSensorSensorOv7750SetGainFloat(ctx, control, fd, *(float*)control->buffer);
    case DRV_SENSOR_CONFIG_GET_READOUT:
        return drvSensorSensorOv7750GetReadoutUs(ctx, control, (u32*)control->buffer);
    case DRV_SENSOR_GET_FRAME_RATE:
        return drvSensorSensorOv7750GetFrameRate(ctx, control, fd, (u32*)control->buffer);
    case DRV_SENSOR_SET_FRAME_RATE:
        return drvSensorSensorOv7750SetFrameRate(ctx, control, fd, *(u32*)control->buffer);
    case DRV_SENSOR_CONFIG_FLIP:
        return drvSensorSensorOv7750Flip(ctx, control, *(u32*)control->buffer);
    case DRV_SENSOR_CONFIG_MIRROR:
        return drvSensorSensorOv7750Mirror(ctx, control, *(u32*)control->buffer);
    case DRV_SENSOR_GET_TESTPATERN_CRC:
        *(u32*)control->buffer = OV7750_TESTPATTERN_CRC_10BIT;
        return 0;
    default:
        return drvSensorSensorConfig(ctx, control);
    }

    return 0;
}

struct DrvSensorControlFunctions drvSensorOv7750Control =
{
                .open = drvSensorSensorOpen,
                .close = drvSensorSensorClose,
                .config = drvSensorSensorOv7750Config,
                .power_up = drvSensorSensorPowerUp,
                .power_down = drvSensorSensorPowerDown,
};
