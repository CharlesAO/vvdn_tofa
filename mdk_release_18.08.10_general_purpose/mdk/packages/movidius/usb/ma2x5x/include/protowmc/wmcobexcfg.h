/* wmcobexcfg.h	Mon Jul  8 2002 11:55:32 tmm */

/*

Module:  wmcobexcfg.h

Function:
	WMC OBEX SubClass configuration linkage header file.

Version:
	V1.79a	Mon Jul  8 2002 11:55:32 tmm	Edit level 1

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

#ifndef _WMCOBEXCFG_H_		/* prevent multiple includes */
#define _WMCOBEXCFG_H_

#ifndef _WMCOBEX_H_
# include "wmcobex.h"
#endif

#ifndef _WMCTACFG_H_
# include "wmctacfg.h"
#endif

/****************************************************************************\
|
|	Here is the linkage to the default configuration.
|
\****************************************************************************/

/* The Obex subclass's default TA configuration structure */
extern CONST UPROTO_WMCTA_CONFIG			
gk_WmcSubClass_Obex_TaConfig;

/**** end of wmcobexcfg.h ****/
#endif /* _WMCOBEXCFG_H_ */
