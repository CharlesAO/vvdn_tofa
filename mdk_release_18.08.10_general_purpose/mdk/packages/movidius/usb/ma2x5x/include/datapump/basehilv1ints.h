/* basehilv1ints.h	Mon May 27 2002 19:11:33  tmm */

/*

Module:  basehilv1ints.h

Function:
	Deprecated V1 interrupt structures.

Version:
	V1.64a	Mon May 27 2002 19:11:33	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.64a  Mon May 27 2002 19:11:33  tmm
	Module created.

*/

#ifndef _BASEHILV1INTS_H_		/* prevent multiple includes */
#define _BASEHILV1INTS_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

/****************************************************************************\
|
|	Deprecated interrupt hooking routines.
|
\****************************************************************************/


__TMS_FNTYPE_DEF(
	INTFN,
	__TMS_VOID,
		(
		__TMS_ARG_UINT8 /*intnum*/,
		__TMS_VOID * /* ctx */
		)
	);

__TMS_BEGIN_DECLS
__TMS_PINTFN	UHIL_HookInterrupt __TMS_P((
			__TMS_ARG_UINT8 intnum,
			__TMS_PINTFN newfn, 
			__TMS_VOID *ctx
			));
__TMS_END_DECLS

/**** end of basehilv1ints.h ****/
#endif /* _BASEHILV1INTS_H_ */
