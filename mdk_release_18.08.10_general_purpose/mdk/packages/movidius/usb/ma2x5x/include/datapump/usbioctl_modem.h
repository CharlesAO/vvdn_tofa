/* usbioctl_modem.h	Thu Jan 27 2005 12:50:39 chwon */

/*

Module:  usbioctl_modem.h

Function:
	IOCTL codes for upper edge outcalls for USB modems.

Version:
	V1.91a  Thu Jan 27 2005 12:50:39 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2002-2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	July 2002

Revision history:
   1.79a  Wed Jul 10 2002 13:21:25  tmm
	Module created.

   1.79a  7/31/2002  tmm
	Correct documentation.

   1.79b  12/6/2002  chwon
	Use __TMS_CONCAT() instead of ##. This is more portable.

   1.87b  Mon March 8 2004 09:35:00  mats
   	Correct comments for USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG_SETUP_V1 &
   	USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG_SETUP_V1.
   	Note: No code changes !!

   1.91a  Thu Jan 27 2005 12:50:39  chwon
	Use __TMS_CONCAT() instead of ## because this is more portable code.

*/

#ifndef _USBIOCTL_MODEM_H_		/* prevent multiple includes */
#define _USBIOCTL_MODEM_H_

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _CDCLINECODING_H_
# include "cdclinecoding.h"
#endif

/****************************************************************************\
|
|	The plane selector type, documented later as part of the MODEM
|	QUEUE_IN IOCTL.
|
\****************************************************************************/

enum __TMS_ENUMNAME(USBPUMP_MODEM_PLANE_SELECTOR)
	{
	__TMS_USBPUMP_MODEM_PLANE_ENCAPS,
	__TMS_USBPUMP_MODEM_PLANE_DATA
	};

__TMS_TYPE_DEF_ENUM(USBPUMP_MODEM_PLANE_SELECTOR);

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_MODEM_PLANE_ENCAPS	__TMS_USBPUMP_MODEM_PLANE_ENCAPS
# define USBPUMP_MODEM_PLANE_DATA	__TMS_USBPUMP_MODEM_PLANE_DATA
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/****************************************************************************\
|
|	The `EDGE' IOCTLs are sent outwards
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_EDGE_MODEM(i, t, NAME)			\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_MODEM_EDGE, (i),		\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_EDGE_MODEM_, NAME, _ARG) \
		)

#define	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK			\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM(0, RW, SET_BREAK)

#define	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE		\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM(1, RW, SET_CONTROL_LINE_STATE)

#define	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING			\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM(2, RW, SET_LINE_CODING)

#define	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE			\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM(3, RW, SET_ACM_FEATURE)

#define	__TMS_USBPUMP_IOCTL_EDGE_MODEM_START_PLANE			\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM(4, RW, START_PLANE)

#define	__TMS_USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE			\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM(5, W, STOP_PLANE)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK
# define USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE
# define USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING
# define USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE
# define USBPUMP_IOCTL_EDGE_MODEM_START_PLANE	__TMS_USBPUMP_IOCTL_EDGE_MODEM_START_PLANE
# define USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE	__TMS_USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/****************************************************************************\
|
|	The non-EDGE IOCTLs arrive inwards from the OS-specific drivers
|
\****************************************************************************/

/**** define the MODEM in-calls ****/
#define __TMS_USBPUMP_IOCTL_MODEM(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_MODEM,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_MODEM_, NAME, _ARG)	\
		)

#define	__TMS_USBPUMP_IOCTL_MODEM_SET_UART_STATE			\
	__TMS_USBPUMP_IOCTL_MODEM(0, RW, SET_UART_STATE)

#define	__TMS_USBPUMP_IOCTL_MODEM_QUEUE_IN				\
	__TMS_USBPUMP_IOCTL_MODEM(1, W, QUEUE_IN)

#define	__TMS_USBPUMP_IOCTL_MODEM_QUEUE_OUT				\
	__TMS_USBPUMP_IOCTL_MODEM(2, W, QUEUE_OUT)

#define	__TMS_USBPUMP_IOCTL_MODEM_CANCEL_IN				\
	__TMS_USBPUMP_IOCTL_MODEM(3, W, CANCEL_IN)

#define	__TMS_USBPUMP_IOCTL_MODEM_CANCEL_OUT				\
	__TMS_USBPUMP_IOCTL_MODEM(4, W, CANCEL_OUT)

#define	__TMS_USBPUMP_IOCTL_MODEM_GET_PLANE_INFO			\
	__TMS_USBPUMP_IOCTL_MODEM(5, RW, GET_PLANE_INFO)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_MODEM_SET_UART_STATE	__TMS_USBPUMP_IOCTL_MODEM_SET_UART_STATE
# define USBPUMP_IOCTL_MODEM_QUEUE_IN	__TMS_USBPUMP_IOCTL_MODEM_QUEUE_IN
# define USBPUMP_IOCTL_MODEM_QUEUE_OUT	__TMS_USBPUMP_IOCTL_MODEM_QUEUE_OUT
# define USBPUMP_IOCTL_MODEM_CANCEL_IN	__TMS_USBPUMP_IOCTL_MODEM_CANCEL_IN
# define USBPUMP_IOCTL_MODEM_CANCEL_OUT	__TMS_USBPUMP_IOCTL_MODEM_CANCEL_OUT
# define USBPUMP_IOCTL_MODEM_GET_PLANE_INFO	__TMS_USBPUMP_IOCTL_MODEM_GET_PLANE_INFO
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

Type:	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_HOW

Function:
	Abstract type indicating how the host wanted to change the break

Definition:
	typedef enum {
		USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_OFF,
		USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ON,
		USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_TIMED
		} USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_HOW, 
		*PUSBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_HOW;

Description:
	CDC SET_BREAK commands can specify three possible states:  OFF, in
	which case break is statically not asserted; ON, in which case break
	is statically asserted; and TIMED, in which case, break is asserted
	for a specified number of milliseconds.  This enumeration type is
	used to convey this information to clients.

*/

enum __TMS_ENUMNAME(USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_HOW)
	{
	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_OFF,
	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ON,
	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_TIMED
	};

__TMS_TYPE_DEF_ENUM(USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_HOW);


/*

IOCTL:	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK

Index:	Type:	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG
	Name:	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing SET_BREAK

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_SET_BREAK_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_SET_BREAK_ARG structure.

	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_HOW How;
		IN: indicates the kind of break request.

	UINT uMillisec OPTIONAL;
		IN: if how is .._TIMED, then this specifies how many
		millisecond long the break signal should be.

	BOOL fReject;
		OUT:  if set TRUE, then the edge driver would like the
		USB core to reject the request, if possible.  Note, though,
		that due to hardware constraints it may not be possible to
		fail the request.

Description:
	This IOCTL is sent from a leaf modem/serial emulation function to the
	OS-specific driver that has opened/connected to the leaf object.
	It is sent whenever the host changes the break state.

	fReject is an advisory indication, which may be used to flag to 
	the protocol that the upper edge cannot actually operate the
	data planes at this time.  Because of hardware or protocol
	limitations, this might or might not be honored by the lower layers.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG *pArg,
		VOID *pClientContext,
		USBPUMP_OBJECT_HEADER *pObject,
		USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_HOW How,
		UINT uMillisec OPTIONAL
		);

	This macro will correctly initialize a .._SET_BREAK_ARG structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	The client is responsible for processing the uMillisec argument
	by setting up whatever kind of timing loop is appropriate.  usMillisec
	should be ignored unless How is 
	USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_TIMED.

	If the client doesn't provide this function, the break state will
	be tracked by the base implementation, but no other action will
	be taken.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_HOW
			How;
	__TMS_UINT	uMillisec;	/* IN: duration (if How == .._TIMED) */

	__TMS_BOOL	fReject;	/* OUT:  set TRUE if the edge
					||  can be brought up now, FALSE
					||  otherwise.  If possible, will
					||  be used to fail the activate
					||  request on the bus, but this
					||  is not always possible.
					*/
	};

/* the setup for the .._SET_BREAK_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_How,							\
	ARG_uMillisec							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->How = (ARG_How);					\
	__pArg->uMillisec = (ARG_uMillisec);				\
	__pArg->fReject = __TMS_FALSE;					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_How,							\
	ARG_uMillisec							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_BREAK_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_How,							\
	ARG_uMillisec							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE

Index:	Type:	USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG
	Name:	USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when the control-line state changes.

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG 
			structure.

	USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG has the 
	following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	UINT NewValue;
		IN: the control line state value.  

		The mask USB_Comm_SET_CONTROL_LINE_STATE_wValue_DTR represets
		the value of DTR.

		THe mask USB_Comm_SET_CONTROL_LINE_STATE_wValue_RTS represents
		the value of RTS.
		
	BOOL fReject;
		OUT:  if set TRUE, then the edge driver would like the
		USB core to reject the request, if possible.  Note, though,
		that due to hardware constraints it may not be possible to
		fail the request.

Description:
	The base implementation calls this routine whenever the host
	modifies the state of DTR or RTS.  NewValue is an 8 bit mask
	indicating the new state of DTR and RTS, as given by the
	CDC ACM spec (or in "usbcdc11.h").  The client should do whatever
	makes sense to communicate the state change to the higher level.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG *pArg,
		VOID *pClientContext,
		USBPUMP_OBJECT_HEADER *pObject,
		UINT NewValue
		);

	This macro will correctly initialize an .._SET_CONTROL_LINE_STATE_ARG
	structure.  If the layout of the argument structure changes in the 
	future, this macro will initialize the new elements in a forward-
	compatible way.

Returns:
	No explicit result.

Notes:
	If this function is not provided, the base code tracks the state 
	of DTR and RTS commanded by the host, but takes no other actions.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	__TMS_UINT	NewValue;	/* IN: DTR and RTS values */
	__TMS_BOOL	fReject;	/* OUT:  set TRUE if the edge
					||  can be brought up now, FALSE
					||  otherwise.  If possible, will
					||  be used to fail the activate
					||  request on the bus, but this
					||  is not always possible.
					*/
	};

/* the setup for the .._SET_CONTROL_LINE_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG_SETUP_V1( \
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_NewValue							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->NewValue = (ARG_NewValue);				\
	__pArg->fReject = __TMS_FALSE;					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_NewValue							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_CONTROL_LINE_STATE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_NewValue							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING

Index:	Type:	USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG
	Name:	USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing SET_LINE_CODING

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_MODE_SET_LINE_CODING_ARG structure.

	USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	CONST USB_Comm_LINE_CODING *pCoding;
		IN: the actual coding values: baudrate, bits per char,
		parity, stop bits.

	BOOL fReject;
		OUT:  if set TRUE, then the edge driver would like the
		USB core to reject the request, if possible.  Note, though,
		that due to hardware constraints it may not be possible to
		fail the request.

Description:
	This IOCTL is sent from a leaf modem/serial emulation function to the
	OS-specific driver that has opened/connected to the leaf object.
	The host uses a SET_LINE_CODING request to change the baudrate,
	bits per character, and parity of the connection.  This
	client-supplied routine is called after basic parameters of
	the SET_LINE_CODING operation have been verified.  The client
	has the additional opportunity to reject the operation, by setting
	fReject TRUE.

	fReject is an advisory indication, which may be used to flag to 
	the protocol that the upper edge cannot actually operate the
	data planes at this time.  Because of hardware or protocol
	limitations, this might or might not be honored by the lower layers.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG *pArg,
		VOID *pClientContext,
		USBPUMP_OBJECT_HEADER *pObject,
		CONST USB_Comm_LINE_CODING *pCoding
		);

	This macro will correctly initialize a ..SET_LINE_CODING_ARG 
	structure.  If the layout of the argument structure changes in the 
	future, this macro will initialize the new elements in a forward-
	compatible way.

Notes:
	If this IOCTL is not supplied, any SET_LINE_CODING operation that
	is legal within the ACM spec is permitted by the base code.  In any
	case, the current values are stored in the WMCSUBCLASS_MODEM
	structure.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	__TMS_CONST __TMS_USB_Comm_LINE_CODING 
			*pCoding;	/* IN: the coding structure */

	__TMS_BOOL	fReject;	/* OUT:  set TRUE if the edge
					||  can be brought up now, FALSE
					||  otherwise.  If possible, will
					||  be used to fail the activate
					||  request on the bus, but this
					||  is not always possible.
					*/
	};

/* the setup for the .._SET_LINE_CODING_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_pCoding							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->pCoding = (ARG_pCoding);				\
	__pArg->fReject = __TMS_FALSE;					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_pCoding							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_LINE_CODING_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_pCoding							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE

Index:	Type:	USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG
	Name:	USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing SET_ACM_FEATURE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG 
			structure.

	USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG has the following 
	elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	UINT FeatureIndex;
		IN: the feature index that is being changed.

	UINT FeatureValue;	
		IN: the value that is being changed.

	BOOL fReject;
		OUT:  if set TRUE, then the edge driver would like the
		USB core to reject the request, if possible.  Note, though,
		that due to hardware constraints it may not be possible to
		fail the request.

Description:
	This IOCTL is sent from a leaf modem/serial emulation function to the
	OS-specific driver that has opened/connected to the leaf object.
	It is sent whenever the host changes the value of a feature.

	fReject is an advisory indication, which may be used to flag to 
	the protocol that the upper edge cannot actually operate the
	data planes at this time.  Because of hardware or protocol
	limitations, this might or might not be honored by the lower layers.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG *pArg,
		VOID *pClientContext,
		USBPUMP_OBJECT_HEADER *pObject,
		UINT FeatureIndex,
		UINT FeatureValue
		);

	This macro will correctly initialize a .._SET_ACM_FEATURE_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	this macro will initialize the new elements in a forward-compatible
	way.

Notes:
	If the client doesn't provide this function, the break state will
	be tracked by the base implementation, but no other action will
	be taken.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	__TMS_UINT	FeatureIndex;	/* IN: duration (if How == .._TIMED) */
	__TMS_UINT	FeatureValue;	/* IN: the feature value 
					||	setting
					*/

	__TMS_BOOL	fReject;	/* OUT:  set TRUE if the edge
					||  can be brought up now, FALSE
					||  otherwise.  If possible, will
					||  be used to fail the activate
					||  request on the bus, but this
					||  is not always possible.
					*/
	};

/* the setup for the .._SET_ACM_FEATURE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_FeatureIndex,						\
	ARG_FeatureValue						\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->FeatureIndex = (ARG_FeatureIndex);			\
	__pArg->FeatureValue = (ARG_FeatureValue);			\
	__pArg->fReject = __TMS_FALSE;					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_FeatureIndex,						\
	ARG_FeatureValue						\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_MODEM_SET_ACM_FEATURE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pCtx,							\
	ARG_FeatureIndex,						\
	ARG_FeatureValue						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_EDGE_MODEM_START_PLANE

Index:	Type:	USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG
	Name:	USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing START_PLANE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG 
			structure.

	USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG has the following 
	elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	VOID *pClientContext;
		IN: client context supplied when registering.

	USBPUMP_MODEM_PLANE_SELECTOR iPlane;
		IN: The plane index.  This is either 
		USBPUMP_MODEM_PLANE_ENCAPS or USBPUMP_MODEM_PLANE_DATA+(i), 
		to select the plane on a multi-data-plane device.

	BOOL fReject;
		OUT:  if set TRUE, then the edge driver would like the
		USB core to reject the request, if possible.  Note, though,
		that due to hardware constraints it may not be possible to
		fail the request.

Description:
	This IOCTL is sent from a leaf modem/serial emulation function to the
	OS-specific driver that has opened/connected to the leaf object.
	It is sent whenever the host starts up a plane.

	fReject is an advisory indication, which may be used to flag to 
	the protocol that the upper edge cannot actually operate the
	data planes at this time.  Because of hardware or protocol
	limitations, this might or might not be honored by the lower layers.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		USBPUMP_OBJECT_HEADER *pClientObject,
		VOID *pClientContext,
		USBPUMP_MODEM_PLANE_SELECTOR iPlane
		);

	This macro will correctly initialize a .._START_PLANE_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will redefine this macro to initialize the new elements in a 
	forward-compatible way, and will create a _Vx() macro that allows
	access to the new parameters.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_USBPUMP_OBJECT_HEADER
			*pClientObject;	/* IN: pointer to client object
					||     from open
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR
			iPlane;		/* IN: the plane selector */
	__TMS_BOOL	fReject;	/* OUT:  set TRUE if the edge
					||  can be brought up now, FALSE
					||  otherwise.  If possible, will
					||  be used to fail the activate
					||  request on the bus, but this
					||  is not always possible.
					*/
	};

/* the setup for the .._START_PLANE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG_SETUP_V1(	\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientObject,						\
	ARG_pCtx,							\
	ARG_iPlane							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientObject = (ARG_pClientObject);			\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->iPlane = (ARG_iPlane);					\
	__pArg->fReject = __TMS_FALSE;					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientObject,						\
	ARG_pCtx,							\
	ARG_iPlane							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_MODEM_START_PLANE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientObject,						\
	ARG_pCtx,							\
	ARG_iPlane							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE

Index:	Type:	USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG
	Name:	USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG_SETUP_V1

Function:
	Notification IOCTL issued when processing STOP_PLANE

Input:
	pInParam	points to a USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG 
			structure; normally NULL.

	USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG has the following 
	elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the object that is being brought up.  Presumably
		this client has registered with the client as the owning
		driver.

	USBPUMP_OBJECT_HEADER *pClientObject;
		IN: points to client object that was registered at open time.

	VOID *pClientContext;
		IN: client context supplied when registering.

	USBPUMP_MODEM_PLANE_SELECTOR iPlane;
		IN: The plane index.  This is either 
		USBPUMP_MODEM_PLANE_ENCAPS or USBPUMP_MODEM_PLANE_DATA+(i), 
		to select the plane on a multi-data-plane device.

Description:
	This IOCTL is sent from a leaf modem/serial emulation function to the
	OS-specific driver that has opened/connected to the leaf object.
	It is sent whenever the host stops a plane.

	Unlike START, there is no way to reject a stop.

Setup Macro:
	VOID USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG_SETUP_V1(
		USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		USBPUMP_OBJECT_HEADER *pClientObject,
		VOID *pClientContext,
		USBPUMP_MODEM_PLANE_SELECTOR iPlane
		);

	This macro will correctly initialize a .._STOP_PLANE_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will redefine this macro to initialize the new elements in a 
	forward-compatible way, and will create a _Vx() macro that allows
	access to the new parameters.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to object that
					||     is coming up.
					*/
	__TMS_USBPUMP_OBJECT_HEADER
			*pClientObject;	/* IN: pointer to client object
					||     from open
					*/
	__TMS_VOID	*pClientContext; /* IN: client context */
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR
			iPlane;		/* IN: the plane selector */
	};

/* the setup for the .._STOP_PLANE_ARG struct */
#define __TMS_USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientObject,						\
	ARG_pCtx,							\
	ARG_iPlane							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pClientObject = (ARG_pClientObject);			\
	__pArg->pClientContext = (ARG_pCtx);				\
	__pArg->iPlane = (ARG_iPlane);					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientObject,						\
	ARG_pCtx,							\
	ARG_iPlane							\
	)								\
   __TMS_USBPUMP_IOCTL_EDGE_MODEM_STOP_PLANE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pClientObject,						\
	ARG_pCtx,							\
	ARG_iPlane							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_MODEM_SET_UART_STATE

Index:	Type:	USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG
	Name:	USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG_SETUP_V1
	Const:	USBPUMP_MODEM_UART_DCD
		USBPUMP_MODEM_UART_DSR
		USBPUMP_MODEM_UART_RI
		USBPUMP_MODEM_UART_FE
		USBPUMP_MODEM_UART_PE
		USBPUMP_MODEM_UART_OE
		USBPUMP_MODEM_UART_CTS


Function:
	Control IOCTL received from client OS code.

Input:
	pInParam	points to a USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG structure.

	USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the modem function object.

	UINT Mask;
		The mask of bits that are to be affected in the UART
		state.

	UINT Value;
		The values.

	The abstract definition of this has the following bit layouts,
	which (by design) correspond to the SERIAL_STATE bits.

	0	DCD		USBPUMP_MODEM_UART_DCD
	1	DSR		USBPUMP_MODEM_UART_DSR
	2	BREAK		USBPUMP_MODEM_UART_BREAK
	3	RI		USBPUMP_MODEM_UART_RI
	4	Framing		USBPUMP_MODEM_UART_FE
	5	Parity		USBPUMP_MODEM_UART_PE
	6	Overrun		USBPUMP_MODEM_UART_OE
	7	<<reserved for use as CTS>>
				USBPUMP_MODEM_UART_CTS
	8..15	<<reserved>>

Description:
	This IOCTL is sent from an OS-specific driver to change the state
	of the virtual UART.

Setup Macro:
	VOID USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG_SETUP_V1(
		USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		UINT Mask,
		UINT Value
		);

	This macro will correctly initialize a .._SET_UART_STATE_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in 
	a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to modem fn */
	__TMS_UINT	Mask;		/* IN: the mask */
	__TMS_UINT	Value;		/* IN/OUT: the value */
	};

/* the setup for the .._MODEM_SET_UART_STATE_ARG struct */
#define __TMS_USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Mask,							\
	ARG_Value							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG * __TMS_CONST	\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->Mask = (ARG_Mask);					\
	__pArg->Value = (ARG_Value);					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Mask,							\
	ARG_Value							\
	)								\
   __TMS_USBPUMP_IOCTL_MODEM_SET_UART_STATE_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_Mask,							\
	ARG_Value							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*
|| The UART bit names
*/
#define	__TMS_USBPUMP_MODEM_UART_DCD		0x00000001
#define	__TMS_USBPUMP_MODEM_UART_DSR		0x00000002
#define	__TMS_USBPUMP_MODEM_UART_BREAK		0x00000004
#define	__TMS_USBPUMP_MODEM_UART_RI		0x00000008
#define	__TMS_USBPUMP_MODEM_UART_FE		0x00000010
#define	__TMS_USBPUMP_MODEM_UART_PE		0x00000020
#define	__TMS_USBPUMP_MODEM_UART_OE		0x00000040
#define	__TMS_USBPUMP_MODEM_UART_CTS		0x00000080
/*	<<reserved>>				bits 8..31 */

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_MODEM_UART_DCD		__TMS_USBPUMP_MODEM_UART_DCD
# define USBPUMP_MODEM_UART_DSR		__TMS_USBPUMP_MODEM_UART_DSR
# define USBPUMP_MODEM_UART_BREAK	__TMS_USBPUMP_MODEM_UART_BREAK
# define USBPUMP_MODEM_UART_RI		__TMS_USBPUMP_MODEM_UART_RI
# define USBPUMP_MODEM_UART_FE		__TMS_USBPUMP_MODEM_UART_FE
# define USBPUMP_MODEM_UART_PE		__TMS_USBPUMP_MODEM_UART_PE
# define USBPUMP_MODEM_UART_OE		__TMS_USBPUMP_MODEM_UART_OE
# define USBPUMP_MODEM_UART_CTS		__TMS_USBPUMP_MODEM_UART_CTS
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_MODEM_QUEUE_IN

Index:	Type:	USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG
	Name:	USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG_SETUP_V1
	Type:	USBPUMP_MODEM_PLANE_SELECTOR
	Const:	USBPUMP_MODEM_PLANE_ENCAPS
		USBPUMP_MODEM_PLANE_DATA

Function:
	Send data from a UBUFQE to the host.

Input:
	pInParam	points to a USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG structure.

	USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the modem function object.

	UBUFQE *pQe;
		The queue element to be submitted.  uqe_pPipe will be
		overwritten as needed.

	USBPUMP_MODEM_PLANE_SELECTOR iPlane;
		The plane index.  This is either USBPUMP_MODEM_PLANE_ENCAPS
		or USBPUMP_MODEM_PLANE_DATA + (i), to select the plane
		on a multi-data-plane device.

Description:
	This IOCTL is sent from an OS-specific driver to submit a UBUFQE
	to the IN stream of the specified logical data plane.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the QEs were accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG_SETUP_V1(
		USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		UBUFQE *pQe,
		USBPUMP_MODEM_PLANE_SELECTOR iPlane
		);

	This macro will correctly initialize a .._QUEUE_IN_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in 
	a forward-compatible way.

Notes:
	This IOCTL is essentially equivalent to
	USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG; it's separated essentially for
	ease of use.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to modem fn */
	__TMS_UBUFQE	*pQe;		/* IN: the mask */
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR	
			iPlane;		/* IN: the plane selector */
	};

/* the setup for the .._MODEM_QUEUE_IN_ARG struct */
#define __TMS_USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pQe,							\
	ARG_iPlane							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pQe = (ARG_pQe);					\
	__pArg->iPlane = (ARG_iPlane);					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pQe,							\
	ARG_iPlane							\
	)								\
   __TMS_USBPUMP_IOCTL_MODEM_QUEUE_IN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pQe,							\
	ARG_iPlane							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_MODEM_QUEUE_OUT

Index:	Type:	USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG
	Name:	USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG_SETUP_V1

Function:
	Queue a UBUFQE to be filled by the host via the OUT stream of
	a given data plane.

Input:
	pInParam	points to a USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG structure.

	USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the modem function object.

	UBUFQE *pQe;
		The queue element to be submitted.  uqe_pPipe will be
		overwritten as needed.

	USBPUMP_MODEM_PLANE_SELECTOR iPlane;
		The plane index.  This is either USBPUMP_MODEM_PLANE_ENCAPS
		or USBPUMP_MODEM_PLANE_DATA + (i), to select the plane
		on a multi-data-plane device.

Description:
	This IOCTL is sent from an OS-specific driver to submit a UBUFQE
	to the OUT stream of the specified logical data plane.

Results:
	USBPUMP_IOCTL_RESULT_SUCCESS if the QEs were accepted and the
	completion routine will be called in due course.  Some other
	result if the IOCTL is not implemented, or if the internal
	plumbing is in some way deficient.

Setup Macro:
	VOID USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG_SETUP_V1(
		USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		UBUFQE *pQe,
		USBPUMP_MODEM_PLANE_SELECTOR iPlane
		);

	This macro will correctly initialize a .._QUEUE_OUT_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in 
	a forward-compatible way.

Notes:
	This IOCTL is essentially equivalent to
	USBPUMP_IOCTL_MODEM_QUEUE_IN; it's separated essentially for
	ease of use.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to modem fn */
	__TMS_UBUFQE	*pQe;		/* IN: the mask */
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR	
			iPlane;		/* IN: the plane selector */
	};

/* the setup for the .._MODEM_QUEUE_OUT_ARG struct */
#define __TMS_USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_pQe,							\
	ARG_iPlane							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->pQe = (ARG_pQe);					\
	__pArg->iPlane = (ARG_iPlane);					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pQe,							\
	ARG_iPlane							\
	)								\
   __TMS_USBPUMP_IOCTL_MODEM_QUEUE_OUT_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_pQe,							\
	ARG_iPlane							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_MODEM_CANCEL_IN

Index:	Type:	USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG
	Name:	USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG_SETUP_V1

Function:
	Cancel all the UBUFQEs pending on the IN stream of a data plane.

Input:
	pInParam	points to a USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG structure.

	USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the modem function object.

	USBPUMP_MODEM_PLANE_SELECTOR iPlane;
		The plane index.  This is either USBPUMP_MODEM_PLANE_ENCAPS
		or USBPUMP_MODEM_PLANE_DATA + (i), to select the plane
		on a multi-data-plane device.

	USTAT Why;
		The completion code to be delivered as the reason for
		the cancellation.  Can be USTAT_KILL.

Description:
	This IOCTL is sent from an OS-specific driver to cancel all pending
	UBUFQEs queued to the IN stream of a specified data plane.  Because of
	internal constraints cancelled UBUFQEs are not guaranteed to complete
	inmmediately, but rather "as soon as possible".

Setup Macro:
	VOID USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG_SETUP_V1(
		USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		USBPUMP_MODEM_PLANE_SELECTOR iPlane,
		USTAT Why
		);

	This macro will correctly initialize a .._CANCEL_IN_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in 
	a forward-compatible way.

Notes:
	This IOCTL is essentially equivalent to
	USBPUMP_IOCTL_MODEM_CANCEL_OUT; it's separated essentially for
	ease of use.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to modem fn */
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR	
			iPlane;		/* IN: the plane selector */
	__TMS_USTAT	Why;		/* IN: the completion status */
	};

/* the setup for the .._CANCEL_IN_ARG struct */
#define __TMS_USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iPlane,							\
	ARG_Why								\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->iPlane = (ARG_iPlane);					\
	__pArg->Why = (ARG_Why);					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_iPlane,							\
	ARG_Why								\
	)								\
   __TMS_USBPUMP_IOCTL_MODEM_CANCEL_IN_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_iPlane,							\
	ARG_Why								\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_MODEM_CANCEL_OUT

Index:	Type:	USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG
	Name:	USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG_SETUP_V1

Function:
	Cancel all the UBUFQEs pending on the OUT stream of a given plane.

Input:
	pInParam	points to a USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG structure.

	USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the modem function object.

	USBPUMP_MODEM_PLANE_SELECTOR iPlane;
		The plane index.  This is either USBPUMP_MODEM_PLANE_ENCAPS
		or USBPUMP_MODEM_PLANE_DATA + (i), to select the plane
		on a multi-data-plane device.

	USTAT Why;
		The completion code to be delivered as the reason for
		the cancellation.  Can be USTAT_KILL.

Description:
	This IOCTL is sent from an OS-specific driver to submit a UBUFQE
	to the OUT stream of the specified logical data plane.

Setup Macro:
	VOID USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG_SETUP_V1(
		USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject,
		USBPUMP_MODEM_PLANE_SELECTOR iPlane,
		USTAT Why
		);

	This macro will correctly initialize a .._CANCEL_OUT_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in 
	a forward-compatible way.

Notes:
	This IOCTL is essentially equivalent to
	USBPUMP_IOCTL_MODEM_CANCEL_IN; it's separated essentially for
	ease of use.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to modem fn */
	__TMS_USBPUMP_MODEM_PLANE_SELECTOR	
			iPlane;		/* IN: the plane selector */
	__TMS_USTAT	Why;		/* IN: the completion status */
	};

/* the setup for the .._CANCEL_OUT_ARG struct */
#define __TMS_USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject,							\
	ARG_iPlane,							\
	ARG_Why								\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->iPlane = (ARG_iPlane);					\
	__pArg->Why = (ARG_Why);					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_iPlane,							\
	ARG_Why								\
	)								\
   __TMS_USBPUMP_IOCTL_MODEM_CANCEL_OUT_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject,							\
	ARG_iPlane,							\
	ARG_Why								\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*

IOCTL:	USBPUMP_IOCTL_MODEM_GET_PLANE_INFO

Index:	Type:	USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG
	Name:	USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG_SETUP_V1

Function:
	Return information about the dataplane configuration of the
	underlying function.

Input:
	pInParam	points to a USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG structure.

	USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG has the following elements:

	USBPUMP_OBJECT_HEADER *pObject;
		IN: points to the modem function object (this is the I/O
		    object returned by the OPEN).

	BOOL bHasControlPlane;
		OUT: TRUE if the function supports a distinct control plane.

	UINT uNumDataPlanes;
		OUT:  set to the count of distinct data planes.

Description:
	This IOCTL is sent from an OS-specific driver in order to obtain
	information about the configuration of the specified abstract modem.

Setup Macro:
	VOID USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG_SETUP_V1(
		USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG *pArg,
		USBPUMP_OBJECT_HEADER *pObject
		);

	This macro will correctly initialize a .._GET_PLANE_INFO_ARG 
	structure.
	If the layout of the argument structure changes in the future,
	MCCI will change this macro to initialize the new elements in 
	a forward-compatible way.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG)
	{
	__TMS_USBPUMP_OBJECT_HEADER
			*pObject;	/* IN: pointer to modem fn */
	__TMS_BOOL	bHasControlPlane;
					/* OUT: set true if there's a control
					||	plane.
					*/
	__TMS_UINT	uNumDataPlanes;	/* OUT: set to the number of distinct
					||	data planes.
					*/
	};

/* the setup for the .._GET_PLANE_INFO_ARG struct */
#define __TMS_USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG_SETUP_V1(		\
	pArg,								\
	ARG_pObject							\
	)								\
   do	{								\
	__TMS_USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG * __TMS_CONST		\
		__pArg = (pArg);					\
	__pArg->pObject = (ARG_pObject);				\
	__pArg->bHasControlPlane = __TMS_FALSE;				\
	__pArg->uNumDataPlanes = 0;					\
   	} while (0)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject							\
	)								\
   __TMS_USBPUMP_IOCTL_MODEM_GET_PLANE_INFO_ARG_SETUP_V1(			\
	pArg,								\
	ARG_pObject							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_modem.h ****/
#endif /* _USBIOCTL_MODEM_H_ */
