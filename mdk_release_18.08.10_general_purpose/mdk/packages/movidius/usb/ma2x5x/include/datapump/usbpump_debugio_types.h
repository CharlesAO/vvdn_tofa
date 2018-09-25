/* usbpump_debugio_types.h		Wed Dec 02 2009 12:08:35 skpark */

/*

Module:  usbpump_debugio_types.h

Function:
	Define types used in Debug-IO API

Version:
	V2.01a	Wed Dec 02 2009 12:08:35 skpark	Edit level 3

Copyright notice:
	This file copyright (C) 2009 by	

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Mats Webjorn, MCCI Corporation	April 2009

Revision history:
   2.01a  Tue Apr 21 2009 09:24:09 maw
	7563: Module created from usbpump_abstract_sio_types.h EL 3

   2.01a  Tue Dec 01 2009 12:45:59  chwon
	9357: Fixed doc-xml error in the USBPUMP_DEBUGIO_RECEIVE_QE_FN header

   2.01a  Wed Dec 02 2009 12:08:35  skpark
	9357: Fixed invalid token error while generating chm file

*/

#ifndef _USBPUMP_DEBUGIO_TYPES_H_	/* prevent multiple includes */
#define _USBPUMP_DEBUGIO_TYPES_H_

#ifndef _USBPUMP_DEBUGIO_QE_H_
# include "usbpump_debugio_qe.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION(USBPUMP_DEBUGIO);
__TMS_TYPE_DEF_STRUCT(USBPUMP_DEBUGIO_SESSION_APIS);

__TMS_TYPE_DEF(USBPUMP_DEBUGIO_EVENT, __TMS_UINT);
__TMS_TYPE_DEF(USBPUMP_DEBUGIO_RXDATAFLOW, __TMS_UINT);


/*

Type:	USBPUMP_DEBUGIO_OPEN_CB_FN

Function:
	C function type for open-completion callback.

Definition:
	typedef VOID USBPUMP_DEBUGIO_OPEN_CB_FN(
		PVOID pCallBackContext,
		PVOID pOpenRequestMemory,
		PUSBPUMP_DEBUGIO_SESSION pSession,
		USBPUMP_DEBUGIO_STATUS Status
		);

	typedef USBPUMP_DEBUGIO_OPEN_CB_FN 
		*PUSBPUMP_DEBUGIO_OPEN_CB_FN;

Description:
	This function is called to signal completion of the open processing, 
	and to return the necessary API handles. Status is zero for success, 
	and non-zero for failure.  The possible error conditions include:
	
	� Already open by another client
	� Some kind of plumbing problem
	� Invalid parameter of some kind
	
	The session handle pSession is a handle to be used in subsequent 
	session-based API operations.
	If successful, the API function tables from pApiBuffer parameter to 
	UsbPumpDebugIo_OpenSession() have been filled in with the method 
	functions to be used. 
	
Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_OPEN_CB_FN,
__TMS_VOID,
	(
	__TMS_PVOID 				pCallBackContext,
	__TMS_PVOID				pOpenRequestMemory,
	__TMS_PUSBPUMP_DEBUGIO_SESSION	pSession,
	__TMS_USBPUMP_DEBUGIO_STATUS 	Status
	));

/*

Type:	USBPUMP_DEBUGIO_CLOSE_CB_FN

Function:
	C function type for close-completion callback.

Definition:
	typedef VOID USBPUMP_DEBUGIO_CLOSE_CB_FN(
		PVOID pCallBackContext,
		USBPUMP_DEBUGIO_STATUS Status
		);

	typedef USBPUMP_DEBUGIO_CLOSE_CB_FN 
		*PUSBPUMP_DEBUGIO_CLOSE_CB_FN;

Description:
	This function is called to signal completion of the close processing.  
	Status is zero for success, and non-zero for failure.  The possible 
	error conditions include:
	
	� Invalid session handle

Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_CLOSE_CB_FN,
__TMS_VOID,
	(
	__TMS_PVOID 				pCallBackContext,
	__TMS_USBPUMP_DEBUGIO_STATUS 	Status
	));

/*

Type:	USBPUMP_DEBUGIO_CLOSE_FN

Function:
	C function type for close-completion callback.

Definition:
	typedef VOID USBPUMP_DEBUGIO_CLOSE_FN(
		PUSBPUMP_DEBUGIO_SESSION pSession,
		PUSBPUMP_DEBUGIO_CLOSE_CB_FN pCallBackFn,
		PVOID pCallBackContext
		);

	typedef USBPUMP_DEBUGIO_CLOSE_FN 
		*PUSBPUMP_DEBUGIO_CLOSE_FN;

Description:
	This function closes a session between the client and the 
	USBPUMP_DEBUGIO instance.  After the client issues this call, 
	the client may no longer use the session handle.  After this call 
	indicates completion, no further notifications or callbacks will be 
	made to the callback functions.
	
	During close processing, all pending operations will be cancelled 
	if necessary and the appropriate callbacks will be made for those 
	operations.  After all pending operations have completed (and their 
	respective callback functions have been called), the callback routine 
	will be called.
	
	The data pointer pCallBackContext is used when calling the close-complete 
	callback function.

Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_CLOSE_FN,
__TMS_VOID,
	(
	__TMS_PUSBPUMP_DEBUGIO_SESSION 	pSession,
	__TMS_PUSBPUMP_DEBUGIO_CLOSE_CB_FN pCallBackFn,
	__TMS_PVOID 				pCallBackContext
	));

/*

Type:	USBPUMP_DEBUGIO_SEND_QE_FN

Function:
	C function type to queue a buffer for sending data over a serial link.

Definition:
	typedef VOID USBPUMP_DEBUGIO_SEND_QE_FN(
		PUSBPUMP_DEBUGIO_SESSION pSession,
		PUSBPUMP_DEBUGIO_SEND_QE pSendQe,
		PUSBPUMP_DEBUGIO_SEND_QE_CB_FN pCallBackFn,
		PVOID pCallBackContext
		);

	typedef USBPUMP_DEBUGIO_SEND_QE_FN 
		*PUSBPUMP_DEBUGIO_SEND_QE_FN;

Description:
	This routine is used to queue a collection one or more Ethernet 
	frames for transmission over the network.  
	To call this routine, the client first prepares a 
	USBPUMP_DEBUGIO_SEND_QE element describing the frames to be 
	sent (using one USBPUMP_DEBUGIO_SEND_FRAME_INFO element per 
	frame), and then calls this routine.

	The data pointer pCallBackContext is used for the completion 
	function. The caller is required to provide a non-NULL the 
	callback function pointer, and appropriate completion context.  
	The PUSBPUMP_DEBUGIO_TX_CB_FN implementation stores this 
	in the appropriate fields of the USBPUMP_DEBUGIO_SEND_QE, 
	overwriting the previous values of pSendQe->pCallBackFn and 
	pSendQe->pCallBackContext. The callback function is called when 
	all bytes have been sent.

Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_SEND_QE_FN,
__TMS_VOID,
	(
	__TMS_PUSBPUMP_DEBUGIO_SESSION 		pSession,
	__TMS_PUSBPUMP_DEBUGIO_SEND_QE 		pSendQe,
	__TMS_PUSBPUMP_DEBUGIO_SEND_QE_CB_FN 	pCallBackFn,
	__TMS_PVOID 				pCallBackContext
	));

/*

Type:	USBPUMP_DEBUGIO_RECEIVE_QE_FN

Function:
	C function type for function used to queue a
	USBPUMP_DEBUGIO_RECEIVE_QE to receive serial data over a serial link.

Definition:
	typedef VOID USBPUMP_DEBUGIO_RECEIVE_QE_FN(
		PUSBPUMP_DEBUGIO_SESSION pSession,
		PUSBPUMP_DEBUGIO_RECEIVE_QE pReceiveQe,
		PUSBPUMP_DEBUGIO_RECEIVE_QE_CB_FN pCallBackFn,
		PVOID pCallBackContext
		);

	typedef USBPUMP_DEBUGIO_RECEIVE_QE_FN 
		*PUSBPUMP_DEBUGIO_RECEIVE_QE_FN;

Description:
	This routine is used to queue a buffer to be used to receive 
	Ethernet frames over the network.  To call this routine, the 
	client first prepares a USBPUMP_DEBUGIO_RECEIVE_QE 
	specifying the receive buffer, and then calls this routine.
	
Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_RECV_QE_FN,
__TMS_VOID,
	(
	__TMS_PUSBPUMP_DEBUGIO_SESSION 		pSession,
	__TMS_PUSBPUMP_DEBUGIO_RECV_QE 		pRecvQe,
	__TMS_PUSBPUMP_DEBUGIO_RECV_QE_CB_FN 	pCallBackFn,
	__TMS_PVOID 				pCallBackContext
	));

/*

Type:	USBPUMP_DEBUGIO_GET_RXDATAFLOW_CB_FN

Function:
	C function type for get-flowctrl-completion callback.

Definition:
	typedef VOID USBPUMP_DEBUGIO_GET_RXDATAFLOW_CB_FN(
		PVOID pCallBackContext,
		USBPUMP_DEBUGIO_RXDATAFLOW fEnable,
		USBPUMP_DEBUGIO_STATUS Status
		);

	typedef USBPUMP_DEBUGIO_GET_RXDATAFLOW_CB_FN 
		*PUSBPUMP_DEBUGIO_GET_RXDATAFLOW_CB_FN;

Description:
	This function is called to signal back the value of current 
	flowcontrol state.  Depending on the status, fEnable may or 
	may not be valid.  
	The possible error conditions include:
		Invalid session handle
		Transport is not currently active (or transport has become inactive).
		Invalid parameter

Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_GET_RXDATAFLOW_CB_FN,
__TMS_VOID,
	(
	__TMS_PVOID				pCallBackContext,
	__TMS_USBPUMP_DEBUGIO_RXDATAFLOW	fEnable,
	__TMS_USBPUMP_DEBUGIO_STATUS		Status
	));

/*

Type:	USBPUMP_DEBUGIO_GET_RXDATAFLOW_FN

Function:
	C function type for function used to get current 
	flow-control state on serial link

Definition:
	typedef VOID USBPUMP_DEBUGIO_GET_RXDATAFLOW_FN(
		PUSBPUMP_DEBUGIO_SESSION pSession,
		PUSBPUMP_DEBUGIO_GET_RXDATAFLOW_CB_FN pCallBackFn,
		PVOID pCallBackContext
		);

	typedef USBPUMP_DEBUGIO_GET_RXDATAFLOW_FN 
		*PUSBPUMP_DEBUGIO_GET_RXDATAFLOW_FN;

Description:
	This routine is used to reteive the current flow-control 
	state on serial link for selected SIO session.
	
Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_GET_RXDATAFLOW_FN,
__TMS_VOID,
	(
	__TMS_PUSBPUMP_DEBUGIO_SESSION			pSession,
	__TMS_PUSBPUMP_DEBUGIO_GET_RXDATAFLOW_CB_FN 	pCallBackFn,
	__TMS_PVOID					pCallBackContext
	));

/*

Type:	USBPUMP_DEBUGIO_SET_RXDATAFLOW_CB_FN

Function:
	C function type for set-flowctrl-completion callback.

Definition:
	typedef VOID USBPUMP_DEBUGIO_SET_RXDATAFLOW_CB_FN(
		PVOID pCallBackContext,
		USBPUMP_DEBUGIO_STATUS Status
		);

	typedef USBPUMP_DEBUGIO_SET_RXDATAFLOW_CB_FN 
		*PUSBPUMP_DEBUGIO_SET_RXDATAFLOW_CB_FN;

Description:
	This function is called to signal that processing of 
	the flow-control parameter is complete.  
	Depending on the status, fEnable may or may not have been set.  
	The possible error conditions include:
		Invalid session handle
		Transport is not currently active (or transport has become inactive).
		Invalid parameter

Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_SET_RXDATAFLOW_CB_FN,
__TMS_VOID,
	(
	__TMS_PVOID			pCallBackContext,
	__TMS_USBPUMP_DEBUGIO_STATUS	Status
	));

/*

Type:	USBPUMP_DEBUGIO_SET_RXDATAFLOW_FN

Function:
	C function type for function used to set current 
	flow-control state on serial link

Definition:
	typedef VOID USBPUMP_DEBUGIO_GET_RXDATAFLOW_FN(
		PUSBPUMP_DEBUGIO_SESSION pSession,
		USBPUMP_DEBUGIO_RXDATAFLOW fEnable,
		PUSBPUMP_DEBUGIO_GET_RXDATAFLOW_CB_FN pCallBackFn,
		PVOID pCallBackContext
		);

	typedef USBPUMP_DEBUGIO_GET_RXDATAFLOW_FN 
		*PUSBPUMP_DEBUGIO_GET_RXDATAFLOW_FN;

Description:
	This routine is used to set the flow-control state on serial 
	link for selected SIO session.
	
Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_SET_RXDATAFLOW_FN,
__TMS_VOID,
	(
	__TMS_PUSBPUMP_DEBUGIO_SESSION			pSession,
	__TMS_USBPUMP_DEBUGIO_RXDATAFLOW		fEnable,
	__TMS_PUSBPUMP_DEBUGIO_SET_RXDATAFLOW_CB_FN 	pCallBackFn,
	__TMS_PVOID					pCallBackContext
	));

/*

Type:	USBPUMP_DEBUGIO_SESSION_APIS

Function:
	C structure type; represents a collection of abstract API functions
	used by client to access the Debug-IO

Description:
	This structure is used by UsbPumpDebugIo_OpenSession to 
	return pointers to the API functions the client will use to 
	communicate with the SIO during the session.

Contents:
	|PUSBPUMP_DEBUGIO_CLOSE_FN pCloseApi;|
		Pointer to function to close a session between the client 
		and the USBPUMP_DEBUGIO instance.

	|PUSBPUMP_DEBUGIO_SEND_QE_FN pSendApi;|
		Pointer to function used to queue a buffer to
		send over the serial link.

	|PUSBPUMP_DEBUGIO_RECEIVE_QE_FN pReceiveApi;|
		Pointer to function used to queue a buffer to receive
		data over the seria link

	|PUSBPUMP_DEBUGIO_GET_RXDATAFLOW_FN pGetRxDataFlowApi;|
		Pointer to function used to get current flow-control state 
		on serial link.

	|PUSBPUMP_DEBUGIO_SET_RXDATAFLOW_FN pSetRxDataFlowApi;|
		Pointer to function used to set flow-control state 
		on serial link.

Notes:

See Also:
	USBPUMP_DEBUGIO_CLOSE_FN, USBPUMP_DEBUGIO_SEND_QE_FN,
	USBPUMP_DEBUGIO_RECV_QE_FN, USBPUMP_DEBUGIO_GET_RXDATAFLOW_FN
	and USBPUMP_DEBUGIO_SET_RXDATAFLOW_FN

*/

struct __TMS_STRUCTNAME(USBPUMP_DEBUGIO_SESSION_APIS)
	{ 
	__TMS_PUSBPUMP_DEBUGIO_CLOSE_FN			pCloseApi;
	__TMS_PUSBPUMP_DEBUGIO_SEND_QE_FN		pSendApi;
	__TMS_PUSBPUMP_DEBUGIO_RECV_QE_FN		pRecvApi;
	__TMS_PUSBPUMP_DEBUGIO_GET_RXDATAFLOW_FN	pGetRxDataFlowApi;
	__TMS_PUSBPUMP_DEBUGIO_SET_RXDATAFLOW_FN	pSetRxDataFlowApi;
	};

/*

Type:	USBPUMP_DEBUGIO_STATUS_CHANGE_CB_FN

Function:
	C function type for function that is called by the 
	transport layer to notify the client whenever
	status change events occur.

Definition:
	typedef VOID USBPUMP_DEBUGIO_STATUS_CHANGE_CB_FN(
   		PVOID pClientHandle,
   		USBPUMP_DEBUGIO_EVENT Event
		);

	typedef USBPUMP_DEBUGIO_STATUS_CHANGE_CB_FN 
		*PUSBPUMP_DEBUGIO_STATUS_CHANGE_CB_FN;

Description:
	Event indicates one of the following:
	
	For all implementations:
		Start of low-level session
		End of low-level session
	
	As a device:
		USB bus suspend without remote wakeup enable
		USB bus suspend with remote wakeup enable
		USB bus resume
	
	Note that beginning of session is signalled through a separate 
	callback.
	
Returns:
	Nothing

*/

__TMS_FNTYPE_DEF(
USBPUMP_DEBUGIO_STATUS_CHANGE_CB_FN,
__TMS_VOID,
	(
   	__TMS_PVOID			pClientHandle,
   	__TMS_USBPUMP_DEBUGIO_EVENT	Event
	));

/**** end of usbpump_debugio_types.h ****/
#endif /* _USBPUMP_DEBUGIO_TYPES_H_ */
