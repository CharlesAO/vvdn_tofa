/* usbpump_usbdi_device_arrival.h	Fri Apr 19 2013 11:03:02 chwon */

/*

Module:  usbpump_usbdi_device_arrival.h

Function:
	Data structures for the USBDI "Device Arrival Listener" API.

Version:
	V3.11c	Fri Apr 19 2013 11:03:02 chwon	Edit level 8

Copyright notice:
	This file copyright (C) 2010-2011, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	January 2010

Revision history:
   3.01a  Sun Jan  3 2010 18:04:54  tmm
	9677: Module created.

   3.01a  Mon Jan 04 2010 23:19:05  chwon
	9688: Add USBPUMP_USBDI_DEVICE_DEPARTURE_INFO and setup macro.
	9642: Add device speed info in the USBPUMP_USBDI_DEVICE_ARRIVAL_INFO.

   3.01a  Sun Jan 17 2010 16:43:47  tmm
	9688: add bus address to USBPUMP_USBDI_DEVICE_ARRIVAL_INFO.

   3.01d  Wed Feb 09 2011 12:28:52  chwon
	12638: add device removable in the USBPUMP_USBDI_DEVICE_ARRIVAL_INFO.

   3.01d  Fri Feb 25 2011 11:24:19  ssyang
	12820: 12834: add bRemoteWakeupAttribute in the
	USBPUMP_USBDI_DEVICE_ARRIVAL_INFO.

   3.01f  Thu Aug 18 2011 17:23:35  cccho
	13946: add bIndexProduct to USBPUMP_USBDI_DEVICE_ARRIVAL_INFO.

   3.01f  Wed Aug 31 2011 10:48:48  cccho
	12152: add fHaveUaspInterface to USBPUMP_USBDI_DEVICE_ARRIVAL_INFO.

   3.11c  Fri Apr 19 2013 11:03:02  chwon
	17054: Added fOsDescNotSupported to USBPUMP_USBDI_DEVICE_ARRIVAL_INFO.

*/

#ifndef _USBPUMP_USBDI_DEVICE_ARRIVAL_H_	/* prevent multiple includes */
#define _USBPUMP_USBDI_DEVICE_ARRIVAL_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

/****************************************************************************\
|
|	USBD Device Arrival Information.
|
\****************************************************************************/

#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_DISABLE	0
#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_USE_INTERNAL	1
#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_USE_EXTERNAL	2

union __TMS_UNIONNAME(USBPUMP_USBDI_NORMALIZED_ROUTE_STRING)
	{
	/* routing by tier; level 0 is root port */
	__TMS_UINT8					RouteTier[6];
	__TMS_UINT64					RouteString;
	};

struct __TMS_STRUCTNAME(USBPUMP_USBDI_DEVICE_ARRIVAL_INFO)
	{
	__TMS_UINT32					ulMagic;
	__TMS_UINT16					usSize;
	__TMS_UINT8					bVersion;
	__TMS_USBPUMP_DEVICE_SPEED			bSpeed;
	__TMS_VOID *					pListenerContext;
	__TMS_USBPUMP_USBDI_USBD *			pUsbd;
	__TMS_USBPUMP_USBDI_FUNCTION *			pParentHub;
	__TMS_USBPUMP_USBDI_PORT *			pPort;
	__TMS_CONST __TMS_TEXT *			pDeviceId;
	__TMS_CONST __TMS_TEXT *			pInstanceId;
	__TMS_BYTES					nDeviceId;
	__TMS_BYTES					nInstanceId;
	__TMS_USBPUMP_USBDI_NORMALIZED_ROUTE_STRING	uRouteString;
	/*
	|| note that at least 1 bytes are wasted here. put pointers above,
	|| byte/short data here.
	*/
	__TMS_UINT8					bAddress;
	__TMS_UINT8					bDeviceRemovable;
	__TMS_UINT8					bRemoteWakeupAttribute;
	__TMS_UINT8					bIndexProduct;
	__TMS_UINT					fHaveUaspInterface: 1;
	__TMS_UINT					fOsDescNotSupported: 1;
	};

#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_MAGIC			\
	__TMS_UHIL_MEMTAG('U', 'D', 'A', 'I')

#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V6(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct,					\
		a_fHaveUaspInterface,					\
		a_fOsDescNotSupported					\
		)							\
    do	{								\
	(a_pInfo)->ulMagic = __TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_MAGIC; \
	(a_pInfo)->usSize = sizeof(*(a_pInfo));				\
	(a_pInfo)->bVersion = 1;					\
	(a_pInfo)->bSpeed = (a_bSpeed);					\
	(a_pInfo)->pListenerContext = (a_pArrivalListenerContext);	\
	(a_pInfo)->pUsbd = (a_pUsbd);					\
	(a_pInfo)->pParentHub = (a_pParentHub);				\
	(a_pInfo)->pPort = (a_pPort);					\
	(a_pInfo)->pDeviceId = (a_pDeviceId);				\
	(a_pInfo)->nDeviceId = (a_nDeviceId);				\
	(a_pInfo)->pInstanceId = (a_pInstanceId);			\
	(a_pInfo)->nInstanceId = (a_nInstanceId);			\
	(a_pInfo)->uRouteString = (a_routingString);			\
	(a_pInfo)->bAddress = (a_bAddress);				\
	(a_pInfo)->bDeviceRemovable = (a_bDeviceRemovable);		\
	(a_pInfo)->bRemoteWakeupAttribute = (a_bRemoteWakeupAttribute);	\
	(a_pInfo)->bIndexProduct = (a_bIndexProduct);			\
	(a_pInfo)->fHaveUaspInterface = (a_fHaveUaspInterface);		\
	(a_pInfo)->fOsDescNotSupported = (a_fOsDescNotSupported);	\
	} while (0)

#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V5(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct,					\
		a_fHaveUaspInterface					\
		)							\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V6(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct,					\
		a_fHaveUaspInterface,					\
		/* V6: fOsDescNotSupported */ FALSE			\
		)

#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V4(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct						\
		)							\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V5(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct,					\
		/* V5: fHaveUaspInterface */ 0				\
		)

#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V3(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute				\
		)							\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V4(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		/* V4: bIndexProduct */ 0				\
		)

#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V2(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable					\
		)							\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V3(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		/* V3: bRemoteWakeupAttribute */ 0			\
		)
		
#define	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V1(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress						\
		)							\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V2(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		/* V2: bDeviceRemovable */ 1				\
		)


/****************************************************************************\
|
|	USBD Device Departure Information.
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_USBDI_DEVICE_DEPARTURE_INFO)
	{
	__TMS_UINT32					ulMagic;
	__TMS_UINT16					usSize;
	__TMS_UINT16					usVersion;
	__TMS_VOID *					pListenerContext;
	__TMS_USBPUMP_USBDI_USBD *			pUsbd;
	__TMS_USBPUMP_USBDI_FUNCTION *			pFunction;
	__TMS_USBPUMP_USBDI_PORT *			pPort;
	};

#define	__TMS_USBPUMP_USBDI_DEVICE_DEPARTURE_INFO_MAGIC			\
	__TMS_UHIL_MEMTAG('U', 'D', 'D', 'I')

#define	__TMS_USBPUMP_USBDI_DEVICE_DEPARTURE_INFO_SETUP_V1(		\
		a_pInfo,						\
		a_pDepartureListenerContext,				\
		a_pUsbd,						\
		a_pFunction,						\
		a_pPort							\
		)							\
    do	{								\
	(a_pInfo)->ulMagic = __TMS_USBPUMP_USBDI_DEVICE_DEPARTURE_INFO_MAGIC; \
	(a_pInfo)->usSize = sizeof(*(a_pInfo));				\
	(a_pInfo)->usVersion = 1;					\
	(a_pInfo)->pListenerContext = (a_pDepartureListenerContext);	\
	(a_pInfo)->pUsbd = (a_pUsbd);					\
	(a_pInfo)->pFunction = (a_pFunction);				\
	(a_pInfo)->pPort = (a_pPort);					\
	} while (0)


/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_DISABLE	\
   __TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_DISABLE
# define USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_USE_INTERNAL	\
   __TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_USE_INTERNAL
# define USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_USE_EXTERNAL	\
   __TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_RESULT_USE_EXTERNAL
# define USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_MAGIC	\
   __TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_MAGIC
# define USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V6(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct,					\
		a_fHaveUaspInterface,					\
		a_fOsDescNotSupported					\
		)	\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V6(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct,					\
		a_fHaveUaspInterface,					\
		a_fOsDescNotSupported					\
		)
# define USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V5(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct,					\
		a_fHaveUaspInterface					\
		)	\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V5(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct,					\
		a_fHaveUaspInterface					\
		)
# define USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V4(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct						\
		)	\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V4(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute,				\
		a_bIndexProduct						\
		)
# define USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V3(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute				\
		)	\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V3(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable,					\
		a_bRemoteWakeupAttribute				\
		)
# define USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V2(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable					\
		)	\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V2(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress,						\
		a_bDeviceRemovable					\
		)
# define USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V1(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress						\
		)	\
	__TMS_USBPUMP_USBDI_DEVICE_ARRIVAL_INFO_SETUP_V1(		\
		a_pInfo,						\
		a_pArrivalListenerContext,				\
		a_pUsbd,						\
		a_pParentHub,						\
		a_pPort,						\
		a_pDeviceId,						\
		a_nDeviceId,						\
		a_pInstanceId,						\
		a_nInstanceId,						\
		a_routingString,					\
		a_bSpeed,						\
		a_bAddress						\
		)
# define USBPUMP_USBDI_DEVICE_DEPARTURE_INFO_MAGIC	\
   __TMS_USBPUMP_USBDI_DEVICE_DEPARTURE_INFO_MAGIC
# define USBPUMP_USBDI_DEVICE_DEPARTURE_INFO_SETUP_V1(		\
		a_pInfo,						\
		a_pDepartureListenerContext,				\
		a_pUsbd,						\
		a_pFunction,						\
		a_pPort							\
		)	\
	__TMS_USBPUMP_USBDI_DEVICE_DEPARTURE_INFO_SETUP_V1(		\
		a_pInfo,						\
		a_pDepartureListenerContext,				\
		a_pUsbd,						\
		a_pFunction,						\
		a_pPort							\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_device_arrival.h ****/
#endif /* _USBPUMP_USBDI_DEVICE_ARRIVAL_H_ */
