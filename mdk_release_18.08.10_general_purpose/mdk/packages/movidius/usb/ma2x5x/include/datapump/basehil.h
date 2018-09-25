/* basehil.h	Mon Jun  1 2015 00:18:16 tmm */

/*

Module:  basehil.h

Function:
	Basic HIL functionality. Any items/functions here must be supported
	by all HIL ports.

Version:
	V3.15b	Mon Jun  1 2015 00:18:16 tmm	Edit level 28

Copyright notice:
	This file copyright (C) 1997-2005, 2007-2008, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	John Keys, MCCI Corporation	August 1997

Revision history:
   1.10a  Mon Aug  4 1997 09:45:19  johnk
	Module created.

   1.10c  8/20/1997	johnk
	New functions

   1.20e  3/9/1998  tmm
	Change UHIL_fill, UHIL_cpybuf to use VOID * args.

   1.43b  9/1/1998  june,tmm
	ttusb_FirmwareInit(), upon analysis, was determined to be a useless
	artifact of the original code.  It was removed, and for the Uss820
	port it was replaced by Uss820_FirmwareInit().  Because each platform
	(hardware * chip) will have some chip-specific features, there's
	no need for a platform-independent FirmwareInit function; and the
	platform-independent FirmwareInit causes more problems by making
	it hard to pass platform-specific info to the various layers.

   1.43j  9/8/1998  tmm
	UHIL_FindDescriptor now takes a root-table argument; and we now
	declare a typedef.

   1.48a  3/17/1999 tmm
	UHIL_dispatchevent() needs to indicate whether we're in the
	DataPump.

   1.50a  8/11/1999  tmm
	Add UHIL_MEMTAG, a portable way of defining magic numbers that will
	dump nicely.

   1.50b 8/21/1999 tmm
	Add UHIL_SYNCHRONIZATION_BLOCK, UHIL_SynchronizeToDataPumpDevice,
	etc.  Add UHIL_ReleaseEventLock().  Add logic to allow for per-
	platform printq customization.

   1.51a 2/26/2000  ritu/tmm
	Add definitions for the abstract interrupt control subsystem.
	Deprecate the old "UHIL_HookInterrupt" function, but leave it
	in for compatibility.

   1.60c  5/4/2000  tmm
	Add missing __END_DECLS at end of MMU and Cache management
	section.

   1.60f  6/6/2000  tmm
	Eschew metabrackets.

   1.60n  9/28/2000  tmm
	Add platform-malloc, -free functions.

   1.60v  12/5/2000  tmm
	Found another set of size_t vs BYTES mismatch; BYTES will break
	fewer things in the rest of our code.  This was on the -malloc and
	-free functions.

   1.61a  12/27/2000  tmm
	Install standard cloaking for integration with ThreadX port.

   1.63d  20/09/2001  mats
   	Serial number ptr added to UPLATFORM.

   1.63d  11/6/2001  tmm
	Removed serial number pointer and mac addr pointer from UPLATFORM
	in favor of IOCTLs

   1.63f  24/3/2002  maw
	Added SWC-code range for W32 and OSE, moved 
	UHIL_INTERRUPT_RESOURCE_HANDLE from basehiltypes.h

   1.79a  6/5/2002  tmm
	Major repartitioning; basehil.h is now starting to go away (much
	as usbpump.h is), in an effort to limit header file bloat and
	excessive includes of stuff that isn't needed.

   1.87b  2/29/2004  tmm
	Clean up problems found by autodoc.

   1.93c  Fri Dec 09 2005 09:12:05  cccho
	UHIL_FindDescriptorV2 function declaration.

   1.97j  Fri Nov 23 2007 07:16:14  tmm
	3522:  do not include hilbusio.h here!  If you do, it makes many
	things that should be portable subject to recompile on every
	different platform (wrong approach).

   1.97j  Sat Dec 01 2007 22:44:17  prathap
	UDP-6288: Function 'UHIL_FindWusbDescriptorV2_5' has been declared.
	This function has been hard coded now since the current version of
	USBRC doesn't support WUSB descriptors. This has to be modified
	accordingly once the USBRC has been upgraded for supporting WUSB
	descriptors.

   1.97j  Fri Dec 07 2007 12:09:12  yssaroha
	UDP-6288: Function 'UHIL_FindWusbDescriptorV2_5' is renamed as
	'UHIL_FindDescriptorV3' and prototype is updated to included
	current speed as argument.

   1.97j  Fri Nov 14 2008 11:19:32  sivaraj
	UDP-6288: Removed UHIL_FindDescriptorV3() declaration.

   3.15b  Mon Jun  1 2015 00:18:16  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _BASEHIL_H_		/* prevent multiple includes */
#define _BASEHIL_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

/*
|| get IOCTL definitions
*/
#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

/*
|| We need the platform structure.
*/
#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

/****************************************************************************\
|
|	Initialization
|
\****************************************************************************/

__TMS_BEGIN_DECLS 
__TMS_VOID	UHIL_InitVars __TMS_P ((__TMS_VOID));
__TMS_VOID	UHIL_IdleLoop __TMS_P ((__TMS_VOID));
__TMS_END_DECLS


/****************************************************************************\
|
|	Interrupt hooking (the old way)
|
\****************************************************************************/

#if __TMS_BASEHIL_INCLUDE_V1_INTERRUPTS
# include "basehilv1ints.h"
#endif

/****************************************************************************\
|
|	Starting with production Version 2 (internal V1.51), we 
|	have an abstract interrupt subsystem interface.  All new MCCI
|	code should use this mechanism.
|
\****************************************************************************/

#ifndef _UHILINTSYS_H_
# include "uhilintsys.h"
#endif

/****************************************************************************\
|
|	Idle-time polling routines
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
	FIRMWAREPOLLFN,
	__TMS_VOID,
		(__TMS_VOID *)
	);

struct TTUSB_POLLCONTEXT
	{
	__TMS_PFIRMWAREPOLLFN	upc_pfn;	/* polling function	*/
	__TMS_VOID	       *upc_ctx;	/* context for function */
	};
	
#ifndef _UHILEVENT_H_
# include "uhilevent.h"
#endif

/*
|| Libarary routines
*/
__TMS_BEGIN_DECLS

__TMS_PFIRMWAREPOLLFN UHIL_SetFirmwarePoll __TMS_P((__TMS_PUPOLLCONTEXT, 
					   __TMS_PFIRMWAREPOLLFN, 
					   __TMS_VOID *));
					   
__TMS_BOOL	UHIL_DoPoll __TMS_P((__TMS_PUPOLLCONTEXT ppoll, 
				  __TMS_PUEVENTCONTEXT peq));

__TMS_END_DECLS


/****************************************************************************\
|
|	MMU and Cache management
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(UHIL_PHYSICAL_MAPPING);

#ifndef __TTUSB_UHIL_PHYSICAL_MAPPING_DEFINED
# define __TTUSB_UHIL_PHYSICAL_MAPPING_DEFINED
struct __TMS_STRUCTNAME(UHIL_PHYSICAL_MAPPING)
	{
	__TMS_ADDRBITS	uhpmap_physaddr;	/* the base physical address */
	__TMS_ADDRBITS	uhpmap_length;		/* how many bytes in this
						|| mapping item.
						*/
	};
#endif /* __TTUSB_UHIL_PHYSICAL_MAPPING_DEFINED */

__TMS_BEGIN_DECLS

/*
|| Translate a CPU virtual address in the current mapping to a corresponding
|| physical address.  A very simple service normally used when running on 
|| platforms with limited mapping.  It could be argued that having this
|| is just a temptation to the unwary, but a full MDL-style API is out
|| of scope at the moment.
*/
__TMS_UINT UHIL_CurrentMappingRangeToNumberOfSegments(
	__TMS_UHIL_BUSHANDLE BusHandle,
	__TMS_CONST __TMS_VOID *pBase,
	__TMS_ADDRBITS nBytes
	);

__TMS_UINT UHIL_CurrentMappingToPhysicalVector(
	__TMS_UHIL_BUSHANDLE BusHandle,
	__TMS_CONST __TMS_VOID *pBase,
	__TMS_ADDRBITS nBytes,
	__TMS_UHIL_PHYSICAL_MAPPING *pMappingOut,
	__TMS_UINT nMappingOut
	);

/* 
|| Invalidate the cache for a region of memory, and don't return until memory 
|| is consistent with the CPU's view of it.   pBase_CurrentMapping is a 
|| virtual address (only matters if you have an MMU) and is according to 
|| how the CPU views memory (only matters if physical addresses are different 
|| than CPU addresses).
*/
__TMS_VOID UHIL_SynchronizeMemory(
	__TMS_CONST __TMS_VOID *pBase_CurrentMapping,
	__TMS_ADDRBITS nBytes
	);
__TMS_END_DECLS


/****************************************************************************\
|
|	Software Check (SWC)
|
\****************************************************************************/

#ifndef _UHILSWC_H_
# include "uhilswc.h"
#endif

/****************************************************************************\
|
|	Error codes
|
\****************************************************************************/

#ifndef _UHILERRCODES_H_
# include "uhilerrcodes.h"
#endif


/****************************************************************************\
|
|	Kernel Helper Routines
|
\****************************************************************************/

#ifndef _USBPUMPLIB_H_
# include "usbpumplib.h"
#endif

/* function declarations */
__TMS_BEGIN_DECLS

__TMS_UHILSTATUS UHIL_FindDescriptor(
	__TMS_CONST __TMS_USBRC_ROOTTABLE * __TMS_CONST /* root pointer */, 
	__TMS_ARG_USHORT  /* type   */, 
	__TMS_ARG_USHORT  /* index  */, 
	__TMS_ARG_USHORT  /* lid    */, 
	__TMS_BYTES     * /* psize  */, 
	__TMS_CONST __TMS_UCHAR    ** /* ppDesc */  
	);

__TMS_UHILSTATUS UHIL_FindDescriptorV2(
	__TMS_CONST __TMS_USBRC_ROOTTABLE * __TMS_CONST /* root pointer */,
	__TMS_USBPUMP_DEVICE_SPEED	/* current speed */,
	__TMS_USBPUMP_DEVICE_SPEED_MASK	/* supported speeds */,
	__TMS_ARG_USHORT	/* type */,
	__TMS_ARG_USHORT	/* index */,
	__TMS_ARG_USHORT	/* lid */,
	__TMS_BYTES      *	/* psize */,
	__TMS_CONST __TMS_UCHAR     **	/* ppDesc */
	);

/**** OBSOLETE ****/
__TMS_VOID	UHIL_setivecbase __TMS_P((	\
				__TMS_VOID * \
				));
/**** END OBSOLETE ****/

__TMS_END_DECLS

/**** end of basehil.h ****/
#endif /* _BASEHIL_H_ */
