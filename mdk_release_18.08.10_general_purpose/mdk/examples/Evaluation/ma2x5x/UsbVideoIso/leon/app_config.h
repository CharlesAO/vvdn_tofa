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

#ifndef LOS_APP_CONFIG_H
#define LOS_APP_CONFIG_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ 12000 // 12MHz

#define APP_UPA_CLOCKS  (DEV_UPA_SH0      | \
                         DEV_UPA_SH1      | \
                         DEV_UPA_SH2      | \
                         DEV_UPA_SH3      | \
                         DEV_UPA_SH4      | \
                         DEV_UPA_SH5      | \
                         DEV_UPA_SH6      | \
                         DEV_UPA_SHAVE_L2 | \
                         DEV_UPA_CDMA     | \
                         DEV_UPA_CTRL      )

#define MSS_CLOCKS      ( DEV_MSS_APB_SLV     |     \
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
                          DEV_MSS_MIPI        |     \
                          DEV_MSS_CIF0        |     \
                          DEV_MSS_LCD         |     \
                          DEV_MSS_AMC         |     \
                          DEV_MSS_SIPP        |     \
                          DEV_MSS_TIM         )

#define APP_SIPP_CLOCKS ( DEV_SIPP_SIPP_ABPSLV | \
                          DEV_SIPP_APB_SLV     | \
                          DEV_SIPP_MIPI        | \
                          DEV_SIPP_LUT         | \
                          DEV_SIPP_MIPI_RX1    )

//clock defines
#define CLOCKS_MIPICFG (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)

// USB streaming defines
#define MAX_USED_BUF      4

// SuperSpeed settings
#define FRAME_WIDTH_SS    2104
#define FRAME_HEIGHT_SS   1560
#define FRAME_SIZE_SS    (FRAME_WIDTH_SS * FRAME_HEIGHT_SS * 2)

// HighSpeed settings
#define FRAME_WIDTH_HS    640
#define FRAME_HEIGHT_HS   480
#define FRAME_SIZE_HS    (FRAME_WIDTH_HS * FRAME_HEIGHT_HS * 2)

#define DDR_AREA      __attribute__((section(".ddr_direct.bss")))

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int32_t initClocksAndMemory(void);
void blocksResetSiliconSpecific(void);

#endif // LOS_APP_CONFIG_H
