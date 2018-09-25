/* usbpump_usbphy_types.h	Tue Dec 14 2010 11:00:27 skpark */

/*

Module:  usbpump_usbphy_types.h

Function:
	Definition of USBPHY types

Version:
	V3.01d	Tue Dec 14 2010 11:00:27 skpark	Edit level 2

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2009

Revision history:
   2.01a  Tue Nov 03 2009 12:13:11 chwon
	9234: Module created.

   3.01d  Tue Dec 14 2010 11:00:27  skpark
	11667: Add USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE

*/

#ifndef _USBPUMP_USBPHY_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_USBPHY_TYPES_H_

/*
|| we need the base DataPump types
*/
#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	USBPHY common types
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION	(USBPUMP_USBPHY);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_PORT_STATUS);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBPHY_CONFIG_INFO);

__TMS_TYPE_DEF_ARG	(USBPUMP_USBPHY_STATE, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBPHY_HCD_EVENT, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBPHY_DCD_EVENT, UINT32);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBPHY_HCD_ABUS_REQUEST_MODE, UINT32);

__TMS_FNTYPE_DEF(
USBPUMP_USBPHY_HCD_EVENT_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pHcdObject */,
	__TMS_VOID *			/* pHcdContext */,
	__TMS_USBPUMP_USBPHY_HCD_EVENT	/* HcdEventCode */,
	__TMS_VOID *			/* pHcdEventInfo */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBPHY_HCD_ISR_FN,
__TMS_BOOL,
	(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pHcdObject */,
	__TMS_ADDRBITS_PTR_UNION	/* InterruptStatusInfo */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBPHY_DCD_EVENT_FN,
__TMS_VOID,
	(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pDcdObject */,
	__TMS_VOID *			/* pDcdContext */,
	__TMS_USBPUMP_USBPHY_DCD_EVENT	/* DcdEventCode */,
	__TMS_VOID *			/* pDcdEventInfo */
	));

__TMS_FNTYPE_DEF(
USBPUMP_USBPHY_DCD_ISR_FN,
__TMS_BOOL,
	(
	__TMS_USBPUMP_OBJECT_HEADER *	/* pDcdObject */,
	__TMS_ADDRBITS_PTR_UNION	/* InterruptStatusInfo */
	));


/****************************************************************************\
|
|	PHY initialization function definition
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
USBPUMP_USBPHY_INIT_FN,
__TMS_USBPUMP_USBPHY *,
	(
	__TMS_UPLATFORM *				/* pPlatform */,
	__TMS_CONST __TMS_USBPUMP_USBPHY_CONFIG_INFO *	/* pPhyConfig */,
	__TMS_CONST __TMS_VOID *			/* pPrivateConfig */
	));


/**** end of usbpump_usbphy_types.h ****/
#endif /* _USBPUMP_USBPHY_TYPES_H_ */
