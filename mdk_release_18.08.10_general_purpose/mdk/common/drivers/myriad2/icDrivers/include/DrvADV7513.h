///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IC Driver for the HDMI ADV7513 chip.
///
#ifndef __DRVADV7513_H__
#define __DRVADV7513_H__
// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvI2cMaster.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data
// ----------------------------------------------------------------------------
typedef enum ADV7513ContfigMode
{
    ADV7513_720P30,
    ADV7513_1080P60
}ADV7513ContfigMode_t;

// 3:  Exported Functions
// ----------------------------------------------------------------------------
u8 initADV7513reg(I2CM_Device * dev, ADV7513ContfigMode_t cfg);

#ifdef __cplusplus
}
#endif

#endif //__DRVADV7513_H__
