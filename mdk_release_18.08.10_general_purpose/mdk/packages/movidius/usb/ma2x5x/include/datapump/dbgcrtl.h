/* dbgcrtl.h	Mon Aug 18 1997 12:49:51  johnk */

/*

Module:  dbgcrtl.h

Function:
	Definitions for using the 'C' runtime library I/O routines
	 as the UHIL output device.

Version:
	V1.10c	Mon Aug 18 1997 12:49:51	johnk	Edit level 1

Copyright notice:
	This file copyright (C) 1997 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	John Keys, MCCI Corporation	August 1997

Revision history:
   1.10c  Mon Aug 18 1997 12:49:51  johnk
	Module created.

*/

#ifndef _DBGCRTL_H_		/* prevent multiple includes */
#define _DBGCRTL_H_


__BEGIN_DECLS
VOID		UPF_CrtConsoleSetup ARGS((VOID));
__END_DECLS

/**** end of dbgcrtl.h ****/
#endif /* _DBGCRTL_H_ */
