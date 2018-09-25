/*
 *  tm27.h
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_TMTEST27
#error "This is an RTEMS internal file you must not include directly."
#endif

#ifndef __tm27_h
#define __tm27_h

/*
 *  Define the interrupt mechanism for Time Test 27
 *
 *  NOTE: Since the interrupt code for the SPARC supports both synchronous
 *        and asynchronous trap handlers, support for testing with both
 *        is included.
 */

#define SIS_USE_SYNCHRONOUS_TRAP  0

/*
 *  The synchronous trap is an arbitrarily chosen software trap.
 */

#if (SIS_USE_SYNCHRONOUS_TRAP == 1)

#define TEST_VECTOR SPARC_SYNCHRONOUS_TRAP( 0x90 )

#define MUST_WAIT_FOR_INTERRUPT 1

#define Install_tm27_vector( handler ) \
  set_vector( (handler), TEST_VECTOR, 1 );

#define Cause_tm27_intr() \
  __asm__ volatile( "ta 0x10; nop " );

#define Clear_tm27_intr() /* empty */

#define Lower_tm27_intr() /* empty */

/*
 *  The asynchronous trap is an arbitrarily chosen ERC32 interrupt source.
 */

#else   /* use a regular asynchronous trap */

#define TEST_INTERRUPT_SOURCE (IRQ_GPIO_0)
#define TEST_VECTOR MYRIAD2_TRAP_TYPE( TEST_INTERRUPT_SOURCE )
#define TEST_INTERRUPT_SOURCE2 (TEST_INTERRUPT_SOURCE + 1)
#define TEST_VECTOR2 MYRIAD2_TRAP_TYPE (TEST_INTERRUPT_SOURCE2)
#define MUST_WAIT_FOR_INTERRUPT 1
#define TEST_TRAP1_PRIORITY     1
#define TEST_TRAP2_PRIORITY     2
#define TEST_TRAP1_TYPE         POS_EDGE_INT
#define TEST_TRAP2_TYPE         POS_EDGE_INT

#define Install_tm27_vector( handler ) \
  BSP_Set_interrupt_type_priority(TEST_INTERRUPT_SOURCE, TEST_TRAP1_TYPE, TEST_TRAP1_PRIORITY); \
  BSP_Set_interrupt_type_priority(TEST_INTERRUPT_SOURCE2, TEST_TRAP2_TYPE, TEST_TRAP2_PRIORITY); \
  BSP_shared_interrupt_register(TEST_INTERRUPT_SOURCE, NULL, handler, NULL); \
  BSP_shared_interrupt_register(TEST_INTERRUPT_SOURCE2, NULL, handler, NULL);

#define Cause_tm27_intr() \
  do { \
    BSP_Force_interrupt( TEST_INTERRUPT_SOURCE+(Interrupt_nest>>1)); \
    nop(); \
    nop(); \
    nop(); \
  } while (0)

#define Clear_tm27_intr() \
  BSP_Clear_interrupt( TEST_INTERRUPT_SOURCE )

#define Lower_tm27_intr() /* empty */

#endif

#endif
