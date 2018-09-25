/* usbpump_usbdi_class_msd_types.h	Wed Aug 14 2013 13:44:46 chwon */

/*

Module:  usbpump_usbdi_class_msd_types.h

Function:
	Definition of mass storage class driver types

Version:
	V3.11d	Wed Aug 14 2013 13:44:46 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	August 2013

Revision history:
   3.11d  Wed Aug 14 2013 13:44:46 chwon
	9010: Module created.

*/

#ifndef _USBPUMP_USBDI_CLASS_MSD_TYPES_H_	/* prevent multiple includes */
#define _USBPUMP_USBDI_CLASS_MSD_TYPES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

/****************************************************************************\
|
|	Mass Storage Class Driver types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_MSD_CONFIG);

__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_CLASS_MSD_INCALL);
__TMS_TYPE_DEF_UNION	(USBPUMP_USBDI_FUNCTION_MSD_INCALL);

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_MSD_REQUEST);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_CLASS_MSD_REQUEST_CODE, UINT8);
__TMS_TYPE_DEF_ARG	(USBPUMP_USBDI_CLASS_MSD_REQUEST_STAT, UINT8);

/**** end of usbpump_usbdi_class_msd_types.h ****/
#endif /* _USBPUMP_USBDI_CLASS_MSD_TYPES_H_ */
