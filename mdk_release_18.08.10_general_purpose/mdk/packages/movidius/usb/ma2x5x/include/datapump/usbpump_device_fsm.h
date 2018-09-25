/* usbpump_device_fsm.h	Tue Jan 10 2012 16:22:49 chwon */

/*

Module:  usbpump_device_fsm.h

Function:
	Definition of device finite state machine

Version:
	V3.01f	Tue Jan 10 2012 16:22:49 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2010-2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChiaChu Cho, MCCI Corporation	August 2010

Revision history:
   3.01d  Fri Aug 20 2010 02:07:27 cccho
	6150, 8071: Module created.

   3.01f  Thu Oct 06 2011 11:41:55  cccho
	13233: add fHwFeature_DetectEndpontActivity to USBPUMP_DEVICE_FSM.

   3.01f  Tue Jan 10 2012 16:22:49  chwon
	14623: Add fCheckAutoWakeup flags to the USBPUMP_DEVICE_FSM.
	Changed wait resume timeout value.

*/

#ifndef _USBPUMP_DEVICE_FSM_H_		/* prevent multiple includes */
#define _USBPUMP_DEVICE_FSM_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USETUP_H_
# include "usetup.h"
#endif

#ifndef _UEVENTNODE_H_
# include "ueventnode.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

/****************************************************************************\
|
|	Forward Definitions
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_DEVICE_FSM);
__TMS_TYPE_DEF_ARG	(USBPUMP_DEVICE_FSM_STATE, UINT8);

__TMS_TYPE_DEF_FUNCTION(
USBPUMP_DEVICE_FSM_FINALIZATION_FN,
	__TMS_VOID,
		(
		__TMS_VOID *		/* pDoneInfo */,
		__TMS_UDEVICE	*	/* pDevice */
		)
	);

/****************************************************************************\
|
|	The states for the USB device FSM
|
\****************************************************************************/

#define	__TMS_USBPUMP_DEVICE_FSM_stNoChange			0
#define	__TMS_USBPUMP_DEVICE_FSM_stInitial			1
#define	__TMS_USBPUMP_DEVICE_FSM_stDetached			2
#define	__TMS_USBPUMP_DEVICE_FSM_stAttached			3
#define	__TMS_USBPUMP_DEVICE_FSM_stPowered			4
#define	__TMS_USBPUMP_DEVICE_FSM_stSuspendedPowered		5
#define	__TMS_USBPUMP_DEVICE_FSM_stReset			6
#define	__TMS_USBPUMP_DEVICE_FSM_stDefault			7
#define	__TMS_USBPUMP_DEVICE_FSM_stSuspendedDefault		8
#define	__TMS_USBPUMP_DEVICE_FSM_stAddress			9
#define	__TMS_USBPUMP_DEVICE_FSM_stSuspendedAddress		10
#define	__TMS_USBPUMP_DEVICE_FSM_stConfigured			11
#define	__TMS_USBPUMP_DEVICE_FSM_stSuspendedConfigured		12
#define	__TMS_USBPUMP_DEVICE_FSM_stSuspendedRemoteWakeUp	13

#define	__TMS_USBPUMP_DEVICE_FSM_st__MAX			14

#define	__TMS_USBPUMP_DEVICE_FSM_STATES__INIT	\
	"stNoChange",				\
	"stInitial",				\
	"stDetached",				\
	"stAttached",				\
	"stPowered",				\
	"stSuspendedPowered",			\
	"stReset",				\
	"stDefault",				\
	"stSuspendedDefault",			\
	"stAddress",				\
	"stSuspendedAddress",			\
	"stConfigured",				\
	"stSuspendedConfigured",		\
	"stSuspendedRemoteWakeUp",		\
	"st__MAX"


struct __TMS_STRUCTNAME(USBPUMP_DEVICE_FSM)
	{
	__TMS_USBPUMP_DEVICE_FSM_STATE		CurrentState;

	/* speed requested from reset */
	__TMS_USBPUMP_DEVICE_SPEED		NewSpeed;

	/* host initiated resume delay from L1 sleep */
	__TMS_UINT8				bHostInitiatedResumeDuration;

	__TMS_USETUP				uSetup;

	__TMS_UCONFIG *				pNewConfig;
	__TMS_UCHAR *				pSetupPacket;

	__TMS_USBPUMP_TIMER			Timer;

	/*
	|| The finalization info
	*/
	__TMS_USBPUMP_DEVICE_FSM_FINALIZATION_FN *	pFinalizeFn;
	__TMS_VOID *					pFinalizeInfo;

	/*
	|| device hardware feature.
	*/
	__TMS_UINT		fHwFeature_CanDetectCable: 1;
	__TMS_UINT		fHwFeature_AutoRespondSetAddress: 1;
	__TMS_UINT		fHwFeature_SupportVbusNotification: 1;
	__TMS_UINT		fHwFeature_DetectEndpontActivity: 1;

	/*
	|| flags
	|| external flags, set outside the FSM, read insdie the FSM
	*/
	__TMS_UINT			fRun: 1;
	__TMS_UINT			fCableDetect: 1;
	__TMS_UINT			fVBus: 1;
	__TMS_UINT			fSuspended: 1;
	__TMS_UINT			fReset: 1;
	__TMS_UINT			fAddressed: 1;
	__TMS_UINT			fCfgChange: 1;
	__TMS_UINT			fRemoteWakeup: 1;
	__TMS_UINT			fTimerPending: 1;
	__TMS_UINT			fTimerEvent: 1;
	__TMS_UINT			fL1Sleep: 1;

	/*
	|| internal flags.
	*/
	__TMS_UINT			fCheckAutoWakeup: 1;

	/*
	|| flags
	|| internal flags, visible to the outside world
	*/
	__TMS_UINT			fDeferred: 1;
	/*
	|| fBusy is set while control is dynamically descended from
	|| device evaluation loop.  Used for detecting reentry
	*/
	__TMS_UINT			fBusy: 1;

	/* administrative flags from elsewhere */
	};

/****************************************************************************\
|
| The Device Event Timings (in millisec)
|
\****************************************************************************/

/*
|| USB 2.0 table 7-14 T_WTRSM:
||	period of idle bus before device can initiate resume: min 5msec
||
|| USB 3.0: table 6-22. tNoLFPSResponseTimeout:
||	U3 wakeup is max 10msec
||
|| So we are going to use 10msec for UDEVICE_TIME_WAIT_RESUME.
*/
#define	__TMS_UDEVICE_TIME_WAIT_RESUME_MIN	5
#define	__TMS_UDEVICE_TIME_WAIT_RESUME		10

/*
|| After remote wakeup, DCD will notify resume event. But we could not
|| guarantee old DCD generate resume event. So we are going to use timer with
|| long delay value.
*/
#define	__TMS_UDEVICE_TIME_WAIT_BUS_RESUME	2000	/* 2 sec */


/****************************************************************************\
|
|	Device FSM functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_DEVICE_FSM_FINALIZATION_FN
		UsbPumpDevice_DeviceFsm_Finalized;

__TMS_VOID	UsbPumpDevice_DeviceFsm_Init(
			__TMS_UDEVICE *			/* pDevice */,
			__TMS_USBPUMP_DEVICE_FSM_FINALIZATION_FN * /* pFinalizeFn */,
			__TMS_VOID *			/* pFinalizeInfo */
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evAttach(
			__TMS_UDEVICE *			/* pDevice */
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evDetach(
			__TMS_UDEVICE *			/* pDevice */
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evDetectCable(
			__TMS_UDEVICE *			/* pDevice */,
			__TMS_BOOL			/* fCableDetected */
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evRemoteWakeup(
			__TMS_UDEVICE *	pDevice
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evReset(
			__TMS_UDEVICE *			/* pDevice */,
			__TMS_USBPUMP_DEVICE_SPEED	/* newSpeed */
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evResume(
			__TMS_UDEVICE *			/* pDevice */
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evSetAddress(
			__TMS_UDEVICE *			/* pDevice */,
			__TMS_CONST __TMS_USETUP *	/* pSetup */,
			__TMS_UCHAR *			/* pSetupPacket */
			);

__TMS_BOOL	UsbPumpDevice_DeviceFsm_evSetConfig(
			__TMS_UDEVICE *			/* pDevice */,
			__TMS_ARG_USHORT		/* config */,
			__TMS_UINT8 *			/* pSetupPacket */
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evSuspend(
			__TMS_UDEVICE *			/* pDevice */
			);

__TMS_VOID	UsbPumpDevice_DeviceFsm_evSleep(
			__TMS_UDEVICE *	/* pDevice */,
			__TMS_BOOL	/* fRemoteWakeEnable */,
			__TMS_UINT8	/* bHostInitiatedResumeDuration */
			);

__TMS_CONST __TMS_TEXT *
		UsbPump_DeviceFsm_StateName(
			__TMS_USBPUMP_DEVICE_FSM_STATE	/* state */
			);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_DEVICE_FSM_stNoChange	\
   __TMS_USBPUMP_DEVICE_FSM_stNoChange
# define USBPUMP_DEVICE_FSM_stInitial	\
   __TMS_USBPUMP_DEVICE_FSM_stInitial
# define USBPUMP_DEVICE_FSM_stDetached	\
   __TMS_USBPUMP_DEVICE_FSM_stDetached
# define USBPUMP_DEVICE_FSM_stAttached	\
   __TMS_USBPUMP_DEVICE_FSM_stAttached
# define USBPUMP_DEVICE_FSM_stPowered	\
   __TMS_USBPUMP_DEVICE_FSM_stPowered
# define USBPUMP_DEVICE_FSM_stSuspendedPowered	\
   __TMS_USBPUMP_DEVICE_FSM_stSuspendedPowered
# define USBPUMP_DEVICE_FSM_stReset	\
   __TMS_USBPUMP_DEVICE_FSM_stReset
# define USBPUMP_DEVICE_FSM_stDefault	\
   __TMS_USBPUMP_DEVICE_FSM_stDefault
# define USBPUMP_DEVICE_FSM_stSuspendedDefault	\
   __TMS_USBPUMP_DEVICE_FSM_stSuspendedDefault
# define USBPUMP_DEVICE_FSM_stAddress	\
   __TMS_USBPUMP_DEVICE_FSM_stAddress
# define USBPUMP_DEVICE_FSM_stSuspendedAddress	\
   __TMS_USBPUMP_DEVICE_FSM_stSuspendedAddress
# define USBPUMP_DEVICE_FSM_stConfigured	\
   __TMS_USBPUMP_DEVICE_FSM_stConfigured
# define USBPUMP_DEVICE_FSM_stSuspendedConfigured	\
   __TMS_USBPUMP_DEVICE_FSM_stSuspendedConfigured
# define USBPUMP_DEVICE_FSM_stSuspendedRemoteWakeUp	\
   __TMS_USBPUMP_DEVICE_FSM_stSuspendedRemoteWakeUp
# define USBPUMP_DEVICE_FSM_stWaitResumeEvent	\
   __TMS_USBPUMP_DEVICE_FSM_stWaitResumeEvent
# define USBPUMP_DEVICE_FSM_st__MAX	\
   __TMS_USBPUMP_DEVICE_FSM_st__MAX
# define USBPUMP_DEVICE_FSM_STATES__INIT	\
   __TMS_USBPUMP_DEVICE_FSM_STATES__INIT
# define UDEVICE_TIME_WAIT_RESUME	\
   __TMS_UDEVICE_TIME_WAIT_RESUME
# define UDEVICE_TIME_WAIT_BUS_RESUME	\
   __TMS_UDEVICE_TIME_WAIT_BUS_RESUME
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_device_fsm.h ****/
#endif /* _USBPUMP_DEVICE_FSM_H_ */
