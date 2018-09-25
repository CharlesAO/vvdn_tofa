/* usbpump_dcd_initvector_defines.h	Sat Jul 25 2015 15:23:24 tmm */

/*

Module:  usbpump_dcd_initvector_defines.h

Function:
	Boilerplate for the multi-DCD initialization strategy

Version:
	V3.15c	Sat Jul 25 2015 15:23:24 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2015

Revision history:
   3.15c  Sun Jul 19 2015 15:17:22  tmm
	19324: Module created.

   3.15c  Sat Jul 25 2015 15:23:24  tmm
	19324: improve after review.

*/

#ifndef _USBPUMP_DCD_INITVECTOR_DEFINES_H_		/* prevent multiple includes */
#define _USBPUMP_DCD_INITVECTOR_DEFINES_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBAPPINIT_H_
# include "usbappinit.h"
#endif

/****************************************************************************\
|
|	This complicated code creates external references for the USB 
|	descriptor tables. If any of the sytmbols DESCRIPTOR_ROOT_0 
|	through DESRIPTOR_ROOT_9 are defined as preprocessor symbols, then
|	each defined DESCRIPTOR_ROOT_{n} is treated as the name of a
|	USBRC_ROOTTABLE, and is declared. If none of these is defined, then
|	a single external reference is declared to gk_UsbDecriptorRoot,
|	and DESCRIPTOR_ROOT_0 is set to gk_UsbDescriptorRoot.
|
\****************************************************************************/


#define __TMS_EXTERN_USBRC_ROOTTABLE(a__NAME)		\
	extern __TMS_CONST __TMS_USBRC_ROOTTABLE a__NAME

#ifdef DESCRIPTOR_ROOT_0
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_0);
#endif
#ifdef DESCRIPTOR_ROOT_1
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_1);
#endif
#ifdef DESCRIPTOR_ROOT_2
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_2);
#endif
#ifdef DESCRIPTOR_ROOT_3
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_3);
#endif
#ifdef DESCRIPTOR_ROOT_4
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_4);
#endif
#ifdef DESCRIPTOR_ROOT_5
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_5);
#endif
#ifdef DESCRIPTOR_ROOT_6
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_6);
#endif
#ifdef DESCRIPTOR_ROOT_7
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_7);
#endif
#ifdef DESCRIPTOR_ROOT_8
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_8);
#endif
#ifdef DESCRIPTOR_ROOT_9
	__TMS_EXTERN_USBRC_ROOTTABLE(DESCRIPTOR_ROOT_9);
#endif
#ifndef	DESCRIPTOR_ROOT_0
# ifndef DESCRIPTOR_ROOT_1
#  ifndef DESCRIPTOR_ROOT_2
#   ifndef DESCRIPTOR_ROOT_3
#    ifndef DESCRIPTOR_ROOT_4
#     ifndef DESCRIPTOR_ROOT_5
#      ifndef DESCRIPTOR_ROOT_6
#       ifndef DESCRIPTOR_ROOT_7
#        ifndef DESCRIPTOR_ROOT_8
#         ifndef DESCRIPTOR_ROOT_9
#	    define DESCRIPTOR_ROOT_0 gk_UsbDescriptorRoot
	__TMS_EXTERN_USBRC_ROOTTABLE(gk_UsbDescriptorRoot);
#         endif
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif

#define	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
	a__UsbDescriptorRoot, a_DebugFlags, a_pAppInfo			\
	)								\
	__TMS_USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2(		\
		/* pDescriptorTable */ &(a__UsbDescriptorRoot),		\
		/* DebugFlags */ (a_DebugFlags),			\
		/* pAppProbeFunction */ __TMS_NULL,			\
		/* pAppInitFunction */					\
			UsbPump_GenericApplicationInit_Protocols,	\
		/* pAppInfo */ (a_pAppInfo)				\
		)


/* now, define a chain of macros that will generate all the init nodes */
#define	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_0(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_0, a_DebugFlags, a_pAppInfo		\
		)

#ifdef DESCRIPTOR_ROOT_1
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_1(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_0(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_1, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_1(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_0(a_DebugFlags, a_pAppInfo)
#endif

#ifdef DESCRIPTOR_ROOT_2
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_2(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_1(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_2, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_2(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_1(a_DebugFlags, a_pAppInfo)
#endif

#ifdef DESCRIPTOR_ROOT_3
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_3(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_2(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_3, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_3(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_2(a_DebugFlags, a_pAppInfo)
#endif

#ifdef DESCRIPTOR_ROOT_4
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_4(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_3(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_4, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_4(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_3(a_DebugFlags, a_pAppInfo)
#endif

#ifdef DESCRIPTOR_ROOT_5
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_5(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_4(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_5, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_5(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_4(a_DebugFlags, a_pAppInfo)
#endif

#ifdef DESCRIPTOR_ROOT_6
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_6(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_5(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_6, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_6(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_5(a_DebugFlags, a_pAppInfo)
#endif

#ifdef DESCRIPTOR_ROOT_7
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_7(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_6(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_7, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_7(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_6(a_DebugFlags, a_pAppInfo)
#endif

#ifdef DESCRIPTOR_ROOT_8
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_8(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_7(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_8, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_8(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_7(a_DebugFlags, a_pAppInfo)
#endif

#ifdef DESCRIPTOR_ROOT_9
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_9(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_8(a_DebugFlags, a_pAppInfo),	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
		DESCRIPTOR_ROOT_9, a_DebugFlags, a_pAppInfo		\
		)
#else
# define __TMS_USBPUMP_DCD_INIT_VECTOR_NODE_9(				\
	a_DebugFlags, a_pAppInfo					\
	)								\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_8(a_DebugFlags, a_pAppInfo)
#endif

/*

Name:	USBPUMP_DCD_INIT_VECTOR_NODES_V1()

Index:	Name:	__TMS_USBPUMP_DCD_INIT_VECTOR_NODES_V1()

Function:
	Generate the body of a multi-DCD-capable application init vector.

Definition:
	static CONST USB_DATAPUMP_APPLICATION_INIT_VECTOR MyVector[] =
		{
		USBPUMP_DCD_INIT_VECTOR_NODES_V1(
			ULONG ulDebugFlags,
			CONST USBPUMP_PROTOCOL_INIT_NODE_VECTOR *pProtoInit
			)
		};

Description:
	This macro expands to a suitable list of
	USB_DATAPUMP_APPLICATION_INIT_VECTOR_INIT_V2() entries, one for each
	DCD known at build time.  Each entry shares the common debug flag 
	setting and protocol-init vector given by the parameters
	ulDebugFlags and pProtoInit.

Returns:
	This macro expands to a comma-separated list of one or more
	USB_DATAPUMP_APPLICATION_INIT_VECTOR initialization entries.

Notes:
	Because the DataPump application-initializatin and
	protocol-initialization layers are loosely coupled, the type of the
	pProtoInit argument is not confirmed. This is probably not great
	design, but a new API would be required to change this.

	The USB_DATAPUMP_APPLICATION_INIT_VECTOR is really a DCD-
	initialization vector; but again, it was an abstraction designed to
	serve more uses than we eventually settled on.

	This API is explicitly versioned. If updates to the core DataPump
	cause us to want to add or remove parameters, we'll create a new API
	with a different version number, and we'll change the expansion of
	USBPUMP_DCD_INIT_VECTOR_NODES_V1() to accommodate the changes.

See also:
	USB_DATAPUMP_APPLICATION_INIT_VECTOR.

*/

#define	__TMS_USBPUMP_DCD_INIT_VECTOR_NODES_V1(a_DebugFlags, a_pAppInfo) \
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_9(a_DebugFlags, a_pAppInfo)

/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define EXTERN_USBRC_ROOTTABLE(a__NAME)	\
   __TMS_EXTERN_USBRC_ROOTTABLE(a__NAME)
# define USBPUMP_DCD_INIT_VECTOR_NODE(				\
	a__UsbDescriptorRoot, a_DebugFlags, a_pAppInfo			\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE(				\
	a__UsbDescriptorRoot, a_DebugFlags, a_pAppInfo			\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_0(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_0(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_1(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_1(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_1(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_1(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_2(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_2(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_2(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_2(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_3(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_3(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_3(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_3(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_4(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_4(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_4(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_4(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_5(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_5(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_5(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_5(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_6(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_6(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_6(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_6(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_7(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_7(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_7(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_7(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_8(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_8(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_8(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_8(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_9(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_9(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODE_9(				\
	a_DebugFlags, a_pAppInfo					\
	)	\
	__TMS_USBPUMP_DCD_INIT_VECTOR_NODE_9(				\
	a_DebugFlags, a_pAppInfo					\
	)
# define USBPUMP_DCD_INIT_VECTOR_NODES_V1(a_DebugFlags, a_pAppInfo)	\
   __TMS_USBPUMP_DCD_INIT_VECTOR_NODES_V1(a_DebugFlags, a_pAppInfo)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_dcd_initvector_defines.h ****/
#endif /* _USBPUMP_DCD_INITVECTOR_DEFINES_H_ */
