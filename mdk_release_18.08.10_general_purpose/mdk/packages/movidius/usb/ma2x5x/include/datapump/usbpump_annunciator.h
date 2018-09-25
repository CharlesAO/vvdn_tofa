/* usbpump_annunciator.h	Sun May 17 2009 15:34:41 chwon */

/*

Module:  usbpump_annunciator.h

Function:
	Definition of USBPUMP_ANNUNCIATOR object

Version:
	V2.01a	Sun May 17 2009 15:34:41 chwon	Edit level 6

Copyright notice:
	This file copyright (C) 2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	February 2008

Revision history:
   1.97k  Fri Feb 15 2008 13:57:03 chwon
	3567: Module created.

   1.97k  Mon Mar 10 2008 10:40:00  chwon
	3567: fixed missing while (0) in the _SETUP macro

   1.97k  Tue Mar 18 2008 16:04:29  chwon
	3567: add sizeNotificationInfo parameter in the
	USBPUMP_ANNUNCIATOR_NOTIFICATION_FN and
	USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_FN

   1.97k  Tue Apr 01 2008 15:02:52  chwon
	3567: fixed USBPUMP_ANNUNCIATOR_NOTIFICATION_FN() definition comment

   1.97k  Wed Jul 16 2008 23:25:56  chwon
	4250: use generic INCALL and OUTCALL definition.
	4303: add USBPUMP_SESSION_HANDLE type and use this for session handle
	type instead of VOID *.

   1.97k  Mon Aug 04 2008 10:06:43  chwon
	4250: use USBPUMP_API_INCALL_TRAILER and USBPUMP_API_OUTCALL_TRAILER

   2.01a  Sun May 17 2009 15:34:41  chwon
	8040: Add USBPUMP_ANNUNCIATOR_STATUS_SUCCESS/FAILED macro

*/

#ifndef _USBPUMP_ANNUNCIATOR_H_		/* prevent multiple includes */
#define _USBPUMP_ANNUNCIATOR_H_

#ifndef _USBIOCTL_API_H_
# include "usbioctl_api.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_ARG	(USBPUMP_ANNUNCIATOR_STATUS, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_ANNUNCIATOR_NOTIFICATION, UINT32);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_ANNUNCIATOR);
__TMS_TYPE_DEF_UNION	(USBPUMP_ANNUNCIATOR_OBSERVER_INCALL);
__TMS_TYPE_DEF_UNION	(USBPUMP_ANNUNCIATOR_SENDER_INCALL);
__TMS_TYPE_DEF_UNION	(USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_ANNUNCIATOR_SESSION);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_ANNUNCIATOR_SYNC_BLOCK);


/****************************************************************************\
|
|	Annunciator observer in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_ANNUNCIATOR_REGISTER_NOTIFICATION_FN

Function:
	Register notification

Definition:
	typedef USBPUMP_ANNUNCIATOR_STATUS
	(*USBPUMP_ANNUNCIATOR_REGISTER_NOTIFICATION_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle
		);

Description:
	This is observer in-call function.  It will be called by observer to
	register notification.  Annunciator turn on register notification and
	annunciator will send notification when new notification is available.

Returns:
	USBPUMP_ANNUNCIATOR_STATUS_OK
	USBPUMP_ANNUNCIATOR_STATUS_INVALID_SESSION_HANDLE

Notes:
	None.

See also:
	USBPUMP_ANNUNCIATOR_UNREGISTER_NOTIFICATION_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_ANNUNCIATOR_REGISTER_NOTIFICATION_FN,
__TMS_USBPUMP_ANNUNCIATOR_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */
	));

/*

Type:  USBPUMP_ANNUNCIATOR_UNREGISTER_NOTIFICATION_FN

Function:
	Unregister notification

Definition:
	typedef USBPUMP_ANNUNCIATOR_STATUS
	(*USBPUMP_ANNUNCIATOR_UNREGISTER_NOTIFICATION_FN)(
		USBPUMP_SESSION_HANDLE	SessionHandle
		);

Description:
	This is observer in-call function.  It will be called by observer to
	unregister notification.  Annunciator turn off register notification and
	annunciator will not send notification when new notification is available.

Returns:
	USBPUMP_ANNUNCIATOR_STATUS_OK
	USBPUMP_ANNUNCIATOR_STATUS_INVALID_SESSION_HANDLE

Notes:
	None.

See also:
	USBPUMP_ANNUNCIATOR_REGISTER_NOTIFICATION_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_ANNUNCIATOR_UNREGISTER_NOTIFICATION_FN,
__TMS_USBPUMP_ANNUNCIATOR_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE	/* SessionHandle */
	));


/*
||	Annunciator Observer InCalls
*/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_API_INCALL_HEADER;

	__TMS_USBPUMP_ANNUNCIATOR_REGISTER_NOTIFICATION_FN *	pRegisterFn;
	__TMS_USBPUMP_ANNUNCIATOR_UNREGISTER_NOTIFICATION_FN *	pUnregisterFn;

	__TMS_USBPUMP_API_INCALL_TRAILER;
	};

union __TMS_UNIONNAME	(USBPUMP_ANNUNCIATOR_OBSERVER_INCALL)
	{
	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_CONTENTS	Observer;
	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION;
	};

#define	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_INIT_V1(			\
	a_pCloseFn,								\
	a_pRegisterFn,								\
	a_pUnregisterFn								\
	)									\
	{{									\
	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(				\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,				\
		sizeof(__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_INCALL),		\
		a_pCloseFn							\
		),								\
	a_pRegisterFn,								\
	a_pUnregisterFn,							\
	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(				\
		__TMS_USBPUMP_API_INCALL_MAGIC_END				\
		)								\
	}}

#define	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pRegisterFn,								\
	a_pUnregisterFn								\
	)									\
    do	{									\
	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(				\
		a_pInCall,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,				\
		sizeof(*(a_pInCall)),						\
		a_pCloseFn							\
		);								\
	(a_pInCall)->Observer.pRegisterFn = a_pRegisterFn;			\
	(a_pInCall)->Observer.pUnregisterFn = a_pUnregisterFn;			\
	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(				\
		a_pInCall,							\
		Observer,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_END				\
		);								\
	} while (0)


/****************************************************************************\
|
|	Annunciator sender in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_FN

Function:
	Send notification to observer with synchronous

Definition:
	typedef USBPUMP_ANNUNCIATOR_STATUS
	(*USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_ANNUNCIATOR_NOTIFICATION	NotificationId,
		CONST VOID *				pNotificationInfo,
		BYTES					sizeNotificationInfo
		);

Description:
	This is sender in-call function.  It will be called by sender to
	send notification to observer.  This is synchronous call.

Returns:
	USBPUMP_ANNUNCIATOR_STATUS_OK
	USBPUMP_ANNUNCIATOR_STATUS_INVALID_SESSION_HANDLE

Notes:
	None.

See also:
	USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_FN,
__TMS_USBPUMP_ANNUNCIATOR_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,
	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION	/* NotificationId */,
	__TMS_CONST __TMS_VOID *		/* pNotificationInfo */,
	__TMS_BYTES				/* sizeNotificationInfo */
	));

/*

Type:  USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_FN

Function:
	Send notification to observer with asynchronous

Definition:
	typedef VOID
	(*USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_CB_FN *pCB,
		VOID *					pCallBackContext,
		USBPUMP_ANNUNCIATOR_SYNC_BLOCK *	pSyncBlock,
		USBPUMP_ANNUNCIATOR_NOTIFICATION	NotificationId,
		CONST VOID *				pNotificationInfo,
		BYTES					sizeNotificationInfo
		);

Description:
	This is sender in-call function.  It will be called by sender to
	send notification to observer.  This is asynchronous call. Sender should
	provide USBPUMP_ANNUNCIATOR_SYNC_BLOCK memory and this memory will be
	used by annunicator internally.  When sender get callback, sender can
	free this memory.

Returns:
	Nothing.

Notes:
	This is asynchronous function. Specified callback function will be called
	when complete sending notification or error. Here is error code of this:

	USBPUMP_ANNUNCIATOR_STATUS_OK
	USBPUMP_ANNUNCIATOR_STATUS_INVALID_SESSION_HANDLE
	USBPUMP_ANNUNCIATOR_STATUS_INVALID_PARAMETER

See also:
	USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *				/* pClientContext */,
	__TMS_USBPUMP_ANNUNCIATOR_SYNC_BLOCK *	/* pSyncBlock */,
	__TMS_USBPUMP_ANNUNCIATOR_STATUS	/* Status */
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE		/* SessionHandle */,		\
	__TMS_USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_CB_FN * /* pCB */,	\
	__TMS_VOID *				/* pCallBackContext */,		\
	__TMS_USBPUMP_ANNUNCIATOR_SYNC_BLOCK *	/* pSyncBlock */,		\
	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION	/* NotificationId */,		\
	__TMS_CONST __TMS_VOID *		/* pNotificationInfo */,	\
	__TMS_BYTES				/* sizeNotificationInfo */	\
	));

struct __TMS_STRUCTNAME(USBPUMP_ANNUNCIATOR_SYNC_BLOCK)
	{
	__TMS_CALLBACKCOMPLETION		Event;
	__TMS_USBPUMP_ANNUNCIATOR *		pAnnunciator;
	__TMS_USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_CB_FN *pCallBack;
	__TMS_VOID *				pCallBackContext;
	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION	NotificationId;
	__TMS_CONST __TMS_VOID *		pNotificationInfo;
	__TMS_BYTES				sizeNotificationInfo;
	};


/*
||	Annunciator Observer InCalls
*/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_ANNUNCIATOR_SENDER_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_ANNUNCIATOR_SENDER_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_API_INCALL_HEADER;

	__TMS_USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_FN *	pSendFn;
	__TMS_USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_ASYNC_FN *	pSendAsyncFn;

	__TMS_USBPUMP_API_INCALL_TRAILER;
	};

union __TMS_UNIONNAME(USBPUMP_ANNUNCIATOR_SENDER_INCALL)
	{
	__TMS_USBPUMP_ANNUNCIATOR_SENDER_INCALL_CONTENTS	Sender;
	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION;
	};

#define	__TMS_USBPUMP_ANNUNCIATOR_SENDER_INCALL_INIT_V1(			\
	a_pCloseFn,								\
	a_pSendFn,								\
	a_pSendAsyncFn								\
	)									\
	{{									\
	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(				\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,				\
		sizeof(__TMS_USBPUMP_ANNUNCIATOR_SENDER_INCALL),		\
		a_pCloseFn							\
		),								\
	a_pSendFn,								\
	a_pSendAsyncFn,								\
	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(				\
		__TMS_USBPUMP_API_INCALL_MAGIC_END				\
		)								\
	}}

#define	__TMS_USBPUMP_ANNUNCIATOR_SENDER_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pSendFn,								\
	a_pSendAsyncFn								\
	)									\
    do  {									\
	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(				\
		a_pInCall,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,				\
		sizeof(*(a_pInCall)),						\
		a_pCloseFn							\
		);								\
	(a_pInCall)->Sender.pSendFn = a_pSendFn;				\
	(a_pInCall)->Sender.pSendAsyncFn = a_pSendAsyncFn;			\
	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(				\
		a_pInCall,							\
		Sender,								\
		__TMS_USBPUMP_API_INCALL_MAGIC_END				\
		);								\
	} while (0)


/****************************************************************************\
|
|	Annunciator observer out-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_ANNUNCIATOR_NOTIFICATION_FN

Function:
	Deliver notification to observer

Definition:
	typedef VOID
	(*USBPUMP_ANNUNCIATOR_NOTIFICATION_FN)(
		VOID *					pClientHandle,
		USBPUMP_ANNUNCIATOR_NOTIFICATION	NotificationId,
		CONST VOID *				pNotificationInfo,
		BYTES					sizeNotificationInfo
		);

Description:
	This is observer out-call function.  It will be called by annunciator to
	deliver notification to observer.

Returns:
	Nothing

Notes:
	None.

See also:
	USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_ANNUNCIATOR_NOTIFICATION_FN,
__TMS_VOID,
	(
	__TMS_VOID *				/* pClientHandle */,
	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION	/* NotificationId */,
	__TMS_CONST __TMS_VOID *		/* pNotificationInfo */,
	__TMS_BYTES				/* sizeNotificationInfo */
	));


__TMS_TYPE_DEF_STRUCT	(USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_CONTENTS)
	{
	__TMS_USBPUMP_API_OUTCALL_HEADER;

	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION_FN *	pNotificationFn;

	__TMS_USBPUMP_API_OUTCALL_TRAILER;
	};

union __TMS_UNIONNAME(USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL)
	{
	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_CONTENTS	Observer;
	__TMS_USBPUMP_API_OUTCALL_CONTENTS__UNION;
	};

#define	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_INIT_V1(			\
	a_pNotificationFn							\
	)									\
	{{									\
	__TMS_USBPUMP_API_OUTCALL_HEADER_INIT_V1(				\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,				\
		sizeof(__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL)		\
		),								\
	a_pNotificationFn,							\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_INIT_V1(				\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END				\
		)								\
	}}

#define	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_SETUP_V1(			\
	a_pOutCall,								\
	a_pNotificationFn							\
	)									\
    do	{									\
	__TMS_USBPUMP_API_OUTCALL_HEADER_SETUP_V1(				\
		a_pOutCall,							\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,				\
		sizeof(*(a_pOutCall))						\
		);								\
	(a_pOutCall)->Observer.pNotificationFn = a_pNotificationFn;		\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_SETUP_V1(				\
		a_pOutCall,							\
		Observer,							\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END				\
		);								\
	} while (0)


/****************************************************************************\
|
|	Definition of USBPUMP_ANNUNCIATOR_SESSION
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_ANNUNCIATOR_SESSION)
	{
	__TMS_USBPUMP_ANNUNCIATOR *			pAnnunciator;

	__TMS_USBPUMP_ANNUNCIATOR_SESSION *		pNext;
	__TMS_USBPUMP_ANNUNCIATOR_SESSION *		pLast;

	__TMS_UINT32					fObserverSession : 1;
	__TMS_UINT32					fSenderSession : 1;
	__TMS_UINT32					fRegistered : 1;

	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION_FN *	pNotificationFn;
	__TMS_VOID *					pClientHandle;
	};


/****************************************************************************\
|
|	Definition of USBPUMP_ANNUNCIATOR
|
\****************************************************************************/

/*

Type:	USBPUMP_ANNUNCIATOR

Function:
	The generic object that represents a DataPump annunciator.

Description:
	A USBPUMP_ANNUNCIATOR is the annunciator objects provide a way to send
	notifications to the registered objects.  They broadcast notifications
	to any objects that have registered to receive notifications.

	A USBPUMP_ANNUNCIATOR object (or annunciator) provides a mechanism for
	synchronously broadcasting information within a task.

Contents:
	USBPUMP_OBJECT_HEADER ObjectHeader;
		The standard object header.  The tag is |USBPUMP_ANNUNCIATOR_TAG|.
		The IOCTL parent is the pointer to the next object closer
		to the |UPLATFORM| -- depending on the design of the ANNUNCIATOR,
		there may be no objects, or several objects, in series between
		this ANNUNCIATOR and the UPLATFORM.

	UPLATFORM *pPlatform;
		Pointer to the platform object, for convenience.

	CONST USBGUID *pObserverGuid;
		The annunciator Observer's GUID.  For claiming the
		USBPUMP_IOCTL_API_OPEN IOCTL, annunciator check pInArg->pGuid to
		make sure it's the right GUID for the annunciator API.  If not
		match, reject the IOCTL as unclaimed.

	CONST USBGUID *pSenderGuid;
		The annunciator Sender's GUID.  For claiming the
		USBPUMP_IOCTL_API_OPEN IOCTL, annunciator check pInArg->pGuid to
		make sure it's the right GUID for the annunciator API.  If not
		match, reject the IOCTL as unclaimed.

	UINT MaxSession;
		A configuration parameter, set the maximum number of session

	USBPUMP_ANNUNCIATOR_SESSION * pvSession;
		Annunciator open session handling memory blocks.

	USBPUMP_ANNUNCIATOR_SESSION *pFreeSession;
		Head pointer of free session list.

	USBPUMP_ANNUNCIATOR_SESSION *pObserverSession;
		Head pointer of opened observer session list.

	USBPUMP_ANNUNCIATOR_SESSION *pRegisterSession;
		Head pointer of opened and registered observer session list.

	USBPUMP_ANNUNCIATOR_SESSION *pSenderSession;
		Head pointer of opened sender session list.

See Also:
	UsbPumpAnnunciator_Create()
	UsbPumpAnnunciator_Delete()
	UsbPumpAnnunciator_Init()

*/

struct __TMS_STRUCTNAME(USBPUMP_ANNUNCIATOR)
	{
	/*
	|| UPLATFORMs are USBPUMP objects, so must begin with a header.
	*/
	__TMS_USBPUMP_OBJECT_HEADER			ObjectHeader;

	/*
	|| The annunciator implementation may need to use platform routines;
	|| so it stashes a UPLATFORM pointer.
	*/
	__TMS_UPLATFORM *				pPlatform;

	__TMS_CONST __TMS_USBGUID *			pObserverGuid;
	__TMS_CONST __TMS_USBGUID *			pSenderGuid;

	__TMS_UINT					MaxSession;
	__TMS_USBPUMP_ANNUNCIATOR_SESSION *		pvSession;

	__TMS_USBPUMP_ANNUNCIATOR_SESSION *		pFreeSession;

	__TMS_USBPUMP_ANNUNCIATOR_SESSION *		pObserverSession;
	__TMS_USBPUMP_ANNUNCIATOR_SESSION *		pRegisterSession;

	__TMS_USBPUMP_ANNUNCIATOR_SESSION *		pSenderSession;
	};


/*
|| Object related things
*/

/* the DataPump annunciator object tag */
#define	__TMS_USBPUMP_ANNUNCIATOR_TAG	__TMS_UHIL_MEMTAG('U', 'A', 'n', 'n')

/* the annunciator object name */
#define	__TMS_USBPUMP_ANNUNCIATOR_OBJECT_NAME(Text)		\
	Text ".annunciator.mcci.com"

#define	__TMS_USBPUMP_ANNUNCIATOR_OBJECT_ENUM_NAME		\
	__TMS_USBPUMP_ANNUNCIATOR_OBJECT_NAME("*")

/*
|| Sample GUID initialization macro. You can generate GUID using "uuidgen -s".
||
|| #define __TMS_USBPUMP_ANNUNCIATOR_GUID_INIT()		\
||	__TMS_USBUUID_INIT(0x3956f29c, 0x8805, 0x42fa,		\
||			   0xb3, 0x17, 0x9b, 0xa8, 0x5e, 0x7a, 0xe6, 0x03)
*/

/* USBPUMP_ANNUNCIATOR_STATUS codes */
#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_OK			\
	__TMS_USBPUMP_API_STATUS_OK
#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_INVALID_PARAMETER	\
	__TMS_USBPUMP_API_STATUS_INVALID_PARAMETER
#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_ARG_AREA_TOO_SMALL	\
	__TMS_USBPUMP_API_STATUS_ARG_AREA_TOO_SMALL
#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_BUFFER_TOO_SMALL	\
	__TMS_USBPUMP_API_STATUS_BUFFER_TOO_SMALL
#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_NOT_SUPPORTED		\
	__TMS_USBPUMP_API_STATUS_NOT_SUPPORTED
#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_NO_MORE_SESSIONS	\
	__TMS_USBPUMP_API_STATUS_NO_MORE_SESSIONS
#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_INVALID_SESSION_HANDLE	\
	__TMS_USBPUMP_API_STATUS_INVALID_SESSION_HANDLE


#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_SUCCESS(Status)	\
	((Status) == __TMS_USBPUMP_ANNUNCIATOR_STATUS_OK)

#define	__TMS_USBPUMP_ANNUNCIATOR_STATUS_FAILED(Status)		\
	((Status) != __TMS_USBPUMP_ANNUNCIATOR_STATUS_OK)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_ANNUNCIATOR *
UsbPumpAnnunciator_Create(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_CONST __TMS_USBGUID *	/* pObserverGuid */,
	__TMS_CONST __TMS_USBGUID *	/* pSenderGuid */,
	__TMS_CONST __TMS_TEXT *	/* pObjectName */,
	__TMS_UINT			/* MaxSession */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pIoctlParent	-- OPTIONAL */
	);

__TMS_VOID
UsbPumpAnnunciator_Delete(
	__TMS_USBPUMP_ANNUNCIATOR *	/* pAnnunciator */
	);

__TMS_VOID
UsbPumpAnnunciator_Init(
	__TMS_USBPUMP_ANNUNCIATOR *	/* pAnnunciator */,
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_CONST __TMS_USBGUID *	/* pObserverGuid */,
	__TMS_CONST __TMS_USBGUID *	/* pSenderGuid */,
	__TMS_CONST __TMS_TEXT *	/* pObjectName */,
	__TMS_UINT			/* MaxSession */,
	__TMS_USBPUMP_ANNUNCIATOR_SESSION *	/* pvSession */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pIoctlParent	-- OPTIONAL */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_INIT_V1(			\
	a_pCloseFn,								\
	a_pRegisterFn,								\
	a_pUnregisterFn								\
	)	\
	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_INIT_V1(			\
	a_pCloseFn,								\
	a_pRegisterFn,								\
	a_pUnregisterFn								\
	)
# define USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pRegisterFn,								\
	a_pUnregisterFn								\
	)	\
	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pRegisterFn,								\
	a_pUnregisterFn								\
	)
# define USBPUMP_ANNUNCIATOR_SENDER_INCALL_INIT_V1(			\
	a_pCloseFn,								\
	a_pSendFn,								\
	a_pSendAsyncFn								\
	)	\
	__TMS_USBPUMP_ANNUNCIATOR_SENDER_INCALL_INIT_V1(			\
	a_pCloseFn,								\
	a_pSendFn,								\
	a_pSendAsyncFn								\
	)
# define USBPUMP_ANNUNCIATOR_SENDER_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pSendFn,								\
	a_pSendAsyncFn								\
	)	\
	__TMS_USBPUMP_ANNUNCIATOR_SENDER_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pSendFn,								\
	a_pSendAsyncFn								\
	)
# define USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_INIT_V1(			\
	a_pNotificationFn							\
	)	\
	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_INIT_V1(			\
	a_pNotificationFn							\
	)
# define USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_SETUP_V1(			\
	a_pOutCall,								\
	a_pNotificationFn							\
	)	\
	__TMS_USBPUMP_ANNUNCIATOR_OBSERVER_OUTCALL_SETUP_V1(			\
	a_pOutCall,								\
	a_pNotificationFn							\
	)
# define USBPUMP_ANNUNCIATOR_TAG	\
   __TMS_USBPUMP_ANNUNCIATOR_TAG
# define USBPUMP_ANNUNCIATOR_OBJECT_NAME(Text)	\
   __TMS_USBPUMP_ANNUNCIATOR_OBJECT_NAME(Text)
# define USBPUMP_ANNUNCIATOR_OBJECT_ENUM_NAME	\
   __TMS_USBPUMP_ANNUNCIATOR_OBJECT_ENUM_NAME
# define USBPUMP_ANNUNCIATOR_STATUS_OK	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_OK
# define USBPUMP_ANNUNCIATOR_STATUS_INVALID_PARAMETER	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_INVALID_PARAMETER
# define USBPUMP_ANNUNCIATOR_STATUS_ARG_AREA_TOO_SMALL	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_ARG_AREA_TOO_SMALL
# define USBPUMP_ANNUNCIATOR_STATUS_BUFFER_TOO_SMALL	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_BUFFER_TOO_SMALL
# define USBPUMP_ANNUNCIATOR_STATUS_NOT_SUPPORTED	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_NOT_SUPPORTED
# define USBPUMP_ANNUNCIATOR_STATUS_NO_MORE_SESSIONS	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_NO_MORE_SESSIONS
# define USBPUMP_ANNUNCIATOR_STATUS_INVALID_SESSION_HANDLE	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_INVALID_SESSION_HANDLE
# define USBPUMP_ANNUNCIATOR_STATUS_SUCCESS(Status)	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_SUCCESS(Status)
# define USBPUMP_ANNUNCIATOR_STATUS_FAILED(Status)	\
   __TMS_USBPUMP_ANNUNCIATOR_STATUS_FAILED(Status)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_annunciator.h ****/
#endif /* _USBPUMP_ANNUNCIATOR_H_ */
