/* pumprctb.h	Mon Jun  1 2015 00:18:42 tmm */

/*

Module:  pumprctb.h

Function:
	Wrapper for stock "usbrctab.h" for use in the DataPump.

Version:
	V3.15b	Mon Jun  1 2015 00:18:42 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 1998, 2002, 2005, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	September 1998

Revision history:
   1.44b  Mon Sep 21 1998 03:23:45  tmm
	Module created.

   1.79a  6/22/2002  tmm
	Add clean types for the function pointers.

   1.93c  Sat Nov 26 2005 11:45:47  tmm
	Turn on the high-speed support.

   3.15b  Mon Jun  1 2015 00:18:42  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _PUMPRCTB_H_		/* prevent multiple includes */
#define _PUMPRCTB_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBRCTAB_H_

/*
|| Special work to include usbrctab.h
*/
# define __USBRCTAB_ROM			/* we don't need a ROM keyword	*/
# define __USBRCTAB_CONST	__TMS_CONST	/* we will take 'const', though	*/
# define __USBRCTAB_WANT_V3	1	/* want the V3 fields		*/
# define __USBRCTAB_WANT_OTHERS	0	/* no 'others' descriptors	*/
# define __USBRCTAB_RESTYPE	0	/* we want pointers		*/
# define __USBRCTAB_TYPESONLY	1	/* we only want the types	*/

# include "usbrctab.h"
#endif

#ifndef __USBRCTAB_VERSION
# error "old version of usbrctab.h in use, please update to V2.23f or later"
#endif

__TMS_TYPE_DEF(USBRC_RESPTR, struct __USBRCTAB_RESPTR);
__TMS_TYPE_DEF(USBRC_LANGIDPTR, struct __USBRCTAB_LANGIDPTR);
__TMS_TYPE_DEF(USBRC_GETDESCRIPTOR_FN, __USBRCTAB_GETDESCRIPTOR_FN);
__TMS_TYPE_DEF(USBRC_SETDESCRIPTOR_FN, __USBRCTAB_SETDESCRIPTOR_FN);
__TMS_TYPE_DEF(USBRC_OS_FEATURE_ROOT, struct __USBRCTAB_OS_FEATURE_ROOT);
__TMS_TYPE_DEF(USBRC_DCD_INIT_FN, __USBRCTAB_DCD_INIT_FN);

/**** end of pumprctb.h ****/
#endif /* _PUMPRCTB_H_ */
