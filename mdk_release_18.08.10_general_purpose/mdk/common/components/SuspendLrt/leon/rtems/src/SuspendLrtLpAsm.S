///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Suspend LRT component: low power ASSEMBLY implementation
///
#include <rtems/asm.h>
#include <rtems/score/cpu.h>
#include <swcLeonUtilsDefines.h>

#define FRAME_OFFSET_PSR            (0)
#define FRAME_OFFSET_WIM            (FRAME_OFFSET_PSR        + 4)
#define FRAME_OFFSET_TBR            (FRAME_OFFSET_WIM        + 4)
#define FRAME_OFFSET_Y              (FRAME_OFFSET_TBR        + 4)
#define FRAME_OFFSET_REGS_G2_G3     (FRAME_OFFSET_Y          + 4)
#define FRAME_OFFSET_REGS_G4_G5     (FRAME_OFFSET_REGS_G2_G3 + 8)
#define FRAME_OFFSET_REGS_G6_G7     (FRAME_OFFSET_REGS_G4_G5 + 8)
#define FRAME_OFFSET_REGS_I0_I1     (FRAME_OFFSET_REGS_G6_G7 + 8)
#define FRAME_OFFSET_REGS_I2_I3     (FRAME_OFFSET_REGS_I0_I1 + 8)
#define FRAME_OFFSET_REGS_I4_I5     (FRAME_OFFSET_REGS_I2_I3 + 8)
#define FRAME_OFFSET_REGS_I6_I7     (FRAME_OFFSET_REGS_I4_I5 + 8)
#define FRAME_OFFSET_REGS_L0_L1     (FRAME_OFFSET_REGS_I6_I7 + 8)
#define FRAME_OFFSET_REGS_L2_L3     (FRAME_OFFSET_REGS_L0_L1 + 8)
#define FRAME_OFFSET_REGS_L4_L5     (FRAME_OFFSET_REGS_L2_L3 + 8)
#define FRAME_OFFSET_REGS_L6_L7     (FRAME_OFFSET_REGS_L4_L5 + 8)
#define FRAME_OFFSET_REGS_O6_O7     (FRAME_OFFSET_REGS_L6_L7 + 8)
#define FRAME_END                   (FRAME_OFFSET_REGS_O6_O7 + 8)

#define FRAME_SIZE ((FRAME_END + CPU_STACK_ALIGNMENT - 1) & ~(CPU_STACK_ALIGNMENT - 1))

.macro backup_reg reg off
    rd \reg, %g2
    st %g2, \off
.endm

.macro restore_reg off reg
    ld \off, %g2
    wr %g2, \reg
.endm

    .section ".data"
    .type frame_storage, #object
    .align 8
SYM(frame_storage):
    .skip FRAME_END
    .size frame_storage, FRAME_END

#define LP_STACK_SIZE (0x200 & ~(CPU_STACK_ALIGNMENT - 1))
    .section ".data"
    .type LP_STACK_PTR, #object
    .align 8
SYM(LP_STACK_PTR):
    .skip LP_STACK_SIZE
SYM(LP_STACK_PTR_END):
    .size LP_STACK_PTR, LP_STACK_SIZE

    .section ".text"
    .type switchStackAndEntersLp_asm, #function
    .align 4
    PUBLIC(switchStackAndEntersLp_asm)
SYM(switchStackAndEntersLp_asm):
    t 0x83                              ! Flush all windows except of the current one.
    nop

    ! o0 = frame_storage
    ! o1 = PSR
    ! o2 = PSR_EF_MASK
    rd   %psr, %o1
    set SYM(frame_storage), %o0
    sethi %hi(SPARC_PSR_EF_MASK), %o2

    ! save PSR & ensure interrupts are disabled
    st   %o1,  [%o0 + FRAME_OFFSET_PSR]
    ! psr with PIL=0xf (to disable IRQ); EFuses (needed by 2450) require FPU to be enabled (EF mask);
    or   %o1,  (SPARC_PSR_PIL_MASK | SPARC_PSR_EF_MASK), %o1
    mov  %o1,  %psr                     ! Set PSR. Write delay 3 instr -- write delay not needed

    std  %i0,  [%o0 + FRAME_OFFSET_REGS_I0_I1]
    std  %i2,  [%o0 + FRAME_OFFSET_REGS_I2_I3]
    std  %i4,  [%o0 + FRAME_OFFSET_REGS_I4_I5]
    std  %i6,  [%o0 + FRAME_OFFSET_REGS_I6_I7]
    std  %l0,  [%o0 + FRAME_OFFSET_REGS_L0_L1]
    std  %l2,  [%o0 + FRAME_OFFSET_REGS_L2_L3]
    std  %l4,  [%o0 + FRAME_OFFSET_REGS_L4_L5]
    std  %l6,  [%o0 + FRAME_OFFSET_REGS_L6_L7]
    std  %o6,  [%o0 + FRAME_OFFSET_REGS_O6_O7]
    std  %g2,  [%o0 + FRAME_OFFSET_REGS_G2_G3]
    std  %g4,  [%o0 + FRAME_OFFSET_REGS_G4_G5]
    std  %g6,  [%o0 + FRAME_OFFSET_REGS_G6_G7]

    ! save WIM, TBR and Y
    backup_reg %wim, [%o0 + FRAME_OFFSET_WIM]
    backup_reg %tbr, [%o0 + FRAME_OFFSET_TBR]
    backup_reg %y  , [%o0 + FRAME_OFFSET_Y]

    set SYM(__lp_trap_table), %g2
    mov %g2, %tbr                       ! Set TBR. Write delay 3 instr

    sethi  %hi(LP_STACK_PTR_END), %g1
    or %g1, %lo(LP_STACK_PTR_END), %g1
    mov %g1, %sp
    save %g1, 0, %sp
    call SuspendLrtIsolated
     nop

    .align 4
    .type restoreStackAndExitLp_asm, #function
    PUBLIC(restoreStackAndExitLp_asm)
SYM(restoreStackAndExitLp_asm):
    mov %psr, %g1
    subcc %g1, %g0, %o3                ! store in o3 the comparison between psr and g0
    ! g1 = frame_storage
    ! g2 = PSR
    sethi  %hi(frame_storage), %g1
    or  %g1, %lo(frame_storage), %g1
    ld [%g1 + FRAME_OFFSET_PSR], %g2

    ! restore PSR, but without interrupts/traps
    or   %g2,  SPARC_PSR_PIL_MASK, %g3  ! psr with PIL=0xf (to disable IRQ)
    wr %g3, %psr                        ! Restore PSR. Write delay 3 instr
    nop                                 ! PSR write delay
    nop
    nop
    ! o0 = frame_storage (from g1)
    ! o1 = PSR           (from g2)
    ! o2 = PSR_EF_MASK
    mov %g1, %o0                         ! sethi  %hi(frame_storage), %o0; or  %o0, %lo(frame_storage), %o0
    mov %g2, %o1                         ! ld [%o0 + FRAME_OFFSET_PSR], %o1
    sethi %hi(SPARC_PSR_EF_MASK), %o2

    restore_reg [%o0 + FRAME_OFFSET_TBR], %tbr
    restore_reg [%o0 + FRAME_OFFSET_WIM], %wim
    restore_reg [%o0 + FRAME_OFFSET_Y],   %y

    ldd  [%o0 + FRAME_OFFSET_REGS_G2_G3], %g2
    ldd  [%o0 + FRAME_OFFSET_REGS_G4_G5], %g4
    ldd  [%o0 + FRAME_OFFSET_REGS_G6_G7], %g6
    ldd  [%o0 + FRAME_OFFSET_REGS_I0_I1], %i0
    ldd  [%o0 + FRAME_OFFSET_REGS_I2_I3], %i2
    ldd  [%o0 + FRAME_OFFSET_REGS_I4_I5], %i4
    ldd  [%o0 + FRAME_OFFSET_REGS_I6_I7], %i6
    ldd  [%o0 + FRAME_OFFSET_REGS_L0_L1], %l0
    ldd  [%o0 + FRAME_OFFSET_REGS_L2_L3], %l2
    ldd  [%o0 + FRAME_OFFSET_REGS_L4_L5], %l4
    ldd  [%o0 + FRAME_OFFSET_REGS_L6_L7], %l6
    ldd  [%o0 + FRAME_OFFSET_REGS_O6_O7], %o6

    flush                               ! we need to flush L1 I$ to ensure tbr changes is applied
    wr %o1, %psr                        ! Restore original PSR. Write delay 3 instr
    nop;nop;nop

    retl
     nop

    .type restoreProcessorAfterLp_asm, #function
    .align 4096
    PUBLIC(restoreProcessorAfterLp_asm)
SYM(restoreProcessorAfterLp_asm):

    ! o1 = PSR
    mov  %psr, %o1
    or   %o1,  (SPARC_PSR_PIL_MASK | SPARC_PSR_EF_MASK), %o1
    mov  %o1,  %psr                ! Set PSR. Write delay 3 instr -- write delay not needed

    ! Fix temp tbr
    set SYM(__lp_trap_table), %g2
    mov %g2, %tbr                  ! Set TBR. Write delay 3 instr

    sethi  %hi(LP_STACK_PTR_END), %g1
    or %g1, %lo(LP_STACK_PTR_END), %g1
    mov %g1, %sp

    set ASR17_DWT, %g1
    wr %g1, %asr17

!; re-configure caches: If the user needs doesn't need them, they will get disabled later on
    set CCR_IB, %g1
    or %g1, CCR_ICS_ENABLED, %g1
    or %g1, CCR_DCS_ENABLED, %g1
    sta %g1, [ %g0 ] __CCR_ASI

    flush                          ! we need to flush L1 I$ to ensure tbr changes is applied
    save %g1, 0, %sp
    call wakeup_from_interrupt
     nop

