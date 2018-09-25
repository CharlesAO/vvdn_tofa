/* protowmc.h	Mon Jan 16 2012 15:12:26 chwon */

/*

Module:  protowmc.h

Function:
	Provides external declarations for accessing WMC implementation from
	upper levels.

Version:
	V3.01f	Mon Jan 16 2012 15:12:26 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2002-2006, 2009, 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	March 2002

Revision history:
   1.79a  Fri Mar  8 2002 11:17:06  tmm
	Module created.

   1.87b  Wed Mar 17 2004 09:31:19  chwon
	Get rid of ARM SDT compiler warning ">= 10 lines of macros".

   1.87b  Wed Apr 28 2004 08:28:15 maw
   	Added uwmc_started to UPROTO_WMCTA and
   	UPROTO_WMCTA_DATA_PLANES_ENABLED macro

   1.97f  Mon Sep 11 2006 13:41:48  chwon
	Add WmcTA_AddDataInterface prototype. Complete type cloaking

   2.01a  Wed Mar 18 2009 12:23:27  chwon
	2829: Use USBPUMP_OBJECT_NAME_FN() to define UPROTO_WMCSUBCLASS_NAME()

   3.01f  Mon Jan 16 2012 15:12:26  chwon
	14623: Update document -- no code change.

*/

#ifndef _PROTOWMC_H_		/* prevent multiple includes */
#define _PROTOWMC_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _UDATASTREAM_H_
# include "udatastream.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _UBUFQE_H_
# include "ubufqe.h"
#endif

#ifndef _USBRINGBUFFER_H_
# include "usbringbuffer.h"
#endif

#ifndef _USBPROTOINIT_H_
# include "usbprotoinit.h"
#endif

#ifndef _USBPUMPOBJECTNAME_H_
# include "usbpumpobjectname.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UPROTO_WMCTA);
__TMS_TYPE_DEF_STRUCT(UPROTO_WMCTA_CONFIG);
__TMS_TYPE_DEF_STRUCT(UPROTO_WMCTA_DOWNCALLS);

__TMS_TYPE_DEF_STRUCT(UPROTO_WMCSUBCLASS);
__TMS_TYPE_DEF_STRUCT(UPROTO_WMCSUBCLASS_UPCALLS);

/****************************************************************************\
|
|	Parameters
|
\****************************************************************************/

/*
|| We centralize processing for SEND_ENCAPSULATED/GET_ENCAPSULATED,
|| and we need a default size for the input and output queues.
*/
#define	__TMS_UPROTO_WMCTA_ENCAPSQUEUE_SIZE	4096

/*
|| This is not a parameter, but it is a symbolic constant.  How big is
|| a "response-available" message?
*/
#define __TMS_UPROTO_WMCTA_RESPONSE_AVAIL_SIZE	8

/****************************************************************************\
|
|	The UPROTO_WMCTA object.
|
\****************************************************************************/


/*
|| The structure of a TA.
*/
struct __TMS_STRUCTNAME(UPROTO_WMCTA)
	{
	/*
	|| Each object begins with a header, inlcuding a tag and a size.
	*/
	__TMS_USBPUMP_OBJECT_HEADER		uwmc_Header;

	/*
	|| Pointer to the APIs exported for clients above us.
	*/
	__TMS_CONST __TMS_UPROTO_WMCTA_DOWNCALLS *uwmc_pDownCalls;

	/*
	|| For convenience: a pointer to our parent UDEVICE.  We could
	|| also get this via the UINTERFACE, above.
	*/
	__TMS_UDEVICE *				uwmc_pDevice;

	/*
	|| pointer to communications class interface for this function.
	*/
	__TMS_UDATAPLANE *			uwmc_pControlPlane;

	/*
	|| For convenience, a pointer to the UDATASTREAM that represents our
	|| notification endpoint.  This can also be reached via the
	|| UINTERFACE above, but it's even more complicated to do so
	|| than getting the UDEVICE.
	*/
	__TMS_UDATASTREAM *			uwmc_pInterruptStream;

	/*
	|| uwmc_up is set TRUE in response to messages from the lower
	|| layers, indicating that the TA is configured and ready to go.
	*/
	__TMS_BOOL				uwmc_up;

	/*
	|| uwmc_started is set TRUE when TA has been activated and dataplane(s)
	|| are started.
	*/
	__TMS_BOOL				uwmc_started;

	/*
	|| We cache the most-recently-received setup packet that was targeting
	|| this function.  This simplifies processing of OUT SETUPs.
	*/
	__TMS_USETUP				uwmc_Setup;

	/*
	|| pCtlBuffer points to a pre-allocated buffer.  nCtlBuffer indicates
	|| the size of the buffer.  When an OUT SETUP is recieved, the data
	|| payload is received into the buffer.
	*/
	__TMS_UINT8 *				uwmc_pCtlBuffer;
	__TMS_USHORT				uwmc_nCtlBuffer;

	/*
	|| We want (possibly) a collection of data planes, though normally
	|| there's just one.
	*/
	__TMS_UDATAPLANE *			uwmc_pDataPlane;

	/*
	|| For each of the possible notifications that can be generated
	|| at this level, we need to have a separate UBUFQE (that is
	|| pre-allocated as part of this structure).  We only have one:
	|| the RESPONSE_AVAILABLE notification for encapsulated commands,
	|| which are assumed to be used by most of the subclasses here.
	*/
	__TMS_UBUFQE				uwmc_ResponseQe;

	/*
	|| This layer deals with converting WMC low-level abstractions into
	|| higher level operations, for all kinds of Comm-class WMC
	|| interfaces.  At some point, we actually have to look at the
	|| USB subclass-specific commands and do different things.  At that
	|| point, we have to dispatch to the sub-class handler that is
	|| specific to the kind of WMC object.
	||
	|| This pointer points to a (predetermined) implementation object that
	|| handles the subclass implementation.  It's an abstract pointer.
	|| There are no objects of kind UPROTO_WMCSUBCLASS, but you will
	|| see objects of type UPROTO_WMCMODEM; and UPROTO_WMCMODEM is
	|| derived from UPROTO_WMCSUBCLASS in the usual type-punning way.
	*/
	__TMS_UPROTO_WMCSUBCLASS *		uwmc_pWmcSubClass;

	/*
	|| It's conveninent to know our "bSubClass" and "bProtocol".  We
	|| might have circumstances in which we need to use a different
	|| descriptor that doesn't match our behavior; these fields are
	|| intended to match our actual behavoir.
	*/
	__TMS_UINT8				bSubClass;
	__TMS_UINT8				bProtocol;

	/*
	|| So that the code is not too wildly abstract, we provide ring
	|| buffer headers for handling encapsulated commands as part of any
	|| WMCTA object.  OBEX doesn't use it (yet) but it might.
	*/
	__TMS_UBUFQE *		uwmc_pEncapsInQe;	/* the in-UBUFQEs */
	__TMS_USB_RING_BUFFER	uwmc_InQueue;		/* for data to the host */

	__TMS_UBUFQE *		uwmc_pEncapsOutQe;	/* the out-UBUFQEs */
	__TMS_USB_RING_BUFFER	uwmc_OutQueue;		/* for data from the host */
	};

#define	__TMS_UPROTO_WMCTA_TAG	__TMS_UHIL_MEMTAG('W', 'm', 'T', 'A')

/* all TAs have the same base name */
#define	__TMS_UPROTO_WMCTA_NAME	"wmc.proto.mcci.com"

/*
|| UPROTO_WMCTA_CAN_ACCEPT_CDC_COMMANDS returns true if TA is up
*/
#define __TMS_UPROTO_WMCTA_CAN_ACCEPT_CDC_COMMANDS(pTA)	\
	((pTA)->uwmc_up)

/*
|| UPROTO_WMCTA_DATA_PLANES_ENABLED returns true if TA is up,
|| it has been activated and all dataplanes are started
*/
#define __TMS_UPROTO_WMCTA_DATA_PLANES_ENABLED(pTA)	\
	(((pTA)->uwmc_up && (pTA)->uwmc_started))

/****************************************************************************\
|
|	The client's incall structure
|
\****************************************************************************/

/*
|| In order to put data into the encapsulated reply buffer, client
|| (WMCSUBCLASS) code calls the PutReplyBuffer function. This function takes
|| a pointer to a buffer, and a byte count.  It copies as many bytes as
|| possible from the buffer to the ring buffer, and returns the count of
|| bytes copied (which might be zero).
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCTA_PUTREPLYBUFFER_FN,
	__TMS_BYTES,
		(
		__TMS_UPROTO_WMCTA *,
		__TMS_CONST __TMS_VOID * /* pBuffer */,
		__TMS_BYTES /* nBuffer */
		));

/*
|| This incall structure collects all the function pointers into a single
|| table.  It's separate from the UPROTO itself so that the table can reside
|| in ROM if needed, and can be shared by all the instances.
*/

/* the incall list */
struct __TMS_STRUCTNAME(UPROTO_WMCTA_INCALLS)
	{
	__TMS_UPROTOWMCTA_PUTREPLYBUFFER_FN *	pPutReplyBuffer;
	};

/*
|| Each in- or out-call structure must have an associated ..._INIT_Vx()
|| macro.  This macro expands to a C initialization constant, suitable
|| for creating a pre-initialized (CONST) structure.
||
|| Architecture: never change this macro's arguments; instead make a V2
|| macro, and then express this macro in terms of the V2 macro.
*/
#define __TMS_UPROTO_WMCTA_INCALLS_INIT_V1(				\
	pPutReplyBuffer							\
	)								\
	{								\
	(pPutReplyBuffer)						\
	}

/*
|| Each in- or out-call structure must have an associated ..._SETUP_Vx()
|| macro.  This macro expands to a sequence of run-time assignments.
|| The first argument is a pointer to the outcall structure; subsequent
|| arguments provide the initial values.  The statement is enclosed in
|| a do .. while(0) block so that it is exactly equivalent to a simple
|| statement (as far as the C compiler is concerned).
||
|| Architecture: never change this macro's arguments; instead make a V2
|| macro, and then express this macro in terms of the V2 macro.
*/
#define __TMS_UPROTO_WMCTA_INCALLS_SETUP_V1(				\
	pIncalls,							\
	ARG_pPutReplyBuffer						\
	)								\
    do	{								\
	(pIncalls)->pPutReplyBuffer = (ARG_pPutReplyBuffer);		\
	} while (0)

/****************************************************************************\
|
|	The structure of an WMCSUBCLASS object
|
\****************************************************************************/

/*
|| The subclass object represents the portion of the TA that cannot be
|| handled by the common CDC primitives.  As compared to the MCPC GL-004
|| implementation, a given TA normally has exactly one subclass; but to
|| make it easier to share code, we use a more dynamic architecture.
||
|| Each subclass object is derived from a common header.
*/

__TMS_TYPE_DEF_STRUCT(UPROTO_WMCSUBCLASS__HDR);
struct __TMS_STRUCTNAME(UPROTO_WMCSUBCLASS__HDR)
	{
	/* object header must be first! see the conversion macros */
	__TMS_USBPUMP_OBJECT_HEADER		Header;

	/* for diagnostics:  the name of the subclass */
	__TMS_CONST __TMS_TEXT *		pSubClassName;

	/*
	|| Each subclass instance object is statically attached to
	|| a single UPROTO_WMCTA.  With WMC (as opposed to MCPC), a subclass
	|| is bound to a UPROTO_WMCTA statically; the relationship is
	|| one-to-one and onto.
	*/
	__TMS_UPROTO_WMCTA *			pParent;

	/*
	|| Each subclass instance must provide an upcall table to be used
	|| by the lower-level wmc TA implementation code.
	*/
	__TMS_CONST __TMS_UPROTO_WMCSUBCLASS_UPCALLS *pUpcalls;

	/*
	|| Each subclass instance can (normally) be opened by an OS-specific
	|| driver.  This is such a common occurance that we put the info
	|| needed here.  Exception:  a virtual ethernet scheme might have
	|| a couple more layers on top of this.  If the object pointer is
	|| NULL, then we have no client object.
	||
	*/
	__TMS_USBPUMP_OBJECT_HEADER *		pClientObject;
	__TMS_VOID *				pClientContext;
	};

/*
|| Any concrete object derived from a WMCSUBCLASS must begin with this
|| macro.
*/
#define __TMS_UPROTO_WMCSUBCLASS_HDR					\
	/* beginning of any SUBCLASS instance */			\
	__TMS_UPROTO_WMCSUBCLASS__HDR	WmcSubClassHdr

/*
|| The generic code needs to be able to refer to a "generic" object: an
|| object with the standard header, but with no content.  Therefore, we
|| define UPROTO_WMCSUBCLASS as such an object.
*/
struct __TMS_STRUCTNAME(UPROTO_WMCSUBCLASS)
	{
	/* the standard header for WMCSUBCLASS objects */
	__TMS_UPROTO_WMCSUBCLASS_HDR;

	/* no subclass-specific content! */
	};

/* the unique tag */
#define	__TMS_UPROTO_WMCSUBCLASS_TAG(a, b)	__TMS_UHIL_MEMTAG('W', 's', a, b)

/*
|| A macro to generate a string constant for a subclass.
*/
#define	__TMS_UPROTO_WMCSUBCLASS_NAME(Text)	\
	__TMS_USBPUMP_OBJECT_NAME_FN("wmc", Text)


/* convert from a derived structure to a generic pointer */
#define __TMS_UPROTO_WMCSUBCLASS_from_Derived(pDerived)			\
	(__TMS_UPROTO_WMCSUBCLASS *)(&(pDerived)->WmcSubClassHdr)

/****************************************************************************\
|
|	The upcall tables for SUBCLASSs
|
\****************************************************************************/

/*
|| Each SUBCLASS implementation must provide a table of functions to be
|| used by the generic code.  Function types are declared here for each
|| of the required or optional functions.  Each function type name has the
|| form UPROTOWMCSUBCLASS_..._FN; this type can be used to declare the
|| prototype of the matching function in the header files.  There's also
|| PUPROTOWMCSUBCLAS_..._FN, which is a pointer to a function of the
|| underlying type.
||
|| In the following discussions, the phrase "If NULL, ..." means that if the
|| pointer of this type in the function table is NULL, the generic code
|| will behave as specified.
*/

/*
|| Each SUBCLASS implementation may provide a probe function.  The probe
|| function eaxamines the descriptors of the device, and the configured
|| state of the library (i.e., what functions are actually in the code-base),
|| and determines whether the subclass can actually support the TA.
|| This mechanism is provided in order to remain consistent with the other
|| WMC-like classes (e.g., MCPC).
||
|| Probe functions return TRUE for a match (i.e., if the generic code should
|| call the CREATE_FN), and FALSE otherwise.
||
|| If the probe function is NULL, the class implementation will assume
|| it should always match.
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_PROBE_FN,
	__TMS_BOOL,
		(
		__TMS_UDEVICE *			/* pDevice */,
		__TMS_UINTERFACE *		/* pUinterface */,
		__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE *,
		__TMS_USBPUMP_OBJECT_HEADER *	/* pProtoInitContext */,
		__TMS_CONST __TMS_UPROTO_WMCTA_CONFIG *
		));

/*
|| Each SUBCLASS must provide a Creation function.  The Creation function
|| must allocate, initialize, and return a pointer to the actual WMC
|| class implementation instance object.
||
|| The Creation function is passed pointers to its base WMCTA, to the
|| Configuration structure that was used to match the subclass, the
|| actual subclass and protocol that matched, and a pointer to the subclass
|| name.
||
|| Creation happens during initialization.
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_CREATE_FN,
	__TMS_UPROTO_WMCSUBCLASS *,
		(
		__TMS_UPROTO_WMCTA *,
		__TMS_CONST __TMS_UPROTO_WMCTA_CONFIG *,
		__TMS_CONST __TMS_UPROTO_WMCSUBCLASS_UPCALLS *,
		__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE *,
		__TMS_USBPUMP_OBJECT_HEADER *
		));

/*
|| Each SUBCLASS must provide a Delete function; this is called if
|| initialization fails or if an external subsystem asks us to shut
|| down.  (Not really used in embedded systems, intended for use in
|| systems such as WinXP Embedded.
||
|| The deletion function must undo all memory allocations it performed, and
|| unlink from any system data structures.
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_DELETE_FN,
	__TMS_VOID, (__TMS_UPROTO_WMCSUBCLASS *)
	);

/*
|| Each SUBCLASS may provide an Activate function.  The Activate function,
|| if not NULL, is called when the underlying TA is configured and ready
|| to accept control-plane commands.  It's called before any control-plane
|| commands are delivered to the SUBCLASS.
||
|| The Activate function is allowed to return FALSE to signal that it's not
|| presently possible to bring the function up, for example if resources
|| are not available.  With some OBEX implementations, for example, the
|| Activate function might return false if the server is busy.
||
|| Some hardware might not give the DataPump the ability to intervene
|| and stall the SET_INTERFACE or SET_CONFIG command that resulted in
|| the ACTIVATE upcall.  These chips actually complete the SETUP transaction
|| before the DataPump is notified.  So failing at this interface does not
|| have portable effects.
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_ACTIVATE_FN,
	__TMS_BOOL, (__TMS_UPROTO_WMCSUBCLASS *)
	);

/*
|| Each SUBCLASS may provide an Deactivate function.  The Deactivate function,
|| if not NULL, is called when the underlying TA is de-configured.  It
|| will never be called unless the TA was previously activated.
|| After it's called, no further control-plane commands are delivered to
|| the SUBCLASS until the next Activate.
||
|| If the Deactivate function is NULL, the SUBCLASS is silently
|| deactivated.
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_DEACTIVATE_FN,
	__TMS_VOID, (__TMS_UPROTO_WMCSUBCLASS *)
	);

/*
|| Each SUBCLASS that implements SEND_ENCAPSULATED_COMMAND must provide
|| a ProcessEncapsulatedCommand function.
||
|| SEND_ENCAPSULATED_COMMAND is not allowed to return STALL due to protocol
|| issues (at least according to MCCI's interpretation).  If a bad command
|| is received, the response must be returned in-band.  Therefore, the
|| ProcessEncapsulatedCommand function does not return status; the DataPump
|| always completes SEND_ENCAPSULATED_COMMAND with success (provided it is
|| able to receive the command).
||
|| The Encapsulated command engine works as follows (at this level).
||
||	1) Each WMCTA has an embedded USB_RING_BUFFER for Encapsulated
||	   commands.
||
||	2) When a SEND_ENCAPSULATED_COMMAND is received, the WMCTA code
||	   receives the data from the host and places the data in the
||	   ring buffer.  The WMCTA code first makes sure there's room in
||	   the ring buffer to receive ALL the data; otherwise it returns
||	   STALL for that command.
||
||	3) After data has been received into the ring buffer, the WMCTA
||	   code calls ProcessEncapsulatedCommand.  This function must
||	   remove as much data as it can from the ring buffer (using
||	   UsbRingBuffer_Get or equivalent) and pass it on to the data
||	   consumer.  So this upcall is basically a notification when
||	   more data is placed in the buffer.  The next layer up is
||	   responsible for figuring out how to pass on to the consumer and
||	   scheduling additional calls to UsbRingBuffer_Get when the
||	   consumer is able to accept more data.
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_PROCESS_ENCAPSULATED_COMMAND_FN,
	__TMS_VOID,
		(
		__TMS_UPROTO_WMCSUBCLASS *
		)
	);

/*
|| Each SUBCLASS that implements GET_ENCAPSULATED_RESPONSE must provide
|| a PullEncapsulatedReply function.
||
|| GET_ENCAPSULATED_REPLY is implemented as a ring buffer that is drained
|| by the WMCTA layer in response to GET_ENCAPSULATED_RESPONSE commands.
||
|| The sequence of operations is as follows:
||
|| 1)	The upper layer puts data into the buffer using a TA incall,
||	pWmcTA->pDownCalls->pPutReplyBuffer.
||
|| 2)	The WMCTA layer captures as much data as it can, and then sends
||	a response-available notification to the host (if necessary).
||
|| 3)	control returns immediately to the client.
||
|| 4)	Later, the host issues GET_ENCAPSULATED_RESPONSE.  The WMCTA layer
||	passes as much data as possible from the ring buffer to the host;
||	and then it calls the PullEncapsulatedReply function, which should
||	use this upcall to put more data into the ring buffer, if
||	possible.
||
|| PullEncapsulatedReply is therefore a notification upcall; it does not
|| return status, instead it uses the upcall to drive the data flow in the
|| next layer.  (You can also say that "this function is a hint to cause
|| the upper layer to try to send more data".)
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_PULL_ENCAPSULATED_REPLY_FN,
	__TMS_VOID,
		(
		__TMS_UPROTO_WMCSUBCLASS *
		)
	);

/*
|| The WMCSUBCLASS implementations may have class specific or vendor-specific
|| commands that are not processed by the DataPump or the WMCTA.  In order
|| to implememt these, the WMCSUBCLASS must provide a ProcessSetup function.
|| This function is called after the SETUP has been received from the host,
|| and after any OUT data has been received.
||
|| The ProcessSetup function is passed a pointer to the SUBCLASS object,
|| a pointer to the SETUP packet, a pointer to any OUT data (or NULL if
|| this is an IN-type SETUP), the size of the OUT data actually received,
|| a pointer to an input buffer which can be used for formatting responses
|| for an IN-type SETUP (or NULL for OUT-type SETUPs), the size of the
|| buffer, and a pointer to a structure that gets the status of the
|| setup processing.
*/
__TMS_TYPE_DEF_STRUCT(WMCSUBCLASS_SETUP_RESULT);
struct __TMS_STRUCTNAME(WMCSUBCLASS_SETUP_RESULT)
	{
	/*
	|| When processing a setup, two results are needed:
	|| a pointer to a data buffer, and a length.  The
	|| results are encoded as follows:
	||
	|| 1) pDataToHost = non-NULL, nDataToHost >= 0:
	||	send the specified buffer to the host
	||	during the IN phase; note that nDataToHost
	||	== 0 forces a ZLP, and is also used to
	||	indicate SUCCESS on an OUT SETUP.
	|| 2) pDataToHost = NULL (in which case nDataToHost
	||	should be zero, but is ignored):  indicates
	||	an error, and a STALL is sent at the
	||	earliest opportunity (data phase or status
	||	phase).
	*/
	__TMS_UINT8 *		pDataToHost;
	__TMS_BYTES		nDataToHost;
	};

__TMS_FNTYPE_DEF(							\
	UPROTOWMCSUBCLASS_PROCESS_SETUP_FN,                             \
	__TMS_VOID,                                                     \
		(                                                       \
		__TMS_UPROTO_WMCSUBCLASS *,                             \
		__TMS_CONST __TMS_USETUP *,                             \
		__TMS_CONST __TMS_UINT8 *  /*pOutData*/,                \
		__TMS_BYTES		   /*nOutData*/,                \
		__TMS_UINT8 *		   /*pInDataBuffer*/,           \
		__TMS_BYTES		   /*nInDataBuffer*/,           \
		__TMS_WMCSUBCLASS_SETUP_RESULT * /*pResultData*/        \
		)                                                       \
	);

/*
|| Each SUBCLASS may provide a BusStateChange function.  This function
|| is called when a "significant" bus state change occurs.
|| The raw UEVENT block is passed, so that the BusStateChange function
|| can decode as needed.
||
|| The following events will be passed:
||	UEVENT_SUSPEND		if we enter suspend
||	UEVENT_L1_SLEEP		if we enter L1 sleep
||	UEVENT_FUNCTION_SUSPEND	if we enter function suspend
||	UEVENT_RESUME		if we leave suspend or L1 sleep
||	UEVENT_FUNCTION_RESUME	if we leave function suspend
||	UEVENT_RESET		if we see a USB reset
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_BUS_STATE_CHANGE_FN,
	__TMS_VOID,
		(
		__TMS_UPROTO_WMCSUBCLASS *,
		__TMS_UEVENT  /* why we're here */,
		__TMS_VOID * /* event-specific info */
		)
	);

/*
|| Each SUBCLASS with a distinct dataplane must provide a StartDataPlane
|| function.  This is called when the dataplane is readied for communication
|| either by SET_INTERFACE, SET_CONFIG, or possibly even via a subclass-
|| specific command.  It's not called for RESUME.
||
|| See udataplane.h for a description of the dataplane.
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_START_DATA_PLANE_FN,
	__TMS_VOID,
		(
		__TMS_UPROTO_WMCSUBCLASS *,
		__TMS_UDATAPLANE *
		)
	);

/*
|| Similarly, each SUBCLASS with a distinct dataplane must provide a
|| StopDataPlane function.  This is called when the dataplane is idled,
|| either by USB RESET, SET_CONFIG to a different configuration, SET_ADDRESS
|| to zero, SET_INTERFACE to a different alternate setting, or removal
|| of cable [which cannot always be detected, as it's hardware-specific].
||  It's not called for SUSPEND.
*/
__TMS_FNTYPE_DEF(
	UPROTOWMCSUBCLASS_STOP_DATA_PLANE_FN,
	__TMS_VOID,
		(
		__TMS_UPROTO_WMCSUBCLASS *,
		__TMS_UDATAPLANE *
		)
	);

/* the outcall list */
struct __TMS_STRUCTNAME(UPROTO_WMCSUBCLASS_UPCALLS)
	{
	__TMS_UPROTOWMCSUBCLASS_PROBE_FN	*pProbe;
	__TMS_UPROTOWMCSUBCLASS_CREATE_FN	*pCreate;
	__TMS_UPROTOWMCSUBCLASS_DELETE_FN	*pDelete;
	__TMS_UPROTOWMCSUBCLASS_ACTIVATE_FN	*pActivate;
	__TMS_UPROTOWMCSUBCLASS_DEACTIVATE_FN	*pDeactivate;
	__TMS_UPROTOWMCSUBCLASS_PROCESS_ENCAPSULATED_COMMAND_FN
						*pProcessEncapsulatedCommand;
	__TMS_UPROTOWMCSUBCLASS_PULL_ENCAPSULATED_REPLY_FN
						*pPullEncapsulatedReply;
	__TMS_UPROTOWMCSUBCLASS_PROCESS_SETUP_FN
						*pProcessSetup;
	__TMS_UPROTOWMCSUBCLASS_BUS_STATE_CHANGE_FN
						*pSuspend;
	__TMS_UPROTOWMCSUBCLASS_START_DATA_PLANE_FN
						*pStartDataPlane;
	__TMS_UPROTOWMCSUBCLASS_STOP_DATA_PLANE_FN
						*pStopDataPlane;
	};

/*
|| architecture: never change this macro's arguments; instead make V2 macro,
|| and express this in terms of the V2 macro.
*/
#define __TMS_UPROTO_WMCSUBCLASS_UPCALLS_INIT_V1(			\
	pProbe,								\
	pCreate,							\
	pDelete,							\
	pActivate,							\
	pDeactivate,							\
	pProcessEncapsulatedCommand,					\
	pPullEncapsulatedReply,						\
	pProcessSetup,							\
	pBusStateChange,						\
	pStartDataPlane,						\
	pStopDataPlane							\
	)								\
	{								\
	(pProbe),							\
	(pCreate),							\
	(pDelete),							\
	(pActivate),							\
	(pDeactivate),							\
	(pProcessEncapsulatedCommand),					\
	(pPullEncapsulatedReply),					\
	(pProcessSetup),						\
	(pBusStateChange),						\
	(pStartDataPlane),						\
	(pStopDataPlane)						\
	}

#define __TMS_UPROTO_WMCSUBCLASS_UPCALLS_SETUP_V1(			\
	pUpcalls,							\
	ARG_pProbe,							\
	ARG_pCreate,							\
	ARG_pDelete,							\
	ARG_pActivate,							\
	ARG_pDeactivate,						\
	ARG_pProcessEncapsulatedCommand,				\
	ARG_pPullEncapsulatedReply,					\
	ARG_pProcessSetup,						\
	ARG_pBusStateChange,						\
	ARG_pStartDataPlane,						\
	ARG_pStopDataPlane						\
	)								\
    do	{								\
	(pUpcalls)->pProbe = (ARG_pProbe);				\
	(pUpcalls)->pCreate = (ARG_pCreate);				\
	(pUpcalls)->pDelete = (ARG_pDelete);				\
	(pUpcalls)->pActivate = (ARG_pActivate);			\
	(pUpcalls)->pDeactivate = (ARG_pDeactivate);			\
	(pUpcalls)->pProcessEncapsulatedCommand =			\
			(ARG_pProcessEncapsulatedCommand);		\
	(pUpcalls)->pPullEncapsulatedReply =				\
			(ARG_pPullEncapsulatedReply);			\
	(pUpcalls)->pProcessSetup = (ARG_pProcessSetup);		\
	(pUpcalls)->pBusStateChange = (ARG_pBusStateChange);		\
	(pUpcalls)->pStartDataPlane = (ARG_pStartDataPlane);		\
	(pUpcalls)->pStopDataPlane = (ARG_pStopDataPlane);		\
	} while (0)


/****************************************************************************\
|
|	The API functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_PROTOCOL_PROBE_FN		WmcTA_ProtocolProbe;
__TMS_USBPUMP_PROTOCOL_CREATE_FN	WmcTA_ProtocolCreate;
__TMS_USBPUMP_PROTOCOL_ADD_INTERFACE_FN WmcTA_AddDataInterface;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPROTO_WMCTA_ENCAPSQUEUE_SIZE	\
   __TMS_UPROTO_WMCTA_ENCAPSQUEUE_SIZE
# define UPROTO_WMCTA_RESPONSE_AVAIL_SIZE	\
   __TMS_UPROTO_WMCTA_RESPONSE_AVAIL_SIZE
# define UPROTO_WMCTA_TAG	\
   __TMS_UPROTO_WMCTA_TAG
# define UPROTO_WMCTA_NAME	\
   __TMS_UPROTO_WMCTA_NAME
# define UPROTO_WMCTA_CAN_ACCEPT_CDC_COMMANDS(pTA)	\
   __TMS_UPROTO_WMCTA_CAN_ACCEPT_CDC_COMMANDS(pTA)
# define UPROTO_WMCTA_DATA_PLANES_ENABLED(pTA)	\
   __TMS_UPROTO_WMCTA_DATA_PLANES_ENABLED(pTA)
# define UPROTO_WMCTA_INCALLS_INIT_V1(				\
	pPutReplyBuffer							\
	)	\
	__TMS_UPROTO_WMCTA_INCALLS_INIT_V1(				\
	pPutReplyBuffer							\
	)
# define UPROTO_WMCTA_INCALLS_SETUP_V1(				\
	pIncalls,							\
	ARG_pPutReplyBuffer						\
	)	\
	__TMS_UPROTO_WMCTA_INCALLS_SETUP_V1(				\
	pIncalls,							\
	ARG_pPutReplyBuffer						\
	)
# define UPROTO_WMCSUBCLASS_HDR	\
   __TMS_UPROTO_WMCSUBCLASS_HDR
# define UPROTO_WMCSUBCLASS_TAG(a, b)	\
   __TMS_UPROTO_WMCSUBCLASS_TAG(a, b)
# define UPROTO_WMCSUBCLASS_NAME(Text)	\
   __TMS_UPROTO_WMCSUBCLASS_NAME(Text)
# define UPROTO_WMCSUBCLASS_from_Derived(pDerived)	\
   __TMS_UPROTO_WMCSUBCLASS_from_Derived(pDerived)
# define UPROTO_WMCSUBCLASS_UPCALLS_INIT_V1(			\
	pProbe,								\
	pCreate,							\
	pDelete,							\
	pActivate,							\
	pDeactivate,							\
	pProcessEncapsulatedCommand,					\
	pPullEncapsulatedReply,						\
	pProcessSetup,							\
	pBusStateChange,						\
	pStartDataPlane,						\
	pStopDataPlane							\
	)	\
	__TMS_UPROTO_WMCSUBCLASS_UPCALLS_INIT_V1(			\
	pProbe,								\
	pCreate,							\
	pDelete,							\
	pActivate,							\
	pDeactivate,							\
	pProcessEncapsulatedCommand,					\
	pPullEncapsulatedReply,						\
	pProcessSetup,							\
	pBusStateChange,						\
	pStartDataPlane,						\
	pStopDataPlane							\
	)
# define UPROTO_WMCSUBCLASS_UPCALLS_SETUP_V1(			\
	pUpcalls,							\
	ARG_pProbe,							\
	ARG_pCreate,							\
	ARG_pDelete,							\
	ARG_pActivate,							\
	ARG_pDeactivate,						\
	ARG_pProcessEncapsulatedCommand,				\
	ARG_pPullEncapsulatedReply,					\
	ARG_pProcessSetup,						\
	ARG_pBusStateChange,						\
	ARG_pStartDataPlane,						\
	ARG_pStopDataPlane						\
	)	\
	__TMS_UPROTO_WMCSUBCLASS_UPCALLS_SETUP_V1(			\
	pUpcalls,							\
	ARG_pProbe,							\
	ARG_pCreate,							\
	ARG_pDelete,							\
	ARG_pActivate,							\
	ARG_pDeactivate,						\
	ARG_pProcessEncapsulatedCommand,				\
	ARG_pPullEncapsulatedReply,					\
	ARG_pProcessSetup,						\
	ARG_pBusStateChange,						\
	ARG_pStartDataPlane,						\
	ARG_pStopDataPlane						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of protowmc.h ****/
#endif /* _PROTOWMC_H_ */
