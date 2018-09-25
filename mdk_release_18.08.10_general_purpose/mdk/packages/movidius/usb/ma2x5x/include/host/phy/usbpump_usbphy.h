/* usbpump_usbphy.h	Mon Jul 14 2014 21:33:35 tmm */

/*

Module:  usbpump_usbphy.h

Function:
	Definition of USBPUMP_USBPHY object

Version:
	V3.13a	Mon Jul 14 2014 21:33:35 tmm	Edit level 10

Copyright notice:
	This file copyright (C) 2005-2009, 2011, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	August 2005

Revision history:
   1.91h  Tue Aug 23 2005 15:35:55  chwon
	Module created.

   1.93a  Fri Oct 07 2005 09:39:28  chwon
	Pull out union definition inside of struct

   1.93a  Wed Oct 19 2005 14:15:37  chwon
	Add pObjectIoctlFn in USBPUMP_USBPHY

   1.97k  Fri Mar 07 2008 12:47:48  chwon
	3567: add AnnunciatorMaxSession to configure OTGFSM annunciator.

   1.97k  Fri Jul 11 2008 13:58:13  chwon
	4286: add USBPUMP_USBPHY_INIT_FN type definition

   1.97k  Tue Aug 05 2008 15:12:07  chwon
	UDP-6309: add pHcdIsrFn and pDcdIsrFn in the USBPUMP_USBPHY

   1.97k  Tue Jan 20 2009 14:30:20  chwon
	7275: add ulDebugFlags in the PHY configuration
	7283: change UDEVICE * and USBPUMP_HCD * to USBPUMP_OBJECT_HEADER *
	and remove inclusion of host header file

   2.01a  Tue Nov 03 2009 12:29:16  chwon
	9234: move common type definition to usbpump_usbphy_types.h

   3.01f  Tue Nov 08 2011 16:40:54  chwon
	9941: add pOtgFsmEvaluateFn in the configuration and add ..._INIT_V4().

   3.13a  Thu Jul 10 2014 20:37:16  tmm
	18309: add "non OTG" variant: UsbPumpUsbPhy_Create_NonOtg(), so we
	don't have to pull in all the OTG stuff at link time.

	18386: Fix the Russian-Doll model for the PHY objects.

*/

#ifndef _USBPUMP_USBPHY_H_		/* prevent multiple includes */
#define _USBPUMP_USBPHY_H_

#ifndef _USBPUMP_USBPHY_TYPES_H_
# include "usbpump_usbphy_types.h"
#endif

#ifndef _USBIOCTL_PHY_H_
# include "usbioctl_phy.h"
#endif

#ifndef _USBIOCTL_OTG_H_
# include "usbioctl_otg.h"
#endif

#ifndef _OTGFSM_H_
# include "otgfsm.h"
#endif

#ifndef _UEVENTNODE_H_
# include "ueventnode.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

/****************************************************************************\
|
|	USBPUMP_USBPHY_PORT_STATUS
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPHY_HOST_PORT_STATUS);
__TMS_TYPE_DEF_STRUCT	(USBPHY_DEVICE_PORT_STATUS);
__TMS_TYPE_DEF_UNION	(USBPHY_PORT_STATUS);

struct __TMS_STRUCTNAME	(USBPHY_HOST_PORT_STATUS)
	{
	/* see usbhub20.h -- status if in host state */
	__TMS_UINT16			wStatus;
	__TMS_UINT16			wChange;
	};

struct __TMS_STRUCTNAME	(USBPHY_DEVICE_PORT_STATUS)
	{
	/* in device mode, this reflects the attach state */
	__TMS_UINT32			Flags;
	};

union __TMS_UNIONNAME	(USBPHY_PORT_STATUS)
	{
	__TMS_UINT32			Global;
	__TMS_USBPHY_HOST_PORT_STATUS	Host;
	__TMS_USBPHY_DEVICE_PORT_STATUS	Device;
	};

struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_PORT_STATUS)
	{
	__TMS_USBPUMP_USBPHY_STATE	PhyState;
	__TMS_USBPHY_PORT_STATUS	Status;
	};

/*

Type:	USBPUMP_USBPHY

Index:	Macro:	USBPUMP_USBPHY_CONTENTS__UNION
	Macro:	USBPUMP_USBPHY_CONTENTS__STRUCT
	Type:	union __TMS_USBPUMP_USBPHY
	Constant:	USBPUMP_USBPHY_TAG

Function:
	The generic object that represents a USB PHY.

Description:
	A USBPUMP_USBPHY is the central context object for a given USB PHY
	instance within the DataPump.

Contents:
	USBPUMP_OBJECT_HEADER ObjectHeader;
		The standard object header.  The tag is |USBPUMP_USBPHY_TAG|.
		The IOCTL parent is the pointer to the next object closer
		to the |UPLATFORM| -- depending on the design of the USBPHY,
		there may be no objects, or several objects, in series
		between this USBPHY and the UPLATFORM.

		This may also be referenced as Phy.ObjectHeader.

	USBPUMP_USBPHY_CONTENTS Phy;
		The common definitions for all USBPHYs, defined below.

	UPLATFORM *Phy.pPlatform;
		Pointer to the platform object, for convenience.

	UINT32 Phy.OpenCount;
		This object open counter

	USBPUMP_OBJECT_HEADER *Phy.pDcdObject;
		Pointer to the DCD object.

	USBPUMP_USBPHY_DCD_EVENT_FN *Phy.pDcdEventFn;
		Pointer to the DCD event function

	VOID *Phy.pDcdContext;
		Pointer to the DCD event context

	USBPUMP_USBPHY_DCD_ISR_FN *pDcdIsrFn;
		Pointer to the DCD primary ISR function

	USBPUMP_OBJECT_HEADER *Phy.pHcdObject;
		Pointer to the HCD object.

	USBPUMP_USBPHY_HCD_EVENT_FN *Phy.pHcdEventFn;
		Pointer to the HCD event function

	VOID *Phy.pHcdContext;
		Pointer to the HCD event context

	USBPUMP_USBPHY_HCD_ISR_FN *pHcdIsrFn;
		Pointer to the HCD primary ISR function

	UINT32	Phy.NumPorts;
		Root hubs can have up to 31 ports in this model, port numbers
		start at one.

	BOOL Phy.fPowerEnable;
		State of power to the PHY

	UINT16 Phy.wStatus;
	UINT16 Phy.wChange;
		State of the root hub -- see usbhub20.h

	USBPUMP_USBPHY_PORT_STATUS *Phy.pPorts;
		Pointer to the port status arrary

Notes:
	The USBPUMP_USBPHY object is intended to be embedded in a larger
	device-hardware specific object, just as a UDEVICE is.  The
	embedding discipline is slightly different, because we are assuming
	ANSI compilers now, which was not the case when the DataPump
	was created.

	In order to define a hardware specific USBPHY object drived from
	the base USBPHY, please follow these rules.  (NB: the following
	rules are for the usual case where the USBPHY implementation is
	using the common USBPHY library as its framework)

	    1)	Define your top level PHY object as a union, using the
		following schema:

.			__TMS_TYPE_DEF_UNION(my_phy_name);

		For example:

.			__TMS_TYPE_DEF_UNION(USBPUMP_USBPHY_ISP1301);

	    2)  Define a "contents" structure for your USBPHY object, using

.			__TMS_TYPE_DEF_STRUCT(my_phy_name_CONTENTS);

		For example:
.			__TMS_TYPE_DEF_STRUCT(USBPUMP_USBPHY_ISP1301_CONTENTS);

	    3)  Define the body of your USBPHY object:

.			struct __TMS_STRUCTNAME(my_phy_name_CONTENTS)
.				{
.				__TMS_USBPUMP_USBPHY_CONTENTS__STRUCT;
.
.				// specific contents here
.				};

	    4)	Define the overall union for your USBPHY object:

.			#define __TMS_my_phy_name_CONTENTS__UNION	\
.				__TMS_USBPUMP_USBPHY_CONTENTS__UNION; \
.				__TMS_my_phy_name_CONTENTS
.
.			union __TMS_UNIONNAME(my_phy_name)
.				{
.				__TMS_my_phy_name_CONTENTS__UNION;
.				};

		Example:

.			#define __TMS_USBPUMP_USBPHY_ISP1301_CONTENTS__UNION \
.				__TMS_USBPUMP_USBPHY_CONTENTS__UNION;	     \
.				__TMS_USBPUMP_USBPHY_ISP1301_CONTENTS Isp1301
.
.			union __TMS_UNIONNAME(USBPUMP_USBPHY_ISP1301)
.				{
.				__TMS_USBPUMP_USBPHY_ISP1301_CONTENTS__UNION;
.				};

	    5)	Then refer to your contents using |pUsbPhy->Isp1301.{whatever}|

	If you follow the above dicipline, you will further be able to
	create types that are based on the USBPUMP_USBPHY_my_phy_name
	structure, and which allow code (especially in macros) to refer to
	fields by name without worrying too much about the type, and without
	using casts.

	Of course, you might want to follow type-cloaking rules.

See Also:

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_CONTENTS);
struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_CONTENTS)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	__TMS_UPLATFORM *			pPlatform;
	__TMS_UINT32				OpenCount;

	/* DCD event handling info */
	__TMS_USBPUMP_OBJECT_HEADER *		pDcdObject;
	__TMS_USBPUMP_USBPHY_DCD_EVENT_FN *	pDcdEventFn;
	__TMS_VOID *				pDcdContext;
	__TMS_USBPUMP_USBPHY_DCD_ISR_FN *	pDcdIsrFn;	/* UDP-6309 */

	__TMS_UEVENTNODE			DcdEventNode;

	/* HCD event handling info */
	__TMS_USBPUMP_OBJECT_HEADER *		pHcdObject;
	__TMS_USBPUMP_USBPHY_HCD_EVENT_FN *	pHcdEventFn;
	__TMS_VOID *				pHcdContext;
	__TMS_USBPUMP_USBPHY_HCD_ISR_FN *	pHcdIsrFn;	/* UDP-6309 */

	/* root hubs can have up to 31 ports in this model */
	__TMS_UINT32				NumPorts;

	/* state of power to the PHY */
	__TMS_BOOL				fPowerEnable;

	/* see usbhub20.h -- state of the root hub: */
	__TMS_UINT16				wStatus;
	__TMS_UINT16				wChange;

	/* port status */
	__TMS_USBPUMP_USBPHY_PORT_STATUS *	pPorts;

	/* OTGFSM context */
	__TMS_USBPUMP_OTGFSM			OtgFsm;
	__TMS_USBPUMP_OTGSTATE			OtgState;

	/* ASYNC IOCTL QE handling */
	__TMS_USBPUMP_IOCTL_QE *		pIoctlQeHead;
	__TMS_UCALLBACKCOMPLETION		IoctlQeEvent;

	/* IOCTL function of larger object for embedding this object */
	__TMS_USBPUMP_OBJECT_IOCTL_FN *		pObjectIoctlFn;
	};

/* prefix your derived object this way */
#define	__TMS_USBPUMP_USBPHY_CONTENTS__STRUCT			\
	__TMS_USBPUMP_USBPHY_CONTENTS	Phy

/* a macro that you can embed in your own union */
#define __TMS_USBPUMP_USBPHY_CONTENTS__UNION			\
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;			\
	__TMS_USBPUMP_USBPHY_CONTENTS	Phy;			\
	__TMS_USBPUMP_USBPHY		PhyCast

union __TMS_UNIONNAME(USBPUMP_USBPHY)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;
	__TMS_USBPUMP_USBPHY_CONTENTS	Phy;
	};

/*
|| Object related things
*/

/* the USB PHY object tag */
#define	__TMS_USBPUMP_USBPHY_TAG	__TMS_UHIL_MEMTAG('U', 'p', 'h', 'y')

/* the default name */
#define	__TMS_USBPUMP_USBPHY_NAME(Text)  Text ".usbphy.mcci.com"

/****************************************************************************\
|
|	The USBPUMP_USBPHY configuration information
|
\****************************************************************************/

struct __TMS_STRUCTNAME	(USBPUMP_USBPHY_CONFIG_INFO)
	{
	__TMS_SIZE_T			UsbPhyObjectSize;
	__TMS_CONST __TMS_TEXT *	UsbPhyObjectName;
	__TMS_UINT16			NumPorts;
	__TMS_UINT16			tb_vbuschrg_srp;
	__TMS_UCALLBACKFN *		pUsbPhyCallbackFn;
	__TMS_UCALLBACKFN *		pOtgFsmCallbackFn;
	__TMS_USBPUMP_OBJECT_IOCTL_FN *	pObjectIoctlFn;
	__TMS_UINT			AnnunciatorMaxSession;	/* 3567 */
	__TMS_UINT32			ulDebugFlags;
	__TMS_USBPUMP_OTGFSM_EVAL_FN *	pOtgFsmEvaluateFn;
	};


/*
|| initialization functions.  Don't change the calling sequence for any
|| macro ending in Vn; instead, create a macro ending in Vn+1, and
|| redefine the Vn macro to expand in an upward-compatible way.
*/
#define	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V4(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags,					\
		a_pOtgFsmEvaluateFn				\
		)						\
	{							\
	(a_UsbPhyObjectSize),					\
	(a_UsbPhyObjectName),					\
	(a_NumPorts),						\
	(a_tb_vbuschrg_srp),					\
	(a_pUsbPhyCallbackFn),					\
	(a_pOtgFsmCallbackFn),					\
	(a_pObjectIoctlFn),					\
	(a_AnnunciatorMaxSession),				\
	(a_ulDebugFlags),					\
	(a_pOtgFsmEvaluateFn)					\
	}

#define	__TMS_USBPUMP_USBPHY_CONFIG_INFO_SETUP_V4(		\
		a_pConfig,					\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags,					\
		a_pOtgFsmEvaluateFn				\
		)						\
   do	{							\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO * __TMS_CONST		\
		pConfig_ = (a_pConfig);				\
								\
	pConfig_->UsbPhyObjectSize = (a_UsbPhyObjectSize);	\
	pConfig_->UsbPhyObjectName = (a_UsbPhyObjectName);	\
	pConfig_->NumPorts = (a_NumPorts);			\
	pConfig_->tb_vbuschrg_srp = (a_tb_vbuschrg_srp);	\
	pConfig_->pUsbPhyCallbackFn = (a_pUsbPhyCallbackFn);	\
	pConfig_->pOtgFsmCallbackFn = (a_pOtgFsmCallbackFn);	\
	pConfig_->pObjectIoctlFn = (a_pObjectIoctlFn);		\
	pConfig_->AnnunciatorMaxSession =			\
		(a_AnnunciatorMaxSession);			\
	pConfig_->ulDebugFlags = (a_ulDebugFlags);		\
	pConfig_->pOtgFsmEvaluateFn = (a_pOtgFsmEvaluateFn);	\
	} while (0)

#define	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V3(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags					\
		)						\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V4(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags,					\
		/* V4: pOtgFsmEvalFn */ UsbPumpOtgFsm_EvaluateFsm_V1	\
		)

#define	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V2(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession				\
		)						\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V3(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		/* V3: ulDebugFlags */ 0			\
		)

#define	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V1(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn				\
		)						\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V2(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		/* V2: AnnunciatorMaxSession */ 0		\
		)


/****************************************************************************\
|
|	Some internal API functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_USBPHY *
UsbPumpUsbPhy_Create(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_CONST __TMS_USBPUMP_USBPHY_CONFIG_INFO * /* pConfig */
	);

__TMS_USBPUMP_USBPHY *
UsbPumpUsbPhy_Create_NonOtg(
	__TMS_UPLATFORM *		/* pPlatform */,
	__TMS_CONST __TMS_USBPUMP_USBPHY_CONFIG_INFO * /* pConfig */
	);

__TMS_VOID
UsbPumpUsbPhy_Delete(
	__TMS_USBPUMP_USBPHY *
	);

__TMS_VOID
UsbPumpUsbPhy_Delete_NonOtg(
	__TMS_USBPUMP_USBPHY *
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpUsbPhy_Ioctl(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pHdr */,
	__TMS_USBPUMP_IOCTL_CODE	/* Ioctl */,
	__TMS_CONST __TMS_VOID *	/* pInParam */,
	__TMS_VOID *			/* pOutParam */
	);

__TMS_USBPUMP_IOCTL_QE *
UsbPumpUsbPhy_GetIoctlQe(
	__TMS_USBPUMP_USBPHY *		/* pUsbPhy */
	);

__TMS_VOID
UsbPumpUsbPhy_PutIoctlQe(
	__TMS_USBPUMP_USBPHY *		/* pUsbPhy */,
	__TMS_USBPUMP_IOCTL_QE *	/* pIoctlQe */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh -l ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBPHY_CONTENTS__STRUCT	__TMS_USBPUMP_USBPHY_CONTENTS__STRUCT
# define USBPUMP_USBPHY_CONTENTS__UNION	__TMS_USBPUMP_USBPHY_CONTENTS__UNION
# define USBPUMP_USBPHY_TAG	__TMS_USBPUMP_USBPHY_TAG
# define USBPUMP_USBPHY_NAME(Text)	__TMS_USBPUMP_USBPHY_NAME(Text)
# define USBPUMP_USBPHY_CONFIG_INFO_INIT_V4(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags,					\
		a_pOtgFsmEvaluateFn				\
		)	\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V4(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags,					\
		a_pOtgFsmEvaluateFn				\
		)
# define USBPUMP_USBPHY_CONFIG_INFO_SETUP_V4(		\
		a_pConfig,					\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags,					\
		a_pOtgFsmEvaluateFn				\
		)	\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO_SETUP_V4(		\
		a_pConfig,					\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags,					\
		a_pOtgFsmEvaluateFn				\
		)
# define USBPUMP_USBPHY_CONFIG_INFO_INIT_V3(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags					\
		)	\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V3(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession,			\
		a_ulDebugFlags					\
		)
# define USBPUMP_USBPHY_CONFIG_INFO_INIT_V2(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession				\
		)	\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V2(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn,				\
		a_AnnunciatorMaxSession				\
		)
# define USBPUMP_USBPHY_CONFIG_INFO_INIT_V1(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn				\
		)	\
	__TMS_USBPUMP_USBPHY_CONFIG_INFO_INIT_V1(		\
		a_UsbPhyObjectSize,				\
		a_UsbPhyObjectName,				\
		a_NumPorts,					\
		a_tb_vbuschrg_srp,				\
		a_pUsbPhyCallbackFn,				\
		a_pOtgFsmCallbackFn,				\
		a_pObjectIoctlFn				\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbphy.h ****/
#endif /* _USBPUMP_USBPHY_H_ */
