///   
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt   
///
/// @brief     Key application system settings
/// 
/// 
/// 

// 1: Includes
// ----------------------------------------------------------------------------
#include "los_app_config.h"
#include "DrvCpr.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data 
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void blocksResetSiliconSpecific(void)
{
    DrvCprSysDeviceAction(SIPP_DOMAIN, ASSERT_RESET, DEV_SIPP_MIPI);
    DrvCprSysDeviceAction(UPA_DOMAIN, ASSERT_RESET,  APP_UPA_CLOCKS);

    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, -1);
//if MSS reset, setup clocks needed here

    DrvCprSysDeviceAction(CSS_DOMAIN,  DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(SIPP_DOMAIN, DEASSERT_RESET, -1);
    DrvCprSysDeviceAction(UPA_DOMAIN,  DEASSERT_RESET, -1);
}

