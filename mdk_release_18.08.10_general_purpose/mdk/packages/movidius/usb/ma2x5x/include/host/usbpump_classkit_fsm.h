/* usbpump_classkit_fsm.h	Fri Apr 07 2017 10:24:12 chwon */

/*

Module:  usbpump_classkit_fsm.h

Function:
	Definition of ClassKit Device Finite State Machine

Version:
	V3.21a	Fri Apr 07 2017 10:24:12 chwon	Edit level 11

Copyright notice:
	This file copyright (C) 2008, 2011, 2013, 2015, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	April 2008

Revision history:
   1.97k  Tue Apr 08 2008 14:22:45 chwon
	Module created.

   1.97k  Tue Jul 01 2008 13:00:12  chwon
	4255: include usbpump_classkit_types.h

   1.97k  Wed Jul 16 2008 08:35:08  maw
	4255: include usbpump_classkit_function.h

   1.97k  Fri Aug 01 2008 11:30:32  chwon
	Add UsbPumpClassKitI_ClassKitFsm_UnconfiguedStateDone()

   1.97k  Fri Aug 29 2008 13:54:27  chwon
	Add pDoneInfo in the UsbPumpClassKitI_ClassKitFsm_SendDeviceRequest().
	Fix typo from UsbPumpClassKitI_ClassKitFsm_UnconfiguedStateDone()
	to UsbPumpClassKitI_ClassKitFsm_UnconfiguredStateDone().

   1.97k  Tue Sep 02 2008 16:36:08  chwon
	Fix typo from pUnconfiguedProcessFn to pUnconfiguredProcessFn.
	Fix typo from pConfiguedProcessFn to pConfiguredProcessFn.

   3.01f  Fri Jun 03 2011 11:14:43  chwon
	13458: 13266: add stErrorCancelUrb state.

   3.11c  Sun Apr 14 2013 22:20:07  chwon
	17015: add stSelectingConfig state.

   3.11d  Tue Jul 16 2013 13:21:21  chwon
	17478: add stSetConfigDelay state. Add timer function prototypes.

   3.15b  Mon Mar 16 2015 13:13:18  chwon
	18673: Remove get device & configuration descriptor states and
	ClassKit FSM functions.

   3.15b  Tue Jun 09 2015 10:55:00  chwon
	19105: Add stGettingLangIds and stGettingStrings states.

   3.21a  Fri Apr 07 2017 10:24:12  chwon
	20517: Add UsbPumpClassKitFunction_GetDeviceRequestTimeout(),
	UsbPumpClassKitFunction_SendDeviceRequest(),
	UsbPumpClassKitFunction_SendGetStringDesc() and
	UsbPumpClassKitFunction_GetStringDescComplete() prototype.

*/

#ifndef _USBPUMP_CLASSKIT_FSM_H_	/* prevent multiple includes */
#define _USBPUMP_CLASSKIT_FSM_H_

#ifndef _USBPUMP_CLASSKIT_TYPES_H_
# include "usbpump_classkit_types.h"
#endif

#ifndef _USBPUMP_CLASSKIT_FUNCTION_H_
# include "usbpump_classkit_function.h"
#endif

/****************************************************************************\
|
|	The states for the ClassKit Device FSM
|
|	You can conveniently generate these in KSH using the commands
|	given in host/notes/tmmnotes.txt (search for HubFsm.cdd)
|
|	Note that the .cdd file is a Cadifra UML Editor State diagram, is
|	in XML, and is to be maintained as a reference.  Apart from the
|	states themselves, we don't generate code from the diagrams so
|	they're mostly informative.
|
\****************************************************************************/

#define	__TMS_USBPUMP_CLASSKIT_FSM_stNoChange			0
#define	__TMS_USBPUMP_CLASSKIT_FSM_stInitial			1
#define	__TMS_USBPUMP_CLASSKIT_FSM_stSuggestingConfig		2
#define	__TMS_USBPUMP_CLASSKIT_FSM_stDefiningConfig		3
#define __TMS_USBPUMP_CLASSKIT_FSM_stUnconfigured		4
#define __TMS_USBPUMP_CLASSKIT_FSM_stSettingConfig		5
#define __TMS_USBPUMP_CLASSKIT_FSM_stConfigured			6
#define __TMS_USBPUMP_CLASSKIT_FSM_stSettingConfig0		7
#define __TMS_USBPUMP_CLASSKIT_FSM_stSettingConfigN		8
#define	__TMS_USBPUMP_CLASSKIT_FSM_stErrorUndoCfg		9
#define	__TMS_USBPUMP_CLASSKIT_FSM_stErrorNotCfg		10
#define	__TMS_USBPUMP_CLASSKIT_FSM_stErrorCancelUrb		11
#define	__TMS_USBPUMP_CLASSKIT_FSM_stSelectingConfig		12
#define	__TMS_USBPUMP_CLASSKIT_FSM_stSetConfigDelay		13
#define	__TMS_USBPUMP_CLASSKIT_FSM_stGettingLangIds		14
#define	__TMS_USBPUMP_CLASSKIT_FSM_stGettingStrings		15
#define	__TMS_USBPUMP_CLASSKIT_FSM_st__MAX			16

#define __TMS_USBPUMP_CLASSKIT_FSM_STATES__INIT	\
	"stNoChange",				\
	"stInitial",				\
	"stSuggestingConfig",			\
	"stDefiningConfig",			\
	"stUnconfigured",			\
	"stSettingConfig",			\
	"stConfigured",				\
	"stSettingConfig0",			\
	"stSettingConfigN",			\
	"stErrorUndoCfg",			\
	"stErrorNotCfg",			\
	"stErrorCancelUrb",			\
	"stSelectingConfig",			\
	"stSetConfigDelay",			\
	"stGettingLangIds",			\
	"stGettingStrings",			\
	"st__MAX"


/****************************************************************************\
|
|	ClassKit FSM Functions
|
\****************************************************************************/

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_Init(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *		pFunction,
	__TMS_USBPUMP_CLASSKIT_FSM_FINALIZE_FN *	pFinalizeFn,
	__TMS_USBPUMP_CLASSKIT_FSM_PROCESS_FN *		pUnconfiguredProcessFn,
	__TMS_USBPUMP_CLASSKIT_FSM_PROCESS_FN *		pConfiguredProcessFn,
	__TMS_USBPUMP_CLASSKIT_FSM_STATE_CHANGE_FN *	pStateChangeFn
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_RequestExit(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_Eval(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_SetConfig0(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_SetConfigN(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_UINT8				iConfig
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_CompleteSetConfigUrb(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_USTAT				Status
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_SetConfiguration(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_USBPUMP_URB *			pUrb,
	__TMS_UINT				ConfigNumber
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_SetEnumerated(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_UnconfiguredStateDone(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_BOOL				fCompleteOk
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_ClearAsyncBits(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_SendDeviceRequest(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_UINT8				bmReqType,
	__TMS_UINT8				bRequest,
	__TMS_UINT16				wIndex,
	__TMS_UINT16				wValue,
	__TMS_UINT16				wLength,
	__TMS_VOID *				pBuffer,
	__TMS_USBPUMP_URB_DONE_FN *		pDoneFn,
	__TMS_VOID *				pDoneInfo
	);

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_CancelUrb(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction
	);

__TMS_USBPUMP_URB_DONE_FN
UsbPumpClassKitI_ClassKitFsm_UrbComplete;

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_TimerCancel(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction
	);

__TMS_USBPUMP_TIMER_DONE_FN
UsbPumpClassKitI_ClassKitFsm_TimerEvent;

__TMS_VOID
UsbPumpClassKitI_ClassKitFsm_TimerStart(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_USBPUMP_TIMER_TIMEOUT		Timeout
	);

__TMS_USBPUMP_USBDI_TIMEOUT
UsbPumpClassKitFunction_GetDeviceRequestTimeout(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_CONST __TMS_VOID *		pBuffer,
	__TMS_BYTES				nBuffer
	);

__TMS_VOID
UsbPumpClassKitFunction_SendDeviceRequest(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_UINT8				bmReqType,
	__TMS_UINT8				bRequest,
	__TMS_UINT16				wIndex,
	__TMS_UINT16				wValue,
	__TMS_UINT16				wLength,
	__TMS_VOID *				pBuffer,
	__TMS_USBPUMP_URB_DONE_FN *		pDoneFn,
	__TMS_VOID *				pDoneInfo,
	__TMS_USBPUMP_BUFFER_HANDLE		hBuffer
	);

__TMS_VOID
UsbPumpClassKitFunction_SendGetStringDesc(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_USBPUMP_URB_DONE_FN *		pDoneFn,
	__TMS_UINT8				stringIndex
	);

__TMS_CHAR *
UsbPumpClassKitFunction_GetStringDescComplete(
	__TMS_USBPUMP_CLASSKIT_FUNCTION *	pFunction,
	__TMS_USBPUMP_URB *			pUrb,
	__TMS_ARG_USTAT				Status
	);


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_CLASSKIT_FSM_stNoChange	\
   __TMS_USBPUMP_CLASSKIT_FSM_stNoChange
# define USBPUMP_CLASSKIT_FSM_stInitial	\
   __TMS_USBPUMP_CLASSKIT_FSM_stInitial
# define USBPUMP_CLASSKIT_FSM_stSuggestingConfig	\
   __TMS_USBPUMP_CLASSKIT_FSM_stSuggestingConfig
# define USBPUMP_CLASSKIT_FSM_stDefiningConfig	\
   __TMS_USBPUMP_CLASSKIT_FSM_stDefiningConfig
# define USBPUMP_CLASSKIT_FSM_stUnconfigured	\
   __TMS_USBPUMP_CLASSKIT_FSM_stUnconfigured
# define USBPUMP_CLASSKIT_FSM_stSettingConfig	\
   __TMS_USBPUMP_CLASSKIT_FSM_stSettingConfig
# define USBPUMP_CLASSKIT_FSM_stConfigured	\
   __TMS_USBPUMP_CLASSKIT_FSM_stConfigured
# define USBPUMP_CLASSKIT_FSM_stSettingConfig0	\
   __TMS_USBPUMP_CLASSKIT_FSM_stSettingConfig0
# define USBPUMP_CLASSKIT_FSM_stSettingConfigN	\
   __TMS_USBPUMP_CLASSKIT_FSM_stSettingConfigN
# define USBPUMP_CLASSKIT_FSM_stErrorUndoCfg	\
   __TMS_USBPUMP_CLASSKIT_FSM_stErrorUndoCfg
# define USBPUMP_CLASSKIT_FSM_stErrorNotCfg	\
   __TMS_USBPUMP_CLASSKIT_FSM_stErrorNotCfg
# define USBPUMP_CLASSKIT_FSM_stErrorCancelUrb	\
   __TMS_USBPUMP_CLASSKIT_FSM_stErrorCancelUrb
# define USBPUMP_CLASSKIT_FSM_stSelectingConfig	\
   __TMS_USBPUMP_CLASSKIT_FSM_stSelectingConfig
# define USBPUMP_CLASSKIT_FSM_stSetConfigDelay	\
   __TMS_USBPUMP_CLASSKIT_FSM_stSetConfigDelay
# define USBPUMP_CLASSKIT_FSM_stGettingLangIds	\
   __TMS_USBPUMP_CLASSKIT_FSM_stGettingLangIds
# define USBPUMP_CLASSKIT_FSM_stGettingStrings	\
   __TMS_USBPUMP_CLASSKIT_FSM_stGettingStrings
# define USBPUMP_CLASSKIT_FSM_st__MAX	\
   __TMS_USBPUMP_CLASSKIT_FSM_st__MAX
# define USBPUMP_CLASSKIT_FSM_STATES__INIT	\
   __TMS_USBPUMP_CLASSKIT_FSM_STATES__INIT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_classkit_fsm.h ****/
#endif /* _USBPUMP_CLASSKIT_FSM_H_ */
