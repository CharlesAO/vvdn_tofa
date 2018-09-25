/* usbioctl_sic.h	Tue May 08 2008 10:14:44 maw */

/*

Module:  usbioctl_sic.h

Function:
	Abstract interfaces for implementing Still Image Capture devices.

Version:
	V1.97k	Tue May 08 2008 10:14:44 maw	Edit level 12

Copyright notice:
	This file copyright (C) 2004-2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjorn, MCCI Corporation	October 2004

Revision history:
   1.89d  Wed Oct 27 2004 17:33:30 maw
	Module created.

   1.91a  Thu Jan 27 2005 12:50:39  chwon
	Use __TMS_CONCAT() instead of ## because this is more portable code.
	Fix up typo.

   1.91b  Fri Feb 25 2005 10:25:55 maw
   	Corrected multiple fields in structures

   1.91b  Mon Apr 18 2005 11:18:01 maw
   	Added completion handle to QUEUE-IN and QUEUE-OUT

   1.91c  Tue Apr 26 2005 10:48:51 maw
	Moved USBPUMP_IOCTL_SIC_STATUS_PHASE to usbioctl_function.h
	Corrected ArmSDT compile error

   1.91d  Tue Jun 14 2005 13:07:04 maw
	Renamed USBTASK_... to UTASK_...
	
   1.91e  Wed Jun 29 2005 00:58:41 maw
   	Added nSend parameter to USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE

   1.91h  Wed Sep 07 2005 19:38:20 maw
   	Added fStall parameter to USBPUMP_IOCTL_SIC_CANCEL

   1.97f  Mon Oct 23 2006 07:59:07  maw
   	Bug 2024: Added Ioctl for RemoteWakeup status

   1.97k  Tue Apr 22 2008 10:56:43  chwon
	4040: use __TMS_FALSE instead of FALSE.
	Generate uncloaked name using uncloak-def.sh.

   1.97k  Tue May 08 2008 10:14:44  maw
   	6088: Add support for additional StreamMask parameter in Cancel
   	Change USBPUMP_SIC_STREAM type from enum to UINT8.

*/

#ifndef _USBIOCTL_SIC_H_		/* prevent multiple includes */
#define _USBIOCTL_SIC_H_

#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

/****************************************************************************\
|
|	The plane selector type, documented later as part of the SIC
|	QUEUE_IN / QUEUE_OUT IOCTL.
|
\****************************************************************************/

__TMS_TYPE_DEF_ARG(USBPUMP_SIC_STREAM, UINT8);

#define __TMS_USBPUMP_SIC_STREAM_CTRL	  ((__TMS_USBPUMP_SIC_STREAM)(1 << 0))
#define __TMS_USBPUMP_SIC_STREAM_DATA_IN  ((__TMS_USBPUMP_SIC_STREAM)(1 << 1))
#define __TMS_USBPUMP_SIC_STREAM_DATA_OUT ((__TMS_USBPUMP_SIC_STREAM)(1 << 2))
#define __TMS_USBPUMP_SIC_STREAM_EVENT_IN ((__TMS_USBPUMP_SIC_STREAM)(1 << 3))


/****************************************************************************\
|
|	The `EDGE' IOCTLs are sent outwards
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_EDGE_SIC(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_SIC_EDGE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_SIC_, NAME, _ARG) \
		)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(0, RW, CANCEL_TRANS)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(1, RW, DEVICE_RESET)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(2, RW, GET_EXTEV)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(3, RW, GET_DEVICE_STATUS)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(4, RW, GOTO_IDLE)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(5, RW, VENDOR_GET_REQ)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(6, RW, VENDOR_SET_REQ)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(7, RW, QUEUE_IN_DONE)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(8, RW, QUEUE_OUT_DONE)

#define	__TMS_USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC(9, RW, SET_REMOTEWAKEUP)


/****************************************************************************\
|
|	The inward API functions
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_SIC(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_SIC,				\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_SIC_, NAME, _ARG)	\
		)

#define	__TMS_USBPUMP_IOCTL_SIC_CANCEL		\
	__TMS_USBPUMP_IOCTL_SIC(0, W, CANCEL)

#define	__TMS_USBPUMP_IOCTL_SIC_GET_STREAM_STATUS	\
	__TMS_USBPUMP_IOCTL_SIC(1, RW, GET_STREAM_STATUS)

#define	__TMS_USBPUMP_IOCTL_SIC_QUEUE_IN	\
	__TMS_USBPUMP_IOCTL_SIC(2, W, QUEUE_IN)

#define	__TMS_USBPUMP_IOCTL_SIC_QUEUE_OUT	\
	__TMS_USBPUMP_IOCTL_SIC(3, W, QUEUE_OUT)

/* moved to common function-Ioctl */
#define __TMS_USBPUMP_IOCTL_SIC_STATUS_PHASE	\
	__TMS_USBPUMP_IOCTL_FUNCTION_STATUS_PHASE

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing CANCEL_TRANS

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	USETUP_HANDLE hSetup;
		IN: Handle of this USB request

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the host has send a CANCEL_TRANS-Request 
	to cancel an ongoing transaction over the bulk-pipe.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_CANCEL_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		VOID *					pClientContext,
		USETUP_HANDLE				hSetup
		);

	This macro will correctly initialize a .._CANCEL_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_CANCEL_REQ_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->hSetup         = (ARG_hSetup);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing DEVICE_RESET

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	USETUP_HANDLE hSetup;
		IN: Handle of this USB request

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the host has send a DEVICE-RESET Request to reset 
	the the SIC function in the device.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		USETUP_HANDLE					hSetup
		);

	This macro will correctly initialize a .._DEVICE_RESET_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_DEVICE_RESET_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->hSetup         = (ARG_hSetup);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing GET_EXTEV

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	UINT16 ReplyMax;
		IN: Max number of bytes in reply

	USETUP_HANDLE hSetup;
		IN: Handle of this USB request

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent when host wants to get extended event data

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		VOID *					pClientContext,
		UINT16					ReplyMax,
		USETUP_HANDLE				hSetup
		);

	This macro will correctly initialize a .._GET_EXTEV_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_UINT16			ReplyMax;	/* IN: size of reply-buffer */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_GET_EXTEV_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_ReplyMax,							\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ReplyMax       = (ARG_ReplyMax);			\
	__pArg->hSetup         = (ARG_hSetup);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing GET_DEVICE_STATUS

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	UINT16 ReplyMax;
		IN: Max number of bytes in reply

	USETUP_HANDLE hSetup;
		IN: Handle of this USB request

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent when host wants to get SIC device status

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		UINT16						ReplyMax,
		USETUP_HANDLE					hSetup
		);

	This macro will correctly initialize a .._GET_DEVICE_STATUS_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_UINT16			ReplyMax;	/* IN: max number of bytes in reply */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_GET_DEVICE_STATUS_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_ReplyMax,							\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG * __TMS_CONST  \
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ReplyMax       = (ARG_ReplyMax);			\
	__pArg->hSetup         = (ARG_hSetup);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing cancellation done and
	goto IDLE state

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver that has completed client requested cancellation.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		VOID *					pClientContext
		);

	This macro will correctly initialize a .._GOTO_IDLE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_GOTO_IDLE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing VENDOR_GET_REQ

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	UINT8 bRequest;
		IN: bRequest from setup-message

	UINT16 wValue;
		IN: wValue from setup-message

	UINT16 wLength;
		IN: wLength from setup-message
		
	USETUP_HANDLE hSetup;
		IN: Handle of this USB request

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host has send a vendor specific device-to-host
	request to the SIC interface.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		UINT8 						bRequest,
		UINT16						wValue,
		UINT16						wLength,
		USETUP_HANDLE					hSetup
		);

	This macro will correctly initialize a .._VENDOR_GET_REQ_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_UINT8			bRequest;	/* IN: bRequest from setup-msg */
	__TMS_UINT16			wValue;		/* IN: wValue from setup-msg */
	__TMS_UINT16			wLength;	/* IN: wLength from setup-msg */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_VENDOR_GET_REQ_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_bRequest,							\
	ARG_wValue,							\
	ARG_wLength,							\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->bRequest       = (ARG_bRequest);			\
	__pArg->wValue         = (ARG_wValue);				\
	__pArg->wLength        = (ARG_wLength);				\
	__pArg->hSetup         = (ARG_hSetup);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing VENDOR_SET_REQ

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	UINT8 bRequest;
		IN: bRequest from setup-message

	UINT16 wValue;
		IN: wValue from setup-message

	UINT16 wLength;
		IN: wLength from setup-message

	USETUP_HANDLE hSetup;
		IN: Handle of this USB request

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host has send a vendor specific device-to-host
	request to the SIC interface.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		UINT8 						bRequest,
		UINT16						wValue,
		UINT16						wLength,
		USETUP_HANDLE					hSetup
		);

	This macro will correctly initialize a .._VENDOR_SET_REQ_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_UINT8			bRequest;	/* IN: bRequest from setup-msg */
	__TMS_UINT16			wValue;		/* IN: wValue from setup-msg */
	__TMS_UINT16			wLength;	/* IN: wLength from setup-msg */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_VENDOR_SET_REQ_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_bRequest,							\
	ARG_wValue,							\
	ARG_wLength,							\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->bRequest       = (ARG_bRequest);			\
	__pArg->wValue         = (ARG_wValue);				\
	__pArg->wLength        = (ARG_wLength);				\
	__pArg->hSetup         = (ARG_hSetup);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing QUEUE_IN_DONE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	USBPUMP_SIC_STREAM Stream;
		IN: In-stream where completion occured

	UTASK_COMPLETION_HANDLE hCompletion;
		IN: Completion handle
		
	VOID * pBuf;
		IN: Pointer to buffer

	BYTES nBuf;
		IN: Number of bytes in buffer

	BYTES nSend;
		IN: Number of bytes send from buffer

	USTAT Status;
		IN: Completion status

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever a buffer that has beeb queued to an in-stream
	has completed.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		USBPUMP_SIC_STREAM				Stream,
		UTASK_COMPLETION_HANDLE				hCompletion,
		VOID *						pBuf,
		BYTES						nBuf,
		BYTES						nSend,
		USTAT						Status
		);

	This macro will correctly initialize a .._QUEUE_IN_DONE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_USBPUMP_SIC_STREAM	Stream;		/* IN: stream type */
	__TMS_UTASK_COMPLETION_HANDLE	hCompletion;	/* IN: completion handle */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBuf;		/* IN: bytes in buffer */
	__TMS_BYTES			nSend;		/* IN: bytes send from buffer */
	__TMS_USTAT			Status;		/* IN: Completion status */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_QUEUE_IN_DONE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_nSend,							\
	ARG_Status							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->Stream         = (ARG_Stream);				\
	__pArg->hCompletion    = (ARG_hCompletion);			\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->nBuf           = (ARG_nBuf);				\
	__pArg->nSend          = (ARG_nSend);				\
	__pArg->Status         = (ARG_Status);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing QUEUE_OUT_DONE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	USBPUMP_SIC_STREAM Stream;
		IN: Out-stream where completion occured

	UTASK_COMPLETION_HANDLE hCompletion;
		IN: Completion handle
		
	VOID * pBuf;
		IN: Pointer to buffer

	BYTES nBuf;
		IN: Number of bytes in buffer

	USTAT Status;
		IN: Completion status

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever a buffer that has been queued to an out-stream
	has completed.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		USBPUMP_SIC_STREAM				Stream,
		UTASK_COMPLETION_HANDLE				hCompletion,
		VOID *						pBuf,
		BYTES						nBuf,
		USTAT						Status
		);

	This macro will correctly initialize a .._QUEUE_OUT_DONE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_USBPUMP_SIC_STREAM	Stream;		/* IN: stream type */
	__TMS_UTASK_COMPLETION_HANDLE	hCompletion;	/* IN: completion handle */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBuf;		/* IN: bytes in buffer */
	__TMS_USTAT			Status;		/* IN: Completion status */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_QUEUE_OUT_DONE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_Status							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->Stream         = (ARG_Stream);				\
	__pArg->hCompletion    = (ARG_hCompletion);			\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->nBuf           = (ARG_nBuf);				\
	__pArg->Status         = (ARG_Status);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP

Index:	Type:	USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG
	Name:	USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing RemoteWakeup

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG structure.

	USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the SIC object.  Presumably this client has 
		registered with the client as the owning driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BOOL fRemoteWakeup;
		IN: Current RemoteWakeup capability

Description:
	This IOCTL is sent from a SIC function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent during startup or whenever RemoteWakeup capability 
	changes in UDEVICE.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		BOOL						fRemoteWakeup
		);

	This macro will correctly initialize a .._REMOTEWAKEUP_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BOOL			fRemoteWakeup;	/* IN: stream type */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._SIC_SET_REMOTEWAKEUP_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fRemoteWakeup						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->fRemoteWakeup  = (ARG_fRemoteWakeup);			\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_SIC_CANCEL

Index:	Type:	USBPUMP_IOCTL_SIC_CANCEL_ARG
	Name:	USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V1

Function:
	Cancel all queued data-stream buffers

Input:
	pInParam	points to a USBPUMP_IOCTL_SIC_CANCEL_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_SIC_CANCEL_ARG structure.

	USBPUMP_IOCTL_SIC_CANCEL_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the SIC function object.

	BOOL fStall;
		IN: Tells SIC whether to set stall on data-ep or not

	USBPUMP_SIC_STREAM StreamMask;
		IN: Tells SIC which streams to cancel

Description:
	This IOCTL is sent from an OS-specific driver to cancel 
	all queued data-stream buffers.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the Ioctl was accepted.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.
	USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE will be issued by a SIC function once
	SIC function has cleared the transport layer.

Setup Macro:
	VOID USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V2(
		USBPUMP_IOCTL_SIC_CANCEL_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *		pObject,
		BOOL				fStall,
		USBPUMP_SIC_STREAM 		StreamMask
		);

	This macro will correctly initialize a .._CANCEL_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_SIC_CANCEL_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_SIC_CANCEL_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BOOL			fStall;
	__TMS_USBPUMP_SIC_STREAM	StreamMask;
	};

/* the setup for the .._SIC_CANCEL_ARG struct */
#define __TMS_USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V2(			\
	pArg,								\
	ARG_pObject,							\
	ARG_fStall,							\
	ARG_StreamMask							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_SIC_CANCEL_ARG * __TMS_CONST		\
	__pArg             = (pArg);					\
	__pArg->pObject    = (ARG_pObject);				\
	__pArg->fStall     = (ARG_fStall);				\
	__pArg->StreamMask = (ARG_StreamMask);				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_fStall							\
	)								\
   __TMS_USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V2(				\
	pArg,								\
	ARG_pObject,							\
	ARG_fStall,							\
	__TMS_USBPUMP_SIC_STREAM_DATA_IN |				\
	__TMS_USBPUMP_SIC_STREAM_DATA_OUT				\
	)

/*

IOCTL:	USBPUMP_IOCTL_SIC_GET_STREAM_STATUS

Index:	Type:	USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG
	Name:	USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG_SETUP_V1

Function:
	Retrieve stream status from SIC

Input:
	pInParam	points to a USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG structure.

	USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the SIC function object.

	USBPUMP_SIC_STREAM Stream;
		IN: Stream to get status on

	USHORT MaxPacketSize;
		OUT: Max packet size for endpoint on stream

	BOOL Halted;
		OUT: State of endpoint
	
Description:
	This IOCTL is sent from an OS-specific driver to as response to a 
	USBPUMP_IOCTL_SIC_DEVICE_RESET.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG_SETUP_V1(
		USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject
		USBPUMP_SIC_STREAM				Stream
		);

	This macro will correctly initialize a .._GET_STREAM_STATUS_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN:  pointer to storage object */
	__TMS_USBPUMP_SIC_STREAM	Stream;		/* IN:  stream type */
	__TMS_USHORT			MaxPacketSize;	/* OUT: wMaxPacketSize for ep */
	__TMS_BOOL			Halted;		/* OUT: ep stalled */
	};

/* the setup for the .._SIC_GET_STREAM_STATUS_ARG struct */
#define __TMS_USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->Stream         = (ARG_Stream);				\
	__pArg->MaxPacketSize  = 0;					\
	__pArg->Halted         = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_SIC_STATUS_PHASE

Index:	Type:	USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG
	Name:	USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG_SETUP_V1

Function:
	Send information on how to complete the status phase of a USB request

	********* THIS IOCTL HAS BEEN REPLACED BY ******************
	********* USBPUMP_IOCTL_FUNCTION_STATUS_PHASE **************
Input:
	pInParam	points to a USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG structure.

	USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG has the following elements:

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
	VOID USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG_SETUP_V1(
		USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG *	pArg,
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

#define __TMS_USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG	\
	__TMS_USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG

#define __TMS_USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG_SETUP_V1	\
	__TMS_USBPUMP_IOCTL_FUNCTION_STATUS_PHASE_ARG_SETUP_V1

/*

IOCTL:	USBPUMP_IOCTL_SIC_QUEUE_IN

Index:	Type:	USBPUMP_IOCTL_SIC_QUEUE_IN_ARG
	Name:	USBPUMP_IOCTL_SIC_QUEUE_IN_ARG_SETUP_V1

Function:
	Queue a buffer to an in-stream.

Input:
	pInParam	points to a USBPUMP_IOCTL_SIC_QUEUE_IN_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_SIC_QUEUE_IN_ARG structure.

	USBPUMP_IOCTL_SIC_QUEUE_IN_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the SIC function object.

	USBPUMP_SIC_STREAM Stream;
		IN: In-stream to queue buffer to

	UTASK_COMPLETION_HANDLE hCompletion;
		IN: Completion handle
		
	VOID *pBuf;
		IN: Pointer to buffer

	BYTES nBuf;
		IN: Number of bytes in buffer

	BOOL fFirst;
		IN: TRUE if this is the first buffer of the transfer

	BOOL fLast;
		IN: TRUE if this is the last buffer of the transfer

	USETUP_HANDLE hSetup;
		IN: Handle USB request related to this buffer

Description:
	This IOCTL is sent from an OS-specific driver to queue a buffer
	to be send to the host.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_SIC_QUEUE_IN_ARG_SETUP_V1(
		USBPUMP_IOCTL_SIC_QUEUE_IN_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		USBPUMP_SIC_STREAM			Stream,
		UTASK_COMPLETION_HANDLE		hCompletion,
		VOID *					pBuf,
		BYTES					nBuf,
		BOOL					fFirst,
		BOOL					fLast,
		USETUP_HANDLE				hSetup
		);

	This macro will correctly initialize a .._QUEUE_IN_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_SIC_QUEUE_IN_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_SIC_QUEUE_IN_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_USBPUMP_SIC_STREAM	Stream;		/* IN: stream type */
	__TMS_UTASK_COMPLETION_HANDLE	hCompletion;	/* IN: completion handle */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBuf;		/* IN: bytes in buffer */
	__TMS_BOOL			fFirst;		/* IN: first buffer flag */
	__TMS_BOOL			fLast;		/* IN: last buffer flag */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	};

/* the setup for the .._SIC_QUEUE_IN_ARG struct */
#define __TMS_USBPUMP_IOCTL_SIC_QUEUE_IN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_fFirst,							\
	ARG_fLast,							\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_SIC_QUEUE_IN_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject     = (ARG_pObject);				\
	__pArg->Stream      = (ARG_Stream);				\
	__pArg->hCompletion = (ARG_hCompletion);			\
	__pArg->pBuf        = (ARG_pBuf);				\
	__pArg->nBuf        = (ARG_nBuf);				\
	__pArg->fFirst      = (ARG_fFirst);				\
	__pArg->fLast       = (ARG_fLast);				\
	__pArg->hSetup      = (ARG_hSetup);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_SIC_QUEUE_OUT

Index:	Type:	USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG
	Name:	USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG_SETUP_V1

Function:
	Queue a buffer to an out-stream.

Input:
	pInParam	points to a USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG structure.

	USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the SIC function object.

	USBPUMP_SIC_STREAM Stream;
		IN: Out-stream to queue buffer to

	UTASK_COMPLETION_HANDLE hCompletion;
		IN: Completion handle
		
	VOID *pBuf;
		IN: Pointer to buffer

	BYTES nBuf;
		IN: Number of bytes in buffer

	BOOL fFirst;
		IN: TRUE if this is the first buffer of the transfer

	BOOL fLast;
		IN: TRUE if this is the last buffer of the transfer

	USETUP_HANDLE hSetup;
		IN: Handle USB request related to this buffer

Description:
	This IOCTL is sent from an OS-specific driver to queue a buffer
	for the host to write to.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG_SETUP_V1(
		USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		USBPUMP_SIC_STREAM			Stream,
		UTASK_COMPLETION_HANDLE		hCompletion,
		VOID *					pBuf,
		BYTES					nBuf,
		BOOL					fFirst,
		BOOL					fLast,
		USETUP_HANDLE				hSetup
		);

	This macro will correctly initialize a .._QUEUE_OUT_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(  USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_USBPUMP_SIC_STREAM	Stream;		/* IN: Stream type */
	__TMS_UTASK_COMPLETION_HANDLE	hCompletion;	/* IN: completion handle */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBuf;		/* IN: bytes in buffer */
	__TMS_BOOL			fFirst;		/* IN: first buffer flag */
	__TMS_BOOL			fLast;		/* IN: last buffer flag */
	__TMS_USETUP_HANDLE		hSetup;		/* IN: command handle */
	};

/* the setup for the .._SIC_QUEUE_OUT_ARG struct */
#define __TMS_USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_fFirst,							\
	ARG_fLast,							\
	ARG_hSetup							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject     = (ARG_pObject);				\
	__pArg->Stream      = (ARG_Stream);				\
	__pArg->hCompletion = (ARG_hCompletion);			\
	__pArg->pBuf        = (ARG_pBuf);				\
	__pArg->nBuf        = (ARG_nBuf);				\
	__pArg->fFirst      = (ARG_fFirst);				\
	__pArg->fLast       = (ARG_fLast);				\
	__pArg->hSetup      = (ARG_hSetup);				\
   	} while (0)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_SIC_STREAM_CTRL	\
   __TMS_USBPUMP_SIC_STREAM_CTRL
# define USBPUMP_SIC_STREAM_DATA_IN	\
   __TMS_USBPUMP_SIC_STREAM_DATA_IN
# define USBPUMP_SIC_STREAM_DATA_OUT	\
   __TMS_USBPUMP_SIC_STREAM_DATA_OUT
# define USBPUMP_SIC_STREAM_EVENT_IN	\
   __TMS_USBPUMP_SIC_STREAM_EVENT_IN
# define USBPUMP_IOCTL_EDGE_SIC(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC(i, t, NAME)
# define USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS
# define USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET
# define USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV
# define USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS
# define USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE
# define USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ
# define USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ
# define USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE
# define USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE
# define USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP	\
   __TMS_USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP
# define USBPUMP_IOCTL_SIC(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_SIC(i, t, NAME)
# define USBPUMP_IOCTL_SIC_CANCEL	\
   __TMS_USBPUMP_IOCTL_SIC_CANCEL
# define USBPUMP_IOCTL_SIC_GET_STREAM_STATUS	\
   __TMS_USBPUMP_IOCTL_SIC_GET_STREAM_STATUS
# define USBPUMP_IOCTL_SIC_QUEUE_IN	\
   __TMS_USBPUMP_IOCTL_SIC_QUEUE_IN
# define USBPUMP_IOCTL_SIC_QUEUE_OUT	\
   __TMS_USBPUMP_IOCTL_SIC_QUEUE_OUT
# define USBPUMP_IOCTL_SIC_STATUS_PHASE	\
   __TMS_USBPUMP_IOCTL_SIC_STATUS_PHASE
# define USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_CANCEL_TRANS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_hSetup							\
	)
# define USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_DEVICE_RESET_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_hSetup							\
	)
# define USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_ReplyMax,							\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_GET_EXTEV_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_ReplyMax,							\
	ARG_hSetup							\
	)
# define USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_ReplyMax,							\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_GET_DEVICE_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_ReplyMax,							\
	ARG_hSetup							\
	)
# define USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_GOTO_IDLE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_bRequest,							\
	ARG_wValue,							\
	ARG_wLength,							\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_GET_REQ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_bRequest,							\
	ARG_wValue,							\
	ARG_wLength,							\
	ARG_hSetup							\
	)
# define USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_bRequest,							\
	ARG_wValue,							\
	ARG_wLength,							\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_VENDOR_SET_REQ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_bRequest,							\
	ARG_wValue,							\
	ARG_wLength,							\
	ARG_hSetup							\
	)
# define USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_nSend,							\
	ARG_Status							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_IN_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_nSend,							\
	ARG_Status							\
	)
# define USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_Status							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_QUEUE_OUT_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_Status							\
	)
# define USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fRemoteWakeup						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_SIC_SET_REMOTEWAKEUP_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fRemoteWakeup						\
	)
# define USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V2(			\
	pArg,								\
	ARG_pObject,							\
	ARG_fStall,							\
	ARG_StreamMask							\
	)	\
	__TMS_USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V2(			\
	pArg,								\
	ARG_pObject,							\
	ARG_fStall,							\
	ARG_StreamMask							\
	)
# define USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_fStall							\
	)	\
	__TMS_USBPUMP_IOCTL_SIC_CANCEL_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_fStall							\
	)
# define USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream							\
	)	\
	__TMS_USBPUMP_IOCTL_SIC_GET_STREAM_STATUS_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream							\
	)
# define USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG	\
   __TMS_USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG
# define USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG_SETUP_V1	\
   __TMS_USBPUMP_IOCTL_SIC_STATUS_PHASE_ARG_SETUP_V1
# define USBPUMP_IOCTL_SIC_QUEUE_IN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_fFirst,							\
	ARG_fLast,							\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_SIC_QUEUE_IN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_fFirst,							\
	ARG_fLast,							\
	ARG_hSetup							\
	)
# define USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_fFirst,							\
	ARG_fLast,							\
	ARG_hSetup							\
	)	\
	__TMS_USBPUMP_IOCTL_SIC_QUEUE_OUT_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_Stream,							\
	ARG_hCompletion,						\
	ARG_pBuf,							\
	ARG_nBuf,							\
	ARG_fFirst,							\
	ARG_fLast,							\
	ARG_hSetup							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_sic.h ****/
#endif /* _USBIOCTL_SIC_H_ */
