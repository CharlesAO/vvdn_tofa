/* usbeem10.h	Tue Dec 08 2009 08:11:59 maw */

/*

Module:  usbeem10.h

Function:
	USB Communication Device Class (CDC) V1.2 Ethernet Emulation Model 
	(EEM) V1.0 constants, augmented by WMC 1.1.

Version:
	V2.01a	Tue Dec 08 2009 08:11:59 maw	Edit level 3

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Saravanan J, MCCI Corporation	March 2009

Revision history:
   2.01a  Mon Mar 30 2009 16:46:26 saravanan
	UDP-7674: Module created.

   2.01a  Wed May 06 2009 14:26:48  maw
   	7674: Add EEM header declaration

   2.01a  Tue Dec 08 2009 08:11:59  maw
	8902: Remove __TMS-prefix from declarations

*/

#ifndef _USBEEM10_H_		/* prevent multiple includes */
#define _USBEEM10_H_

/****************************************************************************\
|
|	subclass, and protocol codes.
|
\****************************************************************************/

/* Interface subclasses */
#define	USB_bInterfaceSubClass_CommEEM		0x0C	/* Ethernet Emulation Model */


/* Comm class Protocol codes */
#define	USB_bInterfaceProtocol_CommEEM		0x07	/* Ethernet Emulation Model */

/****************************************************************************\
|
|	EEM header
|
\****************************************************************************/
struct USB_CommEEM_PacketHeader
	{
	unsigned char	wBitmap[2];
	};

#define USB_CommEEM_PACKET_HEADER__SIZE	2

/* Common field for both command and data */
#define	USB_CommEEM_bmType_COMMAND			0x8000
#define	USB_CommEEM_bmType_DATA				0x0000
#define	USB_CommEEM_bmType_MASK				0x8000

/* Fields for command */

#define	USB_CommEEM_bmReserved_RSV			0x4000
#define	USB_CommEEM_bmReserved_MASK			0x4000

#define	USB_CommEEM_bmEEMCmd_ECHO			(0<<11)
#define	USB_CommEEM_bmEEMCmd_ECHORESPONSE		(1<<11)
#define	USB_CommEEM_bmEEMCmd_SUSPENDHINT		(2<<11)
#define	USB_CommEEM_bmEEMCmd_RESPONSEHINT		(3<<11)
#define	USB_CommEEM_bmEEMCmd_RESPONSECOMPLETEHINT	(4<<11)
#define	USB_CommEEM_bmEEMCmd_TICKLE			(5<<11)
#define	USB_CommEEM_bmEEMCmd_RSV1			(6<<11)
#define	USB_CommEEM_bmEEMCmd_RSV2			(7<<11)
#define	USB_CommEEM_bmEEMCmd_MASK			0x3800

#define	USB_CommEEM_bmEEMCmdParam_MASK			0x07FF

/* Fields for data */
#define	USB_CommEEM_bmCrc_CALCULATED			0x4000
#define	USB_CommEEM_bmCrc_SENTINEL			0x0000
#define	USB_CommEEM_bmCrc_MASK				0x4000

#define	USB_CommEEM_Length_MASK				0x3FFF

/* Get header */
#define	USB_CommEEM_GET_HEADER(ARG_pHdr)				\
	UHIL_LE_GETUINT16(						\
	&(((const struct USB_CommEEM_PacketHeader *)(ARG_pHdr))->wBitmap[0]))

/* Get bmType */
#define	USB_CommEEM_GET_bmType(ARG_pHdr)				\
	(USB_CommEEM_GET_HEADER(ARG_pHdr) & USB_CommEEM_bmType_MASK)

#define	USB_CommEEM_IS_COMMAND(ARG_pHdr)				\
	(USB_CommEEM_GET_bmType(ARG_pHdr) == USB_CommEEM_bmType_COMMAND)

#define	USB_CommEEM_IS_DATA(ARG_pHdr)				\
	(USB_CommEEM_GET_bmType(ARG_pHdr) == USB_CommEEM_bmType_DATA)

/* Get bmReserved */
#define	USB_CommEEM_GET_bmReserved(ARG_pHdr)				\
	(USB_CommEEM_GET_HEADER(ARG_pHdr) & USB_CommEEM_bmReserved_MASK)

#define	USB_CommEEM_IS_RESERVED(ARG_pHdr)				\
	(USB_CommEEM_GET_bmReserved(ARG_pHdr) == USB_CommEEM_bmReserved_RSV)

/* Get bmEEMCmd */
#define	USB_CommEEM_GET_bmEEMCmd(ARG_pHdr)			\
	(USB_CommEEM_GET_HEADER(ARG_pHdr) & USB_CommEEM_bmEEMCmd_MASK)

/* Get bmEEMCmd */
#define	USB_CommEEM_GET_bmEEMCmdParam(ARG_pHdr)			\
	(USB_CommEEM_GET_HEADER(ARG_pHdr) & USB_CommEEM_bmEEMCmdParam_MASK)

/* Get bmCrc */
#define	USB_CommEEM_GET_bmCrc(ARG_pHdr)				\
	(USB_CommEEM_GET_HEADER(ARG_pHdr) & USB_CommEEM_bmCrc_MASK)

#define	USB_CommEEM_IS_CALC_CRC(ARG_pHdr)				\
	(USB_CommEEM_GET_bmCrc(ARG_pHdr) == USB_CommEEM_bmCrc_CALCULATED)

#define	USB_CommEEM_IS_SENTINEL_CRC(ARG_pHdr)				\
	(USB_CommEEM_GET_bmCrc(ARG_pHdr) == USB_CommEEM_bmCrc_SENTINEL)

/* Get Length */
#define	USB_CommEEM_GET_Length(ARG_pHdr)				\
	(USB_CommEEM_GET_HEADER(ARG_pHdr) & USB_CommEEM_Length_MASK)

/**** end of usbeem10.h ****/
#endif /* _USBEEM10_H_ */
