///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Standard I/O Library header
///
/// The stdio library is implemented as using #defines to link to approriate Uart driver functions
///

#ifndef STDIO_H
#define STDIO_H

// 1: Includes
// ----------------------------------------------------------------------------
#include <stddef.h>
#include <DrvUart.h>
#ifdef __RTEMS__
  #include <sparc-elf/include/stdio.h>
#else

#define DONTWRITE_STD_LIBC
// 2: Types
// ----------------------------------------------------------------------------
#ifndef DONTWRITE_STD_LIBC
typedef int FILE;
#endif

// 3:  External prototypes
// ----------------------------------------------------------------------------
int _xprintf( int (*)(int), char*, const char*,... );

// 4:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
#ifndef DONTWRITE_STD_LIBC
#define stdin ((FILE*)1)
#define stdout ((FILE*)2)
#endif

// 5:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

// Optionally disable all system printf operations
// Note: The SYSTEM_PUT_CHAR_FUNCTION Macro atomatically resolves the appropriate
// means for displaying a character on the target platform. (e.g. Board, Software Sim, Hardware sim)
#ifdef NO_PRINT
#   define mvPrintf(...) (void)0
#   define mvPutchar(c)  (void)0
#   define mvPutc(c,f)   (void)0
#   define mvFputc(c,f)  (void)0
#   define mvFputs(s,f)  (void)0
#   define mvFflush(f)   (void)0
#else
#   define mvPrintf(...) (_xprintf(SYSTEM_PUTCHAR_FUNCTION , 0, __VA_ARGS__ ) )
#   define mvPutchar(c)      (SYSTEM_PUTCHAR_FUNCTION(c))
#   define mvPutc(c,f)       (((void)(f)),DrvApbUartPutChar(c))
#   define mvFputc(c,f)      (((void)(f)),DrvApbUartPutChar(c))
#   define mvFputs(s,f)      (((void)(f)),UART_fputs(s))
#   define mvFflush(f)       (((void)(f)),DrvApbUartFlush())
#endif

#ifndef DONTWRITE_STD_LIBC

#define getchar()       (UART_getchar())
#define getc(f)         (((void)(f)),UART_getchar())
#define fgetc(f)        (((void)(f)),UART_fgetc())
#define gets(s)         (UART_gets(s))
#define fgets(s,f)      (((void)(f)),UART_fgets(s))
#define sprintf(s,...)  _xprintf(0,(s),__VA_ARGS__)

#endif

int systemPuts(int (*systemPutCharFn)(int character) ,const char * str);
#endif // __RTEMS__

#endif // STDIO_H
