/* usbprotoinit.h	Mon Jun  1 2015 00:20:38 tmm */

/*

Module:  usbprotoinit.h

Function:
	The USB protocol initialization vector definitions.

Version:
	V3.15b	Mon Jun  1 2015 00:20:38 tmm	Edit level 8

Copyright notice:
	This file copyright (C) 2001-2002, 2006-2007, 2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	December 2001

Revision history:
   1.63g  Sat Dec  8 2001 22:05:16  tmm
	Module created.

   1.79a  6/15/2002  tmm
	Update the protocol vectors.

   1.97e  Wed Aug 16 2006 10:59:17  tmm
	1921: Add UsbPumpProtocolInitNode_TestInterface().

   1.97e  Mon Aug 21 2006 14:13:38  tmm
	Bug 1924: Make USBPUMP_PROTOCOL_INIT_NODE::pOptionalInfo
	CONST VOID * instead of VOID *.

	Bug 1921: add uProbeFlags, pAddInstance to
	USBPUMP_PROTOCOL_INIT_NODE, and create
	USBPUMP_PROTOCOL_INIT_NODE_INIT_V2() macro to allow those
	fields to be set.

   1.97e  Tue Aug 22 2006 12:46:24  chwon
	Regenerate uncloaked names

   1.97j  Thu Nov 01 2007 10:50:31  cccho
	4248: Change the definition of USBPUMP_PROTOCOL_CREATE_FN.

   3.11d  Tue Sep 10 2013 11:42:18  chwon
	17686: Fixed typo in documentation and fixed company name.

   3.15b  Mon Jun  1 2015 00:20:38  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _USBPROTOINIT_H_		/* prevent multiple includes */
#define _USBPROTOINIT_H_

#include "basehiltypes.h"
#include "usbpump_types.h"
#include "usbdesc.h"
#include "usbpumpioctl.h"
#include "usbpumpobject.h"

/****************************************************************************\
|
|	Forward types and function definitions
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_PROTOCOL_INIT_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_PROTOCOL_INIT_NODE_VECTOR);

/*

Type:	USBPUMP_PROTOCOL_INIT_NODE_SETUP_FN

Function:
	Abstract type for the "setup" function entry in a
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR.

Definition:
	typedef USBPUMP_IOCTL_RESULT
		USBPUMP_PROTOCOL_INIT_NODE_SETUP_FN(
			UDEVICE *pDevice,
			CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER **ppContext IN OUT
			);

	typedef USBPUMP_PROTOCOL_INIT_NODE_SETUP_FN
		*PUSBPUMP_PROTOCOL_INIT_NODE_SETUP_FN;

Description:
	Each USBPUMP_PROTOCOL_INIT_NODE_VECTOR instance may optionally
	supply a setup function pointer.  This function is called prior to
	running the initialization loop on the body of the protocol init
	vector.

	Examples of activities that this function might perform are:
	allocating a block of memory to be used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "setup" functions should be
	prototyped using this type, by writing:

		USBPUMP_PROTOCOL_INIT_NODE_SETUP_FN MyFunction;

	rather than by writing:

		USBPUMP_IOCTL_RESULT MyFunction(
			UDEVICE *pDevice,
			CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *ppContext
			);

Returns:
	The explicit result of this function should be
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or some other code for
	failure.

	The setup function is optionally passed, and may optionally return
	a pointer to a DataPump object.  (Using an object header allows
	the protocols to send IOCTLs to the object, if desired.)  ppContext
	points to a cell that is initially either NULL or a pointer to an
	object header.  If NULL, the setup function may change *pContext
	to point to an object header that will be passed to all the probe
	and creation functions, and to the Finish function (if provided).

Bugs:
	This is a method function of USBPUMP_PROTOCOL_INIT_NODE_VECTOR, and
	so ought to have pHdr and ppProtoInitContext as the first arguments.
	But for historical reasons, the UDEVICE is first.

*/

/*
|| The setup function is called so that the configurator can do some
|| "pre-setup", including prompting for other things to do.  It is optional.
|| The result is passed (unchanged) to all the probe and setup functions.
|| It is called prior to running the initialization operations on a given
|| UDEVICE.
*/
__TMS_FNTYPE_DEF(
USBPUMP_PROTOCOL_INIT_NODE_SETUP_FN,
__TMS_USBPUMP_IOCTL_RESULT,
	(
	__TMS_UDEVICE	*,
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE_VECTOR *,
	__TMS_USBPUMP_OBJECT_HEADER **
	));

/*

Type:	USBPUMP_PROTOCOL_INIT_NODE_FINISH_FN

Function:
	Abstract type for the "finish" function entry in a
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR.

Definition:
	typedef USBPUMP_IOCTL_RESULT
		USBPUMP_PROTOCOL_INIT_NODE_FINISH_FN(
			UDEVICE *pDevice,
			CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pProtoInitContext,
			UINT nProtos
			);

	typedef USBPUMP_PROTOCOL_INIT_NODE_FINISH_FN
		*PUSBPUMP_PROTOCOL_INIT_NODE_FINISH_FN;

Description:
	Each USBPUMP_PROTOCOL_INIT_NODE_VECTOR instance may optionally
	supply a "finish" function pointer.  This function is called after
	running the initialization loop on the body of the protocol init
	vector.

	pProtoInitContext is the value returned previously by the SETUP
	function.  If no SETUP function was provided, then pProtoInitContext
	will be NULL.

	Examples of activities that this function might perform are:
	freeing a block of memory used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "finish" functions should be
	prototyped using this type, by writing:

		USBPUMP_PROTOCOL_INIT_NODE_FINISH_FN MyFunction;

	rather than by writing:

		USBPUMP_IOCTL_RESULT MyFunction(
			UDEVICE *pDevice,
			CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pProtoInitContext,
			UINT nProtos
			);

Returns:
	If the finish function returns a USBPUMP_IOCTL_FAILED() code,
	then the result will be taken as the overall result of this
	initialization.  Otherwise the initialization result will be
	USBPUMP_IOCTL_RESULT_SUCCESS.

Bugs:
	This is a method function of pProtoInitContext+
	USBPUMP_PROTOCOL_INIT_NODE, and so ought to have pHdr and
	pProtoInitContext as the fist arguments.  But for historical
	reasons, the UDEVICE is first.

*/

/*
|| The finish function is called so that the application can do some
|| "post-setup", including prompting for other things to do.  It is optional.
*/
__TMS_FNTYPE_DEF(
USBPUMP_PROTOCOL_INIT_NODE_FINISH_FN,
__TMS_USBPUMP_IOCTL_RESULT,
	(
	__TMS_UDEVICE	*,
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE_VECTOR *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pProtoInitContext */,
	__TMS_UINT /* nProtos */
	));

/*

Type:	USBPUMP_PROTOCOL_PROBE_FN

Function:
	Abstract type for the "probe" function entry in a
	USBPUMP_PROTOCOL_INIT_NODE.

Definition:
	typedef BOOL USBPUMP_PROTOCOL_PROBE_FN(
			UDEVICE *pDevice,
			UINTERFACE *pInterface,
			CONST USBPUMP_PROTOCOL_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pProtoInitContext
			);

	typedef USBPUMP_PROTOCOL_PROBE_FN
		*PUSBPUMP_PROTOCOL_PROBE_FN;

Description:
	Each USBPUMP_PROTOCOL_INIT_NODE instance may optionally
	supply a "probe" function pointer.  This function may further
	examine the interface descriptor, and then either accept the
	UINTERFACE or reject it.

	pDevice is the pointer to the governing UDEVICE.

	pInterface points to the UINTERFACE under consideration.

	pIfcDescAltSet points to the interface descriptor that corresponds
	to the UINTERFACE.

	pNode points to the USBPUMP_PROTOCOL_INIT_NODE in question.

	pProtoInitContext is the value returned previously by the
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR's "setup" function.  If no
	SETUP function was provided, then pProtoInitContext
	will be NULL.

	Examples of activities that this function might perform are:
	checking class-specific descriptors such as WMC MDLM GUIDs.

	Functions which are to be used as "probe" functions should be
	prototyped using this type, by writing:

		USBPUMP_PROTOCOL_PROBE_FN MyProbeFunction;

	rather than by writing:

		BOOL MyProbeFunction(
			UDEVICE *pDevice,
			UINTERFACE *pInterface,
			CONST USBPUMP_PROTOCOL_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pProtoInitContext
			);

Returns:
	If the probe function returns FALSE, then UsbPump_CreateProtocols
	will not attempt to call the create function for pNode.  On the other
	hand, if the probe function returns TRUE, then
	UsbPump_CreateProtocols() will call the create function for pNode to
	attach the protocol to the UINTERFACE.

Bugs:
	This is a method function of pProtoInitContext+
	USBPUMP_PROTOCOL_INIT_NODE, and so ought to have pNode and
	pProtoInitContext as the first arguments.  But for historical
	reasons, the UDEVICE and UINTERFACE are first.

*/


/*
|| The protocol probe function, if present, is called prior to
|| creating the protocol instance, so that it can decide whether or not to
|| init.
*/
__TMS_FNTYPE_DEF(
USBPUMP_PROTOCOL_PROBE_FN,
__TMS_BOOL, (
	__TMS_UDEVICE *			/* pDevice */,
	__TMS_UINTERFACE *		/* pUinterface */,
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pProtoInitContext */
	));

/*

Type:	USBPUMP_PROTOCOL_CREATE_FN

Function:
	Abstract type for the "create" function entry in a
	USBPUMP_PROTOCOL_INIT_NODE.

Definition:
	typedef USBPUMP_OBJECT_HEADER *USBPUMP_PROTOCOL_CREATE_FN(
			UDEVICE *pDevice,
			UINTERFACE *pInterface,
			CONST USBPUMP_PROTOCOL_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pProtoInitContext
			);

	typedef USBPUMP_PROTOCOL_CREATE_FN
		*PUSBPUMP_PROTOCOL_CREATE_FN;

Description:
	Each USBPUMP_PROTOCOL_INIT_NODE instance must supply a "create"
	function pointer.  This function is called for each matching
	UINTERFACE, and is expected to attach a protocol to the underlying
	UINTERFACE or UINTERFACESET.

	pDevice is the pointer to the governing UDEVICE.

	pInterface points to the UINTERFACE under consideration.

	pNode points to the USBPUMP_PROTOCOL_INIT_NODE in question.

	pProtoInitContext is the value returned previously by the
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR's "setup" function.  If no
	SETUP function was provided, then pProtoInitContext
	will be NULL.

	Functions which are to be used as "create" functions should be
	prototyped using this type, by writing:

		USBPUMP_PROTOCOL_CREATE_FN MyCreateFunction;

	rather than by writing:

		USBPUMP_OBJECT_HEADER *  MyCreateFunction(
			UDEVICE *pDevice,
			UINTERFACE *pInterface,
			CONST USBPUMP_PROTOCOL_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pProtoInitContext
			);

Returns:
	If the create function returns NULL, then UsbPump_CreateProtocols()
	assumes that the creation failed for some reason, and goes on.
	On the other hand, if the create function returns USBPUMP_OBJECT_HEADER *,
	then UsbPump_CreateProtocols() will assume that the protocol was
	successfully attached, and will increment its count of attached
	protocol instances.

Bugs:
	This is a method function of pProtoInitContext+
	USBPUMP_PROTOCOL_INIT_NODE, and so ought to have pNode and
	pProtoInitContext as the first arguments.  But for historical
	reasons, the UDEVICE and UINTERFACE are first.

Notes:
	This implies that all protocols must be USBPUMP_OBJECTs.

*/


__TMS_FNTYPE_DEF(
USBPUMP_PROTOCOL_CREATE_FN,
__TMS_USBPUMP_OBJECT_HEADER *, (
	__TMS_UDEVICE *			/* initialized device */,
	__TMS_UINTERFACE *		/* pUinterface */,
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pProtoInitContext */
	));

/*

Type:	USBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN

Function:
	Abstract type for the "qualify add interface" function entry in a
	USBPUMP_PROTOCOL_INIT_NODE.

Definition:
	typedef BOOL USBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN(
			CONST USBPUMP_PROTOCOL_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pProtoInitContext,
			UDATAPLANE *pControllingPlane,
			UINTERFACE *pInterfaceToAdd,
			);

	typedef USBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN
		*PUSBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN;

Description:
	Each USBPUMP_PROTOCOL_INIT_NODE instance may supply an "qualify add
	instance" function pointer.  If this function is supplied, the protocol
	framework calls this function as part of the "add other interfaces"
	functionality that collects interfaces across configurations and
	speeds into dataplanes.

	pNode is the pointer to the governing USBPUMP_PROTOCOL_INIT_NODE.

	pProtoInitContext is the value returned previously by the
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR's "setup" function.  If no
	SETUP function was provided, then pProtoInitContext
	will be NULL.

	pControllingPlane is the pointer to the governing UDATAPLANE.

	pInterfaceToAdd points to the UINTERFACE that is to be added to the
	protocol instance.

	Functions which are to be used as "qualify add interface" functions should be
	prototyped using this type, by writing:

		USBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN MyQualifyAddInstanceFunction;

	rather than by writing:

		BOOL MyQualifyAddInstanceFunction(
			CONST USBPUMP_PROTOCOL_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pProtoInitContext
			UDATAPLANE *pDataplane,
			UINTERFACE *pInterface,
			);

Returns:
	If the "qualify add instance" function returns FALSE, then UsbPump_CreateProtocols()
	assumes that the operation failed for some reason, and goes on.
	On the other hand, if the add-instance function returns TRUE, then
	UsbPump_CreateProtocols() will assume that the protocol was
	successfully attached to the UINTERFACE.

Notes:
	This function should *not* be supplied by the protocol logic.
	Protocol-specific qualification should be done in the AddInstance
	function.  This function should only be used for client-specific
	modification of behavior.

*/


__TMS_FNTYPE_DEF(
USBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN,
__TMS_BOOL, (
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE *,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pProtoInitContext */,
	__TMS_UDATAPLANE *		/* pInstancePlane */,
	__TMS_UINTERFACE *		/* pInterfaceToAdd */
	));

/*

Type:	USBPUMP_PROTOCOL_ADD_INTERFACE_FN

Function:
	Abstract type for the "add interface" function entry in a
	USBPUMP_PROTOCOL_INIT_NODE.

Definition:
	typedef BOOL USBPUMP_PROTOCOL_ADD_INTERFACE_FN(
			CONST USBPUMP_PROTOCOL_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pProtoInitContext,
			UDATAPLANE *pControllingPlane,
			UINTERFACE *pInterfaceToAdd,
			);

	typedef USBPUMP_PROTOCOL_ADD_INTERFACE_FN
		*PUSBPUMP_PROTOCOL_ADD_INTERFACE_FN;

Description:
	Each USBPUMP_PROTOCOL_INIT_NODE instance may supply an "add
	instance" function pointer.  If this function is supplied, the protocol
	framework assumes that the protocol is designed to collect multiple
	instances across configuration and speed.  The add-instance function is
	called for each matching UINTERFACE, and is expected to do the necessary
	work to attach the UDATAPLANE to the specified UINTERFACE.

	pNode is the pointer to the governing USBPUMP_PROTOCOL_INIT_NODE.

	pProtoInitContext is the value returned previously by the
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR's "setup" function.  If no
	SETUP function was provided, then pProtoInitContext
	will be NULL.

	pControllingPlane is the pointer to the governing UDATAPLANE.

	pInterfaceToAdd points to the UINTERFACE that is to be added to the
	protocol instance.

	Functions which are to be used as "add interface" functions should be
	prototyped using this type, by writing:

		USBPUMP_PROTOCOL_ADD_INTERFACE_FN MyAddInstanceFunction;

	rather than by writing:

		BOOL MyAddInstanceFunction(
			CONST USBPUMP_PROTOCOL_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pProtoInitContext
			UDATAPLANE *pDataplane,
			UINTERFACE *pInterface,
			);

Returns:
	If the "add instance" function returns FALSE, then UsbPump_CreateProtocols()
	assumes that the operation failed for some reason, and goes on.
	On the other hand, if the add-instance function returns TRUE, then
	UsbPump_CreateProtocols() will assume that the protocol was
	successfully attached to the UINTERFACE.

*/


__TMS_FNTYPE_DEF(
USBPUMP_PROTOCOL_ADD_INTERFACE_FN,
__TMS_BOOL, (
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE *,
	__TMS_USBPUMP_OBJECT_HEADER *	/* pProtoInitContext */,
	__TMS_UDATAPLANE *		/* pInstancePlane */,
	__TMS_UINTERFACE *		/* pInterfaceToAdd */
	));

/*

Type:	USBPUMP_PROTOCOL_INIT_NODE

Index:	Constant:	USBPUMP_PROTOCOL_INIT_FLAG_MATCH_IFCNUM
	Constant:	USBPUMP_PROTOCOL_INIT_FLAG_IGNORE_STRINGS
	Constant:	USBPUMP_PROTOCOL_INIT_FLAG_SEPARATE_ALTSETS
	Constant:	USBPUMP_PROTOCOL_INIT_FLAG_NO_BACK_MATCH
	Type:		USBPUMP_PROTOCOL_INIT_NODE_FLAGS

Function:
	C structure, representing a single protocol to be probed.

Contents:
	The structure has the following contents:

	INT16 sDeviceClass,
	      sDeviceSubClass,
	      sDeviceProtocol;
		Specify the required device class, subclass and protocol.
		-1 will match any value in the device descriptor.

	INT8 sSpeed;
		Specifies the required device speed.  -1 means will match
		any speed.

	INT16 sInterfaceClass,
	      sInterfaceSubClass,
	      sInterfaceProtocol;
		Specify the required interface class, subclass and protocol.
		-1 will match any value in the interface descriptor.

	INT16 sConfigurationValue,
	      sInterfaceNumber,
	      sAlternateSetting;
		Specify the required configuration value, interface number,
		and alternate setting.  -1 will match any values.

	USBPUMP_PROTOCOL_INIT_NODE_FLAGS uProbeFlags;
		Flags that control the probing of multiple instances.  If
		zero, then the default behavior applies.  By default,
		protocols that establish dataplanes are collected across
		configurations and speeds; protocols that use UPIPEs and do
		not establish dataplanes are not collected.  All these
		policies can be overridden using non-zero values of
		uProbeFlags.

		The following probe flags are defined:

		USBPUMP_PROTOCOL_INIT_FLAG_MATCH_IFCNUM causes additional
		interfaces to be permitted to match only if they have
		identical interface numbers.  Otherwise, interface numbers
		are not considered in the match.

		USBPUMP_PROTOCOL_INIT_FLAG_IGNORE_STRINGS causes additional
		interfaces to be permitted to match even if they have different
		string indices.  Otherwise they are matched only if the
		string indices match.

		USBPUMP_PROTOCOL_INIT_FLAG_SEPARATE_ALTSETS causes alternate
		settings to be considered separately.  If clear, all UINTERFACE
		instances are added if the first one is added.

		USBPUMP_PROTOCOL_INIT_FLAG_NO_BACK_MATCH causes all information
		from the base UINTERFACE to be ignored (provided that the probe
		function matches).  If this flag is set, then
		USBPUMP_PROTOCOL_INIT_FLAG_MATCH_IFCNUM is ignored and operation
		is as if USBPUMP_PROTOCOL_INIT_FLAG_IGNORE_STRINGS was given.

		USBPUMP_PROTOCOL_INIT_FLAG_AUTO_ADD causes matching UINTERFACEs
		to be added, even if you don't provide a non-NULL
		pAddInterfaceFunction in the USBPUMP_PROTOCOL_INIT_NODE.  If
		this flag is not set, and pAddInterfaceFunction is NULL, then
		UsbPump_CreateProtocols() will not attempt to collect interfaces
		from other configurations and speeds.

	USBPUMP_PROTOCOL_PROBE_FN *pProbeFunction OPTIONAL;
		If non-NULL, the function to be called to further qualify
		the matching of an interface against a possible protocol.

	USBPUMP_PROTOCOL_CREATE_FN *pCreateFunction;
		The (mandatory) function to be called to create a protocol
		instance.

	USBPUMP_PROTOCOL_ADD_INTERFACE_FN *pAddInterfaceFunction OPTIONAL;
		If non-NULL, the function to be called to perform the
		addition of an additional instance against a possible
		protocol.

	USBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN *pQualifyAddInterfaceFunction OPTIONAL;
		If non-NULL, the function to be called to qualify the
		addition of an additional instance against a possible
		protocol.  This is provided so that the probe function doesn't
		have to be sensitive to client-specific situations.

	CONST VOID *pOptionalInfo OPTIONAL;
		Optional information for the use of the protocol functions.

Description:
	Each USBPUMP_PROTOCOL_INIT_NODE represents one potential binding for
	a protocol, in terms of the bDeviceClass, bDeviceSubClass,
	bDeviceProtocol, bInterfaceClass, bInterfaceSubClass,
	bInterfaceProtocol, bConfigurationValue, bInterfaceNumber and
	bAlternateSetting applicable to the corresponding interface
	descriptor.

	For completeness, sSpeed is included in this structure; it must
	always be coded -1.

	Any of the above values may be -1; in that case, the
	corresponding value in the descriptors may have any value.  All of
	these are constrained to lie in the range [-1..255]; if out
	of range, this USBPUMP_PROTOCOL_INIT_NODE can never be matched by
	UsbPump_CreateProtocols().

	pProbe allows the desiger to add an algorithmic filter;
	pOptionalInfo allows the designer to pass arbitrary information from
	the USBPUMP_PROTOCOL_INIT_NODE to the creationt function.

	pCreate is a pointer to the function that will create the protocol
	instance and attach it to the underlying UINTERFACE or UINTERFACESET.
	It makes no sense to omit this, but UsbPump_CreateProtocols will
	not dereference this pointer if it is in fact coded as NULL.

*/

__TMS_TYPE_DEF_ARG(USBPUMP_PROTOCOL_INIT_NODE_FLAGS, UINT32);

struct __TMS_STRUCTNAME(USBPUMP_PROTOCOL_INIT_NODE)
	{
	/*
	|| To simplify matters, we provide for matching fields for
	|| bInterfaceClass, bInterfaceSubClass, bInterfaceProtocol.
	|| All are signed; if -1, they are wild cards.  Since they have
	|| range -1..255, they need to be INT16.
	*/
	__TMS_INT16				sDeviceClass;
	__TMS_INT16				sDeviceSubClass;
	__TMS_INT16				sDeviceProtocol;
	__TMS_INT16				sInterfaceClass;
	__TMS_INT16				sInterfaceSubClass;
	__TMS_INT16				sInterfaceProtocol;
	__TMS_INT16				sConfigurationValue;
	__TMS_INT16				sInterfaceNumber;
	__TMS_INT16				sAlternateSetting;

	/* speed is -1 for any, (future) other for specific speeds */
	__TMS_INT8				sSpeed;

	/* a field for probe-control flags */
	__TMS_USBPUMP_PROTOCOL_INIT_NODE_FLAGS	uProbeFlags;

	/*
	|| The optional probe function.
	*/
	__TMS_USBPUMP_PROTOCOL_PROBE_FN		*pProbeFunction;

	/*
	|| The protocol creating function.
	*/
	__TMS_USBPUMP_PROTOCOL_CREATE_FN	*pCreateFunction;

	/*
	|| The optional add-instance qualifier function
	*/
	__TMS_USBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN *pQualifyAddInterfaceFunction;

	/*
	|| the optional instance adding function.
	*/
	__TMS_USBPUMP_PROTOCOL_ADD_INTERFACE_FN	*pAddInterfaceFunction;

	/*
	|| A slot for extra information, for use by the Create or Probe
	|| functions in doing their work.
	*/
	__TMS_CONST __TMS_VOID			*pOptionalInfo;
	};

/*
|| The values for uProbeFlags
*/
#define	__TMS_USBPUMP_PROTOCOL_INIT_FLAG_MATCH_IFCNUM		(1u << 0)
#define	__TMS_USBPUMP_PROTOCOL_INIT_FLAG_IGNORE_STRINGS		(1u << 1)
#define	__TMS_USBPUMP_PROTOCOL_INIT_FLAG_SEPARATE_ALTSETS	(1u << 2)
#define	__TMS_USBPUMP_PROTOCOL_INIT_FLAG_NO_BACK_MATCH		(1u << 3)
#define	__TMS_USBPUMP_PROTOCOL_INIT_FLAG_AUTO_ADD		(1u << 4)

/*

Name:  USBPUMP_PROTOCOL_INIT_NODE_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_PROTOCOL_INIT_NODE.

Definition:
	USBPUMP_PROTOCOL_INIT_NODE_INIT_V1(
		INT16	sDeviceClass,
		INT16	sDeviceSubClass,
		INT16	sDeviceProtocol,
		INT16	sInterfaceClass,
		INT16	sInterfaceSubClass,
		INT16	sInterfaceProtocol,
		INT16	sConfigurationValue,
		INT16	sInterfaceNumber,
		INT16	sAlternateSetting,
		INT8	sSpeed,
		USBPUMP_PROTOCOL_PROBE_FN *pProbeFn,
		USBPUMP_PROTOCOL_CREATE_FN *pCreateFn,
		VOID	*pOptionalInfo
		)

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_PROTOCOL_INIT_NODEs.  It expands to whatever is the
	current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

Notes:
	This macro is obsolete.

See:
	USBPUMP_PROTOCOL_INIT_NODE_INIT_V2().

*/


#define __TMS_USBPUMP_PROTOCOL_INIT_NODE_INIT_V1(			\
		sDeviceClass,						\
		sDeviceSubClass,					\
		sDeviceProtocol,					\
		sInterfaceClass,					\
		sInterfaceSubClass,					\
		sInterfaceProtocol,					\
		sConfigurationValue,					\
		sInterfaceNumber,					\
		sAlternateSetting,					\
		sSpeed,							\
		pProbeFunction,						\
		pCreateFunction,					\
		pOptionalInfo						\
		)							\
	__TMS_USBPUMP_PROTOCOL_INIT_NODE_INIT_V2(			\
		(sDeviceClass),						\
		(sDeviceSubClass),					\
		(sDeviceProtocol),					\
		(sInterfaceClass),					\
		(sInterfaceSubClass),					\
		(sInterfaceProtocol),					\
		(sConfigurationValue),					\
		(sInterfaceNumber),					\
		(sAlternateSetting),					\
		(sSpeed),						\
		/* uProbeFlags */ 0,					\
		(pProbeFunction),					\
		(pCreateFunction),					\
		/* pQualifyAddInterface */ __TMS_NULL,			\
		/* pAddInterface */ __TMS_NULL,				\
		(pOptionalInfo)						\
		)

/*

Name:  USBPUMP_PROTOCOL_INIT_NODE_INIT_V2()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_PROTOCOL_INIT_NODE.

Definition:
	USBPUMP_PROTOCOL_INIT_NODE_INIT_V2(
		INT16	sDeviceClass,
		INT16	sDeviceSubClass,
		INT16	sDeviceProtocol,
		INT16	sInterfaceClass,
		INT16	sInterfaceSubClass,
		INT16	sInterfaceProtocol,
		INT16	sConfigurationValue,
		INT16	sInterfaceNumber,
		INT16	sAlternateSetting,
		INT8	sSpeed,
		UINT32	uProbeFlags,
		USBPUMP_PROTOCOL_PROBE_FN *pProbeFn,
		USBPUMP_PROTOCOL_CREATE_FN *pCreateFn,
		USBPUMP_PROTOCOL_QUALIFY_ADD_INTERFACE_FN *pQualifyAddInterfaceFn,
		USBPUMP_PROTOCOL_ADD_INTERFACE_FN *pAddInterfaceFn,
		CONST VOID	*pOptionalInfo
		)

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_PROTOCOL_INIT_NODEs.  It expands to whatever is the
	current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V2() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

/* do NOT add arguments to this macro or add values to the expansion */
/* instead, define a V3 macro and redefine this in terms of the V3 macro */
#define __TMS_USBPUMP_PROTOCOL_INIT_NODE_INIT_V2(			\
		sDeviceClass,						\
		sDeviceSubClass,					\
		sDeviceProtocol,					\
		sInterfaceClass,					\
		sInterfaceSubClass,					\
		sInterfaceProtocol,					\
		sConfigurationValue,					\
		sInterfaceNumber,					\
		sAlternateSetting,					\
		sSpeed,							\
		uProbeFlags,						\
		pProbeFunction,						\
		pCreateFunction,					\
		pQualifyAddInterface,					\
		pAddInterface,						\
		pOptionalInfo						\
		)							\
	{								\
	(sDeviceClass),							\
	(sDeviceSubClass),						\
	(sDeviceProtocol),						\
	(sInterfaceClass),						\
	(sInterfaceSubClass),						\
	(sInterfaceProtocol),						\
	(sConfigurationValue),						\
	(sInterfaceNumber),						\
	(sAlternateSetting),						\
	(sSpeed),							\
	(uProbeFlags),							\
	(pProbeFunction),						\
	(pCreateFunction),						\
	(pQualifyAddInterface),						\
	(pAddInterface),						\
	(pOptionalInfo)							\
	}

/****************************************************************************\
|
|	At init time we need to have a list of USB protocols.  This list
|	effectively configures this instance of the DataPump by providing
|	the list of protocols.  The initialization scheme in the V1 DataPump
|	is dynamic; the config table is logically per UDEVICE.
|
\****************************************************************************/

/*

Type:	USBPUMP_PROTOCOL_INIT_NODE_VECTOR

Function:
	Provides a wrapper for a vector of USBPUMP_PROTOCOL_INIT_NODEs.

Definition:
	The structure has the following contents:

		USBPUMP_PROTOCOL_INIT_NODE_SETUP_FN	*pSetup OPTIONAL;
		USBPUMP_PROTOCOL_INIT_NODE_FINISH_FN	*pFinish OPTIONAL;
		CONST USBPUMP_PROTOCOL_INIT_NODE	*pVector;
		SIZE_T					nVector;

Description:
	pVector points to the base, and nVector states the number of elements,
	of the vector of USBPUMP_PROTOCOL_INIT_NODEs which list the protocols
	which are to be probed at startup.

	pSetup and pFinish optionally allow the designer to proved a wrapper
	for this initialization.

See also:
	UsbPump_CreateProtocols().

*/


/* the vector header */
struct __TMS_STRUCTNAME(USBPUMP_PROTOCOL_INIT_NODE_VECTOR)
	{
	/* an optional function that is called before we start */
	__TMS_USBPUMP_PROTOCOL_INIT_NODE_SETUP_FN		*pSetup;

	/* an optional function that is called when we are done */
	__TMS_USBPUMP_PROTOCOL_INIT_NODE_FINISH_FN		*pFinish;

	/* the vector and its length */
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE		*pVector;
	__TMS_SIZE_T						nVector;
	};

/*

Name:  USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR.

Definition:
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(
		USBPUMP_PROTOCOL_INIT_NODE	VectorName[],
		USBPUMP_PROTOCOL_INIT_SETUP_FN	*pSetup,
		USBPUMP_PROTOCOL_INIT_FINISH_FN	*pFinish
		);

Description:
	This macro is used when initializing a
	USBPUMP_PROTOCOL_INIT_NODE_VECTOR at compile time.  It expands to
	whatever is the current proper value for statically initializing
	such an element, inserting proper defaults for any entries that
	may have been added after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(		\
	VectorName,							\
	pSetup,								\
	pFinish)							\
	{								\
	(pSetup),							\
	(pFinish),							\
	(VectorName),							\
	sizeof(VectorName) / sizeof(VectorName[0])			\
	}

/****************************************************************************\
|
|	Functions...
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_IOCTL_RESULT
UsbPump_CreateProtocols(
	__TMS_UDEVICE *pDevice,
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE_VECTOR *pProtoVec,
	__TMS_USBPUMP_OBJECT_HEADER *pHeader /* OPTIONAL */,
	__TMS_UINT *puNumProtos /* OUT */
	);

__TMS_BOOL
UsbPumpProtocolInitNode_TestInterface(
	__TMS_CONST __TMS_USBPUMP_PROTOCOL_INIT_NODE * /*pInitNode*/,
	__TMS_USBPUMP_OBJECT_HEADER * /*pProtoInitContext*/ /* OPTIONAL */,
	__TMS_UDEVICE * /*pDevice*/,
	__TMS_UINTERFACE * /*pInterface*/
	);

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_PROTOCOL_INIT_FLAG_MATCH_IFCNUM	\
   __TMS_USBPUMP_PROTOCOL_INIT_FLAG_MATCH_IFCNUM
# define USBPUMP_PROTOCOL_INIT_FLAG_IGNORE_STRINGS	\
   __TMS_USBPUMP_PROTOCOL_INIT_FLAG_IGNORE_STRINGS
# define USBPUMP_PROTOCOL_INIT_FLAG_SEPARATE_ALTSETS	\
   __TMS_USBPUMP_PROTOCOL_INIT_FLAG_SEPARATE_ALTSETS
# define USBPUMP_PROTOCOL_INIT_FLAG_NO_BACK_MATCH	\
   __TMS_USBPUMP_PROTOCOL_INIT_FLAG_NO_BACK_MATCH
# define USBPUMP_PROTOCOL_INIT_FLAG_AUTO_ADD	\
   __TMS_USBPUMP_PROTOCOL_INIT_FLAG_AUTO_ADD
# define USBPUMP_PROTOCOL_INIT_NODE_INIT_V1(			\
		sDeviceClass,						\
		sDeviceSubClass,					\
		sDeviceProtocol,					\
		sInterfaceClass,					\
		sInterfaceSubClass,					\
		sInterfaceProtocol,					\
		sConfigurationValue,					\
		sInterfaceNumber,					\
		sAlternateSetting,					\
		sSpeed,							\
		pProbeFunction,						\
		pCreateFunction,					\
		pOptionalInfo						\
		)	\
	__TMS_USBPUMP_PROTOCOL_INIT_NODE_INIT_V1(			\
		sDeviceClass,						\
		sDeviceSubClass,					\
		sDeviceProtocol,					\
		sInterfaceClass,					\
		sInterfaceSubClass,					\
		sInterfaceProtocol,					\
		sConfigurationValue,					\
		sInterfaceNumber,					\
		sAlternateSetting,					\
		sSpeed,							\
		pProbeFunction,						\
		pCreateFunction,					\
		pOptionalInfo						\
		)
# define USBPUMP_PROTOCOL_INIT_NODE_INIT_V2(			\
		sDeviceClass,						\
		sDeviceSubClass,					\
		sDeviceProtocol,					\
		sInterfaceClass,					\
		sInterfaceSubClass,					\
		sInterfaceProtocol,					\
		sConfigurationValue,					\
		sInterfaceNumber,					\
		sAlternateSetting,					\
		sSpeed,							\
		uProbeFlags,						\
		pProbeFunction,						\
		pCreateFunction,					\
		pQualifyAddInterface,					\
		pAddInterface,						\
		pOptionalInfo						\
		)	\
	__TMS_USBPUMP_PROTOCOL_INIT_NODE_INIT_V2(			\
		sDeviceClass,						\
		sDeviceSubClass,					\
		sDeviceProtocol,					\
		sInterfaceClass,					\
		sInterfaceSubClass,					\
		sInterfaceProtocol,					\
		sConfigurationValue,					\
		sInterfaceNumber,					\
		sAlternateSetting,					\
		sSpeed,							\
		uProbeFlags,						\
		pProbeFunction,						\
		pCreateFunction,					\
		pQualifyAddInterface,					\
		pAddInterface,						\
		pOptionalInfo						\
		)
# define USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(		\
	VectorName,							\
	pSetup,								\
	pFinish)	\
	__TMS_USBPUMP_PROTOCOL_INIT_NODE_VECTOR_INIT_V1(		\
	VectorName,							\
	pSetup,								\
	pFinish)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbprotoinit.h ****/
#endif /* _USBPROTOINIT_H_ */
