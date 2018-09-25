/* usbpumproot.h	Sat Jul 27 2002 12:48:11 tmm */

/*

Module:  usbpumproot.h

Function:
	Home for USBPUMP_OBJECT_ROOT and related.

Version:
	V1.79a	Sat Jul 27 2002 12:48:11 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Thu Jul 11 2002 13:58:18  tmm
	Module created.

*/

#ifndef _USBPUMPROOT_H_		/* prevent multiple includes */
#define _USBPUMPROOT_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

/*

Type:	USBPUMP_OBJECT_ROOT

Function:
	The "root object" structure.

Description:
	The USB DataPump's object structure is rooted in this object.
	All other objects should, directly or indirectly, rever to this
	object via tha class hierarchy.

	The root object maintains a directory of objects by name; it
	also maintains a list of all objects that have been registered
	with it.  (These sets are logically disjoint -- neither is
	necessarily a subset of the other.)

	This object is normally created as soon as the UPLATFORM
	is created, by the standard UPLATFORM_SETUP macro.  

Contents:
	USBPUMP_OBJECT_HEADER Header;
		The root object is itself an object.  It is registered
		with itself by name, as "root.mcci.com".

	UPLATFORM *pPlatform;
		The platform pointer registered as the "primary"
		platform.

	USBPUMP_OBJECT_HEADER *pDirectoryHead;
		The head of the list of the named objects registered here
		with USBPUMP_IOCTL_ADD_NAMED_OBJECT.

		This list is maintained in unsorted order, but with all
		similarly-named instances collected together, in sequential
		order of registration.

	USBPUMP_OBJECT_HEADER *pObjectListHead;
		The head of the raw list of objects registered here with
		USBPUMP_IOCTL_ADD_CHILD 

Notes:
	If, for some reason, you have multiple UPLATFORMs, you need to
	be careful initializing UPLATFORMs after the first.

	Customer-supplied code should not use any of the contents of the
	root object directly; instead, such code should use the query
	IOCTLs where possible.

See Also:
	USBPUMP_OBJECT_HEADER
	USBPUMP_IOCTL_ADD_CHILD
	USBPUMP_IOCTL_ADD_NAMED_OBJECT
	USBPUMP_IOCTL_GET_OBJECT_ROOT
	USBPUMP_IOCTL_GET_UPLATFORM

*/

struct __TMS_STRUCTNAME(USBPUMP_OBJECT_ROOT)
	{
	__TMS_USBPUMP_OBJECT_HEADER	Header;

	/*
	|| The root implementation may need to use platform routines;
	|| so it stashes a UPLATFORM pointer.
	*/
	__TMS_UPLATFORM			*pPlatform;

	/*
	|| The root implementation manages the object list, and needs
	|| a list head pointer.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	*pDirectoryHead;

	/*
	|| The root implementation similarly needs a pointer to the total
	|| list of objects connected here (not necessarily all in the
	|| dictionary).
	*/
	__TMS_USBPUMP_OBJECT_HEADER	*pInstanceHead;
	};

#define	__TMS_USBPUMP_OBJECT_ROOT_TAG	__TMS_UHIL_MEMTAG('R', 'o', 'o', 't')

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_OBJECT_ROOT_TAG	__TMS_USBPUMP_OBJECT_ROOT_TAG
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPumpObject_RootInit(
	__TMS_USBPUMP_OBJECT_ROOT *pRoot,
	__TMS_UPLATFORM *pPlatform
	);

__TMS_END_DECLS

/**** end of usbpumproot.h ****/
#endif /* _USBPUMPROOT_H_ */
