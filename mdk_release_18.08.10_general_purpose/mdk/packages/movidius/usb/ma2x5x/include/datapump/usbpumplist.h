/* usbpumplist.h	Thu May 10 2007 12:09:05 chwon */

/*

Module:  usbpumplist.h

Function:
	Macros for circular queue manipulation.

Version:
	V1.97i	Thu May 10 2007 12:09:05 chwon	Edit level 5

Copyright notice:
	This file copyright (C) 2002-2004, 2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	June 2002

Revision history:
   1.79a  Tue Jun 18 2002 02:24:17  tmm
	Module created.

   1.87b  Tue Apr 20 2004 16:03:45  chwon
	Corrected type cloaking.

   1.93e  Fri Jan 13 2006 17:27:37  tmm
	1615: fix list corruption in USBPUMPLIB_LIST_APPEND_LIST when
	second list was more than one node long.

   1.97i  Thu May 10 2007 12:09:05  chwon
	2619: fix USBPUMPLIB_LIST_NODE_INIT() document.

*/

#ifndef _USBPUMPLIST_H_		/* prevent multiple includes */
#define _USBPUMPLIST_H_

#ifndef _USBPUMPENV_H_
# include "usbpumpenv.h"
#endif

/*

Name:	USBPUMPLIB_LIST_APPEND_LIST()

Function:
	Macro: append one circularly-linked list to another.

Definition:
	VOID USBPUMPLIB_LIST_APPEND_LIST(
		type NODE_T,
		NODE_T **ppHead,
		NODE_T *pNode,
		FieldName NODE_T *Next,
		FieldName NODE_T *Last
		);

Description:
	This macro appends the list headed by pNode to the list
	rooted at *ppHead.  NODE_T is the type of each node in the list;
	Next and Last are the *names* of the next and last pointers
	that thread the list through each NODE_T (this is to allow for
	multi-threaded lists).  The list at pNode is appended to the list
	at pHead.

Returns:
	No explicit result.

Notes:
	Often, pNode is a simple list node.  If it is known to be a single
	node, but its state is indeterminate, it must be initialized
	using USBPUMP_LIST_NODE_INIT() prior to calling this macro.

	Although this is a macro, the arguments are evaluated exactly once.

*/

#define __TMS_USBPUMPLIB_LIST_APPEND_LIST(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)								\
    do	{								\
	NODE_T ** __TMS_CONST __ppHead = (ppHead);			\
	NODE_T *  __TMS_CONST __pNode = (pNode);			\
	NODE_T *  __TMS_CONST __pHead = *__ppHead;			\
									\
	if (__pHead == __TMS_NULL)					\
		{							\
		*__ppHead = __pNode;					\
		}							\
	else								\
		{							\
		NODE_T * __TMS_CONST __pTail = __pHead->Last;		\
		NODE_T * __TMS_CONST __pNewTail = __pNode->Last;	\
		__pNode->Last = __pTail;				\
		__pNewTail->Next = __pHead;				\
		__pTail->Next = __pNode;				\
		__pHead->Last = __pNewTail;				\
		}							\
	} while (0)

/*

Name:	USBPUMPLIB_LIST_INSERT_NODE_TAIL()

Function:
	Macro: append a single node to a circularly-linked list.

Definition:
	VOID USBPUMPLIB_LIST_INSERT_NODE_TAIL(
		type NODE_T,
		NODE_T **ppHead,
		NODE_T *pNode,
		FieldName NODE_T *Next,
		FieldName NODE_T *Last
		);

Description:
	This macro appends the list headed by pNode to the list
	rooted at *ppHead.  NODE_T is the type of each node in the list;
	Next and Last are the *names* of the next and last pointers
	that thread the list through each NODE_T (this is to allow for
	multi-threaded lists).  The list at pNode is appended to the list
	at pHead.

Returns:
	No explicit result.

Notes:
	The previous values of the list thread entries in pNode are ignored.

	Although this is a macro, the arguments are evaluated exactly once.

*/

#define __TMS_USBPUMPLIB_LIST_INSERT_NODE_TAIL(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)								\
    do	{								\
	NODE_T ** __TMS_CONST __ppHead = (ppHead);			\
	NODE_T *  __TMS_CONST __pNode = (pNode);			\
	NODE_T *  __TMS_CONST __pHead = *__ppHead;			\
									\
	if (__pHead == __TMS_NULL)					\
		{							\
		__pNode->Next = __pNode;				\
		__pNode->Last = __pNode;				\
		*__ppHead = __pNode;					\
		}							\
	else								\
		{							\
		NODE_T *__TMS_CONST __pTail = __pHead->Last;		\
		__pNode->Next = __pHead;				\
		__pNode->Last = __pTail;				\
		__pTail->Next = __pNode;				\
		__pHead->Last = __pNode;				\
		}							\
	} while (0)

/*

Name:	USBPUMPLIB_LIST_NODE_INIT()

Function:
	Initialize a list node to create a single-element list.

Definition:
	VOID USBPUMPLIB_LIST_NODE_INIT(
		type NODE_T,
		NODE_T *pNode,
		FieldName NODE_T *Next,
		FieldName NODE_T *Last
		);

Description:
	Simply initializes the list in a standard way.

Returns:
	No explicit result.

*/

#define __TMS_USBPUMPLIB_LIST_NODE_INIT(NODE_T, pNode, Next, Last)	\
   do	{								\
	NODE_T * __TMS_CONST __pNode = (pNode);				\
									\
	__pNode->Next = __pNode->Last = __pNode;			\
	} while (0)

/*

Name:	USBPUMPLIB_LIST_FOREACH

Index:	Name:	USBPUMPLIB_LIST_FOREACH_BEGIN()
		USBPUMPLIB_LIST_FOREACH_CONTINUE()
		USBPUMPLIB_LIST_FOREACH_END()

Function:
	Standard list iteration control, assuming list is not changing.

Definition:
	USBPUMPLIB_LIST_FOREACH_BEGIN(
		Type NODETYPE,
		NODETYPE * CONST pHead,
		lv NODETYPE pNode
		)
			{list commands using pNode; }
	USBPUMPLIB_LIST_FOREACH_END(
		lv NODETYPE pNode,
		Field_Name NextName
		)

	or

	USBPUMPLIB_LIST_FOREACH_CONTINUE(
		Type NODETYPE,
		NODETYPE * CONST pHead,
		lv NODETYPE pNode
		)
			{list commands using pNode; }
	USBPUMPLIB_LIST_FOREACH_END(
		lv NODETYPE pNode,
		Field_Name NextName
		)

Description:
	These commands, taken together, are syntactically equivalent to
	'{' and '}'; they enclose a section of code that is iteratively
	executed for each pNode in the list starting at pHead.

	pNode must be a left-value (e.g., a variable name).  pHead is
	be a right-value, referring to a the first node on the list;
	it is evaluated exactly once and then stored for the duration of
	the loop.

	USBPUMPLIB_LIST_FOREACH_BEGIN() initializes pNode to point to the
	beginning of the list, and iterates over the entire list.

	USBPUMPLIB_LIST_FOREACH_CONTINUE() assumes that pNode points to a
	node somewhere in the list, or is NULL, and continues the iteration
	from there until the end of the list is reached.

Returns:
	No explicit result.

Bugs:
	Can generate bizarre compile errors if not used correctly.

*/

#define __TMS_USBPUMPLIB_LIST_FOREACH_BEGIN(TYPE, pHead, pNode)		\
   	{								\
	TYPE * __TMS_CONST __pHead = (pHead);				\
									\
	if (((pNode) = __pHead) != __TMS_NULL)				\
	    do	{
		/**** end of begin part ****/

#define __TMS_USBPUMPLIB_LIST_FOREACH_CONTINUE(TYPE, pHead, pNode)	\
   	{								\
	TYPE * __TMS_CONST __pHead = (pHead);				\
									\
	if ((pNode) != __TMS_NULL)					\
	    do	{
		/**** end of begin part ****/

#define __TMS_USBPUMPLIB_LIST_FOREACH_END(pNode, Next)			\
		(pNode) = (pNode)->Next;				\
		} while ((pNode) != __pHead);				\
	}

/*

Name:	USBPUMPLIB_LIST_REMOVE_NODE()

Function:
	Macro: remove a single node from a circularly-linked list.

Definition:
	VOID USBPUMPLIB_LIST_REMOVE_NODE(
		type NODE_T,
		NODE_T **ppHead,
		NODE_T *pNode,
		FieldName NODE_T *Next,
		FieldName NODE_T *Last
		);

Description:
	The node specified by pNode is removed from the list (field names
	indicated by Next and Last).  If pNode was the head, then *ppHead
	is updated appropriately (either to pNode's successor or to NULL)
	NODE_T is the type of each node in the list;
	Next and Last are the *names* of the next and last pointers
	that thread the list through each NODE_T (this is to allow for
	multi-threaded lists).

Returns:
	No explicit result.

Notes:
	Although this is a macro, the arguments are evaluated exactly once.

*/

#define __TMS_USBPUMPLIB_LIST_REMOVE_NODE(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)								\
    do	{								\
	NODE_T ** __TMS_CONST __ppHead = (ppHead);			\
	NODE_T *  __TMS_CONST __pNode = (pNode);			\
									\
	if (__pNode != __TMS_NULL)					\
		{							\
		NODE_T * __TMS_CONST __pNext = __pNode->Next;		\
		NODE_T * __TMS_CONST __pLast = __pNode->Last;		\
		__pNext->Last = __pLast;				\
		__pLast->Next = __pNext;				\
		__pNode->Next = __pNode->Last = __pNode;		\
									\
		if (__ppHead && *__ppHead == __pNode)			\
			{						\
			/* deleted head of list */			\
			if (__pNode == __pNext)				\
				{					\
				/* deleted only element from list */	\
				*__ppHead = __TMS_NULL;			\
				}					\
			else						\
				{					\
				*__ppHead = __pNext;			\
				}					\
			}						\
		}							\
	} while (0)


/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define USBPUMPLIB_LIST_APPEND_LIST(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)	\
	__TMS_USBPUMPLIB_LIST_APPEND_LIST(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)
# define USBPUMPLIB_LIST_INSERT_NODE_TAIL(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)	\
	__TMS_USBPUMPLIB_LIST_INSERT_NODE_TAIL(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)
# define USBPUMPLIB_LIST_NODE_INIT(NODE_T, pNode, Next, Last)	\
   __TMS_USBPUMPLIB_LIST_NODE_INIT(NODE_T, pNode, Next, Last)
# define USBPUMPLIB_LIST_FOREACH_BEGIN(TYPE, pHead, pNode)	\
   __TMS_USBPUMPLIB_LIST_FOREACH_BEGIN(TYPE, pHead, pNode)
# define USBPUMPLIB_LIST_FOREACH_CONTINUE(TYPE, pHead, pNode)	\
   __TMS_USBPUMPLIB_LIST_FOREACH_CONTINUE(TYPE, pHead, pNode)
# define USBPUMPLIB_LIST_FOREACH_END(pNode, Next)	\
   __TMS_USBPUMPLIB_LIST_FOREACH_END(pNode, Next)
# define USBPUMPLIB_LIST_REMOVE_NODE(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)	\
	__TMS_USBPUMPLIB_LIST_REMOVE_NODE(				\
	NODE_T, ppHead, pNode, Next, Last				\
	)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of usbpumplist.h ****/
#endif /* _USBPUMPLIST_H_ */
