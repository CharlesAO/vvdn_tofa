/* usbhid10.h	Sat May 23 1998 17:22:17  tmm */

/*

Module:  usbhid10.h

Function:
	Definitions for the USB-IF HID device class (wire-level things).

Version:
	V1.00a	Sat May 23 1998 17:22:17	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 1998 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 1998

Revision history:
   1.00a  Sat May 23 1998 17:22:17  tmm
	Module created.

*/

#ifndef _USBHID10_H_		/* prevent multiple includes */
#define _USBHID10_H_

#ifndef _USBIF_H_
# include "usbif.h"
#endif

/****************************************************************************\
|
|	Class, subclass and protocol codes.
|
\****************************************************************************/

/* class code */
#define	USB_bInterfaceClass_HID			3	/* the hid class */

/* subclass */
#define	USB_bInterfaceSubClass_HID_None		0	/* no subclass */
#define	USB_bInterfaceSubClass_HID_Boot		1	/* boot interface */

/* protocols */
#define	USB_bInterfaceProtocol_HID_None		0	/* no protocol */
#define	USB_bInterfaceProtocol_HID_Keyboard	1	/* keyboard boot proto */
#define	USB_bInterfaceProtocol_HID_Mouse	2	/* mouse boot proto */

/****************************************************************************\
|
|	Requests
|
\****************************************************************************/

#define	USB_bRequest_HID_GET_REPORT	0x01	/* get report */
#define	USB_bRequest_HID_GET_IDLE	0x02	/* get idle (optional) */
#define	USB_bRequest_HID_GET_PROTOCOL	0x03	/* get protocol (boot only) */
#define	USB_bRequest_HID_SET_REPORT	0x09	/* set report (optional) */
#define	USB_bRequest_HID_SET_IDLE	0x0A	/* set idle (optional) */
#define	USB_bRequest_HID_SET_PROTOCOL	0x0B	/* set protocol (boot only) */

/* GET/SET reports use wValue high/low to encdoe report type/id */
#define	USB_HID_wValueToReportId(wValue)	((UCHAR) ((wValue) & 0xFF))
#define	USB_HID_wValueToReportType(wValue)	((UCHAR) ((wValue) >> 8))
#define	USB_HID_ReportTypeIdToWValue(Type, Id) \
	((USHORT) ( ((Type) << 8) | (Id)))

#define	USB_HID_ReportType_NULL		0x00
#define	USB_HID_ReportType_Input	0x01
#define	USB_HID_ReportType_Output	0x02
#define	USB_HID_ReportType_Feature	0x03

#define	USB_HID_ReportId_NULL		0x00

/* 
|| SET IDLE carries polling info in the high byte of wValue; GET IDLE
|| returns it in the data byte.
*/
#define	USB_HID_wValueToDuration(wValue)	((UCHAR) ((wValue) >> 8))
#define	USB_HID_DurationReportIdToWValue(Duration, Id) \
	((USHORT) ( ((Duration) << 8) | (Id)))

#define USB_HID_Duration_Indefinite	0x00	/* report changes only */

#define	USB_HID_DurationToMillisec(Duration)	\
		((Duration) << 2)
#define USB_HID_MillisecToDuration(Millisec)	\
		(((Millisec) + 3) >> 2)

/*
|| GET/SET PROTOCOL -- carried in wValue on output, data on input.
*/
#define	USB_HID_Protocol_Boot		0x00	/* select boot protocol */
#define	USB_HID_Protocol_Report		0x01	/* select report protocol */
#define	USB_HID_Protocol_MAXIMUM	0x02	/* no values larger than this,
						|| please.
						*/

/****************************************************************************\
|
|	Descriptors
|
\****************************************************************************/

#define	USB_bDescType_HID		0x21	/* base HID descriptor */
#define	USB_bDescType_HID_REPORT	0x22	/* HID report descriptor */
#define	USB_bDescType_HID_PHYSICAL	0x23	/* HID physical descriptor */

/****************************************************************************\
|
|	HID Report-descriptor Items
|
\****************************************************************************/

#define	USB_HIDITEM_LONG_TAG		0xFE

#define	USB_HIDITEM_SHORT_bSize(tag)	((tag) & 0x3)
#define	USB_HIDITEM_SHORT_bType(tag)	(((tag) >> 2) & 0x3)
#define	USB_HIDITEM_SHORT_bTag(tag)	(((tag) >> 4) & 0xF)

#define	USB_HIDITEM_PLONG_bSize(pTag)	(((PUCHAR)(pTag))[1])
#define	USB_HIDITEM_PLONG_bTag(pTag)	(((PUCHAR)(pTag))[2])

#define	USB_HIDITEM_BuildShortTag(Tag, Type, Size)	\
	((UCHAR) (((Size) & 0x3) | (((Type) & 0x3) << 2) | (((Tag) & 0xF) << 4) ))

/**** item types ****/
#define	USB_HIDITEM_bType_Main		0
#define	USB_HIDITEM_bType_Global	1
#define	USB_HIDITEM_bType_Local		2
#define	USB_HIDITEM_bType_Reserved	3

/**** tags ****/
#define USB_HIDITEM_Main_Input		USB_HIDITEM_BuildShortTag(8, USB_HIDITEM_bType_Main, 0)
#define USB_HIDITEM_Main_Output		USB_HIDITEM_BuildShortTag(9, USB_HIDITEM_bType_Main, 0)
#define USB_HIDITEM_Main_Collection	USB_HIDITEM_BuildShortTag(0xA, USB_HIDITEM_bType_Main, 0)
#define USB_HIDITEM_Main_Feature	USB_HIDITEM_BuildShortTag(0xB, USB_HIDITEM_bType_Main, 0)
#define USB_HIDITEM_Main_EndCollection	USB_HIDITEM_BuildShortTag(0xC, USB_HIDITEM_bType_Main, 0)

#define USB_HIDITEM_Global_UsagePage		USB_HIDITEM_BuildShortTag(0x0, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_LogicalMinimum	USB_HIDITEM_BuildShortTag(0x1, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_LogicalMaximum	USB_HIDITEM_BuildShortTag(0x2, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_PhysicalMinimum	USB_HIDITEM_BuildShortTag(0x3, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_PhysicalMaximum	USB_HIDITEM_BuildShortTag(0x4, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_UnitExponent		USB_HIDITEM_BuildShortTag(0x5, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_Unit			USB_HIDITEM_BuildShortTag(0x6, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_ReportSize		USB_HIDITEM_BuildShortTag(0x7, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_ReportId		USB_HIDITEM_BuildShortTag(0x8, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_ReportCount		USB_HIDITEM_BuildShortTag(0x9, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_Push			USB_HIDITEM_BuildShortTag(0xA, USB_HIDITEM_bType_Global, 0)
#define USB_HIDITEM_Global_Pop			USB_HIDITEM_BuildShortTag(0xB, USB_HIDITEM_bType_Global, 0)

#define USB_HIDITEM_Local_Usage			USB_HIDITEM_BuildShortTag(0x0, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_UsageMinimum		USB_HIDITEM_BuildShortTag(0x1, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_UsageMaximum		USB_HIDITEM_BuildShortTag(0x2, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_DesignatorIndex	USB_HIDITEM_BuildShortTag(0x3, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_DesignatorMinimum	USB_HIDITEM_BuildShortTag(0x4, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_DesignatorMaximum	USB_HIDITEM_BuildShortTag(0x5, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_StringIndex		USB_HIDITEM_BuildShortTag(0x7, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_StringMinimum		USB_HIDITEM_BuildShortTag(0x8, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_StringMaximum		USB_HIDITEM_BuildShortTag(0x9, USB_HIDITEM_bType_Local, 0)
#define USB_HIDITEM_Local_SetDelimiter		USB_HIDITEM_BuildShortTag(0xA, USB_HIDITEM_bType_Local, 0)

/****************************************************************************\
|
|	Physical descriptors -- not supported in this file.
|
\****************************************************************************/


/**** end of usbhid10.h ****/
#endif /* _USBHID10_H_ */
