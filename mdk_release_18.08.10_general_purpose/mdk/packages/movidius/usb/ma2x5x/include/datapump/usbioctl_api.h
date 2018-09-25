/* usbioctl_api.h	Wed Dec 30 2015 17:33:05 tmm */

/*

Module:  usbioctl_api.h

Function:
	Definition of IOCTLs for DataPump API

Version:
	V3.15d	Wed Dec 30 2015 17:33:05 tmm	Edit level 11

Copyright notice:
	This file copyright (C) 2008-2009, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	January 2008

Revision history:
   1.97k  Fri Jan 11 2008 16:08:02 chwon
	3567: Module created.

   1.97k  Mon Mar 10 2008 10:39:12  chwon
	3567: fixed missing while (0) in the _SETUP macro

   1.97k  Wed Jul 16 2008 11:26:18  chwon
	4250: Add generic IN and OUT switch definition. Add MagicBegin and
	MagicEnd in the generic switch for type checking.
	4303: add USBPUMP_SESSION_HANDLE type and use this for session handle
	type instead of VOID *.

   1.97k  Mon Aug 04 2008 10:04:47  chwon
	4250: Add USBPUMP_API_INCALL_TRAILER and USBPUMP_API_OUTCALL_TRAILER
	definition and init macro.

   1.97k  Tue Sep 16 2008 12:03:47  chwon
	Fixed doc-xml error

   2.01a  Fri Mar 27 2009 09:25:14  chwon
	7647: Add UsbPumpObject_SizeOpenSessionRequestMemory() prototype

   2.01a  Sun May 17 2009 11:28:35  tmm
	USBPUMP_SESSION_HANDLE needs to be in basehiltypes.h.  Add
	general "UNSUCCESSFUL" API error.  Reserve all error codes from
	0..0x100 for use as generic API error codes.

   2.01a  Sun May 17 2009 15:27:52  chwon
	8040: Add USBPUMP_API_STATUS type. Add USBPUMP_API_STATUS_SUCCESS() and
	USBPUMP_API_STATUS_FAILED() macro.

   3.15b  Mon Jun  1 2015 00:19:36  tmm
	19211: correct usages of "DataPump".

    3.15d Wed Dec 30 2015 17:33:05  tmm
	19788: Add USBPUMP_IOCTL_API_INDICATE

*/

#ifndef _USBIOCTL_API_H_		/* prevent multiple includes */
#define _USBIOCTL_API_H_

/*
|| we need the base DataPump types
*/
#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMPIOCTLS_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBUUID_H_
# include "usbuuid.h"
#endif

/****************************************************************************\
|
|	Define helper macros.
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_CLASS_API		'u'

/**** helper macro for non-VOID API in-calls ****/
#define __TMS_USBPUMP_IOCTL_API(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_API,				\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_API_, NAME, _ARG)	\
		)


/****************************************************************************\
|
|	The API ioctl codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_API_OPEN		\
	__TMS_USBPUMP_IOCTL_API(0, RW, OPEN)

#define	__TMS_USBPUMP_IOCTL_API_INDICATE		\
	__TMS_USBPUMP_IOCTL_API(1, W, INDICATE)

/****************************************************************************\
|
|	The IOCTL arguments and documentation
|
\****************************************************************************/

/*

IOCTL:	USBPUMP_IOCTL_API_OPEN

Index:	Type:	USBPUMP_IOCTL_API_OPEN_ARG
	Name:	USBPUMP_IOCTL_API_OPEN_ARG_SETUP_V1

Function:
	IOCTL issued by customer to obtain a copy of the supplier's in-calls,
	and at the same time provide the associated out-calls to the supplier.

Input:
	pInParam	points to a USBPUMP_IOCTL_API_OPEN_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_API_OPEN_ARG structure.

	USBPUMP_IOCTL_API_OPEN_ARG has the following elements:

	CONST USBGUID *	pGuid;
		IN: The interface GUID.  This specifies the interface GUID that
		the customer wants to use to communicate with the supplier.
		For claiming the IOCTL, check pInArg->pGuid to make sure it's
		the right GUID for this API.  If not, reject the IOCTL as
		unclaimed.

	VOID *	pClientObject;
		IN: This is optional, and may be NULL.  It may be used if the
		customer is a DataPump object.  If used, it must point to the
		object that represents the customer.  This parameter is primarily
		provided for MCCI's future use.

	USBPUMP_SESSION_HANDLE	SessionHandle;
		OUT: This is a session handle to be used in subsequent session-
		based API operations.

	USBPUMP_API_INCALL *	pInCallApiBuffer;
		OUT: This is a pointer to a buffer to receive the supplier's
		in-call function table.  The supplier fills in the referenced
		buffer during OpenSession processing.  The customer uses these
		functions to make in-calls to the supplier.

	RECSIZE	sizeInCallApiBuffer;
		IN: This specifies the size of the buffer referenced by
		pInCallApiBuffer.

	VOID *	pClientHandle;
		IN: This is passed as a parameter to each of the asynchronous
		callback functions (out-calls).  It is not interpreted by the
		supplier in any way.

	CONST USBPUMP_API_OUTCALL *pOutCallApiBuffer;
		IN: This is a pointer to a buffer containing the supplier's
		out-call function table.  The customer fills in the referenced
		buffer prior to OpenSession processing.  The supplier extracts
		a copy of this buffer to enable it to make out-calls to the
		customer.

	RECSIZE	sizeOutCallApiBuffer;
		IN: This specifies the size of the buffer referenced by
		pOutCallApiBuffer.

	UINT32	Status;
		OUT: This is passed as a parameter to OpenSession callback.

Description:
	The IOCTL will be used by all future objects to provide a unified
	mechanism for exchanging interfaces (i.e., collections of function
	pointers).  The customer must obtain a copy of the supplier's in-calls,
	and at the same time provide the associated out-calls to the supplier.

	Note that the terms "in-call" and "out-call" are from the perspective
	of the supplier.  The supplier takes the role of the "host", and the
	customer takes the role of the "client".  I.e., the direction of the
	calls is referenced from the perspective of the supplier.  Furthermore,
	the terms "interface" and "API" are used interchangeably.

Setup Macro:
	VOID USBPUMP_IOCTL_API_OPEN_ARG_SETUP_V1(
		USBPUMP_IOCTL_API_OPEN_ARG *	pArg,
		CONST USBGUID *			pGuid,
		USBPUMP_OBJECT *		pClientObject,
		USBPUMP_API_INCALL *		pInCallApiBuffer,
		RECSIZE				sizeInCallApiBuffer,
		VOID *				pClientHandle,
		CONST USBPUMP_API_OUTCALL *	pOutCallApiBuffer,
		RECSIZE				sizeOutCallApiBuffer
		);

	This macro correctly initializes a USBPUMP_IOCTL_API_OPEN_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

/* __TMS_TYPE_DEF_HANDLE(USBPUMP_SESSION_HANDLE); from basehiltypes.h */
__TMS_TYPE_DEF_ARG(USBPUMP_API_STATUS, UINT32);

__TMS_FNTYPE_DEF(
USBPUMP_API_OPEN_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *			/* pClientContext */,
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_API_STATUS	/* Status */,
	__TMS_VOID *			/* pOpenRequestMemory */,
	__TMS_RECSIZE			/* sizeOpenRequestMemory */
	));

__TMS_FNTYPE_DEF(
USBPUMP_API_CLOSE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *			/* pClientContext */,
	__TMS_USBPUMP_API_STATUS	/* Status */
	));

__TMS_FNTYPE_DEF(
USBPUMP_API_CLOSE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */,
	__TMS_USBPUMP_API_CLOSE_CB_FN *	/* pCloseCallback */,
	__TMS_VOID *			/* pClientContext */
	));


/****************************************************************************\
|
|	The API in-call function table
|
\****************************************************************************/

#define __TMS_USBPUMP_API_INCALL_HEADER					\
	__TMS_UINT32			MagicBegin;			\
	__TMS_RECSIZE			Size;				\
	/* This is default in-call function */				\
	__TMS_USBPUMP_API_CLOSE_FN *	pCloseFn

#define	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(			\
	a_MagicBegin,							\
	a_Size,								\
	a_pCloseFn							\
	)								\
	(a_MagicBegin),							\
	(a_Size),							\
	(a_pCloseFn)

#define	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(			\
	a_pInCall,							\
	a_MagicBegin,							\
	a_Size,								\
	a_pCloseFn							\
	)								\
	(a_pInCall)->Generic.MagicBegin = (a_MagicBegin);		\
	(a_pInCall)->Generic.Size = (a_Size);				\
	(a_pInCall)->Generic.pCloseFn = (a_pCloseFn)


#define __TMS_USBPUMP_API_INCALL_TRAILER				\
	__TMS_UINT32			MagicEnd

#define	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)								\
	(a_MagicEnd)

#define	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(			\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)								\
	(a_pInCall)->a_Container.MagicEnd = (a_MagicEnd)


__TMS_TYPE_DEF_STRUCT  (USBPUMP_API_INCALL_CONTENTS);
struct __TMS_STRUCTNAME(USBPUMP_API_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_API_INCALL_HEADER;
	__TMS_USBPUMP_API_INCALL_TRAILER;
	};

__TMS_TYPE_DEF_UNION (USBPUMP_API_INCALL);
union __TMS_UNIONNAME(USBPUMP_API_INCALL)
	{
	__TMS_USBPUMP_API_INCALL_CONTENTS	Generic;
	};

#define	__TMS_USBPUMP_API_INCALL_CONTENTS__STRUCT			\
	__TMS_USBPUMP_API_INCALL_CONTENTS	Generic

#define	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION			\
	__TMS_USBPUMP_API_INCALL_CONTENTS	Generic;		\
	__TMS_USBPUMP_API_INCALL		GenericCast

#define	__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN	(0x75616900u + '<')
#define	__TMS_USBPUMP_API_INCALL_MAGIC_END	(0x96165700u + '>')

#define	__TMS_USBPUMP_API_INCALL_INIT_V1(				\
	a_pCloseFn							\
	)								\
	{{								\
	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(			\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,			\
		sizeof(__TMS_USBPUMP_API_INCALL),			\
		a_pCloseFn						\
		),							\
	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(			\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_API_INCALL_SETUP_V1(				\
	a_pInCall,							\
	a_pCloseFn							\
	)								\
    do	{								\
	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(			\
		a_pInCall,						\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,			\
		sizeof(*(a_pInCall)),					\
		a_pCloseFn						\
		);							\
	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(			\
		a_pInCall,						\
		Generic,						\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	The API out-call function table
|
\****************************************************************************/

#define __TMS_USBPUMP_API_OUTCALL_HEADER				\
	__TMS_UINT32			MagicBegin;			\
	__TMS_RECSIZE			Size				\
	/* No common out-call function */

#define	__TMS_USBPUMP_API_OUTCALL_HEADER_INIT_V1(			\
	a_MagicBegin,							\
	a_Size								\
	)								\
	(a_MagicBegin),							\
	(a_Size)

#define	__TMS_USBPUMP_API_OUTCALL_HEADER_SETUP_V1(			\
	a_pOutCall,							\
	a_MagicBegin,							\
	a_Size								\
	)								\
	(a_pOutCall)->Generic.MagicBegin = (a_MagicBegin);		\
	(a_pOutCall)->Generic.Size = (a_Size)


#define __TMS_USBPUMP_API_OUTCALL_TRAILER				\
	__TMS_UINT32			MagicEnd

#define	__TMS_USBPUMP_API_OUTCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)								\
	(a_MagicEnd)

#define	__TMS_USBPUMP_API_OUTCALL_TRAILER_SETUP_V1(			\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)								\
	(a_pOutCall)->a_Container.MagicEnd = (a_MagicEnd)


__TMS_TYPE_DEF_STRUCT  (USBPUMP_API_OUTCALL_CONTENTS);
struct __TMS_STRUCTNAME(USBPUMP_API_OUTCALL_CONTENTS)
	{
	__TMS_USBPUMP_API_OUTCALL_HEADER;
	__TMS_USBPUMP_API_OUTCALL_TRAILER;
	};

__TMS_TYPE_DEF_UNION (USBPUMP_API_OUTCALL);
union __TMS_UNIONNAME(USBPUMP_API_OUTCALL)
	{
	__TMS_USBPUMP_API_OUTCALL_CONTENTS	Generic;
	};

#define	__TMS_USBPUMP_API_OUTCALL_CONTENTS__STRUCT			\
	__TMS_USBPUMP_API_OUTCALL_CONTENTS	Generic

#define	__TMS_USBPUMP_API_OUTCALL_CONTENTS__UNION			\
	__TMS_USBPUMP_API_OUTCALL_CONTENTS	Generic;		\
	__TMS_USBPUMP_API_OUTCALL		GenericCast

#define	__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN	(0x75616f00u + '<')
#define	__TMS_USBPUMP_API_OUTCALL_MAGIC_END	(0xf6165700u + '>')

#define	__TMS_USBPUMP_API_OUTCALL_INIT_V1(				\
	)								\
	{{								\
	__TMS_USBPUMP_API_OUTCALL_HEADER_INIT_V1(			\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,			\
		sizeof(__TMS_USBPUMP_API_OUTCALL)			\
		),							\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_INIT_V1(			\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_API_OUTCALL_SETUP_V1(				\
	a_pOutCall							\
	)								\
    do	{								\
	__TMS_USBPUMP_API_OUTCALL_HEADER_SETUP_V1(			\
		a_pOutCall,						\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,			\
		sizeof(*(a_pOutCall))					\
		);							\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_SETUP_V1(			\
		a_pOutCall,						\
		Generic,						\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	USBPUMP API Status Code Definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_API_STATUS_OK				0
#define	__TMS_USBPUMP_API_STATUS_INVALID_PARAMETER		1
#define	__TMS_USBPUMP_API_STATUS_ARG_AREA_TOO_SMALL		2
#define	__TMS_USBPUMP_API_STATUS_BUFFER_TOO_SMALL		3
#define	__TMS_USBPUMP_API_STATUS_NOT_SUPPORTED			4
#define	__TMS_USBPUMP_API_STATUS_NO_MORE_SESSIONS		5
#define	__TMS_USBPUMP_API_STATUS_INVALID_SESSION_HANDLE		6
#define	__TMS_USBPUMP_API_STATUS_UNSUCCESSFUL			7
/* reserve a large number of error codes for general API */
#define	__TMS_USBPUMP_API_STATUS__MAX				0x100

#define	__TMS_USBPUMP_API_STATUS_SUCCESS(Status)	\
	((Status) == __TMS_USBPUMP_API_STATUS_OK)

#define	__TMS_USBPUMP_API_STATUS_FAILED(Status)		\
	((Status) != __TMS_USBPUMP_API_STATUS_OK)


__TMS_TYPE_DEF_STRUCT  (USBPUMP_IOCTL_API_OPEN_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_API_OPEN_ARG)
	{
	__TMS_CONST __TMS_USBGUID *		pGuid;

	/* a DataPump object, for future use */
	__TMS_USBPUMP_OBJECT *			pClientObject;

	/* use as parameter for in calls */
	__TMS_USBPUMP_SESSION_HANDLE		SessionHandle;

	/* Buffer information for in calls */
	__TMS_USBPUMP_API_INCALL *		pInCallApiBuffer;
	__TMS_RECSIZE				sizeInCallApiBuffer;

	/* use as parameter for out calls */
	__TMS_VOID *				pClientHandle;

	/* Buffer information for out calls */
	__TMS_CONST __TMS_USBPUMP_API_OUTCALL *	pOutCallApiBuffer;
	__TMS_RECSIZE				sizeOutCallApiBuffer;

	/* Open status */
	__TMS_USBPUMP_API_STATUS		Status;
	};

#define	__TMS_USBPUMP_IOCTL_API_OPEN_ARG_SETUP_V1(			\
	a_pArg,								\
	a_pGuid,							\
	a_pClientObject,						\
	a_pInCallApiBuffer,						\
	a_sizeInCallApiBuffer,						\
	a_pClientHandle,						\
	a_pOutCallApiBuffer,						\
	a_sizeOutCallApiBuffer						\
	)								\
   do	{								\
	(a_pArg)->pGuid = a_pGuid;					\
	(a_pArg)->pClientObject = a_pClientObject;			\
	(a_pArg)->SessionHandle = __TMS_NULL;				\
	(a_pArg)->pInCallApiBuffer = a_pInCallApiBuffer;		\
	(a_pArg)->sizeInCallApiBuffer = a_sizeInCallApiBuffer;		\
	(a_pArg)->pClientHandle = a_pClientHandle;			\
	(a_pArg)->pOutCallApiBuffer = a_pOutCallApiBuffer;		\
	(a_pArg)->sizeOutCallApiBuffer = a_sizeOutCallApiBuffer;	\
	} while (0)

/* sizeOpenRequestMemory */
/* Use UsbPumpObject_SizeOpenSessionRequestMemory() instead of this macro. */
#define	__TMS_USBPUMP_API_OPEN_REQUEST_MEMORY_SIZE	\
	UsbPumpObject_SizeOpenSessionRequestMemory(__TMS_NULL)

/*

IOCTL:	USBPUMP_IOCTL_API_INDICATE

Index:	Type:	USBPUMP_IOCTL_API_INDICATE_ARG
	Name:	USBPUMP_IOCTL_API_INCIDATE_ARG_SETUP_V1

Function:
	GUID-based incalls to allow simple messages to be sent via the
	IOCTL message system without defining new codes.

Input:
	pInParam	points to a USBPUMP_IOCTL_API_INDICATE_ARG
			structure.

	pOutParam	NULL

	USBPUMP_IOCTL_API_INDICATE_ARG has the following elements:

	CONST USBGUID *	pGuid;
		IN: The message GUID.  This identifies the format of the rest
		of the message and the meaning of the data.
		
		For claiming the IOCTL, check pInArg->pGuid to make sure it's
		the right GUID for this API.  If not, reject the IOCTL as
		unclaimed.

	PVOID pGuidSpecificInfo[8];
		These parameters are message specific.

Description:
	This IOCTL is a lightweight method of sending one-way information
	from a client to an object in the DataPump. The DataPump doesn't
	interpret the parameters except as indicated above.

Setup Macro:
	VOID USBPUMP_IOCTL_API_INDICATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_API_INDICATE_ARG *pArg,
		CONST USBGUID *			pGuid,
		VOID *p1,
		VOID *p2,
		VOID *p3,
		VOID *p4,
		VOID *p5,
		VOID *p6,
		VOID *p7,
		VOID *p8
		);

	This macro correctly initializes a USBPUMP_IOCTL_API_INDICATE_ARG
	structure. If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT  (USBPUMP_IOCTL_API_INDICATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_API_INDICATE_ARG)
	{
	__TMS_CONST __TMS_USBGUID *		pGuid;
	__TMS_PVOID pGuidSpecificInfo[8];
	};

#define	__TMS_USBPUMP_IOCTL_API_INDICATE_ARG_SETUP_V1(			\
	a_pArg,								\
	a_pGuid,							\
	a_p0, a_p1, a_p2, a_p3, a_p4, a_p5, a_p6, a_p7			\
	)								\
   do	{								\
	(a_pArg)->pGuid = a_pGuid;					\
	(a_pArg)->pGuidSpecificInfo[0] = a_p0;				\
	(a_pArg)->pGuidSpecificInfo[1] = a_p1;				\
	(a_pArg)->pGuidSpecificInfo[2] = a_p2;				\
	(a_pArg)->pGuidSpecificInfo[3] = a_p3;				\
	(a_pArg)->pGuidSpecificInfo[4] = a_p4;				\
	(a_pArg)->pGuidSpecificInfo[5] = a_p5;				\
	(a_pArg)->pGuidSpecificInfo[6] = a_p6;				\
	(a_pArg)->pGuidSpecificInfo[7] = a_p7;				\
	} while (0)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpObject_OpenSession(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */,
	__TMS_VOID *			/* pOpenRequestMemory */,
	__TMS_RECSIZE			/* sizeOpenRequestMemory */,
	__TMS_USBPUMP_API_OPEN_CB_FN *	/* pCallBack */,
	__TMS_VOID *			/* pCallBackContext */,
	__TMS_CONST __TMS_USBGUID *	/* pGuid */,
	__TMS_USBPUMP_OBJECT *		/* pClientObject -- OPTIONAL */,
	__TMS_USBPUMP_API_INCALL *	/* pInCallApiBuffer */,
	__TMS_RECSIZE			/* sizeInCallApiBuffer */,
	__TMS_VOID *    		/* pClientHandle */,
	__TMS_CONST __TMS_USBPUMP_API_OUTCALL *	/* pOutCallApiBuffer */,
	__TMS_RECSIZE			/* sizeOutCallApiBuffer */
	);

__TMS_RECSIZE
UsbPumpObject_SizeOpenSessionRequestMemory(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_CLASS_API	\
   __TMS_USBPUMP_IOCTL_CLASS_API
# define USBPUMP_IOCTL_API(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_API(i, t, NAME)
# define USBPUMP_IOCTL_API_OPEN	\
   __TMS_USBPUMP_IOCTL_API_OPEN
# define USBPUMP_IOCTL_API_INDICATE	\
   __TMS_USBPUMP_IOCTL_API_INDICATE
# define USBPUMP_API_INCALL_HEADER	\
   __TMS_USBPUMP_API_INCALL_HEADER
# define USBPUMP_API_INCALL_HEADER_INIT_V1(			\
	a_MagicBegin,							\
	a_Size,								\
	a_pCloseFn							\
	)	\
	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(			\
	a_MagicBegin,							\
	a_Size,								\
	a_pCloseFn							\
	)
# define USBPUMP_API_INCALL_HEADER_SETUP_V1(			\
	a_pInCall,							\
	a_MagicBegin,							\
	a_Size,								\
	a_pCloseFn							\
	)	\
	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(			\
	a_pInCall,							\
	a_MagicBegin,							\
	a_Size,								\
	a_pCloseFn							\
	)
# define USBPUMP_API_INCALL_TRAILER	\
   __TMS_USBPUMP_API_INCALL_TRAILER
# define USBPUMP_API_INCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)
# define USBPUMP_API_INCALL_TRAILER_SETUP_V1(			\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(			\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)
# define USBPUMP_API_INCALL_CONTENTS__STRUCT	\
   __TMS_USBPUMP_API_INCALL_CONTENTS__STRUCT
# define USBPUMP_API_INCALL_CONTENTS__UNION	\
   __TMS_USBPUMP_API_INCALL_CONTENTS__UNION
# define USBPUMP_API_INCALL_MAGIC_BEGIN	\
   __TMS_USBPUMP_API_INCALL_MAGIC_BEGIN
# define USBPUMP_API_INCALL_MAGIC_END	\
   __TMS_USBPUMP_API_INCALL_MAGIC_END
# define USBPUMP_API_INCALL_INIT_V1(				\
	a_pCloseFn							\
	)	\
	__TMS_USBPUMP_API_INCALL_INIT_V1(				\
	a_pCloseFn							\
	)
# define USBPUMP_API_INCALL_SETUP_V1(				\
	a_pInCall,							\
	a_pCloseFn							\
	)	\
	__TMS_USBPUMP_API_INCALL_SETUP_V1(				\
	a_pInCall,							\
	a_pCloseFn							\
	)
# define USBPUMP_API_OUTCALL_HEADER	\
   __TMS_USBPUMP_API_OUTCALL_HEADER
# define USBPUMP_API_OUTCALL_HEADER_INIT_V1(			\
	a_MagicBegin,							\
	a_Size								\
	)	\
	__TMS_USBPUMP_API_OUTCALL_HEADER_INIT_V1(			\
	a_MagicBegin,							\
	a_Size								\
	)
# define USBPUMP_API_OUTCALL_HEADER_SETUP_V1(			\
	a_pOutCall,							\
	a_MagicBegin,							\
	a_Size								\
	)	\
	__TMS_USBPUMP_API_OUTCALL_HEADER_SETUP_V1(			\
	a_pOutCall,							\
	a_MagicBegin,							\
	a_Size								\
	)
# define USBPUMP_API_OUTCALL_TRAILER	\
   __TMS_USBPUMP_API_OUTCALL_TRAILER
# define USBPUMP_API_OUTCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)
# define USBPUMP_API_OUTCALL_TRAILER_SETUP_V1(			\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_SETUP_V1(			\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)
# define USBPUMP_API_OUTCALL_CONTENTS__STRUCT	\
   __TMS_USBPUMP_API_OUTCALL_CONTENTS__STRUCT
# define USBPUMP_API_OUTCALL_CONTENTS__UNION	\
   __TMS_USBPUMP_API_OUTCALL_CONTENTS__UNION
# define USBPUMP_API_OUTCALL_MAGIC_BEGIN	\
   __TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN
# define USBPUMP_API_OUTCALL_MAGIC_END	\
   __TMS_USBPUMP_API_OUTCALL_MAGIC_END
# define USBPUMP_API_OUTCALL_INIT_V1(				\
	)	\
	__TMS_USBPUMP_API_OUTCALL_INIT_V1(				\
	)
# define USBPUMP_API_OUTCALL_SETUP_V1(				\
	a_pOutCall							\
	)	\
	__TMS_USBPUMP_API_OUTCALL_SETUP_V1(				\
	a_pOutCall							\
	)
# define USBPUMP_API_STATUS_OK	\
   __TMS_USBPUMP_API_STATUS_OK
# define USBPUMP_API_STATUS_INVALID_PARAMETER	\
   __TMS_USBPUMP_API_STATUS_INVALID_PARAMETER
# define USBPUMP_API_STATUS_ARG_AREA_TOO_SMALL	\
   __TMS_USBPUMP_API_STATUS_ARG_AREA_TOO_SMALL
# define USBPUMP_API_STATUS_BUFFER_TOO_SMALL	\
   __TMS_USBPUMP_API_STATUS_BUFFER_TOO_SMALL
# define USBPUMP_API_STATUS_NOT_SUPPORTED	\
   __TMS_USBPUMP_API_STATUS_NOT_SUPPORTED
# define USBPUMP_API_STATUS_NO_MORE_SESSIONS	\
   __TMS_USBPUMP_API_STATUS_NO_MORE_SESSIONS
# define USBPUMP_API_STATUS_INVALID_SESSION_HANDLE	\
   __TMS_USBPUMP_API_STATUS_INVALID_SESSION_HANDLE
# define USBPUMP_API_STATUS_UNSUCCESSFUL	\
   __TMS_USBPUMP_API_STATUS_UNSUCCESSFUL
# define USBPUMP_API_STATUS__MAX	\
   __TMS_USBPUMP_API_STATUS__MAX
# define USBPUMP_API_STATUS_SUCCESS(Status)	\
   __TMS_USBPUMP_API_STATUS_SUCCESS(Status)
# define USBPUMP_API_STATUS_FAILED(Status)	\
   __TMS_USBPUMP_API_STATUS_FAILED(Status)
# define USBPUMP_IOCTL_API_OPEN_ARG_SETUP_V1(			\
	a_pArg,								\
	a_pGuid,							\
	a_pClientObject,						\
	a_pInCallApiBuffer,						\
	a_sizeInCallApiBuffer,						\
	a_pClientHandle,						\
	a_pOutCallApiBuffer,						\
	a_sizeOutCallApiBuffer						\
	)	\
	__TMS_USBPUMP_IOCTL_API_OPEN_ARG_SETUP_V1(			\
	a_pArg,								\
	a_pGuid,							\
	a_pClientObject,						\
	a_pInCallApiBuffer,						\
	a_sizeInCallApiBuffer,						\
	a_pClientHandle,						\
	a_pOutCallApiBuffer,						\
	a_sizeOutCallApiBuffer						\
	)
# define USBPUMP_API_OPEN_REQUEST_MEMORY_SIZE	\
   __TMS_USBPUMP_API_OPEN_REQUEST_MEMORY_SIZE
# define USBPUMP_IOCTL_API_INDICATE_ARG_SETUP_V1(			\
	a_pArg,								\
	a_pGuid,							\
	a_p0, a_p1, a_p2, a_p3, a_p4, a_p5, a_p6, a_p7			\
	)	\
	__TMS_USBPUMP_IOCTL_API_INDICATE_ARG_SETUP_V1(			\
	a_pArg,								\
	a_pGuid,							\
	a_p0, a_p1, a_p2, a_p3, a_p4, a_p5, a_p6, a_p7			\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_api.h ****/
#endif /* _USBIOCTL_API_H_ */
