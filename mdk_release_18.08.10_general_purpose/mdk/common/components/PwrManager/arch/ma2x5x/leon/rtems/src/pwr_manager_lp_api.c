///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     power manager: low power implementation
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvCpr.h>
#include <HglCpr.h>
#include <DrvDdr.h>
#include <DrvGpio.h>
#include <DrvIcb.h>
#include <DrvLeonL2C.h>
#include <DrvLeonDefines.h>
#include <DrvRegUtils.h>
#include <dbgTracerApi.h>
#include "pwr_manager_lp_api.h"

extern void switchStackAndEntersLp_asm(void);
extern void restoreStackAndExitLp_asm(void);

extern uint32_t _LOCKING_AREA_START;  //linker symbols
extern uint32_t _LOCKING_AREA_END;    //

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#if 0
#include <stdio.h>
#define DPRINTF(...) printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

#define L2CACHE_WAY_SIZE            (64 * 1024)
#define CLEAR_CACHE_DISABLE         0
#define FETCH_LINES                 1
#define SET_VALID                   1
#define CLEAR_DIRTY                 1
#define LP_INTERRUPT_PRIORITY       0xE

#define PLL0_DEFAULT_DISABLED_VALUE (0x00013803) //hardcoded !!
#define PLL1_DEFAULT_DISABLED_VALUE (0x000D3803) //
#define PAD_RECEIVER_ON             (1<<11)

#define pwr_manager_get_psr(_psr)                               \
    do {                                                        \
        (_psr) = 0;                                             \
        asm volatile("mov %%psr, %0" : "=r"(_psr) : "0"(_psr)); \
    } while (0)

#define pwr_manager_set_psr(_psr) \
    do {                          \
        asm volatile(             \
            "mov  %0, %%psr\n\t"  \
            "nop; nop; nop"       \
            : "=r"((_psr))        \
            : "0"((_psr)));       \
    } while (0)

#define pwr_manager_disable_interrupts() \
    do {                                 \
        register uint32_t _level;        \
        pwr_manager_get_psr(_level);     \
        _level |= MASK_PSR_PIL;          \
        pwr_manager_set_psr(_level);     \
    } while (0)

#define pwr_manager_enable_interrupts() \
    do {                                \
        register uint32_t _level;       \
        pwr_manager_get_psr(_level);    \
        _level &= ~MASK_PSR_PIL;        \
        pwr_manager_set_psr(_level);    \
    } while (0)

typedef struct {
    uint32_t cpr_pll_ctrl0;      //CPR_PLL_CTRL0
    uint32_t cpr_pll_ctrl1;      //CPR_PLL_CTRL1
    uint32_t l2c_ctrl;           //L2C_LEON_OS_CONTROL
    uint32_t cpr_clk_div;        //CPR_CLK_DIV
} tyLockedPowerStateRecoveryInfo;

typedef struct {
    uint32_t cpr_clk_en0;        //CPR_CLK_EN0
    uint32_t cpr_clk_en1;        //CPR_CLK_EN1
    uint32_t cpr_aux_clk_en;     //CPR_AUX_CLK_EN
    uint32_t cpr_clk_rst0;       //CPR_BLK_RST0
    uint32_t cpr_clk_rst1;       //CPR_BLK_RST1
    uint32_t cpr_aux_clk_rst;    //CPR_AUX_CLK_RST

    uint32_t mss_cpu_clk;        //MSS_CPU_CLK_CTRL
    uint32_t mss_sipp_clk;       //MSS_SIPP_CLK_CTRL
    uint32_t mss_amc_clk;        //MSS_AMC_CLK_CTRL
    uint32_t mss_rstn;           //MSS_RSTN_CTRL
    uint32_t mss_sipp_rstn;      //MSS_SIPP_RSTN_CTRL
    uint32_t mss_amc_rstn;       //MSS_AMC_RSTN_CTRL

    uint32_t cmx_clk_ctrl;       //CMX_CLK_CTRL
    uint32_t cmx_rstn_ctrl;      //CMX_RSTN_CTRL

    uint32_t icb0_en0;           //ICB0_ENABLE_0
    uint32_t icb0_en1;           //ICB0_ENABLE_1

    uint32_t gpio_int0;          //gpio interrupt source0
    uint32_t gpio_mode;          //initial mode of the wakeup pin
} tyPowerStateRecoveryInfo;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

LOCKABLE_DATA LowPowerInfo low_power_data;

// 4: Static Local Data
// ----------------------------------------------------------------------------

              static tyPowerStateRecoveryInfo       power_recovery_info;
LOCKABLE_DATA static tyLockedPowerStateRecoveryInfo locked_power_recovery_info;
LOCKABLE_DATA static tyAuxClkDividerCfg             aux_clk_32_khz[] = {
    {AUX_CLK_MASK_32KHZ, CLK_SRC_32KHZ, 1, 1},  // enable 32khz clock
    {AUX_CLK_MASK_32KHZ, CLK_SRC_PLL0, 1, 1},   // restore clock to pll0
};

// 5: Function Prototypes
// ----------------------------------------------------------------------------
void powerStateExitLowPower(void);

// ----------------------------------------------------------------------------
//enable the IRQ generation in ICB
static inline __attribute__((always_inline)) void icbSetupIrq(uint32_t irq_src, uint32_t priority, uint32_t type) {
    uint32_t t =(irq_src & 0x20 )? 4 : 0;

    // Configure Interrupt and priority
    SET_REG_WORD(ICB0_BASE_ADR + ( irq_src << 2 ), ( priority << 2 )| type );
    SET_REG_WORD(ICB0_ENABLE_0_ADR + t, 1 << (irq_src - t*8) );
}

//disable the IRQ generation in ICB and clear it
static inline __attribute__ ((always_inline)) void icbUnsetupIrq(uint32_t irq_src) {
    uint32_t t =(irq_src & 0x20 )? 4 : 0;

    SET_REG_WORD(ICB0_ENABLE_0_ADR  + t, 0);
    SET_REG_WORD(ICB0_CLEAR_0_ADR  +  t, 1 << (irq_src - t*8) );
    SET_REG_WORD(ICB0_BASE_ADR + ( irq_src << 2 ), 0 );
}

//configure the GPIO/ICB in order to allow GPIO IRQ generation
static inline __attribute__((always_inline)) void configureGpioForWakeup(uint32_t gpio_nb) {
    SET_REG_WORD(GPIO_MODE0_ADR + 4*gpio_nb, D_GPIO_MODE_7 | D_GPIO_DIR_IN | D_GPIO_WAKEUP_ON | PAD_RECEIVER_ON);
    SET_REG_WORD(GPIO_INT0_CFG_ADR, (D_GPIO_INT_GPIO_MASK_0 & gpio_nb) | D_GPIO_INT_EN_0);
    icbSetupIrq(IRQ_GPIO_0, LP_INTERRUPT_PRIORITY, POS_LEVEL_INT);
}

//save the currently enabled interrupts then disable them all (including and especially the
//RTEMS tick interrupts), before entering low power
static void backupAndStopInterrupts(void) {
    //Backup the current interrupts
    power_recovery_info.icb0_en0 = GET_REG_WORD_VAL(ICB0_ENABLE_0_ADR);
    power_recovery_info.icb0_en1 = GET_REG_WORD_VAL(ICB0_ENABLE_1_ADR);

    //Stop all interrupts
    SET_REG_WORD(ICB0_ENABLE_0_ADR, 0);
    SET_REG_WORD(ICB0_ENABLE_1_ADR, 0);
    SET_REG_WORD(ICB0_CLEAR_0_ADR, 0xFFFFFFFF);
    SET_REG_WORD(ICB0_CLEAR_1_ADR, 0xFFFFFFFF);
}

//reenable the interrupts that were active before entering LP
static void restoreInterrupts(void) {
    SET_REG_WORD(ICB0_ENABLE_0_ADR, power_recovery_info.icb0_en0);
    SET_REG_WORD(ICB0_ENABLE_1_ADR, power_recovery_info.icb0_en1);
}

//save all the existing clocks, resets, pll and gpio configuration that will be modified
//by the LP process, before entering low power
static void saveHwContext(void) {
    // Backup the existing clock configuration
    power_recovery_info.cpr_clk_en0 = GET_REG_WORD_VAL(CPR_CLK_EN0_ADR);
    power_recovery_info.cpr_clk_en1 = GET_REG_WORD_VAL(CPR_CLK_EN1_ADR);
    power_recovery_info.cpr_aux_clk_en = GET_REG_WORD_VAL(CPR_AUX_CLK_EN_ADR);
    power_recovery_info.mss_cpu_clk = GET_REG_WORD_VAL(MSS_CPU_CLK_CTRL_ADR);
    power_recovery_info.mss_sipp_clk = GET_REG_WORD_VAL(MSS_SIPP_CLK_CTRL_ADR);
    power_recovery_info.cmx_clk_ctrl = GET_REG_WORD_VAL(CMX_CLK_CTRL);

    // Backup the reset clock configuration
    power_recovery_info.cpr_clk_rst0 = GET_REG_WORD_VAL(CPR_BLK_RST0_ADR);
    power_recovery_info.cpr_clk_rst1 = GET_REG_WORD_VAL(CPR_BLK_RST1_ADR);
    power_recovery_info.cpr_aux_clk_rst = GET_REG_WORD_VAL(CPR_AUX_CLK_RST_ADR);
    power_recovery_info.mss_rstn = GET_REG_WORD_VAL(MSS_RSTN_CTRL_ADR);
    power_recovery_info.mss_sipp_rstn = GET_REG_WORD_VAL(MSS_SIPP_RSTN_CTRL_ADR);
    power_recovery_info.cmx_rstn_ctrl = GET_REG_WORD_VAL(CMX_RSTN_CTRL);

    // Backup the PLL and SYS CLOCK configuration
    locked_power_recovery_info.cpr_pll_ctrl0 = GET_REG_WORD_VAL(CPR_PLL_CTRL0_ADR);
    locked_power_recovery_info.cpr_pll_ctrl1 = GET_REG_WORD_VAL(CPR_PLL_CTRL1_ADR);
    locked_power_recovery_info.cpr_clk_div = GET_REG_WORD_VAL(CPR_CLK_DIV_ADR);

    if (low_power_data.wakeup.source == LP_WAKEUP_SOURCE_GPIO) {
        // Backup the GPIO information for the GPIO we will override
        power_recovery_info.gpio_int0 = GET_REG_WORD_VAL(GPIO_INT0_CFG_ADR);
        power_recovery_info.gpio_mode = GET_REG_WORD_VAL(GPIO_MODE0_ADR + 4*low_power_data.wakeup.data.gpio.number);
    }
}

//restore the initial clocks, resets, pll and gpio configuration, as before entering low power
static void restoreHwContext(void) {
    // Restore clock configs
    SET_REG_WORD(CPR_CLK_EN0_ADR       , power_recovery_info.cpr_clk_en0);
    SET_REG_WORD(CPR_CLK_EN1_ADR       , power_recovery_info.cpr_clk_en1);

    SET_REG_WORD(CPR_AUX_CLK_EN_ADR    , power_recovery_info.cpr_aux_clk_en);
    SET_REG_WORD(MSS_CPU_CLK_CTRL_ADR  , power_recovery_info.mss_cpu_clk);
    SET_REG_WORD(MSS_SIPP_CLK_CTRL_ADR , power_recovery_info.mss_sipp_clk);
    SET_REG_WORD(CMX_CLK_CTRL          , power_recovery_info.cmx_clk_ctrl);

    // And restore reset configuration
    SET_REG_WORD(CPR_BLK_RST0_ADR      , power_recovery_info.cpr_clk_rst0);
    SET_REG_WORD(CPR_BLK_RST1_ADR      , power_recovery_info.cpr_clk_rst1);
    SET_REG_WORD(CPR_AUX_CLK_RST_ADR   , power_recovery_info.cpr_aux_clk_rst);
    SET_REG_WORD(MSS_RSTN_CTRL_ADR     , power_recovery_info.mss_rstn);
    SET_REG_WORD(MSS_SIPP_RSTN_CTRL_ADR, power_recovery_info.mss_sipp_rstn);
    SET_REG_WORD(CMX_RSTN_CTRL         , power_recovery_info.cmx_rstn_ctrl);

    if (low_power_data.wakeup.source == LP_WAKEUP_SOURCE_GPIO) {
        //Restore the GPIO information
        SET_REG_WORD(GPIO_INT0_CFG_ADR                                        , power_recovery_info.gpio_int0);
        SET_REG_WORD(GPIO_MODE0_ADR + 4*low_power_data.wakeup.data.gpio.number, power_recovery_info.gpio_mode);
    }
}

#ifdef USE_SUSPEND_LRT
static void resumeLrt(void) {
    //TODO: Find out if DSU is used or not:

    //Below code assumes DSU is in use
    extern void lrt_restoreProcessorAfterLp_asm(void);
    uint32_t entry_point = (uint32_t)&lrt_restoreProcessorAfterLp_asm;

    // First, break on LRT
    SET_REG_WORD(DSU_LEON_RT_BREAK_SINGLE_STEP_ADR, 0x1);

    // Then reset Leon RT
    SET_REG_WORD(CPR_LEON_RT_VECTOR_ADR, LEON_IRQI_RESET);
    SET_REG_WORD(CPR_LEON_RT_VECTOR_ADR, LEON_IRQI_NONE);

    // Set to LeonRT the halt flag, so it doesn't start running immediately
    SET_REG_WORD(DSU_LEON_RT_CTRL_ADR,
                 DSU_CTRL_PROC_HALT_BIT | DSU_CTRL_DEBUG_MODE_BIT |
                 DSU_CTRL_DSUEN_BIT | DSU_CTRL_BREAK_ON_SW_WATCHPOINT_BIT |
                 DSU_CTRL_BREAK_ON_IU_WATCHPOINT_BIT |
                 DSU_CTRL_BREAK_ON_ERROR_BIT | DSU_CTRL_TRACE_ENABLE_BIT);

    // Set PC/NPC before removing the half and single step flags
    SET_REG_WORD(DSU_LEON_RT_PC_REG_ADR, entry_point);
    SET_REG_WORD(DSU_LEON_RT_NPC_REG_ADR, entry_point + 4);
    SET_REG_WORD(DSU_LEON_RT_BREAK_SINGLE_STEP_ADR, 0x0);
    SET_REG_WORD(DSU_LEON_RT_CTRL_ADR,
                 DSU_CTRL_DEBUG_MODE_BIT | DSU_CTRL_DSUEN_BIT |
                 DSU_CTRL_BREAK_ON_SW_WATCHPOINT_BIT |
                 DSU_CTRL_BREAK_ON_IU_WATCHPOINT_BIT |
                 DSU_CTRL_BREAK_ON_ERROR_BIT | DSU_CTRL_TRACE_ENABLE_BIT);
}
#endif

//turn off the unneeded clocks (the listed ones will remain active),
//then resets the blocks and turn off all power islands (less the POWER_ISLAND_RETENTION and POWER_ISLAND_CSS_DIGITAL)
static LOCKABLE_TEXT void reduceBaselinePower(void) {
    int i;

    uint32_t upa_clocks_to_turn_off = ~0;
    uint32_t mss_clocks_to_turn_off = ~(0
            | DEV_MSS_APB_SLV   //
            | DEV_MSS_APB2_CTRL // These 3 clocks can't be turned off, only reset asserted
            | DEV_MSS_RTBRIDGE  //
            );
    uint32_t aux_clocks_to_turn_off = ~(0
            | AUX_CLK_MASK_UART
            );
    u64 css_clocks_to_turn_off = ~(0
            | DEV_CSS_LOS
            | DEV_CSS_LOS_L2C
            | DEV_CSS_LOS_ICB
            | DEV_CSS_LOS_DSU
            | DEV_CSS_LOS_TIM
            | DEV_CSS_CPR
            | DEV_CSS_AON
            | DEV_CSS_LAHB2SHB
            | DEV_CSS_SAHB2MAHB
            | DEV_CSS_LAHB_CTRL
            | DEV_CSS_APB4_CTRL
            | DEV_CSS_MAHB_CTRL
            | DEV_CSS_APB1_CTRL
            | DEV_CSS_SAHB_CTRL
            | DEV_CSS_UART
            | DEV_CSS_JTAG
            | DEV_CSS_GPIO
            | DEV_CSS_LOS_L2C
            );

    // Turn off unneeded clocks
    DrvCprSysDeviceAction(UPA_DOMAIN,     DISABLE_CLKS, upa_clocks_to_turn_off);
    DrvCprSysDeviceAction(UPA_DOMAIN,     ASSERT_RESET, upa_clocks_to_turn_off);
    DrvCprSysDeviceAction(MSS_DOMAIN,     DISABLE_CLKS, mss_clocks_to_turn_off);
    DrvCprSysDeviceAction(MSS_DOMAIN,     ASSERT_RESET, mss_clocks_to_turn_off);
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, DISABLE_CLKS, aux_clocks_to_turn_off);
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, ASSERT_RESET, aux_clocks_to_turn_off);
    DrvCprSysDeviceAction(CSS_DOMAIN,     DISABLE_CLKS, css_clocks_to_turn_off);

    // Disable all the SHAVE and USB ISLANDS
    for (i=0;i<12;i++)
        DrvCprPowerTurnOffIsland(POWER_ISLAND_SHAVE_0 + i);

    DrvCprPowerTurnOffIsland(POWER_ISLAND_USB);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_CPU);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_AMC);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_SIPP);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_PMB);
}

//mirror a specific area of the DDR inside the L2 cache and lock it (no invalidation/flushing will happen further);
//the mirrored area will contain grouped code and data (according to the ldscript) needed for the low power process.
// precondition: base_end > base_start
static LOCKABLE_TEXT void lockMemoryContentsIntoL2Cache(uint32_t base_start, uint32_t base_end) {
    const int required_ways = 1 + (base_end - base_start - 1) / L2CACHE_WAY_SIZE;
    uint32_t base_address = base_start;
    int i;

    assert(base_start <= base_end && required_ways <= 4);

    locked_power_recovery_info.l2c_ctrl = GET_REG_WORD_VAL(L2C_LEON_OS_CONTROL);

    // if L2 is enabled, flush it before locking; otherwise invalidate it before enabling it
    if ((locked_power_recovery_info.l2c_ctrl & LL2C_CTRL_EN) == 0)
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, 0);
    else
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE_AND_WRITE_BACK, 0);

    // Enable the L2Cache with a COPY_BACK policy but configure for 4 locked ways
    SET_REG_WORD(L2C_LEON_OS_CONTROL, LL2C_CTRL_EN |
                                      LL2C_CTRL_WRITE_POLICY_COPY_BACK |
                                      LL2C_CTRL_REPL__MASTER_INDEX_MODULUS |
                                      LL2C_CTRL_NR_LOCKED_WAYS(required_ways));

    for (i=0; i<required_ways; i++) {
        // Iterate through each way and preload the cache memory with the contents of base_address up to base_address + 256KB
        DrvLL2CFlushOpOnWayAndTag(LL2C_OPERATION_WAY_AND_TAG_UPDATE_VALID_DIRTY_BITS_AND_WRITE_BACK,
                                  CLEAR_CACHE_DISABLE,
                                  0,
                                  base_address,
                                  FETCH_LINES,
                                  SET_VALID,
                                  CLEAR_DIRTY);
        base_address += L2CACHE_WAY_SIZE;
    }

    // Finally perform a read to force the application stall until the L2Cache has finished its preload operations
    // Otherwise there is a potential the that subsequent code is already in the L1 ICache and as such it could be run
    // This can be a problem if (for example) we are about to disable the DDR before the L2 has a chance to complete its operations
    GET_REG_WORD_VAL(base_address);

    return;
}

//unlock and restore the initial L2cache configuration
// Assumptions: This function assumes the L2Cache is in a locked state on entry
static LOCKABLE_TEXT void disableCacheLocking(void) {
    // Reconfigure the cache as it was initially
    SET_REG_WORD(L2C_LEON_OS_CONTROL, locked_power_recovery_info.l2c_ctrl);
    DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, 0);
}

//set the clock used while in LP mode to either the 32Khz source or the a fraction of the oscin
static LOCKABLE_TEXT void setupLpClock(void) {
    // Always bypass the PLLs
    DrvCprPllBypass(BYPASS_ENABLE, BYPASS_ENABLE);

    ///disable the PLLs after bypassing them
    SET_REG_WORD(CPR_PLL_CTRL0_ADR, locked_power_recovery_info.cpr_pll_ctrl0 | 1); // Disable PLL0
    SET_REG_WORD(CPR_PLL_CTRL1_ADR, locked_power_recovery_info.cpr_pll_ctrl1 | 1); // Enable  PLL1

    if (low_power_data.clock.source == LP_SRC_32KHZ_CLK) {
        //Enable the 32khz clock
        HglCprAuxClocksEnable(&aux_clk_32_khz[0]);

        // Switch to 32 Khz clock
        SET_REG_BITS_MASK(AON_RETENTION0_ADR, 1 << 21);  // enable the 32khz oscillator
        SET_REG_BITS_MASK(AON_RETENTION0_ADR, 1 <<  6);  // switch to using the 32 kHz osc
        CLR_REG_BITS_MASK(AON_RETENTION0_ADR, 1 << 15);  // disable osc0 (delayed)
        SET_REG_BITS_MASK(AON_RETENTION0_ADR, 1 <<  5);  // control osc0 enable from bit 15 (acting)
    } else if (low_power_data.clock.source == LP_SRC_SYS_CLK) {
        //if the application needs a fraction of the oscin clk
        if ((low_power_data.clock.data.sys.denominator != 1) || (low_power_data.clock.data.sys.numerator != 1)) {
            // Clock division must be a minimum of 50%
            if ((low_power_data.clock.data.sys.numerator << 1 ) <= low_power_data.clock.data.sys.denominator )
                SET_REG_WORD(CPR_CLK_DIV_ADR, (low_power_data.clock.data.sys.numerator<<16) |
                                              low_power_data.clock.data.sys.denominator);
        }
    }
}

//the wakeup ISR, called from the component specific trap table;
//it firstly restore the clock to be at least oscin, then it continue with the restoration process
LOCKABLE_TEXT void wakeup_from_interrupt(void) {
    if (low_power_data.clock.source == LP_SRC_32KHZ_CLK) {
        // Immediately after waking up we restore the clock from 32Khz to oscin
        SET_REG_BITS_MASK(AON_RETENTION0_ADR, 1 << 15); // Enable osc0
        CLR_REG_BITS_MASK(AON_RETENTION0_ADR, 1 <<  6); // Switch back the PLL0 source to osc0
        CLR_REG_BITS_MASK(AON_RETENTION0_ADR, 1 << 21); // clear the 32khz oscillator
    }

    // unsetup GPIO IRQ; also clear in ICB
    icbUnsetupIrq(IRQ_GPIO_0);

    // wakeup_from_interrupt is called by the trap handler due to our wakeup source.
    // When IRQ occurs, LEON automatically disables traps, thus we have to re-enable them
    pwr_manager_disable_interrupts();
    swcLeonEnableTraps();

    powerStateExitLowPower();

    // NEVER RETURNS: the default return from this function will never be executed,
    // as long as the initial stack will be restored
    asm volatile("nop; nop;");
}


// restoring all the initial configuration of the active state
LOCKABLE_TEXT void powerStateExitLowPower(void) {
    uint32_t  pll0_lock;
    uint32_t  pll1_lock;
    int  i;

    //Restore the initial sys clock divider
    SET_REG_WORD(CPR_CLK_DIV_ADR, locked_power_recovery_info.cpr_clk_div);

    // Next we restart the PLL so that the remainder of the recovery can be done at full speed
    SET_REG_WORD(CPR_PLL_CTRL0_ADR, locked_power_recovery_info.cpr_pll_ctrl0); // Configure PLL0 as per its original settings
    SET_REG_WORD(CPR_PLL_CTRL1_ADR, locked_power_recovery_info.cpr_pll_ctrl1); // Configure PLL1 as per its original settings
    // Wait for both PLL's to lock
    do {
        pll0_lock = GET_REG_WORD_VAL(CPR_PLL_STAT0_ADR) & PLL_LOCKED_MASK;
        pll1_lock = GET_REG_WORD_VAL(CPR_PLL_STAT1_ADR) & PLL_LOCKED_MASK;
    } while ( ! (pll0_lock && pll1_lock));

    // Then take the PLLs out of bypass
    CLR_REG_BITS_MASK(CPR_CLK_BYPASS_ADR, 1 << 0);  // Engage PLL0
    CLR_REG_BITS_MASK(CPR_CLK_BYPASS_ADR, 1 << 1);  // Engage PLL1

    //Disable the 32khz clock
    HglCprAuxClocksEnable(&aux_clk_32_khz[1]);

    // This is needed for some boards, while others work fine -- we have to investigate why this is required
    DrvCprSysDeviceAction(CSS_DOMAIN, ENABLE_CLKS, DEV_CSS_UPA_MAS | DEV_CSS_APB3_CTRL);

    // Restore the GPIO Config
    // This state is application specific so commented out here
    // to illustrate this fact.
    // DrvGpioInitialiseRange(gpioActiveCfg);

    // Re-enable power Islands
    DrvCprPowerTurnOnIsland(POWER_ISLAND_PMB);
    DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_CPU);
    DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_SIPP);
    DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_AMC);
    DrvCprPowerTurnOnIsland(POWER_ISLAND_USB);

    for (i=0;i<12;i++)
        DrvCprPowerTurnOnIsland(POWER_ISLAND_SHAVE_0 + i);

    // Take DDR out of self refresh and re-enable it
    DrvDdrStartUpInSelfRefresh(NULL);
    DrvDdrExitSelfRefresh();

    //Disable the L2Cache lock configuration and invalidate the L2Cache
    disableCacheLocking();
    //starting with this point, the LP stack is not accessible anymore, if L2C in write-through,
    //so don't trigger any win underflow until changing back to RTEMS stack

    //inlined restore of the initial RTEMS stack and registers
    restoreStackAndExitLp_asm();

    // NEVER RETURNS: the default return from this function will never be executed,
    // as long as the initial stack will be restored
    asm volatile("nop; nop;");
}

//this function and all subsequent runs using a special stack layed into the L2cache lockable area of the DDR;
//it locks the low power interest region of the DDR into L2Cache, then continues with running code from L2cache:
//it puts the DDR into self refresh, disable all unneeded clocks and islands,
//then setup the clock to a lower value and enters LOS sleep
LOCKABLE_TEXT void lowPowerEntersIsolated(void) {
    lockMemoryContentsIntoL2Cache((uint32_t)&_LOCKING_AREA_START, (uint32_t)&_LOCKING_AREA_END );

    // Put DDR in Self Refresh and put DRAM controller in sleep state
    DrvDdrEnterSelfRefresh();
    DrvDdrGoIntoLowPowerDataRetention();

    // Disable unused clocks and disable power islands
    reduceBaselinePower();

    //Enable the wakeup by GPIO
    if (low_power_data.wakeup.source == LP_WAKEUP_SOURCE_GPIO)
        configureGpioForWakeup(low_power_data.wakeup.data.gpio.number);

    //switch to a lower clock speed
    setupLpClock();

    pwr_manager_enable_interrupts();

    // Put the LeonOS processor into sleep state
    // will be woken by the chosen IRQ
    asm volatile("wr %g0, %asr19; nop; nop; nop;");

    // NEVER RETURNS: the default return from this function will never be executed,
    // as long as when we wakeup from LP, the initial stack will be restored
    asm volatile("nop; nop;");
}

// Entry point for low power mode
// RTEMS threads switching is disabled at this point and LP process use
// baremetal drivers until we exit LP. after successfully finishing the system
// wakeup, the control is given again to RTEMS
void pwrManagerEnterLowPower(void) {
    DPRINTF(" > Entering low power mode...\n");

    backupAndStopInterrupts();
    saveHwContext();

#ifdef USE_SUSPEND_LRT
    extern struct {
        volatile uint32_t isSuspended;
        volatile uint32_t isResumed;
    }lrt_SuspendLrtState;
    while(lrt_SuspendLrtState.isSuspended != 0)
        asm volatile("nop; nop; nop; nop; nop"); //Delay to not overflow the read buffer
    DrvIcbIrqRemoteTrigger(IRQ_DYNAMIC_0);
    while(lrt_SuspendLrtState.isSuspended != 1)
        asm volatile("nop; nop; nop; nop; nop"); //Delay to not overflow the read buffer
#endif

    //switch from the active RTEMS stack to a L2cache lockable stack then it continues the steps
    //to put the system in LP (finally it calls the lowPowerEntersIsolated)
    switchStackAndEntersLp_asm();

    restoreHwContext();
    restoreInterrupts();

#ifdef USE_SUSPEND_LRT
    while(lrt_SuspendLrtState.isResumed != 0)
        asm volatile("nop; nop; nop; nop; nop"); //Delay to not overflow the read buffer
    resumeLrt();
    while(lrt_SuspendLrtState.isResumed != 1)
        asm volatile("nop; nop; nop; nop; nop"); //Delay to not overflow the read buffer
#endif

    DPRINTF(" < Exiting low power mode...\n");
}
