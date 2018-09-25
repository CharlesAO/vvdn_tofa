/* usbioctl_classes.h	Mon Jun  1 2015 00:19:41 tmm */

/*

Module:  usbioctl_classes.h

Function:
	Central definitions for USB DataPump IOCTL classes.

Version:
	V3.15b	Mon Jun  1 2015 00:19:41 tmm	Edit level 13

Copyright notice:
	This file copyright (C) 2002-2005, 2008, 2012, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Terry Moore, MCCI Corporation	August 2002

Revision history:
   1.79b  Fri Aug  9 2002 05:48:11  tmm
	Module created.

   1.79b  12/3/2002  chwon
	Add special class.

   1.89a  Wed Jun 09 2004 08:27:40 maw
	Add download class.

   1.89b  Sun Aug 08 2004 11:35:10  chwon
	Add device class.

   1.89d  Tue Nov 02 2004 14:41:40 maw
   	Added class type for Still Image Capture

   1.91d  2005-08-15  tmm
	Add USBPUMP_IOCTL_CLASS_NETWORK for network-related IOCTLs

   1.91e  Wed Jun 29 2005 09:43:00 maw
   	Added task class

   1.91h  Mon Jul 25 2005 16:39:52  tmm
	Add class types for HCD, OTG, PHY, and USBDI.  Sort the list,
	and regenerate the cloaked names.

   1.91h  Wed Aug 17 2005 19:02:35  tmm
   	(Unlogged): Dan added USBPUMP_IOCTL_CLASS_HID.  There was a cut
   	and paste error that resulted in two uncloakings.

   	In this revision, we put the IOCTL class codes back into order,
   	and updated commentary to list HID's "v" as taken.

   1.91h  2005-08-22  tmm
	Rename USBPUMP_IOCTL_CLASS_PHY to USBPUMP_IOCTL_CLASS_USBPHY to
	avoid confusion with net phys.

   1.91i  2005-09-24  drep
	Removed USBPUMP_IOCTL_CLASS_HID and created correctly named
	USBPUMP_IOCTL_CLASS_HOST_HID and USBPUMP_IOCTL_CLASS_HOST_STORAGE
	ioctl class codes.

   1.91i  Mon Sep 26 2005 13:22:12  chwon
	Correct type cloaking

   1.93a  Mon Oct 10 2005 10:59:55  chwon
	Merge BRANCH-USBKERN-1_91d-ELBERTA

   1.97k  Tue Dec 09 2008 14:28:48  chwon
	Add comment for new class code

   3.01f  Tue Apr 10 2012 11:10:56  chwon
	14889: Added comment for proxy class code

   3.15b  Mon Jun  1 2015 00:19:41  tmm
	19211: correct usages of "DataPump".

*/

#ifndef _USBIOCTL_CLASSES_H_		/* prevent multiple includes */
#define _USBIOCTL_CLASSES_H_

#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

/****************************************************************************\
|
|	These are the core types.
|
\****************************************************************************/

/*
|| By convention, we define CLASS_.. and CLASS_.._EDGE
|| to be lower and upper-case versions of the same letter,
|| if possible.  However, CLASS_CORE and CLASS_CORE_EDGE,
|| don't follow that convention, for whatever reason.
||
|| The table below is sorted by class-name, not by letter.
||
|| UPDATE THIS LIST of known codes when you add classes
||	aA bB cC dD E F gG hH iI K lL mM nN oO pP R sS tT uU w xX zZ
*/

/*	__TMS_USBPUMP_IOCTL_CLASS_ABSTRACT_NIC		'a'	*/
/*	__TMS_USBPUMP_IOCTL_CLASS_ABSTRACT_NIC_EDGE	'A'	*/
/*	__TMS_USBPUMP_IOCTL_CLASS_ABSTRACT_SIO		'g'	*/
/*	__TMS_USBPUMP_IOCTL_CLASS_ABSTRACT_SIO_EDGE	'G'	*/
/*	__TMS_USBPUMP_IOCTL_CLASS_API			'u'	*/
/*	__TMS_USBPUMP_IOCTL_CLASS_CMDPARSER		'c'	*/
/*	__TMS_USBPUMP_IOCTL_CLASS_CMDPARSER_EDGE	'C'	*/
#define	__TMS_USBPUMP_IOCTL_CLASS_CORE		'U'
#define	__TMS_USBPUMP_IOCTL_CLASS_CORE_EDGE	'E'
#define	__TMS_USBPUMP_IOCTL_CLASS_DEBUG		'D'
#define	__TMS_USBPUMP_IOCTL_CLASS_DEVICE	'd'
#define	__TMS_USBPUMP_IOCTL_CLASS_DOWNLOAD	'l'
#define	__TMS_USBPUMP_IOCTL_CLASS_DOWNLOAD_EDGE	'L'
#define	__TMS_USBPUMP_IOCTL_CLASS_FUNCTION	'F'
#define	__TMS_USBPUMP_IOCTL_CLASS_HCD		'h'
#define	__TMS_USBPUMP_IOCTL_CLASS_HCD_EDGE	'H'
#define	__TMS_USBPUMP_IOCTL_CLASS_HOST_HID	'K'	/* HID host class driver */
#define	__TMS_USBPUMP_IOCTL_CLASS_HOST_STORAGE	'R'	/* MSC host class driver */
#define	__TMS_USBPUMP_IOCTL_CLASS_MODEM		'm'
#define	__TMS_USBPUMP_IOCTL_CLASS_MODEM_EDGE	'M'
#define	__TMS_USBPUMP_IOCTL_CLASS_NETWORK	'n'
#define	__TMS_USBPUMP_IOCTL_CLASS_NETWORK_EDGE	'N'
#define	__TMS_USBPUMP_IOCTL_CLASS_OTG		'o'
#define	__TMS_USBPUMP_IOCTL_CLASS_OTG_EDGE	'O'	/* upper-case 'o' */
#define	__TMS_USBPUMP_IOCTL_CLASS_SIC		'i'
#define	__TMS_USBPUMP_IOCTL_CLASS_SIC_EDGE	'I'
#define	__TMS_USBPUMP_IOCTL_CLASS_SPECIAL	'z'
#define	__TMS_USBPUMP_IOCTL_CLASS_SPECIAL_EDGE	'Z'
#define	__TMS_USBPUMP_IOCTL_CLASS_STORAGE	's'
#define	__TMS_USBPUMP_IOCTL_CLASS_STORAGE_EDGE	'S'
#define	__TMS_USBPUMP_IOCTL_CLASS_TASK		't'
#define	__TMS_USBPUMP_IOCTL_CLASS_TASK_EDGE	'T'
#define	__TMS_USBPUMP_IOCTL_CLASS_USBDI		'b'
#define	__TMS_USBPUMP_IOCTL_CLASS_USBDI_EDGE	'B'
#define	__TMS_USBPUMP_IOCTL_CLASS_USBPHY	'p'
#define	__TMS_USBPUMP_IOCTL_CLASS_USBPHY_EDGE	'P'
/*	__TMS_USBPUMP_IOCTL_CLASS_WUSB		'w'		*/
/*	__TMS_USBPUMP_IOCTL_CLASS_PROXY		'x'		*/
/*	__TMS_USBPUMP_IOCTL_CLASS_PROXY_EDGE	'X'		*/


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_CLASS_CORE	\
   __TMS_USBPUMP_IOCTL_CLASS_CORE
# define USBPUMP_IOCTL_CLASS_CORE_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_CORE_EDGE
# define USBPUMP_IOCTL_CLASS_DEBUG	\
   __TMS_USBPUMP_IOCTL_CLASS_DEBUG
# define USBPUMP_IOCTL_CLASS_DEVICE	\
   __TMS_USBPUMP_IOCTL_CLASS_DEVICE
# define USBPUMP_IOCTL_CLASS_DOWNLOAD	\
   __TMS_USBPUMP_IOCTL_CLASS_DOWNLOAD
# define USBPUMP_IOCTL_CLASS_DOWNLOAD_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_DOWNLOAD_EDGE
# define USBPUMP_IOCTL_CLASS_FUNCTION	\
   __TMS_USBPUMP_IOCTL_CLASS_FUNCTION
# define USBPUMP_IOCTL_CLASS_HCD	\
   __TMS_USBPUMP_IOCTL_CLASS_HCD
# define USBPUMP_IOCTL_CLASS_HCD_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_HCD_EDGE
# define USBPUMP_IOCTL_CLASS_HOST_HID	\
   __TMS_USBPUMP_IOCTL_CLASS_HOST_HID
# define USBPUMP_IOCTL_CLASS_HOST_STORAGE	\
   __TMS_USBPUMP_IOCTL_CLASS_HOST_STORAGE
# define USBPUMP_IOCTL_CLASS_MODEM	\
   __TMS_USBPUMP_IOCTL_CLASS_MODEM
# define USBPUMP_IOCTL_CLASS_MODEM_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_MODEM_EDGE
# define USBPUMP_IOCTL_CLASS_NETWORK	\
   __TMS_USBPUMP_IOCTL_CLASS_NETWORK
# define USBPUMP_IOCTL_CLASS_NETWORK_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_NETWORK_EDGE
# define USBPUMP_IOCTL_CLASS_OTG	\
   __TMS_USBPUMP_IOCTL_CLASS_OTG
# define USBPUMP_IOCTL_CLASS_OTG_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_OTG_EDGE
# define USBPUMP_IOCTL_CLASS_SIC	\
   __TMS_USBPUMP_IOCTL_CLASS_SIC
# define USBPUMP_IOCTL_CLASS_SIC_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_SIC_EDGE
# define USBPUMP_IOCTL_CLASS_SPECIAL	\
   __TMS_USBPUMP_IOCTL_CLASS_SPECIAL
# define USBPUMP_IOCTL_CLASS_SPECIAL_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_SPECIAL_EDGE
# define USBPUMP_IOCTL_CLASS_STORAGE	\
   __TMS_USBPUMP_IOCTL_CLASS_STORAGE
# define USBPUMP_IOCTL_CLASS_STORAGE_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_STORAGE_EDGE
# define USBPUMP_IOCTL_CLASS_TASK	\
   __TMS_USBPUMP_IOCTL_CLASS_TASK
# define USBPUMP_IOCTL_CLASS_TASK_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_TASK_EDGE
# define USBPUMP_IOCTL_CLASS_USBDI	\
   __TMS_USBPUMP_IOCTL_CLASS_USBDI
# define USBPUMP_IOCTL_CLASS_USBDI_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_USBDI_EDGE
# define USBPUMP_IOCTL_CLASS_USBPHY	\
   __TMS_USBPUMP_IOCTL_CLASS_USBPHY
# define USBPUMP_IOCTL_CLASS_USBPHY_EDGE	\
   __TMS_USBPUMP_IOCTL_CLASS_USBPHY_EDGE
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbioctl_classes.h ****/
#endif /* _USBIOCTL_CLASSES_H_ */
