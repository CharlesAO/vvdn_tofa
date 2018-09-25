/* usbpump_host_init.h	Tue Feb 28 2017 12:32:10 chwon */

/*

Module:  usbpump_host_init.h

Function:
	General-purpose initialization discipline for use by USB DataPump
	host applications.

Version:
	V3.19b	Tue Feb 28 2017 12:32:10 chwon	Edit level 8

Copyright notice:
	This file copyright (C) 2008-2010, 2014, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	July 2008

Revision history:
   1.97k  Tue Jul 08 2008 14:08:05 chwon
	4286: Module created.

   1.97k  Tue Jul 22 2008 18:58:06  chwon
	4286: add default linkage declaration

   1.97k  Sun Aug 31 2008 14:56:05  djt
	6427: Removed usbpump_usbdi_tt.h.

   2.01a  Thu Mar 05 2009 10:02:33  djt
	7416: Added SuperSpeed configuration.

   2.01a  Mon Jul 13 2009 10:50:58  chwon
	8436: Fixed documentation.

   3.01a  Sun Jan 03 2010 22:10:42  chwon
	9682: Added fDoNotStartUsbdi parameter in USBPUMP_HOST_INIT_NODE

   3.13b  Sat Sep 27 2014 16:20:14  tmm
	18362: change UsbPump_GenericHostInit() to return number of USBDs
	created.

   3.19b  Tue Feb 28 2017 12:32:11  chwon
	20500: Add StartUsbdiTimerValue in the USBPUMP_HOST_INIT_NODE.

*/

#ifndef _USBPUMP_HOST_INIT_H_		/* prevent multiple includes */
#define _USBPUMP_HOST_INIT_H_

#ifndef _USBPUMP_USBDI_INIT_H_
# include "usbpump_usbdi_init.h"
#endif

#ifndef _USBPUMP_HCD_H_
# include "usbpump_hcd.h"
#endif

#ifndef _UPLATFORM_H_
# include "uplatform.h"
#endif

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_HOST_DRIVER_CLASS_INIT_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR);

__TMS_TYPE_DEF_STRUCT(USBPUMP_HOST_HCD_INIT_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_HOST_HCD_INIT_NODE_VECTOR);

__TMS_TYPE_DEF_STRUCT(USBPUMP_HOST_INIT_NODE);
__TMS_TYPE_DEF_STRUCT(USBPUMP_HOST_INIT_NODE_VECTOR);


/****************************************************************************\
|
|	The Driver Class Initialzation Vector
|
\****************************************************************************/

/*

Type:	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN

Function:
	Abstract type for the "setup" function entry in a
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR.

Definition:
	typedef VOID *
		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN(
			CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader
			);

	typedef USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN
		*PUSBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN;

Description:
	Each USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR instance may optionally
	supply a setup function pointer.  This function is called prior to
	running the initialization loop on the body of the driver class init
	vector.

	Examples of activities that this function might perform are:
	allocating a block of memory to be used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "setup" functions should be
	prototyped using this type, by writing:

		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN MyFunction;

	rather than by writing:

		VOID *MyFunction(
			CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader
			);

Returns:
	The explicit result of this function should be pDriverClassInitContext.
	The result is passed (unchanged) to all the probe and setup functions.

*/

/*
|| The setup function is called so that the configurator can do some
|| "pre-setup", including prompting for other things to do.  It is optional.
|| The result is passed (unchanged) to all the probe and setup functions.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN,
__TMS_VOID *,	/* pDriverClassInitContext */
	(
	__TMS_CONST __TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR * /*pHdr*/,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */
	));

/*

Type:	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN

Function:
	Abstract type for the "finish" function entry in a
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR.

Definition:
	typedef VOID
		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN(
			CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pDriverClassInitContext,
			UINT nDriverClass
			);

	typedef USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN
		*PUSBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN;

Description:
	Each USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR instance may optionally
	supply a "finish" function pointer.  This function is called after
	running the initialization loop on the body of the driver class init
	vector.

	pDriverClassInitContext is the value returned previously by the SETUP
	function.  If no SETUP function was provided, pDriverClassInitContext
	will be NULL.

	Examples of activities that this function might perform are:
	freeing a block of memory used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "finish" functions should be
	prototyped using this type, by writing:

		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN MyFunction;

	rather than by writing:

		VOID MyFunction(
			CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pDriverClassInitContext,
			UINT nDriverClass
			);

Returns:
	Nothing

*/

/*
|| The finish function is called so that the application can do some
|| "post-setup", including prompting for other things to do.  It is optional.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN,
__TMS_VOID,
	(
	__TMS_CONST __TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */,
	__TMS_VOID * /* pDriverClassInitContext */,
	__TMS_UINT /* nDriverClass */
	));

/*

Type:	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN

Function:
	Abstract type for the "probe" function entry in a
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE.

Definition:
	typedef BOOL USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN(
			CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pDriverClassInitContext
			);

	typedef USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN
		*PUSBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN;

Description:
	Each USBPUMP_HOST_DRIVER_CLASS_INIT_NODE instance may optionally
	supply a "probe" function pointer.

	pNode points to the USBPUMP_HOST_DRIVER_CLASS_INIT_NODE in question.

	pObjectHeader is the pointer to the DataPump object.

	pDriverClassInitContext is the value returned previously by the
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR's "setup" function.  If no
	SETUP function was provided, then pDriverClassInitContext
	will be NULL.

	Functions which are to be used as "probe" functions should be
	prototyped using this type, by writing:

		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN MyProbeFunction;

	rather than by writing:

		BOOL MyProbeFunction(
			CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pDriverClassInitContext
			);

Returns:
	If the probe function returns FALSE, then UsbPump_GenericHostInit()
	will not attempt to call the create function for pNode.  On the other
	hand, if the probe function returns TRUE, then
	UsbPump_GenericHostInit() will call the create function for pNode
	to create driver class instance.

*/


/*
|| The driver class probe function, if present, is called prior to
|| creating the driver class instance, so that it can decide whether or not to
|| init.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN,
__TMS_BOOL, (
	__TMS_CONST __TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */,
	__TMS_VOID * /* pDriverClassInitContext */
	));

/*

Type:	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE

Function:
	C structure, representing a single driver class to be created.

Contents:
	The structure has the following contents:

	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN *pProbeFn OPTIONAL;
		If non-NULL, the function to be called to further qualify
		the class protocol.

	USBPUMP_USBDI_DRIVER_CLASS_INIT_FN *pInitFn;
		The (mandatory) function to be called to create a driver class
		instance.

	CONST USBPUMP_USBDI_DRIVER_CLASS_CONFIG *pConfig;
		The (mandatory) driver class configuration information.

	CONST VOID *pPrivateConfig OPTIONAL;
		Optional information for the use of the driver class functions.

	UINT32 DebugFlags;
		The recommended debug flags.

Description:
	Each USBPUMP_HOST_DRIVER_CLASS_INIT_NODE represents one potential
	binding for a class protocol.

	pProbeFn allows the designer to add an algorithmic filter;
	pPrivateConfig allows the designer to pass arbitrary information from
	the USBPUMP_HOST_DRIVER_CLASS_INIT_NODE to the creationt function.

	pInitFn is a pointer to the function that will create the driver class
	instance.  It makes no sense to omit this, but
	UsbPump_GenericHostInit() will not dereference this pointer if
	it is in fact coded as NULL.

*/

struct __TMS_STRUCTNAME(USBPUMP_HOST_DRIVER_CLASS_INIT_NODE)
	{
	/* The optional HCD probe function. */
	__TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN *	pProbeFn;

	/* The driver class initialzation function */
	__TMS_USBPUMP_USBDI_DRIVER_CLASS_INIT_FN *		pInitFn;

	/* The driver class configuration info */
	__TMS_CONST __TMS_USBPUMP_USBDI_DRIVER_CLASS_CONFIG *	pConfig;

	/* An optional driver class private configuration info */
	__TMS_CONST __TMS_VOID *				pPrivateConfig;

	/* The recommended debug flags */
	__TMS_UINT32						DebugFlags;
	};

/*

Name:  USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE.

Definition:
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_INIT_V1(
		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_USBDI_DRIVER_CLASS_INIT_FN *		pInitFn,
		CONST USBPUMP_USBDI_DRIVER_CLASS_CONFIG *	pConfig,
		CONST VOID *					pPrivateConfig,
		UINT32						DebugFlags
		)

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_HOST_DRIVER_CLASS_INIT_NODEs.  It expands to whatever is
	the current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_INIT_V1(	\
		a_pProbeFn,					\
		a_pInitFn,					\
		a_pConfig,					\
		a_pPrivateConfig,				\
		a_DebugFlags					\
		)						\
		{						\
		(a_pProbeFn),					\
		(a_pInitFn),					\
		(a_pConfig),					\
		(a_pPrivateConfig),				\
		(a_DebugFlags)					\
		}

/*

Type:	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR

Function:
	Provides a wrapper for a vector of USBPUMP_HOST_DRIVER_CLASS_INIT_NODEs.

Definition:
	The structure has the following contents:

		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN *pSetupFn;
		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN *pFinishFn;
		CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE	 *pVector;
		BYTES						 nVector;

Description:
	pVector points to the base, and nVector states the number of elements,
	of the vector of USBPUMP_HOST_DRIVER_CLASS_INIT_NODEs which list the
	driver class which are to be probed at startup.

	pSetupFn and pFinishFn optionally allow the designer to proved a wrapper
	for this initialization.

See also:
	UsbPump_GenericHostInit().

*/

struct __TMS_STRUCTNAME(USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR)
	{
	/* an optional function that is called before we start */
	__TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN *pSetupFn;

	/* an optional function that is called when we are done */
	__TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN *pFinishFn;

	/* the vector and its length */
	__TMS_CONST __TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE *	pVector;
	__TMS_UINT						nVector;
	};

/*

Name:  USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR.

Definition:
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_INIT_V1(
		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE VectorName[],
		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_SETUP_FN *pSetup,
		USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_FINISH_FN *pFinish
		);

Description:
	This macro is used when initializing a
	USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR at compile time. It expands
	to whatever is the current proper value for statically initializing
	such an element, inserting proper defaults for any entries that
	may have been added after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_INIT_V1(	\
	VectorName,							\
	pSetup,								\
	pFinish								\
	)								\
	{								\
	(pSetup),							\
	(pFinish),							\
	(VectorName),							\
	sizeof(VectorName) / sizeof(VectorName[0])			\
	}

/****************************************************************************\
|
|	The HCD Initialzation Vector
|
\****************************************************************************/

/*

Type:	USBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN

Function:
	Abstract type for the "setup" function entry in a
	USBPUMP_HOST_HCD_INIT_NODE_VECTOR.

Definition:
	typedef VOID *
		USBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN(
			CONST USBPUMP_HOST_HCD_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader
			);

	typedef USBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN
		*PUSBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN;

Description:
	Each USBPUMP_HOST_HCD_INIT_NODE_VECTOR instance may optionally
	supply a setup function pointer.  This function is called prior to
	running the initialization loop on the body of the HCD init
	vector.

	Examples of activities that this function might perform are:
	allocating a block of memory to be used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "setup" functions should be
	prototyped using this type, by writing:

		USBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN MyFunction;

	rather than by writing:

		VOID *MyFunction(
			CONST USBPUMP_HOST_HCD_INIT_NODE_VECTOR *pHdr,
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
USBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN,
__TMS_VOID *,	/* pHcdInitContext */
	(
	__TMS_CONST __TMS_USBPUMP_HOST_HCD_INIT_NODE_VECTOR * /*pHdr*/,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */
	));

/*

Type:	USBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN

Function:
	Abstract type for the "finish" function entry in a
	USBPUMP_HOST_HCD_INIT_NODE_VECTOR.

Definition:
	typedef VOID
		USBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN(
			CONST USBPUMP_HOST_HCD_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pHcdInitContext,
			UINT nHcd
			);

	typedef USBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN
		*PUSBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN;

Description:
	Each USBPUMP_HOST_HCD_INIT_NODE_VECTOR instance may optionally
	supply a "finish" function pointer.  This function is called after
	running the initialization loop on the body of the HCD init
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

		USBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN MyFunction;

	rather than by writing:

		VOID MyFunction(
			CONST USBPUMP_HOST_HCD_INIT_NODE_VECTOR *pHdr,
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
USBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN,
__TMS_VOID,
	(
	__TMS_CONST __TMS_USBPUMP_HOST_HCD_INIT_NODE_VECTOR *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */,
	__TMS_VOID * /* pHcdInitContext */,
	__TMS_UINT /* nHcd */
	));

/*

Type:	USBPUMP_HOST_HCD_INIT_NODE_PROBE_FN

Function:
	Abstract type for the "probe" function entry in a
	USBPUMP_HOST_HCD_INIT_NODE.

Definition:
	typedef BOOL USBPUMP_HOST_HCD_INIT_NODE_PROBE_FN(
			CONST USBPUMP_HOST_HCD_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pHcdInitContext
			);

	typedef USBPUMP_HOST_HCD_INIT_NODE_PROBE_FN
		*PUSBPUMP_HOST_HCD_INIT_NODE_PROBE_FN;

Description:
	Each USBPUMP_HOST_HCD_INIT_NODE instance may optionally
	supply a "probe" function pointer.

	pNode points to the USBPUMP_HOST_HCD_INIT_NODE in question.

	pObjectHeader is the pointer to the DataPump object.

	pHcdInitContext is the value returned previously by the
	USBPUMP_HOST_HCD_INIT_NODE_VECTOR's "setup" function.  If no
	SETUP function was provided, then pHcdInitContext
	will be NULL.

	Functions which are to be used as "probe" functions should be
	prototyped using this type, by writing:

		USBPUMP_HOST_HCD_INIT_NODE_PROBE_FN MyProbeFunction;

	rather than by writing:

		BOOL MyProbeFunction(
			CONST USBPUMP_HOST_HCD_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pHcdInitContext
			);

Returns:
	If the probe function returns FALSE, then
	UsbPump_GenericHostInit() will not attempt to call the
	create function for pNode.  On the other hand, if the probe function
	returns TRUE, then UsbPumpUsbd_CreateHostControlDrivers() will call
	the create function for pNode to create HCD instance.

*/


/*
|| The driver class probe function, if present, is called prior to
|| creating the driver class instance, so that it can decide whether or not to
|| init.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HOST_HCD_INIT_NODE_PROBE_FN,
__TMS_BOOL, (
	__TMS_CONST __TMS_USBPUMP_HOST_HCD_INIT_NODE *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */,
	__TMS_VOID * /* pHcdInitContext */
	));

/*

Type:	USBPUMP_HOST_HCD_INIT_NODE

Function:
	C structure, representing a single HCD to be created.

Contents:
	The structure has the following contents:

	USBPUMP_HOST_HCD_INIT_NODE_PROBE_FN *pProbeFn OPTIONAL;
		If non-NULL, the function to be called to further qualify
		the HCD.

	USBPUMP_HCD_INIT_FN *pInitFn;
		The (mandatory) function to be called to create a HCD
		instance.

	CONST VOID *pConfig;
		The (mandatory) HCD configuration information.

	UINT32 DebugFlags;
		The recommended debug flags.

Description:
	Each USBPUMP_HOST_HCD_INIT_NODE represents one potential
	binding for a HCD.

	pProbeFn allows the designer to add an algorithmic filter;

	pInitFn is a pointer to the function that will create the HCD
	instance.  It makes no sense to omit this, but
	UsbPump_GenericHostInit() will not dereference this
	pointer if it is in fact coded as NULL.

*/

struct __TMS_STRUCTNAME(USBPUMP_HOST_HCD_INIT_NODE)
	{
	/* The optional HCD probe function. */
	__TMS_USBPUMP_HOST_HCD_INIT_NODE_PROBE_FN *	pProbeFn;

	/* The HCD initialization function. */
	__TMS_USBPUMP_HCD_INIT_FN *			pInitFn;

	/* The HCD configuration info */
	__TMS_CONST __TMS_VOID *			pConfig;

	/* The recommended debug flags */
	__TMS_UINT32					DebugFlags;
	};

/*

Name:  USBPUMP_HOST_HCD_INIT_NODE_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_HCD_INIT_NODE.

Definition:
	USBPUMP_HOST_HCD_INIT_NODE_INIT_V1(
		USBPUMP_HOST_HCD_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_HCD_INIT_FN *			pInitFn,
		CONST VOID *				pConfig,
		UINT32					DebugFlags
		)

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_HOST_HCD_INIT_NODEs.  It expands to whatever is
	the current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_HCD_INIT_NODE_INIT_V1(		\
		a_pProbeFn,					\
		a_pInitFn,					\
		a_pConfig,					\
		a_DebugFlags					\
		)						\
		{						\
		(a_pProbeFn),					\
		(a_pInitFn),					\
		(a_pConfig),					\
		(a_DebugFlags)					\
		}

/*

Type:	USBPUMP_HOST_HCD_INIT_NODE_VECTOR

Function:
	Provides a wrapper for a vector of USBPUMP_HOST_HCD_INIT_NODEs.

Definition:
	The structure has the following contents:

		USBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN *pSetupFn OPTIONAL;
		USBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN *pFinishFn OPTIONAL;
		CONST USBPUMP_HOST_HCD_INIT_NODE	*pVector;
		BYTES					nVector;

Description:
	pVector points to the base, and nVector states the number of elements,
	of the vector of USBPUMP_HOST_HCD_INIT_NODEs which list the HCDs
	which are to be probed at startup.

	pSetupFn and pFinishFn optionally allow the designer to proved a wrapper
	for this initialization.

See also:
	UsbPump_GenericHostInit().

*/

struct __TMS_STRUCTNAME(USBPUMP_HOST_HCD_INIT_NODE_VECTOR)
	{
	/* an optional function that is called before we start */
	__TMS_USBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN *	pSetupFn;

	/* an optional function that is called when we are done */
	__TMS_USBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN *	pFinishFn;

	/* the vector and its length */
	__TMS_CONST __TMS_USBPUMP_HOST_HCD_INIT_NODE *		pVector;
	__TMS_UINT						nVector;
	};

/*

Name:  USBPUMP_HOST_HCD_INIT_NODE_VECTOR_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_HCD_INIT_NODE_VECTOR.

Definition:
	USBPUMP_HOST_HCD_INIT_NODE_VECTOR_INIT_V1(
		USBPUMP_HOST_HCD_INIT_NODE VectorName[],
		USBPUMP_HOST_HCD_INIT_NODE_VECTOR_SETUP_FN *pSetup,
		USBPUMP_HOST_HCD_INIT_NODE_VECTOR_FINISH_FN *pFinish
		);

Description:
	This macro is used when initializing a
	USBPUMP_HOST_HCD_INIT_NODE_VECTOR at compile time. It expands
	to whatever is the current proper value for statically initializing
	such an element, inserting proper defaults for any entries that
	may have been added after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_HCD_INIT_NODE_VECTOR_INIT_V1(	\
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
|	USBD initialization Vector
|
\****************************************************************************/

/*

Type:	USBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN

Function:
	Abstract type for the "setup" function entry in a
	USBPUMP_HOST_INIT_NODE_VECTOR.

Definition:
	typedef VOID *
		USBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN(
			CONST USBPUMP_HOST_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader
			);

	typedef USBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN
		*PUSBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN;

Description:
	Each USBPUMP_HOST_INIT_NODE_VECTOR instance may optionally
	supply a setup function pointer.  This function is called prior to
	running the initialization loop on the body of the USBD init
	vector.

	Examples of activities that this function might perform are:
	allocating a block of memory to be used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "setup" functions should be
	prototyped using this type, by writing:

		USBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN MyFunction;

	rather than by writing:

		VOID *MyFunction(
			CONST USBPUMP_HOST_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader
			);

Returns:
	The explicit result of this function should be pUsbdInitContext.
	The result is passed (unchanged) to all the probe and setup functions.

*/

/*
|| The setup function is called so that the configurator can do some
|| "pre-setup", including prompting for other things to do.  It is optional.
|| The result is passed (unchanged) to all the probe and setup functions.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN,
__TMS_VOID *,	/* pUsbdInitContext */
	(
	__TMS_CONST __TMS_USBPUMP_HOST_INIT_NODE_VECTOR * /*pHdr*/,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */
	));

/*

Type:	USBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN

Function:
	Abstract type for the "finish" function entry in a
	USBPUMP_HOST_INIT_NODE_VECTOR.

Definition:
	typedef VOID
		USBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN(
			CONST USBPUMP_HOST_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pUsbdInitContext,
			UINT nUsbd
			);

	typedef USBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN
		*PUSBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN;

Description:
	Each USBPUMP_HOST_INIT_NODE_VECTOR instance may optionally
	supply a "finish" function pointer.  This function is called after
	running the initialization loop on the body of the USBD init
	vector.

	pUsbdInitContext is the value returned previously by the SETUP
	function.  If no SETUP function was provided, pUsbdInitContext
	will be NULL.

	Examples of activities that this function might perform are:
	freeing a block of memory used as context by the other
	functions.  In fact, this function is expected to be used only
	rarely.

	Functions which are to be used as "finish" functions should be
	prototyped using this type, by writing:

		USBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN MyFunction;

	rather than by writing:

		VOID MyFunction(
			CONST USBPUMP_HOST_INIT_NODE_VECTOR *pHdr,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pUsbdInitContext,
			UINT nUsbd
			);

Returns:
	Nothing.

*/

/*
|| The finish function is called so that the application can do some
|| "post-setup", including prompting for other things to do.  It is optional.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN,
__TMS_VOID,
	(
	__TMS_CONST __TMS_USBPUMP_HOST_INIT_NODE_VECTOR *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */,
	__TMS_VOID * /* pUsbdInitContext */,
	__TMS_UINT /* nUsbd */
	));

/*

Type:	USBPUMP_HOST_INIT_NODE_PROBE_FN

Function:
	Abstract type for the "probe" function entry in a
	USBPUMP_HOST_INIT_NODE.

Definition:
	typedef BOOL USBPUMP_HOST_INIT_NODE_PROBE_FN(
			CONST USBPUMP_HOST_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pUsbdInitContext
			);

	typedef USBPUMP_HOST_INIT_NODE_PROBE_FN
		*PUSBPUMP_HOST_INIT_NODE_PROBE_FN;

Description:
	Each USBPUMP_HOST_INIT_NODE instance may optionally
	supply a "probe" function pointer.

	pNode points to the USBPUMP_HOST_INIT_NODE in question.

	pObjectHeader is the pointer to the DataPump object.

	pUsbdInitContext is the value returned previously by the
	USBPUMP_HOST_INIT_NODE_VECTOR's "setup" function.  If no
	SETUP function was provided, then pUsbdInitContext
	will be NULL.

	Functions which are to be used as "probe" functions should be
	prototyped using this type, by writing:

		USBPUMP_HOST_INIT_NODE_PROBE_FN MyProbeFunction;

	rather than by writing:

		BOOL MyProbeFunction(
			CONST USBPUMP_HOST_INIT_NODE *pNode,
			USBPUMP_OBJECT_HEADER *pObjectHeader,
			VOID *pUsbdInitContext
			);

Returns:
	If the probe function returns FALSE, then
	UsbPump_GenericHostInit() will not attempt to call the
	create function for pNode.  On the other hand, if the probe function
	returns TRUE, then UsbPump_GenericHostInit() will call
	the create function for pNode to create HCD instance.

*/


/*
|| The driver class probe function, if present, is called prior to
|| creating the driver class instance, so that it can decide whether or not to
|| init.
*/
__TMS_FNTYPE_DEF(
USBPUMP_HOST_INIT_NODE_PROBE_FN,
__TMS_BOOL, (
	__TMS_CONST __TMS_USBPUMP_HOST_INIT_NODE *,
	__TMS_USBPUMP_OBJECT_HEADER * /* pObjectHeader */,
	__TMS_VOID * /* pUsbdInitContext */
	));

/*

Type:	USBPUMP_HOST_INIT_NODE

Function:
	C structure, representing a single USBD to be created.

Contents:
	The structure has the following contents:

	USBPUMP_HOST_INIT_NODE_PROBE_FN *pProbeFn OPTIONAL;
		If non-NULL, the function to be called to further qualify
		the USBD.

	USBPUMP_USBDI_USBD_INIT_FN *pUsbdInitFn;
		The (mandatory) function to be called to create a USBD
		instance.

	CONST USBPUMP_USBDI_USBD_CONFIG *pUsbdConfig;
		The (mandatory) USBD configuration information.

	CONST USBPUMP_USBDI_USBD_IMPLEMENTATION *pUsbdImplementation;
		The (mandatory) USBD implementation information.

	USBPUMP_USBDI_USBDTT_INIT_FN *pUsbdTTInitFn OPTIONAL;
		The optional function to be called to initialize the
		transaction translator portion of the USBD.

	CONST USBPUMP_USBDI_USBDTT_CONFIG *pUsbdTTConfig OPTIONAL;
		The optional USBD TT configuration information.

	USBPUMP_USBDI_USBDWIRELESS_INIT_FN *pUsbdWirelessInitFn OPTIONAL;
		The optional function to be called to initialize the wireless
		portion of the USBD.

	CONST USBPUMP_USBDI_USBDWIRELESS_CONFIG *pUsbdWirelessConfig OPTIONAL;
		The optional USBD wireless configuration information.


	USBPUMP_USBDI_USBDSUPERSPEED_INIT_FN *pUsbdSuperSpeedInitFn OPTIONAL;
		The optional function to be called to initialize the SuperSpeed
		portion of the USBD.

	CONST USBPUMP_USBDI_USBDSUPERSPEED_CONFIG *pUsbdSuperSpeedConfig OPTIONAL;
		The optional USBD SuperSpeed configuration information.

	UINT32 DebugFlags;
		The recommended debug flags.

	BOOL fDoNotStartUsbdi;
		If fDoNotStartUsbdi is TRUE, generic host initialization will
		not call USBPUMP_IOCTL_USBDI_START ioctl.  If this is TRUE,
		client should have to call USBPUMP_IOCTL_USBDI_START ioctl
		after DataPump initialization is done.

	UINT16 StartUsbdiTimerValue;
		The generic host initialization will use timer to call
		USBPUMP_IOCTL_USBDI_START ioctl after initialization done.
		This value is only valid if fDoNotStartUsbdi is FALSE.

Description:
	Each USBPUMP_HOST_INIT_NODE represents one potential
	binding for a USBD.

	pProbeFn allows the designer to add an algorithmic filter;

	pUsbdInitFn is a pointer to the function that will create the USBD
	instance.  It makes no sense to omit this, but
	UsbPump_GenericHostInit() will not dereference this
	pointer if it is in fact coded as NULL.

*/

struct __TMS_STRUCTNAME(USBPUMP_HOST_INIT_NODE)
	{
	/* The optional USBD probe function. */
	__TMS_USBPUMP_HOST_INIT_NODE_PROBE_FN *	pProbeFn;

	/* The USBD initialization function. */
	__TMS_USBPUMP_USBDI_USBD_INIT_FN *		pUsbdInitFn;

	/* The USBD configuration info */
	__TMS_CONST __TMS_USBPUMP_USBDI_USBD_CONFIG *	pUsbdConfig;
	__TMS_CONST __TMS_USBPUMP_USBDI_USBD_IMPLEMENTATION *
							pUsbdImplementation;

	/* The optional USBD TT initialization function. */
	__TMS_USBPUMP_USBDI_USBDTT_INIT_FN *		pUsbdTTInitFn;

	/* The optional USBD TT configuration info */
	__TMS_CONST __TMS_USBPUMP_USBDI_USBDTT_CONFIG *	pUsbdTTConfig;

	/* The optional USBD Wireless initialization function. */
	__TMS_USBPUMP_USBDI_USBDWIRELESS_INIT_FN *	pUsbdWirelessInitFn;

	/* The optional USBD Wireless configuration info */
	__TMS_CONST __TMS_VOID *			pUsbdWirelessConfig;

	/* The optional USBD SuperSpeed initialization function. */
	__TMS_USBPUMP_USBDI_USBDSUPERSPEED_INIT_FN *	pUsbdSuperSpeedInitFn;

	/* The optional USBD SuperSpeed configuration info */
	__TMS_CONST __TMS_VOID *			pUsbdSuperSpeedConfig;

	/* The recommended debug flags for USBD */
	__TMS_UINT32					DebugFlags;

	__TMS_BOOL					fDoNotStartUsbdi;

	__TMS_UINT16					StartUsbdiTimerValue;

	/*
	|| HCD initialization vector
	*/
	__TMS_CONST __TMS_USBPUMP_HOST_HCD_INIT_NODE_VECTOR *
							pHcdInitVector;
	/*
	|| Driver Class initialization vector
	*/
	__TMS_CONST __TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *
							pDriverClassInitVector;
	};

/*

Name:  USBPUMP_HOST_INIT_NODE_INIT_V4()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_INIT_NODE.

Definition:
	USBPUMP_HOST_INIT_NODE_INIT_V4(
		USBPUMP_HOST_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_USBDI_USBD_INIT_FN *		pUsbdInitFn,
		CONST VOID *				pUsbdConfig,
		CONST VOID *				pUsbdImplementation,
		USBPUMP_USBDI_USBDTT_INIT_FN *		pUsbdTTInitFn,
		CONST VOID *				pUsbdTTConfig,
		USBPUMP_USBDI_USBDWIRELESS_INIT_FN *	pUsbdWirelessInitFn,
		CONST VOID *				pUsbdWirelessConfig,
		USBPUMP_USBDI_USBDSUPERSPEED_INIT_FN *	pUsbdSuperSpeedInitFn,
		CONST VOID *				pUsbdSuperSpeedConfig,
		UINT32					DebugFlags,
		BOOL					fDoNotStartUsbdi,
		UINT16					StartUsbdiTimerValue,
		CONST USBPUMP_HOST_HCD_INIT_NODE_VECTOR *pHcdInitVector,
		CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *
							pDriverClassInitVector
		);

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_HOST_INIT_NODEs.  It expands to whatever is
	the current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_INIT_NODE_INIT_V4(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_fDoNotStartUsbdi,				\
		a_StartUsbdiTimerValue,				\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)						\
		{						\
		(a_pProbeFn),					\
		(a_pUsbdInitFn),				\
		(a_pUsbdConfig),				\
		(a_pUsbdImplementation),			\
		(a_pUsbdTTInitFn),				\
		(a_pUsbdTTConfig),				\
		(a_pUsbdWirelessInitFn),			\
		(a_pUsbdWirelessConfig),			\
		(a_pUsbdSuperSpeedInitFn),			\
		(a_pUsbdSuperSpeedConfig),			\
		(a_DebugFlags),					\
		(a_fDoNotStartUsbdi),				\
		(a_StartUsbdiTimerValue),			\
		(a_pHcdInitVector),				\
		(a_pDriverClassInitVector)			\
		}

/*

Name:  USBPUMP_HOST_INIT_NODE_INIT_V3()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_INIT_NODE.

Definition:
	USBPUMP_HOST_INIT_NODE_INIT_V3(
		USBPUMP_HOST_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_USBDI_USBD_INIT_FN *		pUsbdInitFn,
		CONST VOID *				pUsbdConfig,
		CONST VOID *				pUsbdImplementation,
		USBPUMP_USBDI_USBDTT_INIT_FN *		pUsbdTTInitFn,
		CONST VOID *				pUsbdTTConfig,
		USBPUMP_USBDI_USBDWIRELESS_INIT_FN *	pUsbdWirelessInitFn,
		CONST VOID *				pUsbdWirelessConfig,
		USBPUMP_USBDI_USBDSUPERSPEED_INIT_FN *	pUsbdSuperSpeedInitFn,
		CONST VOID *				pUsbdSuperSpeedConfig,
		UINT32					DebugFlags,
		BOOL					fDoNotStartUsbdi,
		CONST USBPUMP_HOST_HCD_INIT_NODE_VECTOR *pHcdInitVector,
		CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *
							pDriverClassInitVector
		);

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_HOST_INIT_NODEs.  It expands to whatever is
	the current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_INIT_NODE_INIT_V3(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_fDoNotStartUsbdi,				\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)						\
	__TMS_USBPUMP_HOST_INIT_NODE_INIT_V4(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_fDoNotStartUsbdi,				\
		/* V4: StartUsbdiTimerValue */ 300,		\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)

/*

Name:  USBPUMP_HOST_INIT_NODE_INIT_V2()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_INIT_NODE.

Definition:
	USBPUMP_HOST_INIT_NODE_INIT_V2(
		USBPUMP_HOST_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_USBDI_USBD_INIT_FN *		pUsbdInitFn,
		CONST VOID *				pUsbdConfig,
		CONST VOID *				pUsbdImplementation,
		USBPUMP_USBDI_USBDTT_INIT_FN *		pUsbdTTInitFn,
		CONST VOID *				pUsbdTTConfig,
		USBPUMP_USBDI_USBDWIRELESS_INIT_FN *	pUsbdWirelessInitFn,
		CONST VOID *				pUsbdWirelessConfig,
		USBPUMP_USBDI_USBDSUPERSPEED_INIT_FN *	pUsbdSuperSpeedInitFn,
		CONST VOID *				pUsbdSuperSpeedConfig,
		UINT32					DebugFlags,
		CONST USBPUMP_HOST_HCD_INIT_NODE_VECTOR *pHcdInitVector,
		CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *
							pDriverClassInitVector
		);

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_HOST_INIT_NODEs.  It expands to whatever is
	the current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_INIT_NODE_INIT_V2(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)						\
	__TMS_USBPUMP_HOST_INIT_NODE_INIT_V3(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		/* V3: fDoNotStartUsbdi */ __TMS_FALSE,		\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)

/*

Name:  USBPUMP_HOST_INIT_NODE_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_INIT_NODE.

Definition:
	USBPUMP_HOST_INIT_NODE_INIT_V2(
		USBPUMP_HOST_INIT_NODE_PROBE_FN *	pProbeFn,
		USBPUMP_USBDI_USBD_INIT_FN *		pUsbdInitFn,
		CONST VOID *				pUsbdConfig,
		CONST VOID *				pUsbdImplementation,
		USBPUMP_USBDI_USBDTT_INIT_FN *		pUsbdTTInitFn,
		CONST VOID *				pUsbdTTConfig,
		USBPUMP_USBDI_USBDWIRELESS_INIT_FN *	pUsbdWirelessInitFn,
		CONST VOID *				pUsbdWirelessConfig,
		UINT32					DebugFlags,
		CONST USBPUMP_HOST_HCD_INIT_NODE_VECTOR *pHcdInitVector,
		CONST USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR *
							pDriverClassInitVector
		);

Description:
	This macro is used when initializing a single element of a vector
	of USBPUMP_HOST_INIT_NODEs.  It expands to whatever is
	the current proper value for statically initializing such a node,
	inserting proper defaults for any entries that may have been added
	after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_INIT_NODE_INIT_V1(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_DebugFlags,					\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)						\
	__TMS_USBPUMP_HOST_INIT_NODE_INIT_V2(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		/* V2: SuperSpeedInitFn */ __TMS_NULL,		\
		/* V2: SuperSpeedConfig */ __TMS_NULL,		\
		a_DebugFlags,					\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)						

/*

Type:	USBPUMP_HOST_INIT_NODE_VECTOR

Function:
	Provides a wrapper for a vector of USBPUMP_HOST_INIT_NODEs.

Definition:
	The structure has the following contents:

		USBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN *pSetupFn OPTIONAL;
		USBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN *pFinishFn OPTIONAL;
		CONST USBPUMP_HOST_INIT_NODE	*pVector;
		BYTES					nVector;

Description:
	pVector points to the base, and nVector states the number of elements,
	of the vector of USBPUMP_HOST_INIT_NODEs which list the USBDs
	which are to be probed at startup.

	pSetupFn and pFinishFn optionally allow the designer to proved a wrapper
	for this initialization.

See also:
	UsbPump_GenericHostInit().

*/

struct __TMS_STRUCTNAME(USBPUMP_HOST_INIT_NODE_VECTOR)
	{
	/* an optional function that is called before we start */
	__TMS_USBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN *	pSetupFn;

	/* an optional function that is called when we are done */
	__TMS_USBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN *	pFinishFn;

	/* the vector and its length */
	__TMS_CONST __TMS_USBPUMP_HOST_INIT_NODE *	pVector;
	__TMS_UINT					nVector;
	};

/*

Name:  USBPUMP_HOST_INIT_NODE_VECTOR_INIT_V1()

Function:
	Macro that expands to the static C initializer for a
	USBPUMP_HOST_INIT_NODE_VECTOR.

Definition:
	USBPUMP_HOST_INIT_NODE_VECTOR_INIT_V1(
		USBPUMP_HOST_INIT_NODE VectorName[],
		USBPUMP_HOST_INIT_NODE_VECTOR_SETUP_FN *pSetup,
		USBPUMP_HOST_INIT_NODE_VECTOR_FINISH_FN *pFinish
		);

Description:
	This macro is used when initializing a
	USBPUMP_HOST_INIT_NODE_VECTOR at compile time. It expands
	to whatever is the current proper value for statically initializing
	such an element, inserting proper defaults for any entries that
	may have been added after the _V1() version of this macro was defined.

Expands To:
	'{', followed by the properly parenthesized initialization sequence,
	followed by '}'.  Should normally be followed by a ',' or a close
	'}', to terminate the initializer.

*/

#define __TMS_USBPUMP_HOST_INIT_NODE_VECTOR_INIT_V1(		\
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

extern __TMS_CONST __TMS_USBPUMP_HOST_HCD_INIT_NODE_VECTOR
	gk_UsbPumpHcd_GenericInitVector;		/* Default linkage */

extern __TMS_CONST __TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR
	gk_UsbPumpDriverClass_GenericInitVector;	/* Default linkage */

extern __TMS_CONST __TMS_USBPUMP_HOST_INIT_NODE_VECTOR
	gk_UsbPumpHost_GenericInitVector;		/* Default linkage */

__TMS_UINT
UsbPump_GenericHostInit(
	__TMS_UPLATFORM *pPlatform,
	__TMS_CONST __TMS_USBPUMP_HOST_INIT_NODE_VECTOR *pHostInitVector
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_INIT_V1(	\
		a_pProbeFn,					\
		a_pInitFn,					\
		a_pConfig,					\
		a_pPrivateConfig,				\
		a_DebugFlags					\
		)	\
	__TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_INIT_V1(	\
		a_pProbeFn,					\
		a_pInitFn,					\
		a_pConfig,					\
		a_pPrivateConfig,				\
		a_DebugFlags					\
		)
# define USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_INIT_V1(	\
	VectorName,							\
	pSetup,								\
	pFinish								\
	)	\
	__TMS_USBPUMP_HOST_DRIVER_CLASS_INIT_NODE_VECTOR_INIT_V1(	\
	VectorName,							\
	pSetup,								\
	pFinish								\
	)
# define USBPUMP_HOST_HCD_INIT_NODE_INIT_V1(		\
		a_pProbeFn,					\
		a_pInitFn,					\
		a_pConfig,					\
		a_DebugFlags					\
		)	\
	__TMS_USBPUMP_HOST_HCD_INIT_NODE_INIT_V1(		\
		a_pProbeFn,					\
		a_pInitFn,					\
		a_pConfig,					\
		a_DebugFlags					\
		)
# define USBPUMP_HOST_HCD_INIT_NODE_VECTOR_INIT_V1(	\
	VectorName,						\
	pSetup,							\
	pFinish							\
	)	\
	__TMS_USBPUMP_HOST_HCD_INIT_NODE_VECTOR_INIT_V1(	\
	VectorName,						\
	pSetup,							\
	pFinish							\
	)
# define USBPUMP_HOST_INIT_NODE_INIT_V4(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_fDoNotStartUsbdi,				\
		a_StartUsbdiTimerValue,				\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)	\
	__TMS_USBPUMP_HOST_INIT_NODE_INIT_V4(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_fDoNotStartUsbdi,				\
		a_StartUsbdiTimerValue,				\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)
# define USBPUMP_HOST_INIT_NODE_INIT_V3(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_fDoNotStartUsbdi,				\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)	\
	__TMS_USBPUMP_HOST_INIT_NODE_INIT_V3(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_fDoNotStartUsbdi,				\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)
# define USBPUMP_HOST_INIT_NODE_INIT_V2(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)	\
	__TMS_USBPUMP_HOST_INIT_NODE_INIT_V2(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_pUsbdSuperSpeedInitFn,			\
		a_pUsbdSuperSpeedConfig,			\
		a_DebugFlags,					\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)
# define USBPUMP_HOST_INIT_NODE_INIT_V1(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_DebugFlags,					\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)	\
	__TMS_USBPUMP_HOST_INIT_NODE_INIT_V1(			\
		a_pProbeFn,					\
		a_pUsbdInitFn,					\
		a_pUsbdConfig,					\
		a_pUsbdImplementation,				\
		a_pUsbdTTInitFn,				\
		a_pUsbdTTConfig,				\
		a_pUsbdWirelessInitFn,				\
		a_pUsbdWirelessConfig,				\
		a_DebugFlags,					\
		a_pHcdInitVector,				\
		a_pDriverClassInitVector			\
		)
# define USBPUMP_HOST_INIT_NODE_VECTOR_INIT_V1(		\
	VectorName,						\
	pSetup,							\
	pFinish							\
	)	\
	__TMS_USBPUMP_HOST_INIT_NODE_VECTOR_INIT_V1(		\
	VectorName,						\
	pSetup,							\
	pFinish							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_host_init.h ****/
#endif /* _USBPUMP_HOST_INIT_H_ */
