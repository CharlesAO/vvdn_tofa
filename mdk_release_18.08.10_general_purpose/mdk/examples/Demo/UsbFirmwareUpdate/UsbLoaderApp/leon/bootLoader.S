    #include <swcLeonUtilsDefines.h>
    #include <registersMyriad.h>

!; execute
    .section ".text.boot", "ax", @progbits

    .type   __boot_execute, #function
    .global __boot_execute
    .align 4

    .set __const_0d, 0x76007f70
    .global __const_0d

__boot_execute:

!; disable caches
    sta %g0, [ %g0 ] __CCR_ASI
!; flush caches
    flush
!; save target address (to %g1)
    andn    %o0, 3, %g1 ! %g1 = target_address & 0xFFFFFFFC
!; clear WIM
    wr  %g0, %wim
      nop; nop; nop

!; setup PSR: max PIL == 15, FPU enabled, Supervisor, traps disabled, window 7
    sethi   %hi( PSR_EF | PSR_PIL15 | PSR_S | PSR_CWP7 ), %g2
    wr  %g2, %lo( PSR_EF | PSR_PIL15 | PSR_S | PSR_CWP7 ), %psr
      nop; nop; nop

!; prepare for clearing all the windowed registers, clear %g6:%g7
    sethi   %hi( __const_0d ), %g3
    ldd [ %g3 + %lo( __const_0d )], %g6

!; invalidate window 0
    wr  %g0, WIM_INVD0, %wim
      nop; nop; nop
!; enable traps
    wr  %g2, PSR_PIL15 | PSR_S | PSR_ET | PSR_CWP7, %psr
      nop; nop; nop
!; clear %y
    mov %g0, %y
      nop; nop; nop

!; We have to decrement the stack pointer by 64 to allow for window overflow
!; to save registers on the first hit. This can happen in any of the following
!; functions. So if ever changing this line, please make sure the decrement of 64
!; happens before any call to any function like mvHeapInit or mvInit or any other one.
!; place the stack pointer right under .boot.bss, not clearing the data there
!; as it might be used by the 2nd level boot loader; IT'S UP TO THE APP - IF
!; IT IS KNOWN NOT TO BE A 2ND-LEVEL BOOT LOADER - TO MOVE THE STACK HIGHER
    set ( _LEON_STACK_POINTER - 64), %sp

!; execute and clear %g1
    jmp %g1
      clr   %g1
    .size   __boot_execute, . - __boot_execute

!;=============================================================================

    .section ".sys.rodata.4", "a", @progbits
    .type   __default_fsr_value, #object
    .align  4
__default_fsr_value:
    .word   FSR_RD_NINF | FSR_NS !; round towards -Inf, convert denormals to +-0
    .size   __default_fsr_value, 4
