///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver implementation for OmniVision 9282
///
/// 640x480p @ 30 fps RAW10

#include "FramePumpOv9282.h"

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

#ifdef MA2X8X
#include "OsDrvI2cMasterBusInternal.h" /// TODO try to get rid of dependency (bus_alloc)
#endif

#define MVLOG_UNIT_NAME FramePumpOv9282
#include <mvLog.h>

#define OV9282_TESTPATTERN_CRC_10BIT    (0x05BA8572)
#define OV9282_TESTPATTERN_CRC_8BIT     (0xBFBCA57D)

// System control registers
#define OV9282_SC_MODE_SELECT           (0x0100)
#define OV9282_SC_SOFTWARE_RESET        (0x0103)
#define OV9282_SC_FAST_STANDBY_CTRL     (0x0106)
#define OV9282_SC_SCCB_ID_SID1          (0x0107)
#define OV9282_SC_SCCB_ID_SID0          (0x0109)
#define OV9282_SC_CHIP_ID_HIGH          (0x300A)
#define OV9282_SC_CHIP_ID_LOW           (0x300B)
#define OV9282_SC_SCCB_ID_BROADCAST     (0x302B)
#define OV9282_SC_PROG_ID_ENB           (0x303B)

#define OV9282_CHIP_ID                  (0x9281u) // Chip ID for sensor detection

// Timing control registers
#define OV9282_X_OUTPUT_SIZE_H          (0x3808)
#define OV9282_X_OUTPUT_SIZE_L          (0x3809)
#define OV9282_Y_OUTPUT_SIZE_H          (0x380A)
#define OV9282_Y_OUTPUT_SIZE_L          (0x380B)
#define OV9282_HTS_H                    (0x380C)
#define OV9282_HTS_L                    (0x380D)
#define OV9282_VTS_H                    (0x380E)
#define OV9282_VTS_L                    (0x380F)
#define OV9282_TIMING_FORMAT1           (0x3820) // Vertical timing control
#define OV9282_TFMT1_V_FLIP_BLC         (0x40)   // Vertical flip BLC
#define OV9282_TFMT1_V_FLIP_MASK        (0x04)   // Vertical image flip
#define OV9282_TFMT1_V_BINNING          (0x02)   // Vertical binning
#define OV9282_TIMING_FORMAT2           (0x3821) // Horizontal timing control
#define OV9282_TFMT2_H_MIRROR_MASK      (0x04)   // Horizontal image mirror
#define OV9282_TFMT2_H_BINNING          (0x01)   // Horizontal binning

// Manual AEC/AGC registers
#define OV9282_EXPOSURE_19_16           (0x3500)
#define OV9282_EXPOSURE_15_8            (0x3501)
#define OV9282_EXPOSURE_7_0             (0x3502)
#define OV9282_GAIN_AGC_ADJ             (0x3509)

// Manual AWB Gain registers
#define OV9282_AWB_RED_GAIN_H           (0x3400)
#define OV9282_AWB_RED_GAIN_L           (0x3401)
#define OV9282_AWB_GRN_GAIN_H           (0x3402)
#define OV9282_AWB_GRN_GAIN_L           (0x3403)
#define OV9282_AWB_BLU_GAIN_H           (0x3404)
#define OV9282_AWB_BLU_GAIN_L           (0x3405)
#define OV9282_AWB_MAN_CTRL             (0x3406)

// PLL Registers
#define OV9282_PLL1_PREDIV0             (0x030A)
#define OV9282_PLL1_PREDIV0_MASK        (0x01)
#define OV9282_PLL1_PREDIV              (0x0300)
#define OV9282_PLL1_PREDIV_MASK         (0x07)
#define OV9282_PLL1_MULTI_H             (0x0301)
#define OV9282_PLL1_MULTI_H_MASK        (0x03)
#define OV9282_PLL1_MULTI_L             (0x0302)
#define OV9282_PLL1_MULTI_L_MASK        (0xFF)
#define OV9282_PLL1_MIPI_PREDIV         (0x0303)
#define OV9282_PLL1_MIPI_PREDIV_MASK    (0x0F)
#define OV9282_PLL1_MIPI_DIV            (0x0304)
#define OV9282_PLL1_MIPI_DIV_MASK       (0x03)
#define OV9282_PLL1_SYS_PREDIV          (0x0305)
#define OV9282_PLL1_SYS_PREDIV_MASK     (0x03)
#define OV9282_PLL1_SYS_DIV             (0x0306)
#define OV9282_PLL1_SYS_DIV_MASK        (0x01)

#define OV9282_PLL2_PREDIV0             (0x0314)
#define OV9282_PLL2_PREDIV0_MASK        (0x01)
#define OV9282_PLL2_PREDIV              (0x030B)
#define OV9282_PLL2_PREDIV_MASK         (0x07)
#define OV9282_PLL2_MULTI_H             (0x030C)
#define OV9282_PLL2_MULTI_H_MASK        (0x03)
#define OV9282_PLL2_MULTI_L             (0x030D)
#define OV9282_PLL2_MULTI_L_MASK        (0xFF)
#define OV9282_PLL2_ADC_DIV             (0x0313)
#define OV9282_PLL2_ADC_DIV_MASK        (0x0F)
#define OV9282_PLL2_ANA_DIV             (0x0312)
#define OV9282_PLL2_ANA_DIV_MASK        (0x0F)
#define OV9282_PLL2_SYS_PREDIV          (0x030F)
#define OV9282_PLL2_SYS_PREDIV_MASK     (0x0F)
#define OV9282_PLL2_SYS_DIV             (0x030E)
#define OV9282_PLL2_SYS_DIV_MASK        (0x07)

// Group Hold registers
#define OV9282_GROUP_ADR0               (0x3200) // Start address of Group 0 buffer in sensor SRAM (bits {3:0})
#define OV9282_GROUP_ADR1               (0x3201) // Start address of Group 1 buffer in sensor SRAM (bits {3:0})
#define OV9282_GROUP_LEN0               (0x3204) // Length of Group 0 (read only)
#define OV9282_GROUP_LEN1               (0x3205) // Length of Group 1 (read only)

#define OV9282_GROUP_ACCESS             (0x3208)
#define OV9282_GPH_G0_START             (0x00)   // Group 0 - Start hold
#define OV9282_GPH_G0_END               (0x10)   // Group 0 - End hold
#define OV9282_GPH_G0_DELAY_LAUNCH      (0xA0)   // Group 0 - Delayed (safe) launch
#define OV9282_GPH_G0_QUICK_LAUNCH      (0xE0)   // Group 0 - Quick (immediate) launch
#define OV9282_GPH_G1_START             (0x01)   // Group 1 - Start hold
#define OV9282_GPH_G1_END               (0x11)   // Group 1 - End hold
#define OV9282_GPH_G1_DELAY_LAUNCH      (0xA1)   // Group 1 - Delayed (safe) launch
#define OV9282_GPH_G1_QUICK_LAUNCH      (0xE1)   // Group 1 - Quick (immediate) launch

#define OV9282_GROUP_SWITCH_CTRL        (0x320B) // Configure auto repetitive switch between two selected groups
#define OV9282_GRP_MANUAL_SW            (0x00)   // Auto group switch disabled (manual mode)
#define OV9282_GRP_AUTO_SINGLE_SW       (0x04)   // Single auto group switch mode enabled
#define OV9282_GRP_AUTO_REPEAT_SW       (0x0C)   // Repetitive auto group switch mode enabled

#define OV9282_GROUP_ACTIVE             (0x320D) // Indicates which group is currently active

// Sensor mode: max exposure = 33.23 ms; frame-rate = 30 fps
#define OV9282_PPLN                     (0x02D8)           // Pixels per line (incl. horizontal blanking): reg {0x380C, 0x380D}
#define OV9282_LPFR                     (0x0E50)           // Lines per frame (incl. vertical blanking): reg {0x380E, 0x380F}
#define OV9282_MIN_EXPOSURE_LINES       (10)               // Min lines number that can be used for exposure
#define OV9282_MAX_EXPOSURE_LINES       (OV9282_LPFR - 12) // Max lines number that can be used for exposure

#if (OV9282_MAX_EXPOSURE_LINES > OV9282_LPFR - 12)
    #error "ERROR: OV9282 max exposure (lines) is outside of supported range !!!"
#endif

// For OV9282 sensor the gain is in linear (real) format with 4-bit fractional part
#define OV9282_LIN_GAIN_FRACT_BITS  (4)         // Gain fractional part (bits) for sensors with linear gain
#define OV9282_MIN_TOTAL_GAIN       (1.0f)
#define OV9282_MAX_TOTAL_GAIN       (0xFF / 16.0f)

#define OV9282_START_GAIN           (1.0f)
#define OV9282_START_EXPOSURE_LINES ((0x07F2 > OV9282_MAX_EXPOSURE_LINES) ? OV9282_MAX_EXPOSURE_LINES : 0x07F2) // Integration Time, lines

// Address/Data bytes are sent in big endian order on I2C bus
#define CONV_U16_LE2BE(w)           (((uint16_t)w >> 8) | (((uint16_t)w & 0xFF) << 8))

typedef enum {
    OV9282_WRITE = 0,   // Should match the I2C protocol value for read/write access
    OV9282_READ  = 1
} ov9282Direction;

static const FramePumpCamExpGain ov9282ExpGainRegionTbl[] = {
    { 1000, OV9282_MIN_TOTAL_GAIN},
    {10000, OV9282_MIN_TOTAL_GAIN},
    {10000, 4.0f},
    {20000, 4.0f},
    {20000, 8.0f},
    {33233, 8.0f},
    {33233, OV9282_MAX_TOTAL_GAIN}
};

static const uint16_t cfgLeft[][2] = {
//    {0x3006, 0x02}, // STROBE_out // FSIN_in          /// TODO
};

static const uint16_t cfgRight[][2] = {
//    {0x3006, 0x02}, // STROBE_out  // FSIN in         /// TODO
};
static const uint16_t fullConfig720p[][2] = {
    //1280x720
    {0x0302, 0x32},   // PLL1_multiplier[7:0] = x50
    {0x030D, 0x50},   // PLL2_multiplier[7:0] = x80
    {0x030E, 0x02},   // PLL2_sys_div = /2
    {0x3001, 0x00},   // pad drive capability
    {0x3004, 0x00},   // PLL1_MIPI_div
    {0x3005, 0x00},   //
    {0x3006, 0x04},   // 04 ILPWM pad - output
    {0x3011, 0x0A},   //
    {0x3013, 0x18},   //
    {0x301c, 0xf0},   //
    {0x3022, 0x01},   //
    {0x3030, 0x10},   // Low frame rate set
    {0x3039, 0x32},   // MIPI 2 lane mode
    {0x303A, 0x00},   //
    {0x3500, 0x00},   // integration time Bit[3:0] (high)
    {0x3501, 0x5F},   // integration time Bit[7:0]
    {0x3502, 0x1e},   // integration time Bit[7:0] (low) \ Low 4 bits are fraction bits
    {0x3503, 0x08},    // AEC MANUAL
    {0x3505, 0x8c},    //
    {0x3507, 0x03},    //
    {0x3508, 0x00},    //
    {0x3509, 0x10},   // gain
    {0x3610, 0x80},   //
    {0x3611, 0xa0},   //
    {0x3620, 0x6e},   //
    {0x3632, 0x56},   //
    {0x3633, 0x78},   //
    {0x3666, 0x00},   //
    {0x366f, 0x5a},   //
    {0x3680, 0x84},   //
    {0x3712, 0x80},   //
    {0x372d, 0x22},   //
    {0x3731, 0x80},   //
    {0x3732, 0x30},   //

    {0x3778, 0x00},   //
    {0x377d, 0x22},   //

    {0x3788, 0x02},   //
    {0x3789, 0xa4},   //
    {0x378a, 0x00},   //
    {0x378b, 0x4a},   //
    {0x3799, 0x20},   //

    {0x3800, 0x00},   //
    {0x3801, 0x00},   // TIMING_X_ADDR_START
    {0x3802, 0x00},   //
    {0x3803, 0x00},   // TIMING_Y_ADDR_START

    {0x3804, 0x05},   //
    {0x3805, 0x0F},   // TIMING_X_ADDR_END
    {0x3806, 0x02},   //
    {0x3807, 0xdF},   // TIMING_Y_ADDR_END
    {0x3808, 0x05},   // TIMING_X_OUTPUT_SIZE 0x0500 = 1280
    {0x3809, 0x00},   //
    {0x380a, 0x02},   // TIMING_Y_OUTPUT_SIZE = 0x02d0 = 720
    {0x380b, 0xd0},   //


    {0x380c, 0x05},   // TIMING_HTS (PPLN) high
    {0x380d, 0xFA},   // TIMING_HTS (PPLN) low = 1530
    {0x380e, 0x06},   // TIMING_VTS (LPFR) high
    {0x380f, 0xce},   // TIMING_VTS (LPFR) low = 1742
    {0x3810, 0x00},   // ISP Horizontal Windowing Offset High Byte
    {0x3811, 0x08},   // ISP Horizontal Windowing Offset Low Byte
    {0x3812, 0x00},   // ISP Vertical Windowing Offset High Byte
    {0x3813, 0x08},   // ISP Vertical Windowing Offset Low Byte
    {0x3814, 0x11},   // TIMING_X_INC
    {0x3815, 0x11},   // TIMING_Y_INC
    {0x3820, 0x38},   // 56
    {0x3821, 0x80},   // 128
    {0x3881, 0x42},   // ??
    {0x38a8, 0x02},   // ??
    {0x38a9, 0x80},   // ??
    {0x38b1, 0x00},   // ??
    {0x38c4, 0x00},   // ??
    {0x38c5, 0xc0},   // ??
    {0x38c6, 0x04},   // ??
    {0x38c7, 0x80},   // ??
    {0x3920, 0xff},   // PWM_CTRL_20
    {0x4003, 0x40},   // BLC_CTRL_03
    {0x4008, 0x02},   //
    {0x4009, 0x05},   //
    {0x400c, 0x00},   //
    {0x400d, 0x03},   //
    {0x4010, 0x40},   //
    {0x4043, 0x40},   //
    {0x4307, 0x30},   // EMBED_CTRL
    {0x4317, 0x00},   // MIPI/DVP MODE OPTION
    {0x4501, 0x00},   // ??
    {0x4507, 0x00},   // ??
    {0x4509, 0x80},   // ??
    {0x450a, 0x08},   // ??
    {0x4601, 0x04},   // READ START L
    {0x470f, 0x00},   // BYP_SEL
    {0x4f07, 0x00},   //
    {0x4800, 0x00},   // MIPI_CTRL_00
    {0x5000, 0x9f},   // ISP CTRL 00
    {0x5001, 0x00},   //
    {0x5e00, 0x00},   // Test Pattern
    {0x5d00, 0x07},   // ??
    {0x5d01, 0x00},   // ??
    {0x0101, 0x01},   //
    {0x1000, 0x03},   //
    {0x5a08, 0x84},
};
static const uint16_t fullConfig[][2] = {
    {0x0302, 0x32}, //
    {0x030d, 0x50}, //
    {0x030e, 0x02}, //
    {0x3001, 0x00}, //
    {0x3004, 0x00}, //
    {0x3005, 0x00}, //
    {0x3006, 0x04}, //
    {0x3011, 0x0a}, //
    {0x3013, 0x18}, //
    {0x301c, 0xf0}, //
    {0x3022, 0x01}, //
    {0x3030, 0x10}, //
    {0x3039, 0x12}, // MIPI 1 lane mode
    {0x303a, 0x00}, //

    {0x3500, 0x00}, // Integration time (high)
    {0x3501, 0x0F}, // Integration time
    {0x3502, 0x1e}, // Integration time (low)

    {0x3503, 0x08}, //

    {0x3505, 0x8c}, //
    {0x3507, 0x03}, //
    {0x3508, 0x00}, //

    {0x3509, 0x10}, // Gain

    {0x3610, 0x80}, //
    {0x3611, 0xa0}, //
    {0x3620, 0x6e}, //
    {0x3632, 0x56}, //
    {0x3633, 0x78}, //
    {0x3662, 0x01}, // ;05
    {0x3666, 0x00}, //
    {0x366f, 0x5a}, //
    {0x3680, 0x84}, //
    {0x3712, 0x80}, //
    {0x372d, 0x22}, //
    {0x3731, 0x80}, //
    {0x3732, 0x30}, //
    {0x3778, 0x10}, //
    {0x377d, 0x22}, //
    {0x3788, 0x02}, //
    {0x3789, 0xa4}, //
    {0x378a, 0x00}, //
    {0x378b, 0x4a}, //
    {0x3799, 0x20}, //
    {0x3800, 0x00}, //
    {0x3801, 0x00}, //
    {0x3802, 0x00}, //
    {0x3803, 0x00}, //
    {0x3804, 0x05}, //
    {0x3805, 0x0f}, //
    {0x3806, 0x03}, //
    {0x3807, 0x2f}, //
    {0x3808, 0x02}, //
    {0x3809, 0x80}, //
    {0x380a, 0x01}, //
    {0x380b, 0x90}, //
    {0x380c, 0x02}, // TIMING_HTS (PPLN) high
    {0x380d, 0xd8}, // TIMING_HTS (PPLN) low
    {0x380e, 0x0e}, // TIMING_VTS (LPFR) high
    {0x380f, 0x50}, // TIMING_VTS (LPFR) low
    {0x3810, 0x00}, //
    {0x3811, 0x04}, //
    {0x3812, 0x00}, //
    {0x3813, 0x04}, //
    {0x3814, 0x31}, //
    {0x3815, 0x22}, //
    {0x3820, 0x60}, //
    {0x3821, 0x01}, //
    {0x3881, 0x42}, //
    {0x38a8, 0x02}, //
    {0x38a9, 0x80}, //
    {0x38b1, 0x00}, //
    {0x38c4, 0x00}, //
    {0x38c5, 0xc0}, //
    {0x38c6, 0x04}, //
    {0x38c7, 0x80}, //
    {0x3920, 0xff}, //
    {0x4003, 0x40}, //
    {0x4008, 0x02}, //
    {0x4009, 0x05}, //
    {0x400c, 0x00}, //
    {0x400d, 0x03}, //
    {0x4010, 0x40}, //
    {0x4043, 0x40}, //
    {0x4307, 0x30}, //
    {0x4317, 0x00}, //
    {0x4501, 0x00}, //
    {0x4507, 0x03}, //
    {0x4509, 0x80}, //
    {0x450a, 0x08}, //
    {0x4601, 0x04}, //
    {0x470f, 0x00}, //
    {0x4f07, 0x00}, //
    {0x4800, 0x00}, //
    {0x5000, 0x9f}, //
    {0x5001, 0x00}, //
    {0x5e00, 0x00}, //
    {0x5d00, 0x07}, //
    {0x5d01, 0x00}, //
    {0x0101, 0x01}, //
    {0x1000, 0x03}, //

    {0x5a08, 0x84},
};

static const uint16_t framePumpOv9282Raw8SensorCfg[][2] = {
    {0x3022, 0x01},
    {0x3662, 0x03},
};

static const uint16_t framePumpOv9282SensorColorbarCfg[][2] = {
    {0x5e00, 0x8c},
};

static const uint16_t framePumpOv9282SensorStartStreaming[][2] = {
    {0x0100, 0x01},
};

static const uint16_t framePumpOv9282SensorStopStreaming[][2] = {
    {0x0100, 0x00},
};

static const uint16_t framePumpOv9282Raw10SensorMetadata[][2] = {
    {0x4307, 0x31},
    {0x4816, 0x2B},
};

#define OV9282_ADDITIONAL_CONFIG                                        \
    {                                                                   \
        .regValues = framePumpOv9282SensorColorbarCfg,                  \
        .numberOfRegs = COUNT_OF(framePumpOv9282SensorColorbarCfg), \
        .configType = FRAME_PUMP_COLORBAR,                          \
        .delayMs = 0                                                \
    },                                                          \
    {                                                                   \
        .regValues = framePumpOv9282Raw10SensorMetadata,                   \
        .numberOfRegs = COUNT_OF(framePumpOv9282Raw10SensorMetadata),      \
        .configType = FRAME_PUMP_ENABLE_METADATA,                          \
        .delayMs = 0                                                       \
    },                                                                  \
    {                                                           \
        .regValues = framePumpOv9282SensorStartStreaming,       \
        .numberOfRegs = COUNT_OF(framePumpOv9282SensorStartStreaming), \
        .configType = FRAME_PUMP_START_STREAMING,           \
        .delayMs = 0                                        \
    },                                                  \
    {                                                   \
        .regValues = framePumpOv9282SensorStopStreaming, \
        .numberOfRegs = COUNT_OF(framePumpOv9282SensorStopStreaming), \
        .configType = FRAME_PUMP_STOP_STREAMING,    \
        .delayMs = 0                                \
    },

#define OV9282_BASE_CONFIG_RAW10                                        \
    {                                                                   \
        .regValues = fullConfig,                                        \
        .numberOfRegs = COUNT_OF(fullConfig),                       \
        .configType = FRAME_PUMP_FULL_CONFIG,                       \
        .delayMs = 0                                                \
    },                                                              \
    OV9282_ADDITIONAL_CONFIG

#define OV9282_BASE_CONFIG_RAW10_720P                                   \
    {                                                                   \
        .regValues = fullConfig720p,                                    \
        .numberOfRegs = COUNT_OF(fullConfig),                       \
        .configType = FRAME_PUMP_FULL_CONFIG,                       \
        .delayMs = 0                                                \
    },                                                              \
    OV9282_ADDITIONAL_CONFIG

#define OV9282_BASE_CONFIG_RAW8                \
    OV9282_BASE_CONFIG_RAW10                   \
    {                                                                   \
        .regValues = framePumpOv9282Raw8SensorCfg,                      \
            .numberOfRegs = COUNT_OF(framePumpOv9282Raw8SensorCfg),     \
            .configType = FRAME_PUMP_FULL_CONFIG,                       \
            .delayMs = 0                                                \
            },

#define OV9282_BASE_CONFIG_RAW8_720P                \
    OV9282_BASE_CONFIG_RAW10_720P                   \
    {                                                                   \
        .regValues = framePumpOv9282Raw8SensorCfg,                      \
            .numberOfRegs = COUNT_OF(framePumpOv9282Raw8SensorCfg),     \
            .configType = FRAME_PUMP_FULL_CONFIG,                       \
            .delayMs = 0                                                \
            },
static const FramePumpI2CConfigDescriptor framePumpOv9282Raw10_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW10
};

static const FramePumpI2CConfigDescriptor framePumpOv9282Raw10_right_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW10
    {
        .regValues = cfgRight,
        .numberOfRegs = COUNT_OF(cfgRight),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};

static const FramePumpI2CConfigDescriptor framePumpOv9282Raw10_720P_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW10_720P
};

static const FramePumpI2CConfigDescriptor framePumpOv9282Raw10_720P_right_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW10_720P
    {
        .regValues = cfgRight,
        .numberOfRegs = COUNT_OF(cfgRight),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};

static const FramePumpI2CConfigDescriptor framePumpOv9282Raw10_left_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW10
    {
        .regValues = cfgLeft,
        .numberOfRegs = COUNT_OF(cfgLeft),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};

static const FramePumpI2CConfigDescriptor framePumpOv9282Raw10_720P_left_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW10_720P
    {
        .regValues = cfgLeft,
        .numberOfRegs = COUNT_OF(cfgLeft),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};
static const FramePumpI2CConfigDescriptor framePumpOv9282Raw8_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW8
};
static const FramePumpI2CConfigDescriptor framePumpOv9282Raw8_720P_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW8_720P
};

static const FramePumpI2CConfigDescriptor framePumpOv9282Raw8_right_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW8
    {
        .regValues = cfgRight,
        .numberOfRegs = COUNT_OF(cfgRight),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};
static const FramePumpI2CConfigDescriptor framePumpOv9282Raw8_720P_right_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW8_720P
    {
        .regValues = cfgRight,
        .numberOfRegs = COUNT_OF(cfgRight),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};
static const FramePumpI2CConfigDescriptor framePumpOv9282Raw8_left_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW8
    {
        .regValues = cfgLeft,
        .numberOfRegs = COUNT_OF(cfgLeft),
        .configType = FRAME_PUMP_FULL_CONFIG,
        .delayMs = 0
    },
};

static const FramePumpI2CConfigDescriptor framePumpOv9282Raw8_720P_left_I2CConfigSteps[] = {
    OV9282_BASE_CONFIG_RAW8_720P
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
#define MIPI_MODE_9282_RAW8  MIPI_DATA_MODE3
#define MIPI_MODE_9282_RAW10 MIPI_DATA_MODE1
#else
#define MIPI_MODE_9282_RAW8  MIPI_D_MODE_0
#define MIPI_MODE_9282_RAW10 MIPI_D_MODE_0
#endif

#define FRAMEPUMP_OV9282_SPEC(mipiConf, i2cCfg, bpp, pixelfmt, mipifmt, mipimode) \
{                                                                       \
    .width            = OV9282_BINNED_WIDTH,                                   \
    .height           = OV9282_BINNED_HEIGHT,                                  \
    .type             = FP_MIPI,                                        \
    .spec.mipi = {                                                      \
        .dataMode     = mipimode,                                       \
        .dataRateMbps = 800,                                            \
        .nbOflanes    = 1,                                              \
        .pixelFormat  = mipifmt                                         \
    },                                                                  \
    .nbOfI2CConfigSteps    = COUNT_OF(i2cCfg),                          \
    .i2cConfigSteps        = i2cCfg,                                    \
    .ctlFnc = &framePumpOv9282Control,                                  \
};

#define FRAMEPUMP_OV9282_SPEC_720P(mipiConf, i2cCfg, bpp, pixelfmt, mipifmt, mipimode) \
{                                                                       \
    .width            = OV9282_WIDTH,                                   \
    .height           = OV9282_HEIGHT,                                  \
    .type             = FP_MIPI,                                        \
    .spec.mipi = {                                                      \
        .dataMode     = mipimode,                                       \
        .dataRateMbps = 800,                                            \
        .nbOflanes    = 1,                                              \
        .pixelFormat  = mipifmt                                         \
    },                                                                  \
    .nbOfI2CConfigSteps    = COUNT_OF(i2cCfg),                          \
    .i2cConfigSteps        = i2cCfg,                                    \
    .ctlFnc = &framePumpOv9282Control,                                  \
};

const struct FramePumpSensorDriver framePumpOv9282Raw10_camCfg =
    FRAMEPUMP_OV9282_SPEC(framePumpOv9282Raw10_mipiCfg,
                          framePumpOv9282Raw10_I2CConfigSteps,
                          2, RAW16, CSI_RAW_10, MIPI_MODE_9282_RAW10);
const struct FramePumpSensorDriver framePumpOv9282Raw10_right_camCfg =
        FRAMEPUMP_OV9282_SPEC(framePumpOv9282Raw10_mipiCfg,
                              framePumpOv9282Raw10_right_I2CConfigSteps,
                              2, RAW16, CSI_RAW_10, MIPI_MODE_9282_RAW10);
const struct FramePumpSensorDriver framePumpOv9282Raw10_left_camCfg =
    FRAMEPUMP_OV9282_SPEC(framePumpOv9282Raw10_mipiCfg,
                          framePumpOv9282Raw10_left_I2CConfigSteps,
                          2, RAW16, CSI_RAW_10, MIPI_MODE_9282_RAW10);

const struct FramePumpSensorDriver framePumpOv9282Raw8_camCfg =
    FRAMEPUMP_OV9282_SPEC(framePumpOv9282Raw8_mipiCfg,
                          framePumpOv9282Raw8_I2CConfigSteps,
                          1, RAW8, CSI_RAW_8, MIPI_MODE_9282_RAW8);
const struct FramePumpSensorDriver framePumpOv9282Raw8_right_camCfg =
        FRAMEPUMP_OV9282_SPEC(framePumpOv9282Raw8_mipiCfg,
                              framePumpOv9282Raw8_right_I2CConfigSteps,
                              1, RAW8, CSI_RAW_8, MIPI_MODE_9282_RAW8);
const struct FramePumpSensorDriver framePumpOv9282Raw8_left_camCfg =
    FRAMEPUMP_OV9282_SPEC(framePumpOv9282Raw8_mipiCfg,
                          framePumpOv9282Raw8_left_I2CConfigSteps,
                          1, RAW8, CSI_RAW_8, MIPI_MODE_9282_RAW8);


const struct FramePumpSensorDriver framePumpOv9282Raw10_720P_camCfg =
                FRAMEPUMP_OV9282_SPEC_720P(framePumpOv9282Raw10_mipiCfg,
                          framePumpOv9282Raw10_720P_I2CConfigSteps,
                          2, RAW16, CSI_RAW_10, MIPI_MODE_9282_RAW10);
const struct FramePumpSensorDriver framePumpOv9282Raw10_720P_right_camCfg =
                FRAMEPUMP_OV9282_SPEC_720P(framePumpOv9282Raw10_mipiCfg,
                              framePumpOv9282Raw10_720P_right_I2CConfigSteps,
                              2, RAW16, CSI_RAW_10, MIPI_MODE_9282_RAW10);
const struct FramePumpSensorDriver framePumpOv9282Raw10_720P_left_camCfg =
                FRAMEPUMP_OV9282_SPEC_720P(framePumpOv9282Raw10_mipiCfg,
                          framePumpOv9282Raw10_720P_left_I2CConfigSteps,
                          2, RAW16, CSI_RAW_10, MIPI_MODE_9282_RAW10);

const struct FramePumpSensorDriver framePumpOv9282Raw8_720P_camCfg =
                FRAMEPUMP_OV9282_SPEC_720P(framePumpOv9282Raw8_mipiCfg,
                          framePumpOv9282Raw8_720P_I2CConfigSteps,
                          1, RAW8, CSI_RAW_8, MIPI_MODE_9282_RAW8);
const struct FramePumpSensorDriver framePumpOv9282Raw8_720P_right_camCfg =
                FRAMEPUMP_OV9282_SPEC_720P(framePumpOv9282Raw8_mipiCfg,
                              framePumpOv9282Raw8_720P_right_I2CConfigSteps,
                              1, RAW8, CSI_RAW_8, MIPI_MODE_9282_RAW8);
const struct FramePumpSensorDriver framePumpOv9282Raw8_720P_left_camCfg =
                FRAMEPUMP_OV9282_SPEC_720P(framePumpOv9282Raw8_mipiCfg,
                          framePumpOv9282Raw8_720P_left_I2CConfigSteps,
                          1, RAW8, CSI_RAW_8, MIPI_MODE_9282_RAW8);



static uint32_t pll1PreDiv0LT[2] = {
    1,      // 0x00
    2       // 0x01
};

static float pll1PreDivLT[8] = {
    1.0f,   // 0x00
    1.5f,   // 0x01
    2.0f,   // 0x02
    2.5f,   // 0x03
    3.0f,   // 0x04
    4.0f,   // 0x05
    6.0f,   // 0x06
    8.0f    // 0x07
};

static uint32_t pll1MipiDivLT[4] = {
    4,      // 0x00
    5,      // 0x01
    6,      // 0x02
    8       // 0x03
};

static uint32_t pll2PreDiv0LT[2] = {
    1,      // 0x00
    2       // 0x01
};

static float pll2PreDivLT[8] = {
    1.0f,   // 0x00
    1.5f,   // 0x01
    2.0f,   // 0x02
    2.5f,   // 0x03
    3.0f,   // 0x04
    4.0f,   // 0x05
    6.0f,   // 0x06
    8.0f    // 0x07
};

static float pll2SysDivLT[8] = {
    1.0f,   // 0x00
    1.5f,   // 0x01
    2.0f,   // 0x02
    2.5f,   // 0x03
    3.0f,   // 0x04
    3.5f,   // 0x05
    4.0f,   // 0x06
    5.0f    // 0x07
};

static int32_t htsStored = 0; // Stored HTS value on first run to avoid reading camera registers on each call
static int32_t vtsStored = 0; // Stored VTS value on first run to avoid reading camera registers on each call
static float pclkMhzStored = 0; // Stored PCLK value on first run to avoid reading camera registers on each call
static float sclkMhzStored = 0; // Stored SCLK value on first run to avoid reading camera registers on each call

static rtems_status_code ov9282SendArrayToRegister (int fd, uint16_t reg, uint8_t * array, int count)
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

static rtems_status_code framePumpSensorOv9282Flip (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    int flip)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    int rval = 0;
    uint8_t regVal = 0;

    (void)control;

    i2cCtl.regAddress = OV9282_TIMING_FORMAT1;
    i2cCtl.regValue = &regVal;
    i2cCtl.wordCount = 1;

    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
            mvLog(MVLOG_WARN, "I2C sensor read failed at address 0x%x \n"
                   "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
            return RTEMS_IO_ERROR;
    }

    if (flip) {
        *(uint8_t*)i2cCtl.regValue |= OV9282_TFMT1_V_FLIP_MASK;
    } else {
        *(uint8_t*)i2cCtl.regValue &= ~OV9282_TFMT1_V_FLIP_MASK;
    }

    rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        mvLog(MVLOG_WARN, "I2C sensor configuration failed at address 0x%x \n"
              "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return RTEMS_IO_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv9282Mirror (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    int mirror)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    int rval = 0;
    uint8_t regVal = 0;

    (void)control;

    i2cCtl.regAddress = OV9282_TIMING_FORMAT2;
    i2cCtl.regValue = &regVal;
    i2cCtl.wordCount = 1;

    rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
            mvLog(MVLOG_WARN, "I2C sensor read failed at address 0x%x \n"
                   "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
            return RTEMS_IO_ERROR;
    }

    if (mirror) {
        *(uint8_t*)i2cCtl.regValue |= OV9282_TFMT2_H_MIRROR_MASK;
    } else {
        *(uint8_t*)i2cCtl.regValue &= ~OV9282_TFMT2_H_MIRROR_MASK;
    }

    rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
    if (rval != i2cCtl.wordCount) {
        mvLog(MVLOG_WARN, "I2C sensor configuration failed at address 0x%x \n"
              "        with errcode %d \n", (unsigned int)i2cCtl.regAddress, rval);
        return RTEMS_IO_ERROR;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv9282GetHTS (
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
        /// Get HTS High Byte (0x380C)
        i2cCtl.regAddress = OV9282_HTS_H;
        i2cCtl.regValue = &readbyte[0];
        i2cCtl.wordCount = 1;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }

        /// Get HTS Low Byte (0x380D)
        i2cCtl.regAddress = OV9282_HTS_L;
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

static rtems_status_code framePumpSensorOv9282GetVTS (
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
        i2cCtl.regAddress = OV9282_VTS_H;
        i2cCtl.regValue = &readbyte[0];
        i2cCtl.wordCount = 1;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }

        // Get VTS Low Byte (0x380F)
        i2cCtl.regAddress = OV9282_VTS_L;
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

static rtems_status_code __attribute__((unused)) framePumpSensorOv9282GetPclk (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    float *pclkMhz)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    uint8_t readbyte[2] = {0};
    float refClk = 24.0f;
    uint32_t pll1PreDiv0 = 0;
    uint32_t pll1Multi = 0;
    uint32_t pll1MipiPreDiv = 0;
    uint32_t pll1MipiDiv = 0;
    float pll1PreDiv = 0.0f;
    float pll1Clk = 0.0f;
    int32_t rval = 0;

    (void)control;

    if (pclkMhzStored == 0) { // First run: get PCLK value by reading camera registers and store it
        i2cCtl.regValue = &readbyte[0];
        i2cCtl.wordCount = 1;

        // Get PLL1_pre_divider_0 (0x030A[0])
        i2cCtl.regAddress = OV9282_PLL1_PREDIV0;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll1PreDiv0 = pll1PreDiv0LT[readbyte[0] & OV9282_PLL1_PREDIV0_MASK];

        // Get PLL1_pre_divider (0x0300[2:0])
        i2cCtl.regAddress = OV9282_PLL1_PREDIV;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll1PreDiv = pll1PreDivLT[readbyte[0] & OV9282_PLL1_PREDIV_MASK];

        // Get PLL1_multiplier (0x0301[1:0], 0x0302[7:0])
        i2cCtl.regAddress = OV9282_PLL1_MULTI_H;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll1Multi = (readbyte[0] & OV9282_PLL1_MULTI_H_MASK) << 8;

        i2cCtl.regAddress = OV9282_PLL1_MULTI_L;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll1Multi |= (readbyte[0] & OV9282_PLL1_MULTI_L_MASK);

        // Get PLL1_mipi_pre_divider (0x0303[3:0])
        i2cCtl.regAddress = OV9282_PLL1_MIPI_PREDIV;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll1MipiPreDiv = (readbyte[0] & OV9282_PLL1_MIPI_PREDIV_MASK) + 1;

        // Get PLL1_mipi_divider (0x0304[1:0])
        i2cCtl.regAddress = OV9282_PLL1_MIPI_DIV;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll1MipiDiv = pll1MipiDivLT[readbyte[0] & OV9282_PLL1_MIPI_DIV_MASK];

        // Compute the pixel clock
        pll1Clk = (refClk / pll1PreDiv0 / pll1PreDiv) * pll1Multi;

        mvLog(MVLOG_DEBUG, "PLL1Clk is %f", pll1Clk);
        mvLog(MVLOG_DEBUG, "PLL1MipiClk pre-divider is %u, divider is %u", pll1MipiPreDiv, pll1MipiDiv);

        *pclkMhz = pll1Clk / pll1MipiPreDiv / pll1MipiDiv;
        pclkMhzStored = *pclkMhz;
    } else { // Subsequent runs: return the stored PCLK value
        *pclkMhz = pclkMhzStored;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv9282GetSclk (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    float *sclkMhz)
{
    int fd = ctx->ctlFd;
    struct i2cStruct_t i2cCtl = {0};
    uint8_t readbyte[2] = {0};
    float refClk = 24.0f;
    uint32_t pll2PreDiv0 = 0;
    uint32_t pll2Multi = 0;
    uint32_t pll2SysPreDiv = 0;
    float pll2PreDiv = 0.0f;
    float pll2Clk = 0.0f;
    float pll2SysDiv = 0.0f;
    int32_t rval = 0;

    (void)control;

    if (sclkMhzStored == 0) { // First run: get SCLK value by reading camera registers and store it
        i2cCtl.regValue = &readbyte[0];
        i2cCtl.wordCount = 1;

        // Get PLL2_pre_divider_0 (0x0314[0])
        i2cCtl.regAddress = OV9282_PLL2_PREDIV0;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll2PreDiv0 = pll2PreDiv0LT[readbyte[0] & OV9282_PLL2_PREDIV0_MASK];

        // Get PLL2_pre_divider (0x030B[2:0])
        i2cCtl.regAddress = OV9282_PLL2_PREDIV;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll2PreDiv = pll2PreDivLT[readbyte[0] & OV9282_PLL2_PREDIV_MASK];

        // Get PLL2_multiplier (0x030C[1:0], 0x030D[7:0])
        i2cCtl.regAddress = OV9282_PLL2_MULTI_H;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll2Multi = (readbyte[0] & OV9282_PLL2_MULTI_H_MASK) << 8;

        i2cCtl.regAddress = OV9282_PLL2_MULTI_L;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll2Multi |= (readbyte[0] & OV9282_PLL2_MULTI_L_MASK);

        // Get PLL2_sys_pre_divider (0x030F[3:0])
        i2cCtl.regAddress = OV9282_PLL2_SYS_PREDIV;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll2SysPreDiv = (readbyte[0] & OV9282_PLL2_SYS_PREDIV_MASK) + 1;

        // Get PLL2_sys_divider (0x030E[2:0])
        i2cCtl.regAddress = OV9282_PLL2_SYS_DIV;
        rval = ioctl(fd, CTL_READ_REGISTER, &i2cCtl);
        if(rval != i2cCtl.wordCount) {
            return RTEMS_IO_ERROR;
        }
        pll2SysDiv = pll2SysDivLT[readbyte[0] & OV9282_PLL2_SYS_DIV_MASK];

        // Compute the system clock
        pll2Clk = (refClk / pll2PreDiv0 / pll2PreDiv) * pll2Multi;

        mvLog(MVLOG_DEBUG, "PLL2Clk is %f", pll2Clk);
        mvLog(MVLOG_DEBUG, "PLL2SysClk pre-divider is %u, divider is %f", pll2SysPreDiv, pll2SysDiv);

        *sclkMhz = pll2Clk / pll2SysPreDiv / pll2SysDiv;
        sclkMhzStored = *sclkMhz;
    } else { // Subsequent runs: return the stored SCLK value
        *sclkMhz = sclkMhzStored;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv9282GetReadoutUs (
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

    sc = framePumpSensorOv9282GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv9282GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    *readoutUs = (uint32_t)((float)(camHeight * hts) / sclkMhz);

    mvLog(MVLOG_DEBUG, "Readout time is %lu", *readoutUs);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv9282GetFrameRate (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t *frameRate)
{
    float sclkMhz = 0.0f;
    int32_t hts = 0;
    int32_t vts = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = framePumpSensorOv9282GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv9282GetVTS(ctx, control, &vts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv9282GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    *frameRate = (uint32_t)(sclkMhz * 1000000 / (float)(vts * hts));

    mvLog(MVLOG_DEBUG, "Frame-rate is %lu fps", *frameRate);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv9282SetFrameRate (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t fd,
    uint32_t frameRate)
{
    float sclkMhz = 0.0f;
    float tROW = 0.0f;
    int32_t hts = 0;
    int16_t regVal = 0;

    rtems_status_code sc = framePumpSensorOv9282GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv9282GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    tROW = (1.0 / sclkMhz) * hts;

    regVal = (uint32_t)(1000.0 / frameRate / tROW *1000.0);
    return ov9282SendArrayToRegister(fd, OV9282_VTS_L, (uint8_t*)&regVal, 2);
}

static rtems_status_code framePumpSensorOv9282GetExpGainInfo (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    FramePumpCamExpGainInfo *infoExpGain)
{
    float sclkMhz = 0.0f;
    float tROW = 0.0f;
    int32_t hts = 0;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = framePumpSensorOv9282GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv9282GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    tROW = (1 / sclkMhz) * hts;

    infoExpGain->min.exposureTimeUs = (uint32_t)(OV9282_MIN_EXPOSURE_LINES * tROW);
    infoExpGain->min.gainMultiplier = OV9282_MIN_TOTAL_GAIN;

    infoExpGain->max.exposureTimeUs = (uint32_t)(OV9282_MAX_EXPOSURE_LINES * tROW);
    infoExpGain->max.gainMultiplier = OV9282_MAX_TOTAL_GAIN;

    infoExpGain->start.exposureTimeUs = (uint32_t)(OV9282_START_EXPOSURE_LINES * tROW);
    infoExpGain->start.gainMultiplier = OV9282_START_GAIN;

    infoExpGain->gainFractBits = OV9282_LIN_GAIN_FRACT_BITS;

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv9282GetExpGainTable (
    FramePumpCamExpGainTable *tblExpGain)
{
    int i = 0;
    int rgnNmbr = 0;

    rgnNmbr = COUNT_OF(ov9282ExpGainRegionTbl);
    if (rgnNmbr > FP_EXPGAIN_TBL_MAX_SZ) {
        rgnNmbr = FP_EXPGAIN_TBL_MAX_SZ;
        mvLog(MVLOG_WARN, "OV9282 {Exposure,Gain} regions table is too big -> truncated !!!\n");
    }

    for (i = 0; i < rgnNmbr; i++) {
        tblExpGain->region[i] = ov9282ExpGainRegionTbl[i];
    }

    if (rgnNmbr < FP_EXPGAIN_TBL_MAX_SZ)        /// If table is not full => fill in a termination entry
    {
        tblExpGain->region[i].exposureTimeUs = 0;
        tblExpGain->region[i].gainMultiplier = 0.f;
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code framePumpSensorOv9282GetMaxExposureUs (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control,
    uint32_t *maxExposureUs)
{
    float sclkMhz = 0.0f;
    float tROW = 0.0f;
    int32_t hts = 0;
    int32_t sc = RTEMS_SUCCESSFUL;

    sc = framePumpSensorOv9282GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    sc = framePumpSensorOv9282GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    tROW = (1 / sclkMhz) * hts;

    *maxExposureUs = (uint32_t)(OV9282_MAX_EXPOSURE_LINES * tROW);

    mvLog(MVLOG_DEBUG, "Max Exposure time is %lu", *maxExposureUs);

    return RTEMS_SUCCESSFUL;
}

// exposuretime = (Exposure[19:4] + Exposure[3:0]*1/16) * tROW
// tROW = 1/SCLK * TIMING_HTS
static int framePumpSensorOv9282SetExposureUs ( struct FramePumpVideoContext *ctx,
                                                struct FramePumpControlConfig *control,
                                                uint32_t fd,
                                                uint32_t exposureUs)
{
    float exposureVal = 0.0f;
    int32_t exposureReg = 0;
    int32_t hts = 0;
    float sclkMhz = 0.0f;
    float tROW = 0.0f;
    int32_t sc = RTEMS_SUCCESSFUL;
    uint32_t maxExposure = 0;

    sc = framePumpSensorOv9282GetHTS(ctx, control, &hts);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    mvLog(MVLOG_DEBUG, "HTS is %ld", hts);

    sc = framePumpSensorOv9282GetSclk(ctx, control, &sclkMhz);
    if (sc != RTEMS_SUCCESSFUL) {
        return sc;
    }

    mvLog(MVLOG_DEBUG, "SCLK is %f", sclkMhz);

    tROW = (1 / sclkMhz) * hts;

    maxExposure = (uint32_t)(OV9282_MAX_EXPOSURE_LINES * tROW);

    mvLog(MVLOG_DEBUG, "Max Exposure time is %lu", maxExposure);

    exposureUs = MIN(exposureUs, maxExposure);

    exposureVal = exposureUs / tROW;

    // LOW 4 bits are fraction bits
    exposureVal = exposureVal * 16;

    exposureReg = (uint32_t)exposureVal;

    return ov9282SendArrayToRegister(fd, OV9282_EXPOSURE_7_0, (uint8_t*)&exposureReg, 3);
}

static int framePumpSensorOv9282SetGainFloat(struct FramePumpVideoContext *ctx,
                                             struct FramePumpControlConfig *control,
                                             uint32_t fd,
                                             float gain)
{
    uint8_t regVal[2] = {0};

    (void)ctx;
    (void)control;

    regVal[0] = (uint8_t)(gain * 16);

    return ov9282SendArrayToRegister(fd, OV9282_GAIN_AGC_ADJ, (uint8_t*)regVal, 1);
}

static int framePumpSensorOv9282SetExpGainSafely(struct FramePumpVideoContext *ctx,
                                                 struct FramePumpControlConfig *control,
                                                 uint32_t fd,
                                                 FramePumpCamExpGain *camExpGain)
{
    uint8_t regVal = 0;
    int sts = 0;

    regVal = OV9282_GPH_G0_START;
    sts = ov9282SendArrayToRegister(fd, OV9282_GROUP_ACCESS, &regVal, 1) << 0;

    sts |= framePumpSensorOv9282SetExposureUs(ctx, control, fd, camExpGain->exposureTimeUs) << 8;

    sts |= framePumpSensorOv9282SetGainFloat(ctx, control, fd, camExpGain->gainMultiplier) << 16;

    regVal = OV9282_GPH_G0_END;
    sts |= ov9282SendArrayToRegister(fd, OV9282_GROUP_ACCESS, &regVal, 1) << 24;

    regVal = OV9282_GPH_G0_DELAY_LAUNCH;
    sts |= ov9282SendArrayToRegister(fd, OV9282_GROUP_ACCESS, &regVal, 1) << 28;

    return sts;
}

static rtems_status_code framePumpSensorOv9282Config (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control)
{
    int fd = 0;

    if (FP_IS_SHARED_CTL(control->cfgType)) {
        fd = ctx->ctlFdShared;
    } else {
        fd = ctx->ctlFd;
    }

    switch (FP_STRIP_CTL(control->cfgType)) {
    case FRAME_PUMP_CONFIG_SEND_EXPOSURE:
        return framePumpSensorOv9282SetExposureUs(ctx, control, fd, *(uint32_t*)control->buffer);
    case FRAME_PUMP_GET_MAX_EXPOSURE:
        return framePumpSensorOv9282GetMaxExposureUs(ctx, control, (uint32_t*)control->buffer);
    case FRAME_PUMP_CONFIG_SEND_GAIN:
        return framePumpSensorOv9282SetGainFloat(ctx, control, fd, *(float*)control->buffer);
    case FRAME_PUMP_CONFIG_SEND_EXPGAIN:
        return framePumpSensorOv9282SetExpGainSafely(ctx, control, fd, (FramePumpCamExpGain *)control->buffer);
    case FRAME_PUMP_CONFIG_GET_READOUT:
        return framePumpSensorOv9282GetReadoutUs(ctx, control, (uint32_t*)control->buffer);
    case FRAME_PUMP_CONFIG_FLIP:
        return framePumpSensorOv9282Flip(ctx, control, *(uint32_t*)control->buffer);
    case FRAME_PUMP_CONFIG_MIRROR:
        return framePumpSensorOv9282Mirror(ctx, control, *(uint32_t*)control->buffer);
    case FRAME_PUMP_GET_TESTPATTERN_CRC:
        if(ctx->outSpec.spec.type == RAW8)
            *(uint32_t*)control->buffer = OV9282_TESTPATTERN_CRC_8BIT;
        else
            *(uint32_t*)control->buffer = OV9282_TESTPATTERN_CRC_10BIT;
        return RTEMS_SUCCESSFUL;
    case FRAME_PUMP_GET_FRAMERATE:
        return framePumpSensorOv9282GetFrameRate(ctx, control, (uint32_t*)control->buffer);
    case FRAME_PUMP_SET_FRAMERATE:
        return framePumpSensorOv9282SetFrameRate(ctx, control, fd, *(uint32_t*)control->buffer);
    case FRAME_PUMP_GET_EXPGAIN_INFO:
        return framePumpSensorOv9282GetExpGainInfo(ctx, control, (FramePumpCamExpGainInfo *)control->buffer);
    case FRAME_PUMP_GET_EXPGAIN_TABLE:
        return framePumpSensorOv9282GetExpGainTable((FramePumpCamExpGainTable *)control->buffer);
    default:
        return framePumpSensorConfig(ctx, control);
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code framePumpSensorOv9282FrameCb (
    const struct FramePumpVideoContext *ctx,
    struct FramePumpBuffer *fpb)
{
    (void)ctx;
    (void)fpb;
#if 0  /// exposure calculations from embedded data line:

//{0x4307, 0x31}, // enable embedded data
//{0x4816, 0x2B}, // embedded datatype


    uint32_t actualValue = 0;

//fractional
    actualValue |= ((fpb.fb.p1[28] & 0x0C) >> 2) <<  0; // 2 bits
    actualValue |= ((fpb.fb.p1[22] & 0x03) >> 0) <<  2; // 2 bits

    actualValue |= ((fpb.fb.p1[18] & 0xC0) >> 6) <<  4; // 2 bits
    actualValue |= ((fpb.fb.p1[16] & 0x3F) >> 0) <<  6; // 6 bits
    actualValue |= ((fpb.fb.p1[18] & 0x30) >> 4) << 12; // 2 bits
    actualValue |= ((fpb.fb.p1[14] & 0x3F) >> 0) << 14; // 6 bits


#endif

#if 0  /// gain calculations from embedded data line:

//{0x4307, 0x31}, // enable embedded data
//{0x4816, 0x2B}, // embedded datatype

    uint32_t actualValue = 0;

//fractional
    actualValue |= ((fpb.fb.p1[18] & 0x03) >> 0) <<  0; // 2 bits
    actualValue |= ((fpb.fb.p1[06] & 0x3F) >> 0) <<  2; // 6 bits

#endif

#if 0
    if(ctx->outSpec.hasMetadata)
    {
        uint32_t exposureRegValue = 0;
        uint32_t gainRegValue = 0;

        exposureRegValue |= ((fpb->fb.p1[22] & 0x0F) >> 0) <<  0; // 4 bits
        exposureRegValue |= ((fpb->fb.p1[16] & 0xFF) >> 0) <<  4; // 8 bits
        exposureRegValue |= ((fpb->fb.p1[14] & 0xFF) >> 0) << 12; // 8 bits

        fpb->exposureNs = exposureRegValue; // TODO: calculate time

        gainRegValue |= ((fpb->fb.p1[6] & 0xFF) >> 0) <<  0; // 8 bits

        fpb->gain = gainRegValue; // TODO: calculate time
    }
#endif
    return RTEMS_SUCCESSFUL;
}

struct FramePumpControlFunctions framePumpOv9282Control = {
    .open = framePumpSensorOpen,
    .close = framePumpSensorClose,
    .config = framePumpSensorOv9282Config,
    .power_up = framePumpSensorPowerUp,
    .power_down = framePumpSensorPowerDown,
    .frameCb = framePumpSensorOv9282FrameCb,
};

static rtems_status_code i2cWriteRegAtHackedAddress (
    rtems_device_minor_number minor,
    uint8_t hackedAddress,
    uint16_t regAddr,
    uint8_t data)
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

    sc = rtems_libi2c_send_addr(minor, (int)OV9282_WRITE);
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
    rtems_device_minor_number minor,
    const uint16_t chipID)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    uint16_t regAddr = CONV_U16_LE2BE(OV9282_SC_CHIP_ID_HIGH);
    uint16_t data = 0xDEAD;

    sc = rtems_libi2c_send_start(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_start failed %d", sc);
        return sc;
    }

    sc = rtems_libi2c_send_addr(minor, (int)OV9282_WRITE);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_send_addr W failed %d", sc);
        return sc;
    }

    sc = rtems_libi2c_write_bytes(minor, (uint8_t *)&regAddr, sizeof(regAddr));
    if (sc != sizeof(regAddr)) {
        mvLog(MVLOG_DEBUG, "rtems_libi2c_read/write regaddr failed, sc = %d", sc);
        return RTEMS_IO_ERROR;
    }

    sc = rtems_libi2c_send_addr(minor, (int)OV9282_READ);
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

    mvLog(MVLOG_DEBUG, "Found ov9282 %x", data);

    if (data == CONV_U16_LE2BE(chipID)) {
        return RTEMS_SUCCESSFUL;
    } else {
        return RTEMS_INVALID_ID;
    }
}

#ifdef MA2X8X
void setPin(int fd, int value) {
    ioctl(fd, GPIO_WRITE, &value);
}
#else
void setPin(int fd, int value) {
  write(fd, &value, sizeof(value));
}
#endif

static rtems_status_code ov9282Init (
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *arg)
{
    int sc = RTEMS_SUCCESSFUL;
    int leftResetFd = 0;
    int rightResetFd = 0;
    char gpioName[64] = {0};
    int busNumber = (minor >> 10) & 7;
    uint8_t i2c_addr = minor & 0xFF;
    bool set_addr = false;

    (void)major;
    (void)arg;

    sprintf(gpioName, "/dev/gpio.ov7750_bus%1d_reset_%s", busNumber, "left");
    leftResetFd  = open(gpioName, O_RDWR);

    sprintf(gpioName, "/dev/gpio.ov7750_bus%1d_reset_%s", busNumber, "right");
    rightResetFd = open(gpioName, O_RDWR);

    rtems_task_wake_after(20);
    switch (i2c_addr) {
    case OV9282_I2C_ADDRESS_LEFT:
      setPin(leftResetFd, 0);
      setPin(rightResetFd, 0);
      rtems_task_wake_after(20);
      setPin(leftResetFd, 1);
      set_addr = true;
      break;
    case OV9282_I2C_ADDRESS_RIGHT:
      setPin(rightResetFd, 1);
      set_addr = true;
      break;
    case OV9282_I2C_ADDRESS_LEFT_SID:
      setPin(leftResetFd, 1);
        break;
    case OV9282_I2C_ADDRESS_RIGHT_SID:
      setPin(rightResetFd, 1);
      break;
    default:
        return RTEMS_INVALID_NUMBER;
    }
    rtems_task_wake_after(20);

    close(leftResetFd);
    close(rightResetFd);

#ifdef MA2X8X
    sc = rtems_libi2c_ioctl(minor, OS_DRV_I2C_MASTER_INTERNAL_ALLOC_BUS);
#endif

    // If the address is not controlled by the SID pin, use the default
    // broadcast address to fix the addresses.  Left camera is set up first.
    // Use the default broadcast address to change the address of the left
    // camera while the right camera is held in reset. Also change the
    // broadcast address so the left camera no longer responds to the default.
    // Then, use the default broadcast address to change the address for the
    // right camera with the left camera not in reset (left camera no longer
    // responds to default broadcast address because it has been changed).
    if (set_addr) {
        sc = i2cWriteRegAtHackedAddress(minor,
                                        OV9282_I2C_ADDRESS_BROADCAST_DEFAULT,
                                        OV9282_SC_SCCB_ID_SID0,
                                        i2c_addr << 1);
        if (sc != RTEMS_SUCCESSFUL) {
            goto ov9282InitExit;
        }

        sc = i2cWriteRegAtHackedAddress(minor,
                                        OV9282_I2C_ADDRESS_BROADCAST_DEFAULT,
                                        OV9282_SC_SCCB_ID_SID1,
                                        i2c_addr << 1);
        if (sc != RTEMS_SUCCESSFUL) {
            goto ov9282InitExit;
        }

        sc = i2cWriteRegAtHackedAddress(minor,
                                        OV9282_I2C_ADDRESS_BROADCAST_DEFAULT,
                                        OV9282_SC_PROG_ID_ENB,
                                        0x02);
        if (sc != RTEMS_SUCCESSFUL) {
            goto ov9282InitExit;
        }
    }

    // Change the default broadcast address to our own broadcast address.
    sc = i2cWriteRegAtHackedAddress(minor, i2c_addr,
                                    OV9282_SC_SCCB_ID_BROADCAST,
                                    OV9282_I2C_ADDRESS_BROADCAST << 1);
    if (sc != RTEMS_SUCCESSFUL) {
        goto ov9282InitExit;
    }

    sc = i2cWriteRegAtHackedAddress(minor, i2c_addr, OV9282_SC_PROG_ID_ENB,
                                    0x02);
    if (sc != RTEMS_SUCCESSFUL) {
        goto ov9282InitExit;
    }

    sc = checkChipId(minor, OV9282_CHIP_ID);
    if (sc != RTEMS_SUCCESSFUL) {
        goto ov9282InitExit;
    }

ov9282InitExit:
    mvLog(MVLOG_DEBUG, "ov9282 sc = %d", sc);
    return sc < 0 ? -sc : sc;
}

static rtems_status_code ov9282InitBroadcast (
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *arg)
{
    int sc = RTEMS_SUCCESSFUL;
    (void)major;
    (void)arg;
    sc = checkChipId(minor, OV9282_CHIP_ID);
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

static rtems_driver_address_table libi2cOv9282ProtocolAdrTbl = {
    .initialization_entry = ov9282Init,
    .read_entry = anythingSuccessDevice,
    .write_entry = anythingSuccessDevice,
    .open_entry = anythingSuccessDevice,
    .close_entry = anythingSuccessDevice,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

rtems_libi2c_drv_t libi2cOv9282ProtocolDrvTbl = {
    .ops = &libi2cOv9282ProtocolAdrTbl,
    .size = sizeof(libi2cOv9282ProtocolDrvTbl),
};

static rtems_driver_address_table libi2cOv9282BroadcastProtocolAdrTbl = {
    .initialization_entry = ov9282InitBroadcast,
    .read_entry = anythingSuccessDevice,
    .write_entry = anythingSuccessDevice,
    .open_entry = anythingSuccessDevice,
    .close_entry = anythingSuccessDevice,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

rtems_libi2c_drv_t libi2cOv9282BroadcastProtocolDrvTbl = {
    .ops = &libi2cOv9282BroadcastProtocolAdrTbl,
    .size = sizeof(libi2cOv9282BroadcastProtocolAdrTbl),
};
