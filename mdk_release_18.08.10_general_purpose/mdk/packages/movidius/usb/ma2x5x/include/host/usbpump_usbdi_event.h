/* usbpump_usbdi_event.h	Wed Mar 25 2015 15:20:29 chwon */

/*

Module:  usbpump_usbdi_event.h

Function:
	Definition of USBPUMP_USBDI_EVENT and USBPUMP_USBDI_EVENT_NODE

Version:
	V3.15b	Wed Mar 25 2015 15:20:29 chwon	Edit level 6

Copyright notice:
	This file copyright (C) 2008-2011, 2014-2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	January 2008

Revision history:
   1.97k  Thu Jan 17 2008 10:45:23 chwon
	3567: Module created.

   1.97k  Fri Jan 30 2009 14:31:49  skpark
	7311: Added device ID string in the USBPUMP_USBDI_EVENT_DEVICE_MATCHED
	and USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED notification

   3.01d  Mon Nov 01 2010 11:21:54  chwon
	11668: add USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE notification.

   3.01d  Tue Jan 18 2011 11:31:01  cccho
	12222: add pParentHubPort and ulPortIndex in ..._OVER_CURRENT_INFO and
	add ..._OVER_CURRENT_INFO_INIT_V2 and ..._OVER_CURRENT_INFO_SETUP_V2
	macro.

   3.13a  Fri Jul 11 2014 13:14:56  chwon
	18367: add USBPUMP_USBDI_EVENT_HUB_CONFIGURED notification.

   3.15b  Wed Mar 25 2015 15:20:29  chwon
	19094: Add USBPUMP_USBDI_EVENT_HNP_SUCCESS/FAILED/FINISHED events.

*/

#ifndef _USBPUMP_USBDI_EVENT_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_EVENT_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMP_ANNUNCIATOR_H_
# include "usbpump_annunciator.h"
#endif

/****************************************************************************\
|
|	USBD Annunciator notification event definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED		0
#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED		1
#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_DISABLED		2
#define	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT		3
#define	__TMS_USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER		4
#define	__TMS_USBPUMP_USBDI_EVENT_HUB_NO_POWER			5
#define	__TMS_USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT		6
#define	__TMS_USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER		7
#define	__TMS_USBPUMP_USBDI_EVENT_TREE_TOO_DEEP			8
#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED		9
#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED		10
#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_COMPLETE		11
#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_VANISHED		12
#define	__TMS_USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE		13
#define	__TMS_USBPUMP_USBDI_EVENT_TIMEOUT			14
#define	__TMS_USBPUMP_USBDI_EVENT_INTERNAL_FAILURE		15
#define	__TMS_USBPUMP_USBDI_EVENT_CHANGE_POWER			16
#define	__TMS_USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE		17
#define	__TMS_USBPUMP_USBDI_EVENT_HUB_CONFIGURED		18
#define	__TMS_USBPUMP_USBDI_EVENT_HNP_SUCCESS			19
#define	__TMS_USBPUMP_USBDI_EVENT_HNP_FAILED			20
#define	__TMS_USBPUMP_USBDI_EVENT_HNP_FINISHED			21
#define	__TMS_USBPUMP_USBDI_EVENT__MAX				22


/*
|| When you add names to the above list, please also update the following
|| debugging table.
*/
#define	__TMS_USBPUMP_USBDI_EVENT_NAMES__INIT		\
	{						\
	"USBPUMP_USBDI_EVENT_DEVICE_MATCHED",		\
	"USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED",	\
	"USBPUMP_USBDI_EVENT_DEVICE_DISABLED",		\
	"USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT",	\
	"USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER",	\
	"USBPUMP_USBDI_EVENT_HUB_NO_POWER",		\
	"USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT",		\
	"USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER",		\
	"USBPUMP_USBDI_EVENT_TREE_TOO_DEEP",		\
	"USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED",		\
	"USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED",	\
	"USBPUMP_USBDI_EVENT_DRIVER_COMPLETE",		\
	"USBPUMP_USBDI_EVENT_DEVICE_VANISHED",		\
	"USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE",	\
	"USBPUMP_USBDI_EVENT_TIMEOUT",			\
	"USBPUMP_USBDI_EVENT_INTERNAL_FAILURE",		\
	"USBPUMP_USBDI_EVENT_CHANGE_POWER",		\
	"USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE",	\
	"USBPUMP_USBDI_EVENT_HUB_CONFIGURED",		\
	"USBPUMP_USBDI_EVENT_HNP_SUCCESS",		\
	"USBPUMP_USBDI_EVENT_HNP_FAILED",		\
	"USBPUMP_USBDI_EVENT_HNP_FINISHED"		\
	}


/****************************************************************************\
|
|	USBD Annunciator extra information structure definition
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	__TMS_BOOL			fHnpCapable;
	__TMS_CONST __TMS_TEXT *	pDeviceIdString;
	__TMS_BYTES			sizeDeviceIdString;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_INIT_V2(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_SETUP_V2(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	(a_pInfo)->fHnpCapable = a_fHnpCapable;				\
	(a_pInfo)->pDeviceIdString = a_pDeviceIdString;			\
	(a_pInfo)->sizeDeviceIdString = a_sizeDeviceIdString;		\
	} while (0)

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)								\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_INIT_V2(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	/* V2: pDeviceIdString */ __TMS_NULL,				\
	/* V2: sizeDeviceIdString */ 0					\
	)

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)								\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_SETUP_V2(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	/* V2: pDeviceIdString */ __TMS_NULL,				\
	/* V2: sizeDeviceIdString */ 0					\
	)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	__TMS_BOOL			fHnpCapable;
	__TMS_CONST __TMS_TEXT *	pDeviceIdString;
	__TMS_BYTES			sizeDeviceIdString;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_INIT_V2(	\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)								\
	{								\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_SETUP_V2(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)								\
    do	{								\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	(a_pInfo)->fHnpCapable = a_fHnpCapable;				\
	(a_pInfo)->pDeviceIdString = a_pDeviceIdString;			\
	(a_pInfo)->sizeDeviceIdString = a_sizeDeviceIdString;		\
	} while (0)

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)								\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_INIT_V2(	\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	/* V2: pDeviceIdString */ __TMS_NULL,				\
	/* V2: sizeDeviceIdString */ 0					\
	)

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)								\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_SETUP_V2(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	/* V2: pDeviceIdString */ __TMS_NULL,				\
	/* V2: sizeDeviceIdString */ 0					\
	)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_DEVICE_DISABLED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_DEVICE_DISABLED_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_DISABLED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_DISABLED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	__TMS_USBPUMP_USBDI_PORT *	pParentHubPort;
	__TMS_UINT			ulPortIndex;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_INIT_V2(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	__TMS_NULL,							\
	0								\
	)

#define	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_INIT_V2(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_pParentHubPort,						\
	a_ulPortIndex							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_pParentHubPort,						\
	a_ulPortIndex							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_SETUP_V2(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	__TMS_NULL,							\
	0								\
	)

#define	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_SETUP_V2(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_pParentHubPort,						\
	a_ulPortIndex							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	(a_pInfo)->pParentHubPort = a_pParentHubPort;			\
	(a_pInfo)->ulPortIndex = a_ulPortIndex;				\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	__TMS_BOOL			fDoubleBusPower;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fDoubleBusPower						\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fDoubleBusPower						\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fDoubleBusPower						\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	(a_pInfo)->fDoubleBusPower = a_fDoubleBusPower;			\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_HUB_NO_POWER_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_HUB_NO_POWER_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_NO_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_NO_POWER_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	__TMS_BOOL			fOverCurrent;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fOverCurrent							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fOverCurrent							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fOverCurrent							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	(a_pInfo)->fOverCurrent = a_fOverCurrent;			\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	__TMS_BOOL			fLocalPower;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fLocalPower							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fLocalPower							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fLocalPower							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	(a_pInfo)->fLocalPower = a_fLocalPower;				\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_TREE_TOO_DEEP_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_TREE_TOO_DEEP_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_TREE_TOO_DEEP_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_TREE_TOO_DEEP_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_DRIVER_COMPLETE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_DRIVER_COMPLETE_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_COMPLETE_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort								\
	)								\
	{								\
	a_pFunction,							\
	a_pPort								\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_DRIVER_COMPLETE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort								\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_DEVICE_VANISHED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_DEVICE_VANISHED_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_VANISHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_DEVICE_VANISHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE_INFO)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_TIMEOUT_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_TIMEOUT_INFO)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_TIMEOUT_INFO_INIT_V1(			\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_TIMEOUT_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_INTERNAL_FAILURE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_INTERNAL_FAILURE_INFO)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_INTERNAL_FAILURE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_INTERNAL_FAILURE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_CHANGE_POWER_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_CHANGE_POWER_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	/* the max power available to this port */
	__TMS_UINT16			wMaxPower;
	__TMS_UINT16			wPowerInUse;	/* mA */
	};

#define	__TMS_USBPUMP_USBDI_EVENT_CHANGE_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_wMaxPower,							\
	a_wPowerInUse							\
	)								\
	{								\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_wMaxPower,							\
	a_wPowerInUse							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_CHANGE_POWER_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_wMaxPower,							\
	a_wPowerInUse							\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	(a_pInfo)->wMaxPower = a_wMaxPower;				\
	(a_pInfo)->wPowerInUse = a_wPowerInUse;				\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE_INFO)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	__TMS_USBPUMP_USBDI_PORT_KEY	PortKey;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)								\
	{								\
	a_pPort,							\
	a_PortKey							\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)								\
    do	{								\
	(a_pInfo)->pPort = a_pPort;					\
	(a_pInfo)->PortKey = a_PortKey;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_HUB_CONFIGURED_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_HUB_CONFIGURED_INFO)
	{
	__TMS_USBPUMP_USBDI_FUNCTION *	pFunction;
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_CONFIGURED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort								\
	)								\
	{								\
	a_pFunction,							\
	a_pPort								\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_HUB_CONFIGURED_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort								\
	)								\
    do	{								\
	(a_pInfo)->pFunction = a_pFunction;				\
	(a_pInfo)->pPort = a_pPort;					\
	} while (0)


__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_EVENT_HNP_INFO);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_EVENT_HNP_INFO)
	{
	__TMS_USBPUMP_USBDI_PORT *	pPort;
	};

#define	__TMS_USBPUMP_USBDI_EVENT_HNP_INFO_INIT_V1(			\
	a_pPort								\
	)								\
	{								\
	a_pPort								\
	}

#define	__TMS_USBPUMP_USBDI_EVENT_HNP_INFO_SETUP_V1(			\
	a_pInfo,							\
	a_pPort								\
	)								\
    do	{								\
	(a_pInfo)->pPort = a_pPort;					\
	} while (0)


/****************************************************************************\
|
|	Some useful APIs (mostly for debugging)
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_CONST __TMS_TEXT *
UsbPumpUsbd_EventName(
	__TMS_USBPUMP_ANNUNCIATOR_NOTIFICATION	/* EventCode */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_EVENT_DEVICE_MATCHED	\
   __TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED
# define USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED	\
   __TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED
# define USBPUMP_USBDI_EVENT_DEVICE_DISABLED	\
   __TMS_USBPUMP_USBDI_EVENT_DEVICE_DISABLED
# define USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT	\
   __TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT
# define USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER	\
   __TMS_USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER
# define USBPUMP_USBDI_EVENT_HUB_NO_POWER	\
   __TMS_USBPUMP_USBDI_EVENT_HUB_NO_POWER
# define USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT	\
   __TMS_USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT
# define USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER	\
   __TMS_USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER
# define USBPUMP_USBDI_EVENT_TREE_TOO_DEEP	\
   __TMS_USBPUMP_USBDI_EVENT_TREE_TOO_DEEP
# define USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED	\
   __TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED
# define USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED	\
   __TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED
# define USBPUMP_USBDI_EVENT_DRIVER_COMPLETE	\
   __TMS_USBPUMP_USBDI_EVENT_DRIVER_COMPLETE
# define USBPUMP_USBDI_EVENT_DEVICE_VANISHED	\
   __TMS_USBPUMP_USBDI_EVENT_DEVICE_VANISHED
# define USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE	\
   __TMS_USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE
# define USBPUMP_USBDI_EVENT_TIMEOUT	\
   __TMS_USBPUMP_USBDI_EVENT_TIMEOUT
# define USBPUMP_USBDI_EVENT_INTERNAL_FAILURE	\
   __TMS_USBPUMP_USBDI_EVENT_INTERNAL_FAILURE
# define USBPUMP_USBDI_EVENT_CHANGE_POWER	\
   __TMS_USBPUMP_USBDI_EVENT_CHANGE_POWER
# define USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE	\
   __TMS_USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE
# define USBPUMP_USBDI_EVENT_HUB_CONFIGURED	\
   __TMS_USBPUMP_USBDI_EVENT_HUB_CONFIGURED
# define USBPUMP_USBDI_EVENT_HNP_SUCCESS	\
   __TMS_USBPUMP_USBDI_EVENT_HNP_SUCCESS
# define USBPUMP_USBDI_EVENT_HNP_FAILED	\
   __TMS_USBPUMP_USBDI_EVENT_HNP_FAILED
# define USBPUMP_USBDI_EVENT_HNP_FINISHED	\
   __TMS_USBPUMP_USBDI_EVENT_HNP_FINISHED
# define USBPUMP_USBDI_EVENT__MAX	\
   __TMS_USBPUMP_USBDI_EVENT__MAX
# define USBPUMP_USBDI_EVENT_NAMES__INIT	\
   __TMS_USBPUMP_USBDI_EVENT_NAMES__INIT
# define USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_INIT_V2(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_INIT_V2(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_SETUP_V2(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_SETUP_V2(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_MATCHED_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_INIT_V2(	\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_INIT_V2(	\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_SETUP_V2(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_SETUP_V2(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable,							\
	a_pDeviceIdString,						\
	a_sizeDeviceIdString						\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_NOT_MATCHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey,							\
	a_fHnpCapable							\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_DISABLED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_DISABLED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_DISABLED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_DISABLED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_INIT_V2(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_pParentHubPort,						\
	a_ulPortIndex							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_INIT_V2(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_pParentHubPort,						\
	a_ulPortIndex							\
	)
# define USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_SETUP_V2(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_pParentHubPort,						\
	a_ulPortIndex							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_PORT_OVER_CURRENT_INFO_SETUP_V2(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_pParentHubPort,						\
	a_ulPortIndex							\
	)
# define USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fDoubleBusPower						\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fDoubleBusPower						\
	)
# define USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fDoubleBusPower						\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_DOUBLE_BUS_POWER_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fDoubleBusPower						\
	)
# define USBPUMP_USBDI_EVENT_HUB_NO_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_NO_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_HUB_NO_POWER_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_NO_POWER_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fOverCurrent							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fOverCurrent							\
	)
# define USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fOverCurrent							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_OVER_CURRENT_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fOverCurrent							\
	)
# define USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fLocalPower							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fLocalPower							\
	)
# define USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fLocalPower							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_LOCAL_POWER_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_fLocalPower							\
	)
# define USBPUMP_USBDI_EVENT_TREE_TOO_DEEP_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_TREE_TOO_DEEP_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_TREE_TOO_DEEP_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_TREE_TOO_DEEP_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED_INFO_INIT_V1(	\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DRIVER_LAUNCH_FAILED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_DRIVER_COMPLETE_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort								\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DRIVER_COMPLETE_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort								\
	)
# define USBPUMP_USBDI_EVENT_DRIVER_COMPLETE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort								\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DRIVER_COMPLETE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort								\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_VANISHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_VANISHED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_DEVICE_VANISHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_DEVICE_VANISHED_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_ENUMERATION_FAILURE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_TIMEOUT_INFO_INIT_V1(			\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_TIMEOUT_INFO_INIT_V1(			\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_TIMEOUT_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_TIMEOUT_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_INTERNAL_FAILURE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_INTERNAL_FAILURE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_INTERNAL_FAILURE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_INTERNAL_FAILURE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_CHANGE_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_wMaxPower,							\
	a_wPowerInUse							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_CHANGE_POWER_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_wMaxPower,							\
	a_wPowerInUse							\
	)
# define USBPUMP_USBDI_EVENT_CHANGE_POWER_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_wMaxPower,							\
	a_wPowerInUse							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_CHANGE_POWER_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort,							\
	a_PortKey,							\
	a_wMaxPower,							\
	a_wPowerInUse							\
	)
# define USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE_INFO_INIT_V1(	\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HNP_STOPPED_BY_DEVICE_INFO_SETUP_V1(	\
	a_pInfo,							\
	a_pPort,							\
	a_PortKey							\
	)
# define USBPUMP_USBDI_EVENT_HUB_CONFIGURED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort								\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_CONFIGURED_INFO_INIT_V1(		\
	a_pFunction,							\
	a_pPort								\
	)
# define USBPUMP_USBDI_EVENT_HUB_CONFIGURED_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort								\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HUB_CONFIGURED_INFO_SETUP_V1(		\
	a_pInfo,							\
	a_pFunction,							\
	a_pPort								\
	)
# define USBPUMP_USBDI_EVENT_HNP_INFO_INIT_V1(			\
	a_pPort								\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HNP_INFO_INIT_V1(			\
	a_pPort								\
	)
# define USBPUMP_USBDI_EVENT_HNP_INFO_SETUP_V1(			\
	a_pInfo,							\
	a_pPort								\
	)	\
	__TMS_USBPUMP_USBDI_EVENT_HNP_INFO_SETUP_V1(			\
	a_pInfo,							\
	a_pPort								\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_event.h ****/
#endif /* _USBPUMP_USBDI_EVENT_H_ */
