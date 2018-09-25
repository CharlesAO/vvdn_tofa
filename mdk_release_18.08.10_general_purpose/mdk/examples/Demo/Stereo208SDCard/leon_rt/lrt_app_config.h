///  
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved 
///            For License Warranty see: common/license.txt   
///
/// @brief     Application configuration Leon header 
/// 
/// 
/// 
/// 
/// 

#ifndef LRT_APP_CONFIG_H
#define LRT_APP_CONFIG_H 

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "common_defines.h"
#include "sipp.h"

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise  
int initClocksAndMemory(void);
void configureLutFilterSiliconSpecific(LutParam* lutCfg);

#endif // LRT_APP_CONFIG_H
