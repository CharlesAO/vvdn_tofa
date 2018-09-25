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

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

#define SYS_CLK_KHZ 12000 // 12MHz

#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)

//clock defines
#define APP_CSS_DSS_CLOCKS  ( DEV_CSS_LOS       | \
                              DEV_CSS_LAHB_CTRL | \
                              DEV_CSS_APB4_CTRL | \
                              DEV_CSS_CPR       | \
                              DEV_CSS_LOS_L2C   | \
                              DEV_CSS_LOS_ICB   | \
                              DEV_CSS_LOS_DSU   | \
                              DEV_CSS_GPIO      | \
                              DEV_CSS_JTAG      | \
                              DEV_CSS_UART      | \
                              DEV_CSS_MSS_MAS   | \
                              DEV_CSS_UPA_MAS   | \
                              DEV_CSS_AON         )

#define APP_MSS_CLOCKS      ( DEV_MSS_SIPP      | \
                              DEV_MSS_AMC       | \
                              DEV_MSS_LRT       | \
                              DEV_MSS_LRT_DSU   | \
                              DEV_MSS_LRT_L2C   | \
                              DEV_MSS_LRT_ICB     )

#define APP_UPA_CLOCKS        DEV_UPA_SHAVE_L2       | DEV_UPA_CDMA           | DEV_UPA_CTRL


// 3: Static Local Data
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise  
int initClocksAndMemory(void);

#endif

