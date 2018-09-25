/* kerntypes.h	Wed Nov 21 2007 05:07:25 tmm */

/*

Module:  kerntypes.h

Function:
	Kernel types in the usual form -- all the fundamental guys.

Version:
	V3.07a	Wed Nov 21 2007 05:07:25 tmm	Edit level 6

Copyright notice:
	This file copyright (C) 1996-2005, 2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	December 1996

Revision history:
   2.10a  6/29/1997  tmm
	Module copied into common library.

   3.00b  10/2/2000  tmm
	Get rid of metabracketed #includes for more portability.

   3.05g  2005-05-29  tmm
	Add ADDRBITS_PTR_UNION.

   3.07a  Wed Nov 21 2007 05:07:25  tmm
	3512:  For some reason we forgot to typedef PIOPORT, ARG_IOPORT,
	PARG_IOPORT.

*/

#ifndef _DEF_KERNTYPES_H_		/* prevent multiple includes */
#define _DEF_KERNTYPES_H_

#ifndef _DEF_STDMACS_H_
# include "def/stdmacs.h"	/* get the standard and macros. */
#endif

/* mark component version:  last updated at V3.07a */
#define	__TMS_LIBPORT_KERNTYPES_VERSION		\
	__TMS_LIBPORT_VERSION_CALC(3,7,1,0)

/* Demangle the names we need */
#define	USE_SELECTORS __TMS_USE_SELECTORS

#define	S(t)	__TMS_S(t)	/* return selector type corresponding to t */

typedef	__TMS_ARBPTR	ARBPTR;	/* replaces TEXT * in future usages,
				|| and will eventually change to
				|| VOID * if we are lucky.
				*/

typedef	__TMS_PHYSADDR PHYSADDR; /* physical addresses fit in this integral */

typedef	__TMS_ADDRBITS ADDRBITS; /* bit images of addresses */

typedef	__TMS_ADDRBITS_PTR_UNION
	ADDRBITS_PTR_UNION, *PADDRBITS_PTR_UNION;

typedef __TMS_ADDRDIFF ADDRDIFF; /* address differences are signed diffs of
				 || ADDRBITS
				 */

typedef __TMS_SELECTOR SELECTOR;
				/* holds a "segment number" -- i.e.
				|| a pointer to an object that was
				|| once segment aligned.
				*/

/**** secondary derived types ****/
typedef __TMS_CPUFLAGS	CPUFLAGS;	/* holds cpu psw */
typedef __TMS_ARG_CPUFLAGS ARG_CPUFLAGS; /* widened */
typedef	__TMS_PARAS	PARAS;		/* holds #/ paragraphs */
typedef __TMS_ARG_PARAS ARG_PARAS;	/* widened */
typedef __TMS_RECSIZE	RECSIZE;	/* record size */
typedef __TMS_ARG_RECSIZE ARG_RECSIZE;	/* widened */
typedef	__TMS_IOEDITCH	IOEDITCH;	/* holds UCHAR, or -1, and is efficient */
typedef	__TMS_ARG_IOEDITCH ARG_IOEDITCH; /* widened */

/**** conversions ****/
#define	PARAStoADDRBITS(P)	__TMS_PARAStoADDRBITS(P)
#define ADDRBITStoPARAS(A)	__TMS_ADDRBITStoPARAS(A)
#define	ADDRBITStoPARAS_ROUND(A) __TMS_ADDRBITStoPARAS_ROUND(A)
						/* convert to paragraphs,
						|| rounding up.
						*/

/* I/O port addresses */
typedef	__TMS_IOPORT	IOPORT, *PIOPORT;	/* holds an I/O address */

/* ARG_IOPORT holds an I/O address, widened to int/uint */
typedef	__TMS_ARG_IOPORT ARG_IOPORT, *PARG_IOPORT;

/**** end of kerntypes.h ****/
#endif /* _DEF_KERNTYPES_H_ */
