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
#include <mv_types.h>

#define PARTITION_0         0
#define SHAVES_USED         10 //the image is split in ten parts:
// 1 (one shave run)+ 3 (three shaves run) + 6 (six shaves run) = 10

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

//notify other tools of the l2 cache configuration
extern u32 __l2_config ;

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------


/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise  
int initClocksAndMemory(void);

#endif

