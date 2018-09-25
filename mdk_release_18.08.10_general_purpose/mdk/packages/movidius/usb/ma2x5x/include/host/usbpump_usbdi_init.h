/* usbpump_usbdi_init.h	Mon Mar 24 2008 12:02:38 djt */

/*

Module:  usbpump_usbdi_init.h

Function:
	Structures for initializing USBDI classes etc.

Version:
	V1.97k	Mon Mar 24 2008 12:02:38 djt	Edit level 5

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2005

Revision history:
   1.91d  Sat Jul  2 2005 17:30:50  tmm
	Module created.

   1.91h  Wed Aug 31 2005 06:59:16  tmm
	Dan Repich fixed botched macros USBPUMP_USBDI_INIT_MATCH_LIST_INIT_V1
	and USBPUMP_USBDI_INIT_MATCH_LIST_SETUP_V1.

   1.93c  Fri Dec 02 2005 13:48:08  chwon
	Fixed macro USBPUMP_USBDI_DRIVER_CLASS_CONFIG_INIT_V1 definition.

   1.97k  Mon Mar 24 2008 12:02:38  djt
	2590: changed USBPUMP_USBDI_DRIVER_CLASS_INIT_FN to match documentation.

*/

#ifndef _USBPUMP_USBDI_INIT_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_INIT_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

/****************************************************************************\
|
|	The common init function
|
\****************************************************************************/

__TMS_FNTYPE_DEF(
USBPUMP_USBDI_DRIVER_CLASS_INIT_FN,
__TMS_USBPUMP_USBDI_DRIVER_CLASS *,
	(
	/* pErrorCode OUT */__TMS_USTAT *,
	/* pParent */__TMS_USBPUMP_OBJECT_HEADER *,
	/* pConfig */__TMS_CONST __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG *,
	/* pPrivateConfig OPTIONAL */__TMS_CONST __TMS_VOID *
	));

/****************************************************************************\
|
|	The match-list initialization structure
|
\****************************************************************************/

/* 
|| in usbpump_usbdi_types.h: 
|| __TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_INIT_MATCH_LIST);
|| __TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY); 
*/

struct __TMS_STRUCTNAME(USBPUMP_USBDI_INIT_MATCH_LIST)
	{
	__TMS_UINT32				MagicBegin;
	__TMS_BYTES				nMatchStrings;
	__TMS_CONST __TMS_USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY *
						pvMatchStrings;
	__TMS_UINT32				MagicEnd;
	};

#define	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_BEGIN			\
	__TMS_UHIL_MEMTAG('<', 'I', 'm', 'l')

#define	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_END			\
	__TMS_UHIL_MEMTAG('>', 'I', 'm', 'l')

/*
|| The match list entry is one per pattern, and also gives priority.
*/
struct __TMS_STRUCTNAME(USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY)
	{
	__TMS_CONST __TMS_TEXT *		pPattern;
	__TMS_ARG_USBPUMP_USBDI_MATCH_PRIORITY	uPriority;
	};

#define	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY_INIT_V1(		\
	a_pPattern,							\
	a_uPriority							\
	)								\
	{								\
	(a_pPattern),							\
	(a_uPriority)							\
	}

#define	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY_SETUP_V1(		\
	a_pEntry,							\
	a_pPattern,							\
	a_uPriority							\
	)								\
	{								\
	(a_pEntry)->pPattern = (a_pPattern);				\
	(a_pEntry)->uPriority = (a_uPriority);				\
	}

/*
|| The following macro initializaes a USBPUMP_USBDI_INIT_MATCH_LIST;
|| If you add parameters, please create a new _INIT_V2() and modify
|| _INIT_V1() to call _INIT_V2() supplying appropriate values for
|| upward compatibility.
*/
#define __TMS_USBPUMP_USBDI_INIT_MATCH_LIST_INIT_V1(MatchStringArray)	\
	{								\
	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_BEGIN,		\
	__TMS_LENOF(MatchStringArray),					\
	(MatchStringArray),						\
	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_END			\
	}
 
#define __TMS_USBPUMP_USBDI_INIT_MATCH_LIST_SETUP_V1(			\
	a_pInitList,							\
	a_MatchStringArray,						\
	a_lengthMatchStringArray					\
	)								\
   do	{								\
	(a_pInitList)->MagicBegin =					\
		__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_BEGIN;	\
	(a_pInitList)->nMatchStrings = (a_lengthMatchStringArray);	\
	(a_pInitList)->pvMatchStrings = (a_MatchStringArray);		\
	(a_pInitList)->MagicEnd =					\
		__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_END;		\
	} while (0)

/****************************************************************************\
|
|	The Driver Class Configuration object
|
\****************************************************************************/

/*
|| in usbpump_usbdi_types.h:
|| __TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_DRIVER_CLASS_CONFIG);
*/

struct __TMS_STRUCTNAME(USBPUMP_USBDI_DRIVER_CLASS_CONFIG)
	{
	__TMS_UINT32			MagicBegin;
	__TMS_BYTES			NumInstances;
	__TMS_CONST __TMS_USBPUMP_USBDI_INIT_MATCH_LIST *
					pInitMatchList;
	__TMS_CONST __TMS_TEXT *	pClassName;
	__TMS_CONST __TMS_TEXT *	pFunctionName;
	__TMS_UINT32			MagicEnd;
	};

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_BEGIN		\
	__TMS_UHIL_MEMTAG('<', 'D', 'c', 'f')

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_END		\
	__TMS_UHIL_MEMTAG('>', 'D', 'c', 'f')

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_INIT_V1(		\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances							\
	)								\
	{								\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_BEGIN,		\
	(a_NumInstances),						\
	(a_pInitMatchList),						\
	(a_pClassName),							\
	(a_pFunctionName),						\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_END			\
	}

#define __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_SETUP_V1(		\
	a_pConfig,							\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances							\
	)								\
   do	{								\
	(a_pConfig)->MagicBegin =					\
		__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_BEGIN;	\
	(a_pConfig)->pInitMatchList = (a_pInitMatchList);		\
	(a_pConfig)->pClassName = (a_pClassName);			\
	(a_pConfig)->pFunctionName = (a_pFunctionName);			\
	(a_pConfig)->NumInstances = (a_NumInstances);			\
	(a_pConfig)->MagicEnd =						\
		__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_END;	\
	} while (0)

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_BEGIN	\
   __TMS_USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_BEGIN
# define USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_END	\
   __TMS_USBPUMP_USBDI_INIT_MATCH_LIST_MAGIC_END
# define USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY_INIT_V1(		\
	a_pPattern,							\
	a_uPriority							\
	)	\
	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY_INIT_V1(		\
	a_pPattern,							\
	a_uPriority							\
	)
# define USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY_SETUP_V1(		\
	a_pEntry,							\
	a_pPattern,							\
	a_uPriority							\
	)	\
	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_ENTRY_SETUP_V1(		\
	a_pEntry,							\
	a_pPattern,							\
	a_uPriority							\
	)
# define USBPUMP_USBDI_INIT_MATCH_LIST_INIT_V1(MatchStringArray)	\
   __TMS_USBPUMP_USBDI_INIT_MATCH_LIST_INIT_V1(MatchStringArray)
# define USBPUMP_USBDI_INIT_MATCH_LIST_SETUP_V1(			\
	a_pInitList,							\
	a_MatchStringArray,						\
	a_lengthMatchStringArray					\
	)	\
	__TMS_USBPUMP_USBDI_INIT_MATCH_LIST_SETUP_V1(			\
	a_pInitList,							\
	a_MatchStringArray,						\
	a_lengthMatchStringArray					\
	)
# define USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_BEGIN	\
   __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_BEGIN
# define USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_END	\
   __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_MAGIC_END
# define USBPUMP_USBDI_DRIVER_CLASS_CONFIG_INIT_V1(		\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances							\
	)	\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_INIT_V1(		\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances							\
	)
# define USBPUMP_USBDI_DRIVER_CLASS_CONFIG_SETUP_V1(		\
	a_pConfig,							\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances							\
	)	\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_SETUP_V1(		\
	a_pConfig,							\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_init.h ****/
#endif /* _USBPUMP_USBDI_INIT_H_ */
