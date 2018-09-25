/* usbioctl_storage.h	Thu May 16 2013 12:49:57 chwon */

/*

Module:  usbioctl_storage.h

Function:
	Abstract interfaces for implementing storage devices.

Version:
	V3.11b	Thu May 16 2013 12:49:57 chwon	Edit level 20

Copyright notice:
	This file copyright (C) 2002-2010, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	August 2002

Revision history:
   1.79b  Wed Aug  7 2002 11:39:40  tmm
	Module created.

   1.79b  Fri Oct 18 2002 12:00:00  maw
	Updated after conversion of Mass-Storage protocol to
	ioctl-style interface

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

   1.87a  Fri Oct  17 2003 09:30:00 maw
	Documentation updated. No code changes

   1.87b  Tue March 23 2004 14:45:00 maw
	Updated LoadorEject structure (and macros) with Load-parameter

   1.89b  Mon Aug 16 2004 14:31:24 maw
   	Added USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL. Updated comments

   1.89b  Tue Aug 31 2004 13:38:02 maw
   	Bug 1299: Added fWriteProtected to USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM

   1.89b  Wed Sep 15 2004 14:45:23 maw
   	Updated with support for LUN's

   1.89c  Wed Oct 13 2004 11:59:48 maw
   	Added comment to ResetDevice that LUN is not used for this Ioctl

   1.91a  Thu Jan 27 2005 12:50:39  chwon
	Use __TMS_CONCAT() instead of ## because this is more portable code.

   1.93b  Tue Nov 1 2005 14:45:23 ohjs
	Add ioctls for supporting TransparentMode
	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND
	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE
	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE
	USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE
	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA
	USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA
	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS
	USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA

   1.93b  Fri Nov 11 2005 10:25:53 ohjs
	Add USBPUMP_STORAGE_STATUS_LUN_NOT_READY

   1.97k  Mon Jan 14 2008 09:51:14  ctpeng
	3577: Modify constant of device type for ATAPI Inquiry command.
	Generate unclaoked name using unclaok-defs.sh

   1.97k  Tue May 20 2008 11:28:45  chwon
	6099: add USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND and
	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS

   1.97k  Wed Dec 10 2008 12:14:30  luxw
	7126: Add USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA,
	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE,
	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE,
	USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA.

   2.01a  Tue Apr 21 2009 15:33:31 ctpeng
	7119: Add USBPUMP_STORAGE_STATUS_NO_MEDIA_TRAYIN

   2.01a  Wed Apr 22 2009 16:02:55 ctpeng
	8265: Add wTag in the following Ioctls for proto UASP:
	USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG
	USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG
	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG
	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG
	USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG
	USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG
	USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG
	Create a new Ioctl for uasp command aborting:
	USBPUMP_IOCTL_STORAGE_REMOVE_TAG_ARG
	Add new status for uasp
	USBPUMP_STORAGE_STATUS_NO_BUFFER

   3.01c  Thu Sep 09 2010 11:00:35 ctpeng
     	11120: Add USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX.

   3.01c  Mon Sep 13 2010 12:10:35  chwon
	11120: Fix uncloaked name generation problem.

   3.11b  Thu May 16 2013 12:49:57  chwon
	17202: Add USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN ioctl.

*/

#ifndef _USBIOCTL_STORAGE_H_		/* prevent multiple includes */
#define _USBIOCTL_STORAGE_H_

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

/****************************************************************************\
|
|	The plane selector type, documented later as part of the MODEM
|	QUEUE_IN IOCTL.
|
\****************************************************************************/

__TMS_TYPE_DEF(USBPUMP_STORAGE_DEVICE_TYPE,  __TMS_UINT8);

#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS		0u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_SEQUENTIAL_ACCESS	1u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_PRINTER		2u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_PROCESSOR		3u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_WRITE_ONCE		4u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_CDROM			5u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_SCANNER		6u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_OPT			7u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_CHARGER		8u
#define	__TMS_USBPUMP_STORAGE_DEVICE_TYPE_COMM			9u


__TMS_TYPE_DEF(USBPUMP_STORAGE_STATUS,  __TMS_UINT8);

#define	__TMS_USBPUMP_STORAGE_STATUS_NONE			0u
#define	__TMS_USBPUMP_STORAGE_STATUS_NO_MEDIA			1u
#define	__TMS_USBPUMP_STORAGE_STATUS_WRITE_ERROR		2u
#define	__TMS_USBPUMP_STORAGE_STATUS_READ_ERROR			3u
#define	__TMS_USBPUMP_STORAGE_STATUS_ILLEGAL_OPCODE		4u
#define	__TMS_USBPUMP_STORAGE_STATUS_ILLEGAL_ADDRESS		5u
#define	__TMS_USBPUMP_STORAGE_STATUS_ILLEGAL_CDB_FIELD		6u
#define	__TMS_USBPUMP_STORAGE_STATUS_BAD_LUN			7u
#define	__TMS_USBPUMP_STORAGE_STATUS_ILLEGAL_REQUEST		8u
#define	__TMS_USBPUMP_STORAGE_STATUS_MEDIA_CHANGE		9u
#define	__TMS_USBPUMP_STORAGE_STATUS_POR_DETECTED		10u
#define	__TMS_USBPUMP_STORAGE_STATUS_LUN_NOT_READY		11u
#define	__TMS_USBPUMP_STORAGE_STATUS_NO_MEDIA_TRAYIN		12u
#define	__TMS_USBPUMP_STORAGE_STATUS_NO_BUFFER			13u


/****************************************************************************\
|
|	The `EDGE' IOCTLs are sent outwards
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_STORAGE_EDGE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_STORAGE_, NAME, _ARG) \
		)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ		\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(0, RW, READ)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(1, RW, READ_DONE)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(2, RW, WRITE)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(3, RW, WRITE_DATA)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(4, RW, GET_STATUS)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(5, RW, RESET_DEVICE)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(6, RW, LOAD_OR_EJECT)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(7, RW, PREVENT_REMOVAL)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(8, RW, CLIENT_COMMAND)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(9, RW, CLIENT_SEND_DONE)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(10, RW, CLIENT_RECEIVE_DONE)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(11, RW, CUSTOM_COMMAND)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(12, RW, CUSTOM_SEND_DONE)
	
#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(13, RW, CUSTOM_RECEIVE_DONE)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(14, RW, REMOVE_TAG)

#define	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE(15, RW, LOAD_OR_EJECT_EX)


/****************************************************************************\
|
|	The inward API functions
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_STORAGE(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_STORAGE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_STORAGE_, NAME, _ARG)	\
		)

#define	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ	\
	__TMS_USBPUMP_IOCTL_STORAGE(0, W, QUEUE_READ)

#define	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE	\
	__TMS_USBPUMP_IOCTL_STORAGE(1, W, QUEUE_WRITE)

#define	__TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE	\
	__TMS_USBPUMP_IOCTL_STORAGE(2, W, WRITE_DONE)

#define	__TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM	\
	__TMS_USBPUMP_IOCTL_STORAGE(3, W, SET_CURRENT_MEDIUM)

#define	__TMS_USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES	\
	__TMS_USBPUMP_IOCTL_STORAGE(4, W, SET_DEVICE_PROPERTIES)

#define	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE	\
	__TMS_USBPUMP_IOCTL_STORAGE(5, RW, CLIENT_SET_MODE)

#define	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA	\
	__TMS_USBPUMP_IOCTL_STORAGE(6, W, CLIENT_SEND_DATA)

#define	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA	\
	__TMS_USBPUMP_IOCTL_STORAGE(7, W, CLIENT_RECEIVE_DATA)

#define	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS	\
	__TMS_USBPUMP_IOCTL_STORAGE(8, W, CLIENT_SEND_STATUS)

#define	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA	\
	__TMS_USBPUMP_IOCTL_STORAGE(9, RW, CLIENT_GET_INQUIRY_DATA)

#define	__TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS	\
	__TMS_USBPUMP_IOCTL_STORAGE(10, W, CUSTOM_SEND_STATUS)
	
#define	__TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA	\
	__TMS_USBPUMP_IOCTL_STORAGE(11, W, CUSTOM_SEND_DATA)	

#define	__TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA	\
	__TMS_USBPUMP_IOCTL_STORAGE(12, W, CUSTOM_RECEIVE_DATA)		

#define	__TMS_USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN	\
	__TMS_USBPUMP_IOCTL_STORAGE(13, W, CONTROL_LAST_LUN)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_READ

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V2

Function:
	Notification IOCTL issued when processing READ

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT16 wTag;
		IN: Command Tag

	BYTES Lba;
		IN: indicates starting LBA index

	BYTES LbaCount;
		IN: indicates number of LBA's to read

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the host wants to read a LBA. The client
	will then issue an USBPUMP_IOCTL_STORAGE_QUEUE_READ back to the
	function that will respond with an USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE
	one the read is done, and then it starts all over again with an
	USBPUMP_IOCTL_EDGE_STORAGE_READ.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG *	pArg,
		VOID *					pClientContext,
		USBPUMP_OBJECT_HEADER *			pObject,
		BYTES					iLun,
		UINT16					wTag,
		BYTES					Lba,
		BYTES					LbaCount
		);

	This macro will correctly initialize a .._READ_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	The client is responsible for processing the Lba and LbaCount argument
	by setting up hw whatever kind is appropriate.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT16 			wTag;		/* Command Tag */
	__TMS_BYTES			Lba;		/* IN: index of first LBA */
	__TMS_BYTES			LbaCount;	/* IN: number of LBA's */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_READ_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Lba,							\
	ARG_LbaCount							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->wTag	       = (ARG_wTag);				\
	__pArg->Lba            = (ARG_Lba);				\
	__pArg->LbaCount       = (ARG_LbaCount);			\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V2(	   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,						   \
	ARG_Lba, 						   \
	ARG_LbaCount 						   \
	)							   \
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V3(		   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,						   \
	0,							   \
	ARG_Lba, 						   \
	ARG_LbaCount 						   \
	)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Lba,							\
	ARG_LbaCount							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V2(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	0,								\
	ARG_Lba,							\
	ARG_LbaCount							\
	)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V2

Function:
	Notification IOCTL issued when processing READ_DONE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT16 wTag;
		IN: Command Tag

	VOID * pBuf;
		IN: indicates pointer to buffer

	BYTES nBytes;
		IN: indicates number of byes read

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent when the buffer provided by the client thru a
	USBPUMP_IOCTL_STORAGE_QUEUE_READ has been read by the host.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG *	pArg,
		VOID *					pClientContext,
		USBPUMP_OBJECT_HEADER *			pObject,
		BYTES					iLun,
		UINT16					wTag,
		VOID *					pBuf,
		BYTES					nBytes
		);

	This macro will correctly initialize a .._READ_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT16 			wTag;		/* Command Tag */
	__TMS_VOID *			pBuf;		/* IN: pointer to read-buffer */
	__TMS_BYTES			nBytes;		/* IN: number of bytes read */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_QUEUE_READ_DONE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_nBytes							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->wTag           = (ARG_wTag);				\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->nBytes         = (ARG_nBytes);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V2(	\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_pBuf,						   \
	ARG_nBytes						   \
	)							   \
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V3(	   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	0,							   \
	ARG_pBuf,						   \
	ARG_nBytes						   \
	)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_pBuf,							\
	ARG_nBytes							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	0,								\
	ARG_pBuf,							\
	ARG_nBytes							\
	)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_WRITE

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V2

Function:
	Notification IOCTL issued when processing WRITE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT16 wTag;
		IN: Command Tag

	BYTES Lba;
		IN: indicates starting LBA index

	BYTES LbaCount;
		IN: indicates number of LBA's to write

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host wants to write a LBA.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG *	pArg,
		VOID *					pClientContext,
		USBPUMP_OBJECT_HEADER *			pObject,
		BYTES					iLun,
		UINT16					wTag,
		BYTES					Lba,
		BYTES					LbaCount
		);

	This macro will correctly initialize a .._WRITE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	The client is responsible for processing the Lba and LbaCount argument
	by setting up hw whatever kind is appropriate.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT16 			wTag;		/* Command Tag */
	__TMS_BYTES			Lba;		/* IN: index of first LBA to write */
	__TMS_BYTES			LbaCount;	/* IN: number of LBA's */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_WRITE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Lba,							\
	ARG_LbaCount							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->wTag           = (ARG_wTag);				\
	__pArg->Lba            = (ARG_Lba);				\
	__pArg->LbaCount       = (ARG_LbaCount);			\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_Lba, 						   \
	ARG_LbaCount 						   \
	)							   \
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V3(		   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	0,							   \
	ARG_Lba, 						   \
	ARG_LbaCount 						   \
	)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Lba,							\
	ARG_LbaCount							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V2(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	0,								\
	ARG_Lba,							\
	ARG_LbaCount							\
	)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V2

Function:
	Notification IOCTL issued when processing WRITE_DATA

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID * pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT16 wTag;
		IN: Command Tag

	VOID * pBuf;
		IN: indicates pointer to buffer

	BYTES nBytes;
		IN: indicates number of bytes to write

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent when the buffer provided by the client thru a
	USBPUMP_IOCTL_STORAGE_QUEUE_WRITE has been written to by the host.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG *	pArg,
		VOID *						pClientContext,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		UINT16 						wTag,
		VOID *						pBuf,
		BYTES						nBytes
		);

	This macro will correctly initialize a .._WRITE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT16 			wTag;		/* Command Tag */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBytes;		/* IN: number of bytes written */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_WRITE_DATA_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_nBytes							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->wTag           = (ARG_wTag);				\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->nBytes         = (ARG_nBytes);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V2(	\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_pBuf,						   \
	ARG_nBytes						   \
	)							   \
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V3( 	   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	0,							   \
	ARG_pBuf,						   \
	ARG_nBytes						   \
	)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_pBuf,							\
	ARG_nBytes							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	0,								\
	ARG_pBuf,							\
	ARG_nBytes							\
	)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V2

Function:
	Notification IOCTL issued when processing GET_STATUS

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	USBPUMP_STORAGE_STATUS Status;
		OUT: client supplied status code

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host wants to get the current status of
	the client.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG *	pArg,
		VOID *						pClientContext,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun
		);

	This macro will correctly initialize a .._GET_STATUS_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Client status */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_GET_STATUS_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	0								\
	)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V2

Function:
	Notification IOCTL issued when processing RESET_DEVICE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15 *** NOT USED ***

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host wants to reset the client.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG *	pArg,
		VOID *						pClientContext,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun
		);

	This macro will correctly initialize a .._GET_STATUS_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_RESET_DEVICE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	0								\
	)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V3

Function:
	Notification IOCTL issued when processing LOAD_OR_EJECT

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	BOOL fLoad;
		IN: set TRUE if load-media request

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host wants to load or eject the client
	medium.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V3(
		USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG *	pArg,
		VOID *						pClientContext,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		BOOL						fLoad
		);

	This macro will correctly initialize a .._LOAD_OR_EJECT_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	The host will only request a toggle of the current status of the client
	load/eject status. The client is responsible for informing the host
	if this IOCTL really caused a change in state using
	USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_BOOL			fLoad;		/* IN: set TRUE if load-media request */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_LOAD_OR_EJECT_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fLoad							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG * __TMS_CONST\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->fLoad          = (ARG_fLoad);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fLoad							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	0,								\
	ARG_fLoad							\
	)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	__TMS_FALSE							\
	)								\

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG_SETUP_V1

Function:
	Notification IOCTL issued when SCSI Start Stop Unit

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT8 PowerConditions;
		IN: Power Conditions bits of SCSI Start Stop Unit

	BOOL fNoflushOrFL;
		IN: NO_FLUSH or FL bit of SCSI Start Stop Unit

	BOOL fLoEj;
		IN: LoEj bit of SCSI Start Stop Unit

	BOOL fStart;
		IN: Start bit of SCSI Start Stop Unit

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host send a SCSI Start Stop command.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG *	pArg,
		VOID *						pClientContext,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		UINT8						PowerConditions,
		BOOL						fNoflushOrFL,
		BOOL						fLoEj,
		BOOL						fStart
		);

	This macro will correctly initialize a .._LOAD_OR_EJECT_EX_ARG 
	structure. If the layout of the argument structure changes in the 
	future, this macro will initialize the new elements in a forward-
	compatible way.

Notes:
	The host might request a toggle of the current status of the client
	load/eject status. The client is responsible for informing the host
	if this IOCTL really caused a change in state using
	USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT8			PowerConditions;/* IN: Power Condition bits*/
	__TMS_BOOL			fNoflushOrFL;	/* IN: NO_FLUSH or FL bit */
	__TMS_BOOL			fLoEj;		/* IN: LoEj bit */
	__TMS_BOOL			fStart;		/* IN: Start bit */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_LOAD_OR_EJECT_EX_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_PowerConditions,						\
	ARG_fNoflushOrFL,						\
	ARG_fLoEj,							\
	ARG_fStart							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pObject        	= (ARG_pObject);			\
	__pArg->pClientContext 	= (ARG_pClientContext);			\
	__pArg->iLun           	= (ARG_iLun);				\
	__pArg->PowerConditions = (ARG_PowerConditions);		\
	__pArg->fNoflushOrFL    = (ARG_fNoflushOrFL);			\
	__pArg->fLoEj          	= (ARG_fLoEj);				\
	__pArg->fStart          = (ARG_fStart);				\
	__pArg->Status         	= __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        	= __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V2

Function:
	Notification IOCTL issued when processing PREVENT_ALLOW_MEDIUM_REMOVAL

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	BOOL fPreventRemoval;
		IN: Set to TRUE if prevent-media-removal request

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host wants to prevent the medium from
	beeing REMOVALd.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG *	pArg,
		VOID *							pClientContext,
		USBPUMP_OBJECT_HEADER *					pObject,
		BYTES							iLun,
		BOOL							fPreventRemoval
		);

	This macro will correctly initialize a .._PREVENT_REMOVAL_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	This is usually used by the host during a write to indicate that there
	are pending directory data that must be written to the medium before
	it can be removed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_BOOL			fPreventRemoval;/* IN: set to TRUE to prevent media removal */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_PREVENT_REMOVAL_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fPreventRemoval						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG * __TMS_CONST\
		__pArg = (pArg);					\
	__pArg->pObject         = (ARG_pObject);			\
	__pArg->pClientContext  = (ARG_pClientContext);			\
	__pArg->iLun            = (ARG_iLun);				\
	__pArg->fPreventRemoval = (ARG_fPreventRemoval);		\
	__pArg->Status          = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject         = __TMS_FALSE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fPreventRemoval						\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	0,								\
	ARG_fPreventRemoval						\
	)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing received CBW

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pCbwBuf;
		IN: Pointer to buffer with data from client

	BYTES nCbwBuf;
		IN: Number of bytes available in buffer

	BOOL fReject;
		OUT: set FALSE if the edge accepts the request, TRUE otherwise.
		If fReject is TRUE, mass storage function will take care of
		current CBW. If fReject is FALSE, current CBW will handled by
		client.

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the host send a CBW.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		BYTES						iLun,
		VOID *						pCbwBuf,
		BYTES						nCbwBuf
		);

	This macro will correctly initialize a .._CLIENT_COMMAND_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
			pObject;	/* IN: pointer to storage object */
	__TMS_VOID *	pClientContext; /* IN: client context */
	__TMS_BYTES	iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT8 *	pCbwBuf;	/* IN: pointer of CBW */
	__TMS_BYTES	nCbwBuf;	/* IN: bytes of CBW */
	__TMS_BOOL	fReject;	/* OUT: set FALSE if the edge
					||  accepts the request, TRUE
					||  otherwise.
					*/
	};
/* the setup for the .._STORAGE_CLIENT_COMMAND_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pCbwBuf,							\
	ARG_nCbwBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->pCbwBuf        = (ARG_pCbwBuf);				\
	__pArg->nCbwBuf        = (ARG_nCbwBuf);				\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing send data to host

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pBuf;
		IN: Pointer to buffer with data from client

	BYTES nBuf;
		IN: Number of bytes sent in buffer

	USBPUMP_STORAGE_STATUS Status;
		OUT: Result of edge operation

	BOOL fReject;
		OUT: set FALSE if the edge accepts the request, TRUE otherwise.

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the mass storage function sent a buffer.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *					pObject,
		VOID *							pClientContext,
		BYTES							iLun,
		VOID *							pBuf,
		BYTES							nBuf
		);

	This macro will correctly initialize a .._CLIENT_SEND_DONE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT8 *			pBuf;		/* IN: pointer of sent buffer */
	__TMS_BYTES			nBuf;		/* IN: sent bytes */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge operation */
	__TMS_BOOL			fReject;	/* OUT: set FALSE if the edge
							||  accepts the request, TRUE
							||  otherwise.
							*/
	};

/* the setup for the .._STORAGE_CLIENT_SEND_DONE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->nBuf           = (ARG_nBuf);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing receive data from

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pBuf;
		IN: Pointer to buffer with data from client

	BYTES nBuf;
		IN: Number of bytes received in buffer

	USBPUMP_STORAGE_STATUS Status;
		OUT: Result of edge operation

	BOOL fReject;
		OUT: set FALSE if the edge accepts the request, TRUE otherwise.

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the mass storage function received a data from host.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *					pObject,
		VOID *							pClientContext,
		BYTES							iLun,
		VOID *							pBuf,
		BYTES							nBuf
		);

	This macro will correctly initialize a .._CLIENT_RECEIVE_DONE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT8 *			pBuf;		/* IN: pointer of sent buffer */
	__TMS_BYTES			nBuf;		/* IN: sent bytes */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge operation */
	__TMS_BOOL			fReject;	/* OUT: set FALSE if the edge
							||  accepts the request, TRUE
							||  otherwise.
							*/
	};
/* the setup for the .._STORAGE_CLIENT_SEND_DONE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->nBuf           = (ARG_nBuf);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing REMOVE_TAG

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15 *** NOT USED ***

	BOOL  fAllTag;	
		IN: remove all tags

	UINT16    wTag;		
		IN: TAG in the Command IU		

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver that has opened/connected to the leaf object.
	It is sent whenever the host wants to remove the request with wTag
	from the client.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG *	pArg,
		VOID *						pClientContext,
		USBPUMP_OBJECT_HEADER *	pObject,
		BYTES						iLun,
		BOOL						fAllTag,
		UINT16						wTag		
		);

	This macro will correctly initialize a .._GET_STATUS_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *
					pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_BOOL			fAllTag;	/* remove all tags */
	__TMS_UINT16    		wTag;		/* TAG in the Command IU */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge-op */
	__TMS_BOOL			fReject;	/* OUT: set TRUE if the edge
							||  accepts the request, FALSE
							||  otherwise.  If possible, will
							||  be used to fail the activate
							||  request on the bus, but this
							||  is not always possible.
							*/
	};

/* the setup for the .._STORAGE_REMOVE_TAG_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fAllTag,							\
	ARG_wTag							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->fAllTag 	   = (ARG_fAllTag); 			\
	__pArg->wTag           = (ARG_wTag);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_QUEUE_READ

Index:	Type:	USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG
	Name:	USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V2

Function:
	Send data from a UBUFQE to the host.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG structure.

	USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT16 wTag;
		IN: Command Tag

	VOID *pBuf;
		IN: Pointer to buffer with data from client medium

	BYTES LbaCount;
		IN: Number of LBA's available in buffer

Description:
	This IOCTL is sent from an OS-specific driver when there is
	read data available to be send to host

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V2(
		USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		BYTES					iLun,
		UINT16					wTag,
		VOID *					pBuf,
		BYTES					LbaCount
		);

	This macro will correctly initialize a .._QUEUE_READ_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT16			wTag;		/* Command Tag */	
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			LbaCount;	/* IN: number of LBA's */
	};

/* the setup for the .._STORAGE_QUEUE_READ_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject  = (ARG_pObject);				\
	__pArg->iLun     = (ARG_iLun);					\
	__pArg->wTag	 = (ARG_wTag);					\
	__pArg->pBuf     = (ARG_pBuf);					\
	__pArg->LbaCount = (ARG_LbaCount);				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V2(		\
	pArg,							   	\
   	ARG_pObject, 						   	\
	ARG_iLun,							\
   	ARG_pBuf,						   	\
   	ARG_LbaCount 						   	\
   	)							   	\
   __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V3(		   	\
   	pArg,							   	\
   	ARG_pObject, 						   	\
   	ARG_iLun							\
   	0,							   	\
   	ARG_pBuf,						   	\
   	ARG_LbaCount 						   	\
   	)

#define __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)								\
   __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V2(			\
	pArg,								\
	ARG_pObject,							\
	0,								\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_QUEUE_WRITE

Index:	Type:	USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG
	Name:	USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V2

Function:
	Post a buffer for host to write data to

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG structure.

	USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT16 wTag;
		IN: Command Tag

	VOID *pBuf;
		IN: Pointer to buffer for host to write data to

	BYTES LbaCount;
		IN: Max number of LBA's that may be written to buffer

Description:
	This IOCTL is sent from an OS-specific driver to submit a buffer
	to the host to write data to

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the buffer was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V2(
		USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		BYTES					iLun,
		UINT16					wTag,
		VOID *					pBuf,
		BYTES					LbaCount
		);

	This macro will correctly initialize a .._QUEUE_WRITE_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT16			wTag;		/* Command Tag */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			LbaCount;	/* IN: number of LBA's */
	};

/* the setup for the .._STORAGE_QUEUE_WRITE_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject  = (ARG_pObject);				\
	__pArg->iLun     = (ARG_iLun);					\
	__pArg->wTag     = (ARG_wTag);					\
	__pArg->pBuf     = (ARG_pBuf);					\
	__pArg->LbaCount = (ARG_LbaCount);				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_iLun, 						   \
	ARG_pBuf,						   \
	ARG_LbaCount 						   \
	)							   \
  __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V3( 	   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_iLun, 						   \
	0,							   \
	ARG_pBuf,						   \
	ARG_LbaCount 						   \
	)

#define __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)								\
   __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	0,								\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_WRITE_DONE

Index:	Type:	USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG
	Name:	USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V2

Function:
	Indication to host with status when client has finished writing
	data to medium

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG structure.

	USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT16 wTag;
		IN: Command Tag

	USBPUMP_STORAGE_STATUS Status;
		IN: status of completed write operation

Description:
	This IOCTL is sent from an OS-specific driver to the host
	with status from medium write operation

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if input parameters were accepted.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V2(
		USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *			pObject,
		BYTES					iLun,
		UINT16					wTag,
		USBPUMP_STORAGE_STATUS			Status
		);

	This macro will correctly initialize a .._WRITE_DONE_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT16			wTag;		/* Command Tag */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* IN: Storage-status */
	};

/* the setup for the .._STORAGE_WRITE_DONE_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Status							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->iLun    = (ARG_iLun);					\
	__pArg->wTag    = (ARG_wTag);					\
	__pArg->Status  = (ARG_Status);					\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_iLun, 						   \
	ARG_Status						   \
	)							   \
  __TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V3(		   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_iLun, 						   \
	0,							   \
	ARG_Status						   \
	)

#define __TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Status							\
	)								\
   __TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V2(			\
	pArg,								\
	ARG_pObject,							\
	0,								\
	ARG_Status							\
	)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM

Index:	Type:	USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG
	Name:	USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V3

Function:
	Set properties of current medium.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG structure.

	USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	BOOL fPresent;
		IN: indicates whether medium is present or not

	BOOL fWriteProtected;
		IN: indicates whether medium is writeprotected or not

	BYTES nLba;
		IN: Number of LBA's on medium.

	BYTES LbaSize;
		IN: number of bytes per LBA

Description:
	This IOCTL is sent from an OS-specific driver to set properties
	of current medium

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if input parameters were accepted.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V3(
		USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		BOOL						fPresent,
		BOOL						fWriteProtected,
		BYTES						nLba,
		BYTES						LbaSize
		);

	This macro will correctly initialize a .._SET_CURRENT_MEDIUM_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_BOOL			fPresent;	/* IN: indicates whether medium is present or not */
	__TMS_BOOL			fWriteProtected;/* IN: indicates whether medium is writeprotected or not*/
	__TMS_BYTES			nLba;		/* IN: Number LBA's on current medium */
	__TMS_BYTES			LbaSize;	/* IN: Size of each LBA */
	};

/* the setup for the .._STORAGE_SET_CURRENT_MEDIUM_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_fPresent,							\
	ARG_fWriteProtected,						\
	ARG_nLba,							\
	ARG_LbaSize							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pObject         = (ARG_pObject);			\
	__pArg->iLun            = (ARG_iLun);				\
	__pArg->fPresent        = (ARG_fPresent);			\
	__pArg->fWriteProtected = (ARG_fWriteProtected);		\
	__pArg->nLba            = (ARG_nLba);				\
	__pArg->LbaSize         = (ARG_LbaSize);			\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fPresent,							\
	ARG_fWriteProtected,						\
	ARG_nLba,							\
	ARG_LbaSize							\
	)								\
   __TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	0,								\
	ARG_fPresent,							\
	ARG_fWriteProtected,						\
	ARG_nLba,							\
	ARG_LbaSize							\
	)

#define __TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fPresent,							\
	ARG_nLba,							\
	ARG_LbaSize							\
	)								\
   __TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_fPresent,							\
	__TMS_FALSE,							\
	ARG_nLba,							\
	ARG_LbaSize							\
	)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES

Index:	Type:	USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG
	Name:	USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V2

Function:
	Set mass-storage device properties

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG structure.

	USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	USBPUMP_STORAGE_DEVICE_TYPE DeviceType;
		IN: specfifies the type of device

	BOOL fRemovable;
		IN: specifies whether the device may be removed or not

	CONST TEXT * pVendorId;

	CONST TEXT * pProductId;

	CONST TEXT * pVersion;

Description:
	This IOCTL is sent from an OS-specific driver to set charateristics
	of mass-storage device

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if input parameters were accepted.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V2(
		USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *					pObject,
		BYTES							iLun,
		USBPUMP_STORAGE_DEVICE_TYPE				DeviceType,
		BOOL							fRemovable,
		CONST TEXT * 						pVendorId,
		CONST TEXT *						pProductId,
		CONST TEXT *						pVersion
		);

	This macro will correctly initialize a .._SET_DEVICE_PROPERTIES_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_USBPUMP_STORAGE_DEVICE_TYPE
					DeviceType;	/* IN: specfifies the type of device */
	__TMS_BOOL			fRemovable;	/* IN: specifies whether the device may be removed or not */
	__TMS_CONST __TMS_TEXT *	pVendorId;	/* IN: vendor id string */
	__TMS_CONST __TMS_TEXT *	pProductId;	/* IN: product id string */
	__TMS_CONST __TMS_TEXT *	pVersion;	/* IN: version string */
	};

/* the setup for the .._STORAGE_SET_DEVICE_PROPERTIES_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_DeviceType,							\
	ARG_fRemovable,							\
	ARG_pVendorId,							\
	ARG_pProductId,							\
	ARG_pVersion							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pObject         = (ARG_pObject);			\
	__pArg->iLun            = (ARG_iLun);				\
	__pArg->DeviceType      = (ARG_DeviceType);			\
	__pArg->fRemovable      = (ARG_fRemovable);			\
	__pArg->pVendorId       = (ARG_pVendorId);			\
	__pArg->pProductId      = (ARG_pProductId);			\
	__pArg->pVersion        = (ARG_pVersion);			\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_DeviceType,							\
	ARG_fRemovable,							\
	ARG_pVendorId,							\
	ARG_pProductId,							\
	ARG_pVersion							\
	)								\
   __TMS_USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	0,								\
	ARG_DeviceType,							\
	ARG_fRemovable,							\
	ARG_pVendorId,							\
	ARG_pProductId,							\
	ARG_pVersion							\
	)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE

Index:	Type:	USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG_SETUP_V1

Function:
	Request IOCTL issued by client to get CBW

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG structure.

	USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	BYTES iLun;
		IN: LUN index 0 .. 15

	BOOL  fEnableTransparentMode;
		IN: Current Status

	BOOL fOldMode;
		OUT: Old Status

Description:
	This IOCTL is enable/disable to read CBW.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG_SETUP_V2(
		USBPUMP_IOCTL_STORAGE_CLIENT_GET_COMMAND_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		BOOL						fEnableTransparentMode
		);

	This macro will correctly initialize a .._CLIENT_SET_MODE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_BOOL			fEnableTransparentMode;
	__TMS_BOOL			fOldMode;
	};

/* the setup for the .._STORAGE_CLIENT_SET_MODE_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_fEnableTransparentMode					\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject                = (ARG_pObject);			\
	__pArg->iLun                   = (ARG_iLun);			\
	__pArg->fEnableTransparentMode = (ARG_fEnableTransparentMode);	\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA

Index:	Type:	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG_SETUP_V1

Function:
	Send data from a buffer to the host.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG structure.

	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pBuf;
		IN: Pointer to buffer with data from client

	BYTES nBuf;
		IN: Number of bytes available in buffer

Description:
	This IOCTL is sent from an OS-specific driver when there is
	read data available to be send to host

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG_SETUP_V1(
		USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		VOID *						pBuf,
		BYTES						nBuf
		);

	This macro will correctly initialize a .._CLIENT_SEND_DATA_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBuf;		/* IN: number of bytes */
	};

/* the setup for the .._STORAGE_CLIENT_SEND_DATA_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject                = (ARG_pObject);			\
	__pArg->iLun                   = (ARG_iLun);			\
	__pArg->pBuf 	               = (ARG_pBuf);			\
	__pArg->nBuf                   = (ARG_nBuf);			\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA

Index:	Type:	USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG_SETUP_V1

Function:
	Receive data from the host.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG structure.

	USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pBuf;
		IN: Pointer to buffer with data from client

	BYTES nBuf;
		IN: Number of bytes available in buffer

Description:
	This IOCTL is sent from an OS-specific driver when there is
	write data available to be send to host

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG_SETUP_V1(
		USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		VOID *						pBuf,
		BYTES						nBuf
		);

	This macro will correctly initialize a .._CLIENT_RECEIVE_DATA_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBuf;		/* IN: number of bytes */
	};

/* the setup for the .._STORAGE_CLIENT_RECEIVE_DATA_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject                = (ARG_pObject);			\
	__pArg->iLun                   = (ARG_iLun);			\
	__pArg->pBuf 	               = (ARG_pBuf);			\
	__pArg->nBuf                   = (ARG_nBuf);			\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS

Index:	Type:	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG_SETUP_V1

Function:
	Send status of CSW to the host.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG structure.

	USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT8 CswStatus;
		IN: Status of CSW

	USBPUMP_STORAGE_STATUS StorageStatus;
		IN: Status code of USBPUMP_STORAGE_STATUS

Description:
	This IOCTL is sent from an OS-specific driver when there is
	CSW's status data available to be send to host

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG_SETUP_V1(
		USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		UINT8						CswStatus,
		USBPUMP_STORAGE_STATUS				StorageStatus
		);

	This macro will correctly initialize a .._CLIENT_RECEIVE_DATA_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_UINT8			CswStatus;	/* IN: Status of CSW */
	__TMS_USBPUMP_STORAGE_STATUS	StorageStatus;	/* IN: status of sense */
	};

/* the setup for the .._STORAGE_CLIENT_SEND_STATUS_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_CswStatus,							\
	ARG_StorageStatus						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject                = (ARG_pObject);			\
	__pArg->iLun                   = (ARG_iLun);			\
	__pArg->CswStatus 	       = (ARG_CswStatus);		\
	__pArg->StorageStatus          = (ARG_StorageStatus);		\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA

Index:	Type:	USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG_SETUP_V1

Function:
	Send status of CSW to the host.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG structure.

	USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pBuf;
		IN: pointer of inquiry buffer

	BYTES nBuf;
		IN: size of inquiry buffer

Description:
	This IOCTL is sent from an OS-specific driver when there is
	CSW's status data available to be send to host

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG_SETUP_V1(
		USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		VOID *						pBuf,
		BYTES						nBuf
		);

	This macro will correctly initialize a .._CLIENT_RECEIVE_DATA_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBuf;		/* IN: number of bytes */
	__TMS_BYTES			WriteCount;	/* OUT: number of writed bytes */
	};

/* the setup for the .._STORAGE_CLIENT_GET_INQUIRY_DATA_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject                = (ARG_pObject);			\
	__pArg->iLun                   = (ARG_iLun);			\
	__pArg->pBuf 	               = (ARG_pBuf);			\
	__pArg->nBuf                   = (ARG_nBuf);			\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing received custom CBW

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	CONST UINT8 *pCbwcbBuffer;
		IN: Pointer to CBWCB buffer from host

	BYTES nCbwcbBuffer;
		IN: The valid length of the CBWCB in bytes.
		
	UINT32 DataTransferLength;
		IN: The number of bytes of data that host expects to send/receive
		during the execution of this command.
		
   	BOOL    fDataTransferFromDeviceToHost; 
   		IN: Direction of data transfer. This field is valid only when 
   		DataTransferLength is not zero. 
   		TRUE: Data-In
   		FALSE: Data-Out

	BOOL fReject;
		OUT: set FALSE if the edge accepts the request, TRUE otherwise.
		If fReject is TRUE, mass storage function will take care of
		current CBW. If fReject is FALSE, current CBW will handled by
		client and client should send status using
		USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS ioctl.

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the host send a custom specific CBW.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		VOID *						pClientContext,
		BYTES						iLun,
		CONST VOID *					pCbwcbBuffer,
		BYTES						nCbwcbBuffer,
		UINT32    					DataTransferLength;
   		BOOL      					fDataTransferFromDeviceToHost; 
		);

	This macro will correctly initialize a .._CUSTOM_COMMAND_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;
	__TMS_VOID *			pClientContext;
	__TMS_BYTES			iLun;
	__TMS_CONST __TMS_UINT8 *	pCbwcbBuffer;
	__TMS_BYTES			nCbwcbBuffer;
	__TMS_UINT32    		DataTransferLength;
   	__TMS_BOOL      		fDataTransferFromDeviceToHost; 
	__TMS_BOOL			fReject;
	};

/* the setup for the .._STORAGE_CUSTOM_COMMAND_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pObject,							\
	a_pClientContext,						\
	a_iLun,								\
	a_pCbwcbBuffer,							\
	a_nCbwcbBuffer,							\
	a_DataTransferLength,						\
	a_fDataTransferFromDeviceToHost					\
	)								\
   do	{								\
	(a_pArg)->pObject        = (a_pObject);				\
	(a_pArg)->pClientContext = (a_pClientContext);			\
	(a_pArg)->iLun           = (a_iLun);				\
	(a_pArg)->pCbwcbBuffer   = (a_pCbwcbBuffer);			\
	(a_pArg)->nCbwcbBuffer   = (a_nCbwcbBuffer);			\
	(a_pArg)->DataTransferLength = (a_DataTransferLength);		\
	(a_pArg)->fDataTransferFromDeviceToHost = (a_fDataTransferFromDeviceToHost);\
	(a_pArg)->fReject        = __TMS_TRUE;				\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_pClientContext,						\
	a_iLun,								\
	a_pCbwcbBuffer,							\
	a_nCbwcbBuffer							\
	)								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pObject,							\
	a_pClientContext,						\
	a_iLun,								\
	a_pCbwcbBuffer,							\
	a_nCbwcbBuffer,							\
	0,								\
	__TMS_FALSE							\
	)					  	

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing sending data to host

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pBuf;
		IN: Pointer to buffer with data from client

	BYTES nBuf;
		IN: Number of bytes sent in buffer

	USBPUMP_STORAGE_STATUS Status;
		OUT: Result of edge operation

	BOOL fReject;
		OUT: set FALSE if the edge accepts the request, TRUE otherwise.

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the mass storage function sent a buffer.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *					pObject,
		VOID *							pClientContext,
		BYTES							iLun,
		VOID *							pBuf,
		BYTES							nBuf
		);

	This macro will correctly initialize a .._CUSTOM_SEND_DONE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_VOID *			pBuf;		/* IN: pointer of sent buffer */
	__TMS_BYTES			nBuf;		/* IN: sent bytes */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge operation */
	__TMS_BOOL			fReject;	/* OUT: set FALSE if the edge
							||  accepts the request, TRUE
							||  otherwise.
							*/
	};

/* the setup for the .._STORAGE_CUSTOM_SEND_DONE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->nBuf           = (ARG_nBuf);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_TRUE;				\
   	} while (0)   	
   	

/*

IOCTL:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE

Index:	Type:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG
	Name:	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing receiving data from the host

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG structure.

	USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pBuf;
		IN: Pointer to buffer with data from host

	BYTES nBuf;
		IN: Number of bytes received in buffer

	USBPUMP_STORAGE_STATUS Status;
		OUT: Result of edge operation

	BOOL fReject;
		OUT: set FALSE if the edge accepts the request, TRUE otherwise.

Description:
	This IOCTL is sent from a storage function to the OS-specific
	driver (client) that has opened/connected to the leaf object.
	It is sent whenever the mass storage function received a data from host.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course. Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *					pObject,
		VOID *							pClientContext,
		BYTES							iLun,
		VOID *							pBuf,
		BYTES							nBuf
		);

	This macro will correctly initialize a .._CUSTOM_RECEIVE_DONE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_VOID *			pClientContext; /* IN: client context */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_VOID *			pBuf;		/* IN: pointer of sent buffer */
	__TMS_BYTES			nBuf;		/* IN: sent bytes */
	__TMS_USBPUMP_STORAGE_STATUS	Status;		/* OUT: Result of edge operation */
	__TMS_BOOL			fReject;	/* OUT: set FALSE if the edge
							||  accepts the request, TRUE
							||  otherwise.
							*/
	};
/* the setup for the .._STORAGE_CUSTOM_SEND_DONE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject        = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pClientContext);			\
	__pArg->iLun           = (ARG_iLun);				\
	__pArg->pBuf           = (ARG_pBuf);				\
	__pArg->nBuf           = (ARG_nBuf);				\
	__pArg->Status         = __TMS_USBPUMP_STORAGE_STATUS_NONE;	\
	__pArg->fReject        = __TMS_TRUE;				\
   	} while (0)
   	   	

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS

Index:	Type:	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG_SETUP_V1

Function:
	Send status of CSW to the host.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG structure.

	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	UINT8 bCswStatus;
		IN: This indicates the success or failure of the command.
		The client shall set this byte to zero if the command completed
		successfully. A non-zero value shall indicate a failure during
		command execution.

	USBPUMP_STORAGE_STATUS StorageStatus;
		IN: Status code of USBPUMP_STORAGE_STATUS

Description:
	This IOCTL is sent from an OS-specific driver when there is
	CSW's status data available to be send to host

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG_SETUP_V1(
		USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		UINT8						bCswStatus,
		USBPUMP_STORAGE_STATUS				StorageStatus
		);

	This macro will correctly initialize a .._CUSTOM_RECEIVE_DATA_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;
	__TMS_BYTES			iLun;
	__TMS_UINT8			bCswStatus;
	__TMS_USBPUMP_STORAGE_STATUS	StorageStatus;
	};

/* the setup for the .._STORAGE_CUSTOM_SEND_STATUS_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_iLun,								\
	a_bCswStatus,							\
	a_StorageStatus							\
	)								\
   do	{								\
	(a_pArg)->pObject       = (a_pObject);				\
	(a_pArg)->iLun          = (a_iLun);				\
	(a_pArg)->bCswStatus    = (a_bCswStatus);			\
	(a_pArg)->StorageStatus = (a_StorageStatus);			\
   	} while (0)


/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA

Index:	Type:	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG_SETUP_V1

Function:
	Send data to the host in Custom SCSI Command Data phase.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG structure.

	USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15
		
	VOID *	pBuf;
		IN: The data will be sent to the host.
		
	UINT32	nBuf;
		IN: The size of data to be transferred.

Description:
	This IOCTL is sent from an OS-specific driver when there is
	Data-IN stage available to be sent to host

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG_SETUP_V1(
		USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		VOID *						pBuf
		UINT32						nBuf
		);

	This macro will correctly initialize a .._CUSTOM_SEND_DATA_ARG
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;
	__TMS_BYTES			iLun;
	__TMS_VOID *			pBuf;
	__TMS_UINT32			nBuf;
	};

/* the setup for the .._STORAGE_CUSTOM_SEND_DATA_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_iLun,								\
	a_pBuf,								\
	a_nBuf								\
	)								\
   do	{								\
	(a_pArg)->pObject       = (a_pObject);				\
	(a_pArg)->iLun          = (a_iLun);				\
	(a_pArg)->pBuf		= (a_pBuf);				\
	(a_pArg)->nBuf		= (a_nBuf);				\
   	} while (0)


/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA

Index:	Type:	USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG_SETUP_V1

Function:
	Receive data from the host in Custom SCSI command data phase.

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG structure.

	USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BYTES iLun;
		IN: LUN index 0 .. 15

	VOID *pBuf;
		IN: Pointer to buffer with data from client

	BYTES nBuf;
		IN: Number of bytes available in buffer

Description:
	This IOCTL is sent from an OS-specific driver when there is
	write data available to be send to host

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the data was accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG_SETUP_V1(
		USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BYTES						iLun,
		VOID *						pBuf,
		BYTES						nBuf
		);

	This macro will correctly initialize a .._CUSTOM_RECEIVE_DATA_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: pointer to storage object */
	__TMS_BYTES			iLun;		/* IN: LUN index 0 .. 15 */
	__TMS_VOID *			pBuf;		/* IN: pointer to buffer */
	__TMS_BYTES			nBuf;		/* IN: number of bytes */
	};

/* the setup for the .._STORAGE_CUSTOM_RECEIVE_DATA_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG * __TMS_CONST 	\
		__pArg = (pArg);					\
	__pArg->pObject                = (ARG_pObject);			\
	__pArg->iLun                   = (ARG_iLun);			\
	__pArg->pBuf 	               = (ARG_pBuf);			\
	__pArg->nBuf                   = (ARG_nBuf);			\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN

Index:	Type:	USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG
	Name:	USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG_SETUP_V1

Function:
	Enable / disable last LUN of the multiple LUNs

Input:
	pInParam	points to a USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG
			structure.
	pOutParam	NULL.

	USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the mass storage function object.

	BOOL fEnableLastLun;
		IN: if this fEnableLastLun is FALSE, the mass storage protocol
		will not show up the last LUN to the host machine.  If this
		flag is TRUE, it will show up all LUNs.

Description:
	This IOCTL is sent from an OS-specific driver if the client want to
	disable or enable last LUN operation.  If the device has only one LUN,
	this IOCTL returns USBPUMP_IOCTL_RESULT_INVALID_PARAMETER error code.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if mass storage protocol driver disable
	or enable last LUN successfully.
	USBPUMP_IOCTL_RESULT_INVALID_PARAMETER if mass storage protocol driver
	has only one LUN.  Some other result if the IOCTL is not implemented.

Setup Macro:
	VOID USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG_SETUP_V1(
		USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG *	pArg,
		USBPUMP_OBJECT_HEADER *				pObject,
		BOOL						fEnableLastLun
		);

	This macro will correctly initialize a .._CONTROL_LAST_LUN_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in
	a forward-compatible way.

Notes:

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObject;	/* IN: storage object */
	__TMS_BOOL			fEnableLastLun;	/* IN: control */
	};

/* the setup for the .._STORAGE_CONTROL_LAST_LUN_ARG struct */
#define __TMS_USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fEnableLastLun						\
	)								\
   do	{								\
	(pArg)->pObject = (ARG_pObject);				\
	(pArg)->fEnableLastLun = (ARG_fEnableLastLun);			\
   	} while (0)


/****************************************************************************\
|
|	Uncloaked Names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_DIRECT_ACCESS
# define USBPUMP_STORAGE_DEVICE_TYPE_SEQUENTIAL_ACCESS	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_SEQUENTIAL_ACCESS
# define USBPUMP_STORAGE_DEVICE_TYPE_PRINTER	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_PRINTER
# define USBPUMP_STORAGE_DEVICE_TYPE_PROCESSOR	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_PROCESSOR
# define USBPUMP_STORAGE_DEVICE_TYPE_WRITE_ONCE	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_WRITE_ONCE
# define USBPUMP_STORAGE_DEVICE_TYPE_CDROM	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_CDROM
# define USBPUMP_STORAGE_DEVICE_TYPE_SCANNER	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_SCANNER
# define USBPUMP_STORAGE_DEVICE_TYPE_OPT	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_OPT
# define USBPUMP_STORAGE_DEVICE_TYPE_CHARGER	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_CHARGER
# define USBPUMP_STORAGE_DEVICE_TYPE_COMM	\
   __TMS_USBPUMP_STORAGE_DEVICE_TYPE_COMM
# define USBPUMP_STORAGE_STATUS_NONE	\
   __TMS_USBPUMP_STORAGE_STATUS_NONE
# define USBPUMP_STORAGE_STATUS_NO_MEDIA	\
   __TMS_USBPUMP_STORAGE_STATUS_NO_MEDIA
# define USBPUMP_STORAGE_STATUS_WRITE_ERROR	\
   __TMS_USBPUMP_STORAGE_STATUS_WRITE_ERROR
# define USBPUMP_STORAGE_STATUS_READ_ERROR	\
   __TMS_USBPUMP_STORAGE_STATUS_READ_ERROR
# define USBPUMP_STORAGE_STATUS_ILLEGAL_OPCODE	\
   __TMS_USBPUMP_STORAGE_STATUS_ILLEGAL_OPCODE
# define USBPUMP_STORAGE_STATUS_ILLEGAL_ADDRESS	\
   __TMS_USBPUMP_STORAGE_STATUS_ILLEGAL_ADDRESS
# define USBPUMP_STORAGE_STATUS_ILLEGAL_CDB_FIELD	\
   __TMS_USBPUMP_STORAGE_STATUS_ILLEGAL_CDB_FIELD
# define USBPUMP_STORAGE_STATUS_BAD_LUN	\
   __TMS_USBPUMP_STORAGE_STATUS_BAD_LUN
# define USBPUMP_STORAGE_STATUS_ILLEGAL_REQUEST	\
   __TMS_USBPUMP_STORAGE_STATUS_ILLEGAL_REQUEST
# define USBPUMP_STORAGE_STATUS_MEDIA_CHANGE	\
   __TMS_USBPUMP_STORAGE_STATUS_MEDIA_CHANGE
# define USBPUMP_STORAGE_STATUS_POR_DETECTED	\
   __TMS_USBPUMP_STORAGE_STATUS_POR_DETECTED
# define USBPUMP_STORAGE_STATUS_LUN_NOT_READY	\
   __TMS_USBPUMP_STORAGE_STATUS_LUN_NOT_READY
# define USBPUMP_STORAGE_STATUS_NO_MEDIA_TRAYIN	\
   __TMS_USBPUMP_STORAGE_STATUS_NO_MEDIA_TRAYIN
# define USBPUMP_STORAGE_STATUS_NO_BUFFER	\
   __TMS_USBPUMP_STORAGE_STATUS_NO_BUFFER
# define USBPUMP_IOCTL_EDGE_STORAGE(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE(i, t, NAME)
# define USBPUMP_IOCTL_EDGE_STORAGE_READ	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ
# define USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE
# define USBPUMP_IOCTL_EDGE_STORAGE_WRITE	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE
# define USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA
# define USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS
# define USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE
# define USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT
# define USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL
# define USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND
# define USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE
# define USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE
# define USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND
# define USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE
# define USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE
# define USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG
# define USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX	\
   __TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX
# define USBPUMP_IOCTL_STORAGE(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_STORAGE(i, t, NAME)
# define USBPUMP_IOCTL_STORAGE_QUEUE_READ	\
   __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ
# define USBPUMP_IOCTL_STORAGE_QUEUE_WRITE	\
   __TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE
# define USBPUMP_IOCTL_STORAGE_WRITE_DONE	\
   __TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE
# define USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM	\
   __TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM
# define USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES	\
   __TMS_USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES
# define USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE	\
   __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE
# define USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA	\
   __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA
# define USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA	\
   __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA
# define USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS	\
   __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS
# define USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA	\
   __TMS_USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA
# define USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS	\
   __TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS
# define USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA	\
   __TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA
# define USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA	\
   __TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA
# define USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN	\
   __TMS_USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN
# define USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Lba,							\
	ARG_LbaCount							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Lba,							\
	ARG_LbaCount							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V2(	   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,						   \
	ARG_Lba, 						   \
	ARG_LbaCount 						   \
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V2(	   \
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,						   \
	ARG_Lba, 						   \
	ARG_LbaCount 						   \
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Lba,							\
	ARG_LbaCount							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Lba,							\
	ARG_LbaCount							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_nBytes							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_nBytes							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V2(	\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_pBuf,						   \
	ARG_nBytes						   \
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V2(	\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_pBuf,						   \
	ARG_nBytes						   \
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_pBuf,							\
	ARG_nBytes							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_pBuf,							\
	ARG_nBytes							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Lba,							\
	ARG_LbaCount							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Lba,							\
	ARG_LbaCount							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_Lba, 						   \
	ARG_LbaCount 						   \
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_Lba, 						   \
	ARG_LbaCount 						   \
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Lba,							\
	ARG_LbaCount							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_Lba,							\
	ARG_LbaCount							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_nBytes							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_nBytes							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V2(	\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_pBuf,						   \
	ARG_nBytes						   \
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V2(	\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_pClientContext,					   \
	ARG_iLun,					   	   \
	ARG_pBuf,						   \
	ARG_nBytes						   \
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_pBuf,							\
	ARG_nBytes							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_WRITE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_pBuf,							\
	ARG_nBytes							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_GET_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_RESET_DEVICE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fLoad							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fLoad							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fLoad							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fLoad							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext						\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_PowerConditions,						\
	ARG_fNoflushOrFL,						\
	ARG_fLoEj,							\
	ARG_fStart							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_LOAD_OR_EJECT_EX_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_PowerConditions,						\
	ARG_fNoflushOrFL,						\
	ARG_fLoEj,							\
	ARG_fStart							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fPreventRemoval						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fPreventRemoval						\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fPreventRemoval						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_fPreventRemoval						\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pCbwBuf,							\
	ARG_nCbwBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_COMMAND_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pCbwBuf,							\
	ARG_nCbwBuf							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_SEND_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CLIENT_RECEIVE_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fAllTag,							\
	ARG_wTag							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_REMOVE_TAG_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_fAllTag,							\
	ARG_wTag							\
	)
# define USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)
# define USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V2(		\
	pArg,							   	\
   	ARG_pObject, 						   	\
	ARG_iLun,							\
   	ARG_pBuf,						   	\
   	ARG_LbaCount 						   	\
   	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V2(		\
	pArg,							   	\
   	ARG_pObject, 						   	\
	ARG_iLun,							\
   	ARG_pBuf,						   	\
   	ARG_LbaCount 						   	\
   	)
# define USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_READ_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)
# define USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)
# define USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_iLun, 						   \
	ARG_pBuf,						   \
	ARG_LbaCount 						   \
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_iLun, 						   \
	ARG_pBuf,						   \
	ARG_LbaCount 						   \
	)
# define USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_QUEUE_WRITE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pBuf,							\
	ARG_LbaCount							\
	)
# define USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Status							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V3(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_wTag,							\
	ARG_Status							\
	)
# define USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_iLun, 						   \
	ARG_Status						   \
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V2(		\
	pArg,							   \
	ARG_pObject, 						   \
	ARG_iLun, 						   \
	ARG_Status						   \
	)
# define USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Status							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_WRITE_DONE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Status							\
	)
# define USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_fPresent,							\
	ARG_fWriteProtected,						\
	ARG_nLba,							\
	ARG_LbaSize							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V3(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_fPresent,							\
	ARG_fWriteProtected,						\
	ARG_nLba,							\
	ARG_LbaSize							\
	)
# define USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fPresent,							\
	ARG_fWriteProtected,						\
	ARG_nLba,							\
	ARG_LbaSize							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fPresent,							\
	ARG_fWriteProtected,						\
	ARG_nLba,							\
	ARG_LbaSize							\
	)
# define USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fPresent,							\
	ARG_nLba,							\
	ARG_LbaSize							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_SET_CURRENT_MEDIUM_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fPresent,							\
	ARG_nLba,							\
	ARG_LbaSize							\
	)
# define USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_DeviceType,							\
	ARG_fRemovable,							\
	ARG_pVendorId,							\
	ARG_pProductId,							\
	ARG_pVersion							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V2(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_DeviceType,							\
	ARG_fRemovable,							\
	ARG_pVendorId,							\
	ARG_pProductId,							\
	ARG_pVersion							\
	)
# define USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_DeviceType,							\
	ARG_fRemovable,							\
	ARG_pVendorId,							\
	ARG_pProductId,							\
	ARG_pVersion							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_SET_DEVICE_PROPERTIES_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_DeviceType,							\
	ARG_fRemovable,							\
	ARG_pVendorId,							\
	ARG_pProductId,							\
	ARG_pVersion							\
	)
# define USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_fEnableTransparentMode					\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SET_MODE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_fEnableTransparentMode					\
	)
# define USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)
# define USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_RECEIVE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)
# define USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_CswStatus,							\
	ARG_StorageStatus						\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_SEND_STATUS_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_CswStatus,							\
	ARG_StorageStatus						\
	)
# define USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CLIENT_GET_INQUIRY_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pObject,							\
	a_pClientContext,						\
	a_iLun,								\
	a_pCbwcbBuffer,							\
	a_nCbwcbBuffer,							\
	a_DataTransferLength,						\
	a_fDataTransferFromDeviceToHost					\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pObject,							\
	a_pClientContext,						\
	a_iLun,								\
	a_pCbwcbBuffer,							\
	a_nCbwcbBuffer,							\
	a_DataTransferLength,						\
	a_fDataTransferFromDeviceToHost					\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_pClientContext,						\
	a_iLun,								\
	a_pCbwcbBuffer,							\
	a_nCbwcbBuffer							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_COMMAND_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_pClientContext,						\
	a_iLun,								\
	a_pCbwcbBuffer,							\
	a_nCbwcbBuffer							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_SEND_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)
# define USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_STORAGE_CUSTOM_RECEIVE_DONE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientContext,						\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)
# define USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_iLun,								\
	a_bCswStatus,							\
	a_StorageStatus							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_iLun,								\
	a_bCswStatus,							\
	a_StorageStatus							\
	)
# define USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_iLun,								\
	a_pBuf,								\
	a_nBuf								\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_SEND_DATA_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObject,							\
	a_iLun,								\
	a_pBuf,								\
	a_nBuf								\
	)
# define USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CUSTOM_RECEIVE_DATA_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_iLun,							\
	ARG_pBuf,							\
	ARG_nBuf							\
	)
# define USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fEnableLastLun						\
	)	\
	__TMS_USBPUMP_IOCTL_STORAGE_CONTROL_LAST_LUN_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_fEnableLastLun						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_storage.h ****/
#endif /* _USBIOCTL_STORAGE_H_ */
