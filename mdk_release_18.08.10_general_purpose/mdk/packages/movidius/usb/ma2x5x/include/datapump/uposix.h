/* uposix.h	Tue Oct 20 2009 08:48:30 lin */

/*

Module:  uposix.h

Function:
	The definitions for POSIX API (OS independant)

Version:
	V2.01a	Tue Oct 20 2009 08:48:30 lin	Edit level 18

Copyright notice:
	This file copyright (C) 2005-2009 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Fredrik Norrby, MCCI Corporation	May 2005

Revision history:
   1.91b  Mon May 02 2005 10:22:15  fny
	Module created.

   1.91d  Tue Jun 14 2005 08:20:19  maw
   	Removed memory functions. Use UTASK vesions

   1.91f  Mon Jul 11 2005 01:08:08  maw
   	Changed UPOSIX_DIRENT to match MCCI defs

   1.91h  Thu Sep 08 2005 20:38:04 maw
   	Added errno and ferror fn's

   1.91h  Sat Sep 10 2005 03:16:07  maw
   	Added config structure

   1.91i  Tue Sep 27 2005 08:22:05  maw
   	Add GetMaxPath, and macros for PathCompareBuffer
   	and PathCompareString

   1.93b  Fri Nov 18 2005 10:17:58  maw
   	Added UPOSIX_PERMISSIONS bits

   1.97b  Wed May 03 2006 11:57:01  chwon
	Added UPOSIX_FSTAT and UPOSIX_GETTIME API and related structure

   1.97e  Fri Aug 18 2006 13:30:05 yssaroha
	Added UPOSIX_UTIME API and related structure.
	While solving PR 1915

   1.97f  Tue Feb 06 2007 16:18:07  chwon
	2342: Add UPOSIX context pointer parameter in UPOSIX_GETMAXPATH

   1.97h  Mon Mar 12 2007 11:42:29  chwon
	2498: Added UPOSIX_FSYNC API support

   1.97h  Fri Mar 23 2007 11:18:19  chwon
	2578: added UPOSIX_RENAME API support

   1.97j  Thu Dec 13 2007 11:42:38  chwon
	3118: Change name from UPOSIX_FSYNC to UPOSIX_FFLUSH

   1.97k  Fri Aug 29 2008 14:07:31  yssaroha
	6423: Added posix function UPOSIX_GETTIMEOFDAY64_FN and
	its associated macro and structure
	Updated UPOSIX_TIMESPEC_T structure to use generic type variable

   2.01a  Thu Mar 19 2009 09:31:59  chwon
	6367: Added UPOSIX_DIR_CONTENTS/METHODS, UPOSIX_FILE_CONTENTS/METHODS
	structure. Added new methods of UPOSIX_FILE and UPOSIX_DIR.

   2.01a  Thu Apr 16 2009 15:00:20  lin
	7794: Added UPOSIX_STATFS_T, UPOSIX_SETSIZE_FN, UPOSIX_STATFS_FN

   2.01a  Wed May 13 2009 14:07:30  lin
	7794: Removed UPOSIX_SETSIZE_FN, added UPOSIX_FSETSIZE_FN
	Added UPOSIX_FILE_FSETSIZE_FN

   2.01a  Tue Oct 20 2009 08:48:30  lin
	7838: Added UPOSIX_ERRNO_CONTENTS

*/

#ifndef _UPOSIX_H_		/* prevent multiple includes */
#define _UPOSIX_H_

/* MCCI include files */
#ifndef _UPOSIX_TYPES_H_
# include "uposix_types.h"
#endif

#ifndef _USBPUMP_PATHLIB_TYPES_H_
# include "usbpump_pathlib_types.h"
#endif

/****************************************************************************\
|
|	Macro declarations.
|
\****************************************************************************/


/****************************************************************************\
|
|	Posix type declarations.
|
\****************************************************************************/

/*

Type:	UPOSIX_DIRENT

Index:	Type:	PUPOSIX_DIRENT
	Name:	UPOSIX_DIRENT__HDR()

Function:
	Returns next directory entry from UPOSIX_READDIR()

Description:
	UPOSIX_READDIR() is used to read through directories.  Each call
	returns a pointer to a UPOSIX_DIRECT structure, which is live
	until the next call, and which contains (at the portable level)
	the name of the next file.  The returned name is simply the name
	of the entry within the directory.

Contents:
	TEXT d_name[];
		This contains the UTF-8 representation of the name,
		terminated by a '\0'.  The maximimum length of this string
		is determined by the underlying operating system.

Notes:
	The UPOSIX_DIRENT object is deliberately declared as a template with
	a small size.  If you want to copy the string, make a copy of the
	d_name[] string, not a copy of the UPOSIX_DIRENT instance.

See Also:
	UPOSIX_OPENDIR(), UPOSIX_READDIR()

*/

#define	__TMS_UPOSIX_DIRENT__HDR(nBytes)	\
	__TMS_TEXT	d_name[(nBytes)]

struct __TMS_STRUCTNAME(UPOSIX_DIRENT)
	{
	__TMS_UPOSIX_DIRENT__HDR(1);
	};

/* __TMS_TYPE_DEF_ARG(UPOSIX_FILE_PERM, INT32); */
#define __TMS_UPOSIX_FILE_PERM_NO_PROTECT	0
#define	__TMS_UPOSIX_FILE_PERM_READ_ONLY	1
#define	__TMS_UPOSIX_FILE_PERM_NO_FILE		-1

/* __TMS_TYPE_DEF_ARG(UPOSIX_FILE_SEEK, BYTES); */

#define	__TMS_UPOSIX_FILE_SEEK_SET		0
#define	__TMS_UPOSIX_FILE_SEEK_CUR		1
#define	__TMS_UPOSIX_FILE_SEEK_END		2

/* File Permissions bits used by MKDIR */
#define __TMS_UPOSIX_PERMISSIONS_WIDTH		3
#define __TMS_UPOSIX_PERMISSIONS_USR_SHIFT	0
#define __TMS_UPOSIX_PERMISSIONS_USR_MASK	(((1 <<  __TMS_UPOSIX_PERMISSIONS_WIDTH)-1) \
						     <<  __TMS_UPOSIX_PERMISSIONS_USR_SHIFT)
#define __TMS_UPOSIX_PERMISSIONS_GRP_SHIFT	(__TMS_UPOSIX_PERMISSIONS_USR_SHIFT + \
						 __TMS_UPOSIX_PERMISSIONS_WIDTH)
#define __TMS_UPOSIX_PERMISSIONS_GRP_MASK	(((1 <<  __TMS_UPOSIX_PERMISSIONS_WIDTH)-1) \
						     <<  __TMS_UPOSIX_PERMISSIONS_GRP_SHIFT)
#define __TMS_UPOSIX_PERMISSIONS_OTH_SHIFT	(__TMS_UPOSIX_PERMISSIONS_GRP_SHIFT + \
						 __TMS_UPOSIX_PERMISSIONS_WIDTH)
#define __TMS_UPOSIX_PERMISSIONS_OTH_MASK	(((1 <<  __TMS_UPOSIX_PERMISSIONS_WIDTH)-1) \
						     <<  __TMS_UPOSIX_PERMISSIONS_OTH_SHIFT)

#define __TMS_UPOSIX_PERMISSIONS_IRUSR		(1 << (__TMS_UPOSIX_PERMISSIONS_USR_SHIFT+0))
#define __TMS_UPOSIX_PERMISSIONS_IWUSR		(1 << (__TMS_UPOSIX_PERMISSIONS_USR_SHIFT+1))
#define __TMS_UPOSIX_PERMISSIONS_IXUSR		(1 << (__TMS_UPOSIX_PERMISSIONS_USR_SHIFT+2))
#define __TMS_UPOSIX_PERMISSIONS_IRGRP		(1 << (__TMS_UPOSIX_PERMISSIONS_GRP_SHIFT+0))
#define __TMS_UPOSIX_PERMISSIONS_IWGRP		(1 << (__TMS_UPOSIX_PERMISSIONS_GRP_SHIFT+1))
#define __TMS_UPOSIX_PERMISSIONS_IXGRP		(1 << (__TMS_UPOSIX_PERMISSIONS_GRP_SHIFT+2))
#define __TMS_UPOSIX_PERMISSIONS_IROTH		(1 << (__TMS_UPOSIX_PERMISSIONS_OTH_SHIFT+0))
#define __TMS_UPOSIX_PERMISSIONS_IWOTH		(1 << (__TMS_UPOSIX_PERMISSIONS_OTH_SHIFT+1))
#define __TMS_UPOSIX_PERMISSIONS_IXOTH		(1 << (__TMS_UPOSIX_PERMISSIONS_OTH_SHIFT+2))
#define __TMS_UPOSIX_PERMISSIONS_IRALL		(__TMS_UPOSIX_PERMISSIONS_IRUSR | \
						 __TMS_UPOSIX_PERMISSIONS_IRGRP | \
						 __TMS_UPOSIX_PERMISSIONS_IROTH)
#define __TMS_UPOSIX_PERMISSIONS_IWALL		(__TMS_UPOSIX_PERMISSIONS_IWUSR | \
						 __TMS_UPOSIX_PERMISSIONS_IWGRP | \
						 __TMS_UPOSIX_PERMISSIONS_IWOTH)
#define __TMS_UPOSIX_PERMISSIONS_IXALL		(__TMS_UPOSIX_PERMISSIONS_IXUSR | \
						 __TMS_UPOSIX_PERMISSIONS_IXGRP | \
						 __TMS_UPOSIX_PERMISSIONS_IXOTH)

#define __TMS_UPOSIX_STATFS_F_BSIZE		(1 << 0)
#define __TMS_UPOSIX_STATFS_F_BAVAIL		(1 << 1)
#define __TMS_UPOSIX_STATFS_F_IOSIZE		(1 << 2)
#define __TMS_UPOSIX_STATFS_F_BLOCKS		(1 << 3)
#define __TMS_UPOSIX_STATFS_F_ALL		(__TMS_UPOSIX_STATFS_F_BSIZE | \
						 __TMS_UPOSIX_STATFS_F_BAVAIL | \
						 __TMS_UPOSIX_STATFS_F_BLOCKS | \
						 __TMS_UPOSIX_STATFS_F_IOSIZE)

#define __TMS_UPOSIX_STATFS_IS_F_BLOCKS_VALID(a_validflags)	\
	((a_validflags) & __TMS_UPOSIX_STATFS_F_BLOCKS)
#define __TMS_UPOSIX_STATFS_IS_F_BSIZE_VALID(a_validflags)	\
	((a_validflags) & __TMS_UPOSIX_STATFS_F_BSIZE)
#define __TMS_UPOSIX_STATFS_IS_F_BAVAIL_VALID(a_validflags) \
	((a_validflags) & __TMS_UPOSIX_STATFS_F_BAVAIL)
#define __TMS_UPOSIX_STATFS_IS_F_IOSIZE_VALID(a_validflags) \
	((a_validflags) & __TMS_UPOSIX_STATFS_F_IOSIZE)

struct __TMS_STRUCTNAME(UPOSIX_TIMESPEC_T)
	{
	__TMS_UPOSIX_TIME_T		tv_sec;		/* seconds */
	__TMS_UPOSIX_SNSECONDS_T	tv_nsec;	/* and nanoseconds */
	};

struct __TMS_STRUCTNAME(UPOSIX_STAT_T)
	{
	__TMS_UINT32		st_dev;		/* device containing the file */
	__TMS_UINT32		st_ino;		/* file's serial number */
	__TMS_UPOSIX_FILE_PERM	st_mode;	/* file's mode (protection and type) */
	__TMS_UINT32		st_nlink;	/* number of hard links to the file */
	__TMS_UINT32		st_uid;		/* user-id of owner */
	__TMS_UINT32		st_gid;		/* group-id of owner */
	__TMS_UINT32		st_rdev;	/* device type, for device special file */
	__TMS_UPOSIX_TIMESPEC_T	st_atimespec;	/* time of last access */
	__TMS_UPOSIX_TIMESPEC_T	st_mtimespec;	/* time of last data modification */
	__TMS_UPOSIX_TIMESPEC_T	st_ctimespec;	/* time of last file status change */
	__TMS_UPOSIX_FOFFSET_T	st_size;	/* file size, in bytes */
	__TMS_INT64		st_blocks;	/* blocks allocated for file */
	__TMS_UINT32		st_blksize;	/* optimal file sys I/O ops blocksize */
	__TMS_UINT32		st_flags;	/* user defined flags for file */
	__TMS_UINT32		st_gen;		/* file generation number */
	};

struct __TMS_STRUCTNAME(UPOSIX_STATFS_T)
	{
	__TMS_UINT		f_validflags;	/* contains 1 bit for each of other fields */
	__TMS_UPOSIX_FBLOCKS_T	f_blocks;	/* total blocks */
	__TMS_UPOSIX_FBLOCKS_T	f_bsize;	/* transfer block size */
	__TMS_UPOSIX_FBLOCKS_T	f_bavail;	/* free blocks to all users except root users */
	__TMS_UPOSIX_FBLOCKS_T	f_iosize;	/* optimal transfer block size */
	};

struct __TMS_STRUCTNAME(UPOSIX_TM_T)
	{
	__TMS_UINT16	tm_year;	/* the year, in AD */
	__TMS_UINT16	tm_jday;	/* julian day of this year: 1..366 */
	__TMS_UINT16	tm_hour;	/* hour of day: 0..23 */
	__TMS_UINT16	tm_min;		/* minute of hour: 0..59 */
	__TMS_UINT16	tm_sec;		/* second of min:  0..59 */
	__TMS_UINT16	tm_tenthsec;	/* tenths of a second past the second: 0..9 */
	__TMS_UINT16	tm_month;	/* month of year:  0..11 */
	__TMS_UINT16	tm_mday;	/* day of month:   0..31 */
	__TMS_UINT16	tm_weekday;	/* day of week:    0..6, Sunday = 0 */
	__TMS_UINT32	tm_clock;	/* seconds since 1/1/1970, 0:0 */
	};


struct __TMS_STRUCTNAME(UPOSIX_UTIMEBUF)
	{
	__TMS_UPOSIX_TIMESPEC_T	u_atimespec;	/* time of last access */
	__TMS_UPOSIX_TIMESPEC_T	u_mtimespec;	/* time of last data modification */
	};

struct __TMS_STRUCTNAME(UPOSIX_TIMESPEC64_T)
	{
	__TMS_UPOSIX_TIME64_T 		tv_sec;		/* seconds */
	__TMS_UPOSIX_SNSECONDS64_T 	tv_usec;	/* and nanoseconds */
	};


struct __TMS_STRUCTNAME(UPOSIX_DIR_CONTENTS)
	{
	__TMS_UPOSIX_DIR *			pImplementationContext;
	__TMS_UPOSIX_CTX *			pPosixContext;
	__TMS_CONST __TMS_UPOSIX_DIR_METHODS *	pDirMethods;
	};

struct __TMS_STRUCTNAME(UPOSIX_DIR_METHODS)
	{
	__TMS_UPOSIX_DIR_CLOSEDIR_FN *		pCloseFn;
	__TMS_UPOSIX_DIR_READDIR_FN *		pReadFn;
	};

#define	__TMS_UPOSIX_DIR_METHODS_INIT_V1(	\
		a_pCloseFn,			\
		a_pReadFn			\
		)				\
		{				\
		(a_pCloseFn),			\
		(a_pReadFn)			\
		}


struct __TMS_STRUCTNAME(UPOSIX_FILE_CONTENTS)
	{
	__TMS_UPOSIX_FILE *			pImplementationContext;
	__TMS_UPOSIX_CTX *			pPosixContext;
	__TMS_CONST __TMS_UPOSIX_FILE_METHODS *	pFileMethods;
	};

struct __TMS_STRUCTNAME(UPOSIX_FILE_METHODS)
	{
	__TMS_UPOSIX_FILE_FCLOSE_FN *		pCloseFn;
	__TMS_UPOSIX_FILE_FREAD_FN *		pReadFn;
	__TMS_UPOSIX_FILE_FWRITE_FN *		pWriteFn;
	__TMS_UPOSIX_FILE_FGETC_FN *		pGetcFn;
	__TMS_UPOSIX_FILE_FTELL_FN *		pTellFn;
	__TMS_UPOSIX_FILE_FSEEK_FN *		pSeekFn;
	__TMS_UPOSIX_FILE_FEOF_FN *		pEofFn;
	__TMS_UPOSIX_FILE_FERROR_FN *		pErrorFn;
	__TMS_UPOSIX_FILE_FFLUSH_FN *		pFlushFn;
	__TMS_UPOSIX_FILE_FSETSIZE_FN *		pSetSizeFn;
	};

#define	__TMS_UPOSIX_FILE_METHODS_INIT_V2(	\
		a_pCloseFn,			\
		a_pReadFn,			\
		a_pWriteFn,			\
		a_pGetcFn,			\
		a_pTellFn,			\
		a_pSeekFn,			\
		a_pEofFn,			\
		a_pErrorFn,			\
		a_pFlushFn,			\
		a_pSetSizeFn			\
		)				\
		{				\
		(a_pCloseFn),			\
		(a_pReadFn),			\
		(a_pWriteFn),			\
		(a_pGetcFn),			\
		(a_pTellFn),			\
		(a_pSeekFn),			\
		(a_pEofFn),			\
		(a_pErrorFn),			\
		(a_pFlushFn),			\
		(a_pSetSizeFn)			\
		}

#define	__TMS_UPOSIX_FILE_METHODS_INIT_V1(	\
		a_pCloseFn,			\
		a_pReadFn,			\
		a_pWriteFn,			\
		a_pGetcFn,			\
		a_pTellFn,			\
		a_pSeekFn,			\
		a_pEofFn,			\
		a_pErrorFn,			\
		a_pFlushFn			\
		)				\
	 __TMS_UPOSIX_FILE_METHODS_INIT_V2(	\
		a_pCloseFn,			\
		a_pReadFn,			\
		a_pWriteFn,			\
		a_pGetcFn,			\
		a_pTellFn,			\
		a_pSeekFn,			\
		a_pEofFn,			\
		a_pErrorFn,			\
		a_pFlushFn,			\
		UsbPumpPosix_File_SetSizeStub	\
		)				\

struct __TMS_STRUCTNAME(UPOSIX_ERRNO_CONTENTS)
	{
	__TMS_UPOSIX_ERRNO_T	PosixErrno;
	__TMS_UPOSIX_ERRNO_T	NativeErrno;
	__TMS_BOOL		fValid;
	};

#define __TMS_UPOSIX_ERRNO_CONTENTS_RESET(		\
		a_pCtx,					\
		a_Errno					\
		)					\
    do	{						\
	(a_pCtx)->ErrnoContents.PosixErrno = 0;		\
	(a_pCtx)->ErrnoContents.NativeErrno = (a_Errno);\
	(a_pCtx)->ErrnoContents.fValid = __TMS_FALSE;	\
	} while (0)

#define __TMS_UPOSIX_SET_NATIVE_ERRNO(			\
		a_pCtx,					\
		a_Errno					\
		)					\
    do	{						\
	(a_pCtx)->ErrnoContents.NativeErrno = (a_Errno);\
	(a_pCtx)->ErrnoContents.fValid = __TMS_FALSE;	\
	} while (0)

#define __TMS_UPOSIX_SET_POSIX_ERRNO(			\
		a_pCtx,					\
		a_Errno					\
		)					\
   do	{						\
	(a_pCtx)->ErrnoContents.PosixErrno = (a_Errno);	\
	(a_pCtx)->ErrnoContents.fValid = __TMS_TRUE;	\
	} while (0)

#define __TMS_UPOSIX_GET_NATIVE_ERRNO(			\
		a_pCtx					\
		)					\
	((a_pCtx)->ErrnoContents.NativeErrno)

#define __TMS_UPOSIX_GET_POSIX_ERRNO(			\
		a_pCtx					\
		)					\
	((a_pCtx)->ErrnoContents.PosixErrno)

#define __TMS_UPOSIX_ERRNO_VALID(			\
		a_pCtx					\
		)					\
	((a_pCtx)->ErrnoContents.fValid)

/****************************************************************************\
|
|	POSIX context structure
|
\****************************************************************************/

struct __TMS_STRUCTNAME(UPOSIX_CTX)
	{
	/* Methods function pointer */
	__TMS_CONST __TMS_UPOSIX_SWITCH *	pSwitch;

	/* Error number */
	__TMS_UPOSIX_ERRNO_T			ErrorNo;

	/* Errno contents */
	__TMS_UPOSIX_ERRNO_CONTENTS		ErrnoContents;

	/* optional client context pointer */
	__TMS_VOID *				pClientCtx;

	/* pointer to task header */
	__TMS_UTASK_HDR *			pTaskHdr;

	/* pathlib context */
	__TMS_USBPUMP_PATHLIB_CONTEXT *		pPathlibCtx;
	};

/****************************************************************************\
|
|	This structure is used to pass posix functions
|
\****************************************************************************/

/*

Type:	UPOSIX_SWITCH

Index:	Type:	PUPOSIX_SWITCH
	Name:	UPOSIX_SWITCH_INIT_V7()
	Name:	UPOSIX_SWITCH_INIT_V6()
	Name:	UPOSIX_SWITCH_INIT_V5()
	Name:	UPOSIX_SWITCH_INIT_V4()
	Name:	UPOSIX_SWITCH_INIT_V3()
	Name:	UPOSIX_SWITCH_INIT_V2()
	Name:	UPOSIX_SWITCH_INIT_V1()
	Name:	UPOSIX_MAGIC_BEGIN
	Name:	UPOSIX_MAGIC_END

Function:
	Provides the constant methods for the abstract POSIX-like API.

Description:
	The abstract POSIX API is concretely defined by the contents of
	this switch.  This switch is used for creating the UPOSIX_CTX
	object, and additionally provides methods for use by the operating
	system layer.

Contents:
	ULONG MagicBegin;
		A semaphore integer, set to UPOSIX_MAGIC_BEGIN.

	UPOSIX_INIT_FN * pInit;
		Pointer to the function that creates the posix-context
		object.

	UPOSIX_OPENDIR_FN * pOpenDir;
	UPOSIX_CLOSEDIR_FN * pCloseDir;
	UPOSIX_READDIR_FN * pReadDir;
	UPOSIX_MKDIR_FN * pMkDir;
	UPOSIX_RMDIR_FN * pRmDir;
	UPOSIX_FOPEN_FN * pFOpen;
	UPOSIX_FCLOSE_FN * pFClose;
	UPOSIX_FREAD_FN * pFRead;
	UPOSIX_FWRITE_FN * pFWrite;
	UPOSIX_FGETC_FN * pFGetC;
	UPOSIX_UNLINK_FN * pUnlink;
	UPOSIX_FTELL_FN * pFTell;
	UPOSIX_FSEEK_FN * pFSeek;
	UPOSIX_FEOF_FN * pFeof;
	UPOSIX_FERROR_FN * pFerror;
	UPOSIX_ISDIR_FN * pIsDir;
	UPOSIX_CHMOD_FN * pChMod;
	UPOSIX_GETMOD_FN * pGetMod;
	UPOSIX_GETSIZE_FN * pGetSize;
	UPOSIX_GETERRNO_FN * pGetErrno;
	UPOSIX_SETERRNO_FN * pSetErrno;
	UPOSIX_PATH_COMPARE_BUFFER_FN *pPathCompareBuffer;
	UPOSIX_PATH_COMPARE_STRING_FN *pPathCompareString;
	UPOSIX_GETMAXPATH_FN *pGetMaxPath;
	UPOSIX_FSTAT_FN * pFstat;
	UPOSIX_GETTIME_FN * pGetTime;
	UPOSIX_GETTIME_FN * pUpdateFileTime;
	UPOSIX_FFLUSH_FN * pFflush;
	UPOSIX_RENAME_FN * pRename;
	UPOSIX_GETTIMEOFDAY64_FN * pGettimeOfDay64;
	UPOSIX_DIR_OPEN_FN * pDirOpen;
	UPOSIX_FILE_OPEN_FN * pFileOpen;
		Method function pointers.
	UPOSIX_FSETSIZE_FN * pFSetSize;
	UPOSIX_STATFS_FN *pStatfs;

	ULONG MagicEnd;
		Closing semaphore integer, set to UPOSIX_MAGIC_END.

Notes:
	This structure should be initialized at compile time with
	the following macro:

	CONST UPOSIX_SWITCH
		UPOSIX_SWITCH_INIT_V8(
			UPOSIX_OPENDIR_FN * pOpenDir,
			UPOSIX_CLOSEDIR_FN * pCloseDir,
			UPOSIX_READDIR_FN * pReadDir,
			UPOSIX_MKDIR_FN * pMkDir,
			UPOSIX_RMDIR_FN * pRmDir,
			UPOSIX_FOPEN_FN * pFOpen,
			UPOSIX_FCLOSE_FN * pFClose,
			UPOSIX_FREAD_FN * pFRead,
			UPOSIX_FWRITE_FN * pFWrite,
			UPOSIX_FGETC_FN * pFGetC,
			UPOSIX_UNLINK_FN * pUnlink,
			UPOSIX_FTELL_FN * pFTell,
			UPOSIX_FSEEK_FN * pFSeek,
			UPOSIX_FEOF_FN * pFeof,
			UPOSIX_FERROR_FN * pFerror,
			UPOSIX_ISDIR_FN * pIsDir,
			UPOSIX_CHMOD_FN * pChMod,
			UPOSIX_GETMOD_FN * pGetMod,
			UPOSIX_GETSIZE_FN * pGetSize,
			UPOSIX_GETERRNO_FN * pGetErrno,
			UPOSIX_SETERRNO_FN * pSetErrno,
			UPOSIX_PATH_COMPARE_BUFFER_FN * pPathCompareBuffer,
			UPOSIX_PATH_COMPARE_STRING_FN * pPathCompareString,
			UPOSIX_GETMAXPATH_FN *pGetMaxPath,
			UPOSIX_FSTAT_FN * pFstat,
			UPOSIX_GETTIME_FN * pGetTime,
			UPOSIX_UTIME_FN * pUTime,
			UPOSIX_FFLUSH_FN * pFflush,
			UPOSIX_RENAME_FN * pRename,
			UPOSIX_GETTIMEOFDAY64_FN * pGettimeOfDay64,
			UPOSIX_DIR_OPEN_FN * pDirOpen,
			UPOSIX_FILE_OPEN_FN * pFileOpen,
			UPOSIX_FSETSIZE_FN * pFSetSize,
			UPOSIX_STATFS_FN * pStatfs
			);

	This macro follows the normal MCCI style, and will be
	maintained for forward-compatibility if new methods are
	added.

See Also:
	UPOSIX_INIT()

*/

struct __TMS_STRUCTNAME(UPOSIX_SWITCH)
	{
	__TMS_ULONG					MagicBegin;
	__TMS_UPOSIX_INIT_FN *				pInit;
	__TMS_UPOSIX_OPENDIR_FN *			pOpenDir;
	__TMS_UPOSIX_CLOSEDIR_FN *			pCloseDir;
	__TMS_UPOSIX_READDIR_FN *			pReadDir;
	__TMS_UPOSIX_MKDIR_FN *				pMkDir;
	__TMS_UPOSIX_RMDIR_FN *				pRmDir;
	__TMS_UPOSIX_FOPEN_FN *				pFOpen;
	__TMS_UPOSIX_FCLOSE_FN *			pFClose;
	__TMS_UPOSIX_FREAD_FN *				pFRead;
	__TMS_UPOSIX_FWRITE_FN *			pFWrite;
	__TMS_UPOSIX_FGETC_FN *				pFGetC;
	__TMS_UPOSIX_UNLINK_FN *			pUnlink;
	__TMS_UPOSIX_FTELL_FN *				pFTell;
	__TMS_UPOSIX_FSEEK_FN *				pFSeek;
	__TMS_UPOSIX_FEOF_FN *				pFeof;
	__TMS_UPOSIX_FERROR_FN *			pFerror;
	__TMS_UPOSIX_ISDIR_FN *				pIsDir;
	__TMS_UPOSIX_CHMOD_FN *				pChMod;
	__TMS_UPOSIX_GETMOD_FN *			pGetMod;
	__TMS_UPOSIX_GETSIZE_FN *			pGetSize;
	__TMS_UPOSIX_GETERRNO_FN *			pGetErrno;
	__TMS_UPOSIX_SETERRNO_FN *			pSetErrno;
	__TMS_UPOSIX_PATH_COMPARE_BUFFER_FN *		pPathCompareBuffer;
	__TMS_UPOSIX_PATH_COMPARE_STRING_FN *		pPathCompareString;
	__TMS_UPOSIX_GETMAXPATH_FN *			pGetMaxPath;
	__TMS_UPOSIX_FSTAT_FN *				pFstat;
	__TMS_UPOSIX_GETTIME_FN *			pGetTime;
	__TMS_UPOSIX_UTIME_FN * 			pUTime;
	__TMS_UPOSIX_FFLUSH_FN *			pFflush;
	__TMS_UPOSIX_RENAME_FN *			pRename;
	__TMS_UPOSIX_GETTIMEOFDAY64_FN *		pGettimeOfDay64;
	__TMS_UPOSIX_DIR_OPENDIR_FN *			pDirOpen;
	__TMS_UPOSIX_FILE_FOPEN_FN *			pFileOpen;
	__TMS_UPOSIX_FSETSIZE_FN *			pFSetSize;
	__TMS_UPOSIX_STATFS_FN *			pStatfs;
	__TMS_ULONG					MagicEnd;
	};

#define	__TMS_UPOSIX_SWITCH_BEGIN			(0x75746300 + '<')
#define	__TMS_UPOSIX_SWITCH_END				(0x36475700 + '>')


/*
|| UPOSIX_SWITCH initialization functions.  Don't change the calling sequence
|| for any macro ending in Vn; instead, create a macro ending in Vn+1, and
|| redefine the Vn macro to expand in an upward-compatible way.
*/
#define	__TMS_UPOSIX_SWITCH_INIT_V8( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64, \
		pDirOpen, \
		pFileOpen, \
		pFSetSize, \
		pStatfs \
		) \
	{ \
		__TMS_UPOSIX_SWITCH_BEGIN, \
		(pInit), \
		(pOpenDir), \
		(pCloseDir), \
		(pReadDir), \
		(pMkDir), \
		(pRmDir), \
		(pFOpen), \
		(pFClose), \
		(pFRead), \
		(pFWrite), \
		(pFGetC), \
		(pUnlink), \
		(pFTell), \
		(pFSeek), \
		(pFeof), \
		(pFerror), \
		(pIsDir), \
		(pChMod), \
		(pGetMod), \
		(pGetSize), \
		(pGetErrno), \
		(pSetErrno), \
		(pPathCompareBuffer), \
		(pPathCompareString), \
		(pGetMaxPath), \
		(pFstat), \
		(pGetTime), \
		(pUTime), \
		(pFflush), \
		(pRename), \
		(pGetTimeOfDay64), \
		(pDirOpen), \
		(pFileOpen), \
		(pFSetSize), \
		(pStatfs), \
		__TMS_UPOSIX_SWITCH_END \
	}

#define	__TMS_UPOSIX_SWITCH_INIT_V7( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64, \
		pDirOpen, \
		pFileOpen \
		) \
	 __TMS_UPOSIX_SWITCH_INIT_V8(	\
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64, \
		pDirOpen, \
		pFileOpen, \
		UsbPumpPosix_FsetsizeStub, \
		UsbPumpPosix_StatfsStub	\
		)

#define	__TMS_UPOSIX_SWITCH_INIT_V6( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64 \
		) \
	__TMS_UPOSIX_SWITCH_INIT_V7( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64, \
		UsbPumpPosix_DirOpenStub, \
		UsbPumpPosix_FileOpenStub \
		)

#define	__TMS_UPOSIX_SWITCH_INIT_V5( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename \
		) \
	__TMS_UPOSIX_SWITCH_INIT_V6( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		UsbPumpPosix_GetTimeOfDay64Stub \
		)

#define	__TMS_UPOSIX_SWITCH_INIT_V4( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush \
		) \
	__TMS_UPOSIX_SWITCH_INIT_V5( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		UsbPumpPosix_RenameStub \
		)

#define	__TMS_UPOSIX_SWITCH_INIT_V3( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime \
		) \
	__TMS_UPOSIX_SWITCH_INIT_V4( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		UsbPumpPosix_FsyncStub \
		)

#define	__TMS_UPOSIX_SWITCH_INIT_V2( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime \
		) \
	__TMS_UPOSIX_SWITCH_INIT_V3( \
		(pInit), \
		(pOpenDir), \
		(pCloseDir), \
		(pReadDir), \
		(pMkDir), \
		(pRmDir), \
		(pFOpen), \
		(pFClose), \
		(pFRead), \
		(pFWrite), \
		(pFGetC), \
		(pUnlink), \
		(pFTell), \
		(pFSeek), \
		(pFeof), \
		(pFerror), \
		(pIsDir), \
		(pChMod), \
		(pGetMod), \
		(pGetSize), \
		(pGetErrno), \
		(pSetErrno), \
		(pPathCompareBuffer), \
		(pPathCompareString), \
		(pGetMaxPath), \
		(pFstat), \
		(pGetTime), \
		UsbPumpPosix_UTimeStub \
		)

#define	__TMS_UPOSIX_SWITCH_INIT_V1( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath \
		) \
	__TMS_UPOSIX_SWITCH_INIT_V2( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		UsbPumpPosix_FstatStub, \
		UsbPumpPosix_GetTimeDefault \
		)

/****************************************************************************\
|
|	Task primitive macros
|
\****************************************************************************/

#define __TMS_UPOSIX_INIT(a_pSwitch, a_pPathCtx)	\
	(*(a_pSwitch)->pInit)(a_pSwitch, a_pPathCtx)

#define __TMS_UPOSIX_OPENDIR(a_pCtx, a_pName)	\
	(*(a_pCtx)->pSwitch->pOpenDir)(a_pCtx, a_pName)

#define __TMS_UPOSIX_CLOSEDIR(a_pCtx, a_pDir)	\
	(*(a_pCtx)->pSwitch->pCloseDir)(a_pCtx, a_pDir)

#define __TMS_UPOSIX_READDIR(a_pCtx, a_pDir)	\
	(*(a_pCtx)->pSwitch->pReadDir)(a_pCtx, a_pDir)

#define __TMS_UPOSIX_MKDIR(a_pCtx, a_pName, a_mode) \
	(*(a_pCtx)->pSwitch->pMkDir)(a_pCtx, a_pName, a_mode)

#define __TMS_UPOSIX_RMDIR(a_pCtx, a_pName) \
	(*(a_pCtx)->pSwitch->pRmDir)(a_pCtx, a_pName)

#define __TMS_UPOSIX_FOPEN(a_pCtx, a_pName, a_pMode) \
	(*(a_pCtx)->pSwitch->pFOpen)(a_pCtx, a_pName, a_pMode)

#define __TMS_UPOSIX_FCLOSE(a_pCtx, a_pStream) \
	(*(a_pCtx)->pSwitch->pFClose)(a_pCtx, a_pStream)

#define __TMS_UPOSIX_FREAD(a_pCtx, a_pBuffer, a_n1, a_n2, a_pStream) \
	(*(a_pCtx)->pSwitch->pFRead)(a_pCtx, a_pBuffer, a_n1, a_n2, a_pStream)

#define __TMS_UPOSIX_FWRITE(a_pCtx, a_pBuffer, a_n1, a_n2, a_pStream) \
	(*(a_pCtx)->pSwitch->pFWrite)(a_pCtx, a_pBuffer, a_n1, a_n2, a_pStream)

#define __TMS_UPOSIX_FGETC(a_pCtx, a_pStream)	\
	(*(a_pCtx)->pSwitch->pFGetC)(a_pCtx, a_pStream)

#define __TMS_UPOSIX_UNLINK(a_pCtx, a_pName)	\
	(*(a_pCtx)->pSwitch->pUnlink)(a_pCtx, a_pName)

#define __TMS_UPOSIX_FTELL(a_pCtx, a_pStream)	\
	(*(a_pCtx)->pSwitch->pFTell)(a_pCtx, a_pStream)

#define __TMS_UPOSIX_FSEEK(a_pCtx, a_pStream, a_Offset, a_How) \
	(*(a_pCtx)->pSwitch->pFSeek)(a_pCtx, a_pStream, a_Offset, a_How)

#define __TMS_UPOSIX_FEOF(a_pCtx, a_pStream) \
	(*(a_pCtx)->pSwitch->pFeof)(a_pCtx, a_pStream)

#define __TMS_UPOSIX_FERROR(a_pCtx, a_pStream) \
	(*(a_pCtx)->pSwitch->pFerror)(a_pCtx, a_pStream)

#define __TMS_UPOSIX_ISDIR(a_pCtx, a_pName) \
	(*(a_pCtx)->pSwitch->pIsDir)(a_pCtx, a_pName)

#define __TMS_UPOSIX_CHMOD(a_pCtx, a_pName, a_Perm) \
	(*(a_pCtx)->pSwitch->pChMod)(a_pCtx, a_pName, a_Perm)

#define __TMS_UPOSIX_GETMOD(a_pCtx, a_pName) \
	(*(a_pCtx)->pSwitch->pGetMod)(a_pCtx, a_pName)

#define __TMS_UPOSIX_GETSIZE(a_pCtx, a_pName) \
	(*(a_pCtx)->pSwitch->pGetSize)(a_pCtx, a_pName)

#define	__TMS_UPOSIX_GET_ERRNO(a_pCtx)	\
	(*(a_pCtx)->pSwitch->pGetErrno)(a_pCtx)

#define	__TMS_UPOSIX_SET_ERRNO(a_pCtx, a_errno)	\
	(*(a_pCtx)->pSwitch->pSetErrno)(a_pCtx, a_errno)

#define	__TMS_UPOSIX_PATHCOMPAREBUFFER(a_pCtx, a_pPath1, a_nPath1, a_pPath2, a_nPath2)	\
	(*(a_pCtx)->pSwitch->pPathCompareBuffer)(a_pCtx, a_pPath1, a_nPath1, a_pPath2, a_nPath2)

#define	__TMS_UPOSIX_PATHCOMPARESTRING(a_pCtx, a_pPath1, a_pPath2)	\
	(*(a_pCtx)->pSwitch->pPathCompareString)(a_pCtx, a_pPath1, a_pPath2)

#define	__TMS_UPOSIX_GETMAXPATH(a_pCtx)	\
	(*(a_pCtx)->pSwitch->pGetMaxPath)(a_pCtx)

#define __TMS_UPOSIX_FSTAT(a_pCtx, a_pPath, a_pStat) \
	(*(a_pCtx)->pSwitch->pFstat)(a_pCtx, a_pPath, a_pStat)

#define __TMS_UPOSIX_GETTIME(a_pCtx, pTimeSpec, a_pTm) \
	(*(a_pCtx)->pSwitch->pGetTime)(a_pCtx, pTimeSpec, a_pTm)

#define __TMS_UPOSIX_UTIME(a_pCtx, a_pPath, a_pUTmBuf) \
	(*(a_pCtx)->pSwitch->pUTime)(a_pCtx, a_pPath, a_pUTmBuf)

/* 3118: change name from FSYNC to FFLUSH and add synonym for the macro */
#define __TMS_UPOSIX_FFLUSH(a_pCtx, a_pStream) \
	(*(a_pCtx)->pSwitch->pFflush)(a_pCtx, a_pStream)

#define __TMS_UPOSIX_FSYNC(a_pCtx, a_pStream) \
	__TMS_UPOSIX_FFLUSH(a_pCtx, a_pStream)

#define __TMS_UPOSIX_RENAME(a_pCtx, a_pFromPath, a_ToPath) \
	(*(a_pCtx)->pSwitch->pRename)(a_pCtx, a_pFromPath, a_ToPath)

#define __TMS_UPOSIX_GETTIMEOFDAY64(a_pCtx, a_pTimeSpec64, a_pTimeSpecContext) \
	(*(a_pCtx)->pSwitch->pGettimeOfDay64)(a_pCtx, a_pTimeSpec64, a_pTimeSpecContext)

#define __TMS_UPOSIX_FSETSIZE(a_pCtx, a_pStream, a_newSize)	\
	(*(a_pCtx)->pSwitch->pFSetSize)(a_pCtx, a_pStream, a_newSize)

#define __TMS_UPOSIX_STATFS(a_pCtx, a_pPathName, a_pStatfs)	\
	(*(a_pCtx)->pSwitch->pStatfs)(a_pCtx, a_pPathName, a_pStatfs)

/*
|| UPOSIX_DIR method function
*/

#define __TMS_UPOSIX_DIR_OPENDIR(a_pCtx, a_pName)	\
	(*(a_pCtx)->pSwitch->pDirOpen)(a_pCtx, a_pName)

#define __TMS_UPOSIX_DIR_CLOSEDIR(a_pDir)		\
	(*(a_pDir)->pDirMethods->pCloseFn)(a_pDir)

#define __TMS_UPOSIX_DIR_READDIR(a_pDir)		\
	(*(a_pDir)->pDirMethods->pReadFn)(a_pDir)


/*
|| UPOSIX_FILE method function
*/

#define __TMS_UPOSIX_FILE_FOPEN(a_pCtx, a_pName, a_pMode) \
	(*(a_pCtx)->pSwitch->pFileOpen)(a_pCtx, a_pName, a_pMode)

#define __TMS_UPOSIX_FILE_FCLOSE(a_pStream)	\
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pCloseFn)(a_pStream)

#define __TMS_UPOSIX_FILE_FREAD(a_pBuffer, a_n1, a_n2, a_pStream) \
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pReadFn)(a_pBuffer, a_n1, a_n2, a_pStream)

#define __TMS_UPOSIX_FILE_FWRITE(a_pBuffer, a_n1, a_n2, a_pStream) \
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pWriteFn)(a_pBuffer, a_n1, a_n2, a_pStream)

#define __TMS_UPOSIX_FILE_FGETC(a_pStream)	\
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pGetcFn)(a_pStream)

#define __TMS_UPOSIX_FILE_FTELL(a_pStream)	\
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pTellFn)(a_pStream)

#define __TMS_UPOSIX_FILE_FSEEK(a_pStream, a_Offset, a_How) \
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pSeekFn)(a_pStream, a_Offset, a_How)

#define __TMS_UPOSIX_FILE_FEOF(a_pStream)	\
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pEofFn)(a_pStream)

#define __TMS_UPOSIX_FILE_FERROR(a_pStream)	\
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pErrorFn)(a_pStream)

#define __TMS_UPOSIX_FILE_FFLUSH(a_pStream)	\
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pFlushFn)(a_pStream)

#define __TMS_UPOSIX_FILE_FSETSIZE(a_pStream, a_newSize)	\
	(*((__TMS_UPOSIX_FILE_CONTENTS *)(a_pStream))->pFileMethods->pSetSizeFn)(a_pStream, a_newSize)

/****************************************************************************\
|
|	Function prototypes and externals.
|
\****************************************************************************/

__TMS_BEGIN_DECLS

extern __TMS_CONST __TMS_UPOSIX_SWITCH gk_UPosixSwitch;

__TMS_UPOSIX_PATH_COMPARE_BUFFER_FN	UsbPumpPosix_PathCompareBufferDefault;
__TMS_UPOSIX_PATH_COMPARE_STRING_FN	UsbPumpPosix_PathCompareStringDefault;
__TMS_UPOSIX_FSTAT_FN			UsbPumpPosix_FstatStub;
__TMS_UPOSIX_GETTIME_FN			UsbPumpPosix_GetTimeDefault;
__TMS_UPOSIX_UTIME_FN			UsbPumpPosix_UTimeStub;
__TMS_UPOSIX_FFLUSH_FN			UsbPumpPosix_FsyncStub;
__TMS_UPOSIX_RENAME_FN			UsbPumpPosix_RenameStub;
__TMS_UPOSIX_GETTIMEOFDAY64_FN		UsbPumpPosix_GetTimeOfDay64Stub;
__TMS_UPOSIX_DIR_OPENDIR_FN		UsbPumpPosix_DirOpenStub;
__TMS_UPOSIX_FILE_FOPEN_FN		UsbPumpPosix_FileOpenStub;
__TMS_UPOSIX_FSETSIZE_FN		UsbPumpPosix_FsetsizeStub;
__TMS_UPOSIX_STATFS_FN			UsbPumpPosix_StatfsStub;

__TMS_BOOL UsbPumpPosixLib_Time64ToTime32(
	__TMS_UPOSIX_CTX *			pPosixCtx,
	__TMS_CONST __TMS_UPOSIX_TIMESPEC64_T* 	pTimeSpec64,
	__TMS_UPOSIX_TIMESPEC_T *		pTimeSpec32
	);

__TMS_END_DECLS


/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPOSIX_DIRENT__HDR(nBytes)	\
   __TMS_UPOSIX_DIRENT__HDR(nBytes)
# define UPOSIX_FILE_PERM_NO_PROTECT	\
   __TMS_UPOSIX_FILE_PERM_NO_PROTECT
# define UPOSIX_FILE_PERM_READ_ONLY	\
   __TMS_UPOSIX_FILE_PERM_READ_ONLY
# define UPOSIX_FILE_PERM_NO_FILE	\
   __TMS_UPOSIX_FILE_PERM_NO_FILE
# define UPOSIX_FILE_SEEK_SET	\
   __TMS_UPOSIX_FILE_SEEK_SET
# define UPOSIX_FILE_SEEK_CUR	\
   __TMS_UPOSIX_FILE_SEEK_CUR
# define UPOSIX_FILE_SEEK_END	\
   __TMS_UPOSIX_FILE_SEEK_END
# define UPOSIX_PERMISSIONS_WIDTH	\
   __TMS_UPOSIX_PERMISSIONS_WIDTH
# define UPOSIX_PERMISSIONS_USR_SHIFT	\
   __TMS_UPOSIX_PERMISSIONS_USR_SHIFT
# define UPOSIX_PERMISSIONS_USR_MASK	\
   __TMS_UPOSIX_PERMISSIONS_USR_MASK
# define UPOSIX_PERMISSIONS_GRP_SHIFT	\
   __TMS_UPOSIX_PERMISSIONS_GRP_SHIFT
# define UPOSIX_PERMISSIONS_GRP_MASK	\
   __TMS_UPOSIX_PERMISSIONS_GRP_MASK
# define UPOSIX_PERMISSIONS_OTH_SHIFT	\
   __TMS_UPOSIX_PERMISSIONS_OTH_SHIFT
# define UPOSIX_PERMISSIONS_OTH_MASK	\
   __TMS_UPOSIX_PERMISSIONS_OTH_MASK
# define UPOSIX_PERMISSIONS_IRUSR	\
   __TMS_UPOSIX_PERMISSIONS_IRUSR
# define UPOSIX_PERMISSIONS_IWUSR	\
   __TMS_UPOSIX_PERMISSIONS_IWUSR
# define UPOSIX_PERMISSIONS_IXUSR	\
   __TMS_UPOSIX_PERMISSIONS_IXUSR
# define UPOSIX_PERMISSIONS_IRGRP	\
   __TMS_UPOSIX_PERMISSIONS_IRGRP
# define UPOSIX_PERMISSIONS_IWGRP	\
   __TMS_UPOSIX_PERMISSIONS_IWGRP
# define UPOSIX_PERMISSIONS_IXGRP	\
   __TMS_UPOSIX_PERMISSIONS_IXGRP
# define UPOSIX_PERMISSIONS_IROTH	\
   __TMS_UPOSIX_PERMISSIONS_IROTH
# define UPOSIX_PERMISSIONS_IWOTH	\
   __TMS_UPOSIX_PERMISSIONS_IWOTH
# define UPOSIX_PERMISSIONS_IXOTH	\
   __TMS_UPOSIX_PERMISSIONS_IXOTH
# define UPOSIX_PERMISSIONS_IRALL	\
   __TMS_UPOSIX_PERMISSIONS_IRALL
# define UPOSIX_PERMISSIONS_IWALL	\
   __TMS_UPOSIX_PERMISSIONS_IWALL
# define UPOSIX_PERMISSIONS_IXALL	\
   __TMS_UPOSIX_PERMISSIONS_IXALL
# define UPOSIX_STATFS_F_BSIZE	\
   __TMS_UPOSIX_STATFS_F_BSIZE
# define UPOSIX_STATFS_F_BAVAIL	\
   __TMS_UPOSIX_STATFS_F_BAVAIL
# define UPOSIX_STATFS_F_IOSIZE	\
   __TMS_UPOSIX_STATFS_F_IOSIZE
# define UPOSIX_STATFS_F_BLOCKS	\
   __TMS_UPOSIX_STATFS_F_BLOCKS
# define UPOSIX_STATFS_F_ALL	\
   __TMS_UPOSIX_STATFS_F_ALL
# define UPOSIX_STATFS_IS_F_BLOCKS_VALID(a_validflags)	\
   __TMS_UPOSIX_STATFS_IS_F_BLOCKS_VALID(a_validflags)
# define UPOSIX_STATFS_IS_F_BSIZE_VALID(a_validflags)	\
   __TMS_UPOSIX_STATFS_IS_F_BSIZE_VALID(a_validflags)
# define UPOSIX_STATFS_IS_F_BAVAIL_VALID(a_validflags)	\
   __TMS_UPOSIX_STATFS_IS_F_BAVAIL_VALID(a_validflags)
# define UPOSIX_STATFS_IS_F_IOSIZE_VALID(a_validflags)	\
   __TMS_UPOSIX_STATFS_IS_F_IOSIZE_VALID(a_validflags)
# define UPOSIX_DIR_METHODS_INIT_V1(	\
		a_pCloseFn,			\
		a_pReadFn			\
		)	\
	__TMS_UPOSIX_DIR_METHODS_INIT_V1(	\
		a_pCloseFn,			\
		a_pReadFn			\
		)
# define UPOSIX_FILE_METHODS_INIT_V2(	\
		a_pCloseFn,			\
		a_pReadFn,			\
		a_pWriteFn,			\
		a_pGetcFn,			\
		a_pTellFn,			\
		a_pSeekFn,			\
		a_pEofFn,			\
		a_pErrorFn,			\
		a_pFlushFn,			\
		a_pSetSizeFn			\
		)	\
	__TMS_UPOSIX_FILE_METHODS_INIT_V2(	\
		a_pCloseFn,			\
		a_pReadFn,			\
		a_pWriteFn,			\
		a_pGetcFn,			\
		a_pTellFn,			\
		a_pSeekFn,			\
		a_pEofFn,			\
		a_pErrorFn,			\
		a_pFlushFn,			\
		a_pSetSizeFn			\
		)
# define UPOSIX_FILE_METHODS_INIT_V1(	\
		a_pCloseFn,			\
		a_pReadFn,			\
		a_pWriteFn,			\
		a_pGetcFn,			\
		a_pTellFn,			\
		a_pSeekFn,			\
		a_pEofFn,			\
		a_pErrorFn,			\
		a_pFlushFn			\
		)	\
	__TMS_UPOSIX_FILE_METHODS_INIT_V1(	\
		a_pCloseFn,			\
		a_pReadFn,			\
		a_pWriteFn,			\
		a_pGetcFn,			\
		a_pTellFn,			\
		a_pSeekFn,			\
		a_pEofFn,			\
		a_pErrorFn,			\
		a_pFlushFn			\
		)
# define UPOSIX_ERRNO_CONTENTS_RESET(		\
		a_pCtx,					\
		a_Errno					\
		)	\
	__TMS_UPOSIX_ERRNO_CONTENTS_RESET(		\
		a_pCtx,					\
		a_Errno					\
		)
# define UPOSIX_SET_NATIVE_ERRNO(			\
		a_pCtx,					\
		a_Errno					\
		)	\
	__TMS_UPOSIX_SET_NATIVE_ERRNO(			\
		a_pCtx,					\
		a_Errno					\
		)
# define UPOSIX_SET_POSIX_ERRNO(			\
		a_pCtx,					\
		a_Errno					\
		)	\
	__TMS_UPOSIX_SET_POSIX_ERRNO(			\
		a_pCtx,					\
		a_Errno					\
		)
# define UPOSIX_GET_NATIVE_ERRNO(			\
		a_pCtx					\
		)	\
	__TMS_UPOSIX_GET_NATIVE_ERRNO(			\
		a_pCtx					\
		)
# define UPOSIX_GET_POSIX_ERRNO(			\
		a_pCtx					\
		)	\
	__TMS_UPOSIX_GET_POSIX_ERRNO(			\
		a_pCtx					\
		)
# define UPOSIX_ERRNO_VALID(			\
		a_pCtx					\
		)	\
	__TMS_UPOSIX_ERRNO_VALID(			\
		a_pCtx					\
		)
# define UPOSIX_SWITCH_BEGIN	\
   __TMS_UPOSIX_SWITCH_BEGIN
# define UPOSIX_SWITCH_END	\
   __TMS_UPOSIX_SWITCH_END
# define UPOSIX_SWITCH_INIT_V8( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64, \
		pDirOpen, \
		pFileOpen, \
		pFSetSize, \
		pStatfs \
		)	\
	__TMS_UPOSIX_SWITCH_INIT_V8( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64, \
		pDirOpen, \
		pFileOpen, \
		pFSetSize, \
		pStatfs \
		)
# define UPOSIX_SWITCH_INIT_V7( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64, \
		pDirOpen, \
		pFileOpen \
		)	\
	__TMS_UPOSIX_SWITCH_INIT_V7( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64, \
		pDirOpen, \
		pFileOpen \
		)
# define UPOSIX_SWITCH_INIT_V6( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64 \
		)	\
	__TMS_UPOSIX_SWITCH_INIT_V6( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename, \
		pGetTimeOfDay64 \
		)
# define UPOSIX_SWITCH_INIT_V5( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename \
		)	\
	__TMS_UPOSIX_SWITCH_INIT_V5( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush, \
		pRename \
		)
# define UPOSIX_SWITCH_INIT_V4( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush \
		)	\
	__TMS_UPOSIX_SWITCH_INIT_V4( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime, \
		pFflush \
		)
# define UPOSIX_SWITCH_INIT_V3( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime \
		)	\
	__TMS_UPOSIX_SWITCH_INIT_V3( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime, \
		pUTime \
		)
# define UPOSIX_SWITCH_INIT_V2( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime \
		)	\
	__TMS_UPOSIX_SWITCH_INIT_V2( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath, \
		pFstat, \
		pGetTime \
		)
# define UPOSIX_SWITCH_INIT_V1( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath \
		)	\
	__TMS_UPOSIX_SWITCH_INIT_V1( \
		pInit, \
		pOpenDir, \
		pCloseDir, \
		pReadDir, \
		pMkDir, \
		pRmDir, \
		pFOpen, \
		pFClose, \
		pFRead, \
		pFWrite, \
		pFGetC, \
		pUnlink, \
		pFTell, \
		pFSeek, \
		pFeof, \
		pFerror, \
		pIsDir, \
		pChMod, \
		pGetMod, \
		pGetSize, \
		pGetErrno, \
		pSetErrno, \
		pPathCompareBuffer, \
		pPathCompareString, \
		pGetMaxPath \
		)
# define UPOSIX_INIT(a_pSwitch, a_pPathCtx)	\
   __TMS_UPOSIX_INIT(a_pSwitch, a_pPathCtx)
# define UPOSIX_OPENDIR(a_pCtx, a_pName)	\
   __TMS_UPOSIX_OPENDIR(a_pCtx, a_pName)
# define UPOSIX_CLOSEDIR(a_pCtx, a_pDir)	\
   __TMS_UPOSIX_CLOSEDIR(a_pCtx, a_pDir)
# define UPOSIX_READDIR(a_pCtx, a_pDir)	\
   __TMS_UPOSIX_READDIR(a_pCtx, a_pDir)
# define UPOSIX_MKDIR(a_pCtx, a_pName, a_mode)	\
   __TMS_UPOSIX_MKDIR(a_pCtx, a_pName, a_mode)
# define UPOSIX_RMDIR(a_pCtx, a_pName)	\
   __TMS_UPOSIX_RMDIR(a_pCtx, a_pName)
# define UPOSIX_FOPEN(a_pCtx, a_pName, a_pMode)	\
   __TMS_UPOSIX_FOPEN(a_pCtx, a_pName, a_pMode)
# define UPOSIX_FCLOSE(a_pCtx, a_pStream)	\
   __TMS_UPOSIX_FCLOSE(a_pCtx, a_pStream)
# define UPOSIX_FREAD(a_pCtx, a_pBuffer, a_n1, a_n2, a_pStream)	\
   __TMS_UPOSIX_FREAD(a_pCtx, a_pBuffer, a_n1, a_n2, a_pStream)
# define UPOSIX_FWRITE(a_pCtx, a_pBuffer, a_n1, a_n2, a_pStream)	\
   __TMS_UPOSIX_FWRITE(a_pCtx, a_pBuffer, a_n1, a_n2, a_pStream)
# define UPOSIX_FGETC(a_pCtx, a_pStream)	\
   __TMS_UPOSIX_FGETC(a_pCtx, a_pStream)
# define UPOSIX_UNLINK(a_pCtx, a_pName)	\
   __TMS_UPOSIX_UNLINK(a_pCtx, a_pName)
# define UPOSIX_FTELL(a_pCtx, a_pStream)	\
   __TMS_UPOSIX_FTELL(a_pCtx, a_pStream)
# define UPOSIX_FSEEK(a_pCtx, a_pStream, a_Offset, a_How)	\
   __TMS_UPOSIX_FSEEK(a_pCtx, a_pStream, a_Offset, a_How)
# define UPOSIX_FEOF(a_pCtx, a_pStream)	\
   __TMS_UPOSIX_FEOF(a_pCtx, a_pStream)
# define UPOSIX_FERROR(a_pCtx, a_pStream)	\
   __TMS_UPOSIX_FERROR(a_pCtx, a_pStream)
# define UPOSIX_ISDIR(a_pCtx, a_pName)	\
   __TMS_UPOSIX_ISDIR(a_pCtx, a_pName)
# define UPOSIX_CHMOD(a_pCtx, a_pName, a_Perm)	\
   __TMS_UPOSIX_CHMOD(a_pCtx, a_pName, a_Perm)
# define UPOSIX_GETMOD(a_pCtx, a_pName)	\
   __TMS_UPOSIX_GETMOD(a_pCtx, a_pName)
# define UPOSIX_GETSIZE(a_pCtx, a_pName)	\
   __TMS_UPOSIX_GETSIZE(a_pCtx, a_pName)
# define UPOSIX_GET_ERRNO(a_pCtx)	\
   __TMS_UPOSIX_GET_ERRNO(a_pCtx)
# define UPOSIX_SET_ERRNO(a_pCtx, a_errno)	\
   __TMS_UPOSIX_SET_ERRNO(a_pCtx, a_errno)
# define UPOSIX_PATHCOMPAREBUFFER(a_pCtx, a_pPath1, a_nPath1, a_pPath2, a_nPath2)	\
   __TMS_UPOSIX_PATHCOMPAREBUFFER(a_pCtx, a_pPath1, a_nPath1, a_pPath2, a_nPath2)
# define UPOSIX_PATHCOMPARESTRING(a_pCtx, a_pPath1, a_pPath2)	\
   __TMS_UPOSIX_PATHCOMPARESTRING(a_pCtx, a_pPath1, a_pPath2)
# define UPOSIX_GETMAXPATH(a_pCtx)	\
   __TMS_UPOSIX_GETMAXPATH(a_pCtx)
# define UPOSIX_FSTAT(a_pCtx, a_pPath, a_pStat)	\
   __TMS_UPOSIX_FSTAT(a_pCtx, a_pPath, a_pStat)
# define UPOSIX_GETTIME(a_pCtx, pTimeSpec, a_pTm)	\
   __TMS_UPOSIX_GETTIME(a_pCtx, pTimeSpec, a_pTm)
# define UPOSIX_UTIME(a_pCtx, a_pPath, a_pUTmBuf)	\
   __TMS_UPOSIX_UTIME(a_pCtx, a_pPath, a_pUTmBuf)
# define UPOSIX_FFLUSH(a_pCtx, a_pStream)	\
   __TMS_UPOSIX_FFLUSH(a_pCtx, a_pStream)
# define UPOSIX_FSYNC(a_pCtx, a_pStream)	\
   __TMS_UPOSIX_FSYNC(a_pCtx, a_pStream)
# define UPOSIX_RENAME(a_pCtx, a_pFromPath, a_ToPath)	\
   __TMS_UPOSIX_RENAME(a_pCtx, a_pFromPath, a_ToPath)
# define UPOSIX_GETTIMEOFDAY64(a_pCtx, a_pTimeSpec64, a_pTimeSpecContext)	\
   __TMS_UPOSIX_GETTIMEOFDAY64(a_pCtx, a_pTimeSpec64, a_pTimeSpecContext)
# define UPOSIX_FSETSIZE(a_pCtx, a_pStream, a_newSize)	\
   __TMS_UPOSIX_FSETSIZE(a_pCtx, a_pStream, a_newSize)
# define UPOSIX_STATFS(a_pCtx, a_pPathName, a_pStatfs)	\
   __TMS_UPOSIX_STATFS(a_pCtx, a_pPathName, a_pStatfs)
# define UPOSIX_DIR_OPENDIR(a_pCtx, a_pName)	\
   __TMS_UPOSIX_DIR_OPENDIR(a_pCtx, a_pName)
# define UPOSIX_DIR_CLOSEDIR(a_pDir)	\
   __TMS_UPOSIX_DIR_CLOSEDIR(a_pDir)
# define UPOSIX_DIR_READDIR(a_pDir)	\
   __TMS_UPOSIX_DIR_READDIR(a_pDir)
# define UPOSIX_FILE_FOPEN(a_pCtx, a_pName, a_pMode)	\
   __TMS_UPOSIX_FILE_FOPEN(a_pCtx, a_pName, a_pMode)
# define UPOSIX_FILE_FCLOSE(a_pStream)	\
   __TMS_UPOSIX_FILE_FCLOSE(a_pStream)
# define UPOSIX_FILE_FREAD(a_pBuffer, a_n1, a_n2, a_pStream)	\
   __TMS_UPOSIX_FILE_FREAD(a_pBuffer, a_n1, a_n2, a_pStream)
# define UPOSIX_FILE_FWRITE(a_pBuffer, a_n1, a_n2, a_pStream)	\
   __TMS_UPOSIX_FILE_FWRITE(a_pBuffer, a_n1, a_n2, a_pStream)
# define UPOSIX_FILE_FGETC(a_pStream)	\
   __TMS_UPOSIX_FILE_FGETC(a_pStream)
# define UPOSIX_FILE_FTELL(a_pStream)	\
   __TMS_UPOSIX_FILE_FTELL(a_pStream)
# define UPOSIX_FILE_FSEEK(a_pStream, a_Offset, a_How)	\
   __TMS_UPOSIX_FILE_FSEEK(a_pStream, a_Offset, a_How)
# define UPOSIX_FILE_FEOF(a_pStream)	\
   __TMS_UPOSIX_FILE_FEOF(a_pStream)
# define UPOSIX_FILE_FERROR(a_pStream)	\
   __TMS_UPOSIX_FILE_FERROR(a_pStream)
# define UPOSIX_FILE_FFLUSH(a_pStream)	\
   __TMS_UPOSIX_FILE_FFLUSH(a_pStream)
# define UPOSIX_FILE_FSETSIZE(a_pStream, a_newSize)	\
   __TMS_UPOSIX_FILE_FSETSIZE(a_pStream, a_newSize)
#endif /* !__TMS_CLOAKED_NAMES_ONLY */


/**** end of uposix.h ****/
#endif /* _UPOSIX_H_ */
