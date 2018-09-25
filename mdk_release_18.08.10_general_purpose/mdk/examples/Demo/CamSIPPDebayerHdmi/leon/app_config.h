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
#define SYS_CLK_KHZ             (12000) // 12MHz
#define PLL_DESIRED_FREQ_KHZ    (580000)

#define MSS_CLOCKS (  DEV_MSS_APB_SLV     |     \
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
                      DEV_MSS_TIM)

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

// Clock defines
#define APP_SIPP_CLOCKS (DEV_SIPP_SIGMA          |   \
                      DEV_SIPP_LSC            |   \
                      DEV_SIPP_RAW            |   \
                      DEV_SIPP_DBYR           |   \
                      DEV_SIPP_DOGL           |   \
                      DEV_SIPP_LUMA           |   \
                      DEV_SIPP_SHARPEN        |   \
                      DEV_SIPP_CGEN           |   \
                      DEV_SIPP_MED            |   \
                      DEV_SIPP_CHROMA         |   \
                      DEV_SIPP_CC             |   \
                      DEV_SIPP_LUT            |   \
                      DEV_SIPP_UPFIRDN0       |   \
                      DEV_SIPP_UPFIRDN1       |   \
                      DEV_SIPP_UPFIRDN2       |   \
                      DEV_SIPP_MIPI_RX0       |   \
                      DEV_MSS_SIPP_CLK_GATES  |   \
                      DEV_SIPP_MIPI           |   \
                      DEV_SIPP_SIPP_ABPSLV    |   \
                      DEV_SIPP_APB_SLV)

#define CLOCKS_MIPICFG (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)

// Clock defines
#define CLOCKS_MIPICFG (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int initClocksAndMemory(void);

#endif // APP_CONFIG_H
