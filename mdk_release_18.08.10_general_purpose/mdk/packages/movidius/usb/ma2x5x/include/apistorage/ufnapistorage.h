/* ufnapistorage.h	Thu May 16 2013 12:59:58 chwon */

/*

Module:  ufnapistorage.h

Function:
	Prototypes for use by upper layer clients (os client drivers and
	the like).

Version:
	V3.11b	Thu May 16 2013 12:59:58 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2004-2009, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Mats Webjorn, MCCI Corporation	March 2004

Revision history:
   1.87b  Wed Mar 24 2004 08:15:07 maw
	Module created from proto/msc/i/uclientstorage.h 1.87a

   1.89b  Mon Aug 16 2004 14:31:24 maw
   	Function description corrected

   1.89b  Tue Aug 31 2004 13:38:02 maw
   	Bug 1299: Added UsbFnApiStorage_SetCurrentMediumV2()
	
   1.89b  Wed Sep 15 2004 14:45:23 maw
   	Added API functions with LUN parameter
   	
   1.93b  Tue Nov 1 2005 14:45:23 ohjs
   	ADD API function for SD-JukeBox
	
   1.97k  Tue May 20 2008 16:08:41  chwon
	6099: add UsbFnApiStorage_CustomSendStatus()

   1.97k  Wed Dec 10 2008 15:07:56  luxw
	7126: Add UsbFnApiStorage_CustomSendData and 
	UsbFnApiStorage_CustomReceiveData

   2.01a  Mon Mar 09 2009 16:02:55 ctpeng
   	8265: Added API functions with wTag parameter for UASP

   3.11b  Thu May 16 2013 12:59:58  chwon
	17202: Add UsbFnApiStorage_ControlLastLun() prototype.

*/

#ifndef _UFNAPISTORAGE_H_		/* prevent multiple includes */
#define _UFNAPISTORAGE_H_

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
UsbFnApiStorage_QueueRead(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_QueueReadV2(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_QueueReadV3(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_UINT16 				wTag,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_QueueWrite(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_QueueWriteV2(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_QueueWriteV3(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_UINT16 				wTag,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				LbaCount
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_WriteDone(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_USBPUMP_STORAGE_STATUS		Status
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_WriteDoneV2(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_USBPUMP_STORAGE_STATUS		Status
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_WriteDoneV3(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_UINT16 				wTag,
	__TMS_USBPUMP_STORAGE_STATUS		Status
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_SetCurrentMedium(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BOOL				fPresent,
	__TMS_BYTES				nLba,
	__TMS_BYTES				LbaSize
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_SetCurrentMediumV2(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BOOL				fPresent,
	__TMS_BOOL				fWriteProtected,
	__TMS_BYTES				nLba,
	__TMS_BYTES				LbaSize
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_SetCurrentMediumV3(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_BOOL				fPresent,
	__TMS_BOOL				fWriteProtected,
	__TMS_BYTES				nLba,
	__TMS_BYTES				LbaSize
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_SetDeviceProperties(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_USBPUMP_STORAGE_DEVICE_TYPE	DeviceType,
	__TMS_BOOL				fRemovable,
	__TMS_CONST __TMS_TEXT *		pVendorId,
	__TMS_CONST __TMS_TEXT *		pProductId,
	__TMS_CONST __TMS_TEXT *		pVersion
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_SetDevicePropertiesV2(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_USBPUMP_STORAGE_DEVICE_TYPE	DeviceType,
	__TMS_BOOL				fRemovable,
	__TMS_CONST __TMS_TEXT *		pVendorId,
	__TMS_CONST __TMS_TEXT *		pProductId,
	__TMS_CONST __TMS_TEXT *		pVersion
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_ClientSetMode(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_BOOL				fEnableTransparentMode,
	__TMS_BOOL *				fOldMode
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_ClientSendData(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				nBuf
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_ClientReceiveData(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				nBuf
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_ClientSendStatus(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_UINT8				CswStatus,
	__TMS_USBPUMP_STORAGE_STATUS		StorageStatus
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_ClientGetInquiryData(
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_VOID *				pBuf,
	__TMS_BYTES				nBuf,
	__TMS_BYTES *				pWriteCount
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_CustomSendStatus(		/* 6099 */
	__TMS_USBPUMP_OBJECT_HEADER *		pIoObject,
	__TMS_BYTES				iLun,
	__TMS_UINT8				bCswStatus,
	__TMS_USBPUMP_STORAGE_STATUS		StorageStatus
	);
	
__TMS_USBPUMP_IOCTL_RESULT			
UsbFnApiStorage_CustomSendData(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BYTES			iLun,
	__TMS_VOID *			pBuf,
	__TMS_UINT32			nBuf
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_CustomReceiveData(		
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BYTES			iLun,
	__TMS_VOID *			pBuf,
	__TMS_BYTES			nBuf
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiStorage_ControlLastLun(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_BOOL			fEnableLastLun
	);

__TMS_END_DECLS

/**** end of ufnapistorage.h ****/
#endif /* _UFNAPISTORAGE_H_ */
