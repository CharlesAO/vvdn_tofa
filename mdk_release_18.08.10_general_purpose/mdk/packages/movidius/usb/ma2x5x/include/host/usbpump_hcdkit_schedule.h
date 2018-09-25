/* usbpump_hcdkit_schedule.h	Mon Mar 30 2015 15:27:32 tmm */

/*

Module:  usbpump_hcdkit_schedule.h

Function:
	Definition of USBPUMP_HCDKIT_SCHEDULE

Version:
	V3.15b	Mon Mar 30 2015 15:27:32 tmm	Edit level 6

Copyright notice:
	This file copyright (C) 2006-2008, 2010, 2015 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	October 2006

Revision history:
   1.97f  Fri Oct 06 2006 15:15:22 chwon
	Module created.

   1.97i  Fri Jun 01 2007 13:22:45  chwon
	Add start / stop scheduler support

   1.97j  Fri Oct 19 2007 15:38:31  chwon
	3395: use bit2 of pHcdPipe->HcdReserved[3] to indicate USBPUMP_HCD_PIPE
	is in the scheduler queue.

   1.97k  Tue Apr 15 2008 11:44:16  chwon
	4019: add UsbPumpHcdKitI_ScheduleFindFirstNonPeriodicPipe() prototype.
	Add UsbPumpHcdKitI_ScheduleFindFirstPipe_V2() and
	UsbPumpHcdKitI_ScheduleFindNextPipe_V2() prototype.

   3.01c  Mon Jun 14 2010 11:52:41  chwon
	10557: add 64 bit compiler support -- ADDRBITS type will be 64 bits.

   3.15b  Mon Mar 30 2015 15:27:32  tmm
	19019: improve internal documentation.

*/

#ifndef _USBPUMP_HCDKIT_SCHEDULE_H_	/* prevent multiple includes */
#define _USBPUMP_HCDKIT_SCHEDULE_H_

#ifndef _USBPUMP_HCD_H_
# include "usbpump_hcd.h"
#endif

#ifndef _USBPUMP_HCD_PIPE_H_
# include "usbpump_hcd_pipe.h"
#endif

#ifndef _USBPUMP_HCD_REQUEST_H_
# include "usbpump_hcd_request.h"
#endif

/****************************************************************************\
|
|	Definition for USBPUMP_HCDKIT_SCHEDULE
|
\****************************************************************************/

/*

Type:	USBPUMP_HCDKIT_SCHEDULE

Index:	Type:	USBPUMP_HCDKIT_SCHEDULE_NODE

Function:
	Represent a USB bus schedule in a portable way.

Definition:
	#include "usbpump_hcdkit_schedule.h"

Contents:
	A USBPUMP_HCDKIT_SCHEDULE_NODE contains the following information:

	USBPUMP_HCD_PIPE *pHcdPipe;
		Pointer to the head of the list of HCD pipes associated with
		this point in the schedule, or NULL.

	USBPUMP_HCDKIT_SCHEDULE_NODE *pNext;
		Pointer to the next node in the schedule, or NULL. 

	A USBPUMP_HCDKIT_SCHEDULE contains the following information:

	USBPUMP_HCD *pHcd;
		Link to the associated HCD.

	UINT32 nLevel;
		the number of scheduling levels in the binary tree.

	UINT32 LeafNodeMask;
		2^(nLevel-1) - 1 -- a mask based on the number of nodes
		in the last tier of the tree.  Allows for simple counting
		modulo 2^(nLevel-1); just mask the frame number with
		LeafNodeMask.

	USBPUMP_HCD_PIPE *pCurrentPipe;
		A cursor variable, used for tracking progress through the
		schedule for certain find first/next APIs.

	UINT fCurrentPeriodicPipe: 1;
		TRUE if the current pipe is periodic.

	UINT fScheduleCurrentPipe: 1;
		TRUE if the next call to a find next API should return 
		current pipe rather than advancing.

	UINT fStopSchedule: 1;
		TRUE if the schedule is stopped.

	UINT32 nNonPeriodicRequest;
		A count of the number of non-periodic requests currently
		in the schedule. This can save scheduling time: if no
		non-periodic pipes, no need to walk the schedule.

	UINT32 nNonPeriodicPipe;
		A count of the number of non-periodic pipes currently
		in the schedule.

	USBPUMP_HCD_PIPE *pNonPeriodicPipeHead;
		Pointer to first pipe in the non-periodic list, or NULL.

	UINT32 nPeriodicRequest;
		A count of the number of periodic requests currently in
		the schedule.

	UINT32 nPeriodicPipe;
		A count of the number of periodic pipes currently in the
		schedule.

	USBPUMP_HCDKIT_SCHEDULE_NODE **pvTreeLeafNode;
		Slightly misnamed (should be pvpTreeLeafNode): pointer
		to an array of pointers to the leave nodes. There are
		exactly 2^(nLevel-1) such pointers.

	USBPUMP_HCDKIT_SCHEDULE_NODE *pTreeNodes;
		The array of tree nodes. There are exactly (2^nLevel)-1 
		such nodes.

Description:
	

*/

__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCDKIT_SCHEDULE_NODE);
struct __TMS_STRUCTNAME	(USBPUMP_HCDKIT_SCHEDULE_NODE)
	{
	__TMS_USBPUMP_HCD_PIPE *		pHcdPipe;
	__TMS_USBPUMP_HCDKIT_SCHEDULE_NODE *	pNext;
	};


__TMS_TYPE_DEF_STRUCT	(USBPUMP_HCDKIT_SCHEDULE);
struct __TMS_STRUCTNAME	(USBPUMP_HCDKIT_SCHEDULE)
	{
	__TMS_USBPUMP_HCD *			pHcd;
	__TMS_UINT32				nLevel;
	__TMS_UINT32				LeafNodeMask;

	/*
	|| Current pipe in schedule list.
	|| This is used to select next pipe to schedule.
	*/
	__TMS_USBPUMP_HCD_PIPE *		pCurrentPipe;
	__TMS_UINT				fCurrentPeriodicPipe : 1;
	__TMS_UINT				fScheduleCurrentPipe : 1;
	__TMS_UINT				fStopSchedule : 1;

	/* Non-periodic schedule list */
	__TMS_UINT32				nNonPeriodicRequest;
	__TMS_UINT32				nNonPeriodicPipe;
	__TMS_USBPUMP_HCD_PIPE *		pNonPeriodicPipeHead;

	/* Periodic schedule inverse tree */
	__TMS_UINT32				nPeriodicRequest;
	__TMS_UINT32				nPeriodicPipe;
	__TMS_USBPUMP_HCDKIT_SCHEDULE_NODE **	pvTreeLeafNode;
	__TMS_USBPUMP_HCDKIT_SCHEDULE_NODE *	pTreeNodes;
	};


/****************************************************************************\
|
|	Usage of HcdReserved[3] field of USBPUMP_HCD_PIPE object
|
\****************************************************************************/

/*

Name:	USBPUMP_HCDKIT_SCHEDULE_INIT_NODE()

Index:	Name:	USBPUMP_HCDKIT_SCHEDULE_DEINIT_NODE()
	Name:	USBPUMP_HCDKIT_SCHEDULE_GET_NODE()
	Name:	USBPUMP_HCDKIT_SCHEDULE_SET_NODE()
	Name:	USBPUMP_HCDKIT_SCHEDULE_VALIDATE_HCD_PIPE()
	Name:	USBPUMP_HCDKIT_SCHEDULE_GET_IN_SERVICE()
	Name:	USBPUMP_HCDKIT_SCHEDULE_SET_IN_SERVICE()
	Name:	USBPUMP_HCDKIT_SCHEDULE_CLR_IN_SERVICE()

Function:
	Access the HcdKit-Schedule related bits of HcdReserved[3] 
	entry of USBPUMP_HCD_PIPE object.

Definition:
	VOID USBPUMP_HCDKIT_SCHEDULE_INIT_NODE(
		USBPUMP_HCD_PIPE *pHcdPipe,
		USBPUMP_HCDKIT_SCHEDULE_NODE *pNode
		);

	VOID USBPUMP_HCDKIT_SCHEDULE_DEINIT_NODE(
		USBPUMP_HCD_PIPE *pHcdPipe
		);

	USBUMP_HCDKIT_SCHEDULE_NODE *
		USBPUMP_HCDKIT_SCHEDULE_GET_NODE(
			USBPUMP_HCD_PIPE *pHcdPipe
			);

	VOID USBPUMP_HCDKIT_SCHEDULE_SET_NODE(
		USPBUMP_HCD_PIPE *pHcdPipe, 
		USPBUMP_HCDKIT_SCHEDULE_NODE *pTreeNode
		);

	BOOL USBPUMP_HCDKIT_SCHEDULE_VALIDATE_HCD_PIPE(
		USPBUMP_HCD_PIPE *pHcdPipe
		);

	BOOL USBPUMP_HCDKIT_SCHEDULE_GET_IN_SERVICE(
		USPBUMP_HCD_PIPE *pHcdPipe
		);

	VOID USBPUMP_HCDKIT_SCHEDULE_SET_IN_SERVICE(
		USPBUMP_HCD_PIPE *pHcdPipe
		);

	VOID USBPUMP_HCDKIT_SCHEDULE_CLR_IN_SERVICE(
		USPBUMP_HCD_PIPE *pHcdPipe
		);

Description:
	The USBPUMP_HCDKIT_SCHEDULE implementation functions use
	USBPUMP_HCD_PIPE::HcdReserved[3] to store state relevant
	to a given pipe's position in the schedule. Several functions
	or macros provide access to the data stored in that slot.
	
	USBPUMP_HCDKIT_SCHEDULE_INIT_NODE() initializes the slot,
	and links the USBPUMP_HCD_PIPE to the specified 
	USBPUMP_HCDKIT_SCHEDULE_NODE. 
	
	USBPUMP_HCDKIT_SCHEDULE_DEINIT_NODE() reverses the process.

	USBPUMP_HCDKIT_SCHEDULE_GET_NODE() returns a pointer to 
	the USBUMP_HCDKIT_SCHEDULE_NODE linked to the 
	USBPUMP_HCD_PIPE.

	USBPUMP_HCDKIT_SCHEDULE_VALIDATE_PIPE() returns non-zero if
	the specified pipe is marked as being in a schedule, zero
	otherwise.

	The schedule maintains an in-service flag for each 
	USBPUMP_HCD_PIPE. It sets this flag when it starts 
	servicing a request for that pipe, and clears it
	when complete. While the flag is set, it won't 
	schedule the pipe again. 
	
	USBPUMP_HCDKIT_SET_IN_SERVICE() sets the flag, 
	USBPUMP_HCDKIT_CLR_IN_SERVICE() clears the flag, and
	USBPUMP_HCDKIT_GET_IN_SERVICE() returns non-zero if
	the flag is set, zero otherwise.

*/

/*
|| pHcdPipe->HcdReserved[0] : reserved for HCD
|| pHcdPipe->HcdReserved[1] : reserved for HCD
|| pHcdPipe->HcdReserved[2] : reserved for HCD
|| pHcdPipe->HcdReserved[3] : used by HCDKIT scheduler
||			Bit[31-2] = Tree node pointer if periodic pipe
||			Bit 1     = USBPUMP_HCD_PIPE is added in the scheduler
||			Bit 0     = HCD In Service Request
*/

#define	__TMS_USBPUMP_HCDKIT_SCHEDULE_IN_SERVICE ((__TMS_ADDRBITS)(1 << 0))
#define	__TMS_USBPUMP_HCDKIT_SCHEDULE_PIPE_ADDED ((__TMS_ADDRBITS)(1 << 1))
#define	__TMS_USBPUMP_HCDKIT_SCHEDULE_NODE_MASK  ((__TMS_ADDRBITS)(3 << 0))

#define __TMS_USBPUMP_HCDKIT_SCHEDULE_INIT_NODE(pHcdPipe, pNode)	\
    do	{								\
	(pHcdPipe)->HcdReserved[3].Ptr = pNode;				\
	(pHcdPipe)->HcdReserved[3].Addrbits |=				\
		__TMS_USBPUMP_HCDKIT_SCHEDULE_PIPE_ADDED;		\
	} while (0)

#define __TMS_USBPUMP_HCDKIT_SCHEDULE_DEINIT_NODE(pHcdPipe)		\
    do	{								\
	(pHcdPipe)->HcdReserved[3].Addrbits = 0;			\
	} while (0)

#define __TMS_USBPUMP_HCDKIT_SCHEDULE_GET_NODE(pHcdPipe)		\
	(__TMS_USBPUMP_HCDKIT_SCHEDULE_NODE *)				\
		((pHcdPipe)->HcdReserved[3].Addrbits &			\
		 ~__TMS_USBPUMP_HCDKIT_SCHEDULE_NODE_MASK)

#define __TMS_USBPUMP_HCDKIT_SCHEDULE_SET_NODE(pHcdPipe, pTreeNode)	\
    do	{								\
	(pHcdPipe)->HcdReserved[3].Addrbits &=				\
		__TMS_USBPUMP_HCDKIT_SCHEDULE_NODE_MASK;		\
	(pHcdPipe)->HcdReserved[3].Addrbits |= (__TMS_ADDRBITS)(pTreeNode);\
	} while (0)

#define __TMS_USBPUMP_HCDKIT_SCHEDULE_VALIDATE_HCD_PIPE(pHcdPipe)	\
	((pHcdPipe)->HcdReserved[3].Addrbits &				\
	 __TMS_USBPUMP_HCDKIT_SCHEDULE_PIPE_ADDED)

#define __TMS_USBPUMP_HCDKIT_SCHEDULE_GET_IN_SERVICE(pHcdPipe)		\
	((pHcdPipe)->HcdReserved[3].Addrbits &				\
	 __TMS_USBPUMP_HCDKIT_SCHEDULE_IN_SERVICE)

#define __TMS_USBPUMP_HCDKIT_SCHEDULE_SET_IN_SERVICE(pHcdPipe)		\
    do	{								\
	(pHcdPipe)->HcdReserved[3].Addrbits |=				\
		__TMS_USBPUMP_HCDKIT_SCHEDULE_IN_SERVICE;		\
	} while (0)

#define __TMS_USBPUMP_HCDKIT_SCHEDULE_CLR_IN_SERVICE(pHcdPipe)		\
    do	{								\
	(pHcdPipe)->HcdReserved[3].Addrbits &=				\
		~__TMS_USBPUMP_HCDKIT_SCHEDULE_IN_SERVICE;		\
	} while (0)



/****************************************************************************\
|
|	API functions
|
\****************************************************************************/

__TMS_BEGIN_DECLS

__TMS_USBPUMP_HCDKIT_SCHEDULE *
		UsbPumpHcdKitI_ScheduleCreate(
			__TMS_USBPUMP_HCD *		pHcd,
			__TMS_UINT			nLevel
			);

__TMS_BOOL	UsbPumpHcdKitI_ScheduleInit(
			__TMS_USBPUMP_HCD *		pHcd,
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_UINT			nLevel
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleDeinit(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleFinish(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_USTAT			Status
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleAddPipe(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_USBPUMP_HCD_PIPE *	pHcdPipe
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleRemovePipe(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_USBPUMP_HCD_PIPE *	pHcdPipe
			);

__TMS_USBPUMP_HCD_PIPE *
		UsbPumpHcdKitI_ScheduleFindFirstPipe(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_UINT32			FrameNumber
			);

__TMS_USBPUMP_HCD_PIPE *
		UsbPumpHcdKitI_ScheduleFindFirstPipe_V2(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_UINT32			FrameNumber,
			__TMS_BOOL			fPeriodicOnly
			);

__TMS_USBPUMP_HCD_PIPE *
		UsbPumpHcdKitI_ScheduleFindNextPipe(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule
			);

__TMS_USBPUMP_HCD_PIPE *
		UsbPumpHcdKitI_ScheduleFindNextPipe_V2(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_BOOL			fPeriodicOnly
			);

__TMS_USBPUMP_HCD_PIPE *
		UsbPumpHcdKitI_ScheduleFindFirstNonPeriodicPipe(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule
			);


__TMS_VOID	UsbPumpHcdKitI_ScheduleAddRequest(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_USBPUMP_HCD_PIPE *	pHcdPipe,
			__TMS_USBPUMP_HCD_REQUEST *	pHcdRequest
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleCancelRequests(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_USBPUMP_HCD_PIPE *	pHcdPipe,
			__TMS_USTAT			Status
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleCompleteRequest(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_USBPUMP_HCD_PIPE *	pHcdPipe,
			__TMS_USBPUMP_HCD_REQUEST *	pHcdRequest,
			__TMS_USTAT			Status
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleRemoveRequest(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule,
			__TMS_USBPUMP_HCD_PIPE *	pHcdPipe,
			__TMS_USBPUMP_HCD_REQUEST *	pHcdRequest
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleStart(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule
			);

__TMS_VOID	UsbPumpHcdKitI_ScheduleStop(
			__TMS_USBPUMP_HCDKIT_SCHEDULE *	pSchedule
			);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMP_HCDKIT_SCHEDULE_IN_SERVICE	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_IN_SERVICE
# define USBPUMP_HCDKIT_SCHEDULE_PIPE_ADDED	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_PIPE_ADDED
# define USBPUMP_HCDKIT_SCHEDULE_NODE_MASK	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_NODE_MASK
# define USBPUMP_HCDKIT_SCHEDULE_INIT_NODE(pHcdPipe, pNode)	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_INIT_NODE(pHcdPipe, pNode)
# define USBPUMP_HCDKIT_SCHEDULE_DEINIT_NODE(pHcdPipe)	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_DEINIT_NODE(pHcdPipe)
# define USBPUMP_HCDKIT_SCHEDULE_GET_NODE(pHcdPipe)	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_GET_NODE(pHcdPipe)
# define USBPUMP_HCDKIT_SCHEDULE_SET_NODE(pHcdPipe, pTreeNode)	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_SET_NODE(pHcdPipe, pTreeNode)
# define USBPUMP_HCDKIT_SCHEDULE_VALIDATE_HCD_PIPE(pHcdPipe)	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_VALIDATE_HCD_PIPE(pHcdPipe)
# define USBPUMP_HCDKIT_SCHEDULE_GET_IN_SERVICE(pHcdPipe)	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_GET_IN_SERVICE(pHcdPipe)
# define USBPUMP_HCDKIT_SCHEDULE_SET_IN_SERVICE(pHcdPipe)	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_SET_IN_SERVICE(pHcdPipe)
# define USBPUMP_HCDKIT_SCHEDULE_CLR_IN_SERVICE(pHcdPipe)	\
   __TMS_USBPUMP_HCDKIT_SCHEDULE_CLR_IN_SERVICE(pHcdPipe)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpump_hcdkit_schedule.h ****/
#endif /* _USBPUMP_HCDKIT_SCHEDULE_H_ */
