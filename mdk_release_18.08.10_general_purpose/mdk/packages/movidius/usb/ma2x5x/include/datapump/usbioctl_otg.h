/* usbioctl_otg.h	Mon Jun  1 2015 00:19:45 tmm */

/*

Module:  usbioctl_otg.h

Function:
	IOCTLs specificially for OTG work

Version:
	V3.15b	Mon Jun  1 2015 00:19:45 tmm	Edit level 5

Copyright notice:
	This file copyright (C) 2005-2007, 2011, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Tue Aug 23 2005 04:28:08  tmm
	Module created.

   1.91h  Thu Sep  1 2005 23:58:19  tmm
	Take out reference to HCD types.

   1.97j  Tue Nov 13 2007 15:56:00  chwon
	3471: add USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED

   3.01f  Tue Nov 08 2011 16:40:44  chwon
	9941: add USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE

   3.15b  Mon Jun  1 2015 00:19:45  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _USBIOCTL_OTG_H_		/* prevent multiple includes */
#define _USBIOCTL_OTG_H_

/*
|| we need the base DataPump types
*/
#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMPIOCTLS_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _OTGFSM_TYPES_H_
# include "otgfsm_types.h"
#endif

/****************************************************************************\
|
|	Define helper macros.
|
\****************************************************************************/

/**** helper macro for non-VOID OTG in-calls ****/
#define __TMS_USBPUMP_IOCTL_OTG(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_OTG,				\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_OTGCD_, NAME, _ARG)	\
		)

/****************************************************************************\
|
|	The OTG ioctl codes
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_OTGCD_ENABLE_HNP				\
	__TMS_USBPUMP_IOCTL_OTG(0, W, ENABLE_HNP)

#define	__TMS_USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ				\
	__TMS_USBPUMP_IOCTL_OTG(1, W, A_SUSPEND_REQ)
	
#define	__TMS_USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST				\
	__TMS_USBPUMP_IOCTL_OTG(2, W, A_BUS_REQUEST)

#define	__TMS_USBPUMP_IOCTL_OTGCD_A_BUS_DROP				\
	__TMS_USBPUMP_IOCTL_OTG(3, W, A_BUS_DROP)

#define	__TMS_USBPUMP_IOCTL_OTGCD_A_CLR_ERR				\
	__TMS_USBPUMP_IOCTL_VOID(__TMS_USBPUMP_IOCTL_CLASS_OTG, 4)

#define	__TMS_USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST				\
	__TMS_USBPUMP_IOCTL_OTG(5, W, B_BUS_REQUEST)

#define	__TMS_USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER			\
	__TMS_USBPUMP_IOCTL_OTG(6, W, SET_B_CONNECT_TIMER)

#define	__TMS_USBPUMP_IOCTL_OTGCD_GET_STATE_ASYNC			\
	__TMS_USBPUMP_IOCTL_OTG(7, R_ASYNC, GET_STATE)

#define	__TMS_USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED			\
	__TMS_USBPUMP_IOCTL_OTG(8, W, ENABLE_SRP_BASED)

#define	__TMS_USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE			\
	__TMS_USBPUMP_IOCTL_OTG(9, W, SET_TIMER_VALUE)

/****************************************************************************\
|
|	The IOCTL arguments and documentation
|
\****************************************************************************/


/*

IOCTL:	USBPUMP_IOCTL_OTGCD_ENABLE_HNP

Index:	Type:	USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG
	Type:	PUSBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG
	Name:	USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG_SETUP_V1

Function:
	When device is in host (a) role, enable HNP at next release.

Input:
	This IOCTL takes a parameter of type pointer-to-
	USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG.  The structure contains the
	following fields:

	BOOL fEnable;
		If logically TRUE, HNP will be enabled on next release of
		the bus by the machine.  If logically FALSE, HNP will not
		be enabled on the next release of the bus.

Output:
	No output parameters.

Description:
	Tells the transceiver (which must be in host mode) to prepare for 
	HNP when the HCD releases the port.  Essentially the same as setting 
	a_set_b_hnp_en in the OTG state machine.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.

Setup Macro:
	VOID USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG_SETUP_V1(
		USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG *pArg,
		BOOL fEnable
		);

	Initiailizes the parameter block for the IOCTL.  MCCI guarantees
	that if we add a parameter to this IOCTL, we'll do it in an upward
	compatible function so that old source code using the V1 macro
	won't have to be edited (though it may have to be recompiled).  New
	functionality will be available via a V2, etc. macro.

Notes:
	This is synchronous, but it simply sets a state variable and attempts
	to update the OTG FSM.  So some of the activities initiated by this
	IOCTL will play out asynchronously to the caller.

*/


__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG)
	{
	__TMS_BOOL	fEnable;
	};

#define	__TMS_USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)								\
   do	{								\
	(a_pArg)->fEnable = (a_fEnable);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ

Index:	Type:	USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ_ARG
	Name:	USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ_ARG_SETUP_V1()

Function:
	When operating as a host, requests that the bus be suspended (or
	resumed).

Input:
	BOOL fState;
		If logically TRUE, a suspend request is isused to the OTG
		fsm.  Otherwise, the suspend request is cleared.

Output:
	No output variables.

Description:
	Set (or clear) the OTG FSM's a_suspend_req variable to request that 
	the host bus be suspended (or not). 

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.

Setup Macro:
	VOID USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG_SETUP_V1(
		USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ_ARG *pArg,
		BOOL fEnable
		);

	Initiailizes the parameter block for the IOCTL.  MCCI guarantees
	that if we add a parameter to this IOCTL, we'll do it in an upward
	compatible function so that old source code using the V1 macro
	won't have to be edited (though it may have to be recompiled).  New
	functionality will be available via a V2, etc. macro.

Notes:
	This is synchronous, but it simply sets a state variable and attempts
	to update the OTG FSM.  So some of the activities initiated by this
	IOCTL will play out asynchronously to the caller.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ_ARG)
	{
	__TMS_BOOL	fState;
	};

#define	__TMS_USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fState							\
	)								\
   do	{								\
	(a_pArg)->fState = (a_fState);					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST

Index:	Type:	USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG
	Type:	PUSBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG
	Name:	USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG_SETUP_V1()

Function:
	Request a bus session as an A-role host.

Input:
	Takes a pointer to a USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG structure,
	containing the following fields:

	BOOL fEnable;
		If logically TRUE, then the OTG FSM is instructed that
		the high-level app is requesting an A-bus session.  Otherwise,
		the high-level app is not requesting an A-bus session, and
		the bus will stay in idle (or will be suspended, depending
		on the details of its current state).  Negating this after
		it has been set and the FSM is not in the a_idle state
		will lead to the a_suspend state rather than back to a_idle.


Output:
	No output parameters.

Description:
	Tells the OTG state machine that the upper layers are requesting a 
	bus session.  This directly kicks the finite state machine by setting
	the a_bus_req state flag.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.

Setup Macro:
	VOID USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG_SETUP_V1(
		USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG *pArg,
		BOOL fEnable
		);

	Initializes the parameter block for the IOCTL.  MCCI guarantees
	that if we add a parameter to this IOCTL, we'll do it in an upward
	compatible function so that old source code using the V1 macro
	won't have to be edited (though it may have to be recompiled).  New
	functionality will be available via a V2, etc. macro.

Notes:
	This request is without immediate effect if the FSM is not in an a_* 
	state.  

	Negating request is not the same as setting drop -- only setting 
	drop will get us back to a_idle.

	This is synchronous, but it simply sets a state variable and attempts
	to update the OTG FSM.  So some of the activities initiated by this
	IOCTL will play out asynchronously to the caller.

	It's perfectly reasonable to initialize a_bus_req TRUE, a_bus_drop
	FALSE, and b_bus_req FALSE.  In this case, the FSM will simply
	alternate between host and device mode, depending on what's plugged
	in (mini A or B) and what the other side does.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG)
	{
	__TMS_BOOL	fEnable;
	};

#define	__TMS_USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)								\
   do	{								\
	(a_pArg)->fEnable = (a_fEnable);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_A_BUS_DROP

Index:	Type:	USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG
	Type:	PUSBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG
	Name:	USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG_SETUP_V1()

Function:
	Prevent activation of VBUS by OTG device when in an A-Role.

Input:
	This IOCTL takes a pointer to a paramter of type 
	USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG, containing the following fields.

	BOOL fDrop;
		If logically TRUE, forces the FSM from any a_* state to
		a_idle, where the FSM will remain until the connector
		is changed or until another USBPUMP_IOCTL_OTGCD_A_BUS_DROP
		is issued to clear the request.

Output:
	No output fields.

Description:
	This IOCTL directly controls the a_bus_drop state input of the FSM
	OTG state machine.  If a_bus_drop is set, and an A-type connector
	is plugged in, then the FSM will always remain in the a_idle state,
	which keeps Vbus turned off.  If A-type connector is /not/ plugged
	in, then this state variable has no effect.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.

Setup Macro:
	VOID USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG_SETUP_V1(
		USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG *pArg,
		BOOL fDrop
		);

	Initializes the parameter block for the IOCTL.  MCCI guarantees
	that if we add a parameter to this IOCTL, we'll do it in an upward
	compatible function so that old source code using the V1 macro
	won't have to be edited (though it may have to be recompiled).  New
	functionality will be available via a V2, etc. macro.

Notes:
	This IOCTL is without immediate effect if the device is not in
	an a_* state.

	This is synchronous, but it simply sets a state variable and attempts
	to update the OTG FSM.  So some of the activities initiated by this
	IOCTL will play out asynchronously to the caller.

	Sending this request when fDrop is set will cause the FSM to 
	transition about of the a_vbus_err state.

See Also:
	USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST

*/


__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG)
	{
	__TMS_BOOL	fDrop;
	};

#define	__TMS_USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fDrop								\
	)								\
   do	{								\
	(a_pArg)->fDrop = (a_fDrop);					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_A_CLR_ERR

Function:
	Tells the OTG FSM to clear any current Vbus error state.

Input:
	This IOCTL doesn't take any input parameters.

Output:
	This IOCTL has no output parameters.

Description:
	When in the a_vbus_err state, a client may send this IOCTL to force
	the FSM out of the error state, without having to worry about
	manipulating a_bus_drop.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.

Notes:
	This IOCTL has no effect if the FSM is not in the a_vbus_err state.

*/

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST

Index:	Type:	USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG
	Type:	PUSBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG
	Name:	USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG_SETUP_V1()

Function:
	Tell the FSM that it should attempt a B-bus session if given the
	chance.

Input:
	This IOCTL takes a pointer to a USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG
	structure, with the following relevant fields.

	BOOL fEnable;
		If logically TRUE, then the upper level would like to
		run a bus session if it becomes possible.

Output:
	There are no output fields.

Description:
	The client shall use this IOCTL to set fEnable TRUE while the
	application wishes to perform operations as a host when connected
	as a B-device.  If the B-device is in b_idle state, it will do an
	SRP to try to get the A-device to run a session, and then if the
	A devices enables HNP and grants control to the B device, the
	FSM will enable the B device to operate as a host, until such time
	as the B device decides to reset fEnable.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.

Setup Macro:
	VOID USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG_SETUP_V1(
		USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG *pArg,
		BOOL fEnable
		);

Notes:
	This IOCTL has no effect apart from causing SRP if the other side 
	doesn't enable HNP; it therefore probably has no effect if the 
	descriptors don't include HNP support; and nothing will happen 
	(session-wise) until the other side ends its session.

	In addition, SRP should not be attempted unless the device has
	an OTG descriptor that says the device actually does SRP.

*/


__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG)
	{
	__TMS_BOOL	fEnable;
	};

#define	__TMS_USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)								\
   do	{								\
	(a_pArg)->fEnable = (a_fEnable);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER

Index:	Type:	USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG
	Type:	PUSBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG
	Name:	USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG_SETUP_V1()
	Const:	USBPUMP_OTGCD_B_CONNECT_TIMER_MIN
	Const:	USBPUMP_OTGCD_B_CONNECT_TIMER_INDEFINITE

Function:
	Set the OTG connect timeout value (used in A role).

Input:
	This IOCTL's argument is a USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG
	structure, with the following relevant fields.

	ULONG ulMilliseconds;
		Specifies the timeout, which may be zero (meaning 'no
		timeout') or a number greater than or equal to 1000,
		giving the timeout in milliseconds.

	For convenience, the symbolic constant
	USBPUMP_OTGCD_B_CONNECT_TIMER_MIN gives the minimum permitted connect 
	time; and USBPUMP_OTGCD_B_CONNECT_TIMER_INDEFINITE is the value
	to use to request indefinite wait for the B-device.

Output:
	No output fields.

Description:
	When the OTG port is in state a_wait_bcon, per section 6.8.1.3
	of the spec, the A device is allowed to wait any period of time greater
	than 1.0 seconds for the B device to connect.  No one timer value
	is correct; on some systems, the minimum will be chosen in order to
	preserve power; in others, an indefinite value will be chosen in
	order to allow "fly lights" and similar accessories that use
	USB power.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success,

	USBPUMP_IOCTL_RESULT_INVALID_PARAMETER if the value is not in the
	supported range, or some other value indicating that there's some 
	kind of system plumbing problem.

Setup Macro:
	VOID USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG_SETUP_V1(
		USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG *pArg,
		ULONG ulMilliseconds
		);

Notes:
	Changing the timeout while the FSM is already in state a_wait_bcon
	has no effect until the next time the FSM transitions into the state.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG)
	{
	__TMS_ULONG	ulMilliseconds;
	};

#define	__TMS_USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG_SETUP_V1(	\
	a_pArg,								\
	a_ulMilliseconds						\
	)								\
   do	{								\
	(a_pArg)->ulMilliseconds = (a_ulMilliseconds);			\
	} while (0)

#define	__TMS_USBPUMP_OTGCD_B_CONNECT_TIMER_MIN		1000ul
#define	__TMS_USBPUMP_OTGCD_B_CONNECT_TIMER_INDEFINITE	0

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_GET_STATE_ASYNC

Index:	Type:	USBPUMP_IOCTL_OTGCD_GET_STATE_ARG
	Type:	PUSBPUMP_IOCTL_OTGCD_GET_STATE_ARG

Function:
	Return a snapshot of the OTG state.

Input:
	pInParam	always NULL
	pOutParam	points to a USBPUMP_IOCTL_OTGCD_GET_STATE_ARG
			structure.

Output:
	The USBPUMP_IOCTL_OTGCD_GET_STATE_ARG will have the following output
	fields:

	USBPUMP_OTGSTATE OtgState;
		Set to the current state.

	USBPUMP_OTGVARS OtgVars;
		Set to the vector of state variables.

	UINT32 ulConnectTimer;
		Set to the value currently used for waiting for the B device
		to connect, in millisec.

Description:
	This IOCTL gives upper layers a way to find out what is going on
	in the DCD/HCD/transceiver cloud.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if the state could be found;
	some other result for failure, generally indicating that the OTG
	FSM is not present in the stack.

Setup Macro:
	No setup macro is needed

Notes:
	This is asynch because the data might reside remotely.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_GET_STATE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_GET_STATE_ARG)
	{
	__TMS_USBPUMP_OTGVARS	OtgVars;
	__TMS_UINT32		ulConnectTimer;
	};

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED

Index:	Type:	USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG
	Type:	PUSBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG
	Name:	USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG_SETUP_V1()

Function:
	Set SRP based or Insertion based mode of OTG device when in an A-Role.

Input:
	This IOCTL takes a pointer to a paramter of type 
	USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG, containing the following fields.

	BOOL fEnable;
		If logically TRUE, the FSM will act as SRP based OTG device.
		If it is FALSE, the FSM will act as insertion based OTG device.

Output:
	No output fields.

Description:
	This IOCTL directly controls the state transition between a_idle and
	a_wait_vrise.  On the SRP based mode, FSM will transit from a_idle state
	to a_wait_vrise state only if a_srp_det is TRUE.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success -- which will always be
	the result unless there's some kind of system plumbing problem.

Setup Macro:
	VOID USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG_SETUP_V1(
		USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG *pArg,
		BOOL fEnable
		);

	Initializes the parameter block for the IOCTL.  MCCI guarantees
	that if we add a parameter to this IOCTL, we'll do it in an upward
	compatible function so that old source code using the V1 macro
	won't have to be edited (though it may have to be recompiled).  New
	functionality will be available via a V2, etc. macro.

Notes:
	This IOCTL will not be immediate effect if the device is already in an
	a_host state.

*/


__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG)
	{
	__TMS_BOOL	fEnable;
	};

#define	__TMS_USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fEnable							\
	)								\
   do	{								\
	(a_pArg)->fEnable = (a_fEnable);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE

Index:	Type:	USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG
	Type:	PUSBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG
	Name:	USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG_SETUP_V1()
	Const:	USBPUMP_OTGCD_B_DISCONNECT_TIMER_MIN
	Const:	USBPUMP_OTGCD_B_DISCONNECT_TIMER_INDEFINITE

Function:
	Set the OTG FSM timeout values.

Input:
	This IOCTL's argument is a USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG
	structure, with the following relevant fields.

	ULONG ta_wait_bcon;
		Specifies the timeout for a_wait_bcon_tmr in milliseconds.

	ULONG ta_aidl_bdis;
		Specifies the timeout for a_aidl_bdis_tmr in milliseconds.

	ULONG ta_bidl_adis;
		Specifies the timeout for a_bidl_adis_tmr in milliseconds.

	ULONG ta_wait_vfail;
		Specifies the timeout for a_wait_vfail_tmr in milliseconds.

	ULONG tb_ase0_brst;
		Specifies the timeout for b_ase0_brst_tmr in milliseconds.

	For convenience, the symbolic constant
	USBPUMP_OTGCD_SET_TIMER_KEEP_CURRENT is the value to use current
	timeout value; and USBPUMP_OTGCD_SET_TIMER_INDEFINITE is the value
	to use to request indefinite wait.

Output:
	No output fields.

Description:
	When the OTG port is in state a_suspend, per section 7.4.5.4 of the
	V2.0 spec, the A device is allowed to wait any period of time greater
	than 200 milliseconds for the B device to disconnect.
	This a_aidl_bdis_tmr is started by an A-device when it enters the
	a_suspend state if HNP has been enabled (a_set_b_hnp_en = TRUE).

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success,

	USBPUMP_IOCTL_RESULT_INVALID_PARAMETER if the value is not in the
	supported range, or some other value indicating that there's some 
	kind of system plumbing problem.

Setup Macro:
	VOID USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG_SETUP_V1(
		USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG *pArg,
		ULONG ta_wait_bcon,
		ULONG ta_aidl_bdis,
		ULONG ta_bidl_adis,
		ULONG ta_wait_vfail,
		ULONG tb_ase0_brst
		);

Notes:
	Changing the timeout while the FSM is already in state a_suspend
	has no effect until the next time the FSM transitions into the state.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG)
	{
	__TMS_ULONG	ta_wait_bcon;
	__TMS_ULONG	ta_aidl_bdis;
	__TMS_ULONG	ta_bidl_adis;
	__TMS_ULONG	ta_wait_vfail;
	__TMS_ULONG	tb_ase0_brst;
	};

#define	__TMS_USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG_SETUP_V1(		\
	a_pArg,								\
	a_ta_wait_bcon,							\
	a_ta_aidl_bdis,							\
	a_ta_bidl_adis,							\
	a_ta_wait_vfail,						\
	a_tb_ase0_brst							\
	)								\
   do	{								\
	(a_pArg)->ta_wait_bcon = (a_ta_wait_bcon);			\
	(a_pArg)->ta_aidl_bdis = (a_ta_aidl_bdis);			\
	(a_pArg)->ta_bidl_adis = (a_ta_bidl_adis);			\
	(a_pArg)->ta_wait_vfail = (a_ta_wait_vfail);			\
	(a_pArg)->tb_ase0_brst = (a_tb_ase0_brst);			\
	} while (0)

#define	__TMS_USBPUMP_OTGCD_SET_TIMER_KEEP_CURRENT	0xFFFFFFFF
#define	__TMS_USBPUMP_OTGCD_SET_TIMER_INDEFINITE	0

#define	__TMS_USBPUMP_OTGCD_TA_WAIT_BCON_MIN_V1		1000	/* 1 sec */
#define	__TMS_USBPUMP_OTGCD_TA_WAIT_BCON_MIN_V2		1100	/* 1.1 sec */
#define	__TMS_USBPUMP_OTGCD_TA_WAIT_BCON_MAX_V2		30000	/* 30 sec */

#define	__TMS_USBPUMP_OTGCD_TA_AIDL_BDIS_MIN		200	/* 200 msec */

#define	__TMS_USBPUMP_OTGCD_TA_BIDL_ADIS_MIN		155	/* 155 msec */
#define	__TMS_USBPUMP_OTGCD_TA_BIDL_ADIS_MAX		200	/* 200 msec */

#define	__TMS_USBPUMP_OTGCD_TA_WAIT_VFAIL_MAX		1000	/* 1 sec */

#define	__TMS_USBPUMP_OTGCD_TB_ASE0_BRST_MIN_V1		4	/* 4 msec */
#define	__TMS_USBPUMP_OTGCD_TB_ASE0_BRST_MIN_V2		155	/* 155 msec */


/****************************************************************************\
|
|	uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_OTG(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_OTG(i, t, NAME)
# define USBPUMP_IOCTL_OTGCD_ENABLE_HNP	\
   __TMS_USBPUMP_IOCTL_OTGCD_ENABLE_HNP
# define USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ	\
   __TMS_USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ
# define USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST	\
   __TMS_USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST
# define USBPUMP_IOCTL_OTGCD_A_BUS_DROP	\
   __TMS_USBPUMP_IOCTL_OTGCD_A_BUS_DROP
# define USBPUMP_IOCTL_OTGCD_A_CLR_ERR	\
   __TMS_USBPUMP_IOCTL_OTGCD_A_CLR_ERR
# define USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST	\
   __TMS_USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST
# define USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER	\
   __TMS_USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER
# define USBPUMP_IOCTL_OTGCD_GET_STATE_ASYNC	\
   __TMS_USBPUMP_IOCTL_OTGCD_GET_STATE_ASYNC
# define USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED	\
   __TMS_USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED
# define USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE	\
   __TMS_USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE
# define USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_OTGCD_ENABLE_HNP_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)
# define USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fState							\
	)	\
	__TMS_USBPUMP_IOCTL_OTGCD_A_SUSPEND_REQ_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fState							\
	)
# define USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_OTGCD_A_BUS_REQUEST_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)
# define USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fDrop								\
	)	\
	__TMS_USBPUMP_IOCTL_OTGCD_A_BUS_DROP_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fDrop								\
	)
# define USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_OTGCD_B_BUS_REQUEST_ARG_SETUP_V1(		\
	a_pArg,								\
	a_fEnable							\
	)
# define USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG_SETUP_V1(	\
	a_pArg,								\
	a_ulMilliseconds						\
	)	\
	__TMS_USBPUMP_IOCTL_OTGCD_SET_B_CONNECT_TIMER_ARG_SETUP_V1(	\
	a_pArg,								\
	a_ulMilliseconds						\
	)
# define USBPUMP_OTGCD_B_CONNECT_TIMER_MIN	\
   __TMS_USBPUMP_OTGCD_B_CONNECT_TIMER_MIN
# define USBPUMP_OTGCD_B_CONNECT_TIMER_INDEFINITE	\
   __TMS_USBPUMP_OTGCD_B_CONNECT_TIMER_INDEFINITE
# define USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fEnable							\
	)	\
	__TMS_USBPUMP_IOCTL_OTGCD_ENABLE_SRP_BASED_ARG_SETUP_V1(	\
	a_pArg,								\
	a_fEnable							\
	)
# define USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG_SETUP_V1(		\
	a_pArg,								\
	a_ta_wait_bcon,							\
	a_ta_aidl_bdis,							\
	a_ta_bidl_adis,							\
	a_ta_wait_vfail,						\
	a_tb_ase0_brst							\
	)	\
	__TMS_USBPUMP_IOCTL_OTGCD_SET_TIMER_VALUE_ARG_SETUP_V1(		\
	a_pArg,								\
	a_ta_wait_bcon,							\
	a_ta_aidl_bdis,							\
	a_ta_bidl_adis,							\
	a_ta_wait_vfail,						\
	a_tb_ase0_brst							\
	)
# define USBPUMP_OTGCD_SET_TIMER_KEEP_CURRENT	\
   __TMS_USBPUMP_OTGCD_SET_TIMER_KEEP_CURRENT
# define USBPUMP_OTGCD_SET_TIMER_INDEFINITE	\
   __TMS_USBPUMP_OTGCD_SET_TIMER_INDEFINITE
# define USBPUMP_OTGCD_TA_WAIT_BCON_MIN_V1	\
   __TMS_USBPUMP_OTGCD_TA_WAIT_BCON_MIN_V1
# define USBPUMP_OTGCD_TA_WAIT_BCON_MIN_V2	\
   __TMS_USBPUMP_OTGCD_TA_WAIT_BCON_MIN_V2
# define USBPUMP_OTGCD_TA_WAIT_BCON_MAX_V2	\
   __TMS_USBPUMP_OTGCD_TA_WAIT_BCON_MAX_V2
# define USBPUMP_OTGCD_TA_AIDL_BDIS_MIN	\
   __TMS_USBPUMP_OTGCD_TA_AIDL_BDIS_MIN
# define USBPUMP_OTGCD_TA_BIDL_ADIS_MIN	\
   __TMS_USBPUMP_OTGCD_TA_BIDL_ADIS_MIN
# define USBPUMP_OTGCD_TA_BIDL_ADIS_MAX	\
   __TMS_USBPUMP_OTGCD_TA_BIDL_ADIS_MAX
# define USBPUMP_OTGCD_TA_WAIT_VFAIL_MAX	\
   __TMS_USBPUMP_OTGCD_TA_WAIT_VFAIL_MAX
# define USBPUMP_OTGCD_TB_ASE0_BRST_MIN_V1	\
   __TMS_USBPUMP_OTGCD_TB_ASE0_BRST_MIN_V1
# define USBPUMP_OTGCD_TB_ASE0_BRST_MIN_V2	\
   __TMS_USBPUMP_OTGCD_TB_ASE0_BRST_MIN_V2
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_otg.h ****/
#endif /* _USBIOCTL_OTG_H_ */
