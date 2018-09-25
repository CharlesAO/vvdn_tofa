/* ndfudemo_appinit.c	Wed Dec 24 2014 12:49:05 chwon */

/*

Module:  ndfudemo_appinit.c

Function:
	Initialize the ndfudemo application

Version:
	V3.13b	Wed Dec 24 2014 12:49:05 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2004-2007, 2011, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

	Mats Webjorn, MCCI Corporation	June 2004
*/

/* the app init header file */
#include "usbappinit.h"
#include "usbpumpobjectapi.h"
#include "usbpumproot.h"

/* the NDFU things */
#include "protondfu.h"
#include "ndfucfg.h"

/* and the local declarations */
#include "ndfuapplib.h"

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not 
|	be exported.
|
\****************************************************************************/

#define EXTERN_USBRC_ROOTTABLE(NAME)	\
	extern CONST USBRC_ROOTTABLE NAME;

#ifdef DESCRIPTOR_ROOT_0
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_0)
#endif
#ifdef DESCRIPTOR_ROOT_1
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_1)
#endif
#ifdef DESCRIPTOR_ROOT_2
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_2)
#endif
#ifdef DESCRIPTOR_ROOT_3
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_3)
#endif
#ifdef DESCRIPTOR_ROOT_4
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_4)
#endif
#ifdef DESCRIPTOR_ROOT_5
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_5)
#endif
#ifdef DESCRIPTOR_ROOT_6
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_6)
#endif
#ifdef DESCRIPTOR_ROOT_7
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_7)
#endif
#ifdef DESCRIPTOR_ROOT_8
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_8)
#endif
#ifdef DESCRIPTOR_ROOT_9
	EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_9)
#endif
#ifndef	DESCRIPTOR_ROOT_0
# ifndef DESCRIPTOR_ROOT_1
#  ifndef DESCRIPTOR_ROOT_2
#   ifndef DESCRIPTOR_ROOT_3
#    ifndef DESCRIPTOR_ROOT_4
#     ifndef DESCRIPTOR_ROOT_5
#      ifndef DESCRIPTOR_ROOT_6
#       ifndef DESCRIPTOR_ROOT_7
#        ifndef DESCRIPTOR_ROOT_8
#         ifndef DESCRIPTOR_ROOT_9
	EXTERN_USBRC_ROOTTABLE(gk_UsbDescriptorRoot)
#         endif
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif


static USB_DATAPUMP_APPLICATION_INIT_VECTOR_FINISH_FN NDfuDemoI_AppInit_VectorFinish;

VOID NDfuDemo_ClientCreate(UPLATFORM *pPlatform);

/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only 
|	using the ROM storage class; they may be global.
|
\****************************************************************************/

/*
|| Declare a state-switch to include all DFU states. There are other macros
|| in protondfu.h that defines DFU-state subsets where unused states, i.e for
|| reading data from the device are eliminated.
*/
static USTATESWITCH_NDFU_DECLARE_ALLSTATES_V1(NDfuDemo);	/* See ndfucfg.h for macro */

/**** set up for loopback ****/
static CONST UPROTO_NDFU_CONFIG NDfuConfig =
	UPROTO_NDFU_CONFIG_INIT_V2(
		/* pStateSwitch */	&NDfuDemo_kAllStateSwitch,
		/* pDeviceMode */	&gk_NDfuDemo_DeviceMode,
		/* fUseDataPumpTimer */	FALSE,
		/* fNeedWarmStart */	FALSE,
		/* DebugFlags */	0
		);

/*
|| This table provides the initialization information for the protocols
|| that we might load for this device.
*/
static
CONST USBPUMP_PROTOCOL_INIT_NODE InitNodes[] =
	{
	USBPUMP_PROTOCOL_INIT_NODE_INIT_V1(			\
		/* dev class, subclass, proto */ -1, -1, -1,	\
		/* ifc class */ USB_bInterfaceClass_Dfu,	\
		/* subclass */ USB_bInterfaceSubClass_Dfu,	\
		/* proto */ -1,					\
		/* cfg, ifc, altset */ -1, -1, -1,		\
		/* speed */ -1,					\
		/* probe */ NDfu_ProtocolProbe,			\
		/* create */ NDfu_ProtocolCreate,		\
		/* optional info */ &NDfuConfig			\
		),
	};

static
CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR InitHeader =
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(
		/* name of the vector */ InitNodes,
		/* prefunction */ NULL,
		/* postfunction */ NULL
		);


#define	INIT_VECTOR_NODE(a__UsbDescriptorRoot)				\
	USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(			\
		/* pDescriptorTable */ &a__UsbDescriptorRoot,		\
		/* DebugFlags */					\
			UDMASK_ANY | UDMASK_ERRORS,			\
		/* pAppProbeFunction */ NULL,				\
		/* pAppInitFunction */					\
			UsbPump_GenericApplicationInit_Protocols,	\
		/* pAppInfo */ &InitHeader				\
		)

/*
|| This module exists solely to provide the UsbPumpApplicationInitHdr,
|| which provides the glue between the abstract datapump, the chip layer,
|| the descriptors, and the abstract applicaiton.
*/
static
CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR UsbPumpApplicationInitVector[] =
	{
#ifdef DESCRIPTOR_ROOT_0
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_0),
#endif
#ifdef DESCRIPTOR_ROOT_1
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_1),
#endif
#ifdef DESCRIPTOR_ROOT_2
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_2),
#endif
#ifdef DESCRIPTOR_ROOT_3
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_3),
#endif
#ifdef DESCRIPTOR_ROOT_4
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_4),
#endif
#ifdef DESCRIPTOR_ROOT_5
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_5),
#endif
#ifdef DESCRIPTOR_ROOT_6
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_6),
#endif
#ifdef DESCRIPTOR_ROOT_7
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_7),
#endif
#ifdef DESCRIPTOR_ROOT_8
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_8),
#endif
#ifdef DESCRIPTOR_ROOT_9
	INIT_VECTOR_NODE(DESCRIPTOR_ROOT_9),
#endif
#ifndef	DESCRIPTOR_ROOT_0
# ifndef DESCRIPTOR_ROOT_1
#  ifndef DESCRIPTOR_ROOT_2
#   ifndef DESCRIPTOR_ROOT_3
#    ifndef DESCRIPTOR_ROOT_4
#     ifndef DESCRIPTOR_ROOT_5
#      ifndef DESCRIPTOR_ROOT_6
#       ifndef DESCRIPTOR_ROOT_7
#        ifndef DESCRIPTOR_ROOT_8
#         ifndef DESCRIPTOR_ROOT_9
	INIT_VECTOR_NODE(gk_UsbDescriptorRoot)
#         endif
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif
	};

CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR gk_UsbPumpApplicationInitHdr =
	USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR_INIT_V1(
		UsbPumpApplicationInitVector,
		/* pSetup */ NULL, 
		/* pFinish */ NDfuDemoI_AppInit_VectorFinish
		);


/****************************************************************************\
|
|	VARIABLES:
|
|	If program is to be ROM-able, these must be initialized
|	using the BSS keyword.  (This allows for compilers that require
|	every variable to have an initializer.)  Note that only those 
|	variables owned by this module should be declared here, using the BSS
|	keyword; this allows for linkers that dislike multiple declarations
|	of objects.
|
\****************************************************************************/

/**** the only code in this module is a stub function  *****/
static VOID
NDfuDemoI_AppInit_VectorFinish(
	UPLATFORM *						pPlatform,
	CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *	pVecHdr,
	VOID *							pAppInitContext,
	UINT							nPorts
	)
	{
	USBPUMP_UNREFERENCED_PARAMETER(pVecHdr);
	USBPUMP_UNREFERENCED_PARAMETER(pAppInitContext);
	USBPUMP_UNREFERENCED_PARAMETER(nPorts);

	NDfuDemo_ClientCreate(pPlatform);
	}
