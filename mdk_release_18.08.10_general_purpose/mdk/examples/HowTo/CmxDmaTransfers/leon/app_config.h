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

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define OSC_CLK_KHZ       12000 // 12MHz
#define SYS_CLK_KHZ      480000 // 480MHz

#define SHAVES_USED    12

#define APP_MSS_CLOCKS  (DEFAULT_RTEMS_MSS_LRT_CLOCKS | \
                         DEV_MSS_MXI_CTRL)

#define APP_UPA_CLOCKS  (DEV_UPA_SH0      | \
                         DEV_UPA_SH1      | \
                         DEV_UPA_SH2      | \
                         DEV_UPA_SH3      | \
                         DEV_UPA_SH4      | \
                         DEV_UPA_SH5      | \
                         DEV_UPA_SH6      | \
                         DEV_UPA_SH7      | \
                         DEV_UPA_SH8      | \
                         DEV_UPA_SH9      | \
                         DEV_UPA_SH10     | \
                         DEV_UPA_SH11     | \
                         DEV_UPA_SHAVE_L2 | \
                         DEV_UPA_CDMA     | \
                         DEV_UPA_CTRL      )


// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int initClocksAndMemory(void);

#endif // APP_CONFIG_H
