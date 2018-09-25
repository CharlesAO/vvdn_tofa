/* umscapi_client.h	Thu Oct 06 2011 13:49:27 chwon */

/*

Module:  umscapi_client.h

Function:
	Defines MSCAPI client data structure

Version:
	V3.01f	Thu Oct 06 2011 13:49:27 chwon	Edit level 10

Copyright notice:
	This file copyright (C) 2004, 2009-2011 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2004

Revision history:
   1.87b  Thu Mar 25 2004 16:11:41  chwon
	Module created.

   1.87b  Tue Apr 20 2004 14:23:10  chwon
	Added umscd_fNotSupportNotify flag.

   1.89b  Wed Aug 25 2004 16:43:16  chwon
   	Bug 1246: Add support for USBPUMP_IOCTL_EDGE_STORAGE_PREVENT_REMOVAL

   1.89b  Tue Aug 31 2004 13:16:11  chwon
   	Add umscdl_RequestData; this is only for data transfer.

   1.89b  Mon Oct 11 2004 23:07:47  chwon
	Add LUN support.

   2.01a  Tue Mar 17 2009 13:10:08  chwon
	2829: move out object name from usbpumpobjectname.h to specific object
	header file. Include protomsc.h file.

   2.01a  Wed Apr 22 2009 16:25:34 ctpeng	
	7119: Add umscdl_fCdromTrayIn.
	
   3.01d  Thu Oct 28 2010 09:40:44  chwon
	11752: Add fReadOnly flag in the LUN structure.

   3.01d  Mon Nov 08 2010 13:39:09  chwon
	11832: Add nDataRequest in the MSCAPI_CLIENT_CONFIG and create new
	INIT and SETUP macros.  Add MSCAPI_DATA_REQUEST structure to handle
	MSCAPI data request.

   3.01f  Thu Oct 06 2011 13:49:27  chwon
	14273: Add umscdl_fReadPending flag in the LUN structure.

*/

#ifndef _UMSCAPI_CLIENT_H_		/* prevent multiple includes */
#define _UMSCAPI_CLIENT_H_

#include "usbpumpenv.h"
#include "usbpumpobject.h"
#include "usbioctl_storage.h"
#include "ucallback.h"

#include "umscapi.h"
#include "protomsc.h"

/*

Type:	MSCAPI_CLIENT_CONFIG

Function:
	Contains configuration information of MSCAPI client

Description:
	This configuration table provides object information and interfaces
	between MSCAPI client and service. The MSCAPI client talk with the
	service using the |MSCAPI_CLIENT_CONFIG|.
	To initialize this structure, we defined |MSCAPI_CLIENT_CONFIG_INIT_V1|,
	|MSCAPI_CLIENT_CONFIG_INIT_V2|, |MSCAPI_CLIENT_CONFIG_INIT_V3| macro.

		const MSCAPI_CLIENT_CONFIG gk_MscApi_ClientConfig =
			MSCAPI_CLIENT_CONFIG_INIT_V1(
				your_mscapi_client_object_tag,
				your_mscapi_client_object_name,
				your_mscapi_work_buffer_size,
				your_mscapi_initialize_function,
				your_mscapi_request_function
				);

		const MSCAPI_CLIENT_CONFIG gk_MscApi_ClientConfig =
			MSCAPI_CLIENT_CONFIG_INIT_V2(
				your_mscapi_client_object_tag,
				your_mscapi_client_object_name,
				your_mscapi_work_buffer_size,
				your_mscapi_initialize_function,
				your_mscapi_request_function,
				your_mscapi_number_of_lun
				);

		const MSCAPI_CLIENT_CONFIG gk_MscApi_ClientConfig =
			MSCAPI_CLIENT_CONFIG_INIT_V3(
				your_mscapi_client_object_tag,
				your_mscapi_client_object_name,
				your_mscapi_work_buffer_size,
				your_mscapi_initialize_function,
				your_mscapi_request_function,
				your_mscapi_number_of_lun,
				your_mscapi_number_of_request
				);

Contents:
	|ObjectTag|
		This is MSCAPI client object tag. You can make this tag using
		UHIL_MEMTAG() macro or any 32bits value.

	|pObjectName|
		This is MSCAPI pointer of client object name. For example, RAM
		disk client object name is "ramdisk.osdriver.mcci.com".

	|nWorkBuffer|
		This is size of MSCAPI client working buffer. MSCAPI require
		buffer for reading or writing blocks. MSCAPI client allocate
		specified size of memory. If nWorkBuffer is 0, the client uses
		default value as UPROTO_MSCBOT_MAX_BURST_SIZE. This is defined
		in protomsc.h and it is 32KB.

	|pInitializeFn|
		This is MSCAPI service initialization function. This function
		returns pointer of MSCAPI_SERVICE strucure, but we just define
		the function that returning void *.

	|pRequestFn|
		This is MSCAPI request function. This function is called by the
		MSCAPI client. When the server complete specified request, call
		the request complete function.

	|nLogicalUnit|
		This is number of logical unit supported by MSCAPI service.

	|nDataRequest|
		This is number of MSCAPI data request blocks.

See Also:
	MSCAPI_REQUEST, MSCAPI_CLIENT_CONFIG_INIT_V1

*/

__TMS_TYPE_DEF_STRUCT(MSCAPI_CLIENT_CONFIG);

struct __TMS_STRUCTNAME(MSCAPI_CLIENT_CONFIG)
	{
	__TMS_UINT32		ObjectTag;	/* Object tag for MSCAPI disk */
	__TMS_CONST __TMS_TEXT *pObjectName;	/* Object name for MSCAPI disk */
	__TMS_UINT32		nWorkBuffer;	/* size of working buffer */

	PMSCAPI_INITIALIZE_FN	pInitializeFn;	/* MSCAPI initialize function */
	PMSCAPI_REQUEST_FN	pRequestFn;	/* MSCAPI request function */

	__TMS_UINT32		nLogicalUnit;	/* number of logical unit */
	__TMS_UINT32		nDataRequest;	/* number of MSCAPI request blocks */
	};

#define	__TMS_MSCAPI_CLIENT_CONFIG_INIT_V3(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn,			\
		nLogicalUnit,			\
		nDataRequest			\
		)				\
	{					\
		(ObjectTag),			\
		(pObjectName),			\
		(nWorkBuffer),			\
		(pInitializeFn),		\
		(pRequestFn),			\
		(nLogicalUnit),			\
		(nDataRequest)			\
	}

#define	__TMS_MSCAPI_CLIENT_CONFIG_INIT_V2(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn,			\
		nLogicalUnit			\
		)				\
	__TMS_MSCAPI_CLIENT_CONFIG_INIT_V3(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn,			\
		nLogicalUnit,			\
		/* V3: nDataRequest */ 2	\
		)

#define	__TMS_MSCAPI_CLIENT_CONFIG_INIT_V1(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn			\
		)				\
	__TMS_MSCAPI_CLIENT_CONFIG_INIT_V2(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn,			\
		/* V2: nLogicalUnit */ 1	\
		)

#define	__TMS_MSCAPI_CLIENT_CONFIG_SETUP_V3(		\
		a_pConfig,				\
		a_ObjectTag,				\
		a_pObjectName,				\
		a_nWorkBuffer,				\
		a_pInitializeFn,			\
		a_pRequestFn,				\
		a_nLogicalUnit,				\
		a_nDataRequest				\
		)					\
    do	{						\
	(a_pConfig)->ObjectTag = (a_ObjectTag);		\
	(a_pConfig)->pObjectName = (a_pObjectName);	\
	(a_pConfig)->nWorkBuffer = (a_nWorkBuffer);	\
	(a_pConfig)->pInitializeFn = (a_pInitializeFn);	\
	(a_pConfig)->pRequestFn = (a_pRequestFn);	\
	(a_pConfig)->nLogicalUnit = (a_nLogicalUnit);	\
	(a_pConfig)->nDataRequest = (a_nDataRequest);	\
	} while (0)


/****************************************************************************\
|
|	Forward type references
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(MSCAPI_OBJECT);
__TMS_TYPE_DEF_STRUCT(MSCAPI_OBJECT_DISK);
__TMS_TYPE_DEF_STRUCT(MSCAPI_OBJECT_DISK_LUN);
__TMS_TYPE_DEF_STRUCT(MSCAPI_DATA_REQUEST);

/****************************************************************************\
|
|	The main instance data
|
\****************************************************************************/

/*
|| Conveniently, we can use one primary data structure for handling
|| the interface to the lower level.  Because of the way things work, we
|| can just record the configuration of the lower layers, and forward things
|| to the appropriate application engine:  either an AT interpreter or
|| an OBEX engine.
||
|| When running in a real OS, this structure will probably be used without
|| typecloaking, so we do it simply.
*/

struct __TMS_STRUCTNAME(MSCAPI_OBJECT)
	{
	/*
	|| For convenience, and so we can recieve IOCTLs, we need
	|| an object header.
	*/
	__TMS_USBPUMP_OBJECT_HEADER	Header;

	/*
	|| We need someplace to store the "object handle"
	*/
	__TMS_PUSBPUMP_OBJECT_HEADER	pIoObject;

	/*
	|| For allocations, we really need a pDevice and a pPlatform
	*/
	__TMS_PUDEVICE			pDevice;
	__TMS_PUPLATFORM		pPlatform;
	};


/****************************************************************************\
|
|	The DISK object structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(MSCAPI_OBJECT_DISK)
	{
	__TMS_MSCAPI_OBJECT		umscd_Object;

	/* Logical Unit Information */
	__TMS_BYTES			umscd_nLun;
	__TMS_PMSCAPI_OBJECT_DISK_LUN	umscd_pLun;

	/* MSCAPI Data request */
	__TMS_BYTES			umscd_nRequests;
	__TMS_PMSCAPI_DATA_REQUEST	umscd_pRequests;

	__TMS_PMSCAPI_DATA_REQUEST	umscd_pFreeList;
	__TMS_PMSCAPI_DATA_REQUEST	umscd_pProtocolPending;
	__TMS_PMSCAPI_DATA_REQUEST	umscd_pServiceReading;
	__TMS_PMSCAPI_DATA_REQUEST	umscd_pServiceWriting;
	__TMS_PMSCAPI_DATA_REQUEST	umscd_pServiceComplete;
	};

struct __TMS_STRUCTNAME(MSCAPI_OBJECT_DISK_LUN)
	{
	__TMS_PMSCAPI_OBJECT_DISK	umscdl_pDisk;

	__TMS_BYTES			umscdl_iLun;

	/* Medium informations */
	__TMS_BYTES			umscdl_LbaMax;
	__TMS_BYTES			umscdl_LbaSize;
	__TMS_BYTES			umscdl_Log2LbaSize;

	/*
	|| In order to bounce into the DataPump task, we need a few
	|| __TMS_UCALLBACKCOMPLETION objects.  We use an 'e' prefix
	|| to help us remember that it is accessed with the DataPump
	|| Event API.
	||
	|| umscdl_evRequest is used to tell the DataPump to complete requested
	|| MSC client request
	||
	|| umscdl_evNotify is used to tell the DataPump to notify media change
	*/
	__TMS_UCALLBACKCOMPLETION	umscdl_evRequest;
	__TMS_UCALLBACKCOMPLETION	umscdl_evNotify;

	/* MSCAPI client request structure */
	MSCAPI_REQUEST			umscdl_Request;

	/* MSCAPI client notification structure */
	MSCAPI_NOTIFY			umscdl_Notify;

	/* MSCAPI request function */
	PMSCAPI_REQUEST_FN		umscdl_pRequestFn;

	/* and some info */
	__TMS_USBPUMP_STORAGE_STATUS	umscdl_WriteStatus;
	__TMS_BYTES			umscdl_LbaStart;
	__TMS_BYTES			umscdl_LbaCount;

	__TMS_UINT32			umscdl_fMediumLoaded: 1;
	__TMS_UINT32			umscdl_fPreventRemoval: 1;
	__TMS_UINT32			umscdl_fCdromTrayIn: 1;
	__TMS_UINT32			umscdl_fReadOnly: 1;
	__TMS_UINT32			umscdl_fMediumChanged: 1;
	__TMS_UINT32			umscdl_fInitialized: 1;
	__TMS_UINT32			umscdl_fNotSupportNotify: 1;
	__TMS_UINT32			umscdl_fNotifyMediumLoaded: 1;
	__TMS_UINT32			umscdl_fOpened: 1;
	__TMS_UINT32			umscdl_fActivated: 1;
	__TMS_UINT32			umscdl_fDoRequestCapacity: 1;
	__TMS_UINT32			umscdl_fDoRequestOpen: 1;
	__TMS_UINT32			umscdl_fDoRequestClose: 1;
	__TMS_UINT32			umscdl_fWritePending: 1;
	__TMS_UINT32			umscdl_fReadMore: 1;
	__TMS_UINT32			umscdl_fReadPending: 1;
	};

struct __TMS_STRUCTNAME(MSCAPI_DATA_REQUEST)
	{
	MSCAPI_REQUEST			Request;

	__TMS_UCALLBACKCOMPLETION	Event;

	__TMS_PVOID			pBuffer;
	__TMS_BYTES			nBuffer;

	__TMS_MSCAPI_DATA_REQUEST *	pNext;
	__TMS_MSCAPI_DATA_REQUEST *	pLast;

	__TMS_UINT			State: 8;
	__TMS_UINT			fPreReading: 1;
	__TMS_UINT			fCancelled: 1;

	__TMS_PMSCAPI_OBJECT_DISK_LUN	pLun;
	__TMS_BYTES			LbaStart;
	__TMS_BYTES			LbaCount;
	};

#define	MSCAPI_DATA_REQUEST_STATE_Free			0
#define	MSCAPI_DATA_REQUEST_STATE_ServiceReading	1
#define	MSCAPI_DATA_REQUEST_STATE_ServiceWriting	2
#define	MSCAPI_DATA_REQUEST_STATE_ProtocolReading	3
#define	MSCAPI_DATA_REQUEST_STATE_ProtocolWriting	4


/****************************************************************************\
|
|	The functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL UsbMscApi_CreateDisk(
		__TMS_PUPLATFORM			pPlatform,
		__TMS_PUSBPUMP_OBJECT_HEADER		pFunctionObject,
		__TMS_CONST __TMS_MSCAPI_CLIENT_CONFIG *pMscApiConfig,
		__TMS_PMSCAPI_OBJECT_DISK *		ppDisk
		);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define MSCAPI_CLIENT_CONFIG_INIT_V3(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn,			\
		nLogicalUnit,			\
		nDataRequest			\
		)	\
	__TMS_MSCAPI_CLIENT_CONFIG_INIT_V3(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn,			\
		nLogicalUnit,			\
		nDataRequest			\
		)
# define MSCAPI_CLIENT_CONFIG_INIT_V2(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn,			\
		nLogicalUnit			\
		)	\
	__TMS_MSCAPI_CLIENT_CONFIG_INIT_V2(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn,			\
		nLogicalUnit			\
		)
# define MSCAPI_CLIENT_CONFIG_INIT_V1(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn			\
		)	\
	__TMS_MSCAPI_CLIENT_CONFIG_INIT_V1(	\
		ObjectTag,			\
		pObjectName,			\
		nWorkBuffer,			\
		pInitializeFn,			\
		pRequestFn			\
		)
# define MSCAPI_CLIENT_CONFIG_SETUP_V3(		\
		a_pConfig,				\
		a_ObjectTag,				\
		a_pObjectName,				\
		a_nWorkBuffer,				\
		a_pInitializeFn,			\
		a_pRequestFn,				\
		a_nLogicalUnit,				\
		a_nDataRequest				\
		)	\
	__TMS_MSCAPI_CLIENT_CONFIG_SETUP_V3(		\
		a_pConfig,				\
		a_ObjectTag,				\
		a_pObjectName,				\
		a_nWorkBuffer,				\
		a_pInitializeFn,			\
		a_pRequestFn,				\
		a_nLogicalUnit,				\
		a_nDataRequest				\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of umscapi_client.h ****/
#endif /* _UMSCAPI_CLIENT_H_ */
