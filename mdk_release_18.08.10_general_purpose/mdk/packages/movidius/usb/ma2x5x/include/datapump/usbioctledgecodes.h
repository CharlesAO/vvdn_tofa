/* usbioctledgecodes.h	Tue Jul 31 2018 13:46:03 chwon */

/*

Module:  usbioctledgecodes.h

Function:
	IOCTL codes for talking to OS-specific drivers, and to be used
	by OS-specific drivers to talk to us.

Version:
	V3.21g	Tue Jul 31 2018 13:46:03 chwon	Edit level 9

Copyright notice:
	This file copyright (C) 2002-2007, 2012-2013, 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Wed Jul 10 2002 08:22:48  tmm
	Module created.

   1.79b  Aug 9 2002  tmm
	Use centralized class codes.

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

   1.97j  Fri Dec 14 2007 09:30:04  chwon
	3595: add fRemoteWakeupEnable parameter of USBPUMP_IOCTL_EDGE_BUS_EVENT.
	Generated uncloaked name using uncloak-def.sh.

   3.11b  Fri Dec 14 2012 21:17:17  ldwang
	16256: Implement USBPUMP_IOCTL_EDGE_MODE_ACTIVE.

   3.11b  Mon Jan 07 2013 11:46:20  chwon
	16402: Added USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO and
	USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION ioctl.

   3.11b  Thu Jan 17 2013 12:36:32  chwon
	16256: Removed USBPUMP_IOCTL_EDGE_MODE_ACTIVE because it was misnamed.

   3.21g  Tue Jul 31 2018 13:46:04  chwon
	20862: Add USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY ioctl.

*/

#ifndef _USBIOCTLEDGECODES_H_		/* prevent multiple includes */
#define _USBIOCTLEDGECODES_H_

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

/*
|| edge functions are used for communicating with local OS client drivers.
|| These are issued as upcalls
*/
#define __TMS_USBPUMP_IOCTL_EDGE(t, i, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_CORE_EDGE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_, NAME, _ARG)	\
		)

/* activate the edge */
#define	__TMS_USBPUMP_IOCTL_EDGE_ACTIVATE	\
	__TMS_USBPUMP_IOCTL_EDGE(RW, 0, ACTIVATE)

/* deactivate the edge */
#define	__TMS_USBPUMP_IOCTL_EDGE_DEACTIVATE	\
	__TMS_USBPUMP_IOCTL_EDGE(W, 1, DEACTIVATE)

/* report a bus event */
#define	__TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT	\
	__TMS_USBPUMP_IOCTL_EDGE(W, 2, BUS_EVENT)

/* get Microsoft OS string descriptor information. */
#define	__TMS_USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO	\
	__TMS_USBPUMP_IOCTL_EDGE(RW, 4, GET_MS_OS_DESC_INFO)

/* get function section of the extened compat ID OS descritor. */
#define	__TMS_USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION	\
	__TMS_USBPUMP_IOCTL_EDGE(RW, 5, GET_FUNCTION_SECTION)

/* get function section of the extened compat ID OS descritor. */
#define	__TMS_USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY	\
	__TMS_USBPUMP_IOCTL_EDGE(RW, 6, GET_EXTENDED_PROPERTY)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_ACTIVATE

Index:	Type:	USBPUMP_IOCTL_EDGE_ACTIVATE_ARG
	Name:	USBPUMP_IOCTL_EDGE_ACTIVATE_ARG_SETUP_V1

Function:
	Outcall from conforming protocols to registered clients; indicates
	that the edge has been brought up by a bus event.

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_ACTIVATE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_ACTIVATE_ARG structure.

	USBPUMP_IOCTL_EDGE_ACTIVATE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	BOOL fReject;
		OUT:  if set TRUE, then the edge driver would like the
		USB core to reject the request, if possible.  Note, though,
		that due to hardware constraints it may not be possible to
		fail the request.

Description:
	This IOCTL is sent from a leaf UPROTO/UFUNCTION object to the OS-
	specific driver that has opened/connected to the leaf object.  It
	is sent whenever the host does something that brings up the logical
	function.  Note that this may be sent when there are no data-channels
	ready yet.  This merely means that the control interface of the
	function has been configured and is ready to transfer data.

Returns:
	fReject is an advisory indication, which may be used to flag to 
	the protocol that the upper edge cannot actually operate the
	data streams at this time.  Because of hardware or protocol
	limitations, this might or might not be honored by the lower layers.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_ACTIVATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_ACTIVATE_ARG *pArg,
		VOID *pClientContext,
		USBPUMP_OBJECT_HEADER *pObject
		);

	This macro will correctly initialize an ..ACTIVATE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_ACTIVATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_ACTIVATE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	__TMS_BOOL	fReject;	/* OUT:  set TRUE if the edge
					||  can be brought up now, FALSE
					||  otherwise.  If possible, will
					||  be used to fail the activate
					||  request on the bus, but this
					||  is not always possible.
					*/
	};

/* the setup for the ACTIVATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_ACTIVATE_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_ACTIVATE_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->fReject = __TMS_FALSE;					\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_DEACTIVATE

Index:	Type:	USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG
	Name:	USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG_SETUP_V1

Function:
	Notify OS-specfic drivers that an edge has been brought down
	by the bus.

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG
			structure.
	pOutParam	NULL

	USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: the object that is being deactivated
	
	VOID *pClientContext;
		IN: the context pointer supplied by the client when
		opening/connecting to the UPROTO.

Description:
	The UPROTO issues this call whenever a (protocol-specific) event
	occurs that deactivates the function.

	Unlike the ACTIVATE call, the os-driver has no way to attempt to
	reject this call.  The USB host might have issued a reset -- there's
	no way to prevent, in general, deactivation.

Returns:
	No specific result.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		VOID *pClientContext
		);

	This macro will correctly initialize a ..DEACTIVATE_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	};

/* the setup for the DEACTIVATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pCtx);				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_BUS_EVENT

Index:	Type:	USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG
	Name:	USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V1
	Name:	USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V2

Function:
	Informs an OS-specific driver that a significant USB bus event
	has occurred.

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG
			structure.
	pOutParam	NULL

	USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG has the following components:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: UPROTO object at the lower level.

	VOID *pClientContext;
		IN: Context handle provided when OS-specific driver openend/
		connected to the underlying UPROTO.

	UEVENT EventCode;
		The type of event that occurred.  This will be one of
		UEVENT_SUSPEND, UEVENT_RESUME, UEVENT_ATTACH, UEVENT_DETACH,
		or UEVENT_RESET.  [UEVENT_RESET is actually redundant; it will
		also cause a deactivate event; however this hook may be 
		useful for apps that wish to model the USB state.]

	VOID *pEventSpecificInfo;
		The event-specific information accompanying the UEVENT.

	BOOL fRemoteWakeupEnable;
		IN: set TRUE if remote-wakeup is enabled.

Description:
	Whenever a significant bus event occurs, the governing UPROTO will
	arrange for this outcall to be made to the OS-specific driver.
	Any events that actually change the state of the UPROTO will also
	cause the appropriate functional outcalls to be performed; SUSPEND
	and RESUME don't actually change the state of the UPROTO (according
	to the USB core spec).  

Returns:
	No explicit result.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V2(
		USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		VOID *pClientContext,
		UEVENT EventCode,
		VOID *pEventSpecificInfo,
		BOOL fRemoteWakeupEnable
		);

	This macro will correctly initialize a ..BUS_EVENT_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	__TMS_UEVENT	EventCode;	/* IN: the event code */
	__TMS_VOID	*pEventSpecificInfo; /* IN: the event info */
	__TMS_BOOL	fRemoteWakeupEnable; /* IN: remote wakeup enable */
	};

/* the setup for the BUS_EVENT_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_EventCode,							\
	ARG_pEventSpecificInfo,						\
	ARG_fRemoteWakeupEnable						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->EventCode = (ARG_EventCode);				\
	__pArg->pEventSpecificInfo = (ARG_pEventSpecificInfo);		\
	__pArg->fRemoteWakeupEnable = (ARG_fRemoteWakeupEnable);	\
   	} while (0)

#define __TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_EventCode,							\
	ARG_pEventSpecificInfo						\
	)								\
	__TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_EventCode,							\
	ARG_pEventSpecificInfo,						\
	/* V2: fRemoteWakeupEnable */ __TMS_FALSE			\
	)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO

Index:	Type:	USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG
	Name:	USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG_SETUP_V1

Function:
	Outcall from DataPump core to the protocols; to get the Microsoft OS
	string descriptor information.

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG
			structure.

	pOutParam	points to a USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG
			structure.

	USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG has the following elements:

	UCONFIG *pConfig;
		IN: pointer of UCONFIG.  This is current active configuration.
		The protocol instance should check this UCONFIG structure to
		figure out that protocol is part of active configuration.
		If the protocol object is part of current active configuration,
		it should return function section of the extened compat ID
		feature descriptor.

	BOOL fSupportOsDesc;
		OUT: TRUE if protocol object supports Microsoft OS string
		descriptor feature.

Description:
	This IOCTL is sent from a DataPump core to the UPROTO/UFUNCTION object.
	DataPump core send this IOCTL to get information of Microsoft OS string
	descriptor.  This edge IOCTL will be sent only if client enables
	this feature using USBPUMP_IOCTL_DEVICE_SET_MS_OS_DESCRIPTOR_PROCESS.

Returns:
	Return either USBPUMP_IOCTL_RESULT_NOT_CLAIMED (if the IOCTL code was
	not recognized), USBPUMP_IOCTL_RESULT_SUCCESS (if the IOCTL code was
	recognized and the operation was successfully performed), or some
	error code (if the IOCTL code was recognized, but the operation
	could not be performed for some reason).

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG *pArg,
		UCONFIG *pConfig
		);

	This macro will correctly initialize an ..._GET_FUNCTION_SECTION_ARG
	structure. If the layout of the argument structure changes in the
	future, this macro will initialize the new elements in a forward-
	compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG)
	{
	__TMS_UCONFIG *	pConfig;	/* IN: Active configuration */
	__TMS_BOOL	fSupportOsDesc;	/* OUT: support OS string descriptor */
	};

/* the setup for the ACTIVATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG * __TMS_CONST	\
		v__pArg = (ARG_pArg);					\
	v__pArg->pConfig = (ARG_pConfig);				\
	v__pArg->fSupportOsDesc = __TMS_FALSE;				\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION

Index:	Type:	USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG
	Name:	USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG_SETUP_V1

Function:
	Outcall from DataPump core to the protocols; to get the "function
	section" of the Microsoft extened compat ID feature descriptor.

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG
			structure.

	pOutParam	points to a USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG
			structure.

	USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG has the following elements:

	UCONFIG *pConfig;
		IN: pointer of UCONFIG.  This is current active configuration.
		The protocol instance should check this UCONFIG structure to
		figure out that protocol is part of active configuration.
		If the protocol object is part of current active configuration,
		it should return function section of the extened compat ID
		feature descriptor.

	UINT8 *pBuffer;
	BYTES nBuffer;
		IN: function section save buffer pointer and size of buffer.

	BYTES nActual;
		OUT: actual number of written bytes in the buffer.

Description:
	This IOCTL is sent from a DataPump core to the UPROTO/UFUNCTION object.
	DataPump core send this IOCTL to retrieve all "function section" of the
	Microsoft extened compat ID feature descriptor if client enables
	this feature using USBPUMP_IOCTL_DEVICE_SET_MS_OS_DESCRIPTOR_PROCESS.

Returns:
	Return either USBPUMP_IOCTL_RESULT_NOT_CLAIMED (if the IOCTL code was
	not recognized), USBPUMP_IOCTL_RESULT_SUCCESS (if the IOCTL code was
	recognized and the operation was successfully performed), or some
	error code (if the IOCTL code was recognized, but the operation
	could not be performed for some reason).

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG *pArg,
		UCONFIG *pConfig,
		UINT8 *pBuffer,
		BYTES nBuffer
		);

	This macro will correctly initialize an ..._GET_FUNCTION_SECTION_ARG
	structure. If the layout of the argument structure changes in the
	future, this macro will initialize the new elements in a forward-
	compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG)
	{
	__TMS_UCONFIG *	pConfig;	/* IN: Active configuration */
	__TMS_UINT8 *	pBuffer;	/* IN: function section save buffer */
	__TMS_BYTES	nBuffer;	/* IN: size of pBuffer */
	__TMS_BYTES	nActual;	/* OUT: number of written bytes */
	};

/* the setup for the ACTIVATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig,							\
	ARG_pBuffer,							\
	ARG_nBuffer							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG * __TMS_CONST	\
		v__pArg = (ARG_pArg);					\
	v__pArg->pConfig = (ARG_pConfig);				\
	v__pArg->pBuffer = (ARG_pBuffer);				\
	v__pArg->nBuffer = (ARG_nBuffer);				\
	v__pArg->nActual = 0;						\
   	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY

Index:	Type:	USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG
	Name:	USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG_SETUP_V1

Function:
	Outcall from DataPump core to the protocols; to get the "extened
	property OS feature descriptor" of the Microsoft OS descriptor.

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG
			structure.

	pOutParam	points to a USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG
			structure.

	USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG has the following elements:

	UCONFIG *pConfig;
		IN: pointer of UCONFIG.  This is current active configuration.
		The protocol instance should check this UCONFIG structure to
		figure out that protocol is part of active configuration.
		If the protocol object is part of current active configuration,
		it should return function section of the extened compat ID
		feature descriptor.

	UINT8 *pBuffer;
	BYTES nBuffer;
		IN: function section save buffer pointer and size of buffer.

	UINT nProperty;
		OUT: number of extended properties written in the buffer.

	BYTES nActual;
		OUT: actual number of written bytes in the buffer.

Description:
	This IOCTL is sent from a DataPump core to the UPROTO/UFUNCTION object.
	DataPump core send this IOCTL to retrieve all "extended property" of the
	Microsoft extened property OS feature descriptor if client enables
	this feature using USBPUMP_IOCTL_DEVICE_SET_MS_OS_DESCRIPTOR_PROCESS.

Returns:
	Return either USBPUMP_IOCTL_RESULT_NOT_CLAIMED (if the IOCTL code was
	not recognized), USBPUMP_IOCTL_RESULT_SUCCESS (if the IOCTL code was
	recognized and the operation was successfully performed), or some
	error code (if the IOCTL code was recognized, but the operation
	could not be performed for some reason).

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG *pArg,
		UCONFIG *pConfig,
		UINT8 *pBuffer,
		BYTES nBuffer
		);

	This macro will correctly initialize an ..._GET_EXTENDED_PROPERTY_ARG
	structure. If the layout of the argument structure changes in the
	future, this macro will initialize the new elements in a forward-
	compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG)
	{
	__TMS_UCONFIG *	pConfig;	/* IN: Active configuration */
	__TMS_UINT8 *	pBuffer;	/* IN: function section save buffer */
	__TMS_BYTES	nBuffer;	/* IN: size of pBuffer */
	__TMS_UINT	nProperty;	/* OUT: number of properties */
	__TMS_BYTES	nActual;	/* OUT: number of written bytes */
	};

/* the setup for the ACTIVATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig,							\
	ARG_pBuffer,							\
	ARG_nBuffer							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG * __TMS_CONST \
		v__pArg = (ARG_pArg);					\
	v__pArg->pConfig = (ARG_pConfig);				\
	v__pArg->pBuffer = (ARG_pBuffer);				\
	v__pArg->nBuffer = (ARG_nBuffer);				\
	v__pArg->nProperty = 0;						\
	v__pArg->nActual = 0;						\
   	} while (0)

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE(t, i, NAME)	\
   __TMS_USBPUMP_IOCTL_EDGE(t, i, NAME)
# define USBPUMP_IOCTL_EDGE_ACTIVATE	\
   __TMS_USBPUMP_IOCTL_EDGE_ACTIVATE
# define USBPUMP_IOCTL_EDGE_DEACTIVATE	\
   __TMS_USBPUMP_IOCTL_EDGE_DEACTIVATE
# define USBPUMP_IOCTL_EDGE_BUS_EVENT	\
   __TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT
# define USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO	\
   __TMS_USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO
# define USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION	\
   __TMS_USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION
# define USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY	\
   __TMS_USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY
# define USBPUMP_IOCTL_EDGE_ACTIVATE_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_ACTIVATE_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx							\
	)
# define USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_DEACTIVATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx							\
	)
# define USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_EventCode,							\
	ARG_pEventSpecificInfo,						\
	ARG_fRemoteWakeupEnable						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V2(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_EventCode,							\
	ARG_pEventSpecificInfo,						\
	ARG_fRemoteWakeupEnable						\
	)
# define USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_EventCode,							\
	ARG_pEventSpecificInfo						\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_BUS_EVENT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_EventCode,							\
	ARG_pEventSpecificInfo						\
	)
# define USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_GET_MS_OS_DESC_INFO_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig							\
	)
# define USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig,							\
	ARG_pBuffer,							\
	ARG_nBuffer							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_GET_FUNCTION_SECTION_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig,							\
	ARG_pBuffer,							\
	ARG_nBuffer							\
	)
# define USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig,							\
	ARG_pBuffer,							\
	ARG_nBuffer							\
	)	\
	__TMS_USBPUMP_IOCTL_EDGE_GET_EXTENDED_PROPERTY_ARG_SETUP_V1(	\
	ARG_pArg,							\
	ARG_pConfig,							\
	ARG_pBuffer,							\
	ARG_nBuffer							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctledgecodes.h ****/
#endif /* _USBIOCTLEDGECODES_H_ */
