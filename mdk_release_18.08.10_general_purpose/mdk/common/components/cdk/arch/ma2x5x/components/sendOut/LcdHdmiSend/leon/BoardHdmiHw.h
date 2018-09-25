///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup
/// @{
/// @brief
///
///
///


#ifndef _BOARD_HDMI_HW_API_H_
#define _BOARD_HDMI_HW_API_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"

#ifdef __cplusplus
extern "C" {
#endif


// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

// Initalize I2C, GPIOs, HW specific components
int boardHdmiHwInit(void);

// Program HDMI chip
int boardHdmiHwOpen(void);

// Remove any board specific HDMI HW initializations
int boardHdmiHwDeinit(void);

#ifdef __cplusplus
}
#endif


#endif //_BOARD_HDMI_HW_API_H_
