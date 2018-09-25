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
#include <rtems.h>

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
#define SHAVES_USED (12)

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise  
int initClocksAndMemory(void);

/* functions */
rtems_task Measure_task(rtems_task_argument unused);

#endif // APP_CONFIG_H
