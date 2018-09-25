/* upfmargay.h	Wed Nov 22 2017 10:41:16 chwon */

/*

Module:  upfmargay.h

Function:
	Definition of Sparc sample port

Version:
	V3.21e	Wed Nov 22 2017 10:41:16 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2014-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	October 2014

Revision history:
   3.13b  Thu Oct 09 2014 15:56:28 chwon
	18574: Module created.

   3.15b  Mon Mar 09 2015 15:01:42  chwon
	18966: Include usbpump_application_rtems_api.h and add serial number
	information in the context.

   3.17a  Thu Feb 11 2016 11:55:55  chwon
	19843: Add device start / stop variables in the context.

   3.17a  Wed Jun 29 2016 16:42:15  chwon
	20181: Add upf_margay_pPlatformIoctlFn in the context.

   3.19b  Wed Feb 15 2017 09:29:02  chwon
	20480: Include usbpump_rtems_intr_v2.h file.

   3.19b  Fri Mar 24 2017 12:56:30  chwon
	20312: Add upf_margay_SysClockKhz in the context.

   3.21e  Wed Nov 22 2017 10:41:16  chwon
	20644: Add device MAC address info in the context.
	20713: Add upf_mellivora_VendorCode in the context.

*/

#ifndef _UPFMARGAY_H_	/* prevent multiple includes */
#define _UPFMARGAY_H_

#ifndef _USBPUMP_RTEMS_H_
# include "usbpump_rtems.h"
#endif

#ifndef _USBPUMP_RTEMS_INTR_V2_H_
# include "usbpump_rtems_intr_v2.h"
#endif

#ifndef _USBPUMP_APPLICATION_RTEMS_API_H_
# include "usbpump_application_rtems_api.h"
#endif

#ifndef _USBIOCTL_DEVICE_H_
# include "usbioctl_device.h"
#endif

/****************************************************************************\
|
|	Our platform context structure
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_MARGAY_CONTEXT);
struct __TMS_STRUCTNAME	(USBPUMP_MARGAY_CONTEXT)
	{
	/*
	|| The USB OS none platform structure
	*/
	__TMS_UPLATFORM_RTEMS		upf_margay_PlatformRtems;

	__TMS_USBPUMP_RTEMS_INTERRUPT_SYSTEM_INTERFACE
					upf_margay_RtemsIntSysIfc;

	__TMS_UDEVICE *			upf_margay_uDevices[1];

	__TMS_UINT			upf_margay_fInitialized: 1;
	__TMS_UINT			upf_margay_fCacheEnabled: 1;
	__TMS_UINT			upf_margay_fDeviceStopped: 1;
	__TMS_UINT			upf_margay_fTimerRunning: 1;
	__TMS_UINT			upf_margay_fHostMode: 1;
	__TMS_UINT			upf_margay_fDeviceMacExist: 1;

	__TMS_UINT			upf_margay_VendorCode;

	__TMS_UINT32			upf_margay_SysClockKhz;

	__TMS_USBPUMP_TIMER_TIMEOUT	upf_margay_RestartDelay;

	USBPUMP_APPLICATION_RTEMS_USE_BUS_POWER_FN *
					upf_margay_pUseBusPowerFn;
	USBPUMP_APPLICATION_RTEMS_PLATFORM_IOCTL_FN *
					upf_margay_pPlatformIoctlFn;
	__TMS_UINT8			upf_margay_MacAddress[6];
	__TMS_CHAR			upf_margay_SerialNo[74];
	__TMS_BYTES			upf_margay_nSerialNo;

	__TMS_USBPUMP_TIMER		upf_margay_Timer;

	__TMS_USBPUMP_IOCTL_DEVICE_START_ASYNC_ARG
					upf_margay_StartArg;
	__TMS_USBPUMP_IOCTL_QE 		upf_margay_StartIoCtlQe;
	USBPUMP_APPLICATION_RTEMS_DEVICE_START_DONE_FN *
					upf_margay_pStartDoneFn;
	void *				upf_margay_pStartDoneCtx;

	__TMS_USBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG
					upf_margay_StopArg;
	__TMS_USBPUMP_IOCTL_QE 		upf_margay_StopIoCtlQe;
	USBPUMP_APPLICATION_RTEMS_DEVICE_STOP_DONE_FN *
					upf_margay_pStopDoneFn;
	void *				upf_margay_pStopDoneCtx;
	};

/**** end of upfmargay.h ****/
#endif /* _UPFMARGAY_H_ */
