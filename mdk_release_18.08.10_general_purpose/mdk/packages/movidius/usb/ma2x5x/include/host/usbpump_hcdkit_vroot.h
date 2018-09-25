/* usbpump_hcdkit_vroot.h	Mon Dec 14 2015 12:28:54 chwon */

/*

Module:  usbpump_hcdkit_vroot.h

Function:
	Definition of Virtual root hub functions

Version:
	V3.15d	Mon Dec 14 2015 12:28:54 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2006, 2009, 2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	January 2006

Revision history:
   1.93e  Fri Jan 06 2006 14:03:55 chwon
	Module created.

   1.93e  Thu Mar 02 2006 14:50:15 ssyang
	Add UsbPumpHcdKitI_vRootNotifyStatusChange

   2.01a  Mon Apr 27 2009 13:55:23  skpark
	7701: Added functions for USB 3.0 root hub 

   2.01a  Wed Jun 03 2009 15:59:36  chwon
	7701: Remove UsbPumpHcdKitI_vRootHub30SetPortFeature and
	UsbPumpHcdKitI_vRootHub30ClearPortFeature function

   2.01a  Thu Dec 03 2009 16:26:57  chwon
	9172: Add HcdKit library -- UsbPumpHcdKitI_vRootStartTimer and
	UsbPumpHcdKitI_vRootNotifyPortStatusChange.

   3.11d  Mon Sep 16 2013 16:11:30  chwon
	17698: Add UsbPumpHcdKitI_vRootGetDescriptorRaw() and
	UsbPumpHcdKitI_vRootHub30GetDescriptorRaw() prototype.

   3.15d  Mon Dec 14 2015 12:28:54  chwon
	19694: Add UsbPumpHcdKitI_vRootStopTimer() prototype.

*/

#ifndef _USBPUMP_HCDKIT_VROOT_H_		/* prevent multiple includes */
#define _USBPUMP_HCDKIT_VROOT_H_

#ifndef _USBPUMP_HCDKIT_SWITCH_H_
# include "usbpump_hcdkit_switch.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

#ifndef _USETUP_H_
# include "usetup.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_HUB_FEATURE_FN
					UsbPumpHcdKitI_vRootClearHubFeature;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_CLEAR_PORT_FEATURE_FN
					UsbPumpHcdKitI_vRootClearPortFeature;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_DESC_FN
					UsbPumpHcdKitI_vRootGetDescriptor;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_HUB_STATUS_FN
					UsbPumpHcdKitI_vRootGetHubStatus;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_STATUS_FN
					UsbPumpHcdKitI_vRootGetPortStatus;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_SET_HUB_FEATURE_FN
					UsbPumpHcdKitI_vRootSetHubFeature;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_SET_PORT_FEATURE_FN
					UsbPumpHcdKitI_vRootSetPortFeature;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_SUBMIT_STATUS_READ_FN
					UsbPumpHcdKitI_vRootSubmitStatusRead;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_NOTIFY_STATUS_CHANGE_FN
					UsbPumpHcdKitI_vRootNotifyStatusChange;					
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_TT_REQUEST_FN
					UsbPumpHcdKitI_vRootTtRequest;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_DESC_FN
					UsbPumpHcdKitI_vRootHub30GetDescriptor;
__TMS_USBPUMP_HCDKIT_SWITCH_ROOT_GET_PORT_ERROR_COUNT_FN
					UsbPumpHcdKitI_vRootGetPortErrorCount;

/*
|| HCDKIT library functions
*/
__TMS_VOID
UsbPumpHcdKitI_vRootStartTimer(
	__TMS_USBPUMP_HCDKIT_HCD *		/* pHcdKit */,
	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT *	/* pRootHubPort */,
	__TMS_ARG_USBPUMP_TIMER_TIMEOUT		/* MilliSecond */
	);

__TMS_VOID
UsbPumpHcdKitI_vRootStopTimer(
	__TMS_USBPUMP_HCDKIT_HCD *		/* pHcdKit */,
	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT *	/* pRootHubPort */
	);

__TMS_VOID
UsbPumpHcdKitI_vRootNotifyPortStatusChange(
	__TMS_USBPUMP_HCDKIT_HCD *		/* pHcdKit */,
	__TMS_USBPUMP_HCDKIT_ROOTHUBPORT *	/* pRootHubPort */
	);

__TMS_BYTES
UsbPumpHcdKitI_vRootGetDescriptorRaw(
	__TMS_USBPUMP_HCDKIT_HCD *		/* pHcdKit */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *		/* pRootHub */,
	__TMS_USETUP_WIRE *			/* pSetup */,
	__TMS_UINT8 *				/* pBuffer */,
	__TMS_BYTES				/* nBuffer */
	);

__TMS_BYTES
UsbPumpHcdKitI_vRootHub30GetDescriptorRaw(
	__TMS_USBPUMP_HCDKIT_HCD *		/* pHcdKit */,
	__TMS_USBPUMP_HCDKIT_ROOTHUB *		/* pRootHub */,
	__TMS_USETUP_WIRE *			/* pSetup */,
	__TMS_UINT8 *				/* pBuffer */,
	__TMS_BYTES				/* nBuffer */
	);

__TMS_END_DECLS

/**** end of usbpump_hcdkit_vroot.h ****/
#endif /* _USBPUMP_HCDKIT_VROOT_H_ */
