/*
 * mvStdHelpers.c
 *
 * File created to fill in any missing standard C functionality
 *
 *  Created on: May 16, 2014
 */

//Disable libc overwrites
#define DONTWRITE_STD_LIBC

#include <stdio.h>
#include <stdlib.h>
#include <sys/reent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <mvStdio.h>
#include <stdarg.h>

char* mvInputBuffer;

#pragma weak _fstat_r
int _fstat_r(struct _reent * unused, int filedes, struct stat * buf)
{
    UNUSED(unused);
    UNUSED(filedes);

    //Set the stat structure accordingly. Mostly dummies for bare-metal
    //Dummy device ID
    buf->st_dev=0xC0FE;
    //Dummy inode number
    buf->st_ino=0xBABE;
    //In bare metal all modes are character type (as terminals are)
    buf->st_mode=S_IFCHR;
    //Dummy UID
    buf->st_uid=S_IRWXU;
    //Dummy GID
    buf->st_gid=0x1234;
    //Dummy RGID
    buf->st_rdev=0;
    //Dummy size since all files will be terminal
    buf->st_size=10;
    //Dummy block size since all files will be terminal
    buf->st_blksize=1;
    //Dummy number of blocks since all files will be terminal
    buf->st_blocks=1;
    //Dummy time of access since all files will be terminal
    buf->st_atime=0xBEEF;
    //Dummy last modification since all files are terminal
    buf->st_mtime=0xBEEF;
    //Dummy last status change since all files are terminal
    buf->st_ctime=0xBEEF;

    //And return success for now since we aren't really working with file descriptors
    return 0;
}

#pragma weak _isatty_r
int _isatty_r(struct _reent * unused, int fd)
{
    UNUSED(unused);
    UNUSED(fd);

    //we are not using files, only terminals so this function will allways return
    //1 in our case
    return 1;
}

#pragma weak _close_r
int _close_r(struct _reent * unused, int fd)
{
    UNUSED(unused);
    UNUSED(fd);

    //On the Movidius architecture we never use real files so there is
    //no reason for this function to return anything other than 0 (success)
    return 0;
}

#pragma weak _lseek_r
_off_t _lseek_r(struct _reent * unused,int fildes, _off_t offset, int whence)
{
    UNUSED(unused);
    UNUSED(fildes);
    UNUSED(whence);

    //returning offset every time. Not really used.
    return offset;
}

void mvSetReadPointer(char* address)
{
    mvInputBuffer=address;
}

#pragma weak _read_r
int _read_r(struct _reent * unused,int fildes, void *buf, size_t nbyte)
{
    UNUSED(unused);
    UNUSED(fildes);

    //We can't really "read" on Myriad. We
    //can fake it though as to consume input from a buffer
    size_t readbytes=0;
    while ((*mvInputBuffer != '\0') && (readbytes<nbyte)){
        ((char*)buf)[readbytes]=*mvInputBuffer;
        mvPutchar(*mvInputBuffer);
        readbytes++;
        mvInputBuffer++;
    }
    if (readbytes==nbyte){
        return nbyte;
    }else{
        return 0;
    }
}

#pragma weak _write_r
int _write_r(struct _reent * unused,int fildes, const void *buf, size_t nbyte)
{
    char *str = (char *)buf;

    UNUSED(unused);
    UNUSED(fildes);

#ifndef NO_PRINT
    for(size_t i = 0 ; i < nbyte ; i++)
    {
        //Write the character to the JTAG output
        mvPutchar(str[i]);
    }
#else
    UNUSED(str); // in case it is defined to not used PRINT then handle compiler generated unused warning
#endif

    return nbyte;
}

//-------------------------------------------------------------
//This section refers to some functions needed by libgloss which are not implemented anywhere else
//Just return 1. We don't really expect the serial interface to fail. No way to find out if it has failed either...
#pragma weak outbyte
int outbyte(char x)
{
    UNUSED(x);

    mvPutchar(x);
    return 1;
}

/*
 * print -- do a raw print of a string
 */
#pragma weak print
int print(char *ptr)
{
    int printed=0;
    while (*ptr) {
        outbyte (*ptr++);
        printed++;
    }
    return printed;
}
/*
 * putnum -- print a 32 bit number in hex
 */
void putnum(unsigned int num)
{
    char  buf[9];
    int   cnt;
    char  *ptr;
    int   digit;

    ptr = buf;
    for (cnt = 7 ; cnt >= 0 ; cnt--) {
        digit = (num >> (cnt * 4)) & 0xf;

        if (digit <= 9)
            *ptr++ = (char) ('0' + digit);
        else
            *ptr++ = (char) ('a' - 10 + digit);
    }

    *ptr = (char) 0;
    print (buf);
}
//-------------------------------------------------------------
//--------libgloss functions section---------------------------
//-------------------------------------------------------------

#pragma weak _open_r
int _open_r(struct _reent * unused, const char * file, int flags, int mode)
{
    UNUSED(unused);
    UNUSED(file);
    UNUSED(flags);
    UNUSED(mode);

    // As we are not using files, return 0 (Success)
    return 0;
}

#pragma weak _stat_r
int _stat_r(struct _reent * unused, const char * file, struct stat * buf)
{
    UNUSED(unused);
    UNUSED(file);
    UNUSED(stat);

    //Set the stat structure accordingly. Mostly dummies for bare-metal
    //Dummy device ID
    buf->st_dev=0xC0FE;
    //Dummy inode number
    buf->st_ino=0xBABE;
    //In bare metal all modes are character type (as terminals are)
    buf->st_mode=S_IFCHR;
    //Dummy UID
    buf->st_uid=S_IRWXU;
    //Dummy GID
    buf->st_gid=0x1234;
    //Dummy RGID
    buf->st_rdev=0;
    //Dummy size since all files will be terminal
    buf->st_size=10;
    //Dummy block size since all files will be terminal
    buf->st_blksize=1;
    //Dummy number of blocks since all files will be terminal
    buf->st_blocks=1;
    //Dummy time of access since all files will be terminal
    buf->st_atime=0xBEEF;
    //Dummy last modification since all files are terminal
    buf->st_mtime=0xBEEF;
    //Dummy last status change since all files are terminal
    buf->st_ctime=0xBEEF;
    // Return Success
    return 0;
}

#pragma weak _link_r
int _link_r(struct _reent * unused, const char * old, const char *new)
{
    UNUSED(unused);
    UNUSED(old);
    UNUSED(new);

    // As we are not using files, return 0 (Success)
    return 0;
}

#pragma weak _unlink
int _unlink(struct _reent * unused, const char * name)
{
    UNUSED(unused);
    UNUSED(name);

    // As we are not using files, return 0 (Success)
    return 0;
}

int _wait(struct _reent * unused, int * status)
{
    UNUSED(unused);
    UNUSED(status);

    // There are no system calls implemented for processes,
    // so always return fail, for now.
    return -1;
}

void _exit(int rc)
{
    UNUSED(rc);

    exit(0);
}

int _execve_r(struct _reent * unused, char * name, char ** argv, char ** env)
{
    UNUSED(unused);
    UNUSED(name);
    UNUSED(argv);
    UNUSED(env);

    // For bare-metal implementation only one user thread is posible,
    // so always return fail
    return -1;
}

int _fork_r(struct _reent * unused)
{
    UNUSED(unused);

    // For bare-metal implementation only one user thread is posible,
    // so always return fail
    return -1;
}

int _getpid_r(struct _reent * unused)
{
    UNUSED(unused);

    // For bare-metal implementation only one user thread is posible,
    // so we can return constant here. It's safer to return 1, to
    // avoid treating process id 0 as special process.
    return 1;
}

int _kill_r(struct _reent * unused, int pid, int sig)
{
    UNUSED(unused);
    UNUSED(pid);
    UNUSED(sig);

    // As we don't have processes, which to receive signals,
    // always return fail
    return -1;
}

int _times_r(struct _reent * unused, struct tms * buf)
{
    UNUSED(unused);
    UNUSED(buf);

    // For bare-metal implementaion we don't need any timing information,
    // so always return fail
    return -1;
}

int fstat(int filedes, struct stat * buf)
{
    int ret;
    ret = _fstat_r((struct _reent*)NULL, filedes, buf);

    return ret;
}

int isatty(int fd)
{
    int ret;

    ret = _isatty_r((struct _reent*)NULL, fd);

    return ret;
}

int close(int fd)
{
    int ret;

    ret = _close_r((struct _reent*)NULL, fd);

    return ret;
}

off_t lseek(int fildes, off_t offset, int whence)
{
    long int ret;

    ret = _lseek_r((struct _reent*)NULL, fildes, offset, whence);

    return ret;
}

int read(int fildes, void *buf, size_t nbyte)
{
    int ret;

    ret = _read_r((struct _reent*)NULL, fildes, buf, nbyte);

    return ret;
}

int write(int fildes, const void *buf, size_t nbyte)
{
    int ret;

    ret = _write_r((struct _reent*)NULL, fildes, buf, nbyte);

    return ret;
}

int open(const char * file, int flags, ...)
{
    va_list a_list;
    va_start(a_list, flags);

    int mode;
    int ret;
    mode=va_arg(a_list, u32);
    ret = _open_r((struct _reent*)NULL, file, flags, mode);
    va_end(a_list);
    return ret;
}

int stat(const char * file, struct stat * buf)
{
    int ret;

    ret = _stat_r((struct _reent*)NULL, file, buf);

    return ret;
}

int link(const char * old, const char *new)
{
    int ret;

    ret = _link_r((struct _reent*)NULL, old, new);

    return ret;
}

int unlink(const char * name)
{
    int ret;

    ret = _unlink((struct _reent*)NULL, name);

    return ret;
}

int wait(int * status)
{
    int ret;

    ret = _wait((struct _reent*)NULL, status);

    return ret;
}

int execve(const char * path, char * const argv[], char * const env[])
{
    int ret;

    ret = _execve_r((struct _reent*)NULL, (char*)path, (char**)argv, (char**)env);

    return ret;
}

int fork()
{
    int ret;

    ret = _fork_r((struct _reent*)NULL);

    return ret;
}

int getpid()
{
    int ret;

    ret = _getpid_r((struct _reent*)NULL);

    return ret;
}

int kill(int pid, int sig)
{
    int ret;

    ret = _kill_r((struct _reent*)NULL, pid, sig);

    return ret;
}

clock_t times(struct tms * buf)
{
    int ret;
    ret = _times_r((struct _reent*)NULL, buf);

    return ret;
}

void __cxa_pure_virtual(void) {
    printf("\nError - pure virtual function called\n");
    exit(-1);
}

void *__dso_handle = (void *)0xCAFEBABE;
