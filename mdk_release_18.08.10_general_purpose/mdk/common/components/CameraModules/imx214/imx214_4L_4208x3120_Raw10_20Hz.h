//
// 20fps live images at 4208x3120 pixels, sent on 4 MIPI lanes at 810 Mbps per lane;
//
//
//HEADER_FORMAT = V2.1

#ifndef _IMX214_4L_4208x3120_RAW10_20HZ_H_
#define _IMX214_4L_4208x3120_RAW10_20HZ_H_

#include "mv_types.h"
#include "CameraDefines.h"


#define IMX214_4L_4208x3120_RAW10_20HZ_I2C_CONFIG_STEPS (3)
//there must be at least 3 standard steps present in the file:
//0)          configure registers set0
//(1), (2)... optionally configure other registers set
//N-1)        set the streaming register (individual step)
//N)          set the standby register (individual step)


const u16 imx214_4L_4208x3120_RAW10_20Hz_I2Cregs[][2] = {
//configuration step 0
{0x0101, 0x00}, 
{0x4601, 0x00}, 
{0x4642, 0x05}, 
{0x6276, 0x00}, 
{0x900E, 0x05}, 
{0xA802, 0x90}, 
{0xA803, 0x11}, 
{0xA804, 0x62}, 
{0xA805, 0x77}, 
{0xA806, 0xAE}, 
{0xA807, 0x34}, 
{0xA808, 0xAE}, 
{0xA809, 0x35}, 
{0xAE33, 0x00}, 
{0x4612, 0x29}, 
{0x461B, 0x12}, 
{0x4637, 0x30}, 
{0x463F, 0x18}, 
{0x4641, 0x0D}, 
{0x465B, 0x12}, 
{0x465F, 0x11}, 
{0x4663, 0x11}, 
{0x4667, 0x0F}, 
{0x466F, 0x0F}, 
{0x470E, 0x09}, 
{0x4915, 0x5D}, 
{0x4A5F, 0xFF}, 
{0x4A61, 0xFF}, 
{0x4A73, 0x62}, 
{0x4A85, 0x00}, 
{0x4A87, 0xFF}, 
{0x620E, 0x04}, 
{0x6EB2, 0x01}, 
{0x6EB3, 0x00}, 

// Image quality
{0x6D12, 0x3F}, 
{0x6D13, 0xFF}, 
{0x9344, 0x03}, 
{0x3001, 0x07}, 
{0x69DB, 0x01}, 
{0x974C, 0x00}, 
{0x974D, 0x40}, 
{0x974E, 0x00}, 
{0x974F, 0x40}, 
{0x9750, 0x00}, 
{0x9751, 0x40}, 
{0x6957, 0x01}, 
{0x6987, 0x17}, 
{0x698A, 0x03}, 
{0x698B, 0x03}, 

//Preset setting
//Mode setting 
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
{0x3000, 0x3D}, 
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
{0x0303, 0x02}, 
{0x0305, 0x03}, 
{0x0306, 0x00}, 
{0x0307, 0x64}, 
{0x0309, 0x0A}, 
{0x030B, 0x01}, 
{0x0310, 0x00}, 
//Data Rate setting
{0x0820, 0x0C}, 
{0x0821, 0x80}, 
{0x0822, 0x00}, 
{0x0823, 0x00}, 
//WaterMark setting
{0x3A03, 0x08}, 
{0x3A04, 0x20}, 
//Enable setting
{0x0B06, 0x01}, 
//Test setting
{0x30A2, 0x00}, 
{0x30B4, 0x00}, 
//HDR setting 
{0x3A02, 0xFF}, 
//STATS setting
{0x3013, 0x00}, 
{0x5062, 0x10}, 
{0x5063, 0x70}, 
{0x5064, 0x00}, 
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
//CP clock setting
//INCK setting
//Streaming setting
{0x0101, 0x0},

//IMX214ES1only L - ext clk freq ???
{0x0136, 0x18}, 
{0x0137, 0x00}, 

// Sensor operation start
{0x0100, 0x1},
{0x0100, 0x1},

//configuration step 1 (individual step)
{0x0100, 0x01},  // Enable streaming

//configuration step 2 (individual step)
{0x0100, 0x00}  // Disable streaming

};


I2CConfigDescriptor imx214_4L_4208x3120_RAW10_20Hz_i2cConfigSteps[IMX214_4L_4208x3120_RAW10_20HZ_I2C_CONFIG_STEPS] =
{
      { .numberOfRegs = (sizeof(imx214_4L_4208x3120_RAW10_20Hz_I2Cregs)/sizeof(imx214_4L_4208x3120_RAW10_20Hz_I2Cregs[0]) -2),
        .delayMs = 0,
      },
      { .numberOfRegs = 1, //the streaming configuration step
        .delayMs = 0,
      },
      { .numberOfRegs = 1, //the standby configuration step
        .delayMs = 0
      }
};

const mipiSpec imx214_4L_4208x3120_RAW10_20Hz_mipiCfg =
{
      .dataMode     = MIPI_D_MODE_0,
      .dataRateMbps = 810,
      .nbOflanes    = 4,
      .pixelFormat  = CSI_RAW_10
};

GenericCamSpec imx214_4L_4208x3120_RAW10_20Hz_camCfg =
{
      .frameWidth            = 4208,
      .frameHeight           = 3120,
      .hBackPorch            = 0,
      .hFrontPorch           = 0,
      .vBackPorch            = 0,
      .vFrontPorch           = 0,
      .bytesPerPixel         = 2,
      .internalPixelFormat   = RAW16,
      .mipiCfg               = &imx214_4L_4208x3120_RAW10_20Hz_mipiCfg,
      .idealRefFreq          = 24,
      .sensorI2CAddress1     = (0x20 >> 1),
      .sensorI2CAddress2     = 0,             //only used for stereo cameras
      .nbOfI2CConfigSteps    = IMX214_4L_4208x3120_RAW10_20HZ_I2C_CONFIG_STEPS,
      .i2cConfigSteps        = imx214_4L_4208x3120_RAW10_20Hz_i2cConfigSteps,
      .regSize               = 1,
      .regValues             = imx214_4L_4208x3120_RAW10_20Hz_I2Cregs,
};

#endif // _IMX214_4L_4208x3120_RAW10_20HZ_H_
