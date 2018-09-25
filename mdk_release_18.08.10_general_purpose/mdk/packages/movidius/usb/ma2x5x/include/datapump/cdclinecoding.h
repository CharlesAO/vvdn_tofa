/* cdclinecoding.h	Tue Feb 19 2003 16:40:00 maw */

/*

Module:  cdclinecoding.h

Function:
	CDC line-coding structure, in internal form

Version:
	V1.81a	Tue Feb 19 2003 16:40:00 maw	Edit level 2

Copyright notice:
	This file copyright (C) 2002 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 2002

Revision history:
   1.79a  Mon Jun 24 2002 00:22:11  tmm
	Module created.
	
   1.81a  Tue Feb 19 2003 16:40:00 maw
   	Cloak all structure elements

*/

#ifndef _CDCLINECODING_H_		/* prevent multiple includes */
#define _CDCLINECODING_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/****************************************************************************\
|
|	The internal line-coding structure
|
\****************************************************************************/

/*
|| We define a LINE_CODING structure which represents the internal form
|| of the parameter passed to/from the GET/SET_LINE_CODING management
|| elements.
*/
__TMS_TYPE_DEF_STRUCT(USB_Comm_LINE_CODING);

struct __TMS_STRUCTNAME(USB_Comm_LINE_CODING)
	{
	/*
	|| dwDTERate represents the requested baud rate, in BPS.  It is
	|| stored by this level, but is otherwise not used in any way.
	*/
	__TMS_UINT32		dwDTERate;

	/*
	|| bCharFormat represents the number of stop bits.  The value 
	|| is stored as a choice from USB_Comm_LINE_CODING_bCharFormat_...,
	|| where ... is 1STOP, 1_5STOP, 2STOP.  The name is taken from
	|| the CDC specification; it really should be bStopBits.
	*/
	__TMS_UINT8		bCharFormat;

	/*
	|| bParityType indicates how parity is to be calculated.  The
	|| value is chosen from USB_Comm_LINE_CODING_bParityType_..., where
	|| ... is NONE, ODD< EVEN, MARK, SPACE.
	*/
	__TMS_UINT8		bParityType;

	/*
	|| bDataBits indicates the number of data bits per character; its
	|| permitted values are 5, 6, 7, 8 or 16.  Our implementation will
	|| fail an attempt to chose any value other than 5, 6, 7 or 8.
	*/
	__TMS_UINT8		bDataBits;
	};

/**** end of cdclinecoding.h ****/
#endif /* _CDCLINECODING_H_ */
