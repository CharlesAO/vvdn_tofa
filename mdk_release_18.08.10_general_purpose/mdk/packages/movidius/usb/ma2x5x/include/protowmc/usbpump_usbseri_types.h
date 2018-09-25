/* usbpump_usbseri_types.h	Wed Nov 10 2010 14:29:04 chwon */

/*

Module:  usbpump_usbseri_types.h

Function:
	Types for USBSERI.

Version:
	V3.01d	Wed Nov 10 2010 14:29:04 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2006, 2010 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2006

Revision history:
   1.00a  Tue May 23 2006 06:36:03  tmm
	Module created.

   3.01d  Wed Nov 10 2010 14:29:04  chwon
	11849: add USBSERI_WMC_OBJECT_MODEM_CONFIG type definition.

*/

#ifndef _USBPUMP_USBSERI_TYPES_H_		/* prevent multiple includes */
#define _USBPUMP_USBSERI_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif


/****************************************************************************\
|
|	Forward type references
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBSERI_UART);
__TMS_TYPE_DEF_STRUCT(USBSERI_WMC_OBJECT_MODEM);
__TMS_TYPE_DEF_STRUCT(USBSERI_WMC_OBJECT_MODEM_CONFIG);

/**** end of usbpump_usbseri_types.h ****/
#endif /* _USBPUMP_USBSERI_TYPES_H_ */
