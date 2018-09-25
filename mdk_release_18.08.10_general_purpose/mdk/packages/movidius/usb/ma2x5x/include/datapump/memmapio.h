/* memmapio.h	Fri Jun 07 2013 09:20:43 ssyang */

/*

Module:  memmapio.h

Function:
	Memory-Mapped I/O access macros

Version:
	V3.11c	Fri Jun 07 2013 09:20:43 ssyang	Edit level 8

Copyright notice:
	This file copyright (C) 1998-2005, 2007, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	John Keys, MCCI Corporation	August 1998

Revision history:
   1.00a  Tue Aug 18 1998 10:56:46  johnk
	Module created.

   1.43h  9/8/1998  tmm
	1) we should have been using volatile's.
	2) Add UHIL_insb() and UHIL_outsb, conditioned on "UHIL_insb_defined"
	   and "UHIL_outsb_defined"

   1.50a  7/20/1999  tmm
	bug 258:  Add dword versions of everything.  Change UHIL_insb/outsb 
	to use UINT counts, which should not change anything important; on 
	real 86, UINT==UINT16, which is OK; on 68k UINT==UINT32, which is also
	OK.

   1.91c  Mon Apr 18 2005 11:10:16  chwon
	Correct type cloaking

   1.97j  Sat Nov 17 2007 05:34:05  tmm
	3495:  UHIL_inwi and UHIL_outwi are not valid.  Removed them.

   1.97j  Sat Nov 17 2007 06:02:34  tmm
	3494:  Add dword and byte forms: UHIL_insbi, UHIL_insdi, UHIL_outsbi,
	UHIL_outsdi.

   1.97j  Wed Nov 21 2007 12:55:08  tmm
	UHIL_insbi() [so far unused] had an incorrect cast.  Also, with the
	fixes for 3511, we discovered an error in the header file setup --
	need to #include basehilenv.h, because __TMS_BEGIN_DECLS has to
	be in scope afterwards.

   3.11c  Fri Jun 07 2013 09:20:43  ssyang
	17340: Add cpu memory barrier API

*/

#ifndef _MEMMAPIO_H_		/* prevent multiple includes */
#define _MEMMAPIO_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

/****************************************************************************\
|
|	We define basic I/O macros for each of the basic data types:
|
|	UHIL_<op>b for UINT8
|	UHIL_<op>w for UINT16
|	UHIL_<op>d for UINT32
|
|	The names other than "b" follow the intel convention that w == word ==
|	16 bits and d == dword == 32 bits.
|
|	To allow a calling package to selectively override, we check for each
|	definition whether the macro UHIL_<op><width>_defined is defined;
|	if so, we assume that the corresponding symbol UHIL_<op><width> is 
|	(or will be) defined elsewhere.  By using a separate symbol,
|	we provide for situations in which UHIL_<op><width> must be a
|	function rather than a macro.
|
\****************************************************************************/


#ifndef UHIL_inb_defined
# define UHIL_inb_defined	1
# define UHIL_inb(bus, ioport)	(*((__TMS_VOLATILE __TMS_UINT8 *)(ioport)))
#endif /* UHIL_inb_defined */

#ifndef UHIL_outb_defined
# define UHIL_outb_defined	1
# define UHIL_outb(bus, ioport, val)	\
	(*((__TMS_VOLATILE __TMS_UINT8 *)(ioport)) = ((__TMS_UINT8)(val)))
#endif /* UHIL_outb_defined */
	
#ifndef UHIL_inw_defined
# define UHIL_inw_defined	1
# define UHIL_inw(bus, ioport)	\
	(*((__TMS_VOLATILE __TMS_UINT16 *)(ioport)))
#endif /* UHIL_inw_defined */

#ifndef UHIL_outw_defined
# define UHIL_outw_defined	1
# define UHIL_outw(bus, ioport, val)	\
	(*((__TMS_VOLATILE __TMS_UINT16 *)(ioport)) = ((__TMS_UINT16)(val)))
#endif

#ifndef UHIL_ind_defined
# define UHIL_ind_defined	1
# define UHIL_ind(bus, ioport)	\
	(*((__TMS_VOLATILE __TMS_UINT32 *)(ioport)))
#endif /* UHIL_ind_defined */

#ifndef UHIL_outd_defined
# define UHIL_outd_defined	1
# define UHIL_outd(bus, ioport, val)	\
	(*((__TMS_VOLATILE __TMS_UINT32 *)(ioport)) = ((__TMS_UINT32)(val)))
#endif

/*
|| UHIL_insb() is like the x86 insb() instruction; it reads a sequence
|| of bytes from a port, as quickly as it possibly can.  Note that
|| it matches the x86/68k semantics quite closely:  __nc is a UINT16,
|| not a UINT32, to make things go faster.  nc should be UINT, and 
|| therefore variable width per architecture.
||
|| UHIL_insb expands by default to an in-line moderately optimized
|| data transfer loop.
*/
#ifndef UHIL_insb_defined
# define UHIL_insb_defined	1
# define UHIL_insb(bus, port, buf, nc)	do { \
	__TMS_VOLATILE __TMS_UINT8 * __TMS_CONST __src = (__TMS_VOLATILE __TMS_UINT8 *) (port); \
	__TMS_UINT8 *__buf = (buf); \
	__TMS_UINT __nc = (nc); \
 \
	for (; __nc >= 8; __nc -= 8) \
		{ \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		} \
 \
	switch (__nc) \
		{ \
	case 7: \
		*__buf++ = *__src; \
	case 6: \
		*__buf++ = *__src; \
	case 5: \
		*__buf++ = *__src; \
	case 4: \
		*__buf++ = *__src; \
	case 3: \
		*__buf++ = *__src; \
	case 2: \
		*__buf++ = *__src; \
	case 1: \
		*__buf++ = *__src; \
		} \
	} while (0)
#endif /* UHIL_insb_defined */

/*
|| UHIL_outsb() is like the x86 outsb() instruction; it outputs
|| a buffer to a port.
*/
#ifndef UHIL_outsb_defined
# define UHIL_outsb_defined	1
# define UHIL_outsb(bus, port, buf, nc) do { \
	__TMS_VOLATILE __TMS_UINT8 * __TMS_CONST __dest = (__TMS_VOLATILE __TMS_UINT8 *) (port); \
	__TMS_CONST __TMS_UINT8 *__buf = (buf); \
	__TMS_UINT __nc = (nc); \
 \
	for (; __nc >= 8; __nc -= 8) \
		{ \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		} \
 \
	switch (__nc) \
		{ \
	case 7: \
		*__dest = *__buf++; \
	case 6: \
		*__dest = *__buf++; \
	case 5: \
		*__dest = *__buf++; \
	case 4: \
		*__dest = *__buf++; \
	case 3: \
		*__dest = *__buf++; \
	case 2: \
		*__dest = *__buf++; \
	case 1: \
		*__dest = *__buf++; \
		} \
	} while (0)
#endif /* UHIL_outsb_defined */

/*
|| UHIL_insw() is like the x86 insw() instruction; it reads a sequence
|| of bytes from a port, as quickly as it possibly can.  Note that
|| it matches the x86/68k semantics quite closely:  __nc is a UINT16,
|| not a UINT32, to make things go faster.  nw is a UINT, 
|| and therefore variable width per architecture.
||
|| UHIL_insw expands by default to an in-line moderately optimized
|| data transfer loop.
||
|| buf must be aligned correctly for the architecture.
*/
#ifndef UHIL_insw_defined
# define UHIL_insw_defined	1
# define UHIL_insw(bus, port, buf, nw)	do { \
	__TMS_VOLATILE __TMS_UINT16 * __TMS_CONST __src = (__TMS_VOLATILE __TMS_UINT16 *) (port); \
	__TMS_UINT16 *__buf = (buf); \
	__TMS_UINT __nw = (nw); \
 \
	for (; __nw >= 8; __nw -= 8) \
		{ \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		} \
 \
	switch (__nw) \
		{ \
	case 7: \
		*__buf++ = *__src; \
	case 6: \
		*__buf++ = *__src; \
	case 5: \
		*__buf++ = *__src; \
	case 4: \
		*__buf++ = *__src; \
	case 3: \
		*__buf++ = *__src; \
	case 2: \
		*__buf++ = *__src; \
	case 1: \
		*__buf++ = *__src; \
		} \
	} while (0)
#endif /* UHIL_insw_defined */

/*
|| UHIL_outsw() is like the x86 outwb() instruction; it outputs
|| a buffer to a 16-bit port.  See restrictions mentioned in
|| UHIL_insw().
*/
#ifndef UHIL_outsw_defined
# define UHIL_outsw_defined	1
# define UHIL_outsw(bus, port, buf, nw) do { \
	__TMS_VOLATILE __TMS_UINT16 * __TMS_CONST __dest = (__TMS_VOLATILE __TMS_UINT16 *) (port); \
	__TMS_CONST __TMS_UINT16 *__buf = (buf); \
	__TMS_UINT __nw = (nw); \
 \
	for (; __nw >= 8; __nw -= 8) \
		{ \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		} \
 \
	switch (__nw) \
		{ \
	case 7: \
		*__dest = *__buf++; \
	case 6: \
		*__dest = *__buf++; \
	case 5: \
		*__dest = *__buf++; \
	case 4: \
		*__dest = *__buf++; \
	case 3: \
		*__dest = *__buf++; \
	case 2: \
		*__dest = *__buf++; \
	case 1: \
		*__dest = *__buf++; \
		} \
	} while (0)
#endif /* UHIL_outsw_defined */

/*
|| UHIL_insd() is like the x86 insd() instruction; it reads a sequence
|| of bytes from a port, as quickly as it possibly can.  Note that
|| it matches the x86/68k semantics quite closely:  __nc is a UINT16,
|| not a UINT32, to make things go faster.  It could be argued that
|| nc should be UINT, and therefore variable width per architecture.
||
|| UHIL_insw expands by default to an in-line moderately optimized
|| data transfer loop.
||
|| buf must be aligned correctly for the architecture.
*/
#ifndef UHIL_insd_defined
# define UHIL_insd_defined	1
# define UHIL_insd(bus, port, buf, nd)	do { \
	__TMS_VOLATILE __TMS_UINT32 * __TMS_CONST __src = (__TMS_VOLATILE __TMS_UINT32 *) (port); \
	__TMS_UINT32 *__buf = (buf); \
	__TMS_UINT __nd = (nd); \
 \
	for (; __nd >= 8; __nd -= 8) \
		{ \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		*__buf++ = *__src; \
		} \
 \
	switch (__nd) \
		{ \
	case 7: \
		*__buf++ = *__src; \
	case 6: \
		*__buf++ = *__src; \
	case 5: \
		*__buf++ = *__src; \
	case 4: \
		*__buf++ = *__src; \
	case 3: \
		*__buf++ = *__src; \
	case 2: \
		*__buf++ = *__src; \
	case 1: \
		*__buf++ = *__src; \
		} \
	} while (0)
#endif /* UHIL_insd_defined */

/*
|| UHIL_outsd() is like the x86 outwb() instruction; it outputs
|| a buffer to a 16-bit port.  See restrictions mentioned in
|| UHIL_insd().
*/
#ifndef UHIL_outsd_defined
# define UHIL_outsd_defined	1
# define UHIL_outsd(bus, port, buf, nd) do { \
	__TMS_VOLATILE __TMS_UINT32 * __TMS_CONST __dest = (__TMS_VOLATILE __TMS_UINT32 *) (port); \
	__TMS_CONST __TMS_UINT32 *__buf = (buf); \
	__TMS_UINT __nd = (nd); \
 \
	for (; __nd >= 8; __nd -= 8) \
		{ \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		*__dest = *__buf++; \
		} \
 \
	switch (__nd) \
		{ \
	case 7: \
		*__dest = *__buf++; \
	case 6: \
		*__dest = *__buf++; \
	case 5: \
		*__dest = *__buf++; \
	case 4: \
		*__dest = *__buf++; \
	case 3: \
		*__dest = *__buf++; \
	case 2: \
		*__dest = *__buf++; \
	case 1: \
		*__dest = *__buf++; \
		} \
	} while (0)
#endif /* UHIL_outsd_defined */

/*
|| UHIL_inswb() reads a sequence
|| of bytes from a sequence of ports, as quickly as it possibly can.
||
|| UHIL_insbi expands by default to an in-line moderately optimized
|| data transfer loop.
*/

#ifndef UHIL_insbi_defined
# define UHIL_insbi_defined	1
# define UHIL_insbi(bus, port, buf, nb)	do { \
	__TMS_VOLATILE __TMS_UINT8 * __src = (__TMS_VOLATILE __TMS_UINT8 *) (port); \
	__TMS_UINT8 *__buf = (buf); \
	__TMS_UINT __nb = (nb); \
 \
	for (; __nb >= 8; __nb -= 8) \
		{ \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		} \
 \
	switch (__nb) \
		{ \
	case 7: \
		*__buf++ = *__src++; \
	case 6: \
		*__buf++ = *__src++; \
	case 5: \
		*__buf++ = *__src++; \
	case 4: \
		*__buf++ = *__src++; \
	case 3: \
		*__buf++ = *__src++; \
	case 2: \
		*__buf++ = *__src++; \
	case 1: \
		*__buf++ = *__src++; \
		} \
	} while (0)
#endif /* UHIL_insbi_defined */

/*
|| UHIL_inswi() reads a sequence
|| of words from a sequence of ports, as quickly as it possibly can.
||
|| UHIL_inswi expands by default to an in-line moderately optimized
|| data transfer loop.
||
|| buf must be aligned correctly for the architecture.
*/
#ifndef UHIL_inswi_defined
# define UHIL_inswi_defined	1
# define UHIL_inswi(bus, port, buf, nw)	do { \
	__TMS_VOLATILE __TMS_UINT16 * __src = (__TMS_VOLATILE __TMS_UINT16 *) (port); \
	__TMS_UINT16 *__buf = (buf); \
	__TMS_UINT __nw = (nw); \
 \
	for (; __nw >= 8; __nw -= 8) \
		{ \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		} \
 \
	switch (__nw) \
		{ \
	case 7: \
		*__buf++ = *__src++; \
	case 6: \
		*__buf++ = *__src++; \
	case 5: \
		*__buf++ = *__src++; \
	case 4: \
		*__buf++ = *__src++; \
	case 3: \
		*__buf++ = *__src++; \
	case 2: \
		*__buf++ = *__src++; \
	case 1: \
		*__buf++ = *__src++; \
		} \
	} while (0)
#endif /* UHIL_inswi_defined */

/*
|| UHIL_insdi() reads a sequence
|| of 32-bit dwords from a sequence of ports, as quickly as it possibly can.
||
|| UHIL_insdi expands by default to an in-line moderately optimized
|| data transfer loop.
||
|| buf must be aligned correctly for the architecture.
*/
#ifndef UHIL_insdi_defined
# define UHIL_insdi_defined	1
# define UHIL_insdi(bus, port, buf, nd)	do { \
	__TMS_VOLATILE __TMS_UINT32 * __src = (__TMS_VOLATILE __TMS_UINT32 *) (port); \
	__TMS_UINT32 *__buf = (buf); \
	__TMS_UINT __nd = (nd); \
 \
	for (; __nd >= 8; __nd -= 8) \
		{ \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		*__buf++ = *__src++; \
		} \
 \
	switch (__nd) \
		{ \
	case 7: \
		*__buf++ = *__src++; \
	case 6: \
		*__buf++ = *__src++; \
	case 5: \
		*__buf++ = *__src++; \
	case 4: \
		*__buf++ = *__src++; \
	case 3: \
		*__buf++ = *__src++; \
	case 2: \
		*__buf++ = *__src++; \
	case 1: \
		*__buf++ = *__src++; \
		} \
	} while (0)
#endif /* UHIL_insdi_defined */

/*
|| UHIL_outsbi() outputs a buffer to a sequence of 8-bit ports
*/
#ifndef UHIL_outsbi_defined
# define UHIL_outsbi_defined	1
# define UHIL_outsbi(bus, port, buf, nb) do { \
	__TMS_VOLATILE __TMS_UINT8 * __dest = (__TMS_VOLATILE __TMS_UINT8 *) (port); \
	__TMS_CONST __TMS_UINT8 *__buf = (buf); \
	__TMS_UINT __nb = (nb); \
 \
	for (; __nb >= 8; __nb -= 8) \
		{ \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		} \
 \
	switch (__nb) \
		{ \
	case 7: \
		*__dest++ = *__buf++; \
	case 6: \
		*__dest++ = *__buf++; \
	case 5: \
		*__dest++ = *__buf++; \
	case 4: \
		*__dest++ = *__buf++; \
	case 3: \
		*__dest++ = *__buf++; \
	case 2: \
		*__dest++ = *__buf++; \
	case 1: \
		*__dest++ = *__buf++; \
		} \
	} while (0)
#endif /* UHIL_outsbi_defined */

/*
|| UHIL_outswi() outputs a buffer to a sequence of 16-bit ports
*/
#ifndef UHIL_outswi_defined
# define UHIL_outswi_defined	1
# define UHIL_outswi(bus, port, buf, nw) do { \
	__TMS_VOLATILE __TMS_UINT16 * __dest = (__TMS_VOLATILE __TMS_UINT16 *) (port); \
	__TMS_CONST __TMS_UINT16 *__buf = (buf); \
	__TMS_UINT __nw = (nw); \
 \
	for (; __nw >= 8; __nw -= 8) \
		{ \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		} \
 \
	switch (__nw) \
		{ \
	case 7: \
		*__dest++ = *__buf++; \
	case 6: \
		*__dest++ = *__buf++; \
	case 5: \
		*__dest++ = *__buf++; \
	case 4: \
		*__dest++ = *__buf++; \
	case 3: \
		*__dest++ = *__buf++; \
	case 2: \
		*__dest++ = *__buf++; \
	case 1: \
		*__dest++ = *__buf++; \
		} \
	} while (0)
#endif /* UHIL_outswi_defined */

/*
|| UHIL_outsdi() outputs a buffer to a sequence of 32-bit ports
*/
#ifndef UHIL_outsdi_defined
# define UHIL_outsdi_defined	1
# define UHIL_outsdi(bus, port, buf, nd) do { \
	__TMS_VOLATILE __TMS_UINT32 * __dest = (__TMS_VOLATILE __TMS_UINT32 *) (port); \
	__TMS_CONST __TMS_UINT32 *__buf = (buf); \
	__TMS_UINT __nd = (nd); \
 \
	for (; __nd >= 8; __nd -= 8) \
		{ \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		*__dest++ = *__buf++; \
		} \
 \
	switch (__nd) \
		{ \
	case 7: \
		*__dest++ = *__buf++; \
	case 6: \
		*__dest++ = *__buf++; \
	case 5: \
		*__dest++ = *__buf++; \
	case 4: \
		*__dest++ = *__buf++; \
	case 3: \
		*__dest++ = *__buf++; \
	case 2: \
		*__dest++ = *__buf++; \
	case 1: \
		*__dest++ = *__buf++; \
		} \
	} while (0)
#endif /* UHIL_outsdi_defined */

/*
|| UHIL_CpuBarrierRead() expands to a CPU-level read barrier.
|| Any reads in the code that appear after this barrier must be scheduled to
|| occur after this statement. For an acquire read-lock, UHIL_CpuBarrierRead()
|| usually occurs immediately *after* the read that is being guarded, because
|| it ensures that all reads *before* the call occur before all reads after
|| the call in question.  
||
|| Without any information about the compiler, memmapio.h simply expands this call
|| as a no-op.
*/
#ifndef UHIL_CpuBarrierRead_defined
# define UHIL_CpuBarrierRead_defined	1
# define UHIL_CpuBarrierRead(hBus)	__TMS_SKIP()
#endif /* UHIL_CpuBarrierRead_defined */

/*
|| UHIL_CpuBarrierWrite() expands to a CPU-level read barrier.
|| Any reads in the code that appear after this barrier must be scheduled to
|| occur after this statement. For an release write-barrier, UHIL_CpuBarrierWrite()
|| usually occurs immediately *before* the write that is being guarded, because
|| it ensures that all writes coded *before* the call occur before all writes
|| after the call in question. 
||
|| Without any information about the compiler, memmapio.h simply expands this call
|| as a no-op.
*/
#ifndef UHIL_CpuBarrierWrite_defined
# define UHIL_CpuBarrierWrite_defined	1
# define UHIL_CpuBarrierWrite(hBus)	__TMS_SKIP()
#endif /* UHIL_CpuBarrierWrite_defined */

/*
|| UHIL_CpuBarrierGeneral() expands to a CPU-level moemory barrier.
|| Any memory accesses in the code that appear subsequent to this barrier must
|| be scheduled to occur after this statement. This operation affects hardware
|| and compiler reordering, and allows (in addition) reasoning about what any
|| read after a general barrier will see -- it will certainly see the result
|| of any write before the barrier.
||
|| Without any information about the compiler, memmapio.h simply expands this call
|| as a no-op.
*/
#ifndef UHIL_CpuBarrierGeneral_defined
# define UHIL_CpuBarrierGeneral_defined	1
# define UHIL_CpuBarrierGeneral(hBus)	__TMS_SKIP()
#endif /* UHIL_CpuBarrierGeneral_defined */

/**** end of memmapio.h ****/
#endif /* _MEMMAPIO_H_ */
