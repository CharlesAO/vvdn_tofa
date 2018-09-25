///
/// @file DrvCprDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvCpr
/// @{
/// @brief     Definitions and types needed by Clock Power Reset Module
///

#ifndef DRV_CPR_DEF_H
#define DRV_CPR_DEF_H 

#include "mv_types.h"
#include "registersMyriad.h"
#include "HglCprDefines.h"

//#define DRV_CPR_DEBUG_L1
//#define DRV_CPR_DEBUG_L2
                                                                       
// 1: Defines
// ----------------------------------------------------------------------------
#ifdef DRV_CPR_DEBUG_L1
#   define CPR_DPRINTF1(...) printf(__VA_ARGS__)
#else
#   define CPR_DPRINTF1(...)
#endif

#ifdef DRV_CPR_DEBUG_L2
#   define CPR_DPRINTF2(...) printf(__VA_ARGS__)
#else
#   define CPR_DPRINTF2(...)
#endif

// The following macros are used to generate the 32 bit value needed to program either
// the master clock divider of the individual auxillary clock dividers           
// Note: The Master Clock divider has a fixed input source of CLK_SRC_REFCLK0 (Index 0) as it is not permitted to change the master clock source
// while Myriad2 is operational.                                                   
#define GEN_MASTER_CLK_DIVIDER(numerator, denominator)       ( (                              0  << 28)  | ((numerator & MASK_MAS_DIV) << 16) | ((denominator & MASK_MAS_DIV) << 0)  )
                       
// ---  API Error Codes
// (see HGL CPR error codes)             

#define DONT_TOUCH_MASTER_DIVIDER 0

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

//common types used by HglCpr
typedef tyHglCprAuxClkDividerCfg tyAuxClkDividerCfg;
typedef tyHglCprPllBypassOption  tyPllBypassOption;
typedef tyHglCprPllState         tyCprPllState;
typedef tyHglCprPll              tyCprPll;
typedef tyHglCprClockSrc         tyCprClockSrc;
typedef tyHglCprClockDomain      tyCprClockDomain;
typedef tyHglCprDeviceAction     tyCprDeviceAction;
typedef tyHglClockType           tyClockType;
typedef tyHglCprRational         tyCprRational;
typedef tyHglSocClockConfig      tySocClockConfig;

/// This function will be called by the CPR driver when the current 
typedef u32 (*tySysClkFreqChangeCb) (u32 newSysClockFrequency);  

/// @todo Complete this structure
typedef struct
{
    unsigned int nominalFreqKhz;
    unsigned int status; // e.g. clean or not.. TODO!!
} tyClockConfig;

typedef struct 
{
    u32                     initialised;               // Set to True once DrvCprInit called
    u32                     ref0InClkKhz;              // Only updated on calls to DrvCprSetupClocks
    u32                     ref1InClkKhz;              // Only updated on calls to DrvCprSetupClocks
    tySysClkFreqChangeCb    pfSysClockFreqChange;      // Callback used to notify application of new sysClock frequency
} tyCprState;

typedef struct
{
    u32 saved_CPR_PLL_CTRL0;
    u32 saved_CPR_CLK_BYPASS;
    u32 saved_CPR_CLK_DIV;
} tySystemClockSettings;

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

#endif // DRV_CPR_DEF_H


///@}

