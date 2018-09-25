/* usbpump_usbdi_api.h	Fri May 19 2017 12:03:41 chwon */

/*

Module:  usbpump_usbdi_api.h

Function:
	API functions for USBDI clients.

Version:
	V3.21a	Fri May 19 2017 12:03:41 chwon	Edit level 17

Copyright notice:
	This file copyright (C) 2005-2011, 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Sat Aug 13 2005 18:37:33  tmm
	Module created.

   1.97f  Fri Jan 12 2007 13:17:32  chwon
	2246: add gk_UsbPumpUsbdImplementation_Minimal_HighSpeed prototype

   1.97j  Fri Dec 21 2007 09:49:56  chwon
	3293: fix template and change company name

   1.97k  Wed Mar 05 2008 13:31:45  chwon
	3567: add USBD annunciator GUID and object name

   1.97k  Fri Jul 11 2008 13:51:16  chwon
	4286: use USBPUMP_USBDI_USBD_INIT_FN

   1.97k  Fri Aug 15 2008 13:37:50  chwon
	UDP-4312: add UsbPumpUsbdPort_GetUrbSize()
	UDP-6361: add UsbPumpUsbd_GetUrbSize()
	UDP-6190: add gk_UsbPumpUsbdiUsbdImplementation_Minimal and
	gk_UsbPumpUsbdiUsbdImplementation_Isoch.

   2.01a  Fri Nov 20 2009 13:30:04  chwon
	9463: add gk_UsbPumpUsbdiUsbdImplementation_Stream.

   3.01c  Tue Aug 10 2010 18:42:18  chwon
	10911: add UsbPumpUsbdPort_InitializeInstanceInfo() prototype.

   3.01c  Fri Aug 13 2010 18:11:55  chwon
	10859: add UsbPumpUsbdPort_UnbindFunction() prototype.

   3.01d  Tue Jan 04 2011 13:19:02  chwon
	12293: add UsbPumpUsbdPort_GetPortKey() and
	UsbPumpUsbdPort_GetDefaultPipeHandle() prototype.

   3.01d  Sun Mar 06 2011 22:33:51  chwon
	12831: add UsbPumpUsbdPort_GetBindFunction() prototype.

   3.01f  Tue May 03 2011 14:28:05  chwon
	13335: add UsbPumpUsbdPort_InstanceDeparture() prototype.

   3.01f  Fri Jun 03 2011 11:38:14  chwon
	13458: 13266: add UsbPumpUsbdPort_CancelAllRequests() prototype.

   3.01f  Fri Jun 24 2011 23:17:20  chwon
	13723: add fSuspendPortAfterUnbind in UsbPumpUsbdPort_UnbindFunction().

   3.19a  Fri Oct 07 2016 14:51:03  chwon
	20321: add UsbPumpUsbdPort_CleanUpConfig() prototype.

   3.21a  Fri May 19 2017 12:03:41  chwon
	20563: Add UsbPumpUsbdiLib_ListUsbDevice() prototype.

*/

#ifndef _USBPUMP_USBDI_API_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_API_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USBUUID_H_
# include "usbuuid.h"
#endif

#ifndef _USBDEBUG_H_
# include "usbdebug.h"
#endif

/****************************************************************************\
|
|	USBDI Annunciator GUID definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_ANNUNCIATOR_OBSERVER_GUID_INIT()	\
	__TMS_USBUUID_INIT(0x52ad5103, 0x703e, 0x4f2f,		\
		0xb2, 0x66, 0xc1, 0x59, 0x9f, 0x0e, 0x11, 0x66)

#define	__TMS_USBPUMP_USBDI_ANNUNCIATOR_SENDER_GUID_INIT()	\
	__TMS_USBUUID_INIT(0xe3ed9439, 0xb5ec, 0x438e,		\
		0xb6, 0xec, 0x34, 0x32, 0x16, 0xc0, 0xf9, 0x07)

#define	__TMS_USBPUMP_USBDI_ANNUNCIATOR_OBJECT_NAME		\
	__TMS_USBPUMP_ANNUNCIATOR_OBJECT_NAME("usbdi")


/****************************************************************************\
|
|	Functional API to USBDI components.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UINT8
UsbPumpUsbdPipe_GetEndpointAddress(
	__TMS_USBPUMP_USBDI_PIPE_HANDLE
	);

__TMS_UINT16
UsbPumpUsbdPort_CalculateStandardTimeout(
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT *	/*pPort*/,
	__TMS_CONST __TMS_VOID *		/*pBuffer*/,
	__TMS_BYTES				/*nBuffer*/
	);

__TMS_UINT
UsbPumpUsbdPort_CancelAllRequests(
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT *	/* IN: pPort */
	);

__TMS_VOID
UsbPumpUsbdPort_CleanUpConfig(
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */,
	__TMS_USBPUMP_URB *		/* pUrb */,
	__TMS_USBPUMP_URB_DONE_FN *	/* pDoneFn */,
	__TMS_VOID *			/* pDoneInfo */
	);

__TMS_BYTES
UsbPumpUsbdPort_GetUrbSize(		/* UDP-4312 */
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */,
	__TMS_BYTES			/* BaseUrbSize */
	);

__TMS_VOID
UsbPumpUsbdPort_InitializeInstanceInfo(
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT *	/* IN: pPort */,
	__TMS_USBPUMP_USBDI_INSTANCE_INFO *	/* OUT: pInstanceInfo */
	);

__TMS_USBPUMP_USBDI_PORT_KEY
UsbPumpUsbdPort_GetPortKey(
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT *	/* IN: pPort */
	);

__TMS_USBPUMP_USBDI_PIPE_HANDLE
UsbPumpUsbdPort_GetDefaultPipeHandle(
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT *	/* IN: pPort */
	);

__TMS_USBPUMP_USBDI_FUNCTION *
UsbPumpUsbdPort_GetBindFunction(
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT *	/* IN: pPort */
	);

__TMS_VOID
UsbPumpUsbdPort_InstanceDeparture(
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT *	/* IN: pPort */
	);

__TMS_VOID
UsbPumpUsbdPort_UnbindFunction(
	__TMS_CONST __TMS_USBPUMP_USBDI_PORT *	/* IN: pPort */,
	__TMS_BOOL				/* IN: fSuspendPortAfterUnbind */
	);

__TMS_USBPUMP_USBDI_USBD_INIT_FN	/* 4286 */
UsbPumpUsbd_Initialize;

__TMS_BYTES
UsbPumpUsbd_GetUrbSize(			/* UDP-6361 */
	__TMS_USBPUMP_USBDI_USBD *	/* pUsbd */
	);

__TMS_USBPUMP_USBDI_DRIVER_CLASS *
UsbPumpUsbdiLib_CreateDriverClass(
	__TMS_USBPUMP_OBJECT_HEADER * /*pParent*/,
	__TMS_CONST __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG * /*pConfig*/,
	__TMS_CONST __TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION *
						/*pImplementation*/
	);

__TMS_VOID
UsbPumpUsbdiLib_ListUsbDevice(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_TTUSB_VPRINTF_FN *	/* pPrintFn */,
	__TMS_VOID *			/* pPrintCtx */
	);

/*
|| UDP-6190:
||	Add isochronous transfer support. Create new USBD implementation with
||	naming convention -- gk_UsbPumpUsbdiUsbdImplementation_xxx.
||	But keep old names for backward compatibility.
*/
extern __TMS_CONST __TMS_USBPUMP_USBDI_USBD_IMPLEMENTATION
gk_UsbPumpUsbdImplementation_Minimal;

extern __TMS_CONST __TMS_USBPUMP_USBDI_USBD_IMPLEMENTATION
gk_UsbPumpUsbdImplementation_Minimal_HighSpeed;

extern __TMS_CONST __TMS_USBPUMP_USBDI_USBD_IMPLEMENTATION
gk_UsbPumpUsbdiUsbdImplementation_Minimal;

extern __TMS_CONST __TMS_USBPUMP_USBDI_USBD_IMPLEMENTATION
gk_UsbPumpUsbdiUsbdImplementation_Isoch;

extern __TMS_CONST __TMS_USBPUMP_USBDI_USBD_IMPLEMENTATION
gk_UsbPumpUsbdiUsbdImplementation_Stream;

/* Default linkage for USBDI Annunciator GUID */
extern __TMS_CONST __TMS_USBGUID	gk_UsbPumpUsbdAnnunciator_ObserverGuid;
extern __TMS_CONST __TMS_USBGUID	gk_UsbPumpUsbdAnnunciator_SenderGuid;

/* Default linkage for USBDI annunciator object name */
extern __TMS_CONST __TMS_TEXT		gk_UsbPumpUsbdAnnunciator_ObjectName[];

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_ANNUNCIATOR_OBSERVER_GUID_INIT()	\
   __TMS_USBPUMP_USBDI_ANNUNCIATOR_OBSERVER_GUID_INIT()
# define USBPUMP_USBDI_ANNUNCIATOR_SENDER_GUID_INIT()	\
   __TMS_USBPUMP_USBDI_ANNUNCIATOR_SENDER_GUID_INIT()
# define USBPUMP_USBDI_ANNUNCIATOR_OBJECT_NAME	\
   __TMS_USBPUMP_USBDI_ANNUNCIATOR_OBJECT_NAME
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_api.h ****/
#endif /* _USBPUMP_USBDI_API_H_ */
