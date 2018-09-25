/* wmcdevmgmtcfg.h	Mon Jul  8 2002 11:41:02 tmm */

/*

Module:  wmcdevmgmtcfg.h

Function:
	WMC Device Management SubClass configuration linkage header file.

Version:
	V1.79a	Mon Jul  8 2002 11:41:02 tmm	Edit level 2

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
   1.79a  Sun Jul  7 2002 09:01:52  tmm
	Module created.

*/

#ifndef _WMCDEVMGMTCFG_H_		/* prevent multiple includes */
#define _WMCDEVMGMTCFG_H_

#ifndef _WMCDEVMGMT_H_
# include "wmcdevmgmt.h"
#endif

#ifndef _WMCTACFG_H_
# include "wmctacfg.h"
#endif

/****************************************************************************\
|
|	Here is the linkage to the default configuration.
|
\****************************************************************************/

/* The DevMgmt subclass's default TA configuration structure */
extern CONST UPROTO_WMCTA_CONFIG			
gk_WmcSubClass_DevMgmt_TaConfig;

/**** end of wmcdevmgmtcfg.h ****/
#endif /* _WMCDEVMGMTCFG_H_ */
