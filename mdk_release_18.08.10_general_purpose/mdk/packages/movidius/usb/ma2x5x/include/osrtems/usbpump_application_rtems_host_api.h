/* usbpump_application_rtems_host_api.h	Wed Jan 10 2018 14:58:28 chwon */

/*

Module:  usbpump_application_rtems_host_api.h

Function:
	Home for DataPump host stack API for RTEMS OS

Version:
	V3.21e	Wed Jan 10 2018 14:58:28 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2017-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	May 2017

Revision history:
   3.21a  Fri May 26 2017 15:15:13  chwon
	20480: Module created.

   3.21e  Wed Jan 03 2018 16:44:05  chwon
	20743: Add UsbPumpDataPumpRtemsApi_HostAcmClientCreate().

   3.21e  Wed Jan 10 2018 14:58:28  chwon
	20743: Add UsbPumpDataPumpRtemsHostApp_AcmClientCreate() and
	UsbPumpDataPumpRtemsHostApp_HidClientCreate().

*/

#ifndef _USBPUMP_APPLICATION_RTEMS_HOST_API_H_	/* prevent multiple includes */
#define _USBPUMP_APPLICATION_RTEMS_HOST_API_H_

#ifndef _USBPUMP_APPLICATION_RTEMS_API_H_
# include "usbpump_application_rtems_api.h"
#endif

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

USBPUMP_APPLICATION_BEGIN_DECLS

void *	/* UPLATFORM * */
UsbPump_Rtems_DataPump_HostStartup(
	const USBPUMP_APPLICATION_RTEMS_CONFIGURATION *	/* pAppRtemsConfig */
	);

void
UsbPump_Rtems_DataPump_ListUsbDevice(
	unsigned int	/* OutputBufferSize */
	);

/*
|| Create mass storage class driver client binding with RTEMS file system.
*/
unsigned int
UsbPump_Rtems_DataPump_MsdClientCreate(
	void *		/* pPlatform */,
	unsigned int	/* DebugFlags */,
	const char *	/* pMountBasePath */,
	unsigned int	/* AutoMounterTaskStackSize */,
	unsigned int	/* AutoMounterTaskPriority */
	);

/*
|| Create ACM class driver client binding with RTEMS device file system.
*/
unsigned int
UsbPumpDataPumpRtemsApi_HostAcmClientCreate(
	const char *	/* pHostAcmDeviceFileName */,
	unsigned int	/* nHostAcmDeviceFile */,
	unsigned int	/* DebugFlags */
	);

/*
|| Create host class drivers client for demo purpose.
*/
unsigned int
UsbPump_Rtems_HostApp_MsdClientCreate(
	void *		/* pPlatform */,
	unsigned int	/* DebugFlags */,
	unsigned int	/* MsdClientBufferSize */
	);

unsigned int
UsbPump_Rtems_HostApp_GcdClientCreate(
	void *		/* pPlatform */,
	unsigned int	/* DebugFlags */,
	unsigned int	/* GcdClientBufferSize */,
	unsigned int	/* GcdClientLoopbackCount */,
	unsigned int	/* fGcdClientPerformanceMeasure */
	);

unsigned int
UsbPump_Rtems_HostApp_UvcClientCreate(
	void *		/* pPlatform */,
	unsigned int	/* DebugFlags */,
	unsigned int	/* UvcClientNumRequests */
	);

unsigned int
UsbPumpDataPumpRtemsHostApp_AcmClientCreate(
	unsigned int	/* DebugFlags */
	);

unsigned int
UsbPumpDataPumpRtemsHostApp_HidClientCreate(
	unsigned int	/* DebugFlags */
	);

USBPUMP_APPLICATION_END_DECLS

/**** end of usbpump_application_rtems_host_api.h ****/
#endif /* _USBPUMP_APPLICATION_RTEMS_HOST_API_H_ */
