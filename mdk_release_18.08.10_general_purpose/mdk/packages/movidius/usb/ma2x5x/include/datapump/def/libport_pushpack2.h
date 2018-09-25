/* def/libport_pushpack2.h	Sun Jun 26 2005 18:41:08 tmm */

/*

Module:  def/libport_pushpack2.h

Function:
	Libport version of pushpack2.h

Version:
	V3.05g	Sun Jun 26 2005 18:41:08 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 2005

Revision history:
   3.05g  Sun Jun 26 2005 18:41:08  tmm
	Module created.

*/

/* this module may be included multiple times */

#ifndef __TMS_LIBPORT_HAVE_PUSHPACK
# error "Compiler not set up for def/libport_pushpack2.h -- fix arch/.../i/def/ccdefs.h"
#elif __TMS_LIBPORT_HAVE_PUSHPACK
# include "def/ccpushpack2.h"
#endif

/**** end of pushpack2.h ****/
