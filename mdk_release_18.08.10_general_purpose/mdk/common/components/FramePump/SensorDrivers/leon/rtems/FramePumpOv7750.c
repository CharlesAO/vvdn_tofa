///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver implementation for OmniVision 7750 & 7750
///
/// 640x480p @ 30 fps RAW10

#include "FramePumpOv7750.h"

#include <FramePumpDefines.h>
#include <mv_types.h>
#include <mvMacros.h>
#include "swcFrameTypes.h"

#include <FramePump.h>
#include <FramePumpControl.h>

#ifdef MA2X8X
#include "FramePumpGenericI2CDriver.h"
#else
#include "OsDrvI2cMyr2.h"
#endif

#include <OsDrvGpio.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MVLOG_UNIT_NAME FramePumpOv7750
#include <mvLog.h>

#define OV7750_TESTPATTERN_CRC_8BIT     (0x2C83B24E)
#define OV7750_TESTPATTERN_CRC_10BIT    (0x95873BC3)

// System control registers
#define OV7750_SC_MODE_SELECT           (0x0100)
#define OV7750_SC_SOFTWARE_RESET        (0x0103)
#define OV7750_SC_FAST_STANDBY_CTRL     (0x0106)
#define OV7750_SC_SCCB_ID1              (0x0109)
#define OV7750_SC_CHIP_ID_HIGH          (0x300A)
#define OV7750_SC_CHIP_ID_LOW           (0x300B)
#define OV7750_SC_SCCB_ID2              (0x302B)
#define OV7750_SC_PROG_ID_ENB           (0x303B)

#define OV7750_CHIP_ID                  (0x7750u) // Chip ID for sensor detection

// Timing control registers
#define OV7750_X_OUTPUT_SIZE_H          (0x3808)
#define OV7750_X_OUTPUT_SIZE_L          (0x3809)
#define OV7750_Y_OUTPUT_SIZE_H          (0x380A)
#define OV7750_Y_OUTPUT_SIZE_L          (0x380B)
#define OV7750_HTS_H                    (0x380C)
#define OV7750_HTS_L                    (0x380D)
#define OV7750_VTS_H                    (0x380E)
#define OV7750_VTS_L                    (0x380F)
#define OV7750_TIMING_FORMAT1           (0x3820) // Vertical timing control
#define OV7750_TFMT1_V_FLIP_BLC         (0x40)   // Vertical flip BLC
#define OV7750_TFMT1_V_FLIP_MASK        (0x04)   // Vertical image flip
#define OV7750_TFMT1_V_BINNING          (0x02)   // Vertical binning
#define OV7750_TIMING_FORMAT2           (0x3821) // Horizontal timing control
#define OV7750_TFMT2_H_MIRROR_MASK      (0x04)   // Horizontal image mirror
#define OV7750_TFMT2_H_BINNING          (0x01)   // Horizontal binning

// Manual AEC/AGC registers
#define OV7750_EXPOSURE_19_16           (0x3500)
#define OV7750_EXPOSURE_15_8            (0x3501)
#define OV7750_EXPOSURE_7_0             (0x3502)
#define OV7750_GAIN_AGC_ADJ_1           (0x350A)
#define OV7750_GAIN_AGC_ADJ_2           (0x350B)
#define OV7750_MAN_SNR_GAIN_1           (0x3504)
#define OV7750_MAN_SNR_GAIN_2           (0x3505)

// Manual AWB Gain registers
#define OV7750_AWB_RED_GAIN_H           (0x3400)
#define OV7750_AWB_RED_GAIN_L           (0x3401)
#define OV7750_AWB_GRN_GAIN_H           (0x3402)
#define OV7750_AWB_GRN_GAIN_L           (0x3403)
#define OV7750_AWB_BLU_GAIN_H           (0x3404)
#define OV7750_AWB_BLU_GAIN_L           (0x3405)
#define OV7750_AWB_MAN_CTRL             (0x3406)

// PLL Registers
#define OV7750_PLL1_PRE                 (0x30B4)
#define OV7750_PLL1_MULTI               (0x30B3)
#define OV7750_PLL1_DIV                 (0x30B1)
#define OV7750_PLL1_PIX_DIV             (0x30B0)
#define OV7750_PLL1_MIPI_DIV            (0x30B5)
#define OV7750_PLL2_PRE                 (0x3098)
#define OV7750_PLL2_MULTI               (0x3099)
#define OV7750_PLL2_DIV                 (0x309D)
#define OV7750_PLL2_SYS_DIV             (0x309A)
#define OV7750_PLL2_ADC                 (0x309B)

// Group Hold registers
#define OV7750_GROUP_ADR0               (0x3200) // Start address of Group 0 buffer in sensor SRAM (bits {3:0})
#define OV7750_GROUP_ADR1               (0x3201) // Start address of Group 1 buffer in sensor SRAM (bits {3:0})
#define OV7750_GROUP_LEN0               (0x3204) // Length of Group 0 (read only)
#define OV7750_GROUP_LEN1               (0x3205) // Length of Group 1 (read only)

#define OV7750_GROUP_ACCESS             (0x3208)
#define OV7750_GPH_G0_START             (0x00)   // Group 0 - Start hold
#define OV7750_GPH_G0_END               (0x10)   // Group 0 - End hold
#define OV7750_GPH_G0_DELAY_LAUNCH      (0xA0)   // Group 0 - Delayed (safe) launch
#define OV7750_GPH_G0_QUICK_LAUNCH      (0xE0)   // Group 0 - Quick (immediate) launch
#define OV7750_GPH_G1_START             (0x01)   // Group 1 - Start hold
#define OV7750_GPH_G1_END               (0x11)   // Group 1 - End hold
#define OV7750_GPH_G1_DELAY_LAUNCH      (0xA1)   // Group 1 - Delayed (safe) launch
#define OV7750_GPH_G1_QUICK_LAUNCH      (0xE1)   // Group 1 - Quick (immediate) launch

#define OV7750_GROUP_SWITCH_CTRL        (0x320B) // Configure auto repetitive switch between two selected groups
#define OV7750_GRP_MANUAL_SW            (0x00)   // Auto group switch disabled (manual mode)
#define OV7750_GRP_AUTO_SINGLE_SW       (0x04)   // Single auto group switch mode enabled
#define OV7750_GRP_AUTO_REPEAT_SW       (0x0C)   // Repetitive auto group switch mode enabled

#define OV7750_GROUP_ACTIVE             (0x320D) // Indicates which group is currently active

// Sensor mode: max exposure = 32.94 ms; frame-rate = 30 fps
#define OV7750_PPLN                     (0x03A0)           // Pixels per line (incl. horizontal blanking): reg {0x380C, 0x380D}
#define OV7750_LPFR                     (0x06BC)           // Lines per frame (incl. vertical blanking): reg {0x380E, 0x380F}
#define OV7750_MIN_EXPOSURE_LINES       (10)               // Min lines number that can be used for exposure
#define OV7750_MAX_EXPOSURE_LINES       (OV7750_LPFR - 20) // Max lines number that can be used for exposure

#if (OV7750_MAX_EXPOSURE_LINES > OV7750_LPFR - 20)
    #error "ERROR: OV7750 max exposure (lines) is outside of supported range !!!"
#endif

// For OV7750 sensor rev. 1C & 1D the gain is in linear (real) format with 4-bit fractional part
#define OV7750_LIN_GAIN_FRACT_BITS      (4)         // Gain fractional part (bits) for sensors with linear gain
#define OV7750_MIN_TOTAL_GAIN           (1.0f)
#define OV7750_MAX_TOTAL_GAIN           (0xFF / 16.0f)

#define OV7750_START_GAIN               (1.0f)
#define OV7750_START_EXPOSURE_LINES     ((0x01F8 > OV7750_MAX_EXPOSURE_LINES) ? OV7750_MAX_EXPOSURE_LINES : 0x01F8) // Integration Time, lines

// Address/Data bytes are sent in big endian order on I2C bus
#define CONV_U16_LE2BE(w)               (((uint16_t)w >> 8) | (((uint16_t)w & 0xFF) << 8))

typedef enum {
    OV7750_WRITE = 0,   // Should match the I2C protocol value for read/write access
    OV7750_READ  = 1
} ov7750Direction;

static const FramePumpCamExpGain ov7750ExpGainRegionTbl[] = {
    { 1000, OV7750_MIN_TOTAL_GAIN},
    {10000, OV7750_MIN_TOTAL_GAIN},
    {10000, 4.0f},
    {20000, 4.0f},
    {20000, 8.0f},
    {32944, 8.0f},
    {32944, OV7750_MAX_TOTAL_GAIN}
};

static const uint16_t cfgLeft[][2] = {
    {0x3005, 0x0A}, // STROBE_out // FSIN_in
    {0x3b81, 0xFF}, // STROBE PATTERN
};

static const uint16_t cfgRight[][2] = {
    {0x3005, 0x0A}, // STROBE_out  // FSIN in
    {0x3b81, 0xFF}, // STROBE PATTERN
};


static const uint16_t fullConfig[][2] = {
    //;; V0.4.1, 6/03/2013, initial release
    //;;
    //;; V0.4.2, 6/12/2013
    //;;  Reg.0x3669[4] =1, to enable MIPI drive right away from sleep mode
    //;;  Reg.0x3012[7:6] = 3, LP1 voltage level
    //;;  Reg.0x4819 = 0xaa for MIPI global timing
    //;;  Reg.0x4823 = 0x3e for MIPI global timing
    //;;
    //;; V0.4.3, 6/24/2013
    //;;  Reg.0x3820[6], adjust the read out sequence of the black rows
    //;;  Reg.0x4001[5:0], correct the black row starting address for sub sizes
    //;;
    //;; V0.0.0, 7/04/2013, Rev.1C initial release
    //;;  Reg.0x3509, change to 0x10 from 0x08, use linear gain
    //;;  Reg.0x4001[7], set to 1 to enable slope correction
    //;;  Reg.0x5001[7], set to 1 to enable digital gain compensation
    //;;  Reg.0x4005[5], set to 1 apply the same slope correction for all channels
    //;;
    //;; 02/18/2014, Rev.1E initial release
    //;;
    //;; 04/11/2014, disable slope correction, change timing low dark current pixel
    //;;  Reg.0x4001[7], set to 0 to disable slope correction
    //;;  Reg.0x4005[5], set back to 0 as slope correction is disabled
    //;;  Reg.0x3757, set to 0xB3
    //;;  Reg.0x3630, set to 0x44
    //;;  Reg.0x3631, set to 0x35
    //;;  Reg.0x3634, set to 0x60
    //;;
    //;; 09/23/2014, change register 0x380f to 0x0a from 0x04 to fix the frame rate issue
    //;;
    //;; 05/14/2015, change register 0x380f to 0x0a from 0x04 in low power mode
    //;;
    //;; 05/03/2016, change regiser 0x3674 to 0xef from 0xff to improve 1st frame IQ in trigger mode

    {0x0100, 0x00},
    {0x3005, 0x00},
    {0x3012, 0xc0},
    {0x3013, 0xd2},
    {0x3014, 0x04},
    {0x3016, 0x10},
    {0x3017, 0x00},
    {0x3018, 0x00},
    {0x301a, 0x00},
    {0x301b, 0x00},
    {0x301c, 0x00},
    {0x3023, 0x05},
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
    {0x3106, 0xda},

    {0x3500, 0x00}, // Integration time (high)
    {0x3501, 0x1f}, // Integration time
    {0x3502, 0x80}, // Integration time (low)

    {0x3503, 0x07},
    {0x3509, 0x10},
    {0x350b, 0x10}, // Gain

    {0x3600, 0x1c},
    {0x3602, 0x62},
    {0x3620, 0xb7},
    {0x3622, 0x04},
    {0x3626, 0x21},
    {0x3627, 0x30},
    {0x3630, 0x44},
    {0x3631, 0x35},
    {0x3634, 0x60},
    {0x3636, 0x00},
    {0x3662, 0x01},
    {0x3663, 0x70},
    {0x3664, 0xf0},
    {0x3666, 0x0a},
    {0x3669, 0x1a},
    {0x366a, 0x00},
    {0x366b, 0x50},
    {0x3673, 0x01},
    {0x3674, 0xef},
    {0x3675, 0x03},
    {0x3705, 0xc1},
    {0x3709, 0x40},
    {0x373c, 0x08},
    {0x3742, 0x00},
    {0x3757, 0xb3},
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
    {0x380c, 0x03}, // TIMING_HTS (PPLN) high
    {0x380d, 0xa0}, // TIMING_HTS (PPLN) low
    {0x380e, 0x06}, // TIMING_VTS (LPFR) high
    {0x380f, 0xbc}, // TIMING_VTS (LPFR) low
    {0x3810, 0x00},
    {0x3811, 0x04},
    {0x3812, 0x00},
    {0x3813, 0x05},
    {0x3814, 0x11},
    {0x3815, 0x11},
    {0x3820, 0x40},
    {0x3821, 0x00},
    {0x382f, 0x0e},
    {0x3832, 0x00},
    {0x3833, 0x05},
    {0x3834, 0x00},
    {0x3835, 0x0c},
    {0x3837, 0x00},
    {0x3b80, 0x00},
    {0x3b81, 0xa5},
    {0x3b82, 0x10},
    {0x3b83, 0x00},
    {0x3b84, 0x08},
    {0x3b85, 0x00},
    {0x3b86, 0x01},
    {0x3b87, 0x00},
    {0x3b88, 0x00},
    {0x3b89, 0x00},
    {0x3b8a, 0x00},
    {0x3b8b, 0x05},
    {0x3b8c, 0x00},
    {0x3b8d, 0x00},
    {0x3b8e, 0x00},
    {0x3b8f, 0x1a},
    {0x3b94, 0x05},
    {0x3b95, 0xf2},
    {0x3b96, 0x40},
    {0x3c00, 0x89},
    {0x3c01, 0x63},
    {0x3c02, 0x01},
    {0x3c03, 0x00},
    {0x3c04, 0x00},
    {0x3c05, 0x03},
    {0x3c06, 0x00},
    {0x3c07, 0x06},
    {0x3c0c, 0x01},
    {0x3c0d, 0xd0},
    {0x3c0e, 0x02},
    {0x3c0f, 0x0a},
    {0x4001, 0x42},
    {0x4004, 0x04},
    {0x4005, 0x00},
    {0x404e, 0x01},
    {0x4300, 0xff},
    {0x4301, 0x00},
    {0x4501, 0x48},
    {0x4600, 0x00},
    {0x4601, 0x4e},
    {0x4801, 0x0f},
    {0x4806, 0x0f},
    {0x4819, 0xaa},
    {0x4823, 0x3e},
    {0x4837, 0x19},
    {0x4a0d, 0x00},
    {0x4a47, 0x7f},
    {0x4a49, 0xf0},
    {0x4a4b, 0x30},
    {0x5000, 0x85},
    {0x5001, 0x80},
};

static const uint16_t framePumpOv7750Raw8SensorCfg[][2] = {
    {0x30b0, 0x08}, // TODO: 8BIT
    {0x30b5, 0x00}, // TODO: 8BIT
    {0x3662, 0x03}, // TODO: 8BIT
    {0x4601, 0x60}, // TODO: 8BIT
};

static const uint16_t framePumpOv7750SensorColorbarCfg[][2] = {
    {0x5e00, 0x8c},
};

static const uint16_t framePumpOv7750SensorStartStreaming[][2] = {
    {0x0100, 0x01},
};

static const uint16_t framePumpOv7750SensorStopStreaming[][2] = {
    {0x0100, 0x00},
};

static const uint16_t framePumpOv7251Raw10SensorMetadata[][2] = {
    {0x4307, 0x31},
    {0x4816, 0x2B},
};

#define OV7750_BASE_CONFIG_RAW10                                        \
    {                                                                   \
        .regValues = fullConfig,                                        \
            .numberOfRegs = COUNT_OF(fullConfig),                       \
            .configType = FRAME_PUMP_FULL_CONFIG,                       \
            .delayMs = 0                                                \
            },                                                          \
    {                                                                   \
        .regValues = framePumpOv7750SensorColorbarCfg,             \
            .numberOfRegs = COUNT_OF(framePumpOv7750SensorColorbarCfg), \
            .configType = FRAME_PUMP_COLORBAR,                          \
            .delayMs = 0                                                \
            },                                                          \
    {                                                                   \
.regValues = framePumpOv7251Raw10SensorMetadata,                        \
     .numberOfRegs = COUNT_OF(framePumpOv7251Raw10SensorMetadata),      \
     .configType = FRAME_PUMP_ENABLE_METADATA,                          \
     .delayMs = 0                                                       \
             },                                                         \
    {                                                                   \
        .regValues = framePumpOv7750SensorStartStreaming,          \
            .numberOfRegs = COUNT_OF(framePumpOv7750SensorStartStreaming), \
            .configType = FRAME_PUMP_START_STREAMING,                   \
            .delayMs = 0                                                \
            },                                                          \
    {                                                                   \
        .regValues = framePumpOv7750SensorStopStreaming,           \
            .numberOfRegs = COUNT_OF(framePumpOv7750SensorStopStreaming), \
            .configType = FRAME_PUMP_STOP_STREAMING,                    \
            .delayMs = 0                                                \
    },

#define OV7750_BASE_CONFIG_RAW8                                         \
    OV7750_BASE_CONFIG_RAW10                                            \
    {                                                                   \
        .regValues = framePumpOv7750Raw8SensorCfg,                      \
            .numberOfRegs = COUNT_OF(framePumpOv7750Raw8SensorCfg),     \
            .configType = FRAME_PUMP_FULL_CONFIG,                       \
            .delayMs = 0                                                \
    },


static const FramePumpI2CConfigDescriptor framePumpOv7750Raw10_I2CConfigSteps[] = {
    OV7750_BASE_CONFIG_RAW10
};

static const FramePumpI2CConfigDescriptor framePumpOv7750Raw10_right_I2CConfigSteps[] = {
    OV7750_BASE_CONFIG_RAW10 
    {
        .regValues = cfgRight,
        .numberOfRegs = COUNT_OF(cfgRight),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};

static const FramePumpI2CConfigDescriptor framePumpOv7750Raw10_left_I2CConfigSteps[] = {
    OV7750_BASE_CONFIG_RAW10
    {
        .regValues = cfgLeft,
        .numberOfRegs = COUNT_OF(cfgLeft),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};

static const FramePumpI2CConfigDescriptor framePumpOv7750Raw8_I2CConfigSteps[] = {
        OV7750_BASE_CONFIG_RAW8
};

static const FramePumpI2CConfigDescriptor framePumpOv7750Raw8_right_I2CConfigSteps[] = {
    OV7750_BASE_CONFIG_RAW8
    {
            .regValues = cfgRight,
            .numberOfRegs = COUNT_OF(cfgRight),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
    };

static const FramePumpI2CConfigDescriptor framePumpOv7750Raw8_left_I2CConfigSteps[] = {
    OV7750_BASE_CONFIG_RAW8
    {
        .regValues = cfgLeft,
        .numberOfRegs = COUNT_OF(cfgLeft),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};

#ifdef MA2X8X
#define CSI_RAW_8 CSI_LP_DT_RAW8
#define CSI_RAW_10 CSI_LP_DT_RAW10
#define MIPI_D_MODE_0 MIPI_DATA_MODE0
#endif

#define FRAMEPUMP_OV7750_SPEC(mipiConf, i2cCfg, bpp, pixelfmt, mipifmt) \
{                                                                       \
    .width            = OV7750_WIDTH,                                   \
    .height           = OV7750_HEIGHT,                                  \
    .windowColumnStart = 0,                                             \
    .windowRowStart = 0,                                                \
    .metaROI = { .x0 = 0, .y0 = 0, .columns = 0, .lines = 1},           \
    .type             = FP_MIPI,                                        \
    .spec.mipi = {                                                      \
        .dataMode     = MIPI_D_MODE_0,                                  \
        .dataRateMbps = 600,                                            \
        .nbOflanes    = 1,                                              \
        .pixelFormat  = mipifmt,                                        \
    },                                                                  \
    .nbOfI2CConfigSteps    = COUNT_OF(i2cCfg),                          \
    .i2cConfigSteps        = i2cCfg,                                    \
    .ctlFnc = &framePumpOv7750Control,                                  \
};

const struct FramePumpSensorDriver framePumpOv7750Raw10_camCfg =
    FRAMEPUMP_OV7750_SPEC(framePumpOv7750Raw10_mipiCfg,
                          framePumpOv7750Raw10_I2CConfigSteps,
                              2, RAW16, CSI_RAW_10);
const struct FramePumpSensorDriver framePumpOv7750Raw10_right_camCfg =
    FRAMEPUMP_OV7750_SPEC(framePumpOv7750Raw10_mipiCfg,
                          framePumpOv7750Raw10_right_I2CConfigSteps,
                              2, RAW16, CSI_RAW_10);
const struct FramePumpSensorDriver framePumpOv7750Raw10_left_camCfg =
    FRAMEPUMP_OV7750_SPEC(framePumpOv7750Raw10_mipiCfg,
                          framePumpOv7750Raw10_left_I2CConfigSteps,
                              2, RAW16, CSI_RAW_10);

const struct FramePumpSensorDriver framePumpOv7750Raw8_camCfg =
    FRAMEPUMP_OV7750_SPEC(framePumpOv7750Raw8_mipiCfg,
                          framePumpOv7750Raw8_I2CConfigSteps,
                              1, RAW8, CSI_RAW_8);
const struct FramePumpSensorDriver framePumpOv7750Raw8_right_camCfg =
    FRAMEPUMP_OV7750_SPEC(framePumpOv7750Raw8_mipiCfg,
                          framePumpOv7750Raw8_right_I2CConfigSteps,
                              1, RAW8, CSI_RAW_8);
const struct FramePumpSensorDriver framePumpOv7750Raw8_left_camCfg =
    FRAMEPUMP_OV7750_SPEC(framePumpOv7750Raw8_mipiCfg,
                          framePumpOv7750Raw8_left_I2CConfigSteps,
                              1, RAW8, CSI_RAW_8);

static float pll1PreDivLT[16] = {
    1.0f, // 0x00
    1.0f, // 0x01
    2.0f, // 0x02
    3.0f, // 0x03
    4.0f, // 0x04
    1.5f, // 0x05
    6.0f, // 0x06
    2.5f, // 0x07
    8.0f, // 0x08
    -1.0f, // 0x09
    -1.0f, // 0x0A
    -1.0f, // 0x0B
    -1.0f, // 0x0C
    -1.0f, // 0x0D
    -1.0f, // 0x0E
    -1.0f // 0x0F
};

static float pll2PreDivLT[32] = {
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

static float pll2SysDivLT[16] = {
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

static int32_t htsStored = 0; // Stored HTS value on first run to avoid reading camera registers on each call
static int32_t vtsStored = 0; // Stored VTS value on first run to avoid reading camera registers on each call
static float sclkMhzStored = 0; // Stored SCLK value on first run to avoid reading camera registers on each call

static rtems_status_code ov7750SendArrayToRegister (
    int fd,
    uint16_t reg,
    uint8_t *array,
    int count)
{
    struct i2cStruct_t i2cCtl = {0};
    int i = 0;
    int rval = 0;

    if (array == NULL) {
        return RTEMS_INVALID_ADDRESS;
    }

    for (i = 0; i < count ; i++) {
        i2cCtl.regAddress = reg - i;
        i2cCtl.regValue = &array[i];
        i2cCtl.wordCount = 1;

        rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            mvLog(MVLOG_WARN, "I2C sensor configuration failed at address 0x%x \n"
                   "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
            i -= 1;
            continue;
        }
    }
    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750Flip (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    int flip)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    int rval = 0;
    uint8_t regVal = 0;

    (void)control;

    i2cCtl.regAddress = OV7750_TIMING_FORMAT1;
    i2cCtl.regValue = &regVal;
    i2cCtl.wordCount = 1;

    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        mvLog(MVLOG_WARN, "I2C sensor read failed at address 0x%x \n"
               "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return RTEMS_IO_ERROR;
    }

    if (flip) {
        *(uint8_t*)i2cCtl.regValue |= OV7750_TFMT1_V_FLIP_MASK;
    } else {
        *(uint8_t*)i2cCtl.regValue &= ~OV7750_TFMT1_V_FLIP_MASK;
    }

    rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        mvLog(MVLOG_WARN, "I2C sensor configuration failed at address 0x%x \n"
              "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return RTEMS_IO_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750Mirror (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    int mirror)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    int rval = 0;
    uint8_t regVal = 0;

    (void)control;

    i2cCtl.regAddress = OV7750_TIMING_FORMAT2;
    i2cCtl.regValue = &regVal;
    i2cCtl.wordCount = 1;

    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount){
        mvLog(MVLOG_WARN, "I2C sensor read failed at address 0x%x \n"
               "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return RTEMS_IO_ERROR;
    }

    if (mirror) {
        *(uint8_t*)i2cCtl.regValue |= OV7750_TFMT2_H_MIRROR_MASK;
    } else {
        *(uint8_t*)i2cCtl.regValue &= ~OV7750_TFMT2_H_MIRROR_MASK;
    }

    rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        mvLog(MVLOG_WARN, "I2C sensor configuration failed at address 0x%x \n"
              "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return RTEMS_IO_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750GetHTS (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    int32_t *hts)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    uint8_t readbyte[2] = {0};
    int32_t rval = 0;

    (void)control;

    if (htsStored == 0) { // First run: get HTS value by reading camera registers and store it
        // Get HTS High Byte (0x380C)
        i2cCtl.regAddress = OV7750_HTS_H;
        i2cCtl.regValue = &readbyte[0];
        i2cCtl.wordCount = 1;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }

        // Get HTS Low Byte (0x380D)
        i2cCtl.regAddress = OV7750_HTS_L;
        i2cCtl.regValue = &readbyte[1];
        i2cCtl.wordCount = 1;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }

        *hts = ((uint32_t)readbyte[0]<<8) | (uint32_t)readbyte[1];
        htsStored = *hts;
    } else { // Subsequent runs: return the stored HTS value
        *hts = htsStored;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750GetVTS (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    int32_t *vts)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    uint8_t readbyte[2] = {0};
    int32_t rval = 0;

    (void)control;

    if (vtsStored == 0) { // First run: get VTS value by reading camera registers and store it
        // Get VTS High Byte (0x380E)
        i2cCtl.regAddress = OV7750_VTS_H;
        i2cCtl.regValue = &readbyte[0];
        i2cCtl.wordCount = 1;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }

        // Get VTS Low Byte (0x380F)
        i2cCtl.regAddress = OV7750_VTS_L;
        i2cCtl.regValue = &readbyte[1];
        i2cCtl.wordCount = 1;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }

        *vts = ((uint32_t)readbyte[0]<<8) | (uint32_t)readbyte[1];
        vtsStored = *vts;
    } else { // Subsequent runs: return the stored VTS value
        *vts = vtsStored;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code __attribute__((unused)) framePumpSensorOv7750GetPclk (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    float *pclkMhz)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    uint8_t readbyte[2] = {0};
    float refClk = 24.0f; // TODO: fixme
    float pll1Clk = 0.0f;
    float pll1Multi = 0.0f;
    float pll1PreDiv = 0.0f;
    float pll1Div = 0.0f;
    float PLL1_pix_divider = 0.0f;
    int32_t rval = 0;

    (void)control;

    i2cCtl.regValue = &readbyte[0];
    i2cCtl.wordCount = 1;

    // Get PLL1_pre_divider (0x30B4[3:0])
    i2cCtl.regAddress = OV7750_PLL1_PRE;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        return RTEMS_IO_ERROR;
    }
    pll1PreDiv = pll1PreDivLT[readbyte[0] & 0x0F];

    // Get PLL1_multiplier (0x30B3[7:0])
    i2cCtl.regAddress = OV7750_PLL1_MULTI;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        return RTEMS_IO_ERROR;
    }
    pll1Multi = (float)(readbyte[0] & 0xFF);

    // Get PLL1_divider (0x30B1[4:0])
    i2cCtl.regAddress = OV7750_PLL1_DIV;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        return RTEMS_IO_ERROR;
    }
    pll1Div = (float)(readbyte[0] & 0x1F); // Should be >=1 and <= 16

    // Get PLL1_pix_divider (0x30B0[3:0])
    i2cCtl.regAddress = OV7750_PLL1_PIX_DIV;
    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        return RTEMS_IO_ERROR;
    }
    PLL1_pix_divider = (float) (readbyte[0] & 0x0F); // Should be 0x08 or 0x0A

    // Compute the pixel clock
    pll1Clk = ((refClk / pll1PreDiv) * pll1Multi) / pll1Div;

    mvLog(MVLOG_DEBUG, "PLL1Clk  is %f", pll1Clk);
    mvLog(MVLOG_DEBUG, "PLL1PixClk divider is %f", PLL1_pix_divider);

    *pclkMhz = pll1Clk / PLL1_pix_divider;

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750GetSclk (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    float *sclkMhz)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    uint8_t readbyte[2] = {0};
    float refClk = 24.0f; // TODO: FIXME
    float pll2Clk = 0.0f;
    float pll2Multi = 0.0f;
    float pll2PreDiv = 0.0f;
    float pll2Div= 0.0f;
    float PLL2_sys_divider = 0.0f;
    int32_t rval = 0;

    (void)control;

    if (sclkMhzStored == 0) { // First run: get SCLK value by reading camera registers and store it
        i2cCtl.regValue = &readbyte[0];
        i2cCtl.wordCount = 1;

        // Get PLL2_pre_divider (0x3098[4:0])
        i2cCtl.regAddress = OV7750_PLL2_PRE;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll2PreDiv = pll2PreDivLT[readbyte[0] & 0x1F];

        // Get PLL2_multiplier (0x3099[7:0])
        i2cCtl.regAddress = OV7750_PLL2_MULTI;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll2Multi = (float)(readbyte[0] & 0xFF);

        // Get PLL2_divider (0x309D[2])
        i2cCtl.regAddress = OV7750_PLL2_DIV;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }

        if(readbyte[0] & (1<<2)) {
            pll2Div = 1.5f;
        } else {
            pll2Div = 1.0f;
        }

        // Get PLL2_sys_divider (0x309A[3:0])
        i2cCtl.regAddress = OV7750_PLL2_SYS_DIV;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if(rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        PLL2_sys_divider = pll2SysDivLT[readbyte[0] & 0x0F];

        // Compute the system clock
        pll2Clk = ((refClk / pll2PreDiv) * pll2Multi) / pll2Div;

        mvLog(MVLOG_DEBUG, "PLL2Clk  is %f", pll2Clk);
        mvLog(MVLOG_DEBUG, "PLL2SysClk divider is %f", PLL2_sys_divider);

        *sclkMhz = pll2Clk / PLL2_sys_divider;
        sclkMhzStored = *sclkMhz;
    } else { // Subsequent runs: return the stored SCLK value
        *sclkMhz = sclkMhzStored;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750GetReadoutUs (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t *readoutUs)
{
    uint32_t camHeight = ctx->spec->height;
    float sclkMhz = 0.0f;
    int32_t hts = 0;
    int32_t sc = RTEMS_SUCCESSFUL;

    if (ctx->outSpec.hasMetadata) {
        camHeight += ctx->spec->metaROI.columns;
    }

    sc = framePumpSensorOv7750GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    *readoutUs = (uint32_t)((float)(camHeight * hts) / sclkMhz);

    mvLog(MVLOG_DEBUG, "Readout time is %lu", *readoutUs);

    return RTEMS_SUCCESSFUL;
}

static int framePumpSensorOv7750GetFrameRate (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t *frameRate)
{
    float sclkMhz = 0.0f;
    int32_t hts = 0;
    int32_t vts = 0;
    int32_t sc = RTEMS_SUCCESSFUL;

    sc = framePumpSensorOv7750GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv7750GetVTS(ctx, control, &vts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    *frameRate = (uint32_t)(sclkMhz * 1000000 / (float)(vts * hts));

    mvLog(MVLOG_DEBUG, "Frame-rate is %lu fps", *frameRate);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750SetFrameRate (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t fd,
    uint32_t frameRate)
{
    float sclkMhz = 0.0f;
    float tROW = 0.0f;
    int32_t hts = 0;
    int16_t regVal = 0;

    rtems_status_code sc = framePumpSensorOv7750GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    tROW = (1.0 / sclkMhz) * hts;

    regVal = (uint32_t)(1000.0 / frameRate / tROW *1000.0);
    return ov7750SendArrayToRegister(fd, OV7750_VTS_L, (uint8_t*)&regVal, 2);
}

static rtems_status_code framePumpSensorOv7750GetExpGainInfo (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    FramePumpCamExpGainInfo *infoExpGain)
{
    float sclkMhz = 0.0f;
    float tROW = 0.0f;
    int32_t hts = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = framePumpSensorOv7750GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    tROW = (1 / sclkMhz) * hts;

    infoExpGain->min.exposureTimeUs = (uint32_t)(OV7750_MIN_EXPOSURE_LINES * tROW);
    infoExpGain->min.gainMultiplier = OV7750_MIN_TOTAL_GAIN;

    infoExpGain->max.exposureTimeUs = (uint32_t)(OV7750_MAX_EXPOSURE_LINES * tROW);
    infoExpGain->max.gainMultiplier = OV7750_MAX_TOTAL_GAIN;

    infoExpGain->start.exposureTimeUs = (uint32_t)(OV7750_START_EXPOSURE_LINES * tROW);
    infoExpGain->start.gainMultiplier = OV7750_START_GAIN;

    infoExpGain->gainFractBits = OV7750_LIN_GAIN_FRACT_BITS;

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750GetExpGainTable (
    FramePumpCamExpGainTable *tblExpGain)
{
    int i = 0;
    int rgnNmbr = 0;

    rgnNmbr = COUNT_OF(ov7750ExpGainRegionTbl);
    if (rgnNmbr > FP_EXPGAIN_TBL_MAX_SZ) {
        rgnNmbr = FP_EXPGAIN_TBL_MAX_SZ;
        mvLog(MVLOG_WARN, "OV7750 {Exposure,Gain} regions table is too big -> truncated !!!\n");
    }

    for (i = 0; i < rgnNmbr; i++) {
        tblExpGain->region[i] = ov7750ExpGainRegionTbl[i];
    }

    if (rgnNmbr < FP_EXPGAIN_TBL_MAX_SZ) {
        // If table is not full => fill in a termination entry
        tblExpGain->region[i].exposureTimeUs = 0;
        tblExpGain->region[i].gainMultiplier = 0.f;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv7750GetMaxExposureUs (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t *maxExposureUs)
{
    float sclkMhz = 0.0f;
    float tROW = 0.0f;
    int32_t hts = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = framePumpSensorOv7750GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    tROW = (1 / sclkMhz) * hts;

    *maxExposureUs = (uint32_t)(OV7750_MAX_EXPOSURE_LINES * tROW);

    mvLog(MVLOG_DEBUG, "Max Exposure time is %lu", *maxExposureUs);

    return RTEMS_SUCCESSFUL;
}

// exposuretime = (Exposure[19:4] + Exposure[3:0]*1/16) * tROW
// tROW = 1/SCLK * TIMING_HTS
static rtems_status_code framePumpSensorOv7750SetExposureUs (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t fd,
    uint32_t exposureUs)
{
    float exposureVal = 0.0f;
    int32_t exposureReg = 0;
    int32_t hts = 0;
    float sclkMhz = 0.0f;
    float tROW = 0.0f;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    uint32_t maxExposure = 0;

    sc = framePumpSensorOv7750GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    mvLog(MVLOG_DEBUG, "HTS is %ld", hts);

    sc = framePumpSensorOv7750GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    mvLog(MVLOG_DEBUG, "SCLK is %f", sclkMhz);

    tROW = (1 / sclkMhz) * hts;

    maxExposure = (uint32_t)(OV7750_MAX_EXPOSURE_LINES * tROW);

    mvLog(MVLOG_DEBUG, "Max Exposure time is %lu", maxExposure);

    exposureUs = MIN(exposureUs, maxExposure);

    exposureVal = exposureUs / tROW;

    // LOW 4 bits are fraction bits
    exposureVal = exposureVal * 16;

    exposureReg = (uint32_t)exposureVal;

    return ov7750SendArrayToRegister(fd, OV7750_EXPOSURE_7_0, (uint8_t*)&exposureReg, 3);
}

static int framePumpSensorOv7750SetGainFloat (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t fd,
    float gain)
{
    uint8_t regVal[2] = {0};

    (void)ctx;
    (void)control;

    regVal[0] = (uint8_t)(gain * 16);

    return ov7750SendArrayToRegister(fd, OV7750_GAIN_AGC_ADJ_2, (uint8_t*)regVal, 1);
}

static int framePumpSensorOv7750SetExpGainSafely (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t fd,
    FramePumpCamExpGain *camExpGain)
{
    uint8_t regVal = 0;
    int sts = 0;

    regVal = OV7750_GPH_G0_START;
    sts = ov7750SendArrayToRegister(fd, OV7750_GROUP_ACCESS, &regVal, 1) << 0;

    sts |= framePumpSensorOv7750SetExposureUs(ctx, control, fd, camExpGain->exposureTimeUs) << 8;

    sts |= framePumpSensorOv7750SetGainFloat(ctx, control, fd, camExpGain->gainMultiplier) << 16;

    regVal = OV7750_GPH_G0_END;
    sts |= ov7750SendArrayToRegister(fd, OV7750_GROUP_ACCESS, &regVal, 1) << 24;

    regVal = OV7750_GPH_G0_DELAY_LAUNCH;
    sts |= ov7750SendArrayToRegister(fd, OV7750_GROUP_ACCESS, &regVal, 1) << 28;

    return sts;
}

static rtems_status_code framePumpSensorOv7750Config (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control)
{
    int fd = 0;

    if (FP_IS_SHARED_CTL(control->cfgType) && ctx->ctlFdShared > 0) {
        fd = ctx->ctlFdShared;
    } else {
        fd = ctx->ctlFd;
    }

    switch (FP_STRIP_CTL(control->cfgType)) {
    case FRAME_PUMP_CONFIG_SEND_EXPOSURE:
        return framePumpSensorOv7750SetExposureUs(ctx, control, fd, *(uint32_t*)control->buffer);
    case FRAME_PUMP_GET_MAX_EXPOSURE:
        return framePumpSensorOv7750GetMaxExposureUs(ctx, control, (uint32_t*)control->buffer);
    case FRAME_PUMP_CONFIG_SEND_GAIN:
        return framePumpSensorOv7750SetGainFloat(ctx, control, fd, *(float*)control->buffer);
    case FRAME_PUMP_CONFIG_SEND_EXPGAIN:
        return framePumpSensorOv7750SetExpGainSafely(ctx, control, fd, (FramePumpCamExpGain *)control->buffer);
    case FRAME_PUMP_CONFIG_GET_READOUT:
        return framePumpSensorOv7750GetReadoutUs(ctx, control, (uint32_t*)control->buffer);
    case FRAME_PUMP_CONFIG_FLIP:
        return framePumpSensorOv7750Flip(ctx, control, *(uint32_t*)control->buffer);
    case FRAME_PUMP_CONFIG_MIRROR:
        return framePumpSensorOv7750Mirror(ctx, control, *(uint32_t*)control->buffer);
    case FRAME_PUMP_GET_TESTPATTERN_CRC:
        if (ctx->outSpec.spec.type == RAW8) {
            *(uint32_t*)control->buffer = OV7750_TESTPATTERN_CRC_8BIT;
        } else {
            *(uint32_t*)control->buffer = OV7750_TESTPATTERN_CRC_10BIT;
        }
        return RTEMS_SUCCESSFUL;
    case FRAME_PUMP_GET_FRAMERATE:
        return framePumpSensorOv7750GetFrameRate(ctx, control, (uint32_t*)control->buffer);
    case FRAME_PUMP_SET_FRAMERATE:
        return framePumpSensorOv7750SetFrameRate(ctx, control, fd, *(uint32_t*)control->buffer);
    case FRAME_PUMP_GET_EXPGAIN_INFO:
        return framePumpSensorOv7750GetExpGainInfo(ctx, control, (FramePumpCamExpGainInfo *)control->buffer);
    case FRAME_PUMP_GET_EXPGAIN_TABLE:
        return framePumpSensorOv7750GetExpGainTable((FramePumpCamExpGainTable *)control->buffer);
    default:
        return framePumpSensorConfig(ctx, control);
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code framePumpSensorOv7750FrameCb (
    const struct FramePumpVideoContext *ctx,
    struct FramePumpBuffer *fpb)
{
    if (ctx->outSpec.hasMetadata) {
        uint32_t exposureRegValue = 0;
        uint32_t gainRegValue = 0;

        exposureRegValue |= ((fpb->fb.p1[22] & 0x0F) >> 0) <<  0; // 4 bits
        exposureRegValue |= ((fpb->fb.p1[16] & 0xFF) >> 0) <<  4; // 8 bits
        exposureRegValue |= ((fpb->fb.p1[14] & 0xFF) >> 0) << 12; // 8 bits

        fpb->exposureNs = exposureRegValue; // TODO: calculate time

        gainRegValue |= ((fpb->fb.p1[6] & 0xFF) >> 0) <<  0; // 8 bits

        fpb->gain = gainRegValue; // TODO: calculate time
    }
    return RTEMS_SUCCESSFUL;
}

struct FramePumpControlFunctions framePumpOv7750Control = {
    .open = framePumpSensorOpen,
    .close = framePumpSensorClose,
    .config = framePumpSensorOv7750Config,
    .power_up = framePumpSensorPowerUp,
    .power_down = framePumpSensorPowerDown,
    .frameCb = framePumpSensorOv7750FrameCb,
};

static rtems_status_code i2cWriteRegAtHackedAddress (
    rtems_device_minor_number minor,
    uint8_t hackedAddress, uint16_t regAddr, uint8_t data)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    minor &= ~0xFF;
    minor |=  hackedAddress;

    regAddr = CONV_U16_LE2BE(regAddr);

    sc = rtems_libi2c_send_start(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_start failed %d", sc);
        return sc;
    }

    sc = rtems_libi2c_send_addr(minor, (int)OV7750_WRITE);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_addr W failed %d", sc);
        return sc;
    }

    sc = rtems_libi2c_write_bytes(minor, (uint8_t *)&regAddr, sizeof(regAddr));
    if (sc != sizeof(regAddr)) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_read/write regaddr failed, sc = %d", sc);
        return RTEMS_IO_ERROR;
    }

    sc = rtems_libi2c_write_bytes(minor, (uint8_t *)&data, sizeof(data));
    if (sc != sizeof(data)) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_read/write data failed, sc = %d", sc);
        return RTEMS_IO_ERROR;
    }

    sc = rtems_libi2c_send_stop(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_stop failed, sc = %d", sc);
        return RTEMS_IO_ERROR;
    }

    return sc;
}

static rtems_status_code checkChipId (
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    const uint16_t chipID)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    uint16_t regAddr = CONV_U16_LE2BE(OV7750_SC_CHIP_ID_HIGH);
    uint16_t data = 0xDEAD;

    (void)major;

    sc = rtems_libi2c_send_start(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_start failed %d", sc);
        return sc;
    }

    sc = rtems_libi2c_send_addr(minor, (int)OV7750_WRITE);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_addr W failed %d", sc);
        return sc;
    }

    sc = rtems_libi2c_write_bytes(minor, (uint8_t *)&regAddr, sizeof(regAddr));
    if (sc != sizeof(regAddr)) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_read/write regaddr failed, sc = %d", sc);
        return RTEMS_IO_ERROR;
    }

    sc = rtems_libi2c_send_addr(minor, (int)OV7750_READ);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_addr R failed %d", sc);
        return sc;
    }

    sc = rtems_libi2c_read_bytes(minor, (uint8_t *)&data, sizeof(data));
    if (sc != sizeof(data)) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_read/write data failed, sc = %d", sc);
        return RTEMS_IO_ERROR;
    }

    sc = rtems_libi2c_send_stop(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_stop failed, sc = %d", sc);
        return RTEMS_IO_ERROR;
    }

    mvLog(MVLOG_DEBUG, "Found ov7750 %x", data);

    if (data == CONV_U16_LE2BE(chipID)) {
        return RTEMS_SUCCESSFUL;
    } else {
        return RTEMS_INVALID_ID;
    }
}

static rtems_status_code ov7750Init (
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *arg)
{
    int sc = RTEMS_SUCCESSFUL;
    int leftResetFd = 0;
    int rightResetFd = 0;
    char gpioName[64] = {0};
    int busNumber = (minor >> 10) & 7;
    int value = 0;

    (void)major;
    (void)arg;

    sprintf(gpioName, "/dev/gpio.ov7750_bus%1d_reset_%s", busNumber, "left");
    leftResetFd  = open(gpioName, O_RDWR);

    sprintf(gpioName, "/dev/gpio.ov7750_bus%1d_reset_%s", busNumber, "right");
    rightResetFd = open(gpioName, O_RDWR);

    rtems_task_wake_after(20);
    switch (minor & 0xFF) {
    case 0x61:
        value = 1;
        write(leftResetFd, &value, sizeof(value));
        value = 0;
        write(rightResetFd, &value, sizeof(value));
        break;
    case 0x62:
        value = 1;
        write(rightResetFd, &value, sizeof(value));
        break;
    default:
        return RTEMS_INVALID_NUMBER;
    }
    rtems_task_wake_after(20);

    close(leftResetFd);
    close(rightResetFd);

    sc = i2cWriteRegAtHackedAddress(minor, 0x70, OV7750_SC_SCCB_ID1, (minor & 0xFF) << 1);
    if (sc != RTEMS_SUCCESSFUL) {
        goto ov7750InitExit;
    }

    sc = i2cWriteRegAtHackedAddress(minor, 0x70, OV7750_SC_PROG_ID_ENB, 0x02);
    if (sc != RTEMS_SUCCESSFUL) {
        goto ov7750InitExit;
    }

    sc = i2cWriteRegAtHackedAddress(minor, 0x70, OV7750_SC_SCCB_ID2, 0xE2);
    if (sc != RTEMS_SUCCESSFUL) {
        goto ov7750InitExit;
    }

    sc = i2cWriteRegAtHackedAddress(minor, 0x71, OV7750_SC_PROG_ID_ENB, 0x02);
    if (sc != RTEMS_SUCCESSFUL) {
        goto ov7750InitExit;
    }

    sc = checkChipId(major, minor, OV7750_CHIP_ID);
    if (sc != RTEMS_SUCCESSFUL) {
        goto ov7750InitExit;
    }

ov7750InitExit:
    mvLog(MVLOG_DEBUG, "ov7750 sc = %d", sc);
    return sc < 0 ? -sc : sc;
}

static rtems_status_code ov7750InitBroadcast (
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *arg)
{
    int sc = RTEMS_SUCCESSFUL;
    (void)arg;
    sc = checkChipId(major, minor, OV7750_CHIP_ID);
    return sc < 0 ? -sc : sc;
}

static rtems_status_code anythingSuccessDevice (
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *arg)
{
    (void)major;
    (void)minor;
    (void)arg;
    return RTEMS_SUCCESSFUL;
}

static rtems_driver_address_table libi2cOv7750ProtocolAdrTbl = {
    .initialization_entry = ov7750Init,
    .read_entry = anythingSuccessDevice,
    .write_entry = anythingSuccessDevice,
    .open_entry = anythingSuccessDevice,
    .close_entry = anythingSuccessDevice,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

rtems_libi2c_drv_t libi2cOv7750ProtocolDrvTbl = {
    .ops = &libi2cOv7750ProtocolAdrTbl,
    .size = sizeof(libi2cOv7750ProtocolDrvTbl),
};

static rtems_driver_address_table libi2cOv7750BroadcastProtocolAdrTbl = {
    .initialization_entry = ov7750InitBroadcast,
    .read_entry = anythingSuccessDevice,
    .write_entry = anythingSuccessDevice,
    .open_entry = anythingSuccessDevice,
    .close_entry = anythingSuccessDevice,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

rtems_libi2c_drv_t libi2cOv7750BroadcastProtocolDrvTbl = {
    .ops = &libi2cOv7750BroadcastProtocolAdrTbl,
    .size = sizeof(libi2cOv7750BroadcastProtocolAdrTbl),
};
