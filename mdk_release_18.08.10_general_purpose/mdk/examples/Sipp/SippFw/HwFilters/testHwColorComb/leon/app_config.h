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
#define CLOCKS_MIPICFG (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)

#define MSS_CLOCKS         (  DEV_MSS_APB_SLV     |     \
                              DEV_MSS_APB2_CTRL   |     \
                              DEV_MSS_RTBRIDGE    |     \
                              DEV_MSS_RTAHB_CTRL  |     \
                              DEV_MSS_LRT         |     \
                              DEV_MSS_LRT_DSU     |     \
                              DEV_MSS_LRT_L2C     |     \
                              DEV_MSS_LRT_ICB     |     \
                              DEV_MSS_AXI_BRIDGE  |     \
                              DEV_MSS_MXI_CTRL    |     \
                              DEV_MSS_MXI_DEFSLV  |     \
                              DEV_MSS_AXI_MON     |     \
                              DEV_MSS_CIF0        |     \
                              DEV_MSS_CIF1        |     \
                              DEV_MSS_LCD         |     \
                              DEV_MSS_AMC         |     \
                              DEV_MSS_SIPP        |     \
                              DEV_MSS_TIM         )


#define SIPP_CLOCKS         ( DEV_SIPP_SIPP_ABPSLV  | \
                              DEV_SIPP_MIPI         | \
                              DEV_SIPP_MIPI_RX1     )

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


