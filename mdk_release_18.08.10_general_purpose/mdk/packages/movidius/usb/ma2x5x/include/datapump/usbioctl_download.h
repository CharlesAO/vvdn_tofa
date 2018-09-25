/* usbioctl_download.h	Tue Jun 07 2011 15:14:08 chwon */

/*

Module:  usbioctl_download.h

Function:
	Abstract interfaces for implementing firmware up 'n download devices.

Version:
	V3.01f	Tue Jun 07 2011 15:14:08 chwon	Edit level 6

Copyright notice:
	This file copyright (C) 2004-2006, 2009-2011 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjorn, MCCI Corporation	June 2004

Revision history:
   1.89a  Tue Jun 08 2004 11:43:59 maw
	Module created.

   1.89a  Tue Jun 22 2004 11:20:5 maw
	ARM-ADS compiler problem resolved

   1.91a  Thu Jan 27 2005 12:50:39  chwon
	Use __TMS_CONCAT() instead of ## because this is more portable code.

   1.97f  Mon Sep 11 2006 12:17:59  skpark
	Fixed error in generating chm file

   2.01a  Wed Dec 02 2009 12:08:50  skpark
	9357: Fixed invalid token error while generating chm file

   3.01f  Tue Jun 07 2011 15:14:08  chwon
	11353: add bInterfaceNumber and bAlternateSetting argument for the
	USBPUMP_IOCTL_DOWNLOAD_GET_INFO. Generate uncloaked names.

*/

#ifndef _USBIOCTL_DOWNLOAD_H_		/* prevent multiple includes */
#define _USBIOCTL_DOWNLOAD_H_

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

/****************************************************************************\
|
|	Constants and types
|
\****************************************************************************/

enum __TMS_ENUMNAME(USBPUMP_DOWNLOAD_DEVICE_MODE)
	{
	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_UNDEFINED,	/* Not valid boot mode ! */
	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP,
	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_DFU
	};
__TMS_TYPE_DEF_ENUM(USBPUMP_DOWNLOAD_DEVICE_MODE);

enum __TMS_ENUMNAME(USBPUMP_DOWNLOAD_CLIENT_STATE)
	{
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_0_APP_IDLE          = 0,
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE          = 1,
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_2_DFU_WRITE_IDLE    = 2,
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_3_DFU_WRITE_BUSY    = 3,
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_4_DFU_MANIFEST_BUSY = 4,
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_5_DFU_READ_BUSY     = 5,
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR         = 6
	};
__TMS_TYPE_DEF_ENUM(USBPUMP_DOWNLOAD_CLIENT_STATE);

#define	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE__INIT \
	{ \
	/* CLIENT_STATE_0_APP_IDLE		*/ "App Idle", \
	/* CLIENT_STATE_1_DFU_IDLE 		*/ "Dfu Idle", \
	/* CLIENT_STATE_2_DFU_WRITE_IDLE	*/ "Dfu Write idle", \
	/* CLIENT_STATE_3_DFU_WRITE_BUSY	*/ "Dfu Write busy", \
	/* CLIENT_STATE_4_DFU_MANIFEST_BUSY	*/ "Dfu Manifest busy", \
	/* CLIENT_STATE_5_DFU_READ_BUSY		*/ "Dfu Read busy", \
	/* CLIENT_STATE_6_DFU_ERROR		*/ "Dfu Error" \
	}

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_DOWNLOAD_DEVICE_MODE_UNDEFINED	\
   __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_UNDEFINED
# define USBPUMP_DOWNLOAD_DEVICE_MODE_APP	\
   __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP
# define USBPUMP_DOWNLOAD_DEVICE_MODE_DFU	\
   __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_DFU
# define USBPUMP_DOWNLOAD_CLIENT_STATE_0_APP_IDLE	\
   __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_0_APP_IDLE
# define USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE	\
   __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE
# define USBPUMP_DOWNLOAD_CLIENT_STATE_2_DFU_WRITE_IDLE	\
   __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_2_DFU_WRITE_IDLE
# define USBPUMP_DOWNLOAD_CLIENT_STATE_3_DFU_WRITE_BUSY	\
   __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_3_DFU_WRITE_BUSY
# define USBPUMP_DOWNLOAD_CLIENT_STATE_4_DFU_MANIFEST_BUSY	\
   __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_4_DFU_MANIFEST_BUSY
# define USBPUMP_DOWNLOAD_CLIENT_STATE_5_DFU_READ_BUSY	\
   __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_5_DFU_READ_BUSY
# define USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR	\
   __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR
#endif

/****************************************************************************\
|
|	The `EDGE' IOCTLs are sent outwards
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_DOWNLOAD_EDGE,		\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_, NAME, _ARG) \
		)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(0, W, START_TMR)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(1, RW, OPEN_WRITE)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(2, RW, WRITE)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(3, RW, CLOSE_WRITE)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(4, RW, OPEN_READ)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_READ	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(5, RW, READ)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(6, RW, CLOSE_READ)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(7, RW, MANIFEST)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(8, RW, STATUS)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(9, RW, RESET)

#define	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(10, W, RESTART)


/****************************************************************************\
|
|	The inward API functions
|
\****************************************************************************/

#define __TMS_USBPUMP_DOWNLOAD_CRC_START	(0xffffffff)

#define __TMS_USBPUMP_IOCTL_DOWNLOAD(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_DOWNLOAD,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_DOWNLOAD_, NAME, _ARG) \
		)

#define	__TMS_USBPUMP_IOCTL_DOWNLOAD_CALC_CRC	\
	__TMS_USBPUMP_IOCTL_DOWNLOAD(0, RW, CALC_CRC)

#define	__TMS_USBPUMP_IOCTL_DOWNLOAD_GET_INFO	\
	__TMS_USBPUMP_IOCTL_DOWNLOAD(1, RW, GET_INFO)

#define	__TMS_USBPUMP_IOCTL_DOWNLOAD_TIMEOUT	\
	__TMS_USBPUMP_IOCTL_DOWNLOAD(2, W, TIMEOUT)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing START_TMR

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG
			structure.
	pOutParam	not used

	USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	ULONG TmoInMs;
		IN: Timeout in milliseconds

	ULONG Id;
		IN: Value returned on timeout

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the function needs to start a timer. The client
	will then issue an USBPUMP_IOCTL_DOWNLOAD_TIMEOUT once the timer
	expires.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID * 						pClientContext,
		ULONG						TmoInMs,
		ULONG						Id
		);

	This macro will correctly initialize a .._START_TMR_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	The client is responsible for starting a timer that will expire after TmoInMs
	milliseconds and to store Id to be returned on timeout

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to function object */
	__TMS_VOID * 			pClientContext; /* IN: client context */
	__TMS_ULONG			TmoInMs;	/* IN: timeout in milliseconds */
	__TMS_ULONG			Id;		/* IN: value returned on timeout */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._DOWNLOAD_START_TMR_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_TmoInMs,							\
	ARG_Id								\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->TmoInMs        = (ARG_TmoInMs);				\
	__pArg->Id             = (ARG_Id);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing OPEN_WRITE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

	ULONG NextTmoInMs;
		OUT: the number of milliseconds the client will need to complete
		     the first EDGE_WRITE Ioctl

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this call as part of the transition between
	state 2 dfuIDLE and state 3 dfuDNLOAD-SYNC. The client is expected
	to make appropriate actions in order to be prepared for receiving
	data to the device.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID * 						pClientContext
		);

	This macro will correctly initialize a .._OPEN_WRITE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *		pObject;	/* IN: pointer to function object */
	__TMS_VOID * 				pClientContext; /* IN: client context */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_ULONG				NextTmoInMs;	/* OUT: timeout on first Write Ioctl */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_OPEN_WRITE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->NextTmoInMs    = 0;					\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing WRITE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *  pClientContext;
		IN: client context supplied when registering.

	USHORT nBytes;
		IN: the number bytes to write to the device

	VOID *  pBuf;
		IN: pointer to buffer

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

	ULONG NextTmoInMs;
		OUT: the number of milliseconds the client will need to complete
		     the next EDGE_WRITE Ioctl

Description:
	This IOCTL is sent from a download function to the OS-specific driver that
	has opened/connected to the leaf object.
	The function will make this Ioctl as long as the host has data to download.
	The client must return from the Ioctl within the timeout indicated by the
	previous Ioctl (OpenWrite() or Write())

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		VOID * 					pClientContext,
		USHORT					nBytes,
		VOID * 					pBuf
		);

	This macro will correctly initialize a .._WRITE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
						pObject;	/* IN: pointer to function object */
	__TMS_VOID * 				pClientContext; /* IN: client context */
	__TMS_USHORT				nBytes;		/* IN: the number bytes to write */
	__TMS_VOID * 				pBuf;		/* IN: pointer to buffer */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_ULONG				NextTmoInMs;	/* OUT: timeout on next Write Ioctl */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};


/* the setup for the .._DOWNLOAD_WRITE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_nBytes,							\
	ARG_pBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->nBytes         = (ARG_nBytes);				\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->NextTmoInMs    = 0;					\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing CLOSE_WRITE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

	ULONG NextTmoInMs;
		OUT: the number of milliseconds the client will need to complete
		     the first EDGE_MANIFEST Ioctl

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this Ioctl when the host has no more data to download.
	The client is expected to make appropriate actions in order to be prepared
	for the manifestation phase of the download

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext
		);

	This macro will correctly initialize a .._CLOSE_WRITE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *		pObject;	/* IN: pointer to function object */
	__TMS_VOID *				pClientContext; /* IN: client context */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_ULONG				NextTmoInMs;	/* OUT: timeout on first Manifest Ioctl */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_CLOSE_WRITE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->NextTmoInMs    = 0;					\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing OPEN_READ

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this call as part of the transition between
	state 2 dfuIDLE and state 9 dfuUPLOAD-IDLE. The client is expected
	to make appropriate actions in order to be prepared for transmitting
	data from the device.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext
		);

	This macro will correctly initialize a .._OPEN_READ_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
						pObject;	/* IN: pointer to function object */
	__TMS_VOID *				pClientContext; /* IN: client context */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_OPEN_READ_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_READ

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing READ

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USHORT nMaxBytes;
		IN: the maximum number bytes to read from the device

	VOID *  pBuf;
		IN: pointer to buffer

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

	USHORT nBytes;
		OUT: the actual number bytes read from the device

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this Ioctl as long as the device has data to upload.
	The client must promptly return from this ioctl.  pBuf points to a
	buffer with room for nMaxBytes bytes of data

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject
		VOID *					pClientContext,
		USHORT					nMaxBytes,
		VOID * 					pBuf
		);

	This macro will correctly initialize a .._READ_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *		pObject;	/* IN: pointer to function object */
	__TMS_VOID *				pClientContext; /* IN: client context */
	__TMS_USHORT				nMaxBytes;	/* IN: the maximum number bytes to read */
	__TMS_VOID * 				pBuf;		/* IN: pointer to buffer */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_USHORT				nBytes;		/* OUT: the actual number bytes read */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_READ_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_nMaxBytes,							\
	ARG_pBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->nMaxBytes      = (ARG_nMaxBytes);			\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->nBytes         = 0;					\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing CLOSE_READ

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this Ioctl when the device has had less than
	requested amount of data to upload. The client is expected to make
	appropriate actions to clean up the read operation in order to be
	prepared for a new operation.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext
		);

	This macro will correctly initialize a .._CLOSE_READ_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
						pObject;	/* IN: pointer to function object */
	__TMS_VOID *				pClientContext; /* IN: client context */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_CLOSE_READ_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing MANIFEST

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

	ULONG NextTmoInMs;
		OUT: the number of milliseconds the client will need to complete
		     the next EDGE_MANIFEST Ioctl

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this Ioctl after a successful call to CLOSE_WRITE-Ioctl
	or as long as the client indicates that it isn't ready with the manifestation phase.
	Some devices that cannot program memory while maintaining the USB function may
	store data in some intermediate storage while in the write phase, and then use
	the manifestation phase to transfer data into nonvolatile memory.
	Other devices that are capable of programming memory while maintaining the USB function
	can use this phase to verify that the downloaded data is valid, i.e has a correct checksum.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext
		);

	This macro will correctly initialize a .._MANIFEST_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
						pObject;	/* IN: pointer to storage object */
	__TMS_VOID *				pClientContext; /* IN: client context */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_ULONG				NextTmoInMs;	/* OUT: timeout on first Write Ioctl */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_MANIFEST_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->NextTmoInMs    = 0;					\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing STATUS

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

	UCHAR ClientStatus;
		OUT: client status on return from Ioctl

	UCHAR iString;
		OUT: string descriptor index for vendor specific error

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this Ioctl when it needs to find out the
	Client's internal state and status.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STATUS_MANIFEST_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext
		);

	This macro will correctly initialize a .._STATUS_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *		pObject;	/* IN: pointer to storage object */
	__TMS_VOID *				pClientContext; /* IN: client context */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_UCHAR				ClientStatus;	/* OUT: client status on return from Ioctl */
	__TMS_UCHAR				iString;	/* OUT: string descriptor index
								||  for vendor specific error
								*/
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_STATUS_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->ClientStatus   = USB_Dfu_Status_errUNKNOWN;		\
	__pArg->iString        = 0;					\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing RESET

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USBPUMP_DOWNLOAD_CLIENT_STATE ClientState;
		OUT: client state on return from Ioctl

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this Ioctl when it needs to reset the
	client to a known state.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STATUS_MANIFEST_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext
		);

	This macro will correctly initialize a .._RESET_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *		pObject;	/* IN: pointer to storage object */
	__TMS_VOID *				pClientContext; /* IN: client context */
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	ClientState;	/* OUT: client state on return from Ioctl */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_RESET_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->ClientState    = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART

Index:	Type:	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG
	Name:	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing RESTART

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG structure.

	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	USBPUMP_DOWNLOAD_DEVICE_MODE DeviceMode;
		IN: requested startup mode after warm-boot

Description:
	This IOCTL is sent from a download function to the OS-specific
	driver that has opened/connected to the leaf object.
	The function will make this Ioctl when it needs to do a warm-boot
	of the entire device.
	The client will return from this Ioctl if request is accepted.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STATUS_MANIFEST_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject
		VOID *						pClientContext,
		USBPUMP_DOWNLOAD_DEVICE_MODE			DeviceMode
		);

	This macro will correctly initialize a .._RESTART_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *		pObject;	/* IN: pointer to storage object */
	__TMS_VOID *				pClientContext; /* IN: client context */
	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE	DeviceMode;	/* IN: startup mode after warm-boot */
	__TMS_BOOL				fReject;	/* OUT: set TRUE if the edge
								||  accepts the request, FALSE
								||  otherwise.  If possible, will
								||  be used to fail the activate
								||  request on the bus, but this
								||  is not always possible.
								*/
	};

/* the setup for the .._DOWNLOAD_RESTART_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_DeviceMode							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->DeviceMode     = (ARG_DeviceMode);			\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DOWNLOAD_CALC_CRC

Index:	Type:	USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG
	Name:	USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG_SETUP_V1

Function:
	Request a CRC-32 calculation on block of data

Input:
	pInParam	points to a USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG structure.

	USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the download function object.

	CONST UCHAR *pBuf;
		IN: Pointer to buffer with data to calc CRC on

	BYTES nBytes;
		IN: Number of of bytes in buffer

	UINT32 CrcAcc;
		IN/OUT: Start/end value of CRC-accumulator

Description:
	This IOCTL is sent from an OS-specific driver to request a
	CRC-32 calculation on block of data

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted.
	Some other result if the IOCTL is not implemented, or if
	the internal plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG_SETUP_V1(
		USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		CONST UCHAR *				pBuf,
		BYTES					nBytes,
		UINT32					CrcAcc
		);

	This macro will correctly initialize a .._CALC_CRC_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_CONST __TMS_UCHAR *	pBuf;		/* IN: pointer to buffer to calc CRC on */
	__TMS_BYTES			nBytes;		/* IN: number of bytes in buffer */
	__TMS_UINT32			CrcAcc;		/* IN/OUT: Start/end value of CRC-accumulator */
	};

/* the setup for the .._DOWNLOAD_CALC_CRC_ARG struct */
#define __TMS_USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_nBytes,							\
	ARG_CrcAcc							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject  = (ARG_pObject);				\
	__pArg->pBuf     = (ARG_pBuf);					\
	__pArg->nBytes   = (ARG_nBytes);				\
	__pArg->CrcAcc   = (ARG_CrcAcc);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DOWNLOAD_GET_INFO

Index:	Type:	USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG
	Name:	USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG_SETUP_V1

Function:
	Request function to return the info on the download interface used

Input:
	pInParam	points to a USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG structure.

	USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the download function object.

	UINT8 StringId;
		OUT: index of the string descriptor associated with the interface

	UINT8 bInterfaceNumber;
		OUT: interface number of associated interface

	UINT8 bAlternateSetting;
		OUT: alter setting number of associated interface

Description:
	This IOCTL is sent from an OS-specific driver to get the information
	on the interface used for download.

Results:
	No explicit result.  If the IOCTL fails, the error status is
	returned.

Setup Macro:
	VOID USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG_SETUP_V1(
		USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject
		);

	This macro will correctly initialize a .._GET_INFO_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
			pObject;	/* IN: pointer to storage object */
	__TMS_UINT8	StringId;	/* OUT: string descriptor id */
	__TMS_UINT8	bInterfaceNumber;	/* OUT: interface number */
	__TMS_UINT8	bAlternateSetting;	/* OUT: alt setting number */
	};

/* the setup for the .._DOWNLOAD_GET_INFO_ARG struct */
#define __TMS_USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject  = (ARG_pObject);				\
	__pArg->StringId = 0;						\
	__pArg->bInterfaceNumber = 0;					\
	__pArg->bAlternateSetting = 0;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DOWNLOAD_TIMEOUT

Index:	Type:	USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG
	Name:	USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG_SETUP_V1

Function:
	Signal to protocol that the previously started timer has reached
	its timeout value

Input:
	pInParam	points to a USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG
			structure.
	pOutParam	not used

	USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the download function object.

	UINT Id;
		IN: timer id given when started

Description:
	This IOCTL is sent from an OS-specific driver when the timer started
	by the Edge-Ioctl USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR has has reached
	its timeout value

Results:
	No explicit result.  If the IOCTL fails, the error status is
	returned.

Setup Macro:
	VOID USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG_SETUP_V1(
		USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		UINT					Id
		);

	This macro will correctly initialize a .._TIMEOUT_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:
	For details see MCCI DataPump DFU Protocol User's Guide

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
			pObject;	/* IN: pointer to storage object */
	__TMS_UINT	Id;		/* IN: timer id given when started */
	};

/* the setup for the .._DOWNLOAD_TIMEOUT_ARG struct */
#define __TMS_USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Id								\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject  = (ARG_pObject);				\
	__pArg->Id       = (ARG_Id);					\
   	} while (0)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_DOWNLOAD_CLIENT_STATE__INIT	\
   __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE__INIT
# define USBPUMP_IOCTL_EDGE_DOWNLOAD(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD(i, t, NAME)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_READ	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_READ
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART	\
   __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART
# define USBPUMP_DOWNLOAD_CRC_START	\
   __TMS_USBPUMP_DOWNLOAD_CRC_START
# define USBPUMP_IOCTL_DOWNLOAD(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_DOWNLOAD(i, t, NAME)
# define USBPUMP_IOCTL_DOWNLOAD_CALC_CRC	\
   __TMS_USBPUMP_IOCTL_DOWNLOAD_CALC_CRC
# define USBPUMP_IOCTL_DOWNLOAD_GET_INFO	\
   __TMS_USBPUMP_IOCTL_DOWNLOAD_GET_INFO
# define USBPUMP_IOCTL_DOWNLOAD_TIMEOUT	\
   __TMS_USBPUMP_IOCTL_DOWNLOAD_TIMEOUT
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_TmoInMs,							\
	ARG_Id								\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_TmoInMs,							\
	ARG_Id								\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_nBytes,							\
	ARG_pBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_nBytes,							\
	ARG_pBuf							\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_nMaxBytes,							\
	ARG_pBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_nMaxBytes,							\
	ARG_pBuf							\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_DeviceMode							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_DeviceMode							\
	)
# define USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_nBytes,							\
	ARG_CrcAcc							\
	)	\
	__TMS_USBPUMP_IOCTL_DOWNLOAD_CALC_CRC_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_nBytes,							\
	ARG_CrcAcc							\
	)
# define USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject							\
	)	\
	__TMS_USBPUMP_IOCTL_DOWNLOAD_GET_INFO_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject							\
	)
# define USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Id								\
	)	\
	__TMS_USBPUMP_IOCTL_DOWNLOAD_TIMEOUT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Id								\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_download.h ****/
#endif /* _USBIOCTL_DOWNLOAD_H_ */
