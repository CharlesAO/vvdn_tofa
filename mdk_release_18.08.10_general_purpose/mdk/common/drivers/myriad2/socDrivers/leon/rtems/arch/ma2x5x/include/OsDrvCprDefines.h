///
/// @file OsDrvCprDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvCpr
/// @{
/// @brief     RTEMS CPR Header File 
///

#ifndef OSDRVCPRDEFINES_H
#define OSDRVCPRDEFINES_H 

// Common for all OS drivers 
#include <OsCommon.h>     
// Bare-metal layer include                                                                 
#include <DrvCpr.h>

// 1: Defines
// ----------------------------------------------------------------------------
/* Maximum number of tasks to be notified by the CPR driver on clock change */
#define OS_MYR_MAX_DRVCPR_NOTIFICATION_LIST				32

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef void (*osDrvCprNotification)(u32 newSysClock, void *extrainfo);

#endif // OSDRVCPRDEFINES_H
///@}
