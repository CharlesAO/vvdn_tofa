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

#ifndef LOS_APP_CONFIG_H
#define LOS_APP_CONFIG_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "DrvCprDefines.h"


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "los_app_config_2150.h"
#endif

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ 12000

#define APP_UPA_CLOCKS  (DEV_UPA_SH0      | \
                         DEV_UPA_SH1      | \
                         DEV_UPA_SH2      | \
                         DEV_UPA_SH3      | \
                         DEV_UPA_SHAVE_L2 | \
                         DEV_UPA_CDMA     | \
                         DEV_UPA_CTRL      )

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock needed by this application
///
/// @return    0 on success, non-zero otherwise
int initClocksAndMemory(void);
void blocksResetSiliconSpecific(void);



#endif // LOS_APP_CONFIG_H
