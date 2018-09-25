/* usbpump_usbdi_cfgnodes.h	Thu Oct 04 2012 16:59:15 cccho */

/*

Module:  usbpump_usbdi_cfgnodes.h

Function:
	"Configuration nodes" which represent USB device configs to USBD.

Version:
	V3.11b	Thu Oct 04 2012 16:59:15 cccho	Edit level 12

Copyright notice:
	This file copyright (C) 2008-2010, 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	February 2008

Revision history:
   1.97k  Mon Feb  4 2008 16:54:18  tmm
	3747: Module created.

   1.97k  Wed Oct 01 2008 08:43:03  chwon
	3747: generated uncloaked name

   2.01a  Mon Feb 23 2009 11:52:53  djt
	7416: Added new fields to USBDI_PIPE_NODE for USB 3.0.

   2.01a  Fri Apr 03 2009 09:50:01  djt
	7416: Changed bMaxBurstSize to bMaxBurst.

   2.01a  Mon Nov 16 2009 16:47:41  chwon
	9427: include usbpump_usbdi_types.h

   3.01a  Mon Feb  8 2010 19:53:47  tmm
	9893:  rename bMaxPStreams to wMaxStreamID, resize.

   3.01a  Tue Feb  9 2010 13:54:23  tmm
	9893:  fix resizing; make other small fields in USBDI_PIPE_NODE
	bit fields so that (when running 32 bit) the USBDI_PIPE_NODE size
	doesn't change.

   3.01c  Wed Aug 18 2010 17:55:16  chwon
	10988: add fFunctionSuspended flag in the USBPUMP_USBDI_IFC_NODE.

   3.01c  Wed Sep 08 2010 11:46:46  chwon
	11214: change fFunctionSuspended flag to bStatus and add bit definition

   3.11b  Thu Sep 20 2012 19:08:14  cccho
	15952: add bmAttributesSync and bmAttributesUsage bit fileds in
	USBPUMP_USBDI_PIPE_NODE.

   3.11b  Thu Oct 04 2012 16:59:15  cccho
	15952: add comment. No code changes.

*/

#ifndef _USBPUMP_USBDI_CFGNODES_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_CFGNODES_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

/****************************************************************************\
|
|	Forward definitions
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_CFG_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_IFC_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_ALTSET_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_PIPE_NODE);

/****************************************************************************\
|
|	Structures for representing the topology and schedule
|
\****************************************************************************/

#include "usbpump_pushpack4.h"
__TMS_LIBPORT_PACKED4_DEFINITION(
struct __TMS_STRUCTNAME(USBPUMP_USBDI_CFG_NODE)
)
	{
	__TMS_UINT16	iNextCfgNode;		/* [0]: offset to next node */
	__TMS_UINT8	nInterfaces;		/* [2] */
	__TMS_UINT8	bConfigurationValue;	/* [3] */
	__TMS_UINT8	bmAttributes;		/* [4] */
	__TMS_UINT8	bMaxPower;		/* [5] */
	__TMS_UINT8	Reserved1;		/* [6] */
	__TMS_UINT8	Reserved2;		/* [7] */
	} __TMS_LIBPORT_PACKED4_SUFFIX;

/*
|| The trick below (p[0].iNextCfgNode) forces a typecheck on pNode being
|| the right type.
*/
#define __TMS_USBPUMP_USBDI_CFG_NODE_NEXT(pNode)			\
	((__TMS_USBPUMP_USBDI_CFG_NODE *)				\
	  ((pNode)->iNextCfgNode					\
		? ((char *)&(pNode)[0].iNextCfgNode) +			\
		   (pNode)->iNextCfgNode				\
		: __TMS_NULL)						\
	)

/*
|| The trick below (p[1].iNextCfgNode) forces a typecheck on pNode being
|| the right type.
*/
#define __TMS_USBPUMP_USBDI_CFG_NODE_IFC(pNode)				\
	((__TMS_USBPUMP_USBDI_IFC_NODE *) &((pNode)[1].iNextCfgNode))

#define	__TMS_USBPUMP_USBDI_CFG_NODE_FIRST_IFC(pNode)			\
	((pNode)->nInterfaces ? __TMS_USBPUMP_USBDI_CFG_NODE_IFC(pNode)	\
			      : __TMS_NULL)

/*
|| Set pNode to link to pNode2.
*/
#define	__TMS_USBPUMP_USBDI_CFG_NODE_SET_NEXT(pNode, pNode2)		\
   do	{								\
	__TMS_USBPUMP_USBDI_CFG_NODE * __TMS_CONST __pNode = (pNode);	\
	__TMS_USBPUMP_USBDI_CFG_NODE * __TMS_CONST __pNode2 = (pNode2);	\
									\
	if (__pNode2 == __TMS_NULL ||					\
	    __pNode > __pNode2)						\
		__pNode->iNextCfgNode = 0;				\
	else								\
		__pNode->iNextCfgNode = (char *)__pNode2 -		\
					(char *)__pNode;		\
	} while (0)

/*
|| A formula for estimating a config tree size based on num cfgs,
|| num ifcs per cfg, num altsets per ifc and npipes; the last are
|| "averages" over the tree: 2 configs averaging 3 interfaces per
|| config, averaging one altset per ifc, with (average) 1 pipe per
|| altset is USBPUMP_USBDI_ESTIMATE_CFG_TREE_SIZE(2, 3, 1, 1)
*/
#define __TMS_USBPUMP_USBDI_ESTIMATE_CFG_TREE_SIZE(			\
	ncfg,								\
	nifc,								\
	naltset,							\
	npipe)								\
	((ncfg) * (sizeof(__TMS_USBPUMP_USBDI_CFG_NODE) +		\
	   (nifc) * (sizeof(__TMS_USBPUMP_USBDI_IFC_NODE) +		\
		(naltset) * (sizeof(__TMS_USBPUMP_USBDI_ALTSET_NODE) +	\
			(npipe) * sizeof(__TMS_USBPUMP_USBDI_PIPE_NODE)))) \
	)

/*
|| If you know exactly how many ifcs, altsets, etc, you have across the
|| whole device, you can use this to calculate the tree size.  Note
|| that the above example would be:
||	USBPUMP_USBDI_EXACT_CFG_TREE_SIZE(2, 2*3, 2*3*1, 2*3*1*1)
|| This form of the macro is useful for rational approximations, since
|| you can put in a factor of 10 or 100 in each step and then divide
|| out:  for example USBPUMP_USBDI_ESTIMATE_CFG_TREE_SIZE(1, 1, 1.5, 2.5)
|| w/o floating point is:
||	USBPUMP_USBDI_EXACT_CFG_TREE_SIZE(10, 10, 15, 25) / 10
*/
#define __TMS_USBPUMP_USBDI_EXACT_CFG_TREE_SIZE(			\
	ncfg,								\
	nifc,								\
	naltset,							\
	npipe)								\
	((ncfg) * sizeof(__TMS_USBPUMP_USBDI_CFG_NODE) +		\
	 (nifc) * sizeof(__TMS_USBPUMP_USBDI_IFC_NODE) +		\
	 (naltset) * sizeof(__TMS_USBPUMP_USBDI_ALTSET_NODE) +		\
	 (npipe) * sizeof(__TMS_USBPUMP_USBDI_PIPE_NODE)		\
	)


/****************************************************************************\
|
|	The USBPUMP_USBDI_IFC_NODE describes a collection of alternate
|	settings.
|
\****************************************************************************/


__TMS_LIBPORT_PACKED4_DEFINITION(
struct __TMS_STRUCTNAME(USBPUMP_USBDI_IFC_NODE)
)
	{
	__TMS_UINT16	iNextIfcNode;		/* [0]: offset to next node */
	__TMS_UINT16	iCurrentAltSetting;	/* [2] */
	__TMS_UINT8	bInterfaceNumber;	/* [4] */
	__TMS_UINT8	bNumAltSettings;	/* [5] */
	__TMS_UINT8	Reserved1;		/* [6] */
	__TMS_UINT8	bStatus;	/* [7] */
	} __TMS_LIBPORT_PACKED4_SUFFIX;

#define	__TMS_USBPUMP_USBDI_IFC_NODE_STATUS_REMOTE_WAKE_CAPABLE	(1 << 0)
#define	__TMS_USBPUMP_USBDI_IFC_NODE_STATUS_REMOTE_WAKE_ENABLED	(1 << 1)

/*
|| The trick below (p[0].iNextIfcNode) forces a typecheck on pNode being
|| the right type.
*/
#define __TMS_USBPUMP_USBDI_IFC_NODE_NEXT(pNode)			\
	((__TMS_USBPUMP_USBDI_IFC_NODE *)				\
	  ((pNode)->iNextIfcNode					\
		? ((char *)&(pNode)[0].iNextIfcNode) +			\
		   (pNode)->iNextIfcNode				\
		: __TMS_NULL)						\
	)

/*
|| The trick below (p[1].iNextIfcNode) forces a typecheck on pNode being
|| the right type.
*/
#define __TMS_USBPUMP_USBDI_IFC_NODE_ALTSET(pNode)			\
	((__TMS_USBPUMP_USBDI_ALTSET_NODE *) &((pNode)[1].iNextIfcNode))

#define	__TMS_USBPUMP_USBDI_IFC_NODE_FIRST_ALTSET(pNode)		\
	((pNode)->bNumAltSettings					\
			? __TMS_USBPUMP_USBDI_IFC_NODE_ALTSET(pNode)	\
			: __TMS_NULL)

/*
|| Get the current alt setting.
*/
#define __TMS_USBPUMP_USBDI_IFC_NODE_GET_CURRENT(pNode)			\
	((__TMS_USBPUMP_USBDI_ALTSET_NODE *)				\
	  ((pNode)->iCurrentAltSetting					\
		? ((char *)&(pNode)[0].iNextIfcNode) +			\
		   (pNode)->iCurrentAltSetting				\
		: __TMS_NULL)						\
	)
/*
|| Set pNode to link to pNode2.
*/
#define	__TMS_USBPUMP_USBDI_IFC_NODE_SET_NEXT(pNode, pNode2)		\
   do	{								\
	__TMS_USBPUMP_USBDI_IFC_NODE * __TMS_CONST __pNode = (pNode);	\
	__TMS_USBPUMP_USBDI_IFC_NODE * __TMS_CONST __pNode2 = (pNode2);	\
									\
	if (__pNode2 == __TMS_NULL ||					\
	    __pNode > __pNode2)						\
		__pNode->iNextIfcNode = 0;				\
	else								\
		__pNode->iNextIfcNode = (char *)__pNode2 -		\
					(char *)__pNode;		\
	} while (0)

/*
|| Set pNode to link to pAltSet
*/
#define	__TMS_USBPUMP_USBDI_IFC_NODE_SET_CURRENT(pNode, pNode2)		\
   do	{								\
	__TMS_USBPUMP_USBDI_IFC_NODE * __TMS_CONST __pNode = (pNode);	\
	__TMS_USBPUMP_USBDI_ALTSET_NODE * __TMS_CONST			\
			__pNode2 = (pNode2);				\
									\
	if (__pNode2 == __TMS_NULL ||					\
	    (char *)__pNode > (char *)__pNode2)				\
		__pNode->iCurrentAltSetting = 0;			\
	else								\
		__pNode->iCurrentAltSetting = (char *)__pNode2 -	\
					      (char *)__pNode;		\
	} while (0)

/*
|| represent an alternate setting in canonical form
*/
__TMS_LIBPORT_PACKED4_DEFINITION(
struct __TMS_STRUCTNAME(USBPUMP_USBDI_ALTSET_NODE)
)
	{
	/*
	|| The byte relative offset to the next alt set
	*/
	__TMS_UINT16	iNextAltSetNode;
	__TMS_UINT8	bAlternateSetting;
	__TMS_UINT8	bNumPipes;
	__TMS_UINT8     bInterfaceClass;
	__TMS_UINT8     bInterfaceSubClass;
	__TMS_UINT8     bInterfaceProtocol;
	__TMS_UINT8     iInterface;
	} __TMS_LIBPORT_PACKED4_SUFFIX;

/*
|| The trick below (p[0].iNextAltSetNode) forces a typecheck on pNode being
|| the right type.
*/
#define __TMS_USBPUMP_USBDI_ALTSET_NODE_NEXT(pNode)			\
	((__TMS_USBPUMP_USBDI_ALTSET_NODE *)				\
	  ((pNode)->iNextAltSetNode					\
		? ((char *)&(pNode)[0].iNextAltSetNode) +		\
					(pNode)->iNextAltSetNode	\
		: __TMS_NULL)						\
	)

/*
|| The trick below (p[1].iNextAltSetNode) forces a typecheck on pNode being
|| the right type.
*/
#define __TMS_USBPUMP_USBDI_ALTSET_NODE_PIPE(pNode)			\
	((__TMS_USBPUMP_USBDI_PIPE_NODE *) &((pNode)[1].iNextAltSetNode))

/*
|| Set pNode to link to pNode2.
*/
#define	__TMS_USBPUMP_USBDI_ALTSET_NODE_SET_NEXT(pNode, pNode2)		\
   do	{								\
	__TMS_USBPUMP_USBDI_ALTSET_NODE * __TMS_CONST			\
			__pNode = (pNode);				\
	__TMS_USBPUMP_USBDI_ALTSET_NODE * __TMS_CONST			\
			__pNode2 = (pNode2);				\
									\
	if (__pNode2 == __TMS_NULL ||					\
	    __pNode > __pNode2)						\
		__pNode->iNextAltSetNode = 0;				\
	else								\
		__pNode->iNextAltSetNode = (char *)__pNode2 -		\
					   (char *)__pNode;		\
	} while (0)


/*
|| represent a single pipe in normalized form.
*/
__TMS_LIBPORT_PACKED4_DEFINITION(
struct __TMS_STRUCTNAME(USBPUMP_USBDI_PIPE_NODE)
)
	{
	__TMS_USBPUMP_USBDI_PIPE_HANDLE	hPipe;
	__TMS_UINT32	dwMaxTransferSize;

	/*
	|| all these fileds must fit in a single UIINT32; there are 3 bit
	|| available.
	*/
	__TMS_UINT32	bEndpointAddress: 8,
			bmAttributes: 8,
			bExtraPackets: 2,
			bMaxBurst: 5,
			bMult: 2,
			bmAttributesSync: 2,
			bmAttributesUsage: 2;

	/* polling interval, in (micro)frames */
	__TMS_UINT16	wInterval;

	/* max packet size, in engineering units (not encoded) */
	__TMS_UINT16	wMaxPacketSize;
	
	/* bytes per interval, in engineering units (from ss ep companion) */
	__TMS_UINT16	wBytesPerInterval;

	/* max stream ID, in engineering units */
	__TMS_UINT16	wMaxStreamID;
	} __TMS_LIBPORT_PACKED4_SUFFIX;

#include "usbpump_poppack.h"


/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_CFG_NODE_NEXT(pNode)	\
   __TMS_USBPUMP_USBDI_CFG_NODE_NEXT(pNode)
# define USBPUMP_USBDI_CFG_NODE_IFC(pNode)	\
   __TMS_USBPUMP_USBDI_CFG_NODE_IFC(pNode)
# define USBPUMP_USBDI_CFG_NODE_FIRST_IFC(pNode)	\
   __TMS_USBPUMP_USBDI_CFG_NODE_FIRST_IFC(pNode)
# define USBPUMP_USBDI_CFG_NODE_SET_NEXT(pNode, pNode2)	\
   __TMS_USBPUMP_USBDI_CFG_NODE_SET_NEXT(pNode, pNode2)
# define USBPUMP_USBDI_ESTIMATE_CFG_TREE_SIZE(			\
	ncfg,								\
	nifc,								\
	naltset,							\
	npipe)	\
	__TMS_USBPUMP_USBDI_ESTIMATE_CFG_TREE_SIZE(			\
	ncfg,								\
	nifc,								\
	naltset,							\
	npipe)
# define USBPUMP_USBDI_EXACT_CFG_TREE_SIZE(			\
	ncfg,								\
	nifc,								\
	naltset,							\
	npipe)	\
	__TMS_USBPUMP_USBDI_EXACT_CFG_TREE_SIZE(			\
	ncfg,								\
	nifc,								\
	naltset,							\
	npipe)
# define USBPUMP_USBDI_IFC_NODE_STATUS_REMOTE_WAKE_CAPABLE	\
   __TMS_USBPUMP_USBDI_IFC_NODE_STATUS_REMOTE_WAKE_CAPABLE
# define USBPUMP_USBDI_IFC_NODE_STATUS_REMOTE_WAKE_ENABLED	\
   __TMS_USBPUMP_USBDI_IFC_NODE_STATUS_REMOTE_WAKE_ENABLED
# define USBPUMP_USBDI_IFC_NODE_NEXT(pNode)	\
   __TMS_USBPUMP_USBDI_IFC_NODE_NEXT(pNode)
# define USBPUMP_USBDI_IFC_NODE_ALTSET(pNode)	\
   __TMS_USBPUMP_USBDI_IFC_NODE_ALTSET(pNode)
# define USBPUMP_USBDI_IFC_NODE_FIRST_ALTSET(pNode)	\
   __TMS_USBPUMP_USBDI_IFC_NODE_FIRST_ALTSET(pNode)
# define USBPUMP_USBDI_IFC_NODE_GET_CURRENT(pNode)	\
   __TMS_USBPUMP_USBDI_IFC_NODE_GET_CURRENT(pNode)
# define USBPUMP_USBDI_IFC_NODE_SET_NEXT(pNode, pNode2)	\
   __TMS_USBPUMP_USBDI_IFC_NODE_SET_NEXT(pNode, pNode2)
# define USBPUMP_USBDI_IFC_NODE_SET_CURRENT(pNode, pNode2)	\
   __TMS_USBPUMP_USBDI_IFC_NODE_SET_CURRENT(pNode, pNode2)
# define USBPUMP_USBDI_ALTSET_NODE_NEXT(pNode)	\
   __TMS_USBPUMP_USBDI_ALTSET_NODE_NEXT(pNode)
# define USBPUMP_USBDI_ALTSET_NODE_PIPE(pNode)	\
   __TMS_USBPUMP_USBDI_ALTSET_NODE_PIPE(pNode)
# define USBPUMP_USBDI_ALTSET_NODE_SET_NEXT(pNode, pNode2)	\
   __TMS_USBPUMP_USBDI_ALTSET_NODE_SET_NEXT(pNode, pNode2)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_cfgnodes.h ****/
#endif /* _USBPUMP_USBDI_CFGNODES_H_ */
