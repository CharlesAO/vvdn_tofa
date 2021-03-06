/* usbpump_host_types.h	Tue Feb 09 2010 08:32:18 ssyang */

/*

Module:  usbpump_host_types.h

Function:
	The various scalar types that are needed for the MCCI USB DataPump
	host stack component (crossing USBDI and HCD)

Version:
	V3.01a	Tue Feb 09 2010 08:32:18 ssyang	Edit level 3

Copyright notice:
	This file copyright (C) 2005-2006, 2010 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2005

Revision history:
   1.91d  Mon May 30 2005 17:46:27  tmm
	Module created.

   1.93e  Wed Feb  1 2006 20:31:54  tmm
	Recast to match latest code.

   3.01a  Tue Feb 09 2010 08:32:18  ssyang
	9675: change to use __TMS_TYPE_DEF_ARG, not __TMS_TYPE_DEF macro.
	
*/

#ifndef _USBPUMP_HOST_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_HOST_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	Bandwidth constants
|
\****************************************************************************/

__TMS_TYPE_DEF_ARG(USBPUMP_USBDI_BANDWIDTH, UINT16);

#define	__TMS_USBPUMP_USBDI_BANDWIDTH_MAX	0xFFFF

/****************************************************************************\
|
|	Uncloaked values
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_BANDWIDTH_MAX	\
   __TMS_USBPUMP_USBDI_BANDWIDTH_MAX
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_host_types.h ****/
#endif /* _USBPUMP_HOST_TYPES_H_ */
