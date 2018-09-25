/* usbappinit.h	Sat Sep 27 2014 20:57:03 tmm */

/*

Module:  usbappinit.h

Function:
	General-purpose initialization discipline for use by USB DataPump
	applications.

Version:
	V3.13b	Sat Sep 27 2014 20:57:03 tmm	Edit level 8

Copyright notice:
	This file copyright (C) 2000-2003, 2007, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	September 2000

Revision history:
   1.60n  Wed Sep 27 2000 13:03:44  tmm
	Module created.

   1.60t  11/24/2000  tmm
	Bug 393:  fix prototype for UsbPump_GenericApplicationInit_Port to 
	get rid of SIZE_T.

   1.79a  6/5/2002  tmm
	Get rid of excess header files.

   1.83e  3/6/2003  chwon
	Change struct TTUSB_PLATFORM to __TMS_UPLATFORM.

   1.97j  Sat Nov 03 2007 05:28:20  tmm
	3445:  change type of USB_DATAPUMP_APPLICATION_INIT_VECTOR::
	pOptionalAppInfo from VOID* to CONST VOID*.

   3.13a  Tue Apr 08 2014 16:47:37  chwon
	18164: Remove UsbPortIndex, pDeviceInitFunction and sizeof_Udevice
	in the USB_DATAPUMP_APPLICATION_INIT_VECTOR.

   3.13a  Fri Apr 11 2014 14:42:58  chwon
	18162: Add UsbPump_GenericApplicationInit_Protocols_NoDataplane().

   3.13b  Sat Sep 27 2014 20:57:03  tmm
	18362: Split out types to usbappinit_types.h.

*/

#ifndef _USBAPPINIT_H_		/* prevent multiple includes */
#define _USBAPPINIT_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBAPPINIT_TYPES_H_
# include "usbappinit_types.h"
#endif

/****************************************************************************\
|
|	The data structures which must be built by the application.
|
\****************************************************************************/

/* __TMS_TYPE_DEF_STRUCT(USB_DATAPUMP_APPLICATION_INIT_VECTOR); */
/* __TMS_TYPE_DEF_STRUCT(USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR); */

/* __TMS_TYPE_DEF_STRUCT(USB_DATAPUMP_PORT_INIT_VECTOR); */
/* __TMS_TYPE_DEF_STRUCT(USB_DATAPUMP_PORT_INIT_VECTOR_HDR); */

struct __TMS_STRUCTNAME(USB_DATAPUMP_APPLICATION_INIT_VECTOR)
	{
	/*
	|| We can't do anything without a descriptor table and the
	|| init function.
	*/
	__TMS_CONST struct __USBRCTAB_ROOTTABLE *	pDescriptorTable;

	/*
	|| The recommended debug flags
	*/
	__TMS_UINT32					DebugFlags;

	/*
	|| The optional application probe function.
	*/
	__TMS_USB_DATAPUMP_APPLICATION_PROBE_FN	*	pAppProbeFunction;

	/*
	|| The application initialization function.
	*/
	__TMS_USB_DATAPUMP_APPLICATION_INIT_FN *	pAppInitFunction;

	/*
	|| A slot for extra information -- CONST VOID * so that
	|| CONST objects can easily be slotted here.
	*/
	__TMS_CONST __TMS_VOID *			pOptionalAppInfo;
	};

#define __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(	\
		pDescriptorTable,				\
		DebugFlags,					\
		pAppProbeFunction,				\
		pAppInitFunction,				\
		pOptionalAppInfo				\
		)						\
	{							\
	(pDescriptorTable),					\
	(DebugFlags),						\
	(pAppProbeFunction),					\
	(pAppInitFunction),					\
	(pOptionalAppInfo)					\
	}

#define __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V1(	\
		UsbPortIndex,					\
		pDescriptorTable,				\
		pDeviceInitFunction,				\
		sizeof_Udevice,					\
		DebugFlags,					\
		pAppProbeFunction,				\
		pAppInitFunction,				\
		pOptionalAppInfo				\
		)						\
	__TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(	\
		/* V1: UsbPortIndex, */				\
		pDescriptorTable,				\
		/* V1: pDeviceInitFunction, */			\
		/* V1: sizeof_Udevice, */			\
		DebugFlags,					\
		pAppProbeFunction,				\
		pAppInitFunction,				\
		pOptionalAppInfo				\
		)

/* the vector header */
struct __TMS_STRUCTNAME(USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR)
	{
	/* an optional function that is called before we start */
	__TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_SETUP_FN	*pSetup;

	/* an optional function that is called when we are done */
	__TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_FINISH_FN	*pFinish;

	/* the vector and its length */
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR	*pVector;
	__TMS_SIZE_T	nVector;
	};

#define __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR_INIT_V1(	\
		VectorName,					\
		pSetup,						\
		pFinish						\
		)						\
	{							\
	(pSetup),						\
	(pFinish),						\
	(VectorName),						\
	sizeof(VectorName) / sizeof(VectorName[0])		\
	}


/****************************************************************************\
|
|	The data structures which describe the device ports.
|
\****************************************************************************/

/****************************************************************************\
|
|	For each possible device port, there must be an entry in the device 
|	port init vector.
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USB_DATAPUMP_PORT_INIT_VECTOR)
	{
	/*
	|| The optional port probe function.
	*/
	__TMS_USB_DATAPUMP_PORT_INIT_VECTOR_PROBE_FN	*pPortProbeFunction;

	/*
	|| The application initialization function.
	*/
	__TMS_CONST __TMS_UDEVICESWITCH	*pDeviceSwitch;

	/*
	|| The bushandle 
	*/
	__TMS_UHIL_BUSHANDLE			hBus;

	/*
	|| The base I/O port
	*/
	__TMS_IOPORT				ioPort;

	/*
	|| The configuration structure
	*/
	__TMS_CONST __TMS_VOID			*pConfigInfo;
	__TMS_BYTES				sizeConfigInfo;
	};

#define __TMS_USB_DATAPUMP_PORT_INIT_VECTOR_INIT_V1( \
		pPortProbeFunction, \
		pDeviceSwitch, \
		hBus, \
		ioPort, \
		pConfigInfo, \
		sizeConfigInfo \
		) \
	{ \
	(pPortProbeFunction), \
	(pDeviceSwitch), \
	(hBus), \
	(ioPort), \
	(pConfigInfo), \
	(sizeConfigInfo) \
	}


/****************************************************************************\
|
|	A header structure is used so we only have to carry around
|	a single pointer.
|
\****************************************************************************/

/* the vector header */
struct __TMS_STRUCTNAME(USB_DATAPUMP_PORT_INIT_VECTOR_HDR)
	{
	/* an optional function that is called before we start */
	__TMS_USB_DATAPUMP_PORT_INIT_VECTOR_SETUP_FN	*pSetup;

	/* an optional function that is called when we are done */
	__TMS_USB_DATAPUMP_PORT_INIT_VECTOR_FINISH_FN	*pFinish;

	/* the vector and its length */
	__TMS_CONST __TMS_USB_DATAPUMP_PORT_INIT_VECTOR	*pVector;
	__TMS_BYTES	nVector;
	};

#define __TMS_USB_DATAPUMP_PORT_INIT_VECTOR_HDR_INIT_V1( \
		VectorName, \
		pSetup, \
		pFinish \
		) \
	{ \
	(pSetup), \
	(pFinish), \
	(VectorName), \
	sizeof(VectorName) / sizeof(VectorName[0]) \
	}


/****************************************************************************\
|
|	Functions...
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UINT
UsbPump_GenericApplicationInit(
	__TMS_UPLATFORM *pPlatform,
	__TMS_CONST __TMS_USB_DATAPUMP_PORT_INIT_VECTOR_HDR *pPortVec,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *pAppVec,
	__TMS_UDEVICE **pvDevices_out,
	__TMS_UINT length_vDevices_out,
	__TMS_UINT32 uDebugMask
	);

__TMS_VOID *
UsbPump_GenericApplicationInit_Setup(
	__TMS_UPLATFORM *,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *
	);

struct TTUSB_UDEVICE *
UsbPump_GenericApplicationInit_Port(
	__TMS_UINT UsbPortIndex,
	__TMS_UPLATFORM *,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *,
	__TMS_VOID * /*pAppInitContext*/,
	__TMS_CONST struct TTUSB_UDEVICESWITCH *,
	__TTUSB_HIL_BUSHANDLE,
	__TMS_IOPORT,
	__TMS_UINT32 /*DebugFlags*/,
	__TMS_CONST __TMS_VOID * /*pConfig*/,
	__TMS_BYTES /*nConfig*/
	);

__TMS_VOID
UsbPump_GenericApplicationInit_Finish(
	__TMS_UPLATFORM *,
	__TMS_CONST __TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR *,
	__TMS_VOID * /* pAppInitContext */,
	__TMS_UINT /* number of ports successfully initted */
	);

__TMS_USB_DATAPUMP_APPLICATION_INIT_FN 
UsbPump_GenericApplicationInit_Protocols;

__TMS_USB_DATAPUMP_APPLICATION_INIT_FN
UsbPump_GenericApplicationInit_Protocols_NoDataplane;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(	\
		pDescriptorTable,				\
		DebugFlags,					\
		pAppProbeFunction,				\
		pAppInitFunction,				\
		pOptionalAppInfo				\
		)	\
	__TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(	\
		pDescriptorTable,				\
		DebugFlags,					\
		pAppProbeFunction,				\
		pAppInitFunction,				\
		pOptionalAppInfo				\
		)
# define USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V1(	\
		UsbPortIndex,					\
		pDescriptorTable,				\
		pDeviceInitFunction,				\
		sizeof_Udevice,					\
		DebugFlags,					\
		pAppProbeFunction,				\
		pAppInitFunction,				\
		pOptionalAppInfo				\
		)	\
	__TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V1(	\
		UsbPortIndex,					\
		pDescriptorTable,				\
		pDeviceInitFunction,				\
		sizeof_Udevice,					\
		DebugFlags,					\
		pAppProbeFunction,				\
		pAppInitFunction,				\
		pOptionalAppInfo				\
		)
# define USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR_INIT_V1(	\
		VectorName,					\
		pSetup,						\
		pFinish						\
		)	\
	__TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_HDR_INIT_V1(	\
		VectorName,					\
		pSetup,						\
		pFinish						\
		)
# define USB_DATAPUMP_PORT_INIT_VECTOR_INIT_V1( \
		pPortProbeFunction, \
		pDeviceSwitch, \
		hBus, \
		ioPort, \
		pConfigInfo, \
		sizeConfigInfo \
		)	\
	__TMS_USB_DATAPUMP_PORT_INIT_VECTOR_INIT_V1( \
		pPortProbeFunction, \
		pDeviceSwitch, \
		hBus, \
		ioPort, \
		pConfigInfo, \
		sizeConfigInfo \
		)
# define USB_DATAPUMP_PORT_INIT_VECTOR_HDR_INIT_V1( \
		VectorName, \
		pSetup, \
		pFinish \
		)	\
	__TMS_USB_DATAPUMP_PORT_INIT_VECTOR_HDR_INIT_V1( \
		VectorName, \
		pSetup, \
		pFinish \
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbappinit.h ****/
#endif /* _USBAPPINIT_H_ */
