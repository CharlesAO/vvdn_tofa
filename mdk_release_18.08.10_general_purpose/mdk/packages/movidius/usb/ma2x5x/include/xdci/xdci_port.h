/* xdci_port.h	Thu Jul 10 2014 18:27:43 tmm */

/*

Module:  xdci_port.h

Function:
	Common macros for access XDCI registers.

Version:
	V3.13a	Thu Jul 10 2014 18:27:43 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2014

Revision history:
   3.13a  Thu Jul 10 2014 18:27:43  tmm
	18309: Module created.

*/

#ifndef _XDCI_PORT_H_		/* prevent multiple includes */
#define _XDCI_PORT_H_

#ifdef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/*
|| As usual, we allow the code to override the offest calculation method.
|| XDCI_PORT_OFFSET() is always called with a pDcd pointer that is a
|| PUDEVICE; so in a multi-instance situation, you can readily use the
|| UDEVICE to do a shift or multiply or table lookup to find the register.
|| However, in the common single-instance case this optimizes down to a
|| multiply that can normally be done at compile time.  We use a separate
|| symbol to govern the override so you can even declare a function,
|| if you like.  The prototype for that function should be
||	int XDCI_PORT_OFFSET(USBPUMP_DEVICE_XDCD *, int).
*/
#ifndef XDCI_PORT_OFFSET_DEFINED
# define XDCI_PORT_OFFSET_DEFINED
# if defined(XDCI_PORT_STRIDE)
#  define XDCI_PORT_OFFSET(pDcd, r)	((r) * XDCI_PORT_STRIDE)
# elif defined(HIL_DEFAULT_REG_STRIDE)
#  define XDCI_PORT_OFFSET(pDcd, r)	((r) * HIL_DEFAULT_REG_STRIDE)
# else
#  define XDCI_PORT_OFFSET(pDcd, r)	(r)
# endif
#endif

#define	XDCI_IOPORT_ADDRESS(IoPort, r)				\
	((ARG_IOPORT)((ADDRBITS)((IoPort) +			\
		XDCI_PORT_OFFSET(pDcd, r))))

/**** end of xdci_port.h ****/
#endif /* _XDCI_PORT_H_ */
