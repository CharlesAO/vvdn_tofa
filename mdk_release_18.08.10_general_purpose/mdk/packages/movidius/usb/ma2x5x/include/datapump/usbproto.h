/* usbproto.h	Mon Jun  1 2015 00:20:33 tmm */

/*

Module:  usbproto.h

Function:
	Represents all USB protocols.

Version:
	V3.15b	Mon Jun  1 2015 00:20:33 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2001, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	December 2001

Revision history:
   1.70a  Sat Dec  8 2001 20:51:54  tmm
	Module created.

   3.15b  Mon Jun  1 2015 00:20:33  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _USBPROTO_H_		/* prevent multiple includes */
#define _USBPROTO_H_

/****************************************************************************\
|
|	At init time we need to have a list of USB protocols.  This list
|	effectively configures this instance of the DataPump by providing
|	the list of protocols.  The initialization scheme in the V1 DataPump
|	is dynamic; the config table is logically per UDEVICE, 
|
\****************************************************************************/




/**** end of usbproto.h ****/
#endif /* _USBPROTO_H_ */
