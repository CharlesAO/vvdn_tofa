///
/// @file DrvCpr.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvCpr
/// @{
/// @brief     Driver to control Clocks, Power and Reset
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include <stdio.h>
#include <assert.h>
#include <DrvTimer.h>
#include <DrvRegUtils.h>
#include <stdlib.h>
#include <swcShaveLoader.h>
#include <dbgTracerApi.h>

#include <HglCpr.h>
#include <DrvCpr.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define DEFAULT_REF0_INPUT_CLK_KHZ          (12000)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
tyCprState cprState;
extern u32 trickleTicksArray[];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static int notifySystemClockChange(void);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
u32 DrvCprGetSysClockKhz(void)
{
    return DrvCprGetClockFreqKhz(SYS_CLK, NULL);
}

u32 DrvCprGetSysClocksPerUs(void)
{
    return DrvCprGetClockFreqKhz(SYS_CLK, NULL) / 1000;
}

static int setupSystemClk(int pllTargetFreqKhz, int clockDivNumerator, int clockDivDenominator)
{
    u32 pllConfigWord, counter;

    // Configure the PLL for the desired frequency
    u32 resultClockKhz;
    pllConfigWord = HglCprPllConfValue(cprState.ref0InClkKhz, pllTargetFreqKhz, &resultClockKhz);
    counter       = (DrvCprGetSysClocksPerUs() * PLL_SW_TIMEOUT_US) >> 2;

    HglCprSetupSystemClk(clockDivNumerator, clockDivDenominator, pllConfigWord, counter);

    // Crucially we don't re-enable the PLL at this stage.
    // It is up the to the caller to handle that
    return CPR_SUCCESS;
}

/* Allows to initialize the driver from a known state */
int DrvCprInitClocks(u32 ref0InClkKhz, u32 ref1InClkKhz, u32 unused0, u32 unused1, u32 unused2)
{
    // TODO: Remove the zero initialisation as that is redundant due to bss clearing.
    // Initialise some of the static data
    cprState.pfSysClockFreqChange       = NULL;
    UNUSED(unused0);
    UNUSED(unused1);
    UNUSED(unused2);
    cprState.ref0InClkKhz               = ref0InClkKhz;
    cprState.ref1InClkKhz               = ref1InClkKhz;

    // Set initialised flag to true
    cprState.initialised = TRUE;
    // If necessary.. Do some low power config in here. E.g. USB updates
    return CPR_SUCCESS;
}

// TBD remove DrvTimerInit
int DrvCprInit(void)
{
    // TODO: Remove the zero initialisation as that is redundant due to bss clearing.
    // Initialise some of the static data
    cprState.pfSysClockFreqChange       = NULL;
    // Default to assuming a 12Mhz Clock Oscillator until told otherwise
    cprState.ref0InClkKhz               = 12000;
    cprState.ref1InClkKhz               = 0;

    HglCprMssTimerActivate();  //TODO: remove this one, as the timer functionalities are no more needed by the CPR driver

    // Setup Global Tick Counter using Timer Driver
    DrvTimerInit();

    // Set initialised flag to true
    cprState.initialised = TRUE;

    // If necessary.. Do some low power config in here. E.g. USB updates
    return CPR_SUCCESS;
}

void DrvCprRequestSysClkChangeNotification(tySysClkFreqChangeCb pfSysClockFreqChange)
{
    // User may not need this callback any longer and we check for NULL before calling
    // assert(pfSysClockFreqChange != 0);
    cprState.pfSysClockFreqChange = pfSysClockFreqChange;
    return;
}

#ifdef POWER_AWARE

void DrvCprCheckandstopISR(u32 id)
{
    if (swcShaveRunning(id))
        return;

    if (DrvCprGetUpaClk(id))
        DrvCprClearUpa(id);
}

int DrvCprGetUpaClk(u32 id)
{
    return (HglCprGetUpaClk(id));
}

void DrvCprEnableUpa(u32 id)
{
    HglCprEnableUpa(id);
}

void DrvCprClearUpa(u32 id)
{
    HglCprClearUpa(id);
}

#endif

int DrvCprConfigureSystemClk(int pllTargetFreqKhz, int clockDivNumerator, int clockDivDenominator)
{
    int retVal;

    if ( ! cprState.initialised)
        return ERR_CPR_NOT_INITIALISED;

    DrvCprPllBypass(BYPASS_ENABLE,BYPASS_NOCHANGE); // Bypass Pll0 as we are about to modify system clock

    // Configure the PLL and the Sys clock divider but don't re-enable the PLL yet
    retVal = setupSystemClk(pllTargetFreqKhz,clockDivNumerator,clockDivDenominator);
    if (retVal )
        return retVal;

    // Re-enable PLL
    DrvCprPllBypass(BYPASS_DISABLE,BYPASS_NOCHANGE);

    return DrvCprGetSysClockKhz();
}

int DrvCprAuxClockArrayConfig(const tyAuxClkDividerCfg  pAuxClkCfg[])
{
    HglCprAuxClockArrayConfig((const tyHglCprAuxClkDividerCfg*)pAuxClkCfg);

    return 0;
}

static tyHglCprRational DrvCprGetPll0KhzRational( void )
{
    tyHglCprRational inputClock;
    inputClock = HglCprIntegerToRational(cprState.ref0InClkKhz);
    return HglCprMultiply(inputClock, HglCprGetPllMultiplier(CPR_PLL0));
}

static int isPll1UsingRefclk1(void) {
    return GET_REG_WORD_VAL(CPR_PLL_CTRL1_ADR) & (1 << 18);
}

static tyHglCprRational DrvCprGetPll1KhzRational( void )
{
    tyHglCprRational inputClock;
    if (isPll1UsingRefclk1())
    {
        inputClock = HglCprIntegerToRational(cprState.ref1InClkKhz);
    }
    else
    {
        inputClock = HglCprIntegerToRational(cprState.ref0InClkKhz);
    }
    return HglCprMultiply(inputClock, HglCprGetPllMultiplier(CPR_PLL1));
}

static tyHglCprRational DrvCprGet32kClockKhzRational( void )
{
    // We simply assume that the 32.768Khz clock is present
    tyHglCprRational result;
    if (IS_PLATFORM_VCS)
    {
        result.numerator = 32;
        result.denominator = 1;
    }
    else
    {
        result.numerator = 32768;
        result.denominator = 1000;
    }
    return result;
}

tyHglCprRational DrvCprGetSysClockKhzRational( void )
{
    return HglCprMultiply(DrvCprGetPll0KhzRational(), HglCprGetSysMultiplier());
}

static tyHglCprRational DrvCprGetHalfSysClockKhzRational( void )
{
    tyHglCprRational result = DrvCprGetSysClockKhzRational();
    result.denominator *= 2;
    return result;
}

// Passed an Enum of different clock types, SYSTEM, AUX clocks
tyHglCprRational DrvCprGetClockFreqKhzRational(tyClockType clockType, __attribute__((unused)) tyClockConfig * clkCfg)
{
    tyHglCprRational clockKhz;
    tyCprClockSrc source;
    tyHglCprRational clkIn;
    tyHglCprRational auxClockMult;

    // clkCfg is deliberately unused for now
    // TODO: Update clkCfg structure with details of any clock anomalies
    // For now we deliberately don't bother to update it

    assert(clockType < LAST_CLK);

    switch (clockType)
    {
    case SYS_CLK:
        clockKhz  = DrvCprGetSysClockKhzRational();
        break;
    case SYS_CLK_DIV2:
        clockKhz  = DrvCprGetHalfSysClockKhzRational(); // Its simply half the clock rate of SYS_CLK
        break;
    case PLL0_OUT_CLK:
        clockKhz  = DrvCprGetPll0KhzRational();
        break;
    case PLL1_OUT_CLK:
        clockKhz  = DrvCprGetPll1KhzRational();
        break;
    case REFCLK0:
        clockKhz  = HglCprIntegerToRational(cprState.ref0InClkKhz);
        break;
    case REFCLK1:
        clockKhz  = HglCprIntegerToRational(cprState.ref1InClkKhz);
        break;
    default:   // Assume Auxiliary Clock

        HglCprGetAuxClockMultiplier(clockType, (tyHglCprClockSrc*)&source, &auxClockMult);

        switch (source)
        {
        case CLK_SRC_PLL0:
            clkIn    = DrvCprGetPll0KhzRational();
            break;
        case CLK_SRC_PLL1:
            clkIn    = DrvCprGetPll1KhzRational();
            break;
        case CLK_SRC_REFCLK0:
            clkIn    = HglCprIntegerToRational(cprState.ref0InClkKhz);
            break;
        case CLK_SRC_REFCLK1:
            clkIn    = HglCprIntegerToRational(cprState.ref1InClkKhz);
            break;
        case CLK_SRC_32KHZ:
            clkIn    = DrvCprGet32kClockKhzRational();
            break;
        case CLK_SRC_AUX_CPR:
            clkIn    = DrvCprGetClockFreqKhzRational(AUX_CLK_CPR, clkCfg); // Note: recursive!!
            break;
        case CLK_SRC_SYS_CLK:
            clkIn    = DrvCprGetSysClockKhzRational();
            break;
        case CLK_SRC_SYS_CLK_DIV2:
            clkIn    = DrvCprGetHalfSysClockKhzRational();
            break;
        default:
            assert(FALSE);
            break;
        }

        clockKhz = HglCprMultiply(clkIn, auxClockMult);

        if ( HglCprIsAuxClockEnabled((u32)clockType) == 0 )
        {
            clockKhz = HglCprIntegerToRational(0); // Clock is not enabled so it is 0Khz
        }
        break;
    }

    return clockKhz;
}

// Passed an Enum of different clock types, SYSTEM, AUX clocks
int DrvCprGetClockFreqKhz(tyClockType clockType, __attribute__((unused)) tyClockConfig * clkCfg)
{
    return HglCprRationalToInteger(DrvCprGetClockFreqKhzRational(clockType, clkCfg), HGL_CPR_ROUND_DOWN);
}

// Passed an Enum of different clock types, SYSTEM, AUX clocks
double DrvCprGetClockFreqKhzFloat(tyClockType clockType, __attribute__((unused)) tyClockConfig * clkCfg)
{
    return HglCprRationalToFloat(DrvCprGetClockFreqKhzRational(clockType, clkCfg));
}

int DrvCprPllBypass(tyPllBypassOption pll0Bypass,tyPllBypassOption pll1Bypass)
{
    HglCprPllBypass(pll0Bypass, pll1Bypass);

    // If the main system clock PLL was enabled then we notify applications of
    // a change in system clock event.
    // We don't do this for PLL_Bypass operations as they are considered transient steps on the road
    // to a new frequency.
    if (pll0Bypass == BYPASS_DISABLE)
        notifySystemClockChange();

    return 0;
}


void DrvCprStartAllClocks(void)  // DO NOT USE IT FOR REAL PROJECT - INVOLVES EXTRA POWER CONSUMPTION
{
    HglCprEnableAllClocks();
}

void DrvCprPowerTurnOnIslandsRaw(u32 islands_mask, u32 trickle_ticks, u32 enable_ticks)
{
    HglCprPowerTurnOnIslandsRaw(islands_mask, trickle_ticks, enable_ticks);
}

void DrvCprPowerTurnOffIslandsRaw(u32 islands_mask, u32 iso_ticks, u32 disable_ticks)
{
    HglCprPowerTurnOffIslandsRaw(islands_mask, iso_ticks, disable_ticks);
}

u32 DrvCprPowerGetStatus( void )
{
    return HglCprPowerGetStatus();
}

int DrvCprAuxClocksEnable(u32 auxClksMask,tyCprClockSrc clkSource,u32 numerator, u32 denominator)
{
    tyAuxClkDividerCfg clkSettings;

    clkSettings.auxClockEnableMask     = auxClksMask;
    clkSettings.auxClockSource         = clkSource;
    clkSettings.auxClockDivNumerator   = numerator;
    clkSettings.auxClockDivDenominator = denominator;

    return HglCprAuxClocksEnable(&clkSettings);
}

void DrvCprPowerTurnOnIsland(enum PowerIslandIndex island_index)
{
    dbgLogEvent(ISLAND_TO_SIGNAL(island_index), 1, DEBUG_LOG_LEVEL_HIGH);
    HglCprPowerTurnOnIslandsRaw(1 << island_index, trickleTicksArray[island_index], TURN_ON_ENABLE_TICKS);
}

void DrvCprPowerTurnOffIsland(enum PowerIslandIndex island_index)
{
    dbgLogEvent(ISLAND_TO_SIGNAL(island_index), 0, DEBUG_LOG_LEVEL_HIGH);
    HglCprPowerTurnOffIslandsRaw(1 << island_index, TURN_OFF_ISO_TICKS, TURN_OFF_DISABLE_TICKS);
}

u32 DrvCprGetCprVersionId(void)
{
    return HGL_CPR_GET_CPR_VERSION_ID();
}

static int notifySystemClockChange(void)
{
    // Handles the things that need to happen every time the system clock changes.
    // There are two reasons the system clock can change:
    // 1) DrvCprSetupClocks applies a new clock config
    // 2) DrvCprConfigureSystemClk updates just the system clock
    // In both cases the PLL will first be disabled resulting in a temporary switch to 12Mhz
    // and then once new clock is applied the PLL is re-enabled. At this stage this function is called
    // Note: We have a policy of not notifying the application of an actual PLL Bypass as there is
    // an implicit assumption that this is a transient event and not one that can be validly handled by an application

    // If the user has requested an application callback for frequency we do so now
    u32 sysClockKhz = HglCprRationalToInteger(DrvCprGetSysClockKhzRational(), HGL_CPR_ROUND_DOWN);
    if (cprState.pfSysClockFreqChange != 0)
    {
        cprState.pfSysClockFreqChange(sysClockKhz);
    }

    dbgLogEvent(LOG_EVENT_SYS_CLK_CHANGE, sysClockKhz, DEBUG_LOG_LEVEL_HIGH);
    return 0;
}


static int configurePll(tyCprPll pll, tyCprClockSrc clkSrcPll, int pllTargetFreqKhz)
{
    u32 pllClockKhz;
    u32 pllConfigWord;
    u32 srcClkKhz;
    tyPllBypassOption pll0BypassAction = BYPASS_NOCHANGE;
    tyPllBypassOption pll1BypassAction = BYPASS_NOCHANGE;

    switch (pll)
    {
    case CPR_PLL0:
        CPR_DPRINTF1("\nconfigurePll(PLL0);\n");
        srcClkKhz = cprState.ref0InClkKhz;
        pll0BypassAction = BYPASS_ENABLE;       // Bypass PLL0 before re-configuring it
        break;
    case CPR_PLL1:
        pll1BypassAction = BYPASS_ENABLE;       // Bypass PLL1 before re-configuring it
        switch (clkSrcPll)
        {
        case CLK_SRC_REFCLK0:
            CPR_DPRINTF1("\nconfigurePll(PLL1; src = REFCLK0);\n");
            srcClkKhz = cprState.ref0InClkKhz;
            break;
        case CLK_SRC_REFCLK1:
            CPR_DPRINTF1("\nconfigurePll(PLL1; src = REFCLK1);\n");
            srcClkKhz = cprState.ref1InClkKhz;
            break;
        case CLK_SRC_PLL0:    // These are not available as source for PLL
        case CLK_SRC_PLL1:
        case CLK_SRC_32KHZ:
        case CLK_SRC_AUX_CPR:
        default:
            assert(FALSE);
            return ERR_PLL_CONFIG_INVALID;
            break;
        }
        break;
    default:
        assert(FALSE);
        return ERR_PLL_CONFIG_INVALID;
        break;
    }

    // Configure the PLL for the desired frequency
    pllConfigWord = HglCprPllConfValue(srcClkKhz, pllTargetFreqKhz, &pllClockKhz);
    if (pllClockKhz == 0)
    {
        assert(0);
        return ERR_PLL_CONFIG_INVALID;
    }
    if (pll == CPR_PLL0)
    {
        // For PLL0 we leave the upper clock source bits at zer0
    }
    else
    {
        if (clkSrcPll == CLK_SRC_REFCLK1)
        {
            pllConfigWord |= 1 << 18;
            HglCprSetRetentionRegisters(RETENTION_REG_ENABLE_SECOND_OSCILLATOR);  //enabling the second oscillator
        }
    }

    CPR_DPRINTF1("\npllConfigWord = %08X; Fout = %dKhz\n",pllConfigWord,pllClockKhz);

    HglCprConfigurePll(pll, pllConfigWord, (tyHglCprPllBypassOption)pll0BypassAction, (tyHglCprPllBypassOption)pll1BypassAction);

    // If the main system clock PLL was enabled then we notify applications of
    // a change in system clock event.
    // We don't do this for PLL_Bypass operations as they are considered transient steps on the road
    // to a new frequency.
    if (pll0BypassAction == BYPASS_DISABLE)
        notifySystemClockChange();

    // Crucially this function does not wait for the PLL to lock
    // or take the PLL out of bypass
    // It is up the to caller to handle that
    return CPR_SUCCESS;
}

int DrvCprSetupClocks(const tySocClockConfig *pClkCfg)
{
    int __attribute__((unused)) retVal; // avoid 'unused variable' error from scan build
    tyPllBypassOption pll0BypassAction = BYPASS_NOCHANGE;
    tyPllBypassOption pll1BypassAction = BYPASS_NOCHANGE;

    if (!cprState.initialised)
    {
        retVal = ERR_CPR_NOT_INITIALISED;
        goto CPR_SETUP_ERR;
    }

    int refclk0_changed = 0;
    if (pClkCfg->refClk0InputKhz != cprState.ref0InClkKhz)
    {
        refclk0_changed = 1;
    }
    cprState.ref0InClkKhz = pClkCfg->refClk0InputKhz;
    cprState.ref1InClkKhz = pClkCfg->refClk1InputKhz;

    if (pClkCfg->targetPll0FreqKhz != 0)
    {
        // Set the reference clocks to the requested values
        pll0BypassAction = BYPASS_DISABLE;  // remember to enable PLL1 when complete
    }
    else
    {
        // PLL0 isn't being enabled, as such the system clock will be the
        // the refclk0 clock direct
        // If the input refClock is not the same as the default we
        // need to update the system clock state
        if (refclk0_changed)
        {
            notifySystemClockChange();
        }
    }

    if (pClkCfg->targetPll1FreqKhz != 0)
    {
        pll1BypassAction = BYPASS_DISABLE;  // remember to enable PLL1 when complete
    }

    retVal = HglCprSetupClocks(pClkCfg);
    if(retVal)
        goto CPR_SETUP_ERR;

    // Finally turn back on the PLL (which also notifies the app)
    DrvCprPllBypass(pll0BypassAction, pll1BypassAction);

    return CPR_SUCCESS;

CPR_SETUP_ERR:
    assert(FALSE);
    return retVal;
}

static tyCprPllState WaitForPllStateNotPending(tyCprPll pll)
{
    tyCprPllState pllStatus;
    do
    {
        pllStatus = (pll == CPR_PLL0) ? HglCprGetPllStatus(CPR_PLL0) : HglCprGetPllStatus(CPR_PLL1);
    } while (pllStatus == PLL_STATE_PENDING);
    return pllStatus;
}

int DrvCprSetupJustPll(tyCprPll pll, tyCprClockSrc clkSrcPll, int pllTargetFreqKhz)
{
    int res = configurePll(pll, clkSrcPll, pllTargetFreqKhz);
    if (res!= CPR_SUCCESS)
    {
        return res;
    }
    GET_REG_WORD_VAL(CPR_PLL_STAT0_ADR); // these generate 4 cycles of delay on the CPR apb slave, to make sure that the CPR state becomes up to date.
    GET_REG_WORD_VAL(CPR_PLL_STAT0_ADR);

    tyCprPllState pllStatus = WaitForPllStateNotPending(pll);

    if (pllStatus == PLL_STATE_DONE_HW_TIMEOUT)
    {
        assert(FALSE); // PLL failed to lock.. Fatal error
        return 1;  // TODO: Fix error code
    }

    DrvCprPllBypass( (pll == CPR_PLL0) ? BYPASS_DISABLE : BYPASS_NOCHANGE,
                     (pll == CPR_PLL1) ? BYPASS_DISABLE : BYPASS_NOCHANGE);

    return CPR_SUCCESS;
}

void DrvCprSaveAndSetSystemClock(tySystemClockSettings *systemClockSettings, int sysClockTargetFreqKhz)
{
    systemClockSettings->saved_CPR_PLL_CTRL0 = GET_REG_WORD_VAL(CPR_PLL_CTRL0_ADR);
    systemClockSettings->saved_CPR_CLK_BYPASS = GET_REG_WORD_VAL(CPR_CLK_BYPASS_ADR);
    systemClockSettings->saved_CPR_CLK_DIV = GET_REG_WORD_VAL(CPR_CLK_DIV_ADR);
    DrvCprSetupJustPll(CPR_PLL0, CLK_SRC_REFCLK0, sysClockTargetFreqKhz);
    SET_REG_WORD(CPR_CLK_DIV_ADR, 0x00010001);
}

void DrvCprRestoreSystemClock(const tySystemClockSettings *systemClockSettings)
{
    DrvCprPllBypass(BYPASS_ENABLE, BYPASS_NOCHANGE);
    SET_REG_WORD(CPR_CLK_DIV_ADR, systemClockSettings->saved_CPR_CLK_DIV);
    SET_REG_WORD(CPR_PLL_CTRL0_ADR, systemClockSettings->saved_CPR_PLL_CTRL0);
    WaitForPllStateNotPending(CPR_PLL0);
    SET_REG_WORD(CPR_CLK_BYPASS_ADR, (GET_REG_WORD_VAL(CPR_CLK_BYPASS_ADR) & ~1) | (systemClockSettings->saved_CPR_CLK_BYPASS & 1));
}

void DrvCprSysDeviceAction(tyCprClockDomain clkDomain, tyCprDeviceAction action, u64 devices)
{
    HglCprSysDeviceAction(clkDomain, action, devices);
}

void DrvCprTurnOnShaveMask(u32 shave_mask)
{
    HglCprTurnOnShaveMask(shave_mask);
}

void DrvCprTurnOffShaveMask(u32 shave_mask)
{
    HglCprTurnOffShaveMask(shave_mask);
}


// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TEST CODE below here
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//#include <VcsHooksApi.h>

typedef struct
{
    int     refclk_sel;
    int     fb;
    int     idiv;
    int     odiv;
    int     bs;
    int     bp;
    int     pd;
} t_pll_cfg_s;

void startup_pll(int pll_sel, t_pll_cfg_s *pll_cfg)  //TODO move to HGL?
{
    int regdata;
    int* pll_ctrl_addr   = pll_sel ? (int*)CPR_PLL_CTRL1_ADR : (int*)CPR_PLL_CTRL0_ADR;

    // -------------------------------------------------------------------------------
    // program up PLL 1/0
    // -------------------------------------------------------------------------------
    // Start up the PLL
    regdata = pll_cfg->pd | (pll_cfg->bp<<1) | (pll_cfg->bs<<2) | (pll_cfg->odiv<<4) | (pll_cfg->idiv<<6) | (pll_cfg->fb<<11);
    regdata |= ((pll_cfg->refclk_sel)<<18);
    SET_REG_WORD(pll_ctrl_addr, regdata);
//    unitTestReadWordCheck(pll_ctrl_addr, regdata);
}

void wait4pll_lock(int pll_sel)  //TODO move to HGL?
{
    int regdata;
    int*  pll_stat_addr = pll_sel ? (int*)CPR_PLL_STAT1_ADR : (int*)CPR_PLL_STAT0_ADR;

    // Wait for PLL to lock
    int timeoutcount = 2000;

    regdata = GET_REG_WORD_VAL(pll_stat_addr);
    while ((!((regdata&(0x1<<0)) || (regdata&(0x1<<2)))) && (timeoutcount--)) {
       regdata = GET_REG_WORD_VAL(pll_stat_addr);
    }

    // signal failure on timeout
    if (timeoutcount <= 0)
    {
        // PLL failed to lock
        exit(-1);
        while (1)
            ; // Loop forever if exit doesn't work
    }

    return ;
}

void DrvCprCommonTestConfigureClocks(void)  //TODO (re)move or split to HGL?
{
    // Program the divider to 0
    SET_REG_WORD(CPR_CLK_DIV_ADR, 0x00000000);

    // Turn off regular bypass
    SET_REG_WORD(CPR_CLK_BYPASS_ADR, 0x00000000);
    SET_REG_WORD(CPR_NUM_CYCLES_ADR, 0x0000001f);

    // Boot PLL 0 to 492MHz
    t_pll_cfg_s   pll_cfg0;
    pll_cfg0.fb=40; pll_cfg0.idiv=0 ;pll_cfg0.odiv=1; pll_cfg0.bs=0; pll_cfg0.bp=0b0; pll_cfg0.pd=0b0;
    pll_cfg0.refclk_sel = 0x0; // doesn't matter here
    startup_pll(0, &pll_cfg0);
    wait4pll_lock(0);

    return;
}


///@}

