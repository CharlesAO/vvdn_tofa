/* usbpump_usbdi_superspeed.h	Thu Feb 26 2009 10:15:10 djt */

/*

Module:  usbpump_usbdi_superspeed.h

Function:
	Definition of USBD SuperSpeed configuration.

Version:
	V2.01a	Thu Feb 26 2009 10:15:10 djt	Edit level 1

Copyright notice:
	This file copyright (C) 2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Debbie Totsline, MCCI Corporation	February 2009

Revision history:
   2.01a Thu Feb 26 2009 10:15:10 djt
	7416: Module created.

*/

#ifndef _USBPUMP_USBDI_SUPERSPEED_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_SUPERSPEED_H_

# ifndef _USBPUMP_USBDI_TYPES_H_
#  include "usbpump_usbdi_types.h"
# endif

# ifndef _USBPUMPOBJECT_H_
#  include "usbpumpobject.h"
# endif


/****************************************************************************\
|
|	The USBD SUPERSPEED Configuration object
|
\****************************************************************************/

struct __TMS_STRUCTNAME(USBPUMP_USBDI_USBDSUPERSPEED_CONFIG)
	{
	__TMS_UINT32			MagicBegin;
	__TMS_BYTES			Size;

	/*
	|| For USB 3.0, we need to define the U1/U2 timeout for all
	|| hubs in the system. What is defined here applies to all hubs in the
	|| system.
	||
	|| U1:
	|| 0x0000: transitions disabled; not to initiate or accept link partner
	||	 requests.
	|| 0x0001-0x007F: transitions enabled; able to initiate or accept link
	||       partner requests. Timeout in 1 millisecond units.
	|| 0x0x0080-0x00FE: Reserved
	|| 0x00FF: transition disabled; will accept link partner requests,
	||       however.
	|| -1: system default
	||
	|| U2:
	|| 0x0000: transitions disabled; not to initiate or accept link partner
	||	 requests.
	|| 0x0001-0x00FE: transitions enabled; able to initiate or accept link
	||       partner requests. Timeout in 256 millisecond units.
	|| 0x00FF: transition disabled; will accept link partner requests,
	||       however.
	|| -1: system default
	*/
	__TMS_INT16			U1Timeout;
	__TMS_INT16			U2Timeout;

	/*
	|| The USBD client specifies which power control features to allow in 
	|| the system.
	||
	|| Bit 0: Allow U1 for all USB devices
	|| Bit 1: Allow U2 for all USB devices
	|| Bit 2: Allow LTM for all USB devices that are capable
	|| -1:    system default
	*/
	__TMS_INT16			PowerControlFlags;

	__TMS_UINT32			MagicEnd;
	};

#define	__TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_MAGIC_BEGIN		\
	__TMS_UHIL_MEMTAG('<', 'S', 'c', 'f')

#define	__TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_MAGIC_END		\
	__TMS_UHIL_MEMTAG('>', 'S', 'c', 'f')

#define	__TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_INIT_V1(		\
	a_bU1Timeout,							\
	a_bU2Timeout,							\
	a_bPowerControlFlags						\
	)								\
	{								\
	__TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_MAGIC_BEGIN,		\
	sizeof(__TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG),		\
	(a_bU1Timeout),							\
	(a_bU2Timeout),							\
	(a_bPowerControlFlags),						\
	__TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_MAGIC_END		\
	}

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/
/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_MAGIC_BEGIN	\
   __TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_MAGIC_BEGIN
# define USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_MAGIC_END	\
   __TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_MAGIC_END
# define USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_INIT_V1(		\
	a_bU1Timeout,							\
	a_bU2Timeout,							\
	a_bPowerControlFlags						\
	)	\
	__TMS_USBPUMP_USBDI_USBDSUPERSPEED_CONFIG_INIT_V1(		\
	a_bU1Timeout,							\
	a_bU2Timeout,							\
	a_bPowerControlFlags						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_superspeed.h ****/
#endif /* _USBPUMP_USBDI_SUPERSPEED_H_ */
