/// =====================================================================================
///
///        @file:      pipePrint.c
///        @brief:     IO Console over moviDebug pipe (see ""help pipe")
///        @copyright: All code copyright Movidius Ltd 2014, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

#include <sys/types.h>
#include <sys/reent.h>
#include <mv_types.h>


#ifndef SHAVEPIPEPRINT_SECTION
#define SHAVEPIPEPRINT_SECTION "S.ddr_direct.shaveprint"
#endif

#ifndef SHAVEPIPEPRINT_SIZE
#define SHAVEPIPEPRINT_SIZE (10*1024)
#endif

typedef struct {
    volatile u32  canaryStart;   // Used to detect corruption of queue
    volatile int  in;
    volatile int  out;
    volatile int  queueSize;
    volatile u32  canaryEnd;     // Used to detect corruption of queue
    volatile u8   buffer[SHAVEPIPEPRINT_SIZE];
} tyMvConsoleQueue;


tyMvConsoleQueue mvConsoleShaveTxQueue __attribute__((section(SHAVEPIPEPRINT_SECTION)))=
{
    .canaryStart = 0x11223344,
    .in          = 0,
    .out         = 0,
    .queueSize   = SHAVEPIPEPRINT_SIZE,
    .canaryEnd   = 0xAABBCCDD,
};

tyMvConsoleQueue mvConsoleShaveRxQueue __attribute__((section(SHAVEPIPEPRINT_SECTION)))=
{
    .canaryStart = 0x11223344,
    .in          = 0,
    .out         = 0,
    .queueSize   = SHAVEPIPEPRINT_SIZE,
    .canaryEnd   = 0xAABBCCDD,
};


static inline void * convertToUncachedAddr(void * addr)
{
    if ((u32)addr & 0x80000000)
        addr = (void*)((u32)addr | 0x40000000);
    else // Assume CMX
        addr = (void*)((u32)addr | 0x08000000);
    return addr;
}

// Blocking Queue Add
void mvShavePipePrintQueueAdd  (u8 val)
{
    tyMvConsoleQueue * const q = convertToUncachedAddr((void *)&mvConsoleShaveTxQueue);

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

// Blocking Queue Get
u8 mvShavePipePrintQueueGet  (void)
{
    tyMvConsoleQueue * q = convertToUncachedAddr((void *)&mvConsoleShaveRxQueue);
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

// write() calls _put_output_char() for stdout, stderr
void __attribute__((alias("mvShavePipePrintQueueAdd"))) _put_output_char(int c);

int read(int file, const void *buf, size_t nbyte)
{
    UNUSED(file); // hush the compiler warning.

    size_t i;

    for(i = 0 ; i < nbyte ; i++)
    {
        ((char*)buf)[i] = mvShavePipePrintQueueGet();
        if(((char*)buf)[i] == '\0')
            break;
    }

    return i;
}
