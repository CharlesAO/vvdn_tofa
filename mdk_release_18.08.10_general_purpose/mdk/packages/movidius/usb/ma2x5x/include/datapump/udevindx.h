/* udevindx.h	Fri Dec 17 1999 04:58:28  tmm */

/*

Module:  udevindx.h

Function:
	Defines the standard chip-type indices for simple configuration.

Version:
	V1.50c	Fri Dec 17 1999 04:58:28	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 1999 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	December 1999

Revision history:
   1.50c  Fri Dec 17 1999 04:58:28  tmm
	Module created.

*/

#ifndef _UDEVINDX_H_		/* prevent multiple includes */
#define _UDEVINDX_H_

/****************************************************************************\
|
|	For certain autoconfig strategies, it's useful to have predefined
|	constants that are distinct, and identify the chip for which we are
|	configured.  This module defines those symbols.
|
\****************************************************************************/

/*
|| The Lucent USS820 family
*/
#define	UDEVICE_INDEX_USS820	0x100		/* generic */
#define	UDEVICE_INDEX_USS820B	0x110		/* 820b and 820c */
#define	UDEVICE_INDEX_USS820C	0x111		/* 820c only */

#define	UDEVICE_IS_USS820(x)	(((x) & ~0xFF) == UDEVICE_INDEX_USS820)

/* 0x200 -- reserved for USS620, obsolete */
/* 0x300 -- reserved for Intel 930, obsolete */

/*
|| The Philips D12
*/
#define UDEVICE_INDEX_D12	0x400		/* generic */
#define	UDEVICE_IS_D12(x)	(((x) & ~0xFF) == UDEVICE_INDEX_D12)

/*
|| The National 9602
*/
#define UDEVICE_INDEX_NS9602	0x500		/* generic */
#define	UDEVICE_IS_NS9602(x)	(((x) & ~0xFF) == UDEVICE_INDEX_NS9602)

/*
|| The Broadcom 3310 family
*/
#define UDEVICE_INDEX_BCM3310	0x600		/* generic */
#define	UDEVICE_IS_BCM3310(x)	(((x) & ~0xFF) == UDEVICE_INDEX_BCM3310)


/**** end of udevindx.h ****/
#endif /* _UDEVINDX_H_ */
