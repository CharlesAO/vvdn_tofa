/* ndfudemo_ram.c	Wed Dec 24 2014 13:12:30 chwon */

/*

Module:  ndfudemo_ram.c

Function:
	Demonstrate DFU 1.0 functions to a memory segment.

Version:
	V3.13b	Wed Dec 24 2014 13:12:30 chwon	Edit level 15

Copyright notice:
	This file copyright (C) 2004-2005, 2009-2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

	Mats Webjorn, MCCI Corporation	June 2004
*/

#define __TMS_CLOAKED_NAMES_ONLY	1

#include "udevice.h"
#include "uplatformapi.h"
#include "usbpumpapi.h"
#include "usbpumplib.h"
#include "ueventnode.h"
#include "usbpumpdebug.h"
#include "usbprotoinit.h"
#include "usbpumpobjectapi.h"
#include "usbioctlcodes.h"
#include "usbioctledgecodes.h"
#include "usbpumpioctlapi.h"
#include "ustat.h"
#include "usbdfu10.h"
#include "ufnapidownload.h"
#include "usbioctl_download.h"
#include "ndfuapplib.h"
#include "system.h"
#include "stdio.h"

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/

/* Number of bytes to run CRC on in each turn */
#define CRC_CHUNK_SIZE	(0x80000)
/* Size of transmitted data chunks */
#define TRANSFER_CHUNK_SIZE	(0x1000)

extern u8 NDfuDemo_FlashRegion[];

static __TMS_UEVENTFN
NDfuDemoI_RamLoad_DeviceEvent;

static __TMS_USBPUMP_GET_DEVICEMODE_FN
NDfuDemoI_RamLoad_DeviceMode;

static __TMS_USBPUMP_OBJECT_IOCTL_FN
NDfuDemoI_RamLoad_Ioctl;

/* Generic Ioctls handlers */
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Activate(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_ACTIVATE_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Deactivate(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_CONST __TMS_USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_BusEvent(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_CONST __TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG *
	);

/* DOWNLOAD specific handlers */
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_StartTmr(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_OpenWrite(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Write(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_CloseWrite(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_OpenRead(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Read(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_CloseRead(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Manifest(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Status(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Reset(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG *
	);

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Restart(
	__TMS_NDFUDEMO_RAMLOAD *,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG *
	);

/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/


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

/*
|| gk_NDfuDemo_DeviceMode holds the current device boot-mode, and should be intialized by the
|| boot routine, but in this demo we start up in APP-mode. Later we switch mode in the
|| device-event handler.
*/
__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE gk_NDfuDemo_DeviceMode =
/*	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_DFU; */
	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP;

/*
|| gk_NDfuDemoI_NextDeviceMode holds the requested device boot-mode, and should be read by the
|| boot routine. If this variable is MODE_DFU then the device should be forced into MODE_DFU,
|| but if it is MODE_APP, then other factors must also be taken into account such as validity
|| of APP-image before current mode is set.
*/
__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE gk_NDfuDemo_NextDeviceMode =
	__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_UNDEFINED;

/*
|| Restart-safe variables for keeping important state and status information over
|| a reset.
*/
__TMS_USBPUMP_DOWNLOAD_CLIENT_STATE gk_NDfuDemo_LastClientState;

__TMS_UINT8 gk_NDfuDemo_LastClientStatus;

static int bytesToSend = 0;


/*

Name:	NDfuDemo_CreateRamLoad()

Function:
	Create a DFU 1.0 download interface to ram area

Definition:
	BOOL NDfuDemo_CreateRamLoad(
		UPLATFORM *		pPlatform,
		USBPUMP_OBJECT_HEADER *	pFunctionObject,
		PNDFUDEMO_RAMLOAD *	ppRamLoad
		);

Description:
	Allocate instance data for the specified download object.

Returns:
	TRUE for success, FALSE for failure.

Notes:
	Some awkwardness arises because we need to allocate our
	instance data before we open the device.

	Also, note that there is one DFU app instance for each alternate
	setting * 2 for bus speed

*/

__TMS_BOOL
NDfuDemo_CreateRamLoad(
	__TMS_UPLATFORM *		pPlatform,
	__TMS_USBPUMP_OBJECT_HEADER *	pFunctionObject,
	__TMS_PNDFUDEMO_RAMLOAD *	ppRamLoad
	)
	{
	__TMS_UDEVICE * __TMS_CONST pDevice = UsbPumpObject_GetDevice(
					pFunctionObject
					);
	__TMS_NDFUDEMO_RAMLOAD * __TMS_CONST pRamLoad =
		UsbPumpPlatform_Malloc(
			pPlatform,
			sizeof(*pRamLoad)
			);
	__TMS_BOOL		StatusOK;

	__TMS_USBPUMP_UNREFERENCED_PARAMETER(ppRamLoad);

	StatusOK = (pRamLoad != __TMS_NULL);

	if (StatusOK)
		{
		UHIL_fill(pRamLoad, sizeof(*pRamLoad), 0);

		UsbPumpObject_Init(
			/* pSelf */		&pRamLoad->ObjectHeader,
			/* pClassHeader */	pFunctionObject,
			/* Tag */		__TMS_NDFUDEMO_TAG_RAMLOAD,
			/* Size */		sizeof(*pRamLoad),
			/* pName */		__TMS_NDFUDEMO_NAME_RAMLOAD,
			/* pIoctlParent */	&pPlatform->upf_Header,
			/* pIoctlFn */		NDfuDemoI_RamLoad_Ioctl
			);
		pRamLoad->ObjectHeader.ulDebugFlags = pFunctionObject->ulDebugFlags;

		/* fill in a few more things */
		pRamLoad->uram_pDevice   = pDevice;
		pRamLoad->uram_pPlatform = pPlatform;

		/* now, try to open the object (DFU function driver) */
		pRamLoad->uram_pDfuFnObject = UsbPumpObject_FunctionOpen(
			/* pFunctionObject */ pFunctionObject,
			/* pClientObject */  &pRamLoad->ObjectHeader,
			/* pClientContext */  pRamLoad,
			/* pIoctlResult */    __TMS_NULL
			);

		StatusOK = (pRamLoad->uram_pDfuFnObject != __TMS_NULL);
		}

	if (StatusOK)
		{
		__TMS_USBPUMP_IOCTL_RESULT IoctlResult;

		IoctlResult = UsbFnApiDownload_GetInfoV2(
				pRamLoad->uram_pDfuFnObject,
				&pRamLoad->uram_StringId,
				&pRamLoad->uram_bInterface,
				&pRamLoad->uram_bAlternateSetting
				);

		StatusOK = __TMS_USBPUMP_IOCTL_SUCCESS(IoctlResult);
		}

	if (StatusOK)
		{
		/*
		|| Get which memory region to use for this interface
		*/
		StatusOK = NDfuDemo_Id2Region(
				pDevice,
				pRamLoad->uram_StringId,
				pRamLoad->uram_bInterface,
				pRamLoad->uram_bAlternateSetting,
				&pRamLoad->uram_nBytesRegion,
				&pRamLoad->uram_pRegion
				);
		}

	if (StatusOK && pDevice->udev_bNumConfigs > 1)
		{
		/*
		|| Install DevMode ioctl function to override platform
		|| handler
		*/
		__TMS_USBPUMP_IOCTL_INIT_DEVMODEFN_ARG	Arg;
		__TMS_USBPUMP_IOCTL_RESULT		IoctlResult;

		__TMS_USBPUMP_IOCTL_INIT_DEVMODEFN_ARG_SETUP_V1(
			&Arg,
			NDfuDemoI_RamLoad_DeviceMode,
			pRamLoad
			);

		IoctlResult = UsbPlatformIoctl(
				pPlatform,
				__TMS_USBPUMP_IOCTL_INIT_DEVMODEFN,
				&Arg,
				__TMS_NULL
				);
		StatusOK = __TMS_USBPUMP_IOCTL_SUCCESS(IoctlResult);

		if (!StatusOK)
			{
			__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
				__TMS_UDMASK_ERRORS,
				"?NDfuDemo_CreateRamLoad:"
				" Could not register DeviceModeFn in platform\n"
				));
			}
		}

	if (StatusOK)
		{
		UsbAddEventNode(
			&pDevice->udev_noteq,
			&pRamLoad->uram_DeviceEventNode,
			NDfuDemoI_RamLoad_DeviceEvent,
			pRamLoad
			);

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ANY,
			" NDfuDemo_CreateRamload:"
			" opened %s#%d\n",
			pFunctionObject->pName,
			pFunctionObject->InstanceNumber
			));
		}

	if (!StatusOK)
		{
		/*
		|| Something went wrong above. Close open objects
		*/
		__TMS_TTUSB_OBJPRINTF((pFunctionObject,
			__TMS_UDMASK_ERRORS,
			"?NDfuDemo_CreateRamload:"
			" could not open %s#%d (%p)\n",
			pFunctionObject->pName,
			pFunctionObject->InstanceNumber,
			pFunctionObject
			));

		if (pRamLoad && pRamLoad->uram_pDfuFnObject)
			{
			(__TMS_VOID) UsbPumpObject_FunctionClose(
					pRamLoad->uram_pDfuFnObject,
					&pRamLoad->ObjectHeader,
					/* pIoctlResult */ __TMS_NULL
					);
			pRamLoad->uram_pDfuFnObject = __TMS_NULL;
			}

		if (pRamLoad)
			{
			/*
			|| DeInit and free the devobj
			*/
			UsbPumpObject_DeInit(
				&pRamLoad->ObjectHeader
				);

			UsbPumpPlatform_Free(
				pPlatform,
				pRamLoad,
				sizeof(*pRamLoad)
				);
			}
		}

	return StatusOK;
	}

/*

Name:	NDfuDemoI_RamLoad_DeviceEvent

Function:
	Device Event Handler.

Definition:
	UEVENTFN NDfuDemoI_RamLoad_DeviceEvent;

Description:
	This routine is attached to the device-event node. It is used to
	switch the DFU-device-mode, thereby simulating a mode-switch as
	if the entire device had restarted.

Returns:
	none.

*/

static
__TMS_VOID
NDfuDemoI_RamLoad_DeviceEvent(
	__TMS_PUDEVICE		pDevice,
	__TMS_PUEVENTNODE	pNode,
	__TMS_UEVENT		EventCode,
	__TMS_PVOID		pEventSpecificInfo
	)
	{
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pDevice);
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pNode);

	__TMS_TTUSB_OBJPRINTF((&pDevice->udev_Header,
		__TMS_UDMASK_ENTRY | __TMS_UDMASK_FLOW,
		"+NDfuDemoI_RamLoad_DeviceEvent(%p, %s)\n",
		(__TMS_VOID *)pNode->uev_ctx, UsbPumpEvent_Name(EventCode)
		));

	if (EventCode == __TMS_UEVENT_RESET)
		{
		__TMS_UEVENTRESET_INFO * __TMS_CONST pResetInfo = pEventSpecificInfo;

		/*
		|| New mode has been set on RESET leading-edge by Restart Ioctl.
		*/
		if ((pResetInfo != __TMS_NULL) && pResetInfo->fFinal)
			{
			__TMS_TTUSB_OBJPRINTF((&pDevice->udev_Header,
				__TMS_UDMASK_FLOW,
				" NDfuDemoI_RamLoad_DeviceEvent:"
				" Final-reset\n"
				));

			if (gk_NDfuDemo_NextDeviceMode !=
				__TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_UNDEFINED)
				{
				/*
				|| Ok, so someone wants us to change to a new mode.
				|| Let's do it, but make sure we don't make it twice
				*/

				/* XXX maw JUST FOR TEST
				||
				|| gk_NDfuDemo_DeviceMode     = __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP;
				*/
				gk_NDfuDemo_DeviceMode     = gk_NDfuDemo_NextDeviceMode;
				gk_NDfuDemo_NextDeviceMode = __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_UNDEFINED;

				__TMS_TTUSB_OBJPRINTF((&pDevice->udev_Header,
					__TMS_UDMASK_FLOW,
					" NDfuDemoI_RamLoad_DeviceEvent:"
					" Device-mode changed to %s\n",
					(gk_NDfuDemo_DeviceMode == __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP) ?
					"Application" : "DFU"
					));
				}
			}
		}

	__TMS_TTUSB_OBJPRINTF((&pDevice->udev_Header,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_DeviceEvent\n"
		));
	}

/*

Name:	NDfuDemoI_RamLoad_DeviceMode

Function:
	Platform calllback function for USBPUMP_IOCTL_GET_VIDPIDMODE_EX Ioctl

Definition:
	USBPUMP_GET_DEVICEMODE_FN NDfuDemoI_RamLoad_DeviceMode;

	VOID NDfuDemoI_RamLoad_DeviceMode(
		VOID *					pContext,
		USBPUMP_IOCTL_GET_VIDPIDMODE_ARG *	pVidPidMode
		);

Description:
	This routine is attached to the platform ioctl handler using
	USBPUMP_IOCTL_INIT_DEVMODEFN ioctl. It is then used to override
	the default result returned by the platform handler on the
	USBPUMP_IOCTL_GET_VIDPIDMODE_EX ioctl.
	It is used here to modify the PID and configuration depending on if
	the device is runing in application or DFU mode.

Returns:
	Nothing

Notes:
	None

*/

static
__TMS_VOID
NDfuDemoI_RamLoad_DeviceMode(
	__TMS_VOID *					pContext,
	__TMS_USBPUMP_IOCTL_GET_VIDPIDMODE_ARG *	pVidPidMode
	)
	{
	__TMS_TTUSB_TRACE(
		__TMS_NDFUDEMO_RAMLOAD * __TMS_CONST	pRamLoad = pContext;
		)

	__TMS_USBPUMP_TRACE_PARAMETER(pContext);

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_DeviceMode\n"
		));
/*
||	The following fields are not modified
||
||	pVidPidMode->idVendor;
||	pVidPidMode->bcdDevice;
*/
	/*
	|| This app assumes that the PID suppled by the urc-file is the one
	|| to be used for DFU mode. And application-mode will use PID+1
	||
	|| XXX maw: The configuration number is hardcoded here as I haven't
	||          yet figured out how to do this in a structured manner
	*/
	switch (gk_NDfuDemo_DeviceMode)
		{
	case __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_DFU:
		/*
		|| Reset device Class, SubClass and Protocol since
		|| device in DFU mode should enumerate on interface
		*/
		pVidPidMode->sDeviceClass    = 0;
		pVidPidMode->sDeviceSubClass = 0;
		pVidPidMode->sDeviceProtocol = 0;
		pVidPidMode->idProduct      += 0;
		pVidPidMode->ulMode          = 1;
		break;

	case __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP:
		/*
		|| If this device doesn't use the bus driver then clear
		|| device class, subclass and protocol
		*/
		pVidPidMode->sDeviceClass    = 0;
		pVidPidMode->sDeviceSubClass = 0;
		pVidPidMode->sDeviceProtocol = 0;

		pVidPidMode->idProduct += 1;
		pVidPidMode->ulMode     = 2;
		break;

	default:
		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
			"?NDfuDemoI_RamLoad_DeviceMode:"
			" Invalid device mode %d when enumerating device\n",
			gk_NDfuDemo_DeviceMode
			));
		break;
		}

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_DeviceMode\n"
		));
	}

/*

Name:	NDfuDemoI_RamLoad_Ioctl()

Function:
	Receives upcalls from the UPROTO_NDFU (or other
	compatible) lower layer, and dispatches them.

Definition:
	USBPUMP_OBJECT_IOCTL_FN NDfuDemoI_RamLoad_Ioctl;

	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_Ioctl(
		USBPUMP_OBJECT_HEADER *	pObject,
		USBPUMP_IOCTL_CODE 	Ioctl,
		CONST VOID *		pInParam,
		VOID *			pOutParam
		);

Description:
	When a low-level (USB-centric) event occurs, the low-level
	NDFU-subclass USB protocol layer prepares an appropriate IOCTL,
	which is then directed to the client who has opened the
	lower-level object.

	The following IOCTLs may be received:

	from "usbioctledgecodes.h":

	USBPUMP_IOCTL_EDGE_ACTIVATE
	USBPUMP_IOCTL_EDGE_DEACTIVATE
	USBPUMP_IOCTL_EDGE_BUS_EVENT

	From "usbioctl_download.h":

	USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR
	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE
	USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE
	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE
	USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ
	USBPUMP_IOCTL_EDGE_DOWNLOAD_READ
	USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ
	USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST
	USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS
	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET
	USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART

Returns:
	The normal USBIOCTL result.

Notes:
	Unlike other IOCTL implementation routines, we know that we
	actually are not likely to pass the operation upstream.  So we
	decode the IOCTL before doing the switch, to save time and code.
*/

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Ioctl(
	__TMS_USBPUMP_OBJECT_HEADER *	pObject,
	__TMS_USBPUMP_IOCTL_CODE	Ioctl,
	__TMS_CONST __TMS_VOID *	pInParam,
	__TMS_VOID *			pOutParam
	)
	{
	__TMS_NDFUDEMO_RAMLOAD * __TMS_CONST	pRamLoad =
		__TMS_CONTAINER_OF(
			pObject,
			__TMS_NDFUDEMO_RAMLOAD,
			ObjectHeader
			);

	switch (Ioctl)
		{
	/*
	|| Generic Ioctls here
	*/
	case __TMS_USBPUMP_IOCTL_EDGE_ACTIVATE:
		return NDfuDemoI_RamLoad_Activate(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DEACTIVATE:
		return NDfuDemoI_RamLoad_Deactivate(
			pRamLoad,
			pInParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT:
		return NDfuDemoI_RamLoad_BusEvent(
			pRamLoad,
			pInParam
			);

	/*
	|| STORAGE specific Ioctls here
	*/
	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR:
		return NDfuDemoI_RamLoad_StartTmr(
			pRamLoad,
			pOutParam
			);
	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE:
		return NDfuDemoI_RamLoad_OpenWrite(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE:
		return NDfuDemoI_RamLoad_Write(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE:
		return NDfuDemoI_RamLoad_CloseWrite(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ:
		return NDfuDemoI_RamLoad_OpenRead(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_READ:
		return NDfuDemoI_RamLoad_Read(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ:
		return NDfuDemoI_RamLoad_CloseRead(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST:
		return NDfuDemoI_RamLoad_Manifest(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS:
		return NDfuDemoI_RamLoad_Status(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET:
		return NDfuDemoI_RamLoad_Reset(
			pRamLoad,
			pOutParam
			);

	case __TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART:
		return NDfuDemoI_RamLoad_Restart(
			pRamLoad,
			pOutParam
			);

	default:
		return __TMS_USBPUMP_IOCTL_RESULT_NOT_CLAIMED;
		}
	}

/**********************************************************************************
||
|| Generic Ioctl handlers
||
**********************************************************************************/

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Activate(
	__TMS_NDFUDEMO_RAMLOAD *		pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_ACTIVATE_ARG *	pOutArg
	)
	{
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pOutArg);

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_Activate:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	/*
	|| Initialize state and status according to device mode from backup
	*/
	if (gk_NDfuDemo_DeviceMode == __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_UNDEFINED)
		{
		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
			"?NDfuDemoI_RamLoad_Activate:"
			" Invalid device mode\n"
			));
		}
	else
	if (gk_NDfuDemo_DeviceMode == __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP)
		{
		pRamLoad->uram_State  = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_0_APP_IDLE;
		pRamLoad->uram_Status = USB_Dfu_Status_OK;
		}
	else
	/* This must be __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_DFU */
	if (gk_NDfuDemo_LastClientState == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR)
		{
		pRamLoad->uram_State  = gk_NDfuDemo_LastClientState;
		pRamLoad->uram_Status = gk_NDfuDemo_LastClientStatus;

		/* Make sure we don't get here again by mistake */
		gk_NDfuDemo_LastClientState = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE;
		}
	else
		{
		pRamLoad->uram_State  = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE;
		pRamLoad->uram_Status = USB_Dfu_Status_OK;
		}

	/*
	|| Set fReject if you want to reject the activation,
	|| but don't count on it actually being rejected on the bus.
	*/

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_Activate\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Deactivate(
	__TMS_NDFUDEMO_RAMLOAD * 			  	pRamLoad,
	__TMS_CONST __TMS_USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG *	pInArg
	)
	{
	__TMS_USBPUMP_API_PARAMETER(pRamLoad);
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(pInArg);

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_Deactivate:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pInArg->pObject,
		pInArg->pClientContext
		));

	/*
	|| if you have any local operations to cancel based on
	|| deactivation (e.g., dropping any active calls) you
	|| should do so here.
	*/
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_Deactivate\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_BusEvent(
	__TMS_NDFUDEMO_RAMLOAD *				pRamLoad,
	__TMS_CONST __TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG *	pInArg
	)
	{
	__TMS_USBPUMP_API_PARAMETER(pRamLoad);

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader, 	\
		__TMS_UDMASK_ENTRY,				\
		"+NDfuDemoI_RamLoad_BusEvent:"			\
		" pRamLoad(%p) pObject(%p) pClientContext(%p)"	\
		" Event %d(%s) pInfo(%p)\n",			\
		pRamLoad,					\
		pInArg->pObject,				\
		pInArg->pClientContext,				\
		pInArg->EventCode,				\
		UsbPumpEvent_Name(pInArg->EventCode),		\
		pInArg->pEventSpecificInfo			\
		));

	/*
	|| most likely, you'll want to decode as follows:
	*/
	switch (pInArg->EventCode)
		{
	case __TMS_UEVENT_SUSPEND:
	case __TMS_UEVENT_L1_SLEEP:
	case __TMS_UEVENT_FUNCTION_SUSPEND:
		/* remember that we're now suspended */
		break;

	case __TMS_UEVENT_RESUME:
	case __TMS_UEVENT_FUNCTION_RESUME:
		/* remember that we're now active */
		break;

	case __TMS_UEVENT_ATTACH:
		/* we're attached to a USB host */
		break;

	case __TMS_UEVENT_DETACH:
		/* we're detached from the USB host */
		break;

	case __TMS_UEVENT_RESET:
		/*
		|| we saw a bus reset -- normally not much to be done
		|| here that won't be done by deactivate
		*/
		break;

	default:
		break;
		}

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_BusEvent\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/**********************************************************************************
||
|| Download specific Ioctl handlers
||
**********************************************************************************/

/*

Name:	NDfuDemoI_RamLoad_StartTmr()

Function:
	Called by the function layer to notify the download device when
	the function needs to start a timer.

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_StartTmr(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG *	pOutArg
		);

Description:
	This function is called when the function needs to start
	a timer. Currently it is only used to start a detach-timer in
	DFU-state-1.
	The client shall issue an USBPUMP_IOCTL_DOWNLOAD_TIMEOUT
	once the timer expires.

	It is not a requirement to implement this function, but it
	will increase the end-user experience if is implemented.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS

*/

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_StartTmr(
	__TMS_NDFUDEMO_RAMLOAD *				pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_START_TMR_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_StartTmr:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_FLOW,
		 " NDfuDemoI_RamLoad_StartTmr:"
		 " Timeout(%d ms) Id(%d)\n",
		 pOutArg->TmoInMs, pOutArg->Id
		 ));

	pRamLoad->uram_TmrId = pOutArg->Id;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_StartTmr\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_OpenWrite()

Function:
	Called by the function layer to notify the download device that
	the host is about to start downloading data to the target
	memory region

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_OpenWrite(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_STORAGE_READ_DONE_ARG *	pOutArg
		);

Description:
	The function will make this call as part of the transition between
	state 2 dfuIDLE and state 3 dfuDNLOAD-SYNC. The Client is expected
	to make appropriate actions in order to be prepared for receiving
	data to the device.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS

*/
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_OpenWrite(
	__TMS_NDFUDEMO_RAMLOAD *				pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_WRITE_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_OpenWrite:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE)
		{
		/*
		|| Do whatever is needed to prepare the hw for writing data to it
		*/
		pRamLoad->uram_pDstBuf = pRamLoad->uram_pRegion;
		/*
		|| Set new state and status
		*/
		pRamLoad->uram_State  = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_2_DFU_WRITE_IDLE;
		pRamLoad->uram_Status = USB_Dfu_Status_OK;
		}
	else
		{
		/*
		|| Function is trying to do an illegal state transition
		*/
		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
		 	"?NDfuDemoI_RamLoad_OpenWrite:"
		 	" Open write in state %d\n",
			pRamLoad->uram_State
		 	));
		}

	/*
	|| Return the number of milliseconds the client will need to complete
	|| the first EDGE_WRITE Ioctl
	*/
	pOutArg->NextTmoInMs = 5;

	/*
	|| Return client state on return from Ioctl
	*/
	pOutArg->ClientState = pRamLoad->uram_State;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_OpenWrite\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_Write()

Function:
	Called by the function layer to notify the download device
	that there is data to be written to the memory region


Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_Write(
		NDFUDEMO_RAMLOAD *			pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG *	pOutArg
		);


Description:
	The function layer will make this Ioctl as long as the host
	has data to download.
	The client MUST return from the Ioctl within the timeout indicated
	by the previous Ioctl (OpenWrite() or Write())

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS

*/
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Write(
	__TMS_NDFUDEMO_RAMLOAD *			pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_WRITE_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_Write:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_2_DFU_WRITE_IDLE)
		{
		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_FLOW,
			 " NDfuDemoI_RamLoad_Write:"
			 " 2_DFU_WRITE_IDLE "
			 " Write %d bytes from %p\n",
			 pOutArg->nBytes, pOutArg->pBuf
			 ));

		pRamLoad->uram_nBytes  = pOutArg->nBytes;
		pRamLoad->uram_pSrcBuf = pOutArg->pBuf;

		/*
		|| Set new state
		*/
		pRamLoad->uram_State = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_3_DFU_WRITE_BUSY;
		}

	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_3_DFU_WRITE_BUSY)
		{
		__TMS_USHORT	nBytes;

		/*
		|| Write TRANSFER_CHUNK_SIZE bytes per write-ioctl
		*/
		nBytes = (pRamLoad->uram_nBytes < TRANSFER_CHUNK_SIZE) ? pRamLoad->uram_nBytes : TRANSFER_CHUNK_SIZE;

		if (pRamLoad->uram_pDstBuf + nBytes >
		    pRamLoad->uram_pRegion + pRamLoad->uram_nBytesRegion)
			{
			pRamLoad->uram_State  = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;
			pRamLoad->uram_Status = USB_Dfu_Status_errADDRESS;

			pOutArg->fReject = __TMS_TRUE;

			__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
				__TMS_UDMASK_ERRORS,
		 		"?NDfuDemoI_RamLoad_Write:"
			 	" 3_DFU_WRITE_BUSY "
		 		" Attempt to write beyond end of buffer\n"
		 		));
			}
		else
			{
			__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
				__TMS_UDMASK_FLOW,
				 " NDfuDemoI_RamLoad_Write:"
				 " 3_DFU_WRITE_BUSY "
				 " Continue write %d bytes from %p\n",
				 nBytes, pRamLoad->uram_pSrcBuf
				 ));

			/* Do we already have this data in memory ? */
			if (UHIL_cmpbuf(pRamLoad->uram_pDstBuf, pRamLoad->uram_pSrcBuf, nBytes) == __TMS_FALSE)
				{
				/*
				|| This buffer contains new data. Store it in region
				*/
				UHIL_cpybuf(pRamLoad->uram_pDstBuf, pRamLoad->uram_pSrcBuf, nBytes);
				}

			/* Update buffer pointers and counter */
			pRamLoad->uram_pSrcBuf += nBytes;
			pRamLoad->uram_pDstBuf += nBytes;
			pRamLoad->uram_nBytes  -= nBytes;
			bytesToSend += nBytes;
			if (pRamLoad->uram_nBytes == 0)
				{
				/*
				|| Done writing. Set new state
				*/
				__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
					__TMS_UDMASK_FLOW,
					 " NDfuDemoI_RamLoad_Write:"
					 " 2_DFU_WRITE_IDLE\n"
					 ));

				pRamLoad->uram_State = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_2_DFU_WRITE_IDLE;
				}
			}
		}
	else
		{
		/*
		|| Function is trying to do an illegal state transition
		*/
		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
		 	"?NDfuDemoI_RamLoad_Write:"
		 	" Write in state %d\n",
			 pRamLoad->uram_State
		 	));
		}

	/*
	|| Return the number of milliseconds the client will need to complete
	|| the next EDGE_WRITE Ioctl
	*/
	pOutArg->NextTmoInMs = 5;

	/*
	|| Return client state on return from Ioctl
	*/
	pOutArg->ClientState = pRamLoad->uram_State;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_Write\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_CloseWrite()

Function:
	Called by the function layer to notify the download device
	that there is no more data to be written to the memory region

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_CloseWrite(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG *	pOutArg
		);

Description:
	The function layer will make this Ioctl when the host has no more data
	to download.
	The client is expected to make appropriate actions in order to be prepared
	for the manifestation phase of the download

Returns:
	None

*/

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_CloseWrite(
	__TMS_NDFUDEMO_RAMLOAD *				pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_WRITE_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_CloseWrite:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_2_DFU_WRITE_IDLE)
		{
		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_FLOW,
			" NDfuDemoI_RamLoad_CloseWrite:"
			" Prepare for manifestation\n"
		 	));

		/*
		|| Do whatever is needed to prepare for manifestation phase
		*/
		pRamLoad->uram_pSrcBuf = pRamLoad->uram_pRegion;
		pRamLoad->uram_nBytes  = CRC_CHUNK_SIZE; /* Number of bytes to run CRC on in each turn */
		pRamLoad->uram_CrcAcc  = __TMS_USBPUMP_DOWNLOAD_CRC_START;

		/*
		|| Set new state
		*/
		pRamLoad->uram_State = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_4_DFU_MANIFEST_BUSY;
		}
	else
	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_3_DFU_WRITE_BUSY)
		{
		pRamLoad->uram_State  = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;
		pRamLoad->uram_Status = USB_Dfu_Status_errWRITE;

		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
		 	"?NDfuDemoI_RamLoad_CloseWrite:"
		 	" Close-write in state %d and %d bytes remaining to be written\n",
			 pRamLoad->uram_State, pRamLoad->uram_nBytes
		 	));
		}
	else
		{
		/*
		|| Function is trying to do an illegal state transition
		*/
		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
		 	"?NDfuDemoI_RamLoad_CloseWrite:"
		 	" Close-write in state %d\n",
			pRamLoad->uram_State
		 	));
		}

	/*
	|| Return the number of milliseconds the client will need to complete
	|| the first EDGE_MANIFEST Ioctl
	*/
	pOutArg->NextTmoInMs = 5;

	/*
	|| Return client state on return from Ioctl
	*/
	pOutArg->ClientState = pRamLoad->uram_State;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_CloseWrite\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_OpenRead()

Function:
	Called by the function layer to notify the download device that
	the host is about to start uploading data from the target
	memory region

Definition:
	__TMS_USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_OpenRead(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG *	pOutArg
		);

Description:
	The function will make this call as part of the transition between
	state 2 dfuIDLE and state 9 dfuUPLOAD-IDLE. The client is expected
	to make appropriate actions in order to be prepared for transmitting
	data from the device.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS

Notes:

*/
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_OpenRead(
	__TMS_NDFUDEMO_RAMLOAD *				pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_OpenRead:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE)
		{
		/*
		|| Do whatever is needed to prepare the hw for reading data from it
		*/
		pRamLoad->uram_pSrcBuf = pRamLoad->uram_pRegion;
		pRamLoad->uram_nBytes  = pRamLoad->uram_nBytesRegion - 1;

		/*
		|| Set new state and status
		*/
		pRamLoad->uram_State  = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_5_DFU_READ_BUSY;
		pRamLoad->uram_Status = USB_Dfu_Status_OK;
		}
	else
		{
		/*
		|| Function is trying to do an illegal state transition
		*/
		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
		 	"?NDfuDemoI_RamLoad_OpenRead:"
		 	" Open write in state %d\n",
			pRamLoad->uram_State
		 	));
		}

	/*
	|| Return client state on return from Ioctl
	*/
	pOutArg->ClientState = pRamLoad->uram_State;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_OpenRead\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_Read()

Function:
	This function is called by the mass storage class
	framework to reset the block device.

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_Read(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_OPEN_READ_ARG *	pOutArg
		);

Description:
	This function is called by the mass storage class
	framework whenever a USB reset or a device reset
	command arrives from the host so that we can issue
	a reset command to the device.  This is usually
	caused by the device becoming un-responsive during
	use, to wake the device up initially, or to wake
	the device up after being suspended.

Returns:
	__TMS_USBPUMP_IOCTL_RESULT_SUCCESS

*/
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Read(
	__TMS_NDFUDEMO_RAMLOAD *			pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_READ_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_Read:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_5_DFU_READ_BUSY)
		{
		__TMS_USHORT	nBytes;

		/*
		|| Read out data from device. A "short" frame (less data than
		|| nMaxBytes) will terminate the read and make the function
		|| issue the Close-Read Ioctl
		*/
		nBytes = (pOutArg->nMaxBytes < pRamLoad->uram_nBytes) ?
		          pOutArg->nMaxBytes : pRamLoad->uram_nBytes;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_FLOW,
			" NDfuDemoI_RamLoad_Read:"
			" Read %d bytes starting at %p\n",
			nBytes, pRamLoad->uram_pSrcBuf
			));

		if (nBytes < pOutArg->nMaxBytes)
			__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
				__TMS_UDMASK_FLOW,
				 " NDfuDemoI_RamLoad_Read:"
				 " Read done\n"
				 ));

		UHIL_cpybuf(pOutArg->pBuf, pRamLoad->uram_pSrcBuf, nBytes);
		pRamLoad->uram_pSrcBuf += nBytes;
		pRamLoad->uram_nBytes  -= nBytes;

		pOutArg->nBytes = nBytes;
		}
	else
		{
		/*
		|| Function is trying to do an illegal state transition
		*/
		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
		 	"?NDfuDemoI_RamLoad_Read:"
		 	" Read in state %d\n",
			pRamLoad->uram_State
		 	));
		}

	/*
	|| Return client state on return from Ioctl
	*/
	pOutArg->ClientState = pRamLoad->uram_State;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_Read\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_CloseRead()

Function:
	Called by the function layer to notify the download device
	that the read is terminated.

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_CloseRead(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG *	pOutArg
		);

Description:
	The function will make this Ioctl when the device has had less than
	requested amount of data to upload. The client is expected to make
	appropriate actions to clean up the read operation in order to be
	prepared for a new operation.

Returns:
	__TMS_USBPUMP_IOCTL_RESULT_SUCCESS

*/
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_CloseRead(
	__TMS_NDFUDEMO_RAMLOAD *				pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_CloseRead:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_5_DFU_READ_BUSY)
		{
		/*
		|| Do whatever is needed to clean up the read
		*/

		/*
		|| Set new state
		*/
		pRamLoad->uram_State = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE;
		}
	else
		{
		/*
		|| Function is trying to do an illegal state transition
		*/
		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
		 	"?NDfuDemoI_RamLoad_CloseRead:"
		 	" Close-read in state %d\n",
			pRamLoad->uram_State
		 	));
		}

	/*
	|| Return client state on return from Ioctl
	*/
	pOutArg->ClientState = pRamLoad->uram_State;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_CloseRead\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_Manifest()

Function:
	Called by the function layer to notify the download device
	that the read is terminated.

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_Manifest(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG *	pOutArg
		);

Description:
	The function will make this Ioctl when the device has had less than
	requested amount of data to upload. The client is expected to make
	appropriate actions to clean up the read operation in order to be
	prepared for a new operation.

Returns:
	__TMS_USBPUMP_IOCTL_RESULT_SUCCESS

*/

static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Manifest(
	__TMS_NDFUDEMO_RAMLOAD *				pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_MANIFEST_ARG *	pOutArg
	)
	{
	__TMS_USBPUMP_IOCTL_RESULT		IoctlResult;

	IoctlResult = __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	uint32_t floating_atribute = RTEMS_LOCAL | RTEMS_FLOATING_POINT;
	rtems_name task_name;
	rtems_id dfu_task_id;
	rtems_status_code status;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_Manifest:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	if (pRamLoad->uram_State == __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_4_DFU_MANIFEST_BUSY)
		{
		__TMS_BYTES	nBytes;

		/*
		|| Do whatever is needed to manifest the downloaded image.
		|| This is where you for instance could verify it has a correct CRC.
		*/
		nBytes = (pRamLoad->uram_pSrcBuf + pRamLoad->uram_nBytes <
			  pRamLoad->uram_pRegion + pRamLoad->uram_nBytesRegion) ?
			  pRamLoad->uram_nBytes :
			  (__TMS_BYTES)(pRamLoad->uram_pRegion + pRamLoad->uram_nBytesRegion-pRamLoad->uram_pSrcBuf);

		IoctlResult = UsbFnApiDownload_CalcCrc(
			pRamLoad->uram_pDfuFnObject,
			pRamLoad->uram_pSrcBuf,
			nBytes,
			&pRamLoad->uram_CrcAcc
			);

		if (__TMS_USBPUMP_IOCTL_SUCCESS(IoctlResult))
			{
			pRamLoad->uram_pSrcBuf += nBytes;

			/*
			|| Return the number of milliseconds the client will need to complete
			|| the next EDGE_MANIFEST Ioctl
			*/
			pOutArg->NextTmoInMs = 5;

			if (pRamLoad->uram_pSrcBuf ==
			    pRamLoad->uram_pRegion + pRamLoad->uram_nBytesRegion)
				{
				__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
					__TMS_UDMASK_FLOW,
					" NDfuDemoI_RamLoad_Manifest:"
					" Manifest image done\n"
		 			));

				flash_params.size = bytesToSend;
				flash_params.imgId = 0;
				flash_params.writeImg = 1;
				flash_params.inBuff = &NDfuDemo_FlashRegion;
				flash_params.outBuff = &NDfuDemo_FlashRegion;

				// Create and start the flash update task
				task_name = rtems_build_name('F', 'T', 'A', '0');

				status = rtems_task_create(task_name, 2, RTEMS_MINIMUM_STACK_SIZE,
											RTEMS_INTERRUPT_LEVEL(0),
											floating_atribute,
											&dfu_task_id);

				if(status != RTEMS_SUCCESSFUL) {
					printf("rtems_task_create failed with %d\n", status);
				}

				rtems_task_start(dfu_task_id, flash_app_task, (rtems_task_argument)&flash_params);

				/*
				|| Manifestation done. Go to Idle
				*/
				pRamLoad->uram_State = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE;
				}
			}
		else
			{
			pRamLoad->uram_State  = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR;
			pRamLoad->uram_Status = USB_Dfu_Status_errVERIFY;

			pOutArg->fReject = __TMS_TRUE;

			__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
				__TMS_UDMASK_ERRORS,
			 	"?NDfuDemoI_RamLoad_Manifest:"
			 	" CRC calc failure\n"
			 	));
			}
		}
	else
		{
		/*
		|| Function is trying to do an illegal state transition
		*/
		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
		 	"?NDfuDemoI_RamLoad_Manifest:"
		 	" Close-read in state %d\n",
			pRamLoad->uram_State
		 	));
		}

	/*
	|| Return client state on return from Ioctl
	*/
	pOutArg->ClientState = pRamLoad->uram_State;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_Manifest\n"
		));

	return IoctlResult;
	}

/*

Name:	NDfuDemoI_RamLoad_Status()

Function:
	Called by the function layer to find out the client's internal state
	and status.

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_Status(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG *	pOutArg
		);

Description:

Returns:
	__TMS_USBPUMP_IOCTL_RESULT_SUCCESS

*/
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Status(
	__TMS_NDFUDEMO_RAMLOAD *			pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_STATUS_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_Status:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	/*
	|| Return client state, status and string index
	*/
	pOutArg->ClientState  = pRamLoad->uram_State;
	pOutArg->ClientStatus = pRamLoad->uram_Status;
	pOutArg->iString      = 0;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_Status\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_Reset()

Function:
	Called by the function layer to to reset the client to
	a known state.

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_Reset(
		NDFUDEMO_RAMLOAD *			pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG *	pOutArg
		);

Description:
	The function will make this Ioctl when it needs to reset the
	client to a known state.
	This will depend whether the device-mode the client is in. For
	USBPUMP_DOWNLOAD_DEVICE_MODE_APP -> USBPUMP_DOWNLOAD_CLIENT_STATE_0_APP_IDLE
	USBPUMP_DOWNLOAD_DEVICE_MODE_DFU -> USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE

Returns:
	__TMS_USBPUMP_IOCTL_RESULT_SUCCESS

*/
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Reset(
	__TMS_NDFUDEMO_RAMLOAD *			pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESET_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_Reset:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	switch (gk_NDfuDemo_DeviceMode)
		{
	case __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP :
		pRamLoad->uram_State = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_0_APP_IDLE;
		break;

	case __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_DFU :
		pRamLoad->uram_State = __TMS_USBPUMP_DOWNLOAD_CLIENT_STATE_1_DFU_IDLE;
		break;

	default :
		pOutArg->fReject = __TMS_TRUE;

		__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
			__TMS_UDMASK_ERRORS,
	 		"?NDfuDemoI_RamLoad_Reset:"
	 		" Unknown device boot-mode %d\n",
			gk_NDfuDemo_DeviceMode
	 		));
		}

	/*
	|| Return client state on return from Ioctl
	*/
	pOutArg->ClientState = pRamLoad->uram_State;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_Reset\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}

/*

Name:	NDfuDemoI_RamLoad_Restart()

Function:
	Called by the function layer to notify the download device
	that the read is terminated.

Definition:
	USBPUMP_IOCTL_RESULT NDfuDemoI_RamLoad_Restart(
		NDFUDEMO_RAMLOAD *				pRamLoad,
		USBPUMP_IOCTL_EDGE_DOWNLOAD_CLOSE_READ_ARG *	pOutArg
		);

Description:
	The function will make this Ioctl when the device has had less than
	requested amount of data to upload. The client is expected to make
	appropriate actions to clean up the read operation in order to be
	prepared for a new operation.

	This Ioctl will occur on the leading edge of USB-RESET

Returns:
	__TMS_USBPUMP_IOCTL_RESULT_SUCCESS

*/
static __TMS_USBPUMP_IOCTL_RESULT
NDfuDemoI_RamLoad_Restart(
	__TMS_NDFUDEMO_RAMLOAD *			pRamLoad,
	__TMS_USBPUMP_IOCTL_EDGE_DOWNLOAD_RESTART_ARG *	pOutArg
	)
	{
	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"+NDfuDemoI_RamLoad_Restart:"
		" pRamLoad(%p) pObject(%p) pClientContext(%p)\n",
		pRamLoad,
		pOutArg->pObject,
		pOutArg->pClientContext
		));

	/*
	|| Store away important state and status information somewhere
	|| so that it survives a reset.
	|| If the device wakes up in APP mode after reset, then the state
	|| and status is ignored.
	|| But if the device wakes up in DFU mode after reset, and the state
	|| is USBPUMP_DOWNLOAD_CLIENT_STATE_6_DFU_ERROR it will be retained
	|| so that the host can retrieve the status code for what went wrong
	|| before the reset.
	*/
	gk_NDfuDemo_NextDeviceMode   = pOutArg->DeviceMode;
	gk_NDfuDemo_LastClientState  = pRamLoad->uram_State;
	gk_NDfuDemo_LastClientStatus = pRamLoad->uram_Status;

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_FLOW,
		" NDfuDemoI_RamLoad_Restart:"
		" Request to change Device-mode to %s\n",
		(gk_NDfuDemo_NextDeviceMode == __TMS_USBPUMP_DOWNLOAD_DEVICE_MODE_APP) ?
		"Application" : "DFU"
		));

	__TMS_TTUSB_OBJPRINTF((&pRamLoad->ObjectHeader,
		__TMS_UDMASK_ENTRY,
		"-NDfuDemoI_RamLoad_Restart\n"
		));

	return __TMS_USBPUMP_IOCTL_RESULT_SUCCESS;
	}
