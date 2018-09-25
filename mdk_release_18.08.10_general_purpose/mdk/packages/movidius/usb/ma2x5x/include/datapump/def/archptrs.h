/* def/archptrs.h	Sat Mar 10 2001 15:39:00 tmm */

/*

Module:  def/archptrs.h

Function:
	Architecture-specific bindings for the 32-bit Sparc.
	
Version:
	V3.02d	Sat Mar 10 2001 15:39:00 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2001 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2001

Revision history:
   3.02d  Sat Mar 10 2001 15:39:00  tmm
	Module created.

*/

#ifndef _DEF_ARCHPTRS_H_		/* prevent multiple includes */
#define _DEF_ARCHPTRS_H_

/****************************************************************************\
|
|	Here come the standard types....
|
\****************************************************************************/

#ifndef _DEF_STDMACS_H_	/* get the standard types, uncloaked */
# include "def/stdmacs.h"
#endif

/**** define the standard pointer manipulators as macros ****/
#define	canonical_ptr(P)	((ARBPTR)(P))
#define makeaddrbits(P)		((ADDRBITS)(P))
#define makelong2(lo, hi)	((LONG)((((ULONG)(USHORT)(hi)) << 16) | \
				       (ULONG)(USHORT)(lo)))
#define makeptr(P)		((ARBPTR)((ADDRBITS)(P)))
#define makeptr2(off, sel)	(makeptr((ADDRBITS) (off) + (ADDRBITS) (sel)))
#define offset2ptr(off)		(makeptr(off))
#define ptr2offset(P)		makeaddrbits(P)

#define	sel2ptr(s)		offset2ptr(s)
#define	ptr2sel(P)		makeaddrbits(P)

/**** end of def/archptrs.h ****/
#endif /* _DEF_ARCHPTRS_H_ */
