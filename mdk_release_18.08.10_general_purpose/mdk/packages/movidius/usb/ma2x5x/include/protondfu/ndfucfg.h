/* ndfucfg.h	Thu Oct 28 2010 13:49:20 maw */

/*

Module:  ndfucfg.h

Function:
	Data structures to configure a DFU download protocol

Version:
	V3.01d	Thu Oct 28 2010 13:49:20 maw	Edit level 3

Copyright notice:
	This file copyright (C) 2004,2010 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjorn, MCCI Corporation	June 2004

Revision history:
   1.89a  Tue Jun 08 2004 11:43:59 maw
	Module created.

   1.89b  Tue Sep 14 2004 09:17:04 maw
   	State switch macro-names renamed to ..._V1

   3.01d  Thu Oct 28 2010 13:49:20  maw
   	11310: Added option to use DP timer service
   	11750: Add debug flags to config

*/

#ifndef _NDFUCFG_H_		/* prevent multiple includes */
#define _NDFUCFG_H_

#include "usbdfu10.h"
#include "usbioctl_download.h"

/****************************************************************************\
|
|	Forward type declarations.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UPROTO_NDFU);
__TMS_TYPE_DEF_STRUCT(UPROTO_NDFU_CONFIG); 
__TMS_TYPE_DEF_STRUCT(USTATESWITCH_NDFU);
__TMS_TYPE_DEF_STRUCT(USTATESWITCH_NDFU_FN);

/****************************************************************************\
|
|	The configuration structure
|
\****************************************************************************/

/*

Type:	UPROTO_NDFU_CONFIG

Function:
	Carries configuration information into the portable DFU configuration
	logic.

Contents:
	CONST USTATESWITCH_NDFU * pStateSwitch;
		Pointer to state-handler table. It is used to replace unused 
		state-handlers (i.e read functions if read isn't allowed) with 
		dummy routines so that the DFU firmware image can be minimized.

	USBPUMP_DOWNLOAD_DEVICE_MODE * pDeviceMode;
		Pointer to flag indicating whether the device started in
		USB_Dfu_DeviceMode_APP or USB_Dfu_DeviceMode_DFU

	BOOL fUseDataPumpTimer;
		Flag to tell NDFU whether to use DataPump timer serivce or not.
		If this flag is TRUE then USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR
		will not be issued by protocol.

	BOOL fNeedWarmStart;
		Flag to tell NDFU whether device needs a warm restart to switch
		between APP and DFU mode. This is used during startup when 
		allocating certain resources only used in DFU mode.

	UINT32 DebugFlags;
		Debug flags to be used by NDFU protocol. A value != 0 will 
		overwrite flags set in USB_DATAPUMP_APPLICATION_INIT_VECTOR.

Notes:
	This structure is normally initialized using
	UPROTO_NDFU_CONFIG_INIT_Vx(), or UPROTO_NDFU_CONFIG_SETUP_Vx.

See Also:

*/
struct __TMS_STRUCTNAME(UPROTO_NDFU_CONFIG)
	{
	__TMS_CONST __TMS_USTATESWITCH_NDFU *	pStateSwitch;
	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE *	pDeviceMode;
	__TMS_BOOL				fUseDataPumpTimer;
	__TMS_BOOL				fNeedWarmStart;
	__TMS_UINT32				DebugFlags;
	};

/*

Name:	UPROTO_NDFU_CONFIG_SETUP_V2()

Function:
	Macro: runtime initialization code for UPROTO_NDFU_CONFIG()

Definition:
	VOID UPROTO_NDFU_CONFIG_SETUP_V1(
		UPROTO_NDFU_CONFIG *		pConfig,
		CONST USTATESWITCH_NDFU *	pStateSwitch,
		USBPUMP_DOWNLOAD_DEVICE_MODE *	pDeviceMode,			
		BOOL				fUseDataPumpTimer,
		BOOL				fNeedWarmStart,
		UINT32				DebugFlags
		);

Description:
	This macro allows code to initialize a UPROTO_NDFU_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_NDFU_CONFIG_INIT_V2()

*/

/* don't add a parameter to this macro, create a _V3... instead */
#define	__TMS_UPROTO_NDFU_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode,					\
		ARG_fUseDataPumpTimer,					\
		ARG_fNeedWarmStart,					\
		ARG_DebugFlags						\
		)							\
	do	{							\
		(pConfig)->pStateSwitch	     = (ARG_pStateSwitch);	\
		(pConfig)->pDeviceMode	     = (ARG_pDeviceMode);	\
		(pConfig)->fUseDataPumpTimer = (ARG_fUseDataPumpTimer);	\
		(pConfig)->fNeedWarmStart    = (ARG_fNeedWarmStart);	\
		(pConfig)->DebugFlags      = (ARG_DebugFlags);	\
		} while (0)

#define	__TMS_UPROTO_NDFU_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode						\
		)							\
	__TMS_UPROTO_NDFU_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode,					\
		__TMS_FALSE,						\
		__TMS_TRUE,						\
		0							\
		)

/*

Name:	UPROTO_NDFU_CONFIG_INIT_V2()

Function:
	Macro: compile time initialization for UPROTO_NDFU_CONFIG.

Definition:
	UPROTO_NDFU_CONFIG_INIT_V1(
		CONST USTATESWITCH_NDFU *	pStateSwitch,
		USBPUMP_DOWNLOAD_DEVICE_MODE *	pDeviceMode,			
		BOOL				fUseDataPumpTimer,
		BOOL				fNeedWarmStart,
		UINT32				DebugFlags
		);
	
Description:
	This macro is used when initializing a single UPROTO_NDFU_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V2() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V3... instead */
#define	__TMS_UPROTO_NDFU_CONFIG_INIT_V2(				\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode,					\
		ARG_fUseDataPumpTimer,					\
		ARG_fNeedWarmStart,					\
		ARG_DebugFlags						\
		)							\
	{								\
	(ARG_pStateSwitch),						\
	(ARG_pDeviceMode),						\
	(ARG_fUseDataPumpTimer),					\
	(ARG_fNeedWarmStart),						\
	(ARG_DebugFlags)						\
	}
	
#define	__TMS_UPROTO_NDFU_CONFIG_INIT_V1(				\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode						\
		)							\
	__TMS_UPROTO_NDFU_CONFIG_INIT_V2(				\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode,					\
		__TMS_FALSE,						\
		__TMS_TRUE,						\
		0							\
		)

/****************************************************************************\
|
|	State function types
|
\****************************************************************************/

__TMS_FNTYPE_DEF(	/* State handler function */
USTATESWITCH_NDFU_STATEHANDLER_FN, 
__TMS_BOOL, 	(
	__TMS_PUPROTO_NDFU
	));
	
__TMS_FNTYPE_DEF(	/* Goto-state function */
USTATESWITCH_NDFU_INITSTATE_FN, 
__TMS_BOOL, 	(
	__TMS_PUPROTO_NDFU
	));

/****************************************************************************\
|
|	The state-switch is used by protocol to call protocol handlers
|	int the protocol object.  We use this in order to keep the non-init 
|	code as flexible as possible.
|
\****************************************************************************/

/*
|| *** The state-function switch for each state ***
*/
struct __TMS_STRUCTNAME(USTATESWITCH_NDFU_FN)
	{
	__TMS_PUSTATESWITCH_NDFU_STATEHANDLER_FN	Handler;
	__TMS_PUSTATESWITCH_NDFU_INITSTATE_FN		Init;
	};

#define	__TMS_USTATESWITCH_NDFU_FN_INIT_V1(HandlerFn, InitFn) \
	{ HandlerFn, InitFn }

#define	__TMS_USTATESWITCH_NDFU_FN_INIT_STD(HandlerFn, InitFn) \
	__TMS_USTATESWITCH_NDFU_FN_INIT_V1( HandlerFn, InitFn)

/* Never add an argument to this macro; instead, create a V2 or higher */
#define	__TMS_USTATESWITCH_NDFU_FN_DECLARE_FNS_V1(HandlerFn, InitFn) \
	static __TMS_USTATESWITCH_NDFU_STATEHANDLER_FN	HandlerFn; \
	static __TMS_USTATESWITCH_NDFU_INITSTATE_FN	InitFn

#define	__TMS_USTATESWITCH_NDFU_FN_DECLARE_FNS_STD(HandlerFn, InitFn) \
	__TMS_USTATESWITCH_NDFU_FN_DECLARE_FNS_V1( HandlerFn, InitFn)

/* Declare and init the state-switch */
#define	__TMS_USTATESWITCH_NDFU_FN_DECLARE_STD(HandlerFn, InitFn, State) \
	__TMS_USTATESWITCH_NDFU_FN_DECLARE_FNS_STD(HandlerFn, InitFn);   \
	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN NDfuI_State##State##Switch =   \
		__TMS_USTATESWITCH_NDFU_FN_INIT_STD(HandlerFn, InitFn)

extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State0Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State1Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State1Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State2Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State3Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State4Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State5Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State6Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State7Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State8Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State9Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_State10Switch;
extern	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN	NDfuI_StateNoneSwitch;

/*
|| *** The state-function switch for all states ***
*/

struct __TMS_STRUCTNAME(USTATESWITCH_NDFU)
	{
	__TMS_ULONG					MagicBegin;
	__TMS_CONST __TMS_USTATESWITCH_NDFU_FN *	State[USB_Dfu_ProtoState_CNT];
	__TMS_ULONG					MagicEnd;
	};

#define	__TMS_USTATESWITCH_NDFU_BEGIN	(0x58021878 + '<')
#define	__TMS_USTATESWITCH_NDFU_END	(0x78180258 + '>')

/*
|| Declare the initialization explicitly for a callout (proto) switch.
|| Never add an argument to this macro; instead create a V2 or higher
*/
#define	__TMS_USTATESWITCH_NDFU_INIT_V1(		\
	State0,						\
	State1,						\
	State2,						\
	State3,						\
	State4,						\
	State5,						\
	State6,						\
	State7,						\
	State8,						\
	State9,						\
	State10						\
	)						\
	{						\
	__TMS_USTATESWITCH_NDFU_BEGIN,			\
		{					\
		&NDfuI_State##State0##Switch,		\
		&NDfuI_State##State1##Switch,		\
		&NDfuI_State##State2##Switch,		\
		&NDfuI_State##State3##Switch,		\
		&NDfuI_State##State4##Switch,		\
		&NDfuI_State##State5##Switch,		\
		&NDfuI_State##State6##Switch,		\
		&NDfuI_State##State7##Switch,		\
		&NDfuI_State##State8##Switch,		\
		&NDfuI_State##State9##Switch,		\
		&NDfuI_State##State10##Switch		\
		},					\
	__TMS_USTATESWITCH_NDFU_END			\
	}

/*
|| Init the switch in a standard way -- you can change this definition to
|| use V2, or whatever, as needed.
*/
#define	__TMS_USTATESWITCH_NDFU_INIT_ALLSTATES_V1 \
	__TMS_USTATESWITCH_NDFU_INIT_V1( \
		0, 1, 2, 3, 4, 5, \
		6, 7, 8, 9, 10 \
		)

/*
|| Declare and init the switch in a standard way.
||
|| *** Use one of the following macros to declare the stateswitch ***
*/

/*
|| This switch is used to declare and initialize a state switch
|| containing all states (both application and dfu mode)
*/
#define	__TMS_USTATESWITCH_NDFU_DECLARE_ALLSTATES_V1(Prefix)\
	__TMS_CONST __TMS_USTATESWITCH_NDFU Prefix##_kAllStateSwitch = \
		__TMS_USTATESWITCH_NDFU_INIT_ALLSTATES_V1

/*
|| This switch is used to declare and initialize a state switch
|| containing only application-mode states
*/
#define	__TMS_USTATESWITCH_NDFU_DECLARE_APPMODE_V1(Prefix)\
	__TMS_CONST USTATESWITCH_NDFU Prefix##_kAppModeStateSwitch = \
		__TMS_USTATESWITCH_NDFU_INIT_V1( \
			0, 1, \
			None, None, None, None, None, \
			None, None, None, None \
		)

/*
|| This switch is used to declare and initialize a state switch
|| containing only dfu-mode states used to download data
*/
#define	__TMS_USTATESWITCH_NDFU_DECLARE_DFUMODE_DNLD_V1(Prefix)\
	__TMS_CONST __TMS_USTATESWITCH_NDFU Prefix##_kDfuModeDnLdStateSwitch = \
		__TMS_USTATESWITCH_NDFU_INIT_V1( \
			None, None, \
			2, 3, 4, 5, 6, \
			7, 8, None, 10 \
		)
		
/*
|| This switch is used to declare and initialize a state switch
|| containing only dfu-mode states used to upload data
*/
#define	__TMS_USTATESWITCH_NDFU_DECLARE_DFUMODE_UPLD_V1(Prefix)\
	__TMS_CONST __TMS_USTATESWITCH_NDFU Prefix##_kDfuModeUpLdStateSwitch = \
		__TMS_USTATESWITCH_NDFU_INIT_V1( \
			None, None, \
			2, None, None, None, None, \
			None, None, 9, 10 \
		)

/*
|| This switch is used to declare and initialize a state switch
|| containing all dfu-mode states (both download and upload)
*/
#define	__TMS_USTATESWITCH_NDFU_DECLARE_DFUMODE_DNUPLD_V1(Prefix)\
	__TMS_CONST __TMS_USTATESWITCH_NDFU Prefix##_kDfuModeDnUpLdStateSwitch = \
		__TMS_USTATESWITCH_NDFU_INIT_V1( \
			None, None, \
			2, 3, 4, 5, 6, \
			7, 8, 9, 10 \
		)

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_NDFU_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode,					\
		ARG_fUseDataPumpTimer,					\
		ARG_fNeedWarmStart,					\
		ARG_DebugFlags						\
		)	\
	__TMS_UPROTO_NDFU_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode,					\
		ARG_fUseDataPumpTimer,					\
		ARG_fNeedWarmStart,					\
		ARG_DebugFlags						\
		)
# define UPROTO_NDFU_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode						\
		)	\
	__TMS_UPROTO_NDFU_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode						\
		)
# define UPROTO_NDFU_CONFIG_INIT_V2(				\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode,					\
		ARG_fUseDataPumpTimer,					\
		ARG_fNeedWarmStart,					\
		ARG_DebugFlags						\
		)	\
	__TMS_UPROTO_NDFU_CONFIG_INIT_V2(				\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode,					\
		ARG_fUseDataPumpTimer,					\
		ARG_fNeedWarmStart,					\
		ARG_DebugFlags						\
		)
# define UPROTO_NDFU_CONFIG_INIT_V1(				\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode						\
		)	\
	__TMS_UPROTO_NDFU_CONFIG_INIT_V1(				\
		ARG_pStateSwitch,					\
		ARG_pDeviceMode						\
		)
# define USTATESWITCH_NDFU_FN_INIT_V1(HandlerFn, InitFn)	\
   __TMS_USTATESWITCH_NDFU_FN_INIT_V1(HandlerFn, InitFn)
# define USTATESWITCH_NDFU_FN_INIT_STD(HandlerFn, InitFn)	\
   __TMS_USTATESWITCH_NDFU_FN_INIT_STD(HandlerFn, InitFn)
# define USTATESWITCH_NDFU_FN_DECLARE_FNS_V1(HandlerFn, InitFn)	\
   __TMS_USTATESWITCH_NDFU_FN_DECLARE_FNS_V1(HandlerFn, InitFn)
# define USTATESWITCH_NDFU_FN_DECLARE_FNS_STD(HandlerFn, InitFn)	\
   __TMS_USTATESWITCH_NDFU_FN_DECLARE_FNS_STD(HandlerFn, InitFn)
# define USTATESWITCH_NDFU_FN_DECLARE_STD(HandlerFn, InitFn, State)	\
   __TMS_USTATESWITCH_NDFU_FN_DECLARE_STD(HandlerFn, InitFn, State)
# define USTATESWITCH_NDFU_BEGIN	\
   __TMS_USTATESWITCH_NDFU_BEGIN
# define USTATESWITCH_NDFU_END	\
   __TMS_USTATESWITCH_NDFU_END
# define USTATESWITCH_NDFU_INIT_V1(		\
	State0,						\
	State1,						\
	State2,						\
	State3,						\
	State4,						\
	State5,						\
	State6,						\
	State7,						\
	State8,						\
	State9,						\
	State10						\
	)	\
	__TMS_USTATESWITCH_NDFU_INIT_V1(		\
	State0,						\
	State1,						\
	State2,						\
	State3,						\
	State4,						\
	State5,						\
	State6,						\
	State7,						\
	State8,						\
	State9,						\
	State10						\
	)
# define USTATESWITCH_NDFU_INIT_ALLSTATES_V1	\
   __TMS_USTATESWITCH_NDFU_INIT_ALLSTATES_V1
# define USTATESWITCH_NDFU_DECLARE_ALLSTATES_V1(Prefix)	\
   __TMS_USTATESWITCH_NDFU_DECLARE_ALLSTATES_V1(Prefix)
# define USTATESWITCH_NDFU_DECLARE_APPMODE_V1(Prefix)	\
   __TMS_USTATESWITCH_NDFU_DECLARE_APPMODE_V1(Prefix)
# define USTATESWITCH_NDFU_DECLARE_DFUMODE_DNLD_V1(Prefix)	\
   __TMS_USTATESWITCH_NDFU_DECLARE_DFUMODE_DNLD_V1(Prefix)
# define USTATESWITCH_NDFU_DECLARE_DFUMODE_UPLD_V1(Prefix)	\
   __TMS_USTATESWITCH_NDFU_DECLARE_DFUMODE_UPLD_V1(Prefix)
# define USTATESWITCH_NDFU_DECLARE_DFUMODE_DNUPLD_V1(Prefix)	\
   __TMS_USTATESWITCH_NDFU_DECLARE_DFUMODE_DNUPLD_V1(Prefix)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of ndfucfg.h ****/
#endif /* _NDFUCFG_H_ */
