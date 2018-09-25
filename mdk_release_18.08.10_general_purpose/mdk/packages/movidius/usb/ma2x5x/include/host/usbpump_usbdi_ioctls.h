/* usbpump_usbdi_ioctls.h	Tue May 23 2017 16:37:37 chwon */

/*

Module:  usbpump_usbdi_ioctls.h

Function:
	Home for the USBDI IOCTL definitions.

Version:
	V3.21a	Tue May 23 2017 16:37:37 chwon	Edit level 37

Copyright notice:
	This file copyright (C) 2005-2007, 2009-2013, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Sat Aug 13 2005 23:58:58  tmm
	Module created.

   1.93e  Sat Feb 25 2006 17:54:16  tmm
	Oops.  PORT_IDLE_FUNCTION needs to be synchronous (but just
	sending an indication); and INSTANCE_DEPARTURE needs to be async
	(in case it takes a while for the driver to shut down).

   1.97e  Sun Aug 13 2006 19:07:49  tmm
	Fix typo in documentation (GETSTATE was miskeyed as GETSTATUE).

   1.97j  Mon Nov 05 2007 12:29:55  chwon
	3420: add USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION and
	USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION

   1.97j  Thu Nov 15 2007 16:57:17  chwon
	3471: add USBPUMP_IOCTL_USBDI_PORT_UNIDLE_FUNCTION

   1.97j  Tue Nov 20 2007 11:48:22  chwon
	3471: change from USBPUMP_IOCTL_USBDI_PORT_UNIDLE_FUNCTION to
	USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT

   3.01a  Wed Dec 23 2009 12:36:04  chwon
	9642: add USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER.

   3.01a  Sun Jan  3 2010 19:08:16  tmm
	9698:  fix documentation for
	USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER.

   3.01a  Mon Jan 04 2010 23:15:25  chwon
	9688: add USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER.

   3.01a  Mon Jan 11 2010 22:22:55  chwon
	9728: add USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY.

   3.01b  Mon Mar 08 2010 10:05:43  chwon
	9678: fix document for ..._REGISTER_DEVICE_ARRIVAL_LISTENER and
	..._REGISTER_DEVICE_DEPARTURE_LISTENER.

   3.01c  Fri Mar 26 2010 15:34:06  chwon
	10076: add USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS.
	10082: add USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER.

   3.01c  Thu Apr 22 2010 09:38:53  chwon
	10076: remove USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS.

   3.01c  Thu May 20 2010 13:18:14  chwon
	10076: re-add USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS.
	9569: add USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP.

   3.01c  Fri May 21 2010 16:19:55  chwon
	9569: make USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP to asychronous IOCTL.
	10385: add USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC and
	USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC.

   3.01c  Wed May 26 2010 16:36:02  chwon
	10385: add fDoUnconfigBeforeSuspend parameter for the
	USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC ioctl.

   3.01c  Thu May 27 2010 13:37:08  chwon
	10385: USBDI undefine config if fDoUnconfigBeforeSuspend is TRUE.

   3.01c  Thu Jun 03 2010 14:05:38  chwon
	9569: fix typo in USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC setup macro.

   3.01c  Tue Aug 10 2010 18:51:57  chwon
	10385: add fSendRemoteWakeupEnable parameter for the
	USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC ioctl.

   3.01c  Mon Aug 16 2010 12:23:44  chwon
	10988: add USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC and
	USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC ioctl.

   3.01c  Fri Aug 27 2010 11:28:30  chwon
	10987: add USBPUMP_IOCTL_USBDI_{GET,SET}_AUTO_LPM_ENABLE and
	USBPUMP_IOCTL_USBDI_PORT_{GET,SET}_AUTO_LPM_ENABLE ioctl.
	Change IOCTL offsets to make a room for USBDI.

   3.01c  Wed Sep 01 2010 15:27:39  chwon
	10987: remove USBPUMP_IOCTL_USBDI_{GET,SET}_AUTO_LPM_ENABLE ioctl and
	add USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION ioctl.

   3.01c  Tue Sep 07 2010 10:38:40  chwon
	11202: add USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE ioctl.

   3.01c  Wed Sep 08 2010 16:41:59  chwon
	11020: add USBPUMP_USBDI_PORT pointer parameter in the alert function.

   3.01c  Fri Sep 10 2010 16:34:05  chwon
	11202: add USTAT parameter for pAlertFn and pass USTAT_NOHW when
	port is departing.

   3.01d  Tue Oct 19 2010 20:47:10  chwon
	11376: add USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP ioctl.
	11666: add USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE ioctl.

   3.01d  Mon Jan 10 2011 12:05:40  chwon
	12324: add USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION ioctl.

   3.01d  Thu Jan 20 2011 13:54:50  chwon
	12222: add USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE and
	USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE ioctl.

   3.01d  Thu Mar 10 2011 22:33:57  chwon
	12923: add PowerAction parameter for ..._POWER_STATE_CHANGE_ASYNC ioctl

   3.01f  Fri Jun 24 2011 23:15:54  chwon
	13723: add fSuspendPort parameter for ..._PORT_UNBIND_FUNCTION ioctl.

   3.01f  Wed Jun 27 2012 16:40:24  luxw
	15607: add USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS ioctl.

   3.11c  Fri Apr 12 2013 14:12:58  chwon
	17015: Add USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION ioctl.

   3.21a  Tue May 23 2017 16:37:37  chwon
	20563: Add USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS ioctl.

*/

#ifndef _USBPUMP_USBDI_IOCTLS_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_IOCTLS_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

#ifndef _USBPUMPIOCTLS_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _USBPUMPLIB_H_
# include "usbpumplib.h"
#endif

#ifndef _USBPUMP_USBDI_USBD_H_
# include "usbpump_usbdi_usbd.h"
#endif

#ifndef _USTAT_H_
# include "ustat.h"
#endif

/****************************************************************************\
|
|	Define offsets for the ioctl kinds
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_CLASS_USBDI_USBDI		0
#define	__TMS_USBPUMP_IOCTL_CLASS_USBDI_DRIVER_CLASS	20
#define	__TMS_USBPUMP_IOCTL_CLASS_USBDI_FUNCTION	30
#define	__TMS_USBPUMP_IOCTL_CLASS_USBDI_PORT		40

/* all codes must be less than this value: */
#define	__TMS_USBPUMP_IOCTL_CLASS_USBDI_INTERNAL	128

/****************************************************************************\
|
|	The non-EDGE IOCTLs arrive inwards from USBDI client drivers or
|	functions.
|
\****************************************************************************/

/**** define the USBDI in-calls ****/
#define __TMS_USBPUMP_IOCTL_USBDI(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_USBDI,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_USBDI_, NAME, _ARG)	\
		)

/****************************************************************************\
|
|	Messages to USBDI
|
\****************************************************************************/

/* messages to USBDI: register a driver class */
#define	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_CLASS			\
	__TMS_USBPUMP_IOCTL_USBDI(0, W, REGISTER_CLASS)

/* unregister a driver class */
#define	__TMS_USBPUMP_IOCTL_USBDI_UNREGISTER_CLASS			\
	__TMS_USBPUMP_IOCTL_USBDI(1, W, UNREGISTER_CLASS)

/* start up USBDI */
#define	__TMS_USBPUMP_IOCTL_USBDI_START					\
	__TMS_USBPUMP_IOCTL_USBDI(2, R, STARTSTOP)

/* stop USBDI */
#define	__TMS_USBPUMP_IOCTL_USBDI_STOP					\
	__TMS_USBPUMP_IOCTL_USBDI(3, R, STARTSTOP)

/* get USBD state */
#define	__TMS_USBPUMP_IOCTL_USBDI_GETSTATE				\
	__TMS_USBPUMP_IOCTL_USBDI(4, R, STARTSTOP)

/* register device arrival listener */
#define	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER	\
	__TMS_USBPUMP_IOCTL_USBDI(5, W, REGISTER_DEVICE_ARRIVAL_LISTENER)

/* register device departure listener */
#define	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER	\
	__TMS_USBPUMP_IOCTL_USBDI(6, W, REGISTER_DEVICE_DEPARTURE_LISTENER)

/* register USBD stop listener */
#define	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER		\
	__TMS_USBPUMP_IOCTL_USBDI(7, W, REGISTER_USBDI_STOP_LISTENER)

/* override USBD configuration flags */
#define	__TMS_USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS			\
	__TMS_USBPUMP_IOCTL_USBDI(8, RW, OVERRIDE_CONFIG_FLAGS)

/****************************************************************************\
|
|	Messages to driver-class objects
|
\****************************************************************************/

/* initialize a driver class object */
#define	__TMS_USBPUMP_IOCTL_USBDI_DRIVER_CLASS_INIT			\
	__TMS_USBPUMP_IOCTL_USBDI(20, W, DRIVER_CLASS_INIT)

/* deinitialize a driver class object */
#define	__TMS_USBPUMP_IOCTL_USBDI_DRIVER_CLASS_DEINIT			\
	__TMS_USBPUMP_IOCTL_USBDI(21, W, DRIVER_CLASS_DEINIT)

/****************************************************************************\
|
|	Messages to function objects
|
\****************************************************************************/

/* initialize a driver function (instance) object */
#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_INIT				\
	__TMS_USBPUMP_IOCTL_USBDI(30, W, FUNCTION_INIT)

/* de-initialize a driver function (instance) object */
#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT			\
	__TMS_USBPUMP_IOCTL_USBDI(31, W, FUNCTION_DEINIT)

/* change power state a driver function (instance) object */
#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC	\
	__TMS_USBPUMP_IOCTL_USBDI(32, W_ASYNC, FUNCTION_POWER_STATE_CHANGE_ASYNC)

/* set/reset hub port over-current state a driver function (instance) object */
#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE	\
	__TMS_USBPUMP_IOCTL_USBDI(33, W, FUNCTION_SET_OVER_CURRENT_STATE)

/* get composite port objects */
#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS		\
	__TMS_USBPUMP_IOCTL_USBDI(34, RW, FUNCTION_GET_COMPOSITE_PORTS)

/****************************************************************************\
|
|	Messages to port objects
|
\****************************************************************************/

/* get the URB size for a given connection */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE			\
	__TMS_USBPUMP_IOCTL_USBDI(40, RW, PORT_GET_URB_SIZE)

/* mark a function as idle */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION			\
	__TMS_USBPUMP_IOCTL_USBDI(41, W, PORT_IDLE_FUNCTION)

/* mark a function as suspend */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION			\
	__TMS_USBPUMP_IOCTL_USBDI(42, W, PORT_SUSPEND_FUNCTION)

/* mark a function as resume */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION			\
	__TMS_USBPUMP_IOCTL_USBDI(43, W, PORT_RESUME_FUNCTION)

/* mark a port as unidle */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT		\
	__TMS_USBPUMP_IOCTL_USBDI(44, W, PORT_SET_IDLE_OTG_PORT)

/* match and notify */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY			\
	__TMS_USBPUMP_IOCTL_USBDI(45, RW, PORT_MATCH_AND_NOTIFY)

/* get port status */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS			\
	__TMS_USBPUMP_IOCTL_USBDI(46, RW, PORT_GET_PORT_STATUS)

/* make a port as LPM L1 (Sleep)  */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC			\
	__TMS_USBPUMP_IOCTL_USBDI(47, W_ASYNC, PORT_L1_SLEEP_ASYNC)

/* mark a function as suspend asynchronous */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC			\
	__TMS_USBPUMP_IOCTL_USBDI(48, W_ASYNC, PORT_SUSPEND_ASYNC)

/* mark a function as resume asynchronous */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC			\
	__TMS_USBPUMP_IOCTL_USBDI(49, W_ASYNC, PORT_RESUME_ASYNC)

/* change power state a port object */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC		\
	__TMS_USBPUMP_IOCTL_USBDI(50, W_ASYNC, PORT_POWER_STATE_CHANGE_ASYNC)

/* get auto LPM mode enable */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE		\
	__TMS_USBPUMP_IOCTL_USBDI(51, RW, PORT_GET_AUTO_LPM_ENABLE)

/* set auto LPM mode enable */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE		\
	__TMS_USBPUMP_IOCTL_USBDI(52, W, PORT_SET_AUTO_LPM_ENABLE)

/* set enable / disable wake hardware capability */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE		\
	__TMS_USBPUMP_IOCTL_USBDI(53, W, PORT_ENABLE_WAKE_HARDWARE)

/* set enable / disable wait SRP when connect OTG capable device */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP			\
	__TMS_USBPUMP_IOCTL_USBDI(54, W, PORT_ENABLE_WAIT_SRP)

/* set OTG status change */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE		\
	__TMS_USBPUMP_IOCTL_USBDI(55, W, PORT_SET_OTG_STATUS_CHANGE)

/* unbind/rebind function driver */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION			\
	__TMS_USBPUMP_IOCTL_USBDI(56, W, PORT_UNBIND_FUNCTION)

/* set/reset port over-current state */
#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE		\
	__TMS_USBPUMP_IOCTL_USBDI(57, W, PORT_SET_OVER_CURRENT_STATE)

/****************************************************************************\
|
|	The EDGE IOCTLs are used for notifications from USBDI to its
|	managed objects.
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_EDGE_USBDI(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_USBDI_EDGE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_USBDI_, NAME, _ARG) \
		)

/* upwards notifications */
#define __TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL			\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI(0, W, INSTANCE_ARRIVAL)

#define __TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC		\
	__TMS_USBPUMP_IOCTL_W_ASYNC(					\
		__TMS_USBPUMP_IOCTL_CLASS_USBDI_EDGE,			\
		1,							\
		__TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG \
		)

#define __TMS_USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND			\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI(2, W, DEVICE_SUSPEND)

/* get USBD configuration information */
#define	__TMS_USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION		\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI(3, RW, GET_USBD_CONFIGURATION)

/* select configuration */
#define	__TMS_USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION		\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI(4, RW, SELECT_CONFIGURATION)

/****************************************************************************\
|
|	The IOCTL details follow
|
\****************************************************************************/

/*

IOCTL:	USBPUMP_IOCTL_USBDI_REGISTER_CLASS

Index:	Type:	USBPUMP_IOCTL_USBDI_REGISTER_CLASS_ARG

Function:
	Register a class driver with USBDI.

Input:
	USBPUMP_USBDI_DRIVER_CLASS *pClass;
		Points to the class object to be registered.

Output:
	No output parameters.

Description:
	This IOCTL is issued on behalf of the class driver object
	during initialization by UsbPumpUsbdiLib_CreateDriverClass(),
	in order to notify USBDI of the newly arrived class.  It's
	an IOCTL in order to accommodate remote USBDI implementations.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if USBDI or a proxy couldn't be
		found in the dynamic parent of this object
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V1.91h).

Setup Macro:
	None.

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

See also:
	UsbPumpUsbdiLib_CreateDriverClass()

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_REGISTER_CLASS_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_REGISTER_CLASS_ARG)
	{
	__TMS_USBPUMP_USBDI_DRIVER_CLASS	*pClass;
	};

/*

IOCTL:	USBPUMP_IOCTL_USBDI_UNREGISTER_CLASS

Index:	Type:	USBPUMP_IOCTL_USBDI_UNREGISTER_CLASS_ARG

Function:
	De-register a class driver with USBDI.

Input:
	USBPUMP_USBDI_DRIVER_CLASS *pClass;
		Points to the class object to be de-registered.

Output:
	No output parameters.

Description:
	This IOCTL is issued on behalf of the class driver object
	during de-initialization by UsbPumpUsbdiLib_DeleteDriverClass(),
	in order to notify USBDI of the newly arrived class.  It's
	an IOCTL in order to accommodate remote USBDI implementations.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if USBDI or a proxy couldn't be
		found in the dynamic parent of this object
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V1.91h).

Setup Macro:
	None.

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

See also:
	UsbPumpUsbdiLib_DeleteDriverClass()

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_UNREGISTER_CLASS_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_UNREGISTER_CLASS_ARG)
	{
	__TMS_USBPUMP_USBDI_DRIVER_CLASS	*pClass;
	};

/*

IOCTL:	USBPUMP_IOCTL_USBDI_START

Index:	IOCTL:	USBPUMP_IOCTL_USBDI_STOP
	IOCTL:	USBPUMP_IOCTL_USBDI_GETSTATE
	Type:	USBPUMP_IOCTL_USBDI_STARTSTOP_ARG

Function:
	Start, stop or query run status of USBDI

Input:
	No input parameters.

Output:
	BOOL RunState;
		For GETSTATE, this is set TRUE if USBDI is now
		running, and FALSE otherwise.

		For Start/Stop, this is set TRUE if USBDI was running
		prior to this call, and FALSE otherwise.

Description:
	This IOCTL is issued by system management software to control
	the run/stop status of USBD.  Normally, USBD is started after
	all class drivers have been installed; individual busses or
	ports are then started and stopped using OTG or transceiver APIs
	over a given port instance.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if USBDI or a proxy couldn't be
		found in the dynamic parent of this object
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V1.91h).

Setup Macro:
	None.

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

	The state change doesn't necessarily take effect instantaneously.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_STARTSTOP_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_STARTSTOP_ARG)
	{
	/* set TRUE if was running; FALSE if was stopped */
	__TMS_BOOL	RunState;
	};

/*

IOCTL:	USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER

Index:	Type:	USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG
	Type:	USBPUMP_USBDI_DEVICE_ARRIVAL_LISTENER_FN
	Type:	PUSBPUMP_USBDI_DEVICE_ARRIVAL_LISTENER_FN
	Type:	USBPUMP_USBDI_DEVICE_ARRIVAL_INFO
	Type:	PUSBPUMP_USBDI_DEVICE_ARRIVAL_INFO
	Type:	USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT
	Type:	PUSBPUMP_USBDI_DEVICE_ARRIVAL_RESULT

Function:
	Register an alternative class driver mechanism with USBD.

Input:
	USBPUMP_USBDI_USBD *			pUsbd;
	USBPUMP_USBDI_DEVICE_ARRIVAL_LISTENER_FN *pListenerFn;
	VOID *					pListenerContext;

Output:
	No output parameters.

Description:
	This IOCTL should be issued by the client while USBD is stopped.
	It allows a client to supply a "listener function" that will be called
	by USBD whenever it has found a new device, before USBD searches
	its internal tables.  It's intended for use by operating system
	bindings where the OS is able to load class drivers dynamically.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

	Only one listener may be registered at a time for a given USBDI.

	pListenerFn points to a client-supplied function with the following
	prototype:

	USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT (*pListenerFn)(
		CONST USBPUMP_USBDI_DEVICE_ARRIVAL_INFO *pInfo
		);

	The listener function consults the data in *pInfo.  If it wishes
	to disable the device, it returns FALSE.  If it wishes to provide
	an external driver, USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_USE_EXTERNAL.
	If it wishes the DataPump to try to use internal drivers, it
	returns USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_USE_INTERNAL.  Otherwise, 
	it returns USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_DISABLE, which causes
	the DataPump to act as if no driver was found (no internal matching
	is attempted).

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if USBDI or a proxy couldn't be
		found in the dynamic parent of this object
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V3.01a).

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG *pArg,
		USBPUMP_USBDI_USBD *				pUsbd,
		USBPUMP_USBDI_DEVICE_ARRIVAL_LISTENER_FN *	pListenerFn,
		VOID *						pListenerContext
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG)
	{
	__TMS_USBPUMP_USBDI_USBD *			pUsbd;
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_LISTENER_FN *pListenerFn;
	__TMS_VOID *					pListenerContext;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)							\
    do	{								\
    	(a_pArg)->pUsbd = (a_pUsbd);					\
    	(a_pArg)->pListenerFn = (a_pListenerFn);			\
    	(a_pArg)->pListenerContext = (a_pListenerContext);		\
    	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER

Index:	Type:	USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG
	Type:	USBPUMP_USBDI_DEVICE_DEPARTURE_LISTENER_FN
	Type:	PUSBPUMP_USBDI_DEVICE_DEPARTURE_LISTENER_FN
	Type:	USBPUMP_USBDI_DEVICE_DEPARTURE_INFO
	Type:	PUSBPUMP_USBDI_DEVICE_DEPARTURE_INFO

Function:
	Register an alternative class driver mechanism with USBD.

Input:
	USBPUMP_USBDI_USBD *				pUsbd;
	USBPUMP_USBDI_DEVICE_DEPARTURE_LISTENER_FN *	pListenerFn;
	VOID *						pListenerContext;

Output:
	No output parameters.

Description:
	This IOCTL should be issued by the client while USBD is stopped.
	It allows a client to supply a "listener function" that will be called
	by USBD whenever it has found a device instance departure, before
	USBD sends USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC.
	It's intended for use by operating system bindings where the OS is
	able to unload class drivers dynamically.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

	Only one listener may be registered at a time for a given USBDI.

	pListenerFn points to a client-supplied function with the following
	prototype:

	VOID (*pListenerFn)(
		CONST USBPUMP_USBDI_DEVICE_DEPARTURE_INFO *pInfo
		);

	The listener function consults the data in *pInfo.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if USBDI or a proxy couldn't be
		found in the dynamic parent of this object
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V3.01a).

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG *pArg,
		USBPUMP_USBDI_USBD *				pUsbd,
		USBPUMP_USBDI_DEVICE_DEPARTURE_LISTENER_FN *	pListenerFn,
		VOID *						pListenerContext
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG)
	{
	__TMS_USBPUMP_USBDI_USBD *			pUsbd;
	__TMS_USBPUMP_USBDI_DEVICE_DEPARTURE_LISTENER_FN *pListenerFn;
	__TMS_VOID *					pListenerContext;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)							\
    do	{								\
    	(a_pArg)->pUsbd = (a_pUsbd);					\
    	(a_pArg)->pListenerFn = (a_pListenerFn);			\
    	(a_pArg)->pListenerContext = (a_pListenerContext);		\
    	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER

Index:	Type:	USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG
	Type:	USBPUMP_USBDI_STOP_COMPLETION_LISTENER_FN
	Type:	PUSBPUMP_USBDI_STOP_COMPLETION_LISTENER_FN

Function:
	Register a listener function for USBD stop completion.

Input:
	USBPUMP_USBDI_USBD *				pUsbd;
	USBPUMP_USBDI_STOP_COMPLETION_LISTENER_FN *	pListenerFn;
	VOID *						pListenerContext;

Output:
	No output parameters.

Description:
	It allows a client to supply a "listener function" that will be called
	by USBD when USBD stop processing is completed.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

	Only one listener may be registered at a time for a given USBDI.

	pListenerFn points to a client-supplied function with the following
	prototype:

	VOID (*pListenerFn)(
		VOID *pListenerContext,
		USBPUMP_USBDI_USBD *pUsbd
		);

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if USBDI or a proxy couldn't be
		found in the dynamic parent of this object
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V3.01c).

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG *pArg,
		USBPUMP_USBDI_USBD *				pUsbd,
		USBPUMP_USBDI_STOP_COMPLETION_LISTENER_FN *	pListenerFn,
		VOID *						pListenerContext
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG)
	{
	__TMS_USBPUMP_USBDI_USBD *				pUsbd;
	__TMS_USBPUMP_USBDI_STOP_COMPLETION_LISTENER_FN *	pListenerFn;
	__TMS_VOID *						pListenerContext;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)							\
    do	{								\
    	(a_pArg)->pUsbd = (a_pUsbd);					\
    	(a_pArg)->pListenerFn = (a_pListenerFn);			\
    	(a_pArg)->pListenerContext = (a_pListenerContext);		\
    	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS

Index:	Type:	USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG

Function:
	Override USBD configuration flags.

Input:
	USBPUMP_USBDI_USBD *	pUsbd;
	UINT32			ulFlagMask;
	UINT32			ulFlagBits;

Output:
	UINT32			ulConfigFlags;

Description:
	It allows a client to override USBD configuration flags.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if USBDI or a proxy couldn't be
		found in the dynamic parent of this object
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V3.01f).

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG *pArg,
		USBPUMP_USBDI_USBD *	pUsbd,
		UINT32			ulFlagMask,
		UINT32			ulFlagBits
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG)
	{
	__TMS_USBPUMP_USBDI_USBD *	pUsbd;		/* IN: USBD pointer */
	__TMS_UINT32			ulFlagMask;	/* IN: Clear bits */
	__TMS_UINT32			ulFlagBits;	/* IN: Set bits */
	__TMS_UINT32			ulConfigFlags;	/* OUT: new flags */
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG_SETUP_V1(	\
		a_pArg,							\
		a_pUsbd,						\
		a_ulFlagMask,						\
		a_ulFlagBits						\
		)							\
    do	{								\
	(a_pArg)->pUsbd = (a_pUsbd);					\
	(a_pArg)->ulFlagMask = (a_ulFlagMask);				\
	(a_pArg)->ulFlagBits = (a_ulFlagBits);				\
	(a_pArg)->ulConfigFlags = 0;					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_DRIVER_CLASS_INIT

Index:	Type:	USBPUMP_IOCTL_USBDI_DRIVER_CLASS_INIT_ARG

Function:
	Notify driver-class code that a class object has arrived.

Input:
	USBPUMP_USBDI_DRIVER_CLASS *pClass;
		Pointer to the class object, taken as a generic
		USBDI class object.

Output:
	No output parameters.

Description:
	This IOCTL is issued by the standard initialization code
	in UsbPumpUsbdiLib_CreateDriverClass() when the class object has
	been created, prior to issuing the USBPUMP_IOCTL_USBDI_REGISTER_CLASS
	IOCTL.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success.  A default method
	is provided in USBDI, which will return this value if the
	class driver ioctl function is not provided or doesn't claim
	the IOCTL.

	If the class init code fails the IOCTL, then the class will
	be de-registered and the creation will fail.

Setup Macro:
	None

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_DRIVER_CLASS_INIT_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_DRIVER_CLASS_INIT_ARG)
	{
	__TMS_USBPUMP_USBDI_DRIVER_CLASS	*pClass;
	};

/*

IOCTL:	USBPUMP_IOCTL_USBDI_DRIVER_CLASS_DEINIT

Index:	Type:	USBPUMP_IOCTL_USBDI_DRIVER_CLASS_DEINIT_ARG

Function:
	Notify driver-class code that a class object has deleted.

Input:
	USBPUMP_USBDI_DRIVER_CLASS *pClass;
		Pointer to the class object, taken as a generic
		USBDI class object.

Output:
	No output parameters.

Description:
	This IOCTL is issued by USBDI to the class object, to
	notify the instance object that it has been deleted.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success.  A default method
	is provided in USBDI, which will return this value if the
	class driver ioctl function is not provided or doesn't claim
	the IOCTL.

Setup Macro:
	None

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBDI_DRIVER_CLASS_DEINIT_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBDI_DRIVER_CLASS_DEINIT_ARG)
	{
	__TMS_USBPUMP_USBDI_DRIVER_CLASS *pClass;
	};

/*

IOCTL:	USBPUMP_IOCTL_USBDI_FUNCTION_INIT

Index:	Type:	USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG
	Name:	USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG_SETUP_V1()

Function:
	Notifies a function (instance) object that it has been created
	and initialized (but not yet bound to a device).

Input:
	USBPUMP_USBDI_DRIVER_CLASS *pClass;
		Pointer to the owning class object.  Of course, one
		could find this from the pointer in the function, but
		this reduces the scope of the type-cast.

	USBPUMP_USBDI_FUNCTION *pFunction;
		Pointer to the new instance object.

Output:
	No output parameters.

Description:
	This IOCTL is issued by USBDI to the function instance object, to
	notify the instance object that it has been created and linked into
	the object system.  The instance should perform any necessary pre-
	attach initialization, and should return USBPUMP_IOCTL_RESULT_SUCCESS.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS, or an error code.  Note that the error
	path may not have been extensively tested in V1.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG *pArg,
		USBPUMP_USBDI_DRIVER_CLASS *pClass,
		USBPUMP_USBDI_FUNCTION *pFunction
		);

Notes:

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG)
	{
	__TMS_USBPUMP_USBDI_DRIVER_CLASS	*pClass;
	__TMS_USBPUMP_USBDI_FUNCTION		*pFunction;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClass,						\
	a_pFunction						\
	)							\
   do	{							\
	(a_pArg)->pClass = (a_pClass);				\
	(a_pArg)->pFunction = (a_pFunction);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT

Index:	Type:	USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG
	Name:	USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG_SETUP_V1()

Function:
	Notifies a function (instance) object that it has been deleted.

Input:
	USBPUMP_USBDI_DRIVER_CLASS *pClass;
		Pointer to the owning class object.  Of course, one
		could find this from the pointer in the function, but
		this reduces the scope of the type-cast.

	USBPUMP_USBDI_FUNCTION *pFunction;
		Pointer to the new instance object.

Output:
	No output parameters.

Description:
	This IOCTL is issued by USBDI to the function instance object, to
	notify the instance object that it has been deleted.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS, or an error code.  Note that the error
	path may not have been extensively tested in V1.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG *pArg,
		USBPUMP_USBDI_DRIVER_CLASS *pClass,
		USBPUMP_USBDI_FUNCTION *pFunction
		);

Notes:

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG)
	{
	__TMS_USBPUMP_USBDI_DRIVER_CLASS *pClass;
	__TMS_USBPUMP_USBDI_FUNCTION *pFunction;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClass,						\
	a_pFunction						\
	)							\
   do	{							\
	(a_pArg)->pClass = (a_pClass);				\
	(a_pArg)->pFunction = (a_pFunction);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1()

Function:
	Change power state of the given function

Input:
	USBPUMP_USBDI_FUNCTION *pFunction;
		The USBPUMP_USBDI_FUNCTION of interest.

	USBPUMP_USBDI_POWER_STATE OldPowerState;
		Old power state.

	USBPUMP_USBDI_POWER_STATE NewPowerState;
		New power state.

	USBPUMP_USBDI_POWER_ACTION PowerAction;
		Power action.

	BOOL fWakeEnabled;
		TRUE if remote wakeup is enabled.

Description:
	This IOCTL is sent from the client to change power state of the
	function object.
	The HCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or

	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG *pArg,
		USBPUMP_USBDI_FUNCTION *pFunction,
		USBPUMP_USBDI_POWER_STATE OldPowerState,
		USBPUMP_USBDI_POWER_STATE NewPowerState,
		BOOL fWakeEnabled
		);

*/

/**** parameters to HCD_POWER_STATE_CHANGE_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *		pFunction;	/* IN */
	__TMS_USBPUMP_USBDI_POWER_STATE 	OldPowerState;
	__TMS_USBPUMP_USBDI_POWER_STATE 	NewPowerState;
	__TMS_USBPUMP_USBDI_POWER_ACTION	PowerAction;
	__TMS_BOOL				fWakeEnabled;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pFunction,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_PowerAction,						\
	a_fWakeEnabled						\
	)							\
    do	{							\
	(a_pArg)->pFunction = (a_pFunction);			\
	(a_pArg)->OldPowerState = (a_OldPowerState);		\
	(a_pArg)->NewPowerState = (a_NewPowerState);		\
	(a_pArg)->PowerAction = (a_PowerAction);		\
	(a_pArg)->fWakeEnabled = (a_fWakeEnabled);		\
	} while (0)

#define	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pFunction,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_fWakeEnabled						\
	)							\
	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pFunction,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	/* V2: PowerAction */ __TMS_USBPUMP_USBDI_POWER_ACTION_NONE,	\
	a_fWakeEnabled						\
	)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE

Index:	Type:	USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE_ARG
	Name:	USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE_ARG_SETUP_V1()

Function:
	Set hub port over-current state for a given function.

Input:
	USBPUMP_USBDI_FUNCTION *pFunction;
		The USBPUMP_USBDI_FUNCTION of interest.

	UINT uPortNumber;
		Port number of given hub function instance.

	BOOL fSetOverCurrent;
		If it is TRUE, set over-current state for the hub port. If it is
		FALSE, clear over-current state for the hub port.

Output:
	No output fields.

Description:
	This IOCTL is issued by client to the function instance object, to
	set or clear hub port's over-current state for a given function.
	A given function should be hub function instance and 

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE(
		USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		BOOL fSetOverCurrent
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE_ARG)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_UINT			uPortNumber;
	__TMS_BOOL			fSetOverCurrent;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction,							\
	a_uPortNumber,							\
	a_fSetOverCurrent						\
	)								\
   do	{								\
	(a_pArg)->pFunction = (a_pFunction);				\
	(a_pArg)->uPortNumber = (a_uPortNumber);			\
	(a_pArg)->fSetOverCurrent = (a_fSetOverCurrent);		\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS

Index:	Type:	USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS_ARG
	Name:	USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS_ARG_SETUP_V1()

Function:
	Get composite ports own by a given function.

Input:
	USBPUMP_USBDI_FUNCTION *pFunction;
		The USBPUMP_USBDI_FUNCTION of interest.

Output:
	UINT nCompositePorts;
		Number of composite ports own by pFunction.

	USBPUMP_USBDI_PORT **pvCompositePorts;
		Array of the composite port pointer.

Description:
	This IOCTL is issued by client to the function instance object, to
	get composite ports own by a given function. The pFunction should be
	composite function instance.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS(
		USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS_ARG *pArg,
		USBPUMP_USBDI_FUNCTION *pFunction
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS_ARG)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_UINT			nCompositePorts;
	__TMS_USBPUMP_USBDI_PORT **	pvCompositePorts;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction							\
	)								\
   do	{								\
	(a_pArg)->pFunction = (a_pFunction);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG_SETUP_V1()

Function:
	Fetches information about URBs and HCDs for a given device
	from USBDI.

Input:
	USBPUMP_USBDI_PORT *pPort;
		Pointer to the port object in question.

Output:
	RECSIZE UrbExtraSize;
		Extra bytes needed for URBs sent to this port.

	RECSIZE HcdRequestSize;
		Size needed for an HCD request.

Description:
	This API allows a function driver or other client of a port to
	obtain key information about the connection/path to the HCD.
	This in turn allows the client to allocate URBs that have enough
	room to accommodate the entire path through the stack.

	UrbExtraSize should be added to sizeof(USBPUMP_URB) to get
	the number of bytes to allocate for the URB.  HcdRequestSize
	should be used to compute hte value to be placed in HcdRequestIndex
	within each USBPUMP_URB that is issued to this port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if issued to a USBPUMP_USBDI_PORT.

	Some failure or not-claimed code if issued to an inappropriate
	object.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG *pArg,
		USBPUMP_IOCTL_USBDI_PORT *pPort
		);

Notes:
	Some of this information is also available from the connection
	notification.

See Also:
	USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL

*/

/**** parameters to GET_URB_SIZE ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *pPort; /* IN: pointer to USBDI port object */
	__TMS_RECSIZE	UrbExtraSize;	/* OUT: required URB extra size in bytes */
	__TMS_RECSIZE	HcdRequestSize;	/* OUT: portion assigned to HCD req */
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort								\
	)								\
    do	{								\
	(a_pArg)->pPort = (a_pPort);					\
									\
	/* in theory, this is wasted motion -- but this work makes */	\
	/* the initial arg value predictable, which can help for */	\
	/* trace-based comparisons; it also makes it possible to */	\
	/* make assertions about the final values of UrbExtraSize */	\
	/* and HcdRequestSize */					\
	(a_pArg)->UrbExtraSize = 0;					\
	(a_pArg)->HcdRequestSize = 0;					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) that the function driver is through
	working with the port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	USBPUMP_USBDI_FUNCTION	*pFunction;
		The issuing function.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI that the
	function driver instance is through working with the device.  This
	causes the driver to be detached from the device.  After this IOCTL
	completes, USBDI will park the instance and will cease using the
	device.  If the device is on a root OTG hub, and HNP is enabled and
	sensible, then USBDI will attempt to hand host control over to the
	other device

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION(
		USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		USBPUMP_USBDI_FUNCTION *pFunction
		);

Notes:
	This is a syncronous IOCTL; but note that the actual OTG
	functionality might be remotely located, so it may take a while
	for this actually to have effect.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT	*pPort;
	__TMS_USBPUMP_USBDI_FUNCTION	*pFunction;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->pFunction = (a_pFunction);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) that the function driver suspend the port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	USBPUMP_USBDI_FUNCTION	*pFunction;
		The issuing function.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI that the
	function driver instance is suspending the device. To support remote
	wakeup, class driver should issue the set-feature(RWUP) to the device
	and call this IOCTL.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION(
		USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		USBPUMP_USBDI_FUNCTION *pFunction
		);

Notes:
	This is a syncronous IOCTL; but note that the actual OTG
	functionality might be remotely located, so it may take a while
	for this actually to have effect.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT	*pPort;
	__TMS_USBPUMP_USBDI_FUNCTION	*pFunction;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->pFunction = (a_pFunction);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) that the function driver resume the port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	USBPUMP_USBDI_FUNCTION	*pFunction;
		The issuing function.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI that the
	function driver instance is resuming the device.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION(
		USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		USBPUMP_USBDI_FUNCTION *pFunction
		);

Notes:
	This is a syncronous IOCTL; but note that the actual OTG
	functionality might be remotely located, so it may take a while
	for this actually to have effect.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT	*pPort;
	__TMS_USBPUMP_USBDI_FUNCTION	*pFunction;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->pFunction = (a_pFunction);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) that set idle state for the OTG port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	BOOL fSetIdle;
		If TRUE, it tells to make an idle state for the OTG port.
		If FALSE, it means make unidle sate for the OTG port.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI that the
	port set the idle state with fSetIdle.  If the port is not OTG port,
	it will not do anything.  The HCD or PHY will use this IOCTL for
	handling SRP / Insertion based device.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT(
		USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		BOOL fSetIdle
		);

Notes:
	This is a syncronous IOCTL; but note that the actual OTG
	functionality might be remotely located, so it may take a while
	for this actually to have effect.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_BOOL			fSetIdle;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fSetIdle							\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fSetIdle = (a_fSetIdle);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) to match and bind a function for the port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	CONST TEXT *pDeviceId;
		Pointer to the device ID that was assigned to this device.

	BYTES nDeviceId;
		Size of the device ID buffer.

	CONST TEXT *pInstanceId;
		Pointer to the instance ID -- this is actually
		a null-terminated UTF8 encoding of the serial number.

	BYTES nInstanceId;
		Size of the instance ID buffer.

Output:
	USBPUMP_USBDI_FUNCTION *pFunction;
		Bound function pointer.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI to match and
	bind a function for the given port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY(
		USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		CONST TEXT *pDeviceId,
		BYTES nDeviceId,
		CONST TEXT *pInstanceId,
		BYTES nInstanceId
		);

*/

__TMS_TYPE_DEF_STRUCT  (USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_CONST __TMS_TEXT *	pDeviceId;
	__TMS_BYTES			nDeviceId;
	__TMS_CONST __TMS_TEXT *	pInstanceId;
	__TMS_BYTES			nInstanceId;
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction; /* OUT: bound function */
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pDeviceId,							\
	a_nDeviceId,							\
	a_pInstanceId,							\
	a_nInstanceId							\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->pDeviceId = (a_pDeviceId);				\
	(a_pArg)->nDeviceId = (a_nDeviceId);				\
	(a_pArg)->pInstanceId = (a_pInstanceId);			\
	(a_pArg)->nInstanceId = (a_nInstanceId);			\
	(a_pArg)->pFunction = __TMS_NULL;				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG_SETUP_V1()

Function:
	Fetches information about port status flags for a given port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		Pointer to the port object in question.

Output:
	UINT32	PortStatus;
		Port status flags:
			USBPUMP_USBDI_PORT_STATUS_ENABLED
			USBPUMP_USBDI_PORT_STATUS_CONNECTED

Description:
	This API allows a function driver or other client of a port to
	obtain port status flag information.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if issued to a USBPUMP_USBDI_PORT.

	Some failure or not-claimed code if issued to an inappropriate
	object.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG *pArg,
		USBPUMP_IOCTL_USBDI_PORT *pPort
		);

Notes:
	Some of this information is also available from the connection
	notification.

*/

__TMS_TYPE_DEF_STRUCT  (USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *pPort; /* IN: pointer to USBDI port object */
	__TMS_UINT32		PortStatus;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort								\
	)								\
    do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->PortStatus = 0;					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) that the function driver sleep the port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	BOOL fRemoteWakeEnable;
		TRUE if enable remote wakeup.

	UINT ResumeDelay;
		Resume dealy in usec. If this value is zero, it will use
		default value from USBD configuration.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI that the
	function driver instance is sleeping the device.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC(
		USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		BOOL fRemoteWakeEnable,
		UINT ResumeDelay
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_BOOL			fRemoteWakeEnable;
	__TMS_UINT			ResumeDelay;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fRemoteWakeEnable,						\
	a_ResumeDelay							\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fRemoteWakeEnable = (a_fRemoteWakeEnable);		\
	(a_pArg)->ResumeDelay = (a_ResumeDelay);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) that the function driver suspend the port.

Input:
	USBPUMP_USBDI_FUNCTION	*pFunction;
		The issuing function.

	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	BOOL fDoUnconfigBeforeSuspend;
		If this flag is TRUE and current configuration is not NULL,
		USBDI undefines current configuration before suspend port.

	BOOL fSendRemoteWakeupEnable;
		If this flag is TRUE and device support remote wakeup, USBDI
		will send SET_FEATURE(DeviceRemoteWakeup) before suspend port.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI that the
	function driver instance is suspending the device. To support remote
	wakeup, class driver should issue the set-feature(RWUP) to the device
	and call this IOCTL.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC(
		USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		USBPUMP_USBDI_FUNCTION *pFunction,
		BOOL fDoUnconfigBeforeSuspend
		);

Notes:
	This is an asyncronous IOCTL; IOCTL completion will be called when
	port suspend is completed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT	*pPort;
	__TMS_USBPUMP_USBDI_FUNCTION	*pFunction;
	__TMS_BOOL			fDoUnconfigBeforeSuspend;
	__TMS_BOOL			fSendRemoteWakeupEnable;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction,							\
	a_fDoUnconfigBeforeSuspend,					\
	a_fSendRemoteWakeupEnable					\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->pFunction = (a_pFunction);				\
	(a_pArg)->fDoUnconfigBeforeSuspend = (a_fDoUnconfigBeforeSuspend); \
	(a_pArg)->fSendRemoteWakeupEnable = (a_fSendRemoteWakeupEnable); \
	} while (0)

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction,							\
	a_fDoUnconfigBeforeSuspend					\
	)								\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction,							\
	a_fDoUnconfigBeforeSuspend,					\
	/* V2: fSendRemoteWakeupEnable */ __TMS_FALSE			\
	)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) that the function driver resume the port.

Input:
	USBPUMP_USBDI_FUNCTION	*pFunction;
		The issuing function.

	USBPUMP_USBDI_PORT *pPort;
		The governing port.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI that the
	function driver instance is resuming the device.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC(
		USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		USBPUMP_USBDI_FUNCTION *pFunction
		);

Notes:
	This is an asyncronous IOCTL; IOCTL completion will be called when
	port resume is completed.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT	*pPort;
	__TMS_USBPUMP_USBDI_FUNCTION	*pFunction;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->pFunction = (a_pFunction);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1()

Function:
	Change power state of the given port

Input:
	USBPUMP_USBDI_PORT *pPort;
		The USBPUMP_USBDI_PORT of interest.

	USBPUMP_USBDI_POWER_STATE OldPowerState;
		Old power state.

	USBPUMP_USBDI_POWER_STATE NewPowerState;
		New power state.

	USBPUMP_USBDI_POWER_ACTION PowerAction;
		Power action.

	BOOL fWakeEnabled;
		TRUE if remote wakeup is enabled.

Description:
	This IOCTL is sent from the client to change power state of the
	port object.
	The HCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or

	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		USBPUMP_USBDI_POWER_STATE OldPowerState,
		USBPUMP_USBDI_POWER_STATE NewPowerState,
		BOOL fWakeEnabled
		);

*/

/**** parameters to HCD_POWER_STATE_CHANGE_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *		pPort;	/* IN */
	__TMS_USBPUMP_USBDI_POWER_STATE 	OldPowerState;
	__TMS_USBPUMP_USBDI_POWER_STATE 	NewPowerState;
	__TMS_USBPUMP_USBDI_POWER_ACTION	PowerAction;
	__TMS_BOOL				fWakeEnabled;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pPort,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_PowerAction,						\
	a_fWakeEnabled						\
	)							\
    do	{							\
	(a_pArg)->pPort = (a_pPort);				\
	(a_pArg)->OldPowerState = (a_OldPowerState);		\
	(a_pArg)->NewPowerState = (a_NewPowerState);		\
	(a_pArg)->PowerAction = (a_PowerAction);		\
	(a_pArg)->fWakeEnabled = (a_fWakeEnabled);		\
	} while (0)

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pPort,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_fWakeEnabled						\
	)							\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pPort,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	/* V2: PowerAction */ __TMS_USBPUMP_USBDI_POWER_ACTION_NONE,	\
	a_fWakeEnabled						\
	)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG

Function:
	Get automatic LPM enable information of a given port

Input:
        USBPUMP_USBDI_PORT *	pPort;
                The USBPUMP_USBDI_PORT of interest.

Output:
	BOOL			fEnabled;
	UINT			HirdInMicroseconds;
	UINT			IdleInMicroseconds;

Description:
	It allows a client to read automatic LPM mode enable information
	of a given port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success if input is a 
	USBPUMP_USBDI_PORT

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG *pArg,
		USBPUMP_USBDI_PORT *			pPort
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;			/* IN */
	__TMS_BOOL			fEnabled;		/* OUT */
	__TMS_UINT			HirdInMicroseconds;	/* OUT */
	__TMS_UINT			IdleInMicroseconds;	/* OUT */
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort							\
		)							\
    do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fEnabled = __TMS_FALSE;				\
	(a_pArg)->HirdInMicroseconds = 0;				\
	(a_pArg)->IdleInMicroseconds = 0;				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG

Function:
	Set automatic LPM enable information of a given port

Input:
	USBPUMP_USBDI_PORT *	pPort;
	BOOL			fEnable;
	UINT			HirdInMicroseconds;
	UINT			IdleInMicroseconds;

Output:
	No output parameters.

Description:
	It allows a client to modify automatic LPM mode enable information
	of a given port.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success if the object is
	USBPUMP_USBDI_PORT.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG *pArg,
		USBPUMP_USBDI_PORT *	pPort,
		BOOL			fEnable,
		UINT			HirdInMicroseconds,
		UINT			IdleInMicroseconds
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;			/* IN */
	__TMS_BOOL			fEnable;		/* IN */
	__TMS_UINT			HirdInMicroseconds;	/* IN */
	__TMS_UINT			IdleInMicroseconds;	/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_fEnable,						\
		a_HirdInMicroseconds,					\
		a_IdleInMicroseconds					\
		)							\
    do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fEnable = (a_fEnable);				\
	(a_pArg)->HirdInMicroseconds = (a_HirdInMicroseconds);		\
	(a_pArg)->IdleInMicroseconds = (a_IdleInMicroseconds);		\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG

Function:
	Set wake hardware capability enable or disable for a given port

Input:
	USBPUMP_USBDI_PORT *pPort;
		The USBPUMP_USBDI_PORT of interest.

	USBPUMP_USBDI_PORT_ENABLE_WAKE_HARDWARE_ALERT_FN *pAlertFn;
		The remote wakeup alert function pointer.  If pAlertFn is not
		NULL, it will enable wake hardware capability. When port driver
		detect remote wakeup, it will call pAlertFn to signal a wakeup.
		If pAlertFn is NULL, it will disable wake hardware capability.

	VOID *pContext;
		The context pointer of alert function.

Description:
	It allows a client to enable or disable hardware wakeup notification
	from a given port.

	After successfully enabling wakeup, the client expects that the
	port driver will call pAlertFn to signal a wakeup.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success if the object is
	USBPUMP_USBDI_PORT.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		USBPUMP_USBDI_PORT_ENABLE_WAKE_HARDWARE_ALERT_FN *pAlertFn,
		VOID *pContext
		);

*/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_PORT_ENABLE_WAKE_HARDWARE_ALERT_FN,
__TMS_VOID,
	(
	__TMS_VOID *			/* pContext */,
	__TMS_USBPUMP_USBDI_PORT *	/* pPort */,
	__TMS_USTAT			/* Status */
	));

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;		/* IN */
	__TMS_USBPUMP_USBDI_PORT_ENABLE_WAKE_HARDWARE_ALERT_FN *pAlertFn;
	__TMS_VOID *			pContext;	/* IN */
	
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_pAlertFn,						\
		a_pContext						\
		)							\
    do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->pAlertFn = (a_pAlertFn);				\
	(a_pArg)->pContext = (a_pContext);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG

Function:
	Set enable or disable wait SRP feature when connect OTG capable device
	for a given port

Input:
	USBPUMP_USBDI_PORT *pPort;
		The USBPUMP_USBDI_PORT of interest.

	BOOL fWaitSrpEnable;
		If it is TRUE, driver turn port power off and wait SRP from device.
		If it is FALSE, driver enumerate device without SRP.
		

Description:
	It allows a client to enable or disable wait SRP feature for the OTG
	device from a given port.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success if the object is
	USBPUMP_USBDI_PORT.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		BOOL fWaitSrpEnable
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;			/* IN */
	__TMS_BOOL			fWaitSrpEnable;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG_SETUP_V1(	\
		a_pArg,							\
		a_pPort,						\
		a_fWaitSrpEnable					\
		)							\
    do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fWaitSrpEnable = (a_fWaitSrpEnable);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG
	Type:	PUSBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG

Function:
	Set OTG status change for a given port

Input:
	USBPUMP_USBDI_PORT *pPort;
		The USBPUMP_USBDI_PORT of interest.

	UINT fIdStatusChanged: 1;
		ID pin status changed.

	UINT fIdStatus: 1;
		Current ID pin status. It is valid if fIdStatusChanged is set.

	UINT fSrpDetected: 1;
		SRP from device detected.

	UINT fHnpOk: 1;
		HNP finished successfully.

	UINT fHnpError: 1;
		HNP finished with error.

	UINT fHnpFinished: 1;
		HNP finished by B device.

Description:
	This IOCTL will be sent by HCD or PHY to notify OTG status changes.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success if the object is
	USBPUMP_USBDI_PORT.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG_SETUP_V1(
		USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		BOOL fIdStatusChanged,
		BOOL fIdStatus,
		BOOL fSrpDetected
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_UINT			fIdStatusChanged: 1;
	__TMS_UINT			fIdStatus: 1;
	__TMS_UINT			fSrpDetected: 1;
	__TMS_UINT			fHnpOk: 1;
	__TMS_UINT			fHnpError: 1;
	__TMS_UINT			fHnpFinished: 1;
	};

#define	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_fIdStatusChanged,					\
		a_fIdStatus,						\
		a_fSrpDetected,						\
		a_fHnpOk,						\
		a_fHnpError,						\
		a_fHnpFinished						\
		)							\
    do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fIdStatusChanged = (a_fIdStatusChanged);		\
	(a_pArg)->fIdStatus = (a_fIdStatus);				\
	(a_pArg)->fSrpDetected = (a_fSrpDetected);			\
	(a_pArg)->fHnpOk = (a_fHnpOk);					\
	(a_pArg)->fHnpError = (a_fHnpError);				\
	(a_pArg)->fHnpFinished = (a_fHnpFinished);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V1()

Function:
	Notify USBDI (via a port) that the function driver is unbinded with
	the port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	BOOL fUnbindFunction;
		If it is TRUE, unbind function driver with the port.  If it is
		FALSE, USBDI will try to re-bind function driver.

	BOOL fSuspendPort;
		This flag is only valid if fUnbindFunction is TRUE.
		If it is TRUE, gevin port will be suspended after unbind
		function driver is done.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to notify USBDI that the
	function driver instance is unbinded with the port.  If fUnbindFunction
	is TRUE, USBDI will unbind function driver with the port.  If it is
	FALSE, USBDI will try to re-bind function driver with the port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V2(
		USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		BOOL fUnbindFunction,
		BOOL fSuspendPort
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_BOOL			fUnbindFunction;
	__TMS_BOOL			fSuspendPort;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPort,							\
	a_fUnbindFunction,						\
	a_fSuspendPort							\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fUnbindFunction = (a_fUnbindFunction);		\
	(a_pArg)->fSuspendPort = (a_fSuspendPort);			\
	} while (0)

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fUnbindFunction						\
	)								\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPort,							\
	a_fUnbindFunction,						\
	/* V2: fSuspendPort */ __TMS_FALSE				\
	)

/*

IOCTL:	USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE

Index:	Type:	USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE_ARG
	Name:	USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE_ARG_SETUP_V1()

Function:
	Set port over-current state for a given port.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	BOOL fSetOverCurrent;
		If it is TRUE, set over-current state for the port.  If it is
		FALSE, clear over-current state for the port.

Output:
	No output fields.

Description:
	Sent to USBDI via a USBPUMP_USBDI_PORT to set or clear port over-
	current state for a given port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	some other error code for failure.

Setup Macro:
	VOID USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE(
		USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		BOOL fSetOverCurrent
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_BOOL			fSetOverCurrent;
	};

#define __TMS_USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE_ARG_SETUP_V1( \
	a_pArg,								\
	a_pPort,							\
	a_fSetOverCurrent						\
	)								\
   do	{								\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fSetOverCurrent = (a_fSetOverCurrent);		\
	} while (0)


/****************************************************************************\
|
|	The USBD EDGE IOCTL codes -- these are sent to USBDI objects.
|
\****************************************************************************/

/* in addition to open / close */

/*

IOCTL:	USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL

Index:	Type:	USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_ARG
	Name:	USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_ARG_SETUP_V1()

Function:
	Notifies a function driver that it's been bound to a device instance.

Input:
	USBPUMP_USBDI_FUNCTION *pFunction;
		Pointer to the function object that's the target of
		this operation.  Useful if this IOCTL is being filtered.

	USBPUMP_USBDI_INSTANCE_INFO Info;
		A structure containing information about the arriving
		instance.  Info.pPort points to the USBPUMP_USBDI_PORT
		to be used for communicating with the device.   Info.PortKey
		gives the port key, which is changed for every different
		arrival on a given port.  Info.hDefaultPipe gives the
		pipe handle for the default pipe of the device.
		Info.UrbExtraSize and Info.HcdRequestSize give the extra
		information that's needed for preparing URBs to go to
		the device.

Output:
	No output parameters.

Description:
	This IOCTL is sent to a USBPUMP_USBDI_FUNCTION object after
	it has been attached to a given port by USBDI in response
	to a device arrival.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if the instance can accept the
	connection.

	Some other error code if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_SETUP_V1(
		USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_ARG *pArg,
		USBPUMP_USBDI_FUNCTION *	pFunction,
		USBPUMP_USBDI_PORT *		pPort,
		USBPUMP_USBDI_PORT_KEY		PortKey,
		USBPUMP_USBDI_PIPE_HANDLE	hDefaultPipe,
		RECSIZE				UrbExtraSize,
		RECSIZE				HcdRequestSize
		);

See Also:
	USBPUMP_USBDI_INSTANCE_INFO.

*/


/**** parameters to INSTANCE_ARRIVAL -- same as USBDI_INSTANCE_INFO ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_ARG)
	{
	__TMS_USBPUMP_USBDI_FUNCTION		*pFunction;
	__TMS_USBPUMP_USBDI_INSTANCE_INFO	Info;
	};

#define __TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_hDefaultPipe,							\
	a_UrbExtraSize,							\
	a_HcdRequestSize						\
	)								\
    do	{								\
	(a_pArg)->pFunction = (a_pFunction);				\
	(a_pArg)->Info.pPort = (a_pPort);				\
	(a_pArg)->Info.hDefaultPipe = (a_hDefaultPipe);			\
	(a_pArg)->Info.PortKey = (a_PortKey);				\
	(a_pArg)->Info.UrbExtraSize = (a_UrbExtraSize);			\
	(a_pArg)->Info.HcdRequestSize = (a_HcdRequestSize);		\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC

Index:	Type:	USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG
	Name:	USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG_SETUP_V1()

Function:
	Notify a function driver that the device connected to its port has
	been removed.

Input:
	USBPUMP_USBDI_FUNCTION *pFunction;
		The function that's supposed to be receiving the message.
		This is provided for convenience in case multiple layers
		are filtering the requests.

	USBPUMP_USBDI_PORT *pPort;
		The port of reference.

Output:
	No result.

Description:
	When USBDI detects that a device has been removed, it
	sends this message to the code that manages the affected instance
	object.  The reciever must do any work needed to notify the
	OS-specific clients, release any resources, unlink any dynamically-
	allocated data structure from system lists, and perform any other
	required cleanup prior to returning.  When the IOCTL is completed,
	the USB system will do the work needed to delete the instance.
	Note that the deletion will be deferred so that the caller of
	UsbPumpIoctlQe_Complete() will be able to exit before the rug
	is pulled out from under the caller's feet.  (The caller should
	not, however try to do anything except get out of the way, however;
	but for debug prints, etc, it's much easier if they have the chance
	to have some live context still available.)

Returns:
	The function driver cannot fail this request: the device is gone.
	It shall return USBPUMP_IOCTL_RESULT_SUCCESS.  The function
	driver shall call UsbPUmpIoctlQe_Complete() either during initial
	processing or subsequently when deferred processing is complete.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG *pArg,
		USBPUMP_USBDI_FUNCTION *pFunction,
		USBPUMP_USBDI_PORT *pPort
		);

Notes:

*/


/**** parameters to INSTANCE_DEPARTURE_ASYNC ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *pFunction; /* pointer to function */
	__TMS_USBPUMP_USBDI_PORT *pPort;	/* pointer to port */
	};

#define __TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction,							\
	a_pPort								\
	)								\
    do	{								\
	(a_pArg)->pFunction = (a_pFunction);				\
	(a_pArg)->pPort = (a_pPort);					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND

Index:	Type:	USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG
	Name:	USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG_SETUP_V1()

Function:
	Notifies a function instance that it's port has been suspended or
	resumed.

Input:
	USBPUMP_USBDI_PORT *pPort;

	USBPUMP_USBDI_FUNCTION *pFunction;
		Pointer to the function attached to this port.

	BOOL fSuspended;
		TRUE if the device is now suspended; FALSE if
		the device is now running.

Output:
	No output parameters.

Description:
	When this request arrives at the device, the bus segment for the
	port device has already changed state.  This request gives the
	device driver a chance to present appropriate behavior to its upper
	edge.  Unlike Windows, any request that is pending at the time of
	the suspend stays pending, and will be resumed after the suspend
	is over; so handling this is really up to the device.

	Any pending requests are suspended before the bus is suspended.
	Any suspended requests are resumed after this function returns.

Returns:
	The function driver is not permitted to fail this request; however,
	it need not implement it.  Therefore the permitted results are:

	USBPUMP_IOCTL_RESULT_SUCCESS for success;

	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if there's not any code to handle
	this.

Setup Macro:
	USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG *pArg,
		USBPUMP_USBDI_FUNCTION *pFunction,
		USBPUMP_USBDI_PORT *pPort,
		BOOL fSuspended
		);

Notes:


*/

/**** parameters to DEVICE_SUSPEND ****/
__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *pFunction; /* IN: the function */
	__TMS_USBPUMP_USBDI_PORT *pPort;	/* IN: pointer to USBD port */
	__TMS_BOOL fSuspended;			/* IN: if true, now suspended,
						||     otherwise resumed
						*/
	};

#define	__TMS_USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pFunction,							\
	a_pPort,							\
	a_fSuspended							\
	)								\
    do	{								\
	(a_pArg)->pFunction = (a_pFunction);				\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->fSuspended = (a_fSuspended);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION

Index:	Type:	USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG
	Type:	PUSBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG

Function:
	Get USBD configuration information from client.

Input:
	USBPUMP_USBDI_USBD_CONFIG	Config;

Output:
	USBPUMP_USBDI_USBD_CONFIG	Config;

Description:
	It allows a client to override USBD configuration information at the
	run-time. It will pass current configuration info for reference.
	This IOCTL will be sent by generic host initialization to allow
	a client to override USBD configuration.

	The IOCTL may be sent to any object, provided that the object is
	descended from the USBD instance of interest.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if USBDI or a proxy couldn't be
		found in the dynamic parent of this object
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V3.01c).

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG *pArg,
		USBPUMP_USBDI_USBD_CONFIG *pUsbd
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG)
	{
	__TMS_USBPUMP_USBDI_USBD_CONFIG	Config;		/* IN / OUT */
	};

#define	__TMS_USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG_SETUP_V1( \
		a_pArg,							\
		a_pConfig						\
		)							\
    do	{								\
	UHIL_cpybuf(&(a_pArg)->Config, (a_pConfig), sizeof(*(a_pConfig))); \
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION

Index:	Type:	USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG
	Type:	PUSBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG

Function:
	Ask client to select configuration from class driver.

Input:
	USBPUMP_USBDI_PORT *pPort;
		The governing port.

	USBPUMP_USBDI_FUNCTION *pFunction;
		Pointer to the function attached to this port.

Output:
	BOOL fSelectConfig;
		Select configuration with iConfiguration.  If fSelectConfig is
		TRUE, the class driver sets configuration with iConfiguration
		value.  If fSelectConfig is FALSE, the class driver does not
		select configuration and the device is in unconfigured state.
		The client should set up fSuspendPort flag if this is FALSE.

	UINT8 iConfiguration;
		Selected configuration index. This value is valid only if
		fSelectConfig is TRUE.

	BOOL fSuspendPortIfUnselect;
		Tells the class driver to suspend port. This flag is valid
		only if the fSelectConfig is FALSE.  If fSuspendPortIfUnselect
		is TRUE, the class driver will suspend port.
		If fSuspendPortIfUnselect is FALSE, the class driver will not
		set configuration. 

	BOOL fResetPortBeforeSelect;
		Tells the class driver to reset port before select the device
		configuration.  This flag is valid only if the fSelectConfig
		is TRUE.

Description:
	It allows a client to select the device configuration at the run-time.
	This IOCTL will be sent by generic class driver (or other class driver)
	to allow a client to select the device configuration.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if a client doesn't support this.
	Possibly (in the future) some other error codes, but none at
	time of writing this documentation (V3.11c).

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG *pArg,
		USBPUMP_USBDI_PORT *pPort,
		USBPUMP_USBDI_FUNCTION *pFunction
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;			/* IN */
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;		/* IN */
	__TMS_BOOL			fSelectConfig;		/* OUT */
	__TMS_UINT8			iConfiguration;		/* OUT */
	__TMS_BOOL			fSuspendPortIfUnselect;	/* OUT */
	__TMS_BOOL			fResetPortBeforeSelect;	/* OUT */
	};

#define	__TMS_USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG_SETUP_V1( \
	a_pArg,							\
	a_pPort,						\
	a_pFunction						\
	)							\
    do	{							\
	(a_pArg)->pPort = (a_pPort);				\
	(a_pArg)->pFunction = (a_pFunction);			\
	} while (0)

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_CLASS_USBDI_USBDI	\
   __TMS_USBPUMP_IOCTL_CLASS_USBDI_USBDI
# define USBPUMP_IOCTL_CLASS_USBDI_DRIVER_CLASS	\
   __TMS_USBPUMP_IOCTL_CLASS_USBDI_DRIVER_CLASS
# define USBPUMP_IOCTL_CLASS_USBDI_FUNCTION	\
   __TMS_USBPUMP_IOCTL_CLASS_USBDI_FUNCTION
# define USBPUMP_IOCTL_CLASS_USBDI_PORT	\
   __TMS_USBPUMP_IOCTL_CLASS_USBDI_PORT
# define USBPUMP_IOCTL_CLASS_USBDI_INTERNAL	\
   __TMS_USBPUMP_IOCTL_CLASS_USBDI_INTERNAL
# define USBPUMP_IOCTL_USBDI(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_USBDI(i, t, NAME)
# define USBPUMP_IOCTL_USBDI_REGISTER_CLASS	\
   __TMS_USBPUMP_IOCTL_USBDI_REGISTER_CLASS
# define USBPUMP_IOCTL_USBDI_UNREGISTER_CLASS	\
   __TMS_USBPUMP_IOCTL_USBDI_UNREGISTER_CLASS
# define USBPUMP_IOCTL_USBDI_START	\
   __TMS_USBPUMP_IOCTL_USBDI_START
# define USBPUMP_IOCTL_USBDI_STOP	\
   __TMS_USBPUMP_IOCTL_USBDI_STOP
# define USBPUMP_IOCTL_USBDI_GETSTATE	\
   __TMS_USBPUMP_IOCTL_USBDI_GETSTATE
# define USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER	\
   __TMS_USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER
# define USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER	\
   __TMS_USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER
# define USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER	\
   __TMS_USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER
# define USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS	\
   __TMS_USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS
# define USBPUMP_IOCTL_USBDI_DRIVER_CLASS_INIT	\
   __TMS_USBPUMP_IOCTL_USBDI_DRIVER_CLASS_INIT
# define USBPUMP_IOCTL_USBDI_DRIVER_CLASS_DEINIT	\
   __TMS_USBPUMP_IOCTL_USBDI_DRIVER_CLASS_DEINIT
# define USBPUMP_IOCTL_USBDI_FUNCTION_INIT	\
   __TMS_USBPUMP_IOCTL_USBDI_FUNCTION_INIT
# define USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT	\
   __TMS_USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT
# define USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC
# define USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE	\
   __TMS_USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE
# define USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS	\
   __TMS_USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS
# define USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE
# define USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION
# define USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION
# define USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION
# define USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT
# define USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY
# define USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS
# define USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC
# define USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC
# define USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC
# define USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC
# define USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE
# define USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE
# define USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE
# define USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP
# define USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE
# define USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION
# define USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE	\
   __TMS_USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE
# define USBPUMP_IOCTL_EDGE_USBDI(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_EDGE_USBDI(i, t, NAME)
# define USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL	\
   __TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL
# define USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC	\
   __TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC
# define USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND	\
   __TMS_USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND
# define USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION	\
   __TMS_USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION
# define USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION	\
   __TMS_USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION
# define USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_ARRIVAL_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)
# define USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_DEVICE_DEPARTURE_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)
# define USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_REGISTER_USBDI_STOP_LISTENER_ARG_SETUP_V1( \
		a_pArg,							\
		a_pUsbd,						\
		a_pListenerFn,						\
		a_pListenerContext					\
		)
# define USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG_SETUP_V1(	\
		a_pArg,							\
		a_pUsbd,						\
		a_ulFlagMask,						\
		a_ulFlagBits						\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_OVERRIDE_CONFIG_FLAGS_ARG_SETUP_V1(	\
		a_pArg,							\
		a_pUsbd,						\
		a_ulFlagMask,						\
		a_ulFlagBits						\
		)
# define USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClass,						\
	a_pFunction						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_INIT_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClass,						\
	a_pFunction						\
	)
# define USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClass,						\
	a_pFunction						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_DEINIT_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pClass,						\
	a_pFunction						\
	)
# define USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pFunction,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_PowerAction,						\
	a_fWakeEnabled						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pFunction,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_PowerAction,						\
	a_fWakeEnabled						\
	)
# define USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pFunction,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_fWakeEnabled						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pFunction,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_fWakeEnabled						\
	)
# define USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction,							\
	a_uPortNumber,							\
	a_fSetOverCurrent						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_SET_OVER_CURRENT_STATE_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction,							\
	a_uPortNumber,							\
	a_fSetOverCurrent						\
	)
# define USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_FUNCTION_GET_COMPOSITE_PORTS_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort								\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_URB_SIZE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort								\
	)
# define USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_IDLE_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_RESUME_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fSetIdle							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_IDLE_OTG_PORT_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fSetIdle							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pDeviceId,							\
	a_nDeviceId,							\
	a_pInstanceId,							\
	a_nInstanceId							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_MATCH_AND_NOTIFY_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pDeviceId,							\
	a_nDeviceId,							\
	a_pInstanceId,							\
	a_nInstanceId							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort								\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_PORT_STATUS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort								\
	)
# define USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fRemoteWakeEnable,						\
	a_ResumeDelay							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_L1_SLEEP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fRemoteWakeEnable,						\
	a_ResumeDelay							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction,							\
	a_fDoUnconfigBeforeSuspend,					\
	a_fSendRemoteWakeupEnable					\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction,							\
	a_fDoUnconfigBeforeSuspend,					\
	a_fSendRemoteWakeupEnable					\
	)
# define USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction,							\
	a_fDoUnconfigBeforeSuspend					\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_SUSPEND_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction,							\
	a_fDoUnconfigBeforeSuspend					\
	)
# define USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_RESUME_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_pFunction							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pPort,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_PowerAction,						\
	a_fWakeEnabled						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pPort,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_PowerAction,						\
	a_fWakeEnabled						\
	)
# define USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pPort,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_fWakeEnabled						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pPort,						\
	a_OldPowerState,					\
	a_NewPowerState,					\
	a_fWakeEnabled						\
	)
# define USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort							\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_GET_AUTO_LPM_ENABLE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort							\
		)
# define USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_fEnable,						\
		a_HirdInMicroseconds,					\
		a_IdleInMicroseconds					\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_AUTO_LPM_ENABLE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_fEnable,						\
		a_HirdInMicroseconds,					\
		a_IdleInMicroseconds					\
		)
# define USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_pAlertFn,						\
		a_pContext						\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_pAlertFn,						\
		a_pContext						\
		)
# define USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG_SETUP_V1(	\
		a_pArg,							\
		a_pPort,						\
		a_fWaitSrpEnable					\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_ENABLE_WAIT_SRP_ARG_SETUP_V1(	\
		a_pArg,							\
		a_pPort,						\
		a_fWaitSrpEnable					\
		)
# define USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_fIdStatusChanged,					\
		a_fIdStatus,						\
		a_fSrpDetected,						\
		a_fHnpOk,						\
		a_fHnpError,						\
		a_fHnpFinished						\
		)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_OTG_STATUS_CHANGE_ARG_SETUP_V1( \
		a_pArg,							\
		a_pPort,						\
		a_fIdStatusChanged,					\
		a_fIdStatus,						\
		a_fSrpDetected,						\
		a_fHnpOk,						\
		a_fHnpError,						\
		a_fHnpFinished						\
		)
# define USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPort,							\
	a_fUnbindFunction,						\
	a_fSuspendPort							\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pPort,							\
	a_fUnbindFunction,						\
	a_fSuspendPort							\
	)
# define USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fUnbindFunction						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_UNBIND_FUNCTION_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pPort,							\
	a_fUnbindFunction						\
	)
# define USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE_ARG_SETUP_V1( \
	a_pArg,								\
	a_pPort,							\
	a_fSetOverCurrent						\
	)	\
	__TMS_USBPUMP_IOCTL_USBDI_PORT_SET_OVER_CURRENT_STATE_ARG_SETUP_V1( \
	a_pArg,								\
	a_pPort,							\
	a_fSetOverCurrent						\
	)
# define USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_hDefaultPipe,							\
	a_UrbExtraSize,							\
	a_HcdRequestSize						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_ARRIVAL_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_hDefaultPipe,							\
	a_UrbExtraSize,							\
	a_HcdRequestSize						\
	)
# define USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction,							\
	a_pPort								\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI_INSTANCE_DEPARTURE_ASYNC_ARG_SETUP_V1( \
	a_pArg,								\
	a_pFunction,							\
	a_pPort								\
	)
# define USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pFunction,							\
	a_pPort,							\
	a_fSuspended							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI_DEVICE_SUSPEND_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pFunction,							\
	a_pPort,							\
	a_fSuspended							\
	)
# define USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG_SETUP_V1( \
		a_pArg,							\
		a_pConfig						\
		)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI_GET_USBD_CONFIGURATION_ARG_SETUP_V1( \
		a_pArg,							\
		a_pConfig						\
		)
# define USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG_SETUP_V1( \
	a_pArg,							\
	a_pPort,						\
	a_pFunction						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_USBDI_SELECT_CONFIGURATION_ARG_SETUP_V1( \
	a_pArg,							\
	a_pPort,						\
	a_pFunction						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_ioctls.h ****/
#endif /* _USBPUMP_USBDI_IOCTLS_H_ */
