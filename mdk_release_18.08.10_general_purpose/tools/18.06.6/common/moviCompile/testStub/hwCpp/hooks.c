// These should really be '#include "shave_syscalls.h" - see 'common/moviCompile/src/syscalls/README.txt'
#include <stdint.h>
#include <errno.h>
#include <reent.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <_syslist.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


/* 'errno' is used for error handling in the ISO C library */
#undef errno
extern int errno;


//#define NOINSTR __attribute__((no_sanitize))
#define NOINSTR



// ============================================================================
// From 'sys_write.c'
//

// ---------------------------------------------------------------------------
// Myriad2 Register Definitions
// ---------------------------------------------------------------------------
#if __myriad2__ < 3
// Definitions for Myriad2.1 and Myriad2.2
#define CPR_BASE_ADR                    (0x10F00000)
#define CPR_DETECT_HW_SIM_REGISTER      (CPR_BASE_ADR + 0x00000114)

#define DW_UART0_BASE_ADDR              (0x20E00000)
#else
// Definitions for Myriad2.3
#define CPR_BASE_ADR                    (0x20300000)
#define CPR_DETECT_HW_SIM_REGISTER      (CPR_BASE_ADR + 0x0000011C)

#define DW_UART0_BASE_ADDR              (0x20180000)
#endif // __myriad2__

#define VCS_SIMULATION_MARKER           (1)

#define LHB_ROM_BASE_ADR                (0x7FF00000)
#define DEBUG_MSG_ADR                   (LHB_ROM_BASE_ADR + 0x00007FF8 + 0x00010000)

#define DW_UART0_USR_ADDR               (DW_UART0_BASE_ADDR + 0x7C)
#define DW_UART0_THR_ADDR               (DW_UART0_BASE_ADDR + 0x00)
#define DW_UART0_TFL_ADDR               (DW_UART0_BASE_ADDR + 0x80)
#define DW_UART0_RFL_ADDR               (DW_UART0_BASE_ADDR + 0x84)

#define DW_UART0_RX_FIFO_SIZE           (64)
#define DW_UART0_USR_TFNF               (1 << 1)


// ---------------------------------------------------------------------------
// Register access macros
// ---------------------------------------------------------------------------
#define READ_REGISTER_32(addr)          (*(volatile unsigned int *)(((unsigned int)(addr))))
#define WRITE_REGISTER_32(addr,value)   ((void)(*(volatile unsigned int *)(((unsigned int )(addr))) = (unsigned int)(value)))


// Raw character write routine for SHAVE's UART
__inline void __attribute__((always_inline)) __raw_output_char(int ch)
{
    // In VCS we don't use the UART as a loopback adapter, we have a magic infinite depth register to handle putchar quickly
    if ((( READ_REGISTER_32 ( CPR_DETECT_HW_SIM_REGISTER ) >> 29 ) & 0x7 ) == VCS_SIMULATION_MARKER )
        WRITE_REGISTER_32 ( DEBUG_MSG_ADR, ch & 0xFF );
    else
    {   // Check that there is space in the TX FIFO */
        while (( READ_REGISTER_32 ( DW_UART0_USR_ADDR ) & DW_UART0_USR_TFNF ) == 0 )
            ;   // Wait until the FIFO is not full

        // Assumes the UART is configured in loopback mode.  Checking that
        // there is enough free space.  Limitation: if RX FIFO is full, sent
        // bytes from TX will be lost.
        while (( READ_REGISTER_32 ( DW_UART0_TFL_ADDR ) + READ_REGISTER_32 ( DW_UART0_RFL_ADDR ) + 2 ) >= DW_UART0_RX_FIFO_SIZE )
            ;   // Additional check for default case of loopback mode

        WRITE_REGISTER_32 ( DW_UART0_THR_ADDR, ch & 0xFF );
    }
}


// The implementation of '_write' will only work if the selected output
// file-handle is for 'stdout' or 'stderr.  Since SHAVE does not support
// a file-system all other file handles will fail.
//
// This also does not take into account that the programmer may have
// attempted to rebind the usual file-handles for 'stdout' and 'stderr'
// to another kind of stream.
NOINSTR int _write(int file, const void* p, size_t len)
{
    if ((p != NULL) && ((file == STDOUT_FILENO) || (file == STDERR_FILENO)))
    {
        int i;
        char* ptr = (char*)p;

        for (i = 0; i < len && *ptr != 0; i++)
            __raw_output_char(*ptr++);

        return i;
    }

    // The PGO file uses handle '42'
    if (file == 42)
    {
        for (const char* p = "In PGO '_write' for handle '42'\n"; *p != '\0'; p++)
            __raw_output_char(*p);

        return (int)len;
    }

    errno = EBADF;
    return -1;
}


// ============================================================================
// From 'sys_open.c'
//
// Opens the specified file. Since there is no file-system on SHAVE, this will
// always fail.
NOINSTR int _open(const char* name, int flags, int mode)
{
    for (const char* p = "In '_open' for PGO; request to open file '"; *p != '\0'; p++)
        __raw_output_char(*p);
    for (const char* p = name; *p != '\0'; p++)
        __raw_output_char(*p);
    for (const char* p = "'\n"; *p != '\0'; p++)
        __raw_output_char(*p);

    if (strcmp(name, "default.profraw") == 0)
    {
        // Use handle '42' for the PGO file
        return 42;
    }
    else
    {
        errno = EACCES;
        return -1;
    }
}

// ============================================================================
// From 'sys_close.c'
//
// Close the specified file. Since there is no file system on SHAVE, this will always fail.
NOINSTR int _close(int file)
{
    // The PGO file uses handle '42'
    if (file == 42)
    {
        for (const char* p = "In PGO '_close' for handle '42'\n"; *p != '\0'; p++)
            __raw_output_char(*p);

        return 0;
    }

    errno = EBADF;
    return -1;
}
