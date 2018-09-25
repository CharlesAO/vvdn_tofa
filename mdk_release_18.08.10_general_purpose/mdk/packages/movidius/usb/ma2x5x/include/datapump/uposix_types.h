/* uposix_types.h	Mon Apr 23 2012 15:35:48 yssaroha */

/*

Module:  usposix_types.h

Function:
	Define the basic type tags for MCCI Task API -- useful when interfacing
	to other code subsystems and the full "uposix.h" cannot be included.

Version:
	V3.01f	Mon Apr 23 2012 15:35:48 yssaroha	Edit level 21

Copyright notice:
	This file copyright (C) 2005-2009, 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Fredrik Norrby, MCCI Corporation	February 2005

Revision history:
   1.91b  Thu Apr 28 2005 11:35:09  fny
	Module created.

   1.91d  Tue Jun 14 2005 08:20:19  maw
   	Removed memory functions. Use UTASK vesions
   	Changed filename type to UTF8

   1.91f  2005-07-09  tmm
	Get rid of embedded \xB4 characters for portability.

   1.91h  Sun Sep 04 2005 17:35:01 maw
   	Added pathlib context pointer to init fn

   1.91h  Thu Sep 08 2005 20:38:04 maw
   	Added errno and ferror fn's

   1.91h  Sat Sep 10 2005 03:16:07  maw
   	Added config structure typedef

   1.91h  Fri Sep 09 2005 23:27:43  chwon
	Change UTASK_UTF8 into USBPUMP_UTF8_TEXT

   1.91i  Mon Sep 19 2005 19:26:20  maw
   	Updated comment on GETSIZE, moved UTASK_HDR from utask.h

   1.91i  Tue Sep 27 2005 08:22:05  maw
   	Update parameter to UPOSIX_INIT_FN,
   	add UPOSIX_GETMAXPATH_FN

   1.97b  Wed May 03 2006 11:59:13  chwon
	Added UPOSIX_FSTAT and UPOSIX_GETTIME API and related structure

   1.97e  Fri Aug 18 2006 13:36:05 yssaroha
	Added UPOSIX_UTIME API and related structure.
	While solving PR 1915

   1.97f  Tue Feb 06 2007 16:16:44  chwon
	2342: Add UPOSIX context pointer parameter in UPOSIX_GETMAXPATH_FN()

   1.97h  Mon Mar 12 2007 11:31:51  chwon
	2498: added UPOSIX_FSYNC API

   1.97h  Fri Mar 23 2007 11:11:43  chwon
	2578: added UPOSIX_RENAME API

   1.97j  Thu Dec 13 2007 11:59:25  chwon
	3118: Change name from UPOSIX_FSYNC to UPOSIX_FFLUSH

   1.97k  Fri Aug 29 2008 14:53:29  yssaroha
	6423: Added macro UPOSIX_GETTIMEOFDAY64 and
	UPOSIX_TIMESPEC64_T structure
	- Added missing functional description comments

   2.01a  Thu Mar 19 2009 09:35:19  chwon
	6367:  Added UPOSIX_DIR_CONTENTS/METHODS, UPOSIX_FILE_CONTENTS/METHODS
	types. Added prototype of methods of UPOSIX_FILE and UPOSIX_DIR.

   2.01a  Thu Apr 16 2009 15:00:20  lin
	7794: Added UPOSIX_SETSIZE, UPOSIX_STATFS.

   2.01a  Wed May 13 2009 14:23:18  lin
	7794: Added UPOSIX_FSETSIZE

   2.01a  Tue Oct 20 2009 08:48:30  lin
	7838: Added UPOSIX_ERRNO_CONTENTS

   3.01f  Mon Apr 23 2012 15:35:48  yssaroha
	15220: Updated UPOSIX_FOFFSET_T type from INT32 bit to INT64 bit for
	supporting files greater than 4 GB

*/

#ifndef _UPOSIX_TYPES_H_		/* prevent multiple includes */
#define _UPOSIX_TYPES_H_

#ifndef _DEF_CDEFS_H_
# include "def/cdefs.h"
#endif

#ifndef _BASEHILTYPES_H_
# include "basehiltypes.h"
#endif

#ifndef _UTASK_TYPES_H_
# include "utask_types.h"
#endif

#ifndef _USBPUMP_PATHLIB_H_
# include "usbpump_pathlib.h"
#endif

/****************************************************************************\
|
|	Useful macros
|
\****************************************************************************/



/****************************************************************************\
|
|	Forward types
|
\****************************************************************************/

__TMS_TYPE_DEF_ARG(UPOSIX_ERRNO_T, UINT32);
__TMS_TYPE_DEF_ARG(UPOSIX_FOFFSET_T, INT64);
__TMS_TYPE_DEF_ARG(UPOSIX_FBLOCKS_T, UINT64);
__TMS_TYPE_DEF_ARG(UPOSIX_FSSIZE_T, UINT64);

__TMS_TYPE_DEF_ARG(UPOSIX_TIME64_T, UINT64);
__TMS_TYPE_DEF_ARG(UPOSIX_TIME_T, UINT32);
__TMS_TYPE_DEF_ARG(UPOSIX_SNSECONDS_T, INT32);
__TMS_TYPE_DEF_ARG(UPOSIX_NSECONDS_T, UINT32);
__TMS_TYPE_DEF_ARG(UPOSIX_SNSECONDS64_T, INT64);
__TMS_TYPE_DEF_ARG(UPOSIX_NSECONDS64_T, UINT64);

__TMS_TYPE_DEF_STRUCT(UPOSIX_DIRENT);

__TMS_TYPE_DEF(UPOSIX_DIR, __TMS_VOID);
__TMS_TYPE_DEF(UPOSIX_FILE, __TMS_VOID);

__TMS_TYPE_DEF_STRUCT(UPOSIX_DIR_CONTENTS);
__TMS_TYPE_DEF_STRUCT(UPOSIX_DIR_METHODS);

__TMS_TYPE_DEF_STRUCT(UPOSIX_FILE_CONTENTS);
__TMS_TYPE_DEF_STRUCT(UPOSIX_FILE_METHODS);

__TMS_TYPE_DEF_STRUCT(UPOSIX_ERRNO_CONTENTS);

__TMS_TYPE_DEF_ARG(UPOSIX_FILE_PERM, INT32);
__TMS_TYPE_DEF_ARG(UPOSIX_FILE_SEEK, BYTES);
__TMS_TYPE_DEF_ARG(UPOSIX_PERMISSIONS, BYTES);

__TMS_TYPE_DEF_STRUCT(UPOSIX_SWITCH);
__TMS_TYPE_DEF_STRUCT(UPOSIX_CTX);

__TMS_TYPE_DEF_STRUCT(UPOSIX_STAT_T);
__TMS_TYPE_DEF_STRUCT(UPOSIX_TIMESPEC_T);
__TMS_TYPE_DEF_STRUCT(UPOSIX_TM_T);
__TMS_TYPE_DEF_STRUCT(UPOSIX_UTIMEBUF);
__TMS_TYPE_DEF_STRUCT(UPOSIX_TIMESPEC64_T);
__TMS_TYPE_DEF_STRUCT(UPOSIX_STATFS_T);

/****************************************************************************\
|
|	Posix init functions
|
\****************************************************************************/

/*

Name:	UPOSIX_INIT()

Index:	Type:	UPOSIX_INIT_FN
	Type:	PUPOSIX_INIT_FN

Function:
	Initializes the abstract POSIX-like API.

Definition:
	typedef UPOSIX_CTX * UPOSIX_INIT_FN(
		CONST UPOSIX_SWITCH * 	pMethodFunctions,
		UTASK_HANDLE		hTask
		);

	typedef UPOSIX_INIT_FN * PUPOSIX_INIT_FN;

	UPOSIX_CTX * UPOSIX_INIT(
		CONST UPOSIX_SWITCH * 	pMethodFunctions,
		UTASK_HANDLE		hTask
		);

Description:
	This function allocates and initializes a UPOSIX_CTX object,
	which is used to control the interface between the DataPump
	tasks and the Posix system.  It takes (as input) a pointer to
	a constant UPOSIX_SWITCH structure where the pSwitch element
	provides the method functions; and a handle to the calling task

Returns:
	Pointer to the UPOSIX_CTX object, or NULL if an error occurs.

Notes:
	This function should be called once per task.

	UPOSIX_INIT() is a macro, so parameters may be evaluated multiple
	times.

	The POSIX subsystem is *not* required by the device or host DataPump.
	It is only needed by DataPump applications such as PictBridge.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_INIT_FN,
	__TMS_UPOSIX_CTX *, (
		__TMS_CONST __TMS_UPOSIX_SWITCH *,
		__TMS_UTASK_HANDLE
		)
	);

/****************************************************************************\
|
|	Posix file system functions
|
\****************************************************************************/

/*

Name:	UPOSIX_OPENDIR()

Index:	Type:	UPOSIX_OPENDIR_FN
	Type:	PUPOSIX_OPENDIR_FN
	Type:	UPOSIX_DIR
	Type	PUPOSIX_DIR

Function:
	Open a directory (in an abstract way)

Definition:
	typedef struct __TMS_UPOSIX_DIR UPOSIX_DIR, *PUPOSIX_DIR;

	UPOSIX_DIR *UPOSIX_OPENDIR(
		UPOSIX_CTX *pPosixpContext,
		CONST USBPUMP_UTF8_TEXT *pDirectoryName
		);

	typedef UPOSIX_DIR *UPOSIX_OPENDIR_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pDirectoryName
		);

	typedef UPOSIX_DIR *PUPOSIX_DIR:

Description:
	This function opens a directory in whatever way makes sense to
	the system, and returns a handle which can be used for subsequent
	directory operations.

	pPosixContext is a pointer to the context object.

	pDirectoryName is a pointer to the directory name, which is UTF-8.
	Note that USBPUMP_UTF8_TEXT is compatible with TEXT, so there's no need
	(or benefit, or under MCCI's style rules, allowance) for an explicit
	cast.

Returns:
	A pointer to a UPOSIX_DIR, which can be used in subsequent
	directory operations to scan the directory.

Notes:
	The contents of UPOSIX_DIR are opaque to implementors.

	UPOSIX_OPENDIR() is a macro; the pPosixContext parameter may be
	evaluated more than once.

See Also:
	UPOSIX_CLOSEDIR(), UPOSIX_READDIR()

*/

__TMS_FNTYPE_DEF(
	UPOSIX_OPENDIR_FN,
	__TMS_UPOSIX_DIR *, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_CLOSEDIR()

Index:	Type:	UPOSIX_CLOSEDIR_FN
	Type:	PUPOSIX_CLOSEDIR_FN

Function:
	Close a directory handle opened with UPOSIX_OPENDIR()

Definition:
	INT UPOSIX_CLOSEDIR(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_DIR *pDir
		);

	typedef INT UPOSIX_CLOSEDIR_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_DIR *pDir
		);

	typedef UPOSIX_CLOSEDIR_FN *PUPOSIX_CLOSEDIR_FN;

Description:
	The directory object given by pDir, which must have been
	returned by a previous call to UPOSIX_OPENDIR(), is freed.

	After calling this function, the caller must treat any pointers
	to the object as invalid.

Returns:
	0 for success; -1 for any failure.

Notes:
	UPOSIX_CLOSEDIR() is a macro; the pPosixContext parameter may be
	evaluated more than once.
*/

__TMS_FNTYPE_DEF(
	UPOSIX_CLOSEDIR_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_DIR *
		)
	);

/*

Name:	UPOSIX_READDIR()

Index:	Type:	UPOSIX_READDIR_FN
	Type:	PUPOSIX_READDIR_FN

Function:
	Reads the next entry from an open directory.

Definition:
	UPOSIX_DIRENT *UPOSIX_READDIR(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_DIR *pDir
		);

	typedef UPOSIX_DIRENT *UPOSIX_READDIR_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_DIR *pDir
		);

	typedef UPOSIX_READDIR_FN *PUPOSIX_READDIR_FN;

Description:
	UPOSIX_READDIR() returns a pointer to a UPOSIX_DIRENT structure,
	which is valid until the next call to UPOSIX_READDIR() for this
	directory or the next call to UPOSIX_CLOSEDIR() for this directory.
	The UPOSIX_DIRENT contains the name of the file.

	UPOSIX_READDIR_FN is the function signature for methods that implement
	UPOSIX_READDIR(); and PUPOSIX_READDIR_FN is the corresponding
	function pointer type.

Returns:
	A pointer to a UPOSIX_DIRENT structure, or NULL if the end of file
	has been reached for the directory or if some error has occured.

See Also:
	UPOSIX_OPENDIR(), UPOSIX_DIRENT

Notes:
	UPOSIX_READDIR() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_READDIR_FN,
	__TMS_UPOSIX_DIRENT *, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_DIR *
		)
	);

/*

Name:	UPOSIX_MKDIR()

Index:	Type:	UPOSIX_MKDIR_FN
	Type:	PUPOSIX_MKDIR_FN

Function:
	Make a directory

Definition:
	INT UPOSIX_MKDIR(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName,
		UPOSIX_PERMISSIONS Permissions
		);

	typedef INT UPOSIX_MKDIR_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName,
		UPOSIX_PERMISSIONS Permissions
		);

	typedef UPOSIX_MKDIR_FN *PUPOSIX_MKDIR_FN;

Description:
	The UPOSIX_MKDIR() function shall create a new directory with name
	pPathName.  The file permission bits of the new directory shall be
	initialized from Permissions.

	UPOSIX_MKDIR_FN is the function signature for methods that implement
	UPOSIX_MKDIR(); and PUPOSIX_MKDIR_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_MKDIR() shall return 0.
	Otherwise, -1 shall be returned, no directory shall be created,
	and error code shall be set to indicate the error.  This error code
	shall be retrieved by calling UPOSIX_GETERRNO().

See Also:
	UPOSIX_RMDIR(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_MKDIR() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_MKDIR_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *,
		__TMS_UPOSIX_PERMISSIONS
		)
	);

/*

Name:	UPOSIX_RMDIR()

Index:	Type:	UPOSIX_RMDIR_FN
	Type:	PUPOSIX_RMDIR_FN

Function:
	Remove a directory

Definition:
	INT UPOSIX_RMDIR(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName
		);

	typedef INT UPOSIX_RMDIR_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName
		);

	typedef UPOSIX_RMDIR_FN *PUPOSIX_RMDIR_FN;

Description:
	The UPOSIX_RMDIR() function shall remove a directory whose name is
	given by pPathName.  The directory shall be removed only if it is an
	empty directory.

	UPOSIX_RMDIR_FN is the function signature for methods that implement
	UPOSIX_RMDIR(); and PUPOSIX_RMDIR_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_RMDIR() shall return 0.
	Otherwise, -1 shall be returned, the named directory shall not be
	changed.

See Also:
	UPOSIX_MKDIR(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_RMDIR() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_RMDIR_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_FOPEN()

Index:	Type:	UPOSIX_FOPEN_FN
	Type:	PUPOSIX_FOPEN_FN

Function:
	Open a file stream

Definition:
	UPOSIX_FILE *UPOSIX_FOPEN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pFileName,
		CONST USBPUMP_UTF8_TEXT *pOpenMode
		);

	typedef UPOSIX_FILE *UPOSIX_FOPEN_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pFileName,
		CONST USBPUMP_UTF8_TEXT *pOpenMode
		);

	typedef UPOSIX_FOPEN_FN *PUPOSIX_FOPEN_FN;

Description:
	The UPOSIX_FOPEN() shall open the file whose pathname is the string
	pointed to by pFileName, and associates a stream with it.

	The pOpenMode argument points to a string.  If the string is one of
	the following, the file shall be opened in the indicated mode.
	Otherwise, the behavior is undefined.

	  r or rb
		Open file for reading.
	  w or wb
		Truncate to zero length or create file for writing.
	  a or ab
		Append; open or create file for writing at end-of-file.
	  r+ or rb+ or r+b
		Open file for update (reading and writing).
	  w+ or wb+ or w+b
		Truncate to zero length or create file for update.
	  a+ or ab+ or a+b
		Append; open or create file for update, writing at end-of-file.

	  T
		Temporary file; this can be used any combination of above.

	UPOSIX_FOPEN_FN is the function signature for methods that implement
	UPOSIX_FOPEN(); and PUPOSIX_FOPEN_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FOPEN() shall return a pointer to
	the object controlling the stream.  Otherwise, a null pointer shall be
	returned, and error code shall be set to indicate the error.

See Also:
	UPOSIX_FCLOSE(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FOPEN() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FOPEN_FN,
	__TMS_UPOSIX_FILE *, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_FCLOSE()

Index:	Type:	UPOSIX_FCLOSE_FN
	Type:	PUPOSIX_FCLOSE_FN

Function:
	Close a file stream

Definition:
	INT UPOSIX_FCLOSE(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef INT UPOSIX_FCLOSE_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FCLOSE_FN *PUPOSIX_FCLOSE_FN;

Description:
	The UPOSIX_FCLOSE() shall cause the stream pointed to by stream to be
	flushed and the associated file to be closed.  Any unwritten buffered
	data for the stream shall be written to the file; any unread buffered
	data shall be discarded.

	After the call to UPOSIX_FCLOSE(), any use of stream results in
	undefined behavior.

	UPOSIX_FCLOSE_FN is the function signature for methods that implement
	UPOSIX_FCLOSE(); and PUPOSIX_FCLOSE_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FCLOSE() shall return 0.
	Otherwise, it shall return non-zero, and error code shall be set to
	indicate the error.

See Also:
	UPOSIX_FOPEN(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FCLOSE() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FCLOSE_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FREAD()

Index:	Type:	UPOSIX_FREAD_FN
	Type:	PUPOSIX_FREAD_FN

Function:
	Read block of data from a file stream

Definition:
	SIZE_T UPOSIX_FREAD(
		UPOSIX_CTX *pPosixContext,
		VOID *pBuffer,
		SIZE_T SizeOfBlock,
		SIZE_T NumberOfBlock,
		UPOSIX_FILE *pFileStream
		);

	typedef SIZE_T UPOSIX_FREAD_FN(
		UPOSIX_CTX *pPosixContext,
		VOID *pBuffer,
		SIZE_T SizeOfBlock,
		SIZE_T NumberOfBlock,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FREAD_FN *PUPOSIX_FREAD_FN;

Description:
	The UPOSIX_FREAD() shall read into the array pointed to by pBuffer up
	to NumberOfBlock elements whose size is specified by SizeOfBlock in
	bytes, from the stream pointed to by pFileStream.

	UPOSIX_FREAD_FN is the function signature for methods that implement
	UPOSIX_FREAD(); and PUPOSIX_FREAD_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FREAD() shall return the number of
	elements successfully read which is less than NumberOfBlock only if a
	read error or end-of-file is encountered.  If SizeOfBlock or
	NumberOfBlock is 0, UPOSIX_FREAD() shall return 0 and the contents of
	the array and the state of the stream remain unchanged.
	Otherwise, if a read error occurs, the error indicator for the stream
	shall be set, and error code shall be set to indicate the error.

See Also:
	UPOSIX_FWRITE(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FREAD() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FREAD_FN,
	__TMS_SIZE_T, (
		__TMS_UPOSIX_CTX *,
		__TMS_VOID *,
		__TMS_SIZE_T,
		__TMS_SIZE_T,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FWRITE()

Index:	Type:	UPOSIX_FWRITE_FN
	Type:	PUPOSIX_FWRITE_FN

Function:
	Write block of data from a file stream

Definition:
	SIZE_T UPOSIX_FWRITE(
		UPOSIX_CTX *pPosixContext,
		CONST VOID *pBuffer,
		SIZE_T SizeOfBlock,
		SIZE_T NumberOfBlock,
		UPOSIX_FILE *pFileStream
		);

	typedef SIZE_T UPOSIX_FWRITE_FN(
		UPOSIX_CTX *pPosixContext,
		CONST VOID *pBuffer,
		SIZE_T SizeOfBlock,
		SIZE_T NumberOfBlock,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FWRITE_FN *PUPOSIX_FWRITE_FN;

Description:
	The UPOSIX_FWRITE() shall write, from the array pointed to by pBuffer,
	up to NumberOfBlock elements whose size is specified by SizeOfBlock,
	to the stream pointed to by pFileStream.

	UPOSIX_FWRITE_FN is the function signature for methods that implement
	UPOSIX_FWRITE(); and PUPOSIX_FWRITE_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FWRITE() shall return the number of
	elements successfully written, which is less than NumberOfBlock if a
	write error is encountered.  If SizeOfBlock or NumberOfBlock is 0,
	UPOSIX_FWRITE() shall return 0 and the state of the stream remain
	unchanged.
	Otherwise, if a write error occurs, the error indicator for the stream
	shall be set, and error code shall be set to indicate the error.

See Also:
	UPOSIX_FREAD(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FWRITE() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FWRITE_FN,
	__TMS_SIZE_T, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_VOID *,
		__TMS_SIZE_T,
		__TMS_SIZE_T,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FGETC()

Index:	Type:	UPOSIX_FGETC_FN
	Type:	PUPOSIX_FGETC_FN

Function:
	Get a character from a file stream

Definition:
	INT UPOSIX_FGETC(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef INT UPOSIX_FGETC_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FGETC_FN *PUPOSIX_FGETC_FN;

Description:
	If the end-of-file indicator for the input stream pointed to by
	pFileStream is not set and a next byte is present, the UPOSIX_FGETC()
	function shall obtain the next byte as an unsigned char converted to
	an int, from the input stream pointed to by stream, and advance the
	associated file position indicator for the stream (if defined).

	UPOSIX_FGETC_FN is the function signature for methods that implement
	UPOSIX_FGETC(); and PUPOSIX_FGETC_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FGETC() shall return the next byte
	from the input stream pointed to by stream.  If the end-of-file
	indicator for the stream is set, or if the stream is at end-of-file,
	the end-of-file indicator for the stream shall be set and UPOSIX_FGETC()
	shall return -1.  If a read error occurs, the error indicator for the
	stream shall be set, UPOSIX_FGETC() shall return -1, and error code
	shall be set to indicate the error.

See Also:
	UPOSIX_FREAD(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FGETC() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FGETC_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_UNLINK()

Index:	Type:	UPOSIX_UNLINK_FN
	Type:	PUPOSIX_UNLINK_FN

Function:
	Remove a directory entry

Definition:
	INT UPOSIX_UNLINK(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pFileName
		);

	typedef INT UPOSIX_UNLINK_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pFileName
		);

	typedef UPOSIX_UNLINK_FN *PUPOSIX_UNLINK_FN;

Description:
	The UPOSIX_UNLINK() function shall remove a link to a file.  When the
	file's link count becomes 0 and no process has the file open, the space
	occupied by the file shall be freed and the file shall no longer be
	accessible.

	UPOSIX_UNLINK_FN is the function signature for methods that implement
	UPOSIX_UNLINK(); and PUPOSIX_UNLINK_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, 0 shall be returned.  Otherwise, -1 shall
	be returned and error code shall be set to indicate the error.  If -1
	is returned, the named file shall not be changed.

See Also:
	UPOSIX_RMDIR(), UPOSIX_CLOSE(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_UNLINK() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_UNLINK_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_FTELL()

Index:	Type:	UPOSIX_FTELL_FN
	Type:	PUPOSIX_FTELL_FN

Function:
	Returns the current file position of the stream

Definition:
	UPOSIX_FOFFSET_T UPOSIX_FTELL(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FOFFSET_T UPOSIX_FTELL_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FTELL_FN *PUPOSIX_FTELL_FN;

Description:
	The UPOSIX_FTELL() function obtain the current value of the
	file-position indicator for the stream pointed to by pFileStream.

	UPOSIX_FTELL_FN is the function signature for methods that implement
	UPOSIX_FTELL(); and PUPOSIX_FTELL_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FTELL() shall return the current
	value of the file-position indicator for the stream measured in bytes
	from the beginning of the file.
	Otherwise, UPOSIX_FTELL() shall return -1 and error code shall be set
	to indicate the error.

See Also:
	UPOSIX_FSEEK(), UPOSIX_FEOF(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FTELL() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FTELL_FN,
	__TMS_UPOSIX_FOFFSET_T, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FSEEK()

Index:	Type:	UPOSIX_FSEEK_FN
	Type:	PUPOSIX_FSEEK_FN

Function:
	Reposition a file-position indicator in a stream

Definition:
	INT UPOSIX_FSEEK(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream,
		UPOSIX_FOFFSET_T Offset,
		UPOSIX_FILE_SEEK Whence
		);

	typedef INT UPOSIX_FSEEK_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream,
		UPOSIX_FOFFSET_T Offset,
		UPOSIX_FILE_SEEK Whence
		);

	typedef UPOSIX_FSEEK_FN *PUPOSIX_FSEEK_FN;

Description:
	The UPOSIX_FSEEK() function shall set the file-position indicator for
	the stream pointed to by pFileStream.

	The new position, measured in bytes from the beginning of the file,
	shall be obtained by adding offset to the position specified by Whence.
	The specified point is the beginning of the file for
	UPOSIX_FILE_SEEK_SET, the current value of the file-position indicator
	for UPOSIX_FILE_SEEK_CUR, or end-of-file for UPOSIX_FILE_SEEK_END.

	UPOSIX_FSEEK_FN is the function signature for methods that implement
	UPOSIX_FSEEK(); and PUPOSIX_FSEEK_FN is the corresponding
	function pointer type.

Returns:
	The UPOSIX_FSEEK() function shall return 0 if they succeed.
	Otherwise, UPOSIX_FSEEK() shall return -1 and error code shall be set
	to indicate the error.

See Also:
	UPOSIX_FTELL(), UPOSIX_FEOF(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FSEEK() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FSEEK_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_FILE *,
		__TMS_UPOSIX_FOFFSET_T,
		__TMS_UPOSIX_FILE_SEEK
		)
	);

/*

Name:	UPOSIX_FEOF()

Index:	Type:	UPOSIX_FEOF_FN
	Type:	PUPOSIX_FEOF_FN

Function:
	Test end-of-file indicator on a stream

Definition:
	INT UPOSIX_FEOF(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef INT UPOSIX_FEOF_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FEOF_FN *PUPOSIX_FEOF_FN;

Description:
	The UPOSIX_FEOF() function shall test the end-of-file indicator for
	the stream pointed to by pFileStream.

	UPOSIX_FEOF_FN is the function signature for methods that implement
	UPOSIX_FEOF(); and PUPOSIX_FEOF_FN is the corresponding
	function pointer type.

Returns:
	The UPOSIX_FEOF() function shall return non-zero if and only if the
	end-of-file indicator is set for pFileStream.

See Also:
	UPOSIX_FTELL(), UPOSIX_FSEEK(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FEOF() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FEOF_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FERROR()

Index:	Type:	UPOSIX_FERROR_FN
	Type:	PUPOSIX_FERROR_FN

Function:
	Test error indicator on a stream

Definition:
	INT UPOSIX_FERROR(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef INT UPOSIX_FERROR_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FERROR_FN *PUPOSIX_FERROR_FN;

Description:
	The UPOSIX_FERROR() function shall test the error indicator for
	the stream pointed to by pFileStream.

	UPOSIX_FERROR_FN is the function signature for methods that implement
	UPOSIX_FERROR(); and PUPOSIX_FERROR_FN is the corresponding
	function pointer type.

Returns:
	The UPOSIX_FERROR() function shall return non-zero if and only if the
	error indicator is set for pFileStream.

See Also:
	UPOSIX_GETERRNO()

Notes:
	UPOSIX_FERROR() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FERROR_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_ISDIR()

Index:	Type:	UPOSIX_ISDIR_FN
	Type:	PUPOSIX_ISDIR_FN

Function:
	Test directory or file for given path name

Definition:
	INT UPOSIX_ISDIR(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName
		);

	typedef INT UPOSIX_ISDIR_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName
		);

	typedef UPOSIX_ISDIR_FN *PUPOSIX_ISDIR_FN;

Description:
	The UPOSIX_ISDIR() function shall test the error indicator for
	the stream pointed to by pFileStream.

	UPOSIX_ISDIR_FN is the function signature for methods that implement
	UPOSIX_ISDIR(); and PUPOSIX_ISDIR_FN is the corresponding
	function pointer type.

Returns:
	The UPOSIX_ISDIR() function shall return zero if gevin path name is
	a file and it shall return non-zero if given path name is a directory,
	and it shall return -1 if given path name can't be found.

See Also:
	UPOSIX_GETERRNO()

Notes:
	UPOSIX_ISDIR() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_ISDIR_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_CHMOD()

Index:	Type:	UPOSIX_CHMOD_FN
	Type:	PUPOSIX_CHMOD_FN

Function:
	Change permission mode of a file

Definition:
	INT UPOSIX_CHMOD(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName,
		UPOSIX_FILE_PERM Permission
		);

	typedef INT UPOSIX_CHMOD_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName,
		UPOSIX_FILE_PERM Permission
		);

	typedef UPOSIX_CHMOD_FN *PUPOSIX_CHMOD_FN;

Description:
	The UPOSIX_CHMOD() function shall change the file permission bits of
	the file named by the pPathName pointed to by the path argument to
	the corresponding bits in the Permission argument.

	UPOSIX_CHMOD_FN is the function signature for methods that implement
	UPOSIX_CHMOD(); and PUPOSIX_CHMOD_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, 0 shall be returned; otherwise, -1 shall
	be returned and error code shall be set to indicate the error.
	If -1 is returned, no change to the file mode occurs.

See Also:
	UPOSIX_GETMOD()

Notes:
	UPOSIX_CHMOD() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_CHMOD_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *,
		__TMS_UPOSIX_FILE_PERM
		)
	);

/*

Name:	UPOSIX_GETMOD()

Index:	Type:	UPOSIX_GETMOD_FN
	Type:	PUPOSIX_GETMOD_FN

Function:
	Get permission mode of a file

Definition:
	UPOSIX_FILE_PERM UPOSIX_GETMOD(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName
		);

	typedef UPOSIX_FILE_PERM UPOSIX_GETMOD_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName
		);

	typedef UPOSIX_GETMOD_FN *PUPOSIX_GETMOD_FN;

Description:
	The UPOSIX_GETMOD() function shall read the file permission bits of
	the file named by the pPathName.

	UPOSIX_GETMOD_FN is the function signature for methods that implement
	UPOSIX_GETMOD(); and PUPOSIX_GETMOD_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, file permission enum shall be returned;
	otherwise, UPOSIX_FILE_PERM_NO_FILE shall be returned.

See Also:
	UPOSIX_CHMOD()

Notes:
	UPOSIX_GETMOD() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_GETMOD_FN,
	__TMS_UPOSIX_FILE_PERM, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_GETSIZE()

Index:	Type:	UPOSIX_GETSIZE_FN
	Type:	PUPOSIX_GETSIZE_FN

Function:
	Get size of a file

Definition:
	UPOSIX_FOFFSET_T UPOSIX_GETSIZE(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName
		);

	typedef UPOSIX_FOFFSET_T UPOSIX_GETSIZE_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pPathName
		);

	typedef UPOSIX_GETSIZE_FN *PUPOSIX_GETSIZE_FN;

Description:
	The UPOSIX_GETSIZE() function shall return the number of bytes in
	the file named by the pPathName.

	UPOSIX_GETSIZE_FN is the function signature for methods that implement
	UPOSIX_GETSIZE(); and PUPOSIX_GETSIZE_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, number of bytes in the file shall be
	returned; otherwise, -1 shall be returned.

Notes:
	UPOSIX_GETSIZE() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_GETSIZE_FN,
	__TMS_UPOSIX_FOFFSET_T, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_GETERRNO()

Index:	Type:	UPOSIX_GETERRNO_FN
	Type:	PUPOSIX_GETERRNO_FN

Function:
	Get error code

Definition:
	UPOSIX_ERRNO_T UPOSIX_GETERRNO(
		UPOSIX_CTX *pPosixContext
		);

	typedef UPOSIX_ERRNO_T UPOSIX_GETERRNO_FN(
		UPOSIX_CTX *pPosixContext
		);

	typedef UPOSIX_GETERRNO_FN *PUPOSIX_GETERRNO_FN;

Description:
	The UPOSIX_GETERRNO() function shall return the error code that shall
	be set to indicate the error.

	UPOSIX_GETERRNO_FN is the function signature for methods that implement
	UPOSIX_GETERRNO(); and PUPOSIX_GETERRNO_FN is the corresponding
	function pointer type.

Returns:
	Returns current error code.

See Also:
	UPOSIX_SETERRNO()

Notes:
	UPOSIX_GETERRNO() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_GETERRNO_FN,
	__TMS_UPOSIX_ERRNO_T, (
		__TMS_UPOSIX_CTX *
		)
	);

/*

Name:	UPOSIX_SETERRNO()

Index:	Type:	UPOSIX_SETERRNO_FN
	Type:	PUPOSIX_SETERRNO_FN

Function:
	Set error code

Definition:
	VOID UPOSIX_SETERRNO(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_ERRNO_T NewErrorCode
		);

	typedef VOID UPOSIX_SETERRNO_FN(
		UPOSIX_CTX *pPosixContext,
		UPOSIX_ERRNO_T NewErrorCode
		);

	typedef UPOSIX_SETERRNO_FN *PUPOSIX_SETERRNO_FN;

Description:
	The UPOSIX_SETERRNO() function shall set the error code.

	UPOSIX_SETERRNO_FN is the function signature for methods that implement
	UPOSIX_SETERRNO(); and PUPOSIX_SETERRNO_FN is the corresponding
	function pointer type.

Returns:
	Nothing.

See Also:
	UPOSIX_GETERRNO()

Notes:
	UPOSIX_SETERRNO() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_SETERRNO_FN,
	__TMS_VOID, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_ERRNO_T
		)
	);

/*

Name:	UPOSIX_PATH_COMPARE_BUFFER()

Index:	Type:	UPOSIX_PATH_COMPARE_BUFFER_FN
	Type:	PUPOSIX_PATH_COMPARE_BUFFER_FN

Function:
	Compare two paths, allowing for case-sensitivity.

Definition:
	BOOL UPOSIX_PATH_COMPARE_BUFFER(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pPath1,
		BYTES nPath1,
		CONST USBPUMP_UTF8_TEXT * pPath2,
		BYTES nPath2
		);

	typedef BOOL UPOSIX_PATH_COMPARE_BUFFER_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pPath1,
		BYTES nPath1,
		CONST USBPUMP_UTF8_TEXT * pPath2,
		BYTES nPath2
		);

	typedef UPOSIX_PATH_COMPARE_BUFFER_FN *PUPOSIX_PATH_COMPARE_BUFFER_FN;

Description:
	The UPOSIX_PATH_COMPARE_BUFFER() function compares the paths given by
	pPath1 and pPath2, given lengths nPath1 and nPath2.  Both paths are
	assumed to valid relative to the current and root directory or
	directories that are valid for the current process.

	Comparing two paths seems simple: either use a case-folding compare,
	or use a case-sensitive compare, depending on the characteristics
	of the underlying system.  Unfortunately, modern file systems make
	things much more difficult.  This routine is intended to allow an
	architecture whereby the DataPump path library can support arbitrarily
	complex file systems (through virtual methods) while still providing
	a complete implementation for the typical path.

	UPOSIX_PATH_COMPARE_BUFFER_FN is the function signature for methods
	that implement UPOSIX_PATH_COMPARE_BUFFER(); and
	PUPOSIX_PATH_COMPARE_BUFFER_FN is the corresponding function pointer
	type.

Returns:
	TRUE if the two strings are equivalent after UNICODE case folding,
	FALSE otherwise.

See Also:
	UPOSIX_PATH_COMPARE_STRING(), UsbPumpPosix_PathCompareBufferDefault()

Notes:
	UPOSIX_PATH_COMPARE_BUFFER() is a macro and may evaluate pPosixContext
	more than once.

*/

__TMS_FNTYPE_DEF(
UPOSIX_PATH_COMPARE_BUFFER_FN,
__TMS_BOOL,
	(
	__TMS_UPOSIX_CTX *,
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT * /*pPath1*/,
	__TMS_BYTES /*nPath1*/,
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT * /*pPath2*/,
	__TMS_BYTES /*nPath2*/
	));

/*

Name:	UPOSIX_PATH_COMPARE_STRING()

Index:	Type:	UPOSIX_PATH_COMPARE_STRING_FN
	Type:	PUPOSIX_PATH_COMPARE_STRING_FN

Function:
	Compare two paths, allowing for case-sensitivity.

Definition:
	BOOL UPOSIX_PATH_COMPARE_STRING(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pPath1,
		CONST USBPUMP_UTF8_TEXT * pPath2
		);

	typedef BOOL UPOSIX_PATH_COMPARE_STRING_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pPath1,
		CONST USBPUMP_UTF8_TEXT * pPath2
		);

	typedef UPOSIX_PATH_COMPARE_STRING_FN *PUPOSIX_PATH_COMPARE_STRING_FN;

Description:
	The UPOSIX_PATH_COMPARE_STRING() function compares the paths given by
	pPath1 and pPath2, which are both NULL-terminated strings.   Both paths
	are assumed to valid relative to the current and root directory or
	directories that are valid for the current process.

	Comparing two paths seems simple: either use a case-folding compare,
	or use a case-sensitive compare, depending on the characteristics
	of the underlying system.  Unfortunately, modern file systems make
	things much more difficult.  This routine is intended to allow an
	architecture whereby the DataPump path library can support arbitrarily
	complex file systems (through virtual methods) while still providing
	a complete implementation for the typical path.

	UPOSIX_PATH_COMPARE_STRING_FN is the function signature for methods
	that implement UPOSIX_PATH_COMPARE_STRING(); and
	PUPOSIX_PATH_COMPARE_STRING_FN is the corresponding function pointer
	type.

Returns:
	TRUE if the two strings are equivalent after UNICODE case folding,
	FALSE otherwise.

See Also:
	UPOSIX_PATH_COMPARE_STRING(), UsbPumpPosix_PathCompareBufferDefault()

Notes:
	UPOSIX_PATH_COMPARE_STRING() is a macro and may evaluate pPosixContext
	more than once.

*/

__TMS_FNTYPE_DEF(
UPOSIX_PATH_COMPARE_STRING_FN,
__TMS_BOOL,
	(
	__TMS_UPOSIX_CTX *,
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT * /*pPath1*/,
	__TMS_CONST __TMS_USBPUMP_UTF8_TEXT * /*pPath2*/
	));

/*

Name:	UPOSIX_GETMAXPATH()

Index:	Type:	UPOSIX_GETMAXPATH_FN
	Type:	PUPOSIX_GETMAXPATH_FN

Function:
	Return max number of bytes occupied by a path for this OS

Definition:
	BYTES UPOSIX_GETMAXPATH(
		UPOSIX_CTX *pPosixContext
		);

	typedef BYTES UPOSIX_GETMAXPATH_FN(
		UPOSIX_CTX *pPosixContext
		);

	typedef UPOSIX_GETMAXPATH_FN *PUPOSIX_GETMAXPATH_FN;

Description:
	This function returns the max number of bytes that a path
	may occupy in this OS. Note that each character may occupy
	several bytes if a UTF-8 characters are used.

Returns:
	Max number of bytes which can be used to allocate a work-path buffer.

Notes:
	UPOSIX_GETMAXPATH() is a macro; the pPosixContext parameter may be
	evaluated more than once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_GETMAXPATH_FN,
	__TMS_BYTES, (
		__TMS_UPOSIX_CTX *
		)
	);

/*

Name:	UPOSIX_FSTAT()

Index:	Type:	UPOSIX_FSTAT_FN
	Type:	PUPOSIX_FSTAT_FN

Function:
	Get file status

Definition:
	UPOSIX_ERRNO_T UPOSIX_FSTAT(
		UPOSIX_CTX *pPosixContext
		CONST USBPUMP_UTF8_TEXT *pPathName,
		UPOSIX_STAT_T * pStat
		);

	typedef UPOSIX_ERRNO_T UPOSIX_FSTAT_FN(
		UPOSIX_CTX *pPosixContext
		CONST USBPUMP_UTF8_TEXT *pPathName,
		UPOSIX_STAT_T * pStat
		);

	typedef UPOSIX_FSTAT_FN *PUPOSIX_FSTAT_FN;

Description:
	The UPOSIX_FSTAT() function shall obtain information about a file
	named by the pPathName, and shall write it to the area pointed to
	by pStat.

	UPOSIX_FSTAT_FN is the function signature for methods that implement
	UPOSIX_FSTAT(); and PUPOSIX_FSTAT_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, 0 shall be returned. Otherwise, -1 shall be
	returned and error code set to indicate the error.

See Also:
	UPOSIX_SETERRNO()

Notes:
	UPOSIX_FSTAT() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FSTAT_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT * /*pPath*/,
		__TMS_UPOSIX_STAT_T * /*pStat*/
		)
	);

/*

Name:	UPOSIX_GETTIME()

Index:	Type:	UPOSIX_GETTIME_FN
	Type:	PUPOSIX_GETTIME_FN

Function:
	Get the date and time from UPOSIX_TIMESPEC_T

Definition:
	VOID UPOSIX_GETTIME(
		UPOSIX_CTX *pPosixContext,
		CONST UPOSIX_TIMESPEC_T * pTimeSpec,
		UPOSIX_TM_T * pTime
		);

	typedef VOID UPOSIX_GETTIME_FN(
		UPOSIX_CTX *pPosixContext,
		CONST UPOSIX_TIMESPEC_T * pTimeSpec,
		UPOSIX_TM_T * pTime
		);

	typedef UPOSIX_GETTIME_FN *PUPOSIX_GETTIME_FN;

Description:
	The UPOSIX_GETTIME() function shall obtain the date and time from
	pTimeSpec, and shall write it to the area pointed to by pTime.

	UPOSIX_GETTIME_FN is the function signature for methods that implement
	UPOSIX_GETTIME(); and PUPOSIX_GETTIME_FN is the corresponding
	function pointer type.

Returns:
	No explicit result.

See Also:
	UPOSIX_FSTAT()

Notes:
	UPOSIX_GETTIME() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_GETTIME_FN,
	__TMS_VOID, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_UPOSIX_TIMESPEC_T * /*pTimeSpec*/,
		__TMS_UPOSIX_TM_T * /*pTm*/
		)
	);

/*

Name:	UPOSIX_UTIME()

Index:	Type:	UPOSIX_UTIME_FN
	Type:	PUPOSIX_UTIME_FN

Function:
	Update the access and modification time of the file

Definition:
	VOID UPOSIX_UTIME(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pFilePath,
		UPOSIX_UTIMEBUF * pUTmBuf
		);

	typedef VOID UPOSIX_UTIME_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pFilePath,
		UPOSIX_UTIMEBUF * pUTmBuf
		);

	typedef UPOSIX_UTIME_FN *PUPOSIX_UTIME_FN;

Description:
	The UPOSIX_UTIME() function shall set the access and modification
	times of the file named by the pFilePath argument.

	If pUTmBuf is a null pointer, the access and modification times of
	the file shall be set to the current time.

	If pUTmBuf is not a null pointer, pUTmBuf shall be interpreted as
	a pointer to a UPOSIX_UTIMEBUF structure and the access and modification
	times shall be set to the values contained in the designated structure.

	UPOSIX_UTIME_FN is the function signature for methods that implement
	UPOSIX_UTIME(); and PUPOSIX_UTIME_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, 0 shall be returned. Otherwise, -1 shall be
	returned and error code set to indicate the error, and the file times
	shall not be affected.

See Also:
	UPOSIX_FSTAT()

Notes:
	UPOSIX_UTIME() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_UTIME_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT * /*pFilePath*/,
		__TMS_UPOSIX_UTIMEBUF * /*pUTmBuf*/
		)
	);

/*

Name:	UPOSIX_FFLUSH()

Index:	Type:	UPOSIX_FFLUSH_FN
	Type:	PUPOSIX_FFLUSH_FN

Function:
	Flush all data of a file in buffer caches to the device

Definition:
	INT UPOSIX_FFLUSH(
		UPOSIX_CTX * pPosixContext,
		UPOSIX_FILE * pFile
		);

	typedef INT UPOSIX_FFLUSH_FN(
		UPOSIX_CTX * pPosixContext,
		UPOSIX_FILE * pFile
		);

	typedef UPOSIX_FFLUSH_FN *PUPOSIX_FFLUSH_FN;

Description:
	This function causes all modified data to be moved to a permanent
	storage device.  This normally results in all in-core modified copies
	of buffers for the associated file to be written to a disk.

Returns:
	A 0 value is returned if the operation succeeds, otherwise returns -1
	and the global variable errno indicates the reason for the failure.

Notes:
	UPOSIX_FFLUSH() is a macro; the pPosixContext parameter may be
	evaluated more than once.

*/

/* 3118: change name from FSYNC to FFLUSH and add synonym for the macro */
__TMS_FNTYPE_DEF(
	UPOSIX_FFLUSH_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_UPOSIX_FILE *
		)
	);

#define	__TMS_UPOSIX_FSYNC_FN	__TMS_UPOSIX_FFLUSH_FN
#define	__TMS_PUPOSIX_FSYNC_FN	__TMS_PUPOSIX_FFLUSH_FN

/*

Name:	UPOSIX_RENAME()

Index:	Type:	UPOSIX_RENAME_FN
	Type:	PUPOSIX_RENAME_FN

Function:
	Change name of a file or directory

Definition:
	INT UPOSIX_RENAME(
		UPOSIX_CTX * pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pFromPath,
		CONST USBPUMP_UTF8_TEXT * pToPath
		);

	typedef INT UPOSIX_RENAME_FN(
		UPOSIX_CTX * pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pFromPath,
		CONST USBPUMP_UTF8_TEXT * pToPath
		);

	typedef UPOSIX_RENAME_FN *PUPOSIX_RENAME_FN;

Description:
	This function changes a file or directory name from pFromPath to
	pToPath.  Both pFromPath and pToPath must be of the same type (that is,
	both directories or both non-directories), and must reside on the same
	file system.

Returns:
	A 0 value is returned if the operation succeeds, otherwise returns -1
	and the global variable errno indicates the reason for the failure.

Notes:
	UPOSIX_RENAME() is a macro; the pPosixContext parameter may be
	evaluated more than once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_RENAME_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX * /* pPosixContext */,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT * /* pFromPath */,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT * /* pToPath */
		)
	);

/*

Name:	UPOSIX_GETTIMEOFDAY64()

Index:	Type:	UPOSIX_GETTIMEOFDAY64_FN
	Type:	PUPOSIX_GETTIMEOFDAY64_FN

Function:
	Obtains the current time of the system

Definition:
	INT UPOSIX_GETTIMEOFDAY64(
		UPOSIX_CTX * pPosixContext,
		UPOSIX_TIMESPEC64_T* pTimeSpec,
		VOID * pTimeSpecContext
		);

	typedef INT UPOSIX_GETTIMEOFDAY64_FN(
		UPOSIX_CTX * pPosixContext,
		UPOSIX_TIMESPEC64_T* pTimeSpec,
		VOID * pTimeSpecContext
		);

	typedef UPOSIX_GETTIMEOFDAY64_FN *PUPOSIX_GETTIMEOFDAY64_FN;

Description:
	This function obtains the current time of the system. The time is
	expressed as seconds and nanoseconds since the Epoch, and stored it
	in UPOSIX_TIMESPEC64_T structure. pTimeSpecContext argument is ignored
	without any processing.

Returns:
	0 When success.
	-1 in case of error and POSIX_ERROR is set with appropriate value

Notes:
	UPOSIX_GETTIMEOFDAY64() is a macro; the pPosixContext parameter may be
	evaluated more than once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_GETTIMEOFDAY64_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX * /* pPosixContext */,
		__TMS_UPOSIX_TIMESPEC64_T* /* pTimeSpec64 */,
		__TMS_VOID * /* pTimeSpecContext */
		)
	);


/*

Name:	UPOSIX_FSETSIZE()

Index:	Type:	UPOSIX_FSETSIZE_FN
	Type:	PUPOSIX_FSETSIZE_FN

Function:
	Set the size of a file

Definition:
	INT UPOSIX_FSETSIZE(
		UPOSIX_CTX * pPosixContext,
		UPOSIX_FILE * pPosixFile,
		UPOSIX_FOFFSET_T newSize
		);

	typedef INT UPOSIX_FSETSIZE_FN(
		UPOSIX_CTX * pPosixContext,
		UPOSIX_FILE * pPosixFile,	
		UPOSIX_FOFFSET_T newSize
		);

	typedef UPOSIX_FSETSIZE_FN *PUPOSIX_FSETSIZE_FN;

Description:
	The UPOSIX_FSETSIZE() function sets the size of the file previously
	opened with UPOSIX_FOPEN().  The application should ensure that the
	process has the write permission to the file.  If the current file
	size is larger than the newSize, the extra data is discarded. If
	the current file size is smaller than the newSize, the file is 
	extended, the data read out from the extended area is undefined.
	This function does not modify the file offset associated with the file.

	UPOSIX_FSETSIZE_FN is the function signature for methods that implement
	UPOSIX_FSETSIZE(); and PUPOSIX_FSETSIZE_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, 0 is returned.  Otherwise, returns -1
	and the global variable errno is set to indicate the reason for the
	failure.

See Also:
	UPOSIX_GETSIZE(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FSETSIZE() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FSETSIZE_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,			/* pPosixCtx */
		__TMS_UPOSIX_FILE *,			/* pPosixFile */
		__TMS_UPOSIX_FOFFSET_T			/* newSize */
		)
	);

/*

Name:	UPOSIX_STATFS()

Index:	Type:	UPOSIX_STATFS_FN
	Type:	PUPOSIX_STATFS_FN

Function:
	Return information of a mounted file system

Definition:
	INT UPOSIX_STATFS(
		UPOSIX_CTX * pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pPathName,
		UPOSIX_STATFS_T * pStatfs
		);

	typedef INT UPOSIX_STATFS_FN(
		UPOSIX_CTX * pPosixContext,
		CONST USBPUMP_UTF8_TEXT * pPathName,
		UPOSIX_STATFS_T * pStatfs
		);

	typedef UPOSIX_STATFS_FN *PUPOSIX_STATFS_FN;

Description:
	The UPOSIX_STATFS() function obtains information about a
	mounted file system which contains the file named by the pPathName
	argument.  The information obtained is written to the area pointed
	to by pStatfs.

	UPOSIX_STATFS_FN is the function signature for methods that implement
	UPOSIX_STATFS(); and PUPOSIX_STATFS_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, 0 is returned.  Otherwise, returns -1
	and the corresponding errno is set to indicate the reason for the 
	failure.

See Also:
	UPOSIX_FSTAT(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_STATFS() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_STATFS_FN,
	__TMS_INT, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *,	/* pPathName */
		__TMS_UPOSIX_STATFS_T *			/* pStatfs */
		)
	);

/****************************************************************************\
|
|	UPOSIX_DIR_METHODS function definitions
|
\****************************************************************************/

/*

Name:	UPOSIX_DIR_OPENDIR()

Index:	Type:	UPOSIX_DIR_OPENDIR_FN
	Type:	PUPOSIX_DIR_OPENDIR_FN
	Type:	UPOSIX_DIR
	Type	PUPOSIX_DIR

Function:
	Open a directory (in an abstract way)

Definition:
	typedef struct __TMS_UPOSIX_DIR UPOSIX_DIR, *PUPOSIX_DIR;

	UPOSIX_DIR *UPOSIX_DIR_OPENDIR(
		UPOSIX_CTX *pPosixpContext,
		CONST USBPUMP_UTF8_TEXT *pDirectoryName
		);

	typedef UPOSIX_DIR *UPOSIX_DIR_OPENDIR_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pDirectoryName
		);

	typedef UPOSIX_DIR *PUPOSIX_DIR:

Description:
	This function opens a directory in whatever way makes sense to
	the system, and returns a handle which can be used for subsequent
	directory operations.

	pPosixContext is a pointer to the context object.

	pDirectoryName is a pointer to the directory name, which is UTF-8.
	Note that USBPUMP_UTF8_TEXT is compatible with TEXT, so there's no need
	(or benefit, or under MCCI's style rules, allowance) for an explicit
	cast.

Returns:
	A pointer to a UPOSIX_DIR, which can be used in subsequent
	directory operations to scan the directory.

Notes:
	The contents of UPOSIX_DIR are opaque to implementors.

	UPOSIX_DIR_OPENDIR() is a macro; the pPosixContext parameter may be
	evaluated more than once.

See Also:
	UPOSIX_DIR_CLOSEDIR(), UPOSIX_DIR_READDIR()

*/

__TMS_FNTYPE_DEF(
	UPOSIX_DIR_OPENDIR_FN,
	__TMS_UPOSIX_DIR *, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_DIR_CLOSEDIR()

Index:	Type:	UPOSIX_DIR_CLOSEDIR_FN
	Type:	PUPOSIX_DIR_CLOSEDIR_FN

Function:
	Close a directory handle opened with UPOSIX_OPENDIR()

Definition:
	INT UPOSIX_DIR_CLOSEDIR(
		UPOSIX_DIR *pDir
		);

	typedef INT UPOSIX_DIR_CLOSEDIR_FN(
		UPOSIX_DIR *pDir
		);

	typedef UPOSIX_DIR_CLOSEDIR_FN *PUPOSIX_DIR_CLOSEDIR_FN;

Description:
	The directory object given by pDir, which must have been
	returned by a previous call to UPOSIX_OPENDIR(), is freed.

	After calling this function, the caller must treat any pointers
	to the object as invalid.

Returns:
	0 for success; -1 for any failure, and error code shall be set to
	indicate the error.

Notes:
	UPOSIX_DIR_CLOSEDIR() is a macro; the pPosixContext parameter may be
	evaluated more than once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_DIR_CLOSEDIR_FN,
	__TMS_INT, (
		__TMS_UPOSIX_DIR *
		)
	);

/*

Name:	UPOSIX_DIR_READDIR()

Index:	Type:	UPOSIX_DIR_READDIR_FN
	Type:	PUPOSIX_DIR_READDIR_FN

Function:
	Reads the next entry from an open directory.

Definition:
	UPOSIX_DIRENT *UPOSIX_DIR_READDIR(
		UPOSIX_DIR *pDir
		);

	typedef UPOSIX_DIRENT *UPOSIX_DIR_READDIR_FN(
		UPOSIX_DIR *pDir
		);

	typedef UPOSIX_DIR_READDIR_FN *PUPOSIX_DIR_READDIR_FN;

Description:
	UPOSIX_DIR_READDIR() returns a pointer to a UPOSIX_DIRENT structure,
	which is valid until the next call to UPOSIX_DIR_READDIR() for this
	directory or the next call to UPOSIX_CLOSEDIR() for this directory.
	The UPOSIX_DIRENT contains the name of the file.

	UPOSIX_DIR_READDIR_FN is the function signature for methods that implement
	UPOSIX_DIR_READDIR(); and PUPOSIX_DIR_READDIR_FN is the corresponding
	function pointer type.

Returns:
	A pointer to a UPOSIX_DIRENT structure, or NULL if the end of file
	has been reached for the directory or if some error has occured.

See Also:
	UPOSIX_DIR_OPENDIR(), UPOSIX_DIRENT

Notes:
	UPOSIX_DIR_READDIR() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_DIR_READDIR_FN,
	__TMS_UPOSIX_DIRENT *, (
		__TMS_UPOSIX_DIR *
		)
	);


/****************************************************************************\
|
|	UPOSIX_FILE_METHODS function definitions
|
\****************************************************************************/

/*

Name:	UPOSIX_FILE_FOPEN()

Index:	Type:	UPOSIX_FILE_FOPEN_FN
	Type:	PUPOSIX_FILE_FOPEN_FN

Function:
	Open a file stream

Definition:
	UPOSIX_FILE *UPOSIX_FILE_FOPEN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pFileName,
		CONST USBPUMP_UTF8_TEXT *pOpenMode
		);

	typedef UPOSIX_FILE *UPOSIX_FILE_FOPEN_FN(
		UPOSIX_CTX *pPosixContext,
		CONST USBPUMP_UTF8_TEXT *pFileName,
		CONST USBPUMP_UTF8_TEXT *pOpenMode
		);

	typedef UPOSIX_FILE_FOPEN_FN *PUPOSIX_FILE_FOPEN_FN;

Description:
	The UPOSIX_FILE_FOPEN() shall open the file whose pathname is the string
	pointed to by pFileName, and associates a stream with it.

	The pOpenMode argument points to a string.  If the string is one of
	the following, the file shall be opened in the indicated mode.
	Otherwise, the behavior is undefined.

	  r or rb
		Open file for reading.
	  w or wb
		Truncate to zero length or create file for writing.
	  a or ab
		Append; open or create file for writing at end-of-file.
	  r+ or rb+ or r+b
		Open file for update (reading and writing).
	  w+ or wb+ or w+b
		Truncate to zero length or create file for update.
	  a+ or ab+ or a+b
		Append; open or create file for update, writing at end-of-file.

	  T
		Temporary file; this can be used any combination of above.

	UPOSIX_FILE_FOPEN_FN is the function signature for methods that implement
	UPOSIX_FILE_FOPEN(); and PUPOSIX_FILE_FOPEN_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FILE_FOPEN() shall return a pointer to
	the object controlling the stream.  Otherwise, a null pointer shall be
	returned, and error code shall be set to indicate the error.

See Also:
	UPOSIX_FILE_FCLOSE(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FOPEN() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FOPEN_FN,
	__TMS_UPOSIX_FILE *, (
		__TMS_UPOSIX_CTX *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *,
		__TMS_CONST __TMS_USBPUMP_UTF8_TEXT *
		)
	);

/*

Name:	UPOSIX_FILE_FCLOSE()

Index:	Type:	UPOSIX_FILE_FCLOSE_FN
	Type:	PUPOSIX_FILE_FCLOSE_FN

Function:
	Close a file stream

Definition:
	INT UPOSIX_FILE_FCLOSE(
		UPOSIX_FILE *pFileStream
		);

	typedef INT UPOSIX_FILE_FCLOSE_FN(
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FILE_FCLOSE_FN *PUPOSIX_FILE_FCLOSE_FN;

Description:
	The UPOSIX_FILE_FCLOSE() shall cause the stream pointed to by stream to
	be flushed and the associated file to be closed. Any unwritten buffered
	data for the stream shall be written to the file; any unread buffered
	data shall be discarded.

	After the call to UPOSIX_FILE_FCLOSE(), any use of stream results in
	undefined behavior.

	UPOSIX_FILE_FCLOSE_FN is the function signature for methods that
	implement UPOSIX_FILE_FCLOSE(); and PUPOSIX_FILE_FCLOSE_FN is the
	corresponding function pointer type.

Returns:
	Upon successful completion, UPOSIX_FILE_FCLOSE() shall return 0.
	Otherwise, it shall return non-zero, and error code shall be set to
	indicate the error.

See Also:
	UPOSIX_FILE_FOPEN(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FCLOSE() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FCLOSE_FN,
	__TMS_INT, (
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FILE_FREAD()

Index:	Type:	UPOSIX_FILE_FREAD_FN
	Type:	PUPOSIX_FILE_FREAD_FN

Function:
	Read block of data from a file stream

Definition:
	SIZE_T UPOSIX_FILE_FREAD(
		VOID *pBuffer,
		SIZE_T SizeOfBlock,
		SIZE_T NumberOfBlock,
		UPOSIX_FILE *pFileStream
		);

	typedef SIZE_T UPOSIX_FILE_FREAD_FN(
		VOID *pBuffer,
		SIZE_T SizeOfBlock,
		SIZE_T NumberOfBlock,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FILE_FREAD_FN *PUPOSIX_FILE_FREAD_FN;

Description:
	The UPOSIX_FILE_FREAD() shall read into the array pointed to by pBuffer
	up to NumberOfBlock elements whose size is specified by SizeOfBlock in
	bytes, from the stream pointed to by pFileStream.

	UPOSIX_FILE_FREAD_FN is the function signature for methods that implement
	UPOSIX_FILE_FREAD(); and PUPOSIX_FILE_FREAD_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FILE_FREAD() shall return the number
	of elements successfully read which is less than NumberOfBlock only if
	a read error or end-of-file is encountered.  If SizeOfBlock or
	NumberOfBlock is 0, UPOSIX_FILE_FREAD() shall return 0 and the contents
	of the array and the state of the stream remain unchanged.
	Otherwise, if a read error occurs, the error indicator for the stream
	shall be set, and error code shall be set to indicate the error.

See Also:
	UPOSIX_FILE_FWRITE(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FREAD() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FREAD_FN,
	__TMS_SIZE_T, (
		__TMS_VOID *,
		__TMS_SIZE_T,
		__TMS_SIZE_T,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FILE_FWRITE()

Index:	Type:	UPOSIX_FILE_FWRITE_FN
	Type:	PUPOSIX_FILE_FWRITE_FN

Function:
	Write block of data from a file stream

Definition:
	SIZE_T UPOSIX_FILE_FWRITE(
		CONST VOID *pBuffer,
		SIZE_T SizeOfBlock,
		SIZE_T NumberOfBlock,
		UPOSIX_FILE *pFileStream
		);

	typedef SIZE_T UPOSIX_FILE_FWRITE_FN(
		CONST VOID *pBuffer,
		SIZE_T SizeOfBlock,
		SIZE_T NumberOfBlock,
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FILE_FWRITE_FN *PUPOSIX_FILE_FWRITE_FN;

Description:
	The UPOSIX_FILE_FWRITE() shall write, from the array pointed to by
	pBuffer, up to NumberOfBlock elements whose size is specified by
	SizeOfBlock, to the stream pointed to by pFileStream.

	UPOSIX_FILE_FWRITE_FN is the function signature for methods that
	implement UPOSIX_FILE_FWRITE(); and PUPOSIX_FILE_FWRITE_FN is the
	corresponding function pointer type.

Returns:
	Upon successful completion, UPOSIX_FILE_FWRITE() shall return the number
	of elements successfully written, which is less than NumberOfBlock if a
	write error is encountered.  If SizeOfBlock or NumberOfBlock is 0,
	UPOSIX_FILE_FWRITE() shall return 0 and the state of the stream remain
	unchanged.
	Otherwise, if a write error occurs, the error indicator for the stream
	shall be set, and error code shall be set to indicate the error.

See Also:
	UPOSIX_FILE_FREAD(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FWRITE() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FWRITE_FN,
	__TMS_SIZE_T, (
		__TMS_CONST __TMS_VOID *,
		__TMS_SIZE_T,
		__TMS_SIZE_T,
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FILE_FGETC()

Index:	Type:	UPOSIX_FILE_FGETC_FN
	Type:	PUPOSIX_FILE_FGETC_FN

Function:
	Get a character from a file stream

Definition:
	INT UPOSIX_FILE_FGETC(
		UPOSIX_FILE *pFileStream
		);

	typedef INT UPOSIX_FILE_FGETC_FN(
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FILE_FGETC_FN *PUPOSIX_FILE_FGETC_FN;

Description:
	If the end-of-file indicator for the input stream pointed to by
	pFileStream is not set and a next byte is present, the UPOSIX_FILE_FGETC
	function shall obtain the next byte as an unsigned char converted to
	an int, from the input stream pointed to by stream, and advance the
	associated file position indicator for the stream (if defined).

	UPOSIX_FILE_FGETC_FN is the function signature for methods that implement
	UPOSIX_FILE_FGETC(); and PUPOSIX_FILE_FGETC_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FILE_FGETC() shall return the next
	byte from the input stream pointed to by stream.  If the end-of-file
	indicator for the stream is set, or if the stream is at end-of-file,
	the end-of-file indicator for the stream shall be set and
	UPOSIX_FILE_FGETC() shall return -1.  If a read error occurs, the error
	indicator for the stream shall be set, UPOSIX_FILE_FGETC() shall return
	-1, and error code shall be set to indicate the error.

See Also:
	UPOSIX_FILE_FREAD(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FGETC() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FGETC_FN,
	__TMS_INT, (
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FILE_FTELL()

Index:	Type:	UPOSIX_FILE_FTELL_FN
	Type:	PUPOSIX_FILE_FTELL_FN

Function:
	Returns the current file position of the stream

Definition:
	UPOSIX_FOFFSET_T UPOSIX_FILE_FTELL(
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FOFFSET_T UPOSIX_FILE_FTELL_FN(
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FILE_FTELL_FN *PUPOSIX_FILE_FTELL_FN;

Description:
	The UPOSIX_FILE_FTELL() function obtain the current value of the
	file-position indicator for the stream pointed to by pFileStream.

	UPOSIX_FILE_FTELL_FN is the function signature for methods that implement
	UPOSIX_FILE_FTELL(); and PUPOSIX_FILE_FTELL_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, UPOSIX_FILE_FTELL() shall return the current
	value of the file-position indicator for the stream measured in bytes
	from the beginning of the file.
	Otherwise, UPOSIX_FILE_FTELL() shall return -1 and error code shall be
	set to indicate the error.

See Also:
	UPOSIX_FILE_FSEEK(), UPOSIX_FILE_FEOF(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FTELL() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FTELL_FN,
	__TMS_UPOSIX_FOFFSET_T, (
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FILE_FSEEK()

Index:	Type:	UPOSIX_FILE_FSEEK_FN
	Type:	PUPOSIX_FILE_FSEEK_FN

Function:
	Reposition a file-position indicator in a stream

Definition:
	INT UPOSIX_FILE_FSEEK(
		UPOSIX_FILE *pFileStream,
		UPOSIX_FOFFSET_T Offset,
		UPOSIX_FILE_SEEK Whence
		);

	typedef INT UPOSIX_FILE_FSEEK_FN(
		UPOSIX_FILE *pFileStream,
		UPOSIX_FOFFSET_T Offset,
		UPOSIX_FILE_SEEK Whence
		);

	typedef UPOSIX_FILE_FSEEK_FN *PUPOSIX_FILE_FSEEK_FN;

Description:
	The UPOSIX_FILE_FSEEK() function shall set the file-position indicator
	for the stream pointed to by pFileStream.

	The new position, measured in bytes from the beginning of the file,
	shall be obtained by adding offset to the position specified by Whence.
	The specified point is the beginning of the file for
	UPOSIX_FILE_SEEK_SET, the current value of the file-position indicator
	for UPOSIX_FILE_SEEK_CUR, or end-of-file for UPOSIX_FILE_SEEK_END.

	UPOSIX_FILE_FSEEK_FN is the function signature for methods that implement
	UPOSIX_FILE_FSEEK(); and PUPOSIX_FILE_FSEEK_FN is the corresponding
	function pointer type.

Returns:
	The UPOSIX_FILE_FSEEK() function shall return 0 if they succeed.
	Otherwise, UPOSIX_FILE_FSEEK() shall return -1 and error code shall be
	set to indicate the error.

See Also:
	UPOSIX_FILE_FTELL(), UPOSIX_FILE_FEOF(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FSEEK() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FSEEK_FN,
	__TMS_INT, (
		__TMS_UPOSIX_FILE *,
		__TMS_UPOSIX_FOFFSET_T,
		__TMS_UPOSIX_FILE_SEEK
		)
	);

/*

Name:	UPOSIX_FILE_FEOF()

Index:	Type:	UPOSIX_FILE_FEOF_FN
	Type:	PUPOSIX_FILE_FEOF_FN

Function:
	Test end-of-file indicator on a stream

Definition:
	INT UPOSIX_FILE_FEOF(
		UPOSIX_FILE *pFileStream
		);

	typedef INT UPOSIX_FILE_FEOF_FN(
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FILE_FEOF_FN *PUPOSIX_FILE_FEOF_FN;

Description:
	The UPOSIX_FILE_FEOF() function shall test the end-of-file indicator for
	the stream pointed to by pFileStream.

	UPOSIX_FILE_FEOF_FN is the function signature for methods that implement
	UPOSIX_FILE_FEOF(); and PUPOSIX_FILE_FEOF_FN is the corresponding
	function pointer type.

Returns:
	The UPOSIX_FILE_FEOF() function shall return non-zero if and only if the
	end-of-file indicator is set for pFileStream.

See Also:
	UPOSIX_FILE_FTELL(), UPOSIX_FILE_FSEEK(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FEOF() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FEOF_FN,
	__TMS_INT, (
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FILE_FERROR()

Index:	Type:	UPOSIX_FILE_FERROR_FN
	Type:	PUPOSIX_FILE_FERROR_FN

Function:
	Test error indicator on a stream

Definition:
	INT UPOSIX_FILE_FERROR(
		UPOSIX_FILE *pFileStream
		);

	typedef INT UPOSIX_FILE_FERROR_FN(
		UPOSIX_FILE *pFileStream
		);

	typedef UPOSIX_FILE_FERROR_FN *PUPOSIX_FILE_FERROR_FN;

Description:
	The UPOSIX_FILE_FERROR() function shall test the error indicator for
	the stream pointed to by pFileStream.

	UPOSIX_FILE_FERROR_FN is the function signature for methods that
	implement UPOSIX_FILE_FERROR(); and PUPOSIX_FILE_FERROR_FN is the
	corresponding function pointer type.

Returns:
	The UPOSIX_FILE_FERROR() function shall return non-zero if and only if
	the error indicator is set for pFileStream.

Notes:
	UPOSIX_FILE_FERROR() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FERROR_FN,
	__TMS_INT, (
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FILE_FFLUSH()

Index:	Type:	UPOSIX_FILE_FFLUSH_FN
	Type:	PUPOSIX_FILE_FFLUSH_FN

Function:
	Flush all data of a file in buffer caches to the device

Definition:
	INT UPOSIX_FILE_FFLUSH(
		UPOSIX_FILE * pFile
		);

	typedef INT UPOSIX_FILE_FFLUSH_FN(
		UPOSIX_FILE * pFile
		);

	typedef UPOSIX_FILE_FFLUSH_FN *PUPOSIX_FILE_FFLUSH_FN;

Description:
	This function causes all modified data to be moved to a permanent
	storage device.  This normally results in all in-core modified copies
	of buffers for the associated file to be written to a disk.

Returns:
	A 0 value is returned if the operation succeeds, otherwise returns -1
	and the global variable errno indicates the reason for the failure.

Notes:
	UPOSIX_FILE_FFLUSH() is a macro; the pPosixContext parameter may be
	evaluated more than once.

*/

/* 3118: change name from FSYNC to FFLUSH and add synonym for the macro */
__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FFLUSH_FN,
	__TMS_INT, (
		__TMS_UPOSIX_FILE *
		)
	);

/*

Name:	UPOSIX_FILE_FSETSIZE()

Index:	Type:	UPOSIX_FILE_FSETSIZE_FN
	Type:	PUPOSIX_FILE_FSETSIZE_FN

Function:
	Set the size of a file

Definition:
	INT UPOSIX_FILE_FSETSIZE(
		UPOSIX_FILE	*pFile,
		UPOSIX_FOFFSET_T newSize
		);

	typedef INT UPOSIX_FILE_FSETSIZE_FN(
		UPOSIX_FILE	*pFile,
		UPOSIX_FOFFSET_T newSize
		);

	typedef UPOSIX_FILE_FSETSIZE_FN *PUPOSIX_FILE_FSETSIZE_FN;

Description:
	The UPOSIX_FILE_FSETSIZE() function sets the size of the file with the
	file stream given by pFile.  The application should ensure that the
	process has the write permission to the file.  If the current file
	size is larger than the newSize, the extra data is discarded. If
	the current file size is smaller than the newSize, the file is 
	extended, the data read out from the extended area is undefined.
	This function does not modify the file offset associated with the file.

	UPOSIX_FILE_FSETSIZE_FN is the function signature for methods that implement
	UPOSIX_FILE_FSETSIZE(); and PUPOSIX_FILE_FSETSIZE_FN is the corresponding
	function pointer type.

Returns:
	Upon successful completion, 0 is returned.  Otherwise, returns -1
	and the errno is set to indicate the reason for the failure.

See Also:
	UPOSIX_FSETSIZE(), UPOSIX_GETERRNO()

Notes:
	UPOSIX_FILE_FSETSIZE() is a macro and may evaluate pPosixContext more than
	once.

*/

__TMS_FNTYPE_DEF(
	UPOSIX_FILE_FSETSIZE_FN,
	__TMS_INT, (
		__TMS_UPOSIX_FILE *,
		__TMS_UPOSIX_FOFFSET_T
		)
	);
	
/****************************************************************************\
|
|	Uncloaked definitions
|
\****************************************************************************/

/**** uncloaked names generated by uncloak-defs.sh ****/
#if !__TMS_CLOAKED_NAMES_ONLY
# define UPOSIX_FSYNC_FN	\
   __TMS_UPOSIX_FSYNC_FN
# define PUPOSIX_FSYNC_FN	\
   __TMS_PUPOSIX_FSYNC_FN
#endif /* !__TMS_CLOAKED_NAMES_ONLY */

/**** end of uposix_types.h ****/
#endif /* _UPOSIX_TYPES_H_ */
