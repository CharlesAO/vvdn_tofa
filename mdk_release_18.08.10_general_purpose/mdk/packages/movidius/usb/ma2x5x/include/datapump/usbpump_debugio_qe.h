/* usbpump_debugio_qe.h	Wed Dec 02 2009 12:08:42 skpark */

/*

Module:  usbpump_debugio_qe.h

Function:
	Define USBPUMP_DEBUGIO_SEND_QE, USBPUMP_DEBUGIO_RECV_QE 
	and associated functions

Version:
	V2.01a	Wed Dec 02 2009 12:08:42 skpark	Edit level 2

Copyright notice:
	This file copyright (C) 2009 by	

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Mats Webjörn, MCCI Corporation	April 2009

Revision history:
   2.01a  Tue Apr 21 2009 09:24:09 maw
	7563: Module created from usbpump_abstract_sio_qe.h EL 2

   2.01a  Wed Dec 02 2009 12:08:42  skpark
	9357: Fixed invalid token error while generating chm file

*/

#ifndef _USBPUMP_DEBUGIO_QE_H_	/* prevent multiple includes */
#define _USBPUMP_DEBUGIO_QE_H_

#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif


/****************************************************************************\
|
|	Forwards and common declarations
|
\****************************************************************************/
__TMS_TYPE_DEF_UNION(USBPUMP_DEBUGIO_SESSION);

__TMS_TYPE_DEF_STRUCT(USBPUMP_DEBUGIO_SEND_QE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_DEBUGIO_RECV_QE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_DEBUGIO_CONFIG);

__TMS_TYPE_DEF(USBPUMP_DEBUGIO_STATUS, __TMS_UINT);

/****************************************************************************\
|
|	USBPUMP_DEBUGIO_SEND_QE declarations
|
\****************************************************************************/

/*

Type:	USBPUMP_DEBUGIO_SEND_QE_CB_FN

Function:
	C function type for USBPUMP_DEBUGIO_SEND_QE
	completion functions.

Definition:
	typedef VOID USBPUMP_DEBUGIO_SEND_QE_CB_FN(
		PVOID				pCallBackContext,
		PUSBPUMP_DEBUGIO_SEND_QE	pSendQe,
		USBPUMP_DEBUGIO_STATUS		ErrorCode
		);

	typedef USBPUMP_DEBUGIO_SEND_QE_CB_FN 
		*PUSBPUMP_DEBUGIO_SEND_QE_CB_FN;

Description:
	This function is called to signal that processing of the send QE 
	is complete, and to return ownership of the Send QE and the 
	associated buffer to the client.
	
	The possible error conditions include:
		Invalid session handle
		Transport is not currently active.
		Invalid parameter

Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_SEND_QE_CB_FN,
__TMS_VOID,
	(
	__TMS_PVOID			pCallBackContext,
	__TMS_PUSBPUMP_DEBUGIO_SEND_QE	pSendQe,
	__TMS_USBPUMP_DEBUGIO_STATUS	ErrorCode
	));

/*

Type:	USBPUMP_DEBUGIO_SEND_QE

Function:
	C type for the USB DataPump Debug-IO Buffer Queue Element

Description:
	The client prepares and submits structures of this type in order 
	to send serial data.

Contents:
	|PUSBPUMP_DEBUGIO_SESSION pSession|
		Session handle. Client should not initialize, will be 
		set by API

	|PUSBPUMP_DEBUGIO_SEND_QE pNext|
		Pointer to next QE in list.This field is for use by the 
		Debug-IO. Client should not initialize. Contents 
		unspecified on completion.

	|PUSBPUMP_DEBUGIO_SEND_QE pLast|
		Pointer to previous QE in list. This field is for use by 
		the Debug-IO. Client should not initialize. Contents 
		unspecified on completion.

	|USBPUMP_DEBUGIO_SEND_QE_CB_FN *pCallBackFn|
		Function to be called on completion. Client should not 
		initialize, will be set by the API
		
	|PVOID pCallBackContext|
		Information for use by client. Client should not 
		initialize, will be set by the API.

	|PVOID pClientInfo|
		Information for use by client. 
		If used, client must initilalize, for example via 
		call to UsbPumpDebugIoSendQE_Init()

	|TEXT *pBuffer|
		Pointer to first byte of buffer.

	|BYTES sizeBuffer|
		Size of buffer, in bytes

	|BYTES nBuffer|
		Actual number of bytes in buffer

*/

struct __TMS_STRUCTNAME(USBPUMP_DEBUGIO_SEND_QE)
	{
	__TMS_BYTES				Size;
	__TMS_PUSBPUMP_DEBUGIO_SESSION		pSession;
	__TMS_PUSBPUMP_DEBUGIO_SEND_QE		pNext;
	__TMS_PUSBPUMP_DEBUGIO_SEND_QE		pLast;
	__TMS_USBPUMP_DEBUGIO_SEND_QE_CB_FN *	pCallBackFn;
	__TMS_PVOID				pCallBackContext;
	__TMS_PVOID				pClientInfo;
	__TMS_TEXT *				pBuffer;
	__TMS_BYTES				sizeBuffer;
	__TMS_BYTES				nBuffer;
	};

/*

Type:	USBPUMP_DEBUGIO_RECV_QE_CB_FN

Function:
	C function type for USBPUMP_DEBUGIO_RECV_QE
	completion functions.

Definition:
	typedef VOID USBPUMP_DEBUGIO_RECV_QE_CB_FN(
		PVOID pCallBackContext,
		PUSBPUMP_DEBUGIO_RECV_QE pReceiveQe,
		USBPUMP_DEBUGIO_STATUS ErrorCode
		);

	typedef USBPUMP_DEBUGIO_RECV_QE_CB_FN 
		*PUSBPUMP_DEBUGIO_RECV_QE_CB_FN;

Description:
	This function is called to signal that processing of the receive 
	QE is complete.  Depending on the status, zero or more bytes may
	be present in the buffer. 
	
	If Status is non-zero, pReceiveQe->nBytes will be zero, and 
	no bytes have been placed into the receive buffer. 
	
	If Status is zero, pReceiveQe->nBytes indicates how many bytes
	are present. 
	
	The possible error conditions include:
		Invalid session handle
		Transport is not currently active (or transport has become inactive).
		Invalid parameter

	Buffer and QE ownership is passed from the client to the Debug-IO
	by USBPUMP_DEBUGIO_RECV_QE_FN. Ownership is returned to 
	the client by USBPUMP_DEBUGIO_RECV_QE_CB_FN.
	
	For overlapped processing on the OUT pipes, the client 
	should keep multiple receive QEs pending in the DEBUGIO.

Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_RECV_QE_CB_FN,
__TMS_VOID,
	(
	__TMS_PVOID			pCallBackContext,
	__TMS_PUSBPUMP_DEBUGIO_RECV_QE	pReceiveQe,
	__TMS_USBPUMP_DEBUGIO_STATUS	ErrorCode
	));

/*

Type:	USBPUMP_DEBUGIO_RECV_QE

Function:
	C type for the USB DataPump Debug-IO Buffer Queue Element

Description:
	The client prepares and submits structures of this type in order 
	to receive data.  This element describes a receive buffer.  
	The required size of the receive buffer is determined by the transport.

Contents:
	|BYTES Size|
		Size of the structure, in bytes. 
		Client must initialize, for example via call to 
		UsbPumpDebugIoReceiveQE_Init().

	|PUSBPUMP_DEBUGIO_SESSION pSession|
		Session handle. Client should not initialize, will be 
		set by API.

	|PUSBPUMP_DEBUGIO_RECV_QE pNext|
		Pointer to next QE in list.This field is for use by the 
		Debug-IO. Client should not initialize. Contents 
		unspecified on completion

	|PUSBPUMP_DEBUGIO_RECV_QE pLast|
		Pointer to previous QE in list. This field is for use by 
		the Debug-IO. Client should not initialize. Contents 
		unspecified on completion

	|PUSBPUMP_DEBUGIO_RECV_QE_CB_FN pCallBackFn|
		Function to be called on completion. Client should not 
		initialize, filled in by API.
		
	|PVOID pCallBackContext|
		Information for use by client. Client should not 
		initialize, filled in by API.

	|PVOID pClientInfo|
		Information for use by client. Client should initialize 
		this field if used, for example by calling
		UsbPumpDebugIoReceiveQE_Init().

	|TEXT *pBuffer|
		Pointer to first byte of buffer.

	|BYTES sizeBuffer|
		Size of buffer, in bytes

	|BYTES nBuffer|
		Actual number of bytes received in buffer
		
*/

struct __TMS_STRUCTNAME(USBPUMP_DEBUGIO_RECV_QE)
	{
	__TMS_BYTES				Size;
	__TMS_PUSBPUMP_DEBUGIO_SESSION		pSession;
	__TMS_PUSBPUMP_DEBUGIO_RECV_QE		pNext;
	__TMS_PUSBPUMP_DEBUGIO_RECV_QE		pLast;
	__TMS_PUSBPUMP_DEBUGIO_RECV_QE_CB_FN	pCallBackFn;
	__TMS_PVOID				pCallBackContext;
	__TMS_PVOID				pClientInfo;
	__TMS_TEXT *				pBuffer;
	__TMS_BYTES				sizeBuffer;
	__TMS_BYTES				nBuffer;
	};

/**** end of usbpump_debugio_qe.h ****/
#endif /* _USBPUMP_DEBUGIO_QE_H_ */
