/* usbpump_usbdi_roothub.h	Tue Nov 29 2005 13:56:23 tmm */

/*

Module:  usbpump_usbdi_roothub.h

Function:
	Information related to the root hub.

Version:
	V1.93c	Tue Nov 29 2005 13:56:23 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	November 2005

Revision history:
   1.93c  Tue Nov 29 2005 13:56:23  tmm
	Module created.

*/

#ifndef _USBPUMP_USBDI_ROOTHUB_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_ROOTHUB_H_

#ifndef _USBPUMP_USBDI_CLASS_H_
# include "usbpump_usbdi_class.h"
#endif

#ifndef _USBPUMP_USBDI_FUNCTION_H_
# include "usbpump_usbdi_function.h"
#endif

/****************************************************************************\
|
|	Define the partial pattern for matching the "root hub" VID/PID.
|
\****************************************************************************/

/* the base string */
#define	__TMS_USBPUMP_USBDI_ROOT_HUB_VID_PID	"vid=040e/0100"

/* the canonical dvice ID for the root hub */
#define	__TMS_USBPUMP_USBDI_ROOT_HUB_DEVID				\
	__TMS_USBPUMP_USBDI_ROOT_HUB_VID_PID ";r=0000;"

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_FUNCTION_ROOT_HUB);


/****************************************************************************\
|
|	uncloaked defs
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_ROOT_HUB_VID_PID	\
   __TMS_USBPUMP_USBDI_ROOT_HUB_VID_PID
# define USBPUMP_USBDI_ROOT_HUB_DEVID	\
   __TMS_USBPUMP_USBDI_ROOT_HUB_DEVID
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_roothub.h ****/
#endif /* _USBPUMP_USBDI_ROOTHUB_H_ */
