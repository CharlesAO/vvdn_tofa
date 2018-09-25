/* wmcdiagcfg.h	Thu Mar 17 2005 11:58:22 chwon */

/*

Module:  wmcdiagcfg.h

Function:
	WMC DIAG SubClass configuration linkage header file.

Version:
	V1.91b	Thu Mar 17 2005 11:58:22 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	ChaeHee Won, MCCI Corporation	March 2005

Revision history:
   1.91b  Thu Mar 17 2005 11:58:22  chwon
	Module created.

*/

#ifndef _WMCDIAGCFG_H_		/* prevent multiple includes */
#define _WMCDIAGCFG_H_

#ifndef _WMCDIAG_H_
# include "wmcdiag.h"
#endif

#ifndef _WMCTACFG_H_
# include "wmctacfg.h"
#endif

/****************************************************************************\
|
|	Here is the linkage to the default configuration.
|
\****************************************************************************/

/* The Diag subclass's default TA configuration structure */
extern __TMS_CONST __TMS_UPROTO_WMCTA_CONFIG	gk_WmcSubClass_Diag_TaConfig;

/**** end of wmcdiagcfg.h ****/
#endif /* _WMCDIAGCFG_H_ */
