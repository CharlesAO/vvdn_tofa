/* usbpump_assert.h	Sun Jun 28 2015 22:31:51 tmm */

/*

Module:  usbpump_assert.h

Function:
	USB DataPump assert macros.

Version:
	V3.15b	Sun Jun 28 2015 22:31:51 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2015

Revision history:
   3.15b  Sat May 30 2015 14:45:43  tmm
	19210: Module created.

   3.15b  Sun Jun 28 2015 22:31:51  tmm
	19275: fix typo in name for free builds when no local assert is used.

*/

#ifndef _USBPUMP_ASSERT_H_		/* prevent multiple includes */
#define _USBPUMP_ASSERT_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef __TMS_USBPUMP_ENABLE_ASSERTS
# define __TMS_USBPUMP_ENABLE_ASSERTS	__TMS_USBPUMP_DEBUG
#endif

/*

Macro:	__TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__

Function:
	Compile time override of MCCI debug trace macros.

Definition:
	CFLAGS_PORT += -D__TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__=my_file.h

Description:
	The DataPump allow the platform build system to inject a header file
	to override the default definitions of the DataPump assert macros. 
	This header file must define (as post condition) all of
	the __TMS_USBPUMP_ASSERT() macros: __TMS_USBPUMP_ASSERT() and
	__TMS_USBPUMP_ASSERTMSG().

	This header file will be included after #including "usbpumpenv.h"
	(and possibly after including other DataPump header files, depending 
	on the module).  The header file should take into account the
	configured value of __TMS_USBPUMP_DEBUG; if 0, the assert macros
	should produce no output, if non-zero, the assert macros should
	generate suitable code.

Notes:
	To avoid quoting issues in the build system the file name is
	given directly (without quotes). "usbpump_assert.h" supplies
	the leading and trailing quotes. However, this means that you
	can't use an angle-bracketed include file.

*/


#ifdef __TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__

# if ! (__TMS_LIBPORT_VERSION > __TMS_LIBPORT_VERSION_CALC(3,11,2,0))
#  error __TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__ needs a version of libport that defines __TMS_STRINGVAL.
# endif

/* __TMS_STRINGVAL expands the argument before stringifying */
# include __TMS_STRINGVAL(__TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__)

/* check post-conditions */
# ifndef __TMS_USBPUMP_ASSERT
#   error __TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__ did not define __TMS_USBPUMP_ASSERT()
# endif
# ifndef __TMS_USBPUMP_ASSERTMSG
#   error __TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__ did not define __TMS_USBPUMP_ASSERTMSG()
# endif

#else /* ! defined(__TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__) */

/*
|| The default assert framework
*/

__TMS_BEGIN_DECLS

__TMS_ONEWAY(
__TMS_VOID
UsbPumpLib_Assert(
	__TMS_UPLATFORM 			     *	/* pPlatform */,
	__TMS_CONST __TMS_TYPE_CODESPACE(__TMS_TEXT) * 	/* pAssertText */,
	__TMS_CONST __TMS_TYPE_CODESPACE(__TMS_TEXT) * 	/* pFile */,
	__TMS_ULONG 					/* line Number */,
	__TMS_CONST __TMS_TYPE_CODESPACE(__TMS_TEXT) * 	/* pAssertMessage */
	)
); 

__TMS_END_DECLS

/*

Name:	USBPUMP_ASSERT()

Index:	Name:	USBPUMP_ASSERTMSG()
	Name:	__TMS_USBPUMP_ASSERT()
	Name:	__TMS_USBPUMP_ASSERTMSG()
	Name:	__TMS_USBPUMP_ENABLE_ASSERTS

Function:
	In debug builds, check that a condition is true, and stop execution 
	if not.

Definition:
	#include "usbpump_assert.h"

	VOID USBPUMP_ASSERT(
		UPLATFORM *pPlatform,	// OPTIONAL
		BOOL fMustBeTrue
		);

	VOID USBPUMP_ASSERTMSG(
		UPLATFORM *pPlatform,			// OPTIONAL
		CONST TYPE_CODESPACE(TEXT) *pMessage,	// OPTIONAL
		BOOL fMustBeTrue
		);

Description:
	USBPUMP_ASSERT() and USBPUMP_ASSERTMSG() check the Boolean expression 
	fMustBeTrue. If not FALSE, execution proceeds. If FALSE, a (system-
	specific) assertion failure mechanism is invoked.  

	Depending on system capabilities, USBPUMP_ASSERTMSG() may display the
	message pMessage as part of the failure message.

	If pPlatform is not provided, it may not be possible to print an 
	error message, but the assertion will still be checked.

	The compile-time variable __TMS_USBPUMP_ENABLE_ASSERTS is used to 
	control whether the assert macros actually insert code. If not 
	defined, it is set equal to __TMS_USBPUMP_DEBUG. Thus, a checked build
	will use the asserts to perform run-time checking, and a free build
	will not.

Notes:
	The default implementation calls UsbPumpLib_Assert() if there's a 
	failure. 

	The compile-time variable __TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__ is 
	used to customize the assert implementation for the target platform.

See also:
	__TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__
	UsbPumpLib_Assert()

*/

# if __TMS_USBPUMP_ENABLE_ASSERTS
#  define __TMS_USBPUMP_ASSERT(pPlatform, fMustBeTrue)			    \
	do 	{							    \
		if (! (fMustBeTrue))                                        \
			{                                                   \
			UsbPumpLib_Assert(                                  \
				pPlatform,		    		    \
				__TMS_CONST_CODESPACE_STRING(#fMustBeTrue), \
				__TMS_CONST_CODESPACE_STRING(__FILE__),     \
				__LINE__,                                   \
				__TMS_NULL                                  \
				);                                          \
			}                                                   \
		} while (0)

#  define __TMS_USBPUMP_ASSERTMSG(pPlatform, sMessage, fMustBeTrue)	    \
	do 	{							    \
		if (! (fMustBeTrue))                                        \
			{                                                   \
			UsbPumpLib_Assert(                                  \
				pPlatform,				    \
				__TMS_CONST_CODESPACE_STRING(#fMustBeTrue), \
				__TMS_CONST_CODESPACE_STRING(__FILE__),     \
				__LINE__,                                   \
				__TMS_CONST_CODESPACE_STRING(sMessage)      \
				);                                          \
			}                                                   \
		} while (0)

# else /* ! __TMS_USBPUMP_ENABLE_ASSERTS */

#  define __TMS_USBPUMP_ASSERT(pPlatform, fMustBeTrue)			    \
		do {} while (0)

#  define __TMS_USBPUMP_ASSERTMSG(pPlatform, sMessage, fMustBeTrue)	    \
	do {} while (0)

# endif /* ! __TMS_USBPUMP_ENABLE_ASSERTS */

#endif /* ! defined(__TMS_USBPUMP_LOCAL_ASSERT_INCLUDE__) */

/****************************************************************************\
|
|	Uncloaked definitions....
|
\****************************************************************************/

#if ! __TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_ENABLE_ASSERTS	__TMS_USBPUMP_ENABLE_ASSERTS
# define USBPUMP_ASSERT(pPlatform, fMustBeTrue) \
   __TMS_USBPUMP_ASSERT(pPlatform, fMustBeTrue)
# define USBPUMP_ASSERTMSG(pPlatform, sMessage, fMustBeTrue) \
   __TMS_USBPUMP_ASSERTMSG(pPlatform, sMessage, fMustBeTrue)
#endif

#endif /* _USBPUMP_ASSERT_H_ */
