/* videoclientlib.h	Thu Jan 28 2016 15:14:35 chwon */

/*

Module:  videoclientlib.h

Function:
	APIs for video client library

Version:
	V3.17a	Thu Jan 28 2016 15:14:35 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2007, 2009, 2015-2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaoTsung Perng, MCCI Corporation	September 2007

Revision history:
   1.97j  Fri Sep 14 2007 09:59:12  ctpeng
	Module created.

   2.01a  Wed Dec 02 2009 10:45:09  skpark
	9357: Fixed doc-xml error in revision history

   3.15b  Thu Apr 02 2015 12:36:47  chwon
	19117: Add VideoClientLib_StreamReadIsoch() and
	VideoClientLib_StreamWriteIsoch() prototype.

   3.17a  Thu Jan 28 2016 15:14:35  chwon
	19642: Add read and write V2 function prototypes.

*/


#ifndef _VIDEO_CLIENT_LIB_H_		/* prevent multiple includes */
#define _VIDEO_CLIENT_LIB_H_

#ifndef _PROTOVIDEO_H_
# include "protovideo.h"
#endif

/****************************************************************************\
|
|	Video Protocol Client API Functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_UPROTO_VIDEO_CONTROL_OPEN_FN	VideoClientLib_ControlOpen;
__TMS_UPROTO_VIDEO_CONTROL_CLOSE_FN	VideoClientLib_ControlClose;
__TMS_UPROTO_VIDEO_CONTROL_REQUEST_REPLY_FN
					VideoClientLib_ControlRequestReply;

__TMS_UPROTO_VIDEO_STREAM_OPEN_FN	VideoClientLib_StreamOpen;
__TMS_UPROTO_VIDEO_STREAM_CLOSE_FN	VideoClientLib_StreamClose;
__TMS_UPROTO_VIDEO_STREAM_GET_INFO_FN	VideoClientLib_StreamGetInfo;
__TMS_UPROTO_VIDEO_STREAM_READ_FN	VideoClientLib_StreamRead;
__TMS_UPROTO_VIDEO_STREAM_READ_V2_FN	VideoClientLib_StreamReadV2;
__TMS_UPROTO_VIDEO_STREAM_WRITE_FN	VideoClientLib_StreamWrite;
__TMS_UPROTO_VIDEO_STREAM_WRITE_V2_FN	VideoClientLib_StreamWriteV2;
__TMS_UPROTO_VIDEO_STREAM_REQUEST_REPLY_FN
					VideoClientLib_StreamRequestReply;
__TMS_UPROTO_VIDEO_STREAM_READ_ISOCH_FN	VideoClientLib_StreamReadIsoch;
__TMS_UPROTO_VIDEO_STREAM_READ_ISOCH_V2_FN VideoClientLib_StreamReadIsochV2;
__TMS_UPROTO_VIDEO_STREAM_WRITE_ISOCH_FN VideoClientLib_StreamWriteIsoch;
__TMS_UPROTO_VIDEO_STREAM_WRITE_ISOCH_V2_FN VideoClientLib_StreamWriteIsochV2;

__TMS_END_DECLS

/**** end of videoclientlib.h ****/
#endif /* _VIDEO_CLIENT_LIB_H_ */
