///   
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved. 
///            For License Warranty see: common/license.txt   
///
/// @brief     Key application system settings
/// 
/// 
/// 

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvCpr.h"
#include "DrvDdr.h"
#include "assert.h"
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include "DrvGpioDefines.h"
#include "DrvGpio.h"
#include "registersMyriad.h"
#include "OsDrvShaveL2Cache.h"
#include "OsDrvTimer.h"
#include "DrvMssDefines.h"
#include "OsDrvCpr.h"
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define SYS_CLK_KHZ 12000 // 12MHz

#define L2CACHE_CFG                (SHAVE_L2CACHE_NORMAL_MODE)


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is require here for downstream tools

// 4: Static Local Data 
// ----------------------------------------------------------------------------

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
                              DEV_MSS_AXI_MON     |     \
                              DEV_MSS_TIM         )


#define APP_UPA_CLOCKS  (   DEV_UPA_SH0      | \
                            DEV_UPA_SH1      | \
                            DEV_UPA_SH2      | \
                            DEV_UPA_SH3      | \
                            DEV_UPA_SH4      | \
                            DEV_UPA_SH5      | \
                            DEV_UPA_SH6      | \
                            DEV_UPA_SH7      | \
                            DEV_UPA_SH8      | \
                            DEV_UPA_SH9      | \
                            DEV_UPA_SH10     | \
                            DEV_UPA_SH11     | \
                            DEV_UPA_SHAVE_L2 | \
                            DEV_UPA_CDMA     | \
                            DEV_UPA_CTRL      )


#define CORE_CSS_DSS_CLOCKS   ( DEV_CSS_LOS             |    \
                                DEV_CSS_LAHB_CTRL       |    \
                                DEV_CSS_APB4_CTRL       |    \
                                DEV_CSS_CPR             |    \
                                DEV_CSS_ROM             |    \
                                DEV_CSS_LOS_L2C         |    \
                                DEV_CSS_MAHB_CTRL       |    \
                                DEV_CSS_LOS_ICB         |    \
                                DEV_CSS_LOS_DSU         |    \
                                DEV_CSS_LOS_TIM         |    \
                                DEV_CSS_GPIO            |    \
                                DEV_CSS_JTAG            |    \
                                DEV_CSS_APB1_CTRL       |    \
                                DEV_CSS_APB3_CTRL       |    \
                                DEV_CSS_UART            |    \
                                DEV_CSS_SAHB_CTRL       |    \
                                DEV_CSS_MSS_MAS         |    \
                                DEV_CSS_UPA_MAS         |    \
                                DEV_CSS_DSS_APB         |    \
                                DEV_CSS_DSS_APB_RST_PHY |    \
                                DEV_CSS_DSS_APB_RST_CTRL|    \
                                DEV_CSS_DSS_BUS         |    \
                                DEV_CSS_DSS_BUS_MAHB    |    \
                                DEV_CSS_DSS_BUS_DXI     |    \
                                DEV_CSS_DSS_BUS_AAXI    |    \
                                DEV_CSS_DSS_BUS_MXI     |    \
                                DEV_CSS_LAHB2SHB        |    \
                                DEV_CSS_SAHB2MAHB                )

BSP_SET_CLOCK(12000, 480000, 1, 1,
              CORE_CSS_DSS_CLOCKS, \
            MSS_CLOCKS, \
            APP_UPA_CLOCKS, \
            0x0, \
            0x0);

// Program the L2C at startup
BSP_SET_L2C_CONFIG(0, L2C_REPL_LRU, 0, L2C_MODE_COPY_BACK, 0, NULL);

static tyAuxClkDividerCfg auxClk[] =
{
      {0, 0, 0, 0}, // Null Terminated List
};


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int initClocksAndMemory(void)
{   
    u32 sc;

    // Configure the system
    OsDrvCprInit();
    OsDrvCprOpen();
    OsDrvCprAuxClockArrayConfig(auxClk);

    OsDrvTimerInit();
    DrvDdrInitialise(NULL);

    // Set the shave L2 Cache mode
    sc = OsDrvShaveL2CacheInit(L2CACHE_CFG);
    if(sc)
       return sc;

    //Set Shave L2 cache partitions
    int partition_id;
    OsDrvShaveL2CGetPartition(SHAVEPART256KB, &partition_id);

    //Allocate Shave L2 cache set partitions
    sc = OsDrvShaveL2CacheAllocateSetPartitions();
    if(sc)
        return sc;

    //Assign the one partition defined to all shaves
    for (int i = 0; i < SHAVES_USED; i++)
    {
        OsDrvShaveL2CSetNonWindowedPartition(i, 0, NON_WINDOWED_DATA_PARTITION);
        OsDrvShaveL2CSetNonWindowedPartition(i, 0, NON_WINDOWED_INSTRUCTIONS_PARTITION);
    }

    OsDrvShaveL2CachePartitionInvalidate(0);

    return 0;
}
