///
/// @file HglCpr.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup HglCpr HglCpr Driver
/// @{
/// @brief     Header of HGL for CPR
///


#ifndef HGL_CPR_H__
#define HGL_CPR_H__

#include "HglCommon.h"
#include "HglCprDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------
// API functions
//------------------------------

/// Read the numerator/denominator of the master clock divider
///@param [out] numerator value
///@param [out] denominator value
void HglCprGetMasterClockDivider(u32 *numerator, u32 *denominator);

/// Set the numerator/denominator for the master clock divider
///@param [in] numerator value
///@param [in] denominator value
void HglCprSetMasterClockDivider(u32 numerator, u32 denominator);

/// Trigger a JTAG TRST pin flip in debugger
///@remarks TODO implement the paired functionality in debugger
void HglCprResetBist(void) __attribute__ ((noinline));

/// Start the timer block from the LeonRT
///@remarks TODO remove this as not necessary anymore
void HglCprMssTimerActivate(void);

/// Calculate the frequency in khz of a given clock source + divider pair
///@param [in] clkIn - input clock frequency in Khz
///@param [in] numerator of the divider
///@param [in] denominator of the divider
///@return     calculated frequency in khz
u32  HglCprCalcClk(u32 clkIn,u32 numerator,u32 denominator);

/// Calculate the system clock in khz, based on the given PLL frequency
///@param [in] pllClockKhz - the input PLL frequency
///@return     calculated frequency in khz
u32  HglCprCalcSysClkKhz(u32 pllClockKhz);

/// Allows to work out all the parameters for the PLL in the design except refSrc for PLL1
///@param [in] oscIn - input freq (KHz)
///@param [in] sysClkDesired - desired output freq (KHz)
///@param [out] pAchievedFreqKhz - actual freq achieved (KHz)
///@remarks There might an error of up to oscIn Khz in the actual frequency
///@return Returns the word that is to be written in the PLL register
u32  HglCprPllConfValue(u32 oscIn, u32 sysClkDesired, u32 *pAchievedFreqKhz);

/// Setup the PLL and system clock divider with given values
///@param [in] clockDivNumerator - numerator of the divider to be setup for master clock
///@param [in] clockDivDenominator - denominator of the divider to be setup for master clock
///@param [in] pllConfigWord - configuration word to be setup for master clock
///@param [in] ticksPerUsCounter - ticks to wait for PLL stabilization
///@return always CPR_SUCCESS
///        (if errors appears, they assert the reset)
s32  HglCprSetupSystemClk(int clockDivNumerator, int clockDivDenominator, u32 pllConfigWord, u32 ticksPerUsCounter);

/// Configure all of the specified auxiliary clocks
///@param [in] pAuxClkCfg - pointer to an array of configurations for all the auxiliary clocks
///@return CPR_SUCCESS
///        (if errors appears, they assert the reset)
s32  HglCprAuxClockArrayConfig(const tyHglCprAuxClkDividerCfg  pAuxClkCfg[]);

/// Get the configuration of a specified auxiliary clock
///@param [in] u32 auxClock - the auxiliary clock
///@param [out] source - source for the auxiliary clock
///@param [out] numerator - numerator for the aux clock divider
///@param [out] denominator - denominator for the aux clock divider
void HglCprGetAuxClockConfig(u32 auxClock, tyHglCprClockSrc *source, u32 *numerator, u32 *denominator);

/// Check if a specified auxiliary clock is enabled
///@param [in] auxClockId - the auxiliary clock
///@return  1(enabled) or 0 (disabled)
u32  HglCprIsAuxClockEnabled(u32 auxClockId);

/// Bypass PLL or re-enable it
/// @param [in] pll0Bypass: bypass action for PLL0
/// @param [in] pll1Bypass: bypass action for PLL1
/// @return CPR_SUCCESS
int  HglCprPllBypass(tyHglCprPllBypassOption pll0Bypass,tyHglCprPllBypassOption pll1Bypass);

/// Configure the PLL (without wait for stabilization)
/// @param [in] pll: pll id
/// @param [in] pllConfigWord: pll configuration word
/// @param [in] pll0BypassAction: bypass action for PLL0
/// @param [in] pll1BypassAction: bypass action for PLL1
///@return CPR_SUCCESS
///        (if errors appears, they assert the reset)
int  HglCprConfigurePll(tyHglCprPll pll,u32 pllConfigWord, tyHglCprPllBypassOption pll0BypassAction, tyHglCprPllBypassOption pll1BypassAction);

/// Turn on the specified shaves
/// @param [in] shave_mask: bit mask indicating the shaves
void HglCprTurnOnShaveMask(u32 shave_mask);

/// Turn off the specified shaves
/// @param [in] shave_mask: bit mask indicating the shaves
void HglCprTurnOffShaveMask(u32 shave_mask);

/// Set all the clocks of a domain according to the specified mask
/// @param [in] clkDomain: domain indicator
/// @param [in] value: bit mask indicating the blocks to enable the clock for
void HglCprSetDomainClks(tyHglCprClockDomain clkDomain, u64 value);

/// Enable/disable the clocks for the specified blocks of a domain
/// @param [in] clkDomain: domain indicator
/// @param [in] action: enable/disable action
/// @param [in] devices: bit mask indicating the blocks to which the action is applied
void HglCprChangeClks(tyHglCprClockDomain clkDomain, tyHglCprDeviceAction action, u64 devices);

/// Reset action for the specified blocks of a domain
/// @param [in] domain: domain indicator
/// @param [in] resetType: assert/deassert/pulse reset action
/// @param [in] devices: bit mask indicating the blocks to which the action is applied
void HglCprResetDevices(tyHglCprClockDomain domain, tyHglCprDeviceAction resetType, u64 devices);

/// Enable all the myriad2 clocks
void HglCprEnableAllClocks(void);

/// Turn on the specified power islands
/// @param [in] islands_mask: bit mask indicating the islands
/// @param [in] trickle_ticks: ticks to wait after trickle enabled
/// @param [in] enable_ticks:  ticks to wait after islands enabled
void HglCprPowerTurnOnIslandsRaw(u32 islands_mask, u32 trickle_ticks, u32 enable_ticks);

/// Turn off the specified power islands
/// @param [in] islands_mask: bit mask indicating the islands
/// @param [in] iso_ticks: ticks to wait after iso enabled
/// @param [in] enable_ticks:  ticks to wait after islands disabled
void HglCprPowerTurnOffIslandsRaw(u32 islands_mask, u32 iso_ticks, u32 disable_ticks);

/// Obtain the power islands status
///@return status value
u32  HglCprPowerGetStatus(void);

/// Enable one or more auxillary clocks with a specific divider value
/// @param[in] config - structure containing the aux clocks to configure plus the config info
/// @return  CPR_SUCCESS;
s32  HglCprAuxClocksEnable(const tyHglCprAuxClkDividerCfg *config);

/// update the CSS clock divider (1 or 0.5)
/// @param[in] devices - bit mask indicating the blocks to configure
void HglCprSetCssRateClks(u64 devices);

/// Check if the DDR is currently using the indicated PLL as clock source
/// @param[in] pll - PLL id
int  HglCprIsDdrUsingPll(tyHglCprPll pll);

/// Check if the DDR is currently started
s32  HglCprIsDdrSensitiveToFrequencyChange(void);

/// Obtain the reset status of a specified clock domain
///@param [in] domain - clock domain
///@return bitfield status
u64  HglCprGetResetStatus(tyHglCprClockDomain domain);

/// Set the retention register to a specified value
void HglCprSetRetentionRegisters(u32 setBitValue);

/// Check if a specified shave is clocked, or if the LRT is clocked
///@param [in] 0-15 - shave or shave logic; 16 - LRT logic
///@return - status
int  HglCprGetUpaClk(u32 id);

/// Check the PLL status
///@param [in] pll - PLL id
///@return - status
tyHglCprPllState HglCprGetPllStatus(tyHglCprPll pll);

/// Multiply two Rational numbers
///@param [in] a - Input rational number
///@param [in] b - Input rational number
///@return Output rational number
tyHglCprRational HglCprMultiply(tyHglCprRational a, tyHglCprRational b);

/// Convert integer to a rational number, with denominator of 1
///@param [in] in - Input integer
///@return Output rational number
tyHglCprRational HglCprIntegerToRational(tyHglCprInteger in);

/// Convert a rational number to integer, with specified rounding
///@param [in] in - Input rational
///@param [in] rounding - What type of rounding to use
///@return Output integer number
tyHglCprInteger HglCprRationalToInteger(tyHglCprRational in, tyHglCprRounding rounding);

/// Convert a rational number to floating point
///@param [in] in - Input rational
///@return Output floating point number
double HglCprRationalToFloat(tyHglCprRational in);

/// Get PLL multiplier to apply to the input frequency
///@param [in] pll - select pll
///@return Output rational number
tyHglCprRational HglCprGetPllMultiplier(tyHglCprPll pll);

/// Get the multiplier to apply to PLL0 output to calculate the system clock, using the system clock divider.
///@return Output rational number
tyHglCprRational HglCprGetSysMultiplier(void);

/// Get the multiplier of a specified auxiliary clock
///@param [in] u32 auxClock - the auxiliary clock
///@param [out] source - source for the auxiliary clock
///@param [out] multiplier - multiplier for the aux clock divider
void HglCprGetAuxClockMultiplier(u32 auxClock, tyHglCprClockSrc *source, tyHglCprRational *multiplier);

/// Table driven configuration of all device clocks
///
/// This function may be used to configure the entire clock tree for the system
/// In many applications calling DrvCprInit and this function is all that will be needed
/// @param[in] pClkCfg - pointer to clock configuration structure
/// @return    0 on success, non-zero otherwise
///
s32 HglCprSetupClocks(const tyHglSocClockConfig *pClkCfg);

/// Enable, Disable or reset one or more system devices
/// @param[in] domain  - Subgroup of referenced devices CSS_DOMAIN,MSS_DOMAIN or UPA_DOMAIN
/// @param[in] action  - type of action ENABLE_CLKS, DISABLE_CLKS or  RESET_DEVICES
/// @param[in] devices - 64 bit bitfield representing the devices on which the action is applied
/// @return
///
void HglCprSysDeviceAction(tyHglCprClockDomain clkDomain, tyHglCprDeviceAction action, u64 devices);

/// @brief Configure PLL
///
/// @param[in] pll       PLL to be configured (PLL0 or PLL1)
/// @param[in] clkSrcPll type of pll source (CLK_SRC_REFCLK0 for PLL0, CLK_SRC_REFCLK0 or CLK_SRC_REFCLK1 for PLL1)
/// @param[in] pClkCfg   pointer to clock configuration structure
/// @return 
///         - ERR_PLL_CONFIG_INVALID
///         - CPR_SUCCESS
///
s32 HglConfigurePll(tyHglCprPll pll, tyHglCprClockSrc clkSrcPll, const tyHglSocClockConfig *pClkCfg);

#ifdef POWER_AWARE

void HglCprEnableUpa(u32 id);
void HglCprClearUpa(u32 id);

#endif

//-----------------------------------------------------------------------
// NON API functions (used inside hgl, between common <-> arch functions
//-----------------------------------------------------------------------
s32  hglCprConfigureWaitPll (u32 pllConfigWord, u32 ticksPerUsCounter);
void hglCprAuxClocksEnable(const tyHglCprAuxClkDividerCfg  *pAuxClkCfg);
void hglCprGetDomainBaseReset(tyHglCprClockDomain domain, u32* resetBaseRegister, u32* numWords);

#ifdef __cplusplus
}
#endif

#endif

///@}
