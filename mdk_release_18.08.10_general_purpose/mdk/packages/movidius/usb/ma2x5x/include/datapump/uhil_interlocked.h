/* uhil_interlocked.h	Mon May 11 2009 14:40:56 tmm */

/*

Module:  uhil_interlocked.h

Function:
	APIs for interlocked operations.  See
	notes/uhil_multiprocessor_notes.txt for a discussion of how these
	have to be used.

Version:
	V2.01a	Mon May 11 2009 14:40:56 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 2009

Revision history:
   2.01a  Mon May 11 2009 14:40:56  tmm
	7989: Module created.

*/

#ifndef _UHIL_INTERLOCKED_H_		/* prevent multiple includes */
#define _UHIL_INTERLOCKED_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UINT_PTR					/* returns prior value */
UHIL_InterlockedExchange(
	__TMS_UPLATFORM *,
	__TMS_VOLATILE __TMS_UINT_PTR *		/* pCellToModify */,
	__TMS_UINT_PTR				/* new value */
	);

__TMS_PVOID					/* returns prior value */
UHIL_InterlockedExchangePointer(
	__TMS_UPLATFORM *,
	__TMS_VOLATILE __TMS_PVOID *		/* pCellToModify */,
	__TMS_PVOID				/* new value */
	);

__TMS_UINT_PTR					/* returns prior value */
UHIL_InterlockedCompareExchange(
	__TMS_UPLATFORM *,
	__TMS_VOLATILE __TMS_UINT_PTR *		/* pCellToModify */,
	__TMS_UINT_PTR				/* new value */,
	__TMS_UINT_PTR				/* old value (must match) */
	);

__TMS_PVOID					/* returns prior value */
UHIL_InterlockedCompareExchangePointer(
	__TMS_UPLATFORM *,
	__TMS_VOLATILE __TMS_PVOID *		/* pCellToModify */,
	__TMS_PVOID				/* new value */,
	__TMS_PVOID				/* old value (must match) */
	);

__TMS_UINT_PTR					/* returns prior value */
UHIL_InterlockedExchangeAdd(
	__TMS_UPLATFORM *,
	__TMS_VOLATILE __TMS_UINT_PTR *		/* pCellToModify */,
	__TMS_UINT_PTR				/* amount to add */
	);

__TMS_UINT_PTR					/* returns new value */
UHIL_InterlockedIncrement(
	__TMS_UPLATFORM *,
	__TMS_VOLATILE __TMS_UINT_PTR *		/* pCellToModify */
	);

__TMS_UINT_PTR					/* returns new value */
UHIL_InterlockedDecrement(
	__TMS_UPLATFORM *,
	__TMS_VOLATILE __TMS_UINT_PTR *		/* pCellToModify */
	);

/* 
|| many more can be defined based on mapping types to the above -- for
|| example, we could have INT_PTR versions at practically zero cost.
|| With 32/64 it might be nice to have UINT32/UINT64 versions so that we
|| can control the width... but for now, UINT_PTR meets all our needs.
|| See wdm.h for an example of a more complete set.
*/

__TMS_END_DECLS


/**** end of uhil_interlocked.h ****/
#endif /* _UHIL_INTERLOCKED_H_ */
