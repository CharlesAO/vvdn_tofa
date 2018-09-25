///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver implementation for Imx208
#include "FramePumpImx208.h"
#include "FramePumpControl.h"
#include "mv_types.h"

#define IMX208_TESTPATTERN_CRC_10BIT          (0xE17AB499) // CRC for the lines starting at index 8 (without metadata)
#define IMX208_BINNING_TESTPATTERN_CRC_10BIT  (0xFFFFFFFF) // TODO: update the value !
static const uint16_t imx208_I2Cregs_reset[][2] = {
    // Configuration step 0
    {0x0103, 0x01}, // SW Reset
};

static const uint16_t imx208_binning_I2Cregs_config[][2] = {
    // ----------- Start of official config from spreadsheet ------------
    // PLL Setting
    {0x0305, 0x04},
    {0x0307, 0x87},
    {0x303C, 0x4B},
    {0x30A4, 0x01},
    // Mode setting
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
    // Shutter Gain setting
    {0x0202, 0x01}, // Exposure time
    {0x0203, 0x3b},
    {0x0205, 0x00}, // Analog gain
    {0x020E, 0x00}, // Digital gain
    {0x0210, 0x00}, //
    {0x0212, 0x00}, //
    {0x0214, 0x00}, //
    {0x3343, 0x03}, // Manufacturer prohibited !?!
    {0x30f6, 0x00}, // Manufacturer prohibited !?!
};

static const uint16_t imx208_I2Cregs_config[][2] = {
    // ----------- Start of official config from spreadsheet ------------
    // PLL Setting
    {0x0305, 0x04},
    {0x0307, 0x87},
    {0x303C, 0x4B},
    {0x30A4, 0x00},  // 0x02->60fps, 0x00->30fps, 0x01->15fps
    // Mode setting
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
    // Shutter Gain setting
    {0x0202, 0x02}, // Exposure time
    {0x0203, 0xcb},
    {0x0205, 0xE0}, // Analog gain
    {0x020E, 0x00}, // Digital gain
    {0x0210, 0x00}, //
    {0x0212, 0x00}, //
    {0x0214, 0x00}, //
    // ----------- End of official config from spreadsheet ------------
    // registers obtained out of scoping the dev board
    {0x3343, 0x03}, // Manufacturer prohibited !?!
    {0x30f6, 0x00}, // Manufacturer prohibited !?!
};

static const uint16_t imx208_I2Cregs_testmode[][2] = {
    {0x0600, 0x01}, // Test mode
};

static const uint16_t imx208_I2Cregs_enable[][2] = {
    // Configuration step 1 (individual step)
    {0x0100, 0x01},  // Enable streaming
};

static const uint16_t imx208__I2Cregs_disable[][2] = {
    // Configuration step 2 (individual step)
    {0x0100, 0x00},  // Disable streaming
};

static const FramePumpI2CConfigDescriptor framePumpImx208_binning_I2CConfigSteps[] = {
    {
        .regValues = imx208_I2Cregs_reset,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_reset),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx208_binning_I2Cregs_config,
        .numberOfRegs = COUNT_OF(imx208_binning_I2Cregs_config),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx208_I2Cregs_enable,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_enable),
        .configType = FRAME_PUMP_START_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx208__I2Cregs_disable,
        .numberOfRegs = COUNT_OF(imx208__I2Cregs_disable),
        .configType = FRAME_PUMP_STOP_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx208_I2Cregs_testmode,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_testmode),
        .configType = FRAME_PUMP_COLORBAR,
        .delayMs = 0
    },
};

static const FramePumpI2CConfigDescriptor framePumpImx208_I2CConfigSteps[] = {
    {
        .regValues = imx208_I2Cregs_reset,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_reset),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx208_I2Cregs_config,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_config),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx208_I2Cregs_enable,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_enable),
        .configType = FRAME_PUMP_START_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx208__I2Cregs_disable,
        .numberOfRegs = COUNT_OF(imx208__I2Cregs_disable),
        .configType = FRAME_PUMP_STOP_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx208_I2Cregs_testmode,
        .numberOfRegs = COUNT_OF(imx208_I2Cregs_testmode),
        .configType = FRAME_PUMP_COLORBAR,
        .delayMs = 0
    },
};


rtems_status_code framePumpSensorImx208Config (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control)
{
    switch (FP_STRIP_CTL(control->cfgType)) {
    case FRAME_PUMP_GET_TESTPATTERN_CRC:
        *(uint32_t*)control->buffer = IMX208_TESTPATTERN_CRC_10BIT;
        return RTEMS_SUCCESSFUL;
    default:
        return framePumpSensorConfig(ctx, control);
    }

    return RTEMS_SUCCESSFUL;
}

struct FramePumpControlFunctions framePumpImx208Control = {
    .open = framePumpSensorOpen,
    .close = framePumpSensorClose,
    .config = framePumpSensorImx208Config,
    .power_up = framePumpSensorPowerUp,
    .power_down = framePumpSensorPowerDown,
};

#ifdef MA2X8X
#define CSI_RAW_8 CSI_LP_DT_RAW8
#define CSI_RAW_10 CSI_LP_DT_RAW10
#define MIPI_D_MODE_0 MIPI_DATA_MODE0
#endif

const struct FramePumpSensorDriver framePumpimx208_camCfg = {
    .width            = IMX208_MAX_WIDTH,
    .height           = IMX208_MAX_HEIGHT,
    .metaROI = { .x0 = 0, .y0 = 0, .columns = 0, .lines = 0},
    .type             = FP_MIPI,
    .spec.mipi = {
        .dataMode     = MIPI_D_MODE_0,
        .dataRateMbps = 811,
        .nbOflanes    = 2,
        .pixelFormat  = CSI_RAW_10,
    },
    .nbOfI2CConfigSteps    = COUNT_OF(framePumpImx208_I2CConfigSteps),
    .i2cConfigSteps        = framePumpImx208_I2CConfigSteps,
    .ctlFnc = &framePumpImx208Control,
};

const struct FramePumpSensorDriver framePumpimx208_binning_camCfg = {
    .width            = IMX208_MAX_WIDTH / 2,
    .height           = IMX208_MAX_HEIGHT / 2,
    .metaROI = { .x0 = 0, .y0 = 0, .columns = 0, .lines = 0},
    .type             = FP_MIPI,
    .spec.mipi = {
        .dataMode     = MIPI_D_MODE_0,
        .dataRateMbps = 405,
        .nbOflanes    = 2,
        .pixelFormat  = CSI_RAW_10,
    },
    .nbOfI2CConfigSteps    = COUNT_OF(framePumpImx208_binning_I2CConfigSteps),
    .i2cConfigSteps        = framePumpImx208_binning_I2CConfigSteps,
    .ctlFnc = &framePumpImx208Control,
};
