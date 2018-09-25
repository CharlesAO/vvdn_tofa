/* ueventnode.h	Mon Jun  1 2015 00:19:24 tmm */

/*

Module:  ueventnode.h

Function:
	Home for UEVENT, UEVENT_NODE.

Version:
	V3.15b	Mon Jun  1 2015 00:19:24 tmm	Edit level 19

Copyright notice:
	This file copyright (C) 2002-2005, 2008-2012, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 16:25:45  tmm
	Module created.

   1.79a  6/16/2002  tmm
	600032/1.1.16:  add UEVENT_DATAPLANE, UEVENTDATAPLANE_INFO; add
	(some) embedded documentation.

	Add UEVENT_ATTACH, UEVENT_DETACH.

   1.81a  Thu Feb 20 2003 00:28:35  tmm
	Bugs 999, 1000:  add UEVENTRESET_INFO.

   1.81a  Sat Feb 22 2003 15:20:00 maw
   	Corrected invalid parameter list and extra '}' in 
	|UEVENTRESET_INFO_SETUP_V1|.

   1.87a  Sun Nov  2 2003 00:31:30  tmm
	Improve documentation for autodoc purposes.

   1.89e  2004-11-25  tmm
	For nyctea project, add a platform-specific event code and an
	associated information object.

   1.91a  Thu Dec 23 2004 11:27:55  chwon
	Improve documentation for autodoc purposes.

   1.91h  2005-08-14  tmm
	Break USETUP out to usetup.h.

   1.97k  Tue Sep 23 2008 15:05:48  cccho
	6683: add UEVENT_L1_SLEEP and UEVENTL1SLEEP_INFO.

   2.01a  Mon Mar 30 2009 11:03:46  chwon
	6866:  fix name cloaking.  Add __TMS_PR6866_DISABLE
	as a compile parameter to restore old (incorrect) behavior.

   3.01d  Thu Sep 02 2010 12:26:09  cccho
	6150, 8071: add UEVENT_CABLE and UEVENT_NOCABLE

   3.01f  Fri Jul 01 2011 14:02:15  maw
   	13233: Add UEVENT_DETECT_ENDPOINT_ACTIVITY

   3.01f  Fri Dec 09 2011 14:20:01  chwon
	14572: Add UEVENT_VENDOR_CONTROL and UEVENT_VENDOR_CONTROL_PRE.

   3.01f  Tue Jan 10 2012 16:21:21  chwon
	14623: Add USB 3.0 related event code and information structure.

   3.15b  Mon Jun  1 2015 00:19:24  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _UEVENTNODE_H_		/* prevent multiple includes */
#define _UEVENTNODE_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USETUP_H_
# include "usetup.h"
#endif

/****************************************************************************\
|
| Event handling mechanism:
|
|	The USB DataPump allows applications to decorate the USB data 
|	structure graph with functions to be called upon the occurance
|	of events defined by the USB DataPump.  Events include such
|	things as 'configure change' (applied to the UCONFIG node for
|	active/inactive transitions), 'interface change' (applied to the
|	UINTERFACE nodes for active/inactive transactions), 'set/clear
|	feature', applied to the appropriate node (device/interface/pipe),
|	or 'setup packet' applied to the appropriate node (device/interface/
|	pipe).
|
\****************************************************************************/

/*

Type:	UEVENT

Function:
	Scalar type that identifies specific events.

Definition:
	typedef enum { ... } UEVENT, *PUEVENT;

Description:
	The various values of |UEVENT| carry two kinds of information:  
	1) the kind of event that has occurred; and 2) the kind of structure
	that is being passed to a |UEVENTFN| in the pInfo parameter.

	The following values are defined:

	|UEVENT_CONFIG_SET|
		Configuration change.  |pInfo| points to the raw setup packet,
		as a |PUCHAR|.  This event is reported to the new |UCONFIG|'s
		event queue, and also to all |UINTERFACE|s and |UPIPE|s that
		are being set up.

		It is also sent to the hardware event queue to notify the
		hardware that the data toggles should be reset and to make
		any other hardware changes.

	|UEVENT_CONFIG_UNSET|
		Configuration change as the result of a received USB 
		SET_CONFIG command.
		|pInfo| points to the raw setup packet, as a |PUCHAR|.  This
		event is reported to the previously active |UCONFIG|'s event
		queue, and also to all the interfaces that are being
		torn down.

	|UEVENT_CONTROL|
		Execute an extended (non chapter 9) default pipe command.
		Prescan has passed, and the given setup packet should be
		operated upon.  |pInfo| points to a |UEVENTSETUP| structure.
		The packet is passed to the appropriate (device/interface/
		pipe) level, based on the routing information in the first
		byte and the wIndex fields.  The |UEVENTFN| should either
		set |uec_bAccept| for successful completion; set |uec_bReject| for
		recognition (but failure); or leave the |UEVENTSETUP| alone
		(if it doesn't recognize the setup packet).

	|UEVENT_CONTROL_PRE|
		Prescan for non-chapter-9 default-pipe command handlers.  
		|pInfo| points to a |UEVENTSETUP|.  During command 
		pre-scan, |UEVENTFN|s should do no work other
		than checking to see if the command looks like one
		that the recipient should process.  If so, the |UEVENTFN|
		should simply set the |uec_bAccept| flag, and wait to receive
		the |UEVENT_CONTROL| message to do the operation.

	|UEVENT_DATAPLANE|
		This is a wrapper event, sent to a |UDATAPLANE|'s event queue
		whenever an event occurs for any interface associated with
		the dataplane.  |pInfo| points to a |UEVENTDATAPLANE_INFO| 
		structure, which conveys info about the original event, as
		well as a pointer to the |UINTERFACE| involved in this event.

	|UEVENT_FEATURE|
		A set/clear feature request has been received from the USB
		host targeting some portion of this device.  |pInfo| points
		to a |UEVENTFEATURE| structure.  This event is dispatched to
		the appropriate location in the deivce tree, based on the
		kind of feature being set or cleared (device, interface,
		endpoint).

	|UEVENT_GETDEVSTATUS|
		The USB host is requesting device status.  |pInfo| points to
		a |UCHAR[2]| array.  Byte 0 bit 1 should be set to the power 
		bit for self/bus powered, depending on the current state.  
		Byte 1 is reserved for future use.  The DataPump core will 
		initialize  byte 0 bit 0 to the remote-wakeup status, 
		assuming that the chip driver has reflected 
		SET_FEATURE(RemoteWakeupEnable) up to the DataPump core.

	|UEVENT_GETEPSTATUS|
		Sent to the |UPIPE|, |UDEVICE| and hardware chain when the
		core is trying to process a GET_STATUS(endpoint) command.
		|pInfo| points to a |UCHAR[2]|.  Normally, the event handler
		doesn't need to do anything; the common code will handle it.
		However, this hook gives the ability to return non-standard
		or extended status.

	|UEVENT_GETIFCSTATUS|
		Sent to the |UINTERFACE|, |UDEVICE| and hardware chain when the
		DataPump is trying to process a GET_STATUS(interface) command.
		|pInfo| points to a |UCHAR[2]| array in which the status is
		being built.

	|UEVENT_IFC_SET|
		An interface is becoming active because the USB host sent
		a SET_INTERFACE command.  |pInfo| points to the raw setup 
		packet, as a |PUCHAR|.  This event is reported to the
		activated |UINTERFACE|'s event queue, and also to all 
		|UPIPE|s that are being set up.  It is sent before the
		interface actually becomes active.  If the underlying
		USB hardware supports it, interface event code can fail
		the request, causing an error to be returned to the USB
		host.  Note, though, that many kinds of USB device silicon
		will accept and complete this command in hardware, before
		the DataPump gets a chance to generate this event.

		It is also sent to the hardware event queue to notify the
		hardware that the data toggles should be reset and to make
		any other hardware changes.

	|UEVENT_IFC_UNSET|
		An interface's alternate setting is becoming inactive as a 
		result of a SET_INTERFACE command from the USB host.  
		|pInfo| points to the raw setup packet,
		as a PUCHAR.  This event is reported to the going-inactive 
		|UINTERFACE|'s 	event queue, and also to all |UPIPE|s that 
		are being torn down.

	|UEVENT_INTLOAD|
		Do not use in new code.

	|UEVENT_RESET|
		Sent to the |UDEVICE| event queue; reports a USB reset.  If
		the device was configured, this will also show up at the 
		interface queues of the interfaces being torn down.  The 
		DataPump may create multiple |UEVENT_RESET|s for a single 
		hardware event, and may create |UEVENT_RESET|s for other 
		hardware events.

		Optionally (at the option of the code that generated the
		reset event), |pInfo| points to a UEVENTRESET_INFO structure, 
		which gives extra info about the reset.  If given, |fFinal| 
		will be |TRUE| if and only if this is the last UEVENT_RESET 
		for a given hardware event.  If not given, client code 
		should assume that this |UEVENT_RESET| is not final.  If 
		the |UEVENTRESET_INFO| is given, 
		it also passes an indication of nature of the hardware 
		event, for reference.

		The sequence of events for |UEVENT_RESET| is a (possibly 
		empty) 	series of non-final events followed by either a final
		|UEVENT_RESET|, or the causing event.

	|UEVENT_RESUME|
		Sent to the |UDEVICE| event queue; reports a USB bus resume.  
		|pInfo| is |NULL|.

	|UEVENT_SETADDR|
		Sent to the |UDEVICE| event queue; pInfo points to the raw
		setup packet (as a |PUCHAR|).   This event is only for use
		by the chip driver; the chip driver should verify that
		the set address looks reasonable for this hardware, but
		not set the address.  If the address is unreasonable, the
		chip driver should fail the request.
	
	|UEVENT_SETADDR_EXEC|
		Sent to the |UDEVICE| event queue; |pInfo| points to the raw
		setup packet (as a |PUCHAR|).  This event is only for use by 
		the chip driver.  The chip driver should finish the
		control transfer and arrange to actually set the address 
		after the ACK has been received for the status phase.

	|UEVENT_SUSPEND|
		Sent to the |UDEVICE| event queue; reports a suspend. pInfo
		is |NULL|.

	|UEVENT_ATTACH|
		Sent to the |UDEVICE| event queue; reports a bus attach.
		Not all chips can report this.  |pInfo| is |NULL|.

	|UEVENT_DETACH|
		Sent to the |UDEVICE| event queue; reports a bus detach.
		Not all chips can report this.  |pInfo| is |NULL|.

	|UEVENT_PLATFORM_EXTENSION|
		This event is for use by platform-specific code.  It is not
		used by common DataPump code, but can be used by platform code
		to send signals to (perhaps conditionally present0 higher-
		level code.

	|UEVENT_L1_SLEEP|
		Sent to the |UDEVICE| event queue; reports a sleep. pInfo is
		UEVENTL1SLEEP_INFO.

	|UEVENT_CABLE|
		Sent to the |UDEVICE| event queue; reports a cable detected.

	|UEVENT_NOCABLE|
		Sent to the |UDEVICE| event queue; reports a cable gone.

	|UEVENT_DETECT_ENDPOINT_ACTIVITY|
		Sent to the chip driver;  It is sent to the hardware event
		queue to notify the chip driver to enable the endpoint
		activity detection if the endpoint is inactive or disable the
		detection.  pInfo is UEVENT_DETECT_ENDPOINT_ACTIVITY_INFO.

	|UEVENT_VENDOR_CONTROL|
		Execute an extended (non chapter 9) default pipe vendor command.
		Prescan has passed, and the given setup packet should be
		operated upon.  |pInfo| points to a |UEVENTSETUP| structure.
		The packet is passed to the device level (using the UDEVICE's
		UEVENT chain).  The |UEVENTFN| should either set |uec_bAccept|
		for successful completion; set |uec_bReject| for
		recognition (but failure); or leave the |UEVENTSETUP| alone
		(if it doesn't recognize the setup packet).

	|UEVENT_VENDOR_CONTROL_PRE|
		Prescan for vendor (non-chapter-9 default-pipe) command handler.
		|pInfo| points to a |UEVENTSETUP|.  During command 
		pre-scan, |UEVENTFN|s should do no work other
		than checking to see if the command looks like one
		that the recipient should process.  If so, the |UEVENTFN|
		should simply set the |uec_bAccept| flag, and wait to receive
		the |UEVENT_VENDOR_CONTROL| message to do the operation.

	|UEVENT_SET_SEL|
		Sent to the chip driver;  It is sent to the hardware event
		queue to notify the chip driver to set up exit latency values
		for device controller.  pInfo is UEVENT_SET_SEL_INFO.

	|UEVENT_SET_ISOCH_DELAY|
		Sent to the chip driver;  It is sent to the hardware event
		queue to notify the chip driver to set up isochronous delay
		value for device controller.
		The pInfo is UEVENT_SET_ISOCH_DELAY_INFO.

	|UEVENT_FUNCTION_SUSPEND|
		Sent to the UINTERFACE;  It is sent to the UINTERFACE event
		queue to notify function suspend event to the interface.
		The pInfo is UEVENT_FUNCTION_SUSPEND_INFO.

	|UEVENT_FUNCTION_RESUME|
		Sent to the UINTERFACE;  It is sent to the UINTERFACE event
		queue to notify function resume event to the interface.

	|UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE|
		Sent to the UINTERFACE;  It is sent to the UINTERFACE event
		queue to get function remote wake capability information.
		The pInfo is UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE_INFO.

	|UEVENT_DEVICE_NOTIFICATION|
		Sent to the chip driver;  It is sent to the chip driver to send
		USB 3.0 device notification transaction such as function wake,
		latency tolerance message and bus interval adjustment message.
		The pInfo is UEVENT_DEVICE_NOTIFICATION_INFO.

	|UEVENT_U1_SLEEP|
		Sent to the |UDEVICE| event queue; reports U1 sleep.

	|UEVENT_U2_SLEEP|
		Sent to the |UDEVICE| event queue; reports U2 sleep.

	|UEVENT_EXIT_U1_U2|
		Sent to the chip driver;  It is sent to the hardware event
		queue to notify the chip driver to exit U1 or U2 sleep state.

Notes:
	A table initializer, |UEVENT_NAMES__INIT|, is created.  This can be
	used to initialize a |char *x[]| array for debugging purposes.  The
	resulting array can be indexed by |UEVENT| to get a printable version
	of the event.

See Also:
	UEVENTNODE, UEVENTFN, UEVENTFEATURE, UEVENTDATAPLANE_INFO,
	__TMS_PR6866_DISABLE

*/

/* the event codes */
/* __TMS_TYPE_DEF_ARG(UEVENT, BYTES); */

#define	__TMS_UEVENT_CONFIG_SET 0	/* configuration change -- evinfo points
					|| to setup packet; passed to new config.
					*/
#define __TMS_UEVENT_CONFIG_UNSET 1	/* configuration change -- evinfo points
					|| to setup packet; posted to old config.
					*/
#define	__TMS_UEVENT_IFC_SET	2	/* interface change -- evinfo points to
					|| setup packet; posted to new ifc.
					*/
#define	__TMS_UEVENT_IFC_UNSET	3	/* interface change -- evinfo points to
					|| setup packet; posted to old ifc.
					*/
#define	__TMS_UEVENT_FEATURE	4	/* set/clear feature, 
					|| device/interface/endpoint;
					|| -- evinfo points to event packet.
					*/
#define	__TMS_UEVENT_CONTROL	5	/* control packet outcall; used for 
					|| vendor-specific packets.  evinfo
					|| points to UEVENTSETUP.
					*/
#define	__TMS_UEVENT_SUSPEND	6	/* enter suspend state (device) evinfo:
					|| NULL
					*/
#define	__TMS_UEVENT_RESUME	7	/* leave suspend state (device) evinfo:
					|| NULL
					*/
#define	__TMS_UEVENT_RESET	8	/* USB reset received (device) evinfo:
					|| NULL
					*/
#define	__TMS_UEVENT_SETADDR	9	/* set address received (USETUP) evinfo:
					||  pointer to raw setup packet.
					*/
#define	__TMS_UEVENT_CONTROL_PRE 10	/* pre-scan to test for control packet. 
					|| evinfo points to UEVENTSETUP
					*/
#define	__TMS_UEVENT_INTLOAD	11	/* an interrupt-load event has occurred:
					|| info points to the master interrupt
					|| structure.  Only applies to interrupt
					|| event queues.
					|| evinfo points to UINTSTRUCT
					*/
#define	__TMS_UEVENT_GETDEVSTATUS 12	/* get device status:  evinfo points to
					||  buffer to be filled in.  Byte 0 gets
					||  the power bit for self/bus powered,
					||  depending on state, byte 1 is
					||  reserved.  Byte0 bit 0 is already
					||  set to remote wakeup status from
					||  portable data base.
					*/
#define	__TMS_UEVENT_GETIFCSTATUS 13	/* get interface status:  evinfo points to
					||  buffer to be filled in, 2 bytes long.
					*/
#define	__TMS_UEVENT_GETEPSTATUS 14	/* get endpoint status:  evinfo points to 
					||  buffer to be filled in.
					*/
#define	__TMS_UEVENT_SETADDR_EXEC 15	/* execute a set-addr command; evinfo
					||  points to raw setup packet.
					*/
#define	__TMS_UEVENT_DATAPLANE	16	/* special nested event for dataplanes.
					||  evinfo points to UEVENTDATAPLANE_INFO
					*/
#define	__TMS_UEVENT_ATTACH	17	/* bus attach event (not always possible):
					||  evinfo is NULL.
					*/
#define	__TMS_UEVENT_DETACH	18	/* bus detach event (not always possible):
					||  evinfo is NULL.
					*/
#define	__TMS_UEVENT_PLATFORM_EXTENSION 19
					/* a platform-specific extension --
					||  this is for use by platfrom-specific
					||  code.  The evinfo points to a
					||  UEVENTPLATFORMEXTENSION_INFO object.
					*/
#define	__TMS_UEVENT_L1_SLEEP	20	/* enter the sleep (L1) state -- evinfo:
					|| points to UEVENTL1SLEEP_INFO
					*/
#define	__TMS_UEVENT_CABLE	21	/* cable detected (device) evinfo:
					|| NULL
					*/
#define	__TMS_UEVENT_NOCABLE	22	/* cable missed (device) evinfo:
					|| NULL
					*/
#define	__TMS_UEVENT_DETECT_ENDPOINT_ACTIVITY 23
					/* post to DCD to detect endpoint
					|| activity. evinfo: NULL
					*/
#define	__TMS_UEVENT_VENDOR_CONTROL	24
					/* control packet outcall; used for 
					|| vendor-specific packets.  evinfo
					|| points to UEVENTSETUP.
					*/
#define	__TMS_UEVENT_VENDOR_CONTROL_PRE 25
					/* pre-scan to test for vendor control
					|| packet. evinfo points to UEVENTSETUP
					*/
#define	__TMS_UEVENT_SET_SEL	26	/*
					|| post to DCD to set SEL & PEL. (USB3)
					|| evinfo: UEVENT_SET_SEL_INFO.
					*/
#define	__TMS_UEVENT_SET_ISOCH_DELAY	27
					/*
					|| post to DCD to set isochronous delay.
					|| evinfo: UEVENT_SET_ISOCH_DELAY_INFO.
					*/
#define	__TMS_UEVENT_FUNCTION_SUSPEND	28
					/*
					|| post to interface to notify function
					|| suspend event. The evinfo points to
					|| UEVENT_FUNCTION_SUSPEND_INFO.
					*/
#define	__TMS_UEVENT_FUNCTION_RESUME	29
					/*
					|| post to interface to notify function
					|| resume event. The evinfo: NULL.
					*/
#define	__TMS_UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE 30
					/*
					|| post to interface to get function
					|| remote wake capable. The evinfo:
					|| UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE_INFO.
					*/
#define	__TMS_UEVENT_DEVICE_NOTIFICATION 31
					/*
					|| post to DCD to send device
					|| notification.  The evinfo points to
					|| UEVENT_DEVICE_NOTIFICATION_INFO.
					*/
#define	__TMS_UEVENT_U1_SLEEP	32	/* enter the sleep (U1) state. Post to
					|| the device notification event queue.
					|| The evinfo: NULL.
					*/
#define	__TMS_UEVENT_U2_SLEEP	33	/* enter the sleep (U2) state. Post to
					|| the device notification event queue.
					|| The evinfo: NULL.
					*/
#define	__TMS_UEVENT_EXIT_U1_U2	34	/* post to the chip driver to exit U1 or
					|| U2 sleep state.  The evinfo: NULL.
					*/

/*
|| When you add names to the above list, please also update the following
|| debugging table.
*/
#define	__TMS_UEVENT_NAMES__INIT \
	{ \
	"UEVENT_CONFIG_SET", \
	"UEVENT_CONFIG_UNSET", \
	"UEVENT_IFC_SET", \
	"UEVENT_IFC_UNSET", \
	"UEVENT_FEATURE", \
	"UEVENT_CONTROL", \
	"UEVENT_SUSPEND", \
	"UEVENT_RESUME", \
	"UEVENT_RESET", \
	"UEVENT_SETADDR", \
	"UEVENT_CONTROL_PRE", \
	"UEVENT_INTLOAD", \
	"UEVENT_GETDEVSTATUS", \
	"UEVENT_GETIFCSTATUS", \
	"UEVENT_GETEPSTATUS", \
	"UEVENT_SETADDR_EXEC", \
	"UEVENT_DATAPLANE", \
	"UEVENT_ATTACH", \
	"UEVENT_DETACH", \
	"UEVENT_PLATFORM_EXTENSION", \
	"UEVENT_L1_SLEEP", \
	"UEVENT_CABLE", \
	"UEVENT_NOCABLE", \
	"UEVENT_DETECT_ENDPOINT_ACTIVITY", \
	"UEVENT_VENDOR_CONTROL", \
	"UEVENT_VENDOR_CONTROL_PRE", \
	"UEVENT_SET_SEL", \
	"UEVENT_SET_ISOCH_DELAY", \
	"UEVENT_FUNCTION_SUSPEND", \
	"UEVENT_FUNCTION_RESUME", \
	"UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE", \
	"UEVENT_DEVICE_NOTIFICATION", \
	"UEVENT_U1_SLEEP", \
	"UEVENT_U2_SLEEP", \
	"UEVENT_EXIT_U1_U2" \
	}

/*

Type:	UEVENTFN

Function:
	Process UEVENT notifications for a UEVENTNODE.

Definition:
	typedef VOID (UEVENTFN)(
			UDEVICE *pDevice,
			UEVENTNODE *pEventNode,
			UEVENT EventCode,
			VOID *pEventSpecificInfo
			);

	typedef UEVENTFN *PUEVENTFN;

Description:
	UEVENTFN specifies the functional type for event-processing
	functions that are associated with UEVENTNODEs.

	When an event is dispatched to a specific UEVENTNODE, the
	function is called with pDevice set to the governing UDEVICE,
	pEventNode pointing to the UEVENTNODE in question, EventCode set
	to the event code, and pEventSpecificInfo set according to the
	kind of event.

	A context field is placed in the UEVENTNODE, for the exclusive
	use of the UEVENTFN.

	Each UEVENTNODE, therefore, has associated with it a UEVENTFN 
	function, provided by the code which created the UEVENTNODE.  All 
	such functions share a common prototype, UEVENTFN, and should be 
	declared (in header files) using that type, rather than with an 
	explicit prototype.

	For example, if a particular module creates a UEVENTNODE with 
	a specific event handling function named UsbWmc_IfcEvent, 
	then the *header file* should prototype the function using:

		UEVENTFN UsbWmc_IfcEvent;

	rather than writing:
		VOID UsbWmc_IfcEvent(
			UDEVICE *,
			UEVENTNODE *,
			UEVENT,
			VOID *
			);
			
	If you want to have a prototype for reference in the code, you
	should write the prototype *twice*:

		UEVENTFN UsbWmc_IfcEvent;

		// for reference, the above is equivalent to:
		VOID UsbWmc_IfcEvent(
			UDEVICE *,
			UEVENTNODE *,
			UEVENT,
			VOID *
			);
	
	The justification for this design approach is that it highlights
	the fact that the function prototype is not under the control of
	the function implementor, but rather highlights the fact that the
	function is a method implementation for some class.

Returns:
	Event functions return no specific result.

*/

/* the event function */
__TMS_FNTYPE_DEF(
	UEVENTFN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *, 
		__TMS_UEVENTNODE *, 
		__TMS_UEVENT, 
		__TMS_VOID *evinfo
		)
	);

/*

Type:	UEVENTNODE

Function:
	The generic object used for registering for outcalls from lower
	levels.

Description:
	USB device-side code is under the control of the host computer;
	for correct behavior, device class code must respond correctly to
	specific bus and chapter-9 events.  However, since these are
	effectively up-calls, the USB DataPump cannot directly issue the
	upcalls, unless we were to use conditional compiles or otherwise
	directly include all possible protocols into the core product.

	Instead, USB DataPump uses a consistent mechanism to connect from 
	lower levels to upper levels.  During initailization, upper levels
	allocate and prepare data structures (UEVENTNODEs); they then attach
	these structures to linked lists at predetermined places in the 
	UDEVICE tree.  Then, when a significant event occurs, the core
	DataPump traverses the appropriate list or lists of UEVENTNODEs,
	and reports the event to each member of the list.

	"Reporting an event" is implemented abstractly; each UEVENTNODE
	contains a pointer to a function of type UEVENTFN; the core
	DataPump merely calls the function with a specified set of parameters.

Contents:
	UEVENTFN *uev_pfn;
		The function to be called.
	
	VOID *uev_ctx;
		A context pointer for use by the function.

Private Contents:
	UEVENTNODE *uev_next;
		pointer to the next UEVENTNODE in the list.  Maintained
		by the core DataPump.

	UEVENTNODE *uev_last;

Notes:
	UEVENTNODEs are designed to be embedded, if necessary, in larger
	structures.  The normal MCCI strategy is followed.

	If you are creating a derived UEVENTNODE type, you should define it as
	follows:

		struct myeventnode
			{
			UEVENTNODE_HDR;

			// my additional contents...
			};

	That way, you can refer directly to uev_... names; all the proper
	declarations are hidden in |UEVENTNODE_HDR|.

See Also:
	UEVENTFN, UEVENT.

Bugs:
	The structure tag of UEVENTNODE is not __TMS_STRUCTNAME(UEVENTNODE); 
	for historical reasons, it is TTUSB_EVENTNODE.  This name is
	obsolescent, and will change in a future version, so please use
	the cloaked name (__TMS_UEVENTNODE) rather than |struct
	TTUSB_EVENTNODE| if you need to refer to the structure without
	using undecorated names.

*/

/* the base-class definitions */
struct TTUSB_EVENTNODE_HDR
	{
	__TMS_PUEVENTNODE	uevhh_next;	/* forward link */
	__TMS_PUEVENTNODE	uevhh_last;	/* back link */
	__TMS_PUEVENTFN		uevhh_pfn;	/* function to call */
	__TMS_VOID		*uevhh_ctx;	/* context for function */
	};

/* the macro to embed at the head of derived structures */
#define	__TMS_UEVENTNODE_HDR	struct TTUSB_EVENTNODE_HDR uev_hh

/* the generic version of the structure */
struct TTUSB_EVENTNODE
	{
	__TMS_UEVENTNODE_HDR;
	};

/* sane names */
#define	uev_next	uev_hh.uevhh_next
#define	uev_last	uev_hh.uevhh_last
#define	uev_pfn		uev_hh.uevhh_pfn
#define	uev_ctx		uev_hh.uevhh_ctx

/**** here's a union for the "feature" event types ****/
__TMS_TYPE_DEF(UEVENTFEATURE_EXTRAPTR, union TTUSB_EXTRAPTR);
union	TTUSB_EXTRAPTR
	{
	__TMS_UDEVICE	*pDevice;
	__TMS_UINTERFACESET *pInterfaceSet;
	__TMS_UINTERFACE *pInterface;
	__TMS_UPIPE	*pPipe;
	__TMS_UENDPOINT *pEndpoint;
	};				/* the 'relevant' structure */

/* __TMS_TYPE_DEF(UEVENTFEATURE, struct TTUSB_UEVENT_FEATURE); */
struct TTUSB_UEVENT_FEATURE
	{
	__TMS_USHORT	uef_feature;			/* feature code */
	__TMS_USHORT	uef_index;			/* index */
	__TMS_USHORT	uef_value;			/* the value */
	__TMS_UINT8	*uef_setup;			/* setup packet itself */
	__TMS_UEVENTFEATURE_EXTRAPTR uef_relstruct;	/* the 'relevant' structure */
	};

/*

Type:	UEVENTSETUP

Function:
	Event information block for UEVENT_CONTROL, UEVENT_CONTROL_PRE.

Description:
	When processing setup packets, this structure is passed via the
	information pointer.  Each registered event processor should look
	at the setup packet, and determine whether it will process the
	event itself.  It must then modify the UEVENTSETUP block
	appropriately.

Contents:
	UCHAR uec_accept;
		An event processor should change this to TRUE if it intends
		to process the setup.

	UCHAR uec_reject;
		An event processor should change this to TRUE if it intends
		to reject the setup.

	USETUP uec_ucp;
		The setup packet itself, converted to local byte order and
		lightly decoded.

See Also:
	USETUP

*/

/* __TMS_TYPE_DEF(UEVENTSETUP, struct TTUSB_UEVENTSETUP); */
struct TTUSB_UEVENTSETUP
	{
	__TMS_UCHAR	uec_accept;		/* must be set TRUE to accept */
	__TMS_UCHAR	uec_reject;		/* must be FALSE to accept */
	__TMS_USETUP	uec_ucp;		/* the control packet data */
	};

/*

Type:	UEVENTDATAPLANE_INFO

Function:
	C structure which carries information for UEVENT_DATAPLANE.

Description:
	UEVENT_DATAPLANE processing needs to know which interface the event
	came from.  Rather than break everything else, we add a new event,
	which passes a pointer to this structure as the info parameter.

Contents:
	UINTERFACE *pInterface;
		The reference interface.

	UEVENT InterfaceEventCode;
		The event code that was passed to UsbReportInterfaceEvent().

	VOID *pInterfaceEventInfo;
		The original event information pointer; interpret this
		according to the value of InterfaceEventCode.

*/

__TMS_TYPE_DEF_STRUCT(UEVENTDATAPLANE_INFO);

struct __TMS_STRUCTNAME(UEVENTDATAPLANE_INFO)
	{
	__TMS_UINTERFACE	*pInterface;		/* where from */
	__TMS_UEVENT		InterfaceEventCode;	/* why we're here */
	__TMS_VOID		*pInterfaceEventInfo;	/* and the info */
	};

/*

Type:	UEVENTRESET_INFO

Function:
	C structure which carries information for UEVENT_RESET.

Description:
	UEVENT_RESET processing gets invoked for several reasons.  Sometimes
	clients want more information to let them figure out exactly why the
	reset is happening.  Therefore, UEVENT_RESET carries an optional 
	info pointer; if non-NULL, it points to a UEVENTRESET_INFO structure.

Contents:
	UEVENT RealEventCode;
		The original event that caused the reset

	VOID *pEventInfo;
		Interpret this according to the value of RealEventCode.

	BOOL fFinal;
		TRUE if and only if this is the last message for this reset
		event.

	BOOL fChangedState;
		If fFinal, then TRUE if and only if this changed the state of
		the core DataPump.

*/

__TMS_TYPE_DEF_STRUCT(UEVENTRESET_INFO);

struct __TMS_STRUCTNAME(UEVENTRESET_INFO)
	{
	__TMS_UEVENT		RealEventCode;	/* why we're here */
	__TMS_VOID		*pEventInfo;	/* and the info */
	__TMS_BOOL		fFinal;		/* TRUE if this changed our state */
	__TMS_BOOL		fChangedState;	/* TRUE if this made any difference */
	};

#define __TMS_UEVENTRESET_INFO_SETUP_V1(				\
		pArg, ARG_RealEventCode, ARG_pEventInfo, ARG_fFinal)	\
	do	{							\
		(pArg)->RealEventCode = (ARG_RealEventCode);		\
		(pArg)->pEventInfo = (ARG_pEventInfo);			\
		(pArg)->fFinal = (ARG_fFinal);				\
		(pArg)->fChangedState = 0;				\
	} while (0)

/*

Type:	UEVENTPLATFORMEXTENSION_INFO

Function:
	C structure which carries information for UEVENT_PLATFORM_EXTENSION.

Description:
	UEVENT_PLATFORM_EXTENSION processing is used only for communication
	by platform-specific contents.  It's normally issued by platform-level
	code for making outcalls (for example in response to incoming
	messages to the USB task) without binding to outcall code by name.

Contents:
	UINT fAccept;
		The event processor must set fAccept to TRUE in order to
		signal that it has accepted the operation.  This is
		an OUT parameter.  After event processing, if this field
		is FALSE, the issuer of the event shall assume that no client
		claimed the event.

		THe macros initialize this field to false.

	UINT32 ulCode;
		An event code, for use by the 

	VOID *pEventInfo;
		Interpret this according to the value of ulCode, and according
		to conventions established between the caller and the
		receiver.

	SIZE_T sizeEventInfo;
		size in bytes of the buffer pointed to by pEventInfo; if 0,
		the buffer is empty.

See also:
	UEVENTPLATFORM_EXTENSION_INFO_SETUP_V1()

*/

__TMS_TYPE_DEF_STRUCT(UEVENTPLATFORMEXTENSION_INFO);

struct __TMS_STRUCTNAME(UEVENTPLATFORMEXTENSION_INFO)
	{
	__TMS_UINT32		ulCode;		/* a 'type code' -- opaque to
						||  DataPump, may be used by
						||  platform or ignored.
						*/
	__TMS_VOID		*pEventInfo;	/* pointer to extra information */
	__TMS_SIZE_T		sizeEventInfo;	/* size of eventinfo buffer in bytes */
	__TMS_UINT		fAccept;	/* OUT: if TRUE -- it was claimed */
	};

/*

Name:	UEVENTPLATFORM_EXTENSION_INFO_SETUP_V1()

Function:
	Macro: runtime initialization code for UEVENTPLATFORMEXTENSION_INFO()

Definition:
	VOID UEVENTPLATFORMEXTENSION_INFO_SETUP_V1(
		UEVENTPLATFORMEXTENSION_INFO *pObject,
		UINT32	ulCode,
		VOID	*pEventInfo,
		SIZE_T	sizeEventInfo
		);

Description:
	This macro correctly initializes a UEVENTPLATFORMEXTENSON_INFO
	object.  If the layout of the object changes inthe future, MCCI will
	revise the expansion of this macro to initialize the new elements in a 
	forward-compatible way, and will create a new version of this macro,
	if appropriate, for initializing the new fields.

Returns:
	No explicit result.

*/

#define __TMS_UEVENTPLATFORMEXTENSION_INFO_SETUP_V1(			\
		pArg,							\
		ARG_ulCode,						\
		ARG_pEventInfo,						\
		ARG_sizeEventInfo					\
		)							\
	do	{							\
		(pArg)->ulCode = (ARG_ulCode);				\
		(pArg)->pEventInfo = (ARG_pEventInfo);			\
		(pArg)->sizeEventInfo = (ARG_sizeEventInfo);		\
		(pArg)->fAccept = 0;					\
	} while (0)

/*

Type:	UEVENTL1SLEEP_INFO

Function:
	C structure which carries information for UEVENT_L1_SLEEP.

Description:
	When processing LPM token, this structure is passed via the information
	pointer. Clients could look at the UEVENTL1SLEEP_INFO block and determine
	whether it will process the event itself.

Contents:
	BOOL fRemoteWakeUp;
		Enable the addressed device to wake the host upon any meaningful
		application-specific event.

	UINT HostInitiateResumeDuration;
		Host Initiated Resume Duration (HIRD); The host system
		communicates to the device the durtion of how long the host will
		driver resume when it initiates exit from L1. We use 1 us as
		unit.

*/

__TMS_TYPE_DEF_STRUCT(UEVENTL1SLEEP_INFO);

struct __TMS_STRUCTNAME(UEVENTL1SLEEP_INFO)
	{
	__TMS_BOOL		fRemoteWakeUp;
	__TMS_UINT		HostInitiateResumeDuration;
	};

#define __TMS_UEVENTL1SLEEP_INFO_SETUP_V1(			\
	ARG_pArg,						\
	ARG_fRemoteWakeUp,					\
	ARG_HostInitiateResumeDuration				\
	)							\
    do	{							\
	(ARG_pArg)->fRemoteWakeUp = (ARG_fRemoteWakeUp);	\
	(ARG_pArg)->HostInitiateResumeDuration =		\
		(ARG_HostInitiateResumeDuration);		\
	} while (0)

/*

Type:	UEVENT_DETECT_ENDPOINT_ACTIVITY_INFO

Function:
	C structure which carries information for
	UEVENT_DETECT_ENDPOINT_ACTIVITY.

Description:
	It is sent to the hardware event queue to notify the chip driver to
	enable/disable the endpoint activity detection.

Contents:
	BOOL fDetect;
		Enable the endponit activity detection if TRUE. Disalbe the
		detection if FALSE.

*/

__TMS_TYPE_DEF_STRUCT(UEVENT_DETECT_ENDPOINT_ACTIVITY_INFO);

struct __TMS_STRUCTNAME(UEVENT_DETECT_ENDPOINT_ACTIVITY_INFO)
	{
	__TMS_BOOL	fDetect;
	};

#define __TMS_UEVENT_DETECT_ENDPOINT_ACTIVITY_INFO_SETUP_V1(	\
	ARG_pArg,						\
	ARG_fDetect						\
	)							\
    do	{							\
	(ARG_pArg)->fDetect = (ARG_fDetect);			\
	} while (0)

/*

Type:	UEVENT_SET_SEL_INFO

Function:
	C structure which carries information for UEVENT_SET_SEL.

Description:
	It is sent to the hardware event queue to notify the chip driver to
	set both the U1 and U2 System Exit Latency and the U1 or U2 exit
	latency for all the links between a device and a root port on the
	host.

Contents:
	UINT8 U1SEL;
		Time in us for U1 System Exit Latency.

	UINT8 U1PEL;
		Time in us for U1 Device to Host Exit Latency.

	UINT16 U2SEL;
		Time in us for U2 System Exit Latency.

	UINT16 U2PEL;
		Time in us for U2 Device to Host Exit Latency.

*/

__TMS_TYPE_DEF_STRUCT(UEVENT_SET_SEL_INFO);

struct __TMS_STRUCTNAME(UEVENT_SET_SEL_INFO)
	{
	__TMS_UINT8	U1SEL;
	__TMS_UINT8	U1PEL;
	__TMS_UINT16	U2SEL;
	__TMS_UINT16	U2PEL;
	};

#define __TMS_UEVENT_SET_SEL_INFO_SETUP_V1(			\
	ARG_pArg,						\
	ARG_U1SEL,						\
	ARG_U1PEL,						\
	ARG_U2SEL,						\
	ARG_U2PEL						\
	)							\
    do	{							\
	(ARG_pArg)->U1SEL = (ARG_U1SEL);			\
	(ARG_pArg)->U1PEL = (ARG_U1PEL);			\
	(ARG_pArg)->U2SEL = (ARG_U2SEL);			\
	(ARG_pArg)->U2PEL = (ARG_U2PEL);			\
	} while (0)

/*

Type:	UEVENT_SET_ISOCH_DELAY_INFO

Function:
	C structure which carries information for UEVENT_SET_ISOCH_DELAY.

Description:
	It is sent to the hardware event queue to notify the chip driver to
	inform the device of the delay from the time a host transmits a packet
	to the time it is received by the device.

Contents:
	UINT16 Delay;
		Time in ns for delay.

*/

__TMS_TYPE_DEF_STRUCT(UEVENT_SET_ISOCH_DELAY_INFO);

struct __TMS_STRUCTNAME(UEVENT_SET_ISOCH_DELAY_INFO)
	{
	__TMS_UINT16	Delay;
	};

#define __TMS_UEVENT_SET_ISOCH_DELAY_INFO_SETUP_V1(		\
	ARG_pArg,						\
	ARG_Delay						\
	)							\
    do	{							\
	(ARG_pArg)->Delay = (ARG_Delay);			\
	} while (0)

/*

Type:	UEVENT_FUNCTION_SUSPEND_INFO

Function:
	C structure which carries information for UEVENT_FUNCTION_SUSPEND.

Description:
	It is sent to the interface event queue to notify the interface to
	inform received FUNCTION_SUSPEND feature.

Contents:
	BOOL fRemoteWakeEnable;
		TRUE if function remote wake enabled.

*/

__TMS_TYPE_DEF_STRUCT(UEVENT_FUNCTION_SUSPEND_INFO);

struct __TMS_STRUCTNAME(UEVENT_FUNCTION_SUSPEND_INFO)
	{
	__TMS_BOOL	fRemoteWakeEnable;
	};

#define __TMS_UEVENT_FUNCTION_SUSPEND_INFO_SETUP_V1(		\
	ARG_pArg,						\
	ARG_fRemoteWakeEnable					\
	)							\
    do	{							\
	(ARG_pArg)->fRemoteWakeEnable = (ARG_fRemoteWakeEnable);\
	} while (0)

/*

Type:	UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE_INFO

Function:
	C structure which carries information for the
	UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE.

Description:
	It is sent to the interface event queue to get function remote wake
	capable from the interface.

Contents:
	BOOL fFunctionRemoteWakeCapable;
		TRUE if function can be remote wake capable.

*/

__TMS_TYPE_DEF_STRUCT(UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE_INFO);

struct __TMS_STRUCTNAME(UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE_INFO)
	{
	__TMS_BOOL	fFunctionRemoteWakeCapable;
	};

#define __TMS_UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE_INFO_SETUP_V1(	\
	ARG_pArg,							\
	ARG_fFunctionRemoteWakeCapable					\
	)								\
    do	{								\
	(ARG_pArg)->fFunctionRemoteWakeCapable =			\
		(ARG_fFunctionRemoteWakeCapable);			\
	} while (0)

/*

Type:	UEVENT_DEVICE_NOTIFICATION_INFO

Function:
	C structure which carries information for UEVENT_DEVICE_NOTIFICATION.

Description:
	It is sent to the interface event queue to notify the interface to
	inform received FUNCTION_SUSPEND feature.

Contents:
	UINT NotificationType;
		Notification type. One of following value.
			UEVNET_DEVICE_NOTIFICATION_TYPE_FUNCTION_WAKE
			UEVNET_DEVICE_NOTIFICATION_TYPE_LTM
			UEVNET_DEVICE_NOTIFICATION_TYPE_BUS_INTERVAL_ADJUST

	UINT Interface;
		Interface number. It is valid only if NotiFicationType is
		UEVNET_DEVICE_NOTIFICATION_TYPE_FUNCTION_WAKE.

	UINT Belt;
		The Best Effort Latency Tolerance value. It is valid only if
		NotiFicationType is UEVNET_DEVICE_NOTIFICATION_TYPE_LTM.

	UINT BusIntervalAdjust;
		The Bus Interval Adjustment value.
		It is valid only if NotiFicationType is
		UEVNET_DEVICE_NOTIFICATION_TYPE_BUS_INTERVAL_ADJUST

*/

__TMS_TYPE_DEF_STRUCT(UEVENT_DEVICE_NOTIFICATION_INFO);

struct __TMS_STRUCTNAME(UEVENT_DEVICE_NOTIFICATION_INFO)
	{
	__TMS_UINT	NotificationType;
	__TMS_UINT	Interface;
	__TMS_UINT	Belt;
	__TMS_UINT	BusIntervalAdjust;
	};

#define	__TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_FUNCTION_WAKE		1
#define	__TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_LTM			2
#define	__TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_BUS_INTERVAL_ADJUST	3

#define __TMS_UEVENT_DEVICE_NOTIFICATION_INFO_FUNCTION_WAKE_SETUP_V1(	\
	ARG_pArg,							\
	ARG_Interface							\
	)								\
    do	{								\
	(ARG_pArg)->NotificationType =					\
		__TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_FUNCTION_WAKE;	\
	(ARG_pArg)->Interface = (ARG_Interface);			\
	(ARG_pArg)->Belt = 0;			/* not used */		\
	(ARG_pArg)->BusIntervalAdjust = 0;	/* not used */		\
	} while (0)

#define __TMS_UEVENT_DEVICE_NOTIFICATION_INFO_LTM_SETUP_V1(		\
	ARG_pArg,							\
	ARG_Belt							\
	)								\
    do	{								\
	(ARG_pArg)->NotificationType =					\
		__TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_LTM;		\
	(ARG_pArg)->Interface = 0;		/* not used */		\
	(ARG_pArg)->Belt = (ARG_Belt);					\
	(ARG_pArg)->BusIntervalAdjust = 0;	/* not used */		\
	} while (0)

#define __TMS_UEVENT_DEVICE_NOTIFICATION_INFO_BUS_INTERVAL_ADJUST_SETUP_V1( \
	ARG_pArg,							\
	ARG_BusIntervalAdjust						\
	)								\
    do	{								\
	(ARG_pArg)->NotificationType =					\
	    __TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_BUS_INTERVAL_ADJUST;	\
	(ARG_pArg)->Interface = 0;		/* not used */		\
	(ARG_pArg)->Belt = 0;			/* not used */		\
	(ARG_pArg)->BusIntervalAdjust = (ARG_BusIntervalAdjust);	\
	} while (0)

/****************************************************************************\
|
|	The APIs
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID
UsbAddEventNode(
	__TMS_UEVENTNODE **, 
	__TMS_UEVENTNODE *, 
	__TMS_UEVENTFN *, 
	__TMS_VOID *
	);

__TMS_CONST __TMS_TEXT *
UsbPumpEvent_Name(
	__TMS_UEVENT	/*EventCode*/
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/*

Macro:	__TMS_PR6866_DISABLE

Function:
	Restore old compile-time behavior of usbkern/i/ueventnode.h

Description:
	Due to a bug (reported in PR6866), cloaking was once not
	handled properly for the following macros:

		UEVENT_CONFIG_SET
		UEVENT_CONFIG_UNSET
		UEVENT_IFC_SET
		UEVENT_IFC_UNSET
		UEVENT_FEATURE
		UEVENT_CONTROL
		UEVENT_SUSPEND
		UEVENT_RESUME
		UEVENT_RESET
		UEVENT_SETADDR
		UEVENT_CONTROL_PRE
		UEVENT_INTLOAD
		UEVENT_GETDEVSTATUS
		UEVENT_GETIFCSTATUS
		UEVENT_GETEPSTATUS
		UEVENT_SETADDR_EXEC
		UEVENT_DATAPLANE
		UEVENT_ATTACH
		UEVENT_DETACH
		UEVENT_PLATFORM_EXTENSION
		UEVENT_NAMES__INIT

	Although this has been corrected, it is possible that old customer
	code has cloaking enabled but uses the uncloaked names (because
	prior to the fix for PR6866, there was no other option).

	Rather than require that customers who have this problem change
	their code, the uncloaking for the above symbols is further 
	controlled by the compile-time parameter __TMS_PR6866_DISABLE.
	
	If this parameter is not defined, then i/ueventnode.h will properly
	handle cloaking for the above symbols.  If this parameter is
	defined to a non-zero value, then i/ueventnode.h will always define
	both the uncloaked and the cloaked names (but only for the above-
	mentioned symbols -- other symbols are not affected and will be
	uncloaked in the normal way.

*/

/* be careful to keep support for __TMS_PR6866_DISABLE! */
/* +++ begin manual cloaking +++ */
#ifndef __TMS_PR6866_DISABLE	/* PARAM */
# define __TMS_PR6866_DISABLE 0	/* if not overridden, control using cloaking */
#endif /* __TMS_PR6866_DISABLE */
/* --- end manual cloaking --- */

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY || __TMS_PR6866_DISABLE
# define UEVENT_CONFIG_SET	\
   __TMS_UEVENT_CONFIG_SET
# define UEVENT_CONFIG_UNSET	\
   __TMS_UEVENT_CONFIG_UNSET
# define UEVENT_IFC_SET	\
   __TMS_UEVENT_IFC_SET
# define UEVENT_IFC_UNSET	\
   __TMS_UEVENT_IFC_UNSET
# define UEVENT_FEATURE	\
   __TMS_UEVENT_FEATURE
# define UEVENT_CONTROL	\
   __TMS_UEVENT_CONTROL
# define UEVENT_SUSPEND	\
   __TMS_UEVENT_SUSPEND
# define UEVENT_RESUME	\
   __TMS_UEVENT_RESUME
# define UEVENT_RESET	\
   __TMS_UEVENT_RESET
# define UEVENT_SETADDR	\
   __TMS_UEVENT_SETADDR
# define UEVENT_CONTROL_PRE	\
   __TMS_UEVENT_CONTROL_PRE
# define UEVENT_INTLOAD	\
   __TMS_UEVENT_INTLOAD
# define UEVENT_GETDEVSTATUS	\
   __TMS_UEVENT_GETDEVSTATUS
# define UEVENT_GETIFCSTATUS	\
   __TMS_UEVENT_GETIFCSTATUS
# define UEVENT_GETEPSTATUS	\
   __TMS_UEVENT_GETEPSTATUS
# define UEVENT_SETADDR_EXEC	\
   __TMS_UEVENT_SETADDR_EXEC
# define UEVENT_DATAPLANE	\
   __TMS_UEVENT_DATAPLANE
# define UEVENT_ATTACH	\
   __TMS_UEVENT_ATTACH
# define UEVENT_DETACH	\
   __TMS_UEVENT_DETACH
# define UEVENT_PLATFORM_EXTENSION	\
   __TMS_UEVENT_PLATFORM_EXTENSION
# define UEVENT_L1_SLEEP	\
   __TMS_UEVENT_L1_SLEEP
# define UEVENT_CABLE	\
   __TMS_UEVENT_CABLE
# define UEVENT_NOCABLE	\
   __TMS_UEVENT_NOCABLE
# define UEVENT_DETECT_ENDPOINT_ACTIVITY	\
   __TMS_UEVENT_DETECT_ENDPOINT_ACTIVITY
# define UEVENT_VENDOR_CONTROL	\
   __TMS_UEVENT_VENDOR_CONTROL
# define UEVENT_VENDOR_CONTROL_PRE	\
   __TMS_UEVENT_VENDOR_CONTROL_PRE
# define UEVENT_SET_SEL	\
   __TMS_UEVENT_SET_SEL
# define UEVENT_SET_ISOCH_DELAY	\
   __TMS_UEVENT_SET_ISOCH_DELAY
# define UEVENT_FUNCTION_SUSPEND	\
   __TMS_UEVENT_FUNCTION_SUSPEND
# define UEVENT_FUNCTION_RESUME	\
   __TMS_UEVENT_FUNCTION_RESUME
# define UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE	\
   __TMS_UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE
# define UEVENT_DEVICE_NOTIFICATION	\
   __TMS_UEVENT_DEVICE_NOTIFICATION
# define UEVENT_U1_SLEEP	\
   __TMS_UEVENT_U1_SLEEP
# define UEVENT_U2_SLEEP	\
   __TMS_UEVENT_U2_SLEEP
# define UEVENT_EXIT_U1_U2	\
   __TMS_UEVENT_EXIT_U1_U2
# define UEVENT_NAMES__INIT	\
   __TMS_UEVENT_NAMES__INIT
# define UEVENTNODE_HDR	\
   __TMS_UEVENTNODE_HDR
# define UEVENTRESET_INFO_SETUP_V1(				\
		pArg, ARG_RealEventCode, ARG_pEventInfo, ARG_fFinal)	\
	__TMS_UEVENTRESET_INFO_SETUP_V1(				\
		pArg, ARG_RealEventCode, ARG_pEventInfo, ARG_fFinal)
# define UEVENTPLATFORMEXTENSION_INFO_SETUP_V1(			\
		pArg,							\
		ARG_ulCode,						\
		ARG_pEventInfo,						\
		ARG_sizeEventInfo					\
		)	\
	__TMS_UEVENTPLATFORMEXTENSION_INFO_SETUP_V1(			\
		pArg,							\
		ARG_ulCode,						\
		ARG_pEventInfo,						\
		ARG_sizeEventInfo					\
		)
# define UEVENTL1SLEEP_INFO_SETUP_V1(			\
	ARG_pArg,						\
	ARG_fRemoteWakeUp,					\
	ARG_HostInitiateResumeDuration				\
	)	\
	__TMS_UEVENTL1SLEEP_INFO_SETUP_V1(			\
	ARG_pArg,						\
	ARG_fRemoteWakeUp,					\
	ARG_HostInitiateResumeDuration				\
	)
# define UEVENT_DETECT_ENDPOINT_ACTIVITY_INFO_SETUP_V1(	\
	ARG_pArg,						\
	ARG_fDetect						\
	)	\
	__TMS_UEVENT_DETECT_ENDPOINT_ACTIVITY_INFO_SETUP_V1(	\
	ARG_pArg,						\
	ARG_fDetect						\
	)
# define UEVENT_SET_SEL_INFO_SETUP_V1(			\
	ARG_pArg,						\
	ARG_U1SEL,						\
	ARG_U1PEL,						\
	ARG_U2SEL,						\
	ARG_U2PEL						\
	)	\
	__TMS_UEVENT_SET_SEL_INFO_SETUP_V1(			\
	ARG_pArg,						\
	ARG_U1SEL,						\
	ARG_U1PEL,						\
	ARG_U2SEL,						\
	ARG_U2PEL						\
	)
# define UEVENT_SET_ISOCH_DELAY_INFO_SETUP_V1(		\
	ARG_pArg,						\
	ARG_Delay						\
	)	\
	__TMS_UEVENT_SET_ISOCH_DELAY_INFO_SETUP_V1(		\
	ARG_pArg,						\
	ARG_Delay						\
	)
# define UEVENT_FUNCTION_SUSPEND_INFO_SETUP_V1(		\
	ARG_pArg,						\
	ARG_fRemoteWakeEnable					\
	)	\
	__TMS_UEVENT_FUNCTION_SUSPEND_INFO_SETUP_V1(		\
	ARG_pArg,						\
	ARG_fRemoteWakeEnable					\
	)
# define UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE_INFO_SETUP_V1(	\
	ARG_pArg,							\
	ARG_fFunctionRemoteWakeCapable					\
	)	\
	__TMS_UEVENT_FUNCTION_REMOTE_WAKE_CAPABLE_INFO_SETUP_V1(	\
	ARG_pArg,							\
	ARG_fFunctionRemoteWakeCapable					\
	)
# define UEVNET_DEVICE_NOTIFICATION_TYPE_FUNCTION_WAKE	\
   __TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_FUNCTION_WAKE
# define UEVNET_DEVICE_NOTIFICATION_TYPE_LTM	\
   __TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_LTM
# define UEVNET_DEVICE_NOTIFICATION_TYPE_BUS_INTERVAL_ADJUST	\
   __TMS_UEVNET_DEVICE_NOTIFICATION_TYPE_BUS_INTERVAL_ADJUST
# define UEVENT_DEVICE_NOTIFICATION_INFO_FUNCTION_WAKE_SETUP_V1(	\
	ARG_pArg,							\
	ARG_Interface							\
	)	\
	__TMS_UEVENT_DEVICE_NOTIFICATION_INFO_FUNCTION_WAKE_SETUP_V1(	\
	ARG_pArg,							\
	ARG_Interface							\
	)
# define UEVENT_DEVICE_NOTIFICATION_INFO_LTM_SETUP_V1(		\
	ARG_pArg,							\
	ARG_Belt							\
	)	\
	__TMS_UEVENT_DEVICE_NOTIFICATION_INFO_LTM_SETUP_V1(		\
	ARG_pArg,							\
	ARG_Belt							\
	)
# define UEVENT_DEVICE_NOTIFICATION_INFO_BUS_INTERVAL_ADJUST_SETUP_V1( \
	ARG_pArg,							\
	ARG_BusIntervalAdjust						\
	)	\
	__TMS_UEVENT_DEVICE_NOTIFICATION_INFO_BUS_INTERVAL_ADJUST_SETUP_V1( \
	ARG_pArg,							\
	ARG_BusIntervalAdjust						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of ueventnode.h ****/
#endif /* _UEVENTNODE_H_ */
