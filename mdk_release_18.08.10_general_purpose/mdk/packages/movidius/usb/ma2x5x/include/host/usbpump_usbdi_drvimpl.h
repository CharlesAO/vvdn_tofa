/* usbpump_usbdi_drvimpl.h	Wed Dec 12 2007 16:14:42 skpark */

/*

Module:  usbpump_usbdi_drvimpl.h

Function:
	Home for the USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION object
	and related types.

Version:
	V1.97j	Wed Dec 12 2007 16:14:42 skpark	Edit level 2

Copyright notice:
	This file copyright (C) 2005-2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Thu Aug 11 2005 16:47:05  tmm
	Module created.

   1.97j  Wed Dec 12 2007 16:14:42  skpark
	PR3579: fixed typo in _SETUP_V1 macro, sizeManagementPool is assigned to
	sizeInstancePool, which causes memory allocation error.

*/

#ifndef _USBPUMP_USBDI_DRVIMPL_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_DRVIMPL_H_

# ifndef _USBPUMP_USBDI_TYPES_H_
#  include "usbpump_usbdi_types.h"
# endif


# ifndef _USBPUMPOBJECT_H_
#  include "usbpumpobject.h"
# endif

/****************************************************************************\
|
|	The structure types
|
\****************************************************************************/


struct __TMS_STRUCTNAME	(USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION)
	{
	__TMS_UINT32		MagicBegin;
	__TMS_BYTES		sizeClass;
	__TMS_BYTES		sizeFunction;
	__TMS_CONST __TMS_TEXT	*pClassName;
	__TMS_CONST __TMS_TEXT	*pFunctionName;
	__TMS_USBPUMP_OBJECT_CLASS_IOCTL_FN	*pClassIoctlFn;
	__TMS_USBPUMP_OBJECT_IOCTL_FN	*pFunctionIoctlFn;
	__TMS_BYTES		pipesPerInstance;
	__TMS_BYTES		sizeDriverPool;
	__TMS_BYTES		sizeInstancePool;
	__TMS_BYTES		sizeManagementPool;
	__TMS_UINT32		MagicEnd;
	};

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_BEGIN	\
	__TMS_UHIL_MEMTAG('<', 'D', 'C', 'I')

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_END	\
	__TMS_UHIL_MEMTAG('>', 'D', 'C', 'I')

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_INIT_V1(	\
	a_sizeClass,							\
	a_sizeFunction,							\
	a_pClassName,							\
	a_pFunctionName,						\
	a_pClassIoctlFn,						\
	a_pFunctionIoctlFn,						\
	a_pipesPerInstance,						\
	a_sizeDriverPool,						\
	a_sizeInstancePool,						\
	a_sizeManagementPool						\
	)								\
	{								\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_BEGIN,	\
	(a_sizeClass),							\
	(a_sizeFunction),						\
	(a_pClassName),							\
	(a_pFunctionName),						\
	(a_pClassIoctlFn),						\
	(a_pFunctionIoctlFn),						\
	(a_pipesPerInstance),						\
	(a_sizeDriverPool),						\
	(a_sizeInstancePool),						\
	(a_sizeManagementPool),						\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_END	\
	}

#define	__TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_SETUP_V1(	\
	a_pImplementation,						\
	a_sizeClass,							\
	a_sizeFunction,							\
	a_pClassName,							\
	a_pFunctionName,						\
	a_pClassIoctlFn,						\
	a_pFunctionIoctlFn,						\
	a_pipesPerInstance,						\
	a_sizeDriverPool,						\
	a_sizeInstancePool,						\
	a_sizeManagementPool						\
	)								\
    do	{								\
	(a_pImplementation)->MagicBegin =				\
	  __TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_BEGIN;	\
	(a_pImplementation)->sizeClass = (a_sizeClass);			\
	(a_pImplementation)->sizeFunction = (a_sizeFunction);		\
	(a_pImplementation)->pClassName = (a_pClassName);		\
	(a_pImplementation)->pFunctionName = (a_pFunctionName);		\
	(a_pImplementation)->pClassIoctlFn =				\
				(a_pClassIoctlFn);			\
	(a_pImplementation)->pFunctionIoctlFn = (a_pFunctionIoctlFn);	\
	(a_pImplementation)->pipesPerInstance = (a_pipesPerInstance);	\
	(a_pImplementation)->sizeDriverPool = (a_sizeDriverPool);	\
	(a_pImplementation)->sizeInstancePool = (a_sizeInstancePool);	\
	(a_pImplementation)->sizeManagementPool = (a_sizeManagementPool); \
	(a_pImplementation)->MagicEnd =					\
	  __TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_END;	\
	} while (0)

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_BEGIN	\
   __TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_BEGIN
# define USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_END	\
   __TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_MAGIC_END
# define USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_INIT_V1(	\
	a_sizeClass,							\
	a_sizeFunction,							\
	a_pClassName,							\
	a_pFunctionName,						\
	a_pClassIoctlFn,						\
	a_pFunctionIoctlFn,						\
	a_pipesPerInstance,						\
	a_sizeDriverPool,						\
	a_sizeInstancePool,						\
	a_sizeManagementPool						\
	)	\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_INIT_V1(	\
	a_sizeClass,							\
	a_sizeFunction,							\
	a_pClassName,							\
	a_pFunctionName,						\
	a_pClassIoctlFn,						\
	a_pFunctionIoctlFn,						\
	a_pipesPerInstance,						\
	a_sizeDriverPool,						\
	a_sizeInstancePool,						\
	a_sizeManagementPool						\
	)
# define USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_SETUP_V1(	\
	a_pImplementation,						\
	a_sizeClass,							\
	a_sizeFunction,							\
	a_pClassName,							\
	a_pFunctionName,						\
	a_pClassIoctlFn,						\
	a_pFunctionIoctlFn,						\
	a_pipesPerInstance,						\
	a_sizeDriverPool,						\
	a_sizeInstancePool,						\
	a_sizeManagementPool						\
	)	\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_IMPLEMENTATION_SETUP_V1(	\
	a_pImplementation,						\
	a_sizeClass,							\
	a_sizeFunction,							\
	a_pClassName,							\
	a_pFunctionName,						\
	a_pClassIoctlFn,						\
	a_pFunctionIoctlFn,						\
	a_pipesPerInstance,						\
	a_sizeDriverPool,						\
	a_sizeInstancePool,						\
	a_sizeManagementPool						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_drvimpl.h ****/
#endif /* _USBPUMP_USBDI_DRVIMPL_H_ */
