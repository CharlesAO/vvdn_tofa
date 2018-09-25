/* usbpump_debugio_ioctl.h	Mon May 11 2009 11:07:39 chwon */

/*

Module:  usbpump_debugio_ioctl.h

Function:
	Home for the Debug-IO IOCTL definitions.

Version:
	V2.01a	Mon May 11 2009 11:07:39 chwon	Edit level 2

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
	7563: Module created from usbpump_abstract_sio_ioctl.h EL 3

   2.01a  Mon May 11 2009 11:07:39  chwon
	7563: fixed IOCTL code - not READ. Removed class, function offsets.

*/

#ifndef _USBPUMP_DEBUGIO_IOCTL_H_	/* prevent multiple includes */
#define _USBPUMP_DEBUGIO_IOCTL_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMPIOCTLS_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _USBPUMP_DEBUGIO_TYPES_H_
# include "usbpump_debugio_types.h"
#endif

/****************************************************************************\
|
|	Ioctl class code. To go into i/usbioctl_classes.h
|
|	See PR3524
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_CLASS_DEBUGIO	'g'
#define	__TMS_USBPUMP_IOCTL_CLASS_DEBUGIO_EDGE	'G'


/****************************************************************************\
|
|	The non-EDGE IOCTLs arrive inwards from Debug-IO client drivers 
|	or functions.
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_DEBUGIO(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_DEBUGIO,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_DEBUGIO_, NAME, _ARG)	\
		)


/****************************************************************************\
|
|	Messages to Debug-IO base objects
|
\****************************************************************************/

/* Create a Debug-IO object */
#define	__TMS_USBPUMP_IOCTL_DEBUGIO_CREATE		\
	__TMS_USBPUMP_IOCTL_DEBUGIO(0, W, CREATE)

/* Open a session in a Debug-IO object */
#define	__TMS_USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC	\
	__TMS_USBPUMP_IOCTL_DEBUGIO(1, RW_ASYNC, OPENSESSION_ASYNC)


/****************************************************************************\
|
|	The EDGE IOCTLs are used for notifications from Debug-IO to its
|	managed objects.
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_EDGE_DEBUGIO(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_DEBUGIO_EDGE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_DEBUGIO_, NAME, _ARG) \
		)


/****************************************************************************\
|
|	Upwards notifications from Debug-IO base objects
|
\****************************************************************************/

/* some upwards notification XXXX */


/****************************************************************************\
|
|	The IOCTL details follow
|
\****************************************************************************/

/*

IOCTL:	USBPUMP_IOCTL_DEBUGIO_CREATE

Index:	Type:	USBPUMP_IOCTL_DEBUGIO_CREATE_ARG

Function:
	Create a Debug-IO object

Input:
	UPLATFORM *pPlatform;
		Platform pointer.

Output:
	Nothing

Description:
	This IOCTL urges the IOCTL handler to create a Debug-IO instance

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_FAILED for failure
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if no Debug-IO starter found

Setup Macro:
	USBPUMP_IOCTL_DEBUGIO_CREATE_ARG_SETUP_V1

Notes:

See also:
	UsbPumpDebugIo_Create()

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEBUGIO_CREATE_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEBUGIO_CREATE_ARG)
	{
	__TMS_UPLATFORM *				pPlatform;
	};

#define	__TMS_USBPUMP_IOCTL_DEBUGIO_CREATE_ARG_SETUP_V1( 	\
	a_pArg,							\
	a_pPlatform						\
	)							\
   do	{							\
	(a_pArg)->pPlatform = (a_pPlatform);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC

Index:	Type:	USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC_ARG

Function:
	Open a session in a Debug-IO object  

Input:
	VOID *pClientHandle;
		Pointer passed as a parameter to each of the asynchronous 
		callback functions. It is not interpreted by the Debug-IO 
		in any way

	USBPUMP_OBJECT *pClientObject;
		Optional object pointer that may be used if the client 
		is using the DataPump object system. If used, it must 
		point to the object that represents the client. This 
		parameter is primarily provided for MCCI's future use.

	USBPUMP_DEBUGIO_TRANSPORT_STATUS_CHANGE_CB_FN *pTpStatusChangeCb;
		Function pointer that specifies the function to be called 
		when a transport-status change occurs. Transport status 
		changes may include deactivation of the Debug-IO.
		
	USBPUMP_DEBUGIO_SESSION_APIS *pApiBuffer;
		Pointer to client-supplied memory location that is to be 
		filled with the USBPUMP_DEBUGIO_SESSION_APIS structure.
		This structure contains the function pointers that the client 
		will then use to make API calls.  

Output:
	USBPUMP_DEBUGIO_SESSION *pSession;
		Pointer to session, or NULL if session-open failed

	USBPUMP_DEBUGIO_STATUS Status;
		Status from open-session.

Description:
	This IOCTL establishes a connection between a client and a 
	USBPUMP_DEBUGIO instance. It sets a number of callback functions 
	to be used for processing asynchronous peer operations, and returns a 
	handle that can be used to send additional requests to the Debug-IO.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if Debug-IO couldn't be
		found in the dynamic parent of this object

Setup Macro:
	None.

Notes:

See also:
	UsbPumpDebugIo_OpenSession()

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC_ARG)
	{
	__TMS_VOID *					pClientHandle;
	__TMS_USBPUMP_OBJECT *				pClientObject;
	__TMS_USBPUMP_DEBUGIO_STATUS_CHANGE_CB_FN *	pStatusChangeCb;
	__TMS_USBPUMP_DEBUGIO_SESSION_APIS *		pApiBuffer;
	__TMS_USBPUMP_DEBUGIO_SESSION *			pSession;
	__TMS_USBPUMP_DEBUGIO_STATUS			Status;
	};

#define	__TMS_USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC_ARG_SETUP_V1( \
	a_pArg,							\
	a_pClientHandle,					\
	a_pClientObject,					\
	a_pStatusChangeCb,					\
	a_pApiBuffer						\
	)							\
   do	{							\
	(a_pArg)->pClientHandle    = (a_pClientHandle);		\
	(a_pArg)->pClientObject    = (a_pClientObject);		\
	(a_pArg)->pStatusChangeCb  = (a_pStatusChangeCb);	\
	(a_pArg)->pApiBuffer       = (a_pApiBuffer);		\
	(a_pArg)->pSession         = __TMS_NULL;		\
	(a_pArg)->Status           = 				\
		__TMS_USBPUMP_DEBUGIO_STATUS_FAILED;		\
	} while (0)


/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_CLASS_DEBUGIO	\
   __TMS_USBPUMP_IOCTL_CLASS_DEBUGIO
# define USBPUMP_IOCTL_CLASS_DEBUGIO_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_DEBUGIO_EDGE
# define USBPUMP_IOCTL_DEBUGIO(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_DEBUGIO(i, t, NAME)
# define USBPUMP_IOCTL_DEBUGIO_CREATE	\
   __TMS_USBPUMP_IOCTL_DEBUGIO_CREATE
# define USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC	\
   __TMS_USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC
# define USBPUMP_IOCTL_EDGE_DEBUGIO(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_EDGE_DEBUGIO(i, t, NAME)
# define USBPUMP_IOCTL_DEBUGIO_CREATE_ARG_SETUP_V1( 	\
	a_pArg,							\
	a_pPlatform						\
	)	\
	__TMS_USBPUMP_IOCTL_DEBUGIO_CREATE_ARG_SETUP_V1( 	\
	a_pArg,							\
	a_pPlatform						\
	)
# define USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC_ARG_SETUP_V1( \
	a_pArg,							\
	a_pClientHandle,					\
	a_pClientObject,					\
	a_pStatusChangeCb,					\
	a_pApiBuffer						\
	)	\
	__TMS_USBPUMP_IOCTL_DEBUGIO_OPENSESSION_ASYNC_ARG_SETUP_V1( \
	a_pArg,							\
	a_pClientHandle,					\
	a_pClientObject,					\
	a_pStatusChangeCb,					\
	a_pApiBuffer						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_debugio_ioctl.h ****/
#endif /* _USBPUMP_DEBUGIO_IOCTL_H_ */
