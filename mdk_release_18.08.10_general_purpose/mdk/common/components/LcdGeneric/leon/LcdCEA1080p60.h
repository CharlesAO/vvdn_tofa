///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Display configuration for a CEA Standard 1080p60 interface
///

#include "LcdApiDefines.h"
#include "DrvLcd.h"
#include "DrvLcdDefines.h"

LCDDisplayCfg lcdSpec1080p60 = {
            .width          = 1920,
            .hPulseWidth    = 44,
            .hBackP         = 148,
            .hFrontP        = 88,
            .height         = 1080,
            .vPulseWidth    = 5,
            .vBackP         = 36,
            .vFrontP        = 4,
            .outputFormat   = D_LCD_OUTF_FORMAT_YCBCR422_8B | D_LCD_OUTF_USE_EXTERNAL_CLK,
            .control        = D_LCD_CTRL_OUTPUT_RGB | D_LCD_CTRL_PROGRESSIVE,
            .pixelClockkHz  = 148500
};

