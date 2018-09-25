///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _LRT_APP_CONFIG_H_
#define _LRT_APP_CONFIG_H_

#include <mv_types.h>
#include <sipp.h>


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "lrt_app_config_2150.h"
#endif

//notify other tools of the l2 cache configuration
extern u32 __l2_config ;

#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)

/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int initClocksAndMemory(void);
void configureLutFilterSiliconSpecific(LutParam* lutCfg);
#endif //_LRT_APP_CONFIG_H_
