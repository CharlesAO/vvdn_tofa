/* ccguess.h	Mon May 30 2005 21:23:08 tmm */

/*

Module:  ccguess.h

Function:
	Try to figure out which compiler we are using.

Version:
	V3.05g	Mon May 30 2005 21:23:08 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 1998, 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 1998

Revision history:
   2.10a  Mon Jun  8 1998 21:21:27  tmm
	Module created.

   3.05g  2005-05-30  tmm
	Improve guessing logic so we'll guess right for MSVC.

*/

#ifndef _DEF_CCGUESS_H_		/* prevent multiple includes */
#define _DEF_CCGUESS_H_

/* adapt ourselves to the compiler in use, if possible */
#ifdef	_MWC_
#  ifdef  __STDC__
#	define	_MWC_V4_
#  else
#	define	_MWC_V2_
#  endif
#  define __CompuDAS_H_Compiler_Known	1
#endif

#ifdef _MSC_
#  define __ANSI__
#  define __CompuDAS_H_Compiler_Known	1
#endif

#ifdef _SUNACC_
#  define __ANSI__
#  define __CompuDAS_H_Compiler_Known	1
#endif

#ifdef _GNUC_
#  ifdef __STDC__
#    if  __STDC__ == 1
#	define __ANSI__
#    endif
#  endif
#  define __CompuDAS_H_Compiler_Known	1
#endif

#ifdef _MRI
# ifndef _MRIC_
#  define _MRIC_
# endif
#endif
#ifdef _MRIC_
#  ifdef __STDC__
#    if __STDC__ == 1
#	define __ANSI__
#    endif
#  endif
#  define __CompuDAS_H_Compiler_Known	1
#endif

#ifdef _BORLANDC_
#  ifndef __STDC__
#     define __STDC__
#  elif __STDC__ == 1
#     define __ANSI__
#  endif
#  define __CompuDAS_H_Compiler_Known	1
#endif

/*
|| if we haven't been told what compiler we're using, make
|| a "cunning" guess.
*/
#ifndef __CompuDAS_H_Compiler_Known
/*
|| the likelihood of pre C89 CPP is vanishingly small at this point,
|| so we use the ANSI CPP features.
*/
/*
|| first, guess about ANSI.
*/
#  if defined(__STDC__) && __STDC__ == 1
#    ifndef __ANSI__
#	define __ANSI__
#    endif
#  endif

/*
|| next make the series of compiler guesses.
*/
#  ifdef __GNUC__
#	define _GNUC_		/* we're running GNUC */
#  elif defined(_MSC_VER)
#	define _MSC_		/* we're running MSC */
#  else

/*
|| other guesses would go here; at present, assume non-ANSI
|| which is why the "cunning" is in quotes.
*/
#  endif
#endif	/* __CompuDAS_H_Compiler_Known */

/**** end of ccguess.h ****/
#endif /* _DEF_CCGUESS_H_ */
