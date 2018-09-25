/* usbpump_application_rtems_api.h	Wed Nov 22 2017 10:42:00 chwon */

/*

Module:  usbpump_application_rtems_api.h

Function:
	Definition of DataPump API for RTEMS OS

Version:
	V3.21e	Wed Nov 22 2017 10:42:00 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2015-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	February 2015

Revision history:
   3.15b  Thu Feb 19 2015 11:52:58 chwon
	18966: Module created.

   3.17a  Thu Feb 11 2016 11:56:49  chwon
	19843: Add device start and stop API.

   3.17a  Wed Jun 29 2016 16:40:29  chwon
	20181: Add pPlatformIoctlFn in the configuration.

   3.19a  Thu Dec 15 2016 16:55:58  chwon
	20414: Add fDoNotWaitDebugFlush in the configuration.

   3.19b  Wed Feb 15 2017 09:36:38  chwon
	20480: Add UsbPump_Rtems_DataPump_HostStartup() for host stack support.

   3.21a  Wed May 24 2017 15:33:59  chwon
	20563: Add UsbPump_Rtems_DataPump_ListUsbDevice().

   3.21a  Fri May 26 2017 15:14:20  chwon
	20480: Move host APIs to usbpump_application_rtems_host_api.h.

   3.21a  Tue Jun 06 2017 13:05:25  chwon
	20480: Add device deinit, init, start, stop APIs.

   3.21c  Thu Aug 10 2017 11:48:47  chwon
	20639: Fix typo in UsbPump_Rtems_DataPump_StopDevice() prototype.

   3.21e  Wed Nov 22 2017 10:42:00  chwon
	20644: Add device MAC address in the configuration.
	20713: Add OsStringDescriptorVendorCode in the configuration.

*/

#ifndef _USBPUMP_APPLICATION_RTEMS_API_H_	/* prevent multiple includes */
#define _USBPUMP_APPLICATION_RTEMS_API_H_

/****************************************************************************\
|
|	Define the begin/end declaration tags for C++ co-existance
|
\****************************************************************************/

#ifdef __cplusplus
# define USBPUMP_APPLICATION_BEGIN_DECLS	extern "C" {
# define USBPUMP_APPLICATION_END_DECLS		}
#else
# define USBPUMP_APPLICATION_BEGIN_DECLS	/* nothing */
# define USBPUMP_APPLICATION_END_DECLS		/* nothing */
#endif


/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

typedef struct _USBPUMP_APPLICATION_RTEMS_CONFIGURATION
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION;

/*

Type:	USBPUMP_APPLICATION_RTEMS_USE_BUS_POWER_FN

Function:
	This function returns device use USB bus power or not.

Definition:
	typedef unsigned int
		USBPUMP_APPLICATION_RTEMS_USE_BUS_POWER_FN(
			void
			);

Description:
	This application supplied method, if present in the application
	configuration, is called whenever the DataPump need to know the USB
	device use USB bus power or self power.

Returns:
	TRUE if device use USB bus power now. FALSE if device use self power.

*/

typedef unsigned int
	USBPUMP_APPLICATION_RTEMS_USE_BUS_POWER_FN(
		void
		);

/*

Type:	USBPUMP_APPLICATION_RTEMS_DEVICE_START_DONE_FN

Function:
	This function is callback of the USB device start.

Definition:
	typedef void
		USBPUMP_APPLICATION_RTEMS_DEVICE_START_DONE_FN(
			void *		pClientContext,
			unsigned int	Status
			);

Description:
	This function is callback of the USB device start. This function will
	be called from DataPump context.  So this function need to handle
	inter-task communication if it is required.

Returns:
	No explicit result.

*/

typedef void
	USBPUMP_APPLICATION_RTEMS_DEVICE_START_DONE_FN(
		void *		pClientContext,
		unsigned int	Status
		);

/*

Type:	USBPUMP_APPLICATION_RTEMS_DEVICE_STOP_DONE_FN

Function:
	This function is callback of the USB device stop.

Definition:
	typedef void
		USBPUMP_APPLICATION_RTEMS_DEVICE_STOP_DONE_FN(
			void *		pClientContext,
			unsigned int	Status
			);

Description:
	This function is callback of the USB device stop. This function will
	be called from DataPump context.  So this function need to handle
	inter-task communication if it is required.

Returns:
	No explicit result.

*/

typedef void
	USBPUMP_APPLICATION_RTEMS_DEVICE_STOP_DONE_FN(
		void *		pClientContext,
		unsigned int	Status
		);

#define	USBPUMP_APPLICATION_RTEMS_STATUS_OK		0
#define	USBPUMP_APPLICATION_RTEMS_STATUS_ERROR		1
#define	USBPUMP_APPLICATION_RTEMS_STATUS_BUSY		2


/*

Type:	USBPUMP_APPLICATION_RTEMS_PLATFORM_IOCTL_FN

Function:
	User supplied UPLATFORM ioctl handler.

Definition:
	typedef int
		USBPUMP_APPLICATION_RTEMS_PLATFORM_IOCTL_FN(
			void *		pPlatform,
			unsigned int	IoctlCode,
			const void *	pInParam,
			void *		pOutParam
			);

Description:
	This function is user supplied UPLATFORM ioctl handler.  The DataPump
	platform object ioctl handler calls this function if DataPump platform
	object doesn't support IoctlCode.  If this function doesn't support
	IoctlCode, it should returns USBPUMP_IOCTL_RESULT_NOT_CLAIMED.

Returns:
	USBPUMP_IOCTL_RESULT code.

*/

typedef int
	USBPUMP_APPLICATION_RTEMS_PLATFORM_IOCTL_FN(
		void *		pPlatform,
		unsigned int	IoctlCode,
		const void *	pInParam,
		void *		pOutParam
		);


/****************************************************************************\
|
|	USBPUMP_APPLICATION_RTEMS_CONFIGURATION structure
|
\****************************************************************************/

/*

Type:	USBPUMP_APPLICATION_RTEMS_CONFIGURATION

Function:
	C structure type; DataPump configuration information for RTEMS OS.

Description:
	This structure represents a configuration information for the DataPump
	for Rtems OS.  Client should pass this configuration information when
	initialize the DataPump by calling UsbPump_Rtems_DataPump_Startup().

Contents:
	|unsigned int nEventQueue;|
		Number of the DataPump event queue element.

	|void *pMemoryPool;|
	|unsigned int nMemoryPool;|
		Platform memory pool information.  The pMemoryPool is pointer
		of the platform memory pool and the nMemoryPool is size of the
		platform memory pool.  This is optional. If pMemoryPool is NULL
		or nMemoryPool is zero, it will not create platform abstract
		pool and it will use RTEMS system memory allocation.

	|unsigned int DataPumpTaskPriority;|
		DataPump task priority. The priority range from a high of 1 to
		a low of 255.

	|unsigned int DebugTaskPriority;|
		Debug task priority. The priority range from a high of 1 to
		a low of 255.  This debug task priority should be lower than
		DataPump task priority.

	|unsigned int UsbInterruptPriority;|
		USB interrupt priority value. This will be used to configure
		USB interrupt.

	|const char * pDeviceSerialNumber;|
		This is USB device serial number string. If pDeviceSerialNumber
		is NULL, DataPump will use serial number string in the URC file.

	|USBPUMP_APPLICATION_RTEMS_USE_BUS_POWER_FN * pUseBusPowerFn;|
		This function will be called by DataPump when host sends
		GetDeviceStatus command.  This function returns current device
		power state which use USB bus power or self power.

	|unsigned int fCacheEnabled;|
		This flag represents system enabled cache or not.  The user
		should set this flag to TRUE if system cache is enabled.
		If this flag is TRUE, DCD will flush and invalidate data buffer
		before and after USB IO operation.

	|unsigned int DebugMask;|
		DataPump debug mask. Please refer debug mask definition in the
		usbkern/i/usbpumpdebug.h file.

	|USBPUMP_APPLICATION_RTEMS_PLATFORM_IOCTL_FN * pPlatformIoctlFn;|
		This function will be called by DataPump if DataPump platform
		object ioctl handler doesn't support ioctl.

	|unsigned int fDoNotWaitDebugFlush;|
		This flag controls debug output flush behavior.  If this flag
		is not zero, DataPump thread will not wait all debug output
		message flush out.  This flag should be set to non zero value
		if customer wants to use rtems shell via USB serial with
		checked version DataPump.

	|const unsigned char * pDeviceMacAddress;|
		This is USB device MAC address. It is required if application
		wants to use CDC-NCM device class. If pDeviceMacAddress is
		NULL, DataPump will use hardcoded MAC address. Two MAC address
		is required for one CDC-NCM interface. For example, application
		provide below MyDeviceMacAddress.

		    const unsigned char MyDeviceMacAddress[6] =
			{ 0x00, 0x02, 0xCC, 0xF0, 0x00, 0x06 };

		DataPump use two MAC address -- 0002ccf00006 and 0002ccf00007

	|unsigned int OsStringDescriptorVendorCode;|
		This is Microsoft OS string descriptor vendor code.  If this
		value is not zero, DataPump will register default Microsoft OS
		string descriptor process function and DataPump will support
		Microsoft OS string descriptor if USB class driver support it.

Notes:
	Instances of this object are normally initialized using either
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_Vx() {compile time} or
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_Vx() {runtime}.

See Also:
	USBPUMP_APPLICATION_RTEMS_USE_BUS_POWER_FN

*/

struct _USBPUMP_APPLICATION_RTEMS_CONFIGURATION
	{
	unsigned int		nEventQueue;
	void *			pMemoryPool;
	unsigned int		nMemoryPool;
	unsigned int		DataPumpTaskPriority;
	unsigned int		DebugTaskPriority;
	unsigned int		UsbInterruptPriority;
	const char *		pDeviceSerialNumber;
	USBPUMP_APPLICATION_RTEMS_USE_BUS_POWER_FN *pUseBusPowerFn;
	unsigned int		fCacheEnabled;
	unsigned int		DebugMask;
	USBPUMP_APPLICATION_RTEMS_PLATFORM_IOCTL_FN *pPlatformIoctlFn;
	unsigned int		fDoNotWaitDebugFlush;
	const unsigned char *	pDeviceMacAddress;
	unsigned int		OsStringDescriptorVendorCode;
	};

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V5(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush,					\
		a_pDeviceMacAddress,					\
		a_OsStringDescriptorVendorCode				\
		)							\
	{								\
	(a_nEventQueue),						\
	(a_pMemoryPool),						\
	(a_nMemoryPool),						\
	(a_DataPumpTaskPriority),					\
	(a_DebugTaskPriority),						\
	(a_UsbInterruptPriority),					\
	(a_pDeviceSerialNumber),					\
	(a_pUseBusPowerFn),						\
	(a_fCacheEnabled),						\
	(a_DebugMask),							\
	(a_pPlatformIoctlFn),						\
	(a_fDoNotWaitDebugFlush),					\
	(a_pDeviceMacAddress),						\
	(a_OsStringDescriptorVendorCode)				\
	}

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V4(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush,					\
		a_pDeviceMacAddress					\
		)							\
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V5(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush,					\
		a_pDeviceMacAddress,					\
		/* V4: OsStringDescriptorVendorCode */ 0		\
		)

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V3(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush					\
		)							\
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V4(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush,					\
		/* V4: pDeviceMacAddress */ (const unsigned char *) 0	\
		)

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V2(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn					\
		)							\
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V3(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		/* V3: fDoNotWaitDebugFlush */ 0			\
		)

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V1(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask						\
		)							\
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_INIT_V2(		\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		/* V2: pPlatformIoctlFn */				\
		    (USBPUMP_APPLICATION_RTEMS_PLATFORM_IOCTL_FN *) 0	\
		)

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V5(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush,					\
		a_pDeviceMacAddress,					\
		a_OsStringDescriptorVendorCode				\
		)							\
    do	{								\
	(a_pConfig)->nEventQueue = (a_nEventQueue);			\
	(a_pConfig)->pMemoryPool = (a_pMemoryPool);			\
	(a_pConfig)->nMemoryPool = (a_nMemoryPool);			\
	(a_pConfig)->DataPumpTaskPriority = (a_DataPumpTaskPriority);	\
	(a_pConfig)->DebugTaskPriority = (a_DebugTaskPriority);		\
	(a_pConfig)->UsbInterruptPriority = (a_UsbInterruptPriority);	\
	(a_pConfig)->pDeviceSerialNumber = (a_pDeviceSerialNumber);	\
	(a_pConfig)->pUseBusPowerFn = (a_pUseBusPowerFn);		\
	(a_pConfig)->fCacheEnabled = (a_fCacheEnabled);			\
	(a_pConfig)->DebugMask = (a_DebugMask);				\
	(a_pConfig)->pPlatformIoctlFn = (a_pPlatformIoctlFn);		\
	(a_pConfig)->fDoNotWaitDebugFlush = (a_fDoNotWaitDebugFlush);	\
	(a_pConfig)->pDeviceMacAddress = (a_pDeviceMacAddress);		\
	(a_pConfig)->OsStringDescriptorVendorCode =			\
		(a_OsStringDescriptorVendorCode);			\
	} while (0)

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V4(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush,					\
		a_pDeviceMacAddress					\
		)							\
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V5(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush,					\
		a_pDeviceMacAddress,					\
		/* V4: OsStringDescriptorVendorCode */ 0		\
		)

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V3(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush					\
		)							\
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V4(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		a_fDoNotWaitDebugFlush,					\
		/* V4: pDeviceMacAddress */ (const unsigned char *) 0	\
		)

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V2(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn					\
		)							\
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V3(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		a_pPlatformIoctlFn,					\
		/* V3: fDoNotWaitDebugFlush */ 0			\
		)

#define USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V1(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask						\
		)							\
	USBPUMP_APPLICATION_RTEMS_CONFIGURATION_SETUP_V2(		\
		a_pConfig,						\
		a_nEventQueue,						\
		a_pMemoryPool,						\
		a_nMemoryPool,						\
		a_DataPumpTaskPriority,					\
		a_DebugTaskPriority,					\
		a_UsbInterruptPriority,					\
		a_pDeviceSerialNumber,					\
		a_pUseBusPowerFn,					\
		a_fCacheEnabled,					\
		a_DebugMask,						\
		/* V2: pPlatformIoctlFn */				\
		    (USBPUMP_APPLICATION_RTEMS_PLATFORM_IOCTL_FN *) 0	\
		)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

USBPUMP_APPLICATION_BEGIN_DECLS

void *	/* UPLATFORM * */
UsbPump_Rtems_DataPump_Startup(
	const USBPUMP_APPLICATION_RTEMS_CONFIGURATION *	/* pAppRtemsConfig */
	);

void *	/* UPLATFORM * */
UsbPump_Rtems_DataPump_GetPlatform(
	void
	);

void *	/* UDEVICE * */
UsbPump_Rtems_DataPump_GetDevice(
	void
	);

unsigned int
UsbPump_Rtems_DataPump_DeviceDeinit(
	void
	);

unsigned int
UsbPump_Rtems_DataPump_DeviceInit(
	void
	);

unsigned int
UsbPump_Rtems_DataPump_DeviceStart(
	void
	);

unsigned int
UsbPump_Rtems_DataPump_DeviceStop(
	void
	);

unsigned int
UsbPump_Rtems_DataPump_RestartDevice(
	unsigned int					 /* RestartDelayMS */
	);

void
UsbPump_Rtems_DataPump_StartDevice(
	USBPUMP_APPLICATION_RTEMS_DEVICE_START_DONE_FN * /* pDoneFn */,
	void *						 /* pDoneCtx */
	);

void
UsbPump_Rtems_DataPump_StopDevice(
	USBPUMP_APPLICATION_RTEMS_DEVICE_STOP_DONE_FN *	/* pDoneFn */,
	void *						/* pDoneCtx */
	);

void
UsbPump_Rtems_DataPump_SetDebugFlags(
	const char *	/* pObjectNamePattern */,
	unsigned int	/* ClearDebugFlags */,
	unsigned int	/* SetDebugFlags */
	);

unsigned int
UsbPumpDataPumpRtemsApi_DeviceAbstractNicCreate(
	unsigned int	/* DebugFlags */,
	void *		/* struct rtems_bsdnet_config * pRtemsBsdNetConfig */,
	const char *	/* pDeviceName */,
	unsigned int	/* nReceiveQEs */,
	unsigned int	/* nSendQEs */
	);

unsigned int
UsbPumpDataPumpRtemsApi_DeviceSerialCreate(
	unsigned int	/* DebugFlags */,
	const char *	/* pSerialObjectName */,
	const char *	/* pSerialDeviceName */
	);

USBPUMP_APPLICATION_END_DECLS

/**** end of usbpump_application_rtems_api.h ****/
#endif /* _USBPUMP_APPLICATION_RTEMS_API_H_ */
