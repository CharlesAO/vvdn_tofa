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
#include "DrvCpr.h"

// Clock defines
#define OPIPE_CLOCKS (DEV_SIPP_SIGMA          |   \
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
                      DEV_MSS_LCD         |     \
                      DEV_MSS_AMC         |     \
                      DEV_MSS_SIPP        |     \
                      DEV_MSS_TIM)

#define CLOCKS_MIPICFG (AUX_CLK_MASK_MIPI_ECFG | AUX_CLK_MASK_MIPI_CFG)

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

//notify other tools of the l2 cache configuration
extern u32 __l2_config ;

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise  
int initClocksAndMemory(void);

#endif // APP_CONFIG_H
