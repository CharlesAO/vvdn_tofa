/* ubufqeapi.h	Mon Jun  1 2015 00:18:54 tmm */

/*

Module:  ubufqeapi.h

Function:
	API functions and macros for manipulating UBUFQEs.

Version:
	V3.15b	Mon Jun  1 2015 00:18:54 tmm	Edit level 6

Copyright notice:
	This file copyright (C) 2002, 2004, 2008, 2012, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Mon Jul  8 2002 14:17:59  tmm
	Module created.

   1.87b  2004-03-10  tmm
	Fix doc errors.

   1.97k  Thu Jan 31 2008 14:08:43  tmm
	3735:  as part of the general UBUFQE reorganization, add _V2
	macro that lets caller set UBUFQE size and request code 
	explicitly.  Improve documents, and switch to automatic 
	uncloaking.

   3.01f  Mon Feb 06 2012 13:56:23  chwon
	14799: Added _V3 macro that lets caller set buffer handle.

   3.15b  Mon Jun  1 2015 00:18:54  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _UBUFQEAPI_H_		/* prevent multiple includes */
#define _UBUFQEAPI_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _UBUFQE_H_
# include "ubufqe.h"
#endif

/*

Name:	UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V1()

Function:
	Perform run-time initialization of all required fields for a UBUFQE
	that will be transferred as part of a chain.

Definition:
	VOID UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V1(
			PUDEVICE pDevice,
			PUBUFQE pQe,
			VOID *pBuf,
			BYTES bufsize,
			UBUFIODONEFN *pDoneFn,
			VOID *pDoneInfo,
			UBUFQE_FLAGS Flags,
			UBUFQE_TIMEOUT Timeout OPTIONAL
			);

Description:
	This macro performs in-line setup of a UBUFQE.  It is obsolete
	and has been replaced by the equivalent 
	UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2() API.

	Notice that this operation is intended for use in transferring data
	to a UDEVICE, and so this is intended for use with functions (not
	host-side drivers).

	Notice also that the pipe is not set.  This function is intended
	for use with APIs which will set the pipe (such as the UDATASTREAM
	abstractions).

Returns:
	No explicit result.

See also:
	UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2()
	UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2()

Notes:
	This routine sets up only a single |UBUFQE| in the chain.  To
	set up each entry in the chain, it's necessary to iterate over
	the entire chain.

	The argument order does not comply with current MCCI coding
	conventions; this is a method of UBUFQE, so pQe should be
	first.  New code should use the _V2 macro.

*/

#define __TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V1(		\
		pDevice,					\
		pQe,						\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)						\
	__TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2(		\
		pQe,						\
		sizeof(*(pQe)),					\
		__TMS_UBUFQE_RQ_IMPLICIT,			\
		pDevice,					\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)

/*

Name:	UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2()

Function:
	Perform run-time initialization of all required fields for a UBUFQE
	that will be transferred as part of a chain.

Definition:
	VOID UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2(
			UBUFQE *pQe,
			UBUFQE_LENGTH sizeUbufqe,
			UBUFQE_REQUEST ubufqeRequest,
			UDEVICE *pDevice,
			VOID *pBuf,
			BYTES bufsize,
			UBUFIODONEFN *pDoneFn,
			VOID *pDoneInfo,
			UBUFQE_FLAGS Flags,
			UBUFQE_TIMEOUT Timeout OPTIONAL
			);

Description:
	This macro performs in-line setup of a UBUFQE.  It's called V2, so
	that future enhanced versions of the DataPump can add parameters which
	will be upwards compatible (at the source layer).

	Notice that this operation is intended for use in transferring data
	to a UDEVICE, and so this is intended for use with functions (not
	host-side drivers).

	Notice also that the pipe is not set.  This function is intended
	for use with APIs which will set the pipe (such as the UDATASTREAM
	abstractions).

Returns:
	No explicit result.

See also:
	UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2()

Notes:
	This routine sets up only a single |UBUFQE| in the chain.  To
	set up each entry in the chain, it's necessary to iterate over
	the entire chain.

	The argument order is different than the _V1 macro, to comply
	with current MCCI conventions: this is a method of the UBUFQE.

*/

#define __TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)						\
	__TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		/* V3: hBuffer */ __TMS_NULL,			\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)

/*

Name:	UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3()

Function:
	Perform run-time initialization of all required fields for a UBUFQE
	that will be transferred as part of a chain.

Definition:
	VOID UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3(
		UBUFQE *pQe,
		UBUFQE_LENGTH sizeUbufqe,
		UBUFQE_REQUEST ubufqeRequest,
		UDEVICE *pDevice,
		VOID *pBuf,
		USBPUMP_BUFFER_HANDLE hBuffer,
		BYTES bufsize,
		UBUFIODONEFN *pDoneFn,
		VOID *pDoneInfo,
		UBUFQE_FLAGS Flags,
		UBUFQE_TIMEOUT Timeout OPTIONAL
		);

Description:
	This macro performs in-line setup of a UBUFQE.  It's called V3, so
	that future enhanced versions of the DataPump can add parameters which
	will be upwards compatible (at the source layer).

	Notice that this operation is intended for use in transferring data
	to a UDEVICE, and so this is intended for use with functions (not
	host-side drivers).

	Notice also that the pipe is not set.  This function is intended
	for use with APIs which will set the pipe (such as the UDATASTREAM
	abstractions).

Returns:
	No explicit result.

See also:
	UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V3()

Notes:
	This routine sets up only a single |UBUFQE| in the chain.  To
	set up each entry in the chain, it's necessary to iterate over
	the entire chain.

	The argument order is different than the _V2 macro, to comply
	with current MCCI conventions: this is a method of the UBUFQE.

*/

#define __TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		hBuffer,					\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)						\
   do	{							\
	(pQe)->uqe_length = (sizeUbufqe);			\
	(pQe)->uqe_request = (ubufqeRequest);			\
	(pQe)->uqe_donefn = (pDoneFn);				\
	(pQe)->uqe_doneinfo = (pDoneInfo);			\
	(pQe)->uqe_buf = (pBuf);				\
	(pQe)->uqe_hBuffer = (hBuffer);				\
	(pQe)->uqe_bufsize = (bufsize);				\
	(pQe)->uqe_flags = (Flags);				\
	(pQe)->uqe_timeout = (Timeout);				\
	} while (0)

/*

Name:	UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V1()

Function:
	Perform run-time initialization of all required fields for a UBUFQE
	that will be transferred as a single unit.

Definition:
	VOID UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V1(
			PUDEVICE pDevice,
			PUBUFQE pQe,
			VOID *pBuf,
			BYTES bufsize,
			UBUFIODONEFN *pDoneFn,
			VOID *pDoneInfo,
			UBUFQE_FLAGS Flags,
			UBUFQE_TIMEOUT Timeout OPTIONAL
			);

Description:
	This macro performs in-line setup of a UBUFQE.  It is obsolete
	and has been replaced by the equivalent 
	UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2() API.

	It differs from UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2() in that
	the uqe_next and uqe_last field of the UBUFQE are initialized
	so that the UBUFQE is a properly-formed single element list.

	Notice that this operation is intended for use in transferring data
	to a UDEVICE, and so this is intended for use with functions (not
	host-side drivers).

	Notice also that the pipe is not set.  This function is intended
	for use with APIs which will set the pipe (such as the UDATASTREAM
	abstractions).

Returns:
	No explicit result.

See also:
	UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V1()

Notes:
	The argument order does not comply with current MCCI coding
	conventions; this is a method of UBUFQE, so pQe should be
	first.  New code should use the _V2 macro.

*/

#define __TMS_UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V1(		\
		pDevice,					\
		pQe,						\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)						\
	 __TMS_UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2(		\
		pQe,						\
		sizeof(*(pQe)),					\
		__TMS_UBUFQE_RQ_IMPLICIT,			\
		pDevice,					\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)

/*

Name:	UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2()

Function:
	Perform run-time initialization of all required fields for a UBUFQE
	that will be transferred as a single unit.

Definition:
	VOID UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2(
		UBUFQE *pQe,
		UBUFQE_LENGTH sizeUbufqe,
		UBUFQE_REQUEST ubufqeRequest,
		UDEVICE *pDevice,
		VOID *pBuf,
		BYTES bufsize,
		UBUFIODONEFN *pDoneFn,
		VOID *pDoneInfo,
		UBUFQE_FLAGS Flags,
		UBUFQE_TIMEOUT Timeout OPTIONAL
		);

Description:
	This macro performs in-line setup of a UBUFQE.  It's called V2, so
	that future enhanced versions of the DataPump can add parameters which
	will be upwards compatible (at the source layer).

	It differs from UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2() in that
	the uqe_next and uqe_last field of the UBUFQE are initialized
	so that the UBUFQE is a properly-formed single element list.

	Notice that this operation is intended for use in transferring data
	to a UDEVICE, and so this is intended for use with functions (not
	host-side drivers).

	Notice also that the pipe is not set.  This function is intended
	for use with APIs which will set the pipe (such as the UDATASTREAM
	abstractions).

Returns:
	No explicit result.

See also:
	UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2()

Notes:
	The argument order is different than the _V1 macro, to comply
	with current MCCI conventions: this is a method of the UBUFQE.

*/

#define __TMS_UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)						\
	__TMS_UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		/* V3: hBuffer */ __TMS_NULL,			\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)

/*

Name:	UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V3()

Function:
	Perform run-time initialization of all required fields for a UBUFQE
	that will be transferred as a single unit.

Definition:
	VOID UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V3(
		UBUFQE *pQe,
		UBUFQE_LENGTH sizeUbufqe,
		UBUFQE_REQUEST ubufqeRequest,
		UDEVICE *pDevice,
		VOID *pBuf,
		USBPUMP_BUFFER_HANDLE hBuffer,
		BYTES bufsize,
		UBUFIODONEFN *pDoneFn,
		VOID *pDoneInfo,
		UBUFQE_FLAGS Flags,
		UBUFQE_TIMEOUT Timeout OPTIONAL
		);

Description:
	This macro performs in-line setup of a UBUFQE.  It's called V3, so
	that future enhanced versions of the DataPump can add parameters which
	will be upwards compatible (at the source layer).

	It differs from UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3() in that
	the uqe_next and uqe_last field of the UBUFQE are initialized
	so that the UBUFQE is a properly-formed single element list.

	Notice that this operation is intended for use in transferring data
	to a UDEVICE, and so this is intended for use with functions (not
	host-side drivers).

	Notice also that the pipe is not set.  This function is intended
	for use with APIs which will set the pipe (such as the UDATASTREAM
	abstractions).

Returns:
	No explicit result.

See also:
	UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3()

Notes:
	The argument order is different than the _V2 macro, to comply
	with current MCCI conventions: this is a method of the UBUFQE.

*/

#define __TMS_UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		hBuffer,					\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)						\
   do	{							\
	/* init the list threads, just in case */		\
	(pQe)->uqe_next = (pQe);				\
	(pQe)->uqe_last = (pQe);				\
								\
	/* then use the base API */				\
	__TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		hBuffer,					\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		);						\
	} while (0)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UBUFQE *
UsbPumpQe_AllocateQeBuffers(
	__TMS_UDEVICE *pDevice,
	__TMS_UINT nQe,
	__TMS_BYTES BufSize,
	__TMS_BOOL fIsDevBuf
	);

__TMS_VOID
UsbPumpQe_FreeQeBuffers(
	__TMS_UDEVICE *pDevice,
	__TMS_UBUFQE *ListHead,
	__TMS_BYTES BufSize,
	__TMS_BOOL fIsDevBuf
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V1(		\
		pDevice,					\
		pQe,						\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)	\
	__TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V1(		\
		pDevice,					\
		pQe,						\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)
# define UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)	\
	__TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V2(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)
# define UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		hBuffer,					\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)	\
	__TMS_UBUFQE_DEVICE_CHAIN_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		hBuffer,					\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)
# define UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V1(		\
		pDevice,					\
		pQe,						\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)	\
	__TMS_UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V1(		\
		pDevice,					\
		pQe,						\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)
# define UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)	\
	__TMS_UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V2(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)
# define UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		hBuffer,					\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)	\
	__TMS_UBUFQE_DEVICE_SINGLE_TRANSFER_SETUP_V3(		\
		pQe,						\
		sizeUbufqe,					\
		ubufqeRequest,					\
		pDevice,					\
		pBuf,						\
		hBuffer,					\
		bufsize,					\
		pDoneFn,					\
		pDoneInfo,					\
		Flags,						\
		Timeout						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of ubufqeapi.h ****/
#endif /* _UBUFQEAPI_H_ */
