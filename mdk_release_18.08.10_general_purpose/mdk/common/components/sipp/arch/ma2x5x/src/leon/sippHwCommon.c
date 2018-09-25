///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

/////////////////////////////////////////////////////////////////////////////////
//  Local macros
/////////////////////////////////////////////////////////////////////////////////

#define SIPP_CQ_ADD_WRITE(qu, address, val) { \
                                                qu->quEntry[qu->quNum].addr  = address; \
                                                qu->quEntry[qu->quNum].value = val; \
                                                qu->quNum++; \
                                            }


//WARNING: SIPP filters consider BASE starting from SLICE0, then START/FIRST offsets
//         get added to get the actual address; Therefore resolve the base addresses
//         starting from SLICE0 always.

//########################################################################################
// Compact and initialize buffer definitions
// We need to do this as on context switch we'll just copy the params
// parNo can be 0 or 1. Some HW filters have a single parent, others have 2
//########################################################################################
u32 sippIbufSetup (SippFilter *fptr, u32 parNo)
{
    SippHwBuf  * iBuf;
    SippFilter * par     = fptr->parents[parNo];
    s32          oBufIdx = fptr->parentOBufIdx[parNo];
    u32          totalBits;

    if (par->nLines[oBufIdx] > 0)
    {
        // Allocate some mem for the input buffer description
        fptr->iBuf[parNo] = (SippHwBuf *)sippMemAlloc (&fptr->pPipe->tHeapMCB,
                                                       vPoolSchedule,
                                                       sizeof(SippHwBuf));

        if (fptr->iBuf[parNo] == (SippHwBuf *)NULL)
        {
            return 1;
        }

        //Config the Ibuf
        iBuf       = fptr->iBuf[parNo];
        totalBits  = par->hPadding[oBufIdx] * par->bpp[oBufIdx];
        totalBits += 0x7;
        totalBits &= 0xFFFFFFF8;
        iBuf->base = (u32)par->outputBuffer[oBufIdx] + (totalBits >> 3);

        iBuf->cfg  = par->nLines[oBufIdx]                               | // number of lines
                     ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET)      | // number of planes
                     ((1<<(par->bpp[oBufIdx] >> 4)) << SIPP_FO_OFFSET);   // format

        totalBits  = par->lineStride[oBufIdx] * par->bpp[oBufIdx];
        totalBits += 0x7;
        totalBits &= 0xFFFFFFF8;
        iBuf->ls   = totalBits >> 3;

        totalBits  = par->planeStride[oBufIdx] * par->bpp[oBufIdx];
        totalBits += 0x7;
        totalBits &= 0xFFFFFFF8;
        iBuf->ps   = totalBits >> 3;
        iBuf->ctx  = 0;

        // Enable interrupt after each output line (0) and program line chunk
        totalBits     = par->sliceWidth * par->bpp[oBufIdx];
        totalBits    += 0x7;
        totalBits    &= 0xFFFFFFF8;

        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (fptr->pPipe->pfnSippRuntime == sippOPipeRuntime)
        {
            iBuf->irqRate = (par->oBufs[oBufIdx]->manageReq) ? fptr->irqRatePow : 0;
        }
        else
        #endif
        {
            #ifdef SIPP_SLICE0_ADDRESS
            u32 firstOutSlc = par->firstOutSlc;
            #else
            u32 firstOutSlc = par->firstOutSlc - par->gi->sliceFirst;
            #endif

            iBuf->irqRate = ((totalBits >> 3) << 16) |
                            (firstOutSlc <<  8);
        }
    }
    else
    {
        fptr->iBuf[parNo] = (SippHwBuf *)NULL;
    }

    return 0;
}

//########################################################################################
// Typical OUTPUT buffer setup for a HW module
//########################################################################################
u32 sippObufSetup (SippFilter *fptr, u32 oBufIdx)
{
    SippHwBuf * oBuf;
    u32         numLinesIter     = (fptr->linesPerIter >> 1);
    u32         numLinesIterLog2 = 0;
    u32         totalBits;

    while (numLinesIter)
    {
        numLinesIterLog2++;
        numLinesIter >>= 0x1;
    }

    if (fptr->nLines[oBufIdx] > 0)
    {
        // Allocate some mem for the input buffer description
        fptr->oBuf[oBufIdx] = (SippHwBuf *)sippMemAlloc (&fptr->pPipe->tHeapMCB,
                                                         vPoolSchedule,
                                                         sizeof(SippHwBuf));

        if (fptr->oBuf[oBufIdx] == (SippHwBuf *)NULL)
        {
            return 1;
        }

        // Config the output buffer
        oBuf       = fptr->oBuf[oBufIdx];

        totalBits  = fptr->hPadding[oBufIdx] * fptr->bpp[oBufIdx];
        totalBits += 0x7;
        totalBits &= 0xFFFFFFF8;
        oBuf->base = (u32)fptr->outputBuffer[oBufIdx] + (totalBits >> 3);

        oBuf->cfg  = fptr->nLines[oBufIdx]                              | // number of lines
                     ((fptr->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET)     | // number of planes
                     ((1<<(fptr->bpp[oBufIdx] >> 4)) << SIPP_FO_OFFSET);  // format

        totalBits  = fptr->lineStride[oBufIdx] * fptr->bpp[oBufIdx];
        totalBits += 0x7;
        totalBits &= 0xFFFFFFF8;
        oBuf->ls   = totalBits >> 3;

        totalBits  = fptr->planeStride[oBufIdx] * fptr->bpp[oBufIdx];
        totalBits += 0x7;
        totalBits &= 0xFFFFFFF8;
        oBuf->ps   = totalBits >> 3;

        oBuf->ctx  = 0;

        // Enable interrupt after each output line (0) and program line chunk, first output slice
        totalBits     = fptr->sliceWidth * fptr->bpp[oBufIdx];
        totalBits    += 0x7;
        totalBits    &= 0xFFFFFFF8;

        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (fptr->pPipe->pfnSippRuntime == sippOPipeRuntime)
        {
            oBuf->irqRate = (fptr->oBufs[oBufIdx]->manageReq) ? fptr->irqRatePow : 0;
        }
        else
        #endif
        {
            #ifdef SIPP_SLICE0_ADDRESS
            u32 firstOutSlc = fptr->firstOutSlc;
            #else
            u32 firstOutSlc = fptr->firstOutSlc - fptr->gi->sliceFirst;
            #endif

            oBuf->irqRate = ((totalBits >> 3) << 16) |
                            (firstOutSlc <<  8) |
                            numLinesIterLog2;
        }
    }
    else
    {
        fptr->oBuf[oBufIdx] = (SippHwBuf *)NULL;
    }

    return 0;
}

//########################################################################################
// Allow the irq rate to be change for a specified buffer
//########################################################################################

u32 sippBufSetupIrqRate (SippHwBuf * buf,
                         u32         oBuf,
                         u32         bufId,
                         u32         numLinesPerIrq)
{
    SippHwBuf * targetBuf;
    u32         numLinesIter     = (numLinesPerIrq >> 1);
    u32         numLinesIterLog2 = 0;

    while (numLinesIter)
    {
        numLinesIterLog2++;
        numLinesIter >>= 0x1;
    }

    buf->irqRate &= (~(SIPP_BUFF_IRQ_RATE_MASK));
    buf->irqRate |= numLinesIterLog2;

    // Write this to the register
    if (oBuf)
    {
        targetBuf = (SippHwBuf *)(SIPP_OBUF0_OFF + (bufId * 0x01c));
    }
    else
    {
        targetBuf = (SippHwBuf *)(SIPP_IBUF0_OFF + (bufId * 0x01c));
    }

    SET_REG_WORD((u32)&targetBuf->irqRate, buf->irqRate);

    return 0;
}

u32 sippBufSetupIrqRateCQ (SippHwBuf * buf,
                           u32         oBuf,
                           u32         bufId,
                           u32         numLinesPerIrq,
                           psSippCMDQu pQu)
{
    SippHwBuf * targetBuf;
    u32         numLinesIter     = (numLinesPerIrq >> 1);
    u32         numLinesIterLog2 = 0;

    while (numLinesIter)
    {
        numLinesIterLog2++;
        numLinesIter >>= 0x1;
    }

    buf->irqRate &= (~(SIPP_BUFF_IRQ_RATE_MASK));
    buf->irqRate |= numLinesIterLog2;

    // Write this to the register
    if (oBuf)
    {
        targetBuf = (SippHwBuf *)(SIPP_OBUF0_OFF + (bufId * 0x01c));
    }
    else
    {
        targetBuf = (SippHwBuf *)(SIPP_IBUF0_OFF + (bufId * 0x01c));
    }

    SIPP_CQ_ADD_WRITE(pQu, (u32)&targetBuf->irqRate, buf->irqRate);

    return 0;
}

u32 sippBufGetObufCtx (SippFilter * fptr, u32 oBufIdx)
{
    u32 oBuf = fptr->unit;

    if (fptr->unit == SIPP_DBYR_ID)
    {
        DbyrParam * param      = (DbyrParam *)fptr->params;

        // Set the bit pos of a debayer filter output buffer interrupt(s)
        if (oBufIdx == 0)
        {
            if (param->thresh & DEBAYER_RGB_EN(0x1))
            {
                oBuf = SIPP_DBYR_ID;
            }
            else
            {
                oBuf = SIPP_DBYR_LUMA_ID;
            }
        }

        if (oBufIdx == 1)
        {
            oBuf = SIPP_DBYR_LUMA_ID;
        }
    }

    return (SIPP_OBUF0_OFF + (oBuf * 0x01c) + 0x018);
}


//#######################################################################################
//Need to use centralized SET_REG_WORD/GET_REG_WORD, as they will refer to different
//units; which get decoded based on ADDR range. (e.g. sipp, bicubic)

//Note: on SPARC, SET_REG_WORD is already defined ...
//      on Shave, it makes no sense, so only let definition for Windows-Leon

#if defined(SIPP_PC)
#include "CmxDma.h"
#include "sippHwM2Factory.h"
#include "wrapperSem.h"

// Interrupt service routine callback
void sippGenericPCRuntimeIsr (void *);
void sippGenericRuntimeAsyncCreateIsrThreads ();

CmxDmaCtrlr cmxDma;
SippHwFactory * sippHwAcc;
FILE * fpRegAcc;

Semaphore * serialiseSem;
Semaphore * hwSyncSem;
Semaphore * svuSyncSem;
Semaphore * cmxDmaSyncSem;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void initPCHwModels ()
{
    extern u8 *sippCmxBase;

    // Tell CMXDMA model where base of CMX is:
    cmxDma.SetCmxBasePointer((void *)sippCmxBase);
    sippHwAcc     = new SippHwM2Factory(REV2, sippGenericPCRuntimeIsr);
    serialiseSem  = new Semaphore(1,1);
    hwSyncSem     = new Semaphore(0,1);
    svuSyncSem    = new Semaphore(0,1);
    cmxDmaSyncSem = new Semaphore(0,1);

    // Create the isr threads modelling async isr behaviour
    sippGenericRuntimeAsyncCreateIsrThreads ();

    /* Debug */
    fpRegAcc = fopen ("RegAccess.log", "w");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SET_REG_WORD(u32 addr, u32 value)
{
    // SIPP addr space?
    if ((addr >= SIPP_BASE0_ADR) &&
        (addr <= SIPP_RAW_RESET_CONTROL))
    {
        sippHwAcc->ApbWrite(addr, value);
    }

    /* Debug */
    if (fpRegAcc)
    {
        fprintf(fpRegAcc, "%08x %08x\n", addr, value);
        fflush(fpRegAcc);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
u32 GET_REG_WORD_VAL(u32 addr)
{
    // SIPP addr space?
    if ((addr >= SIPP_BASE0_ADR) &&
        (addr <= SIPP_RAW_RESET_CONTROL))
    {
        return (u32)sippHwAcc->ApbRead(addr);
    }

    sippError ((u32 *)NULL, E_PC_RUNTIME_FAILURE);
    return 0;
    //should never get here !
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SET_REG_DWORD(u32 addr, u64 value)
{
    //TBD: check addr range and invoke proper model WR
    cmxDma.AhbWrite(addr, value);
    /* Debug */
    if (fpRegAcc)
    {
        fprintf(fpRegAcc, "%08x %08x%08x\n", addr, (u32)((value>>32)&0xFFFFFFFF), (u32)(value&0xFFFFFFFF));
        fflush(fpRegAcc);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
u64 GET_REG_DWORD_VAL(u32 addr)
{
    u64 value = cmxDma.AhbRead(addr);

    return value;
}

#endif
