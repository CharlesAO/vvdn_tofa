/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2016 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys_write.c
 *  Description:   Skeleton for system call '_write'
 *    The '_write' system function and helper for outputting raw data to the UART.
 *    '_write' is used by functions like 'printf' to output to files and streams.
 *  --------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

//This coude should only activate for MA2480 builds
#ifdef __ma2480__

// ---------------------------------------------------------------------------
// Myriad2 Register Definitions
// ---------------------------------------------------------------------------

#define LHB_ROM_BASE_ADR                (0x7FF00000)
#define DEBUG_MSG_ADR                   (LHB_ROM_BASE_ADR + 0x00007FF8 + 0x00010000) 

#define CPR_BASE_ADR                    (0x20300000)
#define CPR_DETECT_HW_SIM_REGISTER      (CPR_BASE_ADR + 0x0000011c)

#define DW_UART_BASE_ADDR               (0x20180000)
#define DW_UART_USR_ADDR                (DW_UART_BASE_ADDR + 0x7C)
#define DW_UART_THR_ADDR                (DW_UART_BASE_ADDR + 0x00)
#define DW_UART_TFL_ADDR                (DW_UART_BASE_ADDR + 0x80)
#define DW_UART_RFL_ADDR                (DW_UART_BASE_ADDR + 0x84)

#define VCS_SIMULATION_MARKER           (1)

#define DW_UART_RX_FIFO_SIZE            (64)
#define DW_UART_USR_TFNF                (1 << 1)


// ---------------------------------------------------------------------------
// Register access macros
// ---------------------------------------------------------------------------
#define READ_REGISTER_32(addr)          (*(volatile unsigned int *)(((unsigned int)(addr)))) 
#define WRITE_REGISTER_32(addr,value)   ((void)(*(volatile unsigned int *)(((unsigned int )(addr))) = (unsigned int)(value))) 


// Raw character write routine for SHAVE's UART
__inline void __attribute((always_inline)) __raw_output_char_overload(int ch)
{
    // In VCS we don't use the UART as a loopback adapter, we have a magic infinite depth register to handle putchar quickly
    if ((( READ_REGISTER_32 ( CPR_DETECT_HW_SIM_REGISTER ) >> 29 ) & 0x7 ) == VCS_SIMULATION_MARKER )
        WRITE_REGISTER_32 ( DEBUG_MSG_ADR, ch & 0xFF );
    else
    {   // Check that there is space in the TX FIFO */
        while (( READ_REGISTER_32 ( DW_UART_USR_ADDR ) & DW_UART_USR_TFNF ) == 0 )
            ;   // Wait until the FIFO is not full

        // Assumes the UART is configured in loopback mode.  Checking that
        // there is enough free space.  Limitation: if RX FIFO is full, sent
        // bytes from TX will be lost.
        while (( READ_REGISTER_32 ( DW_UART_TFL_ADDR ) + READ_REGISTER_32 ( DW_UART_RFL_ADDR ) + 2 ) >= DW_UART_RX_FIFO_SIZE )
            ;   // Additional check for default case of loopback mode

        WRITE_REGISTER_32 ( DW_UART_THR_ADDR, ch & 0xFF );
    }
}


// The implementation of '_write' will only work if the selected output
// file-handle is for 'stdout' or 'stderr.  Since SHAVE does not support
// a file-system all other file handles will fail.
//
// This also does not take into account that the programmer may have
// attempted to rebind the usual file-handles for 'stdout' and 'stderr'
// to another kind of stream.
int write(int file, const void* p, size_t len)
{
    if ((p != NULL) && ((file == STDOUT_FILENO) || (file == STDERR_FILENO)))
    {
        size_t i;
        char* ptr = (char*)p;

        for (i = 0; i < len && *ptr != 0; i++){
            __raw_output_char_overload(*ptr++);
        }

        return i;
    }

    errno = EBADF;
    return -1;
}

#endif //__ma2480__
