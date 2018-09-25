/* usbioctl_proxy.h	Thu May 17 2012 15:42:54 chwon */

/*

Module:  usbioctl_proxy.h

Function:
	Definition of proxy IOCTL.

Version:
	V3.01f	Thu May 17 2012 15:42:54 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	April 2012

Revision history:
   3.01f  Mon Apr 09 2012 16:54:46 chwon
	14889: Module created.

   3.01f  Tue Apr 24 2012 11:28:11  chwon
	14889: Changed START and STOP IOCTL to asynch IOCTL.

   3.01f  Thu May 17 2012 15:42:54  chwon
	14889: Added USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE and
	USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP ioctl.

*/

#ifndef _USBIOCTL_PROXY_H_		/* prevent multiple includes */
#define _USBIOCTL_PROXY_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _UEVENTNODE_H_
# include "ueventnode.h"
#endif

/****************************************************************************\
|
|	Define the IOCTL codes.  The actual parameters are defined later;
|	we keep these together so we can easily keep the numbers sequential.
|
\****************************************************************************/

#define	__TMS_USBPUMP_IOCTL_CLASS_PROXY		'x'

/*
|| VOID can't be used as a parameter to a macro because MSC preprocessor
|| convers "VOID" to "void" and then concatenates this to __TMS_USBPUMP_IOCTL_
|| giving the symbol __TMS_USBPUMP_IOCTL_void that isn't defined
*/
#define __TMS_USBPUMP_IOCTL_PROXY_VOID(i)				\
	__TMS_USBPUMP_IOCTL_VOID(					\
		__TMS_USBPUMP_IOCTL_CLASS_PROXY,			\
		(i)							\
		)

#define __TMS_USBPUMP_IOCTL_PROXY(t, i, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_PROXY,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_PROXY_, NAME, _ARG)	\
		)

/* Start proxy protocol */
#define	__TMS_USBPUMP_IOCTL_PROXY_START_ASYNC	\
	__TMS_USBPUMP_IOCTL_PROXY(W_ASYNC, 0, START_ASYNC)

/* Stop proxy protocol */
#define	__TMS_USBPUMP_IOCTL_PROXY_STOP_ASYNC	\
	__TMS_USBPUMP_IOCTL_PROXY(W_ASYNC, 1, STOP_ASYNC)

/* Register proxy bus event notification */
#define	__TMS_USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE	\
	__TMS_USBPUMP_IOCTL_PROXY(W, 2, REGISTER_EVENT_NODE)

/* Set remote wakeup enable to the proxy */
#define	__TMS_USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP	\
	__TMS_USBPUMP_IOCTL_PROXY(RW, 3, CONTROL_REMOTE_WAKEUP)


/****************************************************************************\
|
|	Define the EDGE IOCTL codes.  The actual parameters are defined later;
|	we keep these together so we can easily keep the numbers sequential.
|
\****************************************************************************/

/*
|| edge functions are used for communicating with local OS client drivers.
|| These are issued as upcalls
*/
#define	__TMS_USBPUMP_IOCTL_CLASS_PROXY_EDGE	'X'

/*
|| VOID can't be used as a parameter to a macro because MSC preprocessor
|| convers "VOID" to "void" and then concatenates this to __TMS_USBPUMP_IOCTL_
|| giving the symbol __TMS_USBPUMP_IOCTL_void that isn't defined
*/
#define __TMS_USBPUMP_IOCTL_PROXY_EDGE_VOID(i)				\
	__TMS_USBPUMP_IOCTL_VOID(					\
		__TMS_USBPUMP_IOCTL_CLASS_PROXY_EDGE,			\
		(i)							\
		)

#define __TMS_USBPUMP_IOCTL_PROXY_EDGE(t, i, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_PROXY_EDGE,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_PROXY_EDGE_, NAME, _ARG) \
		)

/* get SIC configuration information */
#define	__TMS_USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG	\
	__TMS_USBPUMP_IOCTL_PROXY_EDGE(RW, 0, GET_MSIC_CONFIG)

/****************************************************************************\
|
|	The IOCTL details follow
|
\****************************************************************************/

/*

IOCTL:	USBPUMP_IOCTL_PROXY_START_ASYNC

Index:	Type:	USBPUMP_IOCTL_PROXY_START_ASYNC_ARG

Function:
	Start proxy protocol driver

Input:
	pInParam	points to a USBPUMP_IOCTL_PROXY_START_ASYNC_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_PROXY_START_ASYNC_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObjectHeader;
		The proxy object of interest.  This is passed to facilitate
		filtering and proxying.

	CONST UCHAR *	pProxyNamePrefix;
		Lower proxy driver name prefix. The proxy driver will use this
		proxy prefix to open lower proxy driver.

Description:
	This IOCTL is issued by the platform in order to start proxy driver.
	The proxy driver will open lower proxy driver and start protocol.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	Possibly some other error codes.

Setup Macro:
	VOID USBPUMP_IOCTL_PROXY_START_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_PROXY_START_ASYNC_ARG *pArg,
		USBPUMP_OBJECT_HEADER *	pObjectHeader,
		CONST UCHAR *		pProxyNamePrefix
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_PROXY_START_ASYNC_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_PROXY_START_ASYNC_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObjectHeader;
	__TMS_CONST __TMS_UCHAR *	pProxyNamePrefix;
	};

#define __TMS_USBPUMP_IOCTL_PROXY_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pObjectHeader,					\
	a_pProxyNamePrefix					\
	)							\
   do	{							\
	(a_pArg)->pObjectHeader = (a_pObjectHeader);		\
	(a_pArg)->pProxyNamePrefix = (a_pProxyNamePrefix);	\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_PROXY_STOP_ASYNC

Index:	Type:	USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG

Function:
	Stop proxy protocol driver.

Input:
	pInParam	points to a USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObjectHeader;
		The proxy object of interest.  This is passed to facilitate
		filtering and proxying.

Description:
	This IOCTL is issued by the platform in order to stop proxy driver.
	The proxy driver will close lower proxy driver and stop protocol.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	Possibly some other error codes.

Setup Macro:
	VOID USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG_SETUP_V1(
		USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG *pArg,
		USBPUMP_OBJECT_HEADER *	pObjectHeader
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObjectHeader;
	};

#define __TMS_USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pObjectHeader						\
	)							\
   do	{							\
	(a_pArg)->pObjectHeader = (a_pObjectHeader);		\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE

Index:	Type:	USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG

Function:
	Register proxy bus event notification.

Input:
	pInParam	points to a USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG
			structure.
	pOutParam	must be NULL.

	USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObjectHeader;
		The proxy object of interest.  This is passed to facilitate
		filtering and proxying.

	UEVENTNODE * pEventNode;
		Event node to register.

	UEVENTFN * pEventFn;
	VOID * pEventCtx;
		Event function and context pointer.

Description:
	This IOCTL is issued by the platform in order to register bus event
	notification node to the proxy driver.
	The proxy driver will call event node function when get bus event.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	Possibly some other error codes.

Setup Macro:
	VOID USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG_SETUP_V1(
		USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG *pArg,
		USBPUMP_OBJECT_HEADER *	pObjectHeader,
		UEVENTNODE * pEventNode,
		UEVENTFN * pEventFn,
		VOID * pEventCtx
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObjectHeader;
	__TMS_UEVENTNODE *		pEventNode;
	__TMS_UEVENTFN *		pEventFn;
	__TMS_VOID *			pEventCtx;
	};

#define __TMS_USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_pEventNode,							\
	a_pEventFn,							\
	a_pEventCtx							\
	)								\
   do	{								\
	(a_pArg)->pObjectHeader = (a_pObjectHeader);			\
	(a_pArg)->pEventNode = (a_pEventNode);				\
	(a_pArg)->pEventFn = (a_pEventFn);				\
	(a_pArg)->pEventCtx = (a_pEventCtx);				\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP

Index:	Type:	USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG

Function:
	Control remote wakeup enable or disable.

Input:
	pInParam	points to a USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG
			structure.
	pOutParam	points to a USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG
			structure.

	USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObjectHeader;
		The proxy object of interest.  This is passed to facilitate
		filtering and proxying.

	BOOL fRemoteWakeupEnable;
		If this is TRUE, proxy driver will do remote wakeup if device
		remote wakeup is enabled and device is suspended state.
		If this is FALSE, proxy driver will not send remote wakeup even
		if device remote wakeup is enabled.

Description:
	This IOCTL is issued by the platform in order to control device remote
	wakeup process.  If fRemoteWakeupEnable is TRUE, proxy driver will do
	remote wakeup if device remote wakeup is enabled and device is
	suspended state.  If fRemoteWakeupEnable is FALSE, proxy driver will
	not send remote wakeup even if device remote wakeup is enabled.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	Possibly some other error codes.

Setup Macro:
	VOID USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG_SETUP_V1(
		USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG *pArg,
		USBPUMP_OBJECT_HEADER *	pObjectHeader,
		BOOL fRemoteWakeupEnable
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObjectHeader;
	__TMS_BOOL			fRemoteWakeupEnable;
	};

#define __TMS_USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_fRemoteWakeupEnable						\
	)								\
   do	{								\
	(a_pArg)->pObjectHeader = (a_pObjectHeader);			\
	(a_pArg)->fRemoteWakeupEnable = (a_fRemoteWakeupEnable);	\
	} while (0)


/****************************************************************************\
|
|	The EDGE IOCTL details follow
|
\****************************************************************************/

/*

IOCTL:	USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG

Index:	Type:	USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG

Function:
	Get Proxy Modular SIC configuration.

Input:
	pInParam	points to a USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG structure.

	USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG has the following elements:

	USBPUMP_OBJECT_HEADER * pObjectHeader;
		The proxy object of interest.  This is passed to facilitate
		filtering and proxying.

	UINT8	bConfigurationValue;
	UINT8	bInterfaceNumber;
		USB device configuration and interface number for specific
		proxy object. Caller will pass current configuration and
		interface number.

Description:
	This IOCTL is issued by the proxy modular SIC protocol driver
	in order to give a chance to override proxy MSIC configuration.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS for success
	USBPUMP_IOCTL_RESULT_NOT_CLAIMED if a proxy couldn't be
		found in the dynamic parent of this object
	Possibly some other error codes.

Setup Macro:
	VOID USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG_SETUP_V1(
		USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG *pArg,
		USBPUMP_OBJECT_HEADER *	pObjectHeader,
		UINT8			bConfigurationValue,
		UINT8			bInterfaceNumber
		);

Notes:
	This is intended for internal use within the DataPump only,
	not for direct use by client driver code.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG);

struct __TMS_STRUCTNAME(USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER *	pObjectHeader;
	__TMS_UINT8			bConfigurationValue;
	__TMS_UINT8			bInterfaceNumber;
	};

#define __TMS_USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_bConfigurationValue,						\
	a_bInterfaceNumber						\
	)								\
   do	{								\
	(a_pArg)->pObjectHeader = (a_pObjectHeader);			\
	(a_pArg)->bConfigurationValue = (a_bConfigurationValue);	\
	(a_pArg)->bInterfaceNumber = (a_bInterfaceNumber);		\
	} while (0)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_CLASS_PROXY	\
   __TMS_USBPUMP_IOCTL_CLASS_PROXY
# define USBPUMP_IOCTL_PROXY_VOID(i)	\
   __TMS_USBPUMP_IOCTL_PROXY_VOID(i)
# define USBPUMP_IOCTL_PROXY(t, i, NAME)	\
   __TMS_USBPUMP_IOCTL_PROXY(t, i, NAME)
# define USBPUMP_IOCTL_PROXY_START_ASYNC	\
   __TMS_USBPUMP_IOCTL_PROXY_START_ASYNC
# define USBPUMP_IOCTL_PROXY_STOP_ASYNC	\
   __TMS_USBPUMP_IOCTL_PROXY_STOP_ASYNC
# define USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE	\
   __TMS_USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE
# define USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP	\
   __TMS_USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP
# define USBPUMP_IOCTL_CLASS_PROXY_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_PROXY_EDGE
# define USBPUMP_IOCTL_PROXY_EDGE_VOID(i)	\
   __TMS_USBPUMP_IOCTL_PROXY_EDGE_VOID(i)
# define USBPUMP_IOCTL_PROXY_EDGE(t, i, NAME)	\
   __TMS_USBPUMP_IOCTL_PROXY_EDGE(t, i, NAME)
# define USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG	\
   __TMS_USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG
# define USBPUMP_IOCTL_PROXY_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pObjectHeader,					\
	a_pProxyNamePrefix					\
	)	\
	__TMS_USBPUMP_IOCTL_PROXY_START_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pObjectHeader,					\
	a_pProxyNamePrefix					\
	)
# define USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pObjectHeader						\
	)	\
	__TMS_USBPUMP_IOCTL_PROXY_STOP_ASYNC_ARG_SETUP_V1(	\
	a_pArg,							\
	a_pObjectHeader						\
	)
# define USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_pEventNode,							\
	a_pEventFn,							\
	a_pEventCtx							\
	)	\
	__TMS_USBPUMP_IOCTL_PROXY_REGISTER_EVENT_NODE_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_pEventNode,							\
	a_pEventFn,							\
	a_pEventCtx							\
	)
# define USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_fRemoteWakeupEnable						\
	)	\
	__TMS_USBPUMP_IOCTL_PROXY_CONTROL_REMOTE_WAKEUP_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_fRemoteWakeupEnable						\
	)
# define USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_bConfigurationValue,						\
	a_bInterfaceNumber						\
	)	\
	__TMS_USBPUMP_IOCTL_PROXY_EDGE_GET_MSIC_CONFIG_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pObjectHeader,						\
	a_bConfigurationValue,						\
	a_bInterfaceNumber						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_proxy.h ****/
#endif /* _USBIOCTL_PROXY_H_ */
