///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IC Driver for the HDMI ADV7513 chip.
///
#ifndef __OS_OV7750_H__
#define __OS_OV7750_H__
// 1: Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include <rtems/libi2c.h>

#include <mv_types.h>


// 2:  Exported Global Data
// ----------------------------------------------------------------------------



#ifdef __cplusplus
extern "C" {
#endif
extern const rtems_libi2c_drv_t ov7750ProtocolDrvTbl;
extern const rtems_libi2c_drv_t ov7750BroadcastProtocolDrvTbl;
#ifdef __cplusplus
}
#endif

#endif //__OS_ADV7513_H__
