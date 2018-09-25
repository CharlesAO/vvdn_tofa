///
/// @file HglCprDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglCpr
/// @{
/// @brief     Definitions header of HGL for CPR
///


#ifndef HGL_CPR_DEFINES_H__
#define HGL_CPR_DEFINES_H__

#include "mv_types.h"
#include "registersMyriad.h"


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
 #include "HglCprDefinesMa2x5x.h"
#endif


// #define DEBUG_HGL_CPR_L1
// #define DEBUG_HGL_CPR_L2

// 1: Defines
// ----------------------------------------------------------------------------
#ifdef DEBUG_HGL_CPR_L1
  #include <stdio.h>
  #define HGL_CPR_DPRINTF1(...) printf(__VA_ARGS__)
#else
  #define HGL_CPR_DPRINTF1(...)
#endif

#ifdef DEBUG_HGL_CPR_L2
  #include <stdio.h>
  #define HGL_CPR_DPRINTF2(...) printf(__VA_ARGS__)
#else
  #define HGL_CPR_DPRINTF2(...)
#endif

#define HGL_CPR_GET_CPR_VERSION_ID() (GET_REG_WORD_VAL(CPR_VERSION_ID_ADR))

// CPR version IDs
#define HGL_VERSION_ID_MA2150_A0 0x3D41221D
#define HGL_VERSION_ID_MA2150_BX 0x3D45221D

////////////////////////////////////////////////////////
// CMX_CLK_CTRL (UPA (micro processor array) Subsystem Clocks (0-31)
////////////////////////////////////////////////////////
#define     DEV_UPA_SH0                 ( (u32)1   <<   UPA_SH0               )
#define     DEV_UPA_SH1                 ( (u32)1   <<   UPA_SH1               )
#define     DEV_UPA_SH2                 ( (u32)1   <<   UPA_SH2               )
#define     DEV_UPA_SH3                 ( (u32)1   <<   UPA_SH3               )
#define     DEV_UPA_SH4                 ( (u32)1   <<   UPA_SH4               )
#define     DEV_UPA_SH5                 ( (u32)1   <<   UPA_SH5               )
#define     DEV_UPA_SH6                 ( (u32)1   <<   UPA_SH6               )
#define     DEV_UPA_SH7                 ( (u32)1   <<   UPA_SH7               )
#define     DEV_UPA_SH8                 ( (u32)1   <<   UPA_SH8               )
#define     DEV_UPA_SH9                 ( (u32)1   <<   UPA_SH9               )
#define     DEV_UPA_SH10                ( (u32)1   <<   UPA_SH10              )
#define     DEV_UPA_SH11                ( (u32)1   <<   UPA_SH11              )
#define     DEV_UPA_SHAVE_L2            ( (u32)1   <<   UPA_SHAVE_L2          )
#define     DEV_UPA_CDMA                ( (u32)1   <<   UPA_CDMA              )
#define     DEV_UPA_BIC                 ( (u32)1   <<   UPA_BIC               )
#define     DEV_UPA_CTRL                ( (u32)1   <<   UPA_CTRL              )
#define     DEV_UPA_MTX                 ( (u32)1   <<   UPA_MTX               )

// Note: Beware the difference between the bitmasks above which are used for enabling multiple clocks and the enum below which is
// used to request info about a specific clock value. They are obviously not interchangeable!!

// -----------------------------------------------------------------------------------------
// ---  HGL CPR error codes
// -----------------------------------------------------------------------------------------
#define CPR_SUCCESS                                     ( 0)
#define ERR_CPR_NOT_INITIALISED                         (-1)
#define ERR_UNSUPPORTED_OSC_CLK                         (-2)
#define ERR_UNABLE_TO_ACHIEVE_OSC_CLK                   (-3)
#define ERR_PLL_SET_TIMEOUT                             (-4)
#define ERR_PLL_LOCK_TIMEOUT                            (-5)
#define ERR_PLL_LOCK_FAILURE                            (-6)
#define ERR_PLL_CONFIG_INVALID                          (-7)


// Constraints for PGN28HPM18MF3000 using high band
#define HGLCPR_BS1_MINFREFK                 (25000   )
#define HGLCPR_BS1_MAXFREFK                 (50000   )
#define HGLCPR_BS1_MINFVCOK                 (1500000 )
#define HGLCPR_BS1_MAXFVCOK                 (3000000 )
#define HGLCPR_BS1_MINFOUTK                 (187500  )
#define HGLCPR_BS1_MAXFOUTK                 (3000000 )

// Constraints for PGN28HPM18MF3000 using low band
#define HGLCPR_BS0_MINFREFK                 (10000   )
#define HGLCPR_BS0_MAXFREFK                 (50000   )
#define HGLCPR_BS0_MINFVCOK                 (800000  )
#define HGLCPR_BS0_MAXFVCOK                 (1600000 )
#define HGLCPR_BS0_MINFOUTK                 (100000  )
#define HGLCPR_BS0_MAXFOUTK                 (1600000 )

// Standard limits for PGN28HPM18MF3000
#define HGLCPR_MINFEEDBACK                  (16      )
#define HGLCPR_MAXFEEDBACK                  (160     )
#define HGLCPR_MINIDIV                      (1       )
#define HGLCPR_MAXIDIV                      (33      )
#define HGLCPR_MINODIV                      (0       )
#define HGLCPR_MAXODIV                      (4       )

#define PLL_LOCKED_MASK                     (1 << 0)
#define PLL_UPDATE_COMPLETE_MASK            (1 << 1)
#define PLL_LOCK_TIMEOUT                    (1 << 2)
#define PLL_LOCKED_OR_TIMEOUT_MASK          (PLL_LOCKED_MASK | PLL_LOCK_TIMEOUT)

#define MAX_ALLOWED_AUX_CONFIGS             (29) // Max is one clock configuration per Aux clock

#define PLL0_BYPASS_BIT                     (1 << 0)
#define PLL1_BYPASS_BIT                     (1 << 1)

#define PLL_SW_TIMEOUT_US                   (5000)   // Wait ~5ms for PLL to lock


#define LRT_BITS    (DEV_MSS_LRT | \
                    DEV_MSS_LRT_DSU | \
                    DEV_MSS_LRT_L2C | \
                    DEV_MSS_LRT_ICB )

#define MASK_MAS_DIV    (0x3FF) // Master divider only supports 10 bit numerators and denomoniators
#define MASK_AUX_DIV    (0xFFF) // Aux dividers support 12 bit numerators and denomoniators
#define MASK_CLK_SRC    (0x007) // Clock source is a 3 bit field where its used

#define TURN_ON_ENABLE_TICKS 0 // 0 means poll CPR_PWR_STATUS
#define TURN_OFF_ISO_TICKS 10
#define TURN_OFF_DISABLE_TICKS 0 // 0 means poll CPR_PWR_STATUS

#define GEN_AUX_CLK_DIVIDER(source, numerator, denominator)  ( ((source          & MASK_CLK_SRC) << 28)  | ((numerator & MASK_AUX_DIV) << 16) | ((denominator & MASK_AUX_DIV) << 0)  )

// -----------------------------------------------------------------------------------------
// ---  Auxilary Clocks
// -----------------------------------------------------------------------------------------
#define     AUX_CLK_MASK_I2S0                   ( (u32)1   <<  CSS_AUX_I2S0                 )
#define     AUX_CLK_MASK_I2S1                   ( (u32)1   <<  CSS_AUX_I2S1                 )
#define     AUX_CLK_MASK_I2S2                   ( (u32)1   <<  CSS_AUX_I2S2                 )
#define     AUX_CLK_MASK_SDIO                   ( (u32)1   <<  CSS_AUX_SDIO                 )
#define     AUX_CLK_MASK_GPIO                   ( (u32)1   <<  CSS_AUX_GPIO                 ) // this is cpr_io_out_clk_0
#define     AUX_CLK_MASK_32KHZ                  ( (u32)1   <<  CSS_AUX_32KHZ                ) // Optional internal source for RTC
#define     AUX_CLK_MASK_UART                   ( (u32)1   <<  CSS_AUX_UART_SCLK            )
#define     AUX_CLK_MASK_IRDA                   ( (u32)1   <<  CSS_AUX_IRDA                 )
#define     AUX_CLK_MASK_CIF0                   ( (u32)1   <<  CSS_AUX_CIF0                 )
#define     AUX_CLK_MASK_CIF1                   ( (u32)1   <<  CSS_AUX_CIF1                 )
#define     AUX_CLK_MASK_LCD                    ( (u32)1   <<  CSS_AUX_LCD                  )
#define     AUX_CLK_MASK_NAL_PAR                ( (u32)1   <<  CSS_AUX_NAL_PAR              )
#define     AUX_CLK_MASK_NAL_SYN                ( (u32)1   <<  CSS_AUX_NAL_SYN              )
#define     AUX_CLK_MASK_MIPI_TX0               ( (u32)1   <<  CSS_AUX_MIPI_TX0             )
#define     AUX_CLK_MASK_MIPI_TX1               ( (u32)1   <<  CSS_AUX_MIPI_TX1             )
#define     AUX_CLK_MASK_USB_PHY_EXTREFCLK      ( (u32)1   <<  CSS_AUX_USB_PHY_EXTREFCLK    )
#define     AUX_CLK_MASK_USB_CTRL_SUSPEND_CLK   ( (u32)1   <<  CSS_AUX_USB_CTRL_SUSPEND_CLK )
#define     AUX_CLK_MASK_USB_CTRL_REF_CLK       ( (u32)1   <<  CSS_AUX_USB_CTRL_REF_CLK     )
#define     AUX_CLK_MASK_DDR_REF                ( (u32)1   <<  CSS_AUX_DDR_REF              )
#define     AUX_CLK_MASK_ROIC                   ( (u32)1   <<  CSS_AUX_ROIC                 )
#define     AUX_CLK_MASK_MEDIA                  ( (u32)1   <<  CSS_AUX_MEDIA                )
#define     AUX_CLK_MASK_DDR_CORE_CTRL          ( (u32)1   <<  CSS_AUX_DDR_CORE_CTRL        )
#define     AUX_CLK_MASK_DDR_CORE_PHY           ( (u32)1   <<  CSS_AUX_DDR_CORE_PHY         ) // Note: Same clock source, independent bit for reset only
#define     AUX_CLK_MASK_MIPI_ECFG              ( (u32)1   <<  CSS_AUX_MIPI_ECFG            )
#define     AUX_CLK_MASK_MIPI_CFG               ( (u32)1   <<  CSS_AUX_MIPI_CFG             )
#define     AUX_CLK_MASK_USB_PHY_REF_ALT_CLK    ( (u32)1   <<  CSS_AUX_USB_PHY_REF_ALT_CLK  )
#define     AUX_CLK_MASK_GPIO1                  ( (u32)1   <<  CSS_AUX_GPIO1                ) // this is cpr_io_out_clk_1
#define     AUX_CLK_MASK_GPIO2                  ( (u32)1   <<  CSS_AUX_GPIO2                ) // this is cpr_io_out_clk_2
#define     AUX_CLK_MASK_GPIO3                  ( (u32)1   <<  CSS_AUX_GPIO3                ) // this is cpr_io_out_clk_3
#define     AUX_CLK_MASK_CPR                    ( (u32)1   <<  CSS_AUX_CPR                  )

#define AUX_CLK_MASK_ALL_CLKS   (0xFFFFFFFF) // Only provided for debug purposes


// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
// Used by drvCprGetClockFreq to identify the clock being requested
typedef enum
{
    AUX_CLK_I2S0                         =  CSS_AUX_I2S0                  ,
    AUX_CLK_I2S1                         =  CSS_AUX_I2S1                  ,
    AUX_CLK_I2S2                         =  CSS_AUX_I2S2                  ,
    AUX_CLK_SDIO                         =  CSS_AUX_SDIO                  ,
    AUX_CLK_GPIO                         =  CSS_AUX_GPIO                  ,
    AUX_CLK_32KHZ                        =  CSS_AUX_32KHZ                 ,
    AUX_CLK_IRDA                         =  CSS_AUX_IRDA                  ,
    AUX_CLK_CIF0                         =  CSS_AUX_CIF0                  ,
    AUX_CLK_CIF1                         =  CSS_AUX_CIF1                  ,
    AUX_CLK_LCD                          =  CSS_AUX_LCD                   ,
    AUX_CLK_NAL_PAR                      =  CSS_AUX_NAL_PAR               ,
    AUX_CLK_NAL_SYN                      =  CSS_AUX_NAL_SYN               ,
    AUX_CLK_MIPI_TX0                     =  CSS_AUX_MIPI_TX0              ,
    AUX_CLK_MIPI_TX1                     =  CSS_AUX_MIPI_TX1              ,
    AUX_CLK_USB_PHY_EXTREFCLK            =  CSS_AUX_USB_PHY_EXTREFCLK     ,
    AUX_CLK_USB_CTRL_SUSPEND_CLK         =  CSS_AUX_USB_CTRL_SUSPEND_CLK  ,
    AUX_CLK_USB_CTRL_REF_CLK             =  CSS_AUX_USB_CTRL_REF_CLK      ,
    AUX_CLK_DDR_REF                      =  CSS_AUX_DDR_REF               ,
    AUX_CLK_ROIC                         =  CSS_AUX_ROIC                  ,
    AUX_CLK_MEDIA                        =  CSS_AUX_MEDIA                 ,
    AUX_CLK_DDR_CORE_CTRL                =  CSS_AUX_DDR_CORE_CTRL         ,
    AUX_CLK_DDR_CORE_PHY                 =  CSS_AUX_DDR_CORE_PHY          ,
    AUX_CLK_MIPI_ECFG                    =  CSS_AUX_MIPI_ECFG             ,
    AUX_CLK_MIPI_CFG                     =  CSS_AUX_MIPI_CFG              ,
    AUX_CLK_USB_PHY_REF_ALT_CLK          =  CSS_AUX_USB_PHY_REF_ALT_CLK   ,
    AUX_CLK_GPIO1                        =  CSS_AUX_GPIO1                 ,
    AUX_CLK_GPIO2                        =  CSS_AUX_GPIO2                 ,
    AUX_CLK_GPIO3                        =  CSS_AUX_GPIO3                 ,
    AUX_CLK_CPR                          =  CSS_AUX_CPR                   ,
    AUX_CLK_TSENS                        =  CSS_AUX_CPR + 1          , // CSS_AUX_TSENS for ma2x5x
    SYS_CLK                              =  CSS_AUX_CPR + 2          , // System clock post master divider x 1 output
    SYS_CLK_DIV2                         =  CSS_AUX_CPR + 3          , // System clock post master divider x 0.5 output (SAHB)
    PLL0_OUT_CLK                         =  CSS_AUX_CPR + 4          , // Output of primary   PLL
    PLL1_OUT_CLK                         =  CSS_AUX_CPR + 5          , // Output of secondary PLL
    REFCLK0                                                          , // RefClk0/osc1 clock input
    REFCLK1                                                          , // RefClk1/osc2 clock input
    LAST_CLK                                                               // This is just a marker for the end of the enum!
}tyHglClockType;

/// @todo Complete this structure
typedef struct
{
    u32 nominalFreqKhz;
    u32 status; // e.g. clean or not.. TODO!!
} tyHglClockConfig;

// Some blocks (pll1 and aux clock divders have configurable input clocks
// this enum describes the valid input clock sources for these blocks
typedef enum
{
    CLK_SRC_PLL0         = 0,
    CLK_SRC_PLL1         = 1,
    CLK_SRC_REFCLK0      = 2,
    CLK_SRC_REFCLK1      = 3,
    CLK_SRC_32KHZ        = 4,
    CLK_SRC_AUX_CPR      = 5,
    CLK_SRC_SYS_CLK      = 6,    // Output from master clock divider
    CLK_SRC_SYS_CLK_DIV2 = 7,    // 0.5 clock output from master clock divider
} tyHglCprClockSrc;

typedef enum
{
    CSS_DOMAIN,
    MSS_DOMAIN,
    UPA_DOMAIN,
    SIPP_DOMAIN,
    CSS_AUX_DOMAIN
} tyHglCprClockDomain;

typedef enum
{
    ENABLE_CLKS,        // Enable the specified clocks without effecting existing clocks
    DISABLE_CLKS,       // Disable the specified clocks without effecting existing clocks
    SET_CLKS,           // Enable the specified clocks and disable all others
    ASSERT_RESET,       // Apply reset and leave it applied
    DEASSERT_RESET,     // Remove reset
    PULSE_RESET,        // Apply then remove reset
    ASSERT_RESET_AND_RESTORE, // Apply reset and leave it as it was found originally
    UPDATE_RATE_CLKS,   // Select between full and half system clock rate
}tyHglCprDeviceAction;

typedef struct
{
    u32               auxClockEnableMask;     // Bitfield contining a number of Auxillary clocks to be enabled
    tyHglCprClockSrc  auxClockSource;         // Clock source to be used for the aux clockse specified in masl
    u32               auxClockDivNumerator;   // Divider Numerator to be applied to the clocks specified
    u32               auxClockDivDenominator; // Divider Numerator to be applied to the clocks specified
} tyHglCprAuxClkDividerCfg;

typedef enum
{
    CPR_PLL0,
    CPR_PLL1,
} tyHglCprPll;

typedef enum
{
    BYPASS_ENABLE,
    BYPASS_DISABLE,
    BYPASS_NOCHANGE,
} tyHglCprPllBypassOption;

typedef enum
{
    PLL_STATE_DONE_LOCKED,
    PLL_STATE_DONE_HW_TIMEOUT,
    PLL_STATE_DONE_UNKNOWN,
    PLL_STATE_PENDING,
} tyHglCprPllState;

typedef long long tyHglCprInteger;

typedef struct
{
    tyHglCprInteger numerator;
    tyHglCprInteger denominator;
} tyHglCprRational;

typedef enum
{
    HGL_CPR_ROUND_DOWN,
    HGL_CPR_ROUND_UP,
    HGL_CPR_ROUND_HALF_UP,
} tyHglCprRounding;

// TODO: Review this struct..
// 1) Is it too complex
// 2) Do I need to have a pll config override value option
typedef struct
{
    u32                             refClk0InputKhz;
    u32                             refClk1InputKhz;
    u32                             targetPll0FreqKhz;
    u32                             targetPll1FreqKhz;
    tyHglCprClockSrc                clkSrcPll1;
    u32                             masterClkDivNumerator;
    u32                             masterClkDivDenominator;
    u64                             cssDssClockEnableMask;
    u32                             upaClockEnableMask;
    u32                             mssClockEnableMask;
    u32                             sippClockEnableMask;
    const tyHglCprAuxClkDividerCfg  *pAuxClkCfg;          // Null Terminated array of aux clock configs
} tyHglSocClockConfig;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////  Compound Clock definitions used in testing
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This Macro is provided as a convenient shortcut to enable the primary UPA clocks by default
// This includes all shaves, cmxdam, Shave L2cache and the UPA control interface
// Warning: This macro will probably be deprecated in a future revision of the MDK
// Production code should enable clocks on an as-needed basis
#define DEFAULT_UPA_CLOCKS              (DEV_UPA_SH0      | \
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
                                         DEV_UPA_CTRL     )

#endif  //HGL_CPR_DEFINES_H__

///@}

