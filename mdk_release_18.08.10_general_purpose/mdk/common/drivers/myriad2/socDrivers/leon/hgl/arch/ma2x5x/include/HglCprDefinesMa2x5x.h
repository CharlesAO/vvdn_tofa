///
/// @file HglCprDefinesMa2x5x.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglCprMa2x5x
/// @{
/// @brief     Definitions header of HGL for CPR, ma2x5x revisions
///


#ifndef __HGL_CPR_DEFINES_MA2X5X_H__
#define __HGL_CPR_DEFINES_MA2X5X_H__

#include "mv_types.h"
#include "registersMyriad.h"
#include "dbgLogEvents.h"

//1. Defines
#define POWER_AWARE_LRT_CORE_ID    16

#define  ALL_ISLANDS_MASK   ((1 << (POWER_ISLAND_DSS + 1)) - 1)

////////////////////////////////////////////////////////
// MSS_CPU_CLK_CTRL_ADR (Media Subsystem Clocks (0-31)
////////////////////////////////////////////////////////
#define     DEV_MSS_APB_SLV            ( (u32)1   <<   MSS_CP_APB_SLV      )
#define     DEV_MSS_APB2_CTRL          ( (u32)1   <<   MSS_CP_APB2_CTRL    )
#define     DEV_MSS_RTBRIDGE           ( (u32)1   <<   MSS_CP_RTBRIDGE     )
#define     DEV_MSS_RTAHB_CTRL         ( (u32)1   <<   MSS_CP_RTAHB_CTRL   )
#define     DEV_MSS_LRT                ( (u32)1   <<   MSS_CP_LRT          )
#define     DEV_MSS_LRT_DSU            ( (u32)1   <<   MSS_CP_LRT_DSU      )
#define     DEV_MSS_LRT_L2C            ( (u32)1   <<   MSS_CP_LRT_L2C      )
#define     DEV_MSS_LRT_ICB            ( (u32)1   <<   MSS_CP_LRT_ICB      )
#define     DEV_MSS_AXI_BRIDGE         ( (u32)1   <<   MSS_CP_AXI_BRIDGE   )
#define     DEV_MSS_MXI_CTRL           ( (u32)1   <<   MSS_CP_MXI_CTRL     )
#define     DEV_MSS_MXI_DEFSLV         ( (u32)1   <<   MSS_CP_MXI_DEFSLV   )
#define     DEV_MSS_AXI_MON            ( (u32)1   <<   MSS_CP_AXI_MON      )
#define     DEV_MSS_NAL                ( (u32)1   <<   MSS_CP_NAL          )
#define     DEV_MSS_CIF0               ( (u32)1   <<   MSS_CP_CIF0         )
#define     DEV_MSS_CIF1               ( (u32)1   <<   MSS_CP_CIF1         )
#define     DEV_MSS_LCD                ( (u32)1   <<   MSS_CP_LCD          )
#define     DEV_MSS_TIM                ( (u32)1   <<   MSS_CP_TIM          )
#define     DEV_MSS_AMC                ( (u32)1   <<   MSS_CP_AMC          )
#define     DEV_MSS_SIPP               ( (u32)1   <<   MSS_CP_SPMP         )
#define     DEV_MSS_MIPI               DEV_MSS_SIPP
// -----------------------------------------------------------------------------------------
// ---  System Clocks
// -----------------------------------------------------------------------------------------

// Special Encoding to allow user to enable modules without knowing which clock tree the module
// belongs to
// The trick is to use u64 datatype to store both clocks

///////////////////////////////////////
// CPRUnitClockEn0 (0-31)
///////////////////////////////////////
#define     DEV_CSS_LOS                 ( 1ULL    <<    CSS_LOS               )
#define     DEV_CSS_LAHB_CTRL           ( 1ULL    <<    CSS_LAHB_CTRL         )
#define     DEV_CSS_APB4_CTRL           ( 1ULL    <<    CSS_APB4_CTRL         )
#define     DEV_CSS_CPR                 ( 1ULL    <<    CSS_CPR               )
#define     DEV_CSS_ROM                 ( 1ULL    <<    CSS_ROM               )
#define     DEV_CSS_LOS_L2C             ( 1ULL    <<    CSS_LOS_L2C           )
#define     DEV_CSS_MAHB_CTRL           ( 1ULL    <<    CSS_MAHB_CTRL         )
#define     DEV_CSS_LOS_ICB             ( 1ULL    <<    CSS_LOS_ICB           )
#define     DEV_CSS_LOS_DSU             ( 1ULL    <<    CSS_LOS_DSU           )
#define     DEV_CSS_LOS_TIM             ( 1ULL    <<    CSS_LOS_TIM           )
#define     DEV_CSS_GPIO                ( 1ULL    <<    CSS_GPIO              )
#define     DEV_CSS_JTAG                ( 1ULL    <<    CSS_JTAG              )
#define     DEV_CSS_SDIO                ( 1ULL    <<    CSS_SDIO              )
#define     DEV_CSS_ROIC                ( 1ULL    <<    CSS_ROIC              )
#define     DEV_CSS_GETH                ( 1ULL    <<    CSS_GETH              )
#define     DEV_CSS_BIST                ( 1ULL    <<    CSS_BIST              )
#define     DEV_CSS_APB1_CTRL           ( 1ULL    <<    CSS_APB1_CTRL         )
#define     DEV_CSS_AHB_DMA             ( 1ULL    <<    CSS_AHB_DMA           )
#define     DEV_CSS_APB3_CTRL           ( 1ULL    <<    CSS_APB3_CTRL         )
#define     DEV_CSS_I2C0                ( 1ULL    <<    CSS_I2C0              )
#define     DEV_CSS_I2C1                ( 1ULL    <<    CSS_I2C1              )
#define     DEV_CSS_I2C2                ( 1ULL    <<    CSS_I2C2              )
#define     DEV_CSS_UART                ( 1ULL    <<    CSS_UART              )
#define     DEV_CSS_SPI0                ( 1ULL    <<    CSS_SPI0              )
#define     DEV_CSS_SPI1                ( 1ULL    <<    CSS_SPI1              )
#define     DEV_CSS_SPI2                ( 1ULL    <<    CSS_SPI2              )
#define     DEV_CSS_I2S0                ( 1ULL    <<    CSS_I2S0              )
#define     DEV_CSS_I2S1                ( 1ULL    <<    CSS_I2S1              )
#define     DEV_CSS_I2S2                ( 1ULL    <<    CSS_I2S2              )
#define     DEV_CSS_SAHB_CTRL           ( 1ULL    <<    CSS_SAHB_CTRL         )
#define     DEV_CSS_MSS_MAS             ( 1ULL    <<    CSS_MSS_MAS           )
#define     DEV_CSS_UPA_MAS             ( 1ULL    <<    CSS_UPA_MAS           )
///////////////////////////////////////
// CPRUnitClockEn1 (32-64)
///////////////////////////////////////
#define     DEV_CSS_DSS_APB             ( 1ULL    <<    CSS_DSS_APB           )
#define     DEV_CSS_DSS_APB_RST_PHY     ( 1ULL    <<    CSS_DSS_APB_RST_PHY   )  // meant to be the same as above  (reset only bit)
#define     DEV_CSS_DSS_APB_RST_CTRL    ( 1ULL    <<    CSS_DSS_APB_RST_CTRL  )
#define     DEV_CSS_DSS_BUS             ( 1ULL    <<    CSS_DSS_BUS           )
#define     DEV_CSS_DSS_BUS_MAHB        ( 1ULL    <<    CSS_DSS_BUS_MAHB      )  // meant to be the same as above  (reset only bit)
#define     DEV_CSS_DSS_BUS_DXI         ( 1ULL    <<    CSS_DSS_BUS_DXI       )
#define     DEV_CSS_DSS_BUS_AAXI        ( 1ULL    <<    CSS_DSS_BUS_AAXI      )
#define     DEV_CSS_DSS_BUS_MXI         ( 1ULL    <<    CSS_DSS_BUS_MXI       )
#define     DEV_CSS_LAHB2SHB            ( 1ULL    <<    CSS_LAHB2SHB          )
#define     DEV_CSS_SAHB2MAHB           ( 1ULL    <<    CSS_SAHB2MAHB         )
#define     DEV_CSS_USB                 ( 1ULL    <<    CSS_USB               )
#define     DEV_CSS_USB_APBSLV          ( 1ULL    <<    CSS_USB_APBSLV        )
#define     DEV_CSS_AON                 ( 1ULL    <<    CSS_AON               )

/////////////////////////////////////////////////////////////
// MSS_SIPP_CLK_CTRL_ADR (SIPP Subsystem Clocks (0-31) Num:28
////////////////////////////////////////////////////////////

#define DEV_SIPP_SIGMA                    ((u32)1   <<  MSS_SP_SIGMA_HWF )
#define DEV_SIPP_LSC                      ((u32)1   <<  MSS_SP_LSC_HWF )
#define DEV_SIPP_RAW                      ((u32)1   <<  MSS_SP_RAW_HWF )
#define DEV_SIPP_DBYR                     ((u32)1   <<  MSS_SP_DBYR_HWF )
#define DEV_SIPP_DOGL                     ((u32)1   <<  MSS_SP_DOGL_HWF )
#define DEV_SIPP_LUMA                     ((u32)1   <<  MSS_SP_LUMA_HWF )
#define DEV_SIPP_SHARPEN                  ((u32)1   <<  MSS_SP_SHARPEN_HWF )
#define DEV_SIPP_CGEN                     ((u32)1   <<  MSS_SP_CGEN_HWF )
#define DEV_SIPP_MED                      ((u32)1   <<  MSS_SP_MED_HWF )
#define DEV_SIPP_CHROMA                   ((u32)1   <<  MSS_SP_CHROMA_HWF )
#define DEV_SIPP_CC                       ((u32)1   <<  MSS_SP_CC_HWF )
#define DEV_SIPP_LUT                      ((u32)1   <<  MSS_SP_LUT_HWF )
#define DEV_SIPP_EDGE_OP                  ((u32)1   <<  MSS_SP_EDGE_OP_HWF )
#define DEV_SIPP_CONV                     ((u32)1   <<  MSS_SP_CONV_HWF )
#define DEV_SIPP_HARRIS                   ((u32)1   <<  MSS_SP_HARRIS_HWF )
#define DEV_SIPP_UPFIRDN0                 ((u32)1   <<  MSS_SP_UPFIRDN0_HWF )
#define DEV_SIPP_UPFIRDN1                 ((u32)1   <<  MSS_SP_UPFIRDN1_HWF )
#define DEV_SIPP_UPFIRDN2                 ((u32)1   <<  MSS_SP_UPFIRDN2_HWF )
#define DEV_SIPP_MIPI_TX0                 ((u32)1   <<  MSS_SP_MIPI_TX0_HWF )
#define DEV_SIPP_MIPI_TX1                 ((u32)1   <<  MSS_SP_MIPI_TX1_HWF )
#define DEV_SIPP_MIPI_RX0                 ((u32)1   <<  MSS_SP_MIPI_RX0_HWF )
#define DEV_SIPP_MIPI_RX1                 ((u32)1   <<  MSS_SP_MIPI_RX1_HWF )
#define DEV_SIPP_MIPI_RX2                 ((u32)1   <<  MSS_SP_MIPI_RX2_HWF )
#define DEV_SIPP_MIPI_RX3                 ((u32)1   <<  MSS_SP_MIPI_RX3_HWF )
#define DEV_MSS_SIPP_CLK_GATES            ((u32)1   <<  MSS_SIPP_CLK_GATES )
#define DEV_SIPP_MIPI                     ((u32)1   <<  MSS_SP_MIPI )        //
#define DEV_SIPP_SIPP_ABPSLV              ((u32)1   <<  MSS_SP_SIPP_ABPSLV ) //GASTEMP better naming ?
#define DEV_SIPP_APB_SLV                  ((u32)1   <<  MSS_SP_APB_SLV )

// Minimum set of CSS clocks to get to UPA domain.
#define CSS_CLKS_NEEDED_FOR_CMX_REGS         (DEV_CSS_LAHB_CTRL                 | \
                                             DEV_CSS_APB4_CTRL                  | \
                                             DEV_CSS_CPR                        | \
                                             DEV_CSS_MAHB_CTRL                  | \
                                             DEV_CSS_APB1_CTRL                  | \
                                             DEV_CSS_APB3_CTRL                  | \
                                             DEV_CSS_SAHB_CTRL                  | \
                                             DEV_CSS_UPA_MAS                    | \
                                             DEV_CSS_LAHB2SHB                   | \
                                             DEV_CSS_SAHB2MAHB                    )

// Minimum set of CSS clocks to get to MSS domain.
#define CSS_CLKS_NEEDED_FOR_MSS_REGS         (DEV_CSS_LAHB_CTRL                 | \
                                             DEV_CSS_APB4_CTRL                  | \
                                             DEV_CSS_CPR                        | \
                                             DEV_CSS_APB1_CTRL                  | \
                                             DEV_CSS_SAHB_CTRL                  | \
                                             DEV_CSS_MSS_MAS                    | \
                                             DEV_CSS_LAHB2SHB                     )

// Minimum set of CSS clocks to get to SIPP domain, exactly the same as for MSS
#define CSS_CLKS_NEEDED_FOR_SIPP_REGS        CSS_CLKS_NEEDED_FOR_MSS_REGS

// Minimum set of MSS clocks to get to SIPP domain
#define MSS_CLKS_NEEDED_FOR_SIPP_REGS        (DEV_MSS_RTBRIDGE                  | \
                                             DEV_MSS_RTAHB_CTRL                 | \
                                             DEV_MSS_SIPP                         )

// Default clocks groups;
// Best practice is to manually select the appropriate clocks to be enabled for a given application.
// However these definitions are provided as a shortcut to enable access to key system functionality such
// as memories and busses. It assumes the user will enable further clocks as necessary.
#define DEFAULT_MSS_CLOCKS             ( DEV_MSS_APB_SLV     |     \
                                         DEV_MSS_APB2_CTRL   |     \
                                         DEV_MSS_RTBRIDGE    |     \
                                         DEV_MSS_RTAHB_CTRL  |     \
                                         DEV_MSS_LRT         |     \
                                         DEV_MSS_LRT_DSU     |     \
                                         DEV_MSS_LRT_L2C     |     \
                                         DEV_MSS_LRT_ICB     )

#define DEFAULT_CORE_CSS_DSS_CLOCKS    ( DEV_CSS_LOS             |    \
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
                                         DEV_CSS_AHB_DMA         |    \
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
                                         DEV_CSS_SAHB2MAHB       |    \
                                         DEV_CSS_AON                      )


#define MINIMUM_SIPP_CLOCKS            ( DEV_SIPP_SIPP_ABPSLV | \
                                         DEV_SIPP_APB_SLV )


#define RETENTION_REG_ENABLE_SECOND_OSCILLATOR (1 << 18)  // enable bit for the second oscillator


#define ISLAND_TO_SIGNAL(island) ({ \
    int __island_ = (island); \
    ((__island_ == POWER_ISLAND_RETENTION) ? LOG_EVENT_RETENTION : \
          (__island_ == POWER_ISLAND_USB) ? LOG_EVENT_USB_POWER : \
          (__island_ >= POWER_ISLAND_CSS_DIGITAL && __island_ <= POWER_ISLAND_PMB) ? \
          (LOG_EVENT_CSS_DIGITAL_POWER + __island_) : \
          (LOG_EVENT_POWER_M2x5x_BASE + __island_));})
// Types
enum PowerIslandIndex
{
    POWER_ISLAND_CSS_DIGITAL,
    POWER_ISLAND_RETENTION,
    POWER_ISLAND_USB,
    POWER_ISLAND_SHAVE_0,
    POWER_ISLAND_SHAVE_1,
    POWER_ISLAND_SHAVE_2,
    POWER_ISLAND_SHAVE_3,
    POWER_ISLAND_SHAVE_4,
    POWER_ISLAND_SHAVE_5,
    POWER_ISLAND_SHAVE_6,
    POWER_ISLAND_SHAVE_7,
    POWER_ISLAND_SHAVE_8,
    POWER_ISLAND_SHAVE_9,
    POWER_ISLAND_SHAVE_10,    
    POWER_ISLAND_SHAVE_11,   
    POWER_ISLAND_PMB,
    POWER_ISLAND_MSS_CPU,
    POWER_ISLAND_MSS_AMC,
    POWER_ISLAND_MSS_SIPP,
    POWER_ISLAND_DSS        
};

#endif  //__HGL_CPR_DEFINES_MA2X5X_H__

///@}
