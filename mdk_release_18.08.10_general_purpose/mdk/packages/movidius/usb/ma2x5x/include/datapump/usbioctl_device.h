/* usbioctl_device.h	Fri Oct 24 2014 11:26:59 chwon */

/*

Module:  usbioctl_device.h

Function:
	IOCTL codes for device objects.

Version:
	V3.13b	Fri Oct 24 2014 11:26:59 chwon	Edit level 17

Copyright notice:
	This file copyright (C) 2004-2007, 2009-2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	August 2004

Revision history:
   1.89b  Sun Aug 08 2004 11:35:10  chwon
	Module created.

   1.89b  Thu Sep 02 2004 15:31:04  chwon
	Add library functions of device IOCTL

   1.91c  Tue May 24 2005 15:36:27  chwon
	Add more device IOCTLs

   1.93b  Mon Nov 07 2005 15:31:29  chwon
	Add USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT.

   1.97i  Wed May 16 2007 11:23:04  chwon
	2812: add USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE

   2.01a  Fri Feb 27 2009 13:38:31  maw
   	7473: Add HARDWARE_INIT, HARDWARE_DEINIT, START and
   	STOP Ioctl

   2.01a  Tue Apr 14 2009 23:55:11  maw
   	7473: Add _ASYNC to IOCTL defs

   3.01c  Fri Aug 20 2010 23:07:08  cccho
	10109: Add USBPUMP_IOCTL_DEVICE_DELAY.

   3.01d  Thu Sep 02 2010 12:28:33  cccho
	6150, 8071: Add USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE.

   3.01d  Mon Dec 13 2010 15:32:47  maw
   	10093: Add USBPUMP_IOCTL_DEVICE_ALLOC_COMMON_BUFFER and
   		USBPUMP_IOCTL_DEVICE_FREE_COMMON_BUFFER

   3.01d  Tue Dec 14 2010 23:11:57  maw
   	10093: Move defs of USBPUMP_IOCTL_DEVICE_ALLOC_COMMON_BUFFER and
   		USBPUMP_IOCTL_DEVICE_FREE_COMMON_BUFFER to usbioctlcodes.h

   3.01f  Fri Jul 01 2011 14:02:15  maw
   	13233: Add fDetectEndpontActivity to
   	USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE

   3.01f  Thu Jan 05 2012 12:51:43  chwon
	14623: Add fLtmCapable to USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE

   3.01f  Thu Apr 19 2012 22:52:28  ssyang
	15208: Add USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC and
	USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN.

   3.11b  Tue Jan 08 2013 15:12:16  chwon
	16402: Added USBPUMP_IOCTL_DEVICE_SET_MS_OS_DESCRIPTOR_PROCESS and
	USBPUMP_IOCTL_DEVICE_GET_MS_OS_DESCRIPTOR_PROCESS ioctl.

   3.11c  Tue Feb 26 2013 22:31:05  chwon
	16402: 16435: Added USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN
	ioctl.  Removed USBPUMP_IOCTL_DEVICE_SET_MS_OS_DESCRIPTOR_PROCESS and
	USBPUMP_IOCTL_DEVICE_GET_MS_OS_DESCRIPTOR_PROCESS ioctl.

   3.13b  Fri Oct 24 2014 11:26:59  chwon
	18600: Fix doc-xml error.

*/

#ifndef _USBIOCTL_DEVICE_H_		/* prevent multiple includes */
#define _USBIOCTL_DEVICE_H_

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

/****************************************************************************\
|
|	Define the IOCTL codes
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_DEVICE(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_DEVICE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_DEVICE_, NAME, _ARG)	\
		)

/* Vbus state check function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE		\
	__TMS_USBPUMP_IOCTL_DEVICE(0, RW, GET_VBUS_STATE)

/* power-state check function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE		\
	__TMS_USBPUMP_IOCTL_DEVICE(1, RW, GET_SELF_POWER_STATE)

/* pulsing remote-wakeup pin function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP		\
	__TMS_USBPUMP_IOCTL_DEVICE(2, W, REMOTE_WAKEUP)

/* D+ pullup check function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE	\
	__TMS_USBPUMP_IOCTL_DEVICE(3, RW, GET_USB_CONNECT_STATE)

/* D+ pullup control function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE	\
	__TMS_USBPUMP_IOCTL_DEVICE(4, RW, SET_USB_CONNECT_STATE)

/* initialize USB hardware function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE		\
	__TMS_USBPUMP_IOCTL_DEVICE(5, W, INIT_USB_HARDWARE)

/* USB clock check function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE		\
	__TMS_USBPUMP_IOCTL_DEVICE(6, RW, GET_USB_CLOCK_STATE)

/* USB clock control function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE		\
	__TMS_USBPUMP_IOCTL_DEVICE(7, RW, SET_USB_CLOCK_STATE)

/* uncached buffer allocate function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER		\
	__TMS_USBPUMP_IOCTL_DEVICE(8, RW, ALLOC_UNCACHE_BUFFER)

/* clear USB external interrupt function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT		\
	__TMS_USBPUMP_IOCTL_DEVICE(9, RW, CLEAR_USB_INTERRUPT)

/* battery charging state control function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE	\
	__TMS_USBPUMP_IOCTL_DEVICE(10, RW, SET_USB_CHARGING_STATE)

/* USB interrupt control function */
#define	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT	\
	__TMS_USBPUMP_IOCTL_DEVICE(11, RW, SET_USB_INTERRUPT)

/* clear STALL but preserve data toggle */
#define	__TMS_USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE	\
	__TMS_USBPUMP_IOCTL_DEVICE(12, W, CLEAR_STALL_PRESERVE_TOGGLE)

#define	__TMS_USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC		\
	__TMS_USBPUMP_IOCTL_DEVICE(13, W_ASYNC, HARDWARE_INIT_ASYNC)

#define	__TMS_USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC	\
	__TMS_USBPUMP_IOCTL_DEVICE(14, W_ASYNC, HARDWARE_DEINIT_ASYNC)

#define	__TMS_USBPUMP_IOCTL_DEVICE_START_ASYNC	\
	__TMS_USBPUMP_IOCTL_DEVICE(15, W_ASYNC, START_ASYNC)

#define	__TMS_USBPUMP_IOCTL_DEVICE_STOP_ASYNC	\
	__TMS_USBPUMP_IOCTL_DEVICE(16, W_ASYNC, STOP_ASYNC)

#define	__TMS_USBPUMP_IOCTL_DEVICE_DELAY	\
	__TMS_USBPUMP_IOCTL_DEVICE(17, W, DELAY)

#define	__TMS_USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE	\
	__TMS_USBPUMP_IOCTL_DEVICE(18, RW, GET_HW_FEATURE_DEVICE)

#define	__TMS_USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC	\
	__TMS_USBPUMP_IOCTL_DEVICE(19, W_ASYNC, SET_POWER_STATE_ASYNC)

#define	__TMS_USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN	\
	__TMS_USBPUMP_IOCTL_DEVICE(20, RW, REGISTER_POWER_REQUEST_FN)

/* register control packet process function in the DataPump core */
#define	__TMS_USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN	\
	__TMS_USBPUMP_IOCTL_DEVICE(21, RW, REGISTER_CONTROL_PROCESS_FN)

/* codes 64 to 128 are reserved for the "device-specific" ifc layer */
#define	__TMS_USBPUMP_IOCTL_DEVICE_IFC_BASE	64

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE

Index:	Type:	USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to get USB VBUS state

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG structure.

	USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fConnect;
		OUT: TRUE if USB VBUS connected, FALSE if disconnected.

Description:
	This IOCTL is sent from an USB chip interface to get USB VBUS connection
	state.  It returns the VBUS connection state.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_GET_VBUS_STATE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_BOOL		fConnect;
	};

/* the setup for the .._DEVICE_GET_VBUS_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDevice							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->fConnect = __TMS_TRUE;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE

Index:	Type:	USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to get USB self-power state

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG structure.

	USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fSelfPower;
		OUT: TRUE if self-powered device, FALSE if bus-powered device

Description:
	This IOCTL is sent from an USB chip interface to get USB self-powered
	device or bus-powered device.  It returns TRUE if self-powered device.
	It returns FALSE if bus-powered device.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_GET_SELF_POWER_STATE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_BOOL		fSelfPower;
	};

/* the setup for the .._DEVICE_GET_SELF_POWER_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->fSelfPower = __TMS_TRUE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP

Index:	Type:	USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG
	Name:	USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to pulse remote wakeup pin

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG
			structure.

	pOutParam	not used, should be NULL.

	USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

Description:
	This IOCTL is sent from an USB chip interface to pulse remote wakeup
	pin.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_REMOTE_WAKEUP_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	};

/* the setup for the .._DEVICE_REMOTE_WAKEUP_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDevice							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE

Index:	Type:	USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to get USB D+ pull-up state

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG structure.

	USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fEnable;
		OUT: TRUE if D+ pull-up enabled, FALSE if disabled

Description:
	This IOCTL is sent from an USB chip interface to get USB D+ pull-up state.
	It returns TRUE if USB D+ pull-up enabled.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_GET_USB_CONNECT_STATE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_BOOL		fEnable;
	};

/* the setup for the .._DEVICE_GET_USB_CONNECT_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->fEnable = __TMS_TRUE;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE

Index:	Type:	USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to set USB D+ pull-up state

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG structure.

	USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fEnable;
		IN: TRUE if enable D+ pull-up, FALSE if disable

	BOOL	fPrevState;
		OUT: previous USB D+ pull-up state

Description:
	This IOCTL is sent from an USB chip interface to set USB D+ pull-up state.
	It returns the previous USB D+ pull-up state.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_SET_USB_CONNECT_STATE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_BOOL		fEnable;
	__TMS_BOOL		fPrevState;
	};

/* the setup for the .._DEVICE_SET_USB_CONNECT_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->fEnable = (ARG_fEnable);				\
	__pArg->fPrevState = __TMS_TRUE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE

Index:	Type:	USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to initialize USB hardware

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG
			structure.

	pOutParam	not used, should be NULL.

	USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

Description:
	This IOCTL is sent from an USB chip interface to initialize USB hardware.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_INIT_USB_HARDWARE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	};

/* the setup for the .._DEVICE_INIT_USB_HARDWARE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE

Index:	Type:	USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to get USB clock state

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG structure.

	USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fEnable;
		OUT: TRUE if USB clock enabled, FALSE if disabled

Description:
	This IOCTL is sent from an USB chip interface to get USB clock state.
	It returns TRUE if USB clock enabled.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_GET_USB_CLOCK_STATE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_BOOL		fEnable;
	};

/* the setup for the .._DEVICE_GET_USB_CLOCK_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->fEnable = __TMS_TRUE;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE

Index:	Type:	USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to set USB clock state

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG structure.

	USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fEnable;
		IN: TRUE if enable USB clock, FALSE if disable

	BOOL	fPrevState;
		OUT: previous USB clock state

Description:
	This IOCTL is sent from an USB chip interface to set USB clock state.
	It returns the previous USB clock state.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_SET_USB_CLOCK_STATE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_BOOL		fEnable;
	__TMS_BOOL		fPrevState;
	};

/* the setup for the .._DEVICE_SET_USB_CLOCK_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->fEnable = (ARG_fEnable);				\
	__pArg->fPrevState = __TMS_TRUE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER

Index:	Type:	USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG
	Name:	USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to allocate uncached memory buffer

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG structure.

	USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	UINT32	nBytes;
		IN: number of bytes

	PVOID	pBuffer;
		OUT: pointer of uncached buffer

Description:
	This IOCTL is sent from an USB chip interface to allocate uncached
	memory buffer.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG *pArg,
		UDEVICE *pDevice,
		UINT32 nBytes
		);

	This macro correctly initializes a .._DEVICE_ALLOC_UNCACHE_BUFFER_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_UINT32		nBytes;
	__TMS_PVOID		pBuffer;
	};

/* the setup for the .._DEVICE_ALLOC_UNCACHE_BUFFER_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_nBytes							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->nBytes = (ARG_nBytes);					\
	__pArg->pBuffer = __TMS_NULL;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT

Index:	Type:	USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG
	Name:	USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to clear USB external interrupt

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG structure.

	USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	UHIL_INTERRUPT_RESOURCE_HANDLE	hExtInt;
		IN: USB external interrupt handle

	BOOL	fIntStatus;
		OUT: specified external interrupt status

Description:
	This IOCTL is sent from an USB chip interface to clear USB external
	interrupt in primary ISR.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG *pArg,
		UDEVICE *pDevice,
		UHIL_INTERRUPT_RESOURCE_HANDLE hExtInt
		);

	This macro correctly initializes a .._DEVICE_CLEAR_USB_INTERRUPT_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG)
	{
	__TMS_UDEVICE *				pDevice;
	__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE	hExtInt;
	__TMS_BOOL				fIntStatus;
	};

/* the setup for the .._PNX5220_CLEAR_USB_INTERRUPT_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_hExtInt							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->hExtInt = (ARG_hExtInt);				\
	__pArg->fIntStatus = __TMS_TRUE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE

Index:	Type:	USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to set battery charging state

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG structure.

	USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fEnable;
		IN: TRUE if enable battery charging, FALSE if disable

	BOOL	fPrevState;
		OUT: previous battery charging state

Description:
	This IOCTL is sent from an USB chip interface to set battery charging
	state using USB VBUS.  It returns the previous battery charging state.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_SET_USB_CHARGING_STATE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_BOOL		fEnable;
	__TMS_BOOL		fPrevState;
	};

/* the setup for the .._DEVICE_SET_USB_CHARGING_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->fEnable = (ARG_fEnable);				\
	__pArg->fPrevState = __TMS_TRUE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT

Index:	Type:	USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG
	Name:	USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to control USB interrupt

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG structure.

	USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fEnable;
		IN: TRUE if enable USB interrupt, FALSE if disable

	BOOL	fPrevState;
		OUT: previous USB interrupt state

Description:
	This IOCTL is sent from an USB chip interface to enable / disable USB
	interrupt.  It returns the previous USB interrupt state.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG *pArg,
		UDEVICE *pDevice
		);

	This macro correctly initializes a .._DEVICE_SET_USB_INTERRUPT_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_BOOL		fEnable;
	__TMS_BOOL		fPrevState;
	};

/* the setup for the .._DEVICE_SET_USB_INTERRUPT_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->fEnable = (ARG_fEnable);				\
	__pArg->fPrevState = __TMS_TRUE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE

Index:	Type:	USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG_SETUP_V1

Function:
	IOCTL issued by client to clear STALL condition and preserve 
	current data toggle

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG
			structure.

	pOutParam	not used, should be NULL.

	USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG has the following
	elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure
	UENDPOINT *pUep;
		IN: points to the UENDPOINT structure

Description:
	This IOCTL is sent from the client to clear STALL condition and
	preserve current data toggle.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG *pArg,
		UDEVICE *pDevice,
		UENDPOINT *pUep
		);

	This macro correctly initializes a
	..._DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG structure.
	If the layout of the argument structure changes in the future,
	MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	__TMS_UENDPOINT *	pUep;
	};

/* the setup for the .._DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice,							\
	ARG_pUep							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG *	\
		__TMS_CONST	__pArg = (pArg);			\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->pUep = (ARG_pUep);					\
   	} while (0)



/*

IOCTL:	USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC

Index:	Type:	USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC
	Name:	USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC_ARG_SETUP_V1()

Function:
	Initialize USB device controller hardware

Input:
	PUDEVICE pDevice;
		The USB device controller of interest.

Description:
	This IOCTL is sent from the client to initialize USB device 
	controller hardware, but without attaching to interrupt. 
	The DCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware is already in use (USBPUMP_IOCTL_RESULT_HARDWARE_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(
		PUSBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC_ARG pArg,
		PUDEVICE pDevice
		);

*/

/**** parameters to DEVICE_HARDWARE_INIT_ASYNC_ ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC_ARG)
	{
	__TMS_PUDEVICE	pDevice;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice							\
	)								\
    do	{								\
	(a_pArg)->pDevice = (a_pDevice);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC

Index:	Type:	USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC
	Name:	USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1()

Function:
	De-initialize USB device controller hardware

Input:
	PUDEVICE pDevice;
		The USB device controller of interest.

Description:
	This IOCTL is sent from the client to de-initialize USB device 
	controller hardware, but without attaching to interrupt. 
	The DCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(
		PUSBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC_ARG pArg,
		PUDEVICE pDevice
		);

*/

/**** parameters to DEVICE_HARDWARE_DEINIT_ASYNC_ ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC_ARG)
	{
	__TMS_PUDEVICE	pDevice;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice							\
	)								\
    do	{								\
	(a_pArg)->pDevice = (a_pDevice);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_START_ASYNC

Index:	Type:	USBPUMP_IOCTL_DEVICE_START_ASYNC
	Name:	USBPUMP_IOCTL_DEVICE_START_ASYNC_ARG_SETUP_V1()

Function:
	Start USB device controller

Input:
	PUDEVICE pDevice;
		The USB device controller of interest.

Description:
	This IOCTL is sent from the client to start USB device 
	controller, and attach to interrupt. 
	The DCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Device already started (USBPUMP_IOCTL_RESULT_ALREADY_STARTED)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_START_ASYNC_ARG_SETUP_V1(
		PUSBPUMP_IOCTL_DEVICE_START_ASYNC_ARG pArg,
		PUDEVICE pDevice
		);

*/

/**** parameters to DEVICE_START_ASYNC_ ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DEVICE_START_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DEVICE_START_ASYNC_ARG)
	{
	__TMS_PUDEVICE	pDevice;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_DEVICE_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pDevice						\
	)							\
    do	{							\
	(a_pArg)->pDevice = (a_pDevice);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_STOP_ASYNC

Index:	Type:	USBPUMP_IOCTL_DEVICE_STOP_ASYNC
	Name:	USBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG_SETUP_V1()

Function:
	Stop USB device controller

Input:
	PUDEVICE pDevice;
		The USB device controller of interest.

Description:
	This IOCTL is sent from the client to stop USB device 
	controller, and detach from interrupt. 
	The DCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Device already stopped (USBPUMP_IOCTL_RESULT_ALREADY_STOPPED)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG_SETUP_V1(
		PUSBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG pArg,
		PUDEVICE pDevice
		);

*/

/**** parameters to DEVICE_STOP_ASYNC_ ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG)
	{
	__TMS_PUDEVICE	pDevice;		/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pDevice						\
	)							\
    do	{							\
	(a_pArg)->pDevice = (a_pDevice);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_DELAY

Index:	Type:	USBPUMP_IOCTL_DEVICE_DELAY_ARG
	Name:	USBPUMP_IOCTL_DEVICE_DELAY_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to ask for delay, in case there is
	no support for USBPUMP_TIMER.

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_DELAY_ARG
			structure.

	pOutParam	not used, should be NULL.

	USBPUMP_IOCTL_DEVICE_DELAY_ARG has the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	USBPUMP_MILLISECONDS	uMilliseconds;
		IN: required wait period in millisecond.

Description:
	This IOCTL is sent to get required waiting period from client if no
	device timer is supported.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_DELAY_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_DELAY_ARG *pArg,
		UDEVICE *pDevice,
		USBPUMP_MILLISECONDS	uMilliseconds
		);

	This macro correctly initializes a .._DEVICE_SUPPORT_DELAY_ARG 
	structure. If the layout of the argument structure changes in the 
	future, MCCI will revise this macro to initialize the new elements in a
	forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_DELAY_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_DELAY_ARG)
	{
	__TMS_UDEVICE *			pDevice;
	__TMS_USBPUMP_MILLISECONDS	uMilliseconds;
	};

/* the setup for the .._DEVICE_SUPPORT_DELAY_ARG struct */
#define __TMS_USBPUMP_IOCTL_DEVICE_DELAY_ARG_SETUP_V1(	\
	pArg,						\
	ARG_pDevice,					\
	ARG_uMilliseconds				\
	)						\
   do	{						\
	__TMS_USBPUMP_IOCTL_DEVICE_DELAY_ARG *		\
		__TMS_CONST	__pArg = (pArg);	\
	__pArg->pDevice = (ARG_pDevice);		\
	__pArg->uMilliseconds = (ARG_uMilliseconds);	\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE

Index:	Type:	USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG
	Name:	USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip get the device description.

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG
			structure.

	USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG has the following
	elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	BOOL	fCanDetectCable;
		OUT: TRUE if device supports cable detection, FALSE if not.

	BOOL	fAutoResponseSetAddress;
		OUT: TRUE if device automatically responds SetAddress, FALSE
		if not

	BOOL	fSupportVbusNotification;
		OUT: TRUE if device support Vbus detection, FALSE if not.

	BOOL	fDetectEndpontActivity;
		OUT: TRUE if device support endpoint activity detection, FALSE
		if not.

	BOOL	fLtmCapable;
		OUT: TRUE if device supports the LTM capability, FALSE if not.

Description:
	This IOCTL is sent from DataPump to get the device controller
	descriptioin.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG	*pArg,
		UDEVICE *		pDevice
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG)
	{
	__TMS_UDEVICE *			pDevice;
	__TMS_BOOL			fCanDetectCable;
	__TMS_BOOL			fAutoRespondSetAddress;
	__TMS_BOOL			fSupportVbusNotification;
	__TMS_BOOL			fDetectEndpontActivity;
	__TMS_BOOL			fLtmCapable;
	};

#define __TMS_USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG_SETUP_V1(	\
	pARG,							\
	ARG_pDevice						\
	)							\
    do	{							\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG *	\
		__TMS_CONST	__pDevDesc = (pARG);		\
	__pDevDesc->pDevice = (ARG_pDevice);			\
	__pDevDesc->fCanDetectCable = __TMS_FALSE;		\
	__pDevDesc->fAutoRespondSetAddress = __TMS_FALSE;	\
	__pDevDesc->fSupportVbusNotification = __TMS_FALSE;	\
	__pDevDesc->fDetectEndpontActivity = __TMS_FALSE;	\
	__pDevDesc->fLtmCapable = __TMS_FALSE;			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC

Index:	Type:	USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG
	Name:	USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG_SETUP_V1()

Function:
	Change power state of USB device controller

Input:
	PUDEVICE pDevice;
		The USB device controller of interest.

	BOOL	fSetLowPower;
		TRUE for low power transition, FALSE for high power transition
		if device is already in target power state, device just returns
		USBPUMP_IOCTL_RESULT_SUCCESS.

	BOOL	fForceTransition;
		If TRUE, device must do power transition. otherwise device can
		fail IOCTL with proper error.
		
		1) fSetLowPower = FALSE (high-power transition)
		device can go to high-power state any time
		
		2) fSetLowPower = TRUE (low-power transition)
		If device is in low-power capable state and is able to keep USB
		connection in low-power state, device just goes to low-power
		state while preserving USB connection with host.
		
		However, if device is in low-power incapable state (like active
		L0/U0 USB bus state) or is not able to keep USB connection in
		low-power state, device's behavior depends on fForceTransition.

		[fForceTransition = TRUE] this happens when device power
		transition request is triggered by system power transition which
		device cannot deny. device has to release USB connection with
		host in order to go to lower-power state.
		
		[fForceTransition = FALSE] this happens when device power
		transition is not triggered by system power transition which
		device can deny. device just fails IOCTL with
		USBPUMP_IOCTL_RESULT_FAILED

	BOOL	fWakeEnabled;
		TRUE if wakeup is enabled.
		if fWakeEnabled is TRUE, device has to do any device-specific
		operation so that wakeup could happen if necessary.
		device can just ignore this flag if any device-specific work is
		unnecessary.

Description:
	This IOCTL is sent from the client to change power state of the
	USB device controller.
	The UDEVICE IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or

	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG *pArg,
		UDEVICE *			pDevice,
		BOOL				fSetLowPower,
		BOOL				fForceTransition,
		BOOL				fWakeEnabled
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG)
	{
	__TMS_UDEVICE *				pDevice;
	__TMS_BOOL 				fSetLowPower;
	__TMS_BOOL 				fForceTransition;
	__TMS_BOOL				fWakeEnabled;
	};

#define	__TMS_USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice,							\
	a_fSetLowPower,							\
	a_fForceTransition,						\
	a_fWakeEnabled							\
	)								\
    do	{								\
	(a_pArg)->pDevice = (a_pDevice);				\
	(a_pArg)->fSetLowPower = (a_fSetLowPower);			\
	(a_pArg)->fForceTransition = (a_fForceTransition);		\
	(a_pArg)->fWakeEnabled = (a_fWakeEnabled);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN

Index:	Type:	USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG
	Name:	USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG_SETUP_V1()

Function:
	Register power request function to USB device controller

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG
			structure.

	pOutParam	points to a USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG
			structure.

	USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG has the following
	elements:
	
	PUDEVICE pDevice;
		IN: The USB device controller of interest.
	
	DATAPUMP_DEVICE_POWER_REQUEST_FN * pPowerRequestFn;
		IN and OUT: Callback function pointer. device should save this
		and pClientContext input parameters in it's internal storage
		when receiving this IOCTL. if pPowerRequestFn provided is not
		NULL, device is supposed to call this function along with the
		client context provided when it wants to change power-state
		from low-power to high-power or from high-power to low-power.
		the second parameter, fRequestLowPower of
		DATAPUMP_DEVICE_POWER_REQUEST_FN indicates target power state
		that it requests. TRUE means low-power state, FALSE means
		high-power state.
		IOCTL handler should fill this and pClientContext with old
		values (if available, NULL if not) on return.

	VOID * pClientContext;
		IN and OUT: Callback context pointer

Description:
	This IOCTL is sent from the client to register a callback function
	which DCD calls when it wants to change power state.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG *pArg,
		UDEVICE *				pDevice,
		DATAPUMP_DEVICE_POWER_REQUEST_FN *	pPowerRequestFn,
		VOID *					pClientContext
		);

*/

__TMS_FNTYPE_DEF(
	USBPUMP_DEVICE_POWER_REQUEST_FN,
	__TMS_VOID,
		(
		__TMS_VOID *	/* pClientContext */,
		__TMS_BOOL	/* fRequestLowPower */
		)
	);
	
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG)
	{
	__TMS_UDEVICE *					pDevice;
	__TMS_USBPUMP_DEVICE_POWER_REQUEST_FN *		pPowerRequestFn;
	__TMS_VOID *					pClientContext;
	};

#define	__TMS_USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG_SETUP_V1(\
	a_pArg,								\
	a_pDevice,							\
	a_pPowerRequestFn,						\
	a_pClientContext						\
	)								\
    do	{								\
	(a_pArg)->pDevice = (a_pDevice);				\
	(a_pArg)->pPowerRequestFn = (a_pPowerRequestFn);		\
	(a_pArg)->pClientContext = (a_pClientContext);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN

Index:	Type:	USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG
	Name:	USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG_SETUP_V1()

Function:
	Register client specified control packet process function in the
	DataPump core.

Input:
	pInParam	points to a USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG
			structure.

	pOutParam	points to a USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG
			structure.

	USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG has the following
	elements:

	UDEVICE *pDevice;
		IN: The USB device controller of interest.

	USBPUMP_CONTROL_PROCESS_FN *pControlProcessFn;
	VOID *pControlProcessCtx;
		IN and OUT: Control packet process function pointer. The device
		should save this function and context input parameters in it's
		internal storage when receiving this IOCTL.

		If pControlProcessFn is not NULL, UsbProcessControlPacket()
		will call this function with pControlProcessCtx when received
		control packet from the host.  The pControlProcessFn() should
		return one of the USBPUMP_CONTROL_PROCESS_STATUS as below:

		|USBPUMP_CONTROL_PROCESS_STATUS_SUCCESS|
			The pControlProcessFn() can handle given control packet
			and caller should do nothing.

		|USBPUMP_CONTROL_PROCESS_STATUS_FAILURE|
			The pControlProcessFn() can handle given control packet
			but pControlProcessFn() want to send stall for this
			control packet.  UsbProcessControlPacket() will send
			stall to the host.

		|USBPUMP_CONTROL_PROCESS_STATUS_DO_NORMAL_PROCESS|
			The pControlProcessFn() can't handle given control
			packet and caller need to do normal control process.

Description:
	This IOCTL is sent from the client to register control packet process
	function pointer.  The registered function will be called by
	DataPump core UsbProcessControlPacket().

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG *pArg,
		UDEVICE *			pDevice,
		USBPUMP_CONTROL_PROCESS_FN *	pControlProcessFn,
		VOID *				pControlProcessCtx
		);

*/

/*

Type:	USBPUMP_CONTROL_PROCESS_STATUS

Function:
	C scalar (unsigned) type for variables that convey control packet
	process status.

Definition:
	typedef	UINT32		 USBPUMP_CONTROL_PROCESS_STATUS,
				*PUSBPUMP_CONTROL_PROCESS_STATUS;
	typedef ARG_UINT32	 ARG_USBPUMP_CONTROL_PROCESS_STATUS,
				*PARG_USBPUMP_CONTROL_PROCESS_STATUS;

Description:
	This type is used to represent a value that contains a status of the
	USBPUMP_CONTROL_PROCESS_FN().

	The following status codes are defined.

	|USBPUMP_CONTROL_PROCESS_STATUS_SUCCESS|
		The registered pControlProcessFn() can handle given control
		packet and UsbProcessControlPacket() should do nothing.

	|USBPUMP_CONTROL_PROCESS_STATUS_FAILURE|
		The registered pControlProcessFn() can handle given control
		packet but pControlProcessFn() want to send stall for this
		control packet.  UsbProcessControlPacket() will send stall
		to the host.

	|USBPUMP_CONTROL_PROCESS_STATUS_DO_NORMAL_PROCESS|
		The registered pControlProcessFn() can't handle given control
		packet and UsbProcessControlPacket() need to do normal control
		packet process.

See also:
	USBPUMP_CONTROL_PROCESS_FN,
	USBPUMP_CONTROL_PROCESS_STATUS_...

*/

#define	__TMS_USBPUMP_CONTROL_PROCESS_STATUS_SUCCESS		0
#define	__TMS_USBPUMP_CONTROL_PROCESS_STATUS_FAILURE		1
#define	__TMS_USBPUMP_CONTROL_PROCESS_STATUS_DO_NORMAL_PROCESS	2

/*

Type:	USBPUMP_CONTROL_PROCESS_FN

Function:
	C function type for control packet process functions.

Definition:
	typedef USBPUMP_CONTROL_PROCESS_STATUS
		USBPUMP_CONTROL_PROCESS_FN(
			UDEVICE *pDevice,
			VOID *pControlProcessCtx,
			CONST USETUP *pSetup
			);

	typedef USBPUMP_CONTROL_PROCESS_FN
		*PUSBPUMP_CONTROL_PROCESS_FN;

Description:
	This function will be called by DataPump core UsbProcessControlPacket()
	when device received control packet from host.  This function needs to
	check pSetup packet and returns one of USBPUMP_CONTROL_PROCESS_STATUS.

Returns:
	USBPUMP_CONTROL_PROCESS_STATUS

*/

/* control packet process function: in usbpump_types.h */
/*
|| __TMS_FNTYPE_DEF(
||	USBPUMP_CONTROL_PROCESS_FN,
||	__TMS_USBPUMP_CONTROL_PROCESS_STATUS,
||		(
||		__TMS_UDEVICE *			pDevice,
||		__TMS_VOID *			pClientContext,
||		__TMS_CONST __TMS_USETUP *	pSetup
||		)
||	);
*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG)
	{
	__TMS_UDEVICE *				pDevice;
	__TMS_USBPUMP_CONTROL_PROCESS_FN *	pControlProcessFn;
	__TMS_VOID *				pControlProcessCtx;
	};

#define	__TMS_USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG_SETUP_V1( \
	a_pArg,								\
	a_pDevice,							\
	a_pControlProcessFn,						\
	a_pControlProcessCtx						\
	)								\
    do	{								\
	(a_pArg)->pDevice = (a_pDevice);				\
	(a_pArg)->pControlProcessFn = (a_pControlProcessFn);		\
	(a_pArg)->pControlProcessCtx = (a_pControlProcessCtx);		\
	} while (0)

/****************************************************************************\
|
|	The device IOCTL support routines
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL	UsbDeviceIoctl_GetVbusState(
			__TMS_PUDEVICE	pDeviceAbstract
			);

__TMS_BOOL	UsbDeviceIoctl_GetSelfPowerState(
			__TMS_PUDEVICE	pDeviceAbstract
			);

__TMS_BOOL	UsbDeviceIoctl_RemoteWakeup(
			__TMS_PUDEVICE	pDeviceAbstract
			);

__TMS_BOOL	UsbDeviceIoctl_GetUsbConnectState(
			__TMS_PUDEVICE	pDeviceAbstract
			);

__TMS_BOOL	UsbDeviceIoctl_SetUsbConnectState(
			__TMS_PUDEVICE	pDeviceAbstract,
			__TMS_BOOL	NewState
			);

__TMS_VOID	UsbDeviceIoctl_InitUsbHardware(
			__TMS_PUDEVICE	pDeviceAbstract
			);

__TMS_BOOL	UsbDeviceIoctl_GetUsbClockState(
			__TMS_PUDEVICE	pDeviceAbstract
			);

__TMS_BOOL	UsbDeviceIoctl_SetUsbClockState(
			__TMS_PUDEVICE	pDeviceAbstract,
			__TMS_BOOL	NewState
			);

__TMS_PVOID	UsbDeviceIoctl_AllocUncacheBuffer(
			__TMS_PUDEVICE	pDeviceAbstract,
			__TMS_UINT32	nBytes
			);

__TMS_BOOL	UsbDeviceIoctl_ClearUsbInterrupt(
			__TMS_PUDEVICE				pDeviceAbstract,
			__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE	hExtInt
			);

__TMS_BOOL	UsbDeviceIoctl_SetUsbChargingState(
			__TMS_PUDEVICE	pDeviceAbstract,
			__TMS_BOOL	fEnable
			);

__TMS_BOOL	UsbDeviceIoctl_SetUsbInterrupt(
			__TMS_PUDEVICE	pDeviceAbstract,
			__TMS_BOOL	fEnable
			);

__TMS_BOOL	UsbDeviceIoctl_ClearStallPreserveToggle(
			__TMS_PUDEVICE		pDeviceAbstract,
			__TMS_PUENDPOINT	pUepAbstract
			);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_DEVICE(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_DEVICE(i, t, NAME)
# define USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE	\
   __TMS_USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE
# define USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE	\
   __TMS_USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE
# define USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP	\
   __TMS_USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP
# define USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE	\
   __TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE
# define USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE	\
   __TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE
# define USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE	\
   __TMS_USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE
# define USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE	\
   __TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE
# define USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE	\
   __TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE
# define USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER	\
   __TMS_USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER
# define USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT	\
   __TMS_USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT
# define USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE	\
   __TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE
# define USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT	\
   __TMS_USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT
# define USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE	\
   __TMS_USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE
# define USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC	\
   __TMS_USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC
# define USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC	\
   __TMS_USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC
# define USBPUMP_IOCTL_DEVICE_START_ASYNC	\
   __TMS_USBPUMP_IOCTL_DEVICE_START_ASYNC
# define USBPUMP_IOCTL_DEVICE_STOP_ASYNC	\
   __TMS_USBPUMP_IOCTL_DEVICE_STOP_ASYNC
# define USBPUMP_IOCTL_DEVICE_DELAY	\
   __TMS_USBPUMP_IOCTL_DEVICE_DELAY
# define USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE	\
   __TMS_USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE
# define USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC	\
   __TMS_USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC
# define USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN	\
   __TMS_USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN
# define USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN	\
   __TMS_USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN
# define USBPUMP_IOCTL_DEVICE_IFC_BASE	\
   __TMS_USBPUMP_IOCTL_DEVICE_IFC_BASE
# define USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_VBUS_STATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_SELF_POWER_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_REMOTE_WAKEUP_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CONNECT_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CONNECT_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)
# define USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_INIT_USB_HARDWARE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_USB_CLOCK_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CLOCK_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)
# define USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_nBytes							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_ALLOC_UNCACHE_BUFFER_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_nBytes							\
	)
# define USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_hExtInt							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_CLEAR_USB_INTERRUPT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_hExtInt							\
	)
# define USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_CHARGING_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)
# define USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_USB_INTERRUPT_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pDevice,							\
	ARG_fEnable							\
	)
# define USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice,							\
	ARG_pUep							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_CLEAR_STALL_PRESERVE_TOGGLE_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice,							\
	ARG_pUep							\
	)
# define USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pDevice						\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pDevice						\
	)
# define USBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pDevice						\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pDevice						\
	)
# define USBPUMP_IOCTL_DEVICE_DELAY_ARG_SETUP_V1(	\
	pArg,						\
	ARG_pDevice,					\
	ARG_uMilliseconds				\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_DELAY_ARG_SETUP_V1(	\
	pArg,						\
	ARG_pDevice,					\
	ARG_uMilliseconds				\
	)
# define USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG_SETUP_V1(	\
	pARG,							\
	ARG_pDevice						\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_GET_HW_FEATURE_DEVICE_ARG_SETUP_V1(	\
	pARG,							\
	ARG_pDevice						\
	)
# define USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice,							\
	a_fSetLowPower,							\
	a_fForceTransition,						\
	a_fWakeEnabled							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_SET_POWER_STATE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pDevice,							\
	a_fSetLowPower,							\
	a_fForceTransition,						\
	a_fWakeEnabled							\
	)
# define USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG_SETUP_V1(\
	a_pArg,								\
	a_pDevice,							\
	a_pPowerRequestFn,						\
	a_pClientContext						\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_REGISTER_POWER_REQUEST_FN_ARG_SETUP_V1(\
	a_pArg,								\
	a_pDevice,							\
	a_pPowerRequestFn,						\
	a_pClientContext						\
	)
# define USBPUMP_CONTROL_PROCESS_STATUS_SUCCESS	\
   __TMS_USBPUMP_CONTROL_PROCESS_STATUS_SUCCESS
# define USBPUMP_CONTROL_PROCESS_STATUS_FAILURE	\
   __TMS_USBPUMP_CONTROL_PROCESS_STATUS_FAILURE
# define USBPUMP_CONTROL_PROCESS_STATUS_DO_NORMAL_PROCESS	\
   __TMS_USBPUMP_CONTROL_PROCESS_STATUS_DO_NORMAL_PROCESS
# define USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG_SETUP_V1( \
	a_pArg,								\
	a_pDevice,							\
	a_pControlProcessFn,						\
	a_pControlProcessCtx						\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN_ARG_SETUP_V1( \
	a_pArg,								\
	a_pDevice,							\
	a_pControlProcessFn,						\
	a_pControlProcessCtx						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

#endif /* _USBIOCTL_DEVICE_H_ */
