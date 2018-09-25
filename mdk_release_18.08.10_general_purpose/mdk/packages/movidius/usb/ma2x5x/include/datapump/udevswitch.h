/* udevswitch.h	Thu Feb 09 2012 15:29:20 chwon */

/*

Module:  udevswitch.h

Function:
	The home for the UDEVSWITCH types and objects

Version:
	V3.01f	Thu Feb 09 2012 15:29:20 chwon	Edit level 8

Copyright notice:
	This file copyright (C) 2002, 2007-2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 22:51:21  tmm
	Module created.

   1.97f  Tue Jan 16 2007 10:01:10  chwon
	2164: add remote wakeup validation in UDEVREMOTEWAKEUP() and make a new
	macro UDEVREMOVEWAKEUP_DIRECT() to do the raw in call.

   1.97k  Thu Oct 16 2008 11:52:12  cccho
	6865: correct typos from UDEVICE_IS_REMOVE_WAKUP_VALID to
	UDEVICE_IS_REMOTE_WAKUP_VALID.

   2.01a  Fri Mar 06 2009 11:19:28  cccho
	6865: correct typos from UDEVICE_IS_REMOTE_WAKUP_VALID to
	UDEVICE_IS_REMOTE_WAKEUP_VALID.

   3.01d  Fri Sep 24 2010 14:16:06  cccho
	6150, 8071: Notify device fsm of remote wakeup event in
	UDEVREMOTEWAKEUP().

   3.01f  Mon Apr 25 2011 14:25:24  chwon
	13269: added UDEVGETUDEVICESIZEFN, UDEVGETUENDPOINTSIZEFN and
	UDEVENDPOINTINITFN device switch functions.

   3.01f  Tue Jan 10 2012 14:37:22  chwon
	14623: added UDEVICE_IS_REMOTE_WAKEUP_VALID_V2() in UDEVREMOTEWAKEUP()
	before call UsbPumpDevice_DeviceFsm_evRemoteWakeup().

   3.01f  Thu Feb 09 2012 15:29:20  chwon
	14804: added UDEVGETFRAMENUMBERFN device switch function and added
	UDEVICESWITCH_INIT_V7() macro.

*/

#ifndef _UDEVSWITCH_H_		/* prevent multiple includes */
#define _UDEVSWITCH_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

/****************************************************************************\
|
|	The device switch structure is used for interfacing to the
|	hardware.
|
\****************************************************************************/

__TMS_FNTYPE_DEF(									\
UDEVINITIALIZEFN,									\
__TMS_USTAT,										\
	(										\
	__TMS_CONST __TMS_UDEVICESWITCH *pSwitch,					\
						/* from chip driver */			\
	__TMS_UDEVICE *, __TMS_BYTES p_size,	/* memory for the actual structure */	\
	__TMS_UPLATFORM *,			/* from app: the platform structure */	\
	__TMS_UHIL_BUSHANDLE, __TMS_IOPORT,	/* from app: the chip addr XXX */	\
	__TMS_UDEVICE_INITFN *,			/* from USBRC */			\
	__TMS_CONST __TMS_USBRC_ROOTTABLE *,	/* from USBRC */			\
	__TMS_UINT32 DebugFlags,		/* from app */				\
	__TMS_CONST __TMS_VOID *pConfigData, __TMS_BYTES nConfigData			\
						/* config info -- chip */		\
						/*  specific. */			\
	));

__TMS_FNTYPE_DEF(
UDEVREPORTEVENTFN,
__TMS_VOID,
	(
	__TMS_UDEVICE *,
	__TMS_ARG_UEVENT,
	__TMS_VOID *
	));

__TMS_FNTYPE_DEF(
UDEVREPLYFN,
__TMS_BYTES,
	(
	__TMS_UDEVICE *,
	__TMS_CONST __TMS_VOID * /*buf*/,
	__TMS_BYTES /*lenRequested*/, __TMS_BYTES /*lenActual*/,
	__TMS_UBUFIODONEFN * /*pDoneFn*/, __TMS_VOID * /*pDoneCtx*/,
	__TMS_UBUFQE_FLAGS /*flags*/
	));

__TMS_FNTYPE_DEF(
UDEVSTARTFN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *
		)
	);

__TMS_FNTYPE_DEF(
UDEVSTOPFN,
	__TMS_VOID,
		(__TMS_UDEVICE *)
	);

__TMS_FNTYPE_DEF(
UDEVREMOTEWAKEUPFN,
	__TMS_VOID,
		(__TMS_UDEVICE *)
	);

__TMS_FNTYPE_DEF(
UDEVALLOCATEFN,
	__TMS_VOID *,
		(__TMS_UDEVICE *, __TMS_BYTES)
	);

__TMS_FNTYPE_DEF(
UDEVFREEFN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *,
		__TMS_VOID *,
		__TMS_BYTES
		)
	);

__TMS_USBPUMP_DECLARE_IOCTL_FNTYPE(
UDEVIOCTLFN,
	__TMS_UDEVICE *
	);

__TMS_FNTYPE_DEF(
UDEVGETUDEVICESIZEFN,
	__TMS_BYTES,
		(
		__TMS_UDEVICE *		/* pDevice */
		)
	);

__TMS_FNTYPE_DEF(
UDEVGETUENDPOINTSIZEFN,
	__TMS_BYTES,
		(
		__TMS_UDEVICE *		/* pDevice */
		)
	);

__TMS_FNTYPE_DEF(
UDEVENDPOINTINITFN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *		/* pDevice */,
		__TMS_UPIPE *		/* pPipe */,
		__TMS_UENDPOINT *	/* pEndpoint */,
		__TMS_UCHAR		/* EndpointId */
		)
	);

__TMS_FNTYPE_DEF(
UDEVGETFRAMENUMBERFN,
	__TMS_VOID,
		(
		__TMS_UDEVICE *			/* pDevice */,
		__TMS_USBPUMP_FRAME_NUMBER *	/* pFrameNumber */
		)
	);

struct TTUSB_UDEVICESWITCH
	{
	__TMS_UDEVINITIALIZEFN		*udevsw_Initialize;
	__TMS_UDEVSTARTFN		*udevsw_Start;
	__TMS_UDEVSTOPFN		*udevsw_Stop;
	__TMS_UDEVREPORTEVENTFN		*udevsw_ReportEvent;
	__TMS_UDEVREPLYFN		*udevsw_Reply;
	__TMS_UDEVREMOTEWAKEUPFN	*udevsw_RemoteWakeup;
	__TMS_UDEVALLOCATEFN		*udevsw_AllocateDeviceBuffer;
	__TMS_UDEVFREEFN		*udevsw_FreeDeviceBuffer;
	__TMS_UDEVIOCTLFN		*udevsw_Ioctl;
	__TMS_UDEVGETUDEVICESIZEFN	*udevsw_GetUdeviceSize;
	__TMS_UDEVGETUENDPOINTSIZEFN	*udevsw_GetUendpointSize;
	__TMS_UDEVENDPOINTINITFN	*udevsw_InitUendpoint;
	__TMS_UDEVGETFRAMENUMBERFN	*udevsw_GetFrameNumber;
	};

/*
|| To initialize:
||	In theory, pool allocation setup info (which is device specific)
||	is passed through (configdata); so is the wiring info.  Note the
||	change in philosophy compared to V1.  The old APIs still exist, but
||	as stubs that call this routine.
*/
#define	__TMS_UDEVINITIALIZE(pSwitch, \
		       self, size, \
		       platform, bushandle, busport, initfn, root, debug, \
		       configdata, size_config) \
	((*(pSwitch)->udevsw_Initialize)( \
		(pSwitch), (self), (size), \
		(platform), (bushandle), (busport), \
		(initfn), (root), (debug), \
		(configdata), (size_config)))

/*
|| To report a device event:
*/
#define	__TMS_UDEVREPORTEVENT(self, event, arg) \
	((*(self)->udev_pSwitch->udevsw_ReportEvent)((self), (event), (arg)))

/*
|| To send a reply to the current control transaction, use
||	UDEVREPLY_V2(self, buf, lenReq, lenActual, donefn, doneinfo, flags);
|| to fail the current control transaction, use
||	UDEVREPLY_V2(self, __TMS_NULL, 0, 0, donefn, doneinfo, 0);
|| This function is mandatory.
*/
#define	__TMS_UDEVREPLY(self, buf, len) \
	__TMS_UDEVREPLY_V2(self, buf, len, len, __TMS_NULL, __TMS_NULL, 0)

#define	__TMS_UDEVREPLY_V2(self, buf, lenReq, lenActual, donefn, doneinfo, flags) \
		((*(self)->udev_pSwitch->udevsw_Reply)( \
			(self), (buf), (lenReq), (lenActual), \
			(donefn), (doneinfo), (flags) \
			))

/*
|| To start the device up:
*/
#define	__TMS_UDEVSTART(self) do { \
	if ((self)->udev_pSwitch->udevsw_Start) \
		{ \
		(*(self)->udev_pSwitch->udevsw_Start)((self)); \
		} \
	} while (0)

/*
|| To request that the device stop:
*/
#define __TMS_UDEVSTOP(self) do { \
	if ((self)->udev_pSwitch->udevsw_Stop) \
		(*(self)->udev_pSwitch->udevsw_Stop)((self)); \
	} while (0)

/*
|| To request a remote wakeup:
*/
#define	__TMS_UDEVREMOTEWAKEUP(self)	\
	__TMS_UDEVREMOTEWAKEUP_V2(self, __TMS_NULL)

#define	__TMS_UDEVREMOTEWAKEUP_V2(self, interface)		\
    do	{							\
	if (UDEVICE_IS_REMOTE_WAKEUP_VALID_V2(self, interface))	\
		{						\
		(self)->udev_pFunctionWakeIfc = (interface);	\
		UsbPumpDevice_DeviceFsm_evRemoteWakeup(self);	\
		}						\
	} while (0)

#define	__TMS_UDEVREMOTEWAKEUP_DIRECT(self) do { \
	if ((self)->udev_pSwitch->udevsw_RemoteWakeup) \
		(*(self)->udev_pSwitch->udevsw_RemoteWakeup)((self)); \
	} while (0)

/*
|| To allocate a packet that is guaranteed OK for data transfer:
*/
#define __TMS_UDEVALLOCATE(self, nbytes) \
	(((self)->udev_pSwitch->udevsw_AllocateDeviceBuffer != __TMS_NULL) \
		? (*(self)->udev_pSwitch->udevsw_AllocateDeviceBuffer)( \
			(self), (nbytes) \
			) \
		: __TMS_NULL)

/*
|| To release a packet allocated by UDEVALLOCATE
*/
#define __TMS_UDEVFREE(self, pBuf, nbytes) do { \
	if ((self)->udev_pSwitch->udevsw_FreeDeviceBuffer) \
		(*(self)->udev_pSwitch->udevsw_FreeDeviceBuffer)( \
			(self), (pBuf), (nbytes) \
			); \
	} while (0)

#define __TMS_UDEVIOCTL(self, Ioctl, pInParam, pOutParam)		\
	(((self)->udev_pSwitch->udevsw_Ioctl != __TMS_NULL)		\
		? (*(self)->udev_pSwitch->udevsw_Ioctl)(		\
			(self), (Ioctl), (pInParam), (pOutParam)	\
			)						\
		: __TMS_USBPUMP_IOCTL_RESULT_NOT_CLAIMED		\
	)

#define __TMS_UDEVICE_GET_UDEVICE_SIZE(pSelf)				\
	(((pSelf)->udev_pSwitch->udevsw_GetUdeviceSize != __TMS_NULL)	\
		? (*(pSelf)->udev_pSwitch->udevsw_GetUdeviceSize)(	\
			(pSelf)						\
			)						\
		: sizeof(*pSelf)					\
	)

#define __TMS_UDEVICE_GET_UENDPOINT_SIZE(pSelf)				\
	(((pSelf)->udev_pSwitch->udevsw_GetUendpointSize != __TMS_NULL)	\
		? (*(pSelf)->udev_pSwitch->udevsw_GetUendpointSize)(	\
			(pSelf)						\
			)						\
		: sizeof(__TMS_UENDPOINT)				\
	)

#define __TMS_UDEVICE_ENDPOINT_INIT(pSelf, pPipe, pEndpoint, EndpointId)\
    do	{								\
	if ((pSelf)->udev_pSwitch->udevsw_InitUendpoint)		\
		(*(pSelf)->udev_pSwitch->udevsw_InitUendpoint)(		\
			(pSelf),					\
			(pPipe),					\
			(pEndpoint),					\
			(EndpointId)					\
			);						\
	} while (0)

#define __TMS_UDEVICE_GET_FRAME_NUMBER(pSelf, pFrameNumber)		\
    do	{								\
	if ((pSelf)->udev_pSwitch->udevsw_GetFrameNumber)		\
		{							\
		(*(pSelf)->udev_pSwitch->udevsw_GetFrameNumber)(	\
			(pSelf),					\
			(pFrameNumber)					\
			);						\
		}							\
	else								\
		{							\
		(pFrameNumber)->StandardFrame = 0;			\
		(pFrameNumber)->NativeFrame = 0;			\
		(pFrameNumber)->Numerator = 1;				\
		(pFrameNumber)->Denominator = 1;			\
		}							\
	} while (0)

/*
|| The device switch initailization functions
*/
#define __TMS_UDEVICESWITCH_INIT(report, reply) \
	__TMS_UDEVICESWITCH_INIT_V2(report, reply, __TMS_NULL, __TMS_NULL)

#define __TMS_UDEVICESWITCH_INIT_V2(report, reply, start, stop) \
	__TMS_UDEVICESWITCH_INIT_V3(report, reply, start, stop, __TMS_NULL)

#define	__TMS_UDEVICESWITCH_INIT_V3(report, reply, start, stop, remoteWakeup) \
	__TMS_UDEVICESWITCH_INIT_V4(__TMS_NULL, start, stop, report, reply, \
		remoteWakeup, \
		__TMS_NULL, \
		__TMS_NULL \
		)

#define	__TMS_UDEVICESWITCH_INIT_V4(init, start, stop, report, reply,	\
			      remotewakeup, allocate, free)		\
	__TMS_UDEVICESWITCH_INIT_V5(init, start, stop, report, reply,	\
			      remotewakeup, allocate, free, __TMS_NULL)

#define	__TMS_UDEVICESWITCH_INIT_V5(init, start, stop, report, reply,	\
			      remotewakeup, allocate, free, ioctl)	\
	__TMS_UDEVICESWITCH_INIT_V6(					\
		init,							\
		start,							\
		stop,							\
		report,							\
		reply,							\
		remotewakeup,						\
		allocate,						\
		free,							\
		ioctl,							\
		/* V6: getUdeviceSize */ __TMS_NULL,			\
		/* V6: getUendpointSize */ __TMS_NULL,			\
		/* V6: uendpointInit */ __TMS_NULL			\
		)

#define	__TMS_UDEVICESWITCH_INIT_V6(					\
		init,							\
		start,							\
		stop,							\
		report,							\
		reply,							\
		remotewakeup,						\
		allocate,						\
		free,							\
		ioctl,							\
		getUdeviceSize,						\
		getUendpointSize,					\
		uendpointInit						\
		)							\
	__TMS_UDEVICESWITCH_INIT_V7(					\
		init,							\
		start,							\
		stop,							\
		report,							\
		reply,							\
		remotewakeup,						\
		allocate,						\
		free,							\
		ioctl,							\
		getUdeviceSize,						\
		getUendpointSize,					\
		uendpointInit,						\
		/* V7: getframenumber */ __TMS_NULL			\
		)

#define	__TMS_UDEVICESWITCH_INIT_V7(					\
		init,							\
		start,							\
		stop,							\
		report,							\
		reply,							\
		remotewakeup,						\
		allocate,						\
		free,							\
		ioctl,							\
		getUdeviceSize,						\
		getUendpointSize,					\
		uendpointInit,						\
		getframenumber						\
		)							\
	{								\
	(init),								\
	(start),							\
	(stop),								\
	(report),							\
	(reply),							\
	(remotewakeup),							\
	(allocate),							\
	(free),								\
	(ioctl),							\
	(getUdeviceSize),						\
	(getUendpointSize),						\
	(uendpointInit),						\
	(getframenumber)						\
	}

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UDEVINITIALIZE(pSwitch, \
		       self, size, \
		       platform, bushandle, busport, initfn, root, debug, \
		       configdata, size_config)	\
	__TMS_UDEVINITIALIZE(pSwitch, \
		       self, size, \
		       platform, bushandle, busport, initfn, root, debug, \
		       configdata, size_config)
# define UDEVREPORTEVENT(self, event, arg)	\
   __TMS_UDEVREPORTEVENT(self, event, arg)
# define UDEVREPLY(self, buf, len)	\
   __TMS_UDEVREPLY(self, buf, len)
# define UDEVREPLY_V2(self, buf, lenReq, lenActual, donefn, doneinfo, flags)	\
   __TMS_UDEVREPLY_V2(self, buf, lenReq, lenActual, donefn, doneinfo, flags)
# define UDEVSTART(self)	\
   __TMS_UDEVSTART(self)
# define UDEVSTOP(self)	\
   __TMS_UDEVSTOP(self)
# define UDEVREMOTEWAKEUP(self)	\
   __TMS_UDEVREMOTEWAKEUP(self)
# define UDEVREMOTEWAKEUP_V2(self, interface)	\
   __TMS_UDEVREMOTEWAKEUP_V2(self, interface)
# define UDEVREMOTEWAKEUP_DIRECT(self)	\
   __TMS_UDEVREMOTEWAKEUP_DIRECT(self)
# define UDEVALLOCATE(self, nbytes)	\
   __TMS_UDEVALLOCATE(self, nbytes)
# define UDEVFREE(self, pBuf, nbytes)	\
   __TMS_UDEVFREE(self, pBuf, nbytes)
# define UDEVIOCTL(self, Ioctl, pInParam, pOutParam)	\
   __TMS_UDEVIOCTL(self, Ioctl, pInParam, pOutParam)
# define UDEVICE_GET_UDEVICE_SIZE(pSelf)	\
   __TMS_UDEVICE_GET_UDEVICE_SIZE(pSelf)
# define UDEVICE_GET_UENDPOINT_SIZE(pSelf)	\
   __TMS_UDEVICE_GET_UENDPOINT_SIZE(pSelf)
# define UDEVICE_ENDPOINT_INIT(pSelf, pPipe, pEndpoint, EndpointId)	\
   __TMS_UDEVICE_ENDPOINT_INIT(pSelf, pPipe, pEndpoint, EndpointId)
# define UDEVICE_GET_FRAME_NUMBER(pSelf, pFrameNumber)	\
   __TMS_UDEVICE_GET_FRAME_NUMBER(pSelf, pFrameNumber)
# define UDEVICESWITCH_INIT(report, reply)	\
   __TMS_UDEVICESWITCH_INIT(report, reply)
# define UDEVICESWITCH_INIT_V2(report, reply, start, stop)	\
   __TMS_UDEVICESWITCH_INIT_V2(report, reply, start, stop)
# define UDEVICESWITCH_INIT_V3(report, reply, start, stop, remoteWakeup)	\
   __TMS_UDEVICESWITCH_INIT_V3(report, reply, start, stop, remoteWakeup)
# define UDEVICESWITCH_INIT_V4(init, start, stop, report, reply,	\
			      remotewakeup, allocate, free)	\
	__TMS_UDEVICESWITCH_INIT_V4(init, start, stop, report, reply,	\
			      remotewakeup, allocate, free)
# define UDEVICESWITCH_INIT_V5(init, start, stop, report, reply,	\
			      remotewakeup, allocate, free, ioctl)	\
	__TMS_UDEVICESWITCH_INIT_V5(init, start, stop, report, reply,	\
			      remotewakeup, allocate, free, ioctl)
# define UDEVICESWITCH_INIT_V6(					\
		init,							\
		start,							\
		stop,							\
		report,							\
		reply,							\
		remotewakeup,						\
		allocate,						\
		free,							\
		ioctl,							\
		getUdeviceSize,						\
		getUendpointSize,					\
		uendpointInit						\
		)	\
	__TMS_UDEVICESWITCH_INIT_V6(					\
		init,							\
		start,							\
		stop,							\
		report,							\
		reply,							\
		remotewakeup,						\
		allocate,						\
		free,							\
		ioctl,							\
		getUdeviceSize,						\
		getUendpointSize,					\
		uendpointInit						\
		)
# define UDEVICESWITCH_INIT_V7(					\
		init,							\
		start,							\
		stop,							\
		report,							\
		reply,							\
		remotewakeup,						\
		allocate,						\
		free,							\
		ioctl,							\
		getUdeviceSize,						\
		getUendpointSize,					\
		uendpointInit,						\
		getframenumber						\
		)	\
	__TMS_UDEVICESWITCH_INIT_V7(					\
		init,							\
		start,							\
		stop,							\
		report,							\
		reply,							\
		remotewakeup,						\
		allocate,						\
		free,							\
		ioctl,							\
		getUdeviceSize,						\
		getUendpointSize,					\
		uendpointInit,						\
		getframenumber						\
		)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of udevswitch.h ****/
#endif /* _UDEVSWITCH_H_ */
