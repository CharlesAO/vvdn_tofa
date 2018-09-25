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

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// UPA Clocks required by the Application
#define APP_UPA_CLOCKS              	((1 << UPA_SH6)     |  \
                                      	(1 << UPA_SH7)      |  \
                                       	(1 << UPA_SHAVE_L2) |  \
                                        (1 << UPA_CDMA)     |  \
                                        (1 << UPA_CTRL)     )

#define APP_MSS_CLOCKS                  (1 << MSS_MXI_CTRL)

// Default clock used by the App
#define DEFAULT_APP_CLOCK_KHZ		266000
#define DEFAULT_OSC_CLOCK_KHZ		12000

// 3: Static Local Data
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int initClocksAndMemory(void);

#endif


