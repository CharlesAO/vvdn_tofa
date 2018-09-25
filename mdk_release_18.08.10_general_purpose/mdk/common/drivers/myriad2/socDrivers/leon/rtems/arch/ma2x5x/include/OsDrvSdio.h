///
/// @file OsDrvSdio.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvSdio OsSdio Driver
/// @{
/// @brief     RTEMS SDIO Header File 
///

#ifndef OSDRVSDIO_H
#define OSDRVSDIO_H 

// Bare-metal layer include                                                                 
#include <OsDrvSdioDefines.h>

#ifdef __cplusplus
extern "C" {
#endif

// 1: Defines
// ----------------------------------------------------------------------------

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

// 4: Export Functions 
// ----------------------------------------------------------------------------

/// Initialize SDIO Driver access 
/// 
/// It is not expected that this function would be needed in normal
/// Operation. 
/// @param[in] info: Describes the SdioEntries
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise 
int OsDrvSdioInit(osDrvSdioEntries_t *info);

#ifdef __cplusplus
}
#endif
	
#endif // OSDRVSDIO_H


///@}
