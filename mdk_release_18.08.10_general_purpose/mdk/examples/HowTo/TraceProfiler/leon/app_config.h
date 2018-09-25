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
#define APP_MSS_CLOCKS              (DEV_MSS_APB_SLV   | \
                                    DEV_MSS_APB2_CTRL  | \
                                    DEV_MSS_RTBRIDGE   | \
                                    DEV_MSS_RTAHB_CTRL | \
                                    DEV_MSS_LRT        | \
                                    DEV_MSS_LRT_DSU    | \
                                    DEV_MSS_LRT_L2C    | \
                                    DEV_MSS_LRT_ICB    | \
                                    DEV_MSS_AXI_BRIDGE | \
                                    DEV_MSS_MXI_CTRL   | \
                                    DEV_MSS_MXI_DEFSLV | \
                                    DEV_MSS_AXI_MON    | \
                                    DEV_MSS_TIM        )


#define APP_UPA_CLOCKS  (DEV_UPA_SH0       | \
                         DEV_UPA_SH1       | \
                         DEV_UPA_SH2       | \
                         DEV_UPA_SH3       | \
                         DEV_UPA_SH4       | \
                         DEV_UPA_SH5       | \
                         DEV_UPA_SH6       | \
                         DEV_UPA_SH7       | \
                         DEV_UPA_SH8       | \
                         DEV_UPA_SH9       | \
                         DEV_UPA_SH10      | \
                         DEV_UPA_SH11      | \
                         DEV_UPA_SHAVE_L2  | \
                         DEV_UPA_CDMA      | \
                         DEV_UPA_CTRL       )


// 3: Static Local Data
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int initClocksAndMemory(void);

#endif


