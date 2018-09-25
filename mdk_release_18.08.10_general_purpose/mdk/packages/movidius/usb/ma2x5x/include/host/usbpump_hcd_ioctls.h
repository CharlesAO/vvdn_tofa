/* usbpump_hcd_ioctls.h	Thu Apr 21 2016 16:36:43 chwon */


/*

Module:  usbpump_hcd_ioctls.h

Function:
	Defines the IOCTLs for HCD objects.

Version:
	V3.17a	Thu Apr 21 2016 16:36:43 chwon	Edit level 28

Copyright notice:
	This file copyright (C) 2005-2007, 2009-2012, 2014, 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Wed Jun  8 2005 18:04:28  tmm
	Module created.

   1.93c  Fri Dec 02 2005 12:41:46  chwon
	Added USBPUMP_IOCTL_HCD_CHECK_B_CONN.

   1.93e  Fri Jan 06 2006 10:39:08  chwon
	Added USBPUMP_IOCTL_HCD_SET_PORT_FEATURE.

   1.93e Mon Jan 30 2006 14:31:15  tmm
	Add USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS

   1.97i  Tue May 15 2007 14:17:35  chwon
	2757: add Selector field in USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG

   1.97i  Thu Aug 02 2007 15:56:51  chwon
	3119: add USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG

   1.97j  Fri Aug 31 2007 12:17:41  chwon
	3212: add USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE

   1.97j  Thu Sep 27 2007 12:08:27  chwon
	3212: remove pHcd in USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG struct

   1.97j  Thu Oct 18 2007 13:44:05  chwon
	3161: add USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP

   1.97j  Tue Nov 13 2007 15:57:05  chwon
	3471: add USBPUMP_USBDI_PORT in the USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG

   1.97j  Wed Nov 14 2007 11:15:47  djt
	3265: Added nTTLevels.

   2.01a  Fri Feb 27 2009 13:38:31  maw
   	7472: Add HARDWARE_INIT, HARDWARE_DEINIT, START and STOP Ioctl

   2.01a  Tue Apr 14 2009 23:55:11  maw
   	7472: Add _ASYNC to IOCTL def

   2.01a  Mon Nov 16 2009 12:14:05  chwon
	9172: Added nRootHubs in the USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG.
	Added pRootHubDevice in the USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG.

   2.01a  Thu Dec 03 2009 16:22:48  chwon
	9172: Fix document -- remove HcdKit.pRootPortStatus.

   3.01c  Wed Mar 17 2010 19:31:55  chwon
	10020: Add USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER ioctl.

   3.01c  Thu Apr 22 2010 14:42:19  chwon
	10182: Add USBPUMP_IOCTL_HCD_OPEN_RAW_API and
	USBPUMP_IOCTL_HCD_CLOSE_RAW_API ioctl.

   3.01c  Fri Apr 23 2010 13:22:32  chwon
	10182: Remove pHcd parameter for USBPUMP_IOCTL_HCD_OPEN_RAW_API and
	USBPUMP_IOCTL_HCD_CLOSE_RAW_API argument structure.

   3.01c  Wed May 19 2010 12:30:06  chwon
	10182: Remove USBPUMP_IOCTL_HCD_CLOSE_RAW_API ioctl because close is
	part of switch.

   3.01c  Mon Aug 16 2010 12:01:34  chwon
	10988: add USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC ioctl.

   3.01d  Tue Oct 19 2010 20:53:56  chwon
	11666: add fOtgNotificationCapable in USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE
	ioctl structure.

   3.01d  Thu Nov 04 2010 13:12:04  chwon
	11666: add fIdPinState in USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE ioctl.

   3.01d  Thu Mar 10 2011 22:36:00  chwon
	12923: add PowerAction parameter for ..._POWER_STATE_CHANGE_ASYNC ioctl

   3.01f  Sun Jun 12 2011 15:28:15  ssyang
	13589: add USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE Ioctl.

   3.01f  Wed Aug 31 2011 17:02:42  chwon
	13501: add USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM ioctl.

   3.01f  Fri Jun 29 2012 18:32:12  cccho
	15628: add USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO ioctl.

   3.13b  Tue Oct 07 2014 16:25:24  chwon
	18569: Add USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE ioctl.

   3.13b  Fri Oct 24 2014 11:08:58  chwon
	18600: Fix doc-xml error.

   3.17a  Thu Apr 21 2016 16:36:43  chwon
	20067: Add USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED ioctl.

*/

#ifndef _USBPUMP_HCD_IOCTLS_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_IOCTLS_H_

#ifndef _USBPUMP_HOST_TYPES_H_
# include "usbpump_host_types.h"
#endif

#ifndef _USBPUMP_HCD_TYPES_H_
# include "usbpump_hcd_types.h"
#endif

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMP_HCD_RAW_API_H_
# include "usbpump_hcd_raw_api.h"
#endif

#ifndef _USBDESC_TYPES_H_
# include "usbdesc_types.h"
#endif

#ifndef _USBHUB20_H_
# include "usbhub20.h"
#endif

/****************************************************************************\
|
|	The IOCTL_EDGE_HCD macros are sent upwards (to USBDI or friends)
|
\****************************************************************************/

/**** a fancy macro for defining the HCD out-calls ****/
#define __TMS_USBPUMP_IOCTL_EDGE_HCD(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_HCD_EDGE, (i),		\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_HCD_, NAME, _ARG) \
		)

/****************************************************************************\
|
|	The non-EDGE IOCTLs arrive inwards from USBDI.
|
\****************************************************************************/

/**** define the HCD in-calls ****/
#define __TMS_USBPUMP_IOCTL_HCD(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_HCD,				\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_HCD_, NAME, _ARG)	\
		)

/**** the IOCTLs ****/
#define	__TMS_USBPUMP_IOCTL_HCD_GET_PORT_COUNT				\
	__TMS_USBPUMP_IOCTL_HCD(0, RW, GET_PORT_COUNT)

#define	__TMS_USBPUMP_IOCTL_HCD_CHECK_B_CONN				\
	__TMS_USBPUMP_IOCTL_HCD(1, RW, CHECK_B_CONN)

#define	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE			\
	__TMS_USBPUMP_IOCTL_HCD(2, W, SET_PORT_FEATURE)

#define	__TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS			\
	__TMS_USBPUMP_IOCTL_HCD(3, RW, GET_SCHEDULE_PARAMS)

#define	__TMS_USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG		\
	__TMS_USBPUMP_IOCTL_HCD(4, RW, GET_ROOT_PORT_HANDOFF_FLAG)

#define	__TMS_USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE				\
	__TMS_USBPUMP_IOCTL_HCD(5, RW, GET_OTG_CAPABLE)

#define	__TMS_USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP			\
	__TMS_USBPUMP_IOCTL_HCD(6, RW, GET_ADDRESS_BITMAP)

#define	__TMS_USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC			\
	__TMS_USBPUMP_IOCTL_HCD(7, W_ASYNC, HARDWARE_INIT_ASYNC)

#define	__TMS_USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC			\
	__TMS_USBPUMP_IOCTL_HCD(8, W_ASYNC, HARDWARE_DEINIT_ASYNC)

#define	__TMS_USBPUMP_IOCTL_HCD_START_ASYNC				\
	__TMS_USBPUMP_IOCTL_HCD(9, W_ASYNC, START_ASYNC)

#define	__TMS_USBPUMP_IOCTL_HCD_STOP_ASYNC				\
	__TMS_USBPUMP_IOCTL_HCD(10, W_ASYNC, STOP_ASYNC)

#define	__TMS_USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER		\
	__TMS_USBPUMP_IOCTL_HCD(11, RW, NORMALIZE_ROOT_HUB_PORT_NUMBER)

#define	__TMS_USBPUMP_IOCTL_HCD_OPEN_RAW_API				\
	__TMS_USBPUMP_IOCTL_HCD(12, R, OPEN_RAW_API)

#define	__TMS_USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC		\
	__TMS_USBPUMP_IOCTL_HCD(13, W_ASYNC, POWER_STATE_CHANGE_ASYNC)

#define	__TMS_USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE		\
	__TMS_USBPUMP_IOCTL_HCD(14, W, ROOTHUB_ENABLE_WAKE_HARDWARE)

#define	__TMS_USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM			\
	__TMS_USBPUMP_IOCTL_HCD(15, W, ENABLE_HARDWARE_LPM)

#define	__TMS_USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO		\
	__TMS_USBPUMP_IOCTL_HCD(16, R, GET_COMPATIBLE_PORT_INFO)

#define	__TMS_USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE		\
	__TMS_USBPUMP_IOCTL_HCD(17, W, SET_FORCE_FULL_SPEED_MODE)

#define	__TMS_USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED			\
	__TMS_USBPUMP_IOCTL_HCD(18, RW, HUB_DEVICE_CONNECTED)

/**** details are below ****/

/****************************************************************************\
|
|	IOCTL details
|
\****************************************************************************/

/*

IOCTL:	USBPUMP_IOCTL_HCD_GET_PORT_COUNT

Index:	Type:	USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG
	Name:	USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG_SETUP_V1()

Function:
	Obtain the total number of ports for the specified HCD.

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest.  This is passed to facilitate
		filtering and proxying.

Output:
	BYTES nRootHubs;
		Number of root hubs on the HCD.

	BYTES nRootHubPorts;
		Number of ports on the root hub.  This is always less than
		255 (due to root hub API limitations).

Description:
	During USBDI initialization, USBDI needs to allocate RAM based
	on the port count of the underlying HCs.  Although we could learn
	how many ports are available via the root hub API, this is
	inconvenient during initialization because it's an asynchronous API.

	Fortunately, however, the HCDs know how many ports could possibly
	be present, and can return a count of ports.  This might be a
	conservative estimate for hardware that is plug-and-play, remote,
	and not present yet; but it will maintain the principle of least
	surprise.

	HCDKIT-based HCDs don't need to worry about this IOCTL; the HCDKIT
	IOCTL dispatcher will process it and hand back the correct result.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success; some other error code
	if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG *pArg,
		USBPUMP_HCD *pHcd
		);

*/

/**** parameters to GET_PORT_COUNT ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG)
	{
	__TMS_USBPUMP_HCD *	pHcd;
	__TMS_BYTES		nRootHubs;
	__TMS_BYTES		nRootHubPorts;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd								\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->nRootHubs = 1;					\
	(a_pArg)->nRootHubPorts = 0;					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_CHECK_B_CONN

Index:	Type:	USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG
	Name:	USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG_SETUP_V1()

Function:
	Obtain the B_CONN state of port for the specified HCD.

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest.  This is passed to facilitate
		filtering and proxying.
	UINT32 iPort;
		Port number

Output:
	BOOL fB_CONN_State;
		TRUE if device is connected, FALSE if device is disconnected.

Description:
	Some PHY cannot detect device plug-out. This IOCTL called by USBPHY
	object to obtain the B_CONN state of port for the specified HCD.

	HCDKIT-based HCDs don't need to worry about this IOCTL; the HCDKIT
	IOCTL dispatcher will process it and hand back the correct result.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success; some other error code
	if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG *pArg,
		USBPUMP_HCD *pHcd
		);

Notes:
	The setup macro ensures that pArg->fB_CONN_State will be
	initially TRUE; so if the IOCTL is not claimed by the HCD, the
	client can assume that fB_CONN_State is TRUE.

*/

/**** parameters to CHECK_B_CONN ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG)
	{
	__TMS_USBPUMP_HCD *	pHcd;
	__TMS_UINT32		iPort;
	__TMS_BOOL		fB_CONN_State;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort								\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->iPort = (a_iPort);					\
	/* if the ioctl isn't claimed, we want state to be TRUE */	\
	(a_pArg)->fB_CONN_State = __TMS_TRUE;				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_SET_PORT_FEATURE

Index:	Type:	USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG
	Name:	USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V1()
	Name:	USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V2()

Function:
	Set/Clear port feature

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest.  This is passed to facilitate
		filtering and proxying.

	USBPUMP_HCD_DEVICE *pRootHubDevice;
		The root hub device of interest.

	UINT32 iPort;
		Port number

	BOOL fSet;
		TRUE if set feature, FALSE if clear feature

	UINT32 Feature;
		Hub class feature selector

	UINT32 Selector;
		Test selector

Output:
	None.

Description:
	This IOCTL called by virtual root-hub handling routine to set or
	clear root-hub port feature.  The HCD should check port state
	and do something if required.

	HCDKIT-based HCDs don't need to worry about this IOCTL; the HCDKIT
	IOCTL dispatcher will process it and hand back the correct result.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success; some other error code
	if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V3(
		USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG *pArg,
		USBPUMP_HCD *pHcd,
		USBPUMP_HCD_DEVICE *pRootHubDevice,
		UINT32 iPort,
		BOOL fSet,
		UINT32 Feature,
		UINT32 Selector
		);

*/

/**** parameters to SET_PORT_FEATURE ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG)
	{
	__TMS_USBPUMP_HCD *		pHcd;
	__TMS_USBPUMP_HCD_DEVICE *	pRootHubDevice;
	__TMS_UINT32			iPort;
	__TMS_BOOL			fSet;
	__TMS_UINT32			Feature;
	__TMS_UINT32			Selector;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V3(		\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_iPort,							\
	a_fSet,								\
	a_Feature,							\
	a_Selector							\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->pRootHubDevice = (a_pRootHubDevice);			\
	(a_pArg)->iPort = (a_iPort);					\
	(a_pArg)->fSet = (a_fSet);					\
	(a_pArg)->Feature = (a_Feature);				\
	(a_pArg)->Selector = (a_Selector);				\
	} while (0)

#define	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort,							\
	a_fSet,								\
	a_Feature,							\
	a_Selector							\
	)								\
	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V3(		\
		a_pArg,							\
		a_pHcd,							\
		/* V3: pRootHubDevice */ (a_pHcd)->Hcd.ppRootHubDevices[0], \
		a_iPort,						\
		a_fSet,							\
		a_Feature,						\
		a_Selector						\
		)

#define	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort,							\
	a_fSet,								\
	a_Feature							\
	)								\
	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V2(		\
		a_pArg,							\
		a_pHcd,							\
		a_iPort,						\
		a_fSet,							\
		a_Feature,						\
		/* V2: Selector */ 0					\
		)

/*

IOCTL:	USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS

Index:	Type:	USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG
	Name:	USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG_SETUP_V1()
	Name:	USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V1()
	Name:	USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V2()

Function:
	Returns info about the HCD's scheduling capabilities.

Input:
	USBPUMP_HCD *pHcd;
		The HCD in question; useful in case we want a class provider.

Output:
	UINT nLevels;
		The number of levels in the HCDs scheduling tree.
		nLevels must be at least nTTLevels plus 3.

	UINT nTTLevels;
		The number of levels in the HCDs TT scheduling tree.

	USBPUMP_USBDI_BANDWIDTH LowSpeedHubClocks;
		The number of clocks the host controller tosses in for
		low-speed hub delay.

	USBPUMP_USBDI_BANDWIDTH LowSpeedHostDelay;
		The number of clocks of delay between transactions for
		low-speed operations.

	USBPUMP_USBDI_BANDWIDTH FullSpeedHostDelay;
		The number of clocks of delay between transactions for
		full-speed operations.

	USBPUMP_USBDI_BANDWIDTH HighSpeedHostDelay;
		The number of clocks of delay between transactions for
		high-speed operations.

	USBPUMP_USBDI_BANDWIDTH MaxPeriodicClocks;
		The maximum number of periodic clocks per frame.

Description:
	The HCD is required to maintain its own schedule tree for the
	physical endpoints.  USBDI assumes that it the HCD may have up
	to six layers in its scheduling tree (and this is, in fact,
	a parameter); but the HCD engineer may not want to allocate that
	much memory.  For example, for simplistic host controllers with
	limited capabilities, the HCD may be designed with just a single
	schedule for periodic endpoints:  poll every endpoint every frame.

	This IOCTL allows the HCD to tell USBDI to limit the number of
	levels USBDI uses in scheduling for a given HCD instance.  USBDI
	will issue this IOCTL for each HCD instance discovered, so its
	perfectly acceptable to have different values for different
	instances.

	USBDI will assume that the HCD can schedule at 6 layers if this
	IOCTL is not implememtned.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success; some other error code
	if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG *pArg,
		USBPUMP_HCD *pHcd
		);

Result Macro:
	The following macro can be used to return the results, with a
	guarantee of source-code compatibility with future structures:

	VOID USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V2(
		USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG *pArg,
		UINT nLevels,
		UINT nTTLevels,
		USBPUMP_USBDI_BANDWIDTH LowSpeedHostDelay,
		USBPUMP_USBDI_BANDWIDTH FullSpeedHostDelay,
		USBPUMP_USBDI_BANDWIDTH HighSpeedHostDelay,
		USBPUMP_USBDI_BANDWIDTH MaxPeriodicClocks,
		USBPUMP_USBDI_BANDWIDTH LowSpeedHubClocks
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG)
	{
	__TMS_USBPUMP_HCD *	pHcd;
	__TMS_UINT		nLevels;
	__TMS_UINT		nTTLevels;
	__TMS_USBPUMP_USBDI_BANDWIDTH	LowSpeedHubClocks;
	__TMS_USBPUMP_USBDI_BANDWIDTH	LowSpeedHostDelay;
	__TMS_USBPUMP_USBDI_BANDWIDTH	FullSpeedHostDelay;
	__TMS_USBPUMP_USBDI_BANDWIDTH	HighSpeedHostDelay;
	__TMS_USBPUMP_USBDI_BANDWIDTH	MaxPeriodicClocks;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->nLevels = 0;						\
	(a_pArg)->nTTLevels = 0;					\
	} while (0)						
    

#define __TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V2(	\
	a_pArg,								\
	a_nLevels,							\
	a_nTTLevels,							\
	a_LowSpeedHostDelay,						\
	a_FullSpeedHostDelay,						\
	a_HighSpeedHostDelay,						\
	a_MaxPeriodicClocks,						\
	a_LowSpeedHubClocks						\
	)								\
   do	{								\
	(a_pArg)->nLevels = (a_nLevels);				\
	(a_pArg)->nTTLevels = (a_nTTLevels);				\
	(a_pArg)->LowSpeedHostDelay = (a_LowSpeedHostDelay);		\
	(a_pArg)->FullSpeedHostDelay = (a_FullSpeedHostDelay);		\
	(a_pArg)->HighSpeedHostDelay = (a_HighSpeedHostDelay);		\
	(a_pArg)->MaxPeriodicClocks = (a_MaxPeriodicClocks);		\
	(a_pArg)->LowSpeedHubClocks = (a_LowSpeedHubClocks);		\
	} while (0)

#define __TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V1(	\
	a_pArg,								\
	a_nLevels,							\
	a_LowSpeedHostDelay,						\
	a_FullSpeedHostDelay,						\
	a_HighSpeedHostDelay,						\
	a_MaxPeriodicClocks,						\
	a_LowSpeedHubClocks						\
	)								\
	__TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V2(	\
	a_pArg,								\
	a_nLevels,							\
	(__TMS_UINT)0,							\
	a_LowSpeedHostDelay,						\
	a_FullSpeedHostDelay,						\
	a_HighSpeedHostDelay,						\
	a_MaxPeriodicClocks,						\
	a_LowSpeedHubClocks						\
	)															

/*

IOCTL:	USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG

Index:	Type:	USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG
	Name:	USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG_SETUP_V1()

Function:
	Returns state of HCD's handoff flag.

Input:
	USBPUMP_HCD *pHcd;
		The HCD in question; useful in case we want a class provider.

	UINT portIndex;
		The port index.

	BOOL fReset;
		If fReset is FALSE, then the value of the flag is returned, and
		not changed.  If fReset is true, then the previous value of the
		handoff flag is returned, but the handoff flag value is reset.

Output:
	BOOL fState;
		The state of handoff flag.

Description:
	The HCD is required to maintain its own handoff flag.  This IOCTL called
	by USBD to obtain the handoff state of port for the specified HCD.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success; some other error code
	if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG *pArg,
		USBPUMP_HCD *pHcd,
		UINT portIndex,
		BOOL fReset
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG)
	{
	__TMS_USBPUMP_HCD *	pHcd;
	__TMS_UINT		portIndex;
	__TMS_BOOL		fReset;
	__TMS_BOOL		fState;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG_SETUP_V1(\
	a_pArg,								\
	a_pHcd,								\
	a_portIndex,							\
	a_fReset							\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->portIndex = (a_portIndex);				\
	(a_pArg)->fReset = (a_fReset);					\
	(a_pArg)->fState = __TMS_FALSE;					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE

Index:	Type:	USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG
	Name:	USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V1()

Function:
	Returns property of port that port is OTG capable or not

Input:
	USBPUMP_HCD_DEVICE *pHcdDevice;
		The HCD device structure.

	USBPUMP_USBDI_PORT *pPort;
		The USBD port structure. The HCD can use this information.

	UINT portIndex;
		The port index.

Output:
	BOOL fOtgCapable;
		The state of OTG capable.

	BOOL fOtgNotificationCapable;
		The state of OTG notification capable. This is only valid if
		fOtgCapable is TRUE.

	BOOL fIdPinState;
		The state of current ID pin. This is only valid if fOtgCapable
		is TRUE. This should be FALSE if fOtgNotificationCapable is
		FALSE.

Description:
	The HCD knows port has OTG capable or not.  This IOCTL called by hub
	driver to obtain the OTG capable state of port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success; some other error code
	if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V2(
		USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG *pArg,
		USBPUMP_HCD_DEVICE *	pHcdDevice,
		USBPUMP_USBDI_PORT *	pPort,
		UINT portIndex
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG)
	{
	__TMS_USBPUMP_HCD_DEVICE *	pHcdDevice;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_UINT			portIndex;
	__TMS_BOOL			fOtgCapable;
	__TMS_BOOL			fOtgNotificationCapable;
	__TMS_BOOL			fIdPinState;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pHcdDevice,						\
	a_pPort,						\
	a_portIndex						\
	)							\
    do	{							\
	(a_pArg)->pHcdDevice = (a_pHcdDevice);			\
	(a_pArg)->pPort = (a_pPort);				\
	(a_pArg)->portIndex = (a_portIndex);			\
	(a_pArg)->fOtgCapable = __TMS_FALSE;			\
	(a_pArg)->fOtgNotificationCapable = __TMS_FALSE;	\
	(a_pArg)->fIdPinState = __TMS_FALSE;			\
	} while (0)

#define	__TMS_USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcdDevice,						\
	a_portIndex						\
	)							\
	__TMS_USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V2(	\
		a_pArg,						\
		a_pHcdDevice,					\
		/* V2: pPort */ __TMS_NULL,			\
		a_portIndex					\
		)

/*

IOCTL:	USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP

Index:	Type:	USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG
	Name:	USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG_SETUP_V1()

Function:
	Obtain the device address assignment bitmap for the specified HCD.

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest.  This is passed to facilitate
		filtering and proxying.

Output:
	UINT8 bAddressBitmap;
		Address assignment bitmap

Description:
	During USBDI initialization, USBDI needs to know HC (Host controller
	chip) limitation for the device address.  HCD should claim this IOCTL
	if HC has limitation of device address.  If not claimed this IOCTL, USBD
	assume HC has no limitation of device address -- valid device address is
	[0 .. 127].

	HCDKIT-based HCDs don't need to worry about this IOCTL; the HCDKIT
	IOCTL dispatcher will process it and hand back the correct result.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success; some other error code
	if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG *pArg,
		USBPUMP_HCD *pHcd
		);

*/

/**** parameters to GET_ADDRESS_BITMAP ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG)
	{
	__TMS_USBPUMP_HCD *	pHcd;			/* IN */
	__TMS_UINT8		bAddressBitmap;		/* OUT */
	};

#define	__TMS_USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->bAddressBitmap = 0x7F;				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC

Index:	Type:	USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG
	Name:	USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG_SETUP_V1()

Function:
	Initialize HCD hardware

Input:
	PUSBPUMP_HCD pHcd;
		The HCD of interest.

Description:
	This IOCTL is sent from the client to initialize HCD hardware, 
	but without attaching to interrupt. 
	The HCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware is already in use (USBPUMP_IOCTL_RESULT_HARDWARE_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(
		PUSBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG pArg,
		PUSBPUMP_HCD pHcd
		);

*/

/**** parameters to HCD_HARDWARE_INIT_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG)
	{
	__TMS_PUSBPUMP_HCD	pHcd;			/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC

Index:	Type:	USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG
	Name:	USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1()

Function:
	De-initialize HCD hardware

Input:
	PUSBPUMP_HCD pHcd;
		The HCD of interest.

Description:
	This IOCTL is sent from the client to de-initialize HCD hardware 
	and detach from interrupt if this hasn't been done. 
	The HCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(
		PUSBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG pArg,
		PUSBPUMP_HCD pHcd
		);

*/

/**** parameters to HCD_HARDWARE_DEINIT_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG)
	{
	__TMS_PUSBPUMP_HCD	pHcd;			/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_START_ASYNC

Index:	Type:	USBPUMP_IOCTL_HCD_START_ASYNC_ARG
	Name:	USBPUMP_IOCTL_HCD_START_ASYNC_ARG_SETUP_V1()

Function:
	Start USB host controller

Input:
	PUSBPUMP_HCD pHcd;
		The HCD of interest.

Description:
	This IOCTL is sent from the client to start USB host 
	controller, and attach to interrupt. 
	The HCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- HCD already started (USBPUMP_IOCTL_RESULT_ALREADY_STARTED)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_START_ASYNC_ARG_SETUP_V1(
		PUSBPUMP_IOCTL_HCD_START_ASYNC_ARG pArg,
		PUSBPUMP_HCD pHcd
		);

*/

/**** parameters to HCD_START_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_START_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_START_ASYNC_ARG)
	{
	__TMS_PUSBPUMP_HCD	pHcd;			/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_HCD_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcd							\
	)							\
    do	{							\
	(a_pArg)->pHcd = (a_pHcd);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_STOP_ASYNC

Index:	Type:	USBPUMP_IOCTL_HCD_STOP_ASYNC_ARG
	Name:	USBPUMP_IOCTL_HCD_STOP_ASYNC_ARG_SETUP_V1()

Function:
	Stop USB host controller

Input:
	PUSBPUMP_HCD pHcd;
		The HCD of interest.

Description:
	This IOCTL is sent from the client to stop USB host 
	controller, and detach from interrupt. 
	The HCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN 
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or
	
	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- HCD already stopped (USBPUMP_IOCTL_RESULT_ALREADY_STOPPED)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_STOP_ASYNC_ARG_SETUP_V1(
		PUSBPUMP_IOCTL_HCD_STOP_ASYNC_ARG pArg,
		PUSBPUMP_HCD pHcd
		);

*/

/**** parameters to HCD_STOP_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_STOP_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_STOP_ASYNC_ARG)
	{
	__TMS_PUSBPUMP_HCD	pHcd;			/* IN */
	};

#define	__TMS_USBPUMP_IOCTL_HCD_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcd							\
	)							\
    do	{							\
	(a_pArg)->pHcd = (a_pHcd);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER

Index:	Type:	USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG
	Name:	USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG_SETUP_V1()

Function:
	Obtain the normalized root port number for the specified root port
	number.

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest.  This is passed to facilitate
		filtering and proxying.

	UINT8 bRootHubPort;
		Root hub port number.

Output:
	UINT8 bNormalizedPort;
		Normalized root hub port number.

Description:
	A USB3 hub arrives as two devices: a super-speed hub and a high-speed
	hub. When the DataPump generates a device arrival notification for the
	super-speed portion of a superspeed hub, it is supposed to use the
	"port number" of the hub in the first position of the
	NORMALIZED_ROUTE_STRING. Similarly, for the notification for the
	high-speed portion of a USB 3.0 hub, the DataPump is supposed to
	provide the "port number" of the hub.

	USBDI needs to know normalized root hub port number. USBDI will
	call this IOCTL to get the normalized root hub port number.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success; some other error code
	if something goes wrong.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG *pArg,
		USBPUMP_HCD *pHcd,
		UINT8 bRootHubPort
		);

*/

/**** parameters to NORMALIZE_ROOT_HUB_PORT_NUMBER ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG)
	{
	__TMS_USBPUMP_HCD *	pHcd;
	__TMS_UINT8		bRootHubPort;
	__TMS_UINT8		bNormalizedPort;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG_SETUP_V1( \
	a_pArg,								\
	a_pHcd,								\
	a_bRootHubPort							\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->bRootHubPort = (a_bRootHubPort);			\
	(a_pArg)->bNormalizedPort = (a_pArg)->bRootHubPort;		\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_OPEN_RAW_API

Index:	Type:	USBPUMP_IOCTL_HCD_OPEN_RAW_API_ARG
	Name:	USBPUMP_IOCTL_HCD_OPEN_RAW_API_ARG_SETUP_V1

Function:
	IOCTL issued by OS driver clients to open HCD raw API connection.

Input:
	pInParam	not used, should be NULL.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_HCD_OPEN_RAW_API_ARG structure.

	USBPUMP_IOCTL_HCD_OPEN_RAW_API_ARG has the following elements:

	USBPUMP_HCD_RAW_API_HANDLE hRawApiHandle;
		OUT: handle of raw api.

	CONST USBPUMP_HCD_RAW_API_SWITCH *pRawApiSwitch;
		OUT: pointer of HCD raw API function switch.

Description:	
	This IOCTL is sent from an OS-specific driver to a USBPUMP_HCD object,
	to open HCD raw API.

Setup Macro:
	No setup macro required.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_HCD_OPEN_RAW_API_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_HCD_OPEN_RAW_API_ARG)
	{
	__TMS_USBPUMP_HCD_RAW_API_HANDLE		hRawApiHandle;
	__TMS_CONST __TMS_USBPUMP_HCD_RAW_API_SWITCH *	pRawApiSwitch;
	};

/*

IOCTL:	USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC

Index:	Type:	USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG
	Name:	USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1()

Function:
	Change power state of USB host controller

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest.

	USBPUMP_USBDI_POWER_STATE OldPowerState;
		Old power state.

	USBPUMP_USBDI_POWER_STATE NewPowerState;
		New power state.

	USBPUMP_USBDI_POWER_ACTION PowerAction;
		Power action.

	BOOL fWakeEnabled;
		TRUE if remote wakeup is enabled.

Description:
	This IOCTL is sent from the client to change power state of the
	USB host controller.
	The HCD IOCTL-handler must assign a USBPUMP_IOCTL_QE_CANCEL_FN
	cancel handler to USBPUMP_IOCTL_QE element pCancelFn.

Returns:
	IOCTL result is returned thru asynchronous callback function.
	USBPUMP_IOCTL_RESULT_SUCCESS for success, or

	- IOCTL-handler missing (USBPUMP_IOCTL_RESULT_NOT_CLAIMED)
	- General failure (USBPUMP_IOCTL_RESULT_FAILED)
	- Invalid parameter (USBPUMP_IOCTL_RESULT_INVALID_PARAMETER)	
	- Hardware is not in use (USBPUMP_IOCTL_RESULT_HARDWARE_NOT_INUSE)	
	- Hardware missing (USBPUMP_IOCTL_RESULT_NO_HARDWARE)
	- Hardware config error (USBPUMP_IOCTL_RESULT_HARDWARE_IOERR)
	- Cancelled (USBPUMP_IOCTL_RESULT_CANCEL_ASYNCHRONOUS)

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG *pArg,
		USBPUMP_HCD *pHcd,
		USBPUMP_USBDI_POWER_STATE OldPowerState,
		USBPUMP_USBDI_POWER_STATE NewPowerState,
		BOOL fWakeEnabled
		);

*/

/**** parameters to HCD_POWER_STATE_CHANGE_ASYNC ****/
__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG)
	{
	__TMS_USBPUMP_HCD *			pHcd;		/* IN */
	__TMS_USBPUMP_USBDI_POWER_STATE 	OldPowerState;
	__TMS_USBPUMP_USBDI_POWER_STATE 	NewPowerState;
	__TMS_USBPUMP_USBDI_POWER_ACTION	PowerAction;
	__TMS_BOOL				fWakeEnabled;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pHcd,								\
	a_OldPowerState,						\
	a_NewPowerState,						\
	a_PowerAction,							\
	a_fWakeEnabled							\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->OldPowerState = (a_OldPowerState);			\
	(a_pArg)->NewPowerState = (a_NewPowerState);			\
	(a_pArg)->PowerAction = (a_PowerAction);			\
	(a_pArg)->fWakeEnabled = (a_fWakeEnabled);			\
	} while (0)

#define	__TMS_USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_OldPowerState,						\
	a_NewPowerState,						\
	a_fWakeEnabled							\
	)								\
	__TMS_USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pHcd,								\
	a_OldPowerState,						\
	a_NewPowerState,						\
	/* V2: PowerAction */ __TMS_USBPUMP_USBDI_POWER_ACTION_NONE,	\
	a_fWakeEnabled							\
	)

/*

IOCTL:	USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE

Index:	Type:	USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG
	Name:	USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1()

Function:
	Enable/disable wake alert from roothub

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest. This is passed to facilitate
		filtering and proxying.

	USBPUMP_HCD_DEVICE *pRootHubDevice;
		The HCD_DEVICE for roothub that client want to enable/disable
		wake alert notification. a roothub is connected to a virtual
		rootport and has 1-to-1 mapping with rootport.
		
	USBPUMP_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ALERT_FN *pAlertFn;
		The remote wakeup alert function pointer.  If pAlertFn is not
		NULL, it will enable wake hardware capability. When conceptual
		rootport need to signal remote wakeup, HCD will call pAlertFn.
		If pAlertFn is NULL, it will disable wake hardware capability.

	VOID *pContext;
		The context pointer of alert function.
		
Description:
	This IOCTL is sent from the client to enable or disalbe hardware wakeup
	notification of the roothub.
	
	This is similar to USBPUMP_USBDI_PORT_ENABLE_WAKE_HARDWARE_ALERT_FN
	ioctl for the USBPUMP_USBDI_PORT object. however the root port exist
	virtually inside HCD and HCD cannot look into USBPUMP_USBDI_PORT
	subclass object (like USBPUMP_USBDI_PORT_PRIVATE) to determine which
	root port it should signal notification to (there can exist multiple
	root port).
	
	Instead, there is 1-to-1 mapping between root port and roothub device
	and HCD_DEVICE object is transparent to HCD. Actually roothub
	HCD_DEVICE objects are maintained by HCD. So, HCD_DEVICE argument is
	more accessible and useful than USBPUMP_USBDI_PORT argument to HCD.
	
	HCD IOCTL-handler should make a copy of pAlertFn and pContext for each
	of roothub so that it could signal notification later when any of
	roothub need to signal a wakeup.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if wakeup notification is enabled/disabled
	successfully.
	USBPUMP_IOCTL_RESULT_ALREADY_REGISTER if client trys to enable
	notification when have enabled already.
	
Setup Macro:
	VOID USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG *pArg,
		USBPUMP_HCD *pHcd,
		USBPUMP_HCD_DEVICE *pRootHubDevice,
		USBPUMP_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ALERT_FN *pAlertFn,
		VOID *pContext
		);

*/

__TMS_FNTYPE_DEF(
USBPUMP_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ALERT_FN,
__TMS_VOID,
	(
	__TMS_VOID *			/* pContext */,
	__TMS_USTAT			/* Status */
	));

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG)
	{
	__TMS_USBPUMP_HCD 					*pHcd;
	__TMS_USBPUMP_HCD_DEVICE				*pRootHubDevice;
	__TMS_USBPUMP_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ALERT_FN	*pAlertFn;
	__TMS_VOID						*pContext;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_pAlertFn,							\
	a_pContext							\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->pRootHubDevice = (a_pRootHubDevice);			\
	(a_pArg)->pAlertFn = (a_pAlertFn);				\
	(a_pArg)->pContext = (a_pContext);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM

Index:	Type:	USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG
	Name:	USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG_SETUP_V1()

Function:
	Enable/disable hardware controlled LPM

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest. This is passed to facilitate
		filtering and proxying.

	USBPUMP_HCD_DEVICE *pRootHubDevice;
		The HCD_DEVICE for roothub that client want to enable/disable
		hardware controlled LPM. A roothub is connected to a virtual
		rootport and has 1-to-1 mapping with rootport.

	UINT8 uPort;
		The port number of root hub device.

	UINT8 bLpmMode;
		Specify hardware controlled LPM mode.

	UINT8 bSlotId;
		Target device slot ID.

	UINT HirdInMicroseconds;
		HIRD (Host Initiated Resume Duration) in microseconds for LPM

	UINT IdleInMicroseconds;
		Hardware controlled LPM mode idle time in microseconds.

	BOOL fEnableRemoteWakeup;
		Enable or disable remote wakeup.

	BOOL fEnableLpm;
		Enable or disable LPM. If the fEnableLPM is TRUE, enable
		hardware controlled LPM.

Description:
	This IOCTL is sent from the USBD to HCD to enable/disable hardware
	controlled LPM for the roothub.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if HCD enable/disable hardware controlled
	LPM function successfully.
	USBPUMP_IOCTL_RESULT_HARDWARE_NOT_SUPPORT if HCD doesn't support it.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG *pArg,
		USBPUMP_HCD *pHcd,
		USBPUMP_HCD_DEVICE *pRootHubDevice,
		UINT8 uPort,
		UINT8 bLpmMode,
		UINT8 bSlotId,
		UINT HirdInMicroseconds,
		UINT IdleInMicroseconds,
		BOOL fEnableRemoteWakeup,
		BOOL fEnableLpm
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG)
	{
	__TMS_USBPUMP_HCD *		pHcd;
	__TMS_USBPUMP_HCD_DEVICE *	pRootHubDevice;
	__TMS_UINT8			uPort;
	__TMS_UINT8			bLpmMode;
	__TMS_UINT8			bSlotId;
	__TMS_UINT			HirdInMicroseconds;
	__TMS_UINT			IdleInMicroseconds;
	__TMS_BOOL			fEnableRemoteWakeup;
	__TMS_BOOL			fEnableLpm;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_uPort,							\
	a_bLpmMode,							\
	a_bSlotId,							\
	a_HirdInMicroseconds,						\
	a_IdleInMicroseconds,						\
	a_fEnableRemoteWakeup,						\
	a_fEnableLpm							\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->pRootHubDevice = (a_pRootHubDevice);			\
	(a_pArg)->uPort = (a_uPort);					\
	(a_pArg)->bLpmMode = (a_bLpmMode);				\
	(a_pArg)->bSlotId = (a_bSlotId);				\
	(a_pArg)->HirdInMicroseconds = (a_HirdInMicroseconds);		\
	(a_pArg)->IdleInMicroseconds = (a_IdleInMicroseconds);		\
	(a_pArg)->fEnableRemoteWakeup = (a_fEnableRemoteWakeup);	\
	(a_pArg)->fEnableLpm = (a_fEnableLpm);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO

Index:	Type:	USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG
	Name:	USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG_SETUP_V1()

Function:
	IOCTL issued by OS driver clients to get HCD compatible port
	information.

Input:
	pInParam	not used, should be NULL.

Output:
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG
			structure.

	USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG has the following 
	elements:

	UINT8	StartPortUsb3;
		the starting port number of USB3 Root Hubt Ports.

	UINT8	PortCountUsb3;
		the number of consecutive USB3 Root Hubt Ports.

	UINT8	StartPortUsb2;
		the starting port number of USB2 Root Hubt Ports.

	UINT8	PortCountUsb2;
		the number of consecutive USB2 Root Hubt Ports.

Description:
	This IOCTL is sent from an OS-specific driver to a USBPUMP_HCD object,
	to get HCD compatible port information.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG *pArg,
		UINT8	StartPortUsb3,
		UINT8	PortCountUsb3,
		UINT8	StartPortUsb2,
		UINT8	PortCountUsb2
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG)
	{
	__TMS_UINT8	StartPortUsb3;
	__TMS_UINT8	PortCountUsb3;
	__TMS_UINT8	StartPortUsb2;
	__TMS_UINT8	PortCountUsb2;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG_SETUP_V1(	\
	a_pArg,								\
	a_StartPortUsb3,						\
	a_PortCountUsb3,						\
	a_StartPortUsb2,						\
	a_PortCountUsb2							\
	)								\
    do	{								\
	(a_pArg)->StartPortUsb3 = (a_StartPortUsb3);			\
	(a_pArg)->PortCountUsb3 = (a_PortCountUsb3);			\
	(a_pArg)->StartPortUsb2 = (a_StartPortUsb2);			\
	(a_pArg)->PortCountUsb2 = (a_PortCountUsb2);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE

Index:	Type:	USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG
	Name:	USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG_SETUP_V1()

Function:
	Set/clear force full speed mode of the root hub port

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest. This is passed to facilitate
		filtering and proxying.

	UINT PortNumber;
		Root hub port number.

	BOOL fForceFullSpeedMode;
		Enable or disable force full speed mode of the root hub port.

Description:
	This IOCTL is sent from the USBD to HCD to enable/disable force full
	speed mode of the root hub port.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if HCD enable/disable force full speed
	mode of the host controller function successfully.
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if HCD doesn't support it.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG *pArg,
		USBPUMP_HCD *pHcd,
		UINT PortNumber,
		BOOL fForceFullSpeedMode
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG)
	{
	__TMS_USBPUMP_HCD *		pHcd;
	__TMS_UINT			PortNumber;
	__TMS_BOOL			fForceFullSpeedMode;
	};

#define	__TMS_USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_PortNumber,							\
	a_fForceFullSpeedMode						\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->PortNumber = (a_PortNumber);				\
	(a_pArg)->fForceFullSpeedMode = (a_fForceFullSpeedMode);	\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED

Index:	Type:	USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG
	Name:	USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG_SETUP_V1()

Function:
	Send hub device connection information

Input:
	USBPUMP_HCD *pHcd;
		The HCD of interest. This is passed to facilitate
		filtering and proxying.

	USBPUMP_USBDI_PORT *pPort;
		The USBD port structure of the attached hub device. The HCD
		can use this information.

	USBPUMP_HCD_PIPE *pHcdPipe;
		The HCD pipe structure of the default pipe of the
		attached hub device. The HCD can use this information.

	USBIF_DEVDESC_WIRE *pDevDesc;
		Device descriptor of the hub device.

	USBIF_HUBDESC_WIRE *pHubDesc;
		Hub descriptor of the hub device.

	UINT Tier;
		The tier of this port.

Output:
	BOOL fNeedTimerForUnIdlePort;
		Set this flag if requires timer when unidle port.

Description:
	This IOCTL is sent from the USBD or client to HCD to notify hub device
	connection.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if HCD process hub device information.
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if HCD doesn't support it.

Setup Macro:
	VOID USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG_SETUP_V1(
		USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG *pArg,
		USBPUMP_HCD *		pHcd,
		USBPUMP_USBDI_PORT *	pPort,
		USBPUMP_HCD_PIPE *	pHcdPipe,
		USBIF_DEVDESC_WIRE *	pDevDesc,
		USBIF_HUBDESC_WIRE *	pHubDesc,
		UINT			Tier
		);

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG)
	{
	__TMS_USBPUMP_HCD *		pHcd;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_HCD_PIPE *	pHcdPipe;
	__TMS_USBIF_DEVDESC_WIRE *	pDevDesc;
	USBIF_HUBDESC_WIRE *		pHubDesc;
	__TMS_UINT			Tier;
	__TMS_BOOL			fNeedTimerForUnIdlePort; /* Output */
	};

#define	__TMS_USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pPort,							\
	a_pHcdPipe,							\
	a_pDevDesc,							\
	a_pHubDesc,							\
	a_Tier								\
	)								\
    do	{								\
	(a_pArg)->pHcd = (a_pHcd);					\
	(a_pArg)->pPort = (a_pPort);					\
	(a_pArg)->pHcdPipe = (a_pHcdPipe);				\
	(a_pArg)->pDevDesc = (a_pDevDesc);				\
	(a_pArg)->pHubDesc = (a_pHubDesc);				\
	(a_pArg)->Tier = (a_Tier);					\
	(a_pArg)->fNeedTimerForUnIdlePort = __TMS_FALSE;		\
	} while (0)

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE_HCD(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_EDGE_HCD(i, t, NAME)
# define USBPUMP_IOCTL_HCD(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_HCD(i, t, NAME)
# define USBPUMP_IOCTL_HCD_GET_PORT_COUNT	\
   __TMS_USBPUMP_IOCTL_HCD_GET_PORT_COUNT
# define USBPUMP_IOCTL_HCD_CHECK_B_CONN	\
   __TMS_USBPUMP_IOCTL_HCD_CHECK_B_CONN
# define USBPUMP_IOCTL_HCD_SET_PORT_FEATURE	\
   __TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE
# define USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS	\
   __TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS
# define USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG	\
   __TMS_USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG
# define USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE	\
   __TMS_USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE
# define USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP	\
   __TMS_USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP
# define USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC	\
   __TMS_USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC
# define USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC	\
   __TMS_USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC
# define USBPUMP_IOCTL_HCD_START_ASYNC	\
   __TMS_USBPUMP_IOCTL_HCD_START_ASYNC
# define USBPUMP_IOCTL_HCD_STOP_ASYNC	\
   __TMS_USBPUMP_IOCTL_HCD_STOP_ASYNC
# define USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER	\
   __TMS_USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER
# define USBPUMP_IOCTL_HCD_OPEN_RAW_API	\
   __TMS_USBPUMP_IOCTL_HCD_OPEN_RAW_API
# define USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC	\
   __TMS_USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC
# define USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE	\
   __TMS_USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE
# define USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM	\
   __TMS_USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM
# define USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO	\
   __TMS_USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO
# define USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE	\
   __TMS_USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE
# define USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED	\
   __TMS_USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED
# define USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd								\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_PORT_COUNT_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd								\
	)
# define USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort								\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_CHECK_B_CONN_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort								\
	)
# define USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V3(		\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_iPort,							\
	a_fSet,								\
	a_Feature,							\
	a_Selector							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V3(		\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_iPort,							\
	a_fSet,								\
	a_Feature,							\
	a_Selector							\
	)
# define USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort,							\
	a_fSet,								\
	a_Feature,							\
	a_Selector							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V2(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort,							\
	a_fSet,								\
	a_Feature,							\
	a_Selector							\
	)
# define USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort,							\
	a_fSet,								\
	a_Feature							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_SET_PORT_FEATURE_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pHcd,								\
	a_iPort,							\
	a_fSet,								\
	a_Feature							\
	)
# define USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)
# define USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V2(	\
	a_pArg,								\
	a_nLevels,							\
	a_nTTLevels,							\
	a_LowSpeedHostDelay,						\
	a_FullSpeedHostDelay,						\
	a_HighSpeedHostDelay,						\
	a_MaxPeriodicClocks,						\
	a_LowSpeedHubClocks						\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V2(	\
	a_pArg,								\
	a_nLevels,							\
	a_nTTLevels,							\
	a_LowSpeedHostDelay,						\
	a_FullSpeedHostDelay,						\
	a_HighSpeedHostDelay,						\
	a_MaxPeriodicClocks,						\
	a_LowSpeedHubClocks						\
	)
# define USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V1(	\
	a_pArg,								\
	a_nLevels,							\
	a_LowSpeedHostDelay,						\
	a_FullSpeedHostDelay,						\
	a_HighSpeedHostDelay,						\
	a_MaxPeriodicClocks,						\
	a_LowSpeedHubClocks						\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_SCHEDULE_PARAMS_RESULT_SETUP_V1(	\
	a_pArg,								\
	a_nLevels,							\
	a_LowSpeedHostDelay,						\
	a_FullSpeedHostDelay,						\
	a_HighSpeedHostDelay,						\
	a_MaxPeriodicClocks,						\
	a_LowSpeedHubClocks						\
	)
# define USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG_SETUP_V1(\
	a_pArg,								\
	a_pHcd,								\
	a_portIndex,							\
	a_fReset							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_ROOT_PORT_HANDOFF_FLAG_ARG_SETUP_V1(\
	a_pArg,								\
	a_pHcd,								\
	a_portIndex,							\
	a_fReset							\
	)
# define USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pHcdDevice,						\
	a_pPort,						\
	a_portIndex						\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V2(	\
	a_pArg,							\
	a_pHcdDevice,						\
	a_pPort,						\
	a_portIndex						\
	)
# define USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcdDevice,						\
	a_portIndex						\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_OTG_CAPABLE_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcdDevice,						\
	a_portIndex						\
	)
# define USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_ADDRESS_BITMAP_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)
# define USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_HARDWARE_INIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)
# define USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_HARDWARE_DEINIT_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd								\
	)
# define USBPUMP_IOCTL_HCD_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcd							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcd							\
	)
# define USBPUMP_IOCTL_HCD_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcd							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pHcd							\
	)
# define USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG_SETUP_V1( \
	a_pArg,								\
	a_pHcd,								\
	a_bRootHubPort							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_NORMALIZE_ROOT_HUB_PORT_NUMBER_ARG_SETUP_V1( \
	a_pArg,								\
	a_pHcd,								\
	a_bRootHubPort							\
	)
# define USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pHcd,								\
	a_OldPowerState,						\
	a_NewPowerState,						\
	a_PowerAction,							\
	a_fWakeEnabled							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V2(	\
	a_pArg,								\
	a_pHcd,								\
	a_OldPowerState,						\
	a_NewPowerState,						\
	a_PowerAction,							\
	a_fWakeEnabled							\
	)
# define USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_OldPowerState,						\
	a_NewPowerState,						\
	a_fWakeEnabled							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_POWER_STATE_CHANGE_ASYNC_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_OldPowerState,						\
	a_NewPowerState,						\
	a_fWakeEnabled							\
	)
# define USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_pAlertFn,							\
	a_pContext							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_ROOTHUB_ENABLE_WAKE_HARDWARE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_pAlertFn,							\
	a_pContext							\
	)
# define USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_uPort,							\
	a_bLpmMode,							\
	a_bSlotId,							\
	a_HirdInMicroseconds,						\
	a_IdleInMicroseconds,						\
	a_fEnableRemoteWakeup,						\
	a_fEnableLpm							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_ENABLE_HARDWARE_LPM_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pRootHubDevice,						\
	a_uPort,							\
	a_bLpmMode,							\
	a_bSlotId,							\
	a_HirdInMicroseconds,						\
	a_IdleInMicroseconds,						\
	a_fEnableRemoteWakeup,						\
	a_fEnableLpm							\
	)
# define USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG_SETUP_V1(	\
	a_pArg,								\
	a_StartPortUsb3,						\
	a_PortCountUsb3,						\
	a_StartPortUsb2,						\
	a_PortCountUsb2							\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_GET_COMPATIBLE_PORT_INFO_ARG_SETUP_V1(	\
	a_pArg,								\
	a_StartPortUsb3,						\
	a_PortCountUsb3,						\
	a_StartPortUsb2,						\
	a_PortCountUsb2							\
	)
# define USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_PortNumber,							\
	a_fForceFullSpeedMode						\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_SET_FORCE_FULL_SPEED_MODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_PortNumber,							\
	a_fForceFullSpeedMode						\
	)
# define USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pPort,							\
	a_pHcdPipe,							\
	a_pDevDesc,							\
	a_pHubDesc,							\
	a_Tier								\
	)	\
	__TMS_USBPUMP_IOCTL_HCD_HUB_DEVICE_CONNECTED_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pHcd,								\
	a_pPort,							\
	a_pHcdPipe,							\
	a_pDevDesc,							\
	a_pHubDesc,							\
	a_Tier								\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcd_ioctls.h ****/
#endif /* _USBPUMP_HCD_IOCTLS_H_ */
