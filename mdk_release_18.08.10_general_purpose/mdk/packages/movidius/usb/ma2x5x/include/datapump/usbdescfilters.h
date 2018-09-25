/* usbdescfilters.h	Tue Jul 23 2013 13:23:43 chwon */

/*

Module:  usbdescfilters.h

Function:
	Descriptor filtering functions from core DataPump

Version:
	V3.11d	Tue Jul 23 2013 13:23:43 chwon	Edit level 7

Copyright notice:
	This file copyright (C) 2002, 2004, 2008-2009, 2012-2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 2002

Revision history:
   1.79a  Sat Jun 22 2002 12:41:54  tmm
	Module created.

   1.89e  Mon Dec 06 2004 09:39:54  chwon
	Add more filter functions prototype.

   1.97k  Wed Sep 24 2008 11:13:20  cccho
	6683: add UsbPumpFilter_GetBinaryObjectStoreDescriptors filter
	function prototype.

   2.01a  Mon Jun 15 2009 12:37:53  chwon
	8251: Added UsbPumpFilter_GetMacAddressDescriptor() prototype

   2.01a  Tue Oct 06 2009 08:53:18  chwon
	9034: Added USBPUMP_GETDESCRFILTER_FAIL filter status code and generate
	uncloaked names using uncloak-defs.sh.

   3.01f  Sun Jan 08 2012 13:54:28  ssyang
	13926: Added UsbPumpFilter_CdcNetworkFunctionalDescriptors() prototype

   3.11d  Tue Jul 23 2013 13:23:43  chwon
	17377: include usbpump_filter_types.h and define filter function
	prototype using USBPUMP_FILTER_GETDESCRIPTOR_FN type.

*/

#ifndef _USBDESCFILTERS_H_		/* prevent multiple includes */
#define _USBDESCFILTERS_H_

#ifndef _USBPUMP_FILTER_TYPES_H_
# include "usbpump_filter_types.h"
#endif

/****************************************************************************\
|
|	Function prototypes
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_FILTER_GETDESCRIPTOR_FN
UsbPumpFilter_GetSerialNumberDescriptor;

__TMS_USBPUMP_FILTER_GETDESCRIPTOR_FN
UsbPumpFilter_GetDescriptor;

__TMS_USBPUMP_FILTER_GETDESCRIPTOR_FN
UsbPumpFilter_MultiConfigModalDescriptors;

__TMS_USBPUMP_FILTER_GETDESCRIPTOR_FN
UsbPumpFilter_GetMacAddressDescriptor;

__TMS_USBPUMP_FILTER_GETDESCRIPTOR_FN
UsbPumpFilter_GetBinaryObjectStoreDescriptors;

__TMS_USBPUMP_FILTER_GETDESCRIPTOR_FN
UsbPumpFilter_CdcNetworkFunctionalDescriptors;

__TMS_END_DECLS

/**** end of usbdescfilters.h ****/
#endif /* _USBDESCFILTERS_H_ */
