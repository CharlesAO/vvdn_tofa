///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API to the Driver for the External PLL CDCE913PW
///
///
///
///
///

#ifndef CDCEL_913_H
#define CDCEL_913_H

#define EXT_PLL_CFG_74MHZ         (1)
#define EXT_PLL_CFG_111MHZ        (2)
#define EXT_PLL_CFG_148MHZ        (3)
#define EXT_PLL_CFG_74_24_24MHZ   (4)  //74.25 Mhz on Y1 (for HDMI usage), 24 Mhz on Y4 + Y5 (for cameras usage)
#define EXT_PLL_CFG_148_24_24MHZ  (5)  //148.5 Mhz on Y1 (for HDMI usage), 24 Mhz on Y4 + Y5 (for cameras usage)
#define EXT_PLL_CFG_74_24_16MHZ   (6)  //74.25 Mhz on Y1 (for HDMI usage), 24 Mhz on Y4 and + 16 Mhz on Y5 (for cameras usage)
#define EXT_PLL_CFG_148_37MHZ     (7)  //148 Mhz on Y1 (for HDMI usage), 37.125 Mhz on Y2

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvI2cMaster.h"

#ifdef __cplusplus
extern "C" {
#endif


// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
int CDCE913Configure(I2CM_Device * dev,u32 config_index);
int CDCE925Configure(I2CM_Device * dev,u32 config_index);

#ifdef __cplusplus
}
#endif

#endif // CDCEL_913_H
