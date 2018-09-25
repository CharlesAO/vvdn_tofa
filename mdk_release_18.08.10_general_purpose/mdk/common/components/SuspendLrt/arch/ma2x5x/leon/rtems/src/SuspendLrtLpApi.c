///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Suspend LRT component: suspend/resume infustructure
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "SuspendLrtLpApi.h"
#include <DrvIcb.h>
#include <DrvLeonL2C.h>
#include <DrvRegUtils.h>
#include <bsp.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#if 0
#include <rtems/bspIo.h>
#include <stdio.h>
#define DPRINTF(...) printk(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

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
    // L2C CONFIG
    uint32_t l2c_ctrl;
    // TIMER_CONFIG
    uint32_t tim_gen_config;
    uint32_t tim_free_cnt0;
    uint32_t tim_free_cnt1;
    struct {
        uint32_t cnt;
        uint32_t reload;
        uint32_t config;
    } tim_cfg[8];
    // ICB_CONFIG
    uint32_t icb_config[64];
    uint32_t icb_enable_0;
    uint32_t icb_enable_1;
    uint32_t icb_sveint[12];
    uint32_t icb_inter;
    uint32_t icb_c2c_int_cfg[3];
} tyPowerStateRecoveryInfo;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
#define DDR_DIRECT __attribute__((section(".ddr_direct.data")))
DDR_DIRECT struct {
    volatile uint32_t isSuspended;
    volatile uint32_t isResumed;
} SuspendLrtState = {0, 0};

// 4: Static Local Data
// ----------------------------------------------------------------------------
static tyPowerStateRecoveryInfo power_recovery_info;

// 5: Function Prototypes
// ----------------------------------------------------------------------------
void powerStateExitLowPower(void);

// enable the IRQ generation in ICB
static inline __attribute__((always_inline)) void icbSetupIrq(uint32_t irq_src,
                                                              uint32_t priority,
                                                              uint32_t type) {
    uint32_t t = (irq_src & 0x20) ? 4 : 0;

    // Configure Interrupt and priority
    SET_REG_WORD(ICB1_BASE_ADR + (irq_src << 2), (priority << 2) | type);
    SET_REG_WORD(ICB1_ENABLE_0_ADR + t, 1 << (irq_src - t * 8));

    pwr_manager_enable_interrupts();
}

// disable the IRQ generation in ICB and clear it
static inline __attribute__((always_inline)) void icbUnsetupIrq(
    uint32_t irq_src) {
    uint32_t t = (irq_src & 0x20) ? 4 : 0;

    SET_REG_WORD(ICB1_ENABLE_0_ADR + t, 0);
    SET_REG_WORD(ICB1_CLEAR_0_ADR + t, 1 << (irq_src - t * 8));
    SET_REG_WORD(ICB1_BASE_ADR + (irq_src << 2), 0);

    pwr_manager_disable_interrupts();
}

// save the currently enabled interrupts then disable them all (including and
// especially the RTEMS tick interrupts), before entering low power
static void saveHwContext(void) {
    // Backup interrupts
    memcpy(power_recovery_info.icb_config, (void *)ICB1_CTRL0_ADR,
           sizeof(power_recovery_info.icb_config));
    power_recovery_info.icb_enable_0 = GET_REG_WORD_VAL(ICB1_ENABLE_0_ADR);
    power_recovery_info.icb_enable_1 = GET_REG_WORD_VAL(ICB1_ENABLE_1_ADR);
    memcpy(power_recovery_info.icb_sveint, (void *)ICB1_SVE0_INT_CFG_ADR,
           sizeof(power_recovery_info.icb_sveint));
    power_recovery_info.icb_inter = GET_REG_WORD_VAL(ICB1_INTER_CPU_INT_ADR);
    memcpy(power_recovery_info.icb_c2c_int_cfg, (void *)ICB1_C2C_INT_CFG0_ADR,
           sizeof(power_recovery_info.icb_c2c_int_cfg));

    // Stop all interrupts
    SET_REG_WORD(ICB1_ENABLE_0_ADR, 0);
    SET_REG_WORD(ICB1_ENABLE_1_ADR, 0);
    SET_REG_WORD(ICB1_CLEAR_0_ADR, 0xFFFFFFFF);
    SET_REG_WORD(ICB1_CLEAR_1_ADR, 0xFFFFFFFF);

    // Stop timers
    power_recovery_info.tim_gen_config = GET_REG_WORD_VAL(TIM1_GEN_CONFIG_ADR);
    SET_REG_WORD(TIM1_GEN_CONFIG_ADR, 0);

    // Backup Timer block
    power_recovery_info.tim_free_cnt1 = GET_REG_WORD_VAL(TIM1_FREE_CNT1_ADR);
    power_recovery_info.tim_free_cnt0 = GET_REG_WORD_VAL(TIM1_FREE_CNT0_ADR);
    for (uint32_t i = 0; i < 8; i++) {
        power_recovery_info.tim_cfg[i].cnt =
            GET_REG_WORD_VAL(TIM1_0_CNT_VAL_ADR + 0x10 * i);
        power_recovery_info.tim_cfg[i].reload =
            GET_REG_WORD_VAL(TIM1_0_RELOAD_VAL_ADR + 0x10 * i);
        power_recovery_info.tim_cfg[i].config =
            GET_REG_WORD_VAL(TIM1_0_CONFIG_ADR + 0x10 * i);
    }
}

// reenable the interrupts that were active before entering LP
static void restoreHwContext(void) {
    // Restore interrupts
    memcpy((void *)ICB1_CTRL0_ADR, power_recovery_info.icb_config,
           sizeof(power_recovery_info.icb_config));
    SET_REG_WORD(ICB1_ENABLE_0_ADR, power_recovery_info.icb_enable_0);
    SET_REG_WORD(ICB1_ENABLE_1_ADR, power_recovery_info.icb_enable_1);
    memcpy((void *)ICB1_SVE0_INT_CFG_ADR, power_recovery_info.icb_sveint,
           sizeof(power_recovery_info.icb_sveint));
    SET_REG_WORD(ICB1_INTER_CPU_INT_ADR, power_recovery_info.icb_inter);
    memcpy((void *)ICB1_C2C_INT_CFG0_ADR, power_recovery_info.icb_c2c_int_cfg,
           sizeof(power_recovery_info.icb_c2c_int_cfg));

    // Restore timers
    SET_REG_WORD(TIM1_FREE_CNT1_ADR, power_recovery_info.tim_free_cnt1);
    SET_REG_WORD(TIM1_FREE_CNT0_ADR, power_recovery_info.tim_free_cnt0);
    for (uint32_t i = 0; i < 8; i++) {
        SET_REG_WORD(TIM1_0_CNT_VAL_ADR + 0x10 * i,
                     power_recovery_info.tim_cfg[i].cnt);
        SET_REG_WORD(TIM1_0_RELOAD_VAL_ADR + 0x10 * i,
                     power_recovery_info.tim_cfg[i].reload);
        SET_REG_WORD(TIM1_0_CONFIG_ADR + 0x10 * i,
                     power_recovery_info.tim_cfg[i].config);
    }
    SET_REG_WORD(TIM1_GEN_CONFIG_ADR, power_recovery_info.tim_gen_config);
}

static void emptyL2Cache(void) {
    power_recovery_info.l2c_ctrl = GET_REG_WORD_VAL(L2C_LEON_RT_CONTROL);

    // if L2 is enabled, flush it; otherwise invalidate it
    if ((power_recovery_info.l2c_ctrl & LL2C_CTRL_EN) == 0)
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, 1);
    else
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE_AND_WRITE_BACK, 1);

    // Perform a read to force the application stall until the L2 has finished
    GET_REG_WORD_VAL(L2C_LEON_RT_CONTROL);

    return;
}

static void restore_caches(void) {
    uint32_t i;
    extern __BSP_L2_CACHE_CONFIG__ BSP_L2_CACHE_CONFIG;
    // L2C is disabled by default, apply configuration only if we enable it
    if (BSP_L2_CACHE_CONFIG.enable) {
        for (i = 0; i < BSP_L2_CACHE_CONFIG.mtrrCount; i++)
            cache_l2c_program_mtrr(BSP_L2_CACHE_CONFIG.mtrr[i].address,
                                   BSP_L2_CACHE_CONFIG.mtrr[i].mask,
                                   BSP_L2_CACHE_CONFIG.mtrr[i].accessMode,
                                   BSP_L2_CACHE_CONFIG.mtrr[i].writeProtected,
                                   i);
        cache_l2c_enable(BSP_L2_CACHE_CONFIG.replacementPolicy,
                         BSP_L2_CACHE_CONFIG.locked_ways,
                         BSP_L2_CACHE_CONFIG.mode);
    }

    // L1C is enabled by default, disable it only if required by configuration
    if (!BSP_L1_CACHE_CONFIG.enableDCache) cache_l1c_disable_data();

    if (!BSP_L1_CACHE_CONFIG.enableICache) cache_l1c_disable_instruction();
}

// the wakeup ISR, called from the component specific trap table;
// it firstly restore the clock to be at least oscin, then it continue with the
// restoration process
void wakeup_from_interrupt(void) {
    // unsetup GPIO IRQ; also clear in ICB
    icbUnsetupIrq(LP_INTERRUPT_NUM);

    // wakeup_from_interrupt is called by the trap handler. We have to enable
    // traps because LEON automatically disables them when a trap occurs
    swcLeonEnableTraps();

    // Restore cache configuration
    restore_caches();

    // inlined restore of the initial RTEMS stack and registers
    restoreStackAndExitLp_asm();

    // NEVER RETURNS: the default return from this function will never be
    // executed,
    // as long as the initial stack will be restored
    asm volatile("nop; nop;");
}

// this function and all subsequent runs using a special stack layed into the
// L2cache lockable area of the DDR;
// it locks the low power interest region of the DDR into L2Cache, then
// continues with running code from L2cache:
// it puts the DDR into self refresh, disable all unneeded clocks and islands,
// then setup the clock to a lower value and enters LOS sleep
void SuspendLrtIsolated(void) {
    emptyL2Cache();

    icbSetupIrq(LP_INTERRUPT_NUM, LP_INTERRUPT_PRIORITY, POS_EDGE_INT);

    SuspendLrtState.isResumed = 0;
    SuspendLrtState.isSuspended = 1;

    // Put the processor into sleep state will be woken by the chosen IRQ
    // THIS FUNCTION NEVER RETURNS; re-entry point: wakeup_from_interrupt
    asm volatile("wr %g0, %asr19; nop; nop; nop; nop; nop");
}

// Entry point for low power mode
// Assumptions: the LP mode is to be offered to RTEMS applications
// after starting the enter LP process, the RTEMS threads switching is disabled
// and the LP process use baremetal drivers
// until the LP is achieved and until LP is exited;
// after successfully finishing the system wakeup, the control is given again to
// RTEMS
void SuspendLrt(void) {
    DPRINTF(" > Suspending Leon RT...\n");

    saveHwContext();

    // switch from the active RTEMS stack to a temp stack, then continue the
    // steps to put the system in LP (finally it calls SuspendLrtIsolated)
    switchStackAndEntersLp_asm();

    restoreHwContext();

    SuspendLrtState.isSuspended = 0;
    SuspendLrtState.isResumed = 1;

    if ((power_recovery_info.l2c_ctrl & LL2C_CTRL_EN) == 1)
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_WRITE_BACK, 0);

    DPRINTF(" < Resuming Leon RT...\n");
}
