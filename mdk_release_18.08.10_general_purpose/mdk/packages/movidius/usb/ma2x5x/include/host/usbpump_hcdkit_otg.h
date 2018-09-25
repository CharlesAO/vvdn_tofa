/* usbpump_hcdkit_otg.h	Tue Mar 12 2013 12:33:48 chwon */

/*

Module:  usbpump_hcdkit_otg.h

Function:
	Definition of HCDKIT OTG function

Version:
	V3.11c	Tue Mar 12 2013 12:33:48 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2013

Revision history:
   3.11c  Tue Mar 12 2013 12:33:48 chwon
	Module created.

*/

#ifndef _USBPUMP_HCDKIT_OTG_H_	/* prevent multiple includes */
#define _USBPUMP_HCDKIT_OTG_H_

#ifndef _USBPUMP_HCDKIT_HCD_H_
# include "usbpump_hcdkit_hcd.h"
#endif

#ifndef _USBPUMP_USBPHY_TYPES_H_
# include "usbpump_usbphy_types.h"
#endif

/****************************************************************************\
|
|	USBPUMP_HCDKIT_OTG Structure
|
\****************************************************************************/

/*

Type:	USBPUMP_HCDKIT_OTG

Index:	Type:	struct __TMS_USBPUMP_HCDKIT_OTG

Function:
	Represents OTG handling information.

Description:
	This structure contains OTG handling informations. This structure will
	be created by UsbPumpHcdKit_InitializeOtg_V1().  HcdKit will have this
	USBPUMP_HCDKIT_OTG structure pointer.  HcdKit OTG common logic uses
	this structure.

	If HCD need to support OTG functionality, HCD initialization routine
	should call UsbPumpHcdKit_InitializeOtg_V1() after initialize HCD
	object.

Contents:
	USBPUMP_HCDKIT_HCD *pHcd;
		Back pointer for HcdKit object.

	USBPUMP_OBJECT_HEADER *pUsbPhy;
		USBPHY object header. This USBPHY object is required to send
		USBPHY IOCTLs.

	USBPUMP_HCDKIT_SWITCH_OTG Switch;
		The methods provided by the hardware-specific code for use
		by HcdKit.

	UINT32 bmOtgPort;
		Bitmap of OTG ports. Each bit represents OTG port or not.
		If port 1 is OTG port, HCD should set bit 1 to 1.

	UINT fManual_A_BUS_REQ: 1;
		This flag represents A_BUS_REQ mode. If this is TRUE, A_BUS_REQ
		mode is manual and application can handle A_BUS_REQ.  If this
		is FALSE, A_BUS_REQ mode is automatic and HCD handle A_BUS_REQ
		using ID pin state.

	BYTES Size;
		Size of allocated USBPUMP_HCDKIT_OTG structure.

	USBPUMP_USBDI_PORT *pPorts[];
		Save USBDI port object pointer for OTG port. In general one of
		root hub port will be OTG port.

Notes:
	None.

See Also:
	USBPUMP_HCDKIT_HCD
	USBPUMP_HCDKIT_SWITCH_OTG

*/

/* __TMS_TYPE_DEF_STRUCT(USBPUMP_HCDKIT_OTG); */

struct __TMS_STRUCTNAME(USBPUMP_HCDKIT_OTG)
	{
	__TMS_USBPUMP_HCDKIT_HCD *		pHcd;
	__TMS_USBPUMP_OBJECT_HEADER *		pUsbPhy;
	__TMS_USBPUMP_HCDKIT_SWITCH_OTG		Switch;
	__TMS_BYTES				Size;
	__TMS_UINT32				bmOtgPort;

	/* Internal flags */
	__TMS_UINT				fManual_A_BUS_REQ: 1;

	/* USBDI Port object which is OTG port. */
	__TMS_USBPUMP_USBDI_PORT *		pPorts[1];
	};



/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPumpHcdKit_InitializeOtg_V1(
	__TMS_USBPUMP_HCDKIT_HCD *			/* pHcd */,
	__TMS_USBPUMP_OBJECT_HEADER *			/* pPhyObject */,
	__TMS_UINT32					/* bmOtgPort */,
	__TMS_CONST __TMS_USBPUMP_HCDKIT_SWITCH_OTG *	/* pSwitch */,
	__TMS_USBPUMP_USBPHY_HCD_EVENT_FN *		/* pPhyHcdEventFn */,
	__TMS_USBPUMP_USBPHY_HCD_ISR_FN *		/* pPhyHcdIsrFn */
	);

__TMS_VOID
UsbPumpHcdKit_DeinitializeOtg(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */
	);

__TMS_USBPUMP_HCDKIT_SWITCH_IOCTL_FN
UsbPumpHcdKitI_OtgIoctl;

__TMS_USBPUMP_USBPHY_HCD_EVENT_FN
UsbPumpHcdKitI_OtgPhyEvent;

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpHcdKitI_OtgSend_A_BUS_DROP(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */,
	__TMS_BOOL			/* fDrop */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpHcdKitI_OtgSend_A_BUS_REQUEST(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */,
	__TMS_BOOL			/* fEnable */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpHcdKitI_OtgSend_B_BUS_REQUEST(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */,
	__TMS_BOOL			/* fEnable */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpHcdKitI_OtgSendHnpStatusEvent(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */,
	__TMS_UINT32			/* HnpStatus */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpHcdKitI_OtgSendIdChangeEvent(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */,
	__TMS_BOOL			/* fIdState */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpHcdKitI_OtgSendPortIdle(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpHcdKitI_OtgSendSrpDetectEvent(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */
	);

__TMS_USBPUMP_IOCTL_RESULT
UsbPumpHcdKitI_OtgSendWaitSrpEnable(
	__TMS_USBPUMP_HCDKIT_HCD *	/* pHcd */,
	__TMS_UINT			/* PortNum */,
	__TMS_BOOL			/* fEnable */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/


/**** end of usbpump_hcdkit_otg.h ****/
#endif /* _USBPUMP_HCDKIT_OTG_H_ */
