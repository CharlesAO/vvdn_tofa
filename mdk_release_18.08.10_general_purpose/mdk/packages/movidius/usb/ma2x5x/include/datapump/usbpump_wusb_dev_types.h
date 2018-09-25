/* usbpump_wusb_dev_types.h	Fri Nov 14 2008 11:19:32 ssatish */

/*

Module:  usbpump_wusb_dev_types.h

Function:
	Define the symbolic types for WUSB device.

Version:
	V1.97j	Fri Nov 14 2008 11:19:32 ssatish	Edit level 1

Copyright notice:
	This file copyright (C) 2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Sathish Kumar, MCCI Corporation	November 2008

Revision history:
   1.97j  Fri Nov 14 2008 11:19:32  ssatish
	UDP-6287: Module created.

*/

#ifndef _USBPUMP_WUSB_DEV_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_WUSB_DEV_TYPES_H_

#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/
__TMS_TYPE_DEF_ARG(USBPUMP_WUSB_DEV_SCAN_MODE_WIRE, UINT8);
__TMS_TYPE_DEF_ARG(USBPUMP_WUSB_DEV_SCAN_MODE, UINT32);
__TMS_TYPE_DEF_ARG(USBPUMP_WUSB_DEV_CONN_STATUS, UINT8);
__TMS_TYPE_DEF_ARG(USBPUMP_WUSB_DEV_NUMERIC_CONN_STATUS, UINT8);
__TMS_TYPE_DEF_ARG(USBPUMP_WUSB_DEV_NUMERIC_VERIFICATION_NUM, UINT16);

__TMS_TYPE_DEF_ARG(USBPUMP_WUSB_DEV_BAND_GRP_NUM, UINT8);
__TMS_TYPE_DEF_ARG(USBPUMP_WUSB_DEV_CHANNEL_NUM, UINT8);

__TMS_TYPE_DEF_STRUCT(USBPUMP_WUSB_DEV_CHID);
__TMS_TYPE_DEF_STRUCT(USBPUMP_WUSB_DEV_CDID);
__TMS_TYPE_DEF_STRUCT(USBPUMP_WUSB_DEV_CKEY);
__TMS_TYPE_DEF_STRUCT(USBPUMP_WUSB_DEV_SCAN_HOST_INFO);
__TMS_TYPE_DEF_STRUCT(USBPUMP_WUSB_DEV_CONN_CONTEXT);
__TMS_TYPE_DEF_STRUCT(USBPUMP_WUSB_DEV_NUMERIC_ASSOC_CC_INFO);

/**** end of usbpump_wusb_dev_types.h ****/
#endif /* _USBPUMP_WUSB_DEV_TYPES_H_ */
