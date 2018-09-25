/* mccidefs.h	Sun Aug 26 2001 23:01:56 tmm */

/*

Module:  mccidefs.h

Function:
	The conventional MCCI definitions.

Version:
	V1.63a	Sun Aug 26 2001 23:01:56 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 1997-2000 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	April 1997

Revision history:
   1.00a  Sat Apr 19 1997 03:11:00  tmm
	Module created.

   1.61a  2001/1/1  tmm
	Honor cloaking.

*/

#ifndef _MCCIDEFS_H_		/* prevent multiple includes */
#define _MCCIDEFS_H_

#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

#if !__TMS_CLOAKED_NAMES_ONLY

# ifndef __WANT_SYMBOLIC_EQS
#  define __WANT_SYMBOLIC_EQS	0
# endif

# if __WANT_SYMBOLIC_EQS && !defined(EQ)
#  define EQ	==
# endif

# if __WANT_SYMBOLIC_EQS && !defined(NE)
#  define NE	!=
# endif

# if __WANT_SYMBOLIC_EQS && !defined(NOT)
#  define NOT	!
# endif /* symbolic eqs */

# ifndef __BEGIN_DECLS
#  define	__BEGIN_DECLS	__TMS_BEGIN_DECLS
# endif

# ifndef __END_DECLS
#  define	__END_DECLS	__TMS_END_DECLS
# endif

#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of mccidefs.h ****/
#endif /* _MCCIDEFS_H_ */
