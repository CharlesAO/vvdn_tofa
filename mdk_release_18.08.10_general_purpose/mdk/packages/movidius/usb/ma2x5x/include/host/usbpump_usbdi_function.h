/* usbpump_usbdi_function.h	Tue Aug 10 2010 18:43:17 chwon */

/*

Module:  usbpump_usbdi_function.h

Function:
	Home of USBPUMP_USBDI_FUNCTION

Version:
	V3.01c	Tue Aug 10 2010 18:43:17 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2005-2007, 2010 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Sun Jun 19 2005 03:36:16  tmm
	Module created.

   1.97h  Thu Mar 29 2007 13:23:18  djt
	Fixed "*.fn.usbdi.mcci.com" pattern name.

   3.01c  Tue Aug 10 2010 18:43:17  chwon
	10911: move USBPUMP_USBDI_INSTANCE_INFO type definition to
	usbpump_usbdi_types.h.

*/

#ifndef _USBPUMP_USBDI_FUNCTION_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_FUNCTION_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

/****************************************************************************\
|
|	The USBDI_INSTANCE_INFO structure is used inside instance objects
|	to represent the binding to a port.  If you change this, you must
|	also consider change the IOCTL ARG_SETUP_V1 for
|	USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL.
|
\****************************************************************************/

/*

Type:	USBPUMP_USBDI_INSTANCE_INFO

Function:
	Represents the binding of a USB function driver to a physical
	device port.

Description:
	Each instance of a USB function driver has to be connected to the
	device that it's managing.  When the device arrives, the driver
	instance needs to know the USB Port to which the device is connected,
	the USBDI handle for the default pipe to the device, and the size of
	the URB and HCD request areas.  The USBPUMP_USBDI_INSTANCE_INFO
	object provides this information.

	Since USB Port objects are reused from device insertion to device
	insertion, and because the user might plug in a different device, the
	USBPUMP_USBDI_INSTANCE_INFO also provides some session
	information, in order to prevent a driver from mistakenly attempting
	to talk to the wrong device using a stale port pointer.

Contents:
	USBPUMP_USBDI_PORT *pPort;
		The port reference.

	USBPUMP_USBDI_PORT_KEY PortKey;
		The port key which matches a given URB request to a given
		instance of the device.  This value is supplied by USBDI, and
		will change on each plug-in.  Therefore any request that is
		lurking in a slow transport can be rejected if it arrives
		after the device departs.

	RECSIZE	UrbTotalSize;
		The total size of the URB object.  If the function driver
		wishes to be loosely coupled to USBDI, it must allocate
		urbs of this size, and then use methods to initialize and
		access the URBs.

	RECSIZE UrbExtraSize;
		The required extra bytes to be added to each URB in order to
		provide the space needed by the USBDI system and HCD for
		processing the URB.

	RECSIZE HcdRequestSize;
		The portion of the extra bytes that is to be allocated for
		the HCD request.

	USBPUMP_USBDI_PIPE_HANDLE hDefaultPipe;
		The handle of the default pipe for this device.

	USBPUMP_DEVICE_SPEED	OperatingSpeed;
		The operating speed of the device.

	UINT8 bTier;
		The tier of thie port.

	UINT16 wMaxPower;
		The maximum power availalbe for use by this port, in units
		of 1 mA (not 2mA, these are natural units).

Notes:
	The HcdRequestSize is always less than or equal to the UrbExtraSize;
	it's included just as another means of error checking.  The client
	is required to pass this down in each URB, which allows USBDI and
	HCDs to verify that the HCD request is really of the appropriate size.

See Also:
	USBPUMP_URB, USBPUMP_USBDI_FUNCTION.

*/

/* in usbpump_usbdi_types.h:
|| __TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_INSTANCE_INFO);
*/

struct __TMS_STRUCTNAME	(USBPUMP_USBDI_INSTANCE_INFO)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;		/* pointer to USBD port */
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;	/* the port key which
							|| authenticates us
							*/
	__TMS_RECSIZE	UrbTotalSize;			/* base URB size */
	__TMS_RECSIZE	UrbExtraSize;			/* required URB extra size in bytes */
	__TMS_RECSIZE	HcdRequestSize;			/* required HCD request size,
							|| in bytes
							*/
	__TMS_USBPUMP_USBDI_PIPE_HANDLE hDefaultPipe;	/* the default pipe */
	__TMS_UINT16			wMaxPower;	/* max power in 1mA ticks */
	__TMS_USBPUMP_DEVICE_SPEED	OperatingSpeed;	/* the operating speed */
	__TMS_UINT8			bTier;		/* tier of this device */
	};

/*

Type:	USBPUMP_USBDI_FUNCTION

Index:	Type:	USBPUMP_USBDI_FUNCTION_CONTENTS
	Macro:	USBPUMP_USBDI_FUNCTION_CONTENTS__STRUCT
	Macro:	USBPUMP_USBDI_FUNCTION_CONTENTS__UNION
	Constant: USBPUMP_USBDI_FUNCTION_TAG
	Macro:	USBPUMP_USBDI_FUNCTION_NAME()

Function:
	Represents a single function-driver instance to USBDI.

Description:
	Each class driver owns a collection of USBPUMP_USBDI_FUNCTION
	objects; these objects are created during class-driver initialization,
	and are dynamically connected to USBPUMP_USBDI_PORT objects based
	on class type matching.

	These function objects are DataPump extensible objects, and so this
	type can be thought of as an abstract view of the concrete function
	object, exported by the concrete class driver.

	These function objects are furthermore part of the DataPump class
	system, so they can be located using
	UsbPumpObject_EnumerateMatchingNames().  Names are normally
	constructed at compile time using the USBPUMP_USBDI_FUNCTION_NAME()
	macro.

Contents:
	USBPUMP_OBJECT_HEADER ObjectHeader;
		The normal DataPump object header.  USBPUMP_USBDI_FUNCTION_TAG
		is used as the tag for all function objects.

	USBPUMP_ABSTRACT_POOL *Function.pPool;
		An abstract pool created at init time for use by the
		function instance driver for dynamic allocations that
		last the lifetime of a single plugin of a device.

	USBPUMP_USBDI_INSTANCE_INFO Function.PortInfo;
		The connection information related to this connection
		to the port.  In particular, Function.PortInfo.pPort
		points to the USBPUMP_USBDI_PORT for the current
		session.

	TEXT *pDeviceId;
		Pointer to the most recent device ID that was assigned to
		this device (or NULL)

	RECSIZE sizeDeviceIdBuffer;
		Size of the device ID buffer.

	TEXT *pInstanceId;
		Pointer to the most recent instance ID -- this is actually
		a null-terminated UTF8 encoding of the serial number.

	RECSIZE sizeInstanceIdBuffer;
		Size of the instance ID buffer.

Notes:
	All function objects can be matched with the pattern
		"*.fn.usbdi.mcci.com"

	The macro:

		USBPUMP_USBDI_FUNCTION_NAME("name")

	generates a string consisting of "name" followed by
	".fn.usbdi.mcci.com".  Therefore, the appropriate way to
	match all functions is to match the string

		USBPUMP_USBDI_FUNCTION_NAME("*")

See Also:
	USBPUMP_USBDI_INSTANCE_INFO, USBPUMP_USBDI_PORT.

*/


/* in usbpump_usbdi_types.h:
|| __TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_FUNCTION);
*/

/* XXX needs a serial-number buffer "from the last time we matched" */
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_FUNCTION_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_FUNCTION_CONTENTS)
	{
	/*
	|| Function.ObjectHeader.pClassParent points to the owning
	|| USBPUMP_USBDI_DRIVER_CLASS object.
	||
	|| Function.ObjectHeader.ClassSiblings point to next and previous
	|| instance in this set of instances.
	*/
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	/*
	|| Architecturally, every function is required to have an
	|| abstract pool.  This pool is created by the class creation
	|| framework.  Of course, the normal framework functions will
	|| behave deterministically if the pointer is NULL.
	*/
	__TMS_USBPUMP_ABSTRACT_POOL	*pPool;

	/*
	|| Function.UsbdiInfo.pPort points to the owning USBDI port, or to NULL
	|| if not bound to a port, and so forth.  This information is
	|| updated by USBDI when it binds an instance to a port.
	*/
	__TMS_USBPUMP_USBDI_INSTANCE_INFO  PortInfo;

	/*
	|| Bookkeeping fields for (attempting to) keep devices showing up
	|| where they last showed up.
	*/
	__TMS_TEXT *	pDeviceId;			/* pointer to devid */
	__TMS_RECSIZE	sizeDeviceIdBuffer;		/* size of buffer */
	__TMS_TEXT *	pInstanceId;			/* pointer to inst id */
	__TMS_RECSIZE	sizeInstanceIdBuffer;		/* size of buffer */

	/*
	|| In order to enable "after-the-fact" drivers, or dynamically
	|| loaded drivers, we include a VOID* for client use.  This is
	|| equivalent to the device extension pointer in WDM.
	*/
	__TMS_VOID *			pFunctionPrivateInfo;
	};


union __TMS_UNIONNAME	(USBPUMP_USBDI_FUNCTION)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;
	__TMS_USBPUMP_USBDI_FUNCTION_CONTENTS	Function;
	};

/* the function contents macros */
#define	__TMS_USBPUMP_USBDI_FUNCTION_CONTENTS__STRUCT			\
	__TMS_USBPUMP_USBDI_FUNCTION_CONTENTS	Function

#define	__TMS_USBPUMP_USBDI_FUNCTION_CONTENTS__UNION			\
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;				\
	__TMS_USBPUMP_USBDI_FUNCTION_CONTENTS	Function;		\
	__TMS_USBPUMP_USBDI_FUNCTION		FunctionCast

/*
|| Object-related things
*/

/* the USB function object tag */
#define	__TMS_USBPUMP_USBDI_FUNCTION_TAG				\
			__TMS_UHIL_MEMTAG('U', 'C', 'f', 'n')

/* the name generator */
#define	__TMS_USBPUMP_USBDI_FUNCTION_NAME(x)				\
			x __TMS_USBPUMP_USBDI_NAME(".fn")

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_FUNCTION_CONTENTS__STRUCT	\
   __TMS_USBPUMP_USBDI_FUNCTION_CONTENTS__STRUCT
# define USBPUMP_USBDI_FUNCTION_CONTENTS__UNION	\
   __TMS_USBPUMP_USBDI_FUNCTION_CONTENTS__UNION
# define USBPUMP_USBDI_FUNCTION_TAG	\
   __TMS_USBPUMP_USBDI_FUNCTION_TAG
# define USBPUMP_USBDI_FUNCTION_NAME(x)	\
   __TMS_USBPUMP_USBDI_FUNCTION_NAME(x)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_function.h ****/
#endif /* _USBPUMP_USBDI_FUNCTION_H_ */
