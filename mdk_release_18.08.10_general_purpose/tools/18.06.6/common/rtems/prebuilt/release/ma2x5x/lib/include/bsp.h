/*  bsp.h
 *
 *  This include file contains all SPARC simulator definitions.
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 *
 *  Ported to ERC32 implementation of the SPARC by On-Line Applications
 *  Research Corporation (OAR) under contract to the European Space
 *  Agency (ESA).
 *
 *  ERC32 modifications of respective RTEMS file: COPYRIGHT (c) 1995.
 *  European Space Agency.
 *
 *  Modified for MYRIAD2 BSP.
 *  COPYRIGHT (c) 2013-2016.
 *  Movidius.
 */

#ifndef LIBBSP_MYRIAD2_BSP_H
#define LIBBSP_MYRIAD2_BSP_H

#include <bspopts.h>
#include <bsp/default-initial-extension.h>

#include <rtems.h>
#include <myriad2.h>
#include <rtems/irq-extension.h>
#include <icb_defines.h>
#include <bsp_custom.h>

#ifdef __cplusplus
extern "C" {
#endif

// Structure to share hardware parameters with the BSP
typedef struct
{
    uint32_t ref0ClockKhz;              /*< Reference oscillator used */
    uint32_t pll0TargetFrequencyKhz;    /*< PLL0 Target Frequency */
    uint32_t currentSysClockKhz;        /*< Current System Clock  */
    uint64_t cssClocks;                 /*< CSS Clocks */
    uint32_t mssClocks;                 /*< MSS Clocks */
    uint32_t upaClocks;                 /*< UPA Clocks */
    uint32_t sippClocks;                /*< SIPP Clocks */
    uint32_t auxClocks;                 /*< AUX Clocks */
    uint16_t masterDividerNumerator;    /*< Master Divider Numerator */
    uint16_t masterDividerDenominator;  /*< Master Divider Denominator */
}__BSP_CLOCK_CONFIG__;

// Macro utility to make it easier to program the system clocks
#define BSP_SET_CLOCK(_refClock, _pll0TargetFreq, _masterNum, _masterDen, _cssClocks, _mssClocks, _upaClocks, _sippClocks, _auxClocks) \
__BSP_CLOCK_CONFIG__ BSP_CLOCK_CONFIG = { \
    .ref0ClockKhz = _refClock, \
    .pll0TargetFrequencyKhz = _pll0TargetFreq, \
    .currentSysClockKhz = _refClock, \
    .cssClocks  = _cssClocks, \
    .mssClocks  = _mssClocks, \
    .upaClocks  = _upaClocks, \
    .sippClocks = _sippClocks, \
    .auxClocks  = _auxClocks, \
    .masterDividerNumerator = _masterNum, \
    .masterDividerDenominator = _masterDen \
};

// Structure to program L2C MTRR registers
typedef struct
{
    uint32_t address; /*< Address to trigger the behaviour */
    uint32_t mask; /*< Mask for the address */
    uint8_t accessMode; /*< Either uncached or write-through */
    uint8_t writeProtected; /*< Either protected or not */
}__BSP_L2_CACHE_MTRR_REGISTER__;

// Structure to control the behaviour of L2C
typedef struct
{
    __BSP_L2_CACHE_MTRR_REGISTER__ *mtrr; /*< Array of MTRR configuration */
    uint8_t mtrrCount; /*< Number of MTRR registers to program */
    uint8_t enable; /*< Enable (1) / Disable (0) */
    uint8_t replacementPolicy; /*< Either L2C_REPL_LRU, L2C_REPL_PSEUDO_RANDOM, L2C_REPL_MASTER_INDEX_REP or L2C_REPL_MASTER_INDEX_MOD */
    uint8_t locked_ways; /*< Cache ways to be locked in Leon L2 cache - please refer to Myriad datasheet if planning to use more than 0 here*/
    uint8_t mode; /*< Either L2C_MODE_COPY_BACK or L2C_MODE_WRITE_THROUGH */
} __BSP_L2_CACHE_CONFIG__;

#define BSP_SET_L2C_CONFIG(_enable, _repl, _locked_ways, _mode, _count, _mtrrArray) \
__BSP_L2_CACHE_CONFIG__ BSP_L2_CACHE_CONFIG = { \
    .mtrr = (__BSP_L2_CACHE_MTRR_REGISTER__ *) _mtrrArray, \
    .mtrrCount = _count, \
    .enable = _enable, \
    .replacementPolicy = _repl, \
    .locked_ways = _locked_ways, \
    .mode = _mode \
};

// Structure to control the behaviour of L1C
typedef struct
{
    uint8_t enableDCache; /*< Enable (1) / Disable (0) */
    uint8_t enableICache; /*< Enable (1) / Disable (0) */
} __BSP_L1_CACHE_CONFIG__;

#define BSP_SET_L1C_CONFIG(_enableD, _enableI) \
__BSP_L1_CACHE_CONFIG__ BSP_L1_CACHE_CONFIG = { \
    .enableDCache = _enableD, \
    .enableICache = _enableI, \
};

// Standard Replacement Policy
#define DEFAULT_RTEMS_L2C_REPLACEMENT_POLICY    L2C_REPL_LRU
// Standard Ways
#define DEFAULT_RTEMS_L2C_LOCKED_WAYS           0
// Standard L2C Mode
#define DEFAULT_RTEMS_L2C_MODE                  L2C_MODE_COPY_BACK

/* System variant: MYRIAD2 */
#define MYRIAD2 1

/* The interrupt priority that the timer tick will have, 1 (low) - 15 (high) */
#define TICK_ISR_PRIORITY 7

/* BSP provides its own Idle thread body */
void *bsp_idle_thread( uintptr_t ignored );

#define BSP_IDLE_TASK_BODY bsp_idle_thread

/* Maximum supported APBUARTs by BSP */
#define BSP_NUMBER_OF_TERMIOS_PORTS 1

/* Configure GRETH driver */
#define GRETH_SUPPORTED

extern int CPU_SPARC_HAS_SNOOPING;

/* Constants */

/*
 *  Information placed in the linkcmds file.
 */

extern int   RAM_START;
extern int   RAM_END;
extern int   RAM_SIZE;

extern int   PROM_START;
extern int   PROM_END;
extern int   PROM_SIZE;

extern int   CLOCK_SPEED;

extern int   end;        /* last address in the program */

/* Index of max interrupt in ICB */
#define MYRIAD_IRQ_MAX_INDEX        (TOTAL_LEON_IRQS - 1)
#define MYRIAD_NUM_INT_LEVELS       NUM_INT_LEVELS

/* set_vec type */
#define SET_VECTOR_RAW  0  /* Raw trap handler */
#define SET_VECTOR_INT  1  /* Trap handler with _ISR_Handler interrupt handler */

rtems_isr_entry set_vector(                     /* returns old vector */
    rtems_isr_entry     handler,                /* isr routine        */
    rtems_vector_number vector,                 /* vector number      */
    int                 type                    /* RTEMS or RAW intr  */
);

void BSP_fatal_exit(uint32_t error);

void bsp_spurious_initialize( void );

/* Interrupt Service Routine (ISR) pointer */
typedef void (*bsp_shared_isr)(void *arg);

/* Initializes the Shared System Interrupt service */
extern void BSP_shared_interrupt_init(void);

/* Called directly from IRQ trap handler TRAP[0x10..0x1F] = IRQ[0..15] */
void bsp_isr_handler(rtems_vector_number vector, CPU_Interrupt_frame *isf, uint32_t irq);

/* Starts PLL0 and enable relevant (user defined) clocks */
extern void bsp_start_clocks(void);

/* chip specific bsp startup */
extern void bsp_custom_start(uint32_t asr17);

/* Registers a IRQ handler, and enable it at IRQ controller.
 *
 * Arguments
 *  irq       System IRQ number
 *  info      Optional Name of IRQ source
 *  isr       Function pointer to the ISR
 *  arg       Second argument to function isr
 */
static __inline__ int BSP_interrupt_register(int irq, const char *info, rtems_interrupt_handler isr, void *arg)
{
    return rtems_interrupt_handler_install(irq, info, RTEMS_INTERRUPT_UNIQUE,   isr, arg);
}

/* Unregister previously registered IRQ handler.
 *
 * Arguments
 *  irq       System IRQ number
 *  isr       Function pointer to the ISR
 *  arg       Second argument to function isr
 */
static __inline__ int BSP_interrupt_unregister(int irq, rtems_interrupt_handler isr, void *arg)
{
    return rtems_interrupt_handler_remove(irq, isr, arg);
}

/* Registers a shared IRQ handler, and enable it at IRQ controller. Multiple
 * interrupt handlers may use the same IRQ number, all ISRs will be called
 * when an interrupt on that line is fired.
 *
 * Arguments
 *  irq       System IRQ number
 *  info      Optional Name of IRQ source
 *  isr       Function pointer to the ISR
 *  arg       Second argument to function isr
 */
static __inline__ int BSP_shared_interrupt_register(int irq, const char *info, bsp_shared_isr isr, void *arg)
{
       return rtems_interrupt_handler_install(irq, info, RTEMS_INTERRUPT_SHARED, isr, arg);
}

/* Unregister previously registered shared IRQ handler.
 *
 * Arguments
 *  irq       System IRQ number
 *  isr       Function pointer to the ISR
 *  arg       Second argument to function isr
 */
static __inline__ int BSP_shared_interrupt_unregister(int irq, bsp_shared_isr isr, void *arg)
{
       return rtems_interrupt_handler_remove(irq, isr, arg);
}

/* Clear interrupt pending on IRQ controller, this is typically done on a
 * level triggered interrupt source such as PCI to avoid taking double IRQs.
 * In such a case the interrupt source must be cleared first on LEON, before
 * acknowledging the IRQ with this function.
 *
 * Arguments
 *  irq       System IRQ number
 */
extern void BSP_shared_interrupt_clear(int irq);

/* Enable Interrupt. This function will unmask the IRQ at the interrupt
 * controller. This is normally done by _register(). Note that this will
 * affect all ISRs on this IRQ.
 *
 * Arguments
 *  irq       System IRQ number
 */
extern void BSP_shared_interrupt_unmask(int irq);

/* Disable Interrupt. This function will mask one IRQ at the interrupt
 * controller. This is normally done by _unregister().  Note that this will
 * affect all ISRs on this IRQ.
 *
 * Arguments
 *  irq         System IRQ number
 */
extern void BSP_shared_interrupt_mask(int irq);

/* BSP PCI Interrupt support */
#define BSP_PCI_shared_interrupt_register    BSP_shared_interrupt_register
#define BSP_PCI_shared_interrupt_unregister  BSP_shared_interrupt_unregister
#define BSP_PCI_shared_interrupt_unmask      BSP_shared_interrupt_unmask
#define BSP_PCI_shared_interrupt_mask        BSP_shared_interrupt_mask
#define BSP_PCI_shared_interrupt_clear       BSP_shared_interrupt_clear

#ifdef __cplusplus
}
#endif

#include <libcpu/cache_.h>

#endif
