/* usbpump_devspeed.h	Tue Sep 10 2013 11:43:22 chwon */

/*

Module:  usbpump_devspeed.h

Function:
	Types, manifest constants and predicates for representing device
	speed.

Version:
	V3.11d	Tue Sep 10 2013 11:43:22 chwon	Edit level 6

Copyright notice:
	This file copyright (C) 2005, 2007, 2009, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   1.91d  Wed Jun  1 2005 23:34:47  tmm
	Module created.

   1.93c  Fri Dec 09 2005 09:12:05  cccho
	Add macros for device speed check.

   1.97j  Fri Dec 14 2007 02:14:30  tmm
	3647: Add speeds for wireless and (future) super-speed.
	
   2.01a  Mon Jun 15 2009 16:37:12  tmm
	8253: Finalize super-speed support.

   3.11d  Tue Sep 10 2013 11:43:22  chwon
	17686: Fixed typo in documentation.

*/

#ifndef _USBPUMP_DEVSPEED_H_		/* prevent multiple includes */
#define _USBPUMP_DEVSPEED_H_

# ifndef _USBPUMP_TYPES_H_
#  include "usbpump_types.h"
# endif

/*

Type:	USBPUMP_DEVICE_SPEED

Index:	Type:	USBPUMP_DEVICE_SPEED
	Type:	PUSBPUMP_DEVICE_SPEED
	Type:	ARG_USBPUMP_DEVICE_SPEED
	Type:	PARG_USBPUMP_DEVICE_SPEED
	Type:	USBPUMP_DEVICE_SPEED_MASK
	Type:	PUSBPUMP_DEVICE_SPEED_MASK
	Type:	ARG_USBPUMP_DEVICE_SPEED_MASK
	Type:	PARG_USBPUMP_DEVICE_SPEED_MASK
	Macro:	USBPUMP_IS_SUPER_SPEED()
	Macro:	USBPUMP_IS_WIRELESS_SPEED()
	Macro:	USBPUMP_IS_HIGH_SPEED()
	Macro:	USBPUMP_IS_FULL_SPEED()
	Macro:	USBPUMP_IS_LOW_SPEED()
	Macro:	USBPUMP_NOT_SUPER_SPEED()
	Macro:	USBPUMP_NOT_WIRELESS_SPEED()
	Macro:	USBPUMP_NOT_HIGH_SPEED()
	Macro:	USBPUMP_NOT_FULL_SPEED()
	Macro:	USBPUMP_NOT_LOW_SPEED()
	Const:	USBPUMP_DEVICE_SPEED_LOW
	Const:	USBPUMP_DEVICE_SPEED_FULL
	Const:	USBPUMP_DEVICE_SPEED_HIGH
	Const:	USBPUMP_DEVICE_SPEED_WIRELESS
	Const:	USBPUMP_DEVICE_SPEED_SUPER
	Const:	USBPUMP_DEVICE_SPEED_LOW_MASK
	Const:	USBPUMP_DEVICE_SPEED_FULL_MASK
	Const:	USBPUMP_DEVICE_SPEED_HIGH_MASK
	Const:	USBPUMP_DEVICE_SPEED_WIRELESS_MASK
	Const:	USBPUMP_DEVICE_SPEED_SUPER_MASK
	Macro:	USBPUMP_NAMES__INIT

Function:
	Scalar type representing the device speed.

Definition:
	typedef {small-integer-type} USBPUMP_DEVICE_SPEED, 
				   *PUSBPUMP_DEVICE_SPEED;

	BOOL USBPUMP_IS_LOW_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);
	BOOL USBPUMP_IS_FULL_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);
	BOOL USBPUMP_IS_HIGH_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);
	BOOL USBPUMP_IS_WIRELESS_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);
	BOOL USBPUMP_IS_SUPER_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);

	BOOL USBPUMP_NOT_LOW_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);
	BOOL USBPUMP_NOT_FULL_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);
	BOOL USBPUMP_NOT_HIGH_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);
	BOOL USBPUMP_NOT_WIRELESS_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);
	BOOL USBPUMP_NOT_SUPER_SPEED(
			USBPUMP_DEVICE_SPEED speed
			);

	CONST TEXT *{device_speed_names}[] = USBPUMP_DEVICE_SPEED__INIT;
	
Description:
	The various values of |USBPUMP_DEVICE_SPEED| represent the
	operation speed of a device.  The values defined are:

	|USBPUMP_DEVICE_SPEED_LOW|
		Low speed (1.5 MHz bus clock).

	|USBPUMP_DEVICE_SPEED_FULL|
		Full speed (12 MHz bus clock).

	|USBPUMP_DEVICE_SPEED_HIGH|
		High speed (480 MHz bus clock)

	|USBPUMP_DEVICE_SPEED_WIRELESS|
		Wireless speed (variable clock, 1.024ms frame rate)

	|USBPUMP_DEVICE_SPEED_SUPER|
		"Super speed" (for future use)

	These values are enough to distinguish the semantics of the
	different possible wired and wireless bus speeds.

	With the various proposed USB enhancements, it's possible that we'll
	need new device speed discriminators.  Anyway, it's dangerous to
	directly encode operations on enumerated types, so we have the
	following predicate macros that return boolean answers:

	USBPUMP_IS_LOW_SPEED(), USBPUMP_IS_FULL_SPEED(),
	USBPUMP_IS_HIGH_SPEED(), USBPUMP_IS_WIRELESS_SPEED(),
	USBPUMP_IS_SUPER_SPEED()
		These macros return TRUE if the speed value indicates
		low, full, or high-speed wired USB, or Wireless USB, or
		super-speed wired USB.

	USBPUMP_NOT_LOW_SPEED(), USBPUMP_NOT_FULL_SPEED(),
	USBPUMP_NOT_HIGH_SPEED(), USBPUMP_NOT_WIRELESS_SPEED(),
	USBPUMP_NOT_SUPER_SPEED()
		These are convenience macros, so the coder can easily
		and clearly represent (for example) "not-high-speed" vs 
		"high-speed" binary disjunctions.  

	In order to represent collections of possible speed, we have
	|USBPUMP_DEVICE_SPEED_MASK| values.  These are bit masks, with
	one bit defined for each speed.  The bits are named
	|USBPUMP_DEVICE_SPEED_MASK_LOW|, |USBPUMP_DEVICE_SPEED_MASK_FULL|, 
	|USBPUMP_DEVICE_SPEED_MASK_HIGH|, |USBPUMP_DEVICE_SPEED_MASK_WIRELESS|,
	and |USBPUMP_DEVICE_SPEED_MASK_SUPER|.

Notes:
	Remember, the opposite of high-speed is *not* "full-speed", but 
	rather 'NOT-high-speed'.

See Also:
	
*/

/* __TMS_TYPE_DEF_ARG(USBPUMP_DEVICE_SPEED, UINT8); */
/* __TMS_TYPE_DEF_ARG(USBPUMP_DEVICE_SPEED_MASK, UINT32); */

#define	__TMS_USBPUMP_DEVICE_SPEED_LOW		0u
#define	__TMS_USBPUMP_DEVICE_SPEED_FULL		1u
#define	__TMS_USBPUMP_DEVICE_SPEED_HIGH		2u
#define	__TMS_USBPUMP_DEVICE_SPEED_WIRELESS	3u
#define	__TMS_USBPUMP_DEVICE_SPEED_SUPER	4u

#define	__TMS_USBPUMP_DEVICE_SPEED_MASK_LOW	\
			(1u << __TMS_USBPUMP_DEVICE_SPEED_LOW)
#define	__TMS_USBPUMP_DEVICE_SPEED_MASK_FULL	\
			(1u << __TMS_USBPUMP_DEVICE_SPEED_FULL)
#define	__TMS_USBPUMP_DEVICE_SPEED_MASK_HIGH	\
			(1u << __TMS_USBPUMP_DEVICE_SPEED_HIGH)
#define	__TMS_USBPUMP_DEVICE_SPEED_MASK_WIRELESS	\
			(1u << __TMS_USBPUMP_DEVICE_SPEED_WIRELESS)
#define	__TMS_USBPUMP_DEVICE_SPEED_MASK_SUPER	\
			(1u << __TMS_USBPUMP_DEVICE_SPEED_SUPER)


#define	__TMS_USBPUMP_IS_LOW_SPEED(x)	((x) == __TMS_USBPUMP_DEVICE_SPEED_LOW)
#define	__TMS_USBPUMP_IS_FULL_SPEED(x)	((x) == __TMS_USBPUMP_DEVICE_SPEED_FULL)
#define	__TMS_USBPUMP_IS_HIGH_SPEED(x)	((x) == __TMS_USBPUMP_DEVICE_SPEED_HIGH)
#define	__TMS_USBPUMP_IS_WIRELESS_SPEED(x)				\
					((x) == __TMS_USBPUMP_DEVICE_SPEED_WIRELESS)
#define	__TMS_USBPUMP_IS_SUPER_SPEED(x)	((x) == __TMS_USBPUMP_DEVICE_SPEED_SUPER)

#define	__TMS_USBPUMP_NOT_LOW_SPEED(x)	((x) != __TMS_USBPUMP_DEVICE_SPEED_LOW)
#define	__TMS_USBPUMP_NOT_FULL_SPEED(x)	((x) != __TMS_USBPUMP_DEVICE_SPEED_FULL)
#define	__TMS_USBPUMP_NOT_HIGH_SPEED(x)	((x) != __TMS_USBPUMP_DEVICE_SPEED_HIGH)
#define	__TMS_USBPUMP_NOT_WIRELESS_SPEED(x)	\
					((x) !=	__TMS_USBPUMP_DEVICE_SPEED_WIRELESS)
#define	__TMS_USBPUMP_NOT_SUPER_SPEED(x)	\
					((x) != __TMS_USBPUMP_DEVICE_SPEED_SUPER)

/*

Name:	USBPUMP_DEVICE_SPEED_MASK_HAS_OTHER_SPEEDS()

Function:
	Given a USBPUMP_DEVICE_SPEED_MASK with bits for supported
	speeds, find out if multiple speeds are supported.

Definition:
	BOOL USBPUMP_DEVICE_SPEED_MASK_HAS_OTHER_SPEEDS(
		USBPUMP_DEVICE_SPEED_MASK x
		);

Description:
	This routine examines the speed mask, and returns TRUE
	if and only if at least two bits are set in the speed 
	mask.  For USB 2.0 compliance, the two bits that are 
	set must be USBPUMP_DEVICE_SPEED_MASK_FULL and
	USBPUMP_DEVICE_SPEED_MASK_HIGH.  

Notes:
	This is implemented as a macro, so x may be evaluated
	several times.

	This routine will return FALSE if USBPUMP_DEVICE_SPEED_MASK
	is zero.  This is necessary for support of older DCDs that
	have not yet been converted for the V2.0 DataPump.

	The results of this routine are not specified if x is not
	a legal speed mask according to USB 2.0.	

Bugs:
	This macro only considers whether a device supports both
	full-speed and high-speed, as "OTHER" speeds only really apply
	in this case.  "SuperSpeed" or "Wireless" speed is not 
	an "OTHER" speed in this case (just as it's not for the 
	USB 2.0 descriptor commands).

*/

#define	__TMS_USBPUMP_DEVICE_SPEED_MASK_HAS_OTHER_SPEEDS(x) 		\
	(((x) & (__TMS_USBPUMP_DEVICE_SPEED_MASK_FULL |			\
		 __TMS_USBPUMP_DEVICE_SPEED_MASK_HIGH)) ==		\
		(__TMS_USBPUMP_DEVICE_SPEED_MASK_FULL | 		\
		 __TMS_USBPUMP_DEVICE_SPEED_MASK_HIGH))

/*

Name:	USBPUMP_DEVICE_SPEED_OTHER()

Function:
	Given a USBPUMP_DEVICE_SPEED for a device, return other
	speed.

Definition:
	USBPUMP_DEVICE_SPEED 
	    USBPUMP_DEVICE_SPEED_OTHER(
		USBPUMP_DEVICE_SPEED Speed
		);

Description:
	This routine examines the speed, which should be one
	of the two legal operating speeds for a USB 2.0 device.
	It returns the other legal operating speed.

Notes:
	This is implemented as a macro, so Speed may be evaluated
	several times.

	The results of this routine are not specified if Speed
       	is not a legal operating speed for a high-speed device.

*/

#define	__TMS_USBPUMP_DEVICE_SPEED_OTHER(x)				\
	((x) == __TMS_USBPUMP_DEVICE_SPEED_HIGH 			\
	      ? __TMS_USBPUMP_DEVICE_SPEED_FULL				\
	      : __TMS_USBPUMP_DEVICE_SPEED_HIGH)

#define	__TMS_USBPUMP_DEVICE_SPEED__INIT				\
	{								\
	"USBPUMP_DEVICE_SPEED_LOW",					\
	"USBPUMP_DEVICE_SPEED_FULL",					\
	"USBPUMP_DEVICE_SPEED_HIGH",					\
	"USBPUMP_DEVICE_SPEED_WIRELESS",				\
	"USBPUMP_DEVICE_SPEED_SUPER"					\
	}

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_DEVICE_SPEED_LOW	\
   __TMS_USBPUMP_DEVICE_SPEED_LOW
# define USBPUMP_DEVICE_SPEED_FULL	\
   __TMS_USBPUMP_DEVICE_SPEED_FULL
# define USBPUMP_DEVICE_SPEED_HIGH	\
   __TMS_USBPUMP_DEVICE_SPEED_HIGH
# define USBPUMP_DEVICE_SPEED_WIRELESS	\
   __TMS_USBPUMP_DEVICE_SPEED_WIRELESS
# define USBPUMP_DEVICE_SPEED_SUPER	\
   __TMS_USBPUMP_DEVICE_SPEED_SUPER
# define USBPUMP_DEVICE_SPEED_MASK_LOW	\
   __TMS_USBPUMP_DEVICE_SPEED_MASK_LOW
# define USBPUMP_DEVICE_SPEED_MASK_FULL	\
   __TMS_USBPUMP_DEVICE_SPEED_MASK_FULL
# define USBPUMP_DEVICE_SPEED_MASK_HIGH	\
   __TMS_USBPUMP_DEVICE_SPEED_MASK_HIGH
# define USBPUMP_DEVICE_SPEED_MASK_WIRELESS	\
   __TMS_USBPUMP_DEVICE_SPEED_MASK_WIRELESS
# define USBPUMP_DEVICE_SPEED_MASK_SUPER	\
   __TMS_USBPUMP_DEVICE_SPEED_MASK_SUPER
# define USBPUMP_IS_LOW_SPEED(x)	\
   __TMS_USBPUMP_IS_LOW_SPEED(x)
# define USBPUMP_IS_FULL_SPEED(x)	\
   __TMS_USBPUMP_IS_FULL_SPEED(x)
# define USBPUMP_IS_HIGH_SPEED(x)	\
   __TMS_USBPUMP_IS_HIGH_SPEED(x)
# define USBPUMP_IS_WIRELESS_SPEED(x)	\
   __TMS_USBPUMP_IS_WIRELESS_SPEED(x)
# define USBPUMP_IS_SUPER_SPEED(x)	\
   __TMS_USBPUMP_IS_SUPER_SPEED(x)
# define USBPUMP_NOT_LOW_SPEED(x)	\
   __TMS_USBPUMP_NOT_LOW_SPEED(x)
# define USBPUMP_NOT_FULL_SPEED(x)	\
   __TMS_USBPUMP_NOT_FULL_SPEED(x)
# define USBPUMP_NOT_HIGH_SPEED(x)	\
   __TMS_USBPUMP_NOT_HIGH_SPEED(x)
# define USBPUMP_NOT_WIRELESS_SPEED(x)	\
   __TMS_USBPUMP_NOT_WIRELESS_SPEED(x)
# define USBPUMP_NOT_SUPER_SPEED(x)	\
   __TMS_USBPUMP_NOT_SUPER_SPEED(x)
# define USBPUMP_DEVICE_SPEED_MASK_HAS_OTHER_SPEEDS(x)	\
   __TMS_USBPUMP_DEVICE_SPEED_MASK_HAS_OTHER_SPEEDS(x)
# define USBPUMP_DEVICE_SPEED_OTHER(x)	\
   __TMS_USBPUMP_DEVICE_SPEED_OTHER(x)
# define USBPUMP_DEVICE_SPEED__INIT	\
   __TMS_USBPUMP_DEVICE_SPEED__INIT
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_devspeed.h ****/
#endif /* _USBPUMP_DEVSPEED_H_ */
