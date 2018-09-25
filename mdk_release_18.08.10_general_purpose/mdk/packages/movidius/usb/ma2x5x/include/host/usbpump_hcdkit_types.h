/* usbpump_hcdkit_types.h	Fri Mar 15 2013 09:41:47 chwon */

/*

Module:  usbpump_hcdkit_types.h

Function:
	Scalar types for HCDKIT.

Version:
	V3.11c	Fri Mar 15 2013 09:41:47 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2005, 2009, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Fri Aug 26 2005 05:18:04  tmm
	Module created.

   2.01a  Mon Nov 16 2009 12:22:01  chwon
	9172: Added USBPUMP_HCDKIT_HUBPORT_STATUS and USBPUMP_HCDKIT_ROOTHUB

   2.01a  Thu Dec 03 2009 14:53:38  chwon
	9172: Added USBPUMP_HCDKIT_ROOTHUBPORT

   3.11c  Fri Mar 15 2013 09:41:47  chwon
	16861: Added USBPUMP_HCDKIT_OTG and USBPUMP_HCDKIT_SWITCH_OTG.

*/

#ifndef _USBPUMP_HCDKIT_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_HCDKIT_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	Declare common central types used by HCDKIT implementation, so
|	that boundary header files can refer to the types (even if they
|	can't be used without the full DataPump host stack.
|
\****************************************************************************/

__TMS_TYPE_DEF_UNION	(USBPUMP_HCDKIT_HCD);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCDKIT_SWITCH);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCDKIT_HUBPORT_STATUS);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCDKIT_ROOTHUB);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCDKIT_ROOTHUBPORT);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCDKIT_OTG);
__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCDKIT_SWITCH_OTG);

/**** end of usbpump_hcdkit_types.h ****/
#endif /* _USBPUMP_HCDKIT_TYPES_H_ */
