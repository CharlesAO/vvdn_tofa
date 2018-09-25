/* vspwire.h	Sat May 03 2003 14:42:41 tmm */

/*

Module:  vspwire.h

Function:
	"on-the-wire" definitions for USB Virtual Serial Port Devices

Version:
	V1.83e	Sun May 04 2003 13:53:38 tmm	Edit level 7

Copyright notice:
	This file copyright (C) 1997-2003 by

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
	Update to match documents.

   1.47d   12/13/1998  tmm
	The special character EVCHAR is NOT the escape char!

   1.53b   3/30/2000  tmm
	Needed to have SET_CHARS in the mask of requests....

   1.83e  Sat May 03 2003 14:42:41  tmm
	Update to match 950115 rev D specification, adding MCCI_SET_BAUDRATE and 
	MCCI_GET_BAUDRATE.

   1.83e  Sun May 04 2003 13:53:38  tmm
	1062:  the VSP event byte definitions were never used.  Begin process
	of obsolescence and preparing for something useful.

*/

#ifndef _VSPWIRE_H_		/* prevent multiple includes */
#define _VSPWIRE_H_

#include "usbif.h"		/* get the USB interface data */

/****************************************************************************\
|
|	The request type codes for VSP control packets.
|
\****************************************************************************/

#define	VSP_bmRequestType_TYPE	USB_bmRequestType_TYPE_VENDOR

#define	VSP_bmRequestType_DEST	USB_bmRequestType_DEST_IFC

#define	VSP_bmRequestType_IN	(VSP_bmRequestType_TYPE | \
				 VSP_bmRequestType_DEST | \
				 USB_bmRequestType_DIR_TOHOST)
				 
#define	VSP_bmRequestType_OUT	(VSP_bmRequestType_TYPE | \
				 VSP_bmRequestType_DEST | \
				 USB_bmRequestType_DIR_TODEV)

/****************************************************************************\
|
|	The request codes for VSP control packets.
|
\****************************************************************************/


#define	VSP_bRequest_IFC_ENABLE		0x00	/* enable/disable interface */
#define	VSP_bRequest_SET_BAUDDIV	0x01	/* set baud rate */
#define	VSP_bRequest_GET_BAUDDIV	0x02	/* get baud rate */
#define	VSP_bRequest_SET_LINE_CTL	0x03	/* set line control */
#define	VSP_bRequest_GET_LINE_CTL	0x04	/* get line control */
#define	VSP_bRequest_SET_BREAK		0x05	/* set break */
#define	VSP_bRequest_IMM_CHAR		0x06	/* send immediate char */
#define	VSP_bRequest_SET_MHS		0x07	/* set modem handshaking */
#define	VSP_bRequest_GET_MDMSTS		0x08	/* get modem status */
#define	VSP_bRequest_SET_XON		0x09	/* emulate XON */
#define	VSP_bRequest_SET_XOFF		0x0A	/* emulate XOFF */
#define	VSP_bRequest_SET_EVENTMASK	0x0B	/* set event mask */
#define	VSP_bRequest_GET_EVENTMASK	0x0C	/* get event mask */
#define	VSP_bRequest_SET_CHAR		0x0D	/* set special char */
#define	VSP_bRequest_GET_CHARS		0x0E	/* get special chars */
#define	VSP_bRequest_GET_PROPS		0x0F	/* get properties */
#define	VSP_bRequest_GET_COMM_STATUS	0x10	/* get comm status */
#define	VSP_bRequest_RESET		0x11	/* reset comm port */
#define	VSP_bRequest_PURGE		0x12	/* purge queue */
#define	VSP_bRequest_SET_FLOW		0x13	/* set flow control */
#define	VSP_bRequest_GET_FLOW		0x14	/* get flow control */
#define VSP_bRequest_EMBED_EVENTS	0x15	/* embed events */
#define VSP_bRequest_GET_EVENTSTATE	0x16	/* Get current event state */
#define VSP_bRequest_SET_RECEIVE	0x17	/* set/clear rx throttle */
#define VSP_bRequest_GET_RECEIVE	0x18	/* get rx throttle  */
#define VSP_bRequest_SET_CHARS		0x19	/* Set chars at once */
#define VSP_bRequest_TEST_SUSPEND	0x1a	/* test if ok to suspend */
#define VSP_bRequest_GET_SUSPEND_DATA	0x1b	/* get state context	 */
#define VSP_bRequest_SET_SUSPEND_DATA	0x1c	/* restore state context */
#define	VSP_bRequest_GET_BAUDRATE	0x1d	/* get baudrate in bits/sec */
#define	VSP_bRequest_SET_BAUDRATE	0x1e	/* set baudrate in bits/sec */

/* the masking scheme below will break if you define a request about 0x1F! */

#define	__VB(n)	(1L << VSP_bRequest_##n)

#define	MASK_VSP_IN_REQUESTS	( \
	__VB(GET_BAUDDIV) | \
	__VB(GET_LINE_CTL) | \
	__VB(GET_MDMSTS) | \
	__VB(GET_EVENTMASK) | \
	__VB(GET_CHARS) | \
	__VB(GET_PROPS) | \
	__VB(GET_COMM_STATUS) | \
	__VB(GET_FLOW) | \
	__VB(GET_EVENTSTATE) | \
	__VB(GET_RECEIVE) | \
	__VB(TEST_SUSPEND) | \
	__VB(GET_SUSPEND_DATA) | \
	__VB(GET_BAUDRATE) \
	)
	

#define MASK_VSP_OUT_REQUESTS (\
	__VB(IFC_ENABLE) | \
	__VB(SET_BAUDDIV) | \
	__VB(SET_LINE_CTL) | \
	__VB(SET_BREAK) | \
	__VB(IMM_CHAR) | \
	__VB(SET_MHS) | \
	__VB(SET_XON) | \
	__VB(SET_XOFF) | \
	__VB(SET_EVENTMASK) | \
	__VB(SET_CHAR) | \
	__VB(RESET) | \
	__VB(PURGE) | \
	__VB(SET_FLOW) | \
	__VB(EMBED_EVENTS) | \
	__VB(SET_RECEIVE) | \
	__VB(SET_CHARS) | \
	__VB(SET_SUSPEND_DATA) | \
	__VB(SET_BAUDRATE) \
	)

#define	MASK_VSP_ALL_REQUESTS (MASK_VSP_IN_REQUESTS | MASK_VSP_OUT_REQUESTS)

/* The 2-byte layout of LINE_CTL data */
#define VSPLCTL_STOPB_MASK	0x0f
#define VSPLCTL_STOPB_1		0		/* 1 stop bit */
#define VSPLCTL_STOPB_1_5	1		/* 1.5 stop bits */
#define VSPLCTL_STOPB_2		2		/* 2 stop bits */
#define VSPLCTL_STOPB_2_OLD	3		/* 2 stop bits */

#define VSPLCTL_PAR_SHIFT	4
#define VSPLCTL_PAR_MASK	(0x0f << VSPLCTL_PAR_SHIFT)
#define VSPLCTL_PAR_NONE	(0 << VSPLCTL_PAR_SHIFT)	 
#define VSPLCTL_PAR_ODD 	(1 << VSPLCTL_PAR_SHIFT)	 
#define VSPLCTL_PAR_EVEN	(2 << VSPLCTL_PAR_SHIFT)	 
#define VSPLCTL_PAR_MARK	(3 << VSPLCTL_PAR_SHIFT)	 
#define VSPLCTL_PAR_SPACE	(4 << VSPLCTL_PAR_SHIFT)	 

#define VSPLCTL_WLEN_SHIFT	8
#define VSPLCTL_WLEN_MASK	(0xff << VSPLCTL_WLEN_SHIFT)
#define VSPLCTL_WLEN_5		(5 << VSPLCTL_WLEN_SHIFT)
#define VSPLCTL_WLEN_6		(6 << VSPLCTL_WLEN_SHIFT)
#define VSPLCTL_WLEN_7		(7 << VSPLCTL_WLEN_SHIFT)
#define VSPLCTL_WLEN_8		(8 << VSPLCTL_WLEN_SHIFT)
#define VSPLCTL_WLEN_16		(16 << VSPLCTL_WLEN_SHIFT)


/* The 2-byte layout of SET_MHS data */
#define VSPMHS_DES_DTRSTATE	(1 << 0)	/* desired DTR state */
#define VSPMHS_DES_RTSSTATE	(1 << 1)	/* desired RTS state */
#define VSPMHS_DTR_MASK		(1 << 8)	/* 0=ignore DTR req  */
#define VSPMHS_RTS_MASK		(1 << 9)	/* 0=ignore RTS req  */

/* The 2-byte layout of EVENTMASK data */
#define VSPEVM_ANY_RX		(1 << 0)	/* receive any character */
#define VSPEVM_SPEC_RX		(1 << 1)	/* receive special char  */
#define VSPEVM_TX_EMPTY		(1 << 2)	/* tx queue empty	 */
#define VSPEVM_CTS_CHG		(1 << 3)	/* CTS change state	 */
#define VSPEVM_DSR_CHG		(1 << 4)	/* DSR change state	 */
#define VSPEVM_RLSD_CHG		(1 << 5)	/* RSLD (DCD) state change */
#define VSPEVM_BREAK		(1 << 6)	/* BREAK char received	 */
#define VSPEVM_LNERR		(1 << 7)	/* line error		 */
#define VSPEVM_RI		(1 << 8)	/* Ring Indicate	 */
#define VSPEVM_RESV1		(1 << 9)	/* reserved		 */
#define VSPEVM_RXQ_80FULL	(1 << 10)	/* rxq is 80% full	 */

/* Special character indicies */
#define VSPSPC_EOFCHAR		0		/* EOF character	*/
#define VSPSPC_ERRCHAR		1		/* error recv. char	*/
#define VSPSPC_BRKCHAR		2		/* break recv. char	*/
#define VSPSPC_EVCHAR		3		/* char that causes the
						||   SPEC_RX flag to
						||   get set.
						*/
#define VSPSPC_XONCHAR		4		/* xon character	*/
#define VSPSPC_XOFFCHAR		5		/* xoff character	*/

/* The 1-byte layout of MDMSTS */
#define VSPMDM_DTR		(1 << 0)	/* DTR state		*/
#define VSPMDM_RTS		(1 << 1)	/* DTR state		*/
#define VSPMDM_CTS		(1 << 4)	/* DTR state		*/
#define VSPMDM_DSR		(1 << 5)	/* DTR state		*/
#define VSPMDM_RI		(1 << 6)	/* DTR state		*/
#define VSPMDM_DCD		(1 << 7)	/* DTR state		*/

/* The 2-byte layout of the PURGE command */
#define VSPPRG_TX_CLRCANCEL	(1 << 0)	/* clear TX queue,	*/
						/*  cancel pending I/O  */
#define VSPPRG_RX_CLRCANCEL	(1 << 1)	/* clear RX queue,	*/
						/*  cancel pending I/O  */
#define VSPPRG_TX_CLR		(1 << 2)	/* clear TX queue	*/
#define VSPPRG_RX_CLR		(1 << 3)	/* clear RX queue	*/

/**** Embedded Events ****/
#define VSPEMB_ESCCHAR_RAW	0		/* The escape char is data */
						/*  from the input stream  */
#define VSPEMB_LSRDATA		1		/* A new LSR value appears */
						/*  as next byte, followed */
						/*  by one data byte.	   */
#define VSPEMB_LSR		2		/* A new LSR value appears */
						/*  as the next byte	   */
#define VSPEMB_MSR		3		/* A new MSR value appears */
						/*  as the next byte	   */
						
/* Embedded LSR value */
#define VSPEMLSR_DRDY		(1 << 0)	/* data ready		*/
#define VSPEMLSR_HWOVR		(1 << 1)	/* rx hw overrun	*/
#define VSPEMLSR_PARERR		(1 << 2)	/* parity error		*/
#define VSPEMLSR_FRERR		(1 << 3)	/* framing error	*/
#define VSPEMLSR_BREAK		(1 << 4)	/* break detected	*/

/* Embedded MSR values */
#define VSPEMMSR_CTS_CHG	(1 << 0)	/* CTS state has changed */
#define VSPEMMSR_DSR_CHG	(1 << 1)	/* DSR state has changed */
#define VSPEMMSR_RI_TEDGE	(1 << 2)	/* trailing edge of RI	 */
#define VSPEMMSR_DCD_CHG	(1 << 3)	/* DCD state has changed */
#define VSPEMMSR_CTS		(1 << 4)	/* current CTS state	 */
#define VSPEMMSR_DSR		(1 << 5)	/* current DSR state	 */
#define VSPEMMSR_RI		(1 << 6)	/* current RI state	 */
#define VSPEMMSR_DCD		(1 << 7)	/* current DCD state	 */

/* the wValue for GET_SUSPEND_DATA/SET_SUSPEND_DATA */
#define	VSP_SUSPEND_wValue_D1	0x0001		/* selects D1 data */
#define	VSP_SUSPEND_wValue_D3	0x0003		/* selects D3 data */

/* The layout of an interrupt byte:  V1 through V3 */
/* 
|| this information is historical, because no firmware implemented this 
|| in a reliable way, and so this should be considered obsolete.  The
|| event bit was more useful than the other implementations.  You may find artifacts
|| of this code in the VSP sources, but it was never actually used.
*/
#define VSPINT_RXCNT_MASK	(0x3f)		/* num data pkts avail */
#define VSPINT_TX_RDY		(1 << 6)	/* ready for tx data */
#define VSPINT_EVT		(1 << 7)	/* an event has occurred */

/* the layout of an interrupt byte:  V4 up */
#define	VSPINT_V4_RSV0		(1 << 0)	/* bit 0: reserved */
#define	VSPINT_V4_RSV1		(1 << 1)	/* bit 1: reserved */
#define	VSPINT_V4_RSV2		(1 << 2)	/* bit 2: reserved */
#define	VSPINT_V4_RSV3		(1 << 3)	/* bit 3: reserved */
#define VSPINT_V4_IN_EMPTY	(1 << 4)	/* there is no data to be read on the IN pipe */
#define	VSPINT_V4_OUT_BUSY	(1 << 5)	/* there is no space avaible for data on the OUT pipe */
#define	VSPINT_V4_RSV6		(1 << 6)	/* bit 6: reserved */
#define	VSPINT_V4_EVT		(1 << 7)	/* an event has occurred */

/**** end of vspwire.h ****/
#endif /* _VSPWIRE_H_ */
