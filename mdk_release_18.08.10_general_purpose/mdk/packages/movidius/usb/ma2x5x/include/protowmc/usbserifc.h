/* usbserifc.h	Thu Oct 23 2014 15:41:20 chwon */

/*

Module:  usbserifc.h

Function:
	Basic serial API for Bella.

Version:
	V3.13b	Thu Oct 23 2014 15:41:20 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2006, 2013-2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	May 2006

Revision history:
   1.00a  Sun May 21 2006 01:06:59  tmm
	Module created.

   1.97d  Thu Jul 20 2006 12:00:04  chwon
	Added start/stop plane event

   3.11b  Tue Jan 22 2013 08:50:11  tmm
	16507: make the read and write APIs use size_t for buffer sizes.

   3.11b  Wed Jan 30 2013 20:55:15  chwon
	16386: add Usbseri_ReadSubmit() and Usbseri_WriteSubmit() prototype.

   3.11b  Fri Jan 25 2013 16:32:32  chwon
	16507: remove stddef.h and change type from size_t to unsigned.

   3.11c  Fri Mar 08 2013 12:16:14  chwon
	16386: Added Usbseri_ReadSubmit_V2() and Usbseri_WriteSubmit_V2().

   3.13b  Thu Oct 23 2014 15:41:20  chwon
	18595: Add USBSERIFC_BEGIN_DECLS and USBSERIFC_END_DECLS.

*/

#ifndef _USBSERIFC_H_		/* prevent multiple includes */
#define _USBSERIFC_H_

/*

	DO NOT INCLUDE OR USE THE DATAPUMP HEADER FILES HERE!!!

This file is intended as a bridge between the DataPump environment and the
client's environment.

*/

#ifdef __cplusplus
# define USBSERIFC_BEGIN_DECLS	extern "C" {
# define USBSERIFC_END_DECLS	}
#else
# define USBSERIFC_BEGIN_DECLS	/* nothing */
# define USBSERIFC_END_DECLS	/* nothing */
#endif

/****************************************************************************\
|
|	Routines exported to the caller
|
\****************************************************************************/

USBSERIFC_BEGIN_DECLS

typedef enum _USBSERI_STATUS
	{
	USBSERI_STATUS_OK = 0,
	USBSERI_STATUS_NO_RESOURCE,
	USBSERI_STATUS_NOT_CONFIGURED,
	USBSERI_STATUS_INVALID_PARAMETER,
	USBSERI_STATUS_IO_ERROR,
	USBSERI_STATUS_NO_CALLBACK
	} USBSERI_STATUS;

typedef void
UsbSeri_SubmitDone_t(
	void *			/*pDoneCtx*/,
	USBSERI_STATUS		/*status*/,
	unsigned char *		/*pBuffer*/,
	unsigned int		/*nBuffer*/,
	unsigned int		/*nActual*/
	);

void
Usbseri_ReadSubmit(
	void *			/*hPort*/,
	unsigned char *		/*pBuffer*/,
	unsigned int		/*nBuffer*/,
	UsbSeri_SubmitDone_t *	/*pDoneFn*/,
	void *			/*pDoneCtx*/
	);

USBSERI_STATUS
Usbseri_ReadSubmit_V2(
	void *			/*hPort*/,
	unsigned char *		/*pBuffer*/,
	unsigned int		/*nBuffer*/,
	int			/*bool: fUncachedBuffer*/,
	UsbSeri_SubmitDone_t *	/*pDoneFn*/,
	void *			/*pDoneCtx*/
	);

void
Usbseri_WriteSubmit(
	void *			/*hPort*/,
	unsigned char *		/*pBuffer*/,
	unsigned int		/*nBuffer*/,
	UsbSeri_SubmitDone_t *	/*pDoneFn*/,
	void *			/*pDoneCtx*/
	);

USBSERI_STATUS
Usbseri_WriteSubmit_V2(
	void *			/*hPort*/,
	unsigned char *		/*pBuffer*/,
	unsigned int		/*nBuffer*/,
	int			/*bool: fUncachedBuffer*/,
	UsbSeri_SubmitDone_t *	/*pDoneFn*/,
	void *			/*pDoneCtx*/
	);

typedef void
USB_SerialEventHandler_t(
	void *			/*hPort*/,
	void *			/*pClientContext*/,
	unsigned char		/*event code */,
	void *			/*pEventData (optional)*/
	);

/*
|| The event codes are brought in from the outside world using a struct
|| that must be filled in and supplied by the client....  Each value may be
|| -1 (indicating that no event of the kind is to be actually sent), otherwise
|| a value in 0..255 giving the desired event code for the event.
*/
typedef struct USBSERI_EVENT_CODE_MAP
	{
	int	RxDataAvailable;
	int	DtrChangeHigh;
	int	DtrChangeLow;
	int	EscapeDataConnect;
	int	TxNotFull;
	int	BreakReceived;
	int	StartPlane;
	int	StopPlane;
	} USBSERI_EVENT_CODE_MAP;

/*
|| When you are initializing an instance of the event code map, please
|| be sure to use these macros, so that your code will work correctly
|| even if we add events in the future.
||
*/
/* DON'T ADD PARAMETERS TO THIS MACRO: make a new version! */
#define	USBSERI_EVENT_CODE_MAP_INIT_V2(					\
	a_RxDataAvailable,						\
	a_DtrChangeHigh,						\
	a_DtrChangeLow,							\
	a_EscapeDataConnect,						\
	a_TxNotFull,							\
	a_BreakReceived,						\
	a_StartPlane,							\
	a_StopPlane							\
	)								\
	{								\
	(a_RxDataAvailable),						\
	(a_DtrChangeHigh),						\
	(a_DtrChangeLow),						\
	(a_EscapeDataConnect),						\
	(a_TxNotFull),							\
	(a_BreakReceived),						\
	(a_StartPlane),							\
	(a_StopPlane)							\
	}

/* DON'T ADD PARAMETERS TO THIS MACRO: make a new version! */
#define	USBSERI_EVENT_CODE_MAP_SETUP_V2(				\
	a_pMap,								\
	a_RxDataAvailable,						\
	a_DtrChangeHigh,						\
	a_DtrChangeLow,							\
	a_EscapeDataConnect,						\
	a_TxNotFull,							\
	a_BreakReceived,						\
	a_StartPlane,							\
	a_StopPlane							\
	)								\
    do	{								\
	(a_pMap)->RxDataAvailable = (a_RxDataAvailable);		\
	(a_pMap)->DtrChangeHigh = (a_DtrChangeHigh);			\
	(a_pMap)->DtrChangeLow = (a_DtrChangeLow);			\
	(a_pMap)->EscapeDataConnect = (a_EscapeDataConnect);		\
	(a_pMap)->TxNotFull = (a_TxNotFull);				\
	(a_pMap)->BreakReceived = (a_BreakReceived);			\
	(a_pMap)->StartPlane = (a_StartPlane);				\
	(a_pMap)->StopPlane = (a_StopPlane);				\
	} while (0)

#define	USBSERI_EVENT_CODE_MAP_INIT_V1(					\
	a_RxDataAvailable,						\
	a_DtrChangeHigh,						\
	a_DtrChangeLow,							\
	a_EscapeDataConnect,						\
	a_TxNotFull,							\
	a_BreakReceived							\
	)								\
	USBSERI_EVENT_CODE_MAP_INIT_V2(					\
		a_RxDataAvailable,					\
		a_DtrChangeHigh,					\
		a_DtrChangeLow,						\
		a_EscapeDataConnect,					\
		a_TxNotFull,						\
		a_BreakReceived,					\
		/* a_StartPlane */ -1,					\
		/* a_StopPlane */ -1					\
		)

/* DON'T ADD PARAMETERS TO THIS MACRO: make a new version! */
#define	USBSERI_EVENT_CODE_MAP_SETUP_V1(				\
	a_pMap,								\
	a_RxDataAvailable,						\
	a_DtrChangeHigh,						\
	a_DtrChangeLow,							\
	a_EscapeDataConnect,						\
	a_TxNotFull,							\
	a_BreakReceived							\
	)								\
	USBSERI_EVENT_CODE_MAP_SETUP_V2(				\
		a_pMap,							\
		a_RxDataAvailable,					\
		a_DtrChangeHigh,					\
		a_DtrChangeLow,						\
		a_EscapeDataConnect,					\
		a_TxNotFull,						\
		a_BreakReceived,					\
		/* a_StartPlane */ -1,					\
		/* a_StopPlane */ -1					\
		)

void *
USB_OpenDevice(
	unsigned		/*instance*/
	);

void
USB_CloseDevice(
	void *			/*hPort returned by USB_OpenDevice*/
	);

unsigned
USB_Read(
	void *			/*hPort*/,
	unsigned char *		/*pBuffer*/,
	unsigned		/*nBuffer*/
	);

unsigned
USB_Write(
	void *			/*hPort*/,
	const unsigned char *	/*pBuffer*/,
	unsigned		/*dataSize*/
	);

int
USB_EnableRxDataFlow(
	void *			/*hPort*/,
	int			/*bool: enable*/
	);

unsigned
USB_QueryDtrRts(
	void *			/*hPort*/
	);

unsigned short
USB_QueryFreeTxSpace(
	void *			/*hPort*/
	);

void
USB_SetClearModemStatus(
	void *			/*hPort*/,
	unsigned int		/*mask*/,
	unsigned int		/*state*/
	);

int
USB_RegisterEventNotify(
	void *				/*hPort*/,
	USB_SerialEventHandler_t *	/*pHandler*/,
	void *				/*pClientContext*/,
	const USBSERI_EVENT_CODE_MAP *	/*pEventMap*/
	);

USBSERIFC_END_DECLS

/****************************************************************************\
|
|	The following signals define the simulated "modem status" bits
|	that are communicated to the host over the USB UART.  These
|	lines might not be supported on all kinds of USB transports; for
|	example, none of these are supported on a "diagnostic" or
|	"device management" port; and CTS is not supported on ACM modems.
|
|	Bits 0..6 deliberately match the meanings assigned by the CDC
|	ACM spec.  Bit 7 is deliberately reserved and will not be used.
|	Bits 8..31 are local extensions to the spec, for signals that we
|	might need to transport, notably CTS.
|
\****************************************************************************/


#define	USBSERI_MODEM_STATE_DCD		0x01u	/* DCD line to host */
#define	USBSERI_MODEM_STATE_DSR		0x02u	/* DSR line to host */
#define	USBSERI_MODEM_STATE_BREAK	0x04u	/* BREAK status to host */
#define	USBSERI_MODEM_STATE_RI		0x08u	/* RI line to host */
#define	USBSERI_MODEM_STATE_FE		0x10u	/* Framing Error status to host */
#define	USBSERI_MODEM_STATE_PE		0x20u	/* Parity Error status to host */
#define	USBSERI_MODEM_STATE_OE		0x40u	/* Overrun Error status to host */
#define	USBSERI_MODEM_STATE_RSV7	0x80u	/* <<reserved>> */
#define	USBSERI_MODEM_STATE_CTS		0x100u	/* CTS line to host */

/* for convenience, a mask of the ACM-defined bits */
#define	USBSERI_MODEM_STATE_MASK_ACM	0x7Fu

/****************************************************************************\
|
|	Similarly, we have DTR and RTS lines in the simulated "modem
|	control" bits that are communicated from the host.  Again, not all
|	USB transport protocols will support these signals.  If they are
|	not supported, the USB UART will assert DTR and RTS as soon as the
|	USB UART function is activated by the host, and will deassert
|	when the function is deactivated.
|
\****************************************************************************/

#define	USBSERI_MODEM_CONTROL_DTR	0x01u	/* DTR line from host */
#define	USBSERI_MODEM_CONTROL_RTS	0x02u	/* RTS line from host */


/**** end of usbserifc.h ****/
#endif /* _USBSERIFC_H_ */
