/* usblib_ioedit.h	Thu Aug 10 2006 00:15:10 chwon */

/*

Module:  usblib_ioedit.h

Function:
	Definition of Ioedit context

Version:
	V1.97e	Thu Aug 10 2006 00:15:10 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Mats Webjörn, MCCI Corporation	June 2006

Revision history:
   1.97d  Thu Jun 08 2006 16:07:54 maw
	Module created.

   1.97d  Wed Jun 14 2006 11:24:42  maw
   	Merged with usblib_ioeditcfg.h

   1.97e  Thu Aug 10 2006 00:15:10  chwon
	Fixed doc-xml error -- "Bad version string"

*/

#ifndef _USBLIB_IOEDIT_H_		/* prevent multiple includes */
#define _USBLIB_IOEDIT_H_

#ifndef _USBPUMP_H_
# include "usbpump.h"
#endif

/****************************************************************************\
|
|	Types and definitions
|
\****************************************************************************/

__TMS_TYPE_DEF(USBPUMP_IOEDIT_CHAR,	__TMS_TEXT);

__TMS_FNTYPE_DEF(
    	USBPUMP_IOEDITGETCH_FN,
    	__TMS_BOOL, (
        	__TMS_VOID *,			/* Client context */
         	__TMS_USBPUMP_IOEDIT_CHAR *	/* Input char */
       	)
    	);

__TMS_FNTYPE_DEF(
    	USBPUMP_IOEDITPUTCH_FN,
    	__TMS_BOOL, (
        	__TMS_VOID *,			/* Client context */
        	__TMS_USBPUMP_IOEDIT_CHAR	/* Output char */
        	)
    	);

#define __TMS_USBPUMP_IOEDITFLAGS_8BIT	(1 << 0)
#define __TMS_USBPUMP_IOEDITFLAGS_ACR	(1 << 1)/* Autowrap */
#define __TMS_USBPUMP_IOEDITFLAGS_UC	(1 << 2)/* Echo upper case */
#define __TMS_USBPUMP_IOEDITFLAGS_NBS	(1 << 3)/* No backspace */
#define __TMS_USBPUMP_IOEDITFLAGS_NCR	(1 << 4)/* No CR (before LF) */
#define __TMS_USBPUMP_IOEDITFLAGS_NLF	(1 << 5)/* No LF (after CR) */

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOEDITFLAGS_8BIT	__TMS_USBPUMP_IOEDITFLAGS_8BIT
# define USBPUMP_IOEDITFLAGS_ACR	__TMS_USBPUMP_IOEDITFLAGS_ACR
# define USBPUMP_IOEDITFLAGS_UC		__TMS_USBPUMP_IOEDITFLAGS_UC
# define USBPUMP_IOEDITFLAGS_NBS	__TMS_USBPUMP_IOEDITFLAGS_NBS
# define USBPUMP_IOEDITFLAGS_NCR	__TMS_USBPUMP_IOEDITFLAGS_NCR
# define USBPUMP_IOEDITFLAGS_NLF	__TMS_USBPUMP_IOEDITFLAGS_NLF
#endif

/****************************************************************************\
|
|   IOEDIT configuration
|
\****************************************************************************/

/*

Type:   USBPUMP_IOEDIT_CONFIG

Function:
	Carries configuration information for IoEdit

Contents:
	USBPUMP_ABSTRACT_POOL * pAbstractPool;
		Pointer to abstract pool context

	USBPUMP_IOEDITGETCH_FN pGetChFn;
        	Function to get a character from raw input stream

	USBPUMP_IOEDITPUTCH_FN pPutChFn;
        	Function to put a character to raw output stream

        VOID * pClientCtx;
        	Pointer to client context

	USBPUMP_IOEDIT_CHAR pBuffer;
        	Pointer to edit buffer

	BYTES sizeBuffer;
        	Sizeof edit buffer in USBPUMP_IOEDIT_CHAR's
        	including terminating zero

        UINT16 Flags;
        	Configuration flags

        CONST USBPUMP_IOEDIT_CHAR * pPrompt;
        	Zero terminated command line prompt

        UINT16 WrapCol;
        	Max line width before line wraps

	USBPUMP_IOEDIT_CHAR FillTrigger;
		Character code to trigger additional output
		of nFill null's

        UINT16 nFill;
        	Number of null to output on FillTrigger

Notes:
	This structure is normally initialized using
	USBPUMP_IOEDIT_CONFIG_INIT_Vx(), or USBPUMP_IOEDIT_CONFIG_SETUP_Vx.

See Also:
	USBPUMP_IOEDIT_CONFIG_SETUP_V1(),
	USBPUMP_IOEDIT_CONFIG_INIT_V1()

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOEDIT_CONFIG);
struct __TMS_STRUCTNAME(USBPUMP_IOEDIT_CONFIG)
	{
	__TMS_USBPUMP_ABSTRACT_POOL * 		pAbstractPool;
	__TMS_USBPUMP_IOEDITGETCH_FN *		pGetChFn;
	__TMS_USBPUMP_IOEDITPUTCH_FN *		pPutChFn;
	__TMS_VOID *				pClientCtx;
	__TMS_USBPUMP_IOEDIT_CHAR *		pBuffer;
	__TMS_BYTES				sizeBuffer;
	__TMS_UINT16				Flags;
	__TMS_CONST __TMS_USBPUMP_IOEDIT_CHAR *	pPrompt;
	__TMS_UINT16				WrapCol;
	__TMS_USBPUMP_IOEDIT_CHAR		FillTrigger;
	__TMS_UINT16				nFill;
	};

/*

Name:   USBPUMP_IOEDIT_CONFIG_SETUP_V1()

Function:
	Macro: runtime initialization code for USBPUMP_IOEDIT_CONFIG()

Definition:
	VOID USBPUMP_IOEDIT_CONFIG_SETUP_V1(
		USBPUMP_IOEDIT_CONFIG *		pConfig,
		USBPUMP_ABSTRACT_POOL * 	pAbstractPool,
		USBPUMP_IOEDITGETCH_FN *	pGetChFn,
		USBPUMP_IOEDITPUTCH_FN *	pPutChFn,
  		VOID *				pClientCtx,
 		USBPUMP_IOEDIT_CHAR		pBuffer,
		BYTES				sizeBuffer,
 		UINT16				Flags,
		CONST USBPUMP_IOEDIT_CHAR *	pPrompt,
		UINT16				WrapCol,
		USBPUMP_IOEDIT_CHAR		FillTrigger,
		UINT16				nFill
   		);

Description:
	This macro allows code to initialize a USBPUMP_IOEDIT_CONFIG
	structure in a forward-compatible way.  It initializes the structure,
	supplying defaults if the structure is changed in the future.

	pConfig is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	USBPUMP_IOEDIT_CONFIG_INIT_V1()

*/

/* don't add a parameter to this macro, create a _Vn+1... instead */
#define __TMS_USBPUMP_IOEDIT_CONFIG_SETUP_V1(			\
		pConfig,					\
		ARG_pAbstractPool,				\
		ARG_pGetChFn,					\
		ARG_pPutChFn,					\
		ARG_pClientCtx,					\
		ARG_pBuffer,					\
		ARG_sizeBuffer,					\
		ARG_Flags,					\
		ARG_pPrompt,					\
		ARG_WrapCol,					\
		ARG_FillTrigger,				\
		ARG_nFill					\
        	)						\
	do  {							\
       		(pConfig)->pAbstractPool = (ARG_pAbstractPool);	\
       		(pConfig)->pGetChFn      = (ARG_pGetChFn);	\
       		(pConfig)->pPutChFn      = (ARG_pPutChFn);	\
       		(pConfig)->pClientCtx    = (ARG_pClientCtx);	\
       		(pConfig)->pBuffer       = (ARG_pBuffer);	\
       		(pConfig)->sizeBuffer    = (ARG_sizeBuffer);	\
       		(pConfig)->Flags         = (ARG_Flags);		\
       		(pConfig)->pPrompt       = (ARG_pPrompt);	\
       		(pConfig)->WrapCol       = (ARG_WrapCol);	\
       		(pConfig)->FillTrigger   = (ARG_FillTrigger);	\
       		(pConfig)->nFill         = (ARG_nFill);		\
       	} while (0);

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOEDIT_CONFIG_SETUP_V1(	\
        	pConfig,			\
		ARG_pAbstractPool,		\
		ARG_pGetChFn,			\
		ARG_pPutChFn,			\
		ARG_pClientCtx,			\
		ARG_pBuffer,			\
		ARG_sizeBuffer,			\
		ARG_Flags,			\
		ARG_pPrompt,			\
		ARG_WrapCol,			\
		ARG_FillTrigger,		\
		ARG_nFill			\
        	)				\
    	__TMS_USBPUMP_IOEDIT_CONFIG_SETUP_V1(	\
        	pConfig,			\
		ARG_pAbstractPool,		\
		ARG_pGetChFn,			\
		ARG_pPutChFn,			\
		ARG_pClientCtx,			\
		ARG_pBuffer,			\
		ARG_sizeBuffer,			\
		ARG_Flags,			\
		ARG_pPrompt,			\
		ARG_WrapCol,			\
		ARG_FillTrigger,		\
		ARG_nFill			\
        	)
#endif

/*

Name:   USBPUMP_IOEDIT_CONFIG_INIT_V1()

Function:
	Macro: compile time initialization for USBPUMP_IOEDIT_CONFIG.

Definition:
	USBPUMP_IOEDIT_CONFIG_INIT_V1(
		USBPUMP_ABSTRACT_POOL * 	pAbstractPool,
		USBPUMP_IOEDITGETCH_FN *	pGetChFn,
		USBPUMP_IOEDITPUTCH_FN *	pPutChFn,
  		VOID *				pClientCtx,
 		USBPUMP_IOEDIT_CHAR		pBuffer,
		BYTES				sizeBuffer,
  		UINT16				Flags,
		CONST USBPUMP_IOEDIT_CHAR *	pPrompt,
		UINT16				WrapCol,
		USBPUMP_IOEDIT_CHAR		FillTrigger,
		UINT16				nFill
	       	);

Description:
    This macro is used when initializing a single USBPUMP_IOEDIT_CONFIG
    instance at compile time.  It expands to whatever is the current
    proper value for statically initializing such a structure, inserting
    proper defaults for any entries that may have been added after the
    _Vn+1() version of this macro was defined.

Expands To:
    '{', followed by the properly parenthesized initialization sequence,
    followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _Vn+1... instead */
#define __TMS_USBPUMP_IOEDIT_CONFIG_INIT_V1(	\
		ARG_pAbstractPool,		\
		ARG_pGetChFn,			\
		ARG_pPutChFn,			\
		ARG_pClientCtx,			\
		ARG_pBuffer,			\
		ARG_sizeBuffer,			\
		ARG_Flags,			\
		ARG_pPrompt,			\
		ARG_WrapCol,			\
		ARG_FillTrigger,		\
		ARG_nFill			\
        	)				\
		{				\
		(ARG_pAbstractPool),		\
		(ARG_pGetChFn),			\
		(ARG_pPutChFn),			\
		(ARG_pClientCtx),		\
		(ARG_pBuffer),			\
		(ARG_sizeBuffer),		\
		(ARG_Flags),			\
		(ARG_pPrompt),			\
		(ARG_WrapCol),			\
		(ARG_FillTrigger),		\
		(ARG_nFill)			\
    		}

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOEDIT_CONFIG_INIT_V1(		\
		ARG_pAbstractPool,		\
		ARG_pGetChFn,			\
		ARG_pPutChFn,			\
		ARG_pClientCtx,			\
		ARG_pBuffer,			\
		ARG_sizeBuffer,			\
		ARG_Flags,			\
		ARG_pPrompt,			\
		ARG_WrapCol,			\
		ARG_FillTrigger,		\
		ARG_nFill			\
        )					\
	__TMS_USBPUMP_IOEDIT_CONFIG_INIT_V1(	\
		ARG_pAbstractPool,		\
		ARG_pGetChFn,			\
		ARG_pPutChFn,			\
		ARG_pClientCtx,			\
		ARG_pBuffer,			\
		ARG_sizeBuffer,			\
		ARG_Flags,			\
		ARG_pPrompt,			\
		ARG_WrapCol,			\
		ARG_FillTrigger,		\
		ARG_nFill			\
        )
#endif

/****************************************************************************\
|
|	The functions.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_VOID * UsbPumpLib_IoeditInit(
	__TMS_CONST __TMS_USBPUMP_IOEDIT_CONFIG *
	);

__TMS_VOID UsbPumpLib_IoeditBegin(
	__TMS_VOID *
	);

__TMS_BOOL UsbPumpLib_Ioedit(
	__TMS_VOID *
	);

__TMS_END_DECLS

#endif /* _USBLIB_IOEDIT_H_ */

/**** end of usblib_ioedit.h ****/
