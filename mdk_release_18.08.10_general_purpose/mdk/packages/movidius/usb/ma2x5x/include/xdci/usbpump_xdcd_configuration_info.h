/* usbpump_xdcd_configuration_info.h	Mon Oct 23 2017 19:27:16 chwon */

/*

Module:  usbpump_xdcd_configuration_info.h

Function:
	Definition of the XDCD configuration

Version:
	V3.21c	Mon Oct 23 2017 19:27:16 chwon	Edit level 6

Copyright notice:
	This file copyright (C) 2014, 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2014

Revision history:
   3.13a  Thu Jul 10 2014 15:45:37  tmm
	18309: Module created.

   3.13b  Wed Dec 17 2014 12:03:29  chwon
	18777: Add device alloc and free function in the configuration.

   3.13b  Thu Dec 18 2014 13:00:52  chwon
	18779: Add USBPUMP_XDCD_WIRING_FORCE_{HIGH|FULL}_SPEED wiring flags.

   3.17a  Fri Mar 25 2016 21:14:23  skpark
	20030: Add USBPUMP_XDCD_WIRING_CLEAR_EVENT_MASK_IN_SCISR

   3.21b  Wed Jul 19 2017 17:56:51  chwon
	20620: Add USBPUMP_XDCD_WIRING_FORCE_SUPER_SPEED wiring flag.

   3.21c  Mon Oct 23 2017 19:27:16  chwon
	20694: Add USBPUMP_XDCD_WIRING_DISABLE_DEVICE_INITIATE_U1_U2 flag.

*/

#ifndef _USBPUMP_XDCD_CONFIGURATION_INFO_H_	/* prevent multiple includes */
#define _USBPUMP_XDCD_CONFIGURATION_INFO_H_

#ifndef _XDCDKERN_TYPES_
# include "xdcdkern_types.h"
#endif

#ifndef _UDEVSWITCH_H_
# include "udevswitch.h"
#endif

/****************************************************************************\
|
|	This structure is used to pass configuration information from
|	platform initialization to the XDCI init code (possibly via
|	other subroutines).
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_XDCD_CONFIGURATION_INFO)
	{
	__TMS_UINT32					ulWiring;
	__TMS_VOID *					pPool;
	__TMS_ADDRBITS					PoolSize;

	__TMS_UHIL_INTERRUPT_RESOURCE_HANDLE		hUsbInt;
	__TMS_UINT 					ControlReplySize;
	__TMS_UINT32					Alignment;

	/*
	|| Bus DMA Burst Len
	*/
	__TMS_UINT16					DmaBurstSize;
	__TMS_UINT16					nEvents;

	/*
	|| HIRD threshold
	*/
	__TMS_UINT16					HirdThreshold;

	/* NUMP value (1 - 16) */
	__TMS_UINT8					NumP;

	__TMS_UDEVIOCTLFN *				pDcdIoctl;
	__TMS_USBPUMP_XDCD_GET_BASE_ADDRESS_FN *	pGetBaseAddrFn;
	__TMS_USBPUMP_XDCD_DMA_FLUSH_FN *		pDmaFlushFn;
	__TMS_USBPUMP_XDCD_DMA_INVALIDATE_FN *		pDmaInvalidateFn;

	__TMS_USBPUMP_XDCD_GET_PHYSICAL_ADDR_FN *	pGetPhysicalAddrFn;

	__TMS_USBPUMP_XDCD_SET_POWER_STATE_TO_D0_FN *	pSetPowerStateToD0Fn;
	__TMS_USBPUMP_XDCD_SET_POWER_STATE_TO_D3_FN *	pSetPowerStateToD3Fn;
	__TMS_USBPUMP_XDCD_IS_PME_GENERATED_FN *	pIsPmeGeneratedFn;
	__TMS_USBPUMP_XDCD_IS_CONNECTED_TO_HOST_FN *	pIsConnectedToHostFn;

	__TMS_UDEVALLOCATEFN *				pDeviceAllocFn;
	__TMS_UDEVFREEFN *				pDeviceFreeFn;
	};

/*
|| initialization functions.  Don't change the calling sequence for any
|| macro ending in Vn; instead, create a macro ending in Vn+1, and
|| redefine the Vn macro to expand in an upward-compatible way.
*/
#define	__TMS_USBPUMP_XDCD_CONFIGURATION_INFO_INIT_V2(		\
		a_ulWiring,					\
		a_pPool,					\
		a_PoolSize,					\
		a_hUsbInt,					\
		a_ControlReplySize,				\
		a_Alignment,					\
		a_DmaBurstSize,					\
		a_nEvents,					\
		a_HirdThreshold,				\
		a_NumP,						\
		a_pDcdIoctl,					\
		a_pGetBaseAddrFn,				\
		a_pDmaFlushFn,					\
		a_pDmaInvalidateFn,				\
		a_pGetPhysicalAddrFn,				\
		a_pSetPowerStateToD0Fn,				\
		a_pSetPowerStateToD3Fn,				\
		a_pIsPmeGeneratedFn,				\
		a_pIsConnectedToHostFn,				\
		a_pDeviceAllocFn,				\
		a_pDeviceFreeFn					\
		)						\
	{							\
	(a_ulWiring),						\
	(a_pPool),						\
	(a_PoolSize),						\
	(a_hUsbInt),						\
	(a_ControlReplySize),					\
	(a_Alignment),						\
	(a_DmaBurstSize),					\
	(a_nEvents),						\
	(a_HirdThreshold),					\
	(a_NumP),						\
	(a_pDcdIoctl),						\
	(a_pGetBaseAddrFn),					\
	(a_pDmaFlushFn),					\
	(a_pDmaInvalidateFn),					\
	(a_pGetPhysicalAddrFn),					\
	(a_pSetPowerStateToD0Fn),				\
	(a_pSetPowerStateToD3Fn),				\
	(a_pIsPmeGeneratedFn),					\
	(a_pIsConnectedToHostFn),				\
	(a_pDeviceAllocFn),					\
	(a_pDeviceFreeFn)					\
	}

#define	__TMS_USBPUMP_XDCD_CONFIGURATION_INFO_INIT_V1(		\
		a_ulWiring,					\
		a_pPool,					\
		a_PoolSize,					\
		a_hUsbInt,					\
		a_ControlReplySize,				\
		a_Alignment,					\
		a_DmaBurstSize,					\
		a_nEvents,					\
		a_HirdThreshold,				\
		a_NumP,						\
		a_pDcdIoctl,					\
		a_pGetBaseAddrFn,				\
		a_pDmaFlushFn,					\
		a_pDmaInvalidateFn,				\
		a_pGetPhysicalAddrFn,				\
		a_pSetPowerStateToD0Fn,				\
		a_pSetPowerStateToD3Fn,				\
		a_pIsPmeGeneratedFn,				\
		a_pIsConnectedToHostFn				\
		)						\
	__TMS_USBPUMP_XDCD_CONFIGURATION_INFO_INIT_V2(		\
		a_ulWiring,					\
		a_pPool,					\
		a_PoolSize,					\
		a_hUsbInt,					\
		a_ControlReplySize,				\
		a_Alignment,					\
		a_DmaBurstSize,					\
		a_nEvents,					\
		a_HirdThreshold,				\
		a_NumP,						\
		a_pDcdIoctl,					\
		a_pGetBaseAddrFn,				\
		a_pDmaFlushFn,					\
		a_pDmaInvalidateFn,				\
		a_pGetPhysicalAddrFn,				\
		a_pSetPowerStateToD0Fn,				\
		a_pSetPowerStateToD3Fn,				\
		a_pIsPmeGeneratedFn,				\
		a_pIsConnectedToHostFn,				\
		/* V2: pDeviceAllocFn */ __TMS_NULL,		\
		/* V2: pDeviceFreeFn */ __TMS_NULL		\
		)


/****************************************************************************\
|
|	The wiring flags are used to communicate hardware design decisions
|	to the chip driver.
|
\****************************************************************************/

/**** bit names for udevxdcd_wiring ****/
/*

USBPUMP_XDCD_WIRING_BUSPOWER --
	This bit should be set device can be bus powered (static).
	DONT CHANGE THIS ON THE FLY if device is sometimes one, sometimes
	the other. If need to filter the dynamic reports that are based
	on this, hook the device-level events and update the data emitted by
	the chip code.  Get help from MCCI engineering if this is what
	you need to do....

USBPUMP_XDCD_WIRING_ENABLE_LPM --
	This bit should be set if LPM is enabled.

USBPUMP_XDCD_WIRING_UHILAUX_CACHED --
	This bit should be set if memory allocated with UHILAUX API is cached.

USBPUMP_XDCD_WIRING_PWROPT_CLOCKGATING --
	This bit should be set if clock gating power option is enabled.

USBPUMP_XDCD_WIRING_PWROPT_HIBERNATION --
	This bit should be set if hibernation power option is enabled.

USBPUMP_XDCD_WIRING_HIBERNATION_ON_DISCONNECT --
	This bit should be set if hibernation on disconnect is enabled.

USBPUMP_XDCD_WIRING_PHY_16BIT --
	This bit should be set if 16bit PHY is used.

USBPUMP_XDCD_WIRING_OTG --
	This bit should be set if the DCD is a part of an OTG driver.

USBPUMP_XDCD_WIRING_USE_POWER_REQUEST --
	This bit should be set if the DCD needs to call the registered power
	request function on hibernation/resume.

USBPUMP_XDCD_WIRING_CONTROL_PMU --
	This bit should be set if the DCD needs to use PMU control functions
	from configuration. This bit shouldn't be use with
	USBPUMP_XDCD_WIRING_USE_POWER_REQUEST.

USBPUMP_XDCD_WIRING_COPY_DESCRIPTOR_DATA --
	This bit should be set if the DCD needs to copy device descriptor data
	to UDEVICE reply buffer.  This flag is useful if platform doesn't
	support DMA operation for code space buffer.

USBPUMP_XDCD_WIRING_FORCE_HIGH_SPEED --
	This bit should be set if the platform wants to connect high-speed or
	the maximum speed the application can support. To support super-speed
	connection, this flag should not be set.

USBPUMP_XDCD_WIRING_FORCE_FULL_SPEED --
	This bit should be set if the platform wants to connect full-speed or
	the maximum speed the application can support. To support super-speed
	connection, this flag should not be set.

USBPUMP_XDCD_WIRING_CLEAR_EVENT_MASK_IN_SCISR --
	This bit should be set if the DCD needs to clear the event mask
	in the secondary message based ISR.

USBPUMP_XDCD_WIRING_FORCE_SUPER_SPEED --
	This bit should be set if the platform wants to connect super-speed or
	the maximum speed the application can support. This option is valid
	only if XDCI support USB 3.1 capability. To support enhanced
	super-speed connection, this flag should not be set.

USBPUMP_XDCD_WIRING_DISABLE_DEVICE_INITIATE_U1_U2 --
	This bit should be set if the platform doesn't want to enable device
	initiate U1 and U2. If this wiring flag set, DCD will not set INITU1ENA
	and INITU2ENA bit in the DCTL register.

*/

#define	__TMS_USBPUMP_XDCD_WIRING_BUSPOWER			(1 << 0)
#define __TMS_USBPUMP_XDCD_WIRING_ENABLE_LPM			(1 << 1)
#define __TMS_USBPUMP_XDCD_WIRING_UHILAUX_CACHED		(1 << 2)
#define __TMS_USBPUMP_XDCD_WIRING_PWROPT_CLOCKGATING		(1 << 3)
#define __TMS_USBPUMP_XDCD_WIRING_PWROPT_HIBERNATION		(1 << 4)
#define __TMS_USBPUMP_XDCD_WIRING_HIBERNATION_ON_DISCONNECT	(1 << 5)
#define __TMS_USBPUMP_XDCD_WIRING_PHY_16BIT			(1 << 6)
#define __TMS_USBPUMP_XDCD_WIRING_OTG				(1 << 7)
#define __TMS_USBPUMP_XDCD_WIRING_USE_POWER_REQUEST		(1 << 8)
#define __TMS_USBPUMP_XDCD_WIRING_CONTROL_PMU			(1 << 9)
#define __TMS_USBPUMP_XDCD_WIRING_COPY_DESCRIPTOR_DATA		(1 << 10)
#define __TMS_USBPUMP_XDCD_WIRING_FORCE_HIGH_SPEED		(1 << 11)
#define __TMS_USBPUMP_XDCD_WIRING_FORCE_FULL_SPEED		(1 << 12)
#define __TMS_USBPUMP_XDCD_WIRING_CLEAR_EVENT_MASK_IN_SCISR	(1 << 13)
#define __TMS_USBPUMP_XDCD_WIRING_FORCE_SUPER_SPEED		(1 << 14)
#define __TMS_USBPUMP_XDCD_WIRING_DISABLE_DEVICE_INITIATE_U1_U2	(1 << 15)

/**** DMA Bursts ****/
#define __TMS_USBPUMP_XDCD_DMA_BURST_SINGLE	0
#define __TMS_USBPUMP_XDCD_DMA_BURST_INCR	1
#define __TMS_USBPUMP_XDCD_DMA_BURST_INCR4	3
#define __TMS_USBPUMP_XDCD_DMA_BURST_INCR8	7
#define __TMS_USBPUMP_XDCD_DMA_BURST_INCR16	15
#define __TMS_USBPUMP_XDCD_DMA_BURST_INCR32	31
#define __TMS_USBPUMP_XDCD_DMA_BURST_INCR64	63
#define __TMS_USBPUMP_XDCD_DMA_BURST_INCR128	127
#define __TMS_USBPUMP_XDCD_DMA_BURST_INCR256	255

/**** HIRD Thresholds ****/
#define __TMS_USBPUMP_XDCD_HIRDTHRES_60		0	/* 60 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_135	1	/* 135 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_210	2	/* 210 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_285	3	/* 285 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_360	4	/* 360 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_435	5	/* 435 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_510	6	/* 510 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_585	7	/* 585 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_660	8	/* 660 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_735	9	/* 735 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_810	10	/* 810 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_885	11	/* 885 */
#define __TMS_USBPUMP_XDCD_HIRDTHRES_960	12	/* 960 */


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_XDCD_CONFIGURATION_INFO_INIT_V2(		\
		a_ulWiring,					\
		a_pPool,					\
		a_PoolSize,					\
		a_hUsbInt,					\
		a_ControlReplySize,				\
		a_Alignment,					\
		a_DmaBurstSize,					\
		a_nEvents,					\
		a_HirdThreshold,				\
		a_NumP,						\
		a_pDcdIoctl,					\
		a_pGetBaseAddrFn,				\
		a_pDmaFlushFn,					\
		a_pDmaInvalidateFn,				\
		a_pGetPhysicalAddrFn,				\
		a_pSetPowerStateToD0Fn,				\
		a_pSetPowerStateToD3Fn,				\
		a_pIsPmeGeneratedFn,				\
		a_pIsConnectedToHostFn,				\
		a_pDeviceAllocFn,				\
		a_pDeviceFreeFn					\
		)	\
	__TMS_USBPUMP_XDCD_CONFIGURATION_INFO_INIT_V2(		\
		a_ulWiring,					\
		a_pPool,					\
		a_PoolSize,					\
		a_hUsbInt,					\
		a_ControlReplySize,				\
		a_Alignment,					\
		a_DmaBurstSize,					\
		a_nEvents,					\
		a_HirdThreshold,				\
		a_NumP,						\
		a_pDcdIoctl,					\
		a_pGetBaseAddrFn,				\
		a_pDmaFlushFn,					\
		a_pDmaInvalidateFn,				\
		a_pGetPhysicalAddrFn,				\
		a_pSetPowerStateToD0Fn,				\
		a_pSetPowerStateToD3Fn,				\
		a_pIsPmeGeneratedFn,				\
		a_pIsConnectedToHostFn,				\
		a_pDeviceAllocFn,				\
		a_pDeviceFreeFn					\
		)
# define USBPUMP_XDCD_CONFIGURATION_INFO_INIT_V1(		\
		a_ulWiring,					\
		a_pPool,					\
		a_PoolSize,					\
		a_hUsbInt,					\
		a_ControlReplySize,				\
		a_Alignment,					\
		a_DmaBurstSize,					\
		a_nEvents,					\
		a_HirdThreshold,				\
		a_NumP,						\
		a_pDcdIoctl,					\
		a_pGetBaseAddrFn,				\
		a_pDmaFlushFn,					\
		a_pDmaInvalidateFn,				\
		a_pGetPhysicalAddrFn,				\
		a_pSetPowerStateToD0Fn,				\
		a_pSetPowerStateToD3Fn,				\
		a_pIsPmeGeneratedFn,				\
		a_pIsConnectedToHostFn				\
		)	\
	__TMS_USBPUMP_XDCD_CONFIGURATION_INFO_INIT_V1(		\
		a_ulWiring,					\
		a_pPool,					\
		a_PoolSize,					\
		a_hUsbInt,					\
		a_ControlReplySize,				\
		a_Alignment,					\
		a_DmaBurstSize,					\
		a_nEvents,					\
		a_HirdThreshold,				\
		a_NumP,						\
		a_pDcdIoctl,					\
		a_pGetBaseAddrFn,				\
		a_pDmaFlushFn,					\
		a_pDmaInvalidateFn,				\
		a_pGetPhysicalAddrFn,				\
		a_pSetPowerStateToD0Fn,				\
		a_pSetPowerStateToD3Fn,				\
		a_pIsPmeGeneratedFn,				\
		a_pIsConnectedToHostFn				\
		)
# define USBPUMP_XDCD_WIRING_BUSPOWER	\
   __TMS_USBPUMP_XDCD_WIRING_BUSPOWER
# define USBPUMP_XDCD_WIRING_ENABLE_LPM	\
   __TMS_USBPUMP_XDCD_WIRING_ENABLE_LPM
# define USBPUMP_XDCD_WIRING_UHILAUX_CACHED	\
   __TMS_USBPUMP_XDCD_WIRING_UHILAUX_CACHED
# define USBPUMP_XDCD_WIRING_PWROPT_CLOCKGATING	\
   __TMS_USBPUMP_XDCD_WIRING_PWROPT_CLOCKGATING
# define USBPUMP_XDCD_WIRING_PWROPT_HIBERNATION	\
   __TMS_USBPUMP_XDCD_WIRING_PWROPT_HIBERNATION
# define USBPUMP_XDCD_WIRING_HIBERNATION_ON_DISCONNECT	\
   __TMS_USBPUMP_XDCD_WIRING_HIBERNATION_ON_DISCONNECT
# define USBPUMP_XDCD_WIRING_PHY_16BIT	\
   __TMS_USBPUMP_XDCD_WIRING_PHY_16BIT
# define USBPUMP_XDCD_WIRING_OTG	\
   __TMS_USBPUMP_XDCD_WIRING_OTG
# define USBPUMP_XDCD_WIRING_USE_POWER_REQUEST	\
   __TMS_USBPUMP_XDCD_WIRING_USE_POWER_REQUEST
# define USBPUMP_XDCD_WIRING_CONTROL_PMU	\
   __TMS_USBPUMP_XDCD_WIRING_CONTROL_PMU
# define USBPUMP_XDCD_WIRING_COPY_DESCRIPTOR_DATA	\
   __TMS_USBPUMP_XDCD_WIRING_COPY_DESCRIPTOR_DATA
# define USBPUMP_XDCD_WIRING_FORCE_HIGH_SPEED	\
   __TMS_USBPUMP_XDCD_WIRING_FORCE_HIGH_SPEED
# define USBPUMP_XDCD_WIRING_FORCE_FULL_SPEED	\
   __TMS_USBPUMP_XDCD_WIRING_FORCE_FULL_SPEED
# define USBPUMP_XDCD_WIRING_CLEAR_EVENT_MASK_IN_SCISR	\
   __TMS_USBPUMP_XDCD_WIRING_CLEAR_EVENT_MASK_IN_SCISR
# define USBPUMP_XDCD_WIRING_FORCE_SUPER_SPEED	\
   __TMS_USBPUMP_XDCD_WIRING_FORCE_SUPER_SPEED
# define USBPUMP_XDCD_WIRING_DISABLE_DEVICE_INITIATE_U1_U2	\
   __TMS_USBPUMP_XDCD_WIRING_DISABLE_DEVICE_INITIATE_U1_U2
# define USBPUMP_XDCD_DMA_BURST_SINGLE	\
   __TMS_USBPUMP_XDCD_DMA_BURST_SINGLE
# define USBPUMP_XDCD_DMA_BURST_INCR	\
   __TMS_USBPUMP_XDCD_DMA_BURST_INCR
# define USBPUMP_XDCD_DMA_BURST_INCR4	\
   __TMS_USBPUMP_XDCD_DMA_BURST_INCR4
# define USBPUMP_XDCD_DMA_BURST_INCR8	\
   __TMS_USBPUMP_XDCD_DMA_BURST_INCR8
# define USBPUMP_XDCD_DMA_BURST_INCR16	\
   __TMS_USBPUMP_XDCD_DMA_BURST_INCR16
# define USBPUMP_XDCD_DMA_BURST_INCR32	\
   __TMS_USBPUMP_XDCD_DMA_BURST_INCR32
# define USBPUMP_XDCD_DMA_BURST_INCR64	\
   __TMS_USBPUMP_XDCD_DMA_BURST_INCR64
# define USBPUMP_XDCD_DMA_BURST_INCR128	\
   __TMS_USBPUMP_XDCD_DMA_BURST_INCR128
# define USBPUMP_XDCD_DMA_BURST_INCR256	\
   __TMS_USBPUMP_XDCD_DMA_BURST_INCR256
# define USBPUMP_XDCD_HIRDTHRES_60	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_60
# define USBPUMP_XDCD_HIRDTHRES_135	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_135
# define USBPUMP_XDCD_HIRDTHRES_210	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_210
# define USBPUMP_XDCD_HIRDTHRES_285	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_285
# define USBPUMP_XDCD_HIRDTHRES_360	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_360
# define USBPUMP_XDCD_HIRDTHRES_435	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_435
# define USBPUMP_XDCD_HIRDTHRES_510	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_510
# define USBPUMP_XDCD_HIRDTHRES_585	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_585
# define USBPUMP_XDCD_HIRDTHRES_660	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_660
# define USBPUMP_XDCD_HIRDTHRES_735	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_735
# define USBPUMP_XDCD_HIRDTHRES_810	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_810
# define USBPUMP_XDCD_HIRDTHRES_885	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_885
# define USBPUMP_XDCD_HIRDTHRES_960	\
   __TMS_USBPUMP_XDCD_HIRDTHRES_960
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_xdcd_configuration_info.h ****/
#endif /* _USBPUMP_XDCD_CONFIGURATION_INFO_H_ */
