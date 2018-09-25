/* usbpump_classkit_api.h	Sun May 17 2009 15:45:11 chwon */

/*

Module:  usbpump_classkit_api.h

Function:
	Definition of ClassKit driver class IN/OUT call

Version:
	V2.01a	Sun May 17 2009 15:45:11 chwon	Edit level 8

Copyright notice:
	This file copyright (C) 2008-2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	April 2008

Revision history:
   1.97k  Tue Apr 01 2008 14:04:49 chwon
	Module created.

   1.97k  Tue Jun 03 2008 10:26:04  chwon
	Fixed typo.

   1.97k  Tue Jul 01 2008 13:05:55  chwon
	4165: redefine USBPUMP_CLASSKIT_STATUS code using USBPUMP_API_STATUS code
	4252: add USBPUMP_CLASSKIT_STATUS__MAX definition

   1.97k  Wed Jul 16 2008 11:45:07  chwon
	4301: remove USBPUMP_CLASSKIT_REGISTER_NOTIFICATION_FN and
	USBPUMP_CLASSKIT_UNREGISTER_NOTIFICATION_FN API.
	Add USBPUMP_CLASSKIT_GET_BOUND_DEVICES_FN API.
	4250: use generic INCALL and OUTCALL definition.
	4303: use USBPUMP_SESSION_HANDLE instead of VOID *

   1.97k  Mon Jul 21 2008 15:47:47  chwon
	4250: fixed ...CALL_CONTENTS__UNION definition

   1.97k  Fri Jul 25 2008 16:01:39  wkkim
	Fixed compile error in USBPUMP_CLASSKIT_CLASS_INCALL_SETUP_V1 definition.

   1.97k  Mon Aug 04 2008 10:09:19  chwon
	4250: use USBPUMP_API_INCALL_TRAILER and USBPUMP_API_OUTCALL_TRAILER

   2.01a  Sun May 17 2009 15:45:11  chwon
	8040: Add USBPUMP_CLASSKIT_STATUS_SUCCESS() and
	USBPUMP_CLASSKIT_STATUS_FAILED() macro.

*/

#ifndef _USBPUMP_CLASSKIT_API_H_	/* prevent multiple includes */
#define _USBPUMP_CLASSKIT_API_H_

#ifndef _USBPUMP_CLASSKIT_TYPES_H_
# include "usbpump_classkit_types.h"
#endif

#ifndef _USBIOCTL_API_H_
# include "usbioctl_api.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION	(USBPUMP_CLASSKIT_CLASS_INCALL);
__TMS_TYPE_DEF_UNION	(USBPUMP_CLASSKIT_CLASS_OUTCALL);
__TMS_TYPE_DEF_UNION	(USBPUMP_CLASSKIT_FUNCTION_INCALL);
__TMS_TYPE_DEF_UNION	(USBPUMP_CLASSKIT_FUNCTION_OUTCALL);


/****************************************************************************\
|
|	ClassKit driver class in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_CLASSKIT_OPEN_FUNCTION_FN

Function:
	Open a function driver instance

Definition:
	typedef VOID
	(*USBPUMP_CLASSKIT_OPEN_FUNCTION_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_CLASSKIT_OPEN_FUNCTION_CB_FN *	pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_FUNCTION *		pFunction,
		USBPUMP_CLASSKIT_FUNCTION_INCALL *	pInCallApiBuffer,
		RECSIZE					sizeInCallApiBuffer,
		VOID * 					pClientHandle,
		CONST USBPUMP_CLASSKIT_FUNCTION_OUTCALL *pOutCallApiBuffer,
		RECSIZE					sizeOutCallApiBuffer
		);

Description:
	This is driver class in-call function.  It will be called by
	client to open a function driver instance from driver class.  The client
	will receive pFunctionHandle through the callback function then the
	client can call function in-calls using pFunctionHandle.

Returns:
	Nothing

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_CLASSKIT_OPEN_FUNCTION_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallBackContext */,
	__TMS_USBPUMP_CLASSKIT_STATUS			/* ErrorCode */,
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */
	));

__TMS_FNTYPE_DEF(
USBPUMP_CLASSKIT_OPEN_FUNCTION_FN,						\
__TMS_VOID,									\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* SessionHandle */,	\
	__TMS_USBPUMP_CLASSKIT_OPEN_FUNCTION_CB_FN *	/* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_USBPUMP_USBDI_FUNCTION *			/* pFunction */, 	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL *	/* pInCallBuffer */,	\
	__TMS_RECSIZE					/* sizeInCallBuffer */,	\
	__TMS_VOID *					/* pClientHandle */,	\
	__TMS_CONST __TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL */* pOutCallBuffer */,	\
	__TMS_RECSIZE					/* sizeOutCallBuffer */	\
	));


/*

Type:  USBPUMP_CLASSKIT_GET_NUM_DEVICES_FN

Function:
	Get the number of functions currently bound to the ClassKit driver class

Definition:
	typedef VOID
	(*USBPUMP_CLASSKIT_GET_NUM_DEVICES_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_CLASSKIT_GET_NUM_DEVICES_CB_FN *pCallback,
		VOID *					pCallbackContext
		);

Description:
	This is ClassKit driver class in-call API.  It will be called by
	client to get the number of functions currently bound to the ClassKit
	driver class.

Returns:
	Nothing

Notes:
	None.

See also:
	USBPUMP_CLASSKIT_GET_BOUND_DEVICES_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_CLASSKIT_GET_NUM_DEVICES_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pCallbackContext */,
	__TMS_USBPUMP_CLASSKIT_STATUS			/* ErrorCode */,
	__TMS_UINT32					/* ulNumOfInstances */
	));

__TMS_FNTYPE_DEF(
USBPUMP_CLASSKIT_GET_NUM_DEVICES_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* SessionHandle */,
	__TMS_USBPUMP_CLASSKIT_GET_NUM_DEVICES_CB_FN *	/* pCallback */,
	__TMS_VOID *					/* pCallbackContext */
	));


/*

Type:  USBPUMP_CLASSKIT_GET_BOUND_DEVICES_FN

Function:
	Get the information of functions currently bound to the ClassKit driver
	class

Definition:
	typedef VOID
	(*USBPUMP_CLASSKIT_GET_BOUND_DEVICES_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_CLASSKIT_GET_BOUND_DEVICES_CB_FN *pCallback,
		VOID *					pCallbackContext,
		USBPUMP_USBDI_FUNCTION **		pFunctionVector,
		BYTES					sizeFunctionVector
		);

Description:
	This is ClassKit driver class in-call API.  It will be called by
	client to get the information of functions currently bound to the
	ClassKit driver class.  If client provide pFunctionVector buffer,
	it will fill bound funtion information and it will set nWrittenFunction
	to number of copied instance information. If pFunctionVector is NULL,
	this function will return USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER
	error code.

Returns:
	Nothing

Notes:
	None.

See also:
	USBPUMP_CLASSKIT_GET_BOUND_DEVICES_FN

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_CLASSKIT_GET_BOUND_DEVICES_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pCallbackContext */,	\
	__TMS_USBPUMP_CLASSKIT_STATUS			/* ErrorCode */,	\
	__TMS_UINT32					/* nWrittenFunction */,	\
	__TMS_USBPUMP_USBDI_FUNCTION **			/* pFunctionVector */,	\
	__TMS_BYTES					/* sizeFunctionVector */\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_CLASSKIT_GET_BOUND_DEVICES_FN,						\
__TMS_VOID,									\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* SessionHandle */,	\
	__TMS_USBPUMP_CLASSKIT_GET_BOUND_DEVICES_CB_FN */* pCallback */,	\
	__TMS_VOID *					/* pCallbackContext */,	\
	__TMS_USBPUMP_USBDI_FUNCTION **			/* pFunctionVector */,	\
	__TMS_BYTES					/* sizeFunctionVector */\
	));


#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER				\
	__TMS_USBPUMP_API_INCALL_HEADER;					\
	/* ClassKit default in-calls */						\
	__TMS_USBPUMP_CLASSKIT_OPEN_FUNCTION_FN *	pOpenFunctionFn;	\
	__TMS_USBPUMP_CLASSKIT_GET_NUM_DEVICES_FN *	pGetNumDevicesFn;	\
	__TMS_USBPUMP_CLASSKIT_GET_BOUND_DEVICES_FN *	pGetBoundDevicesFn

#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)								\
	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(			\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,			\
		a_Size,							\
		a_pCloseFn						\
		),							\
	(a_pOpenFunctionFn),						\
	(a_pGetNumDevicesFn),						\
	(a_pGetBoundDevicesFn)

#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_SETUP_V1(		\
	a_pInCall,							\
	a_Size,								\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)								\
	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(			\
		a_pInCall,						\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,			\
		a_Size,							\
		a_pCloseFn						\
		);							\
	(a_pInCall)->ClassKit.pOpenFunctionFn = (a_pOpenFunctionFn);	\
	(a_pInCall)->ClassKit.pGetNumDevicesFn = (a_pGetNumDevicesFn);	\
	(a_pInCall)->ClassKit.pGetBoundDevicesFn = (a_pGetBoundDevicesFn)


#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER			\
	__TMS_USBPUMP_API_INCALL_TRAILER

#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)								\
	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)

#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_SETUP_V1(		\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)								\
	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(			\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER;
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER;
	};

union __TMS_UNIONNAME	(USBPUMP_CLASSKIT_CLASS_INCALL)
	{
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS	ClassKit;
	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__STRUCT		\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS	ClassKit

#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__UNION		\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS	ClassKit;	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL		ClassKitCast;	\
	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION


#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_CLASSKIT_CLASS_INCALL),		\
		a_pCloseFn,						\
		a_pOpenFunctionFn,					\
		a_pGetNumDevicesFn,					\
		a_pGetBoundDevicesFn					\
		),							\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn						\
	)								\
    do	{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_SETUP_V1(		\
		a_pInCall,						\
		sizeof(*(a_pInCall)),					\
		a_pCloseFn,						\
		a_pOpenFunctionFn,					\
		a_pGetNumDevicesFn,					\
		a_pGetBoundDevicesFn					\
		);							\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_SETUP_V1(		\
		a_pInCall,						\
		ClassKit,						\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	ClassKit driver class out-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_CLASSKIT_NOTIFICATION_FN

Function:
	Deliver notification to the registered client

Definition:
	typedef VOID
	(*USBPUMP_CLASSKIT_NOTIFICATION_FN)(
		VOID *				pClientHandle,
		USBPUMP_CLASSKIT_NOTIFICATION	NotificationId,
		CONST VOID *			pNotificationInfo,
		BYTES				sizeNotificationInfo
		);

Description:
	This is ClassKit out-call function.  It will be called by driver class to
	deliver notification to the registered client.

Returns:
	Nothing

Notes:
	None.

See also:
	USBPUMP_CLASSKIT_SEND_NOTIFICATION_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_CLASSKIT_NOTIFICATION_FN,
__TMS_VOID,
	(
	__TMS_VOID *				/* pClientHandle */,
	__TMS_USBPUMP_CLASSKIT_NOTIFICATION	/* NotificationId */,
	__TMS_CONST __TMS_VOID *		/* pNotificationInfo */,
	__TMS_BYTES				/* sizeNotificationInfo */
	));


#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER			\
	__TMS_USBPUMP_API_OUTCALL_HEADER;				\
	__TMS_USBPUMP_CLASSKIT_NOTIFICATION_FN *	pNotificationFn

#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pNotificationFn						\
	)								\
	__TMS_USBPUMP_API_OUTCALL_HEADER_INIT_V1(			\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,			\
		a_Size							\
		),							\
	(a_pNotificationFn)

#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER_SETUP_V1(		\
	a_pOutCall,							\
	a_Size,								\
	a_pNotificationFn						\
	)								\
	__TMS_USBPUMP_API_OUTCALL_HEADER_SETUP_V1(			\
		a_pOutCall,						\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,			\
		a_Size							\
		);							\
	(a_pOutCall)->ClassKit.pNotificationFn = (a_pNotificationFn)

#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER			\
	__TMS_USBPUMP_API_OUTCALL_TRAILER

#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)								\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)

#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER_SETUP_V1(		\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)								\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_SETUP_V1(			\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER;
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER;
	};

union __TMS_UNIONNAME	(USBPUMP_CLASSKIT_CLASS_OUTCALL)
	{
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS	ClassKit;
	__TMS_USBPUMP_API_OUTCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS__STRUCT		\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS	ClassKit

#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS__UNION		\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS	ClassKit;	\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL		ClassKitCast;	\
	__TMS_USBPUMP_API_OUTCALL_CONTENTS__UNION


#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_INIT_V1(			\
	a_pNotificationFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL),		\
		a_pNotificationFn					\
		),							\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_SETUP_V1(			\
	a_pOutCall,							\
	a_pNotificationFn						\
	)								\
    do	{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER_SETUP_V1(		\
		a_pOutCall,						\
		sizeof(*(a_pOutCall)),					\
		a_pNotificationFn					\
		);							\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER_SETUP_V1(		\
		a_pOutCall,						\
		ClassKit,						\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	ClassKit driver function in-call function type definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER			\
	__TMS_USBPUMP_API_INCALL_HEADER

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pCloseFn							\
	)								\
	__TMS_USBPUMP_API_INCALL_HEADER_INIT_V1(			\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,			\
		a_Size,							\
		a_pCloseFn						\
		)

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_SETUP_V1(		\
	a_pInCall,							\
	a_Size,								\
	a_pCloseFn							\
	)								\
	__TMS_USBPUMP_API_INCALL_HEADER_SETUP_V1(			\
		a_pInCall,						\
		__TMS_USBPUMP_API_INCALL_MAGIC_BEGIN,			\
		a_Size,							\
		a_pCloseFn						\
		)

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER			\
	__TMS_USBPUMP_API_INCALL_TRAILER

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)								\
	__TMS_USBPUMP_API_INCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_SETUP_V1(	\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)								\
	__TMS_USBPUMP_API_INCALL_TRAILER_SETUP_V1(			\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER;
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER;
	};

union __TMS_UNIONNAME	(USBPUMP_CLASSKIT_FUNCTION_INCALL)
	{
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS	ClassKit;
	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__STRUCT		\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS	ClassKit

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__UNION		\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS	ClassKit;	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL		ClassKitCast;	\
	__TMS_USBPUMP_API_INCALL_CONTENTS__UNION


#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_INIT_V1(			\
	a_pCloseFn							\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL),		\
		a_pCloseFn						\
		),							\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn							\
	)								\
    do	{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_SETUP_V1(		\
		a_pInCall,						\
		sizeof(*(a_pInCall)),					\
		a_pCloseFn						\
		);							\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_SETUP_V1(	\
		a_pInCall,						\
		ClassKit,						\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	ClassKit driver function out-call function type definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER			\
	__TMS_USBPUMP_API_OUTCALL_HEADER;				\
	__TMS_USBPUMP_CLASSKIT_NOTIFICATION_FN *	pNotificationFn

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pNotificationFn						\
	)								\
	__TMS_USBPUMP_API_OUTCALL_HEADER_INIT_V1(			\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,			\
		a_Size							\
		),							\
	(a_pNotificationFn)

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER_SETUP_V1(	\
	a_pOutCall,							\
	a_Size,								\
	a_pNotificationFn						\
	)								\
	__TMS_USBPUMP_API_OUTCALL_HEADER_SETUP_V1(			\
		a_pOutCall,						\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_BEGIN,			\
		a_Size							\
		);							\
	(a_pOutCall)->ClassKit.pNotificationFn = (a_pNotificationFn)

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER			\
	__TMS_USBPUMP_API_OUTCALL_TRAILER

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER_INIT_V1(	\
	a_MagicEnd							\
	)								\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_INIT_V1(			\
	a_MagicEnd							\
	)

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER_SETUP_V1(	\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)								\
	__TMS_USBPUMP_API_OUTCALL_TRAILER_SETUP_V1(			\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER;
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER;
	};

union __TMS_UNIONNAME	(USBPUMP_CLASSKIT_FUNCTION_OUTCALL)
	{
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS	ClassKit;
	__TMS_USBPUMP_API_OUTCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS__STRUCT	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS	ClassKit

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS__UNION		\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS ClassKit;	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL		ClassKitCast;	\
	__TMS_USBPUMP_API_OUTCALL_CONTENTS__UNION


#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_INIT_V1(		\
	a_pNotificationFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL),	\
		a_pNotificationFn					\
		),							\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER_INIT_V1(	\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_SETUP_V1(		\
	a_pOutCall,							\
	a_pNotificationFn						\
	)								\
    do	{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER_SETUP_V1(	\
		a_pOutCall,						\
		sizeof(*(a_pOutCall)),					\
		a_pNotificationFn					\
		);							\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER_SETUP_V1(	\
		a_pOutCall,						\
		ClassKit,						\
		__TMS_USBPUMP_API_OUTCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	USBPUMP_CLASSKIT_STATUS Codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_CLASSKIT_STATUS_OK			\
	__TMS_USBPUMP_API_STATUS_OK
#define	__TMS_USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER		\
	__TMS_USBPUMP_API_STATUS_INVALID_PARAMETER
#define	__TMS_USBPUMP_CLASSKIT_STATUS_ARG_AREA_TOO_SMALL	\
	__TMS_USBPUMP_API_STATUS_ARG_AREA_TOO_SMALL
#define	__TMS_USBPUMP_CLASSKIT_STATUS_BUFFER_TOO_SMALL		\
	__TMS_USBPUMP_API_STATUS_BUFFER_TOO_SMALL
#define	__TMS_USBPUMP_CLASSKIT_STATUS_NOT_SUPPORTED		\
	__TMS_USBPUMP_API_STATUS_NOT_SUPPORTED
#define	__TMS_USBPUMP_CLASSKIT_STATUS_NO_MORE_SESSIONS		\
	__TMS_USBPUMP_API_STATUS_NO_MORE_SESSIONS
#define	__TMS_USBPUMP_CLASSKIT_STATUS_INVALID_SESSION_HANDLE	\
	__TMS_USBPUMP_API_STATUS_INVALID_SESSION_HANDLE

#define	__TMS_USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE	\
	(__TMS_USBPUMP_API_STATUS__MAX + 0)
#define	__TMS_USBPUMP_CLASSKIT_STATUS_FUNCTION_ALREADY_OPENED	\
	(__TMS_USBPUMP_API_STATUS__MAX + 1)
#define	__TMS_USBPUMP_CLASSKIT_STATUS_FUNCTION_NOT_OPENED	\
	(__TMS_USBPUMP_API_STATUS__MAX + 2)

#define	__TMS_USBPUMP_CLASSKIT_STATUS__MAX	/* 4252 */	\
	(__TMS_USBPUMP_API_STATUS__MAX + 3)


#define	__TMS_USBPUMP_CLASSKIT_STATUS_SUCCESS(Status)		\
	((Status) == __TMS_USBPUMP_CLASSKIT_STATUS_OK)

#define	__TMS_USBPUMP_CLASSKIT_STATUS_FAILED(Status)		\
	((Status) != __TMS_USBPUMP_CLASSKIT_STATUS_OK)


/****************************************************************************\
|
|	CLASSKIT Notification Definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL		0
#define	__TMS_USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE		1
#define	__TMS_USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN		2
#define	__TMS_USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE		3

#define	__TMS_USBPUMP_CLASSKIT_EVENT__MAX			4 /* 4252 */

/*
|| When you add names to the above list, please also update the following
|| debugging table.
*/
#define	__TMS_USBPUMP_CLASSKIT_EVENT_NAMES__INIT		\
	"USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL",		\
	"USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE",		\
	"USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN",			\
	"USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE",		\


__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_UINT			nOpenClients;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_UINT			nOpenClients;
	};


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_API_CLOSE_FN
UsbPumpClassKitI_CloseSession;

__TMS_USBPUMP_CLASSKIT_OPEN_FUNCTION_FN
UsbPumpClassKitI_OpenFunction;

__TMS_USBPUMP_CLASSKIT_GET_NUM_DEVICES_FN
UsbPumpClassKitI_GetNumDevices;

__TMS_USBPUMP_CLASSKIT_GET_BOUND_DEVICES_FN
UsbPumpClassKitI_GetBoundDevices;

__TMS_USBPUMP_API_CLOSE_FN
UsbPumpClassKitI_CloseFunction;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_CLASSKIT_CLASS_INCALL_HEADER	\
   __TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER
# define USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)
# define USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_SETUP_V1(		\
	a_pInCall,							\
	a_Size,								\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_SETUP_V1(		\
	a_pInCall,							\
	a_Size,								\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)
# define USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER	\
   __TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER
# define USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)
# define USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_SETUP_V1(		\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_SETUP_V1(		\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)
# define USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__STRUCT	\
   __TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__STRUCT
# define USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__UNION	\
   __TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__UNION
# define USBPUMP_CLASSKIT_CLASS_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_INIT_V1(			\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn						\
	)
# define USBPUMP_CLASSKIT_CLASS_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_SETUP_V1(			\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn						\
	)
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER	\
   __TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pNotificationFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pNotificationFn						\
	)
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER_SETUP_V1(		\
	a_pOutCall,							\
	a_Size,								\
	a_pNotificationFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_HEADER_SETUP_V1(		\
	a_pOutCall,							\
	a_Size,								\
	a_pNotificationFn						\
	)
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER	\
   __TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER_SETUP_V1(		\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_TRAILER_SETUP_V1(		\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS__STRUCT	\
   __TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS__STRUCT
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS__UNION	\
   __TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_CONTENTS__UNION
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_INIT_V1(			\
	a_pNotificationFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_INIT_V1(			\
	a_pNotificationFn						\
	)
# define USBPUMP_CLASSKIT_CLASS_OUTCALL_SETUP_V1(			\
	a_pOutCall,							\
	a_pNotificationFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_CLASS_OUTCALL_SETUP_V1(			\
	a_pOutCall,							\
	a_pNotificationFn						\
	)
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pCloseFn							\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pCloseFn							\
	)
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_SETUP_V1(		\
	a_pInCall,							\
	a_Size,								\
	a_pCloseFn							\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_SETUP_V1(		\
	a_pInCall,							\
	a_Size,								\
	a_pCloseFn							\
	)
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_INIT_V1(		\
	a_MagicEnd							\
	)
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_SETUP_V1(	\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_SETUP_V1(	\
	a_pInCall,							\
	a_Container,							\
	a_MagicEnd							\
	)
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__STRUCT	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__STRUCT
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__UNION	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__UNION
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_INIT_V1(			\
	a_pCloseFn							\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_INIT_V1(			\
	a_pCloseFn							\
	)
# define USBPUMP_CLASSKIT_FUNCTION_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn							\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn							\
	)
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pNotificationFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER_INIT_V1(		\
	a_Size,								\
	a_pNotificationFn						\
	)
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER_SETUP_V1(	\
	a_pOutCall,							\
	a_Size,								\
	a_pNotificationFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_HEADER_SETUP_V1(	\
	a_pOutCall,							\
	a_Size,								\
	a_pNotificationFn						\
	)
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER_INIT_V1(	\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER_INIT_V1(	\
	a_MagicEnd							\
	)
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER_SETUP_V1(	\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_TRAILER_SETUP_V1(	\
	a_pOutCall,							\
	a_Container,							\
	a_MagicEnd							\
	)
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS__STRUCT	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS__STRUCT
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS__UNION	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_CONTENTS__UNION
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_INIT_V1(		\
	a_pNotificationFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_INIT_V1(		\
	a_pNotificationFn						\
	)
# define USBPUMP_CLASSKIT_FUNCTION_OUTCALL_SETUP_V1(		\
	a_pOutCall,							\
	a_pNotificationFn						\
	)	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_OUTCALL_SETUP_V1(		\
	a_pOutCall,							\
	a_pNotificationFn						\
	)
# define USBPUMP_CLASSKIT_STATUS_OK	\
   __TMS_USBPUMP_CLASSKIT_STATUS_OK
# define USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER	\
   __TMS_USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER
# define USBPUMP_CLASSKIT_STATUS_ARG_AREA_TOO_SMALL	\
   __TMS_USBPUMP_CLASSKIT_STATUS_ARG_AREA_TOO_SMALL
# define USBPUMP_CLASSKIT_STATUS_BUFFER_TOO_SMALL	\
   __TMS_USBPUMP_CLASSKIT_STATUS_BUFFER_TOO_SMALL
# define USBPUMP_CLASSKIT_STATUS_NOT_SUPPORTED	\
   __TMS_USBPUMP_CLASSKIT_STATUS_NOT_SUPPORTED
# define USBPUMP_CLASSKIT_STATUS_NO_MORE_SESSIONS	\
   __TMS_USBPUMP_CLASSKIT_STATUS_NO_MORE_SESSIONS
# define USBPUMP_CLASSKIT_STATUS_INVALID_SESSION_HANDLE	\
   __TMS_USBPUMP_CLASSKIT_STATUS_INVALID_SESSION_HANDLE
# define USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE	\
   __TMS_USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE
# define USBPUMP_CLASSKIT_STATUS_FUNCTION_ALREADY_OPENED	\
   __TMS_USBPUMP_CLASSKIT_STATUS_FUNCTION_ALREADY_OPENED
# define USBPUMP_CLASSKIT_STATUS_FUNCTION_NOT_OPENED	\
   __TMS_USBPUMP_CLASSKIT_STATUS_FUNCTION_NOT_OPENED
# define USBPUMP_CLASSKIT_STATUS__MAX	\
   __TMS_USBPUMP_CLASSKIT_STATUS__MAX
# define USBPUMP_CLASSKIT_STATUS_SUCCESS(Status)	\
   __TMS_USBPUMP_CLASSKIT_STATUS_SUCCESS(Status)
# define USBPUMP_CLASSKIT_STATUS_FAILED(Status)	\
   __TMS_USBPUMP_CLASSKIT_STATUS_FAILED(Status)
# define USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL	\
   __TMS_USBPUMP_CLASSKIT_EVENT_DEVICE_ARRIVAL
# define USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE	\
   __TMS_USBPUMP_CLASSKIT_EVENT_DEVICE_DEPARTURE
# define USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN	\
   __TMS_USBPUMP_CLASSKIT_EVENT_FUNCTION_OPEN
# define USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE	\
   __TMS_USBPUMP_CLASSKIT_EVENT_FUNCTION_CLOSE
# define USBPUMP_CLASSKIT_EVENT__MAX	\
   __TMS_USBPUMP_CLASSKIT_EVENT__MAX
# define USBPUMP_CLASSKIT_EVENT_NAMES__INIT	\
   __TMS_USBPUMP_CLASSKIT_EVENT_NAMES__INIT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_classkit_api.h ****/
#endif /* _USBPUMP_CLASSKIT_API_H_ */
