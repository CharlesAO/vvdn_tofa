/* usbpump_usbdi_usbd.h	Fri Feb 24 2017 17:08:51 chwon */

/*

Module:  usbpump_usbdi_usbd.h

Function:
	The definitions for USBPUMP_USBDI_USBD and friends.

Version:
	V3.19b	Fri Feb 24 2017 17:08:51 chwon	Edit level 28

Copyright notice:
	This file copyright (C) 2005-2012, 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	August 2005

Revision history:
   1.91h  Sat Aug 13 2005 20:07:11  tmm
	Module created.

   1.93c  Fri Dec 02 2005 13:48:08  chwon
	Fixed macro USBPUMP_USBDI_USBD_CONFIG_INIT_V1 definition.

   1.93e  Fri Feb 24 2006 20:26:03  tmm
	Add #ports and urb max size to configuration.

   1.97a  Thu Apr  6 2006 14:26:43  tmm
	Fix cloaking error in the USBPUMP_USBDI_USBD_CONFIG_INIT_V2 definition.
	Recloaked everything.

   1.97k  Wed Mar 05 2008 13:45:37  chwon
	3567: add AnnunciatorMaxSession in the configuration.

   1.97k  Tue Jan 20 2009 14:30:58  chwon
	7275: add ulDebugFlags in the USBD configuration

   2.01a  Thu May 14 2009 16:25:15  chwon
	Fix cloaking error in the USBPUMP_USBDI_USBD_CONTENTS

   2.01a  Mon Dec 14 2009 13:03:50  chwon
	9569: add HIRD parameter in the USBD configuration

   3.01a  Mon Dec 21 2009 13:00:48  chwon
	9569: fix UTF-8 special character in the comment

   3.01c  Fri Mar 26 2010 12:26:17  chwon
	10075: add tStdRequestMinimum parameter in the USBD configuration.

   3.01c  Tue May 04 2010 15:20:04  chwon
	9569: change HIRD to be expressed in real-world units (usec),
	not the crazy encoding in the spec.

   3.01c  Fri Aug 27 2010 11:48:09  chwon
	10987: add tAutoLpmIdleTime and Flags in the configuration.

   3.01c  Wed Sep 01 2010 15:31:34  chwon
	11136: add USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1_U2 and
	USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_LTM flags.

   3.01c  Thu Sep 16 2010 09:44:03  chwon
	11285: add USBPUMP_USBDI_USBD_CONFIG_FLAG_DONT_VALIDATE_SERIAL_NUM flag

   3.01d  Thu Oct 07 2010 15:14:23  chwon
	11575: add USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_xxx flags.

   3.01d  Mon Oct 11 2010 09:45:35  chwon
	11609: add USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_HUB_AUTO_SUSPEND.

   3.01d  Sat Nov 06 2010 12:21:59  chwon
	11725: add USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SLEEP.

   3.01d  Tue Jan 04 2011 15:37:28  chwon
	12064: split USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1_U2 to
	USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1 and ..._U2.

   3.01d  Mon Jan 24 2011 15:44:42  chwon
	12064: add bMinU1TimeoutEncode and bMinU2TimeoutEncode in the config.

   3.01d  Thu Mar 17 2011 11:38:10  chwon
	12871: add bHubU1TimeoutEncode and bHubU2TimeoutEncode in the config.

   3.01d  Fri Mar 25 2011 15:42:54  chwon
	13048: add USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SHUTDOWN.

   3.01d  Tue Apr 05 2011 14:29:21  chwon
	13048: change USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SHUTDOWN
	to USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_RESTART.

   3.01f  Mon May 02 2011 10:30:14  luxw
	13284: Added USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SHUTDOWN.

   3.01f  Thu Sep 01 2011 12:27:57  chwon
	13501: Added USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_HARDWARE_LPM and
	USBPUMP_USBDI_USBD_CONFIG_FLAG_HARDWARE_LPM_MODE_1 flags.

   3.01f  Thu May 31 2012 15:52:32  chwon
	14821: Added LPM errata BESL support -- update document and change
	HIRD default value. Added ..._CONFIG_FLAG_DONT_LPM_IF_MISMATCH_BESL.

   3.19b  Fri Feb 24 2017 17:08:51  chwon
	20480: Add timer values which use for Windows driver.

*/

#ifndef _USBPUMP_USBDI_USBD_H_		/* prevent multiple includes */
#define _USBPUMP_USBDI_USBD_H_

#ifndef _USBPUMP_USBDI_TYPES_H_
# include "usbpump_usbdi_types.h"
#endif

#ifndef _USBPUMPOBJECT_H_
# include "usbpumpobject.h"
#endif

/****************************************************************************\
|
|	The USBDI_USBD object is used to represent USBD to itself
|
\****************************************************************************/

__TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_USBD_CONTENTS);

struct __TMS_STRUCTNAME(USBPUMP_USBDI_USBD_CONTENTS)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__STRUCT;

	/* public USBD contents go here */
	__TMS_UPLATFORM	*pPlatform;
	};

union __TMS_UNIONNAME(USBPUMP_USBDI_USBD)
	{
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;
	__TMS_USBPUMP_USBDI_USBD_CONTENTS	Usbd;
	};

/*
|| Inheritance macros, but only for use by implementation code.
*/
#define __TMS_USBPUMP_USBDI_USBD_CONTENTS__STRUCT		\
	__TMS_USBPUMP_USBDI_USBD_CONTENTS	Usbd

#define	__TMS_USBPUMP_USBDI_USBD_CONTENTS__UNION		\
	__TMS_USBPUMP_OBJECT_CONTENTS__UNION;			\
	__TMS_USBPUMP_USBDI_USBD_CONTENTS	Usbd;		\
	__TMS_USBPUMP_USBDI_USBD		UsbdCast


/*
|| Object-related things
*/

/* the USBD object tag */
#define	__TMS_USBPUMP_USBDI_USBD_TAG	__TMS_UHIL_MEMTAG('U', 'S', 'B', 'D')

/* 
|| the default name -- something like x.usbd.usbdi.mcci.com, e.g.
||	"local.usbd.usbdi.mcci.com" or "proxy.usbd.usbdi.mcci.com";
||	match against USBPUMP_USBDI_NAME("*").
*/
#define	__TMS_USBPUMP_USBDI_USBD_NAME(x)	\
	__TMS_USBPUMP_USBDI_NAME(x ".usbd")

/*
|| the metaclass name for USBD.
*/
#define	__TMS_USBPUMP_USBDI_USBD_METACLASS_NAME			\
	__TMS_USBPUMP_USBDI_NAME("usbd.class")

/****************************************************************************\
|
|	The USBD Configuration object
|
\****************************************************************************/

/*
|| in usbpump_usbdi_types.h:
|| __TMS_TYPE_DEF_STRUCT(USBPUMP_USBDI_USBD_CONFIG);
*/

struct __TMS_STRUCTNAME(USBPUMP_USBDI_USBD_CONFIG)
	{
	__TMS_UINT32			MagicBegin;
	__TMS_BYTES			Size;
	__TMS_CONST __TMS_TEXT *	pUsbdName;
	__TMS_BYTES			maxNestedCompletions;
	__TMS_UINT16			sizeConfigBuffer;
	__TMS_UINT16			sizeStringDescBuffer;

	/*
	|| minimal USBDI uses this to increase the size for
	|| the HUB urbs... this is in addition to the HCD request
	|| bytes....
	*/
	__TMS_BYTES			maxUrbExtraBytes;

	/*
	|| timing constants
	*/
	__TMS_UINT16			tAttachDebounce;
	__TMS_UINT16			tResetRecovery;
	__TMS_UINT16			tSetAddrCompletion;
	__TMS_UINT16			tSetAddrRecovery;
	__TMS_UINT16			tStdRequestNoData;
	__TMS_UINT16			tStdRequestData1;
	__TMS_UINT16			tStdRequestDataN;
	__TMS_UINT16			tStdRequestMinimum;
	__TMS_UINT16			tResumeRecovery;

	/*
	|| We need to provide for a certain number of hubs: we
	|| initialize the hub class automatically, and we use this
	|| plan for the number of non-root ports.
	*/
	__TMS_UINT8			bNumberHubs;
	__TMS_UINT8			bPortsPerHub;

	/*
	|| USB3 U1/U2 timeout minimum value. If calculated U1/U2 timeout value
	|| is less than this value, USBD will use specified minimum U1/U2
	|| timeout value.
	*/
	__TMS_UINT8			bMinU1TimeoutEncode;
	__TMS_UINT8			bMinU2TimeoutEncode;

	/*
	|| USB3 U1/U2 timeout value for hub device. If connected device is hub
	|| and bHubU1TimeoutEncode is zero, hub driver will set U1 timeout
	|| value by using bMinU1TimeoutEncode. If bHubU1TimeoutEncode is not
	|| zero, hub driver will use this value as minimum U1 timeout value.
	|| If connected device is hub and bHubU2TimeoutEncode is zero, hub
	|| driver will not set U2 timeout. If bHubU2TimeoutEncode is not zero,
	|| hub driver will set U2 timeout with this value.
	*/
	__TMS_UINT8			bHubU1TimeoutEncode;
	__TMS_UINT8			bHubU2TimeoutEncode;

	/*
	|| HIRD (Host Initiated Resume Duration) in microseconds for LPM
	*/
	__TMS_UINT			tHostInitiatedResumeDuration;

	/*
	|| automatic LPM mode idle time in microseconds
	*/
	__TMS_UINT			tAutoLpmIdleTime;

	/*
	|| The caller may optionally override the "generic" match
	|| IDs that we normally use.
	*/
	__TMS_CONST __TMS_USBPUMP_USBDI_INIT_MATCH_LIST
					*pHubIdOverrides;

	/*
	|| USBD annunciator object info -- number of max annunciator session
	*/
	__TMS_UINT			AnnunciatorMaxSession;

	/*
	|| the debug flags
	*/
	__TMS_UINT32			ulDebugFlags;

	/*
	|| configuration flags
	*/
	__TMS_UINT32			Flags;

	__TMS_UINT32			MagicEnd;
	};

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_MAGIC_BEGIN			\
	__TMS_UHIL_MEMTAG('<', 'U', 'c', 'f')

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_MAGIC_END			\
	__TMS_UHIL_MEMTAG('>', 'U', 'c', 'f')

/*
|| table 7-14, page 188: attach debounce time
*/
#define	__TMS_USBPUMP_USB20_TATTDB_SPEC		100
#define	__TMS_USBPUMP_USB20_TATTDB_DEFAULT	150

/*
|| table 7-14, page 188: reset recovery time
*/
#define	__TMS_USBPUMP_USB20_TRSTRCY_SPEC	10
#define	__TMS_USBPUMP_USB20_TRSTRCY_DEFAULT	50
#define	__TMS_USBPUMP_USB20_TRSTRCY_WINDOWS	100

/*
|| 9.2.6.3, page 246:  set address recovery time
*/
#define	__TMS_USBPUMP_USB20_TSETADDRRCY_SPEC	2
#define	__TMS_USBPUMP_USB20_TSETADDRRCY_DEFAULT	10
#define	__TMS_USBPUMP_USB20_TSETADDRRCY_WINDOWS	60

/*
|| table 7-14, page 188: Set address completion time
*/
#define	__TMS_USBPUMP_USB20_TDSETADDR_SPEC	50
#define	__TMS_USBPUMP_USB20_TDSETADDR_DEFAULT	100

/*
|| table 7-14, page 188: Resume recovery time
*/
#define	__TMS_USBPUMP_USB20_TRSMRCY_SPEC	10
#define	__TMS_USBPUMP_USB20_TRSMRCY_DEFAULT	20
#define	__TMS_USBPUMP_USB20_TRSMRCY_WINDOWS	40

/*
|| table 7-14, page 189: Time to complete std request with no data
*/
#define	__TMS_USBPUMP_USB20_TDRQCMPLTND_SPEC	50
#define	__TMS_USBPUMP_USB20_TDRQCMPLTND_DEFAULT	100

/*
|| table 7-14, page 189: time to deliver first and intermediate data 
|| packets for standard request
*/
#define	__TMS_USBPUMP_USB20_TDRETDATA1_SPEC	500
#define	__TMS_USBPUMP_USB20_TDRETDATA1_DEFAULT	600

/*
|| table 7-14, page 189: time to deliver last data/status for standard
|| request.
*/
#define	__TMS_USBPUMP_USB20_TDRETDATAN_SPEC	50
#define	__TMS_USBPUMP_USB20_TDRETDATAN_DEFAULT	60

/*
|| Microsoft host stack use 5 seconds for standard request.
*/
#define	__TMS_USBPUMP_USB20_TSTDREQUEST_MINIMUM_DEFAULT	5000	/* 5 seconds */
#define	__TMS_USBPUMP_USB20_TSTDREQUEST_MINIMUM_WINDOWS	5000	/* 5 seconds */

/*
|| USB 2.0 Extension for LPM: HIRD parameter.
|| The HIRD value is a 4-bit encoded value. Table X-X1. BESL & HIRD Encodings
||	+-------+----------+----------+
||	| Value | BESL(us) | HIRD(us) |
||	+-------+----------+----------+
||	| 0     |      125 |       75 |
||	| 1     |      150 |      100 |
||	| 2     |      200 |      150 |
||	| 3     |      300 |      250 |
||	| 4     |      400 |      350 |
||	| 5     |      500 |      450 |
||	| 6     |     1000 |      950 |
||	| 7     |     2000 |     1950 |
||	| 8     |     3000 |     2950 |
||	| 9     |     4000 |     3950 |
||	| 10    |     5000 |     4950 |
||	| 11    |     6000 |     5950 |
||	| 12    |     7000 |     6950 |
||	| 13    |     8000 |     7950 |
||	| 14    |     9000 |     8950 |
||	| 15    |    10000 |     9950 |
||	+-------+----------+----------+
||
|| The duration of the resume signaling is specified by the host in a parameter
|| called the HIRD (Host Initiated Resume Duration) and is in the range between
|| TL1HubDrvResume1(min) and TL1HubDriveResume1(max).
||
|| USB2_LinkPowerManagement_ECN: table 4-6, page 23:
||   Parameter		Min	Max	Units
||   TL1HubDrvResume1	75	9960	us
||   TL1HubDrvResume2	60	990	us
*/
#define	__TMS_USBPUMP_USB20_LPM_HIRD_DEFAULT	3950	/* us */

/*
|| Default value for the automatic LPM mode idle time in microseconds.
*/
#define	__TMS_USBPUMP_USBDI_AUTO_LPM_IDLE_TIME_DEFAULT	50000	/* us */

/*
|| USBDI USBD configration flags
*/
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_AUTO_LPM		(1u << 0)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1			(1u << 1)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U2			(1u << 2)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1_U2		\
	(__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1 |		\
	 __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U2)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_LTM			(1u << 3)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_DONT_VALIDATE_SERIAL_NUM	(1u << 4)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_CONNECTION		(1u << 5)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_DISCONNECTION	(1u << 6)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_OVERCURRENT	(1u << 7)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_HUB_AUTO_SUSPEND	(1u << 8)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SLEEP	(1u << 9)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_RESTART (1u << 10)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SHUTDOWN (1u << 11)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_HARDWARE_LPM	(1u << 12)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_HARDWARE_LPM_MODE_1	(1u << 13)
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_DONT_LPM_IF_MISMATCH_BESL	(1u << 14)


/*
|| USBDI USBD configration compile-time / run-time initialization macros
*/
#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V1(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer						\
	)								\
    __TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V2(				\
		(a_pUsbdName),						\
		(a_maxNestedCompletions),				\
		(a_sizeConfigBuffer),					\
		/* serial buf size */ 0,				\
		/* urb extra bytes */ 0,				\
		__TMS_USBPUMP_USB20_TATTDB_DEFAULT,			\
		__TMS_USBPUMP_USB20_TRSTRCY_DEFAULT,			\
		__TMS_USBPUMP_USB20_TDSETADDR_DEFAULT,			\
		__TMS_USBPUMP_USB20_TSETADDRRCY_DEFAULT,		\
		__TMS_USBPUMP_USB20_TDRQCMPLTND_DEFAULT,		\
		__TMS_USBPUMP_USB20_TDRETDATA1_DEFAULT,			\
		__TMS_USBPUMP_USB20_TDRETDATAN_DEFAULT,			\
		__TMS_USBPUMP_USB20_TRSMRCY_DEFAULT,			\
		/* number of hubs */ 2,					\
		/* number of ports per hub */ 7,			\
		/* hub id overrides */ __TMS_NULL			\
		)

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V1(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer						\
	)								\
    __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V2(				\
		(a_pConfig),						\
		(a_pUsbdName),						\
		(a_maxNestedCompletions),				\
		(a_sizeConfigBuffer),					\
		/* serial buf size */ 0,				\
		/* urb extra bytes */ 0,				\
		__TMS_USBPUMP_USB20_TATTDB_DEFAULT,			\
		__TMS_USBPUMP_USB20_TRSTRCY_DEFAULT,			\
		__TMS_USBPUMP_USB20_TDSETADDR_DEFAULT,			\
		__TMS_USBPUMP_USB20_TSETADDRRCY_DEFAULT,		\
		__TMS_USBPUMP_USB20_TDRQCMPLTND_DEFAULT,		\
		__TMS_USBPUMP_USB20_TDRETDATA1_DEFAULT,			\
		__TMS_USBPUMP_USB20_TDRETDATAN_DEFAULT,			\
		__TMS_USBPUMP_USB20_TRSMRCY_DEFAULT,			\
		/* number of hubs */ 2,					\
		/* number of ports per hub */ 7,			\
		/* hub id overrides */ __TMS_NULL			\
		)

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V2(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides						\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V3(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	/* V3: AnnunciatorMaxSession */	0				\
	)

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V2(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides						\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V3(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	/* V3: AnnunciatorMaxSession */	0				\
	)

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V3(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession						\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V4(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	/* V4: ulDebugFlags */ 0	/* Inherit from parent */	\
	)

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V3(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession						\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V4(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	/* V4: ulDebugFlags */ 0	/* Inherit from parent */	\
	)

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V4(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V5(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	/* V5: tHostInitiatedResumeDuration */				\
		__TMS_USBPUMP_USB20_LPM_HIRD_DEFAULT,			\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V4(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V5(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	/* V5: tHostInitiatedResumeDuration */				\
		__TMS_USBPUMP_USB20_LPM_HIRD_DEFAULT,			\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V5(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V6(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	/* V6: tStdRequestMinimum */ 0,	/* turn off this feature */	\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V5(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V6(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	/* V6: tStdRequestMinimum */ 0,	/* turn off this feature */	\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V6(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V7(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	/* V7: tAutoLpmIdleTime */ __TMS_USBPUMP_USBDI_AUTO_LPM_IDLE_TIME_DEFAULT, \
	/* V7: Flags */	0						\
	)

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V6(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V7(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	/* V7: tAutoLpmIdleTime */ __TMS_USBPUMP_USBDI_AUTO_LPM_IDLE_TIME_DEFAULT, \
	/* V7: Flags */	0						\
	)

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V7(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags								\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V8(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	/* V8: bMinU1TimeoutEncode */ 0,				\
	/* V8: bMinU2TimeoutEncode */ 0					\
	)

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V7(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags								\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V8(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	/* V8: bMinU1TimeoutEncode */ 0,				\
	/* V8: bMinU2TimeoutEncode */ 0					\
	)

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V8(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode						\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V9(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode,						\
	/* V9: bHubU1TimeoutEncode */ 0,				\
	/* V9: bHubU2TimeoutEncode */ 0	/* don't set hub U2 timeout */	\
	)

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V8(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode						\
	)								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V9(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode,						\
	/* V9: bHubU1TimeoutEncode */ 0,				\
	/* V9: bHubU2TimeoutEncode */ 0	/* don't set hub U2 timeout */	\
	)

#define	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V9(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode,						\
	a_bHubU1TimeoutEncode,						\
	a_bHubU2TimeoutEncode						\
	)								\
	{								\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_MAGIC_BEGIN,			\
	sizeof(__TMS_USBPUMP_USBDI_USBD_CONFIG),			\
	(a_pUsbdName),							\
	(a_maxNestedCompletions),					\
	(a_sizeConfigBuffer),						\
	(a_sizeStringDescBuffer),					\
	(a_maxUrbExtraBytes),						\
	(a_tAttachDebounce),						\
	(a_tResetRecovery),						\
	(a_tSetAddrCompletion),						\
	(a_tSetAddrRecovery),						\
	(a_tStdRequestNoData),						\
	(a_tStdRequestData1),						\
	(a_tStdRequestDataN),						\
	(a_tStdRequestMinimum),						\
	(a_tResumeRecovery),						\
	(a_bNumberHubs),						\
	(a_bPortsPerHub),						\
	(a_bMinU1TimeoutEncode),					\
	(a_bMinU2TimeoutEncode),					\
	(a_bHubU1TimeoutEncode),					\
	(a_bHubU2TimeoutEncode),					\
	(a_tHostInitiatedResumeDuration),				\
	(a_tAutoLpmIdleTime),						\
	(a_pHubIdOverrides),						\
	(a_AnnunciatorMaxSession),					\
	(a_ulDebugFlags),						\
	(a_Flags),							\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_MAGIC_END			\
	}

#define __TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V9(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode,						\
	a_bHubU1TimeoutEncode,						\
	a_bHubU2TimeoutEncode						\
	)								\
   do	{								\
	(a_pConfig)->MagicBegin =					\
		__TMS_USBPUMP_USBDI_USBD_CONFIG_MAGIC_BEGIN;		\
	(a_pConfig)->Size = sizeof(*(a_pConfig));			\
	(a_pConfig)->pUsbdName = (a_pUsbdName);				\
	(a_pConfig)->maxNestedCompletions = (a_maxNestedCompletions);	\
	(a_pConfig)->sizeConfigBuffer = (a_sizeConfigBuffer);		\
	(a_pConfig)->sizeStringDescBuffer = (a_sizeStringDescBuffer);	\
	(a_pConfig)->maxUrbExtraBytes = (a_maxUrbExtraBytes);		\
	(a_pConfig)->tAttachDebounce = (a_tAttachDebounce);		\
	(a_pConfig)->tResetRecovery = (a_tResetRecovery);		\
	(a_pConfig)->tSetAddrCompletion = (a_tSetAddrCompletion);	\
	(a_pConfig)->tSetAddrRecovery = (a_tSetAddrRecovery);		\
	(a_pConfig)->tStdRequestNoData = (a_tStdRequestNoData);		\
	(a_pConfig)->tStdRequestData1 = (a_tStdRequestData1);		\
	(a_pConfig)->tStdRequestDataN = (a_tStdRequestDataN);		\
	(a_pConfig)->tStdRequestMinimum = (a_tStdRequestMinimum);	\
	(a_pConfig)->tResumeRecovery = (a_tResumeRecovery);		\
	(a_pConfig)->bNumberHubs = (a_bNumberHubs);			\
	(a_pConfig)->bPortsPerHub = (a_bPortsPerHub);			\
	(a_pConfig)->bMinU1TimeoutEncode = (a_bMinU1TimeoutEncode);	\
	(a_pConfig)->bMinU2TimeoutEncode = (a_bMinU2TimeoutEncode);	\
	(a_pConfig)->bHubU1TimeoutEncode = (a_bHubU1TimeoutEncode);	\
	(a_pConfig)->bHubU2TimeoutEncode = (a_bHubU2TimeoutEncode);	\
	(a_pConfig)->tHostInitiatedResumeDuration =			\
		(a_tHostInitiatedResumeDuration);			\
	(a_pConfig)->tAutoLpmIdleTime = (a_tAutoLpmIdleTime);		\
	(a_pConfig)->pHubIdOverrides = (a_pHubIdOverrides);		\
	(a_pConfig)->AnnunciatorMaxSession = (a_AnnunciatorMaxSession);	\
	(a_pConfig)->ulDebugFlags = (a_ulDebugFlags);			\
	(a_pConfig)->Flags = (a_Flags);					\
	(a_pConfig)->MagicEnd =						\
		__TMS_USBPUMP_USBDI_USBD_CONFIG_MAGIC_END;		\
	} while (0)


/****************************************************************************\
|
|	Uncloaking
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_USBDI_USBD_CONTENTS__STRUCT	\
   __TMS_USBPUMP_USBDI_USBD_CONTENTS__STRUCT
# define USBPUMP_USBDI_USBD_CONTENTS__UNION	\
   __TMS_USBPUMP_USBDI_USBD_CONTENTS__UNION
# define USBPUMP_USBDI_USBD_TAG	\
   __TMS_USBPUMP_USBDI_USBD_TAG
# define USBPUMP_USBDI_USBD_NAME(x)	\
   __TMS_USBPUMP_USBDI_USBD_NAME(x)
# define USBPUMP_USBDI_USBD_METACLASS_NAME	\
   __TMS_USBPUMP_USBDI_USBD_METACLASS_NAME
# define USBPUMP_USBDI_USBD_CONFIG_MAGIC_BEGIN	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_MAGIC_BEGIN
# define USBPUMP_USBDI_USBD_CONFIG_MAGIC_END	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_MAGIC_END
# define USBPUMP_USB20_TATTDB_SPEC	\
   __TMS_USBPUMP_USB20_TATTDB_SPEC
# define USBPUMP_USB20_TATTDB_DEFAULT	\
   __TMS_USBPUMP_USB20_TATTDB_DEFAULT
# define USBPUMP_USB20_TRSTRCY_SPEC	\
   __TMS_USBPUMP_USB20_TRSTRCY_SPEC
# define USBPUMP_USB20_TRSTRCY_DEFAULT	\
   __TMS_USBPUMP_USB20_TRSTRCY_DEFAULT
# define USBPUMP_USB20_TRSTRCY_WINDOWS	\
   __TMS_USBPUMP_USB20_TRSTRCY_WINDOWS
# define USBPUMP_USB20_TSETADDRRCY_SPEC	\
   __TMS_USBPUMP_USB20_TSETADDRRCY_SPEC
# define USBPUMP_USB20_TSETADDRRCY_DEFAULT	\
   __TMS_USBPUMP_USB20_TSETADDRRCY_DEFAULT
# define USBPUMP_USB20_TSETADDRRCY_WINDOWS	\
   __TMS_USBPUMP_USB20_TSETADDRRCY_WINDOWS
# define USBPUMP_USB20_TDSETADDR_SPEC	\
   __TMS_USBPUMP_USB20_TDSETADDR_SPEC
# define USBPUMP_USB20_TDSETADDR_DEFAULT	\
   __TMS_USBPUMP_USB20_TDSETADDR_DEFAULT
# define USBPUMP_USB20_TRSMRCY_SPEC	\
   __TMS_USBPUMP_USB20_TRSMRCY_SPEC
# define USBPUMP_USB20_TRSMRCY_DEFAULT	\
   __TMS_USBPUMP_USB20_TRSMRCY_DEFAULT
# define USBPUMP_USB20_TRSMRCY_WINDOWS	\
   __TMS_USBPUMP_USB20_TRSMRCY_WINDOWS
# define USBPUMP_USB20_TDRQCMPLTND_SPEC	\
   __TMS_USBPUMP_USB20_TDRQCMPLTND_SPEC
# define USBPUMP_USB20_TDRQCMPLTND_DEFAULT	\
   __TMS_USBPUMP_USB20_TDRQCMPLTND_DEFAULT
# define USBPUMP_USB20_TDRETDATA1_SPEC	\
   __TMS_USBPUMP_USB20_TDRETDATA1_SPEC
# define USBPUMP_USB20_TDRETDATA1_DEFAULT	\
   __TMS_USBPUMP_USB20_TDRETDATA1_DEFAULT
# define USBPUMP_USB20_TDRETDATAN_SPEC	\
   __TMS_USBPUMP_USB20_TDRETDATAN_SPEC
# define USBPUMP_USB20_TDRETDATAN_DEFAULT	\
   __TMS_USBPUMP_USB20_TDRETDATAN_DEFAULT
# define USBPUMP_USB20_TSTDREQUEST_MINIMUM_DEFAULT	\
   __TMS_USBPUMP_USB20_TSTDREQUEST_MINIMUM_DEFAULT
# define USBPUMP_USB20_TSTDREQUEST_MINIMUM_WINDOWS	\
   __TMS_USBPUMP_USB20_TSTDREQUEST_MINIMUM_WINDOWS
# define USBPUMP_USB20_LPM_HIRD_DEFAULT	\
   __TMS_USBPUMP_USB20_LPM_HIRD_DEFAULT
# define USBPUMP_USBDI_AUTO_LPM_IDLE_TIME_DEFAULT	\
   __TMS_USBPUMP_USBDI_AUTO_LPM_IDLE_TIME_DEFAULT
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_AUTO_LPM	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_AUTO_LPM
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U2	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U2
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1_U2	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_U1_U2
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_LTM	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_LTM
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_DONT_VALIDATE_SERIAL_NUM	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_DONT_VALIDATE_SERIAL_NUM
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_CONNECTION	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_CONNECTION
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_DISCONNECTION	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_DISCONNECTION
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_OVERCURRENT	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_WAKE_ON_OVERCURRENT
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_HUB_AUTO_SUSPEND	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_HUB_AUTO_SUSPEND
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SLEEP	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SLEEP
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_RESTART	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_RESTART
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SHUTDOWN	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_PORT_POWER_OFF_WHEN_SHUTDOWN
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_HARDWARE_LPM	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_ENABLE_HARDWARE_LPM
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_HARDWARE_LPM_MODE_1	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_HARDWARE_LPM_MODE_1
# define USBPUMP_USBDI_USBD_CONFIG_FLAG_DONT_LPM_IF_MISMATCH_BESL	\
   __TMS_USBPUMP_USBDI_USBD_CONFIG_FLAG_DONT_LPM_IF_MISMATCH_BESL
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V1(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V1(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V1(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V1(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V2(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V2(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V2(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V2(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V3(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V3(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V3(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V3(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V4(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V4(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V4(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V4(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V5(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V5(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V5(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V5(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V6(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V6(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V6(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V6(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags							\
	)
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V7(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags								\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V7(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags								\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V7(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags								\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V7(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags								\
	)
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V8(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V8(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V8(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V8(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_INIT_V9(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode,						\
	a_bHubU1TimeoutEncode,						\
	a_bHubU2TimeoutEncode						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_INIT_V9(			\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode,						\
	a_bHubU1TimeoutEncode,						\
	a_bHubU2TimeoutEncode						\
	)
# define USBPUMP_USBDI_USBD_CONFIG_SETUP_V9(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode,						\
	a_bHubU1TimeoutEncode,						\
	a_bHubU2TimeoutEncode						\
	)	\
	__TMS_USBPUMP_USBDI_USBD_CONFIG_SETUP_V9(			\
	a_pConfig,							\
	a_pUsbdName,							\
	a_maxNestedCompletions,						\
	a_sizeConfigBuffer,						\
	a_sizeStringDescBuffer,						\
	a_maxUrbExtraBytes,						\
	a_tAttachDebounce,						\
	a_tResetRecovery,						\
	a_tSetAddrCompletion,						\
	a_tSetAddrRecovery,						\
	a_tStdRequestNoData,						\
	a_tStdRequestData1,						\
	a_tStdRequestDataN,						\
	a_tStdRequestMinimum,						\
	a_tResumeRecovery,						\
	a_bNumberHubs,							\
	a_bPortsPerHub,							\
	a_tHostInitiatedResumeDuration,					\
	a_pHubIdOverrides,						\
	a_AnnunciatorMaxSession,					\
	a_ulDebugFlags,							\
	a_tAutoLpmIdleTime,						\
	a_Flags,							\
	a_bMinU1TimeoutEncode,						\
	a_bMinU2TimeoutEncode,						\
	a_bHubU1TimeoutEncode,						\
	a_bHubU2TimeoutEncode						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_usbdi_usbd.h ****/
#endif /* _USBPUMP_USBDI_USBD_H_ */
