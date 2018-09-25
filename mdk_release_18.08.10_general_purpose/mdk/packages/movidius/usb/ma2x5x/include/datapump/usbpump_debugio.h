/* usbpump_debugio.h	Tue Apr 21 2009 09:24:09 maw */

/*

Module:  usbpump_debugio.h

Function:
	Base object definition for Debug-IO

Version:
	V2.01a	Tue Apr 21 2009 09:24:09 maw	Edit level 1

Copyright notice:
	This file copyright (C) 2009 by	

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Mats Webjörn, MCCI Corporation	April 2009

Revision history:
   2.01a  Tue Apr 21 2009 09:24:09 maw
	7563: Module created usbpump_abstract_sio.h EL 2

*/

#ifndef _USBPUMP_DEBUGIO_H_	/* prevent multiple includes */
#define _USBPUMP_DEBUGIO_H_

#ifndef _USBPUMPDEBUG_H_
# include "usbpumpdebug.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMP_DEBUGIO_DEFINES_H_
# include "usbpump_debugio_defines.h"
#endif

#ifndef _USBPUMP_DEBUGIO_TYPES_H_
#include "usbpump_debugio_types.h"
#endif

#ifndef _USBPUMP_DEBUGIO_IOCTL_H_
# include "usbpump_debugio_ioctl.h"
#endif

/****************************************************************************\
|
|	Typedefs
|
\****************************************************************************/


/*

Type:	USBPUMP_DEBUGIO_OPEN_QE

Function:
	Container of USBPUMP_IOCTL_QE and 
	USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ARG

Description:
	The USBPUMP_IOCTL_QE and open-session argument structure may 
	not be located on the stack during asynchronous IOCTL processing.
	
	The client should also not be aware of the inner details of
	the IOCTL handling. Therefore UsbPumpDebugIo_OpenSession()
	requires the client to pass in an untyped memory block that
	can be used as container for the USBPUMP_IOCTL_QE and 
	USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ARG. 

Contents:
	USBPUMP_DEBUGIO_OPEN_CB_FN *pCallBackFn;
		Pointer to OpenSession completion function
	
	VOID *pCallBackContext;
		Pointer passed as a parameter to the callback function 
		that indicates the completion of the open function

	USBPUMP_IOCTL_QE Qe;
		Asynchronous IOCTL QE.

	USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ARG Arg;
		Argument for Debug-IO open-session IOCTL

Notes:

See Also:

*/

__TMS_TYPE_DEF_STRUCT  (USBPUMP_DEBUGIO_OPEN_QE);
struct __TMS_STRUCTNAME(USBPUMP_DEBUGIO_OPEN_QE)
	{
	__TMS_USBPUMP_DEBUGIO_OPEN_CB_FN *		pCallBackFn;
	__TMS_VOID *					pCallBackContext;
	__TMS_USBPUMP_IOCTL_QE				IoctlQe;
	__TMS_USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC_ARG 
							Arg;
	};

/*

Type:	USBPUMP_DEBUGIO_SESSION

Index:	Type:	USBPUMP_DEBUGIO_SESSION_CONTENTS
	Macro:	USBPUMP_DEBUGIO_SESSION_CONTENTS__STRUCT
	Macro:	USBPUMP_DEBUGIO_SESSION_CONTENTS__UNION

Function:
	Represents a single Debug-IO session instance.

Description:
	Each Debug-IO object maintains a collection of session objects
	which represent the binding between the Debug-IO object and the client.

Contents:
	USBPUMP_DEBUGIO_SESSION *pNext, *pLast;
		Pointers to sibling USBPUMP_DEBUGIO_SESSIONs that are 
		collected by the parent Debug-IO object.

	USBPUMP_DEBUGIO * pDebugIo;
		Pointer to parent Debug-IO object

	VOID * pClientHandle;
		Handle used in subsequent outcalls from Debug-IO to client.
		It is not interpreted by the Debug-IO in any way.
	
	USBPUMP_OBJECT * pClientObject;
		Optional object pointer that may be used if the client is using 
		the DataPump object system.  If used, it must point to the object 
		that represents the client.  This parameter is primarily provided 
		for MCCI's future use.
		
	USBPUMP_DEBUGIO_STATUS_CHANGE_CB_FN *pStatusChangeCb;
		Specifies the function to be called when the Debug-IO when
		there is a status change in Debug-IO.
	
Notes:

See Also:

*/


/* in usbpump_debugio_qe.h:
|| __TMS_TYPE_DEF_UNION(USBPUMP_DEBUGIO_SESSION);
*/

__TMS_TYPE_DEF_STRUCT  (USBPUMP_DEBUGIO_SESSION_CONTENTS);
struct __TMS_STRUCTNAME(USBPUMP_DEBUGIO_SESSION_CONTENTS)
	{
	__TMS_USBPUMP_DEBUGIO_SESSION *		pNext;
	__TMS_USBPUMP_DEBUGIO_SESSION *		pLast;

	__TMS_USBPUMP_DEBUGIO *			pDebugIo;

	__TMS_VOID *				pClientHandle;
	__TMS_USBPUMP_OBJECT *			pClientObject;

	__TMS_USBPUMP_DEBUGIO_STATUS_CHANGE_CB_FN *
						pStatusChangeCb;

	__TMS_UCALLBACKCOMPLETION		OpenSessionCompletion;

	/*
	|| Debug-IO config
	*/
	__TMS_BOOL				ConnectState;
	__TMS_ULONG				Speed;
	__TMS_BYTES				StopBits;
	__TMS_BYTES				Parity;

	__TMS_PUSBPUMP_DEBUGIO_RECV_QE		pPendRecvQeHead;
	__TMS_PUSBPUMP_DEBUGIO_SEND_QE		pPendSendQeHead;
	};

union __TMS_UNIONNAME(USBPUMP_DEBUGIO_SESSION)
	{
	__TMS_USBPUMP_DEBUGIO_SESSION_CONTENTS	Session;
	};

/* the function contents macros */
#define	__TMS_USBPUMP_DEBUGIO_SESSION_CONTENTS__STRUCT		\
	__TMS_USBPUMP_DEBUGIO_SESSION_CONTENTS	Session

#define	__TMS_USBPUMP_DEBUGIO_SESSION_CONTENTS__UNION		\
	__TMS_USBPUMP_DEBUGIO_SESSION_CONTENTS	Session;	\
	__TMS_USBPUMP_DEBUGIO_SESSION		SessionCast

/**** sane names ****/
#define	ses_pNext			Session.pNext
#define	ses_pLast			Session.pLast
#define	ses_pDebugIo			Session.pDebugIo
#define	ses_pClientHandle		Session.pClientHandle
#define	ses_pClientObject		Session.pClientObject
#define	ses_pStatusChangeCb		Session.pStatusChangeCb
#define	ses_OpenSessionCompletion	Session.OpenSessionCompletion
#define	ses_ConnectState		Session.ConnectState
#define	ses_Speed			Session.Speed
#define	ses_StopBits			Session.StopBits
#define	ses_Parity			Session.Parity
#define	ses_pPendRecvQeHead		Session.pPendRecvQeHead
#define	ses_pPendSendQeHead		Session.pPendSendQeHead

/*

Type:	USBPUMP_DEBUGIO

Index:	Type:	USBPUMP_DEBUGIO_CONTENTS
	Macro:	USBPUMP_DEBUGIO_CONTENTS__STRUCT
	Macro:	USBPUMP_DEBUGIO_CONTENTS__UNION
	Constant: USBPUMP_DEBUGIO_TAG
	Macro:	USBPUMP_DEBUGIO_NAME()

Function:
	Represents a single Debug-IO instance.

Description:
	Each physical debug io transport is represented by a Debug-IO object;
	these objects are created during initialization.

	These function objects are part of the DataPump class system, so they 
	can be located using UsbPumpObject_EnumerateMatchingNames().  
	Names are normally constructed at compile time using the 
	USBPUMP_DEBUGIO_NAME() macro.

Contents:
	USBPUMP_OBJECT_CONTENTS__STRUCT;
		The normal DataPump object header.  USBPUMP_DEBUGIO_TAG
		is used as the tag for all function objects.

Notes:
	All function objects can be matched with the pattern
		"debug-io.*.fn.mcci.com"

	The macro:
		USBPUMP_OBJECT_NAME_FN("name")

	generates a string consisting of "name" followed by
	".fn.mcci.com".  Therefore, the appropriate way to
	match all functions is to match the string

		USBPUMP_OBJECT_NAME_ENUM_DEBUGIO

	which will match all implementations of Debug-IO

See Also:

*/


/* in usbpump_debugio_types.h:
|| __TMS_TYPE_DEF_UNION	(USBPUMP_DEBUGIO);
*/

__TMS_TYPE_DEF_STRUCT  (USBPUMP_DEBUGIO_CONTENTS);
struct __TMS_STRUCTNAME(USBPUMP_DEBUGIO_CONTENTS)
	{
	/*
	|| This is a standard Usbpump object
	*/
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	/*
	|| Head pointers for sessions in use, and free sessions
	*/
	__TMS_USBPUMP_DEBUGIO_SESSION *
					pSessionHead;

	__TMS_USBPUMP_DEBUGIO_SESSION *
					pFreeSessionHead;

	__TMS_UINT			uInstance;

	/*
	|| Lower transport state
	*/
	__TMS_BOOL			fTransportActive;
	};


union __TMS_UNIONNAME(USBPUMP_DEBUGIO)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;
	__TMS_USBPUMP_DEBUGIO_CONTENTS	DebugIo;
	};

/* the function contents macros */
#define	__TMS_USBPUMP_DEBUGIO_CONTENTS__STRUCT		\
	__TMS_USBPUMP_DEBUGIO_CONTENTS	DebugIo

#define	__TMS_USBPUMP_DEBUGIO_CONTENTS__UNION		\
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;		\
	__TMS_USBPUMP_DEBUGIO_CONTENTS	DebugIo;	\
	__TMS_USBPUMP_DEBUGIO		DebugIoCast

/**** sane names ****/
#define	dbgio_pSessionHead		DebugIo.pSessionHead
#define	dbgio_pFreeSessionHead		DebugIo.pFreeSessionHead
#define	dbgio_fTransportActive		DebugIo.fTransportActive

/****************************************************************************\
|
|	Externals
|
\****************************************************************************/


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_DEBUGIO_SESSION_CONTENTS__STRUCT	\
   __TMS_USBPUMP_DEBUGIO_SESSION_CONTENTS__STRUCT
# define USBPUMP_DEBUGIO_SESSION_CONTENTS__UNION	\
   __TMS_USBPUMP_DEBUGIO_SESSION_CONTENTS__UNION
# define USBPUMP_DEBUGIO_CONTENTS__STRUCT	\
   __TMS_USBPUMP_DEBUGIO_CONTENTS__STRUCT
# define USBPUMP_DEBUGIO_CONTENTS__UNION	\
   __TMS_USBPUMP_DEBUGIO_CONTENTS__UNION
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_debugio.h ****/
#endif /* _USBPUMP_DEBUGIO_H_ */
