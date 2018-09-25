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

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define SHAVES_USED         (12)
#define PARTITION_0	         (0)

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

//notify other tools of the l2 cache configuration
extern u32 __l2_config ;

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------


/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise  
s32 initClocksAndMemory(void);
void turnOnShavePowerIslands(u32 nbShaves);
void turnOffShavePowerIslands(void);
void powerUpShavesAndDDR(void);

#endif

