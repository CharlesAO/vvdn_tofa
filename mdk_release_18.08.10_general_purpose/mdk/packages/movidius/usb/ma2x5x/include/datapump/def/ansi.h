/* ansi.h	Sat Mar 10 2001 15:39:00 tmm */

/*

Module:  def/ansi.h

Function:
	Define all the low-level compiler-specific things.

Version:
	V3.02d	Sat Mar 10 2001 15:39:00 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2001 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2001

Revision history:
   3.02d  Sat Mar 10 2001 15:39:00  tmm
	Module created.

*/

#ifndef	_DEF_ANSI_H_		/* prevent multiple includes */
#define	_DEF_ANSI_H_

/****************************************************************************\
|
|  Types which are fundamental to the combination of "arch" and compiler
|  are defined here.  All are defined with the prefix __TMS_xxx, in order
|  that we not pollute the name space.  Note that this scheme is distinctly
|  similar to the _BSD_type scheme used in NetBSD; but we use __TMS_ so
|  that we don't have any collisions in case include files get screwed up.
|
|  When an include file converts __TMS_<type> into <type>, it must
|  also undefine the <type>.
|
\****************************************************************************/

#define	__TMS_CLOCK_T_		unsigned long	/* clock() */
#define	__TMS_ARG_CLOCK_T_	unsigned long	/* widened */
#define	__TMS_PTRDIFF_T_	int		/* ptr1 - ptr2 */
#define	__TMS_ARG_PTRDIFF_T_	int		/* widened */
#define	__TMS_SIZE_T_		unsigned int	/* sizeof() */
#define	__TMS_ARG_SIZE_T_	unsigned int	/* widened */
#define	__TMS_SSIZE_T_		int		/* byte count or error */
#define	__TMS_ARG_SSIZE_T_	int		/* widened */
#define	__TMS_TIME_T_		unsigned long	/* time() */
#define	__TMS_ARG_TIME_T_	unsigned long	/* widened */
#define	__TMS_CLOCKID_T_	int		/* future: e.g., CLOCK_REAL_TIME */
#define	__TMS_TIMER_T_		int		/* future:  not clear that
						|| this is used at all in NetBSD
						*/

/**** a va_list type must be defined because of ansi nastiness ****/
#define	__TMS_VA_LIST_		char *		/* va_list */

/****************************************************************************\
|
|  We chose to follow a somewhat similar policy to NetBSD with respect
|  to wide chars.  However:  on some machines, wide chars may need to
|  be longs -- e.g., 16-bit-int machines.  Furthermore, it is not clear
|  that the is... routines are even supposed to work on wchar_t types.
|
|  In any event, WCHAR_T will get mapped onto "wide chars", and CTYPE_T
|  will be used for the ctype function args.
|
\****************************************************************************/

#define	__TMS_WCHAR_T_	int			/* wchar_t, a 32-bit int */
#define	__TMS_RUNE_T_	int			/* must be same as WCHAR_t */
#define	__TMS_CTYPE_T_	int			/* args to ctype fns. */

/**** end of def/ansi.h ****/
#endif /* _DEF_ANSI_H_ */
