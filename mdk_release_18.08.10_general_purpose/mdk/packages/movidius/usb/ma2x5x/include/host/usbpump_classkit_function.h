/* usbpump_classkit_function.h	Tue Apr 18 2017 11:21:11 chwon */

/*

Module:  usbpump_classkit_function.h

Function:
	Definitions related to USBPUMP_CLASSKIT_FUNCTION objects.

Version:
	V3.21a	Tue Apr 18 2017 11:21:11 chwon	Edit level 19

Copyright notice:
	This file copyright (C) 2008-2010, 2013, 2015-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	April 2008

Revision history:
   1.97k  Thu Apr 03 2008 09:41:14 chwon
	Module created.

   1.97k  Wed Apr 30 2008 14:05:08  wkkim
	Added fNoMemory flag in FSM structure.

   1.97k  Thu May 01 2008 10:17:57  chwon
	Added error handling flags and add class driver flags for
	the class driver specific usage.

   1.97k  Tue Jul 01 2008 13:15:33  chwon
	4243: add wDeviceRequestTimeout in the USBPUMP_CLASSKIT_FSM. It allows
	to change device request timeout value at run-time.

   1.97k  Wed Jul 23 2008 11:23:54  chwon
	Add ClassKit.nUrb; it contains size of URB that pointed by pUrb

   1.97k  Thu Jul 24 2008 10:53:29  chwon
	Remove including usbpump_usbdi_cfgnodes.h for backward compatibility

   1.97k  Thu Jul 31 2008 09:53:55  chwon
	Remove ClassKit.nUrb because it is same as PortInfo.UrbTotalSize.
	Remove fIdlePortIfError, fSuspendPortIfError flag.

   1.97k  Fri Aug 29 2008 14:09:32  chwon
	Use __TMS_TYPE_DEF_ARG instead of __TMS_TYPE_DEF

   1.97k  Tue Sep 02 2008 16:36:22  chwon
	Fix typo from pUnconfiguedProcessFn to pUnconfiguredProcessFn.
	Fix typo from pConfiguedProcessFn to pConfiguredProcessFn.

   2.01a  Fri Mar 13 2009 15:58:07  chwon
	7436: Change USBPUMP_CLASSKIT_FSM_STATE type from UINT8 to UINT.

   3.01d  Mon Dec 13 2010 15:29:46  chwon
	12147: add fRestoreNextCfgNode flag in the USBPUMP_CLASSKIT_FSM.

   3.11c  Fri Apr 12 2013 14:15:33  chwon
	17015: add fSuspendPortIfError and fResetPortBeforeEnum flag in the 
	FSM

   3.11d  Tue Jul 16 2013 14:15:57  chwon
	17478: add timer management flags in the FSM and add pPlatform in the
	USBPUMP_CLASSKIT_FUNCTION.

   3.15b  Mon Mar 16 2015 13:13:15  chwon
	18673: Add configuration handling flags and pUrbSetConfig in the FSM.
	Add USBPUMP_CLASSKIT_CONFIG_DESC handling variables and rename from
	sizeConfigDescBuffer to sizeConfigDescBundles in the
	USBPUMP_CLASSKIT_FUNCTION.

   3.15b  Mon Mar 23 2015 13:36:57  chwon
	18673: Add fSetConfigDelay and rename DeviceRequestTimeout in the FSM.

   3.15b  Tue Jun 09 2015 10:55:52  chwon
	19105: Add fGetVendorString, fGetProductString and fGetSerialString
	in the FSM. Add lang id and string buffers in the function context.

   3.15c  Wed Jul 15 2015 11:17:28  chwon
	19317: Add fWaitClassDriverDone in the FSM.

   3.17a  Sun Mar  6 2016 16:02:19  tmm
	19967: Allow compilation with type-cloaking turned on.

   3.21a  Tue Apr 18 2017 11:21:12  chwon
	20517: Add buffer enter and exit scope API.

*/

#ifndef _USBPUMP_CLASSKIT_FUNCTION_H_	/* prevent multiple includes */
#define _USBPUMP_CLASSKIT_FUNCTION_H_

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

#ifndef _USBPUMP_CLASSKIT_SESSION_H_
# include "usbpump_classkit_session.h"
#endif

#ifndef _USBPUMP_USBDI_REQUEST_H_
# include "usbpump_usbdi_request.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMP_TIMER_H_
# include "usbpump_timer.h"
#endif

#ifndef _USBDESC_H_
# include "usbdesc.h"
#endif

/****************************************************************************\
|
|	Forward Definitions
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION	(USBPUMP_CLASSKIT_CONFIG_TREE);
__TMS_TYPE_DEF_UNION	(USBPUMP_CLASSKIT_CONFIG_DESC);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_CLASSKIT_FSM);
__TMS_TYPE_DEF		(USBPUMP_CLASSKIT_FSM_STATE, __TMS_UINT);

__TMS_TYPE_DEF_FUNCTION(
USBPUMP_CLASSKIT_FSM_FINALIZE_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_CLASSKIT_FUNCTION *	/* pFunction */
		));

__TMS_TYPE_DEF_FUNCTION(
USBPUMP_CLASSKIT_FSM_PROCESS_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_CLASSKIT_FUNCTION *	/* pFunction */,
		__TMS_BOOL				/* fEntry */
		));

__TMS_TYPE_DEF_FUNCTION(
USBPUMP_CLASSKIT_FSM_STATE_CHANGE_FN,
	__TMS_VOID,
		(
		__TMS_USBPUMP_CLASSKIT_FUNCTION *	/* pFunction */,
		__TMS_USBPUMP_CLASSKIT_FSM_STATE	/* PreviousState */,
		__TMS_USBPUMP_CLASSKIT_FSM_STATE	/* CurrentState */
		));


/****************************************************************************\
|
|	ClassKit Device FSM
|
\****************************************************************************/

union __TMS_UNIONNAME	(USBPUMP_CLASSKIT_CONFIG_TREE)
	{
	__TMS_USBPUMP_USBDI_CFG_NODE	ConfigNode;

	/* minimum buffer size */
	__TMS_UINT8	Buffer[ sizeof(__TMS_USBPUMP_USBDI_CFG_NODE) +
				sizeof(__TMS_USBPUMP_USBDI_IFC_NODE) +
				sizeof(__TMS_USBPUMP_USBDI_ALTSET_NODE) +
				sizeof(__TMS_USBPUMP_USBDI_PIPE_NODE) ];
	};

union __TMS_UNIONNAME	(USBPUMP_CLASSKIT_CONFIG_DESC)
	{
	__TMS_USBIF_CFGDESC_WIRE	ConfigDesc;

	__TMS_UINT8	Buffer[__TMS_LENGTHOF_USBIF_CFGDESC_WIRE];
	};

struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_FSM)
	{
	__TMS_USBPUMP_CLASSKIT_FSM_STATE		CurrentState;
	__TMS_USBPUMP_CLASSKIT_FSM_STATE		UnconfiguredState;
	__TMS_USBPUMP_CLASSKIT_FSM_STATE		ConfiguredState;

	__TMS_UINT8					iConfig;
	__TMS_UINT8					ErrorCount;
	__TMS_UINT8					MaxErrorCount;
	__TMS_UINT8					Reserved1;

	/* flags */
	/*
	|| external flags, set outside the FSM, read inside the FSM
	*/
	__TMS_UINT					fUnplugged: 1;
	__TMS_UINT					fRun: 1;

	__TMS_UINT					fSuspendPortIfError: 1;
	__TMS_UINT					fResetPortBeforeEnum: 1;
	__TMS_UINT					fSetConfigExplicit: 1;

	__TMS_UINT					fEnumerated: 1;
	__TMS_UINT					fConfigured: 1;

	__TMS_UINT					fDoSetConfig0: 1;
	__TMS_UINT					fDoSetConfigN: 1;

	__TMS_UINT					fGetVendorString: 1;
	__TMS_UINT					fGetProductString: 1;
	__TMS_UINT					fGetSerialString: 1;

	__TMS_UINT					fWaitClassDriverDone: 1;

	/*
	|| external flags reserved for class driver specific usage
	*/
	__TMS_UINT					fExternalFlag1: 1;
	__TMS_UINT					fExternalFlag2: 1;
	__TMS_UINT					fExternalFlag3: 1;
	__TMS_UINT					fExternalFlag4: 1;


	__TMS_USBPUMP_USBDI_TIMEOUT			DeviceRequestTimeout;

	/**********************************************\
	| internal flags, visible to the outside world |
	\**********************************************/

	/*
	|| fBusy is set while control is dynamically descended from
	|| hub evaluation loop.  Used for detecting reentry
	*/
	__TMS_UINT					fBusy: 1;

	/*
	|| fDeferred is set whenever reentry is detected, and forces
	|| an extra evaluation step.  Note that a non-"state entry"
	|| action that reenters the state machine on every evaluation will
	|| cause an indefinite loop in the FSM.
	*/
	__TMS_UINT					fDeferred: 1;

	/*
	|| internal-use-only flags
	*/

	/*
	|| fUrbPending is set when an URB is posted to USBDI, and cleared
	|| when the URB completes.
	|| fUrbCompleteOK is reset when an urb is posted to USBDI, and set
	|| when the URB completes successfully.
	|| fUrbCompleteNG is reset when an URB is posted to USBDI, and set
	|| when the URB completes with an error.
	*/
	__TMS_UINT					fUrbPending: 1;
	__TMS_UINT					fUrbCompleteOK: 1;
	__TMS_UINT					fUrbCompleteNG: 1;

	__TMS_UINT					fTimer: 1;
	__TMS_UINT					fTimerError: 1;
	__TMS_UINT					fTimerPending: 1;

	__TMS_UINT					fUnconfiguredNG: 1;
	__TMS_UINT					fUnconfiguredOK: 1;

	__TMS_UINT					fNoBusPower: 1;
	__TMS_UINT					fNoMemory: 1;

	__TMS_UINT					fSendArrivalEvent: 1;
	__TMS_UINT					fSendDepartureEvent: 1;

	__TMS_UINT					fRestoreNextCfgNode: 1;
	__TMS_UINT					fDefineConfigDone: 1;
	__TMS_UINT					fSetConfigDelay: 1;

	/*
	|| internal flags reserved for class driver specific usage
	*/
	__TMS_UINT					fInternalFlag1: 1;
	__TMS_UINT					fInternalFlag2: 1;
	__TMS_UINT					fInternalFlag3: 1;
	__TMS_UINT					fInternalFlag4: 1;
	__TMS_UINT					fInternalFlag5: 1;
	__TMS_UINT					fInternalFlag6: 1;
	__TMS_UINT					fInternalFlag7: 1;
	__TMS_UINT					fInternalFlag8: 1;

	/*
	|| Internal: timer used for doing sequencing.
	*/
	__TMS_USBPUMP_TIMER				Timer;

	__TMS_USBPUMP_URB *				pUrbSetConfig;

	/*
	|| The finalization info
	*/
	__TMS_USBPUMP_CLASSKIT_FSM_FINALIZE_FN *	pFinalizeFn;

	__TMS_USBPUMP_CLASSKIT_FSM_PROCESS_FN *		pUnconfiguredProcessFn;
	__TMS_USBPUMP_CLASSKIT_FSM_PROCESS_FN *		pConfiguredProcessFn;
	__TMS_USBPUMP_CLASSKIT_FSM_STATE_CHANGE_FN *	pStateChangeFn;
	};


/****************************************************************************\
|
|	The USBDI ClassKit driver function object
|
\****************************************************************************/

struct __TMS_STRUCTNAME	(USBPUMP_CLASSKIT_FUNCTION_CONTENTS)
	{
	__TMS_USBPUMP_USBDI_FUNCTION_CONTENTS__STRUCT;

	__TMS_USBPUMP_CLASSKIT_SESSION *		pSessionHead;
	__TMS_UPLATFORM *				pPlatform;

	__TMS_UINT					nOpened: 8;

	__TMS_UINT					fAllowMultipleOpen: 1;

	/*
	|| flags reserved for class driver specific use
	*/
	__TMS_UINT					fClassDriverFlag1: 1;
	__TMS_UINT					fClassDriverFlag2: 1;
	__TMS_UINT					fClassDriverFlag3: 1;
	__TMS_UINT					fClassDriverFlag4: 1;

	/*
	|| A flag to tell us if the FSM is running.
	*/
	__TMS_UINT					fFsmRunning: 1;

	/*
	|| The ClassKit Device FSM
	*/
	__TMS_USBPUMP_CLASSKIT_FSM			Fsm;

	/*
	|| Urb used for Device requests originating from the ClassKit Driver.
	*/
	__TMS_USBPUMP_URB *				pUrb;

	/*
	|| If we're unplugging, we need to hold onto the IoctlQe
	*/
	__TMS_USBPUMP_IOCTL_QE *			pDepartureIoctlQe;

	/*
	|| the config tree information
	*/
	__TMS_USBPUMP_CLASSKIT_CONFIG_TREE *		pConfigTree;
	__TMS_UINT16					nConfigTree;

	/*
	|| Pointer to the config tree in use
	*/
	__TMS_UINT16					nConfigRoot;
	__TMS_USBPUMP_USBDI_CFG_NODE *			pConfigRoot;

	/*
	|| the device descriptor buffer
	*/
	__TMS_USBIF_DEVDESC_WIRE			DeviceDescriptor;

	__TMS_UINT16					wLANGID;
	__TMS_UINT8 *					pStringBuffer;
	__TMS_CHAR *					pVendorString;
	__TMS_CHAR *					pProductString;
	__TMS_CHAR *					pSerialString;

	/*
	|| the config descriptor buffer
	*/
	__TMS_BYTES					sizeConfigDescBundles;
	__TMS_USBPUMP_CLASSKIT_CONFIG_DESC *		pConfigDesc;
	__TMS_USBPUMP_CLASSKIT_CONFIG_DESC **		pvConfigDescs;
	__TMS_USBPUMP_CLASSKIT_CONFIG_DESC *		pConfigDescBundles;
	};

union __TMS_UNIONNAME	(USBPUMP_CLASSKIT_FUNCTION)
	{
	__TMS_USBPUMP_USBDI_FUNCTION_CONTENTS__UNION;
	__TMS_USBPUMP_CLASSKIT_FUNCTION_CONTENTS	ClassKit;
	};


#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_CONTENTS__STRUCT		\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_CONTENTS	ClassKit

#define	__TMS_USBPUMP_CLASSKIT_FUNCTION_CONTENTS__UNION			\
	__TMS_USBPUMP_USBDI_FUNCTION_CONTENTS__UNION;			\
	__TMS_USBPUMP_CLASSKIT_FUNCTION_CONTENTS	ClassKit;	\
	__TMS_USBPUMP_CLASSKIT_FUNCTION			ClassKitCast


#define __TMS_USBPUMP_OBJECT_HEADER_TO_CLASSKIT_FUNCTION(pObject)	\
	((__TMS_USBPUMP_CLASSKIT_FUNCTION *) &(pObject)->Tag)

#define	__TMS_USBPUMP_USBDI_FUNCTION_TO_CLASSKIT_DRIVER_CLASS(pFn)	\
	((__TMS_USBPUMP_CLASSKIT_CLASS *)				\
	 (pFn)->Function.ObjectHeader.pClassParent)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpClassKitI_DeinitFunction(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *		pFunction,
	__TMS_USBPUMP_USBDI_DRIVER_CLASS *		pDriverClass
	);

__TMS_VOID
UsbPumpClassKitI_InitFunction_V1(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *		pFunction,
	__TMS_USBPUMP_USBDI_DRIVER_CLASS *		pDriverClass,
	__TMS_BOOL					fAllowMultipleOpen
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpClassKitI_InstanceArrival_V1(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *		pFunction,
	__TMS_USBPUMP_CLASSKIT_FSM_PROCESS_FN *		pUnconfiguredProcessFn,
	__TMS_USBPUMP_CLASSKIT_FSM_PROCESS_FN *		pConfiguredProcessFn,
	__TMS_USBPUMP_CLASSKIT_FSM_STATE_CHANGE_FN *	pStateChangeFn
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpClassKitI_InstanceDeparture(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *		pFunction,
	__TMS_USBPUMP_IOCTL_QE *			pIoctlQe
	);

__TMS_VOID
UsbPumpClassKitI_SendFunctionNotification(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *		pFunction,
	__TMS_USBPUMP_CLASSKIT_NOTIFICATION		NotificationId,
	__TMS_CONST __TMS_VOID *			pNotificationInfo,
	__TMS_BYTES					sizeNotificationInfo
	);

__TMS_BOOL
UsbPumpClassKitFunction_HbufferEnterScope(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_USBPUMP_BUFFER_HANDLE *		phBuffer,
	__TMS_VOID *				pBuffer,
	__TMS_BYTES				nBuffer
	);

__TMS_VOID
UsbPumpClassKitFunction_HbufferExitScope(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_USBPUMP_BUFFER_HANDLE *		phBuffer
	);

/*
|| UsbPumpClassKitI_FunctionIoctl is sample code.  Function driver should
|| implement own IOCTL handler based on this.
*/
__TMS_USBPUMP_OBJECT_IOCTL_FN
UsbPumpClassKitI_FunctionIoctl;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_CLASSKIT_FUNCTION_CONTENTS__STRUCT	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_CONTENTS__STRUCT
# define USBPUMP_CLASSKIT_FUNCTION_CONTENTS__UNION	\
   __TMS_USBPUMP_CLASSKIT_FUNCTION_CONTENTS__UNION
# define USBPUMP_OBJECT_HEADER_TO_CLASSKIT_FUNCTION(pObject)	\
   __TMS_USBPUMP_OBJECT_HEADER_TO_CLASSKIT_FUNCTION(pObject)
# define USBPUMP_USBDI_FUNCTION_TO_CLASSKIT_DRIVER_CLASS(pFn)	\
   __TMS_USBPUMP_USBDI_FUNCTION_TO_CLASSKIT_DRIVER_CLASS(pFn)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_classkit_function.h ****/
#endif /* _USBPUMP_CLASSKIT_FUNCTION_H_ */
