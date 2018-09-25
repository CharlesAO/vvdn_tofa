/* ubufqe.h	Mon Aug 10 2015 14:35:31 tmm */

/*

Module:  ubufqe.h

Function:
	Define UBUFQE and associated functions.

Version:
	V3.15c	Mon Aug 10 2015 14:35:31 tmm	Edit level 20

Copyright notice:
	This file copyright (C) 2002-2009, 2011-2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.64a  Mon May 27 2002 16:47:59  tmm
	Module created.

   1.87a  Sun Nov  2 2003 00:50:03  tmm
	Clean up documentation for autodoc project.

   1.91a  Thu Feb 10 2005 09:38:45  chwon
	Add uqe_usrflags into UBUFQE.

   1.97i  Wed May 16 2007 11:13:08  chwon
	2889: take out uqe_usrflags

   1.97k  Thu Jan 31 2008 12:54:20  tmm
	3735: reorganize UBUFQE to allow 32-bit sizes and allow 
	better ISOCH support, using the host stack's URB as a 
	model.

	3746:  fix name cloaking.  Add __TMS_PR3746_DISABLE
	as a compile parameter to restore old (incorrect)
	behavior.

   1.97k  Fri Jul 18 2008 09:31:11  chwon
	4312: add USBPUMP_BUFFER_HANDLE in the UBUFQE

   1.97k  Tue Sep 16 2008 12:39:35  chwon
	Fixed doc-xml error

   1.97k  Fri Oct 10 2008 09:14:52  chwon
	6833: re-add uqe_usrflags

   2.01a  Mon Feb 16 2009 09:40:47  chwon
	4312: add USBPUMP_BUFFER_HANDLE for isoch packet header

   2.01a  Mon Apr 27 2009 16:14:11  chwon
	3735: Fixed usage of __TMS_CONTAINER_OF() macro.

   2.01a  Wed Jul 22 2009 12:08:18  chwon
	8253: add UBUFQE_RQ_SS_STREAM_IN/OUT and related structure to support
	USB 3.0 streams.

   2.01a  Wed Aug 19 2009 14:34:36  chwon
	8748: add UsbPumpBufferQeI_HbufferEnterScope() and
	UsbPumpBufferQeI_HbufferExitScope().  UsbPumpBufferQeI_Validate() calls
	HbufferEnterScope() to prepare buffer handle.  UBUFQE_COMPLETE() calls
	HbufferExitScope() to dereference the buffer handle.

   3.01f  Fri Sep 30 2011 16:29:41  cccho
	13233: activate the IN endpoint if there is a completion on an OUT
	pipe in the same interface.

   3.01f  Wed Feb 08 2012 11:18:49  chwon
	14805: Add UBUFQEFLAG_ISOCH_ASAP flag.

   3.01f  Mon May 21 2012 12:01:29  chwon
	15387: Add internal flags -- UBUFQEFLAG_INTERNAL_BUFFER_ENTERSCOPE and
	UBUFQEFLAG_INTERNAL_ISOCHDESC_ENTERSCOPE.

   3.11c  Mon Feb 18 2013 14:58:28  chwon
	16704: Add UBUFQEFLAG_NO_CACHE flag.

   3.11e  Thu Jan 02 2014 12:40:02  chwon
	17972: Add UBUFQE_RQ_SGL_STREAM_IN and UBUFQE_RQ_SGL_STREAM_OUT request
	and related structure.

   3.11e  Fri Jan 10 2014 16:31:11  chwon
	17972: Fix typo in __TMS_UBUFQE_SGNODE_BODY() macro.

   3.15b  Sun May 31 2015 21:40:46  tmm
	19019: improve documentation.

   3.15b  Sun May 31 2015 21:40:46  tmm
	19211: correct usages of "DataPump".

   3.15c  Mon Aug 10 2015 14:35:31  tmm
	19019: improve documentation.

*/

#ifndef _UBUFQE_H_		/* prevent multiple includes */
#define _UBUFQE_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	The UBUFQE structure
|
\****************************************************************************/

/*

Type:	UBUFIODONEFN

Function:
	C function type for UBUFQE I/O completion functions.

Definition:
	typedef VOID UBUFIODONEFN(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint,
			UBUFQE *pQe
			);

	typedef UBUFIODONEFN *PUBUFIODONEFN;

Description:
	Each UBUFQE carries with it a pointer to an I/O completion function,
	provided by the routine that submitted the UBUFQE.
	When the lower layer has completed processing the UBUFQE, it takes
	the following steps.

	1) pQe->uqe_status is set to the (non-zero) completion status.
	2) the completion function is called.

	All UBUFQE completion functions share a common prototype,
	UBUFIODONEFN, and should be declared in header files using
	UBUFIODONEFN, rather than with an explicit prototype.

	For example, if a concrete implementation defines an completion
	function named UsbWmc_MyDoneFn, then the *header file* or static
	defininiton portion of the file should prototype the function using:

		UBUFIODONEFN UsbWmc_MyDoneFn;

	rather than writing:
		VOID UsbWmc_MyDoneFn(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint,
			UBUFQE *pQe
			);

	If you want to have a prototype for reference in the code, you
	should write the prototype *twice*:

		UBUFIODONEFN UsbWmc_MyDoneFn;

		// for reference, the above is equivalent to:
		VOID UsbWmc_MyDoneFn(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint,
			UBUFQE *pQe
			);

	The justification for this design approach is that it highlights
	the fact that the function prototype is not under the control of
	the function implementor, but rather highlights the fact that the
	function is a method implementation for some class.

Returns:
	UBUFIODONEFNs do not return any explicit result.  The UBUFQE
	status should be examined, and the UBUFQE should be disposed of
	in an appropriate way.

Notes:
	UBUFIODONEFNs are really virtual methods of the UBUFQE, so the
	parameter order does not comply with current MCCI coding standards.

*/

/* I/O completion function */
/* -- this is in usbpump_types.h */
/** __TMS_FNTYPE_DEF(
* 	UBUFIODONEFN,
* 	__TMS_VOID,
* 		(
* 		__TMS_UDEVICE *,
* 		__TMS_UENDPOINT *,
* 		__TMS_UBUFQE *
* 		)
* 	);
*/

/*

Type:	UBUFQE_FLAGS

Function:
	C scalar (unsigned) type for variables that convey UBUFQE flags.

Definition:
	typedef	UINT16		 UBUFQE_FLAGS,
				*PUBUFQE_FLAGS;
	typedef ARG_UINT16	 ARG_UBUFQE_FLAGS,
				*PARG_UBUFQE_FLAGS;

Description:
	This type is used to represent a value that contains a UBUFQE flag
	setting.  Users should not write code that assumes that this value
	has any particular bit-width.

See also:
	UBUFQE, UBUFQEFLAG_...

Notes:
	Prior to V1.97k, this was an 8-bit type.

*/

/* I/O buffer flags: in usbpump_types.h */
/* __TMS_TYPE_DEF_ARG(UBUFQE_FLAGS, UINT16); */

/*

Type:	UBUFQE_LENGTH

Index:	Type:	PUBUFQE_LENGTH
	Type:	ARG_UBUFQE_LENGTH
	Type:	PARG_UBUFQE_LENGTH

Function:
	C scalar (unsigned) type for variables that convey UBUFQE sizes.

Definition:
	typedef	UINT16		 UBUFQE_LENGTH,
				*PUBUFQE_LENGTH;
	typedef ARG_UINT16	 ARG_UBUFQE_LENGTH,
				*PARG_UBUFQE_LENGTH;

Description:
	This type is used to represent a value that contains the size
	of a UBUFQE.  Note that since this is a UINT16, UBUFQEs cannot be
	larger than 64K bytes.  (This is not a practical restriction.)
	Users should not write code that assumes that this value
	has any particular bit-width.

See also:
	UBUFQE.

*/

/* I/O buffer flags: in usbpump_types.h */
/* __TMS_TYPE_DEF_ARG(UBUFQE_LENGTH, UINT16); */

/*

Type:	UBUFQE_REQUEST

Index:	Type:	PUBUFQE_REQUEST
	Type:	ARG_UBUFQE_REQUEST
	Type:	PARG_UBUFQE_REQUEST
	Const:	UBUFQE_RQ_ZERO
	Const:	UBUFQE_RQ_LEGACY
	Const:	UBUFQE_RQ_IMPLICIT
	Const:	UBUFQE_RQ_STREAM_IN
	Const:	UBUFQE_RQ_STREAM_OUT
	Const:	UBUFQE_RQ_PACKET_IN
	Const:	UBUFQE_RQ_PACKET_OUT
	Const:	UBUFQE_RQ_ISOCH_IN
	Const:	UBUFQE_RQ_ISOCH_OUT
	Const:	UBUFQE_RQ__N
	Macro:	UBUFQE_RQ_SIZE__INIT
	Macro:	UBUFQE_RQ_NAME__INIT

Function:
	C scalar (unsigned) type for variables that convey UBUFQE sizes.

Definition:
	typedef	UINT8		 UBUFQE_REQUEST,
				*PUBUFQE_REQUEST;
	typedef ARG_UINT8	 ARG_UBUFQE_REQUEST,
				*PARG_UBUFQE_REQUEST;

Description:
	This type is used to represent a value that contains the size
	of a UBUFQE.  Note that since this is a UINT8, only 256
	requests are possible.

	Users should not write code that assumes that this value
	has any particular bit-width.

	The following request codes are defined.

	|UBUFQE_RQ_ZERO|
		This is the operation code of zero.  It indicates
		that the UBUFQE has been prepared by legacy code,
		but not yet detected.

	|UBUFQE_RQ_LEGACY|
		This is like UBUFQE_RQ_IMPLICIT, but it indicates
		that the UBUFQE was prepared by legacy code, 
		detected by the DataPump on submission, and that
		the uqe_length was set by the DataPump (rather than
		by the caller).

	|UBUFQE_RQ_IMPLICIT|
		The operation defined by the UBUFQE is defined
		by the kind of pipe that is the target.  For
		bulk IN, interrupt IN and the default pipe's
		IN-part, the request causes data to be transmitted 
		to the host.  For bulk OUT, interrupt OUT and
		the default pipe's OUT-part, the request causes
		data to be received from the host.  For isoch-OUT
		pipes, data is received and is additionally 
		formatted with interleaved headers.  For isoch-
		IN pipes, data in the buffer should consist of
		interleaved headers and packets.

	|UBUFQE_RQ_STREAM_IN|
		Transmit bulk, interrupt, control data 
		to the host via an IN pipe.  UBUFQE has the
		normal size.  

		For bulk, interrupt and control pipes, data is
		transmitted from the buffer, and is packetized
		as appropriate.

	|UBUFQE_RQ_STREAM_OUT|
		Receive data from a bulk, interrupt or control
		OUT pipe.  Data is placed contiguosly into the
		buffer.  UBUFQE is normal size.

	|UBUFQE_RQ_PACKET_IN|
		Transmit data to the host over an isochronous
		IN pipe.  Data must consist of a properly-
		formatted sequence of headers.  Headers are
		updated with status after transmit, so this 
		cannot be the same as UBUFQE_RQ_STREAM_IN, because
		the buffer is not read-only.
			
	|UBUFQE_RQ_PACKET_OUT|
		Receive data from an isoch OUT pipe.  Received 
		data is placed into the buffer with headers 
		interleved.  All the data is contained in the buffer 
		so this may be thought of as a special kind of
		streaming.  However, a separate request is 
		provided for symmetry with UBUFQE_RQ_PACKET_IN.

	|UBUFQE_RQ_ISOCH_OUT|
		Receive isochronous data from a isoch-OUT 
		pipe, placing packet information in a separate
		array of USBPUMP_ISOCH_PACKET_DESCRs.  The 
		UBUFQE must be larger than normal, and should be
		at least as large as sizeof(UBUFQE_ISOCH_OUT).

	|UBUFQE_RQ_ISOCH_IN|
		Transmit isochronous data to an isoch-IN 
		pipe, placing packet information in a separate
		array of USBPUMP_ISOCH_PACKET_DESCRs.  The 
		UBUFQE must be larger than normal, and should be
		at least as large as sizeof(UBUFQE_ISOCH_IN).

	|UBUFQE_RQ_SS_STREAM_IN|
		Transmit USB 3.0 stream data to the host via an IN pipe
		with stream ID.  The UBUFQE must be larger than normal,
		and should be at least as large as
		sizeof(UBUFQE_SS_STREAM_IN).

	|UBUFQE_RQ_SS_STREAM_OUT|
		Receive USB 3.0 stream data from host via an OUT pipe
		with stream ID.  The UBUFQE must be larger than normal,
		and should be at least as large as
		sizeof(UBUFQE_SS_STREAM_OUT).

	|UBUFQE_RQ_SGL_STREAM_IN|
		Transmit bulk or interrupt data using a scatter/gather
		list. This also supports USB 3.0 streams (but can be
		used with any endpoint.  The UBUFQE must be larger than 
		normal, and should be at least as large as
		sizeof(UBUFQE_RQ_SGL_STREAM_IN).

	|UBUFQE_RQ_SGL_STREAM_OUT|
		Receive bulk or interrupt data using a scatter/gather
		list. This also supports USB 3.0 streams (but can be
		used with any endpoint.  The UBUFQE must be larger than 
		normal, and should be at least as large as
		sizeof(UBUFQE_RQ_SGL_STREAM_OUT).

	|UBUFQE_RQ__N|
		This integer constant is one greater than the highest
		UBUFQE_REQUEST code.


	The macro |UBUFQE_RQ_SIZE__INIT| can be used to initialize an
	array that maps in-range UBUFQE_REQUEST values to equivalent
	minimum sizes of the UBUFQE.

	The macro |UBUFQE_RQ_NAME__INIT| can be used to initialize a
	string array that maps UBUFQE_REQUEST values to equivalent
	human-readable names.

See also:
	UBUFQE.

*/

/* I/O buffer flags: in usbpump_types.h */
/* __TMS_TYPE_DEF_ARG(UBUFQE_REQUEST, UINT8); */

#define	__TMS_UBUFQE_RQ_ZERO		0u
#define	__TMS_UBUFQE_RQ_LEGACY		1u
#define	__TMS_UBUFQE_RQ_IMPLICIT	2u
#define	__TMS_UBUFQE_RQ_STREAM_IN	3u
#define	__TMS_UBUFQE_RQ_STREAM_OUT	4u
#define	__TMS_UBUFQE_RQ_PACKET_IN	5u
#define	__TMS_UBUFQE_RQ_PACKET_OUT	6u
#define	__TMS_UBUFQE_RQ_ISOCH_IN	7u
#define	__TMS_UBUFQE_RQ_ISOCH_OUT	8u
#define	__TMS_UBUFQE_RQ_SS_STREAM_IN	9u
#define	__TMS_UBUFQE_RQ_SS_STREAM_OUT	10u
#define	__TMS_UBUFQE_RQ_SGL_STREAM_IN	11u
#define	__TMS_UBUFQE_RQ_SGL_STREAM_OUT	12u

/* 
|| number of defined reqeusts.  this must be one greater than 
|| the last defined request -- for making arrays, etc 
*/
#define	__TMS_UBUFQE_RQ__N		13u

#define __TMS_UBFUQE_RQ_SIZE__INIT					\
	/* UBUFQE_RQ_ZERO */		sizeof(struct TTUSB_UBUFQE),	\
	/* UBUFQE_RQ_LEGACY */		sizeof(struct TTUSB_UBUFQE),	\
	/* UBUFQE_RQ_IMPLICIT */	sizeof(struct TTUSB_UBUFQE),	\
	/* UBUFQE_RQ_STREAM_IN */	sizeof(__TMS_UBUFQE_STREAM_IN),	\
	/* UBUFQE_RQ_STREAM_OUT	*/	sizeof(__TMS_UBUFQE_STREAM_OUT),\
	/* UBUFQE_RQ_PACKET_IN */	sizeof(__TMS_UBUFQE_PACKET_IN),	\
	/* UBUFQE_RQ_PACKET_OUT */	sizeof(__TMS_UBUFQE_PACKET_OUT),\
	/* UBUFQE_RQ_ISOCH_IN */	sizeof(__TMS_UBUFQE_ISOCH_IN),	\
	/* UBUFQE_RQ_ISOCH_OUT */	sizeof(__TMS_UBUFQE_ISOCH_OUT),	\
	/* UBUFQE_RQ_SS_STREAM_IN */	sizeof(__TMS_UBUFQE_SS_STREAM_IN), \
	/* UBUFQE_RQ_SS_STREAM_OUT */	sizeof(__TMS_UBUFQE_SS_STREAM_OUT), \
	/* UBUFQE_RQ_SGL_STREAM_IN */	sizeof(__TMS_UBUFQE_SGL_STREAM_IN), \
	/* UBUFQE_RQ_SGL_STREAM_OUT */	sizeof(__TMS_UBUFQE_SGL_STREAM_OUT)

#define	__TMS_UBUFQE_RQ_NAME__INIT					\
	/* UBUFQE_RQ_ZERO */		"UBUFQE_RQ_ZERO",		\
	/* UBUFQE_RQ_LEGACY */		"UBUFQE_RQ_LEGACY",		\
	/* UBUFQE_RQ_IMPLICIT */	"UBUFQE_RQ_IMPLICIT",		\
	/* UBUFQE_RQ_STREAM_IN */	"UBUFQE_RQ_STREAM_IN",		\
	/* UBUFQE_RQ_STREAM_OUT	*/	"UBUFQE_RQ_STREAM_OUT",		\
	/* UBUFQE_RQ_PACKET_IN */	"UBUFQE_RQ_PACKET_IN",		\
	/* UBUFQE_RQ_PACKET_OUT */	"UBUFQE_RQ_PACKET_OUT",		\
	/* UBUFQE_RQ_ISOCH_IN */	"UBUFQE_RQ_ISOCH_IN",		\
	/* UBUFQE_RQ_ISOCH_OUT */	"UBUFQE_RQ_ISOCH_OUT",		\
	/* UBUFQE_RQ_SS_STREAM_IN */	"UBUFQE_RQ_SS_STREAM_IN",	\
	/* UBUFQE_RQ_SS_STREAM_OUT */	"UBUFQE_RQ_SS_STREAM_OUT",	\
	/* UBUFQE_RQ_SGL_STREAM_IN */	"UBUFQE_RQ_SGL_STREAM_IN",	\
	/* UBUFQE_RQ_SGL_STREAM_OUT */	"UBUFQE_RQ_SGL_STREAM_OUT"

/*
|| a mask of requests that take more than the minimum size
*/
#define	__TMS_UBUFQE_RQMASK__LARGE			\
	((1u << __TMS_UBUFQE_RQ_ISOCH_IN) |		\
	 (1u << __TMS_UBUFQE_RQ_ISOCH_OUT) |		\
	 (1u << __TMS_UBUFQE_RQ_SS_STREAM_IN) |		\
	 (1u << __TMS_UBUFQE_RQ_SS_STREAM_OUT) | 	\
	 (1u << __TMS_UBUFQE_RQ_SGL_STREAM_IN) |	\
	 (1u << __TMS_UBUFQE_RQ_SGL_STREAM_OUT))

/*

Type:	UBUFQE

Index:	Const:	UBUFQEFLAG_PREBREAK
	Const:	UBUFQEFLAG_POSTBREAK
	Const:	UBUFQEFLAG_SYNC
	Const:	UBUFQEFLAG_STALL
	Const:	UBUFQEFLAG_SHORTCOMPLETES
	Const:	UBUFQEFLAG_DEFINITE_LENGTH
	Const:	UBUFQEFLAG_IDLE_COMPLETES
	Const:	UBUFQEFLAG_PROTO_RESERVED
	Const:	UBUFQEFLAG_ISOCH_ASAP
	Const:	UBUFQEFLAG_NO_CACHE
	Type:	struct TTUSB_UBUFQE
	Type:	UBUFQE_IMPLICIT
	Type:	UBUFQE_USER_EXTENSION
	Type:	UBUFQE_STREAM_IN
	Type:	UBUFQE_STREAM_OUT
	Type:	UBUFQE_STREAM_INOUT

Function:
	C type for the USB DataPump Buffer Queue Element

Description:
	|UBUFQE|s are used to represent asynchronous I/O operations within
	the DataPump.  A caller prepares a |UBUFQE| and submits it to the
	transport layer, frequently using |UsbPipeQueue()|.  The transport
	layer enqueues the |UBUFQE| and returns as quickly as possible.

	Later, when the operation on the buffer completes, a call-back
	function is called to indicate that the buffer has completed.

	While the |UBUFQE| is owned by the lower layer, the client must
	not change any contents of the |UBUFQE|.  After the |UBUFQE| is
	handed back to the client, the lower layer must neither read nor
	write the |UBUFQE|.

	The basic UBUFQE and UBUFQE_IMPLICIT types of UBUFQEs are for a
	coding style in which a common form of UBUFQE is used, regardless
	of the destination pipe type. The destination pipe type implicitly
	determines the operation.  More advanced UBUFQE types are used
	for coding style in which the code explcitly indicates the kind
	of operation it expects to perform; the DCD can then check that
	the pipe-type matches the operation requested.

	This implicit style works acceptably for simple operations, but 
	more advanced (and uncommon) isochronous transfers require more 
	information.  See the UBUFQE_* variants in the "See Also" list 
	for more advanced, or more general, structures. See UBUFQE_GENERIC
	for a union that combines all the possibilities.

	UBUFQE_STREAM_IN and UBUFQE_STREAM_OUT are identical to UBUFQE,
	except that they specify bulk or interrupt transfers of the given
	kind. IN means "transmit to host", OUT means "receive from host".
	UBUFQE_STREAM_INOUT is a convenience type, which can be used when
	one wants to refer to a request that is known to be either
	UBUFQE_STREAM_IN or UBUFQE_STREAM_OUT.

	UBUFQE_PACKET_IN and UBUFQE_PACKET_OUT are identical to UBUFQE,
	except that they specify isochronous transfers of the given
	kind. IN means "transmit to host", OUT means "receive from host".
	Both transfer kinds use isochronous headers that are interleaved
	with data in the same buffer. UBUFQE_PACKET_INOUT is a convenience 
	type, which can be used when one wants to refer to a request that is 
	known to be either UBUFQE_PACKET_IN or UBUFQE_PACKET_OUT. These
	transfers (and the equivalent implicit UBUFQE) treat the buffer
	as a sequence of UISOBUFPACKET structures. For both OUT and IN
	transactions, the buffer must be initialized by the client, and
	will be updated as necessary. (Note that this form is not
	DMA-friendly, so this form is deprecated. However, not all DCDs
	have been updated to support the UBUFQE_RQ_ISOCH_IN and
	UBUFQE_RQ_ISOCH_OUT requests. Consult the DCD documentation for 
	more information.)

	All UBUFQE variants share a common header with common field names,
	and the header will have the same memory layout, independent of the
	UBUFQE variant. That header, and the field names, are doucmented here.

Contents:
	|UBUFQE_LENGTH uqe_length;|
		Size of the |UBUFQE|, in bytes.  This 16-bit field was added 
		for V2.0 of the DataPump, in order to allow Isochronous 
		UBUFQEs to supply an extra buffer pointer without burdening 
		all UBUFQEs with the extra memory overhead.  If zero, then 
		|UsbPipeQueue()| will assume that the UBUFQE was prepared by 
		legacy code, and will set this field to sizeof(UBUFQE).  
		|UsbPipeQueueBuffer()| and |UsbPipeQueueBufferWithTimeout()|
		will fill this in appropriately.

	|UBUFQE_REQUEST uqe_request;|
		The kind of operation to be performed by the DataPump for 
		this UBUFQE.  If zero (i.e. if the value is 
		|UBUFQE_RQ_IMPLICIT|), then the kind of operation is 
		determined by the kind of pipe.  

	|USTAT uqe_status;|
		This eight-bit field is used by the transport layer to return 
		completion status.

	|UBUFQE_FLAGS uqe_flags;|
		This 16-bit field provides extra semantic information about 
		the information in the buffer.  The transport updates the 
		flags to return additional semantic information.

	|UBUFQE_TIMEOUT uqe_timeout;|
		If |UBUFQEFLAGS_IDLE_COMPLETES| is set, the client must set
		this to the "inactivity timeout", in milliseconds.  It is
		not changed by the driver.  This is a 16-bit field.

	|UBUFQE *uqe_next, *uqe_last|
		point to the next and previous |UBUFQE| on a circular list.  
		Many |UBUFQE| APIs assume that the |UBUFQE| links are always 
		valid; the |UBUFQE| must always be in a single-element
		list containing itself.  This allows those APIs to receive 
		lists of |UBUFQE|s with a single call and allows (limited)
		support for scatter/gather.

	|UBUFIODONEFN uqe_donefn;|
	|VOID *uqe_doneinfo;|
		Specify the completion function to be called when the
		|UBUFQE| completes.  |uqe_doneinfo| is for use by clients,
		and is not interpreted by transport layers.

	|UPIPE *uqe_pPipe;|
		Normally, specifies the pipe to which this |UBUFQE| is to
		be submitted.

	|VOID *uqe_extension;|
		This field is for the exclusive use of the DCD. Any initial 
		value will be ignored.  Clients may, of course, use this 
		when the |UBUFQE| is not residing with the DCD.

	|UBUFQE_USER_EXTENSION uqe_userExtension;|
		This is a union, reserved for use by the client. It is never
		referenced by the DCD. There are two ways of viewing this
		field.

		|UINT_PTR all;|
			A pointer-size integer.

		|UINT8 flags;|
			Eight bits of flags. This is provided for backwards
			compatibilty.

	|UINT8 uqe_usrflags;|
		This is a |#define|, equivalent to |uqe_userExtension.flags|.
		It's primarily for backwards compatibility. Do not use this
		in new code.

	|VOID *uqe_buf;|
	|CONST VOID *uqe_buf;|
	|USBPUMP_BUFFER_POINTER uqe_buf;|
		All UBUFQE variants have a field named uqe_buf, but the type
		varies depending on the appropriate buffer use. The legacy
		UBUFQE can be used for reading data into a buffer or for
		writing, as determined by the pipe type. It uses |VOID *| as
		the type of uqe_buf, even though this is not ideal for
		IN pipes (where we'd prefer to use |CONST VOID *|.

		The newer forms of UBUFQE use types that follow the intended
		use of the UBUFQE. UBUFEQE_STREAM_IN, UBUFQE_ISOCH_IN,
		UBUFQE_SS_STREAM_IN, and UBUFQE_SQL_STREAM_IN use |CONST
		VOID *uqe_buf|.  UBUFQE_IMPLICIT, UBUFQE_STREAM_INOUT,
		UBUFQE_SS_STREAM_INOUT, and UBUFQE_SGL_STREAM_INOUT use
		|USBPUMP_BUFFER_POINTER uqe_buf|, so uqe_buf can be viewed
		as a read-only buffer using |uqe_buf.pcWriteBuffer|, or as a 
		read/write buffer using |uqe_buf.pReadBuffer|. ("ReadBuffer"
		means a buffer that receives data read from a device; "
		"WriteBuffer" means a buffer that is a pure source of data
		written to a device). All other UBUFQE variants use 
		|VOID *uqe_buf|.

	|USBPUMP_BUFFER_HANDLE uqe_hBuffer;|
		The "buffer handle" for |uqe_buf|. Buffer handles are
		platform-specific; they provide extra, platform-specific
		information. If this field is NULL, the DCD may use it
		as a holder for a temporary buffer handle.  This is useful
		for things like physical memory scatter/gather lists.
		If non-NULL at entry, the DCD will assume that it's set
		to a handle compatible with the platform's DMA buffer-handle
		mechanisms, and will not overwrite it.

	|BYTES uqe_bufsize;|
	|BYTES uqe_bufindex;|
	|BYTES uqe_bufars;|
		Pointer to the data buffer, and size information.
		|uqe_bufsize| states the overall size of the buffer, in bytes,
		as determined by the client; this is never changed by the
		transport layer.

		|uqe_bufindex| is used by the transport layer to track its own
		progress through the buffer.

		|uqe_bufars| will always be set by the transport layer to
		reflect the number of bytes written to or read from the
		buffer.

	|UBUFQE_FLAGS uqe_flags;|
		Flag bits for telling the DCD additional things about the
		operation. The meaning of the flag bits changes depending 
		on whether this |UBUFQE| is used for receiving data into the 
		buffer, or transmitting data out of the buffer.

	The bits following bits are defined in UBUFQE_FLAGS.

	|UBUFQEFLAG_PREBREAK|
		Transmit:  the client sets this bit to request that the
		transport ensure that the contents of this |UBUFQE| are
		not logically combined with any previously queued data.
		This is frequently used on message-oriented streams, when
		the beginning of the message is the first byte in the
		associated buffer.  The transport does not change this bit.

		Receive:  this flag is not used, and is ignored by the
		transport layer.

	|UBUFQEFLAG_POSTBREAK|
		Transmit:  the client sets this bit to request that the
		transport ensure that the contents of this |UBUFQE| are
		not logically combined with any subsequenty queued data.
		Forces a Zero-Length Packet if the message is a multiple
		of wMaxPacketLength, and forces the message to be transmitted.
		This is frequently used on message-oriented streams, when
		the end of the message is the last byte in the
		associated buffer.  The transport does not change this bit.

		Receive:  this flag is changed by the transport prior to
		completing the |UBUFQE|.  It will be set if the
		operation ended with a short packet.  It will be clear if
		the operation ended with a packet that was |wMaxPacketLength|
		bytes long.

	|UBUFQEFLAG_SYNC|
		Transmit:  if appropriate and possible for the given hardware,
		do not complete this |UBUFQE| until all data has been moved
		from the hardware to the host.  Otherwise the |UBUFQE| will
		be completed as soon as the data has been drained from the
		buffer; in this case, the client cannot tell if the host
		has actually received the data.

		Receive:  not used.

	|UBUFQEFLAG_STALL|
		Transmit or receive:  When this |UBUFQE| reaches the head of
		the queue, the endpoint is stalled, and the |UBUFQE| is
		completed.  No more data will be transferred until the
		endpoint is unstalled by host activity.

	|UBUFQEFLAG_SHORTCOMPLETES|
		Transmit:  not used.

		Receive:  if set, and a short packet is received, the |UBUFQE|
		will be completed.  Otherwise, the |UBUFQE| will be held by 
		the transport layer until it is completely filled, or until 
		some other completion condition occurs.  For ISOCH pipes, a
		missing packet will also cause a completion (but not a 
		packet that is present but short).

	|UBUFQEFLAG_DEFINITE_LENGTH|
		Transmit:  not used.

		Receive:  if set, the client wants to receive a message that
		is exactly the length of the bufefer.  If reset, the DataPump
		transport attempts to put the first part in the first
		available |UBUFQE|, and the second part in the subsequent
		|UBUFQE|.

	|UBUFQEFLAG_IDLE_COMPLETES|
		Transmit:  not used.

		Receive:  if set, the transport is to complete the |UBUFQE|
		if (1) the bufer has data in it; (2) the host doesn't send
		any more data for uqe_timeout milliseconds.

		Not all hardware can implement this function efficiently, or
		at all.

	|UBUFQEFLAG_PROTO_RESERVED|
		Transmit: not used
		Receive: not used.

		This flag is reserved for use by DataPump protocols for
		marking UBUFQEs in any way that seems useful.

	|UBUFQEFLAG_ISOCH_ASAP|
		Transmit or Receive:  if set, let the DCD begin the 
		isochronous transfer at the next possible opportunity. 
		The DCD will update IsochStartFrame to reflect the starting
		frame number. If not set, client must set IsochStartFrame 
		and hardware will schedule isochronous transfer at specified 
		frame number.

		This flag is only valid if UBUFQE request is 
		UBUFQE_RQ_ISOCH_IN or UBUFQE_RQ_ISOCH_OUT. Other ISOCH 
		operations (UBUFQE_RQ_PACKET_IN/OUT or legacy operations) 
		are always ASAP.

	|UBUFQEFLAG_NO_CACHE|
		Transmit or Receive:  if set, this flag advises the DCD 
		that the buffer is in non-cached memory. This allows the DCD 
		to avoid cleaning the cache before starting IN-pipe DMA and
		to avoid invalidating the cache before starting OUT-pipe DMA.

See also:
	__TMS_PR3746_DISABLE
	UBUFQE_IMPLICIT, 
	UBUFQE_PACKET_IN, UBUFQE_PACKET_OUT, UBUFQE_PACKET_INOUT,
	UBUFQE_ISOCH_IN, UBUFQE_ISOCH_OUT, UBUFQE_ISOCH_INOUT,
	UBUFQE_SS_STREAM_IN, UBUFQE_SS_STREAM_OUT, UBUFQE_SS_STREAM_INOUT
	UBUFQE_SGL_STREAM_IN, UBUFQE_SGL_STREAM_OUT, UBUFQE_SGL_STREAM_INOUT,
	UBUFQE_GENERIC

Notes:
	Because of history, |UBUFQE| is equivalent to |struct TTUSB_UBUFQE|, 
	rather than |struct __TMS_STRUCTNAME(UBUFQE)|.  This doesn't follow 
	current MCCI coding style.

*/

/**** flags ****/
/*
|| note that PREBREAK and POSTBREAK only apply to data sent to the host;
|| SYNC applies to completions in both directions.
*/

/*
|| When transmitting to the host, the UBUFQE flag UBUFQEFLAG_PREBREAK
|| can be set by the user.  If so, the DataPump will not combine that
|| buffer-full of data with a previous buffer-full to form a packet.
|| Otherwise, for bulk endpoints, the DataPump will operate in a
|| streaming mode, combining buffers of data to form full packets
|| as necessary.
||
|| This flag is not used in UBUFQEs used for receiving data from
|| the host.
*/
#define	__TMS_UBUFQEFLAG_PREBREAK (1 << 0)	/* break stream before */

/*
|| When transmitting data to the host, the caller can set
|| UBUFQEFLAG_POSTBREAK.  The DataPump will ensure that the data in
|| this buffer is not combined with any subsequent buffers; that the
|| last packet formed from this buffer will end after the last byte in
|| the buffer.
||
|| When receiving data from the host, this bit is an output from the
|| DataPump.  The DataPump will set this flag
|| if (1) the end of the data in the buffer represents a packet boundary;
|| (2) the inbound packet was a short packet; and (3) the endpoint is
|| a BULK or CONTROL endpoint.  Otherwise the DataPump will reset this flag.
*/
#define	__TMS_UBUFQEFLAG_POSTBREAK (1 << 1)	/* break stream after */

/*
|| When transmitting data to the host, UBUFQEFLAG_SYNC will cause the buffer
|| completion to be delayed until the data has actually been transferred
|| to the host (not just put into the buffer).  This is only implemented
|| for bulk-ish things.
*/
#define	__TMS_UBUFQEFLAG_SYNC	(1 << 2)	/* synchronous status */

/*
|| To stall an endpoint, create a packet with UBUFQEFLAG_STALL set, and
|| with a zero-length buffer.  When the packet reaches the head of the
|| queue, the endpoint will be stalled.
*/
#define __TMS_UBUFQEFLAG_STALL	(1 << 3)	/* stall request */

/*
|| When receiving a packet from the host, the caller should set this
|| flag if a short packet (for bulk) or missing packet (for iso) is
|| to cause an early completion of the UBUFQE.
*/
#define	__TMS_UBUFQEFLAG_SHORTCOMPLETES (1 << 4) /* short packets complete early */

/*
|| When receiving data from the host, the caller should set this flag
|| if the caller expects exactly a certain amount of data, and any
|| excess in the last packet should cause an error condition.  This
|| flag is often used when receiving setup info from the host.  This
|| normally has effect only on the last packet that goes into the buffer.
*/
#define	__TMS_UBUFQEFLAG_DEFINITE_LENGTH (1 << 5) /* length is definite, and
						  || must not cause a split.
						  */
/*
|| When receiving data from the host, the caller should set this flag
|| if the timeout value is to be used as an idle time.
*/
#define __TMS_UBUFQEFLAG_IDLE_COMPLETES (1 << 6) /* idle time on the bus
						 || completes a
						 || partially-filled packet.
						 || Timeout value is set
						 || in the ubufqe.
						 */

/*
|| Note that VSP uses bit 7.
*/
#define	__TMS_UBUFQEFLAG_PROTO_RESERVED (1 << 7) /* reserved for use by
						 || protocols
						 */

/*
|| Isochronous transfer flag for UBUFQE_RQ_ISOCH_IN and UBUFQE_RQ_ISOCH_OUT.
|| If set, let hardware schedule isochronous transfer and hardware will
|| update IsochStartFrame on completion.  If not set, client should set
|| IsochStartFrame and hardware will schedule isochronous transfer at
|| specified frame number.
*/
#define	__TMS_UBUFQEFLAG_ISOCH_ASAP (1 << 8)

/*
|| If set, let DCD know buffer is non-cache memory, so DCD doesn't need to
|| clean or invalidate the cache.
*/
#define	__TMS_UBUFQEFLAG_NO_CACHE (1 << 9)

/*
|| Internal flags -- we reserve bits 12 to bit 15 for this purpose
|| UBUFQEFLAG_INTERNAL_BUFFER_ENTERSCOPE set when enter scope for buffer.
|| UBUFQEFLAG_INTERNAL_ISOCHDESC_ENTERSCOPE set when enter scope for isoch 
||   desc.
*/
#define	__TMS_UBUFQEFLAG_INTERNAL_BUFFER_ENTERSCOPE	(1 << 12)
#define	__TMS_UBUFQEFLAG_INTERNAL_ISOCHDESC_ENTERSCOPE	(1 << 13)

/*
|| The user-extension was formerly 8 bits; we extended it to ptr-width but
|| for backwards compatibility allow old code to treat it as a byte. 
*/
__TMS_TYPE_DEF_UNION	(UBUFQE_USER_EXTENSION);
union __TMS_UNIONNAME	(UBUFQE_USER_EXTENSION)
	{
	__TMS_UINT8	flags;
	__TMS_UINT_PTR	all;
	};

/* 
|| We have a lot of structures that need identical layouts, so we define a 
|| number of macros and sub-structures.
*/

/*
|| Scatter-gather lists can be for buffers that are IN, OUT, or agnostic
|| Because of the way C works, we need a macro to help out.
*/
#define	__TMS_UBUFQE_SGNODE_BODY(BufType)					\
	BufType		pBuffer;	/* pointer to this buffer segment */	\
	__TMS_BYTES	nBytes		/* size of this buffer segement */

__TMS_TYPE_DEF_STRUCT(UBUFQE_SGNODE_IN);
__TMS_TYPE_DEF_STRUCT(UBUFQE_SGNODE_OUT);
__TMS_TYPE_DEF_STRUCT(UBUFQE_SGNODE_INOUT);

/* an "IN" SGNODE is for an IN pipe, and so the input buffers are const */
struct __TMS_STRUCTNAME(UBUFQE_SGNODE_IN)
	{
	__TMS_UBUFQE_SGNODE_BODY(__TMS_CONST __TMS_VOID *);
	};

/* an "OUT" SGNODE is for an OUT pipe, and so the input buffers are non-CONST */
struct __TMS_STRUCTNAME(UBUFQE_SGNODE_OUT)
	{
	__TMS_UBUFQE_SGNODE_BODY(__TMS_VOID *);
	};

/*
|| an "INOUT" SGNODE is used when you have common code for IN and OUT pipes, 
|| so you're not sure whether you want CONST or non-CONST
*/
struct __TMS_STRUCTNAME(UBUFQE_SGNODE_INOUT)
	{
	__TMS_UBUFQE_SGNODE_BODY(__TMS_USBPUMP_BUFFER_POINTER);
	};

/*
|| the common header for any UBUFQE variant
*/
#define	__TMS_UBUFQE__HDR(BufType)						\
	__TMS_UBUFQE_LENGTH uqe_length;	/* 16-bit length of UBUFQE */		\
					/* user r/w, driver r/o */		\
	__TMS_UBUFQE_REQUEST uqe_request; /* 8-bit UBUFQE request code */	\
					/* user r/w, driver r/o */		\
	__TMS_USTAT	uqe_status;	/* 8-bit completion status */		\
					/* user: r/o; driver: r/w */		\
	__TMS_UBUFQE_FLAGS uqe_flags;	/* 16-bit buffer control */		\
					/* user: r/w; driver: r/w */		\
	__TMS_UBUFQE_TIMEOUT uqe_timeout; /* 16-bit optional timeout, in ms */	\
					/* user: r/w; driver: r/o */		\
	/* end of alignment-controlled header */				\
										\
	__TMS_UBUFQE	*uqe_next;	/* forward link */			\
					/* user: r/w; driver: r/w */		\
	__TMS_UBUFQE	*uqe_last;	/* back link */				\
					/* user: r/w; driver: r/w */		\
	__TMS_UBUFIODONEFN *uqe_donefn;	/* completion fn */			\
					/* user: r/w; driver: r/o */		\
	__TMS_UPIPE	*uqe_pPipe;	/* the pipe we're working on */		\
					/* user: r/w; driver: r/o */		\
	__TMS_VOID	*uqe_doneinfo;	/* completion info */			\
					/* user: r/w; driver: r/o */		\
	__TMS_VOID	*uqe_extension;	/* extension pointer */			\
					/* user: n/a; driver: r/w */		\
	BufType		uqe_buf;	/* associated buffer */			\
					/* user: r/w; driver: r/o */		\
	__TMS_USBPUMP_BUFFER_HANDLE uqe_hBuffer;				\
					/* buffer handle */			\
					/* user: r/w; driver: r/o */		\
	__TMS_UBUFQE_USER_EXTENSION uqe_userExtension;				\
					/* user extension */			\
					/* user: r/w; driver: r/o */		\
	__TMS_BYTES	uqe_bufsize;	/* size of associated buffer */		\
					/* user: r/w; driver: r/o */		\
	__TMS_BYTES	uqe_bufars;	/* actual record size of buf */		\
					/* user: r/o; driver: r/w */		\
	__TMS_BYTES	uqe_bufindex	/* where we are in the buffer */	\
					/* user: r/o; driver: r/w */

/* uqe_usrflags definition for backward compatibility */
#define	uqe_usrflags	uqe_userExtension.flags

/*
|| the legacy queue-element layout.
*/
struct TTUSB_UBUFQE
	{
	/* for legacy reasons, all buffers are non-CONST */
	__TMS_UBUFQE__HDR(__TMS_VOID *);
	};

/*
|| the implicit queue-element layout(s)
*/
__TMS_TYPE_DEF_STRUCT	(UBUFQE_IMPLICIT);

struct __TMS_STRUCTNAME	(UBUFQE_IMPLICIT)
	{
	__TMS_UBUFQE__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	};

/*
|| in case anybody wants to clean up their code, these are all 
|| equivalent to the old UBUFQE, but are more strongly typed, and
|| are more like the host stack's URB. 
*/
__TMS_TYPE_DEF_STRUCT	(UBUFQE_STREAM_IN);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_STREAM_OUT);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_STREAM_INOUT);

/* use this if you know it's an IN */
struct __TMS_STRUCTNAME	(UBUFQE_STREAM_IN)
	{
	__TMS_UBUFQE__HDR(__TMS_CONST __TMS_VOID *);
	};

/* use this if you know it's an OUT (therefore buffer is updated) */
struct __TMS_STRUCTNAME	(UBUFQE_STREAM_OUT)
	{
	__TMS_UBUFQE__HDR(__TMS_VOID *);
	};

/* use this if you've got common code for IN and OUT */
struct __TMS_STRUCTNAME	(UBUFQE_STREAM_INOUT)
	{
	__TMS_UBUFQE__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	};

/*
|| For legacy isoch interleaved packets.  Note that all three are the same,
|| because even in the IN case, the buffer pointer is not 
|| CONST VOID *.
*/
__TMS_TYPE_DEF_STRUCT	(UBUFQE_PACKET_IN);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_PACKET_OUT);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_PACKET_INOUT);

/* use this if you know it's an IN */
struct __TMS_STRUCTNAME	(UBUFQE_PACKET_IN)
	{
	__TMS_UBUFQE__HDR(__TMS_VOID *);
	};

/* use this if you know it's an OUT (therefore buffer is updated) */
struct __TMS_STRUCTNAME	(UBUFQE_PACKET_OUT)
	{
	__TMS_UBUFQE__HDR(__TMS_VOID *);
	};

/* use this if you've got common code for IN and OUT */
struct __TMS_STRUCTNAME	(UBUFQE_PACKET_INOUT)
	{
	__TMS_UBUFQE__HDR(__TMS_VOID *);
	};

/*

Type:	UBUFQE_ISOCH_INOUT

Index:	Type:	UBUFQE_ISOCH_IN
	Type:	UBUFQE_ISOCH_OUT

Function:
	The request packet used for 

Description:
	xxx

Contents:
	xxx

Notes:
	xxx

See Also:
	xxx

*/


/*
|| the DataPump V2 isochronous queue-element layout
*/
__TMS_TYPE_DEF_STRUCT	(UBUFQE_ISOCH_IN);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_ISOCH_OUT);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_ISOCH_INOUT);

#define	__TMS_UBUFQE_ISOCH__HDR(BufType)			\
	/* same as ... */					\
	__TMS_UBUFQE__HDR(BufType);				\
	/* plus... */						\
	__TMS_USBPUMP_ISOCH_PACKET_DESCR *pIsochDescr;		\
	__TMS_USBPUMP_BUFFER_HANDLE       hIsochDescr;		\
	__TMS_BYTES	IsochDescrSize;				\
	__TMS_BYTES	nIsochErrs;				\
	__TMS_UINT32	IsochStartFrame				\

struct __TMS_STRUCTNAME	(UBUFQE_ISOCH_INOUT)
	{
	__TMS_UBUFQE_ISOCH__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	};

struct __TMS_STRUCTNAME	(UBUFQE_ISOCH_IN)
	{
	__TMS_UBUFQE_ISOCH__HDR(__TMS_CONST __TMS_VOID *);
	};

struct __TMS_STRUCTNAME	(UBUFQE_ISOCH_OUT)
	{
	__TMS_UBUFQE_ISOCH__HDR(__TMS_VOID *);
	};

/*
|| For USB 3.0 streams.
*/
__TMS_TYPE_DEF_STRUCT	(UBUFQE_SS_STREAM_IN);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_SS_STREAM_OUT);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_SS_STREAM_INOUT);

#define	__TMS_UBUFQE_SS_STREAM__HDR(BufType)			\
	/* same as ... */					\
	__TMS_UBUFQE__HDR(BufType);				\
	/* plus... */						\
	__TMS_UINT_PTR	DcdReserved;				\
	__TMS_UINT16	StreamID

/* use this if you know it's an IN */
struct __TMS_STRUCTNAME	(UBUFQE_SS_STREAM_IN)
	{
	__TMS_UBUFQE_SS_STREAM__HDR(__TMS_CONST __TMS_VOID *);
	};

/* use this if you know it's an OUT (therefore buffer is updated) */
struct __TMS_STRUCTNAME	(UBUFQE_SS_STREAM_OUT)
	{
	__TMS_UBUFQE_SS_STREAM__HDR(__TMS_VOID *);
	};

/* use this if you've got common code for IN and OUT */
struct __TMS_STRUCTNAME	(UBUFQE_SS_STREAM_INOUT)
	{
	__TMS_UBUFQE_SS_STREAM__HDR(__TMS_USBPUMP_BUFFER_POINTER);
	};

/*
|| for scatter-gather list regular and USB 3.0 streams.
*/
__TMS_TYPE_DEF_STRUCT	(UBUFQE_SGL_STREAM_IN);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_SGL_STREAM_OUT);
__TMS_TYPE_DEF_STRUCT	(UBUFQE_SGL_STREAM_INOUT);

#define	__TMS_UBUFQE_SGL_STREAM__HDR(BufType, SglType)			\
	/* same as ... */						\
	__TMS_UBUFQE__HDR(BufType);					\
	/* plus... */							\
	__TMS_UINT_PTR	DcdReserved;	/* for handling USB3 streams */	\
	/* the scatter/gather list */					\
	SglType		*pvSgNodes;	/* pointer to vector of SGNODEs */ \
	__TMS_UINT16	StreamID;	/* stream ID or			\
					|| zero for non-USB3 stream	\
					*/				\
	__TMS_UINT16	nSgNodes	/* number of elements in the SGL */


/* use this if you know it's an IN */
struct __TMS_STRUCTNAME	(UBUFQE_SGL_STREAM_IN)
	{
	__TMS_UBUFQE_SGL_STREAM__HDR(
		__TMS_CONST __TMS_VOID *,
		__TMS_UBUFQE_SGNODE_IN
		);
	};

/* use this if you know it's an OUT (therefore buffer is updated) */
struct __TMS_STRUCTNAME	(UBUFQE_SGL_STREAM_OUT)
	{
	__TMS_UBUFQE_SGL_STREAM__HDR(__TMS_VOID *, __TMS_UBUFQE_SGNODE_OUT);
	};

/* use this if you've got common code for IN and OUT */
struct __TMS_STRUCTNAME	(UBUFQE_SGL_STREAM_INOUT)
	{
	__TMS_UBUFQE_SGL_STREAM__HDR(
		__TMS_USBPUMP_BUFFER_POINTER,
		__TMS_UBUFQE_SGNODE_INOUT
		);
	};

/*
|| For convenience, we have a generic UBUFQE that can be used for any
|| request.  This is defined for symmetry, but most protocols use
|| the basic UBUFQE instead (both for historical reasons, and to
|| save memory).
*/
__TMS_TYPE_DEF_UNION	(UBUFQE_GENERIC);

union __TMS_UNIONNAME	(UBUFQE_GENERIC)
	{
	/* new names */
	struct TTUSB_UBUFQE		UbufqeLegacy;
	__TMS_UBUFQE_IMPLICIT		UbufqeImplicit;
	__TMS_UBUFQE_STREAM_IN		UbufqeStreamIn;
	__TMS_UBUFQE_STREAM_OUT		UbufqeStreamOut;
	__TMS_UBUFQE_STREAM_INOUT	UbufqeStreamInOut;
	__TMS_UBUFQE_PACKET_IN		UbufqePacketIn;
	__TMS_UBUFQE_PACKET_OUT		UbufqePacketOut;
	__TMS_UBUFQE_PACKET_INOUT	UbufqePacketInOut;
	__TMS_UBUFQE_ISOCH_IN		UbufqeIsochIn;
	__TMS_UBUFQE_ISOCH_OUT		UbufqeIsochOut;
	__TMS_UBUFQE_ISOCH_INOUT	UbufqeIsochInOut;
	__TMS_UBUFQE_SS_STREAM_IN	UbufqeSsStreamIn;
	__TMS_UBUFQE_SS_STREAM_OUT	UbufqeSsStreamOut;
	__TMS_UBUFQE_SS_STREAM_INOUT	UbufqeSsStreamInOut;
	__TMS_UBUFQE_SGL_STREAM_IN	UbufqeSgStreamIn;
	__TMS_UBUFQE_SGL_STREAM_OUT	UbufqeSgStreamOut;
	__TMS_UBUFQE_SGL_STREAM_INOUT	UbufqeSgStreamInOut;
	};

/*

Name:	UBUFQE_TO_GENERIC()

Index:	Name:	CONST_UBUFQE_TO_GENERIC()

Function:
	Convert a UBUFQE pointer to a UBUFQE_GENERIC pointer,
	without using an explicit cast.

Definition:
	UBUFQE_GENERIC * UBUFQE_TO_GENERIC(
		UBUFQE *pQe
		);

	CONST UBUFQE_GENERIC * CONST_UBUFQE_TO_GENERIC(
		CONST UBUFQE *pQe
		);

Description:
	These macros provide a reasonably safe way to convert
	a legacy UBUFQE pointer to a UBUFQE, without using
	an explicit type cast.

	A few light-weight (but not foolproof) checks are
	applied to make sure that pQe [at compile time] really
	points to a UBUFQE.

Returns:
	Pointer to the equivalent union object.

*/

#define	__TMS_UBUFQE_TO_GENERIC(pQe)		\
	__TMS_CONTAINER_OF(pQe, __TMS_UBUFQE_GENERIC, UbufqeLegacy.uqe_length)

#define	__TMS_CONST_UBUFQE_TO_GENERIC(pQe)	\
	__TMS_CONTAINER_OF(pQe, __TMS_CONST __TMS_UBUFQE_GENERIC, UbufqeLegacy.uqe_length)

/*
|| the macro version of the checker -- in-line.  If pQe != NULL and
|| pQe->uqe_length > sizeof(UBUFQE), and the checker fails, then we are
|| guaranteed that we'll call UsbPumpBufferQeI_CheckRequestSize(), which will
|| complete the request if it seems reasonable (safe) to do so.
||
|| DCD code doesn't need to call this, because UsbPipeQueue() always does.
||
|| A non-macro version is also available, see API functions.
*/
#define	UsbPumpBufferQeI_Validate(pDevice, pQe)					\
	((pQe) != __TMS_NULL &&							\
	 ((pQe)->uqe_length != 0 || UsbPumpBufferQeI_FixLegacy(pDevice, pQe)) &&\
	 ((pQe)->uqe_length >= sizeof(struct TTUSB_UBUFQE) &&			\
	  (pQe)->uqe_request < __TMS_UBUFQE_RQ__N &&				\
	  (((1u << (pQe)->uqe_request) & __TMS_UBUFQE_RQMASK__LARGE) == 0 ||	\
	   UsbPumpBufferQeI_CheckRequestSize(pDevice, pQe))) &&			\
	 UsbPumpBufferQeI_HbufferEnterScope(pDevice, pQe)			\
	)

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/


__TMS_BEGIN_DECLS

__TMS_VOID
UsbPumpEndpointI_ActivateWhenComplete(
	__TMS_UDEVICE *,	/* pDevice */
	__TMS_UENDPOINT *	/* pUep */
	);

/* 
|| a non-macro version -- the braces around the function name stop the pre-
|| processor from expanding the name 
*/
__TMS_BOOL
(UsbPumpBufferQeI_Validate)(
	__TMS_UDEVICE *pDevice,
	__TMS_UBUFQE *pQe
	);

__TMS_BOOL
UsbPumpBufferQeI_FixLegacy(
	__TMS_UDEVICE *pDevice,
	__TMS_UBUFQE *pQe
	);

__TMS_BOOL
UsbPumpBufferQeI_CheckRequestSize(
	__TMS_UDEVICE *pDevice,
	__TMS_UBUFQE *pQe
	);

__TMS_BOOL
UsbPumpBufferQeI_HbufferEnterScope(
	__TMS_UDEVICE *pDevice,
	__TMS_UBUFQE *pQe
	);

__TMS_VOID
UsbPumpBufferQeI_HbufferExitScope(
	__TMS_UDEVICE *pDevice,
	__TMS_UBUFQE *pQe
	);

__TMS_END_DECLS

/*

Name:	UBUFQE_COMPLETE()

Function:
	Set status and complete a UBUFQE in a standard way.

Definition:
	VOID UBUFQE_COMPLETE(
		UDEVICE *pDevice,
		UENDPOINT *pEndpoint,
		UBUFQE *pQe,
		USTAT Status
		);

Description:
	The status field of the UBUFQE is set from Status, and the
	UBUFQE is completed by calling the completion function.
	After calling this routine, users should not touch the UBUFQE.

Returns:
	No explicit result.

See also:
	__TMS_PR3746_DISABLE

*/

/**** completion processing macro ****/
#define __TMS_UBUFQE_COMPLETE(pDevice, pEndpoint, pQe, stat)		\
    do	{								\
	(pQe)->uqe_status = (stat);					\
	UsbPumpBufferQeI_HbufferExitScope(pDevice, pQe);		\
	UsbPumpEndpointI_ActivateWhenComplete(				\
		(pDevice), (pEndpoint));				\
	if ((pQe)->uqe_donefn)						\
		(*(pQe)->uqe_donefn)((pDevice), (pEndpoint), (pQe));	\
	} while (0)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/*

Macro:	__TMS_PR3746_DISABLE

Function:
	Restore old compile-time behavior of usbkern/i/ubufqe.h

Description:
	Due to a bug (reported in PR3746), cloaking was once not
	handled properly for the following macros:

		UBUFQE_COMPLETE()
		UBUFQEFLAG_PREBREAK
		UBUFQEFLAG_POSTBREAK
		UBUFQEFLAG_SYNC
		UBUFQEFLAG_STALL
		UBUFQEFLAG_SHORTCOMPLETES
		UBUFQEFLAG_DEFINITE_LENGTH
		UBUFQEFLAG_IDLE_COMPLETES
		UBUFQEFLAG_PROTO_RESERVED

	Although this has been corrected, it is possible that old customer
	code has cloaking enabled but uses the uncloaked names (because
	prior to the fix for PR3746, there was no other option).

	Rather than require that customers who have this problem change
	their code, the uncloaking for the above symbols is further 
	controlled by the compile-time parameter __TMS_PR3746_DISABLE.
	
	If this parameter is not defined, then i/ubufqe.h will properly
	handle cloaking for the above symbols.  If this parameter is
	defined to a non-zero value, then i/ubufqe.h will always define
	both the uncloaked and the cloaked names (but only for the above-
	mentioned symbols -- other symbols are not affected and will be
	uncloaked in the normal way.

*/

/* be careful to keep support for __TMS_PR3746_DISABLE! */
#ifndef __TMS_PR3746_DISABLE	/* PARAM */
# define __TMS_PR3746_DISABLE 0	/* if not overridden, control using cloaking */
#endif /* __TMS_PR3746_DISABLE */

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY || __TMS_PR3746_DISABLE
# define UBUFQE_RQ_ZERO	\
   __TMS_UBUFQE_RQ_ZERO
# define UBUFQE_RQ_LEGACY	\
   __TMS_UBUFQE_RQ_LEGACY
# define UBUFQE_RQ_IMPLICIT	\
   __TMS_UBUFQE_RQ_IMPLICIT
# define UBUFQE_RQ_STREAM_IN	\
   __TMS_UBUFQE_RQ_STREAM_IN
# define UBUFQE_RQ_STREAM_OUT	\
   __TMS_UBUFQE_RQ_STREAM_OUT
# define UBUFQE_RQ_PACKET_IN	\
   __TMS_UBUFQE_RQ_PACKET_IN
# define UBUFQE_RQ_PACKET_OUT	\
   __TMS_UBUFQE_RQ_PACKET_OUT
# define UBUFQE_RQ_ISOCH_IN	\
   __TMS_UBUFQE_RQ_ISOCH_IN
# define UBUFQE_RQ_ISOCH_OUT	\
   __TMS_UBUFQE_RQ_ISOCH_OUT
# define UBUFQE_RQ_SS_STREAM_IN	\
   __TMS_UBUFQE_RQ_SS_STREAM_IN
# define UBUFQE_RQ_SS_STREAM_OUT	\
   __TMS_UBUFQE_RQ_SS_STREAM_OUT
# define UBUFQE_RQ_SGL_STREAM_IN	\
   __TMS_UBUFQE_RQ_SGL_STREAM_IN
# define UBUFQE_RQ_SGL_STREAM_OUT	\
   __TMS_UBUFQE_RQ_SGL_STREAM_OUT
# define UBUFQE_RQ__N	\
   __TMS_UBUFQE_RQ__N
# define UBFUQE_RQ_SIZE__INIT	\
   __TMS_UBFUQE_RQ_SIZE__INIT
# define UBUFQE_RQ_NAME__INIT	\
   __TMS_UBUFQE_RQ_NAME__INIT
# define UBUFQE_RQMASK__LARGE	\
   __TMS_UBUFQE_RQMASK__LARGE
# define UBUFQE_SGNODE_BODY(BufType)	\
   __TMS_UBUFQE_SGNODE_BODY(BufType)
# define UBUFQE__HDR(BufType)	\
   __TMS_UBUFQE__HDR(BufType)
# define UBUFQE_ISOCH__HDR(BufType)	\
   __TMS_UBUFQE_ISOCH__HDR(BufType)
# define UBUFQE_SS_STREAM__HDR(BufType)	\
   __TMS_UBUFQE_SS_STREAM__HDR(BufType)
# define UBUFQE_SGL_STREAM__HDR(BufType, SglType)	\
   __TMS_UBUFQE_SGL_STREAM__HDR(BufType, SglType)
# define UBUFQE_TO_GENERIC(pQe)	\
   __TMS_UBUFQE_TO_GENERIC(pQe)
# define CONST_UBUFQE_TO_GENERIC(pQe)	\
   __TMS_CONST_UBUFQE_TO_GENERIC(pQe)
# define UBUFQE_COMPLETE(pDevice, pEndpoint, pQe, stat)	\
   __TMS_UBUFQE_COMPLETE(pDevice, pEndpoint, pQe, stat)
# define UBUFQEFLAG_PREBREAK	\
   __TMS_UBUFQEFLAG_PREBREAK
# define UBUFQEFLAG_POSTBREAK	\
   __TMS_UBUFQEFLAG_POSTBREAK
# define UBUFQEFLAG_SYNC	\
   __TMS_UBUFQEFLAG_SYNC
# define UBUFQEFLAG_STALL	\
   __TMS_UBUFQEFLAG_STALL
# define UBUFQEFLAG_SHORTCOMPLETES	\
   __TMS_UBUFQEFLAG_SHORTCOMPLETES
# define UBUFQEFLAG_DEFINITE_LENGTH	\
   __TMS_UBUFQEFLAG_DEFINITE_LENGTH
# define UBUFQEFLAG_IDLE_COMPLETES	\
   __TMS_UBUFQEFLAG_IDLE_COMPLETES
# define UBUFQEFLAG_PROTO_RESERVED	\
   __TMS_UBUFQEFLAG_PROTO_RESERVED
# define UBUFQEFLAG_ISOCH_ASAP	\
   __TMS_UBUFQEFLAG_ISOCH_ASAP
# define UBUFQEFLAG_NO_CACHE	\
   __TMS_UBUFQEFLAG_NO_CACHE
# define UBUFQEFLAG_INTERNAL_BUFFER_ENTERSCOPE	\
   __TMS_UBUFQEFLAG_INTERNAL_BUFFER_ENTERSCOPE
# define UBUFQEFLAG_INTERNAL_ISOCHDESC_ENTERSCOPE	\
   __TMS_UBUFQEFLAG_INTERNAL_ISOCHDESC_ENTERSCOPE
# define PR3746_DISABLE	\
   __TMS_PR3746_DISABLE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of ubufqe.h ****/
#endif /* _UBUFQE_H_ */
