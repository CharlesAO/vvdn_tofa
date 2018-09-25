/* usbpump_query_performance_counter.h	Sun Jun 28 2015 21:56:06 tmm */

/*

Module:  usbpump_query_performance_counter.h

Function:
	The wrapper for the performance counter logic.

Version:
	V3.15b	Sun Jun 28 2015 21:56:06 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	February 2015

Revision history:
   3.15a  Sat Feb  7 2015 16:23:46  tmm
	18922: Module created.

   3.15b  Wed Mar  4 2015 21:00:45  tmm
	19019: documentation improvements.

*/

#ifndef _USBPUMP_QUERY_PERFORMANCE_COUNTER_H_		/* prevent multiple includes */
#define _USBPUMP_QUERY_PERFORMANCE_COUNTER_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _UHILINTSYS_H_
# include "uhilintsys.h"
#endif

/* 
|| at the moment, we only support performance counters on some platforms, but
|| we want to provide support without conditional compile. Hence the usual
|| workarounds.
*/

/*

Macro:	__TMS_USBPUMP_LOCAL_PERFORMANCE_COUNTER_INCLUDE__

Function:
	Compile time overide for providing UsbPumpQueryPerformanceCounter()

Definition:
	CFLAGS_PORT += -D__TMS_USBPUMP_LOCAL_PERFORMANCE_COUNTER_INCLUDE__=myfile.h

Description:
	The DataPump allows the platform buildsystem to inject a header file
	to override the default definitions of
	UsbPumpQueryPerformanceCounter() and related facilities.

	The injected header file must define (as a post-condition) the 
	following CPP and C APIs:

	typedef <signedIntegerType> USBPUMP_PERFORMANCE_COUNTER;
	#define USBPUMP_PERFORMANCE_COUNTER_MAX	<someValue>
	#define	USBPUMP_PERFORMANCE_COUNTER_MIN	<someValue>

	USPUMP_PERFORMANCE_COUNTER UsbPumpQueryPerformanceCounter(
		VOID
		);

	USBPUMP_PERFORMANCE_COUNTER UsbPumpQueryPerformanceFrequency(
		VOID
		);

	USBPUMP_PERFORMANCE_COUNTER UsbPumpPerformanceCounterDelta(
		USBPUMP_PERFORMANCE_COUNTER t2,
		USBPUMP_PERFORMANCE_COUNTER t1
		);

	#define __TMS_USBPUMP_HAS_PERFORMANCE_COUNTER <boolean>

Notes:
	To avoid command-line quoting issues in the build system, the file 
	name is given directly  (without quotes). 
	"usbpump_query_performance_counter.h"
	supplies the leading and trailing quotes. However, this means that
	you can't refer to an angle-bracketed include file.

*/

#ifdef	__TMS_USBPUMP_LOCAL_PERFORMANCE_COUNTER_INCLUDE__
# if ! (__TMS_LIBPORT_VERSION > __TMS_LIBPORT_VERSION_CALC(3,11,2,0))
#  error __TMS_USBPUMP_LOCAL_PERFORMANCE_COUNTER_INCLUDE__ needs a version of libport that defines __TMS_STRINGVAL.
# endif

/* __TMS_STRINGVAL expands the argument before stringifying */
# include __TMS_STRINGVAL(__TMS_USBPUMP_LOCAL_PERFORMANCE_COUNTER_INCLUDE__)

# ifndef __TMS_USBPUMP_HAS_PERFORMANCE_COUNTER
#  error __TMS_USBPUMP_LOCAL_PERFORMANCE_COUNTER_INCLUDE__ must define __TMS_USBPUMP_HAS_PERFORMANCE_COUNTER
# endif
#else
# define __TMS_USBPUMP_HAS_PERFORMANCE_COUNTER	0
#endif

/*

Name:	UsbPumpQueryPerformanceCounter()

Index:	Name:	UsbPumpQueryPerformanceFrequency()
	Name:	UsbPumpPerformanceCounter_Delta()
	Name:	UsbPumpInterruptConnectionHandle_QueryEntryTime()
	Name:	USBPUMP_HAS_PERFORMANCE_COUNTER
	Type:	USBPUMP_PERFORMANCE_COUNTER
	Name:	USBPUMP_PERFORMANCE_COUNTER_MIN
	Name:	USBPUMP_PERFORMANCE_COUNTER_MAX
	Name:	USBPUMP_PERFORMANCE_COUNTER_COND1()
	Name:	USBPUMP_PERFORMANCE_COUNTER_VARIABLE()

Function:
	Access high performance platform CPU cycle counter.

Definition:
	#include "usbpump_query_performance_counter.h"

	typedef {some-signed-int} USBPUMP_PERFORMANCE_COUNTER;

	USBPUMP_PERFORMANCE_COUNTER UsbPumpQueryPerformanceCounter(VOID);

	USBPUMP_PERFORMANCE_COUNTER UsbPumpQueryPerformanceFrequency(VOID);

	USBPUMP_PERFORMANCE_COUNTER UsbPumpPerformanceCounter_Delta(
		USBPUMP_PERFORMANCE_COUNTER t2,
		USBPUMP_PERFORMANCE_COUNTER t1
		);

	#define	USBPUMP_PERFORMANCE_COUNTER_MAX {positiveMaxInt}
	#define	USBPUMP_PERFORMANCE_COUNTER_MIN {negativeMinInt}

	#define USBPUMP_PERFORMANCE_COUNTER_COND1(s) 
			==> s if performance counters are provided
			==> nothing if no performance counters are provided.

	USBPUMP_PERFORMANCE_COUNTER 
		UsbPumpInterruptConnectionHandle_QueryEntryTime(
			UHIL_INTERRUPT_CONNECTION_HANDLE hcInt
			);
	
	USBPUMP_PERFORMANCE_COUNTER_VARIABLE(someVariable);

Description:
	The performance counter interface of the DataPump assumes a
	free-running counter that increments at some fixed frequency, and
	ranges through all the values of some integral type. (Therefore,
	differences can be computed using normal C substraction, ignoring
	overflow, provided that samples are "close enough" to each other.)
	Some support is provided for platforms with fixed frequency but
	not full-range counters, but this is not the intended target.

	UsbPumpQueryPerformanceCounter() is defined to return the current
	value of the counter. UsbPumpQueryPerformanceFrequency() is defined
	to return the number of ticks per second. Clients are entitled
	to assume that this rate is fixed.

	UsbPumpPerformanceCounter_Delta(t2, t1) returns t2-t1, allowing
	for wrap-around. No indication is given of overflow; and the 
	result is signed (so will be negative if t2 is less than t1).

	USBPUMP_PERFORMANCE_COUNTER is a signed symbolic type for the 
	performance counter value. This is likely to be INT32 or INT64, 
	depending on the platform.  USBPUMP_PERFORMANCE_COUNTER_MIN and
	USBPUMP_PERFORMANCE_COUNTER_MAX give the minimum and maximum values,
	suitable for initialization (for things like tracking minimum and
	maximum times).

	If the platform doesn't support performance counters, dummy versions
	of the functions are provided, returning 0 for the value of the
	performance counter, and 1 for the value of the performance frequency.
	The type USBPUMP_PERFORMANCE_COUNTER is defined based on the platform
	'int' type, and maxima and minima are suitably defined. The delta
	function simply subtracts two ints.

	Code wrapped in USBPUMP_PERFORMANCE_COUNTER_COND1() is expanded
	if the platform supports performance counters, and suppressed
	otherwise -- so it's just syntactic sugar for #if.  You can also
	use #if USBPUMP_HAS_PERFORMANCE_COUNTER, if your coding style rules
	permit this.  (MCCI internal code must use
	USBPUMP_PERFORMANCE_COUNTER_COND1() if possible.)

	UsbPumpInterruptConnectionHandle_QueryEntryTime() allows an ISR
	to get the timestamp of when the low-level primary ISR was called.
	The platform is responsible for getting this as early as possible.
	If the time stamp isn't available, this returns 0.

	USBPUMP_PERFORMANCE_COUNTER_VARIABLE() marks its argument as
	used when performance counting is disabled. This allows avoidance
	of C warnings and lots of extra conditional compiles.

Returns:
	See description.

*/

/****************************************************************************\
|
|	Stub implementation
|
\****************************************************************************/

#if ! __TMS_USBPUMP_HAS_PERFORMANCE_COUNTER

__TMS_TYPE_DEF(USBPUMP_PERFORMANCE_COUNTER, INT);

# define __TMS_USBPUMP_PERFORMANCE_COUNTER_MIN __TMS_INT_MIN
# define __TMS_USBPUMP_PERFORMANCE_COUNTER_MAX __TMS_INT_MAX

# define UsbPumpQueryPerformanceCounter()	0
# define UsbPumpQueryPerformanceFreqeuncy()	1

# if __TMS_LIBPORT_HAS_INLINE
static __TMS_forceinline
__TMS_USBPUMP_PERFORMANCE_COUNTER
UsbPumpPerformanceCounter_Delta(
	__TMS_USBPUMP_PERFORMANCE_COUNTER t2,
	__TMS_USBPUMP_PERFORMANCE_COUNTER t1
	)
	{
	return t2 - t1;
	}

static __TMS_forceinline
__TMS_USBPUMP_PERFORMANCE_COUNTER
UsbPumpInterruptConnectionHandle_QueryEntryTime(
	__TMS_UHIL_INTERRUPT_CONNECTION_HANDLE hcInt
	)
	{
	__TMS_USBPUMP_UNREFERENCED_PARAMETER(hcInt);

	return 0;
	}
# else
#  define UsbPumpPerformanceCounter_Delta(t2, t1)		\
	((__TMS_USBPUMP_PERFORMANCE_COUNTER)			\
	 ((__TMS_USBPUMP_PERFORMANCE_COUNTER)(t2) -		\
	  (__TMS_USBPUMP_PERFORMANCE_COUNTER)(t1)))
#  define UsbPumpInterruptConnectionHandle_QueryEntryTime(hcInt)	0
# endif /* __TMS_LIBPORT_HAS_INLINE */

/* define the conditional compile to exclude code */
# define __TMS_USBPUMP_PERFORMANCE_COUNTER_COND1(x)	/* nothing */

# define __TMS_USBPUMP_PERFORMANCE_COUNTER_VARIABLE(x)		\
		__TMS_LIBPORT_UNREFERENCED_VARIABLE(x)

#else /* __TMS_USBPUMP_HAS_PERFORMANCE_COUNTER */

/* define the conditional compile to include code */
# define __TMS_USBPUMP_PERFORMANCE_COUNTER_COND1(x)	x

/* if it's unreferenced, it's really unreferenced */
# define __TMS_USBPUMP_PERFORMANCE_COUNTER_VARIABLE(x)	__TMS_SKIP()

#endif /* __TMS_USBPUMP_HAS_PERFORMANCE_COUNTER */

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HAS_PERFORMANCE_COUNTER	\
   __TMS_USBPUMP_HAS_PERFORMANCE_COUNTER
# define USBPUMP_PERFORMANCE_COUNTER_MIN	\
   __TMS_USBPUMP_PERFORMANCE_COUNTER_MIN
# define USBPUMP_PERFORMANCE_COUNTER_MAX	\
   __TMS_USBPUMP_PERFORMANCE_COUNTER_MAX
# define USBPUMP_PERFORMANCE_COUNTER_COND1(x)	\
   __TMS_USBPUMP_PERFORMANCE_COUNTER_COND1(x)
# define USBPUMP_PERFORMANCE_COUNTER_VARIABLE(x)	\
   __TMS_USBPUMP_PERFORMANCE_COUNTER_VARIABLE(x)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_query_performance_counter.h ****/
#endif /* _USBPUMP_QUERY_PERFORMANCE_COUNTER_H_ */
