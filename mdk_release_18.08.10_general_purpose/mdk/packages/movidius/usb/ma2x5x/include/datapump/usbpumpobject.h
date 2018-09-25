/* usbpumpobject.h	Fri Feb 24 2017 17:06:02 chwon */

/*

Module:  usbpumpobject.h

Function:
	Object headers and common structures for USB DataPump Objects

Version:
	V3.19b	Fri Feb 24 2017 17:06:02 chwon	Edit level 16

Copyright notice:
	This file copyright (C) 2002-2008, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.63i  Sat May 18 2002 20:47:49  tmm
	Module created.

   1.79a  7/10/2002  tmm
	Routines should be named UsbPumpObject_..., not UsbPumpLib_Object...

   1.91b  Mon Apr 04 2005 11:10:47 maw
   	Added handle to object header

   1.91c  Thu May 12 2005 08:51:24 maw
   	Corrected invalid object name

   1.91d  2005-06-06  tmm
	Add asynchronous IOCTL framework.

   1.91d  2005-06-22  tmm
	Add IOCTL cancel and set-cancel functions.

   1.91h  2005-08-22  tmm
	Add USBPUMP_OBJECT_METACLASS_TAG.

   1.91h  Wed Aug 24 2005 14:13:41  chwon
	Add fSafe in USBPUMP_IOCTL_QE.
	Change return value of UsbPumpObject_IoctlAsync().

   1.91h   Fri Sep 02 2005 11:54:39  chwon
	Changed fCancelled and fSafe flag to fFlags

   1.93b   Wed Nov 16 2005 13:53:48  chwon
	1582: Add ulDebugFlags in USBPUMP_OBJECT_HEADER

   1.93c  Wed Nov 30 2005 18:29:01  tmm
	Make the class children pointer use USBPUMP_OBJECT_HEADER.
	Change the classes to use a central IOCTL function instead of
	trying to convince all the implementors to remember to add
	the class-specific behavior.  Leave a slot so the implementors
	can add their own additional methods.

   1.97i  Tue May 22 2007 08:35:17  chwon
	2907: fix return type of UsbPumpObject_IoctlAsync(). It should be VOID.

   1.97k  Thu May 08 2008 11:57:42  chwon
	4085: add USBPUMP_TIMER in the USBPUMP_IOCTL_QE.

   3.19b  Fri Feb 24 2017 17:06:03  chwon
	20480: Fix USBPUMP_OBJECT_HEADER::pName type.

*/

#ifndef _USBPUMPOBJECT_H_		/* prevent multiple includes */
#define _USBPUMPOBJECT_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

/****************************************************************************\
|
|	Forward type definitions
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_QE);
__TMS_TYPE_DEF_UNION	(USBPUMP_OBJECT);
__TMS_TYPE_DEF_UNION	(USBPUMP_OBJECT_CLASS);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_OBJECT_CLASS_CONTENTS);

/*

Type:	USBPUMP_OBJECT_IOCTL_FN

Function:
	C function type for USBPUMP_OBJECT_HEADER IOCTL method functions.

Definition:
	typedef USBPUMP_IOCTCL_RESULT
		USBPUMP_OBJECT_IOCTL_FN(
			USBPUMP_OBJECT_HEADER *pObject,
			USBPUMP_IOCTL_CODE Ioctl
			CONST VOID *pInParam,
			VOID *pOutParam
			);

	typedef USBPUMP_OBJECT_IOCTL_FN *PUSBPUMP_OBJECT_IOCTL_FN;

Description:
	Each USBPUMP_OBJECT_HEADER has associated with it an IOCTL function,
	provided by a class-specific function.  All such functions share
	a common prototype, USBPUMP_OBJECT_IOCTL_FN, and should be declared
	in header files using that type, rather than with an explicit
	prototype.

	For example, if a concrete object implementation defines an ioctl
	function named UsbWmc_Ioctl, then the *header file* should prototype
	the function using:

		USBPUMP_OBJECT_IOCTL_FN UsbWmc_Ioctl;

	rather than writing:
		USBPUMP_IOCTL_RESULT UsbWmc_Ioctl(
			USBPUMP_OBJECT_HEADER *p,
			USBPUMP_IOCTL_CODE,
			CONST VOID *,
			VOID *
			);

	If you want to have a prototype for reference in the code, you
	should write the prototype *twice*:

		USBPUMP_OBJECT_IOCTL_FN UsbWmc_Ioctl;

		// for reference, the above is equivalent to:
		USBPUMP_IOCTL_RESULT UsbWmc_Ioctl(
			USBPUMP_OBJECT_HEADER *p,
			USBPUMP_IOCTL_CODE,
			CONST VOID *,
			VOID *
			);

	The justification for this design approach is that it highlights
	the fact that the function prototype is not under the control of
	the function implementor, but rather highlights the fact that the
	function is a method implementation for some class.

Returns:
	Any USBPUMP_OBJECT_IOCTL_FN must return either
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED (if the IOCTL code was not
	recognized), USBPUMP_IOCTL_RESULT_SUCCESS (if the IOCTL code was
	recognized and the operation was successfully performed), or some
	error code (if the IOCTL code was recognized, but the operation
	could not be performed for some reason).

*/

__TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(
USBPUMP_OBJECT_IOCTL_FN,
	__TMS_USBPUMP_OBJECT_HEADER *
);

/*

Type:	USBPUMP_OBJECT_CLASS_IOCTL_FN

Function:
	C function type for USBPUMP_OBJECT_CLASS IOCTL method functions.

Definition:
	typedef USBPUMP_IOCTL_RESULT
		USBPUMP_OBJECT_CLASS_IOCTL_FN(
			USBPUMP_OBJECT_CLASS *pClass,
			USBPUMP_IOCTL_CODE Ioctl
			CONST VOID *pInParam,
			VOID *pOutParam
			);

	typedef USBPUMP_OBJECT_CLASS_IOCTL_FN *PUSBPUMP_OBJECT_CLASS_IOCTL_FN;

Description:
	USBPUMP_OBJECT_CLASS objects are derived from the base type of
	USBPUMP_OBJECT.  All such objects are required to have certain base
	IOCTL behavior.  To enforce that, the base object IOCTL function is
	always set to an internally-supplied IOCTL dispatcher that provides
	the base behavior.

	However, class implementations may need additional behavior.  For
	that reason, each USBPUMP_OBJECT_CLASS object has a class-specific
	IOCTL function, which can provide the additional behavior.

Returns:
	Any USBPUMP_OBJECT_CLASS_IOCTL_FN must return either
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED (if the IOCTL code was not
	recognized), USBPUMP_IOCTL_RESULT_SUCCESS (if the IOCTL code was
	recognized and the operation was successfully performed), or some
	error code (if the IOCTL code was recognized, but the operation
	could not be performed for some reason).

*/

/* this macro enforces the normal coding style for IOCTL functions */
__TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(
USBPUMP_OBJECT_CLASS_IOCTL_FN,
	__TMS_USBPUMP_OBJECT_CLASS *
);

/*

Type:	USBPUMP_OBJECT_LIST

Function:
	Provides a standard doubly-linked list thread component for
	USBPUMP_OBJECT_HEADERs.

Definition:
	USBPUMP_OBJECT_LIST has the following contents:

		USBPUMP_OBJECT_HEADER *pNext;
		USBPUMP_OBJECT_HEADER *pLast;

Description:
	USBPUMP_OBJECT_HEADERs are likely to be on multiple lists.  For
	consistency, rather than having many nodes named pXXXnext and
	pXXXlast, we define a structure that just contains the pNext and
	pLast for the particular list.

*/

struct __TMS_STRUCTNAME(USBPUMP_OBJECT_LIST)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pNext;
	__TMS_USBPUMP_OBJECT_HEADER	*pLast;
	};

/*

Type:	USBPUMP_OBJECT_HANDLE

Function:
	Abstract handle to USBPUMP_OBJECT_HEADERs.

Definition:
	TYPE_DEF_HANDLE(USBPUMP_OBJECT_HANDLE);

Description:
	Objects containing USBPUMP_OBJECT_HEADERs may be created in multiple
	instances. External task wishing to communicate with some object
	instance need a handle as reference to this instance, that is unique
	even if the entire DataPump is restarted.

*/

__TMS_TYPE_DEF_HANDLE(USBPUMP_OBJECT_HANDLE);

#define __TMS_USBPUMP_INVALID_OBJECT_HANDLE	((__TMS_USBPUMP_OBJECT_HANDLE)0)

/*

Type:	USBPUMP_OBJECT_HEADER

Function:
	Contains the basic information and metainformation about any
	USB DataPump object.

Definition:
	USBPUMP_OBJECT_HEADER has the following contents:

		ULONG			Tag;
		SIZE_T			Size;
		CONST TEXT		*pName;
		ULONG			InstanceNumber;
		USBPUMP_OBJECT_LIST	Directory;
		USBPUMP_OBJECT_HEADER	*pClassParent;
		USBPUMP_OBJECT_LIST	ClassSiblings;

		USBPUMP_OBJECT_IOCTL_FN	*pIoctl;
		USBPUMP_OBJECT_HEADER	*pIoctlParent;
		USBPUMP_OBJECT_LIST	IoctlSiblings;

Description:
	Many USB DataPump data structures are conveniently modeled with
	a common set of behaviors and attributes:  names (which are really
	tuples that allow structured matching of like functions), common
	behaviors (modeled by IOCTLs), and static relationships (modelled
	as a tree).

	The USBPUMP_OBJECT_HEADER structure collects the basic features
	together into a single data structure, which can be placed at the
	beginining of any structure which is to be treated as an object.
	These behaviors and attributes are then extended by additional data
	that is in the structure in which the USBPUMP_OBJECT_HEADER is
	embedded.

	|Tag| is a unique and arbitrary tag, assigned by the object designer.
	It often is a number that dumps as a 4-byte ASCII constant.

	|Size| is the size of the overall structure, and is provided for
	convenience in debugging.

	|pName| is the pointer to the object name.  Object names are actually
	compound -- objects can have multiple names, representing the object
	in terms of its structure or in terms of its behavior.

	|InstanceNumber| provides a unique and simple way to differentiate
	among multiple instances of objects with the same name.

	|Directory| is a list node; the object directory uses these fields.

	|pStaticParent| points to the object that is structurally the parent
	of this object.

	|Siblings| is a list node that is used to chain together all the
	children of a given Static Parent.

	|pIoctl| points to the optional IOCTL dispatch function for this
	object.

	|pIoctlParent| points to the next object that is to receive any
	IOCTLs not claimed by this object.

See Also:
	UsbPumpObject_Ioctl()
	UsbPumpLib_MatchPattern()

*/

/* the header for all objects: */
struct __TMS_STRUCTNAME(USBPUMP_OBJECT_HEADER)
	{
	/*
	|| The object tag is assigned by the class that creates the
	|| object.
	*/
	__TMS_ULONG			Tag;

	/*
	|| The object size is the number of bytes in the overall object.
	*/
	__TMS_SIZE_T			Size;

	/*
	|| The name and instance number uniquely identify this object.
	*/
	__TMS_CONST __TMS_TEXT		*pName;
	__TMS_ULONG			InstanceNumber;

	/*
	|| Handle that uniquely identifies this object instance
	|| from the same instance after a DataPump restart
	*/
	__TMS_USBPUMP_OBJECT_HANDLE	Handle;

	/*
	|| A pair of links (next/last) are provided for the use of the
	|| object directory.
	*/
	__TMS_USBPUMP_OBJECT_LIST	Directory;


	/*
	|| An object may have a parent.  This parent is another object;
	|| it normally is associated with the code which created and manages
	|| the object.  This relationsihp is static, and therefore the
	|| pointer is named 'pStaticParent'.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	*pClassParent;

	/*
	|| A pair of links (next/last) are provided for the use of the
	|| parent to keep track of its children
	*/
	__TMS_USBPUMP_OBJECT_LIST	ClassSiblings;


	/*
	|| The IOCTL function allows behaviors to be defined and exported
	|| without anything about a given object.  These behaviors can be
	|| overridden, inherited, etc.  However, IOCTLs are slower than
	|| direct function calls.  Direct function call methods are exported
	|| by objects via protocol switches, but (of course) then the caller
	|| has to import the appropriate C header file and must be aware of
	|| the underlying type of the object.
	*/
	__TMS_USBPUMP_OBJECT_IOCTL_FN	*pIoctl;

	/*
	|| Since IOCTLs might not be claimed, we may need a way to pass the
	|| IOCTL to our parent.  It's convenient to define a common way of
	|| doing this; that allows central code to avoid recursing in the
	|| common case where IOCTLs are just passed to lower layers
	|| without examination.  pIoctlParent points to the object that
	|| takes IOCTLs we don't claim.  It should be NULL if this object
	|| logically terminates its IOCTL chain.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	*pIoctlParent;

	__TMS_USBPUMP_OBJECT_LIST	IoctlSiblings;

	/* the debug flags */
	__TMS_UINT32			ulDebugFlags;
	};

union __TMS_UNIONNAME(USBPUMP_OBJECT)
	{
	__TMS_USBPUMP_OBJECT_HEADER	ObjectHeader;
	};

#define __TMS_USBPUMP_OBJECT_CONTENTS__STRUCT			\
	__TMS_USBPUMP_OBJECT_HEADER	ObjectHeader

#define	__TMS_USBPUMP_OBJECT_CONTENTS__UNION			\
	__TMS_USBPUMP_OBJECT_HEADER	ObjectHeader;		\
	__TMS_USBPUMP_OBJECT		ObjectCast

/****************************************************************************\
|
|	The Object Class object
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_OBJECT_CLASS_CONTENTS)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	/*
	|| since lists are always using OBJECT_HEADER threads, this
	|| makes life more sane.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	*pClassChildren;

	/*
	|| Since we insist on common class behavior, the class method
	|| takes over the ioctl function.  That means we need an
	|| additional pointer here for creator-specified behavior.
	*/
	__TMS_USBPUMP_OBJECT_CLASS_IOCTL_FN	*pIoctl;
	};

union __TMS_UNIONNAME(USBPUMP_OBJECT_CLASS)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;

	__TMS_USBPUMP_OBJECT_CLASS_CONTENTS	Class;
	};

#define	__TMS_USBPUMP_OBJECT_CLASS_CONTENTS__STRUCT			\
	__TMS_USBPUMP_OBJECT_CLASS_CONTENTS	Class

#define __TMS_USBPUMP_OBJECT_CLASS_CONTENTS__UNION			\
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;				\
	__TMS_USBPUMP_OBJECT_CLASS_CONTENTS	Class;			\
	__TMS_USBPUMP_OBJECT_CLASS		ClassCast

#define	__TMS_USBPUMP_OBJECT_METACLASS_TAG	\
		__TMS_UHIL_MEMTAG('M', 'C', 'l', 's')

/*

Type:	USBPUMP_IOCTL_QE_DONE_FN

Function:
	C function type for asynchronous IOCTL completion call-back functions.

Definition:
	typedef VOID
		USBPUMP_IOCTL_QE_DONE_FN(
			USBPUMP_IOCTL_QE *pIoctlQe,
			VOID *pDoneInfo,
			USBPUMP_IOCTL_RESULT result,
			VOID *pOutArg
			);
	typedef USBPUMP_IOCTL_QE_DONE_FN *PUSBPUMP_IOCTL_QE_DONE_FN;

Description:
	This function type is used for client-supplied completion functions
	that are passed to the asynchronous IOCTL facility.

Notes:
	Completion routinnes for USBPUMP_IOCTL_QEs that were submitted with
	safe completion policy are always called from the event dispath
	loop. On the other hand, completion routines that were submitted
	with fast completion policy may be called directly from the
	point of completion.  It's the completion routine's job to manage
	stack depth.

	From the moment this routine is called, the completion routine
	is passed ownership of the queue elmeent for disposal or reuse.

See Also:


*/

__TMS_FNTYPE_DEF(
	USBPUMP_IOCTL_QE_DONE_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_IOCTL_QE		*  /* pIoctlQe */,
		__TMS_VOID			*  /* pDoneInfo */,
		__TMS_USBPUMP_IOCTL_RESULT	   /* result */,
		__TMS_VOID			*  /* pOutArg */
		)
	);

/*

Type:	USBPUMP_IOCTL_QE_CANCEL_FN

Function:
	Cancel method for USBPUMP_IOCTL_QEs.

Definition:
	typedef VOID
		USBPUMP_IOCTL_QE_CANCEL_FN(
			USBPUMP_IOCTL_QE *pIoctlQe,
			VOID *pCancelInfo
			);

Description:
	When a service is queueing an IOCTL_QE for later processing, it
	may optionally set a cancel routine.  This will allow the service
	to be notified if the IOCTL_QE is subsequently cancelled.

	pIoctlQe->pCancelFn will be set to |NULL| before this function is
	called.

Notes:
	The function is called in DataPump context.

See Also:


*/

__TMS_FNTYPE_DEF(
	USBPUMP_IOCTL_QE_CANCEL_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_IOCTL_QE		*  /* pIoctlRq */,
		__TMS_VOID			*  /* pCancelInfo */
		)
	);

/*

Type:	USBPUMP_IOCTL_QE

Function:
	The queue element for asynchronous IOCTL operations.

Description:
	When issuing an asychronous IOCTL, the client has to supply a
	request block (or queue element) of type |USBPUMP_IOCTLQE|.  This
	element is composed of public and private portions, as outlined
	below.

Contents:
	The following entries may be referenced directly by user code.

	USBPUMP_OBJECT_HEADER *pTarget;
		The pointer to the target object.

	USBPUMP_IOCTL_CODE Request;
		The request code.

	CONST VOID *pInArg;
		The input arg.

	VOID *pOutArg;
		The output arg.

	USBPUMP_IOCTL_QE_DONE_FN pDoneFn;


	Private entries:

	UINT16 usLength;
		Length of strucure, in bytes, for version checking.

	UINT8 ucVersion;
		Version number of structure.  Will be initialized to
		USBPUMP_IOCTL_QE_VERSION_1 by the internal mechanisms.

	UINT8 fCancelled;
	USBPUMP_IOCTL_QE_CANCEL_FN pCancelFn;
		The cancellation function.  Originally NULL.  Set non-NULL
		whenever the queue element is in a state where it can be
		cancelled.  The IOCTL canceller will call this function
		(in DataPump context) to perform the cancellation.

	VOID *pServiceContext;
		This pointer is reserved for use by the internal context.

	CALLBACKCOMPLETION CompletionEvent;
		If CompletionEvent.callback_pfn is NULL, then this is to be
		a fast completion. Otherwise, it will be a safe completion,
		and CompletionEvent is set up to cause the client's completion
		to be called from the event loop using this event block.

	USBPUMP_OBJECT_HEADER *pCurrent;
		The pointer to the object to which we're delivering this
		IOCTL.

	USBPUMP_IOCTL_RESULT Result;
		The result.

	USBPUMP_IOCTL_QE *pNext, *pLast;
		Forward and back links when this IOCTL is queued.

Notes:
	Client code should not refer to the private entries listed above.

See Also:
	USBPUMP_IOCTL, USBPUMP_IOCTL_QE_DONE_FN, USBPUMP_IOCTL_QE_CANCEL_FN.

*/

/* __TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_QE); */

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_QE)
	{
	__TMS_UINT16			usLength;
	__TMS_UINT8			ucVersion;
	__TMS_UINT8			fFlags;

	__TMS_USBPUMP_IOCTL_QE		*pNext;
	__TMS_USBPUMP_IOCTL_QE		*pLast;

	__TMS_USBPUMP_OBJECT_HEADER	*pTarget;

	__TMS_USBPUMP_OBJECT_HEADER	*pCurrent;
	__TMS_USBPUMP_IOCTL_CODE	Request;
	__TMS_USBPUMP_IOCTL_RESULT	Result;
	__TMS_CONST __TMS_VOID		*pInArg;
	__TMS_VOID			*pOutArg;

	__TMS_USBPUMP_IOCTL_QE_DONE_FN	*pDoneFn;
	__TMS_VOID			*pDoneInfo;

	__TMS_USBPUMP_IOCTL_QE_CANCEL_FN *pCancelFn;
	__TMS_VOID			*pCancelInfo;
	__TMS_UCALLBACKCOMPLETION	CompletionEvent;

	__TMS_USBPUMP_TIMER		TimeoutTimer;	/* 4085 */
	};

/* this is the version tag of the version structure */
#define	__TMS_USBPUMP_IOCTL_QE_VERSION_1	1
#define	__TMS_USBPUMP_IOCTL_QE_VERSION_2	2

/* USBPUMP_IOCTL_QE fFlags */
#define	__TMS_USBPUMP_IOCTL_QE_FLAG_CANCELLED	(1 << 0)
#define	__TMS_USBPUMP_IOCTL_QE_FLAG_SAFE	(1 << 1)
#define	__TMS_USBPUMP_IOCTL_QE_FLAG_TIMING	(1 << 2)
#define	__TMS_USBPUMP_IOCTL_QE_FLAG_TIMED_OUT	(1 << 3)

/****************************************************************************\
|
|	Library functions are in usbpumpobjectapi.h; but the primary
|	init/deinit functions are here.  Please think hard before
|	adding functions beyond ..._Init(), ..._DeInit(), and ..._Ioctl()
|	here.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpObject_DeInit(
	__TMS_USBPUMP_OBJECT_HEADER *	/*pOld*/
	);

__TMS_VOID
UsbPumpObject_Init(
	__TMS_USBPUMP_OBJECT_HEADER *	/*pNew*/,
	__TMS_USBPUMP_OBJECT_HEADER *	/*pClassHeader*/,
	__TMS_UINT32			/*Tag*/,
	__TMS_SIZE_T			/*Size*/,
	__TMS_CONST __TMS_TEXT *	/*pName*/,
	__TMS_USBPUMP_OBJECT_HEADER *	/*pIoctlParent OPTIONAL */,
	__TMS_PUSBPUMP_OBJECT_IOCTL_FN	/*pIoctlFn OPTIONAL */
	);

__TMS_VOID
UsbPumpObjectClass_Init(
	__TMS_USBPUMP_OBJECT_CLASS *		/*pNew*/,
	__TMS_USBPUMP_OBJECT_HEADER *		/*pClassParent*/,
	__TMS_UINT32				/*Tag*/,
	__TMS_SIZE_T				/*Size*/,
	__TMS_CONST __TMS_TEXT *		/*pName*/,
	__TMS_USBPUMP_OBJECT_HEADER *		/*pIoctlParent OPTIONAL */,
	__TMS_PUSBPUMP_OBJECT_CLASS_IOCTL_FN	/*pIoctlFn OPTIONAL */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpObject_Ioctl(
	__TMS_USBPUMP_OBJECT_HEADER	* /*pHdr*/ ,
	__TMS_USBPUMP_IOCTL_CODE	  /*Request*/ ,
	__TMS_CONST __TMS_VOID		* /*pInBuffer*/ ,
	__TMS_VOID			* /*pOutBuffer*/
	);

__TMS_VOID
UsbPumpObject_IoctlAsync(
	__TMS_USBPUMP_OBJECT_HEADER	*  /*pHdr*/ ,
	__TMS_USBPUMP_IOCTL_CODE	   /*Request*/ ,
	__TMS_CONST __TMS_VOID		*  /*pInBuffer*/ ,
	__TMS_VOID			*  /*pOutBuffer*/ ,
	__TMS_USBPUMP_IOCTL_QE		*  /*pRequest */ ,
	__TMS_RECSIZE			   /*SizeRq*/ ,
	__TMS_USBPUMP_IOCTL_QE_DONE_FN	*  /*pDoneFn*/ ,
	__TMS_VOID			*  /*pDoneCtx*/ ,
	__TMS_BOOL			   /*fSafe*/
	);

__TMS_VOID
UsbPumpObject_IoctlAsync_V2(	/* 4085 */
	__TMS_USBPUMP_OBJECT_HEADER	*  /*pHdr*/ ,
	__TMS_USBPUMP_IOCTL_CODE	   /*Request*/ ,
	__TMS_CONST __TMS_VOID		*  /*pInBuffer*/ ,
	__TMS_VOID			*  /*pOutBuffer*/ ,
	__TMS_USBPUMP_IOCTL_QE		*  /*pRequest */ ,
	__TMS_RECSIZE			   /*SizeRq*/ ,
	__TMS_USBPUMP_IOCTL_QE_DONE_FN	*  /*pDoneFn*/ ,
	__TMS_VOID			*  /*pDoneCtx*/ ,
	__TMS_BOOL			   /*fSafe*/,
	__TMS_USHORT			   /*nMillisecs*/
	);

/*
|| since the USBPUMP_IOCTL_QE is defined in this module, we view this as
|| a method of a class that comes from this module, so we also define
|| it here.  Note, however, that the name is different, as this is not
|| an object method but rather an USBPUMP_IOCTL_QE method.
*/
__TMS_VOID
UsbPumpIoctlQe_Cancel(
	__TMS_USBPUMP_IOCTL_QE *	/* pRequestToCancel */
	);

__TMS_BOOL
UsbPumpIoctlQe_SetCancelRoutine(
	__TMS_USBPUMP_IOCTL_QE *		/* pRequest */,
	__TMS_USBPUMP_IOCTL_QE_CANCEL_FN *	/* pCancelFn */,
	__TMS_VOID *				/* pCancelInfo */
	);

__TMS_VOID
UsbPumpIoctlQe_Complete(
	__TMS_UPLATFORM *,
	__TMS_USBPUMP_IOCTL_QE *,
	__TMS_USBPUMP_IOCTL_RESULT
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaking
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_INVALID_OBJECT_HANDLE	\
   __TMS_USBPUMP_INVALID_OBJECT_HANDLE
# define USBPUMP_OBJECT_CONTENTS__STRUCT	\
   __TMS_USBPUMP_OBJECT_CONTENTS__STRUCT
# define USBPUMP_OBJECT_CONTENTS__UNION	\
   __TMS_USBPUMP_OBJECT_CONTENTS__UNION
# define USBPUMP_OBJECT_CLASS_CONTENTS__STRUCT	\
   __TMS_USBPUMP_OBJECT_CLASS_CONTENTS__STRUCT
# define USBPUMP_OBJECT_CLASS_CONTENTS__UNION	\
   __TMS_USBPUMP_OBJECT_CLASS_CONTENTS__UNION
# define USBPUMP_OBJECT_METACLASS_TAG	\
   __TMS_USBPUMP_OBJECT_METACLASS_TAG
# define USBPUMP_IOCTL_QE_VERSION_1	\
   __TMS_USBPUMP_IOCTL_QE_VERSION_1
# define USBPUMP_IOCTL_QE_VERSION_2	\
   __TMS_USBPUMP_IOCTL_QE_VERSION_2
# define USBPUMP_IOCTL_QE_FLAG_CANCELLED	\
   __TMS_USBPUMP_IOCTL_QE_FLAG_CANCELLED
# define USBPUMP_IOCTL_QE_FLAG_SAFE	\
   __TMS_USBPUMP_IOCTL_QE_FLAG_SAFE
# define USBPUMP_IOCTL_QE_FLAG_TIMING	\
   __TMS_USBPUMP_IOCTL_QE_FLAG_TIMING
# define USBPUMP_IOCTL_QE_FLAG_TIMED_OUT	\
   __TMS_USBPUMP_IOCTL_QE_FLAG_TIMED_OUT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpumpobject.h ****/
#endif /* _USBPUMPOBJECT_H_ */
