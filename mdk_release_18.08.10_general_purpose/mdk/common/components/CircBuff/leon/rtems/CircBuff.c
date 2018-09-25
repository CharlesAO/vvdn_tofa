#include <assert.h>
#include "CircBuff.h"

// Align to 16 bytes
#define D_ALIGN(x) (((x) + 15) & (~15))

//=====================================================================
// "totLen" = len per run (multiple of lineW)
int32_t CircBuffInit(CBuff *cb, TransferDir dir,
              uint32_t cmxAddr, uint32_t    ddrAddr,
              uint32_t cmxH   , uint32_t    ddrH,
              uint32_t totLen , uint32_t    lineW)
{
    int32_t status;

    assert((cmxAddr & 0xF)==0); //cmx buff must be aligned
    cb->dir          = dir;
    cb->cmx.base     = cmxAddr;
    cb->cmx.height   = cmxH;
    cb->ddr.base     = ddrAddr;
    cb->ddr.height   = ddrH;
    cb->lnPerRun     = totLen / lineW;
    cb->cpLines      = 0;
    cb->width        = lineW;
    cb->ddr.lnStride = lineW;
    cb->ddr.curLine  = 0;
    cb->cmx.lnStride = D_ALIGN(lineW);
    cb->cmx.curLine  = 0;

    status = OsDrvCmxDmaCreateStrideTransaction( &cb->h,      &cb->t,
                                            (uint8_t*)1, (uint8_t*)1, // src/dst addr are set later properly
                                                  lineW,       lineW, // src/dst width
                                                  lineW,       lineW, // src/dst stride
                                                 totLen);
    return status;
}

//=====================================================================
// Kick a transfer and rool line indices for next run
int32_t CircBuffFeed(CBuff *cb)
{
    //-pick next CDMA handle
    //-update src/dst
    //-kick DMA

    int32_t status;

    uint32_t ddrAddr = cb->ddr.base + cb->ddr.curLine * cb->ddr.lnStride;
    uint32_t cmxAddr = cb->cmx.base + cb->cmx.curLine * cb->cmx.lnStride;

    #if defined(CBUFF_PAD)
        //add magic padding at each group of LnPerRun lines
        cmxAddr += (cb->cmx.curLine / cb->lnPerRun) * cb->cmxPad;
    #endif

    if(cb->dir == Cmx2Ddr) {
        cb->t.dst = (void*)ddrAddr;
        cb->t.src = (void*)cmxAddr;
    }
    else {
        cb->t.dst = (void*)cmxAddr;
        cb->t.src = (void*)ddrAddr;
    }

    status = OsDrvCmxDmaStartTransfer(&cb->h, 1);

    // Update for next run
    cb->ddr.curLine += cb->lnPerRun;      // ddr
    cb->cmx.curLine += cb->lnPerRun;      // cmx
    if(cb->cmx.curLine >= cb->cmx.height) // cmx
        cb->cmx.curLine = 0;              // cmx

    return status;
}
