/* otgfsm.h	Mon Mar 02 2015 17:10:01 chwon */

/*

Module:  otgfsm.h

Function:
	Definitions for the OTGFSM.

Version:
	V3.15b	Mon Mar 02 2015 17:10:01 chwon	Edit level 12

Copyright notice:
	This file copyright (C) 2005-2008, 2011-2012, 2014, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Sat Aug  6 2005 18:17:20  tmm
	Module created.

   1.97j  Thu Oct 04 2007 09:09:56  skpark
	3212: increase TA_AIDL_BDIS from 200 to 210

   1.97k  Thu Mar 06 2008 16:22:33  chwon
	3567: add OTGFSM annunciator info in USBPUMP_OTGFSM. Add OTGFSM
	notification ID definition.

   1.97k  Mon Mar 10 2008 10:42:20  chwon
	3567: add USBPUMP_OTGFSM_EVENT_DETECT_SRP event

   1.97k  Thu Mar 13 2008 13:44:30  chwon
	3567: add UsbPumpOtgFsm_EventName() for debugging purpose

   1.97k  Tue Mar 18 2008 16:35:43  chwon
	3567: add sizeNotificationInfo parameter in the
	USBPUMP_ANNUNCIATOR_SEND_NOTIFICATION_FN

   1.97k  Wed Jul 16 2008 23:23:35  chwon
	4303: use USBPUMP_SESSION_HANDLE instead of VOID *

   3.01f  Tue Nov 08 2011 16:40:36  chwon
	9941: add OTGFSM evaluate and ioctl function pointer, timeout values,
	and flags for the V2 FSM in the USBPUMP_OTGFSM. Add V1 and V2 evaluate
	function and UsbPumpOtgFsm_Initialize_V3().

   3.11b  Fri Nov 02 2012 15:19:06  chwon
	16135: add fInitialized flag in the USBPUMP_OTGFSM.

   3.13a  Wed Jul 16 2014 18:06:49  tmm
	18397: we need to be able to cleanly fail, so add new 
	UsbPumpOtgFsm_Initialize_V4() with result type of BOOL.  Add a
	UsbPumpOtgFsm_Deinitialize().

   3.15b  Mon Mar 02 2015 17:10:01  chwon
	19011: Add pEventNotifyFn and fTimerUse_ta_bidl_adis in USBPUMP_OTGFSM.
	Add USBPUMP_OTGFSM_EVENT_HNP_FINISH event code.

*/

#ifndef _OTGFSM_H_		/* prevent multiple includes */
#define _OTGFSM_H_

#ifndef _OTGFSM_TYPES_H_
# include "otgfsm_types.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

#ifndef _USBPUMP_ANNUNCIATOR_H_
# include "usbpump_annunciator.h"
#endif

/****************************************************************************\
|
|	The OTGFSM object collects all the above information.
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_OTGFSM)
	{
	/* for efficiency, we keep a copy of the platform pointer */
	__TMS_UPLATFORM	*		pPlatform;

	/* we need a timer object for handling timeouts */
	__TMS_USBPUMP_TIMER		Timer;

	/* object header for debug message */
	__TMS_USBPUMP_OBJECT_HEADER *	pObjectHeader;

	/* OTGFSM evaluate function and ioctl function pointer */
	__TMS_USBPUMP_OTGFSM_EVAL_FN *	pEvaluateFn;
	__TMS_USBPUMP_OTGFSM_IOCTL_FN *	pIoctlFn;
	__TMS_USBPUMP_OTGFSM_EVENT_NOTIFY_FN *pEventNotifyFn;

	/* the OTG state variables interface us to the world */
	__TMS_USBPUMP_OTGVARS		Vars;

	/* this variable indicates which timer (if any) is running */
	__TMS_USBPUMP_OTGTIMERID	CurrentTimerId;

	/*
	|| The a_wait_bcon timeout needs to be programmable, possibly
	|| dynamically.  It's given in milliseconds, and must be at
	|| least 1000.  If zero, then no timeout is used.  Note
	|| the restricted range from 0 to 65.535 seconds.
	*/
	__TMS_UINT16			ta_wait_bcon_short;

	/*
	|| The a_aidl_bdis timeout needs to be programmable, possibly
	|| dynamically.  It's given in milliseconds, and must be at
	|| least 200.  If zero, then no timeout is used.  Note
	|| the restricted range from 0 to 65.535 seconds.
	*/
	__TMS_UINT16			ta_aidl_bdis;

	/*
	|| The a_bidl_adis timeout needs to be programmable, possibly
	|| dynamically.  It's given in milliseconds, and must be in range
	|| from 155 to 200.
	*/
	__TMS_UINT16			ta_bidl_adis;

	/*
	|| The a_wait_vfail timeout needs to be programmable, possibly
	|| dynamically.  It's given in milliseconds, and must be less than
	|| equal 1000.
	*/
	__TMS_UINT16			ta_wait_vfail;

	/*
	|| The b_ase0_brst timeout needs to be programmable, possibly
	|| dynamically.  It's given in milliseconds, and must be at
	|| least 155.
	*/
	__TMS_UINT16			tb_ase0_brst;

	/*
	|| The following timeout is port-dependent -- it depends on the
	|| power supply and the capacitance.  It can be no more than 100,
	|| because of the TB_SRP_INIT constraint, and normally must
	|| be much less.  MCCI calculates that for an ISP1301 and 1uf
	|| bulk capacitance, this should be 20ms; for an ISP1301 and 6.5uf
	|| bulk capacitance, this should be 30ms.  If your system can hold
	|| millisecond timer accuracy, 30ms should work in every case.  But
	|| if not, you'll have to adjust the value used here.
	*/
	__TMS_UINT16			tb_vbuschrg_srp;

	/*
	|| This flag is used to remember that OTGFSM version 1.0.
	*/
	__TMS_UINT			fOtgFsmV1: 1;
	__TMS_UINT			fInitialized: 1;

	/*
	|| This flag is used to remember that we need to
	|| post the event.  It's only an optimization.
	*/
	__TMS_UINT			fUpdateNeeded: 1;

	/*
	|| fBusy is set while control is dynamically descended from
	|| otg fsm evaluation loop.  Used for detecting reentry
	*/
	__TMS_UINT			fBusy: 1;

	/*
	|| fDeferred is set whenever reentry is detected, and forces
	|| an extra evaluation step.  Note that a non-"state entry"
	|| action that reenters the state machine on every evaluation will
	|| cause an indefinite loop in the FSM.
	*/
	__TMS_UINT			fDeferred: 1;

	/* Timer handling flags */
	__TMS_UINT			fTimerPending: 1;
	__TMS_UINT			fTimer: 1;
	__TMS_UINT			fTimerUse_ta_bidl_adis: 1;

	/*
	|| The following event gets posted when the FSM outputs
	|| change or when the state of the FSM changes. We are using
	|| UsbPumpPlatform_PostIfNotBusy(), so we hold the callback
	|| context in a separate cell.  Alternately we could make the
	|| output change event be derived from UCALLBACKCOMPLETION and
	|| put the context pointer in the larger structure.  Same
	|| amount of storage, and this way requires less typing....
	*/
	__TMS_VOID *			pFsmOutputChangeEvent_context;
	__TMS_UCALLBACKCOMPLETION	FsmOutputChangeEvent;

	/* The OTGCD Annunciator information */
	__TMS_USBPUMP_ANNUNCIATOR *	pAnnunciator;
	__TMS_USBPUMP_SESSION_HANDLE	hAnnunciatorSession;
	__TMS_USBPUMP_ANNUNCIATOR_SENDER_INCALL	AnnunciatorInCall;
	};

#define	__TMS_USBPUMP_OTGFSM_ANNUNCIATOR_OBSERVER_GUID_INIT()	\
	__TMS_USBUUID_INIT(0x1aea6fcc, 0xe75e, 0x4252,		\
		0x8c, 0x85, 0x1f, 0xfa, 0x00, 0x0d, 0x26, 0x1d)

#define	__TMS_USBPUMP_OTGFSM_ANNUNCIATOR_SENDER_GUID_INIT()	\
	__TMS_USBUUID_INIT(0xc28c56f2, 0x9a1d, 0x4d7b,		\
		0x8d, 0x76, 0xcf, 0xf8, 0x80, 0x5c, 0x0a, 0x11)

#define	__TMS_USBPUMP_OTGFSM_ANNUNCIATOR_OBJECT_NAME		\
	__TMS_USBPUMP_ANNUNCIATOR_OBJECT_NAME("otgfsm")


/****************************************************************************\
|
|	OTGFSM Annunciator notification event definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_OTGFSM_EVENT_START_SRP	0
#define	__TMS_USBPUMP_OTGFSM_EVENT_SRP_FAIL	1
#define	__TMS_USBPUMP_OTGFSM_EVENT_SRP_SUCCESS	2
#define	__TMS_USBPUMP_OTGFSM_EVENT_OVER_POWER	3
#define	__TMS_USBPUMP_OTGFSM_EVENT_NOT_SUPPORT	4
#define	__TMS_USBPUMP_OTGFSM_EVENT_TRYING_HNP	5
#define	__TMS_USBPUMP_OTGFSM_EVENT_HNP_SUCCESS	6
#define	__TMS_USBPUMP_OTGFSM_EVENT_HNP_FAIL	7
#define	__TMS_USBPUMP_OTGFSM_EVENT_DETECT_SRP	8
#define	__TMS_USBPUMP_OTGFSM_EVENT_HNP_FINISH	9

/*
|| When you add names to the above list, please also update the following
|| debugging table.
*/
#define	__TMS_USBPUMP_OTGFSM_EVENT_NAMES__INIT		\
	{						\
	"USBPUMP_OTGFSM_EVENT_START_SRP",		\
	"USBPUMP_OTGFSM_EVENT_SRP_FAIL",		\
	"USBPUMP_OTGFSM_EVENT_SRP_SUCCESS",		\
	"USBPUMP_OTGFSM_EVENT_OVER_POWER",		\
	"USBPUMP_OTGFSM_EVENT_NOT_SUPPORT",		\
	"USBPUMP_OTGFSM_EVENT_TRYING_HNP",		\
	"USBPUMP_OTGFSM_EVENT_HNP_SUCCESS",		\
	"USBPUMP_OTGFSM_EVENT_HNP_FAIL",		\
	"USBPUMP_OTGFSM_EVENT_DETECT_SRP",		\
	"USBPUMP_OTGFSM_EVENT_HNP_FINISH"		\
	}


/****************************************************************************\
|
|	API functions - think of these as C++ non-virtual methods of the
|	OTGFSM object.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* Default linkage for OTGFSM annunciator GUID */
extern __TMS_CONST __TMS_USBGUID	gk_UsbPumpOtgFsmAnnunciator_ObserverGuid;
extern __TMS_CONST __TMS_USBGUID	gk_UsbPumpOtgFsmAnnunciator_SenderGuid;

/* Default linkage for OTGFSM annunciator object name */
extern __TMS_CONST __TMS_TEXT		gk_UsbPumpOtgFsmAnnunciator_ObjectName[];


__TMS_VOID
UsbPumpOtgFsm_Deinitialize(
	__TMS_USBPUMP_OTGFSM * /*pOtgFsm*/
	);

__TMS_VOID
UsbPumpOtgFsm_EvaluateOtgState(
	__TMS_USBPUMP_OTGFSM * /*pOtgFsm*/
	);

__TMS_USBPUMP_OTGFSM_EVAL_FN
UsbPumpOtgFsm_EvaluateFsm_V1;

__TMS_USBPUMP_OTGFSM_EVAL_FN
UsbPumpOtgFsm_EvaluateFsm_V2;

__TMS_VOID
UsbPumpOtgFsm_Initialize_V1(
	__TMS_USBPUMP_OTGFSM *	/* pNewOtgFsm */,
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_UCALLBACKFN *	/* pCallback -- callback for state changes */,
	__TMS_VOID *		/* pCallbackContext */,
	__TMS_UINT16		/* tb_vbuschrg_srp */
	);

__TMS_VOID
UsbPumpOtgFsm_Initialize_V2(
	__TMS_USBPUMP_OTGFSM *	/* pNewOtgFsm */,
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_UCALLBACKFN *	/* pCallback -- callback for state changes */,
	__TMS_VOID *		/* pCallbackContext */,
	__TMS_UINT16		/* tb_vbuschrg_srp */,
	__TMS_UINT		/* AnnunciatorMaxSession */
	);

__TMS_VOID
UsbPumpOtgFsm_Initialize_V3(
	__TMS_USBPUMP_OTGFSM *	/* pNewOtgFsm */,
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_UCALLBACKFN *	/* pCallback -- callback for state changes */,
	__TMS_VOID *		/* pCallbackContext */,
	__TMS_UINT16		/* tb_vbuschrg_srp */,
	__TMS_UINT		/* AnnunciatorMaxSession */,
	__TMS_USBPUMP_OTGFSM_EVAL_FN *	/* pEvalFn */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader -- OPTIONAL */
	);

__TMS_BOOL
UsbPumpOtgFsm_Initialize_V4(
	__TMS_USBPUMP_OTGFSM *	/* pNewOtgFsm */,
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_UCALLBACKFN *	/* pCallback -- callback for state changes */,
	__TMS_VOID *		/* pCallbackContext */,
	__TMS_UINT16		/* tb_vbuschrg_srp */,
	__TMS_UINT		/* AnnunciatorMaxSession */,
	__TMS_USBPUMP_OTGFSM_EVAL_FN *	/* pEvalFn */,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pObjectHeader -- OPTIONAL */
	);

__TMS_USBPUMP_OTGFSM_IOCTL_FN	UsbPumpOtgFsm_Ioctl;

__TMS_VOID
UsbPumpOtgFsm_SendNotification(
	__TMS_USBPUMP_OTGFSM *			/*pOtgFsm*/,
	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION	/*NotificationId*/,
	__TMS_CONST __TMS_VOID *		/*pNotificationInfo*/,
	__TMS_BYTES				/*sizeNotificationInfo*/
	);

__TMS_CONST __TMS_TEXT *
UsbPumpOtgFsm_EventName(
	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION	/* EventCode */
	);

__TMS_CONST __TMS_TEXT *
UsbPumpOtgFsm_StateName(
	__TMS_USBPUMP_OTGSTATE		/* StateCode */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_OTGFSM_ANNUNCIATOR_OBSERVER_GUID_INIT()	\
   __TMS_USBPUMP_OTGFSM_ANNUNCIATOR_OBSERVER_GUID_INIT()
# define USBPUMP_OTGFSM_ANNUNCIATOR_SENDER_GUID_INIT()	\
   __TMS_USBPUMP_OTGFSM_ANNUNCIATOR_SENDER_GUID_INIT()
# define USBPUMP_OTGFSM_ANNUNCIATOR_OBJECT_NAME	\
   __TMS_USBPUMP_OTGFSM_ANNUNCIATOR_OBJECT_NAME
# define USBPUMP_OTGFSM_EVENT_START_SRP	\
   __TMS_USBPUMP_OTGFSM_EVENT_START_SRP
# define USBPUMP_OTGFSM_EVENT_SRP_FAIL	\
   __TMS_USBPUMP_OTGFSM_EVENT_SRP_FAIL
# define USBPUMP_OTGFSM_EVENT_SRP_SUCCESS	\
   __TMS_USBPUMP_OTGFSM_EVENT_SRP_SUCCESS
# define USBPUMP_OTGFSM_EVENT_OVER_POWER	\
   __TMS_USBPUMP_OTGFSM_EVENT_OVER_POWER
# define USBPUMP_OTGFSM_EVENT_NOT_SUPPORT	\
   __TMS_USBPUMP_OTGFSM_EVENT_NOT_SUPPORT
# define USBPUMP_OTGFSM_EVENT_TRYING_HNP	\
   __TMS_USBPUMP_OTGFSM_EVENT_TRYING_HNP
# define USBPUMP_OTGFSM_EVENT_HNP_SUCCESS	\
   __TMS_USBPUMP_OTGFSM_EVENT_HNP_SUCCESS
# define USBPUMP_OTGFSM_EVENT_HNP_FAIL	\
   __TMS_USBPUMP_OTGFSM_EVENT_HNP_FAIL
# define USBPUMP_OTGFSM_EVENT_DETECT_SRP	\
   __TMS_USBPUMP_OTGFSM_EVENT_DETECT_SRP
# define USBPUMP_OTGFSM_EVENT_HNP_FINISH	\
   __TMS_USBPUMP_OTGFSM_EVENT_HNP_FINISH
# define USBPUMP_OTGFSM_EVENT_NAMES__INIT	\
   __TMS_USBPUMP_OTGFSM_EVENT_NAMES__INIT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of otgfsm.h ****/
#endif /* _OTGFSM_H_ */
