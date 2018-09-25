/* usbpump_debugio_proto.h	Tue Apr 21 2009 09:24:09 maw */

/*

Module:  usbpump_debugio_proto.h

Function:
	Prototypes for common Debug-IO functions and 
	global variables.

Version:
	V2.01a	Tue Apr 21 2009 09:24:09 maw	Edit level 1

Copyright notice:
	This file copyright (C) 2009 by	

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Mats Webjörn, MCCI Corporation	April 2009

Revision history:
   2.01a  Tue Apr 21 2009 09:24:09 maw
	7563: Module created from usbpump_abstract_sio_proto.h EL 2

*/

#ifndef _USBPUMP_DEBUGIO_PROTO_H_	/* prevent multiple includes */
#define _USBPUMP_DEBUGIO_PROTO_H_

#ifndef _USBPUMP_DEBUGIO_TYPES_H_
# include "usbpump_debugio_types.h"
#endif


__TMS_BEGIN_DECLS

__TMS_USBPUMP_DEBUGIO_STATUS
UsbPumpDebugIoI_SessionIsOpen(
	__TMS_PUSBPUMP_DEBUGIO_SESSION	pSession
	);

__TMS_VOID
UsbPumpDebugIoI_CompleteQe(
	__TMS_PUSBPUMP_DEBUGIO_SESSION __TMS_CONST	pSession,
	__TMS_USBPUMP_DEBUGIO_STATUS			Status
	);

__TMS_END_DECLS

/**** end of usbpump_debugio_proto.h ****/
#endif /* _USBPUMP_DEBUGIO_PROTO_H_ */
