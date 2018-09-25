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

#ifndef LOS_APP_CONFIG_2150_H
#define LOS_APP_CONFIG_2150_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "DrvCprDefines.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
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


// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------


#endif // LOS_APP_CONFIG_2150_H
