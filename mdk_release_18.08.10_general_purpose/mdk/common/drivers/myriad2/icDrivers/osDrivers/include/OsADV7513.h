///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IC Driver for the HDMI ADV7513 chip.
///
#ifndef __OS_ADV7513_H__
#define __OS_ADV7513_H__
// 1: Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include <rtems/libi2c.h>

#include <mv_types.h>

#define ADV7513_NAME "adv7513"

#define ADV7513_SLAVE_ADDRESS 0x39
// 2:  Exported Global Data
// ----------------------------------------------------------------------------

typedef enum ADV7513ConfigMode_t
{
    ADV7513_CFG_DEFAULT = 0,  // crash and burn
    ADV7513_CFG_720P30 = 1,
    ADV7513_CFG_1080P60 = 2
}ADV7513ConfigMode_t;


#ifdef __cplusplus
extern "C" {
#endif

    extern const rtems_libi2c_drv_t adv7513ProtocolDrvTbl;

#ifdef __cplusplus
}
#endif

#endif //__OS_ADV7513_H__
