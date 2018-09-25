///
/// @file HglCprMa2x5x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglCprMa2x5x
/// @{
/// @brief     HGL source file for CPR, ma2x5x revisions
///


// System Includes
// ----------------------------------------------------------------------------
#include "HglCpr.h"
#include "dbgTracerApi.h"
#include "mvMacros.h"
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include <swcWhoAmI.h>
#include "assert.h"

#define DOMAINS_NUMBER    5
#define CLOCKS_ADDRESSES  3

//GLOBAL DATA
const u32 trickleTicksArray[] = {
// This is likely that it's going to differ significantly for each island:
    0,   // CSS, not applicable
    120, // Retention
    120, // USB
    120, // Shave 0
    120,
    120,
    120,
    120,
    120,
    120,
    120,
    120,
    120,
    120,
    120, // Shave 11
    1200, // PMB
    120, // MSS CPU
    120, // MSS AMC
    120, // MSS SIPP
    120, // DSS
};


//--------------------------------------------------------
// functions not exported as Hgl API
//--------------------------------------------------------
void hglCprGetDomainBaseReset(tyHglCprClockDomain domain, u32* resetBaseRegister, u32* numWords)
{
    switch (domain)
    {
    case CSS_DOMAIN:
        *resetBaseRegister = CPR_BLK_RST0_ADR;
        *numWords  = 2;   // Operate on Reg0,1
        break;
    case UPA_DOMAIN:
        *resetBaseRegister = CMX_RSTN_CTRL;
        break;
    case MSS_DOMAIN:
        *resetBaseRegister = MSS_RSTN_CTRL_ADR;
        break;
    case SIPP_DOMAIN:
        *resetBaseRegister = MSS_SIPP_RSTN_CTRL_ADR;
        break;
    case CSS_AUX_DOMAIN:
        *resetBaseRegister = CPR_AUX_CLK_RST_ADR;
        break;
    default:
        assert(0);
        return;
    }

    return;
}

void hglCprGetDomainBaseClock(tyHglCprClockDomain clkDomain, tyHglCprDeviceAction actionType, u32* clkBaseRegister, u32* numWords)
{
    const u32 domainAddr[CLOCKS_ADDRESSES][DOMAINS_NUMBER] = {
    //        CSS_DOMAIN         , MSS_DOMAIN          , UPA_DOMAIN  ,     SIPP_DOMAIN          , CSS_AUX_DOMAIN
            { CPR_CLK_EN0_SET_ADR, MSS_CPU_CLK_SET_ADR,  CMX_CLK_CTRL_SET, MSS_SIPP_CLK_SET_ADR,  CPR_AUX_CLK_EN_ADR}, //address for atomic setting bits; currently used for ENABLE_CLKS user actions
            { CPR_CLK_EN0_CLR_ADR, MSS_CPU_CLK_CLR_ADR,  CMX_CLK_CTRL_CLR, MSS_SIPP_CLK_CLR_ADR,  CPR_AUX_CLK_EN_ADR}, //address for atomic clearing bits; currently used for DISABLE_CLKS user actions
            { CPR_CLK_EN0_ADR,     MSS_CPU_CLK_CTRL_ADR, CMX_CLK_CTRL,     MSS_SIPP_CLK_CTRL_ADR, CPR_AUX_CLK_EN_ADR}  //address for non atomic operations; currently used for SET_CLKS user actions
    };

    assert(clkDomain <= CSS_AUX_DOMAIN );
    assert(actionType <= SET_CLKS);

    *clkBaseRegister = domainAddr[actionType][clkDomain];

    if ( clkDomain == CSS_DOMAIN )
        *numWords = 2;
    else
        *numWords = 1;

    return;
}

//----------------------------------------------------------
//exported API functions
//----------------------------------------------------------
void HglCprSetRetentionRegisters(u32 setBitValue)
{
    u32 reg0 = GET_REG_WORD_VAL(AON_RETENTION0_ADR);
    reg0 |= setBitValue;
    SET_REG_WORD(AON_RETENTION0_ADR, reg0);
}

s32 HglCprAuxClocksEnable(const tyHglCprAuxClkDividerCfg  *pAuxClkCfg)
{
    hglCprAuxClocksEnable(pAuxClkCfg);

    return CPR_SUCCESS;
}

s32 HglCprSetupSystemClk(int clockDivNumerator, int clockDivDenominator, u32 pllConfigWord, u32 ticksPerUsCounter)
{
    int retVal;

    assert((clockDivNumerator   & (~MASK_MAS_DIV )) == 0);          // Value can't be more than 10 bits
    assert((clockDivDenominator & (~MASK_MAS_DIV )) == 0);          // Value can't be more than 10 bits

    if (clockDivNumerator != clockDivDenominator)                         // Only check ratio if not in bypass
        assert( (clockDivNumerator<<1) <= clockDivDenominator );          // Clock division must be a minimum of 50%

    retVal = hglCprConfigureWaitPll(pllConfigWord, ticksPerUsCounter);
    if (retVal )
        return retVal;

    HglCprSetMasterClockDivider(clockDivNumerator, clockDivDenominator);

    // Crucially we don't re-enable the PLL at this stage.
    // It is up the to the caller to handle that
    return CPR_SUCCESS;
}

void HglCprEnableAllClocks(void)
{
    SET_REG_WORD(CPR_CLK_EN0_ADR,0xFFFFFFFF);
    SET_REG_WORD(CPR_CLK_EN1_ADR,0xFFFFFFFF);

    // Enable PMB subsystem gated clocks and resets
    SET_REG_WORD   (CMX_RSTN_CTRL,0x00000000);               // engage reset
    SET_REG_WORD   (CMX_CLK_CTRL, 0xffffffff);               // turn on clocks
    SET_REG_WORD   (CMX_RSTN_CTRL,0xffffffff);               // dis-engage reset
    // Enable media subsystem gated clocks and resets
    SET_REG_WORD(MSS_CLK_CTRL_ADR,      0xffffffff);
    SET_REG_WORD(MSS_RSTN_CTRL_ADR,     0xffffffff);
    SET_REG_WORD(MSS_SIPP_CLK_CTRL_ADR, 0xffffffff);
}

void HglCprSetDomainClks(tyHglCprClockDomain clkDomain, u64 value)
{
    u32 *deviceWords;
    u32 baseClock;
    u32 numWords = 1; // Most blocks only have one word to control clk/rst but CSS has 2

    deviceWords = (u32*)&value;

    hglCprGetDomainBaseClock(clkDomain, SET_CLKS, &baseClock, &numWords);

    SET_REG_WORD(baseClock, deviceWords[0]);
    if (numWords  == 2)
        SET_REG_WORD(baseClock + 4, deviceWords[1]);

    // Delaying 10 cycles to ensure we wait long enough for the
    // hardcoded 8 sysclk reset active period in the CPR block
    // This is common as it does no harm in the clocking case
    HglDelay(40);

    HGL_CPR_DPRINTF1(" HglCprSetDomainClks[%d](%08X)=> %08X%08X\n",clkDomain, baseClock, (numWords  == 2)?(GET_REG_WORD_VAL(baseClock+4)):0, GET_REG_WORD_VAL(baseClock));

    return;
}

void HglCprChangeClks(tyHglCprClockDomain clkDomain, tyHglCprDeviceAction userAction, u64 devices)
{
    u32  *deviceWords;
    u32  baseClock;
    u32  registerAction = SET_WORD; //For ma2x5x, most blocks have specific addresses for atomic set/clear, so the action is always SET_WORD,
                                    //  only the address change, based on action; the CSS_AUX does not have atomic set/clr, so it will need specific actions based on user request
    u32  numWords = 1;           //Most blocks only have one word to control clk/rst but CSS has 2

    deviceWords = (u32*)&devices;

    hglCprGetDomainBaseClock(clkDomain, userAction, &baseClock, &numWords);

    if (clkDomain == CSS_AUX_DOMAIN)
    {
        if (userAction == ENABLE_CLKS)
            registerAction = SET_BITS;
        else if (userAction == DISABLE_CLKS)
                registerAction = CLR_BITS;
             //else default the registerAction to SET_WORDS
    }

    DrvRegBitMaskOperation((void *)baseClock, registerAction, deviceWords[0]);
    if (numWords  == 2)
        DrvRegBitMaskOperation((void *)baseClock + 4, registerAction, deviceWords[1]);

    // Delaying 10 cycles to ensure we wait long enough for the
    // hardcoded 8 sysclk reset active period in the CPR block
    // This is common as it does no harm in the clocking case
    HglDelay(40);

    HGL_CPR_DPRINTF1(" HglCprChangeClks[%d,%d](%08X) %llX => %08X%08X\n",clkDomain, userAction, baseClock, devices, (numWords  == 2)?(GET_REG_WORD_VAL(baseClock+4)):0, GET_REG_WORD_VAL(baseClock));

    return;
}

s32 HglCprSetupClocks(const tyHglSocClockConfig * pClkCfg)
{
    s32 res = 0;
    tyHglCprPllState __attribute__((unused)) pll0Status;
    tyHglCprPllState pll1Status;

    if (pClkCfg->targetPll0FreqKhz != 0)
    {
        // Set the reference clocks to the requested values
        res = HglConfigurePll(CPR_PLL0, CLK_SRC_REFCLK0, pClkCfg);
        if(res)
            return ERR_UNABLE_TO_ACHIEVE_OSC_CLK;
    }

    if (pClkCfg->targetPll1FreqKhz != 0)
    {
       res = HglConfigurePll(CPR_PLL1, pClkCfg->clkSrcPll1, pClkCfg);
       if(res)
            return ERR_UNABLE_TO_ACHIEVE_OSC_CLK;
    }

    pll0Status = HglCprGetPllStatus(CPR_PLL0); //replaced register read with function call for sync/delay on the APB4 bus

    // TODO: This also needs a software timeout to protect the hardware timeout,
    // just in case there are problems with CPR_NUM_CYCLES, or if there is no clock
    // at the PLL input.
    do
    {
        pll0Status = HglCprGetPllStatus(CPR_PLL0);
        pll1Status = HglCprGetPllStatus(CPR_PLL1);
    } while ((pll0Status == PLL_STATE_PENDING) || ((pll1Status == PLL_STATE_PENDING) && (pClkCfg->targetPll1FreqKhz != 0)) );

    if ((pClkCfg->targetPll0FreqKhz != 0 && pll0Status == PLL_STATE_DONE_HW_TIMEOUT) ||
        (pClkCfg->targetPll1FreqKhz != 0 && pll1Status == PLL_STATE_DONE_HW_TIMEOUT))
    {
        return ERR_PLL_LOCK_TIMEOUT;
    }

   // Set the system clock divider
    assert((pClkCfg->masterClkDivNumerator   & (~MASK_MAS_DIV)) == 0);                      // Value can't be more than 10 bits
    assert((pClkCfg->masterClkDivDenominator & (~MASK_MAS_DIV)) == 0);                      // Value can't be more than 10 bits

    // only set clock divider if we don't have the special value (0,0), which means don't touch the clock divider
    if (pClkCfg->masterClkDivNumerator != 0 || pClkCfg->masterClkDivDenominator != 0)
    {
        if (pClkCfg->masterClkDivNumerator != pClkCfg->masterClkDivDenominator)                 // Only check for bad ratio if not in bypass
            assert((pClkCfg->masterClkDivNumerator << 1 ) <= pClkCfg->masterClkDivDenominator );    // Clock division must be a minimum of 50%

        HglCprSetMasterClockDivider(pClkCfg->masterClkDivNumerator, pClkCfg->masterClkDivDenominator);
    }

    // Enable the relevant system clocks
    //TODO: optimize in order to only reset/enable these additionally clocks if the associated domains really have something enabled !
    HglCprSetDomainClks(CSS_DOMAIN, (pClkCfg->cssDssClockEnableMask | CSS_CLKS_NEEDED_FOR_MSS_REGS | CSS_CLKS_NEEDED_FOR_CMX_REGS | CSS_CLKS_NEEDED_FOR_SIPP_REGS));
    HglCprResetDevices (MSS_DOMAIN, DEASSERT_RESET, MSS_CLKS_NEEDED_FOR_SIPP_REGS);  //TODO2: auto deassert resets for all the domains, before enabling the clocks (this one is inherited from old driver implementation)?
    HglCprSetDomainClks(MSS_DOMAIN, (pClkCfg->mssClockEnableMask | MSS_CLKS_NEEDED_FOR_SIPP_REGS));
    HglCprSetDomainClks(UPA_DOMAIN, pClkCfg->upaClockEnableMask);
    HglCprSetDomainClks(SIPP_DOMAIN, pClkCfg->sippClockEnableMask);

    // Configure all of the specified Auxilary clocks
    HglCprAuxClockArrayConfig(pClkCfg->pAuxClkCfg);

    return CPR_SUCCESS;
}

void HglCprSysDeviceAction(tyHglCprClockDomain clkDomain, tyHglCprDeviceAction action, u64 devices)
{
    HGL_CPR_DPRINTF1("\nHglCprSysDeviceAction(%08X, %08X, %08X%08X)\n", clkDomain, action, (u32)(devices>>32), (u32)devices);

    if (clkDomain == UPA_DOMAIN)
        HglCprChangeClks(CSS_DOMAIN, ENABLE_CLKS, CSS_CLKS_NEEDED_FOR_CMX_REGS);

    if (clkDomain == MSS_DOMAIN)
        HglCprChangeClks(CSS_DOMAIN, ENABLE_CLKS, CSS_CLKS_NEEDED_FOR_MSS_REGS);

    if (clkDomain == SIPP_DOMAIN)
    {
        HglCprChangeClks(CSS_DOMAIN, ENABLE_CLKS, CSS_CLKS_NEEDED_FOR_MSS_REGS |
                                                  CSS_CLKS_NEEDED_FOR_SIPP_REGS );
        HglCprChangeClks(MSS_DOMAIN, ENABLE_CLKS, MSS_CLKS_NEEDED_FOR_SIPP_REGS);
        HglCprResetDevices(MSS_DOMAIN, DEASSERT_RESET, MSS_CLKS_NEEDED_FOR_SIPP_REGS);
    }

    switch (action)
    {
    case SET_CLKS:
        HglCprSetDomainClks((tyHglCprClockDomain)clkDomain, devices);
        break;
    case ENABLE_CLKS:
        HglCprChangeClks((tyHglCprClockDomain)clkDomain, ENABLE_CLKS, devices);
        break;
    case DISABLE_CLKS:
        HglCprChangeClks((tyHglCprClockDomain)clkDomain, DISABLE_CLKS, devices);
        break;
    case ASSERT_RESET:
        HglCprResetDevices((tyHglCprClockDomain)clkDomain, ASSERT_RESET, devices);
        break;
    case DEASSERT_RESET:
        HglCprResetDevices((tyHglCprClockDomain)clkDomain, DEASSERT_RESET, devices);
        break;
    case PULSE_RESET:
        HglCprResetDevices((tyHglCprClockDomain)clkDomain, PULSE_RESET, devices);
        break;
    case ASSERT_RESET_AND_RESTORE:
        HglCprResetDevices((tyHglCprClockDomain)clkDomain, ASSERT_RESET_AND_RESTORE, devices);
        break;
    case UPDATE_RATE_CLKS:  // Only applies to CSS clocks
        // Flag an error if someone attempts to rate change non-css clocks
        assert(clkDomain == CSS_DOMAIN);
        HglCprSetCssRateClks(devices);
        break;
    default:
        assert(0);
        break;
    }
}

#ifdef POWER_AWARE

volatile u32 dbg_shaveclk=0;

void HglCprEnableUpa(u32 id)
{
    if (id!=POWER_AWARE_LRT_CORE_ID)
    {
        HglCprChangeClks(UPA_DOMAIN, ENABLE_CLKS, (u64)id);
        dbgLogEvent(ISLAND_TO_SIGNAL(id + POWER_ISLAND_SHAVE_0), 1, DEBUG_LOG_LEVEL_HIGH);
        HglCprPowerTurnOnIslandsRaw(1 << ((enum PowerIslandIndex)(id + POWER_ISLAND_SHAVE_0)),
                                    trickleTicksArray[id + POWER_ISLAND_SHAVE_0],
                                    TURN_ON_ENABLE_TICKS);
        dbg_shaveclk |= 1<<id;
    }
    else
    {
        HglCprChangeClks(MSS_DOMAIN, ENABLE_CLKS, LRT_BITS);
        dbgLogEvent(ISLAND_TO_SIGNAL(POWER_ISLAND_MSS_CPU), 1, DEBUG_LOG_LEVEL_HIGH);
        HglCprPowerTurnOnIslandsRaw(1 << POWER_ISLAND_MSS_CPU , trickleTicksArray[POWER_ISLAND_MSS_CPU] , TURN_ON_ENABLE_TICKS);
        dbgLogEvent(ISLAND_TO_SIGNAL(POWER_ISLAND_MSS_AMC), 1, DEBUG_LOG_LEVEL_HIGH);
        HglCprPowerTurnOnIslandsRaw(1 << POWER_ISLAND_MSS_AMC , trickleTicksArray[POWER_ISLAND_MSS_AMC] , TURN_ON_ENABLE_TICKS);
        dbgLogEvent(ISLAND_TO_SIGNAL(POWER_ISLAND_MSS_SIPP), 1, DEBUG_LOG_LEVEL_HIGH);
        HglCprPowerTurnOnIslandsRaw(1 << POWER_ISLAND_MSS_SIPP, trickleTicksArray[POWER_ISLAND_MSS_SIPP], TURN_ON_ENABLE_TICKS);
    }

}

void HglCprClearUpa(u32 id)
{
    if (id!=POWER_AWARE_LRT_CORE_ID)
    {
        dbgLogEvent(ISLAND_TO_SIGNAL(id + POWER_ISLAND_SHAVE_0), 0, DEBUG_LOG_LEVEL_HIGH);
        HglCprPowerTurnOffIslandsRaw(1 << ((enum PowerIslandIndex)(id + POWER_ISLAND_SHAVE_0)),
                                     TURN_OFF_ISO_TICKS,
                                     TURN_OFF_DISABLE_TICKS);
        HglCprChangeClks(UPA_DOMAIN, DISABLE_CLKS, (u64)id);
        dbg_shaveclk &= ~(1<<id);
    }
    else
    {

        HglCprChangeClks(MSS_DOMAIN, DISABLE_CLKS, LRT_BITS);
        dbgLogEvent(ISLAND_TO_SIGNAL(POWER_ISLAND_MSS_CPU), 0, DEBUG_LOG_LEVEL_HIGH);
        HglCprPowerTurnOffIslandsRaw(1 << POWER_ISLAND_MSS_CPU, TURN_OFF_ISO_TICKS, TURN_OFF_DISABLE_TICKS);
        dbgLogEvent(ISLAND_TO_SIGNAL(POWER_ISLAND_MSS_AMC), 0, DEBUG_LOG_LEVEL_HIGH);
        HglCprPowerTurnOffIslandsRaw(1 << POWER_ISLAND_MSS_AMC, TURN_OFF_ISO_TICKS, TURN_OFF_DISABLE_TICKS);
        dbgLogEvent(ISLAND_TO_SIGNAL(POWER_ISLAND_MSS_SIPP), 0, DEBUG_LOG_LEVEL_HIGH);
        HglCprPowerTurnOffIslandsRaw(1 << POWER_ISLAND_MSS_SIPP, TURN_OFF_ISO_TICKS, TURN_OFF_DISABLE_TICKS);

    }

}
#endif  //POWER_AWARE

void HglCprPowerTurnOnIslandsRaw(u32 islands_mask, u32 trickle_ticks, u32 enable_ticks)
{
    const u32 AON_RETENTION0_ENABLE_DDR_IO_RETENTION_BIT = 8;
    if (IS_PLATFORM_FPGA)
    {
        return;
    }
    u32 en = GET_REG_WORD_VAL(AON_PWR_ISLAND_EN_ADR);
    u32 trickle = GET_REG_WORD_VAL(AON_PWR_ISLAND_TRICKLE_EN_ADR);
    u32 iso = GET_REG_WORD_VAL(AON_PWR_ISO_EN0_ADR);
    u32 changed_mask = islands_mask & ((~en) | iso | (~trickle));
    if (changed_mask == 0)
    {
        return;
    }
    u32 en0 = GET_REG_WORD_VAL(CPR_CLK_EN0_ADR);
    u32 en1 = GET_REG_WORD_VAL(CPR_CLK_EN1_ADR);
    u32 aux_en = GET_REG_WORD_VAL(CPR_AUX_CLK_EN_ADR);
    u32 new_trickle = trickle | islands_mask;
    u32 new_en = en | islands_mask;
    u32 new_iso = iso & ~islands_mask;
    uint32_t changed_shaves_mask = (changed_mask >> POWER_ISLAND_SHAVE_0) & ((1 << CFG_NUM_SHAVES) - 1);
    uint32_t shave_clocks_to_restore = 0;

    if (changed_shaves_mask)
    {
        if (!(new_en & (1 << POWER_ISLAND_PMB)))
        {
            assert(0); // It is not allowed to turn on the shaves if the PMB is not being turned on, or if it was not on all along.
        }
    }

    // First disable all clocks that go into a power island we are going to turn on.

    u64 css_clocks_to_handle = DEV_CSS_BIST;

    if (changed_mask & (1 << POWER_ISLAND_DSS))
    {
        css_clocks_to_handle |= DEV_CSS_DSS_APB | DEV_CSS_DSS_BUS;
        HglCprChangeClks(CSS_AUX_DOMAIN, DISABLE_CLKS, (1 << CSS_AUX_DDR_CORE) | (1 << CSS_AUX_DDR_REF));
        // Bringing up the DDR island without retention enabled could cause the IOs to be in an undefined state for a short period of time.
        SET_REG_BIT_NUM(AON_RETENTION0_ADR, AON_RETENTION0_ENABLE_DDR_IO_RETENTION_BIT);
    }

    // If we disable the clock to PMB the SHAVE clocks are disabled too, because SHAVE clocks are generated from the DEV_CSS_UPA_MAS
    if (changed_mask & (1 << POWER_ISLAND_PMB))
    {
        css_clocks_to_handle |= DEV_CSS_UPA_MAS;
    }
    else
    {
        if (changed_shaves_mask)
        {
            shave_clocks_to_restore = GET_REG_WORD_VAL(CMX_CLK_CTRL) & changed_shaves_mask;
            HglCprChangeClks(UPA_DOMAIN, DISABLE_CLKS, changed_shaves_mask);
        }
    }

    if (changed_mask & (1 << POWER_ISLAND_MSS_CPU))
    {
        css_clocks_to_handle |= DEV_CSS_MSS_MAS;
        HglCprChangeClks(CSS_AUX_DOMAIN, DISABLE_CLKS, AUX_CLK_MASK_CIF0 | AUX_CLK_MASK_CIF1 | AUX_CLK_MASK_LCD | AUX_CLK_MASK_NAL_PAR | AUX_CLK_MASK_NAL_SYN | AUX_CLK_MASK_MIPI_TX0 | AUX_CLK_MASK_MIPI_TX1 | AUX_CLK_MASK_MIPI_CFG | AUX_CLK_MASK_MIPI_ECFG);
    }

    if (changed_mask & (1 << POWER_ISLAND_USB))
    {
        css_clocks_to_handle |= DEV_CSS_USB | DEV_CSS_USB_APBSLV;
        HglCprChangeClks(CSS_AUX_DOMAIN, DISABLE_CLKS, (1 << CSS_AUX_USB_PHY_REF_ALT_CLK) | (1 << CSS_AUX_USB_CTRL_REF_CLK) | (1 << CSS_AUX_USB_CTRL_SUSPEND_CLK) | (1 << CSS_AUX_USB_PHY_EXTREFCLK));
    }

    HglCprChangeClks(CSS_DOMAIN, DISABLE_CLKS, css_clocks_to_handle);

    // Now we turn on the power island (first trickle switches, and then the main switches)

    SET_REG_WORD(AON_PWR_ISLAND_TRICKLE_EN_ADR, new_trickle);

    HglDelay(trickle_ticks);

    //FIXME - is this even needed any more ?
    en |= islands_mask;
    UNUSED(en);
    SET_REG_WORD(AON_PWR_ISLAND_EN_ADR, new_en);

    if (enable_ticks == 0)
    {
        while (((GET_REG_WORD_VAL(AON_PWR_ISLAND_STATUS) & ALL_ISLANDS_MASK) & islands_mask) == 0);
    }
    else
    {
        HglDelay(enable_ticks);
    }

    // Enable clocks to make sure that any synchronously asserting resets activate when we later assert reset
    HglCprChangeClks(CSS_DOMAIN, ENABLE_CLKS, css_clocks_to_handle);
    // NOTE: no reason to force-re-enable shave or CSS_AUX clocks here, because there are no synchronously asserting resets on those lines

    // Reset everything inside the power island(s) that we are turning on
    HglCprResetDevices(CSS_DOMAIN, PULSE_RESET, DEV_CSS_BIST);


    if (changed_mask & (1 << POWER_ISLAND_MSS_CPU))
    {
        SET_REG_WORD(CPR_LEON_RT_VECTOR_ADR, 0);
        HglCprResetDevices(CSS_DOMAIN, PULSE_RESET, DEV_CSS_MSS_MAS);
    }

    if (changed_mask & (1 << POWER_ISLAND_PMB))
    {
        // If the PMB island is bought up together with any shave islands, then we don't need to reset the shaves separately.
        HglCprResetDevices(CSS_DOMAIN, PULSE_RESET, DEV_CSS_UPA_MAS);
        // There is no point in resetting the shaves, if we are turning on the PMB island in conjunction with other shaves.
        // UPA_MAS should reset all shaves as well. So we only explicitly reset shaves in the else:
    }
    else
    {
        if (changed_shaves_mask)
        {
                u32 old_shave_reset = GET_REG_WORD_VAL(CMX_RSTN_CTRL);
                SET_REG_WORD(CMX_RSTN_CTRL, old_shave_reset & ~changed_shaves_mask);
                SET_REG_WORD(CMX_RSTN_CTRL, old_shave_reset);
        }
    }

    if (changed_mask & (1 << POWER_ISLAND_DSS))
    {
        u64 blk_to_reset =        (DEV_CSS_DSS_APB_RST_CTRL) |
                                  (DEV_CSS_DSS_BUS_MAHB) |
                                  (DEV_CSS_DSS_BUS_DXI) |
                                  (DEV_CSS_DSS_BUS_AAXI) |
                                  (DEV_CSS_DSS_BUS_MXI);
        u32 restore_blk_rst1 = GET_REG_WORD_VAL(CPR_BLK_RST1_ADR);
        HglCprResetDevices(CSS_DOMAIN, PULSE_RESET, blk_to_reset);
        HglCprResetDevices(CSS_DOMAIN, ASSERT_RESET, DEV_CSS_DSS_APB_RST_PHY); // This reset line is a special case that is sticky.
        SET_REG_WORD(CPR_BLK_RST1_ADR, restore_blk_rst1);


        u32 old_aux_reset = GET_REG_WORD_VAL(CPR_AUX_CLK_RST_ADR);
        SET_REG_WORD(CPR_AUX_CLK_RST_ADR, old_aux_reset & ~((1 << CSS_AUX_DDR_CORE_CTRL) | (1 << CSS_AUX_DDR_CORE_CTRL)));
        SET_REG_WORD(CPR_AUX_CLK_RST_ADR, old_aux_reset);
    }

    if (changed_mask & (1 << POWER_ISLAND_USB))
    {
        u64 blk_to_reset =        (DEV_CSS_USB) |
                                  (DEV_CSS_USB_APBSLV);
        HglCprResetDevices(CSS_DOMAIN, PULSE_RESET, blk_to_reset);
    }

    // Restore all clock enables after reset
    // Check first if any of the clocks in the CSS, CSS_AUX and UPA domains were disabled
    u64 css_clocks_that_need_to_be_disabled = (~(((u64)en0) | (((u64)en1) << 32))) & css_clocks_to_handle;
    if (css_clocks_that_need_to_be_disabled)
    {
        HglCprChangeClks(CSS_DOMAIN, DISABLE_CLKS, css_clocks_that_need_to_be_disabled);
    }

    if (changed_mask & (1 << POWER_ISLAND_DSS | 1 << POWER_ISLAND_MSS_CPU | 1 << POWER_ISLAND_USB))
    {
        HglCprChangeClks(CSS_AUX_DOMAIN, ENABLE_CLKS, aux_en);
    }

    if (shave_clocks_to_restore)
    {
        HglCprChangeClks(UPA_DOMAIN, ENABLE_CLKS, shave_clocks_to_restore);
    }

    // Deisolate the newly brought up islands
    SET_REG_WORD(AON_PWR_ISO_EN0_ADR, new_iso);
}

void HglCprPowerTurnOffIslandsRaw(u32 islands_mask, u32 iso_ticks, u32 disable_ticks)
{
    const u32 AON_RETENTION0_ENABLE_DDR_IO_RETENTION_BIT=8;
    if (IS_PLATFORM_FPGA)
    {
        return;
    }
    u32 en = GET_REG_WORD_VAL(AON_PWR_ISLAND_EN_ADR);
    u32 trickle = GET_REG_WORD_VAL(AON_PWR_ISLAND_TRICKLE_EN_ADR);
    u32 iso = GET_REG_WORD_VAL(AON_PWR_ISO_EN0_ADR);
    u32 changed_mask = islands_mask & (en | (~iso) | trickle);
    if (changed_mask == 0)
    {
        return;
    }

    if (changed_mask & (1 << POWER_ISLAND_DSS))
    {
        // Attempting the Disable the DRAM DSS island.
        // Before we can do this it is necessary to put the DDR I/O's into retention mode
        // otherwise there will be an excessive power draw on the DRAM_MVDDA line.
        // This will be disabled by the DDR driver on next init
        SET_REG_BIT_NUM(AON_RETENTION0_ADR,AON_RETENTION0_ENABLE_DDR_IO_RETENTION_BIT);
    }

    u32 new_iso = iso | islands_mask;
    u32 new_en = en & ~islands_mask;
    u32 new_trickle = trickle & ~islands_mask;

    SET_REG_WORD(AON_PWR_ISO_EN0_ADR, new_iso);

    HglDelay(iso_ticks);

    SET_REG_WORD(AON_PWR_ISLAND_TRICKLE_EN_ADR, new_trickle);
    SET_REG_WORD(AON_PWR_ISLAND_EN_ADR, new_en);
    if (disable_ticks == 0)
    {
        while (((GET_REG_WORD_VAL(AON_PWR_ISLAND_STATUS) & ALL_ISLANDS_MASK) & islands_mask) != 0)
        {
            // supposed to be empty
        }
    }
    else
    {
        HglDelay(disable_ticks);
    }
}

u32 HglCprPowerGetStatus()
{
    return (GET_REG_WORD_VAL(AON_PWR_ISLAND_STATUS) & ALL_ISLANDS_MASK);
}

s32 HglCprIsDdrSensitiveToFrequencyChange(void)
{
    tyHwPlatform platform = DrvRegGetHardwarePlatform();
    if ((platform == PLATFORM_MOVISIM) ||
        (platform == PLATFORM_FPGA))
    {
        return 0;
    }

    if (HglCprPowerGetStatus() & (1 << POWER_ISLAND_DSS))
    {
        // Is the DDR power island on?
        if (((u32)HglCprGetResetStatus(CSS_AUX_DOMAIN) & (1 << CSS_AUX_DDR_CORE_CTRL)) != 0)
        {
            // Is the core reset deasserted?
            return 1;
        }
    }
    return 0;
}


///@}
