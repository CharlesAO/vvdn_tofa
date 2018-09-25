/* vspctlda.h	Sun May 04 2003 14:22:19 tmm */

/*

Module:  vspctlda.h

Function:
	VSP Control Packet Data Structs and Definitions.  See 950115 for more info.

Version:
	V1.83c	Sun May 04 2003 14:22:19 tmm	Edit level 8

Copyright notice:
	This file copyright (C) 1997-2003 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	John Keys, MCCI Corporation	August 1997

Revision history:
   1.10d  Sun Aug 24 1997 19:27:28  johnk
	Module created.

   1.50a  3/15/1999  tmm
	Change to using __TMS_TYPE_DEF_STRUCT, because that makes the
	file more useful.

   1.60c  3/31/2000  tmm
	Change the mask for VSPFLOW_CTL_NOSET, so that we ignore the error
	abort bit if set.

   1.60d  6/22/2000  tmm
	Fix errors in baudrate table.

   1.61c  1/5/2001  tmm
	Bug 415: add LENGTH_..._WIRE.  Add wire version of VSPSPCCHARS,
	for consistency and so we can add a LENGTH_..._WIRE macro.

   1.83c  Sun May 04 2003 14:22:19  tmm
	1063:  add the TEST_SUSPEND response structure.
 
*/

#ifndef _VSPCTLDA_H_		/* prevent multiple includes */
#define _VSPCTLDA_H_


/*
|| Comms Properties
*/
__TMS_TYPE_DEF_STRUCT(VSPCOMMPROPS);
__TMS_TYPE_DEF_STRUCT(VSPCOMMPROPS_WIRE);

struct __TMS_STRUCTNAME(VSPCOMMPROPS)
	{
	__TMS_UINT16	wLength;	/* size of response in bytes (0x64) */
	__TMS_UINT16  bcdVersion;
	__TMS_UINT32  ulServiceMask;
	__TMS_UINT32  ulReserved1;
	__TMS_UINT32  ulMaxTxQueue;
	__TMS_UINT32  ulMaxRxQueue;
	__TMS_UINT32  ulMaxBaud;
	__TMS_UINT32  ulProvSubType;
	__TMS_UINT32  ulProvCaps;
	__TMS_UINT32  ulSettableParams;
	__TMS_UINT32  ulSettableBaud;
	__TMS_UINT16  wSettableData;
	__TMS_UINT16  wSettableStopParity;
	__TMS_UINT32  ulCurrentTxQueue;
	__TMS_UINT32  ulCurrentRxQueue;
	__TMS_UINT32  ulProvSpec1;
	__TMS_UINT32  ulProvSpec2;
	__TMS_UINT16  uniProvName[20];
	};

struct __TMS_STRUCTNAME(VSPCOMMPROPS_WIRE)
	{
	__TMS_UINT8	wLength[2];	/* size of response in bytes (0x64) */
	__TMS_UINT8	bcdVersion[2];
	__TMS_UINT8	ulServiceMask[4];
	__TMS_UINT8	ulReserved1[4];
	__TMS_UINT8	ulMaxTxQueue[4];
	__TMS_UINT8	ulMaxRxQueue[4];
	__TMS_UINT8	ulMaxBaud[4];
	__TMS_UINT8	ulProvSubType[4];
	__TMS_UINT8	ulProvCaps[4];
	__TMS_UINT8	ulSettableParams[4];
	__TMS_UINT8	ulSettableBaud[4];
	__TMS_UINT8	wSettableData[2];
	__TMS_UINT8	wSettableStopParity[2];
	__TMS_UINT8	ulCurrentTxQueue[4];
	__TMS_UINT8	ulCurrentRxQueue[4];
	__TMS_UINT8	ulProvSpec1[4];
	__TMS_UINT8	ulProvSpec2[4];
	__TMS_UINT8	uniProvName[20][2];
	};

#define __TMS_LENGTHOF_VSPCOMMPROPS_WIRE	\
	(__TMS_OFFSET(__TMS_VSPCOMMPROPS_WIRE, uniProvName) + 2 * 20)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define LENGTHOF_VSPCOMMPROPS_WIRE	__TMS_LENGTHOF_VSPCOMMPROPS_WIRE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */
	
/* for bcdVersion */	
#define VSPCPROP_BCDVERSION	0x100

/* for ulServiceMask */
#define	VSPCPROP_SVM_SERIALCOMM	1	/* provider code:  "serial comm" */

/* for ulProvSubType -- must match numerically with values in ntddser.h */
#define	VSPCPROP_TYPE_UNSPECIFIED 0	/* not specified */
#define	VSPCPROP_TYPE_RS232	1	/* RS-232 port */
#define	VSPCPROP_TYPE_RS422	3	/* RS-422 port */
#define VSPCPROP_TYPE_MODEM	6	/* or similar TA-like-thing */

/* for ulProvCaps */
#define VSPCPROP_CAPS_DTRDSR	(1 << 0)	/* have DTR & DSR */
#define VSPCPROP_CAPS_RTSCTS	(1 << 1)	/* have RTS & CTS */
#define VSPCPROP_CAPS_CD	(1 << 2)	/* have CD */
#define VSPCPROP_CAPS_CKPAR	(1 << 3)	/* can check parity */
#define VSPCPROP_CAPS_XONXOFF	(1 << 4)	/* can do XON/XOFF */
#define VSPCPROP_CAPS_XCHARS	(1 << 5)	/* can set XON/XOF chars */
/*				(1 << 6)	-- reserved */
/*				(1 << 7)	-- reserved */
#define VSPCPROP_CAPS_SPECCHARS	(1 << 8)	/* can set special chars */
#define VSPCPROP_CAPS_16BITMODE (1 << 9)	/* can handle 16-bit chars */

/* for ulSettableParams */
#define VSPCPROP_SP_PARITY	(1 << 0)	/* can set parity type */
#define VSPCPROP_SP_BAUD	(1 << 1)	/* can set baudrate */
#define VSPCPROP_SP_WLEN	(1 << 2)	/* can set word length */
#define VSPCPROP_SP_STOPB	(1 << 3)	/* can set stop bits */
#define VSPCPROP_SP_HS		(1 << 4)	/* can set handshaking */
#define VSPCPROP_SP_CKPAR	(1 << 5)	/* can set parity checking */
#define VSPCPROP_SP_CD		(1 << 6)	/* can set carrier-detect
						|| checking 
						*/

/* for ulSettableBaud */
#define VSPCPROP_SB_75		(1 << 0)
#define VSPCPROP_SB_110		(1 << 1)
#define VSPCPROP_SB_134		(1 << 2)
#define VSPCPROP_SB_150		(1 << 3)
#define VSPCPROP_SB_300		(1 << 4)
#define VSPCPROP_SB_600		(1 << 5)
#define VSPCPROP_SB_1200	(1 << 6)
#define VSPCPROP_SB_1800	(1 << 7)
#define VSPCPROP_SB_2400	(1 << 8)
#define VSPCPROP_SB_4800	(1 << 9)
#define VSPCPROP_SB_7200	(1 << 10)
#define VSPCPROP_SB_9600	(1 << 11)
#define VSPCPROP_SB_14400	(1 << 12)
#define VSPCPROP_SB_19200	(1 << 13)
#define VSPCPROP_SB_38400	(1 << 14)
#define VSPCPROP_SB_56000	(1 << 15)
#define VSPCPROP_SB_128000	(1 << 16)
#define VSPCPROP_SB_115200	(1 << 17)
#define VSPCPROP_SB_57600	(1 << 18)
#define VSPCPROP_SB_USERDEF	(1 << 28)

/* For wSettableData */
#define VSPCPROP_SD_5		(1 << 0)
#define VSPCPROP_SD_6		(1 << 1)
#define VSPCPROP_SD_7		(1 << 2)
#define VSPCPROP_SD_8		(1 << 3)
#define VSPCPROP_SD_16		(1 << 4)
#define VSPCPROP_SD_16X		(1 << 5)
	
/* For wSettableStopParity */
#define VSPCPROP_SSP_1SB	(1 << 0)
#define VSPCPROP_SSP_1P5SB	(1 << 1)
#define VSPCPROP_SSP_2SB	(1 << 2)
#define VSPCPROP_SSP_PARNONE	(1 << 8)
#define VSPCPROP_SSP_PARODD	(1 << 9)
#define VSPCPROP_SSP_PAREVN	(1 << 10)
#define VSPCPROP_SSP_PARMRK	(1 << 11)
#define VSPCPROP_SSP_PARSPC	(1 << 12)

/* for uniProvName[20] */
#define	VSPCPROP_PROV_MCCI_V1	\
		'M', 'C', 'C', 'I', ' ', 'U', 'S', 'B', ' ', 'S', \
		'e', 'r', 'i', 'a', 'l', ' ', 'V', '1', '.', '0'

#define	VSPCPROP_PROV_MCCI_V2	\
		'M', 'C', 'C', 'I', ' ', 'U', 'S', 'B', ' ', 'S', \
		'e', 'r', 'i', 'a', 'l', ' ', 'V', '2', '.', '0'

#define	VSPCPROP_PROV_MCCI	VSPCPROP_PROV_MCCI_V2

/*
|| Serial Status 
*/

__TMS_TYPE_DEF_STRUCT(VSPSERSTS);
__TMS_TYPE_DEF_STRUCT(VSPSERSTS_WIRE);

struct __TMS_STRUCTNAME(VSPSERSTS)
	{
	__TMS_UINT32	ulErrors;
	__TMS_UINT32	ulHoldReasons;
	__TMS_UINT32  ulAmountInInQueue;
	__TMS_UINT32  ulAmountInOutQueue;
	__TMS_UINT8	bEofReceived;
	__TMS_UINT8	bWaitForImmediate;
	__TMS_UINT8	bResv;
	};

struct __TMS_STRUCTNAME(VSPSERSTS_WIRE)
	{
	__TMS_UINT8	ulErrors[4];
	__TMS_UINT8	ulHoldReasons[4];
	__TMS_UINT8	ulAmountInInQueue[4];
	__TMS_UINT8	ulAmountInOutQueue[4];
	__TMS_UINT8	bEofReceived;
	__TMS_UINT8	bWaitForImmediate;
	__TMS_UINT8	bResv;
	};

#define	__TMS_LENGTHOF_VSPSERSTS_WIRE \
		(__TMS_OFFSET(__TMS_VSPSERSTS_WIRE, bResv) + 1)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define LENGTHOF_VSPSERSTS_WIRE	__TMS_LENGTHOF_VSPSERSTS_WIRE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/* for ulErrors */
#define VSPSERSTS_ERR_BRK	(1 << 0)
#define VSPSERSTS_ERR_FRM	(1 << 1)
#define VSPSERSTS_ERR_HOVR	(1 << 2)
#define VSPSERSTS_ERR_QOVR	(1 << 3)
#define VSPSERSTS_ERR_PAR	(1 << 4)

/* for ulHoldReasons */
#define VSPSERSTS_TXHLD_CTS	(1 << 0)
#define VSPSERSTS_TXHLD_DSR	(1 << 1)
#define VSPSERSTS_TXHLD_DCD	(1 << 2)
#define VSPSERSTS_TXHLD_XON	(1 << 3)
#define VSPSERSTS_TXHLD_XOFF	(1 << 4)
#define VSPSERSTS_TXHLD_BRK	(1 << 5)
#define VSPSERSTS_RXHLD_DSR	(1 << 6)


/*
|| Flow Control 
*/
__TMS_TYPE_DEF_STRUCT(VSPFLOWCTL);
__TMS_TYPE_DEF_STRUCT(VSPFLOWCTL_WIRE);

struct __TMS_STRUCTNAME(VSPFLOWCTL)
	{
	__TMS_UINT32	ulControlHandshake;
	__TMS_UINT32	ulFlowReplace;
	__TMS_UINT32	ulXonLimit;
	__TMS_UINT32	ulXoffLimit;
	};

struct __TMS_STRUCTNAME(VSPFLOWCTL_WIRE)
	{
	__TMS_UINT8	ulControlHandshake[4];
	__TMS_UINT8	ulFlowReplace[4];
	__TMS_UINT8	ulXonLimit[4];
	__TMS_UINT8	ulXoffLimit[4];
	};

#define	__TMS_LENGTHOF_VSPFLOWCTL_WIRE	\
	(__TMS_OFFSET(__TMS_VSPFLOWCTL_WIRE, ulXoffLimit[4]))

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define LENGTHOF_VSPFLOWCTL_WIRE	__TMS_LENGTHOF_VSPFLOWCTL_WIRE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

#define VSPFLOW_CTL_DTR_MASK	(3)		/* The DTR mask */
#define	VSPFLOW_CTL_DTR_SHIFT	0
#define	VSPFLOW_CTL_DTR_OFF	(0 << VSPFLOW_CTL_DTR_SHIFT)
#define	VSPFLOW_CTL_DTR_ON	(1 << VSPFLOW_CTL_DTR_SHIFT)
#define	VSPFLOW_CTL_DTR_HS	(2 << VSPFLOW_CTL_DTR_SHIFT)
#define	VSPFLOW_CTL_RSV2	(1 << 2)	/* reserved */
#define	VSPFLOW_CTL_CTS_HS	(1 << 3)	/* CTS is a handshake line */
#define	VSPFLOW_CTL_DSR_HS	(1 << 4)	/* DSR is a handshake line */
#define	VSPFLOW_CTL_DCD_HS	(1 << 5)	/* DCD is a handshake line */
#define	VSPFLOW_CTL_OUT_HS_MASK \
			(VSPFLOW_CTL_CTS_HS | \
			 VSPFLOW_CTL_DSR_HS | \
			 VSPFLOW_CTL_DCD_HS)
#define	VSPFLOW_CTL_DSR_RXGATE	(1 << 6)	/* DSR gates rx data */
#define	VSPFLOW_CTL_RSV7	(0x7FFFFF80)	/* mask of reserved bits */
#define	VSPFLOW_CTL_IGNORE_31	(1 << 31)	/* preserved but ignored */

#define	VSPFLOW_CTL_NOSET	(VSPFLOW_CTL_RSV2 | VSPFLOW_CTL_RSV7)

/* the flow control mask */
#define	VSPFLOW_FLOW_AUTOTX	(1 << 0)	/* xon/xoff control output */
#define	VSPFLOW_FLOW_AUTORX	(1 << 1)	/* send xon/xoff to control
						|| input (towards host)
						|| data flow.
						*/
#define	VSPFLOW_FLOW_ERROR_CHAR	(1 << 2)	/* substitute error char */
#define	VSPFLOW_FLOW_NULL_STRIPPING (1 << 3)	/* delete nulls */
#define	VSPFLOW_FLOW_BREAK_CHAR (1 << 4)	/* substitute break char */
#define	VSPFLOW_FLOW_RSV5	(1 << 5)	/* reserved */

#define	VSPFLOW_FLOW_RTS_SHIFT	6		/* RTS field base */
#define	VSPFLOW_FLOW_RTS_MASK	(3 << VSPFLOW_FLOW_RTS_SHIFT)
#define	VSPFLOW_FLOW_RTS_OFF	(0 << VSPFLOW_FLOW_RTS_SHIFT)	/* RTS is statically off */
#define	VSPFLOW_FLOW_RTS_ON	(1 << VSPFLOW_FLOW_RTS_SHIFT)	/* RTS is statically on */
#define	VSPFLOW_FLOW_RTS_FLOW	(2 << VSPFLOW_FLOW_RTS_SHIFT)	/* RTS is rx flow control */
#define	VSPFLOW_FLOW_RTS_TXGATE	(3 << VSPFLOW_FLOW_RTS_SHIFT)	/* RTS is tx envelope */
#define	VSPFLOW_FLOW_RSV8	(0x7FFFFE00)	/* bits 30..8 are reserved */
#define	VSPFLOW_FLOW_XOFF_CONTINUE (1u << 31)	/* send xoff even if stopped */

#define	VSPFLOW_FLOW_NOSET	(VSPFLOW_FLOW_RSV8 | VSPFLOW_FLOW_RSV5)

/*
|| Special Characters 
*/

__TMS_TYPE_DEF_STRUCT(VSPSPCCHARS);
__TMS_TYPE_DEF_STRUCT(VSPSPCCHARS_WIRE);

struct __TMS_STRUCTNAME(VSPSPCCHARS)
	{
	__TMS_UINT8	bEOFChar;
	__TMS_UINT8	bErrorChar;
	__TMS_UINT8	bBreakChar;
	__TMS_UINT8	bEventChar;
	__TMS_UINT8	XonChar;
	__TMS_UINT8	XoffChar;
	};

struct __TMS_STRUCTNAME(VSPSPCCHARS_WIRE)
	{
	__TMS_UINT8	bEOFChar;
	__TMS_UINT8	bErrorChar;
	__TMS_UINT8	bBreakChar;
	__TMS_UINT8	bEventChar;
	__TMS_UINT8	XonChar;
	__TMS_UINT8	XoffChar;
	};

#define	__TMS_LENGTHOF_VSPSPCCHARS_WIRE	\
	(__TMS_OFFSET(__TMS_VSPSPCCHARS_WIRE, XoffChar) + 1)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define LENGTHOF_VSPSPCCHARS_WIRE	__TMS_LENGTHOF_VSPSPCCHARS_WIRE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/*
|| Data returned from MCCI_TEST_SUSPEND
*/
__TMS_TYPE_DEF_STRUCT(VSP_TESTSUSPEND_RESULT_V1);
__TMS_TYPE_DEF_STRUCT(VSP_TESTSUSPEND_RESULT_V1_WIRE);

struct __TMS_STRUCTNAME(VSP_TESTSUSPEND_RESULT_V1)
	{
	__TMS_UINT16	wTestLength;
	__TMS_UINT16	bcdTestFormat;
	__TMS_UINT16	bmSuspendFlags;
	__TMS_UINT16	wLengthD1Data;
	__TMS_UINT16	wLengthD3Data;
	};

struct __TMS_STRUCTNAME(VSP_TESTSUSPEND_RESULT_V1_WIRE)
	{
	__TMS_UINT8	wTestLength[2];
	__TMS_UINT8	bcdTestFormat[2];
	__TMS_UINT8	bmSuspendFlags[2];
	__TMS_UINT8	wLengthD1Data[2];
	__TMS_UINT8	wLengthD3Data[2];
	};

/* the size of the structure on the wire, and the value to put in wTestLength */
#define	__TMS_LENGTHOF_VSP_TESTSUSPEND_RESULT_V1_WIRE	\
	(__TMS_OFFSET(__TMS_VSP_TESTSUSPEND_RESULT_V1_WIRE, wLengthD3Data[1]) + 1)

/* the version to be used */
#define __TMS_VSP_TESTSUSPEND_RESULT_VERSION_1	0x0100

/* the suspend flags */
#define __TMS_VSP_TESTSUSPEND_FLAGS_D1_OK	(0x0001)
#define __TMS_VSP_TESTSUSPEND_FLAGS_D1_OK_SAVE	(0x0002)
#define __TMS_VSP_TESTSUSPEND_FLAGS_D3_OK	(0x0010)
#define __TMS_VSP_TESTSUSPEND_FLAGS_D3_OK_SAVE	(0x0020)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define LENGTHOF_VSP_TESTSUSPEND_RESULT_V1_WIRE	\
		__TMS_LENGTHOF_VSP_TESTSUSPEND_RESULT_V1_WIRE
# define VSP_TESTSUSPEND_RESULT_VERSION_1 __TMS_VSP_TESTSUSPEND_RESULT_VERSION_1
# define VSP_TESTSUSPEND_FLAGS_D1_OK		__TMS_VSP_TESTSUSPEND_FLAGS_D1_OK
# define VSP_TESTSUSPEND_FLAGS_D1_OK_SAVE	__TMS_VSP_TESTSUSPEND_FLAGS_D1_OK_SAVE
# define VSP_TESTSUSPEND_FLAGS_D3_OK		__TMS_VSP_TESTSUSPEND_FLAGS_D3_OK
# define VSP_TESTSUSPEND_FLAGS_D3_OK_SAVE	__TMS_VSP_TESTSUSPEND_FLAGS_D3_OK_SAVE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of vspctlda.h ****/
#endif /* _VSPCTLDA_H_ */
