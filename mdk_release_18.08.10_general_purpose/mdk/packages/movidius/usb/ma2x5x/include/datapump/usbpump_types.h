/* usbpump_types.h	Wed Mar  4 2015 18:13:30 tmm */

/*

Module:  usbpump_types.h

Function:
	define the basic type tags -- useful when interfacing to
	other code subsystems and the full "usbpump.h" cannot be included.

Version:
	V3.15b	Wed Mar  4 2015 18:13:30 tmm	Edit level 21

Copyright notice:
	This file copyright (C) 2001-2008, 2011-2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	January 2001

Revision history:
   1.61d  Sat Jan  6 2001 19:39:28  tmm
	Module created from pieces of usbpump.h

   1.63i  5/19/2002  tmm
	Added USBPUMP_OBJECT_HEADER, USBPUMP_OBJECT_LIST.

   1.79a  5/26/2002  tmm
	Sort by name; and include the desccriptor types.  Also add more of the
	types from "ueventnode.h"

   1.79a  6/2/2002  tmm
	Add the UDATASTREAM object.

   1.79a  7/11/2002  tmm
	Add the USBPUMP_OBJECT_ROOT type.

   1.87b  2004-03-10  tmm
	1229: Documentation changes only:  docs for LENOF.

   1.91b  Sun Apr 24 2005 11:56:47 maw
   	Added USETUP_HANDLE

   1.91d  2005-06-02  tmm
	Add USBPUMP_DEVICE_SPEED type so we can use it in function
	prototypes (without having to include the definitions).

   1.91d  2005-06-17  tmm
	Add USPUMP_DEVICE_SPEED_MASK type, for same reason.

   1.91d  Mon Jul 04 2005 11:39:15  chwon
	Add USBPUMP_MILLISECONDS type.

   1.91h  Fri Aug 26 2005 15:04:29  chwon
	Add USBPUMP_TIMER and USBPUMP_TIMER_SWITCH type.

   1.91h  Thu Sep  1 2005 23:56:08  tmm
	Add USBPUMP_HCD type.

   1.91h  Fri Sep 09 2005 17:36:03  chwon
	Add UTF32, UTF16 and UTF8 type.

   1.91i  Sat Sep 24 2005 00:18:07  chwon
	Move USBPUMP_HCD type to usbpump_hcd_types.h.

   1.97j  Thu Nov 01 2007 10:50:31  cccho
	4248: add USBPUMP_ALLOCATION_TRACKING type.

   1.97k  Mon Feb  4 2008 17:20:59  tmm
	3735:  as part of the general UBUFQE reorganization, add UBUFQE_TIMEOUT,
	UBUFQE_LENGTH, UBUFQE_REQUEST type definition.

	3747: bring USBPUMP_ISOCH_PACKET_DESCR and USBPUMP_BUFFER_POINTER
	into this file.  (Possibly wrong place, needs review.)

   1.97k  Fri Jul 18 2008 09:30:12  chwon
	4312: add USBPUMP_BUFFER_HANDLE type

   3.01f  Thu Sep 22 2011 14:00:45  cccho
	13233: add UDEVICE_ENDPOINT_ACTIVITY_STATE type. move
	UDEVICE_LINK_STATE type from udevice.h	

   3.01f  Thu Feb 09 2012 16:06:21  chwon
	14805: change USBPUMP_ISOCH_PACKET_DESCR::uOffset from BYTES to UINT32.
	14804: add USBPUMP_FRAME_NUMBER structure.

   3.01f  Fri Mar 30 2012 10:16:15  cccho
	15053: add USBPUMP_POWER_STATE and USBPUMP_POWER_ACTION types.

   3.11c  Tue Feb 26 2013 22:56:36  chwon
	16402: 16435: Added USBPUMP_CONTROL_PROCESS_STATUS and
	USBPUMP_CONTROL_PROCESS_FN types.

   3.15b  Wed Mar  4 2015 18:13:30  tmm
	19019: documentation cleanup.

*/

#ifndef _USBPUMP_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_TYPES_H_

#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

#ifndef _BASEHILTYPES_H_
# include "basehiltypes.h"
#endif

#ifndef _USBDESC_TYPES_H_
# include "usbdesc_types.h"
#endif

/****************************************************************************\
|
|	The various object types that everyone needs.
|
\****************************************************************************/

/*

Name:	LENOF()

Index:	Name:	__TMS_LENOF()

Function:
	C macro, calculates the number of entries in an array whose size is
	known at compile time.

Definition:
	size_t LENOF(
		Array_Variable Array
		);

Description:
	This convenience macro calculates the number of elements in
	|Array|.  It is equivalent to (sizeof(Array) / sizeof(Array[0])).

	The C builtin |sizeof(Array)| returns the size of |Array| in
	bytes.  By comparison, |LENOF(Array)| returns ths number of
	elements in the array, which is more useful for iterating
	over the elements in the array.

	Even though the size of an array is often given by a manifest
	constant, it's easy to slip and use the wrong constant.  The LENOF 
	macro avoids the problem, by computing the length of precisely the
	object whose length you want to know.

Returns:
	The number of elements in the Array.

Bugs:
	If you use LENOF() with a pointer to an array, the resulting code 
	will compile, but will get the wrong answer.  The size of any pointer 
	is typically 4 bytes; this number has no relationship to the number 
	of elements in the array.

	Unfortunately, at least in ANSI C89, a formal function parameter
	declared as an array is converted silentely to a pointer.  So 
	if you write, for example

		size_t foo(int arg[5]) { return LENOF(arg); }

	foo() will always return 4, rather than the expected 20 (on 32-bit,
	or more specifically ILP32, systems).

*/

/**** the useful macros ****/
#ifndef __TMS_LENOF
# define __TMS_LENOF(Array)	(sizeof(Array) / sizeof((Array)[0]))
#endif

#if !__TMS_CLOAKED_NAMES_ONLY
# ifndef LENOF
#  define LENOF(Array)	(sizeof(Array) / sizeof((Array)[0]))
# endif /* LENOF */
#endif /* __TMS_CLOAKED_NAMES_ONLY */

/*
|| find the containing object.
*/
#define __TTUSB_CONTAINER_OF(p, TypeOfContainer, FieldInContainer) \
	((TypeOfContainer *) (((char *) (p)) - __TMS_OFFSET(TypeOfContainer, FieldInContainer)))

/*
|| Define a function type and its pointer type.  Three flavors are
|| possible....  with type prefix, without type prefix, or both.
*/
#define	__TTUSB_FNTYPE_DEF_PFX(Pfx, FunctionType, ResultType, ArgList) \
	typedef	ResultType (Pfx##FunctionType) ArgList; \
	typedef Pfx##FunctionType *Pfx##P##FunctionType

#define	__TTUSB_FNTYPE_DEF_RAW(FunctionType, ResultType, ArgList) \
	typedef	ResultType (FunctionType) ArgList; \
	typedef FunctionType *P##FunctionType

#define	__TTUSB_FNTYPE_DEF(FunctionType, ResultType, ArgList) \
	__TTUSB_FNTYPE_DEF_PFX(__TTUSB_, FunctionType, ResultType, ArgList); \
	__TTUSB_FNTYPE_DEF_RAW(FunctionType, ResultType, ArgList)

#define	__TTUSB_TYPE_DEF_PFX(Pfx, TypeName, TypeExpr) \
	typedef	TypeExpr Pfx##TypeName, *Pfx##P##TypeName

#define	__TTUSB_TYPE_DEF_RAW(TypeName, TypeExpr) \
	typedef	TypeExpr TypeName, *P##TypeName

#define	__TTUSB_TYPE_DEF(TypeName, TypeExpr) \
	__TTUSB_TYPE_DEF_PFX(__TTUSB_, TypeName, TypeExpr); \
	__TTUSB_TYPE_DEF_RAW(TypeName, TypeExpr)

#define	__TTUSB_TYPE_DEF_STRUCT(StructName) \
	__TTUSB_TYPE_DEF(StructName, struct __TTUSB_##StructName)

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/


/* declare the structure tags & forward types */
__TMS_TYPE_DEF(UBUFQE, struct TTUSB_UBUFQE);
__TMS_TYPE_DEF_ARG(UBUFQE_FLAGS, UINT16);
__TMS_TYPE_DEF_ARG(UBUFQE_TIMEOUT, UINT16);
__TMS_TYPE_DEF_ARG(UBUFQE_LENGTH, UINT16);
__TMS_TYPE_DEF_ARG(UBUFQE_REQUEST, UINT8);

__TMS_TYPE_DEF(UCONFIG, struct TTUSB_UCONFIG);

__TMS_TYPE_DEF_STRUCT(UDATAPLANE);

__TMS_TYPE_DEF_STRUCT(UDATASTREAM);

__TMS_TYPE_DEF(UDEVICE, struct TTUSB_UDEVICE);

__TMS_TYPE_DEF(UDEVICESWITCH, struct TTUSB_UDEVICESWITCH);

__TMS_TYPE_DEF(UENDPOINT, struct TTUSB_UENDPOINT);

__TMS_TYPE_DEF(UENDPOINTSWITCH, struct TTUSB_UENDPOINTSWITCH);

__TMS_TYPE_DEF_ARG_CLOAK(UEVENT, BYTES);
#if !__TMS_CLOAKED_NAMES_ONLY
__TMS_TYPE_DEF_DISCLOSE_ARG(UEVENT);
#endif

__TMS_TYPE_DEF(UEVENTFEATURE, struct TTUSB_UEVENT_FEATURE);
__TMS_TYPE_DEF(UEVENTNODE, struct TTUSB_EVENTNODE);
__TMS_TYPE_DEF(UEVENTSETUP, struct TTUSB_UEVENTSETUP);

__TMS_TYPE_DEF(UINTERFACE, struct TTUSB_UINTERFACE);

__TMS_TYPE_DEF(UINTERFACESET, struct TTUSB_UINTERFACESET);

__TMS_TYPE_DEF(UPIPE, struct TTUSB_UPIPE);

__TMS_TYPE_DEF_UNION	(USBPUMP_ABSTRACT_POOL);
__TMS_TYPE_DEF_ARG	(USBPUMP_DEVICE_SPEED, UINT8);
__TMS_TYPE_DEF_ARG	(USBPUMP_DEVICE_SPEED_MASK, UINT32);

__TMS_TYPE_DEF_STRUCT(USBPUMP_OBJECT_HEADER);

__TMS_TYPE_DEF_STRUCT(USBPUMP_OBJECT_LIST);

__TMS_TYPE_DEF_STRUCT(USBPUMP_OBJECT_ROOT);

/* the USBRC root-table pointer */
__TMS_TYPE_DEF(USBRC_ROOTTABLE, struct __USBRCTAB_ROOTTABLE);

__TMS_TYPE_DEF(USETUP, struct TTUSB_USETUP);

__TMS_TYPE_DEF_ARG(USTAT, UCHAR);

__TMS_TYPE_DEF_HANDLE(USETUP_HANDLE);

#define __TMS_USETUP_INVALID_HANDLE	((__TMS_USETUP_HANDLE)__TMS_NULL)

#if !__TMS_CLOAKED_NAMES_ONLY
#define USETUP_INVALID_HANDLE		__TMS_USETUP_INVALID_HANDLE
#endif

__TMS_TYPE_DEF_ARG			(USBPUMP_MILLISECONDS, UINT32);
__TMS_TYPE_DEF_STRUCT			(USBPUMP_TIMER);
__TMS_TYPE_DEF_STRUCT			(USBPUMP_TIMER_SWITCH);

__TMS_TYPE_DEF_STRUCT			(USBPUMP_ALLOCATION_TRACKING);

__TMS_TYPE_DEF_ARG(USBPUMP_UTF32_TEXT, INT32);
__TMS_TYPE_DEF_ARG(USBPUMP_UTF16_TEXT, UINT16);
__TMS_TYPE_DEF_ARG(USBPUMP_UTF8_TEXT, TEXT);

__TMS_TYPE_DEF_HANDLE(USBPUMP_BUFFER_HANDLE);	/* 4312 */

__TMS_TYPE_DEF_ARG(UDEVICE_LINK_STATE, UINT8);
__TMS_TYPE_DEF_ARG(UDEVICE_ENDPOINT_ACTIVITY_STATE, UINT8);

__TMS_TYPE_DEF_ARG(USBPUMP_POWER_STATE, UINT32);
__TMS_TYPE_DEF_ARG(USBPUMP_POWER_ACTION, UINT32);


/*
|| Because we effectively have multiple inheritance, we have a headache.
|| We need to do some data structure initialization from inside a function
|| who cannot (architecturally) know the layout of the structure.  So
|| rather than split the function into two pieces, we instead pass a callout
|| into that function.  The function prototype uses UDEVICE* to pass the
|| derived UDEVICE object, becuase we don't know the actual type.  The
|| overall size is passed through, as a cheap way of looking for compile
|| errors.
*/
/* document this in udevice.h */
__TMS_FNTYPE_DEF(
UDEVICE_INITFN,
__TMS_VOID,
	(
	__TMS_UDEVICE *,
	__TMS_SIZE_T
	));

/* lifted here from ubufqe.h -- documented there. */
__TMS_FNTYPE_DEF(
	UBUFIODONEFN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *,
		__TMS_UENDPOINT *,
		__TMS_UBUFQE *
		)
	);

/* document this in usbioctl_device.h */
__TMS_TYPE_DEF_ARG(USBPUMP_CONTROL_PROCESS_STATUS, UINT32);

/* document this in usbioctl_device.h */
__TMS_FNTYPE_DEF(
	USBPUMP_CONTROL_PROCESS_FN,
	__TMS_USBPUMP_CONTROL_PROCESS_STATUS,
		(
		__TMS_UDEVICE *			/* pDevice */,
		__TMS_VOID *			/* pClientContext */,
		__TMS_CONST __TMS_USETUP *	/* pSetup */
		)
	);

/*

Type:	USBPUMP_BUFFER_POINTER

Function:
	Basic union for dealing with CONST/non-CONST void without casts.

Description:
	Under GCC and some other compilers, casting to remove CONST is
	not permitted (and the work-around is not portable).  Sometimes
	we need buffer pointers to be either CONST or non-CONST, depending
	on the situation.  In that case, the buffer pointer is passed using
	USBPUMP_BUFFER_POINTER, a union that contains a CONST and a non-CONST
	view of the pointer.

Contents:
	VOID *pReadBuffer;
		The pointer interpreted as referring to buffer that is going to
		receive data from a driver (hence it's a "read buffer").

	CONST VOID *pcWriteBuffer;
		The pointer, interpreted as referring to a buffer that is
		going to supply data to a driver (hence it's a "write
		buffer").

Notes:
	It would be nice to have an ADDRBITS view, as well as an
	appropriately-named "read/write" buffer.  However, we don't want
	to cause bloat for our clients, this is just a way to view a pointer
	with and without CONST attributes.

*/

__TMS_TYPE_DEF_UNION(USBPUMP_BUFFER_POINTER);
union __TMS_UNIONNAME(USBPUMP_BUFFER_POINTER)
	{
	__TMS_VOID *pReadBuffer;
	__TMS_CONST __TMS_VOID *pcWriteBuffer;
	};

/*

Type:	USBPUMP_ISOCH_PACKET_DESCR

Function:
	Out-of-band isochronous packet description

Description:
	Starting with the V2 DataPump, it's possible to divide isochronous
	information up into two parts:  a pure data buffer and an array
	of "packet descriptions".  Each packet description corresponds
	to a single logical USB packet (ignoring the fact that very large
	isoch packets are split into two or three physical packets).

	The structure contains the offset (where the packet is to be placed
	relative to the base of the target buffer), the size of the packet,
	per-packet status, and a spare byte for alignment.

Contents:
	UINT32 uOffset;
		Specifies the offset of the packet buffer within the larger
		transfer buffer.  This is set by the client, and never 
		modified by USBD or the Device DataPump.

	UINT16 usLength;
		Specifies the size of the packet buffer.  If data is being
		sent from the local system, then this specifies the size of
		the data to be sent, and is not modified.  If data is being
		received, the client must initialize this to the maximum
		packet size, and USBD or the Device DataPump will change
		this to the actual number of bytes received.

	UINT8 ucStatus;
		USBD or the Device DataPump sets this to the status of 
		each packet transfer:  USTAT_OK for success or some other
		non-zero value for failure.

	UINT8 ucSpare;
		Spare byte for alignment; initialize to zero.

Notes:
	This structure is packed.

See Also:
	UISOBUFHDR, UISOBUFPKT.

*/

/****************************************************************************\
|
|	USBPUMP_ISOCH_PACKET_DESCR gives the packet-by-packet layout of
|	data buffers for isochronous transfers.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_ISOCH_PACKET_DESCR);

#include "usbpump_pushpack4.h"

__TMS_LIBPORT_PACKED4_DEFINITION(
struct __TMS_STRUCTNAME	(USBPUMP_ISOCH_PACKET_DESCR)
)
	{
	__TMS_UINT32			uOffset;
	__TMS_UINT16			usLength;
	__TMS_UINT8			ucStatus;
	__TMS_UINT8			ucSpare;
	} __TMS_LIBPORT_PACKED4_SUFFIX;

#include "usbpump_poppack.h"


/*

Type:	USBPUMP_FRAME_NUMBER

Function:
	USB frame number structre for dealing with device speed.

Description:
	Starting with the V3 DataPump, it's possible to get USB frame number
	from the chip driver.  This structure contains standard frame number,
	native frame number and factors for doing conversions.

Contents:
	UINT32 StandardFrame;
		the "standard" frame count, in milliseconds (always compatible
		with full speed), as a 32-bit number.

	UINT64 NativeFrame;
		the "native" frame count, (microframes for high speed, frames
		for full speed) -- as a 64-bit number.

	UINT32 Numerator;
	UINT32 Denominator;
		numerator and denominator for converting native frame count to
		standard frame count:  1/1 for full speed, 1/8 for high speed,
		and other values for Wireless USB or interchip USB.  (Native
		Wireless USB is 1000/1024, simplified to 125/128; and for
		interchip USB there are a number of possible values.)

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_FRAME_NUMBER);
struct __TMS_STRUCTNAME	(USBPUMP_FRAME_NUMBER)
	{
	__TMS_UINT32				StandardFrame;
	__TMS_UINT64				NativeFrame;
	__TMS_UINT32				Numerator;
	__TMS_UINT32				Denominator;
	};

/**** end of usbpump_types.h ****/
#endif /* _USBPUMP_TYPES_H_ */
