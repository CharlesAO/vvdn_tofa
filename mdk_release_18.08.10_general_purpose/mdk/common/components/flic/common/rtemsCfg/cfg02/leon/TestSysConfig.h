///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _TEST_SYS_CONFIG_H_
#define _TEST_SYS_CONFIG_H_

#define APP_UPA_CLOCKS  (DEV_UPA_SHAVE_L2 | \
                         DEV_UPA_CDMA     | \
                         DEV_UPA_CTRL      )

/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int TestInitClocksAndMemory(void);

#endif


