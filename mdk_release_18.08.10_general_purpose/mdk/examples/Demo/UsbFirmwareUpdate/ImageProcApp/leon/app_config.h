///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

// PLL desired frequency
#define PLL_DESIRED_FREQ_KHZ			480000
// Default start up clock
#define DEFAULT_OSC0_KHZ			12000
#define L2CACHE_NORMAL_MODE 		(0x6)  // In this mode the L2Cache acts as a cache for the DRAM
#define L2CACHE_CFG     		(L2CACHE_NORMAL_MODE)

int i_initClocksAndMemory(void);

#endif // _APP_CONFIG_H_

/* end of file */