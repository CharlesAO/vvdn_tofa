/* usbpumpparams.h	Sun Nov 29 2015 02:00:59 tmm */

/*

Module:  usbpumpparams.h

Function:
	Compile-time tunable parameters for the USB DataPump

Version:
	V3.15d	Sun Nov 29 2015 02:00:59 tmm	Edit level 9

Copyright notice:
	This file copyright (C) 2002, 2011, 2013, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.64a  Mon May 27 2002 18:56:12  tmm
	Module created.

   3.01f  Tue Sep 06 2011 16:48:10  chwon
	13272: remove use of DEBUG. Define USBPUMP_DEBUG based on DEBUG
	defintion if USBPUMP_DEBUG symbol was not defined.

   3.11b  Sat Jan 12 2013 05:28:25  tmm
	16458: make sure we have a component ID (USBPUMP_COMPONENT_ID) for
	use by tracing and other facilities.

   3.11b  Sat Jan 12 2013 09:38:02  tmm
	16459: add __TMS_USBPUMP_TRACE and USBPUMP_TRACE to control
	compilation of tracing features.

   3.11b  Wed Jan 16 2013 06:23:06  tmm
	16463: add __TMS_DATAPUMP_COMPAT_V3_10 to cover some of the global
	changes that we're making.

   3.11b  Wed Jan 30 2013 16:21:51  chwon
	16459: fixed typo; __TMS_USBUMP_TRACE should be __TMS_USBPUMP_TRACE.

   3.15b  Sat Mar 14 2015 18:32:04  tmm
	19050: Add __TMS_USBPUMP_COMPAT_V3_14 so that we can fix some things
	over a period of time.

	19019: Documention improvements. As a result, removed
	INLINE_UsbMarkCompletionBusy, INLINE_UsbMarkCompletionNotBusy because
	they're not used.

   3.15b  Sun May 31 2015 23:34:54  tmm
	19211: correct usages of "DataPump".

   3.15d  Sun Nov 29 2015 02:00:59  tmm
	19680: add __TMS_USBPUMP_LOCAL_PARAM_INCLUDE__

*/

#ifndef _USBPUMPPARAMS_H_		/* prevent multiple includes */
#define _USBPUMPPARAMS_H_

/* we need the basic libport stuff */
#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

/*

Macro:	__TMS_USBPUMP_LOCAL_PARAMS_INCLUDE__

Function:
	Compile time override of MCCI debug trace macros.

Definition:
	CFLAGS_PORT += -D__TMS_USBPUMP_PARAMS_TRACE_INCLUDE__=my_file.h

Description:
	The DataPump allow the platform build system to inject a header file
	to override the default definitions of the DataPump parameters.

	This header file will be included after #including "usbpumpenv.h"
	(and possibly after including other DataPump header files, depending 
	on the module).  

Notes:
	To avoid quoting issues in the build system the file name is
	given directly (without quotes). "usbpumpdebug.h" supplies
	the leading and trailing quotes. However, this means that you
	can't use an angle-bracketed include file.

*/

#ifdef __TMS_USBPUMP_LOCAL_PARAMS_INCLUDE__

# if ! (__TMS_LIBPORT_VERSION > __TMS_LIBPORT_VERSION_CALC(3,11,2,0))
#  error __TMS_USBPUMP_LOCAL_PARAMS_INCLUDE__ needs a version of libport that defines __TMS_STRINGVAL.
# endif

/* __TMS_STRINGVAL expands the argument before stringifying */
# include __TMS_STRINGVAL(__TMS_USBPUMP_LOCAL_PARAMS_INCLUDE__)
#endif

/****************************************************************************\
|
|	Parameters for tuning declarations
|
\****************************************************************************/

/*

Macro:	WANT_HW_DERIVED_UCONFIG

Function:
	Boolean compile parameter -- enables generation of platform-specific
	UCONFIGs

Description:
	If WANT_HW_DERIVED_UCONFIG is TRUE at compile time, then uconfig.h
	will define the UCONFIG structure in such a way as to allow it to
	be extended with platform-specific definitions.

	By default, this is FALSE.  

Notes:
	This feature is deprecated. MCCI does not have test cases that cover 
	defining WANT_HW_DERIVED_UCONFIG non-zero.

*/

/**** some parameters ****/
#ifndef	WANT_HW_DERIVED_UCONFIG
# define	WANT_HW_DERIVED_UCONFIG	0
#endif

/*

Macro:	WANT_HW_DERIVED_UIFCSET

Function:
	Boolean compile parameter -- enables generation of platform-specific
	UINTERFACECSETs

Description:
	If WANT_HW_DERIVED_UIFCSET is TRUE at compile time, then 
	uinterfaceset.h will define the UINTERFACESET structure in such a way 
	as to allow it to be extended with platform-specific definitions.

	By default, this is FALSE.  

Notes:
	This feature is deprecated. MCCI does not have test cases that cover 
	defining WANT_HW_DERIVED_UIFCSET non-zero.

*/

#ifndef WANT_HW_DERIVED_UIFCSET
# define	WANT_HW_DERIVED_UIFCSET	0
#endif

/*

Macro:	WANT_HW_DERIVED_UINTERFACE

Function:
	Boolean compile parameter -- enables generation of platform-specific
	UINTERFACEs

Description:
	If WANT_HW_DERIVED_UINTERFACE is TRUE at compile time, then 
	uinterface.h will define the UINTERFACE structure in such a way 
	as to allow it to be extended with platform-specific definitions.

	By default, this is FALSE.  

Notes:
	This feature is deprecated. MCCI does not have test cases that cover 
	defining WANT_HW_DERIVED_UINTERFACE non-zero.

*/

#ifndef WANT_HW_DERIVED_UINTERFACE
# define	WANT_HW_DERIVED_UINTERFACE 0
#endif

/* unless someone has said otherwise, we only want cloaked names */
#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

/*

Macro:	MCCI_DEBUG

Function:
	Compile parameter -- if defined, force a debug build.

Description:
	If MCCI_DEBUG is defined at compile time, then __TMS_USBPUMP_DEBUG
	is forced to 1, causing a checked (debug) build.

	If it's not defined, a variety of other things are taken into
	consideration. See USBPUMP_DEBUG.

	By default, this is undefined.  

Notes:
	This feature is deprecated. MCCI does not have test cases that cover 
	defining WANT_HW_DERIVED_UINTERFACE non-zero.

See also:
	USBPUMP_DEBUG

*/

/* MCCI_DEBUG, if defined, forces USBPUMP_DEBUG == 1 */
#ifdef MCCI_DEBUG
# undef		__TMS_USBPUMP_DEBUG
# define	__TMS_USBPUMP_DEBUG 1
#endif

/*

Macro:	__TMS_USBPUMP_DEBUG

Index:	Macro:	USBPUMP_DEBUG
	Macro:	DEBUG

Function:
	Boolean compile parameter, selecting normal (free) or debug (checked)
	build.

Description:
	If __TMS_USBPUMP_DEBUG is defined at compile time, and is TRUE, then
	this is a debug build.  If it's FALSE, then this is a normal build.

	If it's not defined at all, then the macro DEBUG is consulted.
	If DEBUG is defined and empty, or defined and non-zero, then 
	__TMS_USBPUMP_DEBUG will be set TRUE. Otherwise (if undefined or
	defined as 0), __TMS_USBPUMP_DEBUG will be set FALSE.

	USBPUMP_DEBUG tracks __TMS_USBPUMP_DEBUG, and is defined provided
	uncloaked names are defined.

Notes:
	A number of complicated macros __TMS_USBPUMP_DEBUG_... are defined
	in order to let us find out whether DEBUG is empty, zero or non-zero.

See also:
	MCCI_DEBUG

*/

/*
|| Remove use of DEBUG. Switch to __TMS_USBPUMP_DEBUG as the cloaked control
|| symbol, and uncloak it as USBPUMP_DEBUG.
|| If USBPUMP_DEBUG is not defined, need to define USBPUMP_DEBUG symbol based
|| on DEBUG symbol definition.
*/
#ifndef __TMS_USBPUMP_DEBUG
# ifdef DEBUG

/* prepare to find out whether DEBUG is empty, 0 or non-zero */
#  define	__TMS_USBPUMP_DEBUG_TYPE_0	0
#  define	__TMS_USBPUMP_DEBUG_TYPE_1	1
#  define	__TMS_USBPUMP_DEBUG_TYPE_	2
#  define	__TMS_USBPUMP_DEBUG_TYPE_BLANK	__TMS_USBPUMP_DEBUG_TYPE_
#  define	__TMS_USBPUMP_DEBUG_TYPE_SUB(x)	__TMS_USBPUMP_DEBUG_TYPE_##x
#  define	__TMS_USBPUMP_DEBUG_TYPE(x)	__TMS_USBPUMP_DEBUG_TYPE_SUB(x)

/* use the definitions to set __TMS_USBPUMP_DEBUG */
#  if __TMS_USBPUMP_DEBUG_TYPE(DEBUG) == __TMS_USBPUMP_DEBUG_TYPE_BLANK
#   define	__TMS_USBPUMP_DEBUG	1	/* blank means debug on */
#  elif __TMS_USBPUMP_DEBUG_TYPE(DEBUG) == __TMS_USBPUMP_DEBUG_TYPE_0
#   define	__TMS_USBPUMP_DEBUG	0	/* defined zero means debug off */
#  else
#   define	__TMS_USBPUMP_DEBUG	1	/* non-zero value means debug on */
#  endif
# else
#  define	__TMS_USBPUMP_DEBUG	0	/* undefined means debug off */
# endif	/* DEBUG */
#endif

#if !__TMS_CLOAKED_NAMES_ONLY
# define	USBPUMP_DEBUG		__TMS_USBPUMP_DEBUG
#endif

/*
|| Starting with V1.7, we've reorganized the names to make things more
|| sensible.  Defining DATAPUMP_COMPAT_V1_6 will declare macros that
|| allow older code to be recompiled to work with the V1.7 DataPump.
*/

#ifndef __TMS_DATAPUMP_COMPAT_V1_6
# define	__TMS_DATAPUMP_COMPAT_V1_6	0
#endif /* __TMS_DATAPUMP_COMPAT_V1_6 */

/*

Macro:	DATAPUMP_COMPAT_V3_10

Index:	Macro:	__TMS_DATAPUMP_COMPAT_V3_10

Function:
	Boolean compile parameter -- enables inclusion of DataPump V3.10 
	backward compatibility features.

Description:
	Starting after V3.10, we made substantial changes to clean up
	and rationalize code. This parameter, if true, allows code 
	targeting the older DataPump APIs to be compiled.

*/

#ifndef	__TMS_DATAPUMP_COMPAT_V3_10
# define	__TMS_DATAPUMP_COMPAT_V3_10	0
#endif /* __TMS_DATAPUMP_COMPAT_V3_10 */

#if !__TMS_CLOAKED_NAMES_ONLY
# define	DATAPUMP_COMPAT_V3_10	__TMS_DATAPUMP_COMPAT_V3_10
#endif

/*

Macro:	DATAPUMP_COMPAT_V3_14

Index:	Macro:	__TMS_DATAPUMP_COMPAT_V3_14

Function:
	Boolean compile parameter -- enables inclusion of DataPump V3.14
	backward compatibility features.

Description:
	Starting during V3.15b, we cleaned up some things that were misnamed
	and rationalize code. This parameter, if true, allows code 
	targeting the older V3.14 DataPump APIs to be compiled.

*/

#ifndef	__TMS_DATAPUMP_COMPAT_V3_14
# define	__TMS_DATAPUMP_COMPAT_V3_14	0
#endif /* __TMS_DATAPUMP_COMPAT_V3_14 */

#if !__TMS_CLOAKED_NAMES_ONLY
# define	DATAPUMP_COMPAT_V3_14	__TMS_DATAPUMP_COMPAT_V3_14
#endif

/*

Macro:	USBPUMP_COMPONENT_ID

Index:	Macro:	__TMS_USBPUMP_COMPONENT_ID

Function:
	Identifies the component containing a given compile unit.

Description:
	The DataPump build system defines USBPUMP_COMPONENT_ID based on the
	name of the library or program that contains the component. This
	can be used by custom trace systems to provide component-specific
	trace control, to tag trace messages with the originating component,
	and so forth.  

	The build system sets this macro to expand to an identifier-like name
	(although it doesn't prevent it from beginning with a digit). Trace
	components that need a string should use the C preprocessor #
	operator to obtain it. Bear in mind that you must doubly expand to
	get a macro value converted to a string, so if you need a string, 
	you'll need something like the following:

		#define STRING(x) #x
		#define STRINGVAL(x) STRING(x)

		static const char skName[] = STRINGVAL(USBPUMP_COMPONENT_ID);

	If the compile environment doens't define __TMS_USBPUMP_COMPONENT_ID,
	then the following default will be enforced by usbpumpparams.h:

		#define __TMS_USBPUMP_COMPONENT_ID none

*/

/*
|| Make sure we have a component ID, and define the uncloaked version
*/

#ifndef __TMS_USBPUMP_COMPONENT_ID
# define __TMS_USBPUMP_COMPONENT_ID	none
#endif

#if !__TMS_CLOAKED_NAMES_ONLY
# define	USBPUMP_COMPONENT_ID	__TMS_USBPUMP_COMPONENT_ID
#endif

/*

Macro:	__TMS_USBPUMP_TRACE

Index:	Macro:	USBPUMP_TRACE

Function:
	Configuration parameter to control inclusion of trace commands.

Description:
	These object-like macros represent simple truth values.
	__TMS_USBPUMP_TRACE will be non-zero if trace capabilities are
	to be compiled in, zero otherwise.  USBPUMP_TRACE, if defined, is
	always the same value as __TMS_USBPUMP_TRACE.

	USBPUMP_TRACE is always true if USBPUMP_DEBUG is true. However, it's
	possible (in customer-specific builds) to set USBPUMP_TRACE true in
	free builds.  This is not a standard MCCI build configuration, and
	must be done via a line in port/{name}/mk/buildset.var, for example:

		CPPFLAGS_PORT += -D__TMS_USBPUMP_TRACE=1

See also:
	__TMS_USBPUMP_DEBUG

*/

#ifndef __TMS_USBPUMP_TRACE
# define __TMS_USBPUMP_TRACE	__TMS_USBPUMP_DEBUG
#else /* def __TMS_USBPUMP_TRACE */
# if !__TMS_USBPUMP_TRACE && __TMS_USBPUMP_DEBUG
#  error "Unsupported configuration: if debug is on, tracing must be on"
# endif
#endif

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_TRACE	__TMS_USBPUMP_TRACE
#endif

/**** end of usbpumpparams.h ****/
#endif /* _USBPUMPPARAMS_H_ */
