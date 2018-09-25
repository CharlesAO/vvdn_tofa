///
/// @file HglCpr.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglCpr
/// @{
/// @brief     HGL common source file for CPR, all chips revisions
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "HglCpr.h"

#include "mvMacros.h"
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include "swcWhoAmI.h"
#include "assert.h"


// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 trickleTicksArray[];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation



void HglCprGetMasterClockDivider(u32 *numerator, u32 *denominator)
{
    u32 clockDivider;

    clockDivider = GET_REG_WORD_VAL(CPR_CLK_DIV_ADR);
    *denominator = clockDivider & MASK_MAS_DIV;
    *numerator   = (clockDivider >> 16) & MASK_MAS_DIV;
}

void HglCprSetMasterClockDivider(u32 numerator, u32 denominator)
{
    SET_REG_WORD(CPR_CLK_DIV_ADR, (numerator<<16) | denominator);
}

void HglCprGetAuxClockConfig(u32 auxClock, tyHglCprClockSrc *source, u32 *numerator, u32 *denominator)
{
    u32 clockDivider;

    clockDivider = GET_REG_WORD_VAL(CPR_AUX0_CLK_CTRL_ADR + (auxClock << 2));

    *source      = (tyHglCprClockSrc) ((clockDivider >> 28) & MASK_CLK_SRC);
    *numerator   =                    (clockDivider >> 16) & MASK_AUX_DIV;
    *denominator =                    (clockDivider >> 0 ) & MASK_AUX_DIV;
}

u32 HglCprIsAuxClockEnabled(u32 auxClockId)
{
    u32 clksEnabled;

    clksEnabled = GET_REG_WORD_VAL(CPR_AUX_CLK_EN_ADR);

    return ( (clksEnabled  >> auxClockId) & 1);
}

void HglCprResetDevices(tyHglCprClockDomain domain, tyHglCprDeviceAction resetType, u64 devices)
{
    u32  * deviceWords;
    u32  baseReset;
    u32 i, numWords = 1; // Most blocks only have one word to control clk/rst but CSS has 2

    deviceWords = (u32*)&devices;

    hglCprGetDomainBaseReset(domain, &baseReset, &numWords);

    for (i=0;i<numWords;i++)
    {
        switch (resetType)
        {
        case ASSERT_RESET:
            DrvRegBitMaskOperation((void*)baseReset, CLR_BITS, deviceWords[i]);
            break;
        case DEASSERT_RESET:
            DrvRegBitMaskOperation((void*)baseReset, SET_BITS, deviceWords[i]);
            break;
        case PULSE_RESET:
            DrvRegBitMaskOperation((void*)baseReset, CLR_BITS, deviceWords[i]);
            DrvRegBitMaskOperation((void*)baseReset, SET_BITS, deviceWords[i]);
            break;
        case ASSERT_RESET_AND_RESTORE:
            {
                u32 previous = GET_REG_WORD_VAL(baseReset);
                DrvRegBitMaskOperation((void*)baseReset, CLR_BITS, deviceWords[i]);
                SET_REG_WORD(baseReset, previous);
            }
            break;
        default:
            assert(0);
            break;
        }
        baseReset += 4;
    }

    // Delaying 10 cycles to ensure we wait long enough for the
    // hardcoded 8 sysclk reset active period in the CPR block
    // This is common as it does no harm in the clocking case
    HglDelay(40);

    HGL_CPR_DPRINTF1(" HglCprResetDevices(%08X)=> %08X%08X\n",baseReset-numWords*4, (numWords == 2)?(GET_REG_WORD_VAL(baseReset-4)):0, GET_REG_WORD_VAL(baseReset-numWords*4));

    return;
}

void HglCprSetCssRateClks(u64 devices)
{
    u32  * deviceWords;
    void * baseRate;

    deviceWords = (u32 *) &devices;
    baseRate    = (void *)CPR_CLK_RATE0_ADR;

    DrvRegBitMaskOperation(baseRate    , SET_WORD, deviceWords[0]);
    DrvRegBitMaskOperation(baseRate + 4, SET_WORD, deviceWords[1]);

    HGL_CPR_DPRINTF1(" HglCprSetCssRateClks(%08X)=> %08X%08X\n",baseRate, GET_REG_WORD_VAL(baseRate+4), GET_REG_WORD_VAL(baseRate));

    return;
}

u32 HglCprPllConfValue(u32 oscIn, u32 sysClkDesired, u32 * pAchievedFreqKhz)
{
    u32 pllCtrlVal, freq;
    u32 fRef;
    u32 fVco;
    u32 diff, diffBk;

    u32 inDiv, feedbackDiv, outDiv;
    u32 feedbackEstimate;
    u32 inDivBk = 0, feedbackDivBk = 0, outDivBk = 0, bandBk = 0;

    diffBk = 1000000; // 1 GHz difference

    for (inDiv = HGLCPR_MINIDIV ; inDiv < HGLCPR_MAXIDIV ; ++inDiv)
    {
        fRef = oscIn / inDiv ;
        // Check low band constraint
        if ((fRef >= HGLCPR_BS0_MINFREFK) && (fRef <= HGLCPR_BS0_MAXFREFK))
        {
            for (outDiv = HGLCPR_MINODIV ; outDiv < HGLCPR_MAXODIV ; ++outDiv)
            {
                fVco = sysClkDesired  * (1<<outDiv);
                if ((fVco <= HGLCPR_BS0_MAXFVCOK) && (fVco >= HGLCPR_BS0_MINFVCOK))
                {
                    // The following calcuation attempts to find a feedback value which is close to optimum.
                    // However the integer division is imprecise and is rounded down
                    // Furthermore the LSB is knocked off as feedback values must be even
                    feedbackEstimate = ((sysClkDesired * (1 << outDiv) * inDiv) / oscIn) & 0xFFFFFFFE;
                    // As we rounded down, we should also try the next higher feedback value as it might be closer to the target
                    for (feedbackDiv = feedbackEstimate; feedbackDiv <= feedbackEstimate + 2; feedbackDiv += 2)
                    {
                        if ((feedbackDiv >= HGLCPR_MINFEEDBACK) && (feedbackDiv <= HGLCPR_MAXFEEDBACK))
                        {
                            freq = oscIn * feedbackDiv / ((1<<outDiv) * inDiv);

                            if (sysClkDesired > freq)
                                diff = sysClkDesired - freq;
                            else
                                diff = freq - sysClkDesired;
                            // This will make sure that if there is the same result for bands 0 and 1, band 0 settings will be used as it offers best jitter.
                            if (diff <= diffBk)
                            {
                                // If and only if we have a candidate we must repeat the FVco checks
                                // as previously this was only sanity checked based on estimated value rather than achieved.
                                fVco = freq  * (1<<outDiv);
                                HGL_CPR_DPRINTF2("\nPLL_Calc: Try bs:0 fb:%d iDiv:%d outDiv:%d fDes:%d fVco:%d diff:%d\n",feedbackDiv,inDiv,outDiv,sysClkDesired,fVco,diff);
                                if ((fVco <= HGLCPR_BS0_MAXFVCOK) && (fVco >= HGLCPR_BS0_MINFVCOK))
                                {
                                    inDivBk = inDiv;
                                    feedbackDivBk = feedbackDiv;
                                    outDivBk = outDiv;
                                    bandBk = 0;
                                    diffBk = diff;
                                }
                            }
                        }
                    }
                }
            }
        }
        // Check high band constraint
        if ((fRef >= HGLCPR_BS1_MINFREFK) && (fRef <= HGLCPR_BS1_MAXFREFK))
        {
            for (outDiv = HGLCPR_MINODIV ; outDiv < HGLCPR_MAXODIV ; ++outDiv)
            {
                fVco = sysClkDesired  * (1<<outDiv);
                if ((fVco <= HGLCPR_BS1_MAXFVCOK) && (fVco >= HGLCPR_BS1_MINFVCOK))
                {
                    // The following calcuation attempts to find a feedback value which is close to optimum.
                    // However the integer division is imprecise and is rounded down
                    // Furthermore the LSB is knocked off as feedback values must be even
                    feedbackEstimate = ((sysClkDesired * (1 << outDiv) * inDiv) / oscIn) & 0xFFFFFFFE;
                    // As we rounded down, we should also try the next higher feedback value as it might be closer to the target
                    for (feedbackDiv = feedbackEstimate; feedbackDiv <= feedbackEstimate + 2; feedbackDiv += 2)
                    {
                        if ((feedbackDiv >= HGLCPR_MINFEEDBACK) && (feedbackDiv <= HGLCPR_MAXFEEDBACK))
                        {
                            freq = oscIn * feedbackDiv / ((1<<outDiv) * inDiv);

                            if (sysClkDesired > freq)
                                diff = sysClkDesired - freq;
                            else
                                diff = freq - sysClkDesired;

                            if (diff < diffBk)
                            {
                                // If and only if we have a candidate we must repeat the FVco checks
                                // as previously this was only sanity checked based on estimated value rather than achieved.
                                fVco = freq  * (1<<outDiv);
                                HGL_CPR_DPRINTF2("\nPLL_Calc: Try bs:1 fb:%d iDiv:%d outDiv:%d fDes:$%d fVco:%d diff:%d\n",feedbackDiv,inDiv,outDiv,sysClkDesired,fVco,diff);
                                if ((fVco <= HGLCPR_BS1_MAXFVCOK) && (fVco >= HGLCPR_BS1_MINFVCOK))
                                {
                                    inDivBk = inDiv;
                                    feedbackDivBk = feedbackDiv;
                                    outDivBk = outDiv;
                                    bandBk = 1;
                                    diffBk = diff;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    u32 maxDeviation = oscIn + 1;
    if (diffBk > maxDeviation)
    {
        if (pAchievedFreqKhz != NULL)
        {
            *pAchievedFreqKhz = 0;
        }
        return 0; // don't set, difference too big
    }

    pllCtrlVal = ( (inDivBk - 1) << 6)       |
                   ( ((feedbackDivBk >> 1) - 1) << 11) |
                   ( outDivBk << 4 )         |
                   ( bandBk << 2);

    freq = oscIn * feedbackDivBk / ((1<<outDivBk) * inDivBk);

    if (pAchievedFreqKhz != NULL)
    {
        *pAchievedFreqKhz = freq;
    }
    return pllCtrlVal;
}

int HglCprConfigurePll(tyHglCprPll pll,u32 pllConfigWord, tyHglCprPllBypassOption pll0BypassAction, tyHglCprPllBypassOption pll1BypassAction)
{
    u32 pllControlAddress;
    u32 currentPllConfigWord;

    if (pll == CPR_PLL0)
        pllControlAddress = CPR_PLL_CTRL0_ADR;
    else
        pllControlAddress = CPR_PLL_CTRL1_ADR;

    currentPllConfigWord = GET_REG_WORD_VAL(pllControlAddress);
    // mask out register bits that are not used by hardware
    if (pll == CPR_PLL1)
        currentPllConfigWord &= 0x7ffff;
    else
        currentPllConfigWord &= 0x3ffff; // no source select

    if (currentPllConfigWord != (pllConfigWord & 0x7ffff))
    {
        if (HglCprIsDdrSensitiveToFrequencyChange() && HglCprIsDdrUsingPll(pll))
        {
            HGL_CPR_DPRINTF1("\nCPR: Error: DDR must be disabled before changing its input frequency!\n");
            assert(0);
            return ERR_PLL_CONFIG_INVALID;
        }
        else
        {
            // We must bypass the PLL before reconfiguring it
            HglCprPllBypass(pll0BypassAction,pll1BypassAction);

            // Set the new PLL configuration
            SET_REG_WORD(pllControlAddress, pllConfigWord);
        }
    }

    // Crucially this function does not wait for the PLL to lock
    // or take the PLL out of bypass
    // It is up the to caller to handle that
    return CPR_SUCCESS;
}

inline int HglCprIsDdrUsingPll(tyHglCprPll pll)
{
    tyHglCprClockSrc ddrAuxClockSource;
    u32 dummy;

    if (!(GET_REG_WORD_VAL(CPR_AUX_CLK_EN_ADR) & (1 << CSS_AUX_DDR_CORE)))
    {
        return 0;
    }

    HglCprGetAuxClockConfig(CSS_AUX_DDR_CORE, &ddrAuxClockSource, &dummy, &dummy);
    if ((pll == CPR_PLL0) && (ddrAuxClockSource == CLK_SRC_PLL0))
    {
        return 1;
    }

    if ((pll == CPR_PLL1) && (ddrAuxClockSource == CLK_SRC_PLL1))
    {
        return 1;
    }
    return 0;
}

/// @todo remove this one, as the timer functionalities are no more needed by the CPR driver
void HglCprMssTimerActivate(void)  
{
    u32 mssClockEnables;

    // If DrvCprInit() is called from LeonRT, then we assume the special case
    // where only the LeonRT is running
    // In this scenario it is important that the LeonRT Timer block clock is enabled
    if (swcWhoAmI() == PROCESS_LEON_RT)
    {   // Note: This does not need mutex protection as we have already concluded that the
        // LRT is the only running core. Applications with both LOS/LRT should NOT call DrvCprInit() from LRT
        mssClockEnables = GET_REG_WORD_VAL(MSS_CLK_CTRL_ADR);
        if ((mssClockEnables & DEV_MSS_TIM) == 0)
        {
            SET_REG_WORD(MSS_CLK_CTRL_ADR,mssClockEnables | DEV_MSS_TIM);
            u32 mssReset = GET_REG_WORD_VAL(MSS_RSTN_CTRL_ADR);
            SET_REG_WORD(MSS_RSTN_CTRL_ADR, mssReset | DEV_MSS_TIM);
        }
    }
}


int HglCprPllBypass(tyHglCprPllBypassOption pll0Bypass, tyHglCprPllBypassOption pll1Bypass)
{
    u32 bypassRegisterValue;

    bypassRegisterValue = GET_REG_WORD_VAL(CPR_CLK_BYPASS_ADR);

    if (pll0Bypass == BYPASS_ENABLE)
        SET_REG_BITS_MASK(&bypassRegisterValue,PLL0_BYPASS_BIT);

    if (pll0Bypass == BYPASS_DISABLE)
        CLR_REG_BITS_MASK(&bypassRegisterValue,PLL0_BYPASS_BIT);

    if (pll1Bypass == BYPASS_ENABLE)
        SET_REG_BITS_MASK(&bypassRegisterValue,PLL1_BYPASS_BIT);

    if (pll1Bypass == BYPASS_DISABLE)
        CLR_REG_BITS_MASK(&bypassRegisterValue,PLL1_BYPASS_BIT);

    // Perform all the bypass changes in one single register write
    HGL_CPR_DPRINTF1("\nWRITE CLK_BYPASS Reg : 0x%01X\n",bypassRegisterValue);
    SET_REG_WORD(CPR_CLK_BYPASS_ADR, bypassRegisterValue);

    return CPR_SUCCESS;
}


s32 HglCprAuxClockArrayConfig(const tyHglCprAuxClkDividerCfg  pAuxClkCfg[])
{
    int i;

    assert(pAuxClkCfg != 0);

    // Limit the loop in case someone forgets to terminate the list
    for (i = 0 ; i < MAX_ALLOWED_AUX_CONFIGS; i++)
    {
        if (pAuxClkCfg[i].auxClockEnableMask == 0) // Null Entry terminated list
            break;

        HGL_CPR_DPRINTF1("\nAux Clock Config          : %d    ",i);
        HGL_CPR_DPRINTF1("\n   Aux Clock Enable Mask  : %08X  ",pAuxClkCfg[i].auxClockEnableMask);
        HGL_CPR_DPRINTF1("\n   Aux Clock Source       : %08X  ",pAuxClkCfg[i].auxClockSource);
        HGL_CPR_DPRINTF1("\n   Aux Clock Div Num      : %08X  ",pAuxClkCfg[i].auxClockDivNumerator);
        HGL_CPR_DPRINTF1("\n   Aux Clock Div Den      : %08X  ",pAuxClkCfg[i].auxClockDivDenominator);

        (void)HglCprAuxClocksEnable(&pAuxClkCfg[i]);
    }

    return CPR_SUCCESS;
}

tyHglCprRational HglCprMultiply(tyHglCprRational a, tyHglCprRational b)
{
    tyHglCprRational result;
    result.numerator = a.numerator * b.numerator;
    result.denominator = a.denominator * b.denominator;
    return result;
}

tyHglCprRational HglCprIntegerToRational(tyHglCprInteger in)
{
    tyHglCprRational result;
    result.numerator = in;
    result.denominator = 1;
    return result;
}

tyHglCprInteger HglCprRationalToInteger(tyHglCprRational in, tyHglCprRounding rounding)
{
    if (rounding == HGL_CPR_ROUND_UP)
    {
        in.numerator += in.denominator - 1;
    } else if (rounding == HGL_CPR_ROUND_HALF_UP)
    {
        in.numerator += in.denominator / 2;
    }
    return in.numerator / in.denominator;
}

double HglCprRationalToFloat(tyHglCprRational in)
{
    return ((double)in.numerator) / in.denominator;
}

#define PLL_CTRL_PLL_FDC_FIELD_MSB 17
#define PLL_CTRL_PLL_FDC_FIELD_LSB 11
#define PLL_CTRL_PLL_FDC_FIELD_LENGTH (PLL_CTRL_PLL_FDC_FIELD_MSB - PLL_CTRL_PLL_FDC_FIELD_LSB + 1)

#define PLL_CTRL_PLL_IDC_FIELD_MSB 10
#define PLL_CTRL_PLL_IDC_FIELD_LSB 6
#define PLL_CTRL_PLL_IDC_FIELD_LENGTH (PLL_CTRL_PLL_IDC_FIELD_MSB - PLL_CTRL_PLL_IDC_FIELD_LSB + 1)

#define PLL_CTRL_PLL_ODC_FIELD_MSB 5
#define PLL_CTRL_PLL_ODC_FIELD_LSB 4
#define PLL_CTRL_PLL_ODC_FIELD_LENGTH (PLL_CTRL_PLL_ODC_FIELD_MSB - PLL_CTRL_PLL_ODC_FIELD_LSB + 1)

tyHglCprRational HglCprGetPllMultiplier(tyHglCprPll pll)
{
    u32 pll_ctrl_adr = CPR_PLL_CTRL0_ADR;
    u32 pll_stat_adr = CPR_PLL_STAT0_ADR;
    u32 bypass_mask = 1;
    if (pll == CPR_PLL1)
    {
        pll_ctrl_adr = CPR_PLL_CTRL1_ADR;
        pll_stat_adr = CPR_PLL_STAT1_ADR;
        bypass_mask = 2;
    }

    tyHglCprRational result = {1, 1};

    if ((GET_REG_WORD_VAL(CPR_CLK_BYPASS_ADR) & bypass_mask) == 0)
    {
        u32 pllc = GET_REG_WORD_VAL(pll_ctrl_adr);
        if ((pllc & 3) == 0)
        {
            // not powered down, and not force bypassed
            u32 plls = GET_REG_WORD_VAL(pll_stat_adr);
            if ((plls & 3) == 3)
            {
                u32 fdc = (pllc >> PLL_CTRL_PLL_FDC_FIELD_LSB) & ((1 << PLL_CTRL_PLL_FDC_FIELD_LENGTH) - 1);
                u32 idc = (pllc >> PLL_CTRL_PLL_IDC_FIELD_LSB) & ((1 << PLL_CTRL_PLL_IDC_FIELD_LENGTH) - 1);
                u32 odc = (pllc >> PLL_CTRL_PLL_ODC_FIELD_LSB) & ((1 << PLL_CTRL_PLL_ODC_FIELD_LENGTH) - 1);

                u32 fd_real = 2 * (fdc + 1);
                u32 id_real = idc + 1;
                u32 od_real = 1 << odc;

                result.numerator = fd_real;
                result.denominator = id_real * od_real;
            }
        }
    }

    return result;
}

u32 HglCprCalcClk(u32 clkIn, u32 numerator, u32 denominator)
{
    u32 clkKhz;

    HGL_CPR_DPRINTF2("\ncalculateClock() -> %d %d %d\n",clkIn,numerator,denominator);
    // This function only works with "reasonable" numerator values
    // If the pll frequency is 800Mhz we can't deal with numerators over 5368 without
    // overflowing the 32 bit value. I think this assumption is reasonably safe, but adding assert JIC
    assert(numerator < 5000);

    // when divider is 0:0 it is in bypass mode

    if (numerator == 0 || denominator == 0 || numerator >= denominator)
    {
        clkKhz = clkIn;
    }
    else
    {
        clkKhz = clkIn * numerator / denominator;
    }

    HGL_CPR_DPRINTF2("\ncalculateClock() -> %d Khz\n",clkKhz);

    return clkKhz;
}

tyHglCprRational HglCprGetSysMultiplier(void)
{
    tyHglCprRational result;
    u32 cpr_clk_div_val = GET_REG_WORD_VAL(CPR_CLK_DIV_ADR);
    result.numerator   = (cpr_clk_div_val >> 16) & MASK_MAS_DIV;
    result.denominator = (cpr_clk_div_val >> 0 ) & MASK_MAS_DIV;
    if (result.numerator == 0 || result.denominator == 0 || result.numerator >= result.denominator)
    {
        result.numerator = 1;
        result.denominator = 1;
    }

    return result;
}

void HglCprGetAuxClockMultiplier(u32 auxClock, tyHglCprClockSrc *source, tyHglCprRational *multiplier)
{
    u32 clockDivider;
    u32 numerator, denominator;

    clockDivider = GET_REG_WORD_VAL(CPR_AUX0_CLK_CTRL_ADR + (auxClock << 2));

    *source      = (tyHglCprClockSrc) ((clockDivider >> 28) & MASK_CLK_SRC);
    numerator    =                     (clockDivider >> 16) & MASK_AUX_DIV;
    denominator  =                     (clockDivider >> 0 ) & MASK_AUX_DIV;
    if (numerator == 0 || denominator == 0 || numerator >= denominator)
    {
        numerator = 1;
        denominator = 1;
    }
    multiplier->numerator = numerator;
    multiplier->denominator = denominator;
}


u32 HglCprCalcSysClkKhz(u32 pllClockKhz)
{
    u32 numerator;
    u32 denominator;

    numerator = GET_REG_WORD_VAL(CPR_CLK_DIV_ADR);
    denominator = (numerator >> 0 ) & MASK_MAS_DIV;
    numerator   = (numerator >> 16) & MASK_MAS_DIV;

    return HglCprCalcClk(pllClockKhz,numerator,denominator);
}

tyHglCprPllState HglCprGetPllStatus(tyHglCprPll pll)
{
    u32 pllStatus=0;

    switch (pll)
    {
    case CPR_PLL0:
        HGL_CPR_DPRINTF1("\ngetPllState(PLL0);\n");
        pllStatus = GET_REG_WORD_VAL(CPR_PLL_STAT0_ADR);
        break;
    case CPR_PLL1:
        HGL_CPR_DPRINTF1("\ngetPllState(PLL1);\n");
        pllStatus = GET_REG_WORD_VAL(CPR_PLL_STAT1_ADR);
        break;
    default:
        assert(FALSE);
        break;
    }

    if (pllStatus & PLL_UPDATE_COMPLETE_MASK)
    {
        if (pllStatus & PLL_LOCKED_MASK)
            return PLL_STATE_DONE_LOCKED;

        if (pllStatus & PLL_LOCK_TIMEOUT)
            return PLL_STATE_DONE_HW_TIMEOUT;

        return PLL_STATE_DONE_UNKNOWN;
    }
    return PLL_STATE_PENDING;
}

void HglCprTurnOnShaveMask(u32 shave_mask)
{
    HglCprPowerTurnOnIslandsRaw(((shave_mask & 0xFFF) << 3), trickleTicksArray[3], TURN_ON_ENABLE_TICKS);
    HglCprChangeClks(UPA_DOMAIN, ENABLE_CLKS, (shave_mask & 0xFFF));
    HglCprResetDevices(UPA_DOMAIN, PULSE_RESET, (u64)(shave_mask & 0xFFF));
}

void HglCprTurnOffShaveMask(u32 shave_mask)
{
    HglCprResetDevices(UPA_DOMAIN, ASSERT_RESET, (u64)(shave_mask & 0xFFF));
    HglCprChangeClks(UPA_DOMAIN, DISABLE_CLKS, (shave_mask & 0xFFF));
    HglCprPowerTurnOffIslandsRaw(((shave_mask & 0xFFF) << 3), TURN_OFF_ISO_TICKS, TURN_OFF_DISABLE_TICKS);
}

u64  HglCprGetResetStatus(tyHglCprClockDomain domain)
{
    u64  retValue = 0;
    u32  *retValue32;
    u32  baseReset;
    u32  numWords = 1; // Most blocks only have one word to control clk/rst but CSS has 2

    retValue32 = (u32*)&retValue;

    hglCprGetDomainBaseReset(domain, &baseReset, &numWords);

    retValue32[0] = GET_REG_WORD_VAL(baseReset);
    if (numWords  == 2)
        retValue32[1] = GET_REG_WORD_VAL(baseReset+4);

    return retValue;
}

int  HglCprGetUpaClk(u32 id)
{
    if (id!=POWER_AWARE_LRT_CORE_ID)
    {
        u32 val =  GET_REG_WORD_VAL(CMX_CLK_CTRL);
        if (val & (1<<id))
            return 1;
        else
            return 0;
    }
    else
    {
        if (GET_REG_WORD_VAL(MSS_CLK_CTRL_ADR) & (u32)LRT_BITS)
            return 1;
        else
            return 0;
    }
}

//------------------------------------------------------------------------------
// functions not exported as Hgl API
//------------------------------------------------------------------------------
void hglCprAuxClocksEnable(const tyHglCprAuxClkDividerCfg  *pAuxClkCfg)
{
    u32 auxClk;
    u32 auxClockDivider;

    // Ensure that user doesn't specify an invalid value
    assert((pAuxClkCfg->auxClockDivNumerator   & (~MASK_AUX_DIV)) == 0);
    assert((pAuxClkCfg->auxClockDivDenominator & (~MASK_AUX_DIV)) == 0);
    if (pAuxClkCfg->auxClockDivNumerator != pAuxClkCfg->auxClockDivDenominator)                 // Only check for bad ratio if not in bypass
        assert((pAuxClkCfg->auxClockDivNumerator << 1 ) <= pAuxClkCfg->auxClockDivDenominator );    // Clock division must be a minimum of 50%

    auxClockDivider = GEN_AUX_CLK_DIVIDER((u32)pAuxClkCfg->auxClockSource, pAuxClkCfg->auxClockDivNumerator, pAuxClkCfg->auxClockDivDenominator);

    for (auxClk = 0; auxClk <32; auxClk++)
    {
        if ((pAuxClkCfg->auxClockEnableMask & (1<<auxClk)) == (1u<<auxClk))
        {
            u32 oldAuxConfig = GET_REG_WORD_VAL(CPR_AUX0_CLK_CTRL_ADR + (auxClk << 2) );
            if (oldAuxConfig != auxClockDivider)
            {
                // Disabling Aux clk first, to avoid glitches, or bad frequencies
                CLR_REG_BIT_NUM(CPR_AUX_CLK_EN_ADR, auxClk);
                // Temporarly setting source to system clock (%d), for aux clk %d", CLK_SRC_SYS_CLK, auxClk);
                u32 temporaryAuxConfig = GEN_AUX_CLK_DIVIDER(CLK_SRC_SYS_CLK, 0, 0);
                SET_REG_WORD((CPR_AUX0_CLK_CTRL_ADR + (auxClk << 2) ), temporaryAuxConfig);
            }
            SET_REG_WORD((CPR_AUX0_CLK_CTRL_ADR + (auxClk << 2) ), auxClockDivider);
        }
    }
    // Finally enable the relevant clocks
    SET_REG_BITS_MASK(CPR_AUX_CLK_EN_ADR, pAuxClkCfg->auxClockEnableMask);
}

s32 hglCprConfigureWaitPll(u32 pllConfigWord, u32 ticksPerUsCounter)
{
    u32 pll0Status;

    // We must bypass the PLL before reconfiguring it
    HglCprPllBypass(BYPASS_ENABLE, BYPASS_NOCHANGE);
    // Set the new PLL configuration
    SET_REG_WORD(CPR_PLL_CTRL0_ADR, pllConfigWord);
    // Dummy reads to allow for synchronization
    // these generate 4 cycles of delay on the CPR apb slave, to make sure that the CPR state becomes up to date.
    GET_REG_WORD_VAL(CPR_PLL_STAT0_ADR);
    GET_REG_WORD_VAL(CPR_PLL_STAT0_ADR);

    // Wait for PLL to acknowledge its new settings
    do
    {
        pll0Status = GET_REG_WORD_VAL(CPR_PLL_STAT0_ADR);
        ticksPerUsCounter --;
    }
    while (((pll0Status & PLL_UPDATE_COMPLETE_MASK) != PLL_UPDATE_COMPLETE_MASK) && (ticksPerUsCounter));

    // Check Result
    if (ticksPerUsCounter == 0)
    {
        assert(FALSE);
        return ERR_PLL_SET_TIMEOUT;
    }

    // Check PLL LOCK TIMEOUT
    if (pll0Status & PLL_LOCK_TIMEOUT)
    {
        assert(FALSE);
        return ERR_PLL_LOCK_TIMEOUT;
    }

    // Check if the PLL hardware failed to lock
    if ((pll0Status & PLL_LOCKED_MASK) != PLL_LOCKED_MASK)
    {
       return ERR_PLL_LOCK_FAILURE;
    }

    return CPR_SUCCESS;
}

s32 HglConfigurePll(tyHglCprPll pll, tyHglCprClockSrc clkSrcPll, const tyHglSocClockConfig *pClkCfg)
{
    u32 pllClockKhz = 0;
    u32 pllConfigWord = 0;
    u32 srcClkKhz = 0;
    u32 pllTargetFreqKhz = 0;
    tyHglCprPllBypassOption pll0BypassAction = BYPASS_NOCHANGE;
    tyHglCprPllBypassOption pll1BypassAction = BYPASS_NOCHANGE;

    if(pll == CPR_PLL0)
    {
        srcClkKhz = pClkCfg->refClk0InputKhz;
        pllTargetFreqKhz = pClkCfg->targetPll0FreqKhz;
        pll0BypassAction = BYPASS_ENABLE; // Bypass PLL0 before re-configuring it
    }
    if(pll == CPR_PLL1)
    {
        if(clkSrcPll != CLK_SRC_REFCLK0 && clkSrcPll != CLK_SRC_REFCLK1)
            return ERR_PLL_CONFIG_INVALID;
        else
        {
            if (clkSrcPll == CLK_SRC_REFCLK0)
                srcClkKhz = pClkCfg->refClk0InputKhz;
            else
                srcClkKhz = pClkCfg->refClk1InputKhz;
        }

        pllTargetFreqKhz = pClkCfg->targetPll1FreqKhz;
        pll1BypassAction = BYPASS_ENABLE; // Bypass PLL1 before re-configuring it
    }

    // Configure the PLL for the desired frequency
    pllConfigWord = HglCprPllConfValue(srcClkKhz, pllTargetFreqKhz, &pllClockKhz);

    if (pllClockKhz == 0)
    {
        return ERR_PLL_CONFIG_INVALID;
    }

    if (clkSrcPll == CLK_SRC_REFCLK1)
    {
        pllConfigWord |= 1 << 18;
        HglCprSetRetentionRegisters(RETENTION_REG_ENABLE_SECOND_OSCILLATOR);  //enabling the second oscillator
    }

    HGL_CPR_DPRINTF1("\npllConfigWord = %08X; Fout = %dKhz\n", pllConfigWord, pllClockKhz);

    HglCprConfigurePll(pll, pllConfigWord, (tyHglCprPllBypassOption)pll0BypassAction, (tyHglCprPllBypassOption)pll1BypassAction);

    // Crucially this function does not wait for the PLL to lock
    // or take the PLL out of bypass
    // It is up the to caller to handle that
    return CPR_SUCCESS;
}

///@}

