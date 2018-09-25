/* usbpump_usbphy_init.h	Thu Jul 10 2008 16:59:16 chwon */

/*

Module:  usbpump_usbphy_init.h

Function:
	General-purpose initialization discipline for use by USB DataPump
	host applications.

Version:
	V1.97k	Thu Jul 10 2008 16:59:16 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	July 2008

Revision history:
   1.97k  Thu Jul 10 2008 16:59:16 chwon
	4286: Module created.

*/

#ifndef _USBPUMP_USBPHY_INIT_H_		/* prevent multiple includes */
#define _USBPUMP_USBPHY_INIT_H_

#ifndef _USBPUMP_TYPES_H_
# include "usbpump_types.h"
#endif

#ifndef _USBPUMP_USBPHY_H_
# include "usbpump_usbphy.h"
#endif

#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_USBPHY_INIT_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_USBPHY_INIT_NODE_VECTOR);


/****************************************************************************\
|
|	The PHY Initialzation Vector
|
\****************************************************************************/

/*

Type:	USBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN

Function:
	Abstract type for the "setup" function entry in a
	USBPUMP_USBPHY_INIT_NODE_VECTOR.

Definition:
	typedef VOID *
		USBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN(
			CONST USBPUMP_USBPHY_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader
			);

	typedef USBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN
		*PUSBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN;

Description:
	Each USBPUMP_USBPHY_INIT_NODE_VECTOR instance may optionally
	supply a setup function pointer.  This function is called prior to
	running the initialization loop on the body of the PHY init
	vector.

	Examples of activities that this function might perform are:
	allocating a block of memory to be used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "setup" functions should be
	prototyped using this type, by writing:

		USBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN MyFunction;

	rather than by writing:

		VOID *MyFunction(
			CONST USBPUMP_USBPHY_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader
			);

Returns:
	The explicit result of this function should be pHcdInitContext.
	The result is passed (unchanged) to all the probe and setup functions.

*/

/*
|| The setup function is called so that the configurator can do some
|| "pre-setup", including prompting for other things to do.  It is optional.
|| The result is passed (unchanged) to all the probe and setup functions.
*/
__TMS_FNTYPE_DEF(
USBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN,
__TMS_VOID *,	/* pHcdInitContext */
	(
	__TMS_CONST __TMS_USBPUMP_USBPHY_INIT_NODE_VECTOR * /*pHdr*/,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */
	));

/*

Type:	USBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN

Function:
	Abstract type for the "finish" function entry in a
	USBPUMP_USBPHY_INIT_NODE_VECTOR.

Definition:
	typedef USBPUMP_IOCTL_RESULT
		USBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN(
			CONST USBPUMP_USBPHY_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pHcdInitContext,
			UINT nHcd
			);

	typedef USBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN
		*PUSBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN;

Description:
	Each USBPUMP_USBPHY_INIT_NODE_VECTOR instance may optionally
	supply a "finish" function pointer.  This function is called after
	running the initialization loop on the body of the PHY init
	vector.

	pHcdInitContext is the value returned previously by the SETUP
	function.  If no SETUP function was provided, pHcdInitContext
	will be NULL.

	Examples of activities that this function might perform are:
	freeing a block of memory used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "finish" functions should be
	prototyped using this type, by writing:

		USBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN MyFunction;

	rather than by writing:

		USBPUMP_IOCTL_RESULT MyFunction(
			CONST USBPUMP_USBPHY_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pHcdInitContext,
			UINT nHcd
			);

Returns:
	Nothing.

*/

/*
|| The finish function is called so that the application can do some
|| "post-setup", including prompting for other things to do.  It is optional.
*/
__TMS_FNTYPE_DEF(
USBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN,
__TMS_VOID,
	(
	__TMS_CONST __TMS_USBPUMP_USBPHY_INIT_NODE_VECTOR *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */,
	__TMS_VOID * /* pHcdInitContext */,
	__TMS_UINT /* nHcd */
	));

/*

Type:	USBPUMP_USBPHY_INIT_NODE_PROBE_FN

Function:
	Abstract type for the "probe" function entry in a
	USBPUMP_USBPHY_INIT_NODE.

Definition:
	typedef BOOL USBPUMP_USBPHY_INIT_NODE_PROBE_FN(
			CONST USBPUMP_USBPHY_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pHcdInitContext
			);

	typedef USBPUMP_USBPHY_INIT_NODE_PROBE_FN
		*PUSBPUMP_USBPHY_INIT_NODE_PROBE_FN;

Description:
	Each USBPUMP_USBPHY_INIT_NODE instance may optionally
	supply a "probe" function pointer.

	pNode points to the USBPUMP_USBPHY_INIT_NODE in question.

	pObjectHeader is the pointer to the DataPump object.

	pHcdInitContext is the value returned previously by the
	USBPUMP_USBPHY_INIT_NODE_VECTOR's "setup" function.  If no
	SETUP function was provided, then pHcdInitContext
	will be NULL.

	Functions which are to be used as "probe" functions should be
	prototyped using this type, by writing:

		USBPUMP_USBPHY_INIT_NODE_PROBE_FN MyProbeFunction;

	rather than by writing:

		BOOL MyProbeFunction(
			CONST USBPUMP_USBPHY_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pHcdInitContext
			);

Returns:
	If the probe function returns FALSE, then
	UsbPump_GenericPhyInit() will not attempt to call the
	create function for pNode.  On the other hand, if the probe function
	returns TRUE, then UsbPumpUsbd_CreateHostControlDrivers() will call
	the create function for pNode to create PHY instance.

*/


/*
|| The driver class probe function, if present, is called prior to
|| creating the driver class instance, so that it can decide whether or not to
|| init.
*/
__TMS_FNTYPE_DEF(
USBPUMP_USBPHY_INIT_NODE_PROBE_FN,
__TMS_BOOL, (
	__TMS_CONST __TMS_USBPUMP_USBPHY_INIT_NODE *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */,
	__TMS_VOID * /* pHcdInitContext */
	));

/*

Type:	USBPUMP_USBPHY_INIT_NODE

Function:
	C structure, representing a single PHY to be created.

Contents:
	The structure has the following contents:

	USBPUMP_USBPHY_INIT_NODE_PROBE_FN *pProbeFn OPTIONAL;
		If non-NULL, the function to be called to further qualify
		the PHY.

	USBPUMP_USBPHY_INIT_FN *pInitFn;
		The (mandatory) function to be called to create a PHY
		instance.

	CONST VOID *pConfig;
		The (mandatory) PHY configuration information.

	UINT32 DebugFlags;
		The recommended debug flags.

Description:
	Each USBPUMP_USBPHY_INIT_NODE represents one potential
	binding for a PHY.

	pProbeFn allows the designer to add an algorithmic filter;

	pInitFn is a pointer to the function that will create the PHY
	instance.  It makes no sense to omit this, but
	UsbPump_GenericPhyInit() will not dereference this
	pointer if it is in fact coded as NULL.

*/

struct __TMS_STRUCTNAME(USBPUMP_USBPHY_INIT_NODE)
	{
	/* The optional PHY probe function. */
	__TMS_USBPUMP_USBPHY_INIT_NODE_PROBE_FN *	pProbeFn;

	/* The PHY initialization function. */
	__TMS_USBPUMP_USBPHY_INIT_FN *			pInitFn;

	/* The PHY configuration info */
	__TMS_CONST __TMS_USBPUMP_USBPHY_CONFIG_INFO *	pConfig;

	/* The PHY private configuration info */
	__TMS_CONST __TMS_VOID *			pPrivateConfig;

	/* The recommended debug flags */
	__TMS_UINT32					DebugFlags;
	};

/*

Name:  USBPUMP_USBPHY_INIT_NODE_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_USBPHY_INIT_NODE.

Definition:
	USBPUMP_USBPHY_INIT_NODE_INIT_V1(
		USBPUMP_USBPHY_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_USBPHY_INIT_FN *		pInitFn,
		CONST VOID *				pConfig,
		UINT32					DebugFlags
		)

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_USBPHY_INIT_NODEs.  It expands to whatever is
	the current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_USBPHY_INIT_NODE_INIT_V1(		\
		a_pProbeFn,				\
		a_pInitFn,				\
		a_pConfig,				\
		a_pPrivateConfig,			\
		a_DebugFlags				\
		)					\
		{					\
		(a_pProbeFn),				\
		(a_pInitFn),				\
		(a_pConfig),				\
		(a_pPrivateConfig),			\
		(a_DebugFlags)				\
		}

/*

Type:	USBPUMP_USBPHY_INIT_NODE_VECTOR

Function:
	Provides a wrapper for a vector of USBPUMP_USBPHY_INIT_NODEs.

Definition:
	The structure has the following contents:

		USBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN *pSetupFn OPTIONAL;
		USBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN *pFinishFn OPTIONAL;
		CONST USBPUMP_USBPHY_INIT_NODE	*	pVector;
		BYTES					nVector;

Description:
	pVector points to the base, and nVector states the number of elements,
	of the vector of USBPUMP_USBPHY_INIT_NODEs which list the PHYs
	which are to be probed at startup.

	pSetupFn and pFinishFn optionally allow the designer to proved a wrapper
	for this initialization.

See also:
	UsbPump_GenericPhyInit().

*/

struct __TMS_STRUCTNAME(USBPUMP_USBPHY_INIT_NODE_VECTOR)
	{
	/* an optional function that is called before we start */
	__TMS_USBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN *	pSetupFn;

	/* an optional function that is called when we are done */
	__TMS_USBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN *	pFinishFn;

	/* the vector and its length */
	__TMS_CONST __TMS_USBPUMP_USBPHY_INIT_NODE *		pVector;
	__TMS_UINT						nVector;
	};

/*

Name:  USBPUMP_USBPHY_INIT_NODE_VECTOR_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_USBPHY_INIT_NODE_VECTOR.

Definition:
	USBPUMP_USBPHY_INIT_NODE_VECTOR_INIT_V1(
		USBPUMP_USBPHY_INIT_NODE VectorName[],
		USBPUMP_USBPHY_INIT_NODE_VECTOR_SETUP_FN *pSetup,
		USBPUMP_USBPHY_INIT_NODE_VECTOR_FINISH_FN *pFinish
		);

Description:
	This macro is used when initializing a
	USBPUMP_USBPHY_INIT_NODE_VECTOR at compile time. It expands
	to whatever is the current proper value for statically initializing
	such an element, inserting proper defaults for any entries that
	may have been added after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_USBPHY_INIT_NODE_VECTOR_INIT_V1(	\
	VectorName,						\
	pSetup,							\
	pFinish							\
	)							\
	{							\
	(pSetup),						\
	(pFinish),						\
	(VectorName),						\
	sizeof(VectorName) / sizeof(VectorName[0])		\
	}


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_USBPUMP_USBPHY_INIT_NODE_VECTOR
	gk_UsbPumpPhy_GenericInitVector;

__TMS_VOID
UsbPump_GenericPhyInit(
	__TMS_UPLATFORM *	/* pPlatform */,
	__TMS_CONST __TMS_USBPUMP_USBPHY_INIT_NODE_VECTOR * /*pHostInitVector */
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBPHY_INIT_NODE_INIT_V1(		\
		a_pProbeFn,				\
		a_pInitFn,				\
		a_pConfig,				\
		a_pPrivateConfig,			\
		a_DebugFlags				\
		)	\
	__TMS_USBPUMP_USBPHY_INIT_NODE_INIT_V1(		\
		a_pProbeFn,				\
		a_pInitFn,				\
		a_pConfig,				\
		a_pPrivateConfig,			\
		a_DebugFlags				\
		)
# define USBPUMP_USBPHY_INIT_NODE_VECTOR_INIT_V1(	\
	VectorName,						\
	pSetup,							\
	pFinish							\
	)	\
	__TMS_USBPUMP_USBPHY_INIT_NODE_VECTOR_INIT_V1(	\
	VectorName,						\
	pSetup,							\
	pFinish							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbphy_init.h ****/
#endif /* _USBPUMP_USBPHY_INIT_H_ */
