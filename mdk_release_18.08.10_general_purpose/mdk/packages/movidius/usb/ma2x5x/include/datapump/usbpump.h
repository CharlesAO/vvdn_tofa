/* usbpump.h	Sun May 31 2015 22:10:32 tmm */

/*

Module:  usbpump.h

Function:
	TrueTask USB Peripheral DataPump (silicon-independent)

Version:
	V3.15b	Sun May 31 2015 22:10:32 tmm	Edit level 33

Copyright notice:
	This file copyright (C) 1997-2008, 2011-2012, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	March 1997

Revision history:
   1.00a  Sun Mar 16 1997 12:59:53  tmm
	Module created.

   1.00a  5/14/97  tmm
	Fixed a few declarations, and added event processing structure
	for processing control packets.

   1.10d  9/24/1997 johnk
	Add stall flag to ubufqe flags.

   1.20a  3/3/1998  tmm
	Document use of POSTBREAK flags.

   1.20d  3/8/1998  tmm
	Add definition for USTAT_STALL, used if queueing to a
	stalled endpoint.
	
   1.40a  5/10/1998  tmm
	Iso support.  Also, we should not initialize the endpoint's back
	pointer to a non-null value.  So we get rid of the pipe pointer 
	from the generic endpoint init function.

   1.43d  9/5/1998  tmm
	ISO support bug fixes; also clean up ubufqe flag problems with
	old API.

   1.45b 9/29/98  june
	Add support for setting descriptor.

   1.45c 11/9/1998 johnk
	Add UsbPutQe(), usbGetQe(), and UsbFindIndexFromIfcset().

   1.50b  8/21/1999  tmm
	Add UDEVREMOTEWAKEUP().

   1.52a  3/1/2000  tmm
	The IAR compiler for CR-16 doesn't like multiple struct XXX;
	declarations (and it's rather vehement about it).  So remove all
	those.  Then add a "udevhil.h" file, included here, that has
	additional functions that depend on usbpump.h as well as basehil.h.

   1.52b  6/19/2000  maw
	Add USTAT_DEFINITE_LENGTH_UNDERRUN

   1.52c  6/23/2000  maw
	UDEVICEINITIALIZE macro didn't pass the USBRC_ROOTTABLE parameter
	required by the device-init function prototype.

   1.60e  7/21/2000  tmm
	As part of fixing the UsbPostIfNotBusy bug, allow the porting
	engineer to susbstitue subroutines for UsbMarkCompletion[Busy/NotBusy]
	by changing INLINE_... values when compiling.  This is needed if
	pointer updates are not interrupt atomic.

   1.60e  7/23/2000  tmm
	The mpc850 port needs to know what endpoints have been invalidated
	by the upper level, so that unaffected endpoints don't get
	clobbered by chapter-9 changes.  So we added a field, uep_fChanged,
	which is set whenever the endpoint setup is changed by portable
	code, and cleared ONLY by the chip driver, at its discretion.

   1.60m  9/19/2000  tmm
	Merge in changes from USBKERN-MPC850-DEVEL:  mainly add
	MCCI_DEBUG as an input -D flag, which forces debug on if it's
	not already defined.

   1.60n  9/27/2000  tmm
	Make sure UDEVICE_INITFN is cloaked.  Add udev_usbPortIndex for
	use by SNMP-ish things, along with usbDeviceStatus, usbDeviceAddress,
	usbDeviceEnumCounter.

   1.60p  10/2/2000  tmm
	Debug functions take UPLATFORM argument.

   1.60t  11/24/2000  tmm
	Bug 393:  correct BYTES/size_t confusion.

   1.61a  12/26/2000  tmm
	Move the UsbLib_InitDeviceControlEp() function here from the mpc-850
	port.  Add some more cloaking.

   1.61d  1/6/2001  tmm
	Break out the basic object types, so we can integrate more
	easily with other (large) codebases.

   1.61d  1/7/2001 tmm
	Cloak.

   1.63b  8/26/2001  tmm
	ARM SDT support -- get rid of duplicated portindex #define.

   1.63d  11/9/2001  tmm
	Add IOCTL plumbing.  Create UDEVICE_SWITCH_INIT_V5(), add UDEVIOCTL(),
	and define the function type UDEVIOCTLFN.

   1.63i  5/20/2002  tmm
	Rename UsbLib_... to UsbPumpLib_...; but keep the old names as
	macros (for now).

   1.79a  5/27/2002  tmm
	600032 1.1.1: add placeholders for the descriptors.
	Split up into smaller pieces.  Add UsbReportInterafceEvent().

   1.81a  Wed Feb 19 2003 20:59:59  tmm
	Bugs 999, 1000:  Add UsbChangeConfigEx().

   1.83b  Mon Feb 24 2003 10:51:34  tmm
	Add prototype UsbProcessAttach() and UsbProcessDetach() to remove
	compiler warning.

   1.89c  Mon Oct 18 2004 12:10:34 maw
   	UsbProcessSetInterface() parameter name "interface" conflicts
   	when including this file together with Windows header files.
   	Changed to ifc.

   1.91d  2005-06-02  tmm
	It's appropriate to pull in all the device speed constants from this
	header file, so include usbpump_devspeed.h.

   1.93c  Fri Dec 09 2005 09:12:05  cccho
	Added UsbProcessUsbResetV2 for different speed devices.

   1.97a  Tue Mar 28 2006 03:54:30  tmm
	1717: Change prototype of UsbProcessGetDescriptor() so we can
	do further filtering if needed.

   1.97k  Fri Mar 28 2008 08:36:33  chwon
	3735:  UBUFQE buffer sizes are now 32-bits, switch to using BYTES

   1.97k  Tue Sep 23 2008 15:05:48  cccho
	6683: add protoptye UsbProcessL1Suspend.

   1.97k  Mon Aug 11 2008 14:47:43  chwon
	6349: added allocate/free aligned memory from/to platform pool

   1.97k  Wed Oct 08 2008 13:34:23  cccho
   	6829: add prototype UsbCreateDevicePoolV2() and
   	UsbPumpLib_InternalFreeDeviceBuffer().

   3.01f  Fri Jul 01 2011 14:02:15  maw
	13233: Add UsbInterfaceActivateEndpoints() and UsbEndpointActivate().

   3.01f  Tue Jan 10 2012 15:02:43  chwon
	14623: Add UsbProcessSetIsochDelay(), UsbProcessSetSel(),
	UsbProcessU1Sleep() and UsbProcessU2Sleep().

   3.15b  Thu Mar  5 2015 15:10:12  tmm
	19019: Update documentation.

   3.15b  Sun May 31 2015 22:10:32  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _USBPUMP_H_		/* prevent multiple includes */
#define _USBPUMP_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/*

MODULE DOCUMENTATION:  USB DataPump device modeling

	The USB DataPump models a device as a set of collections of 1 or more 
	configurations.  Each set of configurations corresponds to a single
	operating speed of the device (low speed, full speed, high speed, 
	superspeed, etc.) 

	Each configuration is a collection of 1 or more interface sets.
	Each interface set is a collection of 1 or more interfaces.
	(Interfaces correspond directly to interface descriptors; interface
	sets collect the various alternate settings of a given interface.)
	Each interface is a collection of 1 or more endpoints.
	Endpoints are leaf nodes, and correspond directly to endpoint
	descriptors. 

	At any given time, there is a "current" configuration, which
	has "current" interface sets. Each interface set has a "current"
	interface (alternate setting),  which in turn has "current" endpoints.
	The set of current endpoints is formed by iterating over the
	interfaces in the current configuration, and forming the set of
	endpoints attached to the selected alternate settings.  Endpoint 0 is
	not part of this modelling, as it is always active.

	The structures of the the DataPump are static, and fixed size.  
	The data structures respresenting interfaces, interface sets, and 
	configurations are almost always fixed size; but the relevant 
	header files define a number of macros that allows the
	code to be written independently of the decision as to whether
	a structure is fixed size.  

	At each level of the hierarchy, the collections are structured as
	vectors of underlying structures.  If the size of underlying 
	structure is determined by port-specific code, the portable portion
	of the structure includes a size field so that the macros can index 
	(dynamically) through the vector of structures.  If the DataPump
	has been configured so that the underlying structure size is known 
	at compile time, and port-specific indexing can be implemented as 
	simple pointer arithmetic.

	The ability to have port-specific variable-size structures is
	deprecated and will be removed in future releases.

*/

/* 

MODULE DOCUMENTATION:	USB DataPump device-stack type hierarchy

	The core device stack of the DataPump uses the older MCCI hierarchy 
	of derived types (as opposed to the newer MCCI explicit Russian-doll
	hierarchy of types).

	In this model, the source code refers to structure fields using a
	standard prefix for a given class of object.  For example, the 
	standard prefix for field of any object derived from UDEVICE 
	is "udev_...".  
	
	In the older mode, code refers udev_xyz, without worrying
	about where in the hierarchy udev_xyz is defined.  This allows
	structure fields to be moved in the hierarchy without changing
	source code, but requires discipline in making the declarations.
	It also has the disadvantage of using macros, which can make source-
	level debugging more difficult.

	The top (most abstract) level of the hierarchy is always some 
	specific symbolic type. For this example, let's use UDEVICE, the
	abstract type that models any instance of a USB DataPump device.

	The top level is a structure that specifies the overall layout
	of the portable header of the object.

	struct TTUSB_DEVICE_HDR {
		//
		// the contents go here, for example...
		//
		USBPUMP_DEVICE_FSM udevhh_DeviceFsm;
		UINT16		udevhh_usbPortIndex;
		};

	Next, we define a macro which defines the contents of a UDEVICE (no
	matter where it is found).

	#define UDEVICE_CONTENTS struct TTUSB_DEVICE_HDR udevhdr_udevice

	In order that we can have a consistent expansion for our macros, we
	define a UDEVICE as a union (with one element):

	typedef union TTUSB_UDEVICE_UNION {
		UDEVICE_CONTENTS;
		};

	For our clients (who create more specific devices derived from
	UDEVICE), we define a macro:

	#define UDEVICE_HDR union TTUSB_UDEVICE_UNION udev_hdr

	Finally, we create the UDEVICE data type, which is used by portable
	code to access the portable portion of the actual, concrete instances:

	typedef struct TTUSB_UDEVICE
		{
		UDEVICE_HDR;
		} UDEVICE;

	With this complicated set of rules, we access udevhh_usbPortIndex
	in a UDEVICE by writing

		p->udev_hdr.udevhdr_udevice.udevhh_usbPortIndex
	
	(udev_hdr is because of the UDEVICE_HDR macro; udevhdr_udevice is
	because of the UDEVICE_CONTENTS macro; udevhh_usbPortIndex is
	because fo the original structure definition.)

	To make the name easier to type, udevice.h contains definitions for
	simpler names:

	#define udev_usbPortIndex udev_hdr.udevhdr_udevice.udevhh_usbPortIndex

	Thus, if p is a UDEVICE pointer, p->udev_usbPortIndex is the port
	index field.

	The next UDEVICE derivied object is normally defined by the DCD.
	Let's use the sample DCD as the reference. 

	It first defines a structure:

	struct __TMS_UDEVSKL_HDR
		{
		// this must be first:
		UDEVICE_HDR;

		UHIL_BUSHANDLE udevsklhh_Bus;
		IOPORT	udevsklhh_IoPort;
		// and so on
		};

	The UDEVICE_HDR has only one purpose: to reserve space at the 
	front of the structure for the parent object (from which this is
	derived).

	Next, the sample DCD, following the pattern used in the UDEVICE,
	defines a macro and a union.

	#define UDEVSKL_CONTENTS			\
		UDEVICE_CONTENTS;			\
		struct __TMS_UDEVSKL_HDR udevhdr_udevskl

	union __TMS_UDEVSKL_UNION 
		{
		UDEVSKL_CONTENTS;
		};

	This means that the __TMS_UDEVSKL_UNION has two views. One view, named
	udevhdr_udevskl, allows access to the udevsklhh_Bus, etc. The other
	view, named udevhdr_udevice (because of the UDEVICE_CONTENTS macro),
	is used for accessing contents of the struct TTUSB_DEVICE_HDR.

	Next, the sample DCD defines a macro to be used for creating
	datatypes derived from UDEVSKL.

	#define UDEVSKL_HDR union __TMS_UDEVSKL_UNION udev_hdr

	Finally, the sample DCD defines the UDEVSKL object itself.

	typedef struct __TMS_UDEVSKL
		{
		UDEVSKL_HDR;
		} UDEVSKL;

	Again, macros are defined to make typing easier, for example:

	#define udevskl_Bus udev_hdr.udevhdr_udevskl.udevsklhh_Bus

	It should be clear how this macro accesses the udevsklhh_Bus in
	a UDEVSKL instance. 

	Now suppose that pSkl is a pointer to a UDEVSKL, and we write the
	expression pSkl->udev_usbPortIndex.  This expands to 
	
		pSkl->udev_hdr.udevhdr_udevice.udevhh_usbPortIndex

	In this case pSkl->udev_hdr is an object of type union
	__TMS_UDEVSKL_UNION, not union TTUSB_UDEVICE_UNION. But that's OK;
	both unions contain an element named udevhdr_udevice, and in
	both cases, this is of type strut TTUSB_DEVICE_HDR. So 

		pDevice->udev_usbPortIndex

	is exactly the same as 

		pSkl->udev_usbPortIndex

	which is what we wanted to accomplish.

	Further layers of derived structures are created in the same
	fashion.

	1) define the layout of the next layer
		struct Uthislayer_HDR
			{
			UDEVSKL_HDR;	// or {parent}_HDR
			type_t	name1;	// first field at this layer
			//...
			};
	
	2) create the symbol that lists the alternative views for this
	layer.
	
		#define thislayer_CONTENTS		\
			UDEVSKL_CONTENTS		\
			struct thislayer_HDR	udevuu_thislayer;
	
	3) Define the union that will represent this layer and previous
	layers:
	
		union thislayer_UNION
			{
			thislayer_LAYER_CONTENTS;
			};
	
	4) Define the standard header generation macro for clients
	   that might further derive the type.
	
		#define thislayer_HDR	union thislayer_UNION udev_hdr
	
	5) Define the structure that can be used to abstractly refer to
	this structure, and any structures derived from this structure
	
		typedef struct thislayer_t
			{
			thislayer_HDR;
			} thislayer_t;
	
	6) define the sane names:
		#define	udev_name1	udev_hdr.udevuu_thislayer.name1
			...

	This pattern generalizes to structures of any type, not only
	structures derived from UDEVICE. It is used for UPLATFORM, UPIPE,
	and a number of other type hierarchies, primarily for code that
	originated from the device DataPump.

	A similar scheme, without field name macros, is used in the host
	stack and in newer MCCI code. A good example is USBPUMP_USBDI_PORT.

*/

/****************************************************************************\
|
|	The UEVENTNODE types.
|
\****************************************************************************/

#ifndef _UEVENTNODE_H_
# include "ueventnode.h"
#endif

/****************************************************************************\
|
|	The buffer queue element structure
|
\****************************************************************************/

#ifndef _UBUFQE_H_
# include "ubufqe.h"
#endif

/****************************************************************************\
|
|	The UBUFQE status codes.
|
\****************************************************************************/

#ifndef _USTAT_H_
# include "ustat.h"
#endif

/****************************************************************************\
|
|	The structure of ISOCHRONOUS frames
|
\****************************************************************************/

#ifndef _UISOBUFHDR_H_
# include "uisobufhdr.h"
#endif

/****************************************************************************\
|
|	The device switch structure is used for interfacing to the
|	hardware.
|
\****************************************************************************/

#ifndef _UDEVSWITCH_H_
# include "udevswitch.h"
#endif

/****************************************************************************\
|
|	The configuration structure -- represents a single configuration.
|	This is not normally derived.
|
\****************************************************************************/

#ifndef _UCONFIG_H_
# include "uconfig.h"
#endif

/****************************************************************************\
|
|	Interface sets represet each interface; each interface has a
|	collection of "alternative settings".  There is one interface
|	set per interface.
|
|	Again, interface sets are not thought to be normally virtualized,
|	so we do the usual dodges.
|
\****************************************************************************/

#ifndef _UINTERFACESET_H_
# include "uinterfaceset.h"
#endif

/****************************************************************************\
|
|	The standard "interface" structure -- there's one of these
|	for each interface.  Virtualization is thought to be rare, so
|	we use extension pointers for hw level.
|
\****************************************************************************/

#ifndef _UINTERFACE_H_
# include "uinterface.h"
#endif

/****************************************************************************\
|
|	The UPIPE models the actual state of a given endpoint.
|
\****************************************************************************/

#ifndef _UPIPE_H_
# include "upipe.h"
#endif

/****************************************************************************\
|
|	The standard endpoint structure -- this is used to manage I/O
|	and so must be a base type that the hardware level can derive
|	from.  
|
\****************************************************************************/

#ifndef _UENDPOINT_H_
# include "uendpoint.h"
#endif

/****************************************************************************\
|
| The UDEVICE is the top level structure.
|
\****************************************************************************/

#ifndef _UDEVICE_H_
# include "udevice.h"
#endif

/****************************************************************************\
|
| Most protocols use UCALLBACK; so get the callback completion stuff in
| scope.
|
\****************************************************************************/

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

/****************************************************************************\
|
| The UPLATFORM is really a key part of the API.
|
\****************************************************************************/

#ifndef _UPLATFORM_H_
/* # include "uplatform.h" */
#endif

/****************************************************************************\
|
|	The standard functional interfaces
|
\****************************************************************************/

#ifndef _USBPUMPAPI_H_
# include "usbpumpapi.h"
#endif

/****************************************************************************\
|
|	USB device speed is important, so make it available from this
|	include file as well.
|
\****************************************************************************/

#ifndef _USBPUMP_DEVSPEED_H_
# include "usbpump_devspeed.h"
#endif

/****************************************************************************\
|
|	the internal-use-only routines
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/**** the following function is obsolescent ****/
__TMS_VOID	UsbChangeConfig __TMS_P((
			__TMS_UDEVICE *, __TMS_UCONFIG *, 
			__TMS_UEVENT whyin, __TMS_UEVENT whyout, 
			__TMS_VOID *info));
/**** ---- end obsolescent --- ***/

__TMS_BOOL	UsbChangeConfigEx __TMS_P((
			__TMS_UDEVICE *, __TMS_UCONFIG *, 
			__TMS_UEVENT whyin, __TMS_UEVENT whyout, 
			__TMS_VOID *info));
__TMS_VOID	UsbChangeInterface __TMS_P((
			__TMS_UDEVICE *, __TMS_UINTERFACE *,
			__TMS_UEVENT whyin, __TMS_UEVENT whyout, 
			__TMS_VOID *info));
__TMS_VOID	UsbCreateDevicePool __TMS_P((
			__TMS_UDEVICE *, __TMS_VOID *, __TMS_BYTES
			));

__TMS_VOID	UsbCreateDevicePoolV2 __TMS_P((
			__TMS_UDEVICE *, __TMS_VOID *,
			__TMS_BYTES, __TMS_ADDRBITS
			));

__TMS_VOID	UsbInterfaceSetup __TMS_P((
			__TMS_UDEVICE *, __TMS_UINTERFACE *
			));
__TMS_VOID	UsbInterfaceSetupNotify __TMS_P((
			__TMS_UDEVICE *, __TMS_UINTERFACE *, 
			__TMS_UEVENT, __TMS_VOID *
			));
__TMS_VOID	UsbInterfacesetTeardown __TMS_P((
			__TMS_UDEVICE *, __TMS_UINTERFACESET *
			));
__TMS_VOID	UsbInterfacesetTeardownNotify __TMS_P((
			__TMS_UDEVICE *, __TMS_UINTERFACESET *, 
			__TMS_UEVENT, __TMS_VOID *
			));
__TMS_VOID	UsbInterfaceActivateEndpoints __TMS_P((
			__TMS_UDEVICE *, 	/* pDevice */
			__TMS_UINTERFACE *,	/* pInterface */
			__TMS_BOOL 		/* fActive */
			));
__TMS_VOID	UsbEndpointActivate __TMS_P((
			__TMS_UDEVICE *,	/* pDevice */
			__TMS_UENDPOINT *	/* pUep */
			));

/**** the following function is obsolescent ****/
__TMS_VOID	UsbPipeQueueFn __TMS_P((
			__TMS_PUDEVICE, __TMS_PUPIPE, __TMS_PUBUFQE,
			__TMS_VOID */*buf*/, __TMS_BYTES /*size*/,
			__TMS_UBUFIODONEFN *, __TMS_VOID *
			));
/**** ---- end obsolescent --- ***/


/*--- beginning of hardware interface functions ---*/
__TMS_VOID	*UsbHilGetReplyBuffer __TMS_P((__TMS_UDEVICE *));
__TMS_BYTES	UsbHilGetReplyBufferSize __TMS_P((__TMS_UDEVICE *));
__TMS_VOID	UsbProcessAttach __TMS_P((__TMS_UDEVICE *));
__TMS_BOOL	UsbProcessControlPacket __TMS_P((
			__TMS_UDEVICE *, __TMS_UINT8 *
			));
__TMS_VOID	UsbProcessDetach __TMS_P((__TMS_UDEVICE *));
__TMS_VOID	UsbProcessGetConfiguration __TMS_P((
			__TMS_UDEVICE *, 
			__TMS_CONST __TMS_USETUP *
			));
__TMS_BOOL	UsbProcessGetDescriptor __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *
			));
__TMS_VOID	UsbProcessGetDeviceStatus __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *
			));
__TMS_VOID	UsbProcessGetEndpointStatus __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *
			));
__TMS_VOID	UsbProcessGetInterface __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *
			));
__TMS_VOID	UsbProcessGetInterfaceStatus __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *
			));
__TMS_VOID	UsbProcessResume __TMS_P((__TMS_UDEVICE *));
__TMS_VOID	UsbProcessSetAddress __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *, 
			__TMS_UINT8 *
			));
__TMS_BOOL	UsbProcessSetClearDevFeature __TMS_P((
			__TMS_UDEVICE *, __TMS_ARG_USHORT feature,
			__TMS_ARG_BOOL value, __TMS_UINT8 *setup));
__TMS_BOOL	UsbProcessSetClearEpFeature __TMS_P((
			__TMS_UDEVICE *, __TMS_ARG_USHORT ep,
			__TMS_ARG_USHORT feature,
			__TMS_ARG_BOOL value, 
			__TMS_UINT8 *setup));
__TMS_BOOL	UsbProcessSetClearIfcFeature __TMS_P((
			__TMS_UDEVICE *, __TMS_ARG_USHORT ep,
			__TMS_ARG_USHORT feature,
			__TMS_ARG_BOOL value, __TMS_UINT8 *setup
			));
__TMS_BOOL	UsbProcessSetConfig __TMS_P((
			__TMS_UDEVICE *, __TMS_ARG_USHORT config, 
			__TMS_UINT8 *setup
			));
__TMS_VOID	UsbProcessSetDescriptor __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *
			));
__TMS_BOOL	UsbProcessSetInterface __TMS_P((
			__TMS_UDEVICE *, __TMS_ARG_USHORT ifc,
			__TMS_ARG_USHORT index, __TMS_UINT8 *setup
			));
__TMS_BOOL	UsbProcessSetIsochDelay __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *
			));
__TMS_BOOL	UsbProcessSetSel __TMS_P((
			__TMS_UDEVICE *, __TMS_CONST __TMS_USETUP *
			));
__TMS_VOID	UsbProcessSetupPacketRaw __TMS_P((
			__TMS_UDEVICE *, __TMS_UINT8 *pkt
			));
__TMS_VOID	UsbProcessSuspend __TMS_P((
			__TMS_UDEVICE *
			));
__TMS_VOID	UsbProcessU1Sleep(
			__TMS_UDEVICE *		/* pDevice */
			);
__TMS_VOID	UsbProcessU2Sleep(
			__TMS_UDEVICE *		/* pDevice */
			);
__TMS_VOID	UsbProcessUsbReset __TMS_P((
			__TMS_UDEVICE *
			));
__TMS_VOID	UsbProcessUsbResetV2 __TMS_P((
			__TMS_UDEVICE *, __TMS_USBPUMP_DEVICE_SPEED newSpeed
			));
__TMS_VOID	UsbProcessL1Suspend(
			__TMS_UDEVICE	* /* pDevice */,
			__TMS_BOOL	/* fRemoteWake */,
			__TMS_UINT8	/* ucHostInitiatedResumeDuration */
			);

/*--- end of the hardware interface functions ---*/

/* some library functions for use by chip drivers */
__TMS_UDEVALLOCATEFN	UsbPumpLib_DeviceAllocateFromPlatform;
__TMS_UDEVFREEFN	UsbPumpLib_DeviceFreeToPlatform;
__TMS_UDEVALLOCATEFN	UsbPumpLib_InternalAllocateDeviceBuffer;
__TMS_UDEVFREEFN	UsbPumpLib_InternalFreeDeviceBuffer;

__TMS_BOOL		UsbPumpLib_InitDeviceControlEp __TMS_P((
				__TMS_UDEVICE * /*pSelf*/, 
				__TMS_BYTES /* ReplySize */ 
				));

#if !__TMS_CLOAKED_NAMES_ONLY && __TMS_DATAPUMP_COMPAT_V1_6
# define	UsbLib_InitDeviceControlEp	UsbPumpLib_InitDeviceControlEp
#endif /* !__TMS_CLOAKED_NAMES_ONLY && __TMS_DATAPUMP_COMPAT_V1_6 */

/* 6349: added allocate/free aligned memory from/to platform pool */
__TMS_VOID *		UsbPumpDeviceLib_AllocateAlignedBufferFromPlatform(
				__TMS_UDEVICE *	/* pDevice */,
				__TMS_BYTES	/* Size */,
				__TMS_BYTES	/* Alignment */
				);

__TMS_VOID		UsbPumpDeviceLib_FreeAlignedBufferToPlatform(
				__TMS_UDEVICE *	/* pDevice */,
				__TMS_VOID *	/* pBuffer */,
				__TMS_BYTES	/* Size */,
				__TMS_BYTES	/* Alignment */
				);

__TMS_END_DECLS

#ifndef _USBPUMPLIB_H_
# include "usbpumplib.h"
#endif

/****************************************************************************\
|
|	Flags & functions for descriptor filtering
|
\****************************************************************************/

#ifndef _USBDESCFILTERS_H_
# include "usbdescfilters.h"
#endif

/**** end of usbpump.h ****/
#endif /* _USBPUMP_H_ */
