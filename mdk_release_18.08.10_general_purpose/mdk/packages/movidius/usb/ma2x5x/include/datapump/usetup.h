/* usetup.h	Mon Jun  1 2015 00:21:22 tmm */

/*

Module:  usetup.h

Function:
	Home for USETUP and USETUP_WIRE

Version:
	V3.15b	Mon Jun  1 2015 00:21:22 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2005, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Sat Aug 13 2005 21:04:21  tmm
	Module created.

   3.15b  Mon Jun  1 2015 00:21:22  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _USETUP_H_		/* prevent multiple includes */
#define _USETUP_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/*

Type:	USETUP

Index:	Type:	USETUP_WIRE
	Constant: USETUP_WIRE__SIZE

Function:
	C structures that represent a SETUP packet, in local byte order or
	as transmitted on the USB wire.

Description:
	Setup packets are transmitted as a sequence of 8 bytes, modelled as
	USETUP_WIRE objects.  The DataPump converts the packets into USETUP 
	structures; these are structures with named fields, and with multi-
	byte values in local byte order.

Contents:
	A USETUP consists of:

	UCHAR uc_bmRequestType;
		Byte 0, the request/type bitmask.

	UCHAR uc_bRequest;
		Byte 1, the request code.

	USHORT uc_wValue;
		Bytes 2&3, the "Value", converted to local byte order.

	USHORT uc_wIndex;
		Bytes 4&5, the "index", converted to local byte order.

	USHORT uc_wLength;
		Bytes 6&7, the "length", converted to local byte order.

	A USETUP_WIRE is similar, with the following contents.

	UCHAR uc_bmRequestType;
		Byte 0, the request/type bitmask.

	UCHAR uc_bRequest;
		Byte 1, the request code.

	UCHAR uc_wValue[2];
		Bytes 2&3, the "Value", in little-endian order

	UCHAR uc_wIndex[2];
		Bytes 4&5, the "index", in little-endian order

	UCHAR uc_wLength[2];
		Bytes 6&7, the "length", in little-endian order


Notes:
	Don't assume that the USETUP structure is itself 8 bytes long; 
	compilers may decide to spread out the USHORTs for performance 
	reasons.  Similarly , please use USETUP_WIRE__SIZE as the
	size of a setup -- some compilers like to pad structures to
	cache-boundary alignment.

	USETUP is one of the base types of the DataPump.

*/

/* __TMS_TYPE_DEF(USETUP, struct TTUSB_USETUP); */
struct TTUSB_USETUP
	{
	__TMS_UCHAR	uc_bmRequestType;		/* the request type */
	__TMS_UCHAR	uc_bRequest;			/* the request code */
	__TMS_USHORT	uc_wValue;			/* the "value" */
	__TMS_USHORT	uc_wIndex;			/* the "index" */
	__TMS_USHORT	uc_wLength;			/* the "length" */
	};

__TMS_TYPE_DEF_STRUCT(USETUP_WIRE);

/****************************************************************************\
|
|	The wire-level USETUP
|
\****************************************************************************/

#include "usbpump_pushpack1.h"

__TMS_LIBPORT_PACKED1_DEFINITION(
struct __TMS_STRUCTNAME(USETUP_WIRE)
)
	{
	__TMS_UINT8	uc_bmRequestType;		/* the request type */
	__TMS_UINT8	uc_bRequest;			/* the request code */
	__TMS_UINT8	uc_wValue[2];			/* the "value" */
	__TMS_UINT8	uc_wIndex[2];			/* the "index" */
	__TMS_UINT8	uc_wLength[2];			/* the "length" */
	} __TMS_LIBPORT_PACKED1_SUFFIX;

#include "usbpump_poppack.h"

/* just avoid all the issues with how compilers pack things */
#define	__TMS_USETUP_WIRE__SIZE	8

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USETUP_WIRE__SIZE	__TMS_USETUP_WIRE__SIZE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usetup.h ****/
#endif /* _USETUP_H_ */
