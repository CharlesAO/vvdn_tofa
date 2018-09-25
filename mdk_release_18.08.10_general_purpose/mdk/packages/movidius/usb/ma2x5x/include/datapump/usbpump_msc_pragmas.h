/* usbpump_msc_pragmas.h	Tue Nov 27 2007 08:38:58 tmm */

/*

Module:  usbpump_msc_pragmas.h

Function:
	Pragmas to make MSVC (especially V6) behave properly.

Version:
	V1.97j	Tue Nov 27 2007 08:38:58 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	November 2007

Revision history:
   1.97j  Tue Nov 27 2007 08:38:58  tmm
	Module created.

*/

#ifndef _USBPUMP_MSC_PRAGMAS_H_		/* prevent multiple includes */
#define _USBPUMP_MSC_PRAGMAS_H_

#if ! defined(__TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI) 
  /* we're using an older version of libport -- adapt */
# ifdef _MSVC
#  warning __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI not defined, assuming TRUE
#  define __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI 1
# else
#  warning __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI not defined, assuming FALSE
#  define __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI 0
# endif
#endif

#if __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI

/****************************************************************************\
|
|	For /W4 (aka -W4) compiles, we don't want warnings about
|	constant conditional expressions -- we use them a lot.
|
\****************************************************************************/

#pragma warning(disable: 4127)

/****************************************************************************\
|
|	For /W4 (aka -W4) compiles, we don't want MSVC6 warnings about
|	things like the following:  
|
|		UCHAR a, b; ...; a = (cond) ? b : 0;
|
|	MSVC actually warns about the fact that the zero is being converted
|	to a UCHAR and might be truncated.  Alas, to get rid of these, we
|	need to turn off warning 4244.
|
\****************************************************************************/

#pragma warning(disable: 4244)

/****************************************************************************\
|
|	For /W4 (aka -W4) compiles, we don't want warnings about
|	static inlines that got optimized out -- the Windows header files
|	are full of them, even though we don't use them.
|
\****************************************************************************/

#pragma warning(disable: 4514)

#endif /* __TMS_LIBPORT_NEED_MSC_PRAGMAS_FOR_MCCI */

/**** end of usbpump_msc_pragmas.h ****/
#endif /* _USBPUMP_MSC_PRAGMAS_H_ */
