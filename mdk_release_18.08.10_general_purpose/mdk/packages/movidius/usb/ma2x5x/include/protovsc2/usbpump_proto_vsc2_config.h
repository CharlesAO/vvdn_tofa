/* usbpump_proto_vsc2_config.h	Tue Jul 31 2018 13:43:59 chwon */

/*

Module:  usbpump_proto_vsc2_config.h

Function:
	Definition of the VSC protocol configuration.

Version:
	V3.21g	Tue Jul 31 2018 13:44:00 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2013-2014, 2017-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	March 2013

Revision history:
   3.11c  Tue Mar 19 2013 16:49:28 chwon
	16968: Module created.

   3.13b  Tue Oct 14 2014 13:49:45  chwon
	18578: Add NumMaxStreams in the configuration.

   3.21c  Fri Aug 04 2017 12:27:32  chwon
	20629: Add pVsc2InCall in the configuration.

   3.21e  Thu Nov 16 2017 12:14:14  chwon
	20713: Add pOsDescCompatibleId in the configuration.

   3.21g  Tue Jul 31 2018 13:44:00  chwon
	20862: Add extended property information in the configuration.

*/

#ifndef _USBPUMP_PROTO_VSC2_CONFIG_H_	/* prevent multiple includes */
#define _USBPUMP_PROTO_VSC2_CONFIG_H_

#ifndef _USBPUMP_PROTO_VSC2_API_H_
# include "usbpump_proto_vsc2_api.h"
#endif

/****************************************************************************\
|
|	USBPUMP_PROTO_VSC2_CONFIG Structure Definition
|
\****************************************************************************/

/*

Type:	USBPUMP_PROTO_VSC2_CONFIG

Index:	Type:	PUSBPUMP_PROTO_VSC2_CONFIG

Function:
	Configuration information for the VSC protocol.

Description:
	The structure contains VSC protocol configuration information.

Definition:
	typedef struct __TMS_STRUCTNAME(USBPUMP_PROTO_VSC2_CONFIG)
		USBPUMP_PROTO_VSC2_CONFIG, *PUSBPUMP_PROTO_VSC2_CONFIG;

Contents:
	UINT16 Version;
		Version of VSC protocol configuration. It should be 2.

	UINT16 SizeControlBuffer;
		size of control buffer which will be used by VSC protocol to
		receive / send control data.

	CONST TEXT *pObjectName;
		Name of VSC protocol instance object. It should not be NULL.

	UINT32 DebugFlags;
		The VSC protocol object debug flags.  If DebugFlags is zero,
		it will inherit debug flag of parent object which is UDEVICE
		object.

	UINT NumMaxStreams;
		Number of streams to open.  If NumMaxStreams is zero, the vsc
		protocol will detect it.

	USBPUMP_PROTO_VSC2_INCALL pVsc2InCall;
		InCall pointer of the VSC protocol.

	CONST TEXT *pOsDescCompatibleId;
		This is Microsoft OS descriptor compatible ID string. If client
		wants to support Microsoft OS descriptor, client should provide
		this OS descriptor compatible ID string. For example, client
		sets this to "WINUSB" if client wants to support WINUSB.

		Client should use UsbPumpPlatform_RegisterOsDescProcess() API
		to support Microsft OS descriptor.

	UINT16 wOsDescPropertyCount;
	UINT16 nOsDescCustomProperty;
	CONST UINT8 *pOsDescCustomProperty;
		This is Microsoft OS descriptor extended property feature
		information. The wOsDescPropertyCount is number of custom
		extended property sections. The nOsDescCustomProperty is total
		number of bytes of the custom extended property sections which
		pointed by pOsDescCustomProperty.

Notes:
	You should use USBPUMP_PROTO_VSC2_CONFIG_INIT_V5() or higher version
	macro to initialize VSC configuration structure.

	VOID USBPUMP_PROTO_VSC2_CONFIG_INIT_V5(
		BYTES		SizeControlBuffer,
		CONST TEXT *	pObjectName,
		UINT32		DebugFlags,
		UINT		NumMaxStreams,
		CONST USBPUMP_PROTO_VSC2_INCALL *pVsc2InCall,
		CONST TEXT *	pOsDescCompatibleId,
		UINT16		wOsDescPropertyCount,
		UINT16		nOsDescCustomProperty,
		CONST UINT8 *	pOsDescCustomProperty
		);

Example:
	CONST TEXT gk_ProtoVsc2ObjectName[] = USBPUMP_PROTO_VSC2_NAME("sample");
	CONST TEXT gk_ProtoVsc2CompatibleId[] = "WINUSB";
	CONST UINT8 gk_ProtoVsc2Property[] =
		{
		0x84, 0x00, 0x00, 0x00,		// dwSize
		0x01, 0x00, 0x00, 0x00,		// dwPropertyDataType
		0x28, 0x00,			// wPropertyNameLength
		// bPropertyName -- "DeviceInterfaceGUID"
		0x44, 0x00,	// 'D'
		0x65, 0x00,	// 'e'
		0x76, 0x00,	// 'v'
		0x69, 0x00,	// 'i'
		0x63, 0x00,	// 'c'
		0x65, 0x00,	// 'e'
		0x49, 0x00,	// 'I'
		0x6e, 0x00,	// 'n'
		0x74, 0x00,	// 't'
		0x65, 0x00,	// 'e'
		0x72, 0x00,	// 'r'
		0x66, 0x00,	// 'f'
		0x61, 0x00,	// 'a'
		0x63, 0x00,	// 'c'
		0x65, 0x00,	// 'e'
		0x47, 0x00,	// 'G'
		0x55, 0x00,	// 'U'
		0x49, 0x00,	// 'I'
		0x44, 0x00,	// 'D'
		0x00, 0x00,	// '\0'
		0x4e, 0x00, 0x00, 0x00,		// dwPropertyDataLength
		// bPropertyData -- "{8FE6D4D7-49DD-41E7-9486-49AFC6BFE475}"
		0x7B, 0x00,
		0x38, 0x00,
		0x46, 0x00,
		0x45, 0x00,
		0x36, 0x00,
		0x44, 0x00,
		0x34, 0x00,
		0x44, 0x00,
		0x37, 0x00,
		0x2D, 0x00,
		0x34, 0x00,
		0x39, 0x00,
		0x44, 0x00,
		0x44, 0x00,
		0x2D, 0x00,
		0x34, 0x00,
		0x31, 0x00,
		0x45, 0x00,
		0x37, 0x00,
		0x2D, 0x00,
		0x39, 0x00,
		0x34, 0x00,
		0x38, 0x00,
		0x36, 0x00,
		0x2D, 0x00,
		0x34, 0x00,
		0x39, 0x00,
		0x41, 0x00,
		0x46, 0x00,
		0x43, 0x00,
		0x36, 0x00,
		0x42, 0x00,
		0x46, 0x00,
		0x45, 0x00,
		0x34, 0x00,
		0x37, 0x00,
		0x35, 0x00,
		0x7D, 0x00,
		0x00, 0x00
		};

	CONST USBPUMP_PROTO_VSC2_CONFIG gk_ProtoVsc2Config =
		USBPUMP_PROTO_VSC2_CONFIG_INIT_V5(
			512,				// SizeControlBuffer
			gk_ProtoVsc2ObjectName,		// pObjectName
			0,				// DebugFlags
			0,				// NumMaxStreams
			&gk_UsbPumpVsc2_InCallDefault,	// pVsc2InCall
			gk_ProtoVsc2CompatibleId,	// pOsDescCompatibleId
			1,				// wOsDescPropertyCount
			sizeof(gk_ProtoVsc2Property),	// nOsDescCustomProperty
			gk_ProtoVsc2Property		// pOsDescCustomProperty
			);

*/

/* __TMS_TYPE_DEF_STRUCT(USBPUMP_PROTO_VSC2_CONFIG); */
struct __TMS_STRUCTNAME	(USBPUMP_PROTO_VSC2_CONFIG)
	{
	__TMS_UINT16			Version;
	__TMS_UINT16			SizeControlBuffer;
	__TMS_CONST __TMS_TEXT *	pObjectName;
	__TMS_UINT32			DebugFlags;
	__TMS_UINT			NumMaxStreams;
	__TMS_CONST __TMS_USBPUMP_PROTO_VSC2_INCALL *pVsc2InCall;
	__TMS_CONST __TMS_TEXT *	pOsDescCompatibleId;
	__TMS_UINT16			wOsDescPropertyCount;
	__TMS_UINT16			nOsDescCustomProperty;
	__TMS_CONST __TMS_UINT8 *	pOsDescCustomProperty;
	};

/*
|| Architecture: never change this macro's arguments; instead make V6 macro,
|| and express this in terms of the V6 macro.
*/
#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V5(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId,					\
	a_wOsDescPropertyCount,					\
	a_nOsDescCustomProperty,				\
	a_pOsDescCustomProperty					\
	)							\
	{							\
	2,	/* Version 2 */					\
	(a_SizeControlBuffer),					\
	(a_pObjectName),					\
	(a_DebugFlags),						\
	(a_NumMaxStreams),					\
	(a_pVsc2InCall),					\
	(a_pOsDescCompatibleId),				\
	(a_wOsDescPropertyCount),				\
	(a_nOsDescCustomProperty),				\
	(a_pOsDescCustomProperty)				\
	}

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V4(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId					\
	)							\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V5(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId,					\
	/* V5: wOsDescPropertyCount */ 0,			\
	/* V5: nOsDescCustomProperty */ 0,			\
	/* V5: pOsDescCustomProperty */ __TMS_NULL		\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V3(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall						\
	)							\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V4(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	/* V4: pOsDescCompatibleId */ __TMS_NULL		\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V2(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams						\
	)							\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V3(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	/* V3: pVsc2InCall */	&gk_UsbPumpVsc2_InCallDefault	\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V1(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags						\
	)							\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V2(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	/* V2: NumMaxStreams */ 0				\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V5(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId,					\
	a_wOsDescPropertyCount,					\
	a_nOsDescCustomProperty,				\
	a_pOsDescCustomProperty					\
	)							\
    do	{							\
	(a_pConfig)->Version = 2;	/* Version 2 */		\
	(a_pConfig)->SizeControlBuffer = (a_SizeControlBuffer);	\
	(a_pConfig)->pObjectName = (a_pObjectName);		\
	(a_pConfig)->DebugFlags = (a_DebugFlags);		\
	(a_pConfig)->NumMaxStreams = (a_NumMaxStreams);		\
	(a_pConfig)->pVsc2InCall = (a_pVsc2InCall);		\
	(a_pConfig)->pOsDescCompatibleId = (a_pOsDescCompatibleId); \
	(a_pConfig)->wOsDescPropertyCount = (a_wOsDescPropertyCount); \
	(a_pConfig)->nOsDescCustomProperty = (a_nOsDescCustomProperty); \
	(a_pConfig)->pOsDescCustomProperty = (a_pOsDescCustomProperty); \
	} while (0)

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V4(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId					\
	)							\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V5(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId,					\
	/* V5: wOsDescPropertyCount */ 0,			\
	/* V5: nOsDescCustomProperty */ 0,			\
	/* V5: pOsDescCustomProperty */ __TMS_NULL		\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V3(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall						\
	)							\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V4(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	/* V4: pOsDescCompatibleId */ __TMS_NULL		\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V2(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams						\
	)							\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V3(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	/* V3: pVsc2InCall */	&gk_UsbPumpVsc2_InCallDefault	\
	)

#define	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V1(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags						\
	)							\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V2(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	/* V2: NumMaxStreams */ 0				\
	)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_PROTO_VSC2_CONFIG_INIT_V5(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId,					\
	a_wOsDescPropertyCount,					\
	a_nOsDescCustomProperty,				\
	a_pOsDescCustomProperty					\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V5(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId,					\
	a_wOsDescPropertyCount,					\
	a_nOsDescCustomProperty,				\
	a_pOsDescCustomProperty					\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_INIT_V4(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId					\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V4(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId					\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_INIT_V3(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V3(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall						\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_INIT_V2(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V2(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams						\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_INIT_V1(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_INIT_V1(		\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags						\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_SETUP_V5(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId,					\
	a_wOsDescPropertyCount,					\
	a_nOsDescCustomProperty,				\
	a_pOsDescCustomProperty					\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V5(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId,					\
	a_wOsDescPropertyCount,					\
	a_nOsDescCustomProperty,				\
	a_pOsDescCustomProperty					\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_SETUP_V4(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId					\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V4(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall,						\
	a_pOsDescCompatibleId					\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_SETUP_V3(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V3(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams,					\
	a_pVsc2InCall						\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_SETUP_V2(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V2(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags,						\
	a_NumMaxStreams						\
	)
# define USBPUMP_PROTO_VSC2_CONFIG_SETUP_V1(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags						\
	)	\
	__TMS_USBPUMP_PROTO_VSC2_CONFIG_SETUP_V1(		\
	a_pConfig,						\
	a_SizeControlBuffer,					\
	a_pObjectName,						\
	a_DebugFlags						\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_proto_vsc2_config.h ****/
#endif /* _USBPUMP_PROTO_VSC2_CONFIG_H_ */
