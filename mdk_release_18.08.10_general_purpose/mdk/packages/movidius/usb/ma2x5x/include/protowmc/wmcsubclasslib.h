/* wmcsubclasslib.h	Sat Jul 27 2002 10:37:44 tmm */

/*

Module:  wmcsubclasslib.h

Function:
	API functions for subclass implementations.

Version:
	V1.79a	Sat Jul 27 2002 10:37:44 tmm	Edit level 2

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
   1.79a  Sun Jul  7 2002 05:40:22  tmm
	Module created.

*/

#ifndef _WMCSUBCLASSLIB_H_		/* prevent multiple includes */
#define _WMCSUBCLASSLIB_H_

#ifndef _PROTOWMC_H_
# include "protowmc.h"
#endif

#ifndef _USBIOCTLEDGECODES_H_
# include "usbioctledgecodes.h"
#endif

#ifndef _USBIOCTL_MODEM_H_
# include "usbioctl_modem.h"
#endif

/****************************************************************************\
|
|	IOCTLs
|
\****************************************************************************/


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UPROTOWMCSUBCLASS_ACTIVATE_FN WmcSubClassLib_Activate;

__TMS_UPROTOWMCSUBCLASS_BUS_STATE_CHANGE_FN WmcSubClassLib_BusStateChange;

__TMS_UPROTO_WMCSUBCLASS *
WmcSubClassLib_CreateSubClass(
	__TMS_UPROTO_WMCTA *pTa,
	__TMS_CONST __TMS_UPROTO_WMCTA_CONFIG *pConfig,
	__TMS_CONST __TMS_TEXT *pSubClassName
	);

__TMS_UPROTOWMCSUBCLASS_DEACTIVATE_FN WmcSubClassLib_Deactivate;

__TMS_VOID
WmcSubClassLib_DeleteSubClass(
	__TMS_UPROTO_WMCSUBCLASS *pSubClass
	);

__TMS_USBPUMP_IOCTL_RESULT 
WmcSubClassLib_Ioctl(
	__TMS_UPROTO_WMCSUBCLASS *pSubClass,
	__TMS_USBPUMP_IOCTL_CODE Ioctl,
	__TMS_CONST __TMS_VOID *pInParam,
	__TMS_VOID *pOutParam,
	__TMS_UDATASTREAM *pInStream,
	__TMS_UDATASTREAM *pOutStream
	);

__TMS_BOOL
WmcSubClassLib_StartDataPlane(
	__TMS_UPROTO_WMCSUBCLASS *pSubClass,
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR iPlane
	);

__TMS_VOID
WmcSubClassLib_StopDataPlane(
	__TMS_UPROTO_WMCSUBCLASS *pSubClass,
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR iPlane
	);

__TMS_END_DECLS


/**** end of wmcsubclasslib.h ****/
#endif /* _WMCSUBCLASSLIB_H_ */
