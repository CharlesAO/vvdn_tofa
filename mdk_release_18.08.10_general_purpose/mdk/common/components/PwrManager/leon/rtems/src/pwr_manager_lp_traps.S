///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     power manager: low power trap table implementation
///
#include <rtems/asm.h>
#include <rtems/score/cpu.h>

/*
 * Window Overflow optimized trap table entry
 */
#define WOTRAP(_vector, _handler)   \
    sethi %hi(_handler), %l4;       \
    jmp   %l4+%lo(_handler);        \
     save;                          \
    nop

/*
 * Window Underflow optimized trap table entry
 */
#define WUTRAP(_vector, _handler)   \
    mov   %wim, %l3 ;               \
    sethi %hi(_handler), %l4 ;      \
    jmp   %l4+%lo(_handler);        \
     mov  %g0, %wim ! WIM = 0, so that we can restore regardless of WIM

/*
 *  Window overflow trap handler.
 *
 *  On entry:
 *
 *    prev regwin l1 = pc
 *    prev regwin l2 = npc
 */
	.section	".lockable.text"
    .type __lp_window_overflow_trap_handler, #function
	.align	4
SYM(__lp_window_overflow_trap_handler):

    /*
     *  Calculate new WIM by "rotating" the valid bits in the WIM right
     *  by one position.  The following shows how the bits move for a SPARC
     *  cpu implementation where SPARC_NUMBER_OF_REGISTER_WINDOWS is 8.
     *
     *    OLD WIM = 76543210
     *    NEW WIM = 07654321
     *
     *  NOTE: New WIM must be stored in a global register since the
     *        "save" instruction just prior to the load of the wim
     *        register will result in the local register set changing.
     */

    std  %l0, [%sp + 0x00]           ! save local register set
    std  %l2, [%sp + 0x08]
    mov  %wim, %l3
    sll  %l3, SPARC_NUMBER_OF_REGISTER_WINDOWS-1 , %l2 ! l2  = WIM << (Number Windows - 1)
    std  %l4, [%sp + 0x10]
    std  %l6, [%sp + 0x18]
    srl  %l3, 1, %l3                ! l3  = WIM >> 1
    wr   %l3, %l2, %wim             ! WIM = (WIM >> 1) ^
                                    !       (WIM << (Number Windows - 1))
                                    ! 3 instruction delay not needed here
    std  %i0, [%sp + 0x20]          ! save input register set
    std  %i2, [%sp + 0x28]
    std  %i4, [%sp + 0x30]
    std  %i6, [%sp + 0x38]
    restore                         ! Go back to trap window.
    jmp  %l1                        ! Re-execute save.
    rett %l2

    /*
     *  Window underflow trap handler.
     *
     *  On entry:
     *
     *    l1 = pc
     *    l2 = npc
     *    l3 = wim (from trap table)
     */

    .type __lp_window_underflow_trap_handler, #function
	.align	4
SYM(__lp_window_underflow_trap_handler):
    /*
     *  Calculate new WIM by "rotating" the valid bits in the WIM left
     *  by one position.  The following shows how the bits move for a SPARC
     *  cpu implementation where SPARC_NUMBER_OF_REGISTER_WINDOWS is 8.
     *
     *    OLD WIM = 76543210
     *    NEW WIM = 07654321
     *
     *  NOTE: New WIM must be stored in a global register since the
     *        "save" instruction just prior to the load of the wim
     *        register will result in the local register set changing.
     */
    ! In WIM 3 write instruction delay. since WIM<=0 from trap entry
    sll  %l3, 1, %l4                ! l4 = WIM << 1
    srl  %l3, SPARC_NUMBER_OF_REGISTER_WINDOWS-1, %l5
    or   %l5, %l4, %i5              ! %i5= (WIM << 1) |
                                    !      (WIM >> (Number Windows-1))
    restore                         ! Two restores to get into the
    restore %o5, %g0, %l7           ! window to restore. Carry along new WIM
    ldd  [%sp + 0x00], %l0          ! First the local register set
    mov   %l7, %wim                 ! load the new WIM
    ldd  [%sp + 0x08], %l2
    ldd  [%sp + 0x10], %l4
    ldd  [%sp + 0x18], %l6
    ldd  [%sp + 0x20], %i0          ! Then the input registers
    ldd  [%sp + 0x28], %i2
    ldd  [%sp + 0x30], %i4
    ldd  [%sp + 0x38], %i6
    save                            ! Get back to the trap window.
    save
    jmp  %l1                        ! Re-execute restore.
     rett  %l2

    .type __lp_noop_func, #function
	.align	4
SYM(__lp_noop_func):
   b __lp_noop_func
    nop

#define BAD_TRAP    \
  b __lp_noop_func; \
  nop;              \
  nop;              \
  nop;

#define SOFT_TRAP BAD_TRAP

  .section ".lockable.traps", "ax", @progbits
  .align 4096
    PUBLIC(__lp_trap_table)
SYM(__lp_trap_table):
  TRAP(0, SYM(__lp_noop_func) );                ! 00 reset trap
  BAD_TRAP;                                     ! 01 instruction access
  BAD_TRAP;                                     ! 02 illegal instruction
  BAD_TRAP;                                     ! 03 privileged instruction
  BAD_TRAP;                                     ! 04 fp disabled
  WOTRAP(5, SYM(__lp_window_overflow_trap_handler)); ! 05 window overflow
  WUTRAP(6, SYM(__lp_window_underflow_trap_handler));! 06 window underflow
  BAD_TRAP;                                     ! 07 memory address not aligned
  BAD_TRAP;                                     ! 08 fp exception
  BAD_TRAP;                                     ! 09 data access exception
  BAD_TRAP;                                     ! 0A tag overflow
  BAD_TRAP;                                     ! 0B undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 0C - 0F undefined
  BAD_TRAP;                                     ! 10 undefined
  BAD_TRAP;                                     ! 11 masked errors
  BAD_TRAP;                                     ! 12 external 1
  BAD_TRAP;                                     ! 13 external 2
  BAD_TRAP;                                     ! 14 UART A RX/TX
  BAD_TRAP;                                     ! 15 UART B RX/TX
  BAD_TRAP;                                     ! 16 correctable memory error
  BAD_TRAP;                                     ! 17 UART error
  BAD_TRAP;                                     ! 18 DMA access error
  BAD_TRAP;                                     ! 19 DMA timeout
  BAD_TRAP;                                     ! 1A external 3
  BAD_TRAP;                                     ! 1B external 4
  BAD_TRAP;                                     ! 1C general purpose timer
  BAD_TRAP;                                     ! 1D real time clock
  TRAP( 0x1e, SYM(wakeup_from_interrupt) );     ! hardcode 0xE interrupt priority here
  BAD_TRAP;                                     ! 1F watchdog timeout
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 20 - 23 undefined
  BAD_TRAP;                                     ! 24 cp_disabled
            BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 25 - 27 undefined
  BAD_TRAP;                                     ! 28 cp_exception
            BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 29 - 2B undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 2C - 2F undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 30 - 33 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 34 - 37 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 38 - 3B undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 3C - 3F undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 40 - 43 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 44 - 47 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 48 - 4B undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 4C - 4F undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 50 - 53 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 54 - 57 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 58 - 5B undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 5C - 5F undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 60 - 63 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 64 - 67 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 68 - 6B undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 6C - 6F undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 70 - 73 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 74 - 77 undefined
  BAD_TRAP; BAD_TRAP; BAD_TRAP; BAD_TRAP;       ! 78 - 7B undefined
  BAD_TRAP; BAD_TRAP;                           ! 7C - 7D undefined
  .word 0x0c, 0, 0, 0                           ! 7E (12 MHz default)
                                BAD_TRAP;       ! 7F undefined
  SOFT_TRAP;                                    ! 80: TRAP( 0x80, SYM(syscall)) - halt syscall SW trap
            SOFT_TRAP; SOFT_TRAP;               ! 81 - 82
                                   SOFT_TRAP;   ! 83: TRAP( 0x83, SYM(window_flush_trap_handler)) - flush windows SW trap
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 84 - 87
  SOFT_TRAP;                                    ! 88
             SOFT_TRAP;                         ! 89: IRQDIS_TRAP(SYM(syscall_irqdis)) - IRQ Disable syscall trap
                        SOFT_TRAP;              ! 8A: IRQEN_TRAP(SYM(syscall_irqen))   - IRQ Enable syscall trap
                                   SOFT_TRAP;   ! 8B
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 8C - 8F
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 90 - 93
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 94 - 97
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 98 - 9B
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 9C - 9F
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! A0 - A3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! A4 - A7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! A8 - AB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! AC - AF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! B0 - B3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! B4 - B7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! B8 - BB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! BC - BF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! C0 - C3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! C4 - C7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! C8 - CB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! CC - CF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! D0 - D3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! D4 - D7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! D8 - DB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! DC - DF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! E0 - E3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! E4 - E7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! E8 - EB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! EC - EF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! F0 - F3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! F4 - F7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! F8 - FB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! FC - FF

