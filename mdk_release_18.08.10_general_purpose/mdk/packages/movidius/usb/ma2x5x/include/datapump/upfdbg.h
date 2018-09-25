/* upfdbg.h	Tue Sep 06 2011 16:32:01 chwon */

/*

Module:  upfdbg.h

Function:
	This header provides info about optional platform debug code. 
	The header is provided so that sources don't need quite so many
	cobbles to add temp debugging.

Version:
	V3.01f	Tue Sep 06 2011 16:32:01 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 1998, 2011 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	John Keys, MCCI Corporation	February 1998

Revision history:
   1.00a  Thu Feb 26 1998 14:06:26  johnk
	Module created.

   1.43d  9/4/1998  tmm
	Misc cleanups.

   3.01f  Tue Sep 06 2011 16:32:01  chwon
	13272: use USBPUMP_DEBUG instead of DEBUG.

*/

#ifndef _UPFDBG_H_		/* prevent multiple includes */
#define _UPFDBG_H_

#ifndef _USBPUMPPARAMS_H_
# include "usbpumpparams.h"
#endif


#define UPFSIG_OFF	0	/* signal pin to LOW */
#define UPFSIG_ON	1	/* signal pin to HIGH */
#define UPFSIG_TGL	2	/* toggle signal pin state */
#define	UPFSIG_PPULSE	3	/* pulse it high */
#define	UPFSIG_NPULSE	4	/* pulse it low */

__TMS_BEGIN_DECLS

#if __TMS_USBPUMP_DEBUG
VOID UPFDBG_Signal(int signal, int state);
VOID UPFDBG_TooLong(unsigned long howlong);

extern unsigned long gupf_uldbgticks;

#define UPFTIMER_TICK()		++gupf_uldbgticks

#define UPFPROFILE_START	\
	unsigned long upfprof_elapsed, upfprof_start = gupf_uldbgticks;
	
#define UPFPROFILE_STOP		\
	if ((upfprof_elapsed = gupf_uldbgticks - upfprof_start) > 1) \
		{ \
		UPFDBG_TooLong(upfprof_elapsed); \
		}
#else
#define UPFDBG_Signal(signal, state)	do {} while (0)
#define UPFDBG_TooLong(howlong)		do {} while (0)

#define	UPFTIMER_TICK()		do {} while (0)
#define	UPFPROFILE_START	
#define	UPFPROFILE_STOP
#endif

__TMS_END_DECLS

/**** end of upfdbg.h ****/
#endif /* _UPFDBG_H_ */
