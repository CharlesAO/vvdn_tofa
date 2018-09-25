/* usbpumpioctlapi.h	Wed Jul 10 2002 08:36:08  tmm */

/*

Module:  usbpumpioctlapi.h

Function:
	API functions for implementing IOCTLs.

Version:
	V1.79a	Wed Jul 10 2002 08:36:08	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Wed Jul 10 2002 08:36:08  tmm
	Module created.

*/

#ifndef _USBPUMPIOCTLAPI_H_		/* prevent multiple includes */
#define _USBPUMPIOCTLAPI_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

/****************************************************************************\
|
|	Functions for use in the DataPump.
|
\****************************************************************************/

__TMS_BEGIN_DECLS
__TMS_USBPUMP_IOCTL_RESULT
UsbPumpIoctl_CheckParams(
	__TMS_USBPUMP_IOCTL_CODE Ioctl,
	__TMS_CONST __TMS_VOID *pInParam,
	__TMS_VOID *pOutParam
	);

#if !__TMS_CLOAKED_NAMES_ONLY && __TMS_DATAPUMP_COMPAT_V1_6
# define	UsbLib_IoctlCheckParams	UsbPumpIoctl_CheckParams
#endif /* !__TMS_CLOAKED_NAMES_ONLY && __TMS_DATAPUMP_COMPAT_V1_6 */

__TMS_END_DECLS

/**** end of usbpumpioctlapi.h ****/
#endif /* _USBPUMPIOCTLAPI_H_ */
