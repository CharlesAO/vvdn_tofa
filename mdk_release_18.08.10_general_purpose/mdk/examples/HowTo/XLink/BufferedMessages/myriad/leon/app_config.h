///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#include "fatalExtension.h"
#include "mv_types.h"

// PLL desired frequency
#define PLL_DESIRED_FREQ_KHZ   600000

// Default start up clock
#ifdef MA2X8X
#define DEFAULT_OSC_CLOCK_KHZ   24000
#else
#define DEFAULT_OSC_CLOCK_KHZ   12000
#endif

#define L2CACHE_NORMAL_MODE     (0x6)  // In this mode the L2Cache acts as a cache for the DRAM
#define L2CACHE_CFG         (L2CACHE_NORMAL_MODE)

int initClocksAndMemory(void);

#endif // _APP_CONFIG_H_
