/* wmcsklcfg.h	Sun Jul  7 2002 09:01:52  tmm */

/*

Module:  wmcsklcfg.h

Function:
	Sample (skeletal) WMC SubClass configuration linkage header file.

Version:
	V1.79a	Sun Jul  7 2002 09:01:52	tmm	Edit level 1

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

#ifndef _WMCSKLCFG_H_		/* prevent multiple includes */
#define _WMCSKLCFG_H_

#ifndef _WMCSKL_H_
# include "wmcskl.h"
#endif

#ifndef _WMCTACFG_H_
# include "wmctacfg.h"
#endif

/****************************************************************************\
|
|	Here is the linkage to the default configuration.
|
\****************************************************************************/

/* The Skl subclass's default TA configuration structure */
extern CONST UPROTO_WMCTA_CONFIG			
gk_WmcSubClass_Skl_TaConfig;

/**** end of wmcsklcfg.h ****/
#endif /* _WMCSKLCFG_H_ */
