///
/// @file      PlgSourceOpipe.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Source file opipe sipp Rx filter interface.
///            Platform(s) supported : ma2x5x
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <string.h>
#include <DrvTimer.h>
#include <assert.h>
#include <swcLeonUtils.h>
//TBD: find a place for these ...
#include "ipipe.h"
//#include "TimeSyncMgr.h"
//#include "ipipeDbg.h"
//#include "ipipeUtils.h"
#include "PlgSource.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#ifndef RX_CBUF_MUL_H //Opipe specific
#define RX_CBUF_MUL_H 5
#endif
//Required CMX circular buffer height
#ifndef RX_CBUFF_H
#define RX_CBUFF_H (RX_CBUF_MUL_H*IRQ_RATE)
#endif
// just to estimate a quantyty, the final values can be updated at applicayion level from Makefile Define
#ifndef PLG_SRC_MAX_W
#define PLG_SRC_MAX_W ((1936+16)*2 + 2032*2)
#endif
#ifndef PLG_SRC_MAX_SOURCES
#define PLG_SRC_MAX_SOURCES 2
#endif
#ifndef PLG_SRC_CIRC_BUF_SIZE
#define PLG_SRC_CIRC_BUF_SIZE (PLG_SRC_MAX_W*RX_CBUFF_H*PLG_SRC_MAX_SOURCES)
#endif

typedef struct {
    uint8_t *base;
    uint8_t *endPos;
    uint8_t *nxtPos;
} PlgSourceCircBufAlloc;


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
//SippRx pointers to circular output buffers in CMX
//out of 6 sources, we can have max 4 pointer set as there are 4 RX
uint8_t *cSippRx[] = {0,0,0,0,0,0};
uint8_t cBuffRx[PLG_SRC_CIRC_BUF_SIZE] SECTION(".cmx.opipe") ALIGNED(8);
PlgSourceCircBufAlloc plgSourceCircBufAlloc ={
                cBuffRx, &cBuffRx[PLG_SRC_CIRC_BUF_SIZE], cBuffRx
};

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------


//#####################################################################################
uint32_t lineWidthInBytes(uint32_t widthInPixels, uint32_t bpp) {
    uint32_t w = widthInPixels;
    switch(bpp)
    {
    case 10: w = w * 5 / 4; break; //10bit pack
    case 12: w = w * 3 / 2; break; //12bit pack
    case 16: w = w * 2;     break; //flat 16bit
    case  8:                break; //flat 8bit
    default: assert(0);     break; //invalid, (e.g.for 14bit must specify 16bit storage)
    }
    return w;
}

//#####################################################################################
static int mipiDataTypeBpp(int dataType) {
    switch (dataType){
    case IC_IPIPE_RAW_10:            return 10;
    case IC_IPIPE_RAW_12:            return 12;
    case IC_IPIPE_RAW_14:            return 14;
    default:                   assert(0);break; //Unsupported data type.
    }
}
//#####################################################################################




uint8_t* plgSrcAlocRxBufs(uint32_t size) {
    uint32_t sz = (size+0xF)&(~0xF);
    uint8_t *returnVal = NULL;
    uint8_t *nextNextPos =  sz+plgSourceCircBufAlloc.nxtPos;
    assert(nextNextPos < plgSourceCircBufAlloc.endPos);
    returnVal = plgSourceCircBufAlloc.nxtPos;
    plgSourceCircBufAlloc.nxtPos = nextNextPos;
    return returnVal;
}

void plgSrcPurgeCBufsMem(void) {
    plgSourceCircBufAlloc.nxtPos = plgSourceCircBufAlloc.base;
}


void plgSrcOpipeAlloc(uint32_t no, icSourceSetup *src) {
    //WARNING: all sources on same pool, to be fixed !!!
#ifdef WORKAROUND_FOR_FLIC_ON_M2_3xIMX208
    /* source was configured 10bpp unpacked, use maxBpp = 16 */
    uint32_t lineSz = lineWidthInBytes(src->maxWidth, 16);
#else
    uint32_t lineSz = lineWidthInBytes(src->maxWidth, src->maxBpp);
#endif
    cSippRx[no] = (uint8_t*)plgSrcAlocRxBufs(lineSz*RX_CBUFF_H);
}
uint32_t sss[4] = {0,0,0,0};
//#####################################################################################
//Opipe line hit callback;
//check line indices and trigger appropriate Plugin callbacks
void cbOpipeLineHit(Opipe *p) {
    PlgSource *plg    = (PlgSource *)p->params[0];

    if(plg->sofEvent && (p->currHitLine == p->targetLine[SOF_IDX_SRC])) {
        sss[0]++;
        rtems_event_send(plg->srcThread, RTEMS_EVENT_3);
        return;
    }

    if(plg->hitEvent && (p->currHitLine == p->targetLine[HIT_IDX_SRC])) {
        sss[1]++;
        rtems_event_send(plg->srcThread, RTEMS_EVENT_4);
    }
}



//#####################################################################################
//Opipe EOF callback: adjust associated Plugin Frame buffers
static void opipeIspEof(Opipe *p) {
    PlgSource *obj = (PlgSource *)p->params[0];
    sss[2]++;
    rtems_event_send(obj->srcThread, RTEMS_EVENT_2);
}

//#####################################################################################
//OpipeRx creation and initial configuration
void plgSrcCreateRxOpipe(PlgSource *obj, icSourceConfig  *sCfg, uint32_t outFmt)
{
    OpipeRx   *pRx   = &obj->pRx;
    MipiRxCfg *rxCfg = &obj->rxCfg;
    //Figure out resolution based on crop window
    uint32_t stX  = sCfg->cropWindow.x1;
    uint32_t stY  = sCfg->cropWindow.y1;
    uint32_t imgW = sCfg->cropWindow.x2 - sCfg->cropWindow.x1;
    uint32_t imgH = sCfg->cropWindow.y2 - sCfg->cropWindow.y1;
    //Figure out sipp ID:
    uint32_t rxNo = sCfg->mipiRxData.recNrl - IC_SIPP_DEVICE0;
    uint32_t rxID = SIPP_MIPI_RX0_ID + rxNo;


    if (PLG_SOURCE_CREATED == obj->status) {

        plgSrcOpipeAlloc(obj->srcId, obj->srcSetup);
        //At this stage, the circular buffer MUST have been allocated
        assert(cSippRx[obj->srcId] != 0);
        OpipeCreateRx(pRx, imgW, imgH,
                        rxID, outFmt,
                        (uint32_t)cSippRx[obj->srcId], RX_CBUFF_H);
        obj->status = PLG_SOURCE_INUSE;
        //Opipe Line-hit callbacks for current RX unit
        pRx->p.cbLineHit           = cbOpipeLineHit;
        pRx->p.triggerSinkId       = rxID;
        pRx->p.targetLine[SOF_IDX_SRC] = IRQ_RATE;
        pRx->p.targetLine[HIT_IDX_SRC] = obj->lineHitNo & (~0x7);
    }
    else {
        if(PLG_SOURCE_INUSE == obj->status) {
            //Assume that change resolution, so update it
            //Resolution updates (if needed)
            OpipeSetRes(&pRx->p, imgW, imgH);
            // not possible to change board design and to assign other source to other receiver
            assert(sCfg->mipiRxData.recNrl == obj->mipiRxData.recNrl);
        }
        else {
            // try to start a source but not create is first,
            // or a memory corruption overwrite plug-in structure
            assert(0);
        }
    }

    //Set param pointers after create (i.e. post OpipeInit)
    pRx->p.pMipiRxCfg[rxNo] = rxCfg;
    pRx->p.cbEndOfFrame     = opipeIspEof;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Configs must be set BEFORE starting the filter !!!
    pRx->p.rawBits      = sCfg->bitsPerPixel;
    pRx->p.bayerPattern = sCfg->bayerFormat;

    //Enable packed output
    if((outFmt == SIPP_FMT_PACK10)||(outFmt == SIPP_FMT_PACK12)) {
        printf("pack pac\n");
        rxCfg->cfg = 1<<3;
    }

    //single input bayer plane maps on window-0
#define RX_WIN 0
    rxCfg->xStartWidth [RX_WIN] = (imgW<<16) | stX;
    rxCfg->yStartHeight[RX_WIN] = (imgH<<16) | stY;
    rxCfg->sel01                =  0xF<<8; //WIN0 selection : all even/odd lines and columns

    // Receiver can optionally downshift.
    obj->downshift = mipiDataTypeBpp(sCfg->mipiRxData.dataType) - sCfg->bitsPerPixel;
    if(obj->downshift) {
        printf("obj->downshift = %d \n", obj->downshift);
    }
    obj->downshift = 0;
    assert(obj->downshift >= 0); //TODO: Handle promotion.

    rxCfg->mask[RX_WIN]         = (1<<sCfg->bitsPerPixel)-1;
}
