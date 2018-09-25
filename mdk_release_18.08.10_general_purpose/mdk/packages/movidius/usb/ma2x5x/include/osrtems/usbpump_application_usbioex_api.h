/* usbpump_application_usbioex_api.h	Fri Oct 27 2017 20:08:16 Kvandhiadevan*/

/*

Module:  usbpump_application_usbioex_api.h

Function:
	Definition of DataPump API for USBIOEX APP

Version:
	V3.21c	Fri Oct 27 2017 20:08:16  Kvandhiadevan	Edit level 3

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Vandhiadevan Karunamoorthy, MCCI Corporation	August 2017

Revision history:
   3.21c Mon Aug 28 2017 10:14:07  Kvandhiadevan
	20697: Module created

   3.21c  Tue Oct 24 2017 15:43:34  chwon
	20697: Use USBPUMP_APPLICATION_BEGIN_DECLS and ..._END_DECLS.

   3.21c  Fri Oct 27 2017 20:08:16  Kvandhiadevan
	20697: Added Pipe support and Error 

*/

#ifndef _USBPUMP_APPLICATION_USBIOEX_API_H_	/* prevent multiple includes */
#define _USBPUMP_APPLICATION_USBIOEX_API_H_

#ifndef _USBPUMP_APPLICATION_RTEMS_API_H_
# include "usbpump_application_rtems_api.h"
#endif

/****************************************************************************\
|
|	Data Types & define types
|
\****************************************************************************/

#define USBPUMP_USBIOEX_TYPE_BULK		0x0001
#define USBPUMP_USBIOEX_TYPE_ISO		0x0002
#define USBPUMP_USBIOEX_TYPE_INT		0x0003
#define USBPUMP_USBIOEX_MODE_NORMAL		0x0004
#define USBPUMP_USBIOEX_MODE_SWEEP		0x0005
#define USBPUMP_USBIOEX_MODE_RANDOM		0x0006
#define USBPUMP_USBIOEX_TEST_INTEGRITY		0x0007
#define USBPUMP_USBIOEX_TEST_BENCHMARK		0x0008

/****************************************************************************\
|
|	Status & Error code define types
|
\****************************************************************************/

#define USBPUMP_USBIOEX_STATUS_OK		0x0011
#define USBPUMP_USBIOEX_PIPE_ID_ERROR		0x0012

/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

typedef struct _USBPUMP_APPLICATION_USBIOEX_STATUS
	USBPUMP_APPLICATION_USBIOEX_STATUS;
	
typedef struct _USBPUMP_APPLICATION_USBIOEX_LOOPBACK_CONTEXT
	USBPUMP_APPLICATION_USBIOEX_LOOPBACK_CONTEXT;
	
typedef struct _USBPUMP_APPLICATION_USBIOEX_PIPE_CONTEXT
	USBPUMP_APPLICATION_USBIOEX_PIPE_CONTEXT;
	
/****************************************************************************\
|
|	GCD client private data structure
|
\****************************************************************************/

struct _USBPUMP_APPLICATION_USBIOEX_PIPE_CONTEXT
	{
	unsigned int		bConfigurationNumber;
	unsigned int		bInterfaceNumber;
	unsigned int		bAlternateSetting;
	};
struct _USBPUMP_APPLICATION_USBIOEX_STATUS
	{
	bool			bStatus;
	unsigned int		ErrorCode;
	unsigned int		nSucessfullpacket;
	unsigned int		nCorruptpacket;
	unsigned int		nBytesPerSecond;
	};
	
struct _USBPUMP_APPLICATION_USBIOEX_LOOPBACK_CONTEXT
	{
	USBPUMP_APPLICATION_USBIOEX_PIPE_CONTEXT 	
				*pOutPipe;
	USBPUMP_APPLICATION_USBIOEX_PIPE_CONTEXT 	
				*pInPipe;
	unsigned int		EndpointType;
	unsigned int		Mode;
	unsigned int		TestType;
	unsigned int		nIteration;
	unsigned int		SweepStep;
	unsigned int		SweepInitial;
	unsigned int		wMaxpacket;
	unsigned int		nRandomSeed;
	bool			bOutOnly;
	bool			bInOnly;
	};

	
/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

USBPUMP_APPLICATION_BEGIN_DECLS

void
UsbPumpUsbioex_Loopback(
	void						*pGcdDevice,
	USBPUMP_APPLICATION_USBIOEX_LOOPBACK_CONTEXT 	*pLoopbackContext,
	USBPUMP_APPLICATION_USBIOEX_STATUS 		*pOutContext
	);

bool
UsbPumpUsbioexGcd_GetEndpointDescriptor(
	void 		*pDevice,
	void		*pBuffer
	);

bool
UsbPumpUsbioexGcd_GetInterfaceDescriptor(
	void		*pDevice,
	void		*pBuffer
	);

bool
UsbPumpUsbioexGcd_GetConfigurationDescriptor(
	void 		*pDevice,
	void 		*pBuffer
	);

bool
UsbPumpUsbioexGcd_GetDeviceDescriptor(
	void 		*pDevice,
	void 		*pBuffer
	);

void *
UsbPumpUsbioex_GetDevice(
	unsigned short int	VendorID,
	unsigned short int	ProductID,
	unsigned char		*pSerialNumber
	);

USBPUMP_APPLICATION_END_DECLS

#endif /*_USBPUMP_APPLICATION_USBIOEX_API_H_*/
