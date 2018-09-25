/* usbpump_usbd_class_generic_api.h	Sat Dec 12 2009 21:32:40 wkkim */

/*

Module:  usbpump_usbd_class_generic_api.h

Function:
	Definition of API Generic Device functions for USBDI clients.

Version:
	V2.01a	Sat Dec 12 2009 21:32:40 wkkim	Edit level 11

Copyright notice:
	This file copyright (C) 2008-2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Wonkun Kim, MCCI Corporation	April 2008

Revision history:
   1.97k  Wed Apr 30 2008 13:26:29  wkkim
	Module created.

   1.97k  Thu Jul 17 2008 11:49:03  chwon
	4165: redefine USBPUMP_USBDI_GENERIC_STATUS code with
	USBPUMP_CLASSKIT_STATUS code.
	4250: use generic INCALL and OUTCALL definition.
	4303: use USBPUMP_SESSION_HANDLE instead of VOID *

   1.97k  Fri Jul 18 2008 13:14:36  wkkim
	Fix typos

   1.97k  Mon Jul 21 2008 15:53:28  chwon
	4250: fixed USBPUMP_USBDI_CLASS_GENERIC_INCALL definition

   1.97k  Fri Jul 25 2008 15:53:08  wkkim
	Add object enumeration name definition.
	Include GCD configuration header file.

   1.97k  Fri Oct 10 2008 14:43:36  wkkim
	Added status codes related to USBD error codes.

   1.97k  Thu Dec 04 2008 16:32:30  wkkim
	Fixed armads warnings

   2.01a  Tue Mar 31 2009 14:56:34  chwon
	7730: fix typo; UINT32 -> BYTES

   2.01a  Fri Jul 17 2009 10:44:02  skpark
	Fixed macro expansion error

   2.01a  Mon Nov 23 2009 10:13:31  luxw
	9542: Added proto type declaration of stream read/write function
	and callback functions.

   2.01a  Sat Dec 12 2009 21:32:40  wkkim
	9249: Added a reset pipe control flag to GCD reset pipe API

*/

#ifndef _USBPUMP_USBD_CLASS_GENERIC_API_H_	/* prevent multiple includes */
#define _USBPUMP_USBD_CLASS_GENERIC_API_H_

#ifndef _USBPUMP_USBD_CLASS_GENERIC_CONFIG_H_
# include "usbpump_usbd_class_generic_config.h"
#endif

#ifndef _USBPUMP_USBDI_CLASS_H_
# include "usbpump_usbdi_class.h"
#endif

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

#ifndef _USBPUMP_USBDI_INIT_H_
# include "usbpump_usbdi_init.h"
#endif

#ifndef _USBPUMP_CLASSKIT_API_H_
# include "usbpump_classkit_api.h"
#endif

#ifndef _USBPUMP_CLASSKIT_TYPES_H_
# include "usbpump_classkit_types.h"
#endif

/****************************************************************************\
|
|	Generic driver names
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_CLASS_GENERIC_NAME				\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_NAME("generic")

#define	__TMS_USBPUMP_USBDI_FUNCTION_GENERIC_NAME			\
	__TMS_USBPUMP_USBDI_FUNCTION_NAME("generic")

/* generated GUID by executing "uuidgen -s" */
#define	__TMS_USBPUMP_USBDI_CLASS_GENERIC_GUID_INIT()			\
	__TMS_USBUUID_INIT(0x651d5a31, 0xb714, 0x4338,			\
		0x82, 0xae, 0x99, 0x5d, 0xef, 0x57, 0xaf, 0x14)

#define	__TMS_USBPUMP_OBJECT_NAME_ENUM_HOST_GCD  			\
	__TMS_USBPUMP_USBDI_CLASS_GENERIC_NAME

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_GENERIC_DEVICE_STATE);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_GENERIC_STATUS, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_GENERIC_TIMEOUT, UINT16);


/****************************************************************************\
|
|	Generic driver class in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_USBDI_GENERIC_GET_USBD_FEATURE_FN

Function:
	Learn capabilities of USBD

Definition:
	typedef VOID
	(*USBPUMP_USBDI_GENERIC_GET_USBD_FEATURE_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_USBDI_GENERIC_GET_USBD_FEATURE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		CONST TEXT *				pFeatureName,
		SIZE_T					sizeFeatureName
		);

Description:
	This is generic driver class in-call API.  It will be called by
	client to determine a feature which the USB bus driver supports.

Returns:
	Nothing

Notes:
	Useful in order for clients to learn capabilities of USBD.
	Would be better (instead of version) to define "feature masks"
	- "I support Isoch"
	- "I support high-speed"
	- "I support wireless"
	- "I have bug-fix XXX"
	Could use getter methods feature-by-feature
	Could use getter methods based on string feature IDs

See also:
	USBPUMP_USBDI_GENERIC_GET_GENERIC_DRIVER_FEATURE_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_GET_USBD_FEATURE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pClientContext */,
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,
	__TMS_BOOL					/* fFeaturePresent */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_GET_USBD_FEATURE_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* SessionHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_GET_USBD_FEATURE_CB_FN * /* pCallBack */,
	__TMS_VOID *					/* pCallBackContext */,
	__TMS_CONST __TMS_TEXT *			/* pFeatureName */,
	__TMS_SIZE_T					/* sizeFeatureName */
	));

/*

Type:  USBPUMP_USBDI_GENERIC_GET_GENERIC_DRIVER_FEATURE_FN

Function:
	Learn capabilities of generic driver

Definition:
	typedef VOID
	(*USBPUMP_USBDI_GENERIC_GET_GENERIC_DRIVER_FEATURE_FN)(
		USBPUMP_SESSION_HANDLE			SessionHandle,
		USBPUMP_USBDI_GENERIC_GET_GENERIC_DRIVER_FEATURE_CB_FN *
							pCallBack,
		VOID *					pCallBackContext,
		CONST TEXT *				pFeatureName,
		SIZE_T					sizeFeatureName
		);

Description:
	This is generic driver class in-call API. It will be called by
	client to determine a feature which the generic class driver supports.

Returns:
	Nothing

Notes:
	Useful in order for clients to learn capabilities of generic driver.
	Would be better (instead of version) to define "feature masks"
	- "I support Isoch"
	- "I support high-speed"
	- "I support wireless"
	- "I have bug-fix XXX"
	Could use getter methods feature-by-feature
	Could use getter methods based on string feature IDs

See also:
	USBPUMP_USBDI_GENERIC_GET_USBD_FEATURE_FN

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_GET_GENERIC_DRIVER_FEATURE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pClientContext */,
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,
	__TMS_BOOL					/* fFeaturePresent */
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_GET_GENERIC_DRIVER_FEATURE_FN,				\
__TMS_VOID,									\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* SessionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_DRIVER_FEATURE_CB_FN *		\
							/* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_CONST __TMS_TEXT *			/* pFeatureName */,	\
	__TMS_SIZE_T					/* sizeFeatureName */	\
	));


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_GENERIC_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_CLASS_GENERIC_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER;

	/* Generic driver in-calls */
	__TMS_USBPUMP_USBDI_GENERIC_GET_USBD_FEATURE_FN *
							pGetUsbdFeatureFn;
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_DRIVER_FEATURE_FN *
							pGetGenDrvFeatureFn;

	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER;
	};

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_CLASS_GENERIC_INCALL);
union __TMS_UNIONNAME	(USBPUMP_USBDI_CLASS_GENERIC_INCALL)
	{
	__TMS_USBPUMP_USBDI_CLASS_GENERIC_INCALL_CONTENTS	GenDrv;
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_CONTENTS__UNION;
	};


#define	__TMS_USBPUMP_USBDI_CLASS_GENERIC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetUsbdFeatureFn,						\
	a_pGetGenDrvFeatureFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_USBDI_CLASS_GENERIC_INCALL),	\
		a_pCloseFn,						\
		a_pOpenFunctionFn,					\
		a_pGetNumDevicesFn,					\
		a_pGetBoundDevicesFn					\
		),							\
	a_pGetUsbdFeatureFn,						\
	a_pGetGenDrvFeatureFn,						\
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}

#define	__TMS_USBPUMP_USBDI_CLASS_GENERIC_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetUsbdFeatureFn,						\
	a_pGetGenDrvFeatureFn						\
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
	(a_pInCall)->GenDrv.pGetUsbdFeatureFn = a_pGetUsbdFeatureFn;	\
	(a_pInCall)->GenDrv.pGetGenDrvFeatureFn = a_pGetGenDrvFeatureFn; \
	__TMS_USBPUMP_CLASSKIT_CLASS_INCALL_TRAILER_SETUP_V1(		\
		a_pInCall,						\
		GenDrv,							\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		);							\
	} while (0)


/****************************************************************************\
|
|	Generic driver function in-call function type definition
|
\****************************************************************************/

/*

Type:  USBPUMP_USBDI_GENERIC_CANCEL_REQUEST_FN

Function:
	Cancel a pending request

Definition:
	typedef VOID
	(*USBPUMP_USBDI_GENERIC_CANCEL_REQUEST_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		VOID *					pRequestHandle
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to cancel a pending request.

Returns:
	USBPUMP_USBDI_GENERIC_STATUS_OK
	USBPUMP_USBDI_GENERIC_STATUS_INVALID_FUNCTION_HANDLE
	USBPUMP_USBDI_GENERIC_STATUS_FUNCTION_NOT_OPENED
	USBPUMP_USBDI_GENERIC_STATUS_INVALID_PARAMETER

Notes:
	no callback - just marks the request as "cancelled" - if pRequestHandle
	is not valid, then the request is ignored.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_CANCEL_REQUEST_FN,
__TMS_USBPUMP_USBDI_GENERIC_STATUS,
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_VOID *					/* pRequestHandle */
	));

/*

Type:  USBPUMP_USBDI_GENERIC_GET_DEVICE_STATE_FN

Function:
	Get current device state information

Definition:
	typedef VOID *					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_GET_DEVICE_STATE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_GET_DEVICE_STATE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_GENERIC_DEVICE_STATE *	pDeviceState
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to get current device's state information.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_GET_DEVICE_STATE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pClientContext */,
	__TMS_VOID *					/* pRequestHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,
	__TMS_USBPUMP_USBDI_GENERIC_DEVICE_STATE *	/* pDeviceState */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_GET_DEVICE_STATE_FN,
__TMS_VOID *,						/* pRequestHandle */
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_GET_DEVICE_STATE_CB_FN * /* pCallBack */,
	__TMS_VOID *					/* pCallBackContext */,
	__TMS_USBPUMP_USBDI_GENERIC_DEVICE_STATE *	/* pDeviceState */
	));

/*

Type:  USBPUMP_USBDI_GENERIC_GET_DEVICE_DESCRIPTOR_FN

Function:
	Get the device descriptor of the target device.

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_GET_DEVICE_DESCRIPTOR_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_GET_DEVICE_DESCRIPTOR_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		VOID *					pBuffer,
		BYTES					sizeBuffer
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to get the device descriptor of the target device.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None.

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_GET_DEVICE_DESCRIPTOR_CB_FN,				\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_VOID *					/* pBuffer */,		\
	__TMS_BYTES					/* sizeBuffer */,	\
	__TMS_BYTES					/* nBytesRead */	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_GET_DEVICE_DESCRIPTOR_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_DEVICE_DESCRIPTOR_CB_FN *		\
							/* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_VOID *					/* pBuffer */,		\
	__TMS_BYTES					/* sizeBuffer */	\
	));

/*

Type:  USBPUMP_USBDI_GENERIC_GET_CONFIG_DESCRIPTOR_FN

Function:
	Get the configuration descriptor of the target device.

Definition:
	typedef VOID * 					// pRequestHandle
		(*USBPUMP_USBDI_GENERIC_GET_CONFIG_DESCRIPTOR_FN)(
			USBPUMP_SESSION_HANDLE		FunctionHandle,
			USBPUMP_USBDI_GENERIC_GET_CONFIG_DESCRIPTOR_CB_FN *
							pCallBack,
			VOID *				pCallBackContext,
			VOID *				pBuffer,
			BYTES				sizeBuffer,
			UINT				iConfig
			);

Description:
	This is generic driver function in-call API. It will be called by
	client to get the configuration descriptor for a specific configuration
	number of the target device.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None.

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_GET_CONFIG_DESCRIPTOR_CB_FN,				\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_VOID *					/* pBuffer */,		\
	__TMS_BYTES					/* sizeBuffer */,	\
	__TMS_BYTES					/* nBytesRead */	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_GET_CONFIG_DESCRIPTOR_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_CONFIG_DESCRIPTOR_CB_FN * 		\
							/* pCallBack */, 	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_VOID *					/* pBuffer */,		\
	__TMS_BYTES					/* sizeBuffer */,	\
	__TMS_UINT					/* iConfig */		\
	));

/*

Type:  USBPUMP_USBDI_GENERIC_GET_CONFIG_TREE_FN

Function:
	Retrieve the configuration tree
	-- the pointer of USBPUMP_USBDI_CFG_NODE structure

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_GET_CONFIG_TREE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_GET_CONFIG_TREE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		VOID *					pBuffer,
		BYTES					sizeBuffer
		);

Description:
	This is generic driver function in-call API. It will be called by
	client to copy the configuration tree created by DEFINE_CONFIG URB
	to the user buffer. Assumes that the generic driver has already done
	a define-config (which it should have done at enumeration time).

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None.

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_GET_CONFIG_TREE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_VOID *					/* pBuffer */,		\
	__TMS_BYTES					/* sizeBuffer */,	\
	__TMS_BYTES					/* nBytesRead */	\
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_GET_CONFIG_TREE_FN,
__TMS_VOID *,						/* pRequestHandle */
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_GET_CONFIG_TREE_CB_FN * /* pCallBack */,
	__TMS_VOID *					/* pCallBackContext */,
	__TMS_VOID *					/* pBuffer */,
	__TMS_BYTES					/* sizeBuffer */
	));

/*

Type:  USBPUMP_USBDI_GENERIC_READ_CONTROL_PIPE_FN

Function:
	Send an arbitrary command to the device and optionally receive data

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_READ_CONTROL_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_READ_CONTROL_PIPE_CB_FN *	pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		UINT8 					bmRequestType,
		UINT8					bRequest,
		UINT16					wValue,
		UINT16					wIndex,
		VOID *					pBuffer,
		UINT16					sizeBuffer,
		USBPUMP_USBDI_GENERIC_TIMEOUT		milliseconds
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to send an arbitrary command to the device and optionally
	receive data returned from the device.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_READ_CONTROL_PIPE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_VOID *					/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_BYTES					/* nBytesRead */ 	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_READ_CONTROL_PIPE_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_READ_CONTROL_PIPE_CB_FN * /* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */,		\
	__TMS_UINT8 					/* bmRequestType */,	\
	__TMS_UINT8					/* bRequest */,		\
	__TMS_UINT16					/* wValue */, 		\
	__TMS_UINT16					/* wIndex */, 		\
	__TMS_VOID *					/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */,	\
	__TMS_USBPUMP_USBDI_GENERIC_TIMEOUT		/* milliseconds */	\
	));

/*

Type:  USBPUMP_USBDI_GENERIC_WRITE_CONTROL_PIPE_FN

Function:
	Send an arbitrary command with optional additional data

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_WRITE_CONTROL_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_WRITE_CONTROL_PIPE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		UINT8 					bmRequestType,
		UINT8					bRequest,
		UINT16					wValue,
		UINT16					wIndex,
		CONST VOID				pBuffer,
		UINT16					sizeBuffer,
		USBPUMP_USBDI_GENERIC_TIMEOUT		milliseconds
		);

Description:
	This is generic driver function in-call API. It will be called by
	client to send an arbitrary command with optional additional data to
	the device.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None.

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_WRITE_CONTROL_PIPE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_CONST __TMS_VOID *			/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_BYTES					/* nBytesWritten */ 	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_WRITE_CONTROL_PIPE_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_WRITE_CONTROL_PIPE_CB_FN * /* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */, \
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */,		\
	__TMS_UINT8 					/* bmRequestType */,	\
	__TMS_UINT8					/* bRequest */,		\
	__TMS_UINT16					/* wValue */, 		\
	__TMS_UINT16					/* wIndex */, 		\
	__TMS_CONST __TMS_VOID *			/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */,	\
	__TMS_USBPUMP_USBDI_GENERIC_TIMEOUT		/* milliseconds */	\
	));

/*

Type:  USBPUMP_USBDI_GENERIC_READ_BULKINT_PIPE_FN

Function:
	Receive data from a bulk or interrupt pipe

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_READ_BULKINT_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_READ_BULKINT_PIPE_CB_FN *	pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		VOID *					pBuffer,
		BYTES					sizeBuffer,
		USBPUMP_USBDI_GENERIC_TIMEOUT		milliseconds
		);

Description:
	This is generic driver function in-call API. It will be called by
	client to receive data from a bulk or interrupt pipe.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	sizeBuffer can be bigger than 64K because our USBD is happy with very big
	transfers. Always set short packet OK in the URB

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_READ_BULKINT_PIPE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_VOID *					/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_BYTES					/* nBytesRead */ 	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_READ_BULKINT_PIPE_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_READ_BULKINT_PIPE_CB_FN * /* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */,		\
	__TMS_VOID *					/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_USBPUMP_USBDI_GENERIC_TIMEOUT		/* milliseconds */	\
	));

/*

Type:  USBPUMP_USBDI_GENERIC_WRITE_BULKINT_PIPE_FN

Function:
	Send data to a bulk or interrupt pipe

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_WRITE_BULKINT_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_WRITE_BULKINT_PIPE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		CONST VOID *				pBuffer,
		BYTES					sizeBuffer,
		USBPUMP_USBDI_GENERIC_TIMEOUT		milliseconds,
		BOOL					fFullTransfer
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to send data to a bulk or interrupt pipe.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	The sizeBuffer can be bigger than 64K because our USBD is happy with
	very big transfers.
	Use the fFullTransfer flag to set/clear the TRANSFER_FLAG_POST_BREAK
	in the URB.

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_WRITE_BULKINT_PIPE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_CONST __TMS_VOID *			/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_BYTES					/* nBytesWritten */ 	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_WRITE_BULKINT_PIPE_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_WRITE_BULKINT_PIPE_CB_FN * /* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */,		\
	__TMS_CONST __TMS_VOID *			/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_USBPUMP_USBDI_GENERIC_TIMEOUT		/* milliseconds */,	\
	__TMS_BOOL					/* fFullTransfer */	\
	));
	
/*

Type:  USBPUMP_USBDI_GENERIC_READ_STREAM_PIPE_FN

Function:
	Issue an stream read to a pipe

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_READ_STREAM_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_READ_STREAM_PIPE_CB_FN *	pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		VOID *					pBuffer,
		BYTES					sizeBuffer,
		USBPUMP_USBDI_GENERIC_TIMEOUT		milliseconds,
		UINT16					Stream ID
		);

Description:
	This is generic driver function in-call API. It will be called by
	client to issues an stream read to a pipe.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_READ_STREAM_PIPE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_VOID *					/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_BYTES					/* nBytesRead */ 	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_READ_STREAM_PIPE_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_READ_STREAM_PIPE_CB_FN * /* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */,		\
	__TMS_VOID *					/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_USBPUMP_USBDI_GENERIC_TIMEOUT		/* milliseconds */,	\
	__TMS_UINT16					/* Stream ID */		\
	));
	
/*

Type:  USBPUMP_USBDI_GENERIC_WRITE_STREAM_PIPE_FN

Function:
	Issue an stream write to a pipe

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_WRITE_STREAM_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_WRITE_STREAM_PIPE_CB_FN *	pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		VOID *					pBuffer,
		BYTES					sizeBuffer,
		USBPUMP_USBDI_GENERIC_TIMEOUT		milliseconds,
		BOOL					fFullTransfer ,
		UINT16					Stream ID
		);

Description:
	This is generic driver function in-call API. It will be called by
	client to issues an stream write to a pipe.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_WRITE_STREAM_PIPE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_CONST __TMS_VOID *			/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_BYTES					/* nBytesRead */ 	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_WRITE_STREAM_PIPE_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_WRITE_STREAM_PIPE_CB_FN * /* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */,		\
	__TMS_CONST __TMS_VOID *			/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_USBPUMP_USBDI_GENERIC_TIMEOUT		/* milliseconds */,	\
	__TMS_BOOL					/* fFullTransfer */,	\
	__TMS_UINT16					/* Stream ID */		\
	));	

/*

Type:  USBPUMP_USBDI_GENERIC_READ_ISOCH_PIPE_FN

Function:
	Issue an isochronous read to a pipe

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_READ_ISOCH_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_READ_ISOCH_PIPE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		VOID *					pBuffer,
		BYTES					sizeBuffer,
		USBPUMP_USBDI_GENERIC_TIMEOUT		milliseconds,
		USBPUMP_ISOCH_PACKET_DESCR *		pIsochDescr,
		BYTES					IsochDescrSize,
		UINT32					IsochStartFrame,
		BOOL					fAsap
		);

Description:
	This is generic driver function in-call API. It will be called by
	client to issues an isochronous read to a pipe.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	sizeBuffer can be bigger than 64K because our USBD is happy with very
	big transfers.
	Always set short packet OK in the URB

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_READ_ISOCH_PIPE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_VOID *					/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_BYTES					/* nBytesRead */, 	\
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */, 	\
	__TMS_BYTES					/* IsochDescrSize */,	\
	__TMS_UINT32					/* IsochStartFrame */,	\
	__TMS_BYTES					/* nIsochErrs */	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_READ_ISOCH_PIPE_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_READ_ISOCH_PIPE_CB_FN * /* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */,		\
	__TMS_VOID *					/* pBuffer */,		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_USBPUMP_USBDI_GENERIC_TIMEOUT		/* milliseconds */, 	\
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */, 	\
	__TMS_BYTES					/* IsochDescrSize */,	\
	__TMS_UINT32					/* IsochStartFrame */,	\
	__TMS_BOOL					/* fAsap */		\
	));

/*

Type:  USBPUMP_USBDI_GENERIC_WRITE_ISOCH_PIPE_FN

Function:
	Issue an Isochronous write to a pipe

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_WRITE_ISOCH_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_WRITE_ISOCH_PIPE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		CONST VOID *				pBuffer,
		BYTES					sizeBuffer,
		USBPUMP_USBDI_GENERIC_TIMEOUT		milliseconds,
		USBPUMP_ISOCH_PACKET_DESCR *		pIsochDescr,
		BYTES					IsochDescrSize,
		UINT32					IsochStartFrame,
		BOOL					fAsap
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to issues an Isochronous write to a pipe.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	sizeBuffer can be bigger than 64K because our USBD is happy with very
	big transfers.
	Always set short packet OK in the URB

*/

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_WRITE_ISOCH_PIPE_CB_FN,					\
__TMS_VOID,									\
	(									\
	__TMS_VOID *					/* pClientContext */,	\
	__TMS_VOID *					/* pRequestHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,	\
	__TMS_CONST __TMS_VOID *			/* pBuffer */, 		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_BYTES					/* nBytesWritten */, 	\
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */, 	\
	__TMS_BYTES					/* IsochDescrSize */,	\
	__TMS_UINT32					/* IsochStartFrame */,	\
	__TMS_BYTES					/* nIsochErrs */	\
	));

__TMS_FNTYPE_DEF(								\
USBPUMP_USBDI_GENERIC_WRITE_ISOCH_PIPE_FN,					\
__TMS_VOID *,						/* pRequestHandle */	\
	(									\
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,	\
	__TMS_USBPUMP_USBDI_GENERIC_WRITE_ISOCH_PIPE_CB_FN * /* pCallBack */,	\
	__TMS_VOID *					/* pCallBackContext */,	\
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */,		\
	__TMS_CONST __TMS_VOID *			/* pBuffer */,		\
	__TMS_BYTES					/* sizeBuffer */, 	\
	__TMS_USBPUMP_USBDI_GENERIC_TIMEOUT		/* milliseconds */, 	\
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *		/* pIsochDescr */, 	\
	__TMS_BYTES					/* IsochDescrSize */,	\
	__TMS_UINT32					/* IsochStartFrame */,	\
	__TMS_BOOL					/* fAsap */		\
	));

/*

Type:  USBPUMP_USBDI_GENERIC_ABORT_PIPE_FN

Function:
	Abort all pending I/O for a given pipe

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_ABORT_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_ABORT_PIPE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to abort all pending I/O for a given pipe.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	Note that ABORT_PIPE completion does not mean that all I/Os for that
	pipe have already complete.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_ABORT_PIPE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pClientContext */,
	__TMS_VOID *					/* pRequestHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_ABORT_PIPE_FN,
__TMS_VOID *,						/* pRequestHandle */
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_ABORT_PIPE_CB_FN *	/* pCallBack */,
	__TMS_VOID *					/* pCallBackContext */,
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */
	));

/*

Type:  USBPUMP_USBDI_GENERIC_RESET_PIPE_FN

Function:
	Issue a reset-pipe operation to USBD

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_RESET_PIPE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_RESET_PIPE_CB_FN * pCallBack,
		VOID *					pCallBackContext,
		USBPUMP_USBDI_PIPE_HANDLE		hPipe,
		UINT32					ResetPipeFlags
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to issue a reset-pipe operation to USBD, which clears out stall
	conditions and so forth.  If issued on the device-level handle,
	the control pipe gets reset.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_RESET_PIPE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pClientContext */,
	__TMS_VOID *					/* pRequestHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */,
	__TMS_USBPUMP_USBDI_PIPE_HANDLE			/* hPipe */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_RESET_PIPE_FN,
__TMS_VOID *,						/* pRequestHandle */
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_RESET_PIPE_CB_FN *	/* pCallBack */,
	__TMS_VOID *					/* pCallBackContext */,
	__TMS_USBPUMP_USBDI_PIPE_HANDLE,		/* hPipe */
	__TMS_UINT32					/* ResetPipeFlags */
	));

/*

Type:  USBPUMP_USBDI_GENERIC_CYCLE_PORT_FN

Function:
	Simulate a device removal and re-insertion at the root hub

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_CYCLE_PORT_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_CYCLE_PORT_FN *	pCallBack,
		VOID *					pCallBackContext
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to simulate a device removal and re-insertion at the root hub.
	Because the device is "removed" all open handles to the device should
	be closed immediately after making this call.  The handles can be
	reopened after closing them, but they must be closed as the driver
	cannot delete its device object in response to the removal as long as
	open handles exist.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	Not implemented in USBD

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_CYCLE_PORT_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pClientContext */,
	__TMS_VOID *					/* pRequestHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_CYCLE_PORT_FN,
__TMS_VOID *,						/* pRequestHandle */
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_CYCLE_PORT_CB_FN *	/* pCallBack */,
	__TMS_VOID *					/* pCallBackContext */
	));

/*

Type:  USBPUMP_USBDI_GENERIC_SUSPEND_DEVICE_FN

Function:
	Suspend a specific device

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_SUSPEND_DEVICE_FN)(
		VOID *					pFunctionHandle,
		USBPUMP_USBDI_GENERIC_SUSPEND_DEVICE_FN * pCallBack,
		VOID *					pCallBackContext
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to suspend specific device.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_SUSPEND_DEVICE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pClientContext */,
	__TMS_VOID *					/* pRequestHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_SUSPEND_DEVICE_FN,
__TMS_VOID *,						/* pRequestHandle */
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_SUSPEND_DEVICE_CB_FN * /* pCallBack */,
	__TMS_VOID *					/* pCallBackContext */
	));

/*

Type:  USBPUMP_USBDI_GENERIC_RESUME_DEVICE_FN

Function:
	Resume a specific device

Definition:
	typedef VOID * 					// pRequestHandle
	(*USBPUMP_USBDI_GENERIC_RESUME_DEVICE_FN)(
		USBPUMP_SESSION_HANDLE			FunctionHandle,
		USBPUMP_USBDI_GENERIC_INSTANCE_HANDLE	hGenericInstance,
		USBPUMP_USBDI_GENERIC_RESUME_DEVICE_FN * pCallBack,
		VOID *					pCallBackContext
		);

Description:
	This is generic driver function in-call API.  It will be called by
	client to resume a specific device.

Returns:
	USBPUMP_USBDI_GENERIC_REQUEST_HANDLE

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_RESUME_DEVICE_CB_FN,
__TMS_VOID,
	(
	__TMS_VOID *					/* pClientContext */,
	__TMS_VOID *					/* pRequestHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_STATUS		/* ErrorCode */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_GENERIC_RESUME_DEVICE_FN,
__TMS_VOID *,						/* pRequestHandle */
	(
	__TMS_USBPUMP_SESSION_HANDLE			/* FunctionHandle */,
	__TMS_USBPUMP_USBDI_GENERIC_RESUME_DEVICE_CB_FN * /* pCallBack */,
	__TMS_VOID *					/* pCallBackContext */
	));


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_CONTENTS)
	{
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER;

	__TMS_USBPUMP_USBDI_GENERIC_CANCEL_REQUEST_FN *
						pCancelRequestFn;
	__TMS_USBPUMP_USBDI_GENERIC_GET_DEVICE_STATE_FN *
						pGetDeviceStateFn;
	__TMS_USBPUMP_USBDI_GENERIC_GET_DEVICE_DESCRIPTOR_FN *
						pGetDeviceDescriptorFn;
	__TMS_USBPUMP_USBDI_GENERIC_GET_CONFIG_DESCRIPTOR_FN *
						pGetConfigDescriptorFn;
	__TMS_USBPUMP_USBDI_GENERIC_GET_CONFIG_TREE_FN *
						pGetConfigTreeFn;
	__TMS_USBPUMP_USBDI_GENERIC_READ_CONTROL_PIPE_FN *
						pReadControlPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_WRITE_CONTROL_PIPE_FN *
						pWriteControlPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_READ_BULKINT_PIPE_FN *
						pReadBulkIntPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_WRITE_BULKINT_PIPE_FN *
						pWriteBulkIntPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_READ_STREAM_PIPE_FN *
						pReadStreamPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_WRITE_STREAM_PIPE_FN *
						pWriteStreamPipeFn;										
	__TMS_USBPUMP_USBDI_GENERIC_READ_ISOCH_PIPE_FN *
						pReadIsochPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_WRITE_ISOCH_PIPE_FN *
						pWriteIsochPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_ABORT_PIPE_FN *
						pAbortPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_RESET_PIPE_FN *
						pResetPipeFn;
	__TMS_USBPUMP_USBDI_GENERIC_CYCLE_PORT_FN *
						pCyclePortFn;
	__TMS_USBPUMP_USBDI_GENERIC_SUSPEND_DEVICE_FN *
						pSuspendDeviceFn;
	__TMS_USBPUMP_USBDI_GENERIC_RESUME_DEVICE_FN *
						pResumeDeviceFn;

	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER;
	};

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_FUNCTION_GENERIC_INCALL);
union __TMS_UNIONNAME	(USBPUMP_USBDI_FUNCTION_GENERIC_INCALL)
	{
	__TMS_USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_CONTENTS GenDrv;
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_CONTENTS__UNION;
	};

#define	__TMS_USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pCancelRequestFn,						\
	a_pGetDeviceStateFn,						\
	a_pGetDeviceDescriptorFn,					\
	a_pGetConfigDescriptorFn,					\
	a_pGetConfigTreeFn,						\
	a_pReadControlPipeFn,						\
	a_pWriteControlPipeFn,						\
	a_pReadBulkIntPipeFn,						\
	a_pWriteBulkIntPipeFn,						\
	a_pReadStreamPipeFn,						\
	a_pWriteStreamPipeFn,						\
	a_pReadIsochPipeFn,						\
	a_pWriteIsochPipeFn,						\
	a_pAbortPipeFn,							\
	a_pResetPipeFn,							\
	a_pCyclePortFn,							\
	a_pSuspendDeviceFn,						\
	a_pResumeDeviceFn						\
	)								\
	{{								\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_INIT_V1(		\
		sizeof(__TMS_USBPUMP_USBDI_FUNCTION_GENERIC_INCALL),	\
		a_pCloseFn						\
		),							\
	a_pCancelRequestFn,						\
	a_pGetDeviceStateFn,						\
	a_pGetDeviceDescriptorFn,					\
	a_pGetConfigDescriptorFn,					\
	a_pGetConfigTreeFn,						\
	a_pReadControlPipeFn,						\
	a_pWriteControlPipeFn,						\
	a_pReadBulkIntPipeFn,						\
	a_pWriteBulkIntPipeFn,						\
	a_pReadStreamPipeFn,						\
	a_pWriteStreamPipeFn,						\
	a_pReadIsochPipeFn,						\
	a_pWriteIsochPipeFn,						\
	a_pAbortPipeFn,							\
	a_pResetPipeFn,							\
	a_pCyclePortFn,							\
	a_pSuspendDeviceFn,						\
	a_pResumeDeviceFn,						\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_INIT_V1(		\
		__TMS_USBPUMP_API_INCALL_MAGIC_END			\
		)							\
	}}
	
#define	__TMS_USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pCancelRequestFn,							\
	a_pGetDeviceStateFn,							\
	a_pGetDeviceDescriptorFn,						\
	a_pGetConfigDescriptorFn,						\
	a_pGetConfigTreeFn,							\
	a_pReadControlPipeFn,							\
	a_pWriteControlPipeFn,							\
	a_pReadBulkIntPipeFn,							\
	a_pWriteBulkIntPipeFn,							\
	a_pReadStreamPipeFn,							\
	a_pWriteStreamPipeFn,							\
	a_pReadIsochPipeFn,							\
	a_pWriteIsochPipeFn,							\
	a_pAbortPipeFn,								\
	a_pResetPipeFn,								\
	a_pCyclePortFn,								\
	a_pSuspendDeviceFn,							\
	a_pResumeDeviceFn							\
	)									\
    do	{									\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_HEADER_SETUP_V1(			\
		a_pInCall,							\
		sizeof(*(a_pInCall)),						\
		a_pCloseFn							\
		);								\
	(a_pInCall)->GenDrv.pCancelRequestFn = a_pCancelRequestFn;		\
	(a_pInCall)->GenDrv.pGetDeviceStateFn = a_pGetDeviceStateFn;		\
	(a_pInCall)->GenDrv.pGetDeviceDescriptorFn = a_pGetDeviceDescriptorFn;	\
	(a_pInCall)->GenDrv.pGetConfigDescriptorFn = a_pGetConfigDescriptorFn;	\
	(a_pInCall)->GenDrv.pGetConfigTreeFn = a_pGetConfigTreeFn;		\
	(a_pInCall)->GenDrv.pReadControlPipeFn = a_pReadControlPipeFn;		\
	(a_pInCall)->GenDrv.pWriteControlPipeFn = a_pWriteControlPipeFn;	\
	(a_pInCall)->GenDrv.pReadBulkIntPipeFn = a_pReadBulkIntPipeFn;		\
	(a_pInCall)->GenDrv.pWriteBulkIntPipeFn = a_pWriteBulkIntPipeFn;	\
	(a_pInCall)->GenDrv.pReadStreamPipeFn = a_pReadStreamPipeFn;		\
	(a_pInCall)->GenDrv.pWriteStreamPipeFn = a_pWriteStreamPipeFn;		\
	(a_pInCall)->GenDrv.pReadIsochPipeFn = a_pReadIsochPipeFn;		\
	(a_pInCall)->GenDrv.pWriteIsochPipeFn = a_pWriteIsochPipeFn;		\
	(a_pInCall)->GenDrv.pAbortPipeFn = a_pAbortPipeFn;			\
	(a_pInCall)->GenDrv.pResetPipeFn = a_pResetPipeFn;			\
	(a_pInCall)->GenDrv.pCyclePortFn = a_pCyclePortFn;			\
	(a_pInCall)->GenDrv.pSuspendDeviceFn = a_pSuspendDeviceFn;		\
	(a_pInCall)->GenDrv.pResumeDeviceFn = a_pResumeDeviceFn;		\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_INCALL_TRAILER_SETUP_V1(		\
		a_pInCall,							\
		GenDrv,								\
		__TMS_USBPUMP_API_INCALL_MAGIC_END				\
		);								\
	} while (0)

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_USBGUID	gk_UsbPumpUsbdiClassGeneric_Guid;

extern __TMS_CONST __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG
					gk_UsbPumpUsbdiGeneric_ClassConfig;

__TMS_USBPUMP_USBDI_DRIVER_CLASS_INIT_FN
UsbPumpUsbdiClassGeneric_Initialize;

__TMS_CONST __TMS_TEXT *
UsbPumpUsbdiClassGeneric_StatusName(
	__TMS_USBPUMP_USBDI_GENERIC_STATUS
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	USBPUMP_USBDI_GENERIC_STATUS Codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_OK				\
	__TMS_USBPUMP_CLASSKIT_STATUS_OK
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_INVALID_PARAMETER		\
	__TMS_USBPUMP_CLASSKIT_STATUS_INVALID_PARAMETER
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_ARG_AREA_TOO_SMALL		\
	__TMS_USBPUMP_CLASSKIT_STATUS_ARG_AREA_TOO_SMALL
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_BUFFER_TOO_SMALL		\
	__TMS_USBPUMP_CLASSKIT_STATUS_BUFFER_TOO_SMALL
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_NOT_SUPPORTED		\
	__TMS_USBPUMP_CLASSKIT_STATUS_NOT_SUPPORTED
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_NO_MORE_SESSIONS		\
	__TMS_USBPUMP_CLASSKIT_STATUS_NO_MORE_SESSIONS
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_INVALID_SESSION_HANDLE	\
	__TMS_USBPUMP_CLASSKIT_STATUS_INVALID_SESSION_HANDLE
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_INVALID_FUNCTION_HANDLE	\
	__TMS_USBPUMP_CLASSKIT_STATUS_INVALID_FUNCTION_HANDLE
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_FUNCTION_ALREADY_OPENED	\
	__TMS_USBPUMP_CLASSKIT_STATUS_FUNCTION_ALREADY_OPENED
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_FUNCTION_NOT_OPENED		\
	__TMS_USBPUMP_CLASSKIT_STATUS_FUNCTION_NOT_OPENED

#define __TMS_USBPUMP_USBDI_GENERIC_STATUS_INTERNAL_ERROR		\
	(__TMS_USBPUMP_CLASSKIT_STATUS__MAX + 0)
#define __TMS_USBPUMP_USBDI_GENERIC_STATUS_NO_MEMORY			\
	(__TMS_USBPUMP_CLASSKIT_STATUS__MAX + 1)
#define __TMS_USBPUMP_USBDI_GENERIC_STATUS_ALREADY_COMPLETED		\
	(__TMS_USBPUMP_CLASSKIT_STATUS__MAX + 2)

#define __TMS_USBPUMP_USBDI_GENERIC_STATUS__INIT	\
	"GENERIC_STATUS_OK",				\
	"GENERIC_STATUS_INVALID_PARAMETER",		\
	"GENERIC_STATUS_ARG_AREA_TOO_SMALL",		\
	"GENERIC_STATUS_BUFFER_TOO_SMALL",		\
	"GENERIC_STATUS_NOT_SUPPORTED",			\
	"GENERIC_STATUS_NO_MORE_SESSIONS",		\
	"GENERIC_STATUS_INVALID_SESSION_HANDLE",	\
	"GENERIC_STATUS_INVALID_FUNCTION_HANDLE",	\
	"GENERIC_STATUS_FUNCTION_ALREADY_OPENED",	\
	"GENERIC_STATUS_FUNCTION_NOT_OPENED",		\
	"GENERIC_STATUS_INTERNAL_ERROR",		\
	"GENERIC_STATUS_NO_MEMORY",			\
	"GENERIC_STATUS_ALREADY_COMPLETED",		\
	"status__MAX"

/*
|| Macro related to converting USB status and Generic Driver status
*/
#define __TMS_USBPUMP_USBDI_GENERIC_USBD_ERROR_MASK	(1 << 8)
#define __TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(UrbStatus)	\
	((UrbStatus) | __TMS_USBPUMP_USBDI_GENERIC_USBD_ERROR_MASK)

#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_BUSY		\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_BUSY)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_KILL		\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_KILL)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_IOERR		\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_IOERR)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_STALL		\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_STALL)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_LENGTH_OVERRUN	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_LENGTH_OVERRUN)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_LENGTH_UNDERRUN	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_LENGTH_UNDERRUN)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_INVALID_PARAM	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_INVALID_PARAM)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NOHW		\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_NOHW)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_IN_USE		\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_IN_USE)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_MEMORY		\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_NO_MEMORY)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_BANDWIDTH	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_NO_BANDWIDTH)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_BUS_POWER	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_NO_BUS_POWER)
#define	__TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_INTERNAL_ERROR	\
	__TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(__TMS_USTAT_INTERNAL_ERROR)


/****************************************************************************\
|
|	Definition of USBPUMP_USBDI_GENERIC_DEVICE_STATE
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_USBDI_GENERIC_DEVICE_STATE)
	{
	__TMS_BOOL	fRemoved;
	__TMS_BOOL	fStopped;
	__TMS_BOOL	fFunctionOpened;
	};

/****************************************************************************\
|
|	Generic Class Driver Notification Definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_GENERIC_EVENT_DEVICE_SUSPENDED	\
	(__TMS_USBPUMP_CLASSKIT_EVENT__MAX + 0)
#define	__TMS_USBPUMP_USBDI_GENERIC_EVENT_DEVICE_RESUMED	\
	(__TMS_USBPUMP_CLASSKIT_EVENT__MAX + 1)

#define	__TMS_USBPUMP_USBDI_GENERIC_EVENT__MAX			\
	(__TMS_USBPUMP_CLASSKIT_EVENT__MAX + 2)

/*
|| When you add names to the above list, please also update the following
|| debugging table.
*/
#define	__TMS_USBPUMP_USBDI_GENERIC_EVENT_NAMES__INIT		\
	"USBPUMP_USBDI_GENERIC_EVENT_DEVICE_SUSPENDED"		\
	"USBPUMP_USBDI_GENERIC_EVENT_DEVICE_RESUMED"

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_GENERIC_EVENT_DEVICE_SUSPENDED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_GENERIC_EVENT_DEVICE_SUSPENDED_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_GENERIC_EVENT_DEVICE_RESUMED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_GENERIC_EVENT_DEVICE_RESUMED_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	};

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_CLASS_GENERIC_NAME	\
   __TMS_USBPUMP_USBDI_CLASS_GENERIC_NAME
# define USBPUMP_USBDI_FUNCTION_GENERIC_NAME	\
   __TMS_USBPUMP_USBDI_FUNCTION_GENERIC_NAME
# define USBPUMP_USBDI_CLASS_GENERIC_GUID_INIT()	\
   __TMS_USBPUMP_USBDI_CLASS_GENERIC_GUID_INIT()
# define USBPUMP_OBJECT_NAME_ENUM_HOST_GCD	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_HOST_GCD
# define USBPUMP_USBDI_CLASS_GENERIC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetUsbdFeatureFn,						\
	a_pGetGenDrvFeatureFn						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_GENERIC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetUsbdFeatureFn,						\
	a_pGetGenDrvFeatureFn						\
	)
# define USBPUMP_USBDI_CLASS_GENERIC_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetUsbdFeatureFn,						\
	a_pGetGenDrvFeatureFn						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_GENERIC_INCALL_SETUP_V1(		\
	a_pInCall,							\
	a_pCloseFn,							\
	a_pOpenFunctionFn,						\
	a_pGetNumDevicesFn,						\
	a_pGetBoundDevicesFn,						\
	a_pGetUsbdFeatureFn,						\
	a_pGetGenDrvFeatureFn						\
	)
# define USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pCancelRequestFn,						\
	a_pGetDeviceStateFn,						\
	a_pGetDeviceDescriptorFn,					\
	a_pGetConfigDescriptorFn,					\
	a_pGetConfigTreeFn,						\
	a_pReadControlPipeFn,						\
	a_pWriteControlPipeFn,						\
	a_pReadBulkIntPipeFn,						\
	a_pWriteBulkIntPipeFn,						\
	a_pReadStreamPipeFn,						\
	a_pWriteStreamPipeFn,						\
	a_pReadIsochPipeFn,						\
	a_pWriteIsochPipeFn,						\
	a_pAbortPipeFn,							\
	a_pResetPipeFn,							\
	a_pCyclePortFn,							\
	a_pSuspendDeviceFn,						\
	a_pResumeDeviceFn						\
	)	\
	__TMS_USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_INIT_V1(		\
	a_pCloseFn,							\
	a_pCancelRequestFn,						\
	a_pGetDeviceStateFn,						\
	a_pGetDeviceDescriptorFn,					\
	a_pGetConfigDescriptorFn,					\
	a_pGetConfigTreeFn,						\
	a_pReadControlPipeFn,						\
	a_pWriteControlPipeFn,						\
	a_pReadBulkIntPipeFn,						\
	a_pWriteBulkIntPipeFn,						\
	a_pReadStreamPipeFn,						\
	a_pWriteStreamPipeFn,						\
	a_pReadIsochPipeFn,						\
	a_pWriteIsochPipeFn,						\
	a_pAbortPipeFn,							\
	a_pResetPipeFn,							\
	a_pCyclePortFn,							\
	a_pSuspendDeviceFn,						\
	a_pResumeDeviceFn						\
	)
# define USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pCancelRequestFn,							\
	a_pGetDeviceStateFn,							\
	a_pGetDeviceDescriptorFn,						\
	a_pGetConfigDescriptorFn,						\
	a_pGetConfigTreeFn,							\
	a_pReadControlPipeFn,							\
	a_pWriteControlPipeFn,							\
	a_pReadBulkIntPipeFn,							\
	a_pWriteBulkIntPipeFn,							\
	a_pReadStreamPipeFn,							\
	a_pWriteStreamPipeFn,							\
	a_pReadIsochPipeFn,							\
	a_pWriteIsochPipeFn,							\
	a_pAbortPipeFn,								\
	a_pResetPipeFn,								\
	a_pCyclePortFn,								\
	a_pSuspendDeviceFn,							\
	a_pResumeDeviceFn							\
	)	\
	__TMS_USBPUMP_USBDI_FUNCTION_GENERIC_INCALL_SETUP_V1(			\
	a_pInCall,								\
	a_pCloseFn,								\
	a_pCancelRequestFn,							\
	a_pGetDeviceStateFn,							\
	a_pGetDeviceDescriptorFn,						\
	a_pGetConfigDescriptorFn,						\
	a_pGetConfigTreeFn,							\
	a_pReadControlPipeFn,							\
	a_pWriteControlPipeFn,							\
	a_pReadBulkIntPipeFn,							\
	a_pWriteBulkIntPipeFn,							\
	a_pReadStreamPipeFn,							\
	a_pWriteStreamPipeFn,							\
	a_pReadIsochPipeFn,							\
	a_pWriteIsochPipeFn,							\
	a_pAbortPipeFn,								\
	a_pResetPipeFn,								\
	a_pCyclePortFn,								\
	a_pSuspendDeviceFn,							\
	a_pResumeDeviceFn							\
	)
# define USBPUMP_USBDI_GENERIC_STATUS_OK	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_OK
# define USBPUMP_USBDI_GENERIC_STATUS_INVALID_PARAMETER	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_INVALID_PARAMETER
# define USBPUMP_USBDI_GENERIC_STATUS_ARG_AREA_TOO_SMALL	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_ARG_AREA_TOO_SMALL
# define USBPUMP_USBDI_GENERIC_STATUS_BUFFER_TOO_SMALL	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_BUFFER_TOO_SMALL
# define USBPUMP_USBDI_GENERIC_STATUS_NOT_SUPPORTED	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_NOT_SUPPORTED
# define USBPUMP_USBDI_GENERIC_STATUS_NO_MORE_SESSIONS	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_NO_MORE_SESSIONS
# define USBPUMP_USBDI_GENERIC_STATUS_INVALID_SESSION_HANDLE	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_INVALID_SESSION_HANDLE
# define USBPUMP_USBDI_GENERIC_STATUS_INVALID_FUNCTION_HANDLE	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_INVALID_FUNCTION_HANDLE
# define USBPUMP_USBDI_GENERIC_STATUS_FUNCTION_ALREADY_OPENED	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_FUNCTION_ALREADY_OPENED
# define USBPUMP_USBDI_GENERIC_STATUS_FUNCTION_NOT_OPENED	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_FUNCTION_NOT_OPENED
# define USBPUMP_USBDI_GENERIC_STATUS_INTERNAL_ERROR	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_INTERNAL_ERROR
# define USBPUMP_USBDI_GENERIC_STATUS_NO_MEMORY	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_NO_MEMORY
# define USBPUMP_USBDI_GENERIC_STATUS_ALREADY_COMPLETED	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_ALREADY_COMPLETED
# define USBPUMP_USBDI_GENERIC_STATUS__INIT	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS__INIT
# define USBPUMP_USBDI_GENERIC_USBD_ERROR_MASK	\
   __TMS_USBPUMP_USBDI_GENERIC_USBD_ERROR_MASK
# define USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(UrbStatus)	\
   __TMS_USBPUMP_USBDI_GENERIC_GET_GENERIC_STATUS(UrbStatus)
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_BUSY	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_BUSY
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_KILL	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_KILL
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_IOERR	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_IOERR
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_STALL	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_STALL
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_LENGTH_OVERRUN	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_LENGTH_OVERRUN
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_LENGTH_UNDERRUN	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_LENGTH_UNDERRUN
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_INVALID_PARAM	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_INVALID_PARAM
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NOHW	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NOHW
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_IN_USE	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_IN_USE
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_MEMORY	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_MEMORY
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_BANDWIDTH	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_BANDWIDTH
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_BUS_POWER	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_NO_BUS_POWER
# define USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_INTERNAL_ERROR	\
   __TMS_USBPUMP_USBDI_GENERIC_STATUS_USBD_USTAT_INTERNAL_ERROR
# define USBPUMP_USBDI_GENERIC_EVENT_DEVICE_SUSPENDED	\
   __TMS_USBPUMP_USBDI_GENERIC_EVENT_DEVICE_SUSPENDED
# define USBPUMP_USBDI_GENERIC_EVENT_DEVICE_RESUMED	\
   __TMS_USBPUMP_USBDI_GENERIC_EVENT_DEVICE_RESUMED
# define USBPUMP_USBDI_GENERIC_EVENT__MAX	\
   __TMS_USBPUMP_USBDI_GENERIC_EVENT__MAX
# define USBPUMP_USBDI_GENERIC_EVENT_NAMES__INIT	\
   __TMS_USBPUMP_USBDI_GENERIC_EVENT_NAMES__INIT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbd_class_generic_api.h ****/
#endif /* _USBPUMP_USBD_CLASS_GENERIC_API_H_ */
