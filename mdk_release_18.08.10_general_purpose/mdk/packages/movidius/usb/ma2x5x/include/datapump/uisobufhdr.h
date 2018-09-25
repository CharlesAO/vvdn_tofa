/* uisobufhdr.h	Sun May 31 2015 22:10:23 tmm */

/*

Module:  uisobufhdr.h

Function:
	Home for UISOBUFHDR

Version:
	V3.15b	Sun May 31 2015 22:10:23 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 2002-2003, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	May 2002

Revision history:
   1.79a  Mon May 27 2002 22:44:39  tmm
	Module created.

   1.79a  7/12/2002  tmm
	Merge in changes from 1.63j (Diab/msc cleanup).

   1.85a  Mon Jul 21 2003 09:34:35  chwon
	Correct UISOBUFPACKET_ROUNDSIZE().

   3.15b  Sun May 31 2015 22:10:23  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _UISOBUFHDR_H_		/* prevent multiple includes */
#define _UISOBUFHDR_H_

#ifndef _USBPUMPENV_H_
#include "usbpumpenv.h"
#endif


/*
|| When the DataPump receives isochronous data, it builds an interleaved
|| header/data stream in the input buffer.  When it transmits isochronous
|| data, it expects to find a similar structure.  The header contains
|| two fields.  ishodr_size is the size of the header+size of the data +
|| any padding required to get to the next header.  isohdr_ars contains
|| the count of the number of bytes actually in the data stream; this will
|| always be in [0..wMaxPacketSize].
*/
__TMS_TYPE_DEF(UISOBUFHDR, struct TTUSB_ISOBUFHDR);

struct TTUSB_ISOBUFHDR
	{
	/*
	|| We start with a word that describes the maximum size of
	|| each frame.  For RX, this is automatically filled in
	|| by the DataPump; for TX, this should be filled in by
	|| the client.  This size should include any required "round-up"
	|| so that the next bufhdr is naturally aligned.
	*/
	__TMS_UINT16	isohdr_size;

	/*
	|| We then have the actual record size ("ars") for the data
	|| in the buffer.  This must be less than or equal to the
	|| size.
	*/
	__TMS_UINT16	isohdr_ars;
	};

/*
|| For convenience when actually building or processing an ISO
|| packet, here's a structure containing the header and the data.
*/
__TMS_TYPE_DEF(UISOBUFPACKET, struct TTUSB_ISOBUFPACKET);

struct TTUSB_ISOBUFPACKET
	{
	__TMS_UISOBUFHDR isobuf_hdr;
	__TMS_UCHAR	isobuf_data[1];
	};

/*
|| Given the size of an isochronous data packet, figure out how
|| many bytes of store are needed to hold the packet, a header,
|| and any alignment bytes at the end.  The argument should nominally
|| be the max packet size, not the current packet size.
||
|| The complicated expression normally simplifies to the first clause,
|| because UISOBUFHDR is normally 4 bytes (a power of two) long.  If
|| it's not, then the second version will be selected at compile time.
*/
#define UISOBUFPACKET_ROUNDSIZE(pktsize) \
    (__TMS_OFFSET(__TMS_UISOBUFPACKET, isobuf_data[0]) + \
	(((sizeof(__TMS_UISOBUFHDR) & (1+~sizeof(__TMS_UISOBUFHDR))) == sizeof(__TMS_UISOBUFHDR)) ? \
		(((pktsize) + sizeof(__TMS_UISOBUFHDR) - 1) & (1+~sizeof(__TMS_UISOBUFHDR))) : \
		(((pktsize) + sizeof(__TMS_UISOBUFHDR) - 1) / sizeof(__TMS_UISOBUFHDR) * sizeof(__TMS_UISOBUFHDR))))

/**** end of uisobufhdr.h ****/
#endif /* _UISOBUFHDR_H_ */
