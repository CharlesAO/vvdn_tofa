/* usbpump_taskinit.h	Thu Feb 16 2006 16:16:38 maw */

/*

Module:  usbpump_taskinit.h

Function:
	General-purpose task initialization discipline for use to start USB DataPump tasks.

Version:
	V1.92.5	Thu Feb 16 2006 16:16:38 maw	Edit level 4

Copyright notice:
	This file copyright (C) 2005-2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjorn, MCCI Corporation	September 2005

Revision history:
   1.91h  Sun Sep 11 2005 11:30:23 maw
	Module created.

   1.91i  Tue Sep 20 2005 12:31:30  chwon
	Added extern keyword in gk_UsbPumpTask_InitVectorHdr.

   1.91i  Mon Sep 26 2005 13:36:43  chwon
	Removed gk_UsbPumpTask_InitVectorHdr declaration

   1.92.5  Thu Feb 16 2006 16:16:38 maw
   	Added USBPUMP_TASK_INIT_NODE_SETUP_V1 macro
   	
*/

#ifndef _USBPUMP_TASKINIT_H_		/* prevent multiple includes */
#define _USBPUMP_TASKINIT_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif

/****************************************************************************\
|
|	The data structures which describe the tasks.
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_TASK_INIT_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_TASK_INIT_VECTOR_HDR);

/*
|| The optional setup function is called so that the boot task can do some
|| "pre-setup", including prompting for other things to do.
|| The result is passed (unchanged) to all the probe and setup functions.
*/
__TMS_FNTYPE_DEF(
USBPUMP_TASK_INIT_VECTOR_SETUP_FN,
__TMS_VOID *,
	(
	__TMS_CONST __TMS_USBPUMP_TASK_INIT_VECTOR_HDR *
	));

/*
|| The optional finish function is called so that the boot task can do some
|| "post-setup", including prompting for other things to do.
*/
__TMS_FNTYPE_DEF(
USBPUMP_TASK_INIT_VECTOR_FINISH_FN,
__TMS_VOID,
	(
	__TMS_CONST __TMS_USBPUMP_TASK_INIT_VECTOR_HDR *,
	__TMS_VOID * /* pInitVectorSetupContext */
	));

/*
|| The optional task probe function is called prior to starting
|| the task, so that it can be decided whether or not to start it.
*/
__TMS_FNTYPE_DEF(
USBPUMP_TASK_INIT_NODE_PROBE_FN,
__TMS_BOOL, (
	__TMS_CONST __TMS_USBPUMP_TASK_INIT_NODE *,
	__TMS_VOID * /* pInitVectorSetupContext */
	));

/*
|| The mandatory task start function creates (if not already created) and 
|| starts the task and provides any requred process and protocol 
|| configuration parameters
*/
__TMS_FNTYPE_DEF(
USBPUMP_TASK_INIT_NODE_START_FN,
__TMS_BOOL, (
	__TMS_BOOL,
	__TMS_CONST __TMS_USBPUMP_TASK_INIT_NODE *,
	__TMS_VOID * /* pInitVectorSetupContext */
	));

struct __TMS_STRUCTNAME(USBPUMP_TASK_INIT_NODE)
	{
	/*
	|| The mandatory task name
	*/
	__TMS_CONST __TMS_TEXT *			tin_pName;
	
	/*
	|| The optional task probe function.
	*/
	__TMS_USBPUMP_TASK_INIT_NODE_PROBE_FN *		tin_pProbeFn;

	/*
	|| The mandatory task create and start function
	*/
	__TMS_USBPUMP_TASK_INIT_NODE_START_FN *		tin_pStartFn;

	/*
	|| The recommended debug flags
	*/
	__TMS_UINT32					tin_DebugFlags;

	/*
	|| The optional memory pool to be used by the task if the 
	|| OS implementation doesn't provide suitable memory allocation
	|| methods
	*/
	__TMS_UINT8 *					tin_pMemPool;
	__TMS_BYTES					tin_sizeMemPool;

	/*
	|| A slot for extra information. This is where OS specific
	|| task info may be provided
	*/
	__TMS_VOID *					tin_pOptionalOsInfo;
	};

/*

Name:	USBPUMP_TASK_INIT_NODE_SETUP_V1()

Function:
	Macro: runtime initialization code for USBPUMP_TASK_INIT_NODE

Definition:
	USBPUMP_TASK_INIT_NODE_SETUP_V1(
		USBPUMP_TASK_INIT_NODE *		pNode,
		CONST __TMS_TEXT *			pName,
		USBPUMP_TASK_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_TASK_INIT_NODE_START_FN *	pStartFn,
		UINT32					DebugFlags,
		UINT8 *					pMemPool,
		BYTES					sizeMemPool,
		VOID *					pOptionalOsInfo
		);

Description:
	This macro allows code to initialize a USBPUMP_TASK_INIT_NODE 
	structure in a forward-compatible way.  It initializes the structure, 
	supplying defaults if the structure is changed in the future.

	pNode is a pointer to an uninitialized structure; it does not
	need to be zeroed.

Returns:
	No explicit result.

See also:
	USBPUMP_TASK_INIT_NODE_INIT_V1()

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define __TMS_USBPUMP_TASK_INIT_NODE_CONFIG_V1(				\
		ARG_pNode,						\
		ARG_pName,						\
		ARG_pProbeFn,						\
		ARG_pStartFn, 						\
		ARG_DebugFlags,						\
		ARG_pMemPool,						\
		ARG_sizeMemPool,					\
		ARG_pOptionalOsInfo 					\
		) 							\
	do	{							\
		(pNode)->pName           = (ARG_pName);			\
		(pNode)->pProbeFn        = (ARG_pProbeFn);		\
		(pNode)->pStartFn        = (ARG_pStartFn);		\
		(pNode)->DebugFlags      = (ARG_DebugFlags);		\
		(pNode)->pMemPool        = (ARG_pMemPool);		\
		(pNode)->sizeMemPool     = (ARG_sizeMemPool);		\
		(pNode)->pOptionalOsInfo = (ARG_pOptionalOsInfo);	\
		} while (0)

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_TASK_INIT_NODE_CONFIG_V1(	\
		ARG_pNode,			\
		ARG_pName,			\
		ARG_pProbeFn,			\
		ARG_pStartFn, 			\
		ARG_DebugFlags,			\
		ARG_pMemPool,			\
		ARG_sizeMemPool,		\
		ARG_pOptionalOsInfo 		\
		) 				\
	__TMS_USBPUMP_TASK_INIT_NODE_CONFIG_V1(	\
		ARG_pNode,			\
		ARG_pName,			\
		ARG_pProbeFn,			\
		ARG_pStartFn, 			\
		ARG_DebugFlags,			\
		ARG_pMemPool,			\
		ARG_sizeMemPool,		\
		ARG_pOptionalOsInfo 		\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/*

Name:	USBPUMP_TASK_INIT_NODE_INIT_V1()

Function:
	Macro: compile time initialization for USBPUMP_TASK_INIT_NODE

Definition:
	USBPUMP_TASK_INIT_NODE_INIT_V1(
		CONST __TMS_TEXT *			pName,
		USBPUMP_TASK_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_TASK_INIT_NODE_START_FN *	pStartFn,
		UINT32					DebugFlags,
		UINT8 *					pMemPool,
		BYTES					sizeMemPool,
		VOID *					pOptionalOsInfo
		);

Description:
	This macro is used when initializing a single USBPUMP_TASK_INIT_NODE
	instance at compile time.  It expands to whatever is the current 
	proper value for statically initializing such a structure, inserting 
	proper defaults for any entries that may have been added after the _V1() 
	version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ';'.

*/

/* don't add a parameter to this macro, create a _V2... instead */
#define __TMS_USBPUMP_TASK_INIT_NODE_INIT_V1(	\
		ARG_pName,			\
		ARG_pProbeFn,			\
		ARG_pStartFn, 			\
		ARG_DebugFlags,			\
		ARG_pMemPool,			\
		ARG_sizeMemPool,		\
		ARG_pOptionalOsInfo 		\
		) 				\
	{					\
		(ARG_pName),			\
		(ARG_pProbeFn),			\
		(ARG_pStartFn),			\
		(ARG_DebugFlags),		\
		(ARG_pMemPool),			\
		(ARG_sizeMemPool),		\
		(ARG_pOptionalOsInfo) 		\
	}

#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_TASK_INIT_NODE_INIT_V1(	\
		ARG_pName,			\
		ARG_pProbeFn,			\
		ARG_pStartFn, 			\
		ARG_DebugFlags,			\
		ARG_pMemPool,			\
		ARG_sizeMemPool,		\
		ARG_pOptionalOsInfo 		\
		) 				\
	__TMS_USBPUMP_TASK_INIT_NODE_INIT_V1(	\
		ARG_pName,			\
		ARG_pProbeFn,			\
		ARG_pStartFn, 			\
		ARG_DebugFlags,			\
		ARG_pMemPool,			\
		ARG_sizeMemPool,		\
		ARG_pOptionalOsInfo 		\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/* the vector header */
struct __TMS_STRUCTNAME(USBPUMP_TASK_INIT_VECTOR_HDR)
	{
	/* an optional function that is called before we start */
	__TMS_USBPUMP_TASK_INIT_VECTOR_SETUP_FN	*	tivh_pSetupFn;

	/* an optional function that is called when we are done */
	__TMS_USBPUMP_TASK_INIT_VECTOR_FINISH_FN *	tivh_pFinishFn;

	/* the vector and its length */
	__TMS_CONST __TMS_USBPUMP_TASK_INIT_NODE *	tivh_pVector;
	__TMS_SIZE_T					tivh_nVector;
	};

#define __TMS_USBPUMP_TASK_INIT_VECTOR_HDR_INIT_V1(		\
		VectorName,					\
		pSetupFn,					\
		pFinishFn					\
		)						\
	{							\
		(pSetupFn),					\
		(pFinishFn),					\
		(VectorName),					\
		sizeof(VectorName) / sizeof(VectorName[0])	\
	}

/* uncloak the names */
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_TASK_INIT_VECTOR_HDR_INIT_V1(			\
		VectorName,					\
		pSetupFn,					\
		pFinishFn					\
		)						\
	__TMS_USBPUMP_TASK_INIT_VECTOR_HDR_INIT_V1(		\
		VectorName,					\
		pSetupFn,					\
		pFinishFn					\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/****************************************************************************\
|
|	Functions...
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_BOOL
UsbPump_GenericTaskInit(
	__TMS_CONST __TMS_USBPUMP_TASK_INIT_VECTOR_HDR *
	);

__TMS_END_DECLS

/**** end of usbpump_taskinit.h ****/
#endif /* _USBPUMP_TASKINIT_H_ */
