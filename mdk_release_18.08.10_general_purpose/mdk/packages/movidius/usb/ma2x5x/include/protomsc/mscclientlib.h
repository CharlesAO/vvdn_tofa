/* mscclientlib.h	Wed Sep 15 2004 14:45:23 maw */

/*

Module:  mscclientlib.h

Function:
	Prototypes for use by upper layer clients (os client drivers and
	the like).

Version:
	V1.89b  Wed Sep 15 2004 14:45:23 maw	Edit level 4

Copyright notice:
	This file copyright (C) 2002-2004 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjörn, MCCI Corporation	October 2002

Revision history:
   1.79b  Fri Oct 18 2002 12:00:00  maw
	Module created.

   1.87a  Fri Mar 05 2004 12:14:15  chwon
	Correct type cloaking.
  	
   1.89b  Mon Sep 06 2004 16:42:21 maw
   	Bug 1299: MscStorageClientLib_SetCurrentMediumV2()

   1.89b  Wed Sep 15 2004 14:45:23 maw
   	Added API functions with LUN parameter
*/

#ifndef _MSCCLIENTLIB_H_		/* prevent multiple includes */
#define _MSCCLIENTLIB_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBIOCTL_STORAGE_H_
# include "usbioctl_storage.h"
#endif

/****************************************************************************\
|
|	The client library functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_QueueRead(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_VOID *			pBuf,
	__TMS_BYTES			LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_QueueReadV2(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BYTES			iLun,
	__TMS_VOID *			pBuf,
	__TMS_BYTES			LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_QueueWrite(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_VOID *			pBuf,
	__TMS_BYTES			LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_QueueWriteV2(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BYTES			iLun,
	__TMS_VOID *			pBuf,
	__TMS_BYTES			LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_WriteDone(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_USBPUMP_STORAGE_STATUS	Status
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_WriteDoneV2(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BYTES			iLun,
	__TMS_USBPUMP_STORAGE_STATUS	Status
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_SetCurrentMedium(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BOOL			fPresent,
	__TMS_BYTES			nLba,
	__TMS_BYTES			LbaSize
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_SetCurrentMediumV2(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BOOL			fPresent,
	__TMS_BOOL			fWriteProtected,
	__TMS_BYTES			nLba,
	__TMS_BYTES			LbaSize
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_SetCurrentMediumV3(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BYTES			iLun,
	__TMS_BOOL			fPresent,
	__TMS_BOOL			fWriteProtected,
	__TMS_BYTES			nLba,
	__TMS_BYTES			LbaSize
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_SetDeviceProperties(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_USBPUMP_STORAGE_DEVICE_TYPE	DeviceType,
	__TMS_BOOL				fRemovable,
	__TMS_CONST __TMS_TEXT *		pVendorId,
	__TMS_CONST __TMS_TEXT *		pProductId,
	__TMS_CONST __TMS_TEXT *		pVersion
	);

__TMS_USBPUMP_IOCTL_RESULT
MscStorageClientLib_SetDevicePropertiesV2(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_USBPUMP_STORAGE_DEVICE_TYPE	DeviceType,
	__TMS_BOOL				fRemovable,
	__TMS_CONST __TMS_TEXT *		pVendorId,
	__TMS_CONST __TMS_TEXT *		pProductId,
	__TMS_CONST __TMS_TEXT *		pVersion
	);

__TMS_END_DECLS

/**** end of mscclientlib.h ****/
#endif /* _MSCCLIENTLIB_H_ */
