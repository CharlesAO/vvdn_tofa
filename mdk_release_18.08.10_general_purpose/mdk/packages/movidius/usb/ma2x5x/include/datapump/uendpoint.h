/* uendpoint.h	Sat Mar 14 2015 18:24:00 tmm */

/*

Module:  uendpoint.h

Function:
	Home for UENDPOINT

Version:
	V3.15b	Sat Mar 14 2015 18:24:00 tmm	Edit level 13

Copyright notice:
	This file copyright (C) 2002-2008, 2010-2011, 2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 23:01:26  tmm
	Module created.

   1.79b  Mon Sep 23 2002 17:30:00  maw
   	UENDPOINT_COUNT_PENDING_BYTES corrected. Returned an invalid pLastQe 
	value.

   1.79b  Mon Nov 18 2002 15:30:00 maw
	Added cloaking to UENDPOINT_HDR

   1.79b  12/3/2002  chwon
	Get rid of uninitialized variable warning in 
	UENDPOINT_COUNT_PENDING_BYTES.

   1.91c  Tue May 24 2005 12:32:21  chwon
	Add UsbEndpointLockIo() & UsbEndpointUnlockIo()

   1.97k  Tue Apr 22 2008 11:37:19  chwon
	3735: reorganize UBUFQE to allow 32-bit sizes -- change input 
	parameter from USHORT to BYTES. Generate uncloaked name using 
	uncloak-def.sh.
	3746:  fix name cloaking for UBUFQEFLAG_xxx
	4040: use __TMS_NULL instead of NULL.

   3.01c  Sun Aug 29 2010 02:44:04  cccho
	10109: Add UENDPOINTSWITCH_INIT_V4 and UENDPOINTSWITCH_QUERYSTATUS_FN
	function prototype.

   3.01c  Wed Sep 01 2010 17:51:38  chwon
	11117: increase uephh_Size field from 8 bits to 10 bits and change
	uephh_siolock and uephh_ucTimeoutFrames to bit field.

   3.01d  Mon Jan 03 2011 12:31:23  cccho
	12155: add UENDPOINT_AUTO_REMOTE_WAKEUP_OK()

   3.01f  Fri Jul 01 2011 14:02:15  maw
   	13233: add endpoint activity flag uep_fActive, 
   	uep_fActivateWhenComplete.

   3.11c  Mon Feb 18 2013 15:54:49  chwon
	16743: Add UENDPOINT_CAN_AUTO_REMOTE_WAKEUP() and
	UENDPOINT_CHECK_AUTO_REMOTE_WAKEUP() macro.

   3.15b  Thu Mar 12 2015 20:56:27  tmm
	19042: As first step for improving naming, add new-style names
	UENDPOINTSWITCH_STARTIO_FN (USTARTIOFN), UENDPOINTSWITCH_CANCELIO_FN
	(UCANCELIOFN), UENDPOINTSWITCH_PREPIO_FN (UPREPIOFN),
	UENDPOINTSWITCH_EVENT_FN (UEPEVENTFN), UENDPOINTSWITCH_TIMEOUT_FN
	(UTIMEOUTFN).

	19019: Document things much more completely.

	19051: Add prototype for UsbPumpEndpointSwitch_PrepIoStub.

	Fix some misspellings of "Endpoint", "Swtich" etc in macro params.  

    3.15b  Sat Mar 14 2015 18:24:00  tmm
	19050: Add UsbPumpEndpointSwitch_QueryStatusStub(), and only publish
	prototype for UsbPumpEndpointSwitch_QeuryStatusStub if 
	__TMS_DATAPUMP_COMPAT_V3_14 is TRUE.  The code with the incorrect 
	name remains in the library.

*/

#ifndef _UENDPOINT_H_		/* prevent multiple includes */
#define _UENDPOINT_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPPARAMS_H_
# include "usbpumpparams.h"
#endif

#ifndef _UDEVICE_H_
# include "udevice.h"
#endif

#ifndef _UPIPE_H_
# include "upipe.h"
#endif

/****************************************************************************\
|
|	The standard endpoint structure -- this is used to manage I/O
|	and so must be a base type that the hardware level can derive
|	from.  
|
\****************************************************************************/

/*

Type:	UENDPOINT

Index:	Name:	UENDPOINT_HDR
	Name:	UsbGenericEndpointInit()
	Name:	UENDPOINT_SIZE()
	Name:	UENDPOINT_INDEX()
	Name:	UENDPOINT_NEXT()
	Name:	UsbEndpointLockIo()
	Name:	UsbEndpointUnlockIo()

Function:
	The abstract type representing any hardware endpoint to the DataPump.

Definition:
	#include "uendpoint.h"

	typedef struct TTUSB_ENDPOINT UENDPOINT, *PUENDPOINT;

	#define UENDPOINT_HDR ...

	typedef struct UENDPOINT_{derived} 
		{
		UENDPOINT_HDR;

		// additional specific contents for this derived type.
		} UENDPOINT_{derived};

	VOID UsbGenericEndpointInit(
		UENDPOINT_{derived} *pEndpoint
		);

	UINT UENDPOINT_SIZE(
		UENDPOINT *pEndpoint
		);

	// not really a function: careful!
	VOID UENDPOINT_INDEX(
		lv UENDPOINT *pResult,
		UENDPOINT *pBaseEndpoint,
		int iEndpointIndex
		);

	UENDPOINT *UENDPOINT_NEXT(
		UENDPOINT *pThisEndpoint
		);

	VOID UsbEndpointLockIo(
		UENDPOINT *pEndpoint
		);

	VOID UsbEndpointUnlockIo(
		UENDPOINT *pEndpoint
		);

Description:
	Hardware endpoints are modeled to the portable software by structures
	that are derived from the UENDPOINT base type.

	See below for a description of contents.

	UsbGenericEndpointInit() is a macro used by DCD-specific code to 
	initialize the portable part of the UENDPOINT. It must be used with
	a pointer derived from UENDPOINT, such that sizeof(*pEndpoint)
	matches the size of the concrete instance in memory.  UHIL_fill()
	*must* be called prior to calling this macro to ensure that the
	UENDPOINT memory is all zero.

	UENDPOINT_SIZE() returns the concrete size of the derived endpoint
	indicated by pEndpoint.
	
	UENDPOINT_INDEX() and UENDPOINT_NEXT() are used for traversing a vector
	of UENDPOINTs.  UENDPOINT_NEXT() returns the successor UENDPOINT of a 
	given endpoint. UENDPOINT_INDEX() in effect calles UENDPOINT_NEXT() 
	zero or more times to find the iEndpointIndex'th succssor.

	UsbEndpointLockIo() and UsbEndpointUnlockIo() manipulate the
	start-I/O lock for the endpoint. Calls to UsbEndpointLockIo() and
	UsbEndpointUnlockIo() nest. UsbEndpointUnlockIo() doesn't automatically
	invoke USTARTIO(pEndpoint) if operations were bypassed while the
	endpoint was locked.

Contents:
	UBUFQE *uep_pPending;
		Pointer to head of list of UBUFQEs that are waiting to be
		processed. Portable software is free to remove a UBUFQE
		from this list without informing the DCD. (Therefore, the
		DCD will normally remove a UBUFQE from this list while it
		is is processing the UBUFQE.)

	UPIPE *uep_pPipe;
		If the endpoint is configured, uep_pPipe points to the UPIPE
		(and therefore indirectly to the UINTERFACE, UCONFIG, etc.)
		that governs how the endpoint is configured. The DataPump
		allows for radical reuse of hardware based on configuration
		changes, and therefore this pointer tracks the currently
		configured state.

		If the endpoint is not currently configured, uep_pPipe is
		NULL.

	VOID *uep_pExtension;
		The application extension pointer. Not used by the DCD or
		by the portable DataPump device framework.

	CONST UENDPOINTSWITCH *uep_switch;
		The endpoint methods for this endpoint. This is updated
		based on the current configuration of the endpoint. Even
		if the endpoint is not currently configured, this will
		point to a table of valid function pointers.

	UINT uep_stall: 1;
		A flag; the endpoint is currently halted if TRUE.

	UNIT uep_fChanged: 1;
		A flag. The state of the endpoint has changed at the
		portable level, and so the driver needs to communicate the
		change to the hardware.

	UINT uep_fActive: 1;
		Used in LPM and auto-wakeup management, especially for IN
		pipes. Set active by portable code and device drivers when
		it's know that the host driver is polling the endpoint;
		set inactive after a SET_CONFIG, SET_INTERFACE, etc.
		involving the endpoint (until we can prove that the
		endpoint is being polled by a host driver).  When this
		flag is clear, local traffic queued for the endpoint 
		does not get a STARTIO.  See UDP-13233 for more details.

	UINT uep_fActivateWhenComplete: 1;
		Used in LPM and auto-wakeup management. Any completion
		of a UBUFQE due to traffic on this endpoint is taken as
		evidence that a class driver is talking to the device, and
		so all endpoints in this function should be activated.

	UINT uep_Size: 10;
		Size in bytes of endpoint structure, including the device-
		dependent section. A UENDPOINT is normally just the header
		of a larger device-dpendent structure; this field gives
		the overall size.
		
	UINT uep_siolock: 8;
		The interlock for preventing start-I/O reentrancy. When
		this is non-zero, a start-I/O targeting this endpoint has
		no effect.

	UINT uep_ucTimeoutFrames: 8;
		The counter (OUT pipes only) for activating a bulk or int
		out endpoint after receipt of partial data. The timer starts
		downcounting once some data is received, and it counts
		once per millisecond. If it reaches zero before the buffer
		fills or a short packet is received, the operation is
		completed anyway.

Notes:
	UENDPOINT is the innermost of a two-layer Russian doll of data
	structures. For historical reasons, the implementation doesn't
	follow the full pattern.

	There's no back-pointer from the UENDPOINT to the containing UDEVICE.

	UsbGenericEndpointInit() assumes that the data structure has been
	pre-zeroed by the caller. 

	The iEndpointIndex argument to UENDPOINT_INDEX() is signed, but
	it's not possible to use negative indices.

	The pEndpoint argument to UsbGenericEndpointInit() must be of the
	final (fully derived) type of the underlying endpoint. If an argument
	of type UENDPOINT is passed, the field uep_Size will silently be
	set to sizeof(UENDPOINT), which is probably not correct.

	UsbEndpointUnlockIo() doesn't automatically invoke USTARTIO(pEndpoint)
	if operations were bypassed while the endpoint was locked.

See Also:
	UDEVICE

*/

/**** the contents of the embedded header ****/
struct TTUSB_UENDPOINT_HDR
	{
	__TMS_UBUFQE *uephh_pending;	/* queue elements being filled */
	__TMS_UPIPE *uephh_pPipe;	/* pointer to the pipe we work for;
					|| if NULL, then not tied to a pipe,
					|| so this endpoint cannot do I/O.
					|| All of the information about the
					|| current endpoint configuration
					|| is held in the UPIPE.
					*/
	__TMS_VOID *uephh_pExtension;	/* application extension */
	__TMS_CONST __TMS_UENDPOINTSWITCH *uephh_pSwitch;	
					/* pointer to the endpoint switch
					|| struct, which allows dispatch
					|| based on device/endpoint type.
					*/
	__TMS_UINT uephh_stall: 1;	/* stalled if TRUE */
	__TMS_UINT uephh_fChanged: 1;	/* configuration of ep changed */
	__TMS_UINT uephh_fActive: 1;	/* ep is active if TRUE */
	__TMS_UINT uephh_fActivateWhenComplete: 1;
					/* if TRUE, activate endpoints in the
					|| same interface when UBUFQE is
					|| completed
					*/
	__TMS_UINT uephh_Size: 10;	/* size of this endpoint struct */
	__TMS_UINT uephh_siolock: 8;	/* start-I/O lock-out count */
	__TMS_UINT uephh_ucTimeoutFrames: 8; /* timeout down-counter */
	};

/**** the macro to embed in derived structures ****/
#define	__TMS_UENDPOINT_HDR	struct TTUSB_UENDPOINT_HDR uep_hh


/**** the generic structure ****/
struct TTUSB_UENDPOINT
	{
	__TMS_UENDPOINT_HDR;
	};

/**** sane names ****/
#define	uep_pending			uep_hh.uephh_pending
#define	uep_Size			uep_hh.uephh_Size
#define	uep_pSwitch			uep_hh.uephh_pSwitch
#define	uep_pPipe			uep_hh.uephh_pPipe
#define	uep_pExtension			uep_hh.uephh_pExtension
#define	uep_stall			uep_hh.uephh_stall
#define uep_fChanged			uep_hh.uephh_fChanged
#define uep_fActive			uep_hh.uephh_fActive
#define uep_fActivateWhenComplete	uep_hh.uephh_fActivateWhenComplete
#define	uep_siolock			uep_hh.uephh_siolock
#define	uep_ucTimeoutFrames		uep_hh.uephh_ucTimeoutFrames

/**** the initializer ****/
#define	UsbGenericEndpointInit(pep)		\
    do	{					\
    	(pep)->uep_pending = __TMS_NULL;	\
	(pep)->uep_pPipe = __TMS_NULL;		\
	(pep)->uep_Size = sizeof(*(pep));	\
	(pep)->uep_fChanged = __TMS_TRUE;	\
	(pep)->uep_fActive = __TMS_TRUE;	\
	(pep)->uep_fActivateWhenComplete = __TMS_FALSE;	\
	} while (0)

/**** start I/O lock handling ****/
#define UsbEndpointLockIo(pEndpoint)		\
	do { ++((pEndpoint)->uep_siolock); } while (0)

#define UsbEndpointUnlockIo(pEndpoint)		\
	do { --((pEndpoint)->uep_siolock); } while (0)

/**** iterators ****/
#define	UENDPOINT_SIZE(p)	((p)->uep_Size)
#define	UENDPOINT_INDEX(newp, p, i)	\
    do	{ \
	int __i = (i); \
	(newp) = (__TMS_UENDPOINT *) (p); \
	for (; __i > 0; --__i) \
		(newp) = (__TMS_UENDPOINT *) \
			((char *)(newp) + UENDPOINT_SIZE(newp)); \
	} while (0)
#define	UENDPOINT_NEXT(p) \
	((__TMS_UENDPOINT *)((char *)(p) + UENDPOINT_SIZE(p)))

/****************************************************************************\
|
|	The Endpoint Switch contains dispatch functions for the
|	endpoint.
|
\****************************************************************************/


/*

Type:	UENDPOINTSWITCH_STARTIO_FN

Index:	Type:	USTARTIO_FN
	Type:	PUENDPOINTSWITCH_STARTIO_FN
	Type:	PUSTARTIO_FN
	Name:	USTARTIO()

Function:
	DCD-specific method for starting I/O queued to an endpoint.

Definition:
	#include "uendpoint.h"

	typedef VOID (UENDPOINTSWITCH_STARTIO_FN)(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint
			);

	typedef UENDPOINTSWITCH_STARTIO_FN *PUENDPOINTSWITCH_STARTIO_FN;

	typedef UENDPOINTSWITCH_STARTIO_FN USTARTIOFN, *PUSTARTIOFN;

	VOID USTARTIO(
		UDEVICE *pDevice,
		UENDPOINT *pEndpoint
		);

Description:
	Whenever the portable code wants the DCD to look at the pending
	queue for an endpoint, it calls this function. The DCD then checks
	whether the endpoint is busy, and if not, scans the pending queue, 
	typically removing as many UBUFQEs as can be passed to the hardware.
	(UBUFQEs in the pending queue may be removed by the upper layers
	at any time, and this might prove embarassing if DMA or primary
	ISR activities are still working with the buffer.)

	The macro USTARTIO() invokes the start-I/O method for the specific
	endpoint.

Returns:
	No explicit result.

See also:
	UENDPOINTSWITCH, UENDPOINT

*/

__TMS_BEGIN_DECLS

__TMS_FNTYPE_DEF(
UENDPOINTSWITCH_STARTIO_FN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *, 
		__TMS_UENDPOINT *
		)
	);

__TMS_TYPE_DEF(USTARTIOFN, __TMS_UENDPOINTSWITCH_STARTIO_FN);

/*

Type:	UENDPOINTSWITCH_CANCELIO_FN

Index:	Type:	UCANCELIOFN
	Type:	PUENDPOINTSWITCH_CANCELIO_FN
	Type:	PUCANCELIOFN
	Name:	UCANCELIO()

Function:
	DCD-specific method for canceling I/O queued for an endpoint.

Definition:
	#include "uendpoint.h"

	typedef VOID (UENDPOINTSWITCH_CANCELIO_FN)(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint,
			USTAT Status
			);

	typedef UENDPOINTSWITCH_CANCELIO_FN *PUENDPOINTSWITCH_CANCELIO_FN;

	typedef UENDPOINTSWITCH_CANCELIO_FN UCANCELIOFN, *PUCANCELIOFN;

	VOID UCANCELIO(
		UDEVICE *pDevice,
		UENDPOINT *pEndpoint
		);

Description:
	Under some circumstances, the portable code needs to cancel I/O that
	is pending for a given endpoint. Although it can simply grab the
	UBUFQEs that are in the public pending queue, the portable code must
	ask the DCD to cancel operations that are pending with the hardware.
	It does so by calling this method function.

	In response to this primitive, the DCD begins the process of wrapping
	up any operations that are in-process for the given endpoint. The
	DCD may delay completions as long as needed for hardware to complete.
	Upon return from this call, the portable code can only assume that the
	UBUFQEs will eventually be completed.

	The parameter Status specifies the status to be used when completing
	the UBUFQE.

	The macro UCANCELIO() invokes the cancel I/O method for a given 
	endpoint. 

Returns:
	No explicit result.

See also:
	UENDPOINTSWITCH, UENDPOINT

*/

__TMS_FNTYPE_DEF(
UENDPOINTSWITCH_CANCELIO_FN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *, 
		__TMS_UENDPOINT *, 
		__TMS_USTAT /*why*/
		)
	);

__TMS_TYPE_DEF(UCANCELIOFN, __TMS_UENDPOINTSWITCH_CANCELIO_FN);

/*

Type:	UENDPOINTSWITCH_PREPIO_FN

Index:	Type:	UPREPIOFN
	Type:	PUENDPOINTSWITCH_PREPIO_FN
	Type:	PUPREPIOFN
	Name:	UPREPIO()

Function:
	DCD-specific method to examine a UBUFQE prior to queueing it.

Definition:
	#include "uendpoint.h"

	typedef VOID (UENDPOINTSWITCH_PREPIO_FN)(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint,
			UBUFQE *pQe
			);

	typedef UENDPOINTSWITCH_PREPIO_FN *PUENDPOINTSWITCH_PREPIO_FN;

	typedef	UENDPOINTSWITCH_PREPIO_FN UPREPIOFN, *PUPREPIOFN;

	VOID UPREPIO(
		UDEVICE *pDevice,
		UENDPOINT *pEndpoint,
		UBUFQE *pQe
		);

Description:
	The portable code calls this method for each UBUFQE that's submitted
	to the DCD for processing. The method is invoked prior to inserting
	the UBUFQE into the pending queue.

	The DCD is allowed to do whatever seems useful based on the content
	of the UBUFQE, but ownership of the UBUFQE remains with the portable
	layer. In practice, this routine is not used, and the DCD provides
	a stub routine from the core library.

	The macro UPREPIO() invokes this method.

Returns:
	No explicit result.

Notes:
	This function is deprecated. It was intended to allow the DCD to fill 
	in DCD-specific fields in the UBUFQE, but this can just as easily be
	done when processing the UBUFQE in the start-I/O handler.

	The release procedures of the DataPump don't include regression tests
	to ensure that this method is called on all relevant code paths.

See also:
	UENDPOINTSWITCH, UENDPOINT

*/

__TMS_FNTYPE_DEF(
UENDPOINTSWITCH_PREPIO_FN,
	 __TMS_VOID,
		(
		__TMS_UDEVICE *, 
		__TMS_UENDPOINT *, 
		__TMS_UBUFQE *
		)
	);

__TMS_TYPE_DEF(UPREPIOFN, __TMS_UENDPOINTSWITCH_PREPIO_FN);

/*

Type:	UENDPOINTSWITCH_EVENT_FN

Function:
	Optional method function for handling endpoint-specific interrupts.

Definition:
	#include "uendpoint.h"

	typedef VOID (UENDPOINTSWITCH_EVENT_FN)(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint
			);

	typedef UENDPOINTSWITCH_EVENT_FN *PUENDPOINTSWITCH_EVENT_FN;

	typedef UENDPOINTSWITCH_EVENT_FN UEPEVENTFN, *PUEPEVENTFN;

	VOID UEPEVENT(
		UDEVICE *pDevice,
		UENDPOINT *pEndpoint
		);

Description:
	This endpoint switch method is intended to be called from the
        device interrupt processor when an event is detected for the
        underlying endpoint.

	The DCD may provide this routine as a CONVENIENCE to itself; the 
	DCD is not REQUIRED to provide it.  It's only called (if at all) 
	from code in the DCD. 

	It's defined as part of the portable UENDPOINTSWITCH so that DCDs 
	don't have to provide a DCD-specific parallel set of endpoint-type-
	specific methods.

	This method can be useful for DCDs managing DCIs which have a large 
	number of symmetrical endpoints (which is most common).

	However, for some USB DCIs like the one in SA-1110, it's easier 
	just to do the work directly in the primary ISR, without this extra
	level of indirection.

	The macro UEPEVENT() invokes this method.

Returns:
	No explicit result.

See also:
	UENDPOINTSWITCH, UENDPOINT, UsbEpswEpEventStub()

*/

__TMS_FNTYPE_DEF(
UENDPOINTSWITCH_EVENT_FN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *, 
		__TMS_UENDPOINT *
		)
	);

__TMS_TYPE_DEF(UEPEVENTFN, __TMS_UENDPOINTSWITCH_EVENT_FN);

/*

Type:	UENDPOINTSWITCH_TIMEOUT_FN

Index:	Type:	PUENDPOINTSWITCH_TIMEOUT_FN
	Type:	UTIMEOUTFN
	Type:	PUTIMEOUTFN
	Name:	UTIMEOUT()

Function:
	Optional method function for handling endpoint-specific timeouts.

Definition:
	#include "uendpoint.h"

	typedef VOID (UENDPOINTSWITCH_TIMEOUT_FN)(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint
			);

	typedef UENDPOINTSWITCH_TIMEOUT_FN *PUENDPOINTSWITCH_TIMEOUT_FN;

	typedef UENDPOINTSWITCH_TIMEOUT_FN UTIMEOUTFN, *PUTIMEOUTFN;

	VOID UTIMEOUT(
		UDEVICE *pDeivce,
		UENDPOINT *pEndpoint
		);

Description:
	This endpoint switch method is intended to be called from the
        device interrupt processor when a timeout is detected for the
        underlying endpoint.

	The DCD may provide this routine as a CONVENIENCE to itself; the 
	DCD is not REQUIRED to provide it.  It's only called (if at all) 
	from code in the DCD. 

	It's defined as part of the portable UENDPOINTSWITCH so that DCDs 
	don't have to provide a DCD-specific parallel set of endpoint-type-
	specific methods.

	This method can be useful for DCDs managing DCIs which have a large 
	number of symmetrical endpoints (which is most common).

	However, for some USB DCIs like the one in SA-1110, it's easier 
	just to do the work directly in the primary ISR, without this extra
	level of indirection.

	The macro UTIMEOUT() invokes this method.

Returns:
	No explicit result.

See also:
	UENDPOINTSWITCH, UENDPOINT, UsbEpswTimeoutStub()

*/

__TMS_FNTYPE_DEF(
UENDPOINTSWITCH_TIMEOUT_FN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *, 
		__TMS_UENDPOINT *
		)
	);

__TMS_TYPE_DEF(UTIMEOUTFN, __TMS_UENDPOINTSWITCH_TIMEOUT_FN);

/*

Type:	UENDPOINTSWITCH_QUERYSTATUS_FN

Index:	Type:	PUENDPOINTSWITCH_QUERYSTATUS_FN
	Name:	UENDPOINTSWITCH_QUERYSTATUS()

Function:
	Query an endpoint to see whether traffic is pending.

Definition:
	#include "uendpoint.h"

	typedef BOOL (UENDPOINTSWITCH_QUERYSTATUS_FN)(
			UDEVICE *pDevice,
			UENDPOINT *pEndpoint
			);

	typedef UENDPOINTSWITCH_QUERYSTATUS_FN
	       *PUENDPOINTSWITCH_QUERYSTATUS_FN;

	BOOL UENDPOINTSWITCH_QUERYSTATUS(
		UDEVICE *pDevice,
		UENDPOINT *pEndpoint
		);

Description:
	The portable code calls this method in order to determine whether
	the endpoint is active.

	This method need only be implemented for IN endpoints. The default 
	pipe is not required to provide this method for its control-IN 
	endpoint.  UsbPumpEndpointSwitch_QueryStatusStub() is a suitable
	default method for endpoints that don't need this method.

	If the endpoint is an IN endpoint, the DCD is required to check its 
	internal queues, and return TRUE if any UBUFQE is pending for this 
	endpoint in an internal queue. (The portable queue has already 
	checked pEndpoint->uqe_pending, so the DCD should not bother with 
	this.)

	For IN endpoints, if no UBUFQE is pending, the DCD is also required 
	to check IN FIFOs to see whether traffic is pending to the host 
	(presumably sent by a UBUFQE that already completed). If so, the DCD 
	shall return TRUE.

	In all other cases, this method shall return FALSE.

Returns:
	TRUE if this is an IN endpoint, and if undelivered traffic is
	either in the DCD or in the transmit FIFOs of the endpoint.

Notes:
	If __TMS_DATAPUMP_COMPAT_V3_14 is defined, a synonymous routine
	UsbPumpEndpointSwtich_QeuryStatusStub [sic] is provided.  Obviously, 
	the name is misspelled.

See also:
	UENDPOINTSWITCH, UENDPOINT, 
	UsbPumpEndpointSwitch_QueryStatusStub()
	UsbPumpEndpointSwitch_QeuryStatusStub() [sic]

*/

__TMS_TYPE_DEF_FUNCTION(
UENDPOINTSWITCH_QUERYSTATUS_FN,
	__TMS_BOOL,
		(
		__TMS_UDEVICE *,
		__TMS_UENDPOINT *
		)
	);

/* stub functions */
__TMS_UENDPOINTSWITCH_PREPIO_FN		UsbPumpEndpointSwitch_PrepIoStub;
__TMS_UENDPOINTSWITCH_EVENT_FN		UsbEpswEpEventStub;
__TMS_UENDPOINTSWITCH_TIMEOUT_FN	UsbEpswTimeoutStub;
__TMS_UENDPOINTSWITCH_QUERYSTATUS_FN	UsbPumpEndpointSwitch_QueryStatusStub;

#if __TMS_DATAPUMP_COMPAT_V3_14
__TMS_UENDPOINTSWITCH_QUERYSTATUS_FN	UsbPumpEndpointSwitch_QeuryStatusStub;
#endif

__TMS_END_DECLS

/*

Type:	UENDPOINTSWITCH

Index:	Name:	UENDPOINTSWITCH_INIT_V4()
	Name:	UENDPOINTSWITCH_INIT_V3()
	Name:	UENDPOINTSWITCH_INIT_V2()
	Name:	UENDPOINTSWITCH_INIT_V1()

Function:
	Method table for endpoints.

Definition:
	#include "uendpoint.h"

	typedef struct TTUSB_UENDPOINTSWITCH UENDPOINTSWITCH;

	CONST UENDPOINTSWITCH epSwitch = 
		UENDPOINTSWITCH_INIT_V1(
			UENDPOINTSWITCH_STARTIO_FN *pStartIoFn,
			UENDPOINTSWITCH_CANCELIO_FN *pCancelIoFn,
			UENDPOINTSWITCH_PREPIO_FN *pPrepIoFn
			);

	CONST UENDPOINTSWITCH epSwitch = 
		UENDPOINTSWITCH_INIT_V2(
			UENDPOINTSWITCH_STARTIO_FN *pStartIoFn,
			UENDPOINTSWITCH_CANCELIO_FN *pCancelIoFn,
			UENDPOINTSWITCH_PREPIO_FN *pPrepIoFn,
			UENDPOINTSWITCH_EVENT_FN *pEpEventFn
			);

	CONST UENDPOINTSWITCH epSwitch = 
		UENDPOINTSWITCH_INIT_V3(
			UENDPOINTSWITCH_STARTIO_FN *pStartIoFn,
			UENDPOINTSWITCH_CANCELIO_FN *pCancelIoFn,
			UENDPOINTSWITCH_PREPIO_FN *pPrepIoFn,
			UENDPOINTSWITCH_EVENT_FN *pEpEventFn,
			UENDPOINTSWITCH_TIMEOUT_FN *pTimeoutFn
			);

	CONST UENDPOINTSWITCH epSwitch = 
		UENDPOINTSWITCH_INIT_V4(
			UENDPOINTSWITCH_STARTIO_FN *pStartIoFn,
			UENDPOINTSWITCH_CANCELIO_FN *pCancelIoFn,
			UENDPOINTSWITCH_PREPIO_FN *pPrepIoFn,
			UENDPOINTSWITCH_EVENT_FN *pEpEventFn,
			UENDPOINTSWITCH_TIMEOUT_FN *pTimeoutFn,
			UENDPOINTSWITCH_QUERYSTATUS_FN *pQueryStatusFn
			);

Description:
	The UENDPOINTSWITCH structure provides a table of function pointers
	to be used by portable code to invoke method functions for a given
	endpoint. Since in most DCI architectures a given endpoint may be of
	many different types (at different times), the DCD normally 
	defines one instance of the switch for each possible endpoint type,
	and then dynamicaly changes uep_pSwitch to point to the appropriate
	instance when the UENDPOINT is bound to a pipe.

	The UENDPOINTSWITCH_INIT_V4(), etc., macros allow for safe
	initialization of the endpoint instances. If a new entry has to be
	added, we create a new (Vn) macro, and provide suitable stubs.

	The function pointers are never allowed to be NULL; instead, stubs
	(pointers to functions that do nothing and return) must be provided.
	The portable library provides suitable stubs for some (but not all)
	of the entries in the UENDPOINTSWITCH.

Contents:
	UENDPOINTSWITCH_STARTIO_FN *uepsw_StartIo;
		The start-I/O method.

	UENDPOINTSWITCH_CANCELIO_FN *uepsw_CancelIo;
		The cancel-I/O method.

	UENDPOINTSWITCH_PREPIO_FN *uepsw_PrepIo;
		The prepare-for-I/O method. May be stubbed.

	UENDPOINTSWITCH_EVENT_FN *uepsw_EpEvent;
		The (DCD-defined) endpoint-event function. May be stubbed.

	UENDPOINTSWITCH_TIMEOUT_FN *uepsw_Timeout;
		The (DCD-defined) endpoint time-out function. May be stubbed.

	UENDPOINTSWITCH_QUERYSTATUS_FN	*uepsw_QueryStatus;
		The query status function -- only needs to be non-stub for
		non-default-pipe IN endpoints


See Also:
	UENDPOINT

*/

struct TTUSB_UENDPOINTSWITCH
	{
	__TMS_UENDPOINTSWITCH_STARTIO_FN	*uepsw_StartIo;
	__TMS_UENDPOINTSWITCH_CANCELIO_FN	*uepsw_CancelIo;
	__TMS_UENDPOINTSWITCH_PREPIO_FN		*uepsw_PrepIo;
	__TMS_UENDPOINTSWITCH_EVENT_FN		*uepsw_EpEvent;
	__TMS_UENDPOINTSWITCH_TIMEOUT_FN	*uepsw_Timeout;
	__TMS_UENDPOINTSWITCH_QUERYSTATUS_FN	*uepsw_QueryStatus;
	};

#define	__TMS_USTARTIO(pDev, pEp)	\
		((*((pEp)->uep_pSwitch)->uepsw_StartIo)((pDev), (pEp)))

#define	__TMS_UCANCELIO(pDev, pEp, why)	\
		((*((pEp)->uep_pSwitch)->uepsw_CancelIo)((pDev), (pEp), (why)))

#define	__TMS_UPREPIO(pDev, pEp, pQe)	\
		((*((pEp)->uep_pSwitch)->uepsw_PrepIo)((pDev), (pEp), (pQe)))

#define __TMS_UEPEVENT(pDev, pEp) \
		((*((pEp)->uep_pSwitch)->uepsw_EpEvent)((pDev), (pEp)))

#define	__TMS_UTIMEOUT(pDev, pEp)	\
		((*((pEp)->uep_pSwitch)->uepsw_Timeout)((pDev), (pEp)))

#define	__TMS_UENDPOINTSWITCH_QUERYSTATUS(pDev, pEp)	\
		((*((pEp)->uep_pSwitch)->uepsw_QueryStatus)(	\
			(pDev), (pEp)))

/* use these for initializing endpoint switches */
#define __TMS_UENDPOINTSWITCH_INIT_V1(StartIo, CancelIo, PrepIo) \
	__TMS_UENDPOINTSWITCH_INIT_V2(StartIo, CancelIo, PrepIo, \
		UsbEpswEpEventStub)

#define	__TMS_UENDPOINTSWITCH_INIT_V2(StartIo, CancelIo, PrepIo, EpEvent) \
	__TMS_UENDPOINTSWITCH_INIT_V3(StartIo, CancelIo, PrepIo, EpEvent, \
		UsbEpswTimeoutStub \
		)

#define	__TMS_UENDPOINTSWITCH_INIT_V3(StartIo, CancelIo, PrepIo, EpEvent, Timeout) \
	__TMS_UENDPOINTSWITCH_INIT_V4(			\
		StartIo,				\
		CancelIo,				\
		PrepIo,					\
		EpEvent,				\
		Timeout,				\
		UsbPumpEndpointSwitch_QueryStatusStub	\
		)

#define	__TMS_UENDPOINTSWITCH_INIT_V4(	\
	uEndpointSwitch_StartIo,	\
	uEndpointSwitch_CancelIo,	\
	uEndpointSwitch_PrepIo,		\
	uEndpointSwitch_EpEvent,	\
	uEndpointSwitch_Timeout,	\
	uEndpointSwitch_QueryStatus	\
	)				\
	{				\
	(uEndpointSwitch_StartIo),	\
	(uEndpointSwitch_CancelIo),	\
	(uEndpointSwitch_PrepIo),	\
	(uEndpointSwitch_EpEvent),	\
	(uEndpointSwitch_Timeout),	\
	(uEndpointSwitch_QueryStatus)	\
	}

/*

Name:	UENDPOINT_CANPUTSIMPLE()

Function:
	Macro predicate:  returns true if a given QE can be put directly as
	a single packet.

Definition:
	BOOL UENDPOINT_CANPUTSIMPLE(
		UENDPOINT *pep, 
		UBUFQE *pqe,
		BYTES MaxPacketSize,
		BYTES nAvail
		);

Description:
	pqe is the UBUFQE that is about to be written to the endpoint pep.
	wMaxPacketsize is the current packet size for that endpoint; nAvail
	is the number of bytes in that UBUFQE that haven't yet been written.

Returns:
	TRUE if and only if this UBUFQE can really be written to the 
	fifo and committed as a packet (even if it's short).

Notes:
	This macro takes into account the state of the pending queue, the
	postbreak flags of the current pqe and the prebreak and stall flags
	of the head of the pending queue.

*/

/*
|| This macro tells us whether we can put this qe in a single operation.
*/
#define	__TMS_UENDPOINT_CANPUTSIMPLE(pep, pqe, pktsize, navail) \
	((pktsize) <= (navail) || \
	 (pep)->uep_pending == __TMS_NULL || \
	 ((pqe)->uqe_flags & __TMS_UBUFQEFLAG_POSTBREAK) || \
	 ((pep)->uep_pending->uqe_flags & \
	  (__TMS_UBUFQEFLAG_PREBREAK | __TMS_UBUFQEFLAG_STALL)))

/*

Name:	UENDPOINT_COUNT_PENDING_BYTES()

Function:
	For non-simple packet writes, calculate the packet size and last
	UBUFQE.

Definition:
	VOID UENDPOINT_COUNT_PENDING_BYTES(
		IN CONST UENDPOINT *pep,
		IN CONST BYTES MaxPacketSize,
		IN OUT BYTES *pNavail,
		OUT UENDPOINT **pplastqe
		);

Description:
	On the assumption that UENDPOINT_CANPUTSIMPLE is false, and that
	*pNavail initially holds the number of bytes that will be written
	to the packet from the current UBUFQE, UENDPOINT_COUNT_PENDING_BYTES()
	looks ahead in the pending queue until either (1) UBUFQEs that would
	get the packet size to MaxPacketSize have been seen, (2) the end
	of the pending list is encountered; (3) a UBUFQE is encountered with
	flags that require ending the packet with a short packet.

Returns:
	No explicit result.  (*pNavail) is increased to the actual packet
	size, and *pplastqe is set to point to the last ubufqe in the pending
	list that should be processed into the current packet. 

Notes:
	Assumes that UENDPOINT_CANPUTSIMPLE() has been tested and is in
	fact FALSE.

*/

/*
|| This macro scans through the pending list, and tries to figure out how to
|| assemble fragments into a single packet, assuming that navail is the number
|| of bytes that can be put from the current packet, and that CANPUTSIMPLE
|| was false.
*/
#define	__TMS_UENDPOINT_COUNT_PENDING_BYTES(pep, pktsize, pnavail, pplastqe) \
    do	{ \
	__TMS_PUBUFQE __pthisqe; \
	__TMS_PUBUFQE __pnextqe; \
	\
	__pthisqe = __pnextqe = (pep)->uep_pending; \
	while (__pnextqe && *(pnavail) < (pktsize)) \
		{ \
		\
		__pthisqe   = __pnextqe; \
		*(pnavail) += __pthisqe->uqe_bufsize; \
		if ((__pthisqe->uqe_flags & __TMS_UBUFQEFLAG_POSTBREAK) || \
		    (__pnextqe = __pthisqe->uqe_next) == (pep)->uep_pending || \
		    (__pnextqe->uqe_flags & \
		     (__TMS_UBUFQEFLAG_PREBREAK | __TMS_UBUFQEFLAG_STALL))) \
			break; \
		} \
	\
	if (*(pnavail) > (pktsize)) \
		{ \
		*(pnavail) = (pktsize); \
		} \
	*(pplastqe) = __pthisqe; \
	} while (0)

/*

Name:	UENDPOINT_AUTO_REMOTE_WAKEUP_OK()

Function:
	Return the endpoint's eligibility to issue auto remote wakeup.

Definition:
	BOOL UENDPOINT_AUTO_REMOTE_WAKEUP_OK(
		IN CONST UENDPOINT *pUep,
		IN BOOL	fTrueForExamine
		);

Description:
	Take fTrueForExamine as the feature bit for backwards compatibility,
	If fTrueForExamine is FALSE, bypass the endpoint validation and return
	TRUE.
	If fTrueForExamine is TRUE, examine the endpoint's eligility of auto
	remote wakeup.  Currently BULK/INT IN pipes are eligible to issue auto
	remote wakeup.

Returns:
	TRUE if this endpoint is eligible to issue auto remote wakeup.
	FLASE otherwise.

Notes:
	Don't use this macro. Use UENDPOINT_CAN_AUTO_REMOTE_WAKEUP() or
	UENDPOINT_CHECK_AUTO_REMOTE_WAKEUP().

*/

#define	__TMS_UENDPOINT_AUTO_REMOTE_WAKEUP_OK(pUep, fTrueForExamine)	\
	(! (fTrueForExamine) ||						\
	((pUep)->uep_pPipe != __TMS_NULL &&				\
	(pUep)->uep_fActive &&						\
	(__TMS_UPIPE_SETTING_MASK((pUep)->uep_pPipe->upipe_bmAttributes,\
				  (pUep)->uep_pPipe->upipe_bEndpointAddress) \
			& __TMS_UPIPE_SETTING_MASK_BULKINT_IN)))

/*

Name:	UENDPOINT_CAN_AUTO_REMOTE_WAKEUP()

Function:
	Return the endpoint's eligibility to issue auto remote wakeup.

Definition:
	BOOL UENDPOINT_CAN_AUTO_REMOTE_WAKEUP(
		IN CONST UENDPOINT *pUep
		);

Description:
	This macro examines the endpoint's eligility of auto remote wakeup.
	Currently BULK/INT IN pipes are eligible to issue auto remote wakeup.

Returns:
	TRUE if this endpoint is eligible to issue auto remote wakeup.
	FLASE otherwise.

*/

#define	__TMS_UENDPOINT_CAN_AUTO_REMOTE_WAKEUP(pUep)		\
	((pUep)->uep_pPipe != __TMS_NULL &&			\
	 (pUep)->uep_fActive &&					\
	 (__TMS_UPIPE_GET_SETTING_MASK((pUep)->uep_pPipe) &	\
	  __TMS_UPIPE_SETTING_MASK_BULKINT_IN))

/*

Name:	UENDPOINT_CHECK_AUTO_REMOTE_WAKEUP()

Function:
	Check the endpoint can issue auto remote wakeup.

Definition:
	BOOL UENDPOINT_CHECK_AUTO_REMOTE_WAKEUP(
		IN CONST UENDPOINT *pUep,
		IN CONST UDEVICE *pDevice
		);

Description:
	This macro checks the endpoint need to  auto remote wakeup.
	Currently BULK/INT IN pipes are eligible to issue auto remote wakeup.

Returns:
	TRUE if this endpoint is eligible to issue auto remote wakeup.
	FLASE otherwise.

*/

#define	__TMS_UENDPOINT_CHECK_AUTO_REMOTE_WAKEUP(pUep, pDevice)	\
	(! __TMS_UDEVICE_IS_L0_STATE(pDevice) &&		\
	 /* ! __TMS_UDEVICE_IS_U0_STATE(pDevice) && */		\
	 (pDevice)->udev_fValidateEpforAutoRemoteWakeup &&	\
	 __TMS_UENDPOINT_CAN_AUTO_REMOTE_WAKEUP(pUep))


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UENDPOINT_HDR	\
   __TMS_UENDPOINT_HDR
# define USTARTIO(pDev, pEp)	\
   __TMS_USTARTIO(pDev, pEp)
# define UCANCELIO(pDev, pEp, why)	\
   __TMS_UCANCELIO(pDev, pEp, why)
# define UPREPIO(pDev, pEp, pQe)	\
   __TMS_UPREPIO(pDev, pEp, pQe)
# define UEPEVENT(pDev, pEp)	\
   __TMS_UEPEVENT(pDev, pEp)
# define UTIMEOUT(pDev, pEp)	\
   __TMS_UTIMEOUT(pDev, pEp)
# define UENDPOINTSWITCH_QUERYSTATUS(pDev, pEp)	\
   __TMS_UENDPOINTSWITCH_QUERYSTATUS(pDev, pEp)
# define UENDPOINTSWITCH_INIT_V1(StartIo, CancelIo, PrepIo)	\
   __TMS_UENDPOINTSWITCH_INIT_V1(StartIo, CancelIo, PrepIo)
# define UENDPOINTSWITCH_INIT_V2(StartIo, CancelIo, PrepIo, EpEvent)	\
   __TMS_UENDPOINTSWITCH_INIT_V2(StartIo, CancelIo, PrepIo, EpEvent)
# define UENDPOINTSWITCH_INIT_V3(StartIo, CancelIo, PrepIo, EpEvent, Timeout)	\
   __TMS_UENDPOINTSWITCH_INIT_V3(StartIo, CancelIo, PrepIo, EpEvent, Timeout)
# define UENDPOINTSWITCH_INIT_V4(	\
	uEndpointSwitch_StartIo,	\
	uEndpointSwitch_CancelIo,	\
	uEndpointSwitch_PrepIo,		\
	uEndpointSwitch_EpEvent,	\
	uEndpointSwitch_Timeout,	\
	uEndpointSwitch_QueryStatus	\
	)	\
	__TMS_UENDPOINTSWITCH_INIT_V4(	\
	uEndpointSwitch_StartIo,	\
	uEndpointSwitch_CancelIo,	\
	uEndpointSwitch_PrepIo,		\
	uEndpointSwitch_EpEvent,	\
	uEndpointSwitch_Timeout,	\
	uEndpointSwitch_QueryStatus	\
	)
# define UENDPOINT_CANPUTSIMPLE(pep, pqe, pktsize, navail)	\
   __TMS_UENDPOINT_CANPUTSIMPLE(pep, pqe, pktsize, navail)
# define UENDPOINT_COUNT_PENDING_BYTES(pep, pktsize, pnavail, pplastqe)	\
   __TMS_UENDPOINT_COUNT_PENDING_BYTES(pep, pktsize, pnavail, pplastqe)
# define UENDPOINT_AUTO_REMOTE_WAKEUP_OK(pUep, fTrueForExamine)	\
   __TMS_UENDPOINT_AUTO_REMOTE_WAKEUP_OK(pUep, fTrueForExamine)
# define UENDPOINT_CAN_AUTO_REMOTE_WAKEUP(pUep)	\
   __TMS_UENDPOINT_CAN_AUTO_REMOTE_WAKEUP(pUep)
# define UENDPOINT_CHECK_AUTO_REMOTE_WAKEUP(pUep, pDevice)	\
   __TMS_UENDPOINT_CHECK_AUTO_REMOTE_WAKEUP(pUep, pDevice)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of uendpoint.h ****/
#endif /* _UENDPOINT_H_ */
