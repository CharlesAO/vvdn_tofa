/* usbpump_ioctl_device_xdcd.h	Thu Nov 09 2017 15:30:31 chwon */

/*

Module:  usbpump_ioctl_device_xdcd.h

Function:
	XDCD-specific IOCTLs.

Version:
	V3.21d	Thu Nov 09 2017 15:30:31 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2014, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2014

Revision history:
   3.13a  Thu Jul 10 2014 16:55:25  tmm
	18309: Module created.

   3.19b  Fri Mar 24 2017 12:55:26  chwon
	20312: Add USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD.

   3.21d  Thu Nov 09 2017 15:30:31  chwon
	20707: Add USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION and
	USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION.

*/

#ifndef _USBPUMP_IOCTL_DEVICE_XDCD_H_		/* prevent multiple includes */
#define _USBPUMP_IOCTL_DEVICE_XDCD_H_

#ifndef _USBIOCTL_DEVICE_H_
# include "usbioctl_device.h"
#endif

/****************************************************************************\
|
|	XDCD specific device IOCTL definition
|
\****************************************************************************/

#define	__TMS_USBPUMP_XDCD_IOCTL_BASE				\
	__TMS_USBPUMP_IOCTL_DEVICE_IFC_BASE

#define	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT	\
	__TMS_USBPUMP_IOCTL_DEVICE(				\
		__TMS_USBPUMP_XDCD_IOCTL_BASE, W, XDCD_REGISTER_SOF_CLIENT)

#define	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD	\
	__TMS_USBPUMP_IOCTL_DEVICE(				\
		__TMS_USBPUMP_XDCD_IOCTL_BASE+1, W, XDCD_DEALY_UTMI_CLOCK_GOOD)

#define	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION	\
	__TMS_USBPUMP_IOCTL_DEVICE(				\
		__TMS_USBPUMP_XDCD_IOCTL_BASE+2, RW, XDCD_GET_FIFO_CONFIGURATION)

#define	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION	\
	__TMS_USBPUMP_IOCTL_DEVICE(				\
		__TMS_USBPUMP_XDCD_IOCTL_BASE+3, W, XDCD_SET_FIFO_CONFIGURATION)

/*

IOCTL:	USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT

Index:	Type:	USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG
	Name:	USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG_SETUP_V1

Function:
	IOCTL issued by USB chip interface to register SOF client callback 
	function.

Input:
	pInParam	points to a 
			USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG
			structure.

	pOutParam	None.

	USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG has
	the following elements:

	UCALLBACKCOMPLETION *	pCallback;
		IN: points to the callback completion structure.

	BOOL		fMicroFrameCallback;
		IN: TRUE if DCD calls callback on every micro-frame event.

Description:
	The client should send this IOCTL to register the callback function to
	recieve SOF event. The client should pass NULL to unregister the
	callback when SOF event is no longer needed.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG *pArg,
		UCALLBACKCOMPLETION *	pCallback,
		BOOL			fMicroFrameCallback
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG)
	{
	__TMS_UCALLBACKCOMPLETION *		pCallback;
	__TMS_BOOL				fMicroFrameCallback;
	};

#define __TMS_USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG_SETUP_V1( \
	pArg,								\
	ARG_pCallback,							\
	ARG_fMicroFrameCallback						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pCallback = (ARG_pCallback);				\
	__pArg->fMicroFrameCallback = (ARG_fMicroFrameCallback);	\
   	} while (0)


/*

IOCTL:	USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD

Index:	Type:	USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG
	Name:	USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG_SETUP_V1

Function:
	IOCTL issued by DCD to delay UTMI clock is good.

Input:
	pInParam	points to a 
			USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG
			structure.

	pOutParam	None.

	USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG has
	the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

Description:
	The DCD sends this IOCTL to delay UTMI clock is good.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG *pArg,
		UDEVICE *pDevice
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG)
	{
	__TMS_UDEVICE *		pDevice;
	};

#define __TMS_USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG * __TMS_CONST \
		__pArg = (pArg);					\
	__pArg->pDevice = (ARG_pDevice);				\
   	} while (0)


/*

IOCTL:	USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION

Index:	Type:	USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG
	Name:	USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG_SETUP_V1

Function:
	IOCTL issued by client to get XDCI FIFO configuration information.

Input:
	pInParam	points to a 
			USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG
			structure.

	pOutParam	points to a 
			USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG
			structure.

	USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG has
	the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	UINT nTxFifo;
		OUT: Number of TxFifo information is valid.

	UINT16 TxFifo[16];
		OUT: TxFifo contains TXFIFO size information. TxFifo Bit[14:0]
		contains depth of TxFIFO in MDWIDTH-bit words. Bit[15] is
		indicating asynchronous(0) or periodic(1) TXFIFO.

	UINT nRxFifo;
		OUT: Number of TxFifo information is valid.

	UINT16 RxFifo[16];
		OUT: RxFifo contains RXFIFO size information. RxFifo Bit[14:0]
		contains depth of RxFIFO in MDWIDTH-bit words. Bit[15] is
		indicating asynchronous(0) or periodic(1) RXFIFO.

Description:
	The client sends this IOCTL to retrieve TX and RX FIFO configuration
	information. The XDCI XDCD supports this IOCTL only if XDCI core is
	USB 3.1 core.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG *pArg,
		UDEVICE *pDevice
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG)
	{
	__TMS_UDEVICE *		pDevice;	/* IN */
	__TMS_UINT		nTxFifo;	/* OUT */
	__TMS_UINT16		TxFifo[16];	/* OUT */
	__TMS_UINT		nRxFifo;	/* OUT */
	__TMS_UINT16		RxFifo[16];	/* OUT */
	};

#define __TMS_USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG *	\
		__TMS_CONST __pArg = (pArg);				\
	__pArg->pDevice = (ARG_pDevice);				\
   	} while (0)


/*

IOCTL:	USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION

Index:	Type:	USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG
	Name:	USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG_SETUP_V1

Function:
	IOCTL issued by client to set XDCI FIFO configuration.

Input:
	pInParam	points to a 
			USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG
			structure.

	pOutParam	None.

	USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG has
	the following elements:

	UDEVICE *pDevice;
		IN: points to the UDEVICE structure

	UINT nTxFifo;
		IN: Number of TxFifo information is valid. If you set to zero,
		XDCI DCD will not change TX FIFO configuration.

	CONST UINT16 *pTxFifo;
		IN: TxFifo contains TXFIFO size information. TxFifo Bit[14:0]
		contains depth of TxFIFO in MDWIDTH-bit words. Bit[15] is
		indicating asynchronous(0) or periodic(1) TXFIFO.

	UINT nRxFifo;
		IN: Number of TxFifo information is valid. If you set to zero,
		XDCI DCD will not change RX FIFO configuration.

	CONST UINT16 * pRxFifo;
		IN: RxFifo contains RXFIFO size information. RxFifo Bit[14:0]
		contains depth of RxFIFO in MDWIDTH-bit words. Bit[15] is
		indicating asynchronous(0) or periodic(1) RXFIFO.

Description:
	The client sends this IOCTL to set up TX and RX FIFO configuration
	information. The XDCI XDCD supports this IOCTL only if XDCI core is
	USB 3.1 core.

Setup Macro:
	VOID USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG_SETUP_V1(
		USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG *pArg,
		UDEVICE *	pDevice,
		UINT		nTxFifo,
		CONST UINT16 *	pTxFifo,
		UINT		nRxFifo,
		CONST UINT16 *	pRxFifo
		);

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG)
	{
	__TMS_UDEVICE *			pDevice;	/* IN */
	__TMS_UINT			nTxFifo;	/* IN */
	__TMS_CONST __TMS_UINT16 *	pTxFifo;	/* IN */
	__TMS_UINT			nRxFifo;	/* IN */
	__TMS_CONST __TMS_UINT16 *	pRxFifo;	/* IN */
	};

#define __TMS_USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice,							\
	ARG_nTxFifo,							\
	ARG_pTxFifo,							\
	ARG_nRxFifo,							\
	ARG_pRxFifo							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG *	\
		__TMS_CONST __pArg = (pArg);				\
	__pArg->pDevice = (ARG_pDevice);				\
	__pArg->nTxFifo = (ARG_nTxFifo);				\
	__pArg->pTxFifo = (ARG_pTxFifo);				\
	__pArg->nRxFifo = (ARG_nRxFifo);				\
	__pArg->pRxFifo = (ARG_pRxFifo);				\
   	} while (0)


/****************************************************************************\
|
|	Uncloaked types
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_XDCD_IOCTL_BASE	\
   __TMS_USBPUMP_XDCD_IOCTL_BASE
# define USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT	\
   __TMS_USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT
# define USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD	\
   __TMS_USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD
# define USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION	\
   __TMS_USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION
# define USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION	\
   __TMS_USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION
# define USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG_SETUP_V1( \
	pArg,								\
	ARG_pCallback,							\
	ARG_fMicroFrameCallback						\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_REGISTER_SOF_CLIENT_ARG_SETUP_V1( \
	pArg,								\
	ARG_pCallback,							\
	ARG_fMicroFrameCallback						\
	)
# define USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_DEALY_UTMI_CLOCK_GOOD_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_GET_FIFO_CONFIGURATION_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice							\
	)
# define USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice,							\
	ARG_nTxFifo,							\
	ARG_pTxFifo,							\
	ARG_nRxFifo,							\
	ARG_pRxFifo							\
	)	\
	__TMS_USBPUMP_IOCTL_DEVICE_XDCD_SET_FIFO_CONFIGURATION_ARG_SETUP_V1( \
	pArg,								\
	ARG_pDevice,							\
	ARG_nTxFifo,							\
	ARG_pTxFifo,							\
	ARG_nRxFifo,							\
	ARG_pRxFifo							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_ioctl_device_xdcd.h ****/
#endif /* _USBPUMP_IOCTL_DEVICE_XDCD_H_ */
