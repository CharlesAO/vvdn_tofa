/* vscapp_outcall.c	Fri Sep 05 2014 16:30:24 chwon */

/*

Module:  vscapp_outcall.c

Function:
	Home for gk_UsbPumpProtoVsc2_OutCall

Version:
	V3.13b	Fri Sep 05 2014 16:30:24 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2013-2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

	ChaeHee Won, MCCI Corporation	December 2013

Revision history:
   3.11d  Wed Dec 18 2013 11:16:34  chwon
	17949: Module created.

   3.13b  Fri Sep 05 2014 16:30:24  chwon
	18506: Fix diab compiler warning -- make static function.

*/

#include "usbpump_vsc2app.h"
#include "usbpump_proto_vsc2_api.h"
#include "usbpumpapi.h"
#include "usbpumpdebug.h"
#include "usbpumplib.h"
#include "uplatformapi.h"
#include "usbFlasher.h"

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/

static
USBPUMP_PROTO_VSC2_EVENT_FN
UsbPumpVscAppI_Event;

static
USBPUMP_PROTO_VSC2_SETUP_VALIDATE_FN
UsbPumpVscAppI_SetupValidate;

static
USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN
UsbPumpVscAppI_SetupProcess;

static
VOID
UsbPumpVscAppI_StartLoopback(
	USBPUMP_VSC2APP_CONTEXT *	pSelf
	);

static
UBUFIODONEFN
UsbPumpVscAppI_LoopbackReceiveDone;

static
UBUFIODONEFN
UsbPumpVscAppI_LoopbackTransferDone;

static
VOID
UsbPumpVscAppI_FreeRxQe(
	USBPUMP_VSC2APP_CONTEXT *	pSelf
	);


/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/

CONST USBPUMP_PROTO_VSC2_OUTCALL	gk_UsbPumpProtoVsc2_OutCall =
	USBPUMP_PROTO_VSC2_OUTCALL_INIT_V1(
		UsbPumpVscAppI_Event,
		UsbPumpVscAppI_SetupValidate,
		UsbPumpVscAppI_SetupProcess
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


/*

Name:	UsbPumpVscAppI_Event

Function:
	Deliver vsc protocol event to the registered client

Definition:
	VOID
	UsbPumpVscAppI_Event(
		VOID *				ClientHandle,
		USBPUMP_PROTO_VSC2_EVENT	Event,
		CONST VOID *			pEventInfo
		);

Description:
	This is vsc protocol driver out-call function.  It will be called
	by vsc protocol driver to deliver vsc protocol event to the
	registered client.

Returns:
	No explicit result.

*/

static
VOID
UsbPumpVscAppI_Event(
	VOID *				ClientHandle,
	USBPUMP_PROTO_VSC2_EVENT	Event,
	CONST VOID *			pEventInfo
	)
	{
	USBPUMP_VSC2APP_CONTEXT * CONST	pSelf = ClientHandle;

	USBPUMP_UNREFERENCED_PARAMETER(pEventInfo);

	switch (Event)
		{
	case USBPUMP_PROTO_VSC2_EVENT_INTERFACE_UP:
		{
		TTUSB_PLATFORM_PRINTF((
			pSelf->pPlatform,
			UDMASK_ANY,
			" UsbPumpVscAppI_Event: interface up.\n"
			));

		pSelf->fInterfaceUp = TRUE;

		UsbPumpVscAppI_StartLoopback(pSelf);
		}
		break;

	case USBPUMP_PROTO_VSC2_EVENT_INTERFACE_DOWN:
		{
		TTUSB_PLATFORM_PRINTF((
			pSelf->pPlatform,
			UDMASK_ANY,
			" UsbPumpVscAppI_Event: interface down.\n"
			));

		pSelf->fInterfaceUp = FALSE;
		}
		break;

	case USBPUMP_PROTO_VSC2_EVENT_RESUME:
	case USBPUMP_PROTO_VSC2_EVENT_SUSPEND:
		/* Need to notify client */
		break;

	default:
		break;
		}
	}

/*

Name:	UsbPumpVscAppI_SetupValidate

Function:
	Validate vsc control request to the registered client.

Definition:
	USBPUMP_PROTO_VSC2_SETUP_STATUS
	UsbPumpVscAppI_SetupValidate(
		VOID *		ClientHandle,
		CONST USETUP *	pSetup
		);

Description:
	This is vsc protocol driver out-call function.  It will be called by
	vsc protocol driver when receive vendor specific request from host.
	Client provided USBPUMP_PROTO_VSC2_SETUP_VALIDATE_FN() should validate
	vendor specific request.  If client can accept this control request,
	client should return USBPUMP_PROTO_VSC2_SETUP_STATUS_ACCEPTED.  If this
	control request is unknown (can't accept this control request),
	client should return USBPUMP_PROTO_VSC2_SETUP_STATUS_NOT_CLAIMED.
	If client knows this request but wants to reject this request, client
	should return USBPUMP_PROTO_VSC2_SETUP_STATUS_REJECTED.

	If client accepted this vendor specific control request, client will
	get USBPUMP_PROTO_VSC2_SETUP_PROCESS_FN() callback to process accepted
	control request.

	If client returns USBPUMP_PROTO_VSC2_SETUP_STATUS_REJECTED, vsc protocol
	will send STALL.

	If client returns USBPUMP_PROTO_VSC2_SETUP_STATUS_NOT_CLAIMED, protocol
	do nothing for this vendor specific command.

Returns:
	USBPUMP_PROTO_VSC2_SETUP_STATUS

*/

static
USBPUMP_PROTO_VSC2_SETUP_STATUS
UsbPumpVscAppI_SetupValidate(
	VOID *		ClientHandle,
	CONST USETUP *	pSetup
	)
	{
	USBPUMP_VSC2APP_CONTEXT * CONST	pSelf = ClientHandle;
	USBPUMP_PROTO_VSC2_SETUP_STATUS	Status;

	pSelf->fAcceptSetup = FALSE;
	Status = USBPUMP_PROTO_VSC2_SETUP_STATUS_NOT_CLAIMED;

	if (pSetup->uc_bmRequestType == USB_bmRequestType_HVDEV)
		{
		if (pSetup->uc_bRequest == 0)
			{
			pSelf->fAcceptSetup = TRUE;
			Status = USBPUMP_PROTO_VSC2_SETUP_STATUS_ACCEPTED;
			}
		else
			{
			Status = USBPUMP_PROTO_VSC2_SETUP_STATUS_REJECTED;
			}
		}
	else if (pSetup->uc_bmRequestType == USB_bmRequestType_DVDEV)
		{
		if (pSetup->uc_bRequest == 0)
			{
			pSelf->fAcceptSetup = TRUE;
			Status = USBPUMP_PROTO_VSC2_SETUP_STATUS_ACCEPTED;
			}
		else
			{
			Status = USBPUMP_PROTO_VSC2_SETUP_STATUS_REJECTED;
			}
		}

	return Status;
	}

/*

Name:	UsbPumpVscAppI_SetupProcess

Function:
	Process vsc control request.

Definition:
	BOOL
	UsbPumpVscAppI_SetupProcess(
		VOID *		ClientHandle,
		CONST USETUP *	pSetup,
		VOID *		pBuffer,
		UINT16		nBuffer
		);

Description:
	This is vsc protocol driver out-call function.  It will be called by
	vsc protocol driver to process vendor specific control request.

	If direction of vendor request is from host to device, vsc protocol
	receives data from host and protocol passes received data thru pBuffer
	and nBuffer.  Client processes control data in the pBuffer and client
	should send reply using USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN().

	If direction of vendor request is from device to host, vsc protocol
	provide data buffer (pBuffer and nBuffer).  Client processes control
	request and copies data to the pBuffer. Client should send reply using
	USBPUMP_PROTO_VSC2_CONTROL_REPLY_FN().

	If this process function returns FALSE, the VSC protocol driver will
	send STALL for this setup packet.  If it returns TRUE, client should
	handle this setup packet.

Returns:
	TRUE if client process this setup packet.  FALSE if client doesn't
	want to process this setup packet and want to send STALL.

*/

static
BOOL
UsbPumpVscAppI_SetupProcess(
	VOID *		ClientHandle,
	CONST USETUP *	pSetup,
	VOID *		pBuffer,
	UINT16		nBuffer
	)
	{
	USBPUMP_VSC2APP_CONTEXT * CONST	pSelf = ClientHandle;

	if (! pSelf->fAcceptSetup)
		return FALSE;

	pSelf->fAcceptSetup = FALSE;

	if (pSetup->uc_bmRequestType == USB_bmRequestType_HVDEV)
		{
		/* pSelf->pControlBuffer has received data from host. */
		TTUSB_PLATFORM_PRINTF((
			pSelf->pPlatform,
			UDMASK_ANY,
			" UsbPumpVscApp_Setup: Received %d bytes:"
			" %02x %02x %02x %02x ...\n",
			nBuffer,
			((UINT8 *) pBuffer)[0],
			((UINT8 *) pBuffer)[1],
			((UINT8 *) pBuffer)[2],
			((UINT8 *) pBuffer)[3]
			));

		set_flash_update_flag();

		/* Send control status data */
		(*pSelf->InCall.Vsc.pControlReplyFn)(
			pSelf->hSession,
			pBuffer,
			0
			);
		}
	else if (pSetup->uc_bmRequestType == USB_bmRequestType_DVDEV)
		{
		UINT16	Size;

		/* Send control data -- just copy SETUP packet */
		Size = sizeof(*pSetup);
		if (Size > nBuffer)
			Size = nBuffer;

		UHIL_cpybuf(pBuffer, pSetup, Size);

		(*pSelf->InCall.Vsc.pControlReplyFn)(
			pSelf->hSession,
			pBuffer,
			Size
			);
		}
	else
		{
		/* Send reply... here just STALL */
		(*pSelf->InCall.Vsc.pControlReplyFn)(
			pSelf->hSession,
			NULL,
			0
			);
		}

	return TRUE;
	}

/*

Name:	UsbPumpVscAppI_StartLoopback

Function:
	Start loopback

Definition:
	VOID
	UsbPumpVscAppI_StartLoopback(
		USBPUMP_VSC2APP_CONTEXT *	pSelf
		);

Description:
	This function starts data loopback operations.

Returns:
	No explicit result.

*/

static
VOID
UsbPumpVscAppI_StartLoopback(
	USBPUMP_VSC2APP_CONTEXT *	pSelf
	)
	{
	UBUFQE *	pQe;

	if (pSelf->hStreamIn == NULL || pSelf->hStreamOut == NULL)
		{
		TTUSB_PLATFORM_PRINTF((
			pSelf->pPlatform,
			UDMASK_ERRORS,
			"?UsbPumpVscAppI_Event:"
			" no IN(%p) or OUT(%p) Stream found\n",
			pSelf->hStreamIn,
			pSelf->hStreamOut
			));
		return;
		}

	while ((pQe = UsbGetQe(&pSelf->pFreeQeHeadOut)) != NULL)
		{
		USBPUMP_VSC2APP_REQUEST * CONST	pRequest =
			__TMS_CONTAINER_OF(
				pQe,
				USBPUMP_VSC2APP_REQUEST,
				Vsc.Qe.UbufqeLegacy
				);

		UsbPumpProtoVsc2Request_PrepareLegacy(
			&pRequest->Vsc,
			pSelf->hStreamOut,
			pRequest->pBuffer,
			NULL,	/* hBuffer */
			pRequest->nBuffer,
			UBUFQEFLAG_SHORTCOMPLETES,	/* TransferFlags */
			UsbPumpVscAppI_LoopbackReceiveDone,
			pSelf
			);

		(*pSelf->InCall.Vsc.pSubmitRequestFn)(
			pSelf->hSession,
			&pRequest->Vsc
			);
		}
	}

static
VOID
UsbPumpVscAppI_LoopbackReceiveDone(
	UDEVICE *	pDevice,
	UENDPOINT *	pUep,
	UBUFQE *	pQe
	)
	{
	USBPUMP_VSC2APP_REQUEST * CONST
		pRequest = __TMS_CONTAINER_OF(
				pQe,
				USBPUMP_VSC2APP_REQUEST,
				Vsc.Qe.UbufqeLegacy
				);
	USBPUMP_VSC2APP_CONTEXT * CONST	pSelf = pQe->uqe_doneinfo;
	UBUFQE *			pQeIn;

	USBPUMP_UNREFERENCED_PARAMETER(pDevice);
	USBPUMP_UNREFERENCED_PARAMETER(pUep);

	if (pQe->uqe_status != USTAT_OK)
		{
		TTUSB_PLATFORM_PRINTF((
			pSelf->pPlatform,
			UDMASK_ERRORS,
			"?UsbPumpVscAppI_LoopbackReceiveDone:"
			" pRequest(%p) error %s(%d)\n",
			pRequest,
			UsbPumpStatus_Name(pQe->uqe_status),
			pQe->uqe_status
			));

		if (pQe->uqe_status == USTAT_NOTCFG)
			{
			UsbPutQe(
				&pSelf->pFreeQeHeadOut,
				&pRequest->Vsc.Qe.UbufqeLegacy
				);
			UsbPumpVscAppI_FreeRxQe(pSelf);
			}
		else
			{
			/* try to receive data again */
			UsbPumpProtoVsc2Request_SetTransferFlags(
				&pRequest->Vsc,
				UBUFQEFLAG_SHORTCOMPLETES
				);
			(*pSelf->InCall.Vsc.pSubmitRequestFn)(
				pSelf->hSession,
				&pRequest->Vsc
				);
			}
		return;
		}

	/* copy received data to send buffer and send data */
	pQe->uqe_bufindex = 0;

	if (pSelf->pRxQeHead == NULL)
		{
		while ((pQeIn = UsbGetQe(&pSelf->pFreeQeHeadIn)) != NULL)
			{
			USBPUMP_VSC2APP_REQUEST * CONST
					pRequestIn = __TMS_CONTAINER_OF(
							pQeIn,
							USBPUMP_VSC2APP_REQUEST,
							Vsc.Qe.UbufqeLegacy
							);
			BYTES		Size;
			UBUFQE_FLAGS	Flags;

			if (pQe->uqe_bufars > pRequestIn->nBuffer)
				{
				Size = pRequestIn->nBuffer;
				Flags = 0;
				}
			else
				{
				Size = pQe->uqe_bufars;
				Flags = pRequest->Vsc.Qe.UbufqeLegacy.uqe_flags
				      & UBUFQEFLAG_POSTBREAK;
				}

			UHIL_cpybuf(
				pRequestIn->pBuffer,
				(UINT8 *) pQe->uqe_buf + pQe->uqe_bufindex,
				Size
				);

			UsbPumpProtoVsc2Request_PrepareLegacy(
				&pRequestIn->Vsc,
				pSelf->hStreamIn,
				pRequestIn->pBuffer,
				NULL,	/* hBuffer */
				Size,
				Flags,	/* TransferFlags */
				UsbPumpVscAppI_LoopbackTransferDone,
				pSelf
				);

			/* submit request to send loopback data */
			(*pSelf->InCall.Vsc.pSubmitRequestFn)(
				pSelf->hSession,
				&pRequestIn->Vsc
				);

			pQe->uqe_bufindex += Size;
			if ((pQe->uqe_bufars -= Size) == 0)
				{
				/* submit request to receive new data */
				UsbPumpProtoVsc2Request_SetTransferFlags(
					&pRequest->Vsc,
					UBUFQEFLAG_SHORTCOMPLETES
					);
				(*pSelf->InCall.Vsc.pSubmitRequestFn)(
					pSelf->hSession,
					&pRequest->Vsc
					);
				return;
				}
			}
		}

	UsbPutQe(&pSelf->pRxQeHead, &pRequest->Vsc.Qe.UbufqeLegacy);
	}

static
VOID
UsbPumpVscAppI_LoopbackTransferDone(
	UDEVICE *	pDevice,
	UENDPOINT *	pUep,
	UBUFQE *	pQe
	)
	{
	USBPUMP_VSC2APP_REQUEST * CONST
		pRequest = __TMS_CONTAINER_OF(
				pQe,
				USBPUMP_VSC2APP_REQUEST,
				Vsc.Qe.UbufqeLegacy
				);
	USBPUMP_VSC2APP_CONTEXT * CONST	pSelf = pQe->uqe_doneinfo;
	UBUFQE *			pQeOut;

	USBPUMP_UNREFERENCED_PARAMETER(pDevice);
	USBPUMP_UNREFERENCED_PARAMETER(pUep);

	if (pQe->uqe_status != USTAT_OK)
		{
		TTUSB_PLATFORM_PRINTF((
			pSelf->pPlatform,
			UDMASK_ERRORS,
			"?UsbPumpVscAppI_LoopbackTransferDone:"
			" pRequest(%p) error %s(%d)\n",
			pRequest,
			UsbPumpStatus_Name(pQe->uqe_status),
			pQe->uqe_status
			));
		}

	if (pQe->uqe_status == USTAT_NOTCFG)
		{
		UsbPutQe(&pSelf->pFreeQeHeadIn, &pRequest->Vsc.Qe.UbufqeLegacy);
		UsbPumpVscAppI_FreeRxQe(pSelf);
		return;
		}

	if ((pQeOut = pSelf->pRxQeHead) == NULL)
		{
		UsbPutQe(&pSelf->pFreeQeHeadIn, &pRequest->Vsc.Qe.UbufqeLegacy);
		}
	else
		{
		USBPUMP_VSC2APP_REQUEST * CONST
				pRequestOut = __TMS_CONTAINER_OF(
						pQeOut,
						USBPUMP_VSC2APP_REQUEST,
						Vsc.Qe.UbufqeLegacy
						);
		BYTES		Size;
		UBUFQE_FLAGS	QeFlags;

		if (pQeOut->uqe_bufars > pRequest->nBuffer)
			{
			Size = pRequest->nBuffer;
			QeFlags = 0;
			}
		else
			{
			Size = pQeOut->uqe_bufars;
			QeFlags = pRequestOut->Vsc.Qe.UbufqeLegacy.uqe_flags &
				  UBUFQEFLAG_POSTBREAK;
			}

		UHIL_cpybuf(
			pRequest->pBuffer,
			(UINT8 *) pQeOut->uqe_buf + pQeOut->uqe_bufindex,
			Size
			);

		UsbPumpProtoVsc2Request_PrepareLegacy(
			&pRequest->Vsc,
			pSelf->hStreamIn,
			pRequest->pBuffer,
			NULL,	/* hBuffer */
			Size,
			QeFlags,	/* TransferFlags */
			UsbPumpVscAppI_LoopbackTransferDone,
			pSelf
			);

		/* submit request to send loopback data */
		(*pSelf->InCall.Vsc.pSubmitRequestFn)(
			pSelf->hSession,
			&pRequest->Vsc
			);

		pQeOut->uqe_bufindex += Size;
		if ((pQeOut->uqe_bufars -= Size) == 0)
			{
			/* remove QE from the list */
			UsbGetQe(&pSelf->pRxQeHead);
			/* submit request to receive new data */
			UsbPumpProtoVsc2Request_SetTransferFlags(
				&pRequestOut->Vsc,
				UBUFQEFLAG_SHORTCOMPLETES
				);
			(*pSelf->InCall.Vsc.pSubmitRequestFn)(
				pSelf->hSession,
				&pRequestOut->Vsc
				);
			}
		}
	}

static
VOID
UsbPumpVscAppI_FreeRxQe(
	USBPUMP_VSC2APP_CONTEXT *	pSelf
	)
	{
	UBUFQE *	pQe;

	while ((pQe = UsbGetQe(&pSelf->pRxQeHead)) != NULL)
		{
		UsbPutQe(&pSelf->pFreeQeHeadOut, pQe);
		}
	}

/**** end of vscapp_outcall.c ****/
