///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///
///
///
///
///

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "lrt_app_config_2150.h"
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------


// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int initClocksAndMemory(void);
void configureLutFilterSiliconSpecific(LutParam* lutCfg);

#endif // APP_CONFIG_H
