///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
#ifndef __OS_IMX214_H__
#define __OS_IMX214_H__
// 1: Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include <rtems/libi2c.h>

#include <mv_types.h>


// 2:  Exported Global Data
// ----------------------------------------------------------------------------
#define IMX214_I2C_ADDR     (0x20 >> 1)


#ifdef __cplusplus
extern "C" {
#endif
extern const rtems_libi2c_drv_t imx214ProtocolDrvTbl;
#ifdef __cplusplus
}
#endif

#endif //__OS_IMX214_H__
