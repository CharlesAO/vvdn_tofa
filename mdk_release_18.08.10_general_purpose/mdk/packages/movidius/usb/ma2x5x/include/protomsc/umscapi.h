/* umscapi.h	Thu Oct 28 2010 09:15:58 chwon */

/*

Module:  umscapi.h

Function:
	Defines data structure for Mass Storage Class API.

Version:
	V3.01d	Thu Oct 28 2010 09:15:58 chwon	Edit level 6

Copyright notice:
	This file copyright (C) 2004-2005, 2009-2010 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	January 2004

Revision history:
   1.87a  Wed Jan 28 2004 00:42:35  tmm
	Module created, based on TrueTask <def/scsi.h>, V2.00a
	Tue Feb  6 1996 14:35:27

   1.87b  Thu Mar 25 2004 16:11:41  chwon
	Module created from SCSI based mscspi.h

   1.89b  Mon Oct 11 2004 23:07:47  chwon
	Add LUN support.

   1.93a  Fri Oct 07 2005 09:39:28  chwon
	Pull out struct definition inside of union

   2.01a  Tue Apr 21 2009 15:33:31 ctpeng
	7119: Add MSCAPI_REQUEST_PREVENT & MSCAPI_REQUEST_LOADEJECT and
	the following NOTIFY events:
	MSCAPI_NOTIFY_CDROM_TRAYIN_HAS_MEDIUM
	MSCAPI_NOTIFY_CDROM_TRAYIN_NO_MEDIUM
	MSCAPI_NOTIFY_CDROM_TRAYOUT

   3.01d  Thu Oct 28 2010 09:15:58  chwon
	11752: Add fReadOnly flag in the MSCAPI_REQUEST_READ_CAPACITY request.

*/

#ifndef _UMSCAPI_H_		/* prevent multiple includes */
#define _UMSCAPI_H_

/*

MODULE DOCUMENTATION:  Mass Storage Class API

The Mass Storage Class API (MSCAPI) is an abstract, portable API that
is used to access storage blocks on random-access media (such as disks,
Compact Flash cards, SD cards, MMC cards, and storage devices with similar
semantics).

MCCI provides this API as a simple way for others to implement
low-level drivers that can interface easily with the MCCI USB DataPump and
other MCCI products, without requiring that the customers integrate
their code closely with the DataPump, and without depending on other
MCCI include files.

For convenience, we define MSCAPI *clients* and MSCAPI *services*. MCCI
provides an MSCAPI client as part of its DataPump code release.
Others provide MSCAPI services that bridge from this API to low level
hardware.

The API provides the following general functionality:

1) A client can discover services.

2) All communications between clients and services take place
asynchronously.  The client prepares a request and submits it to the service.
Later, when the request completes, the service calls the client back.

3) All communications from client to server are controlled by
request blocks.  These block is allocated by the client (somehow, perhaps
statically).  It is released (either for reuse or for return to system
memory, at the client's option) when the client receives the "completion"
callback.

4) Services can also communicate a limited set of events to their
clients,

5) the hardware used to commuincate with media is logically viewed in
terms of busses, controllers, and logical units.

6) Media request blocks convey either *management requests* or *media
requests* to logical units.

7) Management requests are effectively addressed to the control
plane of the connection.  Each different management request is distinguished
by a distinct code passed in |mscr_fnc|.


NOTES:

The API is specified as a single set of include files that
have no external requirements.  To avoid name-space collisions,
all top-level names begin with "MSCAPI_".

To support small compilers, the full (general) MCCI
libport mechanism of type cloaking is not used.

The API is fully asynchronous; this allows a service to be implemented
in the same thread, in a separate thread, or even on a separate processor,
as needed.

*/

/****************************************************************************\
|
|	Basic Types
|
\****************************************************************************/

#ifndef MSCAPI_UINT8_TYPE
typedef	unsigned char		MSCAPI_UINT8, *PMSCAPI_UINT8;
#else
typedef MSCAPI_UINT8_TYPE	MSCAPI_UINT8, *PMSCAPI_UINT8;
#endif

#ifndef MSCAPI_SINT8_TYPE
typedef	signed char		MSCAPI_SINT8, *PMSCAPI_SINT8;
#else
typedef MSCAPI_SINT8_TYPE	MSCAPI_SINT8, *PMSCAPI_SINT8;
#endif

#ifndef MSCAPI_UINT16_TYPE
typedef unsigned short		MSCAPI_UINT16, *PMSCAPI_UINT16;
#else
typedef MSCAPI_UINT16_TYPE	MSCAPI_UINT16, *PMSCAPI_UINT16;
#endif

#ifndef MSCAPI_UINT32_TYPE
typedef unsigned long		MSCAPI_UINT32, *PMSCAPI_UINT32;
#else
typedef MSCAPI_UINT32_TYPE	MSCAPI_UINT32, *PMSCAPI_UINT32;
#endif

#ifndef MSCAPI_VOID_TYPE
typedef	void			MSCAPI_VOID, *PMSCAPI_VOID;
#else
typedef MSCAPI_VOID_TYPE	MSCAPI_VOID, *PMSCAPI_VOID;
#endif

/****************************************************************************\
|
|	Related Types
|
\****************************************************************************/

typedef	MSCAPI_UINT8		MSCAPI_COMMAND, *PMSCAPI_COMMAND;
typedef	MSCAPI_UINT8		MSCAPI_STATUS, *PMSCAPI_STATUS;
typedef	MSCAPI_UINT8		MSCAPI_LUN, *PMSCAPI_LUN;

/****************************************************************************\
|
|	Forward references and opaque types
|
\****************************************************************************/

typedef	struct _MSCAPI_SERVICE	MSCAPI_SERVICE, *PMSCAPI_SERVICE;
typedef struct _MSCAPI_REQUEST	MSCAPI_REQUEST, *PMSCAPI_REQUEST;
typedef struct _MSCAPI_NOTIFY	MSCAPI_NOTIFY,  *PMSCAPI_NOTIFY;

typedef union _MSCAPI_REQUEST_DATA MSCAPI_REQUEST_DATA, *PMSCAPI_REQUEST_DATA;


/*

Type:	MSCAPI_REQUEST

Function:
	Represents a single request from MSCAPI client to MSCAPI service.

Description:
	In order to make a mass storage API request, a client allocates and
	initializes an |MSCAPI_REQUEST| object, and passes it to the
	appropriate service for handling.  The service processes the
	request, and then notifies the client (by calling a call-back
	function passed by the client).

Contents:
	|MSCAPI_REQUEST *mscr_pNext, *mscr_pLast|
		point to the next and previous |MSCAPI_REQUEST| on
		a circular list.

	|mscr_fnc|
		Specify MSCAPI request command code by MSCAPI client.
		MSCAPI Service must preserve this field.

	|mscr_status|
		MSCAPI service return result of requested command.

	|mscr_donefn|
	|mscr_doneinfo|
		Specify the completion function to be called by service when
		the |MSCAPI_REQUEST| completes.  |mscr_doneinfo| is for use
		by clients, and is not interpreted by service.

	|mscr_data|
		This is additional data information of MSCAPI request command.
		Each request command has own structure. This structure defined
		in |MSCAPI_REQUEST_DATA|.

Notes:
	mscr_fnc, mscr_donefn, and mscr_doneinfo should not be changed by
	MSCAPI service.

See Also:
	MSCAPI_NOTIFY, MSCAPI_REQUEST_DATA

*/

/*

Type:	MSCAPI_REQUEST_DATA

Function:
	Represents a additional parameter data for each MSCAPI request command

Description:
	In order to make a mass storage API request, a client set appropriate
	request command to |mscr_fnc| and set additional parameters using this
	structure.

Contents:
	|Open|
	|Open.reserved|
		This is for |MSCAPI_REQUEST_OPEN| command. No more parameter
		is required, but we just define reserved field.

	|Close|
	|Close.reserved|
		This is for |MSCAPI_REQUEST_CLOSE| command. No more parameter
		is required, but we just define reserved field.

	|Notify|
	|Notify.pNotify|
		This is for |MSCAPI_REQUEST_REGISTER_NOTIFY| command.
		MSCAPI service should be save this pointer of |MSCAPI_NOTIFY|.
		When the service detect storage insertion or removal, the
		service notifies detected information to the client using
		this |MSCAPI_NOTIFY|.

	|Capacity|
	|Capacity.BlockSize|
	|Capacity.BlockCount|
	|Capacity.fReadonly|
		This is for |MSCAPI_REQUEST_READ_CAPACITY| command. The MSCAPI
		service put the storage capacity information into BlockSize and
		BlockCount. If there is no medium, the service must put 0 both
		BlockSize and BlockCount.  If medium is read only, the MSCAPI
		service should set fReadOnly flag to TRUE.

	|Read|
	|Read.BlockNo|
	|Read.BlockCount|
	|Read.pBuffer|
		This is for |MSCAPI_REQUEST_READ_BLOCK| command.
		The BlockNo has starting logical block number (zero origin) to
		read. The BlockCount has number of logical blocks to read.
		The pBuffer has buffer pointer to read blocks.

	|Write|
	|Write.BlockNo|
	|Write.BlockCount|
	|Write.pBuffer|
		This is for |MSCAPI_REQUEST_WRITE_BLOCK| command.
		The BlockNo has starting logical block number (zero origin) to
		write. The BlockCount has number of logical blocks to write.
		The pBuffer has buffer pointer to write blocks.

	|Prevent|
	|Prevent.Locked|
		This is for |MSCAPI_REQUEST_PREVENT| command.
		The MSCAPI client use this request to notify to the MSCAPI
		service that medium is locked or not.

	|LoadEject|
	|LoadEject.Load|
		This is for |MSCAPI_REQUEST_LOADEJECT| command.
		The MSCAPI client requests to load or eject medium.

See Also:
	MSCAPI_REQUEST

*/

typedef	void
MSCAPI_REQUEST_DONE_FN(
	MSCAPI_REQUEST *	/*pRequest*/,
	void *			/*pInfo*/
	);

typedef MSCAPI_REQUEST_DONE_FN *PMSCAPI_REQUEST_DONE_FN;

typedef struct _MSCAPI_DATA_OPEN		MSCAPI_DATA_OPEN;
typedef struct _MSCAPI_DATA_CLOSE		MSCAPI_DATA_CLOSE;
typedef struct _MSCAPI_DATA_REGISTER_NOTIFY	MSCAPI_DATA_REGISTER_NOTIFY;
typedef struct _MSCAPI_DATA_READ_CAPACITY	MSCAPI_DATA_READ_CAPACITY;
typedef struct _MSCAPI_DATA_READ_BLOCK		MSCAPI_DATA_READ_BLOCK;
typedef struct _MSCAPI_DATA_WRITE_BLOCK		MSCAPI_DATA_WRITE_BLOCK;
typedef struct _MSCAPI_DATA_PREVENT		MSCAPI_DATA_PREVENT;
typedef struct _MSCAPI_DATA_LOADEJECT		MSCAPI_DATA_LOADEJECT;

struct _MSCAPI_DATA_OPEN
	{
	void *		reserved;	/* Reserved */
	};

struct _MSCAPI_DATA_CLOSE
	{
	void *		reserved;	/* Reserved */
	};

struct _MSCAPI_DATA_REGISTER_NOTIFY
	{
	MSCAPI_NOTIFY *	pNotify;	/* pointer of MSCAPI_NOTIFY */
	};

struct _MSCAPI_DATA_READ_CAPACITY
	{
	MSCAPI_UINT32	BlockSize;	/* storage block size in bytes */
	MSCAPI_UINT32	BlockCount;	/* storage total block count */
	MSCAPI_UINT32	fReadOnly;	/* storage is read only */
	};

struct _MSCAPI_DATA_READ_BLOCK
	{
	MSCAPI_UINT32	BlockNo;	/* starting logical block (zero origin) */
	MSCAPI_UINT32	BlockCount;	/* requested number of blocks to read */
	MSCAPI_UINT8 *	pBuffer;	/* read buffer pointer */
	};

struct _MSCAPI_DATA_WRITE_BLOCK
	{
	MSCAPI_UINT32	BlockNo;	/* starting logical block number */
	MSCAPI_UINT32	BlockCount;	/* requested number of blocks to write */
	const MSCAPI_UINT8 *pBuffer;	/* write buffer pointer */
	};

struct _MSCAPI_DATA_PREVENT
	{
	MSCAPI_UINT8	Locked;		/* Lock or Unlock */
	};

struct _MSCAPI_DATA_LOADEJECT
	{
	MSCAPI_UINT8	Load;		/* Load or Eject*/
	};

union _MSCAPI_REQUEST_DATA
	{
	MSCAPI_DATA_OPEN		Open;
	MSCAPI_DATA_CLOSE		Close;
	MSCAPI_DATA_REGISTER_NOTIFY	Notify;
	MSCAPI_DATA_READ_CAPACITY	Capacity;
	MSCAPI_DATA_READ_BLOCK		Read;
	MSCAPI_DATA_WRITE_BLOCK		Write;
	MSCAPI_DATA_PREVENT		Prevent;
	MSCAPI_DATA_LOADEJECT		LoadEject;	
	};

struct _MSCAPI_REQUEST
	{
	/* linkage fields; used by "current owner" of request ad lib */
	MSCAPI_REQUEST *	mscr_pNext;
	MSCAPI_REQUEST *	mscr_pLast;

	MSCAPI_SERVICE *	mscr_pService;	/* IN: the service object,
						|| opaque to clients
						*/

	MSCAPI_COMMAND		mscr_fnc;	/* IN: MSCAPI request command */
	MSCAPI_LUN		mscr_lun;	/* IN: the logical unit number */
	MSCAPI_STATUS		mscr_status;	/* OUT: result code */

	MSCAPI_REQUEST_DONE_FN *mscr_donefn;	/* IN: request completion function */
	void *			mscr_doneinfo;	/* IN: info for request completion */

	MSCAPI_REQUEST_DATA	mscr_data;	/* additional data */
	};

/* mscr_fnc */
#define	MSCAPI_REQUEST_NONE		0	/* No request */
#define	MSCAPI_REQUEST_OPEN		1	/* Open storage */
#define	MSCAPI_REQUEST_CLOSE		2	/* Close storage */
#define	MSCAPI_REQUEST_REGISTER_NOTIFY	3	/* Register MSCAPI notification */
#define	MSCAPI_REQUEST_READ_CAPACITY	4	/* Get storage capacity */
#define	MSCAPI_REQUEST_READ_BLOCK	5	/* Read multiple blocks */
#define	MSCAPI_REQUEST_WRITE_BLOCK	6	/* write multiple blocks */
#define	MSCAPI_REQUEST_PREVENT		7	/* Lock storage */
#define	MSCAPI_REQUEST_LOADEJECT	8	/* Load storage */

/* mscr_status */
#define	MSCAPI_STATUS_NONE		0	/* No status */
#define	MSCAPI_STATUS_OK		1
#define	MSCAPI_STATUS_ERROR		2
#define	MSCAPI_STATUS_NOT_SUPPORT	3	/* not supported request command */

/*

Type:	MSCAPI_NOTIFY

Function:
	Represents a single notification from MSCAPI service to MSCAPI client.

Description:
	In order to make a mass storage API notification, a client allocates
	and initializes a |MSCAPI_NOTIFY| object, and passes it to the
	appropriate service using MSCAPI_REQUEST_REGISTER_NOTIFY request.
	The service processes the request, and then notifies the client
	(by calling a call-back function passed by the client).
	When the service detect medium insertion or removal event, the service
	notify event to DataPump by calling a notify function passed by
	|MSCAPI_NOTIFY| object.

Contents:
	|mscn_notify|
		Specify notification code.

	|mscn_notifyfn|
	|mscn_notifyinfo|
		Specify the notification function to be called by service when
		the service detect medium insertion or removal event.
		|mscn_notifyinfo| is for use by clients, and is not interpreted
		by the service.

	|mscn_blksize|
		This is only for |MSCAPI_NOTIFY_MEDIUM_CHANGE| notification.
		The service put logical block size in bytes.

	|mscn_blkcnt|
		This is only for |MSCAPI_NOTIFY_MEDIUM_CHANGE| notification.
		The service put total number of logical blocks.

See Also:
	MSCAPI_REQUEST

*/

typedef	void
MSCAPI_NOTIFY_FN(
	MSCAPI_NOTIFY *		/*pNotify*/,
	void *			/*pInfo*/
	);

typedef MSCAPI_NOTIFY_FN *PMSCAPI_NOTIFY_FN;


struct _MSCAPI_NOTIFY
	{
	MSCAPI_UINT32		mscn_notify;	/* notify code */
	MSCAPI_NOTIFY_FN *	mscn_notifyfn;	/* notify function */
	void *			mscn_notifyinfo;/* notify function info */
	MSCAPI_UINT32		mscn_blksize;	/* the block size in bytes */
	MSCAPI_UINT32		mscn_blkcnt;	/* the total number of blocks */
	};

/* mscn_notify */
#define	MSCAPI_NOTIFY_NONE			0
#define	MSCAPI_NOTIFY_MEDIUM_INSERT		1
#define	MSCAPI_NOTIFY_MEDIUM_REMOVE		2
#define	MSCAPI_NOTIFY_MEDIUM_CHANGE		3	/* valid mscn_blksize & mscn_blkcnt */
#define	MSCAPI_NOTIFY_CDROM_TRAYIN_HAS_MEDIUM	4
#define	MSCAPI_NOTIFY_CDROM_TRAYIN_NO_MEDIUM	5
#define	MSCAPI_NOTIFY_CDROM_TRAYOUT		6

/*

	We define some interfaces between MSCAPI client and service. The MSCAPI
	client talks with the service using these interfaces

	MSCAPI_INITIALIZE_FN
		This is MSCAPI service initialization function. This function
		returns pointer of MSCAPI_SERVICE strucure, but we just define
		the function that returning void *.

	MSCAPI_REQUEST_FN
		This is MSCAPI request function. This function is called by the
		MSCAPI client. When the server complete specified request, call
		the request complete function.

*/


typedef	void *			/* pServiceContext */
MSCAPI_INITIALIZE_FN(
	void *			/* pClientContext */
	);

typedef MSCAPI_INITIALIZE_FN *PMSCAPI_INITIALIZE_FN;

typedef	void
MSCAPI_REQUEST_FN(
	MSCAPI_REQUEST *	/* pRequest */
	);

typedef MSCAPI_REQUEST_FN *PMSCAPI_REQUEST_FN;

/**** end of umscapi.h ****/
#endif /* _UMSCAPI_H_ */
