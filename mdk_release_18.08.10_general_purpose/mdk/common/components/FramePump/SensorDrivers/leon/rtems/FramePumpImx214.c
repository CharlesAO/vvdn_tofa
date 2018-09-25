///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver implementation for Imx214 @ 30fps

#include "FramePumpImx214.h"
#include "FramePumpControl.h"
#include "mv_types.h"

#define IMX214_BINNING_TESTPATTERN_CRC_10BIT (0xB9DF1362) // CRC for the lines starting at index 8 (without metadata)
#define IMX214_TESTPATTERN_CRC_10BIT         (0x9C0E9348) // CRC for the lines starting at index 8 (without metadata)

static const uint16_t imx214_binning_I2Cregs_config[][2] = {
    // External Clock Setting
    { 0x0136, 0x18},
    { 0x0137, 0x00},

    // Global Setting - basic config
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

    // Global Setting - analog settings
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
    // Mode settings
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

    // Output Size setting
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

    // Clock setting
    { 0x0301, 0x05},
    { 0x0303, 0x04},
    { 0x0305, 0x03},
    { 0x0306, 0x00},
    { 0x0307, 0x96},
    { 0x0309, 0x0A},
    { 0x030B, 0x01},
    { 0x0310, 0x00},

    // Data Rate setting
    { 0x0820, 0x09},
    { 0x0821, 0x60},
    { 0x0822, 0x00},
    { 0x0823, 0x00},

    // WaterMark setting
    { 0x3A03, 0x06},
    { 0x3A04, 0x68},
    { 0x3A05, 0x01},

    // Enable setting
    { 0x0B06, 0x01},
    { 0x30A2, 0x00},

    // Test setting},
    { 0x30B4, 0x00},

    // HDR setting},
    { 0x3A02, 0xFF},

    // STATS setting
    { 0x3013, 0x00},

    // Integration Time Setting
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

    { 0x0600, 0x00},

    // Analog Setting
    { 0x4170, 0x00},
    { 0x4171, 0x10},
    { 0x4176, 0x00},
    { 0x4177, 0x3C},
    { 0xAE20, 0x04},
    { 0xAE21, 0x5C}
};

static const uint16_t imx214_I2Cregs_config[][2] = {
    // Mode setting
    {0x0114, 0x03},
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
    // Output Size setting
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
    // Clock setting
    {0x0301, 0x05},
    {0x0303, 0x02},
    {0x0305, 0x03},
    {0x0306, 0x00},
    {0x0307, 0x96},
    {0x0309, 0x0A},
    {0x030B, 0x01},
    {0x0310, 0x00},
    // Data Rate setting
    {0x0820, 0x12},
    {0x0821, 0xC0},
    {0x0822, 0x00},
    {0x0823, 0x00},
    // WaterMark setting
    {0x3A03, 0x08},
    {0x3A04, 0xC0},
    {0x3A05, 0x02},
    // Enable setting
    {0x0B06, 0x01},
    {0x30A2, 0x00},
    // Test setting
    {0x30B4, 0x00},
    // HDR setting
    {0x3A02, 0xFF},
    // STATS setting
    {0x3013,  0x00},
    // Integration Time Setting
    {0x0202, 0x0C},
    {0x0203, 0x4E},
    {0x0224, 0x01},
    {0x0225, 0xF4},
    // Gain Setting
    {0x0204, 0x00},
    {0x0205, 0x00},
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
    // Analog Setting
    {0x4170, 0x00},
    {0x4171, 0x10},
    {0x4176, 0x00},
    {0x4177, 0x3C},
    {0xAE20, 0x04},
    {0xAE21, 0x5C}
};

static const uint16_t imx214_I2Cregs_testmode[][2] = {
    { 0x0601, 0x02}, // Test mode 0x01: single color, 0x02: colour bars.
};

static const uint16_t imx214_I2Cregs_enable[][2] = {
    // Configuration step 1 (individual step)
    {0x0100, 0x01},  // Enable streaming
};

static const uint16_t imx214_I2Cregs_disable[][2] = {
    // Configuration step 2 (individual step)
    {0x0100, 0x00},  // Disable streaming
};

static const FramePumpI2CConfigDescriptor framePumpImx214_binning_I2CConfigSteps[] = {
    {
        .regValues = imx214_binning_I2Cregs_config,
        .numberOfRegs = COUNT_OF(imx214_binning_I2Cregs_config),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx214_I2Cregs_enable,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_enable),
        .configType = FRAME_PUMP_START_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx214_I2Cregs_disable,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_disable),
        .configType = FRAME_PUMP_STOP_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx214_I2Cregs_testmode,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_testmode),
        .configType = FRAME_PUMP_COLORBAR,
        .delayMs = 0
    },
};

static const FramePumpI2CConfigDescriptor framePumpImx214_I2CConfigSteps[] = {
    {
        .regValues = imx214_I2Cregs_config,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_config),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 1
    },
    {
        .regValues = imx214_I2Cregs_enable,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_enable),
        .configType = FRAME_PUMP_START_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx214_I2Cregs_disable,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_disable),
        .configType = FRAME_PUMP_STOP_STREAMING,
        .delayMs = 0
    },
    {
        .regValues = imx214_I2Cregs_testmode,
        .numberOfRegs = COUNT_OF(imx214_I2Cregs_testmode),
        .configType = FRAME_PUMP_COLORBAR,
        .delayMs = 0
    },
};


rtems_status_code framePumpSensorImx214Config (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control)
{
    switch (FP_STRIP_CTL(control->cfgType)) {
    case FRAME_PUMP_GET_TESTPATTERN_CRC:
        *(uint32_t*)control->buffer = IMX214_TESTPATTERN_CRC_10BIT;
        return RTEMS_SUCCESSFUL;
    default:
        return framePumpSensorConfig(ctx, control);
    }

    return RTEMS_SUCCESSFUL;
}

struct FramePumpControlFunctions framePumpImx214Control = {
    .open = framePumpSensorOpen,
    .close = framePumpSensorClose,
    .config = framePumpSensorImx214Config,
    .power_up = framePumpSensorPowerUp,
    .power_down = framePumpSensorPowerDown,
};

#ifdef MA2X8X
#define CSI_RAW_8 CSI_LP_DT_RAW8
#define CSI_RAW_10 CSI_LP_DT_RAW10
#define MIPI_D_MODE_0 MIPI_DATA_MODE0
#endif

const struct FramePumpSensorDriver framePumpimx214_camCfg = {
    .width            = IMX214_MAX_WIDTH,
    .height           = IMX214_MAX_HEIGHT,
    .windowColumnStart = 0,
    .windowRowStart   = 8,
    .metaROI          = { .x0 = 0, .y0 = 0, .columns = 0, .lines = 0},
    .type             = FP_MIPI,
    .spec.mipi = {
        .dataMode     = MIPI_D_MODE_0,
        .dataRateMbps = 1200,
        .nbOflanes    = 4,
        .pixelFormat  = CSI_RAW_10,
    },
    .nbOfI2CConfigSteps    = COUNT_OF(framePumpImx214_I2CConfigSteps),
    .i2cConfigSteps        = framePumpImx214_I2CConfigSteps,
    .ctlFnc = &framePumpImx214Control,
};

const struct FramePumpSensorDriver framePumpimx214_binning_camCfg = {
    .width            = IMX214_MAX_WIDTH / 2,
    .height           = IMX214_MAX_HEIGHT / 2,
    .windowColumnStart = 0,
    .windowRowStart   = 8,
    .metaROI          = { .x0 = 0, .y0 = 0, .columns = 0, .lines = 0},
    .type             = FP_MIPI,
    .spec.mipi = {
        .dataMode     = MIPI_D_MODE_0,
        .dataRateMbps = 1200,
        .nbOflanes    = 2,
        .pixelFormat  = CSI_RAW_10,
    },
    .nbOfI2CConfigSteps    = COUNT_OF(framePumpImx214_binning_I2CConfigSteps),
    .i2cConfigSteps        = framePumpImx214_binning_I2CConfigSteps,
    .ctlFnc = &framePumpImx214Control,
};
