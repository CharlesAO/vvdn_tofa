#include <swcLeonUtilsDefines.h>
#include <registersMyriad.h>
!; Note: Leon L1 ICache/DCache is enabled by default
!; This default can be overridden from the Makefile
!;

		!; processor startup
		.section ".sys.text.start", "ax", @progbits
		.type	start, #function
		.global start
		.align 4
start:
!; Special instructions so we can see the word order on the first fetch from CMX
      set	0x1, %g1
      set	0x2, %g1
      set	0x3, %g1
      set	0x4, %g1
      set	0x5, %g1
      set	0x6, %g1
      set	0x7, %g1
      set	0x8, %g1

!; Before we do ANYTHING fancy we simply try to execute a pile of NOPS
!; If this doesn't work, then we have a bigger problem
!; If subsequent code fails, then it suggests differences in the PSR implementation of 
!; Leon4 etc. This will need more work
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
!; prepare WIM for setting up PSR
		wr	%g0, %wim
!; trigger caches flush
		  flush
#ifndef DISABLE_LEON_FPU
!; prepare PSR and traps config
		set	PSR_EF|PSR_PIL15|PSR_S|PSR_CWP7, %g1
#else
		set	PSR_PIL15|PSR_S|PSR_CWP7, %g1
#endif
		sethi	%hi( traphandler ), %g2
!; setup PSR
		wr	%g1, %psr
!; setup traps
		wr	%g2, %tbr
#ifdef LEON_MVT
		set	ASR17_DWT, %g3
#else
		set	ASR17_SVT|ASR17_DWT, %g3
#endif

!; initialize WIM properly (we're in window 7, so invalidate window 0)
		wr	%g0, WIM_INVD0, %wim
		nop

!; enable single vector trapping, disable write_error traps
		wr	%g3, %asr17
!; configure caches
		set	0, %g2
#ifndef DISABLE_LEON_ICACHE
		or %g2,CCR_ICS_ENABLED,%g2
#else
!; having something in the else branch keeps our instuction addressing the same in either case, and also makes sure that we don't assume that caches are off.
		andn %g2,CCR_ICS_ENABLED,%g2
#endif
#ifndef DISABLE_LEON_DCACHE
		or %g2,CCR_DCS_ENABLED,%g2
#else
!; having something in the else branch keeps our instuction addressing the same in either case, and also makes sure that we don't assume that caches are off.
		andn %g2,CCR_DCS_ENABLED,%g2
#endif
!; prepare & optionally enable cache
		sta	%g2, [ %g0 ] __CCR_ASI

!; clear the application's .bss - also aligned to 8, size a multiple of 8
#ifndef DONT_CLEAR_BSS
		set	__bss_start, %g2
		set	__bss_end, %g3
		clr     %g1
.L.clear_bss:
		cmp	%g2, %g3
		inc	8, %g2
		blu,a	.L.clear_bss
		  std	%g0, [%g2 - 8]
#endif

#ifndef DONT_CLEAR_BSS
!; clear the system .system.bss - note: aligned to 8, goes all the way to RAM's top
		set	__sys_bss_end  , %sp
		set	__sys_bss_start, %g3
.L.clear_system_bss:
		dec	8, %sp
		cmp	%sp, %g3
		bgu	.L.clear_system_bss
		  std	%g0, [ %sp ]
#endif

!; Setup the Leon stack pointer based on symbol defined in the linker script
!; We have to decrement the stack pointer by 64 to allow for window overflow
!; to save registers on the first hit. This can happen in any of the following
!; functions. So if ever changing this line, please make sure the decrement of 64
!; happens before any call to any function like mvHeapInit or mvInit or any other one
		set	( _LEON_STACK_POINTER - 64), %sp

!; enable traps  
		rd	%psr, %g1
		wr	%g1, PSR_ET, %psr
		nop; nop
		clr	%g1
 
#ifndef DISABLE_LEON_FPU
!; use FPU inexact mode
        sethi %hi(__fsr_default) , %g2
		ld	[%g2 + %lo( __fsr_default )], %fsr
		nop; nop
#endif

!; call heap init
		call	__mvHeapInit
		  nop

!; Call C++ global constructors
		call	__mvInit
		  nop

!; call SystemInit
		call	SystemInit
		  nop

!; call main
		call	main
		  nop

!; "return" 0 when main returns
		.size	start, . - start

!;-----------------------------------------------------------------------------

		.global exit
		.type	exit, #function
exit:
!; Save main return exit code in order to return it when finishing
		mov %o0, %l0

!; Call C++ global constructors
		call 	__mvFini
		nop

!; Restore main return exit code in order to return it when finishing
		mov %l0, %o0

!; clear ET
		rd      %psr, %g1
		bclr    PSR_ET, %g1
		wr      %g1, %psr
!; In VCS we want to exit the simulation 
!; Then signal simulation termination by writing 0xFFFFFFFF to the last ROM word
!; This exits VCS only when the primary leon does this write.
		set	TEST_FINISH_ADR, %g2 !; this is two delay instructions
		set	0xFFFFFFFF, %g1
		st	%g1,[%g2]
!; Set %g1 to 1 to signal the debugger that this was a normal clean exit
!; force error_mode with %tbr.tt=0x80 and %g1==0x1, and the exit code in %o0
		mov	1,%g1
		t	0
		.size	exit, . - exit
