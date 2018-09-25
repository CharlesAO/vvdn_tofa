/* usbpump_proto_vsc2_types.h	Wed Aug 02 2017 15:33:43 chwon */

/*

Module:  usbpump_proto_vsc2_types.h

Function:
	Definition of the VSC protocol types

Version:
	V3.21c	Wed Aug 02 2017 15:33:43 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2013, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2013

Revision history:
   3.11c  Fri Mar 22 2013 13:52:48 chwon
	16968: Module created.

   3.21c  Wed Aug 02 2017 15:33:43  chwon
	20629: Add USBPUMP_PROTO_VSC2_STREAM_INFO type.

*/

#ifndef _USBPUMP_PROTO_VSC2_TYPES_H_	/* prevent multiple includes */
#define _USBPUMP_PROTO_VSC2_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	Basic types for VSC protocol.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_PROTO_VSC2);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_PROTO_VSC2_CONFIG);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_PROTO_VSC2_REQUEST);

__TMS_TYPE_DEF_UNION	(USBPUMP_PROTO_VSC2_INCALL);
__TMS_TYPE_DEF_UNION	(USBPUMP_PROTO_VSC2_OUTCALL);

__TMS_TYPE_DEF_HANDLE	(USBPUMP_PROTO_VSC2_STREAM_HANDLE);
__TMS_TYPE_DEF_ARG	(USBPUMP_PROTO_VSC2_SETUP_STATUS, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_PROTO_VSC2_EVENT, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_PROTO_VSC2_STATUS, UINT32);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_PROTO_VSC2_STREAM_INFO);

/**** end of usbpump_proto_vsc2_types.h ****/
#endif /* _USBPUMP_PROTO_VSC2_TYPES_H_ */
