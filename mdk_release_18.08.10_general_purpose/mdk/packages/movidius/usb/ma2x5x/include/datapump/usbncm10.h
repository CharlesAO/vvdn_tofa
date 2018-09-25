/* usbncm10.h	Thu May 08 2014 11:05:00 ssyang */

/*

Module:  usbncm10.h

Function:
	USB Communication Device Class (CDC) V1.2 Ethernet Network Control Model 
	(NCM) V1.0 constants

Version:
	V3.13a	Thu May 08 2014 11:05:00 ssyang	Edit level 8

Copyright notice:
	This file copyright (C) 2009-2011, 2013-2014 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Mats Webjorn, MCCI Corporation	September 2009

Revision history:
   2.01a  Tue Sep 08 2009 23:09:38 maw
	8854: Module created.

   2.01a  Thu Oct 01 2009 22:15:00 maw
   	Remove TMS-prefix from defines, update structure names,
   	update macro parameter names and comments

   3.01c Wed Apr 21 2010 10:54:29 saurabh
	9995: Implemented Errata for NCM Rev 1.0 specification.
	Rename "reserved" at offset 26 of the NTB Parameter Structure 
	to "wNtbOutMaxDatagrams".
	Defined bit map for D5 in bmNetworkCapabilities to signal that
	the device can transfer an extended data structure for NtbInputSize
	in the GetNtbInputSize/SetNtbInputSize requests.
	Updated USB_Comm_NCM_NTB_INPUT_SIZE data structure definition.

   3.01d Wed Jan  5 2011 15:00:46 saurabh
	11649: Correct definition of USB_Comm_NCM_NTH32_IS_VALID_BLOCK_LENGTH
	because the upper bound for NTB32 size should be 0xFFFFFFFF instead
	of 0xFFFF as specified in the previous definition.
	Also updated the macro definition (for both NTB32 and NTB16) to conform
	to MCCI coding standards.

   3.01d  Fri Jan 07 2011 11:48:50  chwon
	11649: do not use __TMS_UINT16 and __TMS_UINT32 without include cdef.h.
	USB class protocol definition header file should not include any header
	file by convention, so replace __TMS_UINT16/32 to constant.

   3.01f  Tue Apr 10 2012 12:10:24  ssyang
	15149: Fixed the incorrect a_wNdpeDatagramLength >= a_wNthHeaderLength
	checking in USB_Comm_NCM_NDP16_IS_VALID_DATAGRAM() macro.
	
   3.11d  Wed Oct 09 2013 13:03:02  chwon
	17778: Add USB_bDescriptorType_Comm_NCM_INTERFACE_PreErrata1 definition
	for pre	NCM 1.0 Errata1.

   3.13a  Thu May 08 2014 11:05:00  ssyang
	18236: Fixed coverity issue in USB_Comm_NCM_NTH16_TO_NDP16_HEADER.

*/

#ifndef _USBNCM10_H_		/* prevent multiple includes */
#define _USBNCM10_H_

/****************************************************************************\
|
|	Subclass, and protocol codes: section 4.1 and 4.2
|
\****************************************************************************/

/* Interface subclasses */
#define	USB_bInterfaceSubClass_CommNCM		0x0D	/* Network Control Model */


/* Comm class Protocol codes */
#define	USB_bInterfaceProtocol_CommNCM_NONE	0x00	/* No encap cmd / resp */
#define	USB_bInterfaceProtocol_CommNCM_EXTERNAL	0xFE	/* External Protocol */

/* Comm data Protocol codes */
#define	USB_bInterfaceProtocol_DataNCM		0x01	/* Network Transfer Block */

/****************************************************************************\
|
|	Functional Detail Descriptor Codes: section 4.4
|
\****************************************************************************/

/*
|| this byte appears in bGuidDescriptorType to identify an NCM
|| functional descriptor.
*/
#define	USB_bDescriptorSubtype_Comm_NCM		0x1A

/****************************************************************************\
|
|	Network Control Model Requests Codes:  section 6.2
|
\****************************************************************************/

#define	USB_bRequest_Ncm_GET_NTB_PARAMETERS	0x80
#define	USB_bRequest_Ncm_GET_NET_ADDRESS	0x81
#define	USB_bRequest_Ncm_SET_NET_ADDRESS	0x82
#define	USB_bRequest_Ncm_GET_NTB_FORMAT		0x83
#define	USB_bRequest_Ncm_SET_NTB_FORMAT		0x84
#define	USB_bRequest_Ncm_GET_NTB_INPUT_SIZE	0x85
#define	USB_bRequest_Ncm_SET_NTB_INPUT_SIZE	0x86
#define	USB_bRequest_Ncm_GET_MAX_DATAGRAM_SIZE	0x87
#define	USB_bRequest_Ncm_SET_MAX_DATAGRAM_SIZE	0x88
#define	USB_bRequest_Ncm_GET_CRC_MODE		0x89
#define	USB_bRequest_Ncm_SET_CRC_MODE		0x8A

/* the request-to-string map must be augmented by USB_bRequest_Comm__INIT */
#define	USB_bRequest_Ncm__INIT				\
	{ USB_bRequest_Ncm_GET_NTB_PARAMETERS,		\
		"NCM_GET_NTB_PARAMETERS" },		\
	{ USB_bRequest_Ncm_GET_NET_ADDRESS,		\
		"NCM_GET_NET_ADDRESS" },		\
	{ USB_bRequest_Ncm_SET_NET_ADDRESS,		\
		"NCM_SET_NET_ADDRESS" },		\
	{ USB_bRequest_Ncm_GET_NTB_FORMAT,		\
		"NCM_GET_NTB_FORMAT" },			\
	{ USB_bRequest_Ncm_SET_NTB_FORMAT,		\
		"NCM_SET_NTB_FORMAT" },			\
	{ USB_bRequest_Ncm_GET_NTB_INPUT_SIZE,		\
		"NCM_GET_NTB_INPUT_SIZE" },		\
	{ USB_bRequest_Ncm_SET_NTB_INPUT_SIZE,		\
		"NCM_SET_NTB_INPUT_SIZE" },		\
	{ USB_bRequest_Ncm_GET_MAX_DATAGRAM_SIZE,	\
		"NCM_GET_MAX_DATAGRAM_SIZE" },		\
	{ USB_bRequest_Ncm_SET_MAX_DATAGRAM_SIZE,	\
		"NCM_SET_MAX_DATAGRAM_SIZE" },		\
	{ USB_bRequest_Ncm_GET_CRC_MODE,		\
		"NCM_GET_CRC_MODE" },			\
	{ USB_bRequest_Ncm_SET_CRC_MODE,		\
		"NCM_SET_CRC_MODE" }

/****************************************************************************\
|
|	Notification Elements:  No other notifications than those defined
|	by CDC ECM are used
|
\****************************************************************************/


/****************************************************************************\
|
|	NCM Functional Descriptor: section 5.2.1
|
\****************************************************************************/

struct USB_Comm_FunctionalDescriptor_NCM
	{
	unsigned char	bFunctionLength;	/* varies */
	unsigned char	bDescriptorType;	/* CS_INTERFACE */
	unsigned char	bDescriptorSubtype;	/* USB_bDescriptorSubtype_Comm_NCM */
	unsigned char	bcdNcmVersion[2];	/* 0x0100 */
	unsigned char	bmNetworkCapabilities;	/* the capabilities mask */
	};

/* size of the descriptor, in bytes */
#define USB_Comm_FunctionalDescriptor_NCD__SIZE	6

/* Pre NCM 1.0 errata, Class-specific descriptor, INTERACE */
#define	USB_bDescriptorType_Comm_NCM_INTERFACE_PreErrata1	0x22

/* the bits in bmNetworkCapabilities */
#define	USB_Comm_NCM_bmNetworkCapabilities_HAS_PACKET_FILTER	(1u << 0)
#define	USB_Comm_NCM_bmNetworkCapabilities_HAS_ADDRESS_CMDS	(1u << 1)
#define	USB_Comm_NCM_bmNetworkCapabilities_HAS_ENCAP_CMD	(1u << 2)
#define	USB_Comm_NCM_bmNetworkCapabilities_HAS_DATAGRAM_CMD	(1u << 3)
#define	USB_Comm_NCM_bmNetworkCapabilities_HAS_CRCMODE_CMD	(1u << 4)
#define	USB_Comm_NCM_bmNetworkCapabilities_HAS_NTB_EXTENDED_INPUT_SIZE	(1u << 5)

#define	USB_Comm_NCM_bmNetworkCapabilities__INIT		\
	{ USB_Comm_NCM_bmNetworkCapabilities_HAS_PACKET_FILTER,	\
		"HAS_PACKET_FILTER" },					\
	{ USB_Comm_NCM_bmNetworkCapabilities_HAS_ADDRESS_CMDS,	\
		"HAS_ADDRESS_CMDS" },					\
	{ USB_Comm_NCM_bmNetworkCapabilities_HAS_ENCAP_CMD,	\
		"HAS_ENCAP_CMD" },					\
	{ USB_Comm_NCM_bmNetworkCapabilities_HAS_DATAGRAM_CMD,	\
		"HAS_DATAGRAM_CMD" },					\
	{ USB_Comm_NCM_bmNetworkCapabilities_HAS_CRCMODE_CMD,	\
		"HAS_CRCMODE_CMD" },					\
	{ USB_Comm_NCM_bmNetworkCapabilities_HAS_NTB_EXTENDED_INPUT_SIZE,	\
		"HAS_NTB_EXTENDED_INPUT_SIZE" }

/****************************************************************************\
|
|	The layout of GET_NTB_PARAMETERS data: section 6.2.1
|
\****************************************************************************/

struct USB_Comm_NCM_NTB_PARAMETERS_RESPONSE
	{
	unsigned char	wLength[2];
	unsigned char	bmNtbFormatsSupported[2];
	unsigned char	dwNtbInMaxSize[4];
	unsigned char	wNdpInDivisor[2];
	unsigned char	wNdpInPayloadRemainder[2];
	unsigned char	wNdpInAlignment[2];
	unsigned char	wReserved14[2];
	unsigned char	dwNtbOutMaxSize[4];
	unsigned char	wNdpOutDivisor[2];
	unsigned char	wNdpOutPayloadRemainder[2];
	unsigned char	wNdpOutAlignment[2];
	unsigned char	wNtbOutMaxDatagrams[2];
	};

#define USB_Comm_NCM_NTB_PARAMETERS_RESPONSE__SIZE	28

#define USB_Comm_NCM_NTB_FORMAT_16_SUPPORTED	(1u << 0)
#define USB_Comm_NCM_NTB_FORMAT_32_SUPPORTED	(1u << 1)

/****************************************************************************\
|
|	The layout of GET/SET_NET_ADDRESS data: section 6.2.2/6.2.3
|
\****************************************************************************/

struct USB_Comm_NCM_NET_ADDRESS
	{
	unsigned char	Address[6];
	};

#define USB_Comm_NCM_NET_ADDRESS__SIZE		6

/****************************************************************************\
|
|	The layout of GET/SET_NTB_FORMAT data: section 6.2.4/6.2.5
|
\****************************************************************************/

struct USB_Comm_NCM_NTB_FORMAT
	{
	unsigned char	wData[2];
	};

#define USB_Comm_NCM_NTB_FORMAT__SIZE		2

#define USB_Comm_NCM_NTB_FORMAT_16		0
#define USB_Comm_NCM_NTB_FORMAT_32		1

/****************************************************************************\
|
|	The layout of GET/SET_NTB_INPUT_SIZE data: section 6.2.6/6.2.7
|
\****************************************************************************/

struct USB_Comm_NCM_NTB_INPUT_SIZE
	{
	unsigned char	dwNtbInMaxSize[4];
	unsigned char	wNtbInMaxDatagrams[2];
	unsigned char	wReserved6[2];
	};

#define USB_Comm_NCM_NTB_INPUT_SIZE__SIZE	4
#define USB_Comm_NCM_NTB_EXTENDED_INPUT_SIZE__SIZE	8

/****************************************************************************\
|
|	The layout of GET/SET_MAX_DATAGRAM_SIZE data: section 6.2.8/6.2.9
|
\****************************************************************************/

struct USB_Comm_NCM_MAX_DATAGRAM_SIZE
	{
	unsigned char	wData[2];
	};

#define USB_Comm_NCM_MAX_DATAGRAM_SIZE__SIZE	2

/****************************************************************************\
|
|	The layout of GET/SET_CRC_MODE payload data: section 6.2.10/6.2.11
|
\****************************************************************************/

struct USB_Comm_NCM_CRC_MODE
	{
	unsigned char	wData[2];
	};

#define USB_Comm_NCM_CRC_MODE__SIZE		2

#define USB_Comm_NCM_CRC_MODE_NOCRC		0x0000u
#define USB_Comm_NCM_CRC_MODE_CRC		0x0001u

/****************************************************************************\
|
|	The layout of an NCM 16-bit Transfer Header (NTH16): section 3.2.1
|
\****************************************************************************/

struct USB_Comm_NCM_NTH16
	{
	unsigned char	dwSignature[4];		/* the signature: the
						|| DWORD 0x484D434E in
						|| little-endian format.
						|| "NCMH"
						*/
	unsigned char	wHeaderLength[2];	/* size of this header
						|| block -- always
						|| NCM_NTH16_TRANSFER_HEADER__SIZE
						*/
	unsigned char	wSequence[2];		/* sequence number */
	unsigned char	wBlockLength[2];	/* size of this NCM block,
						|| in bytes, including
						|| header.
						*/
	unsigned char	wNdpIndex[2];		/* index of first NDP-16 in
						|| this block.
						*/
	};

#define USB_Comm_NCM_NTH16__SIZE		12
#define USB_Comm_NCM_NTB16_SIZE_MAX		0xFFFF
#define USB_Comm_NCM_NTH16_BLOCK_SIGNATURE	0x484D434Eul

#define	USB_Comm_NCM_NTH16_IS_VALID_BLOCK_LENGTH(a_wLength)		\
	(USB_Comm_NCM_NTH16__SIZE <= (a_wLength) &&			\
	 (a_wLength) <= USB_Comm_NCM_NTB16_SIZE_MAX)

/*
|| Validate that the NDP-index is aligned, that it doesn't point into 
|| the NTH, and that at least the NDP wNextNdpIndex is within the NCM block.
|| Then USB_Comm_NCM_NDP16_IS_VALID_SIZE() can be used to validate that
|| the entire NDP is within the NCM block, and finally should
|| USB_Comm_NCM_NDP16_LAST_DATAGRAM_ENTRY() be used to get the last NDP 
|| entry and validate that it's NULL.
*/
#define	USB_Comm_NCM_NTH16_IS_VALID_NDP_INDEX(				\
	a_wBlockLength, a_wNthHeaderLength, a_wNdpIndex)		\
	((((a_wNdpIndex) & 3) == 0) &&					\
	  ((a_wNdpIndex) >= (a_wNthHeaderLength)) &&			\
	 (((a_wBlockLength) && ((a_wNdpIndex) <= ((a_wBlockLength) - 8))) || \
	  ((a_wBlockLength) == 0)))

/****************************************************************************\
|
|	The layout of an NCM 32-bit Transfer Header (NTH32): section 3.2.2
|
\****************************************************************************/

struct USB_Comm_NCM_NTH32
	{
	unsigned char	dwSignature[4];		/* the signature: the
						|| DWORD 0x686D636E in
						|| little-endian format.
						|| "ncmh"
						*/
	unsigned char	wHeaderLength[2];	/* size of this header
						|| block -- always
						|| NCM_NTH32_TRANSFER_HEADER__SIZE
						*/
	unsigned char	wSequence[2];		/* sequence number */
	unsigned char	dwBlockLength[4];	/* size of this NCM block,
						|| in bytes, including
						|| header.
						*/
	unsigned char	dwNdpIndex[4];		/* index of first NDP-32 in
						|| this block.
						*/
	};

#define USB_Comm_NCM_NTH32__SIZE		16
#define USB_Comm_NCM_NTB32_SIZE_MAX		0xFFFFFFFFu
#define USB_Comm_NCM_NTH32_BLOCK_SIGNATURE	0x686D636Eul

#define	USB_Comm_NCM_NTH32_IS_VALID_BLOCK_LENGTH(a_dwLength)		\
	(USB_Comm_NCM_NTH32__SIZE <= (a_dwLength) &&			\
	 (a_dwLength) <= USB_Comm_NCM_NTB32_SIZE_MAX)

/*
|| Validate that the NDP-index is aligned, that it doesn't point into 
|| the NTH, and that at least the NDP dwNextNdpIndex is within the NCM block.
|| Then USB_Comm_NCM_NDP32_IS_VALID_SIZE() can be used to validate that
|| the entire NDP is within the NCM block, and finally should
|| USB_Comm_NCM_NDP32_LAST_DATAGRAM_ENTRY() be used to get the last NDP 
|| entry and validate that it's NULL.
*/
#define	USB_Comm_NCM_NTH32_IS_VALID_NDP_INDEX(				\
	a_dwBlockLength, a_wNthHeaderLength, a_dwNdpIndex) 		\
	((((a_dwNdpIndex) & 3) == 0) &&					\
	  ((a_dwNdpIndex) >= (a_wNthHeaderLength)) &&			\
	 (((a_dwBlockLength) && ((a_dwNdpIndex) <= ((a_dwBlockLength) - 12))) || \
	  ((a_dwBlockLength) == 0)))

/****************************************************************************\
|
|	The layout of an NCM 16-bit Datagram Pointers (NDP16): section 3.3.1
|
\****************************************************************************/

/*

Type:	struct USB_Comm_NCM_NDP16_HEADER

Index:	Const:	USB_Comm_NCM_NDP16_HEADER__SIZE
	Const:	USB_Comm_NCM_NDP16_SIGNATURE_NO_CRC32
	Const:	USB_Comm_NCM_NDP16_SIGNATURE_CRC32
	Name:	USB_Comm_NCM_NDP16__SIZE()
	Name:	USB_Comm_NCM_NDP16_IS_VALID_SIZE()

Function:
	The structure that represents a wire-format 16-bit NCM datagram 
	pointer table.

Description:
	A |struct USB_Comm_NCM_NDP16_HEADER| block describes a set
	of datagrams within a NCM block.

Contents:
	unsigned char dwSignature[4];
		The signature of this object, useful for defending against
		buggy software on the other end of the link.  This
		must always be either |USB_Comm_NCM_NDP16_SIGNATURE_NO_CRC32| or
		|USB_Comm_NCM_NDP16_SIGNATURE_CRC32|.

	unsigned char wLength[2];
		The size of this datagram pointer table in bytes, from the
		signature to the last byte.

	unsigned char wNextNdpIndex[2];
		The index of the next datagram pointer block (or zero).

	unsigned char DatagramPointerArray[];
		The (variable-length) array of datagram pointer entries
		(each one of type |struct USB_Comm_NCM_NDP16_ENTRY|).
		This array always ends with an entry that is all zeroes.
		Therefore, there is always at least two entries in the
		array, one datagram element pointer and a terminator.

See also:
	|struct USB_Comm_NCM_NDP16_ENTRY|

Notes:
	All entries are expressed in little-endian form.

	The size of this structure is given by the value of the macro
	|USB_Comm_NCM_NDP16_ENTRY__SIZE|, which includes
	the guaranteed two entries (one or more datagram entry + one
	terminating NULL datagram entry) in the |DatagramPointerArray|.

	The macro |USB_Comm_NCM_NDP16__SIZE(n)| returns the size in
	bytes of a datagram pointer table including pointers to |n|
	datagram.  |n| should not include the required terminating
	all-zero entry.  So for example, |USB_Comm_NCM_NDP16__SIZE(3)|
	returns 24 (8 bytes of header, 12 bytes of datagram pointer
	entries, and 4 bytes for the guaranteed zero entry at
	the end.

	The macro |USB_Comm_NCM_NDP16_IS_VALID_SIZE(wBlockLen, wNdpIndex,
	wFpLength)| determines whether a datagram pointer table of size
	wFpLength can occur at offset wNdpIndex of a block of size wBlockLen,
	returning TRUE if so, FALSE otherwise.

	The macro |USB_Comm_NCM_NDP16_LENGTH_TO_NUM_ENTRIES(wLength)|
	computes how many datagram pointer entries are in the specified
	datagram pointer table.  For symmetry with |USB_Comm_NCM_NDP16__SIZE()|
	The result does not include the terminating zero entry.
	If |USB_Comm_NCM_NDP16_IS_VALID_SIZE()| is not TRUE for
	this datagram pointer table, then the result of this macro is
	not specified.

*/

struct USB_Comm_NCM_NDP16_HEADER
	{
	unsigned char	dwSignature[4];		/* the signature: the
						|| DWORD 0x304D434E "NCM0" or
						|| DWORD 0x314D434E "NCM1" in
						|| little-endian format.
						*/
	unsigned char	wLength[2];		/* size of this header
						|| block -- at least
						|| USB_Comm_NCM_NDP16__SIZE(1)
						*/
	unsigned char	wNextNdpIndex[2];	/* index to next NDP-table */
	unsigned char	DatagramPointerArray[8];/* first two datagram pointers */
	};

/* becuase of alignment issues, we define this separately */
struct USB_Comm_NCM_NDP16_ENTRY
	{
	unsigned char	wDatagramIndex[2];	/* index of datagram in block */
	unsigned char	wDatagramLength[2];	/* length of datagram in bytes */
	};

#define USB_Comm_NCM_NDP16_HEADER__SIZE		16u
#define USB_Comm_NCM_NDP16_SIGNATURE_NO_CRC32	0x304D434Eul
#define USB_Comm_NCM_NDP16_SIGNATURE_CRC32	0x314D434Eul
#define USB_Comm_NCM_NDP16_ENTRY__SIZE		4u

/* how big must a NDP16 be to hold n frames? */
#define USB_Comm_NCM_NDP16__SIZE(a_n)					\
	(USB_Comm_NCM_NDP16_HEADER__SIZE +				\
		((a_n)-2) * USB_Comm_NCM_NDP16_ENTRY__SIZE)

/* 
|| Validate that the NDP-index is aligned, that the NDP16 is not larger
|| than the NCM block, and that the NDP16 doesn't extend beyond the 
|| end of the NCM block
*/
#define	USB_Comm_NCM_NDP16_IS_VALID_SIZE(				\
	a_wNthBlockLength, a_wNdpIndex, a_wNdpLength)			\
	((((a_wNdpLength) & 3) == 0) &&					\
	  ((a_wNdpLength) < (a_wNthBlockLength)) &&			\
	  ((a_wNdpIndex) <= ((a_wNthBlockLength) - (a_wNdpLength))))

/* Convert a pointer to a NTH16 to a pointer to a NDP16 header */
#define	USB_Comm_NCM_NTH16_TO_NDP16_HEADER(a_pNth, a_wNdpIndex)		\
	((struct USB_Comm_NCM_NDP16_HEADER *)				\
		(((unsigned char *)(a_pNth)->dwSignature) + (a_wNdpIndex)))

/* Convert a pointer to a const NTH16 to a pointer to a const NDP16 header */
#define	USB_Comm_NCM_CONST_NTH16_TO_NDP16_HEADER(a_pNth, a_wNdpIndex)	\
	((const struct USB_Comm_NCM_NDP16_HEADER *)			\
		(((const unsigned char *)(a_pNth)->dwSignature) + (a_wNdpIndex)))

/* Convert a pointer to a NDP16 header and an index to a pointer to a NDP16 entry */
#define	USB_Comm_NCM_NDP16_INDEX_DATAGRAM_ENTRY(a_pNdp, a_iEntry)	\
	((struct USB_Comm_NCM_NDP16_ENTRY *)				\
		(((unsigned char *)(a_pNdp)->DatagramPointerArray) +	\
		 (a_iEntry) * USB_Comm_NCM_NDP16_ENTRY__SIZE))

/* Same as above but for use with const pointers */
#define	USB_Comm_NCM_CONST_NDP16_INDEX_DATAGRAM_ENTRY(a_pNdp, a_iEntry)	\
	((const struct USB_Comm_NCM_NDP16_ENTRY *)			\
		(((const unsigned char *)(a_pNdp)->DatagramPointerArray) + \
		 (a_iEntry) * USB_Comm_NCM_NDP16_ENTRY__SIZE))

/* Convert a pointer to a NDP16 header to a pointer to a NDP16 entry */
#define	USB_Comm_NCM_NDP16_FIRST_DATAGRAM_ENTRY(a_pNdp)			\
	((struct USB_Comm_NCM_NDP16_ENTRY *)(a_pNdp)->DatagramPointerArray)

/* Same as above but for use with const pointers */
#define	USB_Comm_NCM_CONST_NDP16_FIRST_DATAGRAM_ENTRY(a_pNdp)		\
	((const struct USB_Comm_NCM_NDP16_ENTRY *)(a_pNdp)->DatagramPointerArray)

/* given the length, determine how many entries are present, excluding terminator */
#define USB_Comm_NCM_NDP16_LENGTH_TO_NUM_ENTRIES(a_wNdpLength)		\
	(1 + ((a_wNdpLength) - USB_Comm_NCM_NDP16_HEADER__SIZE) /	\
		USB_Comm_NCM_NDP16_ENTRY__SIZE)

/*
|| always just compare last 4 bytes of NDP16 to zero, to validate - after
|| validating size:  for example:
||
||  if ((UHIL_LE_GETUINT32(pFp->dwSignature) != USB_Comm_NCM_NDP16_SIGNATURE_NO_CRC32) &&
||      (UHIL_LE_GETUINT32(pFp->dwSignature) != USB_Comm_NCM_NDP16_SIGNATURE_CRC32))
||	fail;
||
||   wNdpLength = UHIL_LE_GETUINT16(pFp->wLength);
||   if (! USB_Comm_NCM_NDP16_IS_VALID_SIZE(wNthBlockLength, wNdpIndex, wNdpLength)
||	fail;
||
||   if (UHIL_LE_GETUINT32((CONST UINT8 *)pNdp + wNdpLength - 4) != 0)
||	fail;
||
||   ... start processing
||
|| Some people won't have UHIL_LE_GETUINT32, but here's a macro to get
|| a pointer to the last frame entry....
*/

#define	USB_Comm_NCM_NDP16_LAST_DATAGRAM_ENTRY(a_pNdp, a_wFpLength)	\
	USB_Comm_NCM_NDP16_INDEX_DATAGRAM_ENTRY(a_pNdp,			\
		USB_Comm_NCM_NDP16_LENGTH_TO_NUM_ENTRIES(a_wFpLength)	\
		)

#define	USB_Comm_NCM_CONST_NDP16_LAST_DATAGRAM__ENTRY(a_pcNdp, a_wNdpLength) \
	USB_Comm_NCM_CONST_NDP16_INDEX_DATAGRAM__ENTRY(a_pcNdp,		\
		USB_Comm_NCM_NDP16_LENGTH_TO_NUM_ENTRIES(a_wFpLength)	\
		)

/*
|| Macros for processing datagrams -- these don't check for overlap of the
|| NDP16 and the datagrams, but with the processing we have, the behavior 
|| should be deterministic.  IS_VALID_DATAGRAM() doesn't check that the  
|| datagram is a valid Ethernet frame, it just makes sure that all the data 
|| is in the buffer as received.
*/
#define	USB_Comm_NCM_NDP16_IS_VALID_DATAGRAM(				\
	a_wNthBlockLength, a_wNthHeaderLength,  			\
	a_wNdpeDatagramIndex, a_wNdpeDatagramLength			\
	)								\
	((a_wNdpeDatagramIndex) >= (a_wNthHeaderLength) &&		\
	 /* this compare makes sure frame can't overflow buffer */	\
	 (a_wNdpeDatagramLength) <= ((a_wNthBlockLength) - (a_wNthHeaderLength))  &&	\
	 (a_wNdpeDatagramIndex)  <= ((a_wNthBlockLength) - (a_wNdpeDatagramLength)))

/* Generate next datagram pointer from a datagram pointer, avoiding alignment issues */
/* we use wDatagramIndex explictly to check input type and to get to UINT8* */
#define USB_Comm_NCM_NDP16_NEXT_DATAGRAM_ENTRY(a_pNdpe)			\
	((struct USB_Comm_NCM_NDP16_ENTRY *)				\
		(&((a_pNdpe)->wDatagramIndex[0]) +			\
		USB_Comm_NCM_NDP16_ENTRY__SIZE))

/* Same for CONST input pointers */
#define	USB_Comm_NCM_CONST_FPT_NEXT_DATAGRAM_ENTRY(a_pcNdpe)		\
	((const struct USB_Comm_NCM_NDP16_ENTRY *)			\
		(&((a_pcNdpe)->wDatagramIndex[0]) +			\
		USB_Comm_NCM_NDP16_ENTRY__SIZE))

/****************************************************************************\
|
|	The layout of an NCM 32-bit Datagram Pointers (NDP32): section 3.3.1
|
\****************************************************************************/

/*

Type:	struct USB_Comm_NCM_NDP32_HEADER

Index:	Const:	USB_Comm_NCM_NDP32_HEADER__SIZE
	Const:	USB_Comm_NCM_NDP32_SIGNATURE_NO_CRC32
	Const:	USB_Comm_NCM_NDP32_SIGNATURE_CRC32
	Name:	USB_Comm_NCM_NDP32__SIZE()
	Name:	USB_Comm_NCM_NDP32_IS_VALID_SIZE()

Function:
	The structure that represents a wire-format 32-bit NCM datagram 
	pointer table.

Description:
	A |struct USB_Comm_NCM_NDP32_HEADER| block describes a set
	of datagrams within a NCM block.

Contents:
	unsigned char dwSignature[4];
		The signature of this object, useful for defending against
		buggy software on the other end of the link.  This
		must always be either |USB_Comm_NCM_NDP32_SIGNATURE_NO_CRC32| or
		|USB_Comm_NCM_NDP32_SIGNATURE_CRC32|.

	unsigned char wLength[2];
		The size of this datagram pointer table in bytes, from the
		signature to the last byte.

	unsigned char wReserved6[2];
		reserved for future use

	unsigned char dwNextNdpIndex[4];
		The index of the next datagram pointer block (or zero).

	unsigned char dwReserved12[4];
		reserved for future use

	unsigned char DatagramPointerArray[];
		The (variable-length) array of datagram pointer entries
		(each one of type |struct USB_Comm_NCM_NDP32_ENTRY|).
		This array always ends with an entry that is all zeroes.
		Therefore, there is always at least two entries in the
		array, one datagram element pointer and a terminator.

See also:
	|struct USB_Comm_NCM_NDP32_ENTRY|

Notes:
	All entries are expressed in little-endian form.

	The size of this structure is given by the value of the macro
	|USB_Comm_NCM_NDP32_ENTRY__SIZE|, which includes
	the guaranteed two entries (one or more datagram entry + one
	terminating NULL datagram entry) in the |DatagramPointerArray|.

	The macro |USB_Comm_NCM_NDP32__SIZE(n)| returns the size in
	bytes of a datagram pointer table including pointers to |n|
	datagram.  |n| should not include the required terminating
	all-zero entry.  So for example, |USB_Comm_NCM_NDP32__SIZE(3)|
	returns 24 (8 bytes of header, 12 bytes of datagram pointer
	entries, and 4 bytes for the guaranteed zero entry at
	the end.

	The macro |USB_Comm_NCM_NDP32_IS_VALID_SIZE(wBlockLen, wNdpIndex,
	wFpLength)| determines whether a datagram pointer table of size
	wFpLength can occur at offset wNdpIndex of a block of size wBlockLen,
	returning TRUE if so, FALSE otherwise.

	The macro |USB_Comm_NCM_NDP32_LENGTH_TO_NUM_ENTRIES(wLength)|
	computes how many datagram pointer entries are in the specified
	datagram pointer table.  For symmetry with |USB_Comm_NCM_NDP32__SIZE()|
	The result does not include the terminating zero entry.
	If |USB_Comm_NCM_NDP32_IS_VALID_SIZE()| is not TRUE for
	this datagram pointer table, then the result of this macro is
	not specified.

*/

struct USB_Comm_NCM_NDP32_HEADER
	{
	unsigned char	dwSignature[4];		/* the signature: the
						|| DWORD 0x306D636E "ncm0" or
						|| DWORD 0x316D636E "ncm1" in
						|| little-endian format.
						*/
	unsigned char	wLength[2];		/* size of this header
						|| block -- at least
						|| USB_Comm_NCM_NDP32__SIZE(1)
						*/
	unsigned char	wReserved6[2];		/* reserved for future use */
	unsigned char	dwNextNdpIndex[4];	/* index to next NDP-table */
	unsigned char	dwReserved12[4];	/* reserved for future use */
	unsigned char	DatagramPointerArray[16];/* first two datagram pointers */
	};

/* Becuase of alignment issues, we define this separately */
struct USB_Comm_NCM_NDP32_ENTRY
	{
	unsigned char	dwDatagramIndex[4];	/* index of datagram in block */
	unsigned char	dwDatagramLength[4];	/* length of datagram in bytes */
	};

#define USB_Comm_NCM_NDP32_HEADER__SIZE		32u
#define USB_Comm_NCM_NDP32_SIGNATURE_NO_CRC32	0x306D636Eul
#define USB_Comm_NCM_NDP32_SIGNATURE_CRC32	0x316D636Eul
#define USB_Comm_NCM_NDP32_ENTRY__SIZE		8u

/* How big must a NDP32 be to hold n frames? */
#define USB_Comm_NCM_NDP32__SIZE(a_n)				\
	(USB_Comm_NCM_NDP32_HEADER__SIZE +			\
		((a_n)-2) * USB_Comm_NCM_NDP32_ENTRY__SIZE)

/* 
|| Validate that the NDP-index is aligned, that the NDP32 is not larger
|| than the NCM block, and that the NDP32 doesn't extend beyond the 
|| end of the NCM block
*/
#define	USB_Comm_NCM_NDP32_IS_VALID_SIZE(			\
	a_dwNthBlockLength, a_dwNdpIndex, a_wNdpLength)		\
	((((a_wNdpLength) & 3) == 0) &&				\
	  ((a_wNdpLength) < (a_dwNthBlockLength)) &&		\
	  ((a_dwNdpIndex) <= ((a_dwNthBlockLength) - (a_wNdpLength))))

/* Convert a pointer to a NTH32 to a pointer to a NDP32 header */
#define	USB_Comm_NCM_NTH32_TO_NDP32_HEADER(a_pNth, a_dwNdpIndex)	\
	((struct USB_Comm_NCM_NDP32_HEADER *)				\
		(((unsigned char *)(a_pNth)->dwSignature) + (a_dwNdpIndex)))

/* Convert a pointer to a const NTH32 to a pointer to a const NDP32 header */
#define	USB_Comm_NCM_CONST_NTH32_TO_NDP32_HEADER(a_pNth, a_dwNdpIndex)	\
	((const struct USB_Comm_NCM_NDP32_HEADER *)			\
		(((const unsigned char *)(a_pNth)->dwSignature) + (a_dwNdpIndex)))

/* Convert a pointer to a NDP32 header and an index to a pointer to a NDP32 entry */
#define	USB_Comm_NCM_NDP32_INDEX_DATAGRAM_ENTRY(a_pNdp, a_iEntry)	\
	((struct USB_Comm_NCM_NDP32_ENTRY *)				\
		(((unsigned char *)(a_pNdp)->DatagramPointerArray) +	\
		 (a_iEntry) * USB_Comm_NCM_NDP32_ENTRY__SIZE))

/* Same as above but for use with const pointers */
#define	USB_Comm_NCM_CONST_NDP32_INDEX_DATAGRAM_ENTRY(a_pNdp, a_iEntry)	\
	((const struct USB_Comm_NCM_NDP32_ENTRY *)			\
		(((const unsigned char *)(a_pNdp)->DatagramPointerArray) + \
		 (a_iEntry) * USB_Comm_NCM_NDP32_ENTRY__SIZE))

/* Convert a pointer to a NDP32 header to a pointer to a NDP32 entry */
#define	USB_Comm_NCM_NDP32_FIRST_DATAGRAM_ENTRY(a_pNdp)			\
	((struct USB_Comm_NCM_NDP32_ENTRY *)(a_pNdp)->DatagramPointerArray)

/* Same as above but for use with const pointers */
#define	USB_Comm_NCM_CONST_NDP32_FIRST_DATAGRAM_ENTRY(a_pNdp)		\
	((const struct USB_Comm_NCM_NDP32_ENTRY *)(a_pNdp)->DatagramPointerArray)

/* Given the length, determine how many entries are present, excluding terminator */
#define USB_Comm_NCM_NDP32_LENGTH_TO_NUM_ENTRIES(a_wNdpLength)		\
	(1 + ((a_wNdpLength) - USB_Comm_NCM_NDP32_HEADER__SIZE) /	\
		USB_Comm_NCM_NDP32_ENTRY__SIZE)

/*
|| always just compare last 4 bytes of NDP32 to zero, to validate - after
|| validating size:  for example:
||
||  if ((UHIL_LE_GETUINT32(pNdp->dwSignature) != USB_Comm_NCM_NDP32_SIGNATURE_NO_CRC32) &&
||      (UHIL_LE_GETUINT32(pNdp->dwSignature) != USB_Comm_NCM_NDP32_SIGNATURE_CRC32))
||	fail;
||
||   wNdpLength = UHIL_LE_GETUINT16(pNdp->wLength);
||   if (! USB_Comm_NCM_NDP32_IS_VALID_SIZE(dwNthBlockLength, wNdpIndex, wNdpLength)
||	fail;
||
||   if (UHIL_LE_GETUINT32((CONST UINT8 *)pNdp + wNdpLength - 4) != 0)
||	fail;
||
||   ... start processing
||
|| Some people won't have UHIL_LE_GETUINT32, but here's a macro to get
|| a pointer to the last frame entry....
*/

#define	USB_Comm_NCM_NDP32_LAST_DATAGRAM_ENTRY(a_pNdp, a_wNdpLength)	\
	USB_Comm_NCM_NDP32_INDEX_DATAGRAM_ENTRY(a_pNdp,			\
		USB_Comm_NCM_NDP32_LENGTH_TO_NUM_ENTRIES(a_wNdpLength)	\
		)

#define	USB_Comm_NCM_CONST_NDP32_LAST_DATAGRAM__ENTRY(a_pcNdp, a_wNdpLength) \
	USB_Comm_NCM_CONST_NDP32_INDEX_DATAGRAM__ENTRY(a_pcNdp,		\
		USB_Comm_NCM_NDP32_LENGTH_TO_NUM_ENTRIES(a_wNdpLength)	\
		)

/*
|| Macros for processing datagrams -- these don't check for overlap of the
|| NDP32 and the datagrams, but with the processing we have, the behavior 
|| should be deterministic.  IS_VALID_DATAGRAM() doesn't check that the  
|| datagram is a valid Ethernet frame, it just makes sure that all the data 
|| is in the buffer as received.
*/
#define	USB_Comm_NCM_NDP32_IS_VALID_DATAGRAM(				\
	a_dwNthBlockLength, a_wNthHeaderLength,  			\
	a_dwNdpeDatagramIndex, a_dwNdpeDatagramLength			\
	)								\
	((a_dwNdpeDatagramIndex) >= (a_wNthHeaderLength) &&		\
	 /* this compare makes sure frame can't overflow buffer */	\
	 (a_dwNdpeDatagramLength) <= ((a_dwNthBlockLength) - (a_wNthHeaderLength))  &&	\
	 (a_dwNdpeDatagramIndex)  <= ((a_dwNthBlockLength) - (a_dwNdpeDatagramLength)))

/* Generate next datagram pointer from a datagram pointer, avoiding alignment issues */
/* we use dwDatagramIndex explictly to check input type and to get to UINT8* */
#define USB_Comm_NCM_NDP32_NEXT_DATAGRAM_ENTRY(a_pNdpe)			\
	((struct USB_Comm_NCM_NDP32_ENTRY *)				\
		(&((a_pNdpe)->dwDatagramIndex[0]) +			\
		USB_Comm_NCM_NDP32_ENTRY__SIZE))

/* Same for CONST input pointers */
#define	USB_Comm_NCM_CONST_NDP32_NEXT_DATAGRAM_ENTRY(a_pcNdpe)		\
	((const struct USB_Comm_NCM_NDP32_ENTRY *)			\
		(&((a_pcNdpe)->dwDatagramIndex[0]) +			\
		USB_Comm_NCM_NDP32_ENTRY__SIZE))

/**** end of usbncm10.h ****/
#endif /* _USBNCM10_H_ */
