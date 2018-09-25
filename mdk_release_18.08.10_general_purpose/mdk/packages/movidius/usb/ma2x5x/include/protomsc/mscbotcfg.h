/* mscbotcfg.h		Fri Dec 28 2012 12:34:29 chwon */

/*

Module:  mscbotcfg.h

Function:
	Data structures to configure a USB Mass-Storage Bulk-Only protocol

Version:
	V3.11b	Fri Dec 28 2012 12:34:29 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2002-2006, 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjorn, MCCI Corporation	October 2002

Revision history:
   1.79b  Fri Oct 18 2002 12:00:00  maw
	Module created.

   1.85e  Wed Sep 24 2003 13:00:25  chwon
	Added PUPROTO_MSCBOT_DISABLE_INTERFACE_FN.

   1.89b  Mon Jun 28 2004 11:21:08  chwon
	Bug 1263: add UPROTO_MSCBOT_BUS_EVENT_FN

   1.89b  Wed Sep 15 2004 14:45:23  maw
   	Added number of LUN's parameter to config structure

   1.89c  Wed Oct 13 2004 11:59:48 maw
   	Bug 1343: Added reset-interface function to config structure,
   	updated config to V5, added init fn for V4 & V5

   1.97e  Sat Sep 02 2006 10:15:26  chwon
	Add data-in-stuff feature enable flag

   3.11b  Fri Dec 28 2012 12:34:29  chwon
	16407: Added pObjectHeader in the config structure and added SETUP_V7
	and INIT_V7 macro.

*/

#ifndef _MSCBOTCFG_H_		/* prevent multiple includes */
#define _MSCBOTCFG_H_

#ifndef _PROTOMSC_H_
# include "protomsc.h"
#endif

/****************************************************************************\
|
|	Structures declared here
|
\****************************************************************************/


/****************************************************************************\
|
|	The configuration structure
|
\****************************************************************************/

/*

Type:	UPROTO_MSCBOT_CONFIG

Function:
	Carries configuration information into the portable Mass-Storage Bulk-Only
	configuration logic.

Contents:
	VOID * pSubClass;
		Pointer to subclass context block

	PUPROTO_MSCBOT_PROCESS_CB_FN pProcessCB;
		Pointer to subclass specific function to process CB's

	PUPROTO_MSCBOT_ENABLE_INTERFACE_FN pEnableInterface;
		Pointer to subclass specific function to enable interface

	PUPROTO_MSCBOT_DISABLE_INTERFACE_FN pDisableInterface;
		Pointer to subclass specific function to disable interface

	PUPROTO_MSCBOT_RESET_INTERFACE_FN pResetInterface;
		Pointer to subclass specific function to reset interface.
		Interface enable-state is not changed

	PUPROTO_MSCBOT_BUS_EVENT_FN pBusEvent;
		Pointer to subclass specific function to process bus event

	PUINTERFACE pIfc;
		Pointer to interface to attach protocol on

	BYTES nMaxTxDataSize;
		Max size of data-packet that subclass will submit for transmission

	BYTES nLun;
		Number of logical units

	BOOL fEnableDataInStuff;
		Enable Data IN stuff feature. This is useful if USB device does
		not support endpoint stall feature.

	USBPUMP_OBJECT_HEADER *pObjectHeader;
		Pointer to the DataPump object header. This will be used for
		printing debug message.

Notes:
	This structure is normally initialized using
	UPROTO_MSCBOT_CONFIG_INIT_Vx(), or UPROTO_MSCBOT_CONFIG_SETUP_Vx.

See Also:

*/

struct __TMS_STRUCTNAME(UPROTO_MSCBOT_CONFIG)
	{
	__TMS_VOID * 					pSubClass;
	__TMS_PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB;
	__TMS_PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface;
	__TMS_PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface;
	__TMS_PUPROTO_MSCBOT_RESET_INTERFACE_FN		pResetInterface;
	__TMS_PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent;
	__TMS_PUINTERFACE				pIfc;
	__TMS_BYTES					nMaxTxDataSize;
	__TMS_BYTES					nLun;
	__TMS_BOOL					fEnableDataInStuff;
	__TMS_USBPUMP_OBJECT_HEADER *			pObjectHeader;
	};

/*
Name:	UPROTO_MSCBOT_CONFIG_SETUP_V7()

Function:
	Macro: runtime initialization code for UPROTO_MSCBOT_CONFIG()

Definition:
	VOID UPROTO_MSCBOT_CONFIG_SETUP_V7(
		UPROTO_MSCBOT_CONFIG *			pConfig,
		VOID * 					pSubClass,
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_RESET_INTERFACE_FN	pResetInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize,
		BYTES					nLun,
		BOOL					fEnableDataInStuff,
		USBPUMP_OBJECT_HEADER *			pObjectHeader
		);

Description:
	This macro allows code to initialize a UPROTO_MSCBOT_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCBOT_CONFIG_INIT_V7()

*/

/* don't add a parameter to this macro, create a _V8... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V7(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff,					\
		ARG_pObjectHeader					\
		)							\
	do	{							\
		(pConfig)->pSubClass         = (ARG_pSubClass);		\
		(pConfig)->pProcessCB        = (ARG_pProcessCB);	\
		(pConfig)->pEnableInterface  = (ARG_pEnableInterface);	\
		(pConfig)->pDisableInterface = (ARG_pDisableInterface);	\
		(pConfig)->pResetInterface   = (ARG_pResetInterface);	\
		(pConfig)->pBusEvent         = (ARG_pBusEvent);		\
		(pConfig)->pIfc              = (ARG_pIfc);		\
		(pConfig)->nMaxTxDataSize    = (ARG_nMaxTxDataSize);	\
		(pConfig)->nLun              = (ARG_nLun);		\
		(pConfig)->fEnableDataInStuff = (ARG_fEnableDataInStuff); \
		(pConfig)->pObjectHeader     = (ARG_pObjectHeader);	\
		} while (0)

/*
Name:	UPROTO_MSCBOT_CONFIG_SETUP_V6()

Function:
	Macro: runtime initialization code for UPROTO_MSCBOT_CONFIG()

Definition:
	VOID UPROTO_MSCBOT_CONFIG_SETUP_V6(
		UPROTO_MSCBOT_CONFIG *			pConfig,
		VOID * 					pSubClass,
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_RESET_INTERFACE_FN	pResetInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize,
		BYTES					nLun,
		BOOL					fEnableDataInStuff
		);

Description:
	This macro allows code to initialize a UPROTO_MSCBOT_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCBOT_CONFIG_INIT_V6()

*/

/* don't add a parameter to this macro, create a _V7... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V6(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff					\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V7(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff,					\
		/* V7: pObjectHeader */ __TMS_NULL			\
		)

/*
Name:	UPROTO_MSCBOT_CONFIG_SETUP_V5()

Function:
	Macro: runtime initialization code for UPROTO_MSCBOT_CONFIG()

Definition:
	VOID UPROTO_MSCBOT_CONFIG_SETUP_V5(
		UPROTO_MSCBOT_CONFIG *			pConfig,
		VOID * 					pSubClass,
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_RESET_INTERFACE_FN	pResetInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize,
		BYTES					nLun
		);

Description:
	This macro allows code to initialize a UPROTO_MSCBOT_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCBOT_CONFIG_INIT_V5()

*/

/* don't add a parameter to this macro, create a _V6... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V5(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V6(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		/* fEnableDataInStuff */ __TMS_FALSE			\
		)

/*
Name:	UPROTO_MSCBOT_CONFIG_SETUP_V4()

Function:
	Macro: runtime initialization code for UPROTO_MSCBOT_CONFIG()

Definition:
	VOID UPROTO_MSCBOT_CONFIG_SETUP_V4(
		UPROTO_MSCBOT_CONFIG *			pConfig,
		VOID * 					pSubClass,
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize,
		BYTES					nLun
		);

Description:
	This macro allows code to initialize a UPROTO_MSCBOT_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCBOT_CONFIG_INIT_V4()

*/

/* don't add a parameter to this macro, create a _V5... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V4(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V5(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		__TMS_NULL,						\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_SETUP_V3()

Function:
	Macro: runtime initialization code for UPROTO_MSCBOT_CONFIG()

Definition:
	VOID UPROTO_MSCBOT_CONFIG_SETUP_V3(
		UPROTO_MSCBOT_CONFIG *			pConfig,
		VOID * 					pSubClass,
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize
		);

Description:
	This macro allows code to initialize a UPROTO_MSCBOT_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCBOT_CONFIG_INIT_V3()

*/

/* don't add a parameter to this macro, create a _V4... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V3(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V4(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		1							\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_SETUP_V2()

Function:
	Macro: runtime initialization code for UPROTO_MSCBOT_CONFIG()

Definition:
	VOID UPROTO_MSCBOT_CONFIG_SETUP_V2(
		UPROTO_MSCBOT_CONFIG *			pConfig,
		VOID * 					pSubClass,
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize
		);

Description:
	This macro allows code to initialize a UPROTO_MSCBOT_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCBOT_CONFIG_INIT_V2()

*/

/* don't add a parameter to this macro, create a _V3... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V3(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		(__TMS_PUPROTO_MSCBOT_BUS_EVENT_FN)__TMS_NULL,		\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_SETUP_V1()

Function:
	Macro: runtime initialization code for UPROTO_MSCBOT_CONFIG()

Definition:
	VOID UPROTO_MSCBOT_CONFIG_SETUP_V1(
		UPROTO_MSCBOT_CONFIG *			pConfig,
		VOID * 					pSubClass,
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize
		);

Description:
	This macro allows code to initialize a UPROTO_MSCBOT_CONFIG
	structure in a forward-compatible way.  It initializes the
	structure, supplying defaults if the structure is changed
	in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	UPROTO_MSCBOT_CONFIG_INIT_V1()

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		(__TMS_PUPROTO_MSCBOT_DISABLE_INTERFACE_FN)__TMS_NULL,	\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_INIT_V7()

Function:
	Macro: compile time initialization for UPROTO_MSCBOT_CONFIG.

Definition:
	UPROTO_MSCBOT_CONFIG_INIT_V7(
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_RESET_INTERFACE_FN	pResetInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize,
		BYTES					nLun,
		BOOL					fEnableDataInStuff,
		USBPUMP_OBJECT_HEADER *			pObjectHeader
		);

Description:
	This macro is used when initializing a single UPROTO_MSCBOT_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V8... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V7(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff,					\
		ARG_pObjectHeader					\
		)							\
	{								\
	(ARG_pSubClass),						\
	(ARG_pProcessCB),						\
	(ARG_pEnableInterface),						\
	(ARG_pDisableInterface),					\
	(ARG_pResetInterface),						\
	(ARG_pBusEvent),						\
	(ARG_pIfc),							\
	(ARG_nMaxTxDataSize),						\
	(ARG_nLun),							\
	(ARG_fEnableDataInStuff),					\
	(ARG_pObjectHeader)						\
	}

/*

Name:	UPROTO_MSCBOT_CONFIG_INIT_V6()

Function:
	Macro: compile time initialization for UPROTO_MSCBOT_CONFIG.

Definition:
	UPROTO_MSCBOT_CONFIG_INIT_V6(
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_RESET_INTERFACE_FN	pResetInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize,
		BYTES					nLun,
		BOOL					fEnableDataInStuff
		);

Description:
	This macro is used when initializing a single UPROTO_MSCBOT_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V7... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V6(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff					\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V7(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff,					\
		/* V7: pObjectHeader */ __TMS_NULL			\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_INIT_V5()

Function:
	Macro: compile time initialization for UPROTO_MSCBOT_CONFIG.

Definition:
	UPROTO_MSCBOT_CONFIG_INIT_V5(
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_RESET_INTERFACE_FN	pResetInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize,
		BYTES					nLun
		);

Description:
	This macro is used when initializing a single UPROTO_MSCBOT_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V6... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V5(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V6(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		/* fEnableDataInStuff */ __TMS_FALSE			\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_INIT_V4()

Function:
	Macro: compile time initialization for UPROTO_MSCBOT_CONFIG.

Definition:
	UPROTO_MSCBOT_CONFIG_INIT_V4(
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize,
		BYTES					nLun
		);

Description:
	This macro is used when initializing a single UPROTO_MSCBOT_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V5... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V4(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V5(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		(__TMS_PUPROTO_MSCBOT_RESET_INTERFACE_FN)__TMS_NULL,	\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_INIT_V3()

Function:
	Macro: compile time initialization for UPROTO_MSCBOT_CONFIG.

Definition:
	UPROTO_MSCBOT_CONFIG_INIT_V3(
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUPROTO_MSCBOT_BUS_EVENT_FN		pBusEvent,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize
		);

Description:
	This macro is used when initializing a single UPROTO_MSCBOT_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V4... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V3(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V4(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		1							\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_INIT_V2()

Function:
	Macro: compile time initialization for UPROTO_MSCBOT_CONFIG.

Definition:
	UPROTO_MSCBOT_CONFIG_INIT_V2(
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUPROTO_MSCBOT_DISABLE_INTERFACE_FN	pDisableInterface,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize
		);

Description:
	This macro is used when initializing a single UPROTO_MSCBOT_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V3... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V2(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V3(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		(__TMS_PUPROTO_MSCBOT_BUS_EVENT_FN)__TMS_NULL,		\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)

/*

Name:	UPROTO_MSCBOT_CONFIG_INIT_V1()

Function:
	Macro: compile time initialization for UPROTO_MSCBOT_CONFIG.

Definition:
	UPROTO_MSCBOT_CONFIG_INIT_V1(
		PUPROTO_MSCBOT_PROCESS_CB_FN		pProcessCB,
		PUPROTO_MSCBOT_ENABLE_INTERFACE_FN	pEnableInterface,
		PUINTERFACE				pIfc,
		BYTES					nMaxTxDataSize
		);

Description:
	This macro is used when initializing a single UPROTO_MSCBOT_CONFIG
	instance at compile time.  It expands to whatever is the
	current proper value for statically initializing such a structure,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V1(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)							\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V2(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		(__TMS_PUPROTO_MSCBOT_DISABLE_INTERFACE_FN)__TMS_NULL,	\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_MSCBOT_CONFIG_SETUP_V7(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff,					\
		ARG_pObjectHeader					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V7(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff,					\
		ARG_pObjectHeader					\
		)
# define UPROTO_MSCBOT_CONFIG_SETUP_V6(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V6(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff					\
		)
# define UPROTO_MSCBOT_CONFIG_SETUP_V5(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V5(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)
# define UPROTO_MSCBOT_CONFIG_SETUP_V4(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V4(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)
# define UPROTO_MSCBOT_CONFIG_SETUP_V3(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V3(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)
# define UPROTO_MSCBOT_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V2(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)
# define UPROTO_MSCBOT_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_SETUP_V1(				\
		pConfig,						\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)
# define UPROTO_MSCBOT_CONFIG_INIT_V7(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff,					\
		ARG_pObjectHeader					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V7(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff,					\
		ARG_pObjectHeader					\
		)
# define UPROTO_MSCBOT_CONFIG_INIT_V6(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V6(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun,						\
		ARG_fEnableDataInStuff					\
		)
# define UPROTO_MSCBOT_CONFIG_INIT_V5(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V5(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pResetInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)
# define UPROTO_MSCBOT_CONFIG_INIT_V4(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V4(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize,					\
		ARG_nLun						\
		)
# define UPROTO_MSCBOT_CONFIG_INIT_V3(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V3(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pBusEvent,						\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)
# define UPROTO_MSCBOT_CONFIG_INIT_V2(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V2(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pDisableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)
# define UPROTO_MSCBOT_CONFIG_INIT_V1(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)	\
	__TMS_UPROTO_MSCBOT_CONFIG_INIT_V1(				\
		ARG_pSubClass,						\
		ARG_pProcessCB,						\
		ARG_pEnableInterface,					\
		ARG_pIfc,						\
		ARG_nMaxTxDataSize					\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of mscbotcfg.h ****/
#endif /* _MSCBOTCFG_H_ */
