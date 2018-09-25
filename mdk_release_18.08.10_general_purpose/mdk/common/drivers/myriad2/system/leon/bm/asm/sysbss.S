#include <DrvIcbDefines.h>

!;-----------------------------------------------------------------------------
!;- __fsr_default -------------------------------------------------------------
!;-----------------------------------------------------------------------------
#define	FSR_RD_NEAREST	(0<<30)  // Round towards nearest, (even if tie)        
#define	FSR_RD_ZERO  	(1<<30)  // Round towards zero  (truncate/chop)         
#define	FSR_RD_P_INF	(2<<30)  // Round towards plus infinity                 
#define	FSR_RD_N_INF	(3<<30)  // Round towards minus infinity                
#define	FSR_NS			(1<<22)  // Non Standard floating point bit (See Sparc Architecure Manual) 

		.section ".sys.rodata.4", "a", @progbits
		.global __fsr_default
		.type __fsr_default, #object
		.align 4
__fsr_default: .word FSR_RD_NEAREST | FSR_NS
		.size __fsr_default, 4

!;-----------------------------------------------------------------------------
!;- void (*irq_table)()[__N_IRQS__] -------------------------------------------
!;-----------------------------------------------------------------------------

		.section ".sys.bss.4", "aw", @nobits
		.type	__irq_table, #object
		.global __irq_table
		.align	4
__irq_table:
		.skip	4 * MV_TOTAL_LEON_IRQS
		.size	__irq_table, 4 * MV_TOTAL_LEON_IRQS
