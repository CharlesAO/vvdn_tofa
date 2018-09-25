/* usbpump_authentication.h	Mon Dec 19 2011 11:35:37 chwon */

/*

Module:  usbpump_authentication.h

Function:
	Definition of DataPump authentication

Version:
	V3.01f	Mon Dec 19 2011 11:35:38 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2009, 2011 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	September 2009

Revision history:
   2.01a  Sat Sep 12 2009 11:39:30 chwon
	8556: Module created.

   3.01f  Wed Sep 28 2011 16:32:41  chwon
	14300: add USBPUMP_AUTHENTICATION_GET_RESPONSE_ALT definition and
	add SHA1 context to use SHA1 calculation library.

   3.01f  Mon Dec 19 2011 11:35:38  chwon
	14300: add serial number information in USBPUMP_AUTHENTICATION_DEVICE.

*/

#ifndef _USBPUMP_AUTHENTICATION_H_		/* prevent multiple includes */
#define _USBPUMP_AUTHENTICATION_H_

#ifndef _BASEHILTYPES_H_
# include "basehiltypes.h"
#endif

#ifndef _UEVENTNODE_H_
# include "ueventnode.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMPLIB_SHA1_H_
# include "usbpumplib_sha1.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_AUTHENTICATION);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_AUTHENTICATION_DEVICE);


/****************************************************************************\
|
|	Definition of USBPUMP_AUTHENTICATION
|
\****************************************************************************/

/*

Type:	USBPUMP_AUTHENTICATION_DEVICE

Function:
	The structure contains authentication information for UDEVICE.

Description:
	A USBPUMP_AUTHENTICATION_DEVICE structure contains authentication
	information for the UDEVICE.

	The DataPump will have three related variables:
	1) 64-byte buffer for the challenge,
	2) a cell named ChallengeCount, initialized to zero on every USB reset,
	3) a boolean "error flag" named fErrorFlag, that is cleared on every
	   USB reset.

	When processing Send Authentication Challenge, the DataPump will check
	whether Send Authentication Challenge has been received before (i.e.,
	is ChallengeCount zero or non-zero).  If ChallengeCount is zero, and
	wLength is in 1..sizeof(buffer), the DataPump shall arrange to recieve
	the data in to the buffer (and shall set count to wLength).
	If ChallengeCount is zero, but wLength is outside the legal range,
	the DataPump shall return STALL, and shall set fErrorFlag.
	If fErrorFlag is set, the DataPump shall return STALL.

	When the authentication data has been received correctly, the DataPump
	shall return success. If the authentication data is recevied with a
	USB-level error, the DataPump shall return STALL and shall set
	fErrorFlag.

	When processing Get Authentication Response, the DataPump shall verify
	that error flag is reset, that ChallengeCount is non-zero, and that
	wLength is 14h (20 decimal). If not, the DataPump shall set fErrorFlag
	and return STALL. Otherwise, DataPump shall calculate the SHA-1 digest
	of (buffer for count)..<device serial number>>..<<constantString>>,
	and return this during the data phase.

	After the Get Authentication Response command completes, the DataPump
	shall set fErrorFlag.

Contents:
	USBPUMP_AUTHENTICATION *pAuthentication;
		The back pointer to the authentication object.

	UDEVICE *pDevice;
		Pointer to the UDEVICE object, for convenience.

	UINT8 ChallengeBuffer[64];
		The 64-byte buffer for the challenge

	UINT ChallengeCount;
		A challenge count, initialized to zero on every USB reset

	UINT fErrorFlag: 1;
		A boolean "error flag", that is cleared on every USB reset.

	UINT fAcceptSetup: 1;
		A boolean "accept setup packet flag", that is set when get
		authentication setup packet.

	UEVENTNODE DeviceEventNode;
		The device event node to catch device event.

	USBPUMP_AUTHENTICATION_DEVICE *pNext;
	USBPUMP_AUTHENTICATION_DEVICE *pLast;
		The link pointer to maintain list.

See Also:
	USBPUMP_AUTHENTICATION

*/

struct __TMS_STRUCTNAME	(USBPUMP_AUTHENTICATION_DEVICE)
	{
	__TMS_USBPUMP_AUTHENTICATION *		pAuthentication;
	__TMS_UDEVICE *				pDevice;

	__TMS_UINT8				ChallengeBuffer[64];
	__TMS_UINT				ChallengeCount;
	__TMS_UINT				fErrorFlag: 1;
	__TMS_UINT				fAcceptSetup: 1;

	__TMS_UINT8 *				pSerialNo;
	__TMS_BYTES				nSerialNo;

	__TMS_UEVENTNODE			DeviceEventNode;

	__TMS_USBPUMP_AUTHENTICATION_DEVICE *	pNext;
	__TMS_USBPUMP_AUTHENTICATION_DEVICE *	pLast;
	};

/*

Type:	USBPUMP_AUTHENTICATION

Function:
	The DataPump object that represents USBPUMP_AUTHENTICATION.

Description:
	A USBPUMP_AUTHENTICATION object provides the lock host drivers to
	authorized device types.

Contents:
	USBPUMP_OBJECT_HEADER ObjectHeader;
		The standard object header.  The tag is
		|USBPUMP_AUTHENTICATION_TAG|.
		The IOCTL parent is the pointer to the next object closer
		to the |UPLATFORM|.

	UPLATFORM *pPlatform;
		Pointer to the platform object, for convenience.

	CONST TEXT *pAuthString;
		The security sensitive information.  This will be set when
		create authentication object.

	USBPUMP_AUTHENTICATION_DEVICE *pDeviceList;
		The list header to maintain USBPUMP_AUTHENTICATION_DEVICE
		structure. Generally only one node in the list.

	USBPUMP_SHA1_CONTEXT Sha1Context;
		The SHA1 context for SHA1 calcuation library.

See Also:
	UsbPumpAuthentication_Create()
	UsbPumpAuthentication_Delete()

*/

struct __TMS_STRUCTNAME	(USBPUMP_AUTHENTICATION)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	/*
	|| The USBPUMP_AUTHENTICATION implementation may need to use platform
	|| routines; so it stashes a UPLATFORM pointer.
	*/
	__TMS_UPLATFORM *			pPlatform;

	__TMS_CONST __TMS_TEXT *		pAuthString;

	__TMS_USBPUMP_AUTHENTICATION_DEVICE *	pDeviceList;

	__TMS_USBPUMP_SHA1_CONTEXT		Sha1Context;
	};

/*
|| Object related things
*/

/* the DataPump AUTHENTICATION object tag */
#define	__TMS_USBPUMP_AUTHENTICATION_TAG		\
	__TMS_UHIL_MEMTAG('U', 'A', 'u', 't')

/* the UhilAux object name */
#define	__TMS_USBPUMP_AUTHENTICATION_OBJECT_NAME(Text)	\
	Text ".authentication.mcci.com"

#define	__TMS_USBPUMP_AUTHENTICATION_OBJECT_ENUM_NAME	\
	__TMS_USBPUMP_AUTHENTICATION_OBJECT_NAME("*")


/****************************************************************************\
|
|	Authentication vendor specific command definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_AUTHENTICATION_SEND_CHALLENGE	0xCC
#define	__TMS_USBPUMP_AUTHENTICATION_GET_RESPONSE	0xCC
#define	__TMS_USBPUMP_AUTHENTICATION_GET_RESPONSE_ALT	0xCD


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_AUTHENTICATION *
UsbPumpAuthentication_Create(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_CONST __TMS_TEXT *	/* pAuthString */
	);

__TMS_VOID
UsbPumpAuthentication_Delete(
	__TMS_USBPUMP_AUTHENTICATION *	/* pAuthentication */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_AUTHENTICATION_TAG	\
   __TMS_USBPUMP_AUTHENTICATION_TAG
# define USBPUMP_AUTHENTICATION_OBJECT_NAME(Text)	\
   __TMS_USBPUMP_AUTHENTICATION_OBJECT_NAME(Text)
# define USBPUMP_AUTHENTICATION_OBJECT_ENUM_NAME	\
   __TMS_USBPUMP_AUTHENTICATION_OBJECT_ENUM_NAME
# define USBPUMP_AUTHENTICATION_SEND_CHALLENGE	\
   __TMS_USBPUMP_AUTHENTICATION_SEND_CHALLENGE
# define USBPUMP_AUTHENTICATION_GET_RESPONSE	\
   __TMS_USBPUMP_AUTHENTICATION_GET_RESPONSE
# define USBPUMP_AUTHENTICATION_GET_RESPONSE_ALT	\
   __TMS_USBPUMP_AUTHENTICATION_GET_RESPONSE_ALT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_authentication.h ****/
#endif /* _USBPUMP_AUTHENTICATION_H_ */
