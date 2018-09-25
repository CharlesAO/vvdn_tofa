/* usbioctl_function.h	Tue Apr 22 2008 11:00:30 chwon */

/*

Module:  usbioctl_function.h

Function:
	IOCTL codes for function objects.

Version:
	V1.97k	Tue Apr 22 2008 11:00:30 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2002-2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Thu Jul 25 2002 15:41:55  tmm
	Module created.

   1.79b  9 Aug 2002  tmm
	Centralize class codes.

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

   1.91b  Wed Mar 30 2005 08:15:46 maw
   	Add USBPUMP_IOCTL_FUNCTION_MESSAGE, ..._OPEN_MSGAPI 
   	and ..._CLOSE_MSGAPI, 

   1.91c  Tue Apr 26 2005 10:47:13 maw
	Moved USBPUMP_IOCTL_FUNCTION_STATUS_PHASE from usbioctl_sic.h

   1.91d  Tue May 31 2005 09:15:29 maw
   	Updated _OPEN_MSGAPI and _CLOSE_MSGAPI with iProcess and hMailbox 
	param. hTask removed.

   1.91h  Mon Sep  5 2005 08:23:02  tmm
	Add USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT, to find out whether hardward
	is available.  Collect all uncloaked definitions in one place.

   1.97k  Tue Apr 22 2008 11:00:30  chwon
	4040: use __TMS_NULL instead of NULL

*/

#ifndef _USBIOCTL_FUNCTION_H_		/* prevent multiple includes */
#define _USBIOCTL_FUNCTION_H_

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif

/****************************************************************************\
|
|	Define the IOCTL codes
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_FUNCTION(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_FUNCTION,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_FUNCTION_, NAME, _ARG) \
		)

/* open a function */
#define	__TMS_USBPUMP_IOCTL_FUNCTION_OPEN	\
	__TMS_USBPUMP_IOCTL_FUNCTION(0, RW, OPEN)

/* close a function */
#define	__TMS_USBPUMP_IOCTL_FUNCTION_CLOSE	\
	__TMS_USBPUMP_IOCTL_FUNCTION(1, W, CLOSE)

/* send a message to a function */
#define	__TMS_USBPUMP_IOCTL_FUNCTION_MESSAGE	\
	__TMS_USBPUMP_IOCTL_FUNCTION(2, W, MESSAGE)

/* open a function message API */
#define	__TMS_USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI	\
	__TMS_USBPUMP_IOCTL_FUNCTION(3, W, OPEN_MSGAPI)

/* close a function message API */
#define	__TMS_USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI	\
	__TMS_USBPUMP_IOCTL_FUNCTION(4, W, CLOSE_MSGAPI)

/* send status phase on USB request */
#define	__TMS_USBPUMP_IOCTL_FUNCTION_STATUS_PHASE	\
	__TMS_USBPUMP_IOCTL_FUNCTION(4, W, STATUS_PHASE)

/* check whether underlying hardare is actually present */
#define	__TMS_USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT	\
	__TMS_USBPUMP_IOCTL_FUNCTION(5, RW, QUERY_PRESENT)

/*

IOCTL:	USBPUMP_IOCTL_FUNCTION_OPEN

Index:	Type:	USBPUMP_IOCTL_FUNCTION_OPEN_ARG
	Name:	USBPUMP_IOCTL_FUNCTION_OPEN_ARG_SETUP_V1

Function:
	IOCTL issued by OS driver clients to establish a connection to
	the outermost "openable" edge of a protocol stack.

Input:
	pInParam	points to a USBPUMP_IOCTL_FUNCTION_OPEN_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_FUNCTION_OPEN_ARG structure.

	USBPUMP_IOCTL_FUNCTION_OPEN_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObjectToOpen;
		IN: points to the object that is to be opened.  This is
		normally set to the top object in the protocol stack.

	USBPUMP_OBJECT_HEADER *pClientObject;
		IN: points to the client object header.  This will be used
		for IOCTL out-calls.

	VOID *pClientContext;
		IN: opaque client context handle.

	USBPUMP_OBJECT_HEADER *pIoObject;
		OUT: object handle to be used for actual I/O activity.

Description:	
	This IOCTL is sent from an OS-specific driver to a leaf object,
	to notify the leaf object that a client is about to begin I/O.
	The OS-specific driver must prepare a OS-specific driver
	USBPUMP_OBJECT_HEADER, which is then registered with the leaf
	object.  It returns the actual object handle (normally the same
	object as was opened), which is to be used for I/O.

Setup Macro:
	VOID USBPUMP_IOCTL_FUNCTION_OPEN_ARG_SETUP_V1(
		USBPUMP_IOCTL_FUNCTION_OPEN_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObjectToOpen,
		USBPUMP_OBJECT_HEADER *pClientObject,
		VOID *pClientContext
		);

	This macro correctly initializes a .._FUNCTION_OPEN_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a 
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_FUNCTION_OPEN_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_FUNCTION_OPEN_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pObjectToOpen;
	__TMS_USBPUMP_OBJECT_HEADER	*pClientObject;
	__TMS_VOID			*pClientContext;
	__TMS_USBPUMP_OBJECT_HEADER	*pIoObject;
	};

/* the setup for the .._FUNCTION_OBEN_ARG struct */
#define __TMS_USBPUMP_IOCTL_FUNCTION_OPEN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObjectToOpen,						\
	ARG_pClientObject,						\
	ARG_pCtx							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_FUNCTION_OPEN_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObjectToOpen = (ARG_pObjectToOpen);			\
	__pArg->pClientObject = (ARG_pClientObject);			\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->pIoObject = __TMS_NULL;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_FUNCTION_CLOSE

Index:	Type:	USBPUMP_IOCTL_FUNCTION_CLOSE_ARG
	Name:	USBPUMP_IOCTL_FUNCTION_CLOSE_ARG_SETUP_V1

Function:
	IOCTL issued by OS driver clients to cancel a connection to
	the outermost "openable" leaf of a protocol stack.

Input:
	pInParam	points to a USBPUMP_IOCTL_FUNCTION_CLOSE_ARG
			structure.

	pOutParam	not used, should be NULL.

	USBPUMP_IOCTL_FUNCTION_CLOSE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pIoObject;
		IN: points to the object that is to be closed.  This is
		what was returned by the corresponding open.

	USBPUMP_OBJECT_HEADER *pClientObject;
		IN: points to the client object supplied in the initial
		open.

Description:	
	This IOCTL is sent from an OS-specific driver to a leaf object,
	to cancel the previous OPEN operation.

Setup Macro:
	VOID USBPUMP_IOCTL_FUNCTION_CLOSE_ARG_SETUP_V1(
		USBPUMP_IOCTL_FUNCTION_CLOSE_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pIoObject,
		USBPUMP_OBJECT_HEADER *pClientObject
		);

	This macro correctly initializes a .._FUNCTION_CLOSE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a 
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_FUNCTION_CLOSE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_FUNCTION_CLOSE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER	*pIoObject;
	__TMS_USBPUMP_OBJECT_HEADER	*pClientObject;
	};

/* the setup for the .._FUNCTION_OBEN_ARG struct */
#define __TMS_USBPUMP_IOCTL_FUNCTION_CLOSE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pIoObject,							\
	ARG_pClientObject						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_FUNCTION_CLOSE_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pIoObject = (ARG_pIoObject);				\
	__pArg->pClientObject = (ARG_pClientObject);			\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_FUNCTION_MESSAGE

Index:	Type:	USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG
	Name:	USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG_SETUP_V1()

Function:
	Send a message to an object

Input:
	pInParam	points to a USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG has the following elements:

	UTASK_MSG_HANDLE	hMsg;
		IN: message handle

Description:
	This IOCTL is sent from an central message receive function to a 
	leaf object, to have the leaf object parse and performs whatever 
	actions are needed by the message.

Returns:
	USBPUMP_RESULT_SUCCESS:  message was recognized.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG_SETUP_V1(
		USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG *	pArg,
		UTASK_MSG_HANDLE 			hMsg
		);

	This macro will correctly initialize a .._MESSAGE_ARG
	structure.  If the layout of the argument structure changes in 
	the future, MCCI will adjust this macro to initialize the new 
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG)
	{
	__TMS_UTASK_MSG_HANDLE	hMsg;	/* IN: message handle */
	};

/* the setup for the .._MESSAGE_ARG struct */
#define __TMS_USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMsg							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->hMsg       = (ARG_hMsg);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI

Index:	Type:	USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG
	Name:	USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG_SETUP_V1()

Function:
	Open the message API of a function object

Input:
	pInParam	points to a USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG has the following elements:

	UTASK_MAILBOX_HANDLE	hMailbox;
		IN: handle to task mailbox requesting to open the message API

	UTASK_PROCESS_INDEX	iProcess;
		IN: index to process in task requesting to open the message API

Description:
	This IOCTL is sent from an central message receive function to a 
	leaf object, to open the message API in the leaf object and to 
	assign the requesting task as client of the object.

Returns:
	USBPUMP_RESULT_SUCCESS:  message was recognized.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG_SETUP_V1(
		USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG *	pArg,
		UTASK_HANDLE 					hMailbox,
		UTASK_PROCESS_INDEX				iProcess
		);

	This macro will correctly initialize a .._OPEN_MSGAPI_ARG
	structure.  If the layout of the argument structure changes in 
	the future, MCCI will adjust this macro to initialize the new 
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG)
	{
	__TMS_UTASK_MAILBOX_HANDLE	hMailbox;	/* IN: mailbox handle */
	__TMS_UTASK_PROCESS_INDEX	iProcess;	/* IN: process index */
	};

/* the setup for the .._OPEN_MSGAPI_ARG struct */
#define __TMS_USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMailbox,							\
	ARG_iProcess							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->hMailbox       = (ARG_hMailbox);			\
	__pArg->iProcess       = (ARG_iProcess);			\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI

Index:	Type:	USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG
	Name:	USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG_SETUP_V1()

Function:
	Close the message API of a function object

Input:
	pInParam	points to a USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG has the following elements:

	UTASK_MAILBOX_HANDLE	hMailbox;
		IN: handle to task mailbox requesting to close the message API

	UTASK_PROCESS_INDEX	iProcess;
		IN: index to process in task requesting to close the message API

Description:
	This IOCTL is sent from an central message receive function to a 
	leaf object, to close the message API in the leaf object and to 
	remove the requesting task as client of the object.

Returns:
	USBPUMP_RESULT_SUCCESS:  message was recognized.

	Other failure codes are possible if the request is malformed.

Setup Macro:
	VOID USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG_SETUP_V1(
		USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG *	pArg,
		UTASK_HANDLE 					hMailbox,
		UTASK_PROCESS_INDEX				iProcess
		);

	This macro will correctly initialize a .._CLOSE_MSGAPI_ARG
	structure.  If the layout of the argument structure changes in 
	the future, MCCI will adjust this macro to initialize the new 
	elements in a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG)
	{
	__TMS_UTASK_MAILBOX_HANDLE	hMailbox;		/* IN: mailbox handle */
	__TMS_UTASK_PROCESS_INDEX	iProcess;		/* IN: proc index */
	};

/* the setup for the .._CLOSE_MSGAPI_ARG struct */
#define __TMS_USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMailbox,							\
	ARG_iProcess							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->hMailbox      = (ARG_hMailbox);				\
	__pArg->iProcess      = (ARG_iProcess);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_FUNCTION_STATUS_PHASE

Index:	Type:	USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG
	Name:	USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG_SETUP_V1

Function:
	Send information on how to complete the status phase of a USB request

Input:
	pInParam	points to a USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG structure.

	USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the SIC function object.

	BOOL Success;
		IN: TRUE if status phase should indicate success
	
	USETUP_HANDLE hSetup;
		IN: Handle of the USB request that this status
		    phase relates to

Description:
	This IOCTL is sent from an OS-specific driver to as response to a 
	a previous USB request IOCTL.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted.  
	Some other result if the IOCTL is not implemented, 
	or if the internal plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG_SETUP_V1(
		USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		BOOL					Success,
		USETUP_HANDLE				hSetup
		);

	This macro will correctly initialize a .._STATUS_PHASE_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BOOL			Success;	/* IN: Completion status  */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	};

/* the setup for the .._SIC_STATUS_PHASE_ARG struct */
#define __TMS_USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Success,							\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->Success        = (ARG_Success);				\
	__pArg->hSetup         = (ARG_hSetup);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT

Index:	Type:	USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG
	Type:	PUSBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG

Function:
	Query a function to see whether the underlying hardware or facility
	is "present", whatever that means.  For Catena-based DCDs and HCDs,
	this means that the hardware underlying is actually installed.

Input:
	USBPUMP_OBJECT_HEADER *pIoObject;
		The handle returned when opening the device.

Output:
	BOOL fPresent;
		TRUE if the facility is present; FALSE if the facility claimed
		the IOCTL but discovered that the low-level path to the
		hardware doesn't really exist.

Description:
	This IOCTL must *only* be implemented by HCD, DCD, or transceiver
	modules.  Unlike other FUNCTION IOCTLs, it must not be implemented
	by higher level software.  It will return fPresent TRUE if the
	corresponding hardware is powered up and accessible.  USBDI will
	implement this for class driver instances, and will return FALSE if
	either the class driver is selectively suspended or the Host
	Controller is powered down.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS indicates that |fPresent| is valid.

	USBPUMP_IOCTL_RESULT_NOT_CLAIMED indicates that the hardware or
	software doesn't know.

Setup Macro:
	VOID USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG_SETUP_V1(
		USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pIoObject
		);

Notes:
	The status returned might be somewhat out of sync with what we
	could detemrine about external if we used an async request, allowing
	the request to go all the way to the hardware before returning.
	Instead, we recommend a "best-effort" service, backed by indications
	that come up from the hardware or operating system asynchronously.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject;
	__TMS_BOOL			fPresent;
	};

#define __TMS_USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pIoObject							\
	)								\
   do	{								\
	(a_pArg)->pIoObject = (a_pIoObject);				\
	(a_pArg)->fPreset = __TMS_FALSE;				\
	} while (0)

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_FUNCTION(i, t, NAME)	__TMS_USBPUMP_IOCTL_FUNCTION(i, t, NAME)
# define USBPUMP_IOCTL_FUNCTION_OPEN	__TMS_USBPUMP_IOCTL_FUNCTION_OPEN
# define USBPUMP_IOCTL_FUNCTION_CLOSE	__TMS_USBPUMP_IOCTL_FUNCTION_CLOSE
# define USBPUMP_IOCTL_FUNCTION_MESSAGE	__TMS_USBPUMP_IOCTL_FUNCTION_MESSAGE
# define USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI	__TMS_USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI
# define USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI	__TMS_USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI
# define USBPUMP_IOCTL_FUNCTION_STATUS_PHASE	__TMS_USBPUMP_IOCTL_FUNCTION_STATUS_PHASE
# define USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT	__TMS_USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT
# define USBPUMP_IOCTL_FUNCTION_OPEN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObjectToOpen,						\
	ARG_pClientObject,						\
	ARG_pCtx							\
	)	\
	__TMS_USBPUMP_IOCTL_FUNCTION_OPEN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObjectToOpen,						\
	ARG_pClientObject,						\
	ARG_pCtx							\
	)
# define USBPUMP_IOCTL_FUNCTION_CLOSE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pIoObject,							\
	ARG_pClientObject						\
	)	\
	__TMS_USBPUMP_IOCTL_FUNCTION_CLOSE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pIoObject,							\
	ARG_pClientObject						\
	)
# define USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMsg							\
	)	\
	__TMS_USBPUMP_IOCTL_FUNCTION_MESSAGE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMsg							\
	)
# define USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMailbox,							\
	ARG_iProcess							\
	)	\
	__TMS_USBPUMP_IOCTL_FUNCTION_OPEN_MSGAPI_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMailbox,							\
	ARG_iProcess							\
	)
# define USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMailbox,							\
	ARG_iProcess							\
	)	\
	__TMS_USBPUMP_IOCTL_FUNCTION_CLOSE_MSGAPI_ARG_SETUP_V1(		\
	pArg,								\
	ARG_hMailbox,							\
	ARG_iProcess							\
	)
# define USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Success,							\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Success,							\
	ARG_hSetup							\
	)
# define USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pIoObject							\
	)	\
	__TMS_USBPUMP_IOCTL_FUNCTION_QUERY_PRESENT_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pIoObject							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/**** end of usbioctl_function.h ****/
#endif /* _USBIOCTL_FUNCTION_H_ */
