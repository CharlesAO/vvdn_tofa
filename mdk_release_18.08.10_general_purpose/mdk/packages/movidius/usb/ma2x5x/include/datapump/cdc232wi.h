/* cdc232wi.h	Tue Jun 16 1998 16:10:35 johnk */

/*

Module:  cdc232wi.h

Function:
	"on-the-wire" definitions for USB CDC-232 Devices

Version:
	V1.40b	Tue Jun 16 1998 16:10:35 johnk	Edit level 3

Copyright notice:
	This file copyright (C) 1997-1998 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	June 1997

Revision history:
   1.00a  Sun Jun  8 1997 20:49:10  tmm
	Module created.

   1.40b  6/16/1998 johnk
	Module ported from vspwire.h
*/

#ifndef _CDC232WI_H_		/* prevent multiple includes */
#define _CDC232WI_H_

#include "usbif.h"		/* get the USB interface data */
#include "vspwire.h"		/* get the "on-the-wire" definition */

/****************************************************************************\
|
|	The request type codes for CDC-232 control packets.
|
\****************************************************************************/

#define	CDC232_bmRequestType_TYPE	VSP_bmRequestType_TYPE

#define	CDC232_bmRequestType_DEST	VSP_bmRequestType_DEST

#define	CDC232_bmRequestType_IN		VSP_bmRequestType_IN
				 
#define	CDC232_bmRequestType_OUT	VSP_bmRequestType_OUT

/****************************************************************************\
|
|	The request codes for CDC-232 control packets.
|
\****************************************************************************/


#define	CDC232_bRequest_IFC_ENABLE	VSP_bRequest_IFC_ENABLE
#define	CDC232_bRequest_IMM_CHAR	VSP_bRequest_IMM_CHAR
#define	CDC232_bRequest_SET_XON		VSP_bRequest_SET_XON
#define	CDC232_bRequest_SET_XOFF	VSP_bRequest_SET_XOFF
#define	CDC232_bRequest_SET_EVENTMASK	VSP_bRequest_SET_EVENTMASK
#define	CDC232_bRequest_GET_EVENTMASK	VSP_bRequest_GET_EVENTMASK
#define	CDC232_bRequest_GET_CHARS	VSP_bRequest_GET_CHARS
#define	CDC232_bRequest_GET_PROPS	VSP_bRequest_GET_PROPS
#define	CDC232_bRequest_GET_COMM_STATUS	VSP_bRequest_GET_COMM_STATUS
#define	CDC232_bRequest_PURGE		VSP_bRequest_PURGE
#define	CDC232_bRequest_SET_FLOW	VSP_bRequest_SET_FLOW
#define	CDC232_bRequest_GET_FLOW	VSP_bRequest_GET_FLOW
#define CDC232_bRequest_EMBED_EVENTS	VSP_bRequest_EMBED_EVENTS
#define CDC232_bRequest_GET_EVENTSTATE	VSP_bRequest_GET_EVENTSTATE
#define CDC232_bRequest_SET_CHARS	VSP_bRequest_SET_CHARS

#define	__CDC232B(n)	(1L << CDC232_bRequest_##n)

#define	MASK_CDC232_IN_REQUESTS	( \
	__CDC232B(GET_BAUDDIV) | \
	__CDC232B(GET_LINE_CTL) | \
	__CDC232B(GET_MDMSTS) | \
	__CDC232B(GET_EVENTMASK) | \
	__CDC232B(GET_CHARS) | \
	__CDC232B(GET_PROPS) | \
	__CDC232B(GET_COMM_STATUS) | \
	__CDC232B(GET_FLOW) | \
	__CDC232B(GET_EVENTSTATE) \
	)
	

#define MASK_CDC232_OUT_REQUESTS (\
	__CDC232B(IFC_ENABLE) | \
	__CDC232B(IMM_CHAR) | \
	__CDC232B(SET_XON) | \
	__CDC232B(SET_XOFF) | \
	__CDC232B(SET_EVENTMASK) | \
	__CDC232B(PURGE) | \
	__CDC232B(SET_FLOW) | \
	__CDC232B(EMBED_EVENTS) | \
	__CDC232B(SET_CHARS) \
	)

#define	MASK_CDC232_ALL_REQUESTS \
	(MASK_CDC232_IN_REQUESTS | MASK_CDC232_OUT_REQUESTS)
	


/* The 2-byte layout of EVENTMASK data */
#define CDC232EVM_ANY_RX	(1 << 0)	/* receive any character */
#define CDC232EVM_SPEC_RX	(1 << 1)	/* receive special chars */
#define CDC232EVM_TX_EMPTY	(1 << 2)	/* tx queue empty	 */
#define CDC232EVM_CTS_CHG	(1 << 3)	/* CTS change state	 */
#define CDC232EVM_DSR_CHG	(1 << 4)	/* DSR change state	 */
#define CDC232EVM_RLSD_CHG	(1 << 5)	/* RSLD state change	 */
#define CDC232EVM_BREAK		(1 << 6)	/* BREAK char received	 */
#define CDC232EVM_LNERR		(1 << 7)	/* line error		 */
#define CDC232EVM_RI		(1 << 8)	/* Ring Indicate	 */
#define CDC232EVM_RESV1		(1 << 9)	/* reserved		 */
#define CDC232EVM_RXQ_80FULL	(1 << 10)	/* rxq is 80% full	 */

/* Special character indicies */
#define CDC232SPC_EOFCHAR	0		/* EOF character	*/
#define CDC232SPC_ERRCHAR	1		/* error recv. char	*/
#define CDC232SPC_BRKCHAR	2		/* break recv. char	*/
#define CDC232SPC_EVCHAR	3		/* event recv. char	*/
#define CDC232SPC_XONCHAR	4		/* xon character	*/
#define CDC232SPC_XOFFCHAR	5		/* xoff character	*/

/* The 2-byte layout of the PURGE command */
#define CDC232PRG_TX_CLRCANCEL	(1 << 0)	/* clear TX queue,	*/
						/*  cancel pending I/O  */
#define CDC232PRG_RX_CLRCANCEL	(1 << 1)	/* clear RX queue,	*/
						/*  cancel pending I/O  */
#define CDC232PRG_TX_CLR	(1 << 2)	/* clear TX queue	*/
#define CDC232PRG_RX_CLR	(1 << 3)	/* clear RX queue	*/

/**** Embedded Events ****/
#define CDC232EMB_ESCCHAR_RAW	0		/* The escape char is data */
						/*  from the input stream  */
#define CDC232EMB_LSRDATA	1		/* A new LSR value appears */
						/*  as next byte, followed */
						/*  by one data byte.	   */
#define CDC232EMB_LSR		2		/* A new LSR value appears */
						/*  as the next byte	   */
#define CDC232EMB_MSR		3		/* A new MSR value appears */
						/*  as the next byte	   */
						
/* Embedded LSR value */
#define CDC232EMLSR_DRDY	(1 << 0)	/* data ready		*/
#define CDC232EMLSR_HWOVR	(1 << 1)	/* rx hw overrun	*/
#define CDC232EMLSR_PARERR	(1 << 2)	/* parity error		*/
#define CDC232EMLSR_FRERR	(1 << 3)	/* framing error	*/
#define CDC232EMLSR_BREAK	(1 << 4)	/* break detected	*/

/* Embedded MSR values */
#define CDC232EMMSR_CTS_CHG	(1 << 0)	/* CTS state has changed */
#define CDC232EMMSR_DSR_CHG	(1 << 1)	/* DSR state has changed */
#define CDC232EMMSR_RI_TEDGE	(1 << 2)	/* trailing edge of RI	 */
#define CDC232EMMSR_DCD_CHG	(1 << 3)	/* DCD state has changed */
#define CDC232EMMSR_CTS		(1 << 4)	/* current CTS state	 */
#define CDC232EMMSR_DSR		(1 << 5)	/* current DSR state	 */
#define CDC232EMMSR_RI		(1 << 6)	/* current RI state	 */
#define CDC232EMMSR_DCD		(1 << 7)	/* current DCD state	 */

/* The layout of an interrupt byte */
#define CDC232INT_RXCNT_MASK	(0x3f)		/* num data pkts avail	 */
#define CDC232INT_TX_RDY	(1 << 6)	/* ready for tx data	  */
#define CDC232INT_EVT		(1 << 7)	/* an event has occurred */

/**** end of cdc232wi.h ****/
#endif /* _CDC232WI_H_ */
