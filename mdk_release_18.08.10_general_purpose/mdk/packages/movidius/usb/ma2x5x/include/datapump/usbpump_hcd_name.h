/* usbpump_hcd_name.h	Sat Sep  6 2014 23:28:41 tmm */

/*

Module:  usbpump_hcd_name.h

Function:
	USBPUMP_HCD_HAME() -- in core DataPump so can search for HCDs w/o
	other host stack components.

Version:
	V3.13b	Sat Sep  6 2014 23:28:41 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 2014

Revision history:
   3.13b  Sat Sep  6 2014 23:28:41  tmm
	18362: Module created.

*/

#ifndef _USBPUMP_HCD_NAME_H_		/* prevent multiple includes */
#define _USBPUMP_HCD_NAME_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/* the default name */
#define	__TMS_USBPUMP_HCD_NAME(Text)	Text ".hcd.mcci.com"

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HCD_NAME(Text)	\
   __TMS_USBPUMP_HCD_NAME(Text)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcd_name.h ****/
#endif /* _USBPUMP_HCD_NAME_H_ */