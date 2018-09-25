/* usbpump_usbd_class_composite_config.h	Mon Mar 24 2008 14:58:42 djt */

/*

Module:  usbpump_usbd_class_composite_config.h

Function:
	Definition of API Composite driver configuration

Version:
	V1.97k	Mon Mar 24 2008 14:58:42 djt	Edit level 2

Copyright notice:
	This file copyright (C) 2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2008

Revision history:
   1.97k  Wed Mar 19 2008 12:35:38 chwon
	3049: Module created.

   1.97k  Mon Mar 24 2008 14:58:42  djt
	2590: Split out CLASS_COMPOSITE_PRIVATE_CONFIG.

*/

#ifndef _USBPUMP_USBD_CLASS_COMPOSITE_CONFIG_H_	/* prevent multiple includes */
#define _USBPUMP_USBD_CLASS_COMPOSITE_CONFIG_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USBPUMP_USBDI_COMPOSITE_H_
# include "usbpump_usbdi_composite.h"
#endif

#ifndef _USBPUMP_USBDI_INIT_H_
# include "usbpump_usbdi_init.h"
#endif

/****************************************************************************\
|
|	The USBD Composite Class Driver Configuration
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG)
	{
	__TMS_UINT32			MagicBegin;

	/*
	|| The total number of Composite Functions allowed in the system -
	|| not per Composite Device.
	*/
	__TMS_BYTES			NumberCFs;

	/*
	|| The number of Composite Sub Functions allowed per function.
	*/
	__TMS_BYTES			NumberCSFs;

	/*
	|| The maximum number of pipes in a Composite Device
	*/
	__TMS_BYTES			NumberPipes;

	/*
	|| The maximum configuration descriptor size.
	*/
	__TMS_BYTES			SizeConfigDesc;

	__TMS_UINT32			MagicEnd;
	};

__TMS_TYPE_DEF_STRUCT	(USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG);
struct __TMS_STRUCTNAME	(USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG)
	{
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG	ClassConfig;
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG ClassPrivateConfig;
	};


#define	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_BEGIN		\
	__TMS_UHIL_MEMTAG('<', 'T', 'c', 'd')

#define	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_END		\
	__TMS_UHIL_MEMTAG('>', 'T', 'c', 'd')

#define	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG_INIT_V1(	\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)								\
	{								\
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_BEGIN,		\
	(a_NumberCFs),							\
	(a_NumberCSFs),							\
	(a_NumberPipes),						\
	(a_SizeConfigDesc),						\
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_END		\
	}

#define	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG_SETUP_V1(	\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)								\
    do	{								\							\
	(a_pConfig)->MagicBegin =					\
		__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_BEGIN;	\
	(a_pConfig)->NumberCFs = (a_NumberCFs);				\
	(a_pConfig)->NumberCSFs = (a_NumberCSFs);			\
	(a_pConfig)->NumberPipes = (a_NumberPipes);			\
	(a_pConfig)->SizeConfigDesc = (a_SizeConfigDesc);		\
	(a_pConfig)->MagicEnd =						\
		__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_END;	\
	} while (0)

#define	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_INIT_V1(		\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances,							\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)								\
	{								\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_INIT_V1(		\
		a_pInitMatchList,					\
		a_pClassName,						\
		a_pFunctionName,					\
		a_NumInstances						\
		),							\
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG_INIT_V1(	\
		a_NumberCFs,						\
		a_NumberCSFs,						\
		a_NumberPipes,						\
		a_SizeConfigDesc					\
		)							\
	}

#define	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_SETUP_V1(		\
	a_pConfig,							\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances,							\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)								\
    do	{								\
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG_SETUP_V1(		\
		&((a_pConfig)->ClassConfig),				\
		a_pInitMatchList,					\
		a_pClassName,						\
		a_pFunctionName,					\
		a_NumInstances						\
		);							\
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG_SETUP_V1(	\
		&((a_pConfig)->PrivateConfig),				\
		a_NumberCFs,						\
		a_NumberCSFs,						\
		a_NumberPipes,						\
		a_SizeConfigDesc					\
		};							\
	} while (0)


/****************************************************************************\
|
|	Default linkage
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_USBPUMP_USBDI_INIT_MATCH_LIST 
gk_UsbPumpUsbdiClassComposite_InitMatchList;

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_BEGIN	\
   __TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_BEGIN
# define USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_END	\
   __TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_MAGIC_END
# define USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG_INIT_V1(	\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG_INIT_V1(	\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)
# define USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG_SETUP_V1(	\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_PRIVATE_CONFIG_SETUP_V1(	\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)
# define USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_INIT_V1(		\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances,							\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_INIT_V1(		\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances,							\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)
# define USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_SETUP_V1(		\
	a_pConfig,							\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances,							\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)	\
	__TMS_USBPUMP_USBDI_CLASS_COMPOSITE_CONFIG_SETUP_V1(		\
	a_pConfig,							\
	a_pInitMatchList,						\
	a_pClassName,							\
	a_pFunctionName,						\
	a_NumInstances,							\
	a_NumberCFs,							\
	a_NumberCSFs,							\
	a_NumberPipes,							\
	a_SizeConfigDesc						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbd_class_composite_config.h ****/
#endif /* _USBPUMP_USBD_CLASS_COMPOSITE_CONFIG_H_ */
