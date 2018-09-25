/* usbseri_oslinkage.h	Thu Oct 23 2014 14:42:53 chwon */

/*

Module:  usbseri_oslinkage.h

Function:
	Abstract bindings to simple synch primitives for USBSERI.

Version:
	V3.13b	Thu Oct 23 2014 14:42:53 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2006, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2006

Revision history:
   1.00a  Mon May 22 2006 14:33:19  tmm
	Module created.

   3.13b  Thu Oct 23 2014 14:42:53  chwon
	18595: Add USBSERI_OSLINKAGE_BEGIN_DECLS and USBSERI_OSLINKAGE_END_DECLS.

*/

#ifndef _USBSERI_OSLINKAGE_H_		/* prevent multiple includes */
#define _USBSERI_OSLINKAGE_H_

#ifdef __cplusplus
# define USBSERI_OSLINKAGE_BEGIN_DECLS	extern "C" {
# define USBSERI_OSLINKAGE_END_DECLS	}
#else
# define USBSERI_OSLINKAGE_BEGIN_DECLS	/* nothing */
# define USBSERI_OSLINKAGE_END_DECLS	/* nothing */
#endif

/*
|| At compile time, USBSERI_OS_ABSTRACT_EVENT_SIZE must be set large enough
|| to hold whatever is needed by Usbseri_OS_InitializeEvent(), 
|| Usbseri_OS_SetEvent(), and Usbseri_OS_WaitForEvent().
||
|| We do not want to include OS-specific header files in the implementation
|| code, so instead we use this approach.
*/

#ifndef USBSERI_OS_ABSTRACT_EVENT_SIZE	/* PARAM */
# define USBSERI_OS_ABSTRACT_EVENT_SIZE	16
#endif

typedef	struct USBSERI_OS_ABSTRACT_EVENT
	{
	/* 
	|| filler that is overlaid by the actual OS event, whatever it
	|| might be.
	*/
	void	*Filler[USBSERI_OS_ABSTRACT_EVENT_SIZE / sizeof(void*)];
	} USBSERI_OS_ABSTRACT_EVENT;

/****************************************************************************\
|
|	APIs
|
\****************************************************************************/

USBSERI_OSLINKAGE_BEGIN_DECLS

void 
Usbseri_OS_InitializeEvent(
	USBSERI_OS_ABSTRACT_EVENT *pEvent,
	unsigned sizeEvent
	);

void
Usbseri_OS_DeinitializeEvent(
	USBSERI_OS_ABSTRACT_EVENT *pEvent
	);

int
Usbseri_OS_WaitForEvent(
	USBSERI_OS_ABSTRACT_EVENT *pEvent
	);

void
Usbseri_OS_SetEvent(
	USBSERI_OS_ABSTRACT_EVENT *pEvent
	);

USBSERI_OSLINKAGE_END_DECLS

/**** end of usbseri_oslinkage.h ****/
#endif /* _USBSERI_OSLINKAGE_H_ */
