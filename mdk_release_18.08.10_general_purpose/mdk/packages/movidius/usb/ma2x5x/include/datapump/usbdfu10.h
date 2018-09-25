/* usbdfu10.h	Thu Sep  8 2005 15:33:13 tmm */

/*

Module:  usbdfu10.h

Function:
	USB-IF Device Firmware Upgrade protocol, constants and definitions.

Version:
	V1.91h	Thu Sep  8 2005 15:33:13 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2004-2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjörn, MCCI Corporation	June 2004

Revision history:
   1.89a  Wed Jun 09 2004 18:26:14  maw
	Module created.

   1.91h  Thu Sep  8 2005 15:33:13  tmm
	Get rid of "// comments.
  
*/


#ifndef _USBDFU10_H_		/* prevent multiple includes */
#define _USBDFU10_H_

#include "usbif.h"

/****************************************************************************\
|
|	Class, subclass, and protocol codes.
|
\****************************************************************************/

/* device class codes */
#define	USB_bDeviceClass_Dfu				0x00

/* Interface class code */
#define	USB_bInterfaceClass_Dfu				0xFE

/* Interface subclasses */
#define	USB_bInterfaceSubClass_Dfu			0x01	

/* functional descriptor */
#define USB_bDescType_DFUFUNC				0x21	

/* class-specific requests */
#define	USB_bRequest_Dfu_DETACH				0x00
#define	USB_bRequest_Dfu_DNLOAD				0x01
#define	USB_bRequest_Dfu_UPLOAD				0x02
#define	USB_bRequest_Dfu_GETSTATUS			0x03
#define	USB_bRequest_Dfu_CLRSTATUS			0x04
#define	USB_bRequest_Dfu_GETSTATE			0x05
#define	USB_bRequest_Dfu_ABORT				0x06
#define	USB_bRequest_Dfu_NONE				0xFF

/* a table initializer */
#define	USB_bRequest_Dfu__INIT \
	{ USB_bRequest_Dfu_DETACH,	"Detach" }, \
	{ USB_bRequest_Dfu_DNLOAD,	"Download" }, \
	{ USB_bRequest_Dfu_UPLOAD,	"Upload" }, \
	{ USB_bRequest_Dfu_GETSTATUS,	"Get status" }, \
	{ USB_bRequest_Dfu_CLRSTATUS,	"Clear status" }, \
	{ USB_bRequest_Dfu_GETSTATE,	"Get state" }, \
	{ USB_bRequest_Dfu_ABORT,	"Abort" }, \
	{ USB_bRequest_Dfu_NONE,	"None" }

/* #define USB_Dfu_REQ(ReqDir,ReqType,ReqDest,Req) ((USHORT) (((ReqDir | ReqType | ReqDest & USB_bmRequestType_TYPE_MASK) << 8) | Req)) */
#define USB_Dfu_REQ(ReqDir,ReqType,Req) ((USHORT) (((ReqDir | ReqType | USB_bmRequestType_DEST_IFC) << 8) | Req))

#define USB_Dfu_DETACH      USB_Dfu_REQ(USB_bmRequestType_DIR_TODEV,  USB_bmRequestType_TYPE_CLASS, USB_bRequest_Dfu_DETACH)
#define USB_Dfu_DNLOAD      USB_Dfu_REQ(USB_bmRequestType_DIR_TODEV,  USB_bmRequestType_TYPE_CLASS, USB_bRequest_Dfu_DNLOAD)
#define USB_Dfu_UPLOAD      USB_Dfu_REQ(USB_bmRequestType_DIR_TOHOST, USB_bmRequestType_TYPE_CLASS, USB_bRequest_Dfu_UPLOAD)
#define USB_Dfu_GETSTATUS   USB_Dfu_REQ(USB_bmRequestType_DIR_TOHOST, USB_bmRequestType_TYPE_CLASS, USB_bRequest_Dfu_GETSTATUS)
#define USB_Dfu_CLRSTATUS   USB_Dfu_REQ(USB_bmRequestType_DIR_TODEV,  USB_bmRequestType_TYPE_CLASS, USB_bRequest_Dfu_CLRSTATUS)
#define USB_Dfu_GETSTATE    USB_Dfu_REQ(USB_bmRequestType_DIR_TOHOST, USB_bmRequestType_TYPE_CLASS, USB_bRequest_Dfu_GETSTATE)
#define USB_Dfu_ABORT       USB_Dfu_REQ(USB_bmRequestType_DIR_TODEV,  USB_bmRequestType_TYPE_CLASS, USB_bRequest_Dfu_ABORT)
#define USB_Dfu_NONE        USB_Dfu_REQ(USB_bmRequestType_DIR_TODEV,  USB_bmRequestType_TYPE_RSV3,  USB_bRequest_Dfu_NONE)

/****************************************************************************\
|
|	The functional descriptor
|
\****************************************************************************/
typedef struct 
	{
	unsigned char		bLength;		/* sizeof(struct USB_DevDesc)	*/
	unsigned char		bDescriptorType;	/* USB_bDescType_DFUFUNC	*/
	unsigned char		bmAttributes;		/* DFU attributes		*/
	unsigned char		wDetachTimeOut[2];	/* time in ms to detaching	*/
	unsigned char		wTransferSize[2];	/* max no of bytes in ctrl-wr	*/
	} USB_Dfu_FunctionalDescriptor;

/****************************************************************************\
|
|	DFU states
|
\****************************************************************************/

#define USB_Dfu_ProtoState_0_APP_IDLE                    0
#define USB_Dfu_ProtoState_1_APP_DETACH                  1
#define USB_Dfu_ProtoState_2_DFU_IDLE                    2
#define USB_Dfu_ProtoState_3_DFU_DNLOAD_SYNC             3
#define USB_Dfu_ProtoState_4_DFU_DNLOAD_BUSY             4
#define USB_Dfu_ProtoState_5_DFU_DNLOAD_IDLE             5
#define USB_Dfu_ProtoState_6_DFU_MANIFEST_SYNC           6
#define USB_Dfu_ProtoState_7_DFU_MANIFEST                7
#define USB_Dfu_ProtoState_8_DFU_MANIFEST_WAIT_RESET     8
#define USB_Dfu_ProtoState_9_DFU_UPLOAD_IDLE             9
#define USB_Dfu_ProtoState_10_DFU_ERROR                 10
#define USB_Dfu_ProtoState_CNT			    	(USB_Dfu_ProtoState_10_DFU_ERROR+1)

#define	USB_Dfu_ProtoState__INIT \
	{ \
	/* USB_Dfu_ProtoState_0_APP_IDLE		*/ "Application idle",		\
	/* USB_Dfu_ProtoState_1_APP_DETACH		*/ "Application detach",	\
	/* USB_Dfu_ProtoState_2_DFU_IDLE		*/ "Dfu idle",			\
	/* USB_Dfu_ProtoState_3_DFU_DNLOAD_SYNC		*/ "Dfu download sync",		\
	/* USB_Dfu_ProtoState_4_DFU_DNLOAD_BUSY		*/ "Dfu download busy",		\
	/* USB_Dfu_ProtoState_5_DFU_DNLOAD_IDLE		*/ "Dfu download idle",		\
	/* USB_Dfu_ProtoState_6_DFU_MANIFEST_SYNC	*/ "Dfu manifest sync",		\
	/* USB_Dfu_ProtoState_7_DFU_MANIFEST		*/ "Dfu manifest",		\
	/* USB_Dfu_ProtoState_8_DFU_MANIFEST_WAIT_RESET	*/ "Dfu manifest wait reset",	\
	/* USB_Dfu_ProtoState_9_DFU_UPLOAD_IDLE		*/ "Dfu upload idle",		\
	/* USB_Dfu_ProtoState_10_DFU_ERROR		*/ "Dfu error"			\
	}

/****************************************************************************\
|
|	DFU status codes
|
\****************************************************************************/

/* Status codes for GetStatus reply message */

#define USB_Dfu_Status_OK               0x00  /* No error condition present */
#define USB_Dfu_Status_errTARGET        0x01  /* File is not targeted for use by this device */
#define USB_Dfu_Status_errFILE          0x02  /* File is for this device but fails in some vendor-specific verification test */
#define USB_Dfu_Status_errWRITE         0x03  /* Device is unable to write memory */
#define USB_Dfu_Status_errERASE         0x04  /* Memory erase function failed */
#define USB_Dfu_Status_errCHECK_ERASED  0x05  /* Memory erase check failed */
#define USB_Dfu_Status_errPROG          0x06  /* Program memory function failed */
#define USB_Dfu_Status_errVERIFY        0x07  /* Programmed memory failed verification */
#define USB_Dfu_Status_errADDRESS       0x08  /* Cannot program memory due to received address that is out of range */
#define USB_Dfu_Status_errNOTDONE       0x09  /* Received DFU_DNLOAD with wLength=0, but device does not think it has all the data yet */
#define USB_Dfu_Status_errFIRMWARE      0x0A  /* Device firmware is corrupt. It cannot return to run-time (non-DFU) operations */
#define USB_Dfu_Status_errVENDOR        0x0B  /* iString indicate a vendor-specific error.*/
#define USB_Dfu_Status_errUSBR          0x0C  /* Device detected unexpected USB reset signaling */
#define USB_Dfu_Status_errPOR           0x0D  /* Device detected unexpected power on reset */
#define USB_Dfu_Status_errUNKNOWN       0x0E  /* Something went wrong, device does not know what is was */
#define USB_Dfu_Status_errSTALLEDPKT    0x0F  /* Device stalled an unexpected request */

/****************************************************************************\
|
|	Some useful data structures for DFU work.
|
\****************************************************************************/

typedef struct 
	{
	unsigned char bStatus;
	unsigned char bwPollTimeout[3];
	unsigned char bState;
	unsigned char iString;
	} USB_Dfu_GETSTATUS_REPLY;

/****************************************************************************\
|
|	Macro's.
|
\****************************************************************************/

#define USB_Dfu_DEVICE_REPLY_OK(pDev)  UDEVREPLY(pDev, (VOID *) ~0, 0);
#define USB_Dfu_DEVICE_REPLY_NOK(pDev) UDEVREPLY(pDev, (VOID *)  0, 0);

/**** end of usbdfu10.h ****/
#endif /* _USBDFU10_H_ */
