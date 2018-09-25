/* protondfu.h  Mon Jun  1 2015 00:49:32 tmm */

/*

Module:  protondfu.h

Function:
	The definitions for the standard USB DFU protocol on top of
	the MCCI DataPump.

Version:
	V3.15b	Mon Jun  1 2015 00:49:32 tmm	Edit level 6

Copyright notice:
	This file copyright (C) 2004-2006, 2009-2010, 2015 by

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
	Module created from protodfu.h V1.79a

   1.97d  Wed Jun 21 2006 14:33:34  maw
	Added callback context  	

   2.01a  Mon Mar 16 2009 13:46:07  chwon
	2829: move out object name from usbpumpobjectname.h to here.
	Generate uncloaked name using uncloak-defs.sh.

   3.01d  Thu Oct 28 2010 13:49:20  maw
   	11310: Add DataPump timer struct to context

   3.15b  Thu May 21 2015 16:24:35  chwon
	19147: Remove udfu_GetStatusReply in the context.

   3.15b  Mon Jun  1 2015 00:49:32  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _PROTONDFU_H_        /* prevent multiple includes */
#define _PROTONDFU_H_

#ifndef _USBPUMP_H_
# include "usbpump.h"
#endif

#ifndef _USBPUMPOBJECTNAME_H_
# include "usbpumpobjectname.h"
#endif

#ifndef _USBPROTOINIT_H_
# include "usbprotoinit.h"
#endif

#ifndef _UHILERRCODES_H_
# include "uhilerrcodes.h"
#endif

#ifndef _USBIOCTL_DOWNLOAD_H_
# include "usbioctl_download.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

#ifndef _NDFUCFG_H_
# include "ndfucfg.h"
#endif

/****************************************************************************\
|
|	Forward type declarations.
|
\****************************************************************************/

/****************************************************************************\
|
|	Macro's.
|
\****************************************************************************/

#define __TMS_UNDFU_DEFAULT_GETSTATUS_POLL_TMO	10    /* 10 ms */

#define __TMS_UNDFU_STATEHANDLER(p, s) ((p)->udfu_pConfig->pStateSwitch->State[s]->Handler)(p)
#define __TMS_UNDFU_INITSTATE(p, s)    ((p)->udfu_pConfig->pStateSwitch->State[s]->Init)(p)

#define __TMS_NDFU_EDGE_ACCEPT		__TMS_FALSE
#define __TMS_NDFU_EDGE_REJECT		__TMS_TRUE

/****************************************************************************\
|
|	Here are software-check codes used by the DFU
|
\****************************************************************************/

#define __TMS_DFUSWC_BASE	__TMS_UHILSWC_DFUBASE
#define __TMS_DFUSWC_NODESC	(__TMS_DFUSWC_BASE | 1)

/****************************************************************************\
|
|	Some useful data structures for DFU work.
|
\****************************************************************************/

struct __TMS_STRUCTNAME(UPROTO_NDFU)
	{
	/*
	|| Each object begins with a header, inlcuding a tag and a size.
	*/
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	/*
	|| Each subclass instance can (normally) be opened by an OS-specific
	|| driver.  This is where we put the info about the client
	|| If the object pointer is NULL, then we have no client object.
	*/
	__TMS_USBPUMP_OBJECT_HEADER *		udfu_pClientObject;
	__TMS_VOID *				udfu_pClientContext;		/* client context */
	__TMS_UDEVICE	*			udfu_pDevice;			/* the device */
	__TMS_UINTERFACE *			udfu_pInterface;

	__TMS_CONST __TMS_UPROTO_NDFU_CONFIG *	udfu_pConfig;
	__TMS_UEVENTNODE			udfu_InterfaceEventNode;	/* the DFU ifc event node */
	__TMS_UCHAR				udfu_iInterface;		/* string index for DFU interface string desc */

	__TMS_UINT				udfu_Flags;			/* TRUE if we're up */

	__TMS_UCHAR				udfu_State;
	__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE	udfu_ClientState;
	__TMS_UCHAR				udfu_Status;
	__TMS_UCHAR				udfu_iString;			/* string index for vendor specific err */
	__TMS_USETUP				udfu_LastSetup;			/* the last setup packet
										|| we received; useful for
										|| deferred processing
										|| of SET commands.
										*/
	__TMS_UBUFQE				udfu_Qe;
	__TMS_USHORT				udfu_LastLogReq;

	__TMS_ULONG				udfu_NextStatusPollTmo;		/* next GetStatus poll tmo */

	__TMS_UENDPOINT *			udfu_pBulkRxEp;			/* our (optional) bulk-in endpoint */
	__TMS_UENDPOINT *			udfu_pBulkTxEp;			/* our (optional) bulk-out endpoint */
	
	__TMS_VOID *				udfu_pDataBuf;			/* pointer to the data buffer */
	__TMS_USHORT				udfu_usDataSize;		/* size of the data buffer */
	__TMS_BOOL				udfu_DataPending;		/* TRUE if there is data pending */

	__TMS_UCALLBACKCOMPLETION		udfu_pEdgeCallback_sync;	/* callback fn for synchronous edge-calls */

	__TMS_USBPUMP_TIMER			udfu_DetachTimer;

	/*
	|| Storage for DFU functional descriptor settings
	*/
	__TMS_BOOL				udfu_bitWillDetach;
	__TMS_BOOL				udfu_bitManifestationTolerant;
	__TMS_BOOL				udfu_bitCanUpload;
	__TMS_BOOL				udfu_bitCanDownload;
	__TMS_USHORT				udfu_wDetachTmo;		/* time in ms the device will wait for a reset */
	__TMS_USHORT				udfu_wTransferSize;		/* wTransferSize in DFU descriptor */
	};

/*
|| The flags for this protocol.
*/
#define	__TMS_UNDFU_FLAG_UP		(1 << 0)	/* it's up */

#define	__TMS_UPROTO_NDFU_TAG		__TMS_UHIL_MEMTAG('D', 'F', 'U', '2')

/* this expands to something like download.ndfu.fn.mcci.com */
/* remember, you'll normally match on "download.*.fn.mcci.com", or even "download.*.fn.*" */
#define	__TMS_UPROTO_NDFU_NAME	\
	__TMS_USBPUMP_OBJECT_NAME_FN("download", "ndfu" )

/* backward compatibility for PR2829 */
#ifndef	__TMS_USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD
# define __TMS_USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD	\
	__TMS_USBPUMP_OBJECT_NAME_ENUM_FN("download")
#endif

/****************************************************************************\
|
|	Here are the functions
|
\****************************************************************************/

__BEGIN_DECLS

__TMS_USBPUMP_PROTOCOL_CREATE_FN	NDfu_ProtocolCreate;

__TMS_USBPUMP_PROTOCOL_PROBE_FN		NDfu_ProtocolProbe;

__END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UNDFU_DEFAULT_GETSTATUS_POLL_TMO	\
   __TMS_UNDFU_DEFAULT_GETSTATUS_POLL_TMO
# define UNDFU_STATEHANDLER(p, s)	\
   __TMS_UNDFU_STATEHANDLER(p, s)
# define UNDFU_INITSTATE(p, s)	\
   __TMS_UNDFU_INITSTATE(p, s)
#define NDFU_EDGE_ACCEPT	\
  __TMS_NDFU_EDGE_ACCEPT
#define NDFU_EDGE_REJECT	\
  __TMS_NDFU_EDGE_REJECT
# define DFUSWC_BASE	\
   __TMS_DFUSWC_BASE
# define DFUSWC_NODESC	\
   __TMS_DFUSWC_NODESC
# define UNDFU_FLAG_UP	\
   __TMS_UNDFU_FLAG_UP
# define UPROTO_NDFU_TAG	\
   __TMS_UPROTO_NDFU_TAG
# define UPROTO_NDFU_NAME	\
   __TMS_UPROTO_NDFU_NAME
# define USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD	\
   __TMS_USBPUMP_OBJECT_NAME_ENUM_DOWNLOAD
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of protondfu.h ****/
#endif /* _PROTONDFU_H_ */
