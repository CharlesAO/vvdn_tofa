///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IC Driver for the External PLL CDCE913PW
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#ifndef CDCEL_CONFIGS_H__
#define CDCEL_CONFIGS_H__

#include <mv_types.h>
#define SELECT_BYTE_ACCESS (1 << 7) // Bit 7 in the register address sets byte mode when enable.

typedef struct
{
    u8 address;
    u8 value;
    u8 checkBAckWrittenVal; // reads back the written value on I2C and checks it
} adv7513ElementType;

// Register map for ADV7513
static adv7513ElementType initAdv513Regs[] =
{
    {0x98, 0x03, 1}, // must be set to 0x3
    {0x9A, 0xE0, 1}, // must be set to 0xE0
    {0x9C, 0x30, 1}, // must be set to 0x30
    {0xA2, 0xA4, 1}, // must be set
    {0xA3, 0xA4, 1}, // must be set
    {0xE0, 0xD0, 1}, // must be set
    {0xF9, 0x00, 0}, // must be set
    {0xD6, 0xF0, 1}, // HPD always high | TMDS soft turn on ena
    {0x9D, 0x01, 1}, // Input Video CLK Divide - 0x1 divide by 2

    {0x15, 0x01, 1}, /// Input 422  YCrCb with Separate Syncs, do not use audio
    {0x16, 0xb9, 1}, /// Output format 422, 20-bit input, style 1
    {0x17, 0x01, 1}, /// hsync vsyn inverted, DE Enabled
    {0x18, 0x46, 1}, /// CSC disabled
    //{0x36, 0x5},
    {0x40, 0x80, 1}, /// General Control packet enable
    {0x41, 0x10, 1}, /// Power down control
    {0x48, 0x00, 1}, /// Data evenly justified
    {0x49, 0xa8, 1}, /// No truncation
    {0x4C, 0x00, 1}, /// 12 bit Output
    {0x55, 0x20, 1}, /// Set YCrCb 422 in AVinfo Frame
    {0x56, 0x08, 1}, /// Set active format Aspect
    {0x96, 0x20, 0}, /// HPD Interrupt clear
    //{0x98, 0x03, 0}, /// ADI Recommended Write
    //{0x99, 0x02, 0}, /// ADI Recommended Write
    //{0x9C, 0x30}, /// PLL Filter R1 Value
    //{0x9D, 0x61}, /// Set clock divide
    //{0x9D, 0x61}, /// Set clock divide
    //{0xA2, 0xA4}, /// ADI Recommended Write
    //{0xA3, 0xA4, 1}, /// ADI Recommended Write
    {0xA5, 0x04, 1}, /// ADI Recommended Write
    {0xAB, 0x40, 1}, /// ADI Recommended Write
    {0xAF, 0x16, 1}, /// Set HDMI Mode
    {0xBA, 0x60, 1}, /// No clock delay
    {0xD1, 0xFF, 1}, /// ADI Recommended Write
    {0xDE, 0xD8, 1}, /// ADI Recommended Write
    {0xE4, 0x60, 1}, /// VCO_Swing_Reference_Voltage
    {0xFA, 0x7c, 1} /// Nbr of times to search for good phase
};

// Reg map extras while using embedded for DE generation, 720p - 60. Check datasheet
static adv7513ElementType extra720Adv513Regs[] =
{
    {0x35, 0x40, 1},
    {0x36, 0xD9, 1},
    {0x37, 0x0A, 1},
    {0x38, 0x00, 1},
    {0x39, 0x2D, 1},
    {0x3A, 0x00, 1},
    {0xD0, 0x36, 1},
    {0xD7, 0x1B, 1},
    {0xD8, 0x82, 1},
    {0xD9, 0x80, 1},
    {0xDA, 0x14, 1},
    {0xDB, 0x05, 1},
    {0xFB, 0x00, 1}
};

// Reg map extras while using embedded for DE generation, 1080p - 60. Check datasheet
static adv7513ElementType extra1080Adv513Regs[] =
{
    {0x35, 0x2F, 0}, // DE Generation Hsync Delay in pxl
    {0x35, 0x2F, 1}, // +
    {0x36, 0xE9, 1}, // DE GEN in HSyncs

    {0x37, 0x0F, 1}, // DE GEN actve width in pxl
    {0x38, 0x00, 1},

    {0x39, 0x43, 1}, // DE GEN actve height in lines
    {0x3A, 0x80, 1},

    {0xD0, 0x36, 1}, // DE generation then sync adjustment | 1X generated clock synchronizes with Hsync | no delay

    {0xD7, 0x1A, 1}, // blanking
    {0xD8, 0x02, 1},
    {0xD9, 0xC0, 1},
    {0xDA, 0x10, 1},
    {0xDB, 0x05, 1},

    {0xFB, 0x00, 1} // DE gen related
};

#endif
