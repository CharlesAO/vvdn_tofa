/// =====================================================================================
///
///        @file:      pipePrint.c
///        @brief:     IO Console over moviDebug pipe (see ""help pipe")
///        @copyright: All code copyright Movidius Ltd 2014, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///
#ifdef __RTEMS__
#include <rtems.h>
#include <rtems/bspIo.h>
#else
#include <sys/types.h>
#include <sys/reent.h>
#endif
#include <mv_types.h>

#ifndef PIPEPRINT_SECTION
#define PIPEPRINT_SECTION ".ddr_direct.data"
#endif

#ifndef PIPEPRINT_SIZE
#define PIPEPRINT_SIZE (50*1024)
#endif

typedef struct {
    volatile u32  canaryStart;   // Used to detect corruption of queue
    volatile int  in;
    volatile int  out;
    volatile int  queueSize;
    volatile u32  canaryEnd;     // Used to detect corruption of queue
    volatile u8   buffer[PIPEPRINT_SIZE];
} tyMvConsoleQueue;

tyMvConsoleQueue mvConsoleTxQueue __attribute__((section(PIPEPRINT_SECTION)))=
{
    .canaryStart = 0x11223344,
    .in          = 0,
    .out         = 0,
    .queueSize   = PIPEPRINT_SIZE,
    .canaryEnd   = 0xAABBCCDD,
    .buffer      = {0},
};

tyMvConsoleQueue mvConsoleRxQueue __attribute__((section(PIPEPRINT_SECTION)))=
{
    .canaryStart = 0x11223344,
    .in          = 0,
    .out         = 0,
    .queueSize   = PIPEPRINT_SIZE,
    .canaryEnd   = 0xAABBCCDD,
    .buffer      = {0},
};

static inline void * convertToUncachedAddr(void * addr)
{
    if ((u32)addr & 0x80000000)
        addr = (void*)((u32)addr | 0x40000000);
    else // Assume CMX
        addr = (void*)((u32)addr | 0x08000000);
    return addr;
}

// (Optionally) Blocking Queue Add
static
void mvQueueAdd(tyMvConsoleQueue * qPtr, u8 val)
{
    tyMvConsoleQueue * const q = convertToUncachedAddr(qPtr);

    const int qsz = q->queueSize;
    const int qin = q->in;

    const int qinext = (qin + 1) % qsz;

#ifdef PIPEPRINT_WAIT_DEBUG
    do // Wait for space in the Q
    {
    } while (qinext == q->out);
#endif

    q->buffer[qin] = val;
    q->in = qinext;

#ifndef PIPEPRINT_WAIT_DEBUG
    const int qout = q->out;
    if (qout != qinext) {
        return;
    }
    q->out = (qout + 1) % qsz;
#endif
}

// (Optionally) Blocking Queue Get
static
int mvQueueGet(tyMvConsoleQueue *qPtr)
{
    tyMvConsoleQueue * q = convertToUncachedAddr(qPtr);
    const int qout = q->out;
    if (q->in == qout)
    {
#ifdef PIPEPRINT_WAIT_DEBUG
        do // Wait for data in the Q
        {
        } while (q->in == qout);
#else
        return -1;
#endif
    }
    const u8 val = q->buffer[qout];
    q->out = (qout + 1) % q->queueSize;
    return val;
}

#ifdef __RTEMS__

static int buff_debug_init(void)
{
    return 0;
}

static void buff_out_char(char chr)
{
    mvQueueAdd(&mvConsoleTxQueue, chr);
}

static int buff_in_char(void)
{
    return mvQueueGet(&mvConsoleRxQueue);
}
BSP_debug_configuration_type  BSP_debug_configuration = buff_debug_init;
BSP_output_char_function_type BSP_output_char         = buff_out_char;
BSP_polling_getchar_function_type BSP_poll_char       = buff_in_char;

#else

int _write_r(struct _reent * unused,int fildes, const void *buf, size_t nbyte)
{
    char *str = (char *)buf;
    size_t i;

    UNUSED(unused);
    UNUSED(fildes);

    for(i = 0 ; i < nbyte ; i++)
    {
        mvQueueAdd(&mvConsoleTxQueue, str[i]);
    }

    return i;
}

int _read_r(struct _reent * unused,int fildes, void *buf, size_t nbyte)
{
    UNUSED(unused); // hush the compiler warning.
    UNUSED(fildes); // hush the compiler warning.

    size_t i;
    char *p = buf;

    for(i = 0 ; i < nbyte ; i++)
    {
        int i = mvQueueGet(&mvConsoleRxQueue);
        if (i == -1)
            break;
        *p++ = (char)i;
    }

    return i;
}

#endif
