/* usbioctl_network.h	Tue Jul 24 2007 01:08:07 cccho */

/*

Module:  usbioctl_network.h

Function:
	IOCTLs for connecting between MCCI USB networking components.

Version:
	V1.97i	Tue Jul 24 2007 01:08:07 cccho	Edit level 2

Copyright notice:
	This file copyright (C) 2005-2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91d  Mon Aug 15 2005 19:05:23  tmm
	Module created.

   1.97i  Tue Jul 24 2007 01:08:07  cccho
	3087: usbioctl_network.h includes a protocol-specific header file
	(protoeth.h), but it's in a central location.  This means that it can't
	be compiled properly "stand alone", and it violates MCCI IOCTL header
	file design rules.

*/

#ifndef _USBIOCTL_NETWORK_H_		/* prevent multiple includes */
#define _USBIOCTL_NETWORK_H_

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _USBPUMP_IOCTL_H_
# include "usbpumpioctl.h"
#endif


/****************************************************************************\
|
|	Define the IOCTL codes here.
|
\****************************************************************************/

/*
|| for consistency, we define a helper macro that will define the
|| IOCTL code in terms of a corresponding argument type.  This is done
|| for convenience only.
||
|| For example,
||	#define USBPUMP_IOCTL_NETWORK_foo _TMS_USBPUMP_IOCTL_NETWORK(3, RW, foo)
|| is the same as
||	#define USBPUMP_IOCTL_NETWORK_foo \
||			__TMS_USBUMP_IOCTL_RW(
||				__TMS_USBPUMP_IOCTL_CLASS_NETWORK,
||				3,
||				__TMS_USBPUMP_IOCTL_NETWORK_foo_ARG
||				)
||
|| Note that the macro enforces the requirement that the arg type name
|| and the IOCTL name be related.  It doesn't save much typing, but
|| it's reliable.
||
|| For VOID type IOCTLs, you should define directly:
||
||	#define USBPUMP_IOCTL_NETWORK_JUNK \
||		__TMS_USBPUMP_IOCTL_VOID(
||				__TMS_USBPUMP_IOCTL_CLASS_NETWORK,
||				3)
||
*/

#define	__TMS_USBPUMP_IOCTL_NETWORK(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_NETWORK,			\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_NETWORK_, NAME, _ARG)	\
		)

/**** the actual IOCTL codes ****/
#define	__TMS_USBPUMP_IOCTL_NETWORK_GET_VETHER_PARAMS			\
	__TMS_USBPUMP_IOCTL_NETWORK(0, R, GET_VETHER_PARAMS)
#define	__TMS_USBPUMP_IOCTL_NETWORK_VETHER_ATTACH			\
	__TMS_USBPUMP_IOCTL_NETWORK(1, W, VETHER_ATTACH)
#define	__TMS_USBPUMP_IOCTL_NETWORK_GET_ENCMVETHER_PARAMS		\
	__TMS_USBPUMP_IOCTL_NETWORK(0, R, GET_ENCMVETHER_PARAMS)
#define	__TMS_USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH			\
	__TMS_USBPUMP_IOCTL_NETWORK(1, W, ENCMVETHER_ATTACH)

/****************************************************************************\
|
|	Define parameters and provide documentation for each IOCTL here.
|
\****************************************************************************/


/*

IOCTL:	USBPUMP_IOCTL_NETWORK_GET_VETHER_PARAMS

Index:	Type:	USBPUMP_IOCTL_NETWORK_GET_VETHER_PARAMS_ARG

Function:
	Get parameters needed for attaching VETHER to an instance.

Input:
	No input parameters.

Output:
	BYTES UsbTxHeaderSize;
		The desired amount of space to leave at the front of
		each ethernet frame for USB-specific wrappers.
	UPROTO_USBETHER * veth_pProto;
		The lower protocol object vether is attaching to

Description:
	When connecting to a protocol instance, VETHER often wants
	to know things about the protocol instance.  However, it
	may not want to (or may not be directly able to) get this
	information from the underlying protocol data structure.

	For example, some wire-level protocols like RNDIS require
	substantial space in front of each ethernet frame for use in
	describing the packets.  Others, such as CDC Ethernet, don't want any
	space.  Others, such as MCCI variants on CDC Ethernet, may
	want a few bytes of extra space.

	This IOCTL provides a portable, extensible way for VETHER to
	find out what it needs from the underlying protocols.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if the IOCTL is implemented.
	Some other non-success code otherwise.

Setup Macro:
	None used, as this is an OUT-only IOCTL.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_NETWORK_GET_VETHER_PARAMS_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_NETWORK_GET_VETHER_PARAMS_ARG)
	{
	BYTES UsbTxHeaderSize;
	struct __TMS_STRUCTNAME(UPROTO_USBETHER) * veth_pProto;
	};

/*

IOCTL:	USBPUMP_IOCTL_NETWORK_GET_ENCMVETHER_PARAMS

Index:	Type:	USBPUMP_IOCTL_NETWORK_GET_ENCMVETHER_PARAMS_ARG

Function:
	Get parameters needed for attaching ENCMVETHER to an instance.

Input:
	No input parameters.

Output:
	BYTES UsbTxHeaderSize;
		The desired amount of space to leave at the front of
		each ethernet frame for USB-specific wrappers.
	UPROTO_USBENCMETHER * veth_pProto;
		The lower protocol object vether is attaching to

Description:
	When connecting to a protocol instance, ENCMVETHER often wants
	to know things about the protocol instance.  However, it
	may not want to (or may not be directly able to) get this
	information from the underlying protocol data structure.

	For example, some wire-level protocols like RNDIS require
	substantial space in front of each ethernet frame for use in
	describing the packets.  Others, such as CDC Ethernet, don't want any
	space.  Others, such as MCCI variants on CDC Ethernet, may
	want a few bytes of extra space.

	This IOCTL provides a portable, extensible way for ENCMVETHER to
	find out what it needs from the underlying protocols.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if the IOCTL is implemented.
	Some other non-success code otherwise.

Setup Macro:
	None used, as this is an OUT-only IOCTL.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_NETWORK_GET_ENCMVETHER_PARAMS_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_NETWORK_GET_ENCMVETHER_PARAMS_ARG)
	{
	BYTES UsbTxHeaderSize;
	struct __TMS_STRUCTNAME(UPROTO_USBENCMETHER) * veth_pProto;
	};

/*

IOCTL:	USBPUMP_IOCTL_NETWORK_VETHER_ATTACH

Index:	Type:	USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG
	Name:	USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG_SETUP_V1()

Function:
	Notify a network protocol instance that a VETHER object
	wants to attach to the instance.

Input:
	CONST UPROTOSWITCH_USBETHER *pClientSwitch;
		The method table to be used by the protocol instance
		against the pClientContext object.

	VOID *pClientContext;
		The abstract object (not necesarily a DataPump Object!)
		that represents the VETHER-like thing to the protocol
		instance.

Output:
	No out parameters.

Description:
	The VETHER / {RNDIS, CDC ETHER, etc.} hierarchy predates the
	standard DataPump object system, and is asymmetrical.  The
	client knows something about the protocol service, but the
	protocol service knows nothing about the client, apart from
	having a pointer to a set of method functions, and a VOID* pointer
	to be passed as a parameter to those functions.  On the other hand,
	the protocol must export a view of itself that fits into
	the UPROTO_CDCETHER framework.  The client calls the method
	tables embedded in the UPROTO_CDCETHER, using the pointer to
	the UPROTO_CDCETHER as the "self" pointer.

	This IOCTL allows two objects to rendezvous to set up this connection.
	The client sends a pointer to its method table and its context
	handle.  (The method table is not necesarily copied, so it must be
	persistent.)  The protocol grabs the information it needs, and
	establishes the connection.

	During the connection process, the protocol will call the client's
	abstract "Attach" method.  If the connection attempt must fail,
	but the protocol has already attached, the protocol will first call
	the client's "Detach" method.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if the attach was successful.

	Some error code otherwise.

Setup Macro:
	VOID USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG_SETUP_V1(
		USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG Arg,
		VOID *pClientContext,
		CONST UPROTOSWITCH_USBETHER *pClientSwitch
		);

	This macro properly initializes the IOCTL argument object.
	MCCI will not change the arguments to the _V1() macro; if more
	paramters are added to the _ARG, then _V1() will be changed
	to initialize them to appropriate defaults, and a _V2() macro
	will be created for use by new code.

Notes:
	We apologize that this API is somewhat inconsistent, as it
	was retrofitted to an existing implementation.  Normally, nowadays
	we'd use VOID* for handles in each directly and make each layer
	opaque to the next.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG)
	{
	__TMS_VOID *pClientContext;
	__TMS_CONST struct __TMS_STRUCTNAME(UPROTOSWITCH_USBETHER) *pClientSwitch;
	};

/* don't add args to this, create a V2 macro!! */
#define	__TMS_USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pClientContext,						\
	a_pClientSwitch							\
	)								\
   do	{								\
	(a_pArg)->pClientContext = (a_pClientContext);			\
	(a_pArg)->pClientSwitch = (a_pClientSwitch);			\
	} while (0)

/*

IOCTL:	USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH

Index:	Type:	USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG
	Name:	USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG_SETUP_V1()

Function:
	Notify a network protocol instance that a ENCMVETHER object
	wants to attach to the instance.

Input:
	CONST UPROTOSWITCH_USBETHER *pClientSwitch;
		The method table to be used by the protocol instance
		against the pClientContext object.

	VOID *pClientContext;
		The abstract object (not necesarily a DataPump Object!)
		that represents the ENCMVETHER-like thing to the protocol
		instance.

Output:
	No out parameters.

Description:
	The ENCMVETHER / {RNDIS, CDC ETHER, etc.} hierarchy predates the
	standard DataPump object system, and is asymmetrical.  The
	client knows something about the protocol service, but the
	protocol service knows nothing about the client, apart from
	having a pointer to a set of method functions, and a VOID* pointer
	to be passed as a parameter to those functions.  On the other hand,
	the protocol must export a view of itself that fits into
	the UPROTO_CDCETHER framework.  The client calls the method
	tables embedded in the UPROTO_CDCETHER, using the pointer to
	the UPROTO_CDCETHER as the "self" pointer.

	This IOCTL allows two objects to rendezvous to set up this connection.
	The client sends a pointer to its method table and its context
	handle.  (The method table is not necesarily copied, so it must be
	persistent.)  The protocol grabs the information it needs, and
	establishes the connection.

	During the connection process, the protocol will call the client's
	abstract "Attach" method.  If the connection attempt must fail,
	but the protocol has already attached, the protocol will first call
	the client's "Detach" method.

Returns:
	USBPUMP_IOCTL_RESULT_SUCCESS if the attach was successful.

	Some error code otherwise.

Setup Macro:
	VOID USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG_SETUP_V1(
		USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG Arg,
		VOID *pClientContext,
		CONST UPROTOSWITCH_USBETHER *pClientSwitch
		);

	This macro properly initializes the IOCTL argument object.
	MCCI will not change the arguments to the _V1() macro; if more
	paramters are added to the _ARG, then _V1() will be changed
	to initialize them to appropriate defaults, and a _V2() macro
	will be created for use by new code.

Notes:
	We apologize that this API is somewhat inconsistent, as it
	was retrofitted to an existing implementation.  Normally, nowadays
	we'd use VOID* for handles in each directly and make each layer
	opaque to the next.

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG);
struct __TMS_STRUCTNAME	(USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG)
	{
	__TMS_VOID *pClientContext;
	__TMS_CONST struct __TMS_STRUCTNAME(UPROTOSWITCH_USBENCMETHER) *pClientSwitch;
	};

/* don't add args to this, create a V2 macro!! */
#define	__TMS_USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pClientContext,						\
	a_pClientSwitch							\
	)								\
   do	{								\
	(a_pArg)->pClientContext = (a_pClientContext);			\
	(a_pArg)->pClientSwitch = (a_pClientSwitch);			\
	} while (0)

/****************************************************************************\
|
|	Type un-cloaking.
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_NETWORK(i, t, NAME)	\
   __TMS_USBPUMP_IOCTL_NETWORK(i, t, NAME)
# define USBPUMP_IOCTL_NETWORK_GET_VETHER_PARAMS	\
   __TMS_USBPUMP_IOCTL_NETWORK_GET_VETHER_PARAMS
# define USBPUMP_IOCTL_NETWORK_VETHER_ATTACH	\
   __TMS_USBPUMP_IOCTL_NETWORK_VETHER_ATTACH
# define USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pClientContext,						\
	a_pClientSwitch							\
	)	\
	__TMS_USBPUMP_IOCTL_NETWORK_VETHER_ATTACH_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pClientContext,						\
	a_pClientSwitch							\
	)
# define USBPUMP_IOCTL_NETWORK_GET_ENCMVETHER_PARAMS	\
   __TMS_USBPUMP_IOCTL_NETWORK_GET_ENCMVETHER_PARAMS
# define USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH	\
   __TMS_USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH
# define USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG_SETUP_V1(		\
	a_pArg,								\
	a_pClientContext,						\
	a_pClientSwitch							\
	)	\
	__TMS_USBPUMP_IOCTL_NETWORK_ENCMVETHER_ATTACH_ARG_SETUP_V1(	\
	a_pArg,								\
	a_pClientContext,						\
	a_pClientSwitch							\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_network.h ****/
#endif /* _USBIOCTL_NETWORK_H_ */
