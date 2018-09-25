/* ufnapidownload.h	Tue Jun 07 2011 22:27:58 chwon */

/*

Module:  ufnapidownload.h

Function:
	Prototypes for use by upper layer clients to download class protocols 
	(os client drivers and the like).

Version:
	V3.01f	Tue Jun 07 2011 22:27:58 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2004, 2011 by

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
  	
   3.01f  Tue Jun 07 2011 22:27:58  chwon
	11353: add UsbFnApiDownload_GetInfoV2() prototype.

*/

#ifndef _UFNAPIDOWNLOAD_H_		/* prevent multiple includes */
#define _UFNAPIDOWNLOAD_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBIOCTL_DOWNLOAD_H_
# include "usbioctl_download.h"
#endif

/****************************************************************************\
|
|	The client library functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiDownload_CalcCrc(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_CONST __TMS_UCHAR *	pBuf,		/* data to calc crc on */
	__TMS_BYTES 			nBytes,		/* number of bytes */
	__TMS_UINT32 *			pAcc		/* pointer to start/end value of crc-accumulator */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiDownload_GetInfo(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_UINT8 *			pStringId
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiDownload_GetInfoV2(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_UINT8 *			pStringId,
	__TMS_UINT8 *			pInterfaceNumber,
	__TMS_UINT8 *			pAltSettingNumber
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbFnApiDownload_Timeout(
	__TMS_USBPUMP_OBJECT_HEADER *	pIoObject,
	__TMS_UINT32			Id		/* timer id set when started */
	);

__TMS_END_DECLS

/**** end of ufnapidownload.h ****/
#endif /* _UFNAPIDOWNLOAD_H_ */
