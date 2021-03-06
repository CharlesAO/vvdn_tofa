/* uposix_errno.h	Thu Dec 03 2009 12:45:30 lin */

/*

Module:  uposix_errno.h

Function:
	Values for errors for the USB DataPump Posix interface

Version:
	V2.01a  Thu Dec 03 2009 12:45:30 lin	Edit level 3

Copyright notice:
	This file copyright (C) 2005, 2008 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	September 2005

Revision history:
   1.91h  Sat Sep 10 2005 21:09:52  tmm
	Module created.

   1.97k  Wed Sep 03 2008 18:32:47  yssaroha
	6423: Added macro UPOSIX_ERR_ESUCCESS for no error

   2.01a  Thu Dec 03 2009 12:45:30  lin
	7838: Added UPOSIX_ERR_EUNKNOWN for unknown error code

*/

#ifndef _UPOSIX_ERRNO_H_		/* prevent multiple includes */
#define _UPOSIX_ERRNO_H_

/****************************************************************************\
|
|	List of error codes.  This list is loosely based on NetBSD 1.6.1,
|	with only the POSIX base identifier.
|
\****************************************************************************/

#define	__TMS_UPOSIX_ERR_ESUCCESS	0	/* Success/ No Error */
#define	__TMS_UPOSIX_ERR_EPERM		1	/* Operation not permitted */
#define	__TMS_UPOSIX_ERR_ENOENT		2	/* No such file or directory */
#define	__TMS_UPOSIX_ERR_ESRCH		3	/* No such process */
#define	__TMS_UPOSIX_ERR_EINTR		4	/* Interrupted system call */
#define	__TMS_UPOSIX_ERR_EIO		5	/* Input/output error */
#define	__TMS_UPOSIX_ERR_ENXIO		6	/* Device not configured */
#define	__TMS_UPOSIX_ERR_E2BIG		7	/* Argument list too long */
#define	__TMS_UPOSIX_ERR_ENOEXEC	8	/* Exec format error */
#define	__TMS_UPOSIX_ERR_EBADF		9	/* Bad file descriptor */
#define	__TMS_UPOSIX_ERR_ECHILD		10	/* No child processes */
#define	__TMS_UPOSIX_ERR_EDEADLK	11	/* Resource deadlock avoided */
#define	__TMS_UPOSIX_ERR_ENOMEM		12	/* Cannot allocate memory */
#define	__TMS_UPOSIX_ERR_EACCES		13	/* Permission denied */
#define	__TMS_UPOSIX_ERR_EFAULT		14	/* Bad address */
#define	__TMS_UPOSIX_ERR_EBUSY		15	/* Device busy */
#define	__TMS_UPOSIX_ERR_EEXIST		16	/* File exists */
#define	__TMS_UPOSIX_ERR_EXDEV		17	/* Cross-device link */
#define	__TMS_UPOSIX_ERR_ENODEV		18	/* Operation not supported by device */
#define	__TMS_UPOSIX_ERR_ENOTDIR	19	/* Not a directory */
#define	__TMS_UPOSIX_ERR_EISDIR		20	/* Is a directory */
#define	__TMS_UPOSIX_ERR_EINVAL		21	/* Invalid argument */
#define	__TMS_UPOSIX_ERR_ENFILE		22	/* Too many open files in system */
#define	__TMS_UPOSIX_ERR_EMFILE		23	/* Too many open files */
#define	__TMS_UPOSIX_ERR_ENOTTY		24	/* Inappropriate ioctl for device */
#define	__TMS_UPOSIX_ERR_EFBIG		25	/* File too large */
#define	__TMS_UPOSIX_ERR_ENOSPC		26	/* No space left on device */
#define	__TMS_UPOSIX_ERR_ESPIPE		27	/* Illegal seek */
#define	__TMS_UPOSIX_ERR_EROFS		28	/* Read-only file system */
#define	__TMS_UPOSIX_ERR_EMLINK		29	/* Too many links */
#define	__TMS_UPOSIX_ERR_EPIPE		30	/* Broken pipe */
#define	__TMS_UPOSIX_ERR_EDOM		31	/* Numerical argument out of domain */
#define	__TMS_UPOSIX_ERR_ERANGE		32	/* Result too large */
#define	__TMS_UPOSIX_ERR_EAGAIN		33	/* Resource temporarily unavailable */
#define	__TMS_UPOSIX_ERR_ENAMETOOLONG	34	/* File name too long */
#define	__TMS_UPOSIX_ERR_ENOTEMPTY	35	/* Directory not empty */
#define	__TMS_UPOSIX_ERR_ENOLCK		36	/* No locks available */
#define	__TMS_UPOSIX_ERR_ENOSYS		37	/* Function not implemented */
#define	__TMS_UPOSIX_ERR_EILSEQ		38	/* Illegal byte sequence */
#define __TMS_UPOSIX_ERR_EUNKNOWN	39	/* unknown error code */
#define	__TMS_UPOSIX_ERR_ELAST		39	/* Must equal largest errno */

/****************************************************************************\
|
|	Uncloaked names
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPOSIX_ERR_ESUCCESS	\
   __TMS_UPOSIX_ERR_ESUCCESS
# define UPOSIX_ERR_EPERM	\
   __TMS_UPOSIX_ERR_EPERM
# define UPOSIX_ERR_ENOENT	\
   __TMS_UPOSIX_ERR_ENOENT
# define UPOSIX_ERR_ESRCH	\
   __TMS_UPOSIX_ERR_ESRCH
# define UPOSIX_ERR_EINTR	\
   __TMS_UPOSIX_ERR_EINTR
# define UPOSIX_ERR_EIO	\
   __TMS_UPOSIX_ERR_EIO
# define UPOSIX_ERR_ENXIO	\
   __TMS_UPOSIX_ERR_ENXIO
# define UPOSIX_ERR_E2BIG	\
   __TMS_UPOSIX_ERR_E2BIG
# define UPOSIX_ERR_ENOEXEC	\
   __TMS_UPOSIX_ERR_ENOEXEC
# define UPOSIX_ERR_EBADF	\
   __TMS_UPOSIX_ERR_EBADF
# define UPOSIX_ERR_ECHILD	\
   __TMS_UPOSIX_ERR_ECHILD
# define UPOSIX_ERR_EDEADLK	\
   __TMS_UPOSIX_ERR_EDEADLK
# define UPOSIX_ERR_ENOMEM	\
   __TMS_UPOSIX_ERR_ENOMEM
# define UPOSIX_ERR_EACCES	\
   __TMS_UPOSIX_ERR_EACCES
# define UPOSIX_ERR_EFAULT	\
   __TMS_UPOSIX_ERR_EFAULT
# define UPOSIX_ERR_EBUSY	\
   __TMS_UPOSIX_ERR_EBUSY
# define UPOSIX_ERR_EEXIST	\
   __TMS_UPOSIX_ERR_EEXIST
# define UPOSIX_ERR_EXDEV	\
   __TMS_UPOSIX_ERR_EXDEV
# define UPOSIX_ERR_ENODEV	\
   __TMS_UPOSIX_ERR_ENODEV
# define UPOSIX_ERR_ENOTDIR	\
   __TMS_UPOSIX_ERR_ENOTDIR
# define UPOSIX_ERR_EISDIR	\
   __TMS_UPOSIX_ERR_EISDIR
# define UPOSIX_ERR_EINVAL	\
   __TMS_UPOSIX_ERR_EINVAL
# define UPOSIX_ERR_ENFILE	\
   __TMS_UPOSIX_ERR_ENFILE
# define UPOSIX_ERR_EMFILE	\
   __TMS_UPOSIX_ERR_EMFILE
# define UPOSIX_ERR_ENOTTY	\
   __TMS_UPOSIX_ERR_ENOTTY
# define UPOSIX_ERR_EFBIG	\
   __TMS_UPOSIX_ERR_EFBIG
# define UPOSIX_ERR_ENOSPC	\
   __TMS_UPOSIX_ERR_ENOSPC
# define UPOSIX_ERR_ESPIPE	\
   __TMS_UPOSIX_ERR_ESPIPE
# define UPOSIX_ERR_EROFS	\
   __TMS_UPOSIX_ERR_EROFS
# define UPOSIX_ERR_EMLINK	\
   __TMS_UPOSIX_ERR_EMLINK
# define UPOSIX_ERR_EPIPE	\
   __TMS_UPOSIX_ERR_EPIPE
# define UPOSIX_ERR_EDOM	\
   __TMS_UPOSIX_ERR_EDOM
# define UPOSIX_ERR_ERANGE	\
   __TMS_UPOSIX_ERR_ERANGE
# define UPOSIX_ERR_EAGAIN	\
   __TMS_UPOSIX_ERR_EAGAIN
# define UPOSIX_ERR_ENAMETOOLONG	\
   __TMS_UPOSIX_ERR_ENAMETOOLONG
# define UPOSIX_ERR_ENOTEMPTY	\
   __TMS_UPOSIX_ERR_ENOTEMPTY
# define UPOSIX_ERR_ENOLCK	\
   __TMS_UPOSIX_ERR_ENOLCK
# define UPOSIX_ERR_ENOSYS	\
   __TMS_UPOSIX_ERR_ENOSYS
# define UPOSIX_ERR_EILSEQ	\
   __TMS_UPOSIX_ERR_EILSEQ
# define UPOSIX_ERR_UNKNOWN	\
   __TMS_UPOSIX_ERR_UNKNOWN
# define UPOSIX_ERR_ELAST	\
   __TMS_UPOSIX_ERR_ELAST
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of uposix_errno.h ****/
#endif /* _UPOSIX_ERRNO_H_ */
