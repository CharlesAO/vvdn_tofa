/**
 * @file
 * @ingroup sparc_leon3
 * @brief LEON3 BSP data types and macros
 */

/*  leon.h
 *
 *  BASED on LEON3 BSP data types and macros.
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Modified for LEON3 BSP.
 *  COPYRIGHT (c) 2004.
 *  Gaisler Research.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 *
 *  Modified for MYRIAD2 BSP.
 *  COPYRIGHT (c) 2013-2018.
 *  Movidius.
 *
 */
#ifndef _LIBBSP_MYRIAD2_H_
#define _LIBBSP_MYRIAD2_H_

#include <rtems.h>
#include <rtems/score/sparc.h>
#include <myriad2_registers.h>
#include <reg_utils_defines.h>
#include <icb_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MYRIAD2_INTERRUPT_EXTERNAL_1 5

#ifndef ASM
/*
 *  Trap Types for on-chip peripherals
 *
 *  Source: Table 8 - Interrupt Trap Type and Default Priority Assignments
 *
 *  NOTE: The priority level for each source corresponds to the least
 *        significant nibble of the trap type.
 */

#define MYRIAD2_TRAP_TYPE( _source ) SPARC_ASYNCHRONOUS_TRAP((_source) + 0x10)

#define MYRIAD2_TRAP_SOURCE( _trap ) ((_trap) - 0x10)

#define MYRIAD2_INT_TRAP( _trap ) \
  ( (_trap) >= 0x11 && \
    (_trap) <= 0x1F )

#endif

/*
 *  The following defines the bits in the Timer Control Register.
 */
#define MYRIAD_TIMER_EN         (1<<0)
#define MYRIAD_TIMER_RS         (1<<1)
#define MYRIAD_TIMER_IRQEN      (1<<2)
#define MYRIAD_TIMER_CH         (1<<3)
#define MYRIAD_TIMER_IRQPEND    (1<<4)
#define MYRIAD_TIMER_FR         (1<<5)

/*
 *  The following defines the bits in the MYRIAD2 UART Status Register.
 */

#define D_UART_USR_TFNF  (1<<1) // Transmit FIFO Not Full
#define D_UART_USR_RFNE  (1<<3) // Receive FIFO Not Empty

/*
 *  The following defines the bits in the MYRIAD2 UART Control Register.
 */
#define D_UART_MCR_LB    (1<<4) // LoopBack Bit - Modem control

/* Calculate the ICB byte offset we want to access:
 * source >> 5 is the n-th word in the bit array
 */
#define GET_SOURCE_OFFSET(source) (((source) >> 5) << 2 )

void bsp_debug_uart_init(void);

/*
 *  The following defines the bits in the MYRIAD2 Cache Control Register.
 */

/*
 * To determine which ICB and TIM modules should be used by the processor
 */
extern uint32_t __icb_base__;
extern uint32_t __tim_base__;


#ifndef ASM

void myriad2_power_down_loop(void) RTEMS_NO_RETURN;

/*
 * Allows the calle to notify RTEMS that about a system frequency change
 */
void myriad2_frequency_changed(void);

/*
 *  Macros to manipulate the Interrupt Clear, Interrupt Force, Interrupt Mask,
 *  and the Interrupt Pending Registers.
 *
 *  NOTE: For operations which are not atomic, this code disables interrupts
 *        to guarantee there are no intervening accesses to the same register.
 *        The operations which read the register, modify the value and then
 *        store the result back are vulnerable.
 */

extern rtems_interrupt_lock LEON_IrqCtrl_Lock;

#define LEON_IRQCTRL_ACQUIRE( _lock_context ) \
  rtems_interrupt_lock_acquire( &LEON_IrqCtrl_Lock, _lock_context )

#define LEON_IRQCTRL_RELEASE( _lock_context ) \
  rtems_interrupt_lock_release( &LEON_IrqCtrl_Lock, _lock_context )

#if (__myriad2__ >= 3)
#define LEON_IRQCTRL_SYNC(_base) \
    do {                         \
        GET_REG_WORD_VAL(_base); \
    } while (0)
#else
#define LEON_IRQCTRL_SYNC(_base) \
    do {                         \
    } while (0)
#endif

#define LEON_IRQCTRL_WRITE_ICB(_base, _offset, _value) \
  do {                                               \
    SET_REG_WORD(_base + _offset, _value);         \
    LEON_IRQCTRL_SYNC(_base);                      \
  } while (0)

#define LEON_Clear_interrupt( _source ) \
  do { \
    uint32_t _t = GET_SOURCE_OFFSET(_source); \
    uint32_t _mask = 1LU << ((_source) - (_t * 8LU)); \
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_CLEAR_0_OFFSET + _t, _mask); \
  } while (0)

#define LEON_Force_interrupt( _source ) \
  do { \
    uint32_t _t = GET_SOURCE_OFFSET(_source); \
    uint32_t _mask = 1LU << ((_source) - (_t * 8LU)); \
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_SETINT_0_OFFSET + _t, _mask); \
  } while (0)

#define LEON_Is_interrupt_pending( _source ) \
  ({ \
    uint32_t _t = GET_SOURCE_OFFSET(_source); \
    uint32_t _mask = 1LU << ((_source) - (_t * 8LU)); \
    GET_REG_WORD_VAL( __icb_base__ + ICB_PEND_0_OFFSET + _t ) & _mask; \
  })

#define LEON_Cpu_Is_interrupt_masked( _source, _cpu ) \
  ({ \
    uint32_t _t = GET_SOURCE_OFFSET(_source); \
    uint32_t _mask = 1LU << ((_source) - (_t * 8LU)); \
    ( GET_REG_WORD_VAL( __icb_base__ + ICB_ENABLE_0_OFFSET + _t ) & _mask ) ? 0 : 1; \
  })

#define LEON_Cpu_Mask_interrupt( _source, _cpu ) \
  do { \
    rtems_interrupt_lock_context _lock_context; \
    uint32_t _t = GET_SOURCE_OFFSET(_source); \
    uint32_t _mask = 1LU << ((_source) - (_t * 8LU)); \
    LEON_IRQCTRL_ACQUIRE( &_lock_context ); \
    uint32_t _value = GET_REG_WORD_VAL( __icb_base__ + ICB_ENABLE_0_OFFSET + _t ); \
    _value &= ~_mask; \
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_ENABLE_0_OFFSET + _t, _value); \
    LEON_IRQCTRL_RELEASE( &_lock_context ); \
  } while (0)

#define LEON_Cpu_Unmask_interrupt( _source, _cpu ) \
  do { \
    rtems_interrupt_lock_context _lock_context; \
    uint32_t _t = GET_SOURCE_OFFSET(_source); \
    uint32_t _mask = 1LU << ((_source) - (_t * 8LU)); \
    LEON_IRQCTRL_ACQUIRE( &_lock_context ); \
    uint32_t _value = GET_REG_WORD_VAL( __icb_base__ + ICB_ENABLE_0_OFFSET + _t ); \
    _value |= _mask; \
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_ENABLE_0_OFFSET + _t, _value); \
    LEON_IRQCTRL_RELEASE( &_lock_context ); \
  } while (0)

#define LEON_Cpu_Disable_interrupt( _source, _previous, _cpu ) \
  do { \
    rtems_interrupt_lock_context _lock_context; \
    uint32_t _t = GET_SOURCE_OFFSET(_source); \
    uint32_t _mask = ( 1LU << ( (_source) - ( _t * 8LU ) ) ); \
    LEON_IRQCTRL_ACQUIRE( &_lock_context ); \
    uint32_t _value = GET_REG_WORD_VAL( __icb_base__ + ICB_ENABLE_0_OFFSET + _t ); \
    _value &= ~_mask; \
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_ENABLE_0_OFFSET + _t, _value); \
    LEON_IRQCTRL_RELEASE( &_lock_context ); \
    (_previous) &= _mask; \
  } while (0)

#define LEON_Cpu_Restore_interrupt( _source, _previous, _cpu ) \
  do { \
    rtems_interrupt_lock_context _lock_context; \
    uint32_t _t = GET_SOURCE_OFFSET(_source); \
    uint32_t _mask = ( 1LU << ( (_source) - ( _t * 8LU ) ) ); \
    LEON_IRQCTRL_ACQUIRE( &_lock_context ); \
    uint32_t _value = GET_REG_WORD_VAL( __icb_base__ + ICB_ENABLE_0_OFFSET + _t ); \
    _value = (_value & ~mask) | (_previous); \
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_ENABLE_0_OFFSET + _t, _value); \
    LEON_IRQCTRL_RELEASE( &_lock_context ); \
  } while (0)

/* Map single-cpu operations to local CPU */
#define LEON_Is_interrupt_masked( _source ) \
  LEON_Cpu_Is_interrupt_masked(_source, 0)

#define LEON_Mask_interrupt(_source) \
  LEON_Cpu_Mask_interrupt(_source, 0)

#define LEON_Unmask_interrupt(_source) \
  LEON_Cpu_Unmask_interrupt(_source, 0)

#define LEON_Disable_interrupt(_source, _previous) \
  LEON_Cpu_Disable_interrupt(_source, _previous, 0)

#define LEON_Restore_interrupt(_source, _previous) \
  LEON_Cpu_Restore_interrupt(_source, _previous, 0)

/* Make BSPs have common macros for interrupt handling */
#define BSP_Clear_interrupt(_source) \
  LEON_Clear_interrupt(_source)
#define BSP_Force_interrupt(_source) \
  LEON_Force_interrupt(_source)
#define BSP_Is_interrupt_pending(_source) \
  LEON_Is_interrupt_pending(_source)
#define BSP_Is_interrupt_masked(_source) \
  LEON_Is_interrupt_masked(_source)
#define BSP_Unmask_interrupt(_source) \
  LEON_Unmask_interrupt(_source)
#define BSP_Mask_interrupt(_source) \
  LEON_Mask_interrupt(_source)
#define BSP_Disable_interrupt(_source, _previous) \
  LEON_Disable_interrupt(_source, _prev)
#define BSP_Restore_interrupt(_source, _previous) \
  LEON_Restore_interrupt(_source, _previous)

/* Make BSPs have common macros for interrupt handling on any CPU */
#define BSP_Cpu_Is_interrupt_masked(_source, _cpu) \
  LEON_Cpu_Is_interrupt_masked(_source, _cpu)
#define BSP_Cpu_Unmask_interrupt(_source, _cpu) \
  LEON_Cpu_Unmask_interrupt(_source, _cpu)
#define BSP_Cpu_Mask_interrupt(_source, _cpu) \
  LEON_Cpu_Mask_interrupt(_source, _cpu)
#define BSP_Cpu_Disable_interrupt(_source, _previous, _cpu) \
  LEON_Cpu_Disable_interrupt(_source, _prev, _cpu)
#define BSP_Cpu_Restore_interrupt(_source, _previous, _cpu) \
  LEON_Cpu_Restore_interrupt(_source, _previous, _cpu)


/* Load 32-bit word by forcing a cache-miss */
static inline unsigned int leon_r32_no_cache(uintptr_t addr)
{
  unsigned int tmp;
  __asm__ volatile (" lda [%1] 1, %0\n" : "=r"(tmp) : "r"(addr));
  return tmp;
}

/****************** ADDITIONAL Function calls ***************/

/* Set the type of interrupt level or edge ... */
static inline void LEON_Set_interrupt_type(uint32_t irq, uint32_t type)
{
    rtems_interrupt_lock_context _lock_context;
    uint32_t tmp;
    LEON_IRQCTRL_ACQUIRE( &_lock_context );
    tmp = GET_REG_WORD_VAL(__icb_base__ + ICB_CTRL0_OFFSET + ( irq << 2 ));
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_CTRL0_OFFSET + ( irq << 2 ), tmp | type);
    LEON_IRQCTRL_RELEASE( &_lock_context );
}

/* Set the type of interrupt level or edge ... */
static inline void LEON_Set_interrupt_priority(uint32_t irq, uint32_t priority)
{
    rtems_interrupt_lock_context _lock_context;
    uint32_t tmp;
    LEON_IRQCTRL_ACQUIRE( &_lock_context );
    tmp = GET_REG_WORD_VAL(__icb_base__ + ICB_CTRL0_OFFSET + ( irq << 2 ));
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_CTRL0_OFFSET + ( irq << 2 ), tmp | ( priority << 2));
    LEON_IRQCTRL_RELEASE( &_lock_context );
}

/* Set the type of interrupt level or edge ... */
static inline void LEON_Set_interrupt_type_priority(uint32_t irq, uint32_t type, uint32_t priority)
{
    rtems_interrupt_lock_context _lock_context;
    LEON_IRQCTRL_ACQUIRE( &_lock_context );
    LEON_IRQCTRL_WRITE_ICB(__icb_base__, ICB_CTRL0_OFFSET + ( irq << 2 ), ( priority << 2) | type);
    LEON_IRQCTRL_RELEASE( &_lock_context );
}

/* Allows to re-route interrupts from the other Leon */
static inline void LEON_Dynamic_irq_reroute(uint32_t source_irq, uint32_t target_irq, uint32_t enable)
{
    rtems_interrupt_lock_context _lock_context;
    uint32_t t;
    uint32_t bit_offset;
    uint32_t config_value;
    uint32_t val;
    uint32_t base = __icb_base__ == ICB1_BASE_ADR ? ICB0_BASE_ADR : ICB1_BASE_ADR;
    // There are 12 slot configurations divided across 3 registers with 4 configs in each
    t = (((target_irq - IRQ_DYNAMIC_0 ) >> 2) & 0x3) * 4; // Bits 2,3 define the register WORD offset
    bit_offset = (((target_irq - IRQ_DYNAMIC_0 ) >> 0) & 0x3) * 8; // Bottom 2 bits of the offset define byte within the 32 bit word
    config_value = (enable << 7) | (source_irq & 0x3F) ;
    LEON_IRQCTRL_ACQUIRE( &_lock_context );
    // Read current config
    val = GET_REG_WORD_VAL(base + ICB_C2C_INT_CFG0_OFFSET + t);
    // Clear bits and apply changes
    val &= ~((uint32_t)(0xFF << bit_offset));
    val |= (config_value << bit_offset);
    // Write the new value
    LEON_IRQCTRL_WRITE_ICB(base, ICB_C2C_INT_CFG0_OFFSET + t, val);
    LEON_IRQCTRL_RELEASE( &_lock_context );
}

// Allows to trigger an IRQ on the remote Leon
static inline void LEON_Force_remote_interrupt(uint32_t target_irq)
{
    uint32_t t = GET_SOURCE_OFFSET(target_irq);
    uint32_t remote_icb = __icb_base__ == ICB1_BASE_ADR ? ICB0_BASE_ADR : ICB1_BASE_ADR;
    LEON_IRQCTRL_WRITE_ICB(remote_icb, ICB_SETINT_0_OFFSET + t, 1 << ( target_irq - ( t * 8 ) ) );
}

/* common macros for the additional interrupt handling functions */
#define BSP_Set_interrupt_type(irq, type) \
  LEON_Set_interrupt_type(irq, type)
#define BSP_Set_interrupt_priority(irq, type) \
  LEON_Set_interrupt_priority(irq, type)
#define BSP_Set_interrupt_type_priority(irq, type, priority) \
  LEON_Set_interrupt_type_priority(irq, type, priority)
#define BSP_Dynamic_irq_reroute(source_irq, target_irq, enable) \
  LEON_Dynamic_irq_reroute(source_irq, target_irq, enable)
#define BSP_Force_remote_interrupt(target_irq) \
  LEON_Force_remote_interrupt(target_irq)

#endif /* !ASM */

#ifdef __cplusplus
}
#endif

#endif // _LIBBSP_MYRIAD2_H_
