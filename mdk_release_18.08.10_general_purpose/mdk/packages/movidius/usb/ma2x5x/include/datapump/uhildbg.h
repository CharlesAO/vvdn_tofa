/* uhildbg.h	Mon Sep  1 2014 11:54:54 tmm */

/*

Module:  uhildbg.h

Function:
	Standard (platform-independent) debugging function.

Version:
	V3.13b	Mon Sep  1 2014 11:54:54 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 2001-2003, 2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	January 2001

Revision history:
   1.61a  Mon Jan  1 2001 23:22:10  tmm
	Module created.

   1.79a  6/5/2002  tmm
	Reduce include file bloat.

   1.87b  Tue Mar 16 2004 13:16:53  chwon
	Bug 1232: Correct sane name define; don't need type cloaking.

   3.13b  Mon Sep  1 2014 11:54:54  tmm
	18362: allow for static init of the debug print control. Redo
	name cloaking. Fix MCCI. Remove __TMS_P() stuff.

*/

#ifndef _UHILDBG_H_		/* prevent multiple includes */
#define _UHILDBG_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

/****************************************************************************\
|
|	Diagnostic print routines
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
	UPLATFORM_DEBUG_PRINT_BUF_FN,
	__TMS_VOID,
		(
		__TMS_UPLATFORM *,
		__TMS_CHAR __TMS_CONST * /* pBuf */,
		__TMS_BYTES		 /* nBuf */
		)
	);

__TMS_FNTYPE_DEF(
	UPLATFORM_DEBUG_FLUSH_FN,
	__TMS_VOID,
		(
		__TMS_UPLATFORM *
		)
	);

__TMS_FNTYPE_DEF(
	UPLATFORM_DEBUG_PRINT_POLL_FN,
	__TMS_VOID,
		(
		__TMS_UPLATFORM *
		)
	);

__TMS_FNTYPE_DEF(
	UPLATFORM_DEBUG_PRINT_ENABLE_FN,
	__TMS_BOOL,
		(
		__TMS_UPLATFORM *,
		__TMS_BOOL
		)
	);

__TMS_FNTYPE_DEF(
	UPLATFORM_DEBUG_PRINT_CLOSE_FN,
	__TMS_VOID,
		(
		__TMS_UPLATFORM *
		)
	);



/* 
|| the debug print control is a normal MCCI base-type that can have more
|| stuff tacked at the end.
*/
struct __TMS_STRUCTNAME(UPLATFORM_DEBUG_PRINT_CONTROL_HDR)
	{
	__TMS_UPLATFORM_DEBUG_PRINT_BUF_FN	*upfdbghh_pPrintBuf;
	__TMS_UPLATFORM_DEBUG_PRINT_BUF_FN	*upfdbghh_pPrintBufTransparent;
	__TMS_UPLATFORM_DEBUG_FLUSH_FN		*upfdbghh_pFlush;
	__TMS_UPLATFORM_DEBUG_PRINT_POLL_FN	*upfdbghh_pPrintPoll;
	__TMS_UPLATFORM_DEBUG_PRINT_ENABLE_FN	*upfdbghh_pDebugPrintEnable;
	__TMS_UPLATFORM_DEBUG_PRINT_CLOSE_FN	*upfdbghh_pClose;
	__TMS_VOID				*upfdbghh_pContext;
	};

#define	__TMS_UPLATFORM_DEBUG_PRINT_CONTROL_CONTENTS	\
	struct __TMS_STRUCTNAME(UPLATFORM_DEBUG_PRINT_CONTROL_HDR) upfdbghdr_upfdbg

union __TMS_UNIONNAME(UPLATFORM_DEBUG_PRINT_CONTROL_UNION)
	{
	__TMS_UPLATFORM_DEBUG_PRINT_CONTROL_CONTENTS;
	};

#define	__TMS_UPLATFORM_DEBUG_PRINT_CONTROL_HDR	\
	union __TMS_UNIONNAME(UPLATFORM_DEBUG_PRINT_CONTROL_UNION) upfdbg_hdr

/**** the generic structure ****/
struct __TMS_STRUCTNAME(UPLATFORM_DEBUG_PRINT_CONTROL)
	{
	__TMS_UPLATFORM_DEBUG_PRINT_CONTROL_HDR;
	};

/**** sane names ****/
# define upfdbg_pPrintBuf		upfdbg_hdr.upfdbghdr_upfdbg.upfdbghh_pPrintBuf
# define upfdbg_pPrintBufTransparent	upfdbg_hdr.upfdbghdr_upfdbg.upfdbghh_pPrintBufTransparent
# define upfdbg_pFlush			upfdbg_hdr.upfdbghdr_upfdbg.upfdbghh_pFlush
# define upfdbg_pPrintPoll		upfdbg_hdr.upfdbghdr_upfdbg.upfdbghh_pPrintPoll
# define upfdbg_pDebugPrintEnable	upfdbg_hdr.upfdbghdr_upfdbg.upfdbghh_pDebugPrintEnable
# define upfdbg_pClose			upfdbg_hdr.upfdbghdr_upfdbg.upfdbghh_pClose
# define upfdbg_pContext		upfdbg_hdr.upfdbghdr_upfdbg.upfdbghh_pContext

/**** the run-time initializer ****/
/* don't add parameters to this macro; instead create a V2 */
#define __TMS_UPLATFORM_DEBUG_PRINT_CONTROL_RUNTIME_INIT_V1(		\
		pDebugPrintControl,					\
		pCloseFn,						\
		pPrintBuf,						\
		pPrintBufTransparent,					\
		pFlush,							\
		pPrintPoll,						\
		pDebugPrintEnable,					\
		pContext						\
		)							\
	do	{							\
		(pDebugPrintControl)->upfdbg_pPrintBuf = (pPrintBuf);	\
		(pDebugPrintControl)->upfdbg_pPrintBufTransparent =	\
					(pPrintBufTransparent);		\
		(pDebugPrintControl)->upfdbg_pFlush = (pFlush);		\
		(pDebugPrintControl)->upfdbg_pPrintPoll = (pPrintPoll);	\
		(pDebugPrintControl)->upfdbg_pDebugPrintEnable =	\
					(pDebugPrintEnable);		\
		(pDebugPrintControl)->upfdbg_pClose = (pCloseFn);	\
		(pDebugPrintControl)->upfdbg_pContext = (pContext);	\
		} while (0)

/**** the compile-time initializer ****/
/* don't add parameters to this macro; instead create a V2 */
#define __TMS_UPLATFORM_DEBUG_PRINT_CONTROL_COMPILE_INIT_V1(		\
		pCloseFn,						\
		pPrintBuf,						\
		pPrintBufTransparent,					\
		pFlush,							\
		pPrintPoll,						\
		pDebugPrintEnable,					\
		pContext						\
		)							\
	{								\
	/* pPrintBuf */			(pPrintBuf),			\
	/* pPrintBufTransparent */	(pPrintBufTransparent),		\
	/* pFlush */			(pFlush),			\
	/* pPrintPoll */		(pPrintPoll),			\
	/* pDebugPrintEnable */		(pDebugPrintEnable),		\
	/* pCloseFn */			(pCloseFn),			\
	/* pContext */			(pContext)			\
	}

/****************************************************************************\
|
|	The API functions and service routines
|
\****************************************************************************/

__TMS_BEGIN_DECLS

/* these are the library routines -- largely obsolete */
__TMS_VOID	UHIL_PrintStr(__TMS_UPLATFORM *, __TMS_CONST __TMS_TEXT *);
__TMS_VOID	UHIL_PrintUnicodeStr(__TMS_UPLATFORM *, __TMS_CONST __TMS_TEXT *, __TMS_ARG_USHORT);
__TMS_VOID	UHIL_PrintChar(__TMS_UPLATFORM *, __TMS_ARG_CHAR);
__TMS_VOID	UHIL_PrintUshort(__TMS_UPLATFORM *, __TMS_ARG_USHORT);
__TMS_VOID	UHIL_PrintCrlf(__TMS_UPLATFORM *);

__TMS_VOID	UHIL_FlushChar(__TMS_UPLATFORM *);


/**** these are the underlying interfaces to the methods ****/
__TMS_VOID	UHIL_PrintCharTransparent(__TMS_UPLATFORM *, __TMS_ARG_CHAR);
__TMS_BOOL	UHIL_DebugPrintEnable(__TMS_UPLATFORM *, __TMS_ARG_BOOL);
__TMS_VOID	UHIL_PrintPoll(__TMS_UPLATFORM *);

__TMS_END_DECLS

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPLATFORM_DEBUG_PRINT_CONTROL_CONTENTS	\
   __TMS_UPLATFORM_DEBUG_PRINT_CONTROL_CONTENTS
# define UPLATFORM_DEBUG_PRINT_CONTROL_HDR	\
   __TMS_UPLATFORM_DEBUG_PRINT_CONTROL_HDR
# define UPLATFORM_DEBUG_PRINT_CONTROL_RUNTIME_INIT_V1(		\
		pDebugPrintControl,					\
		pCloseFn,						\
		pPrintBuf,						\
		pPrintBufTransparent,					\
		pFlush,							\
		pPrintPoll,						\
		pDebugPrintEnable,					\
		pContext						\
		)	\
	__TMS_UPLATFORM_DEBUG_PRINT_CONTROL_RUNTIME_INIT_V1(		\
		pDebugPrintControl,					\
		pCloseFn,						\
		pPrintBuf,						\
		pPrintBufTransparent,					\
		pFlush,							\
		pPrintPoll,						\
		pDebugPrintEnable,					\
		pContext						\
		)
# define UPLATFORM_DEBUG_PRINT_CONTROL_COMPILE_INIT_V1(		\
		pCloseFn,						\
		pPrintBuf,						\
		pPrintBufTransparent,					\
		pFlush,							\
		pPrintPoll,						\
		pDebugPrintEnable,					\
		pContext						\
		)	\
	__TMS_UPLATFORM_DEBUG_PRINT_CONTROL_COMPILE_INIT_V1(		\
		pCloseFn,						\
		pPrintBuf,						\
		pPrintBufTransparent,					\
		pFlush,							\
		pPrintPoll,						\
		pDebugPrintEnable,					\
		pContext						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/**** end of uhildbg.h ****/
#endif /* _UHILDBG_H_ */
