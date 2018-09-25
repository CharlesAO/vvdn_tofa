/* usbpumpobjectapi.h	Fri Jan 07 2011 17:01:19 chwon */

/*

Module:  usbpumpobjectapi.h

Function:
	API functions for working with objects.  These are mostly for
	use by higher level clients, and so are separated from the base
	type definitions.

Version:
	V3.01d	Fri Jan 07 2011 17:01:19 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2002-2005, 2007, 2009, 2011 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Sat Jul 27 2002 12:58:13  tmm
	Module created.

   1.91b  Mon Apr 04 2005 15:41:17 maw
   	Added defs for UsbPumpObject_FindHandle, UsbPumpObject_NewHandle 
   	and UsbPumpObject_GetPlatform.

   1.91h  2005-08-14  tmm
	Add def for UsbPumpObject_FindOrCreateClass(), and put things back
	in alphabetical order.

   1.91i  Wed Sep 28 2005 23:28:44  tmm
	Correct API definition for UsbPumpObject_FindHandle and 
	UsbPumpObject_NewHandle()

    1.97j  Tue Nov 20 2007 07:57:32  tmm
	3505:  add UsbPumpObject_GetInterruptSystem for virtual interrupt 
	discipline.

   2.01a  Mon Mar 09 2009 11:50:26  chwon
	7536: add UsbPumpObject_EnumerateClassChildren prototype.

   3.01d  Fri Jan 07 2011 11:04:22  chwon
	12315: add UsbPumpObject_FindAndSetDebugFlags(),
	UsbPumpObject_GetDebugFlags() and UsbPumpObject_SetDebugFlags().

   3.01d  Fri Jan 07 2011 17:01:19  chwon
	12315: change UsbPumpObject_FindAndSetDebugFlags input parameter.
	It takes ulFlagsMask (a NAND mask) and ulFlagsBits (an OR mask).

*/

#ifndef _USBPUMPOBJECTAPI_H_		/* prevent multiple includes */
#define _USBPUMPOBJECTAPI_H_

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

/****************************************************************************\
|
|	Library functions for working with objects.
|
\****************************************************************************/


__TMS_BEGIN_DECLS

__TMS_USBPUMP_OBJECT_HEADER *
UsbPumpObject_EnumerateClassChildren(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pClassObject */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pLastObject */
	);

__TMS_USBPUMP_OBJECT_HEADER *
UsbPumpObject_EnumerateMatchingNames(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pDirObject */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pLastObject */,
	__TMS_CONST __TMS_TEXT *	/* pPattern */
	);

__TMS_USBPUMP_OBJECT_HEADER * 
UsbPumpObject_FindHandle(
	__TMS_USBPUMP_OBJECT_ROOT *	/* pRoot */,
	__TMS_USBPUMP_OBJECT_HANDLE	/* hObject */
	);

__TMS_VOID
UsbPumpObject_FindAndSetDebugFlags(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pDirObject */,
	__TMS_CONST __TMS_TEXT *	/* pPattern */,
	__TMS_UINT32			/* ulDebugFlagsMask */,
	__TMS_UINT32			/* ulDebugFlagsBits */
	);

__TMS_USBPUMP_OBJECT_CLASS *
UsbPumpObject_FindOrCreateClass(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pDirObject */,
	__TMS_CONST __TMS_TEXT *	/* pName */
	);
	
__TMS_USBPUMP_OBJECT_HEADER *
UsbPumpObject_FunctionOpen(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pFunctionObject */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pClientObject */,
	__TMS_VOID *			/* pClientContext */,
	__TMS_USBPUMP_IOCTL_RESULT *	/* pResult -- OPTIONAL */
	);

__TMS_BOOL
UsbPumpObject_FunctionClose(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pIoObject */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pClientObject */,
	__TMS_USBPUMP_IOCTL_RESULT *	/* pResult -- OPTIONAL */
	);

__TMS_UINT32
UsbPumpObject_GetDebugFlags(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */
	);

__TMS_UDEVICE *
UsbPumpObject_GetDevice(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */
	);

__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE *
UsbPumpObject_GetInterruptSystem(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */
	);

__TMS_UPLATFORM *
UsbPumpObject_GetPlatform(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */
	);

__TMS_USBPUMP_OBJECT_ROOT *
UsbPumpObject_GetRoot(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */
	);

__TMS_USBPUMP_OBJECT_HANDLE 
UsbPumpObject_NewHandle(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */
	);

__TMS_VOID
UsbPumpObject_SetDebugFlags(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */,
	__TMS_UINT32			/* ulDebugFlags */
	);

__TMS_END_DECLS


/**** end of usbpumpobjectapi.h ****/
#endif /* _USBPUMPOBJECTAPI_H_ */
