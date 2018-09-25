/* hilbusio.h	Mon Mar 26 2001 23:45:24 tmm */

/*

Module:  hilbusio.h

Function:
	GENERIC:  hilbusio.h for Sparc memory mapped I/O

Version:
	V1.61h	Mon Mar 26 2001 23:45:24 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2001 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Tom Balon, MCCI Corporation	Feb 2001

Revision history:
   1.61h  3/26/2001  tmm
	added header tracking back.

*/

#ifndef _HILBUSIO_H_		/* prevent multiple includes */
#define _HILBUSIO_H_

/*

A header file named "hilbusio.h" must be found in the build tree.  Because
effectively all sparc ports can use memmapio.h, this module just has to
point to that (portable) file.

*/

#include "memmapio.h"

/**** end of hilbusio.h ****/
#endif /* _HILBUSIO_H_ */
