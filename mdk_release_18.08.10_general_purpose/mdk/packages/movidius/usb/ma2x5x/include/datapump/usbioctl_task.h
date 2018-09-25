/* usbioctl_task.h	Thu Jul 21 2005 19:35:42 tmm */

/*

Module:  usbioctl_task.h

Function:
	IOCTL codes for tasks

Version:
	V1.91g	Thu Jul 21 2005 19:35:42 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2005 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Mats Webjörn, MCCI Corporation	June 2005

Revision history:
   1.91e  Wed Jun 29 2005 08:48:46 maw
	Module created.

   1.91g  2005-07-21  tmm
	No need for input argument any more on the GET_MESSAGE_BASE
	IOCTL

*/

#ifndef _USBIOCTL_TASK_H_		/* prevent multiple includes */
#define _USBIOCTL_TASK_H_

#ifndef _USBPUMPIOCTL_H_
# include "usbpumpioctl.h"
#endif

#ifndef _USBIOCTL_CLASSES_H_
# include "usbioctl_classes.h"
#endif

#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif

/****************************************************************************\
|
|	Define the IOCTL codes
|
\****************************************************************************/

#define __TMS_USBPUMP_IOCTL_TASK(i, t, NAME)				\
	__TMS_CONCAT(__TMS_USBPUMP_IOCTL_, t)(				\
		__TMS_USBPUMP_IOCTL_CLASS_TASK,				\
		(i),							\
		__TMS_CONCAT3(__TMS_USBPUMP_IOCTL_TASK_, NAME, _ARG)	\
		)

/* get message base */
#define	__TMS_USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE	\
	__TMS_USBPUMP_IOCTL_TASK(0, R, GET_MESSAGE_BASE)

/* get DataPump task-handle */
#define __TMS_USBPUMP_IOCTL_TASK_GET_TASKINFO	\
	__TMS_USBPUMP_IOCTL_TASK(1, R, GET_TASKINFO)

/**** uncloaked names ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE		\
   __TMS_USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE
# define USBPUMP_IOCTL_TASK_GET_TASKINFO	\
   __TMS_USBPUMP_IOCTL_TASK_GET_TASKINFO
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/*

IOCTL:	USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE

Index:	Type:	USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE_ARG
	Name:	USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE_ARG_SETUP_V1

Function:
	IOCTL issued by Task implementation layer to establish the base
	to use when creating message-id's from a base and an offset

Input:
	pInParam	points to a USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE_ARG
			structure.

	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE_ARG structure.

	USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE_ARG has the following elements:

	ADDRBITS MessageBase;
		OUT: Base to add to message offset to get final message id

Description:	
	This IOCTL is sent from an Task-specific implementation to get
	a numeric value to add to an offset that will result in a 
	platform specific message id.

*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_TASK_GET_MESSAGE_BASE_ARG)
	{
	__TMS_ADDRBITS			MessageBase;
	};

/*

IOCTL:	USBPUMP_IOCTL_TASK_GET_TASKINFO

Index:	Type:	USBPUMP_IOCTL_TASK_GET_TASKINFO_ARG

Function:
	Return task and messaging information

Input:
	pInParam	points to a USBPUMP_IOCTL_TASK_GET_TASKINFO_ARG
			structure.
	pOutParam	points to a buffer that is set to the resulting
			USBPUMP_IOCTL_TASK_GET_TASKINFO_ARG structure.

	USBPUMP_IOCTL_TASK_GET_TASKINFO_ARG has the following elements:

	UTASK_PROCESS_HANDLE hProc;		-- OUT: set to DataPump process handle
	
	CONST UTASK_SWITCH * pTaskSwitch;	-- OUT: set to task switch

Description:
	This IOCTL, if directed to a UPLATFORM, UDEVICE or object layered
	upon the same, returns the parameters related to the DataPump task.  
	This is very useful when working at a high level in the object hierarchy.

Returns:
	USBPUMP_RESULT_SUCCESS:  Out parameters set

	Other failure codes are possible if the request is malformed.

*/

/* 
|| USBPUMP_IOCTL_TASK_GET_TASKINFO fills in a buffer with 
|| a pointer to a UTASK_PROCESS_HANDLE and a task switch.
*/

__TMS_TYPE_DEF_STRUCT(USBPUMP_IOCTL_TASK_GET_TASKINFO_ARG);
struct __TMS_STRUCTNAME(USBPUMP_IOCTL_TASK_GET_TASKINFO_ARG)
	{
	__TMS_UTASK_PROCESS_HANDLE 		hProc;		/* OUT: the DataPump process handle */
	__TMS_CONST __TMS_UTASK_SWITCH *	pTaskSwitch;	/* OUT: the task switch */
	};

/**** end of usbioctl_task.h ****/
#endif /* _USBIOCTL_TASK_H_ */
