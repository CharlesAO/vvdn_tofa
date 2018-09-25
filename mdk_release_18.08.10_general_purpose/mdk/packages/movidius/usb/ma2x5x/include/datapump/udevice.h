/* udevice.h	Tue Jul 31 2018 13:49:39 chwon */

/*

Module:  udevice.h

Function:
	Home for the UDEVICE object

Version:
	V3.21g	Tue Jul 31 2018 13:49:39 chwon	Edit level 38

Copyright notice:
	This file copyright (C) 2002-2014, 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 23:03:40  tmm
	Module created.

   1.79b  9/2/2002  chwon
	Fix bug in macro generating UDEVICE name.

   1.91b  Sun Apr 24 2005 11:56:47 maw
   	Added udev_hSetup to UDEVICE

   1.91d  Sun May 29 2005 12:58:18  tmm
	Add documentation on the UDEVICE.

   1.91d  2005-05-31  tmm
	Fix some typos.

   1.91h  2005-08-28  tmm
	1533: Add support for tracking USB 2.0 information:
	udev_HnpEnable, udev_HnpSupport, udev_AltHnpSupport.

   1.93b  Wed Nov 16 2005 13:53:48  chwon
	1582: Remove udevhh_ulDebugFlags and use ulDebugFlags of
	USBPUMP_OBJECT_HEADER

   1.93c  Fri Dec 09 2005 09:12:05  cccho
	Add support for High-Speed information:
	udevh_pAllConfigs, udevhh_wNumAllConfigs and udevhh_bNumHSConfigs.

   1.97e  Sun Aug 20 2006 13:50:14  tmm
	Document how high-speed works.

   1.97f  Tue Dec 12 2006 13:14:44  chwon
	2164: add support for automatic remote wakeup: udev_fSuspendState,
	udev_fAutoRemoteWakeup

   1.97g  Wed Feb 21 2007 18:28:08  chwon
	2164: Change udev_fAutoRemoteWakeup to udev_nAutoRemoteWakeup for
	supporting multi-protocol. Add UDEVICE_CHECK_AUTO_REMOTE_WAKEUP()

   1.97j  Thu Oct 04 2007 18:18:53  chwon
	3253: add udev_bActiveConfigurationValue

   1.97j  Fri Dec 14 2007 10:03:43  chwon
	3595: add UsbPumpDevice_EdgeBusEvent() prototype.
	Generated uncloaked name using uncloak-def.sh.

   1.97k  Thu Mar 20 2008 13:48:51  chwon
	3735: reorganize UBUFQE to allow 32-bit sizes

   1.97k  Tue Apr 22 2008 10:40:37  chwon
	3746:  fix name cloaking for UBUFQEFLAG_xxx

   1.97k  Tue Sep 23 2008 15:05:48  cccho
	6683: add udev_L1RemoteWakeupEnable, udev_LinkState, and
	udev_fLpmEnable. Add prototypes for UsbPumpDeviceI_SetLinkState() and
	UsbPumpDevice_GetEffectiveRWE().

   1.97k  Wed Oct 08 2008 13:34:23  cccho
   	6829: create device abstract memory pool; change type of udev_pPoolHead
   	from UINT8 * to USBPUMP_ABSTRACT_POOL * , add udev_alignmentDevPool and
   	remove udev_pDevPool and udev_sizeDevPool.

   1.97k  Thu Oct 16 2008 11:52:12  cccho
	6865: correct typos from UDEVICE_IS_REMOVE_WAKUP_VALID to
	UDEVICE_IS_REMOTE_WAKUP_VALID.

   2.01a  Fri Mar 06 2009 11:16:54  cccho
	6865: correct typos from UDEVICE_IS_REMOTE_WAKUP_VALID to
	UDEVICE_IS_REMOTE_WAKEUP_VALID

   2.01a  Wed Nov 04 2009 13:16:34  chwon
	6796: add UsbPumpDevice_SetupActiveConfig() prototype

   3.01c  Wed Apr 21 2010 15:13:01  cccho
	10109: add device timer and support method. Add prototype for
	UsbPumpDevice_QueryBulkIntInPendingQe(). Update
	UDEVICE_CHECK_AUTO_REMOTE_WAKEUP() to make sure there is a bus idle
	period before remote wakeup.

   3.01d  Thu Sep 02 2010 12:24:02  cccho
	6150, 8071: add udev_DeviceFsm and move the device timer to DeviceFsm.

   3.01d  Mon Jan 03 2011 12:29:40  cccho
	12155: add udev_fValidateEpforAutoRemoteWakeup and 
	UsbPumpDevice_QueryRemoteWakeupTrafficPending() prototype.

   3.01f  Wed Apr 27 2011 23:02:43  chwon
	13269: added UsbPumpDevice_Init() prototype.

   3.01f  Fri Jul 01 2011 14:02:15  maw
	13233: Add device ep-activity state udev_EpActiveState, inactive
	IN endpint bitmap udev_bmInactiveInEndpoint.  Add
	UDEVICE_ENDPOINT_ACTIVITY_STATE

   3.01f  Thu Dec 15 2011 15:39:18  chwon
	14596: Add UsbPumpDevice_GetMaxControlMaxPacketSize() prototype.

   3.01f  Tue Jan 10 2012 15:06:50  chwon
	14623: Added udev_pFunctionWakeIfc, udev_fHwFeature_LtmCapable,
	udev_fU1Enable, udev_fU2Enable and udev_fLtmEnable in the UDEVICE.
	Added V2 remote wakeup validation macros. Added USB3 device link state.

   3.01f  Thu May 31 2012 10:29:24  chwon
	14821: Added UDEVICE_SUPPORTS_LPM_BESL().

   3.11b  Sat Nov 17 2012 11:59:31  chwon
	15804: Added udev_nActiveConfigurations in the UDEVICE.

   3.11b  Thu Dec 27 2012 13:34:28  chwon
	16401: Added udev_fDoNotAppendPortIndex flag in the UDEVICE.

   3.11b  Mon Jan 07 2013 13:39:43  chwon
	16402: Added udev_bOsDescVendorRequest and udev_fOsDescriptorEnable
	in the UDEVICE.

   3.11c  Tue Feb 26 2013 22:33:51  chwon
	16402: 16435: Removed udev_bOsDescVendorRequest and
	udev_fOsDescriptorEnable in the UDEVICE. Added udev_pControlProcessFn
	and udev_pControlProcessCtx in the UDEVICE.  Added MSFT OS descriptor
	handling function prototypes.

   3.11c  Thu Feb 28 2013 10:24:53  chwon
	16402: 16435: Corrected OS descriptor handling function name and added
	UsbPumpDevice_OsDescProcess() prototype.

   3.11d  Tue Dec 24 2013 12:28:03  chwon
	17961: add UsbPumpDevice_AllocateDeviceBuffer() and
	UsbPumpDevice_FreeDeviceBuffer() prototype.

   3.13a  Thu Apr 10 2014 11:01:49  chwon
	18173: Add udev_fSuperSpeedFeatureSetup in the UDEVICE.

   3.13b  Fri Aug 15 2014 12:07:10  chwon
	18461: Add udev_hUhilAux and udev_pUhilAuxInCall in the UDEVICE.

   3.13b  Wed Sep 17 2014 12:33:14  chwon
	18538: Add udev_bOsDescVendorCode and udev_fUseOwnOsDescVendorCode
	in the UDEVICE.

   3.21g  Tue Jul 31 2018 13:49:39  chwon
	20862: Add UsbPumpDevice_OsDescGetProperty prototype.

*/

#ifndef _UDEVICE_H_		/* prevent multiple includes */
#define _UDEVICE_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _UINTERFACESET_H_
# include "uinterfaceset.h"
#endif

#ifndef _UINTERFACE_H_
# include "uinterface.h"
#endif

#ifndef _UBUFQE_H_
# include "ubufqe.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

#ifndef _USBPUMP_DEVICE_FSM_H_
# include "usbpump_device_fsm.h"
#endif

#if !defined(__TMS_UDEVICE_COMPAT_fSuspendState)
# define __TMS_UDEVICE_COMPAT_fSuspendState	0
#endif

/*

Type:	UDEVICE

Index:	Macro:	UDEVICE_CONTENTS
	Macro:	UDEVICE_HDR
	Type:	union TTUSB_UDEVICE_UNION
	Constant:	UDEVICE_TAG

Function:
	The generic object that represents a USB device controller.

Description:
	A UDEVICE is the central context object for a given USB device
	instance within the DataPump.

Contents:
	USBPUMP_OBJECT_HEADER udev_Header;
		The standard USB Object header.  UDEVICE objects are
		entered into the object table with names matching
		"*.device.mcci.com", where "*" is determined by the
		chip driver.  The 4-byte object tag is UDEVICE_TAG, or
		'UDev' (arranged in byt order so it dumps as "UDev" on the
		target.  The name is computed using UDEVICE_NAME(text),
		which forms the string "{text}.device.mcci.com".

	UINT32 udev_ulDebugFlags;
		This field contains the DataPump debug flags.  If zero,
		no debug output is produced at runtime, even if the
		checked build of the DataPump is being used.  This is
		a bit mask.  See the definitions of the |UDMASK_...|
		flags for details.  For debugging purposes, you are permitted
		to change these as necessary at run-time.  This is initialized
		via the application initialization mechanisms.

	CONST USBRC_ROOTTABLE *udev_pDescriptorRoot;
		Pointer to the root descriptor table for this device instance.
		Initialized by the DataPump.

	CONST USBIF_DEVDESC_WIRE *udev_pDevDesc;
		Pointer to the device descriptor for this device instance,
		from the URC file.  This points to constant data, and
		applications are not permitted to modify this data.  If you
		need to override the device descriptor that is returned to
		the USB host, use a descriptor filter function.

	UINT16 udev_usbPortIndex;
		A utility variable, used for implementing the SNMP USB MIB,
		if needed.  It's set to 0 for the first UDEVICE created, to
		1 for the second UDEVICE created, and so forth.

	UINT8 udev_usbDeviceStatus;
		A utility variable, used for implementing the SNMP USB MIB.
		It's not used unless you have configured the DataPump for
		SNMP support, which is only available as a special order.

	UINT8 udev_usbDeviceAddress;
		A utility variable, used for implementing the SNMP USB MIB.
		It's not used unless you have configured the DataPump for
		SNMP support, which is only available as a special order.

	UINT32 udev_usbDeviceEnumCounter;
		A utility variable, used for implementing the SNMP USB MIB.
		It is incremented every time a set-address is received.
		Note, however, that we are not guaranteed that we can maintain
		this on all hardware, so you should not depend on this counter
		if you wish your code to be portable.

	UINT8 udev_bCurrentSpeed;
		The current speed (one of USBPUMP_DEVICE_SPEED_LOW,
		USBPUMP_DEVICE_SPEED_FULL, USBPUMP_DEVICE_SPEED_HIGH)

	UINT8 udev_bSupportedSpeeds;
		Bit mask of supported speeds.  Bit 0 for low, bit 1 for
		full, bit 2 for high.  [If zero, then the DataPump has
		no high-speed support at all, and bCurrentSpeed is not
		valid.]

	UINT8 udev_RemoteWakeupEnable;
		Controlled by the core DataPump; set TRUE if remote-wakeup is
		enabled.

	UINT8 udev_L1RemoteWakeupEnable;
		Controlled by the core DataPump; set TRUE if L1 remote-wakeup is
		enabled.

	UINT8 udev_LinkState;
		Controlled by the core DataPump; Set to the selected link power
		management state.

	UINT8 udev_fSuspendState;
		Controlled by the core DataPump; set TRUE if suspend state.

	UINTERFACE *udev_pFunctionWakeIfc;
		Controlled by the core DataPump; save interface pointer which
		send device notification for function remote wake.

	UINT16 udev_nFunctionWakeIfc;
		Controlled by the core DataPump; enable counter of interfaces
		that function remote wake feature is enabled.

	UINT16 udev_nAutoRemoteWakeup;
		Controlled by the core DataPump; enable counter of automatic
		remote wakeup feature. If zero, disabled automactic remote
		wakeup feature.

	UINT8 udev_bTestMode;
		Controlled by the core DataPump; set to the selected
		test mode (and non-zero) if the device is in test mode.

	UINT8 udev_fLpmEnable;
		Controlled by the core DataPump; set TRUE when LPM is enabled,
		FALSE otherwise.

	UINT8 udev_HnpEnable;
		Controlled by the core DataPump; set TRUE when HNP is
		enabled, FALSE otherwise.

	UINT8 udev_HnpSupport;
		Controlled by the core DataPump; set TRUE when HNP is
		supported by the host controller (as determined by
		receipt of the OTG HNP support set_feature.

	UINT8 udev_AltHnpSupport;
		Controlled by the core DataPump; set TRUE when HNP is
		supported by the host on some ports on the host controller
		but not necessariliy on this one.

	UINT8 udev_EpActiveState;
		Controlled by the core DataPump; Initialized by device FSM
		init fn, and read by core DataPump to select proper initial 
		value for EP uep_fActive

	UCONFIG *udev_pConfigs;
	UINT8 udev_bNumConfigs;
		Pointer to the array of active |UCONFIG| objects, of length
		given by |udev_bNumConfigs| .  This array defines the
		availalbe configurations for the current speed.  This vector
		is updated during USB Reset processing.

	UCONFIG *udev_pAllConfigs;
	UINT16 udev_wNumAllConfigs;
		Pointer to an array of all |UCONFIG| objects defined for
		this device.  The vector consists of all the full(low)-speed
		config descriptors, followed by all the high-speed config
		descriptors.  (The architecture assumes that devices are
		either low-speed only, full-speed only, or full-speed +
		high-speed.)

	UINT8 udev_bNumHSConfigs;
		Number of high-speed |UCONFIG| objects defiend in
		udev_pAllConfigs.  Since full-speed |UCONFIG| objects
		come first, this value is used in conjunction with
		udev_pAllConfigs and udev_wNumAllConfigs to compute
		the proper values for udev_pConfigs and udev_bNumConfigs
		based on the current speed.

	UINT8 udev_bActiveConfigurationValue;
		Set by the core DataPump before report device event; it is
		configuration value of active configuration.  If device has
		multiple confiuration, it will represent current active
		configuration value.
		When device is configured, it is bConfigurationValue of current
		configuration descriptor.
		When device is not configured yet, it can be zero following case.
		1) device does not support USBPUMP_IOCTL_GET_VIDPIDMODE IOCTL
		2) device has multiple configuration and all configurations are
		   active (device support USBPUMP_IOCTL_GET_VIDPIDMODE and it
		   return ulMode == 0).

	UINT8 udev_nActiveConfigurations;
		Set by the core DataPump before report device event; it is
		number of active configurations.  If device have multiple
		configurations, it will represent how many configuration is
		active now.

	UINT8 udev_bOsDescVendorCode;
		Os descriptor vendor code. This value will be used only if
		udev_fUseOwnOsDescVendorCode is TRUE.

	UCONFIG *udev_pCurrent;
		Pointer to the current |UCONFIG| object, or NULL if not
		configured.

	UPLATFORM *udev_pPlatform;
		Up-pointer to the platorm associated with this UDEVICE.

	UEVENTNODE *udev_noteq;
		Head of the queue of UEVENTNODES linked to this UDEVICE.
		These are attached by clients for receiving host-initiated
		events (or bus-initiated events) that are relevant to this
		UDEVICE.

	VOID *udev_pExtension;
		Arbitrary pointer for use by clients.  Not used by MCCI
		supplied code or protocol implementations.

	USBPUMP_CONTROL_PROCESS_FN *udev_pControlProcessFn;
	VOID *udev_pControlProcessCtx;
		Client registered control packet process function pointer and
		context pointer.  The udev_pControlProcessFn will be registered
		by USBPUMP_IOCTL_DEVICE_REGISTER_CONTROL_PROCESS_FN ioctl.
		This function will be called by UsbProcessControlPacket()
		before do common control packet process.

	The following variables form an interface between the portable code
	and the chip driver(s).

	CONST UDEVICESWITCH *udev_pSwitch;
		Pointer to the object that defines the entry-points into
		the chip driver for the hardware associated with this UDEVICE>

	UENDPOINT *udev_pEndpoints;
	UINT8	udev_bNumEndpoints;
		Pointer to the first entry in the array of UENDPOINT structures
		created by the chip driver. One UENDPOINT must exist for each
		physical endpoint.

		The first two endpoints are used for endpoint 0 (out and in).

		Since UENDPOINTs are abstract (like UDEVICE), this array
		cannot be directly indexed; API functions must be used.

	UINT8 *udev_pReplyBuf;
	BYTES udev_sizeReplyBuf;
		For simplicity, a small buffer is allocated for computing
		replies to standard commands.  Since only one command can be
		generated at a time by the host, only one buffer is needed.
		The size of this buffer is fixed at build time, but the buffer
		is allocated from "device memory" (i.e, memory that this
		device accepts for DMA transfers).

	USBPUMP_ABSTRACT_POOL *udev_pPoolHead;
	ADDRBITS udev_alignmentDevPool;
		device memory pool alignment.

	UINTERFACESET *udev_pvAllInterfaceSets;
	UINT8	udev_bNumAllInterfaceSets;
		Pointer to a vector of UINTERFACESET objects, allocated at
		initialization time by code generated by USBRC.

	UINTERFACE *udev_pvAllInterfaces;
	UINT8	udev_bNumAllInterfaces;
		Pointer to the vector of UINTERFACE objects, one per
		interface descriptor in the descriptor set, allocated at
		initialization time by code generated by USBRC.

	UINT16	udev_bmInactiveInEndpoint;
		bitmapped filed identify the inactive IN endpoints.

	UPIPE *udev_pvAllPipes;
	UINT16 udev_wNumAllPipes;
		Pointer to the vecord of UPIPE objects, one pipe per
		endpoint descriptor in the descriptor set, allocated at
		initialization time by code generated by USBRC.

	USBPUMP_SESSION_HANDLE udev_hUhilAux;
	USBPUMP_UHILAUX_INCALL *udev_pUhilAuxInCall;
		UHILAUX session handle and pointer to the UHILAUX incall
		switch table.  If DCD doesn't support UHILAUX operation,
		DCD should not set these variables.  By default, DCD doesn't
		support UHILAUX operations.

Private Contents:
	UINTERFACESET *udev_pCtrlIfcset;
		To simplify code and handling of events when in the
		unconfigured state, a UINTERFACESET is provided for internal
		use.  DataPump initializations sets this pointer to the
		internal "unconfigured" UINTERFACESET.

	UINTERFACESET udev_ctlifcset;
	UINTERFACE udev_ctlifc;
	UBUFQE udev_ctlsetupbq;
	UBUFQE udev_ctlinbq;
	UBUFQE udev_ctloutbq;
		Used by the DataPump core to implement chapter-9 operations.

	The following variables are bit flags for internal usage.

	UINT32 udev_fHwFeature_LtmCapable: 1;
		Device hardware feature that support LTM or not. Chip driver
		report this feature at initialization time.

	UINT32 udev_fValidateEpforAutoRemoteWakeup: 1;
		This flag represents we need to validate endpoint for automatic
		remote wakeup.  This flag will be modified by
		USBPUMP_IOCTL_SET_AUTO_REMOTE_WAKEUP_STATE ioctl.

	UINT32 udev_fU1Enable: 1;
	UINT32 udev_fU2Enable: 1;
	UINT32 udev_fLtmEnable: 1;
		This flags represents U1/U2/LTM is enabled or not. This flags
		will be set by SETUP command from host.

	UINT32 udev_fDoNotAppendPortIndex: 1;
		This flag represents we don't need to append port index at the
		end of serial number string.  This flag will be set to TRUE at
		the DataPump initialization time if number of port is 1.

	UINT32 udev_fSuperSpeedFeatureSetup: 1;
		This flag set TRUE if DataPump set up super-speed feature.

	UINT32 udev_fUseOwnOsDescVendorCode: 1;
		This flag set TRUE if DataPump needs to use UDEVICE::
		udev_bOsDescVendorCode for OS descriptor vendor code.

Notes:
	A |UDEVICE| object is actually the generic header of a more specific
	object.  Rather than use VOID* pointers, the data structures are
	overlaid using a complex (but efficient) combination of unions and
	structures.  Therefore, you will not find code that allocates a
	|UDEVICE| per-se; instead, you'll find code that allocates objects
	specific to an application, which in turn contain the object defined
	by the chip driver for the device, which in turn contains the
	|UDEVICE|.  So the derivation tree is:

		UDEVICE -->
			UDEV... (for example, UDEV820)
				--> UDEVICE_... (based on application in URC
					file).

	In memory, it looks like this:

		UDEVICE variables
		UDEV... variables (for chip driver)
		UDEVICE_... variables (for application)

	When defining a chip-driver derived UDEVICE object, the standard way
	to do it is to first create a 'header' structure, for example:

.	struct TTUSB_UDEV820_HDR
.		{
.		UDEVICE_HDR;	// room for the constant header
.
.		// other things specifically needed by the USS820
.		IOPORT	udev820hh_ioport;
.
.		// ...
.		};

	Then define a union, containing the common part and the part for the
	chip driver (with a name you define):

.	#define	UDEV820_CONTENTS	\
.		UDEVICE_CONTENTS;	\
.		struct TTUSB_UDEV820_HDR udevhdr_udev820

	Then define the chip driver object:

.	union _UDEV820_UNION
.		{
.		__TMS_UDEV820_CONTENTS;
.		};

	Define a macro for inserting the UDEV820 header into application
	objects:

.	#define	UDEV820_HDR	union TTUSB_UDEV820_UION udev_hdr

	Define the chip driver abstract object:

.	typedef struct TTUSB_UDEV820
.		{
.		UDEV820_HDR:
.		}

	Finally, define the sane names:

.	#define	udev820_ioport	udev_hdr.udevhdr_udev820.udevhh_uss820
.	// ...


	Now any UDEV820 object pointer can be used to access the 'uss820' fields and
	the 'UDEVICE' fields.

See Also:
	UCONFIG, UENDPOINT, UINTERFACE, UINTERFACESET, UPLATFORM, usbrc

Bugs:
	In order to simplify naming, the macros udev_... are defined to select
	the appropriate field.  These can be used with a UDEVICE, a UDEV...,
	or a UDEVICE_... object, as needed.  However, all names of the form
	udev_... should be treated as reserved for use by the DataPump.  You
	should define no names or macros beginning with this prefix in code
	that must include DataPump header files.

*/

/****************************************************************************\
|
| The UDEVICE is the top level structure.
|
\****************************************************************************/

/**** the contents of the embedded header ****/
struct TTUSB_UDEVICE_HDR
	{
	__TMS_USBPUMP_OBJECT_HEADER udevhh_Header;

	/* the root table pointer */
	__TMS_CONST __TMS_USBRC_ROOTTABLE *udevhh_pDescriptorRoot;

	/* the pointer to our device descriptor */
	__TMS_CONST __TMS_USBIF_DEVDESC_WIRE *udevhh_pDevDesc;

	__TMS_USBPUMP_DEVICE_FSM	udevhh_DeviceFsm;

	/*
	|| the USB mib port index, but zero-origin; fields were added
	|| in V1.60n, but support is incomplete.
	*/
	__TMS_UINT16	udevhh_usbPortIndex;	/* the port index, 0-origin */

	/*
	|| The status, from the USB mib.
	*/
	__TMS_UINT8	udevhh_usbDeviceStatus;	/* the current device status */
	/*
	|| The current address, because it's nice to have...
	*/
	__TMS_UINT8	udevhh_usbDeviceAddress; /* the current address */

	/*
	|| How many times have we been enumerated?  Not clear from the
	|| mib what this really is supposed to count....
	*/
	__TMS_UINT32	udevhh_usbDeviceEnumCounter; /* count of 'enumerations' */

	/* 'configurations' -- the vector, and the current one */
	__TMS_UCONFIG	*udevhh_pAllConfigs;	/* vector of known configs for all speed */
	__TMS_UCONFIG	*udevhh_pConfigs;	/* vector of known configs */
	__TMS_UCONFIG	*udevhh_pCurrent;	/* current configuration; or NULL if
						|| not configured.
						*/
	__TMS_VOID	*udevhh_pExtension;	/* application extension */
	__TMS_UPLATFORM *udevhh_pPlatform;	/* pointer to platform for this
						|| device
						*/
	__TMS_CONST __TMS_UDEVICESWITCH *udevhh_pSwitch;
						/* hardware interface switch */
	__TMS_UEVENTNODE *udevhh_noteq;		/* notification list */
	__TMS_UINTERFACESET *udevhh_pCtrlIfcset; /* default ifcset */
	__TMS_UENDPOINT *udevhh_pEndpoints;	/* pointer to array of endpoints */

	__TMS_UINT8	*udevhh_pReplyBuf;	/* pointer to reply buffer */
	__TMS_USBPUMP_ABSTRACT_POOL
			*udevhh_pPoolHead;	/* current pool head */

	/* bookkeeping for initialization */
	__TMS_UINTERFACESET *udevhh_pvAllInterfaceSets;
	__TMS_UINTERFACE *udevhh_pvAllInterfaces;
	__TMS_UPIPE	*udevhh_pvAllPipes;

	/* numerical constants and whatnot */
	__TMS_SIZE_T	udevhh_sizeDevPool;	/* size of pool */
	__TMS_ADDRBITS	udevhh_alignmentDevPool;
	__TMS_BYTES	udevhh_sizeReplyBuf;	/* size of reply buffer */
	__TMS_UINT16	udevhh_wNumAllPipes;	/* number of pipes, total */
	__TMS_UINT8	udevhh_bCurrentSpeed;	/* the device speed */
	__TMS_UINT8	udevhh_bSupportedSpeeds; /* the device speed set */
	__TMS_UINT8	udevhh_RemoteWakeupEnable; /* if TRUE, remote wakeup is
						|| enabled
						*/
	__TMS_UINT8	udevhh_L1RemoteWakeupEnable; /* if TRUE, L1 remote
						|| wakeup is enabled
						*/
	__TMS_UDEVICE_LINK_STATE udevhh_LinkState;	/* link state */
	__TMS_UINT8	udevhh_fSuspendState;	/* if TRUE, suspend state */
	__TMS_UINTERFACE *	udevhh_pFunctionWakeIfc;
						/*
						|| save function wake interface
						*/
	__TMS_UINT16	udevhh_nFunctionWakeIfc;
	__TMS_UINT16	udevhh_nAutoRemoteWakeup;
						/* enable counter of automatic
						|| remote wakeup feature.
						*/
	__TMS_UINT8	udevhh_bTestMode;	/* if non-zero, the most
						|| recently-selected test
						|| mode.  This cannot be
						|| cleared because the spec
						|| requires that this only
						|| be exited by power cycle.
						*/
	__TMS_UINT8	udevhh_fLpmEnable;	/* if TRUE, LPM is enabled */
	__TMS_UINT8	udevhh_HnpEnable;	/* if TRUE, HNP is
						|| enabled.
						*/
	__TMS_UINT8	udevhh_HnpSupport;	/* if TRUE, HNP is supported
						|| on this port.
						*/
	__TMS_UINT8	udevhh_AltHnpSupport;	/* if TRUE, HNP is supported
						|| somewhere on this host.
						*/
	__TMS_UINT8	udevhh_EpActiveState;	/* config for uep_fActive */
	__TMS_UINT16	udevhh_wNumAllConfigs;	/* number of all configs */
	__TMS_UINT8	udevhh_bNumConfigs;	/* number of configs */
	__TMS_UINT8	udevhh_bNumHSConfigs;	/* number of HS configs */
	__TMS_UINT8	udevhh_bNumEndpoints;	/* number of endpoints */
	__TMS_UINT8	udevhh_bNumAllInterfaceSets; /* length of vAllInterfaceSets */
	__TMS_UINT8	udevhh_bNumAllInterfaces; /* length of vAllInterfaces */

	__TMS_UINT8	udevhh_bActiveConfigurationValue;
						/* active configuration value */
	__TMS_UINT8	udevhh_nActiveConfigurations;
	__TMS_UINT8	udevhh_bOsDescVendorCode;

	__TMS_UINT16	udevhh_bmInactiveInEndpoint;

	/* The control interface information */
	__TMS_UINTERFACESET	udevhh_ctlifcset;
	__TMS_UINTERFACE	udevhh_ctlifc;
	__TMS_UBUFQE		udevhh_ctlsetupbq;
	__TMS_UBUFQE		udevhh_ctlinbq;
	__TMS_UBUFQE		udevhh_ctloutbq;
	__TMS_USETUP_HANDLE	udevhh_hSetup;

	/* The feature flags */
	__TMS_UINT32		udevhh_fHwFeature_LtmCapable: 1;
	__TMS_UINT32		udevhh_fValidateEpforAutoRemoteWakeup: 1;
	__TMS_UINT32		udevhh_fU1Enable: 1;
	__TMS_UINT32		udevhh_fU2Enable: 1;
	__TMS_UINT32		udevhh_fLtmEnable: 1;
	__TMS_UINT32		udevhh_fDoNotAppendPortIndex: 1;
	__TMS_UINT32		udevhh_fSuperSpeedFeatureSetup: 1;
	__TMS_UINT32		udevhh_fUseOwnOsDescVendorCode: 1;

	__TMS_USBPUMP_CONTROL_PROCESS_FN *
				udevhh_pControlProcessFn;
	__TMS_VOID *		udevhh_pControlProcessCtx;

	/*
	|| UHILAUX
	*/
	__TMS_USBPUMP_SESSION_HANDLE	udevhh_hUhilAux;
	__TMS_USBPUMP_UHILAUX_INCALL *	udevhh_pUhilAuxInCall;
	};

/**** the union contents ****/
#define	__TMS_UDEVICE_CONTENTS	struct TTUSB_UDEVICE_HDR udevhdr_udevice

union TTUSB_UDEVICE_UNION
	{
	__TMS_UDEVICE_CONTENTS;	/* the contents of the union */
	};

/**** the macro to embed in derived structures ****/
#define	__TMS_UDEVICE_HDR	union TTUSB_UDEVICE_UNION udev_hdr

/**** the generic structure ****/
struct TTUSB_UDEVICE
	{
	__TMS_UDEVICE_HDR;
	};

/**** sane names ****/
#define	udev_Header		udev_hdr.udevhdr_udevice.udevhh_Header
#define	udev_ulDebugFlags	udev_hdr.udevhdr_udevice.udevhh_Header.ulDebugFlags
#define	udev_usbPortIndex	udev_hdr.udevhdr_udevice.udevhh_usbPortIndex
#define	udev_usbDeviceStatus	udev_hdr.udevhdr_udevice.udevhh_usbDeviceStatus
#define	udev_usbDeviceAddress	udev_hdr.udevhdr_udevice.udevhh_usbDeviceAddress

#define	udev_usbDeviceEnumCounter \
				udev_hdr.udevhdr_udevice.udevhh_usbDeviceEnumCounter
#define	udev_pDescriptorRoot	udev_hdr.udevhdr_udevice.udevhh_pDescriptorRoot
#define	udev_pDevDesc		udev_hdr.udevhdr_udevice.udevhh_pDevDesc
#define	udev_DeviceFsm		udev_hdr.udevhdr_udevice.udevhh_DeviceFsm
#define udev_pAllConfigs	udev_hdr.udevhdr_udevice.udevhh_pAllConfigs
#define	udev_pConfigs		udev_hdr.udevhdr_udevice.udevhh_pConfigs
#define	udev_pCurrent		udev_hdr.udevhdr_udevice.udevhh_pCurrent
#define	udev_pExtension		udev_hdr.udevhdr_udevice.udevhh_pExtension
#define	udev_pPlatform		udev_hdr.udevhdr_udevice.udevhh_pPlatform
#define	udev_pSwitch		udev_hdr.udevhdr_udevice.udevhh_pSwitch
#define	udev_noteq		udev_hdr.udevhdr_udevice.udevhh_noteq
#define	udev_pCtrlIfcset	udev_hdr.udevhdr_udevice.udevhh_pCtrlIfcset
#define	udev_pEndpoints		udev_hdr.udevhdr_udevice.udevhh_pEndpoints
#define	udev_pReplyBuf		udev_hdr.udevhdr_udevice.udevhh_pReplyBuf
#define	udev_pPoolHead		udev_hdr.udevhdr_udevice.udevhh_pPoolHead
#define	udev_pvAllInterfaceSets	udev_hdr.udevhdr_udevice.udevhh_pvAllInterfaceSets
#define	udev_pvAllInterfaces	udev_hdr.udevhdr_udevice.udevhh_pvAllInterfaces
#define	udev_pvAllPipes		udev_hdr.udevhdr_udevice.udevhh_pvAllPipes
#define	udev_alignmentDevPool	udev_hdr.udevhdr_udevice.udevhh_alignmentDevPool
#define	udev_sizeReplyBuf	udev_hdr.udevhdr_udevice.udevhh_sizeReplyBuf
#define	udev_wNumAllPipes	udev_hdr.udevhdr_udevice.udevhh_wNumAllPipes
#define	udev_bCurrentSpeed	udev_hdr.udevhdr_udevice.udevhh_bCurrentSpeed
#define	udev_bSupportedSpeeds	udev_hdr.udevhdr_udevice.udevhh_bSupportedSpeeds
#define	udev_RemoteWakeupEnable udev_hdr.udevhdr_udevice.udevhh_RemoteWakeupEnable
#define	udev_L1RemoteWakeupEnable udev_hdr.udevhdr_udevice.udevhh_L1RemoteWakeupEnable
#define	udev_LinkState		udev_hdr.udevhdr_udevice.udevhh_LinkState
#if __TMS_UDEVICE_COMPAT_fSuspendState
# define udev_fSuspendState	udev_hdr.udevhdr_udevice.udevhh_fSuspendState
#endif
#define	udev_pFunctionWakeIfc	udev_hdr.udevhdr_udevice.udevhh_pFunctionWakeIfc
#define	udev_nFunctionWakeIfc	udev_hdr.udevhdr_udevice.udevhh_nFunctionWakeIfc
#define	udev_nAutoRemoteWakeup	udev_hdr.udevhdr_udevice.udevhh_nAutoRemoteWakeup
#define	udev_bTestMode		udev_hdr.udevhdr_udevice.udevhh_bTestMode
#define udev_fLpmEnable		udev_hdr.udevhdr_udevice.udevhh_fLpmEnable
#define	udev_HnpEnable		udev_hdr.udevhdr_udevice.udevhh_HnpEnable
#define	udev_HnpSupport		udev_hdr.udevhdr_udevice.udevhh_HnpSupport
#define	udev_AltHnpSupport	udev_hdr.udevhdr_udevice.udevhh_AltHnpSupport
#define	udev_EpActiveState	udev_hdr.udevhdr_udevice.udevhh_EpActiveState
#define udev_wNumAllConfigs	udev_hdr.udevhdr_udevice.udevhh_wNumAllConfigs
#define	udev_bNumConfigs	udev_hdr.udevhdr_udevice.udevhh_bNumConfigs
#define udev_bNumHSConfigs	udev_hdr.udevhdr_udevice.udevhh_bNumHSConfigs
#define	udev_bNumEndpoints	udev_hdr.udevhdr_udevice.udevhh_bNumEndpoints
#define	udev_bNumAllInterfaceSets udev_hdr.udevhdr_udevice.udevhh_bNumAllInterfaceSets
#define	udev_bNumAllInterfaces	udev_hdr.udevhdr_udevice.udevhh_bNumAllInterfaces
#define	udev_bActiveConfigurationValue udev_hdr.udevhdr_udevice.udevhh_bActiveConfigurationValue
#define	udev_nActiveConfigurations udev_hdr.udevhdr_udevice.udevhh_nActiveConfigurations
#define	udev_bOsDescVendorCode	udev_hdr.udevhdr_udevice.udevhh_bOsDescVendorCode
#define	udev_bmInactiveInEndpoint udev_hdr.udevhdr_udevice.udevhh_bmInactiveInEndpoint
#define	udev_ctlifcset		udev_hdr.udevhdr_udevice.udevhh_ctlifcset
#define	udev_ctlifc		udev_hdr.udevhdr_udevice.udevhh_ctlifc
#define	udev_ctlsetupbq		udev_hdr.udevhdr_udevice.udevhh_ctlsetupbq
#define	udev_ctlinbq		udev_hdr.udevhdr_udevice.udevhh_ctlinbq
#define	udev_ctloutbq		udev_hdr.udevhdr_udevice.udevhh_ctloutbq
#define	udev_hSetup		udev_hdr.udevhdr_udevice.udevhh_hSetup
#define	udev_fHwFeature_LtmCapable udev_hdr.udevhdr_udevice.udevhh_fHwFeature_LtmCapable
#define	udev_fValidateEpforAutoRemoteWakeup udev_hdr.udevhdr_udevice.udevhh_fValidateEpforAutoRemoteWakeup
#define	udev_fU1Enable		udev_hdr.udevhdr_udevice.udevhh_fU1Enable
#define	udev_fU2Enable		udev_hdr.udevhdr_udevice.udevhh_fU2Enable
#define	udev_fLtmEnable		udev_hdr.udevhdr_udevice.udevhh_fLtmEnable
#define	udev_fDoNotAppendPortIndex udev_hdr.udevhdr_udevice.udevhh_fDoNotAppendPortIndex
#define	udev_fSuperSpeedFeatureSetup udev_hdr.udevhdr_udevice.udevhh_fSuperSpeedFeatureSetup
#define	udev_fUseOwnOsDescVendorCode udev_hdr.udevhdr_udevice.udevhh_fUseOwnOsDescVendorCode
#define	udev_pControlProcessFn	udev_hdr.udevhdr_udevice.udevhh_pControlProcessFn
#define	udev_pControlProcessCtx	udev_hdr.udevhdr_udevice.udevhh_pControlProcessCtx
#define	udev_hUhilAux		udev_hdr.udevhdr_udevice.udevhh_hUhilAux
#define	udev_pUhilAuxInCall	udev_hdr.udevhdr_udevice.udevhh_pUhilAuxInCall
/**** finally, a few (deprecated) aliases ****/
#define	udev_pDefaultPipe	udev_pvAllPipes

/*
|| Object related things
*/

/* the USB device object tag */
#define	__TMS_UDEVICE_TAG	__TMS_UHIL_MEMTAG('U', 'D', 'e', 'v')

/* the default name */
#define	__TMS_UDEVICE_NAME(Text)  Text ".device.mcci.com"


/* Remote wakeup validation macro */
#define	UDEVICE_IS_REMOTE_WAKEUP_VALID(pSelf)		\
	UDEVICE_IS_REMOTE_WAKEUP_VALID_V2(pSelf, __TMS_NULL)

#define	UDEVICE_IS_AUTO_REMOTE_WAKEUP_ENABLE(pSelf)	\
	((pSelf)->udev_nAutoRemoteWakeup != 0)

#define	UDEVICE_IS_AUTO_REMOTE_WAKEUP_DISABLE(pSelf)	\
	((pSelf)->udev_nAutoRemoteWakeup == 0)

#define	UDEVICE_IS_AUTO_REMOTE_WAKEUP_VALID(pSelf)	\
	UDEVICE_IS_AUTO_REMOTE_WAKEUP_VALID_V2(pSelf, __TMS_NULL)

#define UDEVICE_CHECK_AUTO_REMOTE_WAKEUP(pSelf)		\
	UDEVICE_CHECK_AUTO_REMOTE_WAKEUP_V2(pSelf, __TMS_NULL)

/* Remote wakeup validation macro for super speed device */
#define	UDEVICE_IS_REMOTE_WAKEUP_VALID_V2(pSelf, pInterface)	\
	UsbPumpDevice_GetEffectiveRWE_V2(pSelf, pInterface)

#define	UDEVICE_IS_AUTO_REMOTE_WAKEUP_VALID_V2(pSelf, pInterface)	\
	(UDEVICE_IS_AUTO_REMOTE_WAKEUP_ENABLE(pSelf) &&			\
	 UDEVICE_IS_REMOTE_WAKEUP_VALID_V2(pSelf, pInterface))

#define UDEVICE_CHECK_AUTO_REMOTE_WAKEUP_V2(pSelf, pInterface)	\
	UsbPumpDevice_CheckAutoRemoteWakeup(pSelf, pInterface)


/*

Type:	UDEVICE_LINK_STATE

Index:	Constant:	UDEVICE_LINK_STATE_L0
	Constant:	UDEVICE_LINK_STATE_L1
	Constant:	UDEVICE_LINK_STATE_L2
	Constant:	UDEVICE_LINK_STATE_L3
	Constant:	UDEVICE_LINK_STATE_NOCABLE
	Constant:	UDEVICE_LINK_STATE_NOVBUS
	Constant:	UDEVICE_LINK_STATE_U0
	Constant:	UDEVICE_LINK_STATE_U1
	Constant:	UDEVICE_LINK_STATE_U2
	Constant:	UDEVICE_LINK_STATE_U3

Function:
	 State codes for the link power management.

Definition:
	typedef UINT8 UDEVICE_LINK_STATE, *PUDEVICE_LINK_STATE;
        typedef ARG_UINT8 ARG_UDEVICE_LINK_STATE, *PARG_UDEVICE_LINK_STATE; 

Contents:
	The |UDEVICE_LINK_STATE| type is used to represent link power management
	states.

        |UDEVICE_LINK_STATE_L0| (USB2: On) 
		The port is enabled for propagation of transaction signaling
		traffic.  While in this state Start-of-Frame (SOF) packets are
		issued by the host at a rate corresponding to the speed of the
		client device (1ms for full-speed, 125us for high-speed).

	|UDEVICE_LINK_STATE_L1| (USB2: Sleep)
		Similar to L2 suspend state but supports finer granularity in
		use. When in L1, the line state is identical to L2 (see below).

	|UDEVICE_LINK_STATE_L2| (USB2: Suspend)
		This is the formalized name for USB 2.0 Suspend state.

	|UDEVICE_LINK_STATE_L3| (USB2: Off)
		The port is not capable of performing any data signaling.
		It corresponds to the powered-off, disconnected and disabled
		states.

	|UDEVICE_LINK_STATE_NOCABLE|
		Not connected at all

	|UDEVICE_LINK_STATE_NOVBUS|
		Connected but no VBus

	|UDEVICE_LINK_STATE_DISABLE|
		Connected and has VBus, but the port is not enabled yet.

        |UDEVICE_LINK_STATE_U0| (USB3: U0) 
		The port is enabled for propagation of transaction signaling
		traffic. This is identical to UDEVICE_LINK_STATE_L0 state.

	|UDEVICE_LINK_STATE_U1| (USB3: U1)
		This state is low power state where no packets are to be
		transmitted and both ports agree to enter a link state where
		a SuperSpeed PHY can be placed into a low power state.

	|UDEVICE_LINK_STATE_U2| (USB3: U2)
		This state is a link state where more power saving
		opportunities are allowed compare to U1, but with an increased
		exit latency.

	|UDEVICE_LINK_STATE_U3| (USB3: Suspend, U3)
		USB3: This state is a link state where a device is put into a
		suspend state. Significant link and device powers are saved.

*/

/* __TMS_TYPE_DEF_ARG(UDEVICE_LINK_STATE, UINT8); */

#define	__TMS_UDEVICE_LINK_STATE_L0		0
#define	__TMS_UDEVICE_LINK_STATE_L1		1
#define	__TMS_UDEVICE_LINK_STATE_L2		2
#define	__TMS_UDEVICE_LINK_STATE_L3		3
#define	__TMS_UDEVICE_LINK_STATE_NOCABLE	4
#define	__TMS_UDEVICE_LINK_STATE_NOVBUS		5
#define	__TMS_UDEVICE_LINK_STATE_DISABLE	6
#define	__TMS_UDEVICE_LINK_STATE_U0		7
#define	__TMS_UDEVICE_LINK_STATE_U1		8
#define	__TMS_UDEVICE_LINK_STATE_U2		9
#define	__TMS_UDEVICE_LINK_STATE_U3		10

#define	__TMS_UDEVICE_LINK_STATE_NAMES__INIT	\
	{					\
	"UDEVICE_LINK_STATE_L0",		\
	"UDEVICE_LINK_STATE_L1",		\
	"UDEVICE_LINK_STATE_L2",		\
	"UDEVICE_LINK_STATE_L3",		\
	"UDEVICE_LINK_STATE_NOCABLE",		\
	"UDEVICE_LINK_STATE_NOVBUS",		\
	"UDEVICE_LINK_STATE_DISABLE",		\
	"UDEVICE_LINK_STATE_U0",		\
	"UDEVICE_LINK_STATE_U1",		\
	"UDEVICE_LINK_STATE_U2",		\
	"UDEVICE_LINK_STATE_U3"			\
	}

#define	__TMS_UDEVICE_IS_SUSPEND_STATE(pDevice)				\
	(((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_L2) ||	\
	 ((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_L1) ||	\
	 ((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_U3))

#define	__TMS_UDEVICE_GET_LINK_STATE(pDevice)	\
	(pDevice)->udev_LinkState

#define __TMS_UDEVICE_IS_L0_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_L0)

#define __TMS_UDEVICE_IS_L1_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_L1)

#define __TMS_UDEVICE_IS_L2_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_L2)

#define __TMS_UDEVICE_IS_L3_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_L3)

#define __TMS_UDEVICE_IS_U0_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_U0)

#define __TMS_UDEVICE_IS_U1_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_U1)

#define __TMS_UDEVICE_IS_U2_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_U2)

#define __TMS_UDEVICE_IS_U1_U2_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_U1 || \
	 (pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_U2)

#define __TMS_UDEVICE_IS_U3_STATE(pDevice)	\
	((pDevice)->udev_LinkState == __TMS_UDEVICE_LINK_STATE_U3)


#define	__TMS_UDEVICE_RECORD_LPM_FEATURE(pDevice, bmAttribute)	\
    do	{							\
	(pDevice)->udev_fLpmEnable =				\
		(bmAttribute) & ((1 << 1) | (1 << 2));		\
	} while(0)

#define __TMS_UDEVICE_SUPPORTS_LPM(pDevice)		\
	((pDevice)->udev_fLpmEnable & (1 << 1))

#define __TMS_UDEVICE_SUPPORTS_LPM_BESL(pDevice)	\
	((pDevice)->udev_fLpmEnable & (1 << 2))

/*

Type:	UDEVICE_ENDPOINT_ACTIVITY_STATE

Index:	Constant:	UDEVICE_ENDPOINT_ACTIVITY_STATE_DISABLED
	Constant:	UDEVICE_ENDPOINT_ACTIVITY_STATE_WAIT_ACTIVE
	Constant:	UDEVICE_ENDPOINT_ACTIVITY_STATE_ACTIVE

Function:
	 State codes for the endpoint active management

Definition:
	typedef UINT8 UDEVICE_ENDPOINT_ACTIVITY_STATE,
		*PUDEVICE_ENDPOINT_ACTIVITY_STATE;
	typedef ARG_UINT8 ARG_UDEVICE_ENDPOINT_ACTIVITY_STATE,
		*PARG_UDEVICE_ENDPOINT_ACTIVITY_STATE; 

Contents:
	The |UDEVICE_ENDPOINT_ACTIVITY_STATE| type is used to represent
	endpoint active management state.

        |UDEVICE_ENDPOINT_ACTIVITY_STATE_DISABLED|
		The DCD does not support endpoint active handling, therefore
		all endpoints should be left active.

	|UDEVICE_ENDPOINT_ACTIVITY_STATE_WAIT_ACTIVE|
		We've not seen any traffic at all since the last SET_CONFIG or
		RESUME (whichever is later).  Once we see traffic from the
		host (indicated by a NAK interrupt) then we can move to
		ACTIVE state, and we can also enable the endpoint at the
		hardware level so we'll receive the next attempt to send data.

	|UDEVICE_ENDPOINT_ACTIVITY_STATE_ACTIVE|
		The endpoints are at active state.

*/

/* __TMS_TYPE_DEF_ARG(UDEVICE_ENDPOINT_ACTIVITY_STATE, UINT8); */

#define	__TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_DISABLED		0
#define	__TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_WAIT_ACTIVE	1
#define	__TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_ACTIVE		2

#define __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_IS_DISABLED(pDevice)	\
	((pDevice)->udev_EpActiveState ==				\
	 __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_DISABLED)

#define __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_IS_NOT_DISABLED(pDevice)	\
	((pDevice)->udev_EpActiveState !=				\
	 __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_DISABLED)

/*

Name:	UDEVICE_CONTROL_QUEUE_REPLY()

Function:
	Common logic to be used by chip implementations for handling replies

Definition:
	VOID UDEVICE_CONTROL_QUEUE_REPLY(
		IN CONST PUDEVICE pSelf,
		IN CONST VOID *pBuf,
		IN BYTES lenRequested,
		IN BYTES lenActual,
		IN UBUFIODONEFN *pDoneFn OPTIONAL,
		IN VOID *pDoneCtx OPTIONAL,
		IN UBUFQE_FLAGS Flags,
		OUT BYTES *pLenQueued
		)

Description:
	This macro encapsulates the basic logic for enqueuing a control
	endpoint reply, on the assumption that the control endpoint replies
	flow back through the UBUFQE mechanism.  Most UDEVREPLY
	implementations will use this macro as the implementation of the
	queueing portion, because that is portable.

Returns:
	No explicit result.  (*pLenQueued) is set to the number of bytes
	actually queued to the host.

Notes:
	Some efforts are made to reduce the number of evaluations to
	a minimum; but remember, this is a macro and parameters might
	be evaluated more than once.

*/

#define UDEVICE_CONTROL_QUEUE_REPLY(pSelf, buf, lenRequested, lenActual, \
				    pDoneFn, pDoneCtx, flags, pLenQueued) \
	do	{						\
		__TMS_UBUFQE * __TMS_CONST __pqe = &(pSelf)->udev_ctlinbq; \
		__TMS_UBUFQE_FLAGS __flags = (flags);		\
		__TMS_CONST __TMS_VOID *__buf = (buf);		\
		__TMS_BYTES __lenQueued;			\
								\
		__flags &= __TMS_UBUFQEFLAG_SYNC;		\
		__lenQueued = (lenRequested);			\
		if (__lenQueued > (lenActual))			\
			{					\
			__flags |= __TMS_UBUFQEFLAG_POSTBREAK;	\
			__lenQueued = (lenActual);		\
			}					\
		else if (__lenQueued == 0)			\
			{					\
			/* zero length response is special! */	\
			__flags |= __TMS_UBUFQEFLAG_POSTBREAK;	\
			}					\
								\
		__pqe->uqe_length = sizeof(*__pqe);		\
		__pqe->uqe_request = __TMS_UBUFQE_RQ_IMPLICIT;	\
		__pqe->uqe_buf = (/* deCONST */ __TMS_VOID *)(__buf);	\
								\
		__pqe->uqe_bufsize = __lenQueued;		\
		__pqe->uqe_flags = __flags;			\
		__pqe->uqe_timeout = 0;				\
		__pqe->uqe_donefn = (pDoneFn);			\
		__pqe->uqe_doneinfo = (pDoneCtx);		\
								\
		UsbPipeQueue((pSelf), __pqe);			\
		*(pLenQueued) = __lenQueued;			\
		} while (0)

/****************************************************************************\
|
|	Some internal API functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_OBJECT_IOCTL_FN	UsbPumpDeviceI_Ioctl;

__TMS_VOID *UsbPumpDevice_AllocateDeviceBuffer(
		__TMS_UDEVICE *		/* pDevice */,
		__TMS_BYTES		/* nBytes */
		);

__TMS_BOOL UsbPumpDevice_CheckAutoRemoteWakeup(
		__TMS_UDEVICE *			/* pDevice */,
		__TMS_UINTERFACE *		/* pInterface */
		);

__TMS_BOOL UsbPumpDevice_EdgeBusEvent(
		__TMS_UDEVICE	*		/* pDevice */,
		__TMS_USBPUMP_OBJECT_HEADER *	/* pSelfObject */,
		__TMS_USBPUMP_OBJECT_HEADER *	/* pClientObject */,
		__TMS_VOID *			/* pClientContext */,
		__TMS_UEVENT			/* EventCode */,
		__TMS_VOID *			/* pEventSpecificInfo */
		);

__TMS_VOID UsbPumpDevice_FreeDeviceBuffer(
		__TMS_UDEVICE *		/* pDevice */,
		__TMS_VOID *		/* pBuffer */,
		__TMS_BYTES		/* nBytes */
		);

__TMS_UDEVICE_INITFN	UsbPumpDevice_Init;

__TMS_VOID UsbPumpDevice_SetupActiveConfig(
		__TMS_UDEVICE	*		/* pDevice */
		);

__TMS_VOID UsbPumpDeviceI_SetLinkState(
		__TMS_UDEVICE	*		/* pDevice */,
		__TMS_UDEVICE_LINK_STATE	/* LinkState */
		);

__TMS_BOOL UsbPumpDevice_GetEffectiveRWE(
		__TMS_UDEVICE	*		/* pDevice */
		);

__TMS_BOOL UsbPumpDevice_GetEffectiveRWE_V2(
		__TMS_UDEVICE *			/* pDevice */,
		__TMS_UINTERFACE *		/* pInterface */
		);

__TMS_USHORT UsbPumpDevice_GetMaxControlMaxPacketSize(
		__TMS_UDEVICE	*		/* pDevice */
		);

__TMS_BOOL UsbPumpDevice_MatchConfig(
		__TMS_UDEVICE *			/* pDevice */,
		__TMS_UINT8			/* bConfigurationValue */
		);

__TMS_BYTES UsbPumpDevice_OsDescGet(
		__TMS_UDEVICE *			/* pDevice */
		);

__TMS_BYTES UsbPumpDevice_OsDescGetProperty(
		__TMS_UDEVICE *			/* pDevice */
		);

__TMS_USBPUMP_CONTROL_PROCESS_FN UsbPumpDevice_OsDescProcess;

__TMS_BOOL UsbPumpDevice_OsDescSupport(
		__TMS_UDEVICE *			/* pDevice */
		);

__TMS_BOOL UsbPumpDevice_QueryBulkIntInPendingQe(
		__TMS_UDEVICE *			/* pDevice */
		);

__TMS_BOOL UsbPumpDevice_QueryInterfaceRemoteWakeupTrafficPending(
		__TMS_UDEVICE *			/* pDevice */,
		__TMS_UINTERFACE *		/* pInterface */
		);

__TMS_BOOL UsbPumpDevice_QueryRemoteWakeupTrafficPending(
		__TMS_UDEVICE *			/* pDevice */
		);

__TMS_VOID UsbPumpDevice_SendFunctionWake(
		__TMS_UDEVICE	*		/* pDevice */,
		__TMS_UINTERFACE *		/* pInterface */
		);

__TMS_VOID UsbPumpDevice_SuperSpeedFeatureSetup(
		__TMS_UDEVICE *			/* pDevice */
		);

__TMS_VOID UsbPumpDeviceI_Delay(
		__TMS_UDEVICE *			/* pDevice */,
		__TMS_USBPUMP_MILLISECONDS	/* uMillisecond */
		);

__TMS_UCONFIG *UsbPumpDeviceI_GetCurrentActiveConfig(
		__TMS_UDEVICE *			/* pDevice */
		);

__TMS_CONST __TMS_TEXT *UsbPumpDeviceLinkState_Name(
		__TMS_UDEVICE_LINK_STATE	/* LinkStateCode */
		);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UDEVICE_COMPAT_fSuspendState	\
   __TMS_UDEVICE_COMPAT_fSuspendState
# define UDEVICE_CONTENTS	\
   __TMS_UDEVICE_CONTENTS
# define UDEVICE_HDR	\
   __TMS_UDEVICE_HDR
# define UDEVICE_TAG	\
   __TMS_UDEVICE_TAG
# define UDEVICE_NAME(Text)	\
   __TMS_UDEVICE_NAME(Text)
# define UDEVICE_LINK_STATE_L0	\
   __TMS_UDEVICE_LINK_STATE_L0
# define UDEVICE_LINK_STATE_L1	\
   __TMS_UDEVICE_LINK_STATE_L1
# define UDEVICE_LINK_STATE_L2	\
   __TMS_UDEVICE_LINK_STATE_L2
# define UDEVICE_LINK_STATE_L3	\
   __TMS_UDEVICE_LINK_STATE_L3
# define UDEVICE_LINK_STATE_NOCABLE	\
   __TMS_UDEVICE_LINK_STATE_NOCABLE
# define UDEVICE_LINK_STATE_NOVBUS	\
   __TMS_UDEVICE_LINK_STATE_NOVBUS
# define UDEVICE_LINK_STATE_DISABLE	\
   __TMS_UDEVICE_LINK_STATE_DISABLE
# define UDEVICE_LINK_STATE_U0	\
   __TMS_UDEVICE_LINK_STATE_U0
# define UDEVICE_LINK_STATE_U1	\
   __TMS_UDEVICE_LINK_STATE_U1
# define UDEVICE_LINK_STATE_U2	\
   __TMS_UDEVICE_LINK_STATE_U2
# define UDEVICE_LINK_STATE_U3	\
   __TMS_UDEVICE_LINK_STATE_U3
# define UDEVICE_LINK_STATE_NAMES__INIT	\
   __TMS_UDEVICE_LINK_STATE_NAMES__INIT
# define UDEVICE_IS_SUSPEND_STATE(pDevice)	\
   __TMS_UDEVICE_IS_SUSPEND_STATE(pDevice)
# define UDEVICE_GET_LINK_STATE(pDevice)	\
   __TMS_UDEVICE_GET_LINK_STATE(pDevice)
# define UDEVICE_IS_L0_STATE(pDevice)	\
   __TMS_UDEVICE_IS_L0_STATE(pDevice)
# define UDEVICE_IS_L1_STATE(pDevice)	\
   __TMS_UDEVICE_IS_L1_STATE(pDevice)
# define UDEVICE_IS_L2_STATE(pDevice)	\
   __TMS_UDEVICE_IS_L2_STATE(pDevice)
# define UDEVICE_IS_L3_STATE(pDevice)	\
   __TMS_UDEVICE_IS_L3_STATE(pDevice)
# define UDEVICE_IS_U0_STATE(pDevice)	\
   __TMS_UDEVICE_IS_U0_STATE(pDevice)
# define UDEVICE_IS_U1_STATE(pDevice)	\
   __TMS_UDEVICE_IS_U1_STATE(pDevice)
# define UDEVICE_IS_U2_STATE(pDevice)	\
   __TMS_UDEVICE_IS_U2_STATE(pDevice)
# define UDEVICE_IS_U1_U2_STATE(pDevice)	\
   __TMS_UDEVICE_IS_U1_U2_STATE(pDevice)
# define UDEVICE_IS_U3_STATE(pDevice)	\
   __TMS_UDEVICE_IS_U3_STATE(pDevice)
# define UDEVICE_RECORD_LPM_FEATURE(pDevice, bmAttribute)	\
   __TMS_UDEVICE_RECORD_LPM_FEATURE(pDevice, bmAttribute)
# define UDEVICE_SUPPORTS_LPM(pDevice)	\
   __TMS_UDEVICE_SUPPORTS_LPM(pDevice)
# define UDEVICE_SUPPORTS_LPM_BESL(pDevice)	\
   __TMS_UDEVICE_SUPPORTS_LPM_BESL(pDevice)
# define UDEVICE_ENDPOINT_ACTIVITY_STATE_DISABLED	\
   __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_DISABLED
# define UDEVICE_ENDPOINT_ACTIVITY_STATE_WAIT_ACTIVE	\
   __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_WAIT_ACTIVE
# define UDEVICE_ENDPOINT_ACTIVITY_STATE_ACTIVE	\
   __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_ACTIVE
# define UDEVICE_ENDPOINT_ACTIVITY_STATE_IS_DISABLED(pDevice)	\
   __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_IS_DISABLED(pDevice)
# define UDEVICE_ENDPOINT_ACTIVITY_STATE_IS_NOT_DISABLED(pDevice)	\
   __TMS_UDEVICE_ENDPOINT_ACTIVITY_STATE_IS_NOT_DISABLED(pDevice)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of udevice.h ****/
#endif /* _UDEVICE_H_ */
