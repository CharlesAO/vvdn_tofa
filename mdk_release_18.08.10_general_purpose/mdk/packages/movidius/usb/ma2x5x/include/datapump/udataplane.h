/* udataplane.h		Mon Jun  1 2015 00:19:06 tmm */

/*

Module:  udataplane.h

Function:
	Home for the UDATAPLANE structure and associated substructures.

Version:
	V3.15b	Mon Jun  1 2015 00:19:06 tmm	Edit level 17

Copyright notice:
	This file copyright (C) 2002-2004, 2006-2007, 2011-2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Thu May 30 2002 12:58:09  tmm
	Module created from mcpcdata.h 1.61j

   1.81a  Wed Feb 19 2003 23:24:00  tmm
	Bug 999, 1000:  add another UEVENT_NODE so we can wire up power events
	correctly.

   1.87a  Sun Nov  2 2003 00:32:48  tmm
	Autodoc improvements.

   1.87b  Tue Apr 27 2004 11:37:36  maw
   	Bug 1239: Added UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN to outswitch

   1.97e  Tue Jul 11 2006 10:51:25  tmm
	Correct documentation (UDATAPLANE_NODE ==> UDATAPLANE)

   1.97e  Mon Aug 07 2006 19:44:02  tmm
	Bug 1891: correct UDATAPLANE_OUTSWITCH_SETUP_V1() broken by bug 1239
	fix.

   1.97e  Mon Aug 14 2006 20:05:05  tmm
	Bug 1911:  add UsbPumpDataPlaneI_DisconnectStreams().

   1.97g  Wed Mar 14 2007 20:25:04  maw
	Bug 2516: add UsbPumpDataPlane_Delete()

   1.97j  Tue Oct 02 2007 16:35:38  chwon
	3253: add pDeviceSetupPacket and pFeatureEvent to outswitch

   3.01f  Fri Jul 01 2011 14:02:15  maw
	13233: Add UsbPumpDataPlane_ActivateEndpoints().

   3.01f  Tue Dec 13 2011 15:07:36  chwon
	14582: Add UsbPumpDataPlane_Deinit().

   3.01f  Wed Jan 18 2012 15:27:18  chwon
	14623: Added fDataPlaneSuspend and fRemoteWakeupEnable flags.

   3.01f  Fri Feb 03 2012 14:04:01  chwon
	14572: Added pVendorSetupPacket to outswitch.

   3.01f  Fri Feb 03 2012 15:29:28  chwon
	14799: Added UsbPumpDataPlane_ReconnectStreams().

   3.11b  Mon Jan 07 2013 13:34:23  chwon
	16431: Changed UDEVICE_OUTSWITCH to UDATAPLANE_OUTSWITCH.
	16432: Updated UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN description and
	added UDATAPLANE_OUTSWITCH::pVendorSetupPacket description.
	16402: Added UDATAPLANE_OUTSWITCH_IOCTL_FN in the UDATAPLANE_OUTSWITCH
	and added V5 init and setup macro.

   3.15b  Mon Jun  1 2015 00:19:06  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _UDATAPLANE_H_		/* prevent multiple includes */
#define _UDATAPLANE_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _UEVENTNODE_H_
# include "ueventnode.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UDATAPLANE_OUTSWITCH);

/*

Type:	UDATAPLANE

Function:
	C structure type; represents a collection of equivalent interfaces
	that are used for data transfers.

Description:
	A dataplane represents a collection of interface settings,
	possibly collected from multiple configurations.  In such a
	collection, only one such setting may be active at a given time.

	|UDATAPLANE|s provide a uniform means of managing this kind of
	complexity for protocol and |UDEVFUNCTION| implementations.  Each
	|UINTERFACE| may be associated with at most one |UDATAPLANE|; the
	DataPump core takes care to deliver any interface level messages to
	the |UDATAPLANE|s as well.

	|UDATAPLANE|s may be used for control plane management (in
	cases such as Communication Device Class, where the control plane
	is associated with a separate interface).

	|UDATAPLANE|s also contain pointers to |UDATASTREAM| objects.
	These are created by the client protocol to represent individual
	logical data streams.

Contents:
	|USBPUMP_OBJECT_HEADER Header;|
		A standard USB DataPump object header.  This allows loosely
		coupled clients to locate this dataplane node.

	|CONST UDATAPLANE_OUTSWITCH *pOutSwitch;|
		A pointer to the outswitch; initially |NULL|.  A (single)
		client can change this to point to a function table for
		additional processing.  Usually used by the |UDATASTREAM|
		layer.

	|VOID *pClientContext;|
		A cell, reserved for use by the owner of the
		|UDATAPLANE_OUTSWITCH| for communicating context upstream.
		Provided for completeness.

	|UDEVICE *pDevice;|
		Pointer to the parent |UDEVICE|.  This is saved for efficiency.

	|UINTERFACE *pCurrentInterface;|
		Pointer to the unique |UINTERFACE| that is active for this
		|UDATAPLANE|.  If |NULL|, no such interface is active.

	|UINTERFACE *pInterfaceListHead;|
		Pointer to the head of the circularly linked list of
		|UINTERFACES| that are grouped into this node. Each
		|UINTERFACE| is linked using the |uifc_pFunctionIfcNext|
		and |uifc_pFunctionIfcLast| fields.
		The |UINTERFACE| are linked in order of discovery.  A circular
		doubly linked list is used for consistency with the rest
		of the DataPump code.

	|UEVENTNODE *pEventNode;|
		Pointer to the head of the |UEVENTNODE| chain for this
		|UDATAPLANE|.  Events affecting any of the underlying
		interfaces will be broadcast to this chain.  This is primarily
		for internal use; clients should be able to get all info in
		a more useful form via the |UDATAPLANE_OUTSWITCH|.

	|UDATASTREAM *pDataStreamHead;|
		Pointer to the head of the list of |UDATASTREAM|s associated
		with this |UDATAPLANE|.

	|UINT32 ulGenerationCount;|
		Incremented by the core DataPump whenever it starts mutating
		the state of a |UDATAPLANE|.  Read-only by everyone else.

	|UINT32 ulSavedGenerationCount;|
		Maintained by the |UDATAPLANE| implementation.  Is set to a
		copy of |ulGenerationCount| whenever an interface up or down
		event is processed.  Read-only by everyone else.  If
		|ulGenerationCount| is not equal to |ulSavedGenerationCount|,
		clients can assume that now is probably not a good time to
		send |UBUFQE|s towards the host, because the data-structures
		are in transition.

Private Contents:
	|UEVENTNODE ManagementEventNode;|
		An eventnode for internal use by the dataplane implementation;
		This should be treated as opaque by client code.

	|UEVENTNODE DeviceEventNode;|
		An eventnode for internal use by the dataplane implementation;
		This should be treated as opaque by client code.

Object Hierarchy:
	|Header.Siblings| is used to link together all |UDATASTREAM|s
	collected under a given |UDATAPLANE|.  |Header.pStaticParent|
	points to the creating object.  |Header.pIoctlParent| also points
	to the creating object.

	|Header.pName| normally points to a name that distinguishes the
	object.   It used by browsing functions, and for binding from the OS
	down to the DataPump.

IOCTLs implemented:
	None; all are passed to the parent object.

Notes:
	Minimal support for |UDATAPLANE|s is integrated into the core DataPump.
	|UDATAPLANE|s are not actually created and instantiated unless the
	V1.8 framework is in use.

	One of the critical uses of |UDATAPLANE|s is to support the
	pipe abstraction created by the |UDATASTREAM| mechanism.  Therefore,
	support for |UDATASTREAM|s is integrated into the |UDATAPLANE|
	implementation (and code supporting |UDATASTREAM|s will be linked
	into the target application if |UDATAPLANE|s are used).

	Really complex functions, such as audio class with multiple interfaces
	and multiple settings per interface, are modelled using separate
	|UDATAPLANE| instances for each interface.  It is up to the
	initialization code to determine how to bind |UDATAPLANE|s across
	configurations.

See Also:
	UDATASTREAM

*/

struct __TMS_STRUCTNAME(UDATAPLANE)
	{
	/*
	|| A dataplane node is a first-class object in the USB DataPump world.
	||
	|| The static parent of a UDATAPLANE is the governing ufunction.
	||
	|| The IOCTL parent of a UDATAPLANE is also the governing object.
	*/
	__TMS_USBPUMP_OBJECT_HEADER		Header;

	/*
	|| Dataplanes are managed outside the object system as well.  So
	|| we need linked list fields.
	*/
	__TMS_UDATAPLANE			*pNext;
	__TMS_UDATAPLANE			*pLast;

	/*
	|| We need an outcall table for clients and a cell for clients to
	|| use if they need to.
	*/
	__TMS_CONST __TMS_UDATAPLANE_OUTSWITCH	*pOutSwitch;
	__TMS_VOID				*pClientContext;

	/*
	|| We save a pointer to our parent UDEVICE, for
	|| efficiency.
	*/
	__TMS_UDEVICE				*pDevice;

	/* the vector of controlled alt settings */
	__TMS_UINTERFACE			*pInterfaceListHead;

	/* the currently active alt setting */
	__TMS_UINTERFACE			*pCurrentInterface;

	/*
	|| The generation count for this dataplane-node.  This can be
	|| tracked against ulSavedGenerationCount to invalidate stale data.
	*/
	__TMS_UINT32				ulGenerationCount;

	/*
	|| The last-processed generation count for this dataplane-node.
	|| This can be tracked by other code to invalidate stale data.
	|| It's set to ulGenerationCount whenever we actually receive
	|| notification of a change; but it is changed by the DataPump
	|| core when it starts processing the change. This makes it much
	|| easier for datastreams to know when to start parking UBUFQEs.
	*/
	__TMS_UINT32				ulSavedGenerationCount;

	/*
	|| The list of UDATASTREAMs attached to this dataplane.
	*/
	__TMS_UDATASTREAM			*pDataStreamHead;

	/*
	|| Clients who want events from this event node attach
	|| to this event node chain entry.  They'll get up/down
	|| and some bus events (anything from the UINTERFACEs).
	|| However, clients are encouraged to use the outswitch instead
	|| of attaching to this node chain.
	*/
	__TMS_UEVENTNODE			*pEventNode;

	/*
	|| Private flags
	*/
	__TMS_UINT				fDataPlaneSuspend: 1;
	__TMS_UINT				fRemoteWakeupEnable: 1;

	/*
	|| Private:  event node for use in managing the dataplane node.
	*/
	__TMS_UEVENTNODE			ManagementEventNode;

	/*
	|| Private:  event node for use in getting device notifications.
	*/
	__TMS_UEVENTNODE			DeviceEventNode;
	};

#define	__TMS_UDATAPLANE_TAG		__TMS_UHIL_MEMTAG('U', 'D', 'p', 'l')
#define	__TMS_UDATAPLANE_OBJECT_NAME	"dataplane.obj.mcci.com"

/*

Type:	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN

Function:
	Method function type:  receive dataplane up/down notifications.

Definition:
	typedef VOID UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN(
			UDATAPLANE *pDataPlane,
			BOOL fTrueIfNowUp
			);

	typedef UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN
		*PUDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN;

Description:
	This method, if present in the dataplane outswitch, is called
	whenever the dataplane changes state in response to a state-change
	message.  It's only called when there is a change in state.

	If an alternate interface setting is selected, the dataplane
	will go down and immediately come back up, as the old setting
	is shut down and the new setting is brought up.

Returns:
	No explicit result.

*/


__TMS_FNTYPE_DEF(
UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN,
	__TMS_VOID,
		(
		__TMS_UDATAPLANE *,
		__TMS_BOOL /* TRUE ==> up, FALSE ==> down */
		)
	);

/*

Type:	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN

Function:
	Method function type:  receive setup packet processing notifications.

Definition:
	typedef VOID UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN(
			UDATAPLANE *pDataPlane,
			UEVENTSETUP *pEventSetup,
			BOOL bTrueIfExecutePhase
			);

	typedef UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN
		*PUDATAPLANE_OUTSWITCH_SETUP_PACKET_FN;

Description:
	This method, if present in the dataplane outswitch, is called
	whenever the dataplane receives a setup packet targeting one
	of the interfaces in the dataplane, receives a setup packet targeting
	the device, or receives a vendor request setup packet.  The flag
	|bTrueIfExecutePhase| is FALSE for the prescan phase, TRUE for the
	execute phase.

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN,
	__TMS_VOID,
		(
		__TMS_UDATAPLANE *,
		__TMS_UEVENTSETUP *,
		__TMS_BOOL /* FALSE => prescan, TRUE => act on it */
		)
	);

/*

Type:	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN

Function:
	Method function type:  receive dataplane suspent/resume, etc,
	notifications.

Definition:
	typedef VOID UDATAPLANE_OUTSWITCH_POWER_EVENT_FN(
			UDATAPLANE *pDataPlane,
			UEVENT EventCode,
			VOID *pEventSpecificInfo
			);

	typedef UDATAPLANE_OUTSWITCH_POWER_EVENT_FN
		*PUDATAPLANE_OUTSWITCH_POWER_EVENT_FN;

Description:
	This method, if present in the dataplane outswitch, is called
	whenever the dataplane receives a state-change message (UEVENT_RESET,
	UEVENT_SUSPEND, UEVENT_RESUME, UEVENT_ATTACH, UEVENT_DETACH,
	UEVENT_L1_SLEEP, UEVENT_FUNCTION_SUSPEND, UEVENT_FUNCTION_RESUME).

Returns:
	No explicit result.

Notes:
	V1.8:  this routine may get called several times in succession when
	processing a single UEVENT_RESET.  This is a property of the underlying
	DataPump implementation.  We plan to change this to a single call per
	reset in a future version, but coders are advised to plan their response
	to UEVENT_RESET appropriately.

*/

__TMS_FNTYPE_DEF(
UDATAPLANE_OUTSWITCH_POWER_EVENT_FN,
	__TMS_VOID,
		(
		__TMS_UDATAPLANE *,
		__TMS_UEVENT,
		__TMS_VOID *
		)
	);

/*

Type:	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN

Function:
	Method function type: dataplane start/stop

Definition:
	typedef BOOL UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN(
			UDATAPLANE *pDataPlane,
			BOOL fTrueIfStart
			);

	typedef UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN
		*PUDATAPLANE_OUTSWITCH_DATAPLANE_START_FN;

Description:
	This method, if present in the dataplane outswitch, is called
	whenever the dataplane is to be started or stopped.

	If an alternate interface setting is selected, the dataplane
	will go down and immediately come back up, as the old setting
	is shut down and the new setting is brought up.

Returns:
	True on successfull start/stop

*/


__TMS_FNTYPE_DEF(
UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN,
	__TMS_BOOL,
		(
		__TMS_UDATAPLANE *,
		__TMS_BOOL /* TRUE ==> start, FALSE ==> stop */
		)
	);

/*

Type:	UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN

Function:
	Method function type:  receive dataplane feature event.

Definition:
	typedef VOID UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN(
			UDATAPLANE *pDataPlane,
			UEVENTFEATURE *pFeatureEventInfo
			);

	typedef UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN
		*UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN;

Description:
	This method, if present in the dataplane outswitch, is called
	whenever the dataplane receives a UEVENT_FEATURE.

Returns:
	No explicit result.

*/

__TMS_FNTYPE_DEF(
UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN,
	__TMS_VOID,
		(
		__TMS_UDATAPLANE *,
		__TMS_UEVENTFEATURE *
		)
	);

/*

Type:	UDATAPLANE_OUTSWITCH_IOCTL_FN

Function:
	Method function type:  dataplane IOCTL handler.

Definition:
	typedef USBPUMP_IOCTL_RESULT
		UDATAPLANE_OUTSWITCH_IOCTL_FN(
			UDATAPLANE *		pDataPlane,
			USBPUMP_IOCTL_CODE	IoctlRequestCode,
			CONST VOID *		pInParam,
			VOID *			pOutParam
			);

	typedef UDATAPLANE_OUTSWITCH_IOCTL_FN
		*UDATAPLANE_OUTSWITCH_IOCTL_FN;

Description:
	This method, if present in the dataplane outswitch, is called
	whenever the dataplane object IOCTL handler receives an EDGE_IOCTL.

Returns:
	Return either USBPUMP_IOCTL_RESULT_NOT_CLAIMED (if the IOCTL code was
	not recognized), USBPUMP_IOCTL_RESULT_SUCCESS (if the IOCTL code was
	recognized and the operation was successfully performed), or some
	error code (if the IOCTL code was recognized, but the operation
	could not be performed for some reason).

*/

__TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(
	UDATAPLANE_OUTSWITCH_IOCTL_FN,
	__TMS_UDATAPLANE *
	);

/****************************************************************************\
|
|	The UDATAPLANE_OUTSWITCH
|
\****************************************************************************/

/*

Type:	UDATAPLANE_OUTSWITCH

Function:
	Outbound switch for UDATAPLANEs

Description:
	UDATAPLANE behavior is divided into two parts.  One part is
	provided by the framework; the other by the next layer.
	Although the next layer is typically a UDATASTREAM, we don't
	want to preclude reuse of this primitive in the future.  Therefore,
	the upcalls are implemented using a protocol out-switch in the
	usual DataPump fashion.

Contents:
	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN *pDataPlaneUp;
		Function called whenever the dataplane goes up (or
		is taken down).

	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pSetupPacket;
		Function called whenver the dataplane receives a setup
		packet targeted at one of its components.

	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN	*pPowerEvent;
		Function called whenever the dataplane receives a
		power management message (suspend, resume, etc.)

	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN *pDataPlaneStart;
		Function called whenever the dataplane is started (or
		stopped).

	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pDeviceSetupPacket;
		Function called whenver the dataplane receives a setup
		packet targeted at the device.

	UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN	*pFeatureEvent;
		Function called whenever the dataplane receives a
		set/clear feature message

	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pVendorSetupPacket;
		Function called whenever the dataplane receives a vendor
		setup packet.

	UDATAPLANE_OUTSWITCH_IOCTL_FN		*pIoctl;
		Function called whenever the dataplane ioctl handler receives
		an EDGE_IOCTL.

Notes:
	Instances of this object are normally initialized using either
	UDATAPLANE_OUTSWITCH_INIT_Vx() {compile time} or
	UDATAPLANE_OUTSWITCH_SETUP_Vx() {runtime}.

See Also:
	UDATAPLANE, UDATASTREAM

*/

struct __TMS_STRUCTNAME(UDATAPLANE_OUTSWITCH)
	{
	__TMS_UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN	*pDataPlaneUp;
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pSetupPacket;
	__TMS_UDATAPLANE_OUTSWITCH_POWER_EVENT_FN	*pPowerEvent;
	__TMS_UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN	*pDataPlaneStart;
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pDeviceSetupPacket;
	__TMS_UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN	*pFeatureEvent;
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pVendorSetupPacket;
	__TMS_UDATAPLANE_OUTSWITCH_IOCTL_FN		*pIoctl;
	};

/*

Name:	UDATAPLANE_OUTSWITCH_INIT_V5()

Function:
	Macro:  generate compile-time init code for a UDATAPLANE outswitch.

Definition:
	macro UDATAPLANE_OUTSWITCH_INIT_V5(
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN	*pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN	*pPowerEventFn,
		UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN *pDataPlaneStartFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pDeviceSetupPacketFn,
		UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN	*pFeatureEventFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pVendorSetupPacketFn,
		UDATAPLANE_OUTSWITCH_IOCTL_FN		*pIoctl
		);

Description:
	This macro should be used to isolate client code from version-to-
	version changes in the structuer of the UDATAPLANE_OUTSWITCH.
	If the structure evolves (adding, for example, new fields), efforts
	will be made so that this macro expands in a forward-compatible
	way.

	This macro expands into the properly-bracketed initializer for a
	UDATAPLANE_OUTSWITCH structure, and typically is used as follows:

	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneUpHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN DataPlaneStartHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN DeviceSetupPacketHandler;
	UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN FeatureEventHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN VendorSetupPacketHandler;
	UDATAPLANE_OUTSWITCH_IOCTL_FN IoctlHandler;

	CONST UDATAPLANE_OUTSWITCH MyOutSwitch =
		UDATAPLANE_OUTSWITCH_INIT_V5(
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler,
			DataPlaneStartHandler,
			DeviceSetupPacketHandler,
			FeatureEventHandler,
			VendorSetupPacketHandler,
			IoctlHandler
			);

	Note that the trailing ';' is needed.

Expands to:
	Expands to '{', the internal initializers, and the closing '}'.
	No trailing semicolon is provided.

See also:
	UDATAPLANE_OUTSWITCH_SETUP_V5()

*/

/*
|| Architecture: never change this macro's arguments; instead make V6 macro,
|| and express this in terms of the V6 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_INIT_V5(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent,							\
	pVendorSetupPacket,						\
	pIoctl								\
	)								\
	{								\
	(pDataPlaneUp),							\
	(pSetupPacket),							\
	(pPowerEvent),							\
	(pDataPlaneStart),						\
	(pDeviceSetupPacket),						\
	(pFeatureEvent),						\
	(pVendorSetupPacket),						\
	(pIoctl)							\
	}

/*

Name:	UDATAPLANE_OUTSWITCH_INIT_V4()

Function:
	Macro:  generate compile-time init code for a UDATAPLANE outswitch.

Definition:
	macro UDATAPLANE_OUTSWITCH_INIT_V4(
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN *pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN *pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN *pPowerEventFn,
		UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN *pDataPlaneStartFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pDeviceSetupPacketFn,
		UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN	*pFeatureEventFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pVendorSetupPacketFn
		)

Description:
	This macro should be used to isolate client code from version-to-
	version changes in the structuer of the UDATAPLANE_OUTSWITCH.
	If the structure evolves (adding, for example, new fields), efforts
	will be made so that this macro expands in a forward-compatible
	way.

	This macro expands into the properly-bracketed initializer for a
	UDATAPLANE_OUTSWITCH structure, and typically is used as follows:

	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneUpHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN DataPlaneStartHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN DeviceSetupPacketHandler;
	UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN FeatureEventHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN VendorSetupPacketHandler;

	CONST UDATAPLANE_OUTSWITCH MyOutSwitch =
		UDATAPLANE_OUTSWITCH_INIT_V4(
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler,
			DataPlaneStartHandler,
			DeviceSetupPacketHandler,
			FeatureEventHandler,
			VendorSetupPacketHandler
			);

	Note that the trailing ';' is needed.

Expands to:
	Expands to '{', the internal initializers, and the closing '}'.
	No trailing semicolon is provided.

See also:
	UDATAPLANE_OUTSWITCH_SETUP_V4()

*/

/*
|| Architecture: never change this macro's arguments; instead make V5 macro,
|| and express this in terms of the V5 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_INIT_V4(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent,							\
	pVendorSetupPacket						\
	)								\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V5(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent,							\
	pVendorSetupPacket,						\
	/* V5: pIoctl */ __TMS_NULL					\
	)

/*

Name:	UDATAPLANE_OUTSWITCH_INIT_V3()

Function:
	Macro:  generate compile-time init code for a UDATAPLANE outswitch.

Definition:
	macro UDATAPLANE_OUTSWITCH_INIT_V3(
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN *pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN *pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN *pPowerEventFn,
		UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN *pDataPlaneStartFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pDeviceSetupPacketFn,
		UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN	*pFeatureEventFn
		)

Description:
	This macro should be used to isolate client code from version-to-
	version changes in the structuer of the UDATAPLANE_OUTSWITCH.
	If the structure evolves (adding, for example, new fields), efforts
	will be made so that this macro expands in a forward-compatible
	way.

	This macro expands into the properly-bracketed initializer for a
	UDATAPLANE_OUTSWITCH structure, and typically is used as follows:

	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneUpHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN DataPlaneStartHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN DeviceSetupPacketHandler;
	UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN FeatureEventHandler;

	CONST UDATAPLANE_OUTSWITCH MyOutSwitch =
		UDATAPLANE_OUTSWITCH_INIT_V3(
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler,
			DataPlaneStartHandler,
			DeviceSetupPacketHandler,
			FeatureEventHandler
			);

	Note that the trailing ';' is needed.

Expands to:
	Expands to '{', the internal initializers, and the closing '}'.
	No trailing semicolon is provided.

See also:
	UDATAPLANE_OUTSWITCH_SETUP_V3()

*/

/*
|| Architecture: never change this macro's arguments; instead make V4 macro,
|| and express this in terms of the V4 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_INIT_V3(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent							\
	)								\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V4(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent,							\
	/* V4: pVendorSetupPacket */ __TMS_NULL				\
	)

/*

Name:	UDATAPLANE_OUTSWITCH_INIT_V2()

Function:
	Macro:  generate compile-time init code for a UDATAPLANE outswitch.

Definition:
	macro UDATAPLANE_OUTSWITCH_INIT_V2(
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN *pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN *pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN *pPowerEventFn,
		UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN *pDataPlaneStartFn
		)

Description:
	This macro should be used to isolate client code from version-to-
	version changes in the structuer of the UDATAPLANE_OUTSWITCH.
	If the structure evolves (adding, for example, new fields), efforts
	will be made so that this macro expands in a forward-compatible
	way.

	This macro expands into the properly-bracketed initializer for a
	UDATAPLANE_OUTSWITCH structure, and typically is used as follows:

	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneUpHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN DataPlaneStartHandler;

	CONST UDATAPLANE_OUTSWITCH MyOutSwitch =
		UDATAPLANE_OUTSWITCH_INIT_V2(
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler,
			DataPlaneStartHandler
			);

	Note that the trailing ';' is needed.

Expands to:
	Expands to '{', the internal initializers, and the closing '}'.
	No trailing semicolon is provided.

See also:
	UDATAPLANE_OUTSWITCH_SETUP_V2()

*/

/*
|| Architecture: never change this macro's arguments; instead make V3 macro,
|| and express this in terms of the V3 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_INIT_V2(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart							\
	)								\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V3(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	/* V3: pDeviceSetupPacket */ __TMS_NULL,			\
	/* V3: pFeatureEvent */ __TMS_NULL				\
	)

/*

Name:	UDATAPLANE_OUTSWITCH_INIT_V1()

Function:
	Macro:  generate compile-time init code for a UDATAPLANE outswitch.

Definition:
	macro UDATAPLANE_OUTSWITCH_INIT_V1(
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN *pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN *pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN *pPowerEventFn
		)

Description:
	This macro should be used to isolate client code from version-to-
	version changes in the structuer of the UDATAPLANE_OUTSWITCH.
	If the structure evolves (adding, for example, new fields), efforts
	will be made so that this macro expands in a forward-compatible
	way.

	This macro expands into the properly-bracketed initializer for a
	UDATAPLANE_OUTSWITCH structure, and typically is used as follows:

	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;

	CONST UDATAPLANE_OUTSWITCH MyOutSwitch =
		UDATAPLANE_OUTSWITCH_INIT_V1(
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler
			);

	Note that the trailing ';' is needed.

Expands to:
	Expands to '{', the internal initializers, and the closing '}'.
	No trailing semicolon is provided.

See also:
	UDATAPLANE_OUTSWITCH_INIT_V2()
	UDATAPLANE_OUTSWITCH_SETUP_V1()

*/

/*
|| Architecture: never change this macro's arguments; instead make V2 macro,
|| and express this in terms of the V2 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_INIT_V1(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent							\
	)								\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V2(								\
	(pDataPlaneUp),							\
	(pSetupPacket),							\
	(pPowerEvent),							\
	(__TMS_NULL)							\
	)

/*

Name:	UDATAPLANE_OUTSWITCH_SETUP_V5()

Function:
	Dynamically initialize a UDATAPLANE outswitch.

Definition:
	VOID UDATAPLANE_OUTSWITCH_SETUP_V5(
		UDATAPLANE_OUTSWITCH			*pSwitch,
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN	*pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN	*pPowerEventFn,
		UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN	*pDataPlaneStartFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pDeviceSetupPacketFn,
		UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN	*pFeatureEventFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pVendorSetupPacketFn,
		UDATAPLANE_OUTSWITCH_IOCTL_FN		*pIoctl
		);

Description:
	This function (implemented as a macro) should be used to isolate
	client code from version-to-version changes in the structure of
	the UDATAPLANE_OUTSWITCH.  If the structure evolves (adding, for
	example, new fields), efforts will be made so that this macro
	expands in a forward-compatible way.

	This macro expands into code which initializes *pOutSwitch.

Example:
	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneUpHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN DataPlaneStartHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN DeviceSetupPacketHandler;
	UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN FeatureEventHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN VendorSetupPacketHandler;
	UDATAPLANE_OUTSWITCH_IOCTL_FN IoctlHandler;
	...

	UDATAPLANE_OUTSWITCH *pMyOutSwitch;
	...

	pMyOutSwitch = UsbPumpLib_DeviceAllocateFromPlatform(
				pPlatform, sizeof(*pMyOutSwitch)
				);

	if (pMyOutSwitch)
		{
		UDATAPLANE_OUTSWITCH_SETUP_V5(
			pMyOutSwitch,
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler,
			DataPlaneStartHandler,
			DeviceSetupPacketHandler,
			FeatureEventHandler,
			VendorSetupPacketHandler,
			IoctlHandler
			);
		}

	Note that the trailing ';' is needed.

Returns:
	No explicit result.

See also:
	UDATAPLANE_OUTSWITCH_INIT_V5()

*/

/*
|| Runtime setup...
*/
/*
|| Architecture: never change this macro's arguments; instead make V6 macro,
|| and express this in terms of the V6 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_SETUP_V5(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent,						\
	a_pVendorSetupPacket,						\
	a_pIoctl							\
	)								\
    do	{								\
	__TMS_UDATAPLANE_OUTSWITCH * __TMS_CONST v_p = (a_pOutSwitch);	\
	v_p->pDataPlaneUp = (a_pDataPlaneUp);				\
	v_p->pSetupPacket = (a_pSetupPacket);				\
	v_p->pPowerEvent = (a_pPowerEvent);				\
	v_p->pDataPlaneStart = (a_pDataPlaneStart);			\
	v_p->pDeviceSetupPacket = (a_pDeviceSetupPacket);		\
	v_p->pFeatureEvent = (a_pFeatureEvent);				\
	v_p->pVendorSetupPacket = (a_pVendorSetupPacket);		\
	v_p->pIoctl = (a_pIoctl);					\
	} while (0)

/*

Name:	UDATAPLANE_OUTSWITCH_SETUP_V4()

Function:
	Dynamically initialize a UDATAPLANE outswitch.

Definition:
	VOID UDATAPLANE_OUTSWITCH_SETUP_V4(
		UDATAPLANE_OUTSWITCH			*pSwitch,
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN	*pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN	*pPowerEventFn,
		UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN	*pDataPlaneStartFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pDeviceSetupPacketFn,
		UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN	*pFeatureEventFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pVendorSetupPacketFn
		);

Description:
	This function (implemented as a macro) should be used to isolate
	client code from version-to-version changes in the structure of
	the UDATAPLANE_OUTSWITCH.  If the structure evolves (adding, for
	example, new fields), efforts will be made so that this macro
	expands in a forward-compatible way.

	This macro expands into code which initializes *pOutSwitch.

Example:
	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneUpHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN DataPlaneStartHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN DeviceSetupPacketHandler;
	UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN FeatureEventHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN VendorSetupPacketHandler;
	...

	UDATAPLANE_OUTSWITCH *pMyOutSwitch;
	...

	pMyOutSwitch = UsbPumpLib_DeviceAllocateFromPlatform(
				pPlatform, sizeof(*pMyOutSwitch)
				);

	if (pMyOutSwitch)
		{
		UDATAPLANE_OUTSWITCH_SETUP_V4(
			pMyOutSwitch,
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler,
			DataPlaneStartHandler,
			DeviceSetupPacketHandler,
			FeatureEventHandler,
			VendorSetupPacketHandler
			);
		}

	Note that the trailing ';' is needed.

Returns:
	No explicit result.

See also:
	UDATAPLANE_OUTSWITCH_INIT_V3()

*/

/*
|| Runtime setup...
*/
/*
|| Architecture: never change this macro's arguments; instead make V5 macro,
|| and express this in terms of the V5 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_SETUP_V4(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent,						\
	a_pVendorSetupPacket						\
	)								\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V5(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent,						\
	a_pVendorSetupPacket,						\
	/* V5: pIoctl */ __TMS_NULL					\
	)

/*

Name:	UDATAPLANE_OUTSWITCH_SETUP_V3()

Function:
	Dynamically initialize a UDATAPLANE outswitch.

Definition:
	VOID UDATAPLANE_OUTSWITCH_SETUP_V3(
		UDATAPLANE_OUTSWITCH			*pSwitch,
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN	*pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN	*pPowerEventFn,
		UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN	*pDataPlaneStartFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pDeviceSetupPacketFn,
		UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN	*pFeatureEventFn
		);

Description:
	This function (implemented as a macro) should be used to isolate
	client code from version-to-version changes in the structure of
	the UDATAPLANE_OUTSWITCH.  If the structure evolves (adding, for
	example, new fields), efforts will be made so that this macro
	expands in a forward-compatible way.

	This macro expands into code which initializes *pOutSwitch.

Example:
	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneUpHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN DataPlaneStartHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN DeviceSetupPacketHandler;
	UDATAPLANE_OUTSWITCH_FEATURE_EVENT_FN FeatureEventHandler;
	...

	UDATAPLANE_OUTSWITCH *pMyOutSwitch;
	...

	pMyOutSwitch = UsbPumpLib_DeviceAllocateFromPlatform(
				pPlatform, sizeof(*pMyOutSwitch)
				);

	if (pMyOutSwitch)
		{
		UDATAPLANE_OUTSWITCH_SETUP_V3(
			pMyOutSwitch,
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler,
			DataPlaneStartHandler,
			DeviceSetupPacketHandler,
			FeatureEventHandler
			);
		}

	Note that the trailing ';' is needed.

Returns:
	No explicit result.

See also:
	UDATAPLANE_OUTSWITCH_INIT_V3()

*/

/*
|| Runtime setup...
*/
/*
|| Architecture: never change this macro's arguments; instead make V4 macro,
|| and express this in terms of the V4 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_SETUP_V3(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent							\
	)								\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V4(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent,						\
	/* V4: pVendorSetupPacket */ __TMS_NULL				\
	)

/*

Name:	UDATAPLANE_OUTSWITCH_SETUP_V2()

Function:
	Dynamically initialize a UDATAPLANE outswitch.

Definition:
	VOID UDATAPLANE_OUTSWITCH_SETUP_V2(
		UDATAPLANE_OUTSWITCH			*pSwitch,
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN	*pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN	*pPowerEventFn,
		UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN	*pDataPlaneStartFn
		);

Description:
	This function (implemented as a macro) should be used to isolate
	client code from version-to-version changes in the structure of
	the UDATAPLANE_OUTSWITCH.  If the structure evolves (adding, for
	example, new fields), efforts will be made so that this macro
	expands in a forward-compatible way.

	This macro expands into code which initializes *pOutSwitch.

Example:
	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneUpHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	UDATAPLANE_OUTSWITCH_DATAPLANE_START_FN DataPlaneStartHandler;
	...

	UDATAPLANE_OUTSWITCH *pMyOutSwitch;
	...

	pMyOutSwitch = UsbPumpLib_DeviceAllocateFromPlatform(
				pPlatform, sizeof(*pMyOutSwitch)
				);

	if (pMyOutSwitch)
		{
		UDATAPLANE_OUTSWITCH_SETUP_V2(
			pMyOutSwitch,
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler,
			DataPlaneStartHandler
			);
		}

	Note that the trailing ';' is needed.

Returns:
	No explicit result.

See also:
	UDATAPLANE_OUTSWITCH_INIT_V2()

*/

/*
|| Runtime setup...
*/
/*
|| Architecture: never change this macro's arguments; instead make V3 macro,
|| and express this in terms of the V3 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_SETUP_V2(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart						\
	)								\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V3(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	/* V3: pDeviceSetupPacket */ __TMS_NULL,			\
	/* V3: pFeatureEvent */ __TMS_NULL				\
	)

/*

Name:	UDATAPLANE_OUTSWITCH_SETUP_V1()

Function:
	Dynamically initialize a UDATAPLANE outswitch.

Definition:
	VOID UDATAPLANE_OUTSWITCH_SETUP_V1(
		UDATAPLANE_OUTSWITCH			*pSwitch,
		UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN	*pDataPlaneUpFn,
		UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN	*pSetupPacketFn,
		UDATAPLANE_OUTSWITCH_POWER_EVENT_FN	*pPowerEventFn
		);

Description:
	This function (implemented as a macro) should be used to isolate
	client code from version-to-version changes in the structure of
	the UDATAPLANE_OUTSWITCH.  If the structure evolves (adding, for
	example, new fields), efforts will be made so that this macro
	expands in a forward-compatible way.

	This macro expands into code which initializes *pOutSwitch.

Example:
	UDATAPLANE_OUTSWITCH_DATAPLANE_UP_FN DataPlaneHandler;
	UDATAPLANE_OUTSWITCH_SETUP_PACKET_FN SetupPacketHandler;
	UDATAPLANE_OUTSWITCH_POWER_EVENT_FN PowerVentHandler;
	...

	UDATAPLANE_OUTSWITCH *pMyOutSwitch;
	...

	pMyOutSwitch = UsbPumpLib_DeviceAllocateFromPlatform(
				pPlatform, sizeof(*pMyOutSwitch)
				);

	if (pMyOutSwitch)
		{
		UDATAPLANE_OUTSWITCH_SETUP_V1(
			pMyOutSwitch,
			DataPlaneUpHandler,
			SetupPacketHander,
			PowerEventHandler
			);
		}

	Note that the trailing ';' is needed.

Returns:
	No explicit result.

See also:
	UDATAPLANE_OUTSWITCH_INIT_V1()

*/

/*
|| Runtime setup...
*/
/*
|| Architecture: never change this macro's arguments; instead make V2 macro,
|| and express this in terms of the V2 macro.
*/
#define __TMS_UDATAPLANE_OUTSWITCH_SETUP_V1(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent							\
	)								\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V2(				\
		a_pOutSwitch,						\
		a_pDataPlaneUp,						\
		a_pSetupPacket,						\
		a_pPowerEvent,						\
		/* V2: pDataPlaneStart */ __TMS_NULL			\
		)

/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* an internal API -- not public use */
__TMS_VOID
UsbPumpDataPlaneI_DisconnectStreams(
	__TMS_UDATAPLANE *pDataPlane
	);

/* from common/dplane_addifc.c */
__TMS_UINT
UsbPumpDataPlane_AddInterface(
	__TMS_UDATAPLANE *pDataPlane,
	__TMS_UINTERFACE *pInterface
	);

/* from common/dplane_addifcsbynum.c */
__TMS_UINT
UsbPumpDataPlane_AddInterfacesByNumber(
	__TMS_UDATAPLANE *pDataPlane,
	__TMS_UCONFIG *pConfig,
	__TMS_CONST __TMS_UINT8 *pvbInterfaceNumber,
	__TMS_BYTES nvbInterfaceNumber
	);

/* from common/dplane_addifcsfromset.c */
__TMS_UINT
UsbPumpDataPlane_AddInterfacesFromSet(
	__TMS_UDATAPLANE *pDataPlane,
	__TMS_UINTERFACESET *pIfcSet
	);

/* from common/dplane_create.c */
__TMS_UDATAPLANE *
UsbPumpDataPlane_Create(
	__TMS_UDEVICE *pDevice,
	__TMS_USBPUMP_OBJECT_HEADER *pParent
	);

/* from common/dplane_deinit.c */
__TMS_VOID
UsbPumpDataPlane_Deinit(
	__TMS_UDATAPLANE *pDataPlane
	);

/* from common/dplane_delete.c */
__TMS_VOID
UsbPumpDataPlane_Delete(
	__TMS_UDATAPLANE *pDataPlane
	);

/* from common/dplane_init.c */
__TMS_VOID
UsbPumpDataPlane_Init(
	__TMS_UDATAPLANE *pDataPlane,
	__TMS_UDEVICE *pDevice,
	__TMS_USBPUMP_OBJECT_HEADER *pIoctlParent /*OPTIONAL*/
	);

/* from common/dplane_matchconfig.c */
__TMS_BOOL
UsbPumpDataPlane_MatchConfig(
	__TMS_UDATAPLANE *	pDataPlane
	);

/* from common/dplane_actep.c */
__TMS_VOID
UsbPumpDataPlane_ActivateEndpoints(
	__TMS_UDATAPLANE *	pDataPlane,
	__TMS_BOOL		fActive
	);

/* from common/dplane_reconnectstreams.c */
__TMS_VOID
UsbPumpDataPlane_ReconnectStreams(
	__TMS_UDATAPLANE *pDataPlane
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UDATAPLANE_TAG	\
   __TMS_UDATAPLANE_TAG
# define UDATAPLANE_OBJECT_NAME	\
   __TMS_UDATAPLANE_OBJECT_NAME
# define UDATAPLANE_OUTSWITCH_INIT_V5(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent,							\
	pVendorSetupPacket,						\
	pIoctl								\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V5(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent,							\
	pVendorSetupPacket,						\
	pIoctl								\
	)
# define UDATAPLANE_OUTSWITCH_INIT_V4(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent,							\
	pVendorSetupPacket						\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V4(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent,							\
	pVendorSetupPacket						\
	)
# define UDATAPLANE_OUTSWITCH_INIT_V3(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent							\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V3(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart,						\
	pDeviceSetupPacket,						\
	pFeatureEvent							\
	)
# define UDATAPLANE_OUTSWITCH_INIT_V2(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart							\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V2(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent,							\
	pDataPlaneStart							\
	)
# define UDATAPLANE_OUTSWITCH_INIT_V1(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent							\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_INIT_V1(				\
	pDataPlaneUp,							\
	pSetupPacket,							\
	pPowerEvent							\
	)
# define UDATAPLANE_OUTSWITCH_SETUP_V5(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent,						\
	a_pVendorSetupPacket,						\
	a_pIoctl							\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V5(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent,						\
	a_pVendorSetupPacket,						\
	a_pIoctl							\
	)
# define UDATAPLANE_OUTSWITCH_SETUP_V4(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent,						\
	a_pVendorSetupPacket						\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V4(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent,						\
	a_pVendorSetupPacket						\
	)
# define UDATAPLANE_OUTSWITCH_SETUP_V3(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent							\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V3(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart,						\
	a_pDeviceSetupPacket,						\
	a_pFeatureEvent							\
	)
# define UDATAPLANE_OUTSWITCH_SETUP_V2(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart						\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V2(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent,							\
	a_pDataPlaneStart						\
	)
# define UDATAPLANE_OUTSWITCH_SETUP_V1(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent							\
	)	\
	__TMS_UDATAPLANE_OUTSWITCH_SETUP_V1(				\
	a_pOutSwitch,							\
	a_pDataPlaneUp,							\
	a_pSetupPacket,							\
	a_pPowerEvent							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of udataplane.h ****/
#endif /* _UDATAPLANE_H_ */
