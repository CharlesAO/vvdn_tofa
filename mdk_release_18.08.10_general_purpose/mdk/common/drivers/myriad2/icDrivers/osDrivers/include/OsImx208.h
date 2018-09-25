///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
#ifndef __OS_IMX208_H__
#define __OS_IMX208_H__
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
extern const rtems_libi2c_drv_t imx208ProtocolDrvTbl;
#ifdef __cplusplus
}
#endif

#endif //__OS_IMX208_H__
