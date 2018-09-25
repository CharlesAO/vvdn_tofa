#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#include <mv_types.h>

/*=[ system configuration ]==================================================*/

// SHAVEs' L2 configuration

#define L2CACHE_CFG     (SHAVE_L2CACHE_NORMAL_MODE)

// clock enables - media subsystem
#define APP_MSS_CLOCKS \
   (DEV_MSS_APB_SLV    | DEV_MSS_APB2_CTRL  | DEV_MSS_RTBRIDGE   | \
    DEV_MSS_RTAHB_CTRL | DEV_MSS_AXI_BRIDGE | DEV_MSS_MXI_CTRL   | \
    DEV_MSS_MXI_DEFSLV | DEV_MSS_AXI_MON    )
#define APP_MSS_LRT_CLOCKS \
   (DEV_MSS_LRT        | DEV_MSS_LRT_DSU    )

// clock enables - uniform processing aray
#define APP_UPA_CLOCKS \
   (DEV_UPA_SH0  | DEV_UPA_CTRL | DEV_UPA_SHAVE_L2)

// clock enables - auxiliaries
#define APP_AUX_CLOCKS \
   (AUX_CLK_MASK_GPIO  | AUX_CLK_MASK_UART          | AUX_CLK_MASK_DDR_REF      | \
    AUX_CLK_MASK_MEDIA | AUX_CLK_MASK_DDR_CORE_CTRL | AUX_CLK_MASK_DDR_CORE_PHY | \
    AUX_CLK_MASK_CPR   )

/*=[ application API ]=======================================================*/

int initClocksAndMemory();

#endif
