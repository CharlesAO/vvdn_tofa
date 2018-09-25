/* uhilintsys.h	Mon Mar 21 2016 11:08:16 chwon */

/*

Module:  uhilintsys.h

Function:
	Home for modern UHIL_INTERRUPT_SYSTEM_INTERFACE functions.

Version:
	V3.17a	Mon Mar 21 2016 11:08:16 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2002, 2009, 2015-2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.69a  Mon May 27 2002 19:20:37  tmm
	Module created.

   2.01a  Mon Mar 30 2009 16:32:48  chwon
	7667: Added UHIL_CONNECT_TO_INTERRUPT_V2_FN in the
	UHIL_INTERRUPT_SYSTEM_INTERFACE structure and created _INIT_V2() macro

   2.01a  Thu Apr 16 2009 14:07:29  chwon
	7667: Added ..._SETUP_Vx() macro.

   2.01a  Fri May 08 2009 16:48:52  chwon
	7667: Change line and message ISR prototype.  Create V2 stub function.

   2.01a  Sat May  9 2009 17:46:22  tmm
	7667: Continue elaborating the new interrupt system interfaces.

   2.01a  Sun May 10 2009 15:51:07  chwon
	7667: Fixed ARMADS warning: >= 10 lines of macro arguments

   3.15a  Thu Apr 02 2015 15:04:21  tmm
	19107: Add multiprocessor interrupt interlock operations.

   3.17a  Mon Mar 21 2016 11:08:16  chwon
	19107: Fix typo in __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V2().

*/

#ifndef _UHILINTSYS_H_		/* prevent multiple includes */
#define _UHILINTSYS_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

#ifndef _BASEHILTYPES_H_
# include "basehiltypes.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

/****************************************************************************\
|
|	Starting with production Version 2 (internal V1.51), we 
|	have an abstract interrupt subsystem interface.  All new MCCI
|	code should use this mechanism.
|
\****************************************************************************/

/* A forward reference, but defined above: */
/* __TMS_TYPE_DEF_STRUCT(UHIL_INTERRUPT_SYSTEM_INTERFACE); */

/*
|| an interrupt connection handle is returned in response to an interrupt
|| connect operation.  It is later used as a parameter to the other
|| interrupt control operations, much in the same way that a file handle
|| is used to operate on a specific file.
*/
__TMS_TYPE_DEF(UHIL_INTERRUPT_CONNECTION_HANDLE, __TMS_VOID *);

/*
|| an interrupt message is an input parameter of
|| UHIL_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN.  It represents message interrupt.
*/
__TMS_TYPE_DEF(UHIL_INTERRUPT_MESSAGE, __TMS_UINT32);

/*

Type:	UHIL_ISR_INTERLOCK_STATE

Function:
	Represents the saved state of an acquired ISR interlock.

Definition:
	#include "basehiltypes.h"

	typedef UINT_PTR UHIL_ISR_INTERLOCK_STATE;

Description:
	Values of this type are used to represent the save state of an
	acquired ISR interlock. The value is UINT_PTR so that (if necessary)
	it can hold a pointer to a nonce object.

Notes:
	Documented in uhilintsys.h because it's really a component of
	uhilintsys.h; but it's in the base types so that people can refer to
	it without including the whole framework.

*/

/* defined elsewhere, documented here */
/* __TMS_TYPE_DEF(UHIL_ISR_INTERLOCK_STATE, __TMS_UINT_PTR); */ 

/*

Type:  UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN

Index:	Type:	UHIL_INTERRUPT_SERVICE_ROUTINE_FN

Function:
	The interrupt service routine for line-mode interrupts in
	uniprocessor model.

Definition:
	typedef BOOL
	(*UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN)(
		VOID *	pIsrContext
		);

	typedef BOOL
	(*UHIL_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN)(
		VOID *	pIsrContext,
		UHIL_INTERRUPT_MESSAGE uMessage
		);
	
	typedef UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN
		*PUHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN;

	typedef UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN
		UHIL_INTERRUPT_SERVICE_ROUTINE_FN,
		*PUHIL_INTERRUPT_SERVICE_ROUTINE_FN;

Description:
	The interrupt service routine is called in response to an interrupt on
	our IRQ.  It is passed a context pointer, specified when the routine
	was registered.  In order to support shared interrupt schemes (like
	PCI), the ISR is required to return TRUE if it thinks that its device
	was interrupting, and FALSE otherwise.   The platform is, of course,
	at liberty to ignore this result.

	Message-mode ISRs can assume that their hardware generated the
	interrupt, and also assume that the interrupt source is edge
	sensitive, not level sensitive.  Therefore, message-mode ISRs
	normally need not access the hardware, and avoid doing so if possible.

	UHIL_INTERRUPT_SERVICE_ROUTINE_FN is equivalent name to
	UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN.  It's provided for
	historical reasons, and should not be used in new code.

Returns:
	TRUE if it thinks that its device was interrupting, FALSE otherwise

Notes:
	This kind of ISR is used with platforms that use the "uniprocessor
	synchronization model" for dealing with interrupts.  On such
	platforms, UHIL_setpsw() and UHIL_di() are effective for diabling
	interrupts, and UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN routines
	may safely call UsbPumpPlatform_SynchronizeToDataPump() and
	the message posting routines.  If the DataPump is to be notified,
	a uniprocessor-model interrupt service routine must explicitly
	do the notification.

	New platforms, DCDs, and HCDs should use the multi-processor model
	by preference.  However, DCDs and HCDs may need to provide
	uniprocessor-model ISRs for supporting older platforms.

	Message-mode ISRs should always return TRUE.

*/

__TMS_FNTYPE_DEF(
	UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN,
	__TMS_BOOL,
	(__TMS_VOID *)
	);

__TMS_FNTYPE_DEF(
	UHIL_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN,
	__TMS_BOOL,
	(
	__TMS_VOID *,
	__TMS_UHIL_INTERRUPT_MESSAGE
	));


/* declare a synonym for backwards compatibility */
__TMS_TYPE_DEF(UHIL_INTERRUPT_SERVICE_ROUTINE_FN, 
	__TMS_UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN
	);

/*

Type:  UHIL_MP_LINE_INTERRUPT_SERVICE_ROUTINE_FN

Index:	Type:	PUHIL_MP_LINE_INTERRUPT_SERVICE_ROUTINE_FN
	Type:	UHIL_MP_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN
	Type:	PUHIL_MP_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN
	Const:	UHIL_MP_INTERRUPT_SYSTEM_RESULT_PROCESSED
	Name:	UHIL_MP_INTERRUPT_SYSTEM_RESULT_WAS_PROCESSED(s)
	Name:	UHIL_MP_INTERRUPT_SYSTEM_RESULT_GET_EVENT(s)

Function:
	The interrupt service routines for multiprocessor-model
	interrupts.

Definition:
	typedef UCALLBACKCOMPLETION *
	(*UHIL_MP_LINE_INTERRUPT_SERVICE_ROUTINE_FN)(
		VOID *	pIsrContext
		);

	typedef UCALLBACKCOMPLETION *
	(*UHIL_MP_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN)(
		VOID *	pIsrContext,
		UHIL_INTERRUPT_MESSAGE	uMessage
		);

	UCALLBACKCOMPLETION *
	UHIL_MP_INTERRUPT_SYSTEM_RESULT_PROCESSED

	BOOL
	UHIL_MP_INTERRUPT_SYSTEM_RESULT_WAS_PROCESSED(
		UCALLBACKCOMPLETION *isrResult
		);

Description:
	Interrupt service routines are called in response to an interrupt on
	our IRQ.  All ISRs are passed a context pointer, specified when the 
	routine was registered.  Message-mode ISRs are additionally passed
	a message index that indicates the particular interrupt that occured.

	In order to support shared interrupt schemes (like
	PCI), a line mode ISR is required to return TRUE if it thinks that 
	its device was interrupting, and FALSE otherwise.  Furthermore,
	in order to support level-sensitive interrupts, a line mode ISR
	must ensure that its hardware is not interrupting.  This normally
	requires accessing device registers, which can be very slow on
	PCI-based systems.

	Message-mode ISRs can assume that their hardware generated the
	interrupt, and also assume that the interrupt source is edge
	sensitive, not level sensitive.  Therefore, message-mode ISRs
	normally need not access the hardware, and avoid doing so if possible.

	Multiprocessor-model ISRs are restricted in terms of the DataPump
	APIs they're premitted to use.  In general, they must not use
	UHIL_di() or UHIL_setpsw(), nor may they call routines that use
	those functions.  This meants that they must not attempt to
	synchronize with the DataPump.

	Instead, multiprocessor-model ISRs normally prepare
	UCALLBACKCOMPLETIONs, using UHIL_InterlockedExchange() or
	UsbPumpPlatform_MarkCompletionNotBusy() to set
	the context pointer.  If an event needs to be posted, they simply
	return the event pointer as the result.  If no event needs to be
	posted, they return UHIL_MP_INTERRUPT_SYSTEM_RESULT_PROCESSED.
	Line mode ISRs that disover that the interrupt is not for their
	hardware should return NULL.

	The macro UHIL_MP_INTERRUPT_SYSTEM_RESULT_WAS_PROCESSED() can
	be applied to the result of an ISR to determine whether the ISR
	processed the interrupt.

	The macro UHIL_MP_INTERRUPT_SYSTEM_RESULT_GET_EVENT() can be 
	applied to the result of an ISR.  If the ISR returned an event
	to be posted to the DataPump, the result will be non-NULL; otherwise
	the result will be NULL.

Returns:
	An ISR returns NULL if it thinks that its device was not interrupting.

	An ISR returns UHIL_INTERRUPT_SYSTEM_RESULT_PROCESSED if it thinks 
	that its device was interrupting, but there's nothing to tell the
	DataPump right now.

	An ISR returns a pointer to a UCALLBACKCOMPLETION to be posted to the
	DataPump event queue if it thinks that its device was interrupting and
	wants the DataPump to continue processing.

	An ISR cannot both indicate that the interrupt was not for this
	hardware and yet post an event to the DataPump.

Notes:
	Message-mode ISRs should always return either
	UHIL_MP_INTERRUPT_SYSTEM_RESULT_PROCESSED or a pointer to a
	UCALLBACKCOMPLETION.

*/

#define	__TMS_UHIL_MP_INTERRUPT_SYSTEM_RESULT_PROCESSED		\
	((__TMS_UCALLBACKCOMPLETION *)(__TMS_VOID *) 1)

#define	__TMS_UHIL_MP_INTERRUPT_SYSTEM_RESULT_WAS_PROCESSED(s)	\
	((s) != __TMS_NULL)

#define	__TMS_UHIL_MP_INTERRUPT_SYSTEM_RESULT_GET_EVENT(s)	\
	((s) != __TMS_NULL &&					\
	 (s) != __TMS_UHIL_MP_INTERRUPT_SYSTEM_RESULT_PROCESSED \
	     ?	(s)						\
	     :	__TMS_NULL)

__TMS_FNTYPE_DEF(
	UHIL_MP_LINE_INTERRUPT_SERVICE_ROUTINE_FN,
	__TMS_UCALLBACKCOMPLETION *,
	(__TMS_VOID *)
	);

__TMS_FNTYPE_DEF(
	UHIL_MP_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN,
	__TMS_UCALLBACKCOMPLETION *,
	(
	__TMS_VOID *,
	__TMS_UHIL_INTERRUPT_MESSAGE
	));

/*

Type:  UHIL_OPEN_INTERRUPT_CONNECTION_FN

Function:
	Open an interrupt connection handle

Definition:
	typedef UHIL_INTERRUPT_CONNECTION_HANDLE
	(*UHIL_OPEN_INTERRUPT_CONNECTION_FN)(
		CONST UHIL_INTERRUPT_SYSTEM_INTERFACE *	pUhilIntSys,
		UHIL_INTERRUPT_RESOURCE_HANDLE		ResourceHandle
		);

Description:
	The open-interrupt function is called to open an interrupt
	connection handle, so we can start managing a particular
	IRQ.  The IRQ is placed in a quieted state.

Returns:
	TRUE if it thinks that its device was interrupting, FALSE otherwise

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
	UHIL_OPEN_INTERRUPT_CONNECTION_FN,
	__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE,
	(
	__TMS_CONST __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE *,
	__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE
	));

/*

Type:  UHIL_CLOSE_INTERRUPT_CONNECTION_FN

Function:
	Close an interrupt connection handle

Definition:
	typedef BOOL
	(*UHIL_CLOSE_INTERRUPT_CONNECTION_FN)(
		UHIL_INTERRUPT_CONNECTION_HANDLE	ConnectionHandle
		);

Description:
	Platforms which support plug and play or device shutdown (e.g, the
	DOS programs which want to unhook and exit) also have to provide a
	close-interrupt-handle primitive.  This can be stubbed on
	embedded platforms which have no use for this.

Returns:
	TRUE if success, FALSE otherwise

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
	UHIL_CLOSE_INTERRUPT_CONNECTION_FN,
	__TMS_BOOL,
	(__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE)
	);

/*

Type:  UHIL_CONNECT_TO_INTERRUPT_FN

Function:
	Connect an ISR to an interrupt

Definition:
	typedef BOOL
	(*UHIL_CONNECT_TO_INTERRUPT_FN)(
		UHIL_INTERRUPT_CONNECTION_HANDLE	ConnectionHandle,
		UHIL_INTERRUPT_SERVICE_ROUTINE_FN *	pIsr,
		VOID *					pIsrContext
		);

Description:
	The connect-to-interrupt function is called to connect an ISR to
	an interrupt.  If the interrupt system permits, the interrupt is
	initially disabled at the interrupt controller.

Returns:
	TRUE if success, FALSE otherwise

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
	UHIL_CONNECT_TO_INTERRUPT_FN,
	__TMS_BOOL,
	(
	__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE,
	__TMS_UHIL_INTERRUPT_SERVICE_ROUTINE_FN *,
	__TMS_VOID *
	));

/*

Type:  UHIL_CONNECT_TO_INTERRUPT_V2_FN

Function:
	Connect ISRs (line and message ISR) to an interrupt

Definition:
	typedef BOOL
	(*UHIL_CONNECT_TO_INTERRUPT_V2_FN)(
		UHIL_INTERRUPT_SYSTEM_INTERFACE			*pIntSys,
		UHIL_INTERRUPT_CONNECTION_HANDLE		hConnect,
		VOID *						pIsrContext,
		UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN *	pLineIsr,
		UHIL_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN *	pMessageIsr,
		UHIL_MP_LINE_INTERRUPT_SERVICE_ROUTINE_FN *	pMpLineIsr,
		UHIL_MP_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN *	pMpMessageIsr
		);

Description:
	The connect-to-interrupt function is called to connect an ISR to
	an interrupt.  At most one of the provided ISRs will be used on
	any given system.  The caller need not provide all four routines
	to this service, but if the system needs to use a routine that is
	not provided, the connection will not be made.  

	In most cases, the platform decides in advance how an interrupt
	needs to be connected.  Furthermore, uniprocessor-model interrupt
	processing might not be available on a given platform.  For
	maximum portability to new systems, DCDs and HCDs should provide
	pMpLineIsr and pMpMessage ISR.  

	If the interrupt system permits, the interrupt is initially disabled 
	at the interrupt controller.

Returns:
	TRUE if success, FALSE otherwise

Notes:
	On systems such as the Windows kernel that don't allow disabling
	interrupts after connecting, the interrupt system may defer
	the physical connection until the interrupt is enabled using
	UHIL_INTERRUPT_CONTROL_FN.

	pIntSys is supplied so that MCCI can provide a default proxy for
	older (uniprocessor-only) platforms.

*/

__TMS_FNTYPE_DEF(						\
	UHIL_CONNECT_TO_INTERRUPT_V2_FN,			\
	__TMS_BOOL,						\
	(							\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE *,		\
	__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE,			\
	__TMS_VOID *,						\
	__TMS_UHIL_LINE_INTERRUPT_SERVICE_ROUTINE_FN *,		\
	__TMS_UHIL_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN *,	\
	__TMS_UHIL_MP_LINE_INTERRUPT_SERVICE_ROUTINE_FN *,	\
	__TMS_UHIL_MP_MESSAGE_INTERRUPT_SERVICE_ROUTINE_FN *	\
	));

/*

Type:  UHIL_DISCONNECT_FROM_INTERRUPT_FN

Function:
	Disconnects an ISR without releasing the interrupt handle

Definition:
	typedef BOOL
	(*UHIL_DISCONNECT_FROM_INTERRUPT_FN)(
		UHIL_INTERRUPT_CONNECTION_HANDLE	ConnectionHandle
		);

Description:
	Platforms which support plug and play or device shutdown (e.g, the
	DOS programs which want to unhook and exit) might also have to use
	the disconnect-from-interrupt primitive, which disconnects an ISR
	without releasing the interrupt handle.  This can be stubbed on
	embedded platforms which have no use for this.

Returns:
	TRUE if success, FALSE otherwise

Notes:
	None.

*/

__TMS_FNTYPE_DEF(
	UHIL_DISCONNECT_FROM_INTERRUPT_FN,
	__TMS_BOOL,
	(__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE)
	);

/*

Type:  UHIL_INTERRUPT_CONTROL_FN

Function:
	Enable / disable interrupt

Definition:
	typedef BOOL
	(UHIL_INTERRUPT_CONTROL_FN)(
		UHIL_INTERRUPT_CONNECTION_HANDLE	ConnectionHandle,
		BOOL					fEnableInterrupt
		);

Description:
	This function is called to enable / disable an interrupt at the
	interrupt controller.

Returns:
	TRUE if success, FALSE otherwise

Notes:
	Somewhat tentatively, we define a function type that is used
	for manipulating the interrupt subsystem (interrupt controller).
	Tmm is nervous because he is aware of platforms on which this
	cannot be implemented.  But there may be chips which absolutely
	require this, so we leave it here.

*/

__TMS_FNTYPE_DEF(
	UHIL_INTERRUPT_CONTROL_FN,
	__TMS_BOOL,
	(__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE, __TMS_BOOL)
	);

/*

Type:	UHIL_INTERRUPT_SYSTEM_IOCTL_FN

Index:	Type:	PUHIL_INTERRUPT_SYSTEM_IOCTL_FN

Function:
	Get information about the interrupt represented by an interrupt
	connection handle.

Definition:
	#include "uhilintsys.h"

	typedef USBPUMP_IOCTL_RESULT
	(UHIL_INTERRUPT_SYSTEM_IOCTL_FN)(
		UHIL_INTERRUPT_SYSTEM_INTERFACE		*pInterruptSystem,
		USBPUMP_IOCTL_CODE			IoctlCode,
		CONST VOID *				pInArg,
		VOID *					pOutArg
		);

	typedef	UHIL_INTERRUPT_SYSTEM_IOCTL_FN
		*PUHIL_INTERRUPT_SYSTEM_IOCTL_FN;

Description:
	UHIL_INTERRUPT_CONNECTION_IOCTL_FN allows clients to get information
	about the implementation and requirements from the interrupt system
	for a given interrupt handle.  It may be called after opening an
	interrupt connection handle, and can return the following information:

	  * whether the interrupt will connect in line mode or message mode;
	  * if in message mode, the number of messages that can be handled
	    by the interrupt system.
	  * whether the interrupt system supports uniprocessor-model ISRs
	  * whether the interrupt system supports multiprocessor-model ISRs

Returns:
	UHIL_INTERRUPT_SYSTEM_IOCTL_FN returns the usual DataPump
	IOCTL result codes.

Notes:
	This seems like overkill, but it's consistent with the rest
	of the DataPump.

*/

/* declare UHIL_INTERRUPT_SYSTEM_IOCTL_FN as a standard IOCTL provider */
__TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(
	UHIL_INTERRUPT_SYSTEM_IOCTL_FN,
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE *
	);


/*

Type:	UHIL_ACQUIRE_ISR_INTERLOCK_FN

Index:	Type:	UHIL_RELEASE_ISR_INTERLOCK_FN

Function:
	Acquire / release the ISR interlock for an interrupt connection
	handle.

Definition:
	typedef UHIL_ISR_INTERLOCK_STATE
		(UHIL_ACQUIRE_ISR_INTERLOCK_FN)(
			UHIL_INTERRUPT_CONNECTION_HANDLE ConnectionHandle
			);

	typedef VOID
		(UHIL_RELEASE_ISR_INTERLOCK_FN)(
			UHIL_INTERRUPT_CONNECTION_HANDLE ConnectionHandle,
			UHIL_ISR_INTERLOCK_STATE SavedState
			);

Description:
	These functions are used to grab and a release an interlock that is
	specific to a given interrupt connection handle.  While holding the
	interlock, the caller is guaranteed that no other CPU will also
	acquire the interlock (and that no interrupt will occur on the
	current CPU that might allow the ISR to run on this CPU).

	Note the weasel words in the above. Specifically, the acquire call 
	does not guarantee that interrupts are disabled on this processor.
	However, the call does guarantee that the interrupt from the specific
	source can't occur on this processor, and that other processors
	will be prevented from driving into the ISR whiel the handle is held.

	This routine must not be called while holding the lock itself, nor
	should it be called while holding another lock. 

Returns:
	Functions of type UHIL_ACQUIRE_ISR_INTERLOCK_FN return an opaque 
	value, of type UHIL_ISR_INTERLOCK_STATE. This value must be used
	in the corresponding call to UHIL_RELEASE_ISR_INTERLOCK_FN.

Notes:
	This must be used carefully, as there may be no good way to check
	for situations such as locking against oneself.

	On uniprocessor systems, a default implementation is provided that
	uses UHIL_di() and UHIL_setpsw().

*/

__TMS_FNTYPE_DEF(
	UHIL_ACQUIRE_ISR_INTERLOCK_FN,
	__TMS_UHIL_ISR_INTERLOCK_STATE,
		(
		__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE
		)
	);

__TMS_FNTYPE_DEF(
	UHIL_RELEASE_ISR_INTERLOCK_FN,
	__TMS_VOID,
		(
		__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE,
		__TMS_UHIL_ISR_INTERLOCK_STATE
		)
	);

/*

Type:	UHIL_INTERRUPT_SYSTEM_INTERFACE

Function:
	The abstract interrupt system interface

Description:
	We wrap the entire abstract interrupt control system into
	a structure.  If there are (logically) multiple interrupt
	control systems in a single computer, then we would have 
	multiples of these.  Each chip port is responsible for using
	these in a "reasonable" way.  We provide no slot for the
	"interrupt system context"; instead we assume that this will
	be embedded, if necessary, in a larger structure, and that
	the interrupt subsystem will find the larger structure
	implicitly from the address of this structure.

Contents:
	UHIL_OPEN_INTERRUPT_CONNECTION_FN *pOpenInterruptConnection;
		Open an interrupt connection handle

	UHIL_CLOSE_INTERRUPT_CONNECTION_FN *pCloseInterruptConnection;
		Close an interrupt connection handle

	UHIL_CONNECT_TO_INTERRUPT_FN *pConnectToInterrupt;
		Connect an ISR to an interrupt

	UHIL_DISCONNECT_FROM_INTERRUPT_FN *pDisconnectFromInterrupt;
		Disconnects an ISR without releasing the interrupt handle

	UHIL_INTERRUPT_CONTROL_FN *pInterruptControl;
		Enable / disable interrupt

	UHIL_CONNECT_TO_INTERRUPT_V2_FN *pConnectToInterruptV2;
		Connect ISRs (line and message ISR) to an interrupt

	UHIL_INTERRUPT_SYSTEM_IOCTL_FN *pConnectionIoctl;
		Query an interrupt connection handle's properties.

	UHIL_INTERRUPT_SYSTEM_IOCTL_FN *pConnectionIoctl;
		Query an interrupt connection handle's properties.

	UHIL_ACQUIRE_ISR_INTERLOCK_FN *pAcquireIsrInterlock;
		Acquire the interlock for a given connection handle.

	UHIL_RELEASE_ISR_INTERLOCK_FN *pReleaseIsrInterlock;
		Release the interlock previously acquired for a given
		connection handle.

See Also:
	UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V3()
	UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V2()
	UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V1()
	UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V3()
	UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V2()
	UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V1()

*/

struct __TMS_STRUCTNAME(UHIL_INTERRUPT_SYSTEM_INTERFACE)
	{
	__TMS_UHIL_OPEN_INTERRUPT_CONNECTION_FN	*pOpenInterruptConnection;
	__TMS_UHIL_CLOSE_INTERRUPT_CONNECTION_FN *pCloseInterruptConnection;
	__TMS_UHIL_CONNECT_TO_INTERRUPT_FN	*pConnectToInterrupt;
	__TMS_UHIL_DISCONNECT_FROM_INTERRUPT_FN	*pDisconnectFromInterrupt;
	__TMS_UHIL_INTERRUPT_CONTROL_FN		*pInterruptControl;
	__TMS_UHIL_CONNECT_TO_INTERRUPT_V2_FN	*pConnectToInterruptV2;
	__TMS_UHIL_INTERRUPT_SYSTEM_IOCTL_FN	*pIoctl;
	__TMS_UHIL_ACQUIRE_ISR_INTERLOCK_FN	*pAcquireIsrInterlock;
	__TMS_UHIL_RELEASE_ISR_INTERLOCK_FN	*pReleaseIsrInterlock;
	};

/*

Macro:	UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V3()

Index:	Macro:	UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V2()
	Macro:	UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V1()

Function:
	Initialize (at compile time) an instance of type
	UHIL_INTERRUPT_SYSTEM_INTERFACE.

Definition:
	#include "uhilintsys.h"

	CONST UHIL_INTERRUPT_SYSTEM_INTERFACE thisIntSys =
	    UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V3(
		UHIL_OPEN_INTERRUPT_CONNECTION_FN  *pOpenInterruptConnection,
		UHIL_CLOSE_INTERRUPT_CONNECTION_FN *pCloseInterruptConnection,
		UHIL_CONNECT_TO_INTERRUPT_FN	   *pConnectToInterrupt, 
		UHIL_DISCONNECT_FROM_INTERRUPT_FN  *pDisconnectFromInterrupt,
		UHIL_INTERRUPT_CONTROL_FN	   *pInterruptControl,
		UHIL_CONNECT_TO_INTERRUPT_V2_FN	   *pConnectToInterruptV2,
		UHIL_INTERRUPT_SYSTEM_IOCTL_FN	   *pIoctl,
		UHIL_ACQUIRE_ISR_INTERLOCK_FN	   *pAcquireIsrInterlock,
		UHIL_RELEASE_ISR_INTERLOCK_FN	   *pReleaseIsrInterlock
		);

	CONST UHIL_INTERRUPT_SYSTEM_INTERFACE thisIntSys =
	    UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V2(
		UHIL_OPEN_INTERRUPT_CONNECTION_FN  *pOpenInterruptConnection,
		UHIL_CLOSE_INTERRUPT_CONNECTION_FN *pCloseInterruptConnection,
		UHIL_CONNECT_TO_INTERRUPT_FN	   *pConnectToInterrupt, 
		UHIL_DISCONNECT_FROM_INTERRUPT_FN  *pDisconnectFromInterrupt,
		UHIL_INTERRUPT_CONTROL_FN	   *pInterruptControl,
		UHIL_CONNECT_TO_INTERRUPT_V2_FN	   *pConnectToInterruptV2,
		UHIL_INTERRUPT_SYSTEM_IOCTL_FN	   *pIoctl
		);

	CONST UHIL_INTERRUPT_SYSTEM_INTERFACE thisIntSys =
	    UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V1(
		UHIL_OPEN_INTERRUPT_CONNECTION_FN  *pOpenInterruptConnection,
		UHIL_CLOSE_INTERRUPT_CONNECTION_FN *pCloseInterruptConnection,
		UHIL_CONNECT_TO_INTERRUPT_FN	   *pConnectToInterrupt, 
		UHIL_DISCONNECT_FROM_INTERRUPT_FN  *pDisconnectFromInterrupt,
		UHIL_INTERRUPT_CONTROL_FN	   *pInterruptControl
		);

Description:
	This family of macros is used to initialize a
	UHIL_INTERRUPT_SYSTEM_INTERFACE at compile time. The macros are
	versioned so that older code need not be updated if the API changes.

	The V1/V2 macros set pAcquireIsrInterlock and pReleaseIsrInterlock to
	UhilInterruptConnection_AcquireIsrLock_1cpu and 
	UhilInterruptConnection_ReleaseIsrLock_1cpu, respectively.

	The V1 macro sets pConnectToInterruptV2 and pIoctl to
	UsbPumpUhilInterruptSystem_ConnectToInterruptV2 and
	UsbPumpUhilInterruptSystem_Ioctl, respectively.

Returns:
	This macro returns a properly-braced initializer for an instance of
	type UHIL_INTERRUPT_SYSTEM_INTERFACE.

Notes:
	For run-time initialization, the similar functions
	UHIL_INTERRUPT_SYSTEM_SETUP_V3(), etc. should be used.

See also:
	UHIL_INTERRUPT_SYSTEM_SETUP_V3(), UHIL_INTERRUPT_SYSTEM_SETUP_V2(),
	UHIL_INTERRUPT_SYSTEM_SETUP_V1()

*/

#define __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V3(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl,				\
		/* v3 */ a_pAcquireIsrLock,			\
		/* v3 */ a_pReleaseIsrLock			\
		)						\
		{						\
		(a_pOpenInterruptConnection),			\
		(a_pCloseInterruptConnection),			\
		(a_pConnectToInterrupt),			\
		(a_pDisconnectFromInterrupt),			\
		(a_pInterruptControl),				\
		(a_pConnectToInterruptV2),			\
		(a_pIoctl),					\
		(a_pAcquireIsrLock),				\
		(a_pReleaseIsrLock)				\
		}

#define __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V2(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl				\
		)						\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V3(		\
		(a_pOpenInterruptConnection),			\
		(a_pCloseInterruptConnection),			\
		(a_pConnectToInterrupt),			\
		(a_pDisconnectFromInterrupt),			\
		(a_pInterruptControl),				\
		(a_pConnectToInterruptV2),			\
		(a_pIoctl),					\
		/* v3 */ UhilInterruptConnection_AcquireIsrLock_1cpu, \
		/* v3 */ UhilInterruptConnection_ReleaseIsrLock_1cpu \
		)
		

#define __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V1(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl				\
		)						\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V2(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ UsbPumpUhilInterruptSystem_ConnectToInterruptV2, \
		/* v2 */ UsbPumpUhilInterruptSystem_Ioctl		\
		)

/*

Name:	UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V3()

Index:	Name:	UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V2()
	Name:	UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V1()

Function:
	Initialize (at compile time) an instance of type
	UHIL_INTERRUPT_SYSTEM_INTERFACE.

Definition:
	#include "uhilintsys.h"

	VOID
	    UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V3(
		UHIL_INTERRUPT_SYSTEM_INTERFACE	   *pUhilIntSys,
		UHIL_OPEN_INTERRUPT_CONNECTION_FN  *pOpenInterruptConnection,
		UHIL_CLOSE_INTERRUPT_CONNECTION_FN *pCloseInterruptConnection,
		UHIL_CONNECT_TO_INTERRUPT_FN	   *pConnectToInterrupt, 
		UHIL_DISCONNECT_FROM_INTERRUPT_FN  *pDisconnectFromInterrupt,
		UHIL_INTERRUPT_CONTROL_FN	   *pInterruptControl,
		UHIL_CONNECT_TO_INTERRUPT_V2_FN	   *pConnectToInterruptV2,
		UHIL_INTERRUPT_SYSTEM_IOCTL_FN	   *pIoctl,
		UHIL_ACQUIRE_ISR_INTERLOCK_FN	   *pAcquireIsrInterlock,
		UHIL_RELEASE_ISR_INTERLOCK_FN	   *pReleaseIsrInterlock
		);

	VOID
	    UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V2(
		UHIL_INTERRUPT_SYSTEM_INTERFACE	   *pUhilIntSys,
		UHIL_OPEN_INTERRUPT_CONNECTION_FN  *pOpenInterruptConnection,
		UHIL_CLOSE_INTERRUPT_CONNECTION_FN *pCloseInterruptConnection,
		UHIL_CONNECT_TO_INTERRUPT_FN	   *pConnectToInterrupt, 
		UHIL_DISCONNECT_FROM_INTERRUPT_FN  *pDisconnectFromInterrupt,
		UHIL_INTERRUPT_CONTROL_FN	   *pInterruptControl,
		UHIL_CONNECT_TO_INTERRUPT_V2_FN	   *pConnectToInterruptV2,
		UHIL_INTERRUPT_SYSTEM_IOCTL_FN	   *pIoctl
		);

	VOID
	    UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V1(
		UHIL_INTERRUPT_SYSTEM_INTERFACE	   *pUhilIntSys,
		UHIL_OPEN_INTERRUPT_CONNECTION_FN  *pOpenInterruptConnection,
		UHIL_CLOSE_INTERRUPT_CONNECTION_FN *pCloseInterruptConnection,
		UHIL_CONNECT_TO_INTERRUPT_FN	   *pConnectToInterrupt, 
		UHIL_DISCONNECT_FROM_INTERRUPT_FN  *pDisconnectFromInterrupt,
		UHIL_INTERRUPT_CONTROL_FN	   *pInterruptControl
		);

Description:
	This family of macros is used to initialize a
	UHIL_INTERRUPT_SYSTEM_INTERFACE at run time. The macros are
	versioned so that older code need not be updated if the API changes.

	The V1/V2 macros set pAcquireIsrInterlock and pReleaseIsrInterlock to
	UhilInterruptConnection_AcquireIsrLock_1cpu and 
	UhilInterruptConnection_ReleaseIsrLock_1cpu, respectively.

	The V1 macro sets pConnectToInterruptV2 and pIoctl to
	UsbPumpUhilInterruptSystem_ConnectToInterruptV2 and
	UsbPumpUhilInterruptSystem_Ioctl, respectively.

Returns:
	No explicit result.

Notes:
	For compile-time initialization, the similar macros
	UHIL_INTERRUPT_SYSTEM_INIT_V3(), etc. should be used.

See also:
	UHIL_INTERRUPT_SYSTEM_INIT_V3(), UHIL_INTERRUPT_SYSTEM_INIT_V2(),
	UHIL_INTERRUPT_SYSTEM_INIT_V1()

*/


#define __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V3(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl,				\
		/* v3 */ a_pAcquireIsrLock,			\
		/* v3 */ a_pReleaseIsrLock			\
		)						\
	do	{						\
		(a_pUhilIntSys)->pOpenInterruptConnection =	\
			(a_pOpenInterruptConnection);		\
		(a_pUhilIntSys)->pCloseInterruptConnection =	\
			(a_pCloseInterruptConnection);		\
		(a_pUhilIntSys)->pConnectToInterrupt =		\
			(a_pConnectToInterrupt);		\
		(a_pUhilIntSys)->pDisconnectFromInterrupt =	\
			(a_pDisconnectFromInterrupt);		\
		(a_pUhilIntSys)->pInterruptControl =		\
			(a_pInterruptControl);			\
		(a_pUhilIntSys)->pConnectToInterruptV2 =	\
			(a_pConnectToInterruptV2);		\
		(a_pUhilIntSys)->pIoctl =			\
			(a_pIoctl);				\
		(a_pUhilIntSys)->pAcquireIsrInterlock =		\
			(a_pAcquireIsrLock);			\
		(a_pUhilIntSys)->pReleaseIsrInterlock =		\
			(a_pReleaseIsrLock);			\
		} while (0)

#define __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V2(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl				\
		)						\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V3(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		a_pConnectToInterruptV2,			\
		a_pIoctl,					\
		/* v3 */ UhilInterruptConnection_AcquireIsrLock_1cpu, \
		/* v3 */ UhilInterruptConnection_ReleaseIsrLock_1cpu \
		)

#define __TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V1(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl				\
		)						\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V2(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ UsbPumpUhilInterruptSystem_ConnectToInterruptV2, \
		/* v2 */ UsbPumpUhilInterruptSystem_Ioctl		\
		)


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* a default connect handler, assuming a uniprocessor-only HIL */
__TMS_UHIL_CONNECT_TO_INTERRUPT_V2_FN
	UsbPumpUhilInterruptSystem_ConnectToInterruptV2;

__TMS_UHIL_INTERRUPT_SYSTEM_IOCTL_FN
	UsbPumpUhilInterruptSystem_Ioctl;

__TMS_UHIL_ACQUIRE_ISR_INTERLOCK_FN
	UhilInterruptConnection_AcquireIsrLock_1cpu;

__TMS_UHIL_RELEASE_ISR_INTERLOCK_FN
	UhilInterruptConnection_ReleaseIsrLock_1cpu;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UHIL_MP_INTERRUPT_SYSTEM_RESULT_PROCESSED	\
   __TMS_UHIL_MP_INTERRUPT_SYSTEM_RESULT_PROCESSED
# define UHIL_MP_INTERRUPT_SYSTEM_RESULT_WAS_PROCESSED(s)	\
   __TMS_UHIL_MP_INTERRUPT_SYSTEM_RESULT_WAS_PROCESSED(s)
# define UHIL_MP_INTERRUPT_SYSTEM_RESULT_GET_EVENT(s)	\
   __TMS_UHIL_MP_INTERRUPT_SYSTEM_RESULT_GET_EVENT(s)
# define UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V3(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl,				\
		/* v3 */ a_pAcquireIsrLock,			\
		/* v3 */ a_pReleaseIsrLock			\
		)	\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V3(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl,				\
		/* v3 */ a_pAcquireIsrLock,			\
		/* v3 */ a_pReleaseIsrLock			\
		)
# define UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V2(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl				\
		)	\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V2(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl				\
		)
# define UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V1(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl				\
		)	\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_INIT_V1(		\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl				\
		)
# define UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V3(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl,				\
		/* v3 */ a_pAcquireIsrLock,			\
		/* v3 */ a_pReleaseIsrLock			\
		)	\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V3(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl,				\
		/* v3 */ a_pAcquireIsrLock,			\
		/* v3 */ a_pReleaseIsrLock			\
		)
# define UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V2(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl				\
		)	\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V2(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl,				\
		/* v2 */ a_pConnectToInterruptV2,		\
		/* v2 */ a_pIoctl				\
		)
# define UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V1(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl				\
		)	\
	__TMS_UHIL_INTERRUPT_SYSTEM_INTERFACE_SETUP_V1(		\
		a_pUhilIntSys,					\
		a_pOpenInterruptConnection,			\
		a_pCloseInterruptConnection,			\
		a_pConnectToInterrupt,				\
		a_pDisconnectFromInterrupt,			\
		a_pInterruptControl				\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of uhilintsys.h ****/
#endif /* _UHILINTSYS_H_ */
