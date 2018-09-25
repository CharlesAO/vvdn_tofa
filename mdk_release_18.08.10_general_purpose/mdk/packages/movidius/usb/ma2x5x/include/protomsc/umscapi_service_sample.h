/* umscapi_service_sample.h	Wed Aug 22 2007 14:03:08 chwon */

/*

Module:  umscapi_service_sample.h

Function:
	Definition of Sample MSCAPI service implementation

Version:
	V1.97j	Wed Aug 22 2007 14:03:08 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2005-2007 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	June 2005

Revision history:
   1.91e  Tue Jun 28 2005 17:12:05  chwon
	Module created.

   1.97j  Wed Aug 22 2007 14:03:08  chwon
	Complete code.

*/

#ifndef _UMSCAPI_SERVICE_SAMPLE_H_		/* prevent multiple includes */
#define _UMSCAPI_SERVICE_SAMPLE_H_

#ifndef _UMSCAPI_CLIENT_H_
# include "umscapi_client.h"
#endif

/****************************************************************************\
|
|	Default linkage
|
\****************************************************************************/

extern __TMS_CONST __TMS_MSCAPI_CLIENT_CONFIG
	gk_MscApiService_ClientConfig;

extern __TMS_CONST __TMS_UPROTO_MSCSUBCLASS_ATAPI_CONFIG
	gk_MscApiService_DiskConfig[];


/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

MSCAPI_INITIALIZE_FN	MscApi_Service_Initialize;
MSCAPI_REQUEST_FN	MscApi_Service_Request;

void		utask_MscApiServiceTask(void);


/* platform provide functions */
__TMS_VOID	utask_SendMessage(
			__TMS_UINT32	Message,
			__TMS_VOID *	pContext
			);

__TMS_UINT32	utask_ReceiveMessage(
			__TMS_VOID **	ppContext
			);

__TMS_BOOL	MSCAPI_ReadCapacity(
			__TMS_UINT32	Device,
			__TMS_UINT32 *	pMaxBlock,
			__TMS_UINT32 *	pSizeBlock
			);

__TMS_BOOL	MSCAPI_ReadBlock(
			__TMS_UINT32	Device,
			__TMS_UINT32	BlockNo,
			__TMS_UINT32	nBlock,
			__TMS_VOID *	pBuffer,
			__TMS_UINT32 *	pnRead
			);

__TMS_BOOL	MSCAPI_WriteBlock(
			__TMS_UINT32	Device,
			__TMS_UINT32	BlockNo,
			__TMS_UINT32	nBlock,
			__TMS_VOID *	pBuffer,
			__TMS_UINT32 *	pnWrite
			);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/


/**** end of umscapi_service_sample.h ****/
#endif /* _UMSCAPI_SERVICE_SAMPLE_H_ */
