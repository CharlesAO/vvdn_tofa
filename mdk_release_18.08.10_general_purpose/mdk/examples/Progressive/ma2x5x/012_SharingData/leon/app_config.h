
///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

// 1: Includes
// ----------------------------------------------------------------------------

//#include <registersMyriad.h>
//#include <mv_types.h>

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

#define DEFAULT_APP_CLOCK_KHZ       240000
#define DEFAULT_OSC_CLOCK_KHZ       12000

#define SHAVES_USED 2 // Use only 1 shave/dyn module

#define APP_UPA_CLOCKS                 ( DEV_UPA_SH0      |  \
                                         DEV_UPA_SH1      |  \
                                         DEV_UPA_SH2      |  \
                                         DEV_UPA_SH3      |  \
                                         DEV_UPA_SH4      |  \
                                         DEV_UPA_SH5      |  \
                                         DEV_UPA_SH6      |  \
                                         DEV_UPA_SH7      |  \
                                         DEV_UPA_SH8      |  \
                                         DEV_UPA_SH9      |  \
                                         DEV_UPA_SH10     |  \
                                         DEV_UPA_SH11     |  \
                                         DEV_UPA_SHAVE_L2 |  \
                                         DEV_UPA_CDMA     |  \
                                         DEV_UPA_CTRL     |  \
                                         DEV_UPA_MTX      )

#define ALIGNED(x)  __attribute__((aligned(x)))

#define DDR_TEXT    __attribute__((section(".ddr.text")))
#define DDR_DATA    __attribute__((section(".ddr.data")))
#define CMX_DATA    __attribute__((section(".cmx.data")))

#define DDR_DIRECT_DATA   __attribute__((section(".ddr_direct.data")))
#define CMX_DIRECT_DATA   __attribute__((section(".cmx_direct.data")))

#define VALUE_SET_IN_LOS 0x11223344
#define VALUE_SET_IN_LRT 0xAABBCCDD

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
///Initializes clocks and memory configurations (including caches) for the current application
int initClocksAndMemory(int * dataPartitionID);

#ifdef __cplusplus
}
#endif

#endif // _APP_CONFIG_H_
