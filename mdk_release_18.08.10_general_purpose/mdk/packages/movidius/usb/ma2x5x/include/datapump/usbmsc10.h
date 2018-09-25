/* usbmsc10.h	Tue Mar 10 2015 12:52:30 tmm */

/*

Module:  usbmsc10.h

Function:
	USB-IF Mass Storage Class - Bulk Only Transport protocol 
	constants and definitions.

Version:
	V3.15b	Tue Mar 10 2015 12:52:30 tmm	Edit level 7

Copyright notice:
	This file copyright (C) 2001-2002, 2009-2010, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation 
 
Author:
	Chris Sawran, MCCI Corporation	July 2001

Revision history:
   1.00a  Fri Jul  6 2001 00:00:01  cds
	Module created.

   1.79b  Fri Oct 4 2002 15:05:00  maw
   	Renamed from proto/usbmass/i/usbmass.h to i/usbmsc10.h
  
   2.01a  Mon Mar 09 2009 16:02:55 ctpeng
   	8265: Add USB_bInterfaceSubClass_MassStorageUasp and
   	USB_Uasp_Descriptor_PipeUsage
  
   2.01a  Tue Dec 01 2009 14:31:32  chwon
	9357: Fixed doc-xml error in revision history. No code change.

   3.01a  Fri Jan 22 2010 11:40:33 ctpeng
   	9784: Remove USB_bInterfaceSubClass_MassStorageUasp and
	add USB_bInterfaceSubclass_ProtocolUasp

   3.15a  Mon Jan 19 2015 02:23:06  tmm
	18860: fix names of interface protocols.

   3.15b  Tue Mar 10 2015 12:52:30  tmm
	18860: remove the non-WIRE forms, so we don't need the full
	MCCI header file system.

*/


#ifndef _USBMSC10_H_		/* prevent multiple includes */
#define _USBMSC10_H_

#ifndef _USBIF_H_
# include "usbif.h"
#endif

/****************************************************************************\
|
|	Class, subclass, and protocol codes.
|
\****************************************************************************/

/* mass storage class device */
#define	USB_bInterfaceClass_MassStorage			0x08

/* Mass Storage class interface constants */
#define	USB_bInterfaceSubClass_MassStorageRbc		0x01
#define	USB_bInterfaceSubClass_MassStorageATAPI		0x02
#define USB_bInterfaceSubClass_MassStorageQIC157	0x03
#define USB_bInterfaceSubClass_MassStorageUFI		0x04
#define USB_bInterfaceSubClass_MassStorageSff8070i	0x05
#define USB_bInterfaceSubClass_MassStorageScsi		0x06

/* A table initializer */
#define	USB_bInterfaceSubClass_MassStorage_INIT \
	{ USB_bInterfaceSubClass_MassStorageRbc, "Reduced Block Commands" }, 	\
	{ USB_bInterfaceSubClass_MassStorageATAPI, "Atapi/MMC-2 Commands" }, 	\
	{ USB_bInterfaceSubClass_MassStorageQIC157, "QIC157 Commands" }, 	\
	{ USB_bInterfaceSubClass_MassStorageUFI, "UFI Commands" }, 		\
	{ USB_bInterfaceSubClass_MassStorageSff8070i, "SFF8070i Commands" }, 	\
	{ USB_bInterfaceSubClass_MassStorageScsi, "SCSI Commands"}

/* Mass Storage class Protocol codes */
#define USB_bInterfaceProtocol_MassStorageCBI		0x00
#define USB_bInterfaceProtocol_MassStorageCB		0x01
#define USB_bInterfaceProtocol_MassStorageBulkOnly	0x50
#define USB_bInterfaceProtocol_MassStorageUasp		0x62

/* A table initializer */
#define	USB_bInterfaceProtocol_MassStorage_INIT \
	{ USB_bInterfaceProtocol_MassStorageCBI, "Control-Bulk-Interrupt Transport" }, 	\
	{ USB_bInterfaceProtocol_MassStorageCB, "Control-Bulk (No Interrupt) Transport" }, \
	{ USB_bInterfaceProtocol_MassStorageBulkOnly, "Bulk-Only Transport" },		\
	{ USB_bInterfaceProtocol_MassStorageUasp, "UASP Transport" }

/* legacy names */
#define USB_bInterfaceSubclass_ProtocolCBI		\
	USB_bInterfaceProtocol_MassStorageCBI
#define USB_bInterfaceSubclass_ProtocolCB		\
	USB_bInterfaceProtocol_MassStorageCB
#define USB_bInterfaceSubclass_ProtocolBulkOnly		\
	USB_bInterfaceProtocol_MassStorageBulkOnly
#define USB_bInterfaceSubclass_ProtocolUasp		\
	USB_bInterfaceProtocol_MassStorageUasp


/* Bulk-only class-specific requests */
#define USB_bRequest_MassStorage_BULK_ONLY_RESET	0xFF
#define USB_bRequest_MassStorage_BULK_ONLY_GET_MAX_LUN	0xFE

/* A table initializer */
#define	USB_bRequest_MassStorage__INIT \
	{ USB_bRequest_MassStorage_BULK_ONLY_RESET, "BULK_ONLY_RESET" }, \
	{ USB_bRequest_MassStorage_BULK_ONLY_GET_MAX_LUN, "BULK_ONLY_GET_MAX_LUN" }

/****************************************************************************\
|
|	Uasp Class-Specific Pipe Usage Descriptor
|
\****************************************************************************/

struct USB_Uasp_Descriptor_PipeUsage
	{
	unsigned char	bFunctionLength;
	unsigned char	bDescriptorType;
	unsigned char	bPipeID;
	unsigned char	bReserved;
	};

#define USB_bDescriptorType_Uasp_PIPE_USAGE	0x24
#define USB_bDescriptorType_Uasp_PIPE_USAGE_COMMAND	0x01
#define USB_bDescriptorType_Uasp_PIPE_USAGE_STATUS	0x02
#define USB_bDescriptorType_Uasp_PIPE_USAGE_DATAIN	0x03
#define USB_bDescriptorType_Uasp_PIPE_USAGE_DATAOUT	0x04

/****************************************************************************\
|
|	The CBW for BOT
|
\****************************************************************************/

#define	USB_MassStorage_BOT_CDB_SIZE	16


struct USB_MassStorage_BOT_CBW_WIRE
	{
	unsigned char	dwCBWSignature[4]; /* always 0x43425355 */
	unsigned char	dwCBWTag[4];	/* CSW associative tag */
	unsigned char	dwCBWDataTransferLength[4];
	unsigned char	bmCBWFlags;	/* 7=0, data out host to device
					|| 6-0 must be zero
					*/
	unsigned char	bCBWLUN;
	unsigned char	bCBWCBLength;	/* length of the CBWCB in bytes */
	unsigned char	CBWCB[USB_MassStorage_BOT_CDB_SIZE];
					/* command block to be executed */
	};

/* Command Block Wrapper constants */
#define USB_MassStorage_BOT_CBW_SIZE		(31)
#define USB_MassStorage_BOT_CBW_SIGNATURE	(0x43425355u)
#define USB_MassStorage_BOT_CBW_FLAGS_DATA_IN	(0x80u)

/****************************************************************************\
|
|	The BOT CSW structures
|
\****************************************************************************/

struct USB_MassStorage_BOT_CSW_WIRE
	{
	unsigned char dwCSWSignature[4];	/* CSW sig: 0x53425355 */
	unsigned char dwCSWTag[4];		/* associative tag */
	unsigned char dwCSWDataResidue[4];	/* residue of transfer */
	unsigned char bCSWStatus;		/* non-zero is failure */
	};

/* Command Status Wrapper constants */
#define USB_MassStorage_BOT_CSW_SIZE			(13)
#define USB_MassStorage_BOT_CSW_SIGNATURE		(0x53425355u)
#define USB_MassStorage_BOT_CSW_STATUS_SUCCESS		(0)
#define USB_MassStorage_BOT_CSW_STATUS_FAILED		(1)
#define USB_MassStorage_BOT_CSW_STATUS_PHASE_ERROR	(2)

/**** end of usbmsc10.h ****/
#endif /* _USBMSC10_H_ */
