///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "OsDrvCprDefines.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define DEFAULT_APP_CLOCK_KHZ       480000
#define DEFAULT_OSC_CLOCK_KHZ       12000

// 3: Data
// ----------------------------------------------------------------------------


// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int i_initClocksAndMemory(void);

#endif

