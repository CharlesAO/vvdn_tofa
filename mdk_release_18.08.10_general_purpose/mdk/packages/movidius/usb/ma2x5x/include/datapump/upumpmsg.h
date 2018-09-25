/* upumpmsg.h	Sat Sep 23 2000 19:12:13  tmm */

/*

Module:  upumpmsg.h

Function:
	The structures for the USBPUMP_GENERIC_MESSAGE interface.

Version:
	V1.60n	Sat Sep 23 2000 19:12:13	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2000 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 2000

Revision history:
   1.60n  Sat Sep 23 2000 19:12:13  tmm
	Module created.

*/

#ifndef _UPUMPMSG_H_		/* prevent multiple includes */
#define _UPUMPMSG_H_

#ifndef _USBPUMP_H_
# include "usbpump.h"
#endif

#ifndef _USBUUID_H_
# include "usbuuid.h"
#endif

/****************************************************************************\
|
|	In order to allow modules to transport generic opaque messages
|	from layer to layer, and to allow generic extensions, we define
|	the following generic message scheme.
|	
|	Generic messages carry a pointer to a UUID (which identifies the
|	actual message type) and a discriminator (indicating the particular
|	message that's being sent, and the kind of transport data).  The
|	discriminator only has meaning in the context of the UUID.
|
\****************************************************************************/


__TMS_TYPE_DEF_STRUCT(USBPUMP_GENERIC_MESSAGE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_GENERIC_MESSAGE_HDR_STRUCT);

struct __TMS_STRUCTNAME(USBPUMP_GENERIC_MESSAGE_HDR_STRUCT)
	{
	__TMS_CONST __TMS_USBUUID	*pUuid;		/* the UUID for the interface */
	__TMS_UINT			MessageCode;	/* the message 
							|| discriminator;
							|| client-defined.
							*/
	__TMS_BYTES			sizeMessage;	/* size of the overall structure,
							|| in bytes.
							*/
	__TMS_BOOL			bAccepted;	/* OUT: this is a result: initially
							|| FALSE, must be set to TRUE
							|| by the client if this is to
							|| be acccpeted.
							*/
	};

#define __TMS_USBPUMP_GENERIC_MESSAGE_HDR	__TMS_USBPUMP_GENERIC_MESSAGE_HDR_STRUCT Hdr

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_GENERIC_MESSAGE_HDR	__TMS_USBPUMP_GENERIC_MESSAGE_HDR
#endif

struct __TMS_STRUCTNAME(USBPUMP_GENERIC_MESSAGE)
	{
	__TMS_USBPUMP_GENERIC_MESSAGE_HDR;	/* the normal header */
	__TMS_UINT8		ClientData[1];	/* the data payload */
	};

/**** end of upumpmsg.h ****/
#endif /* _UPUMPMSG_H_ */
