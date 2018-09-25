#include <string.h>
#include <assert.h>
#include <registersMyriad.h>
#include <DrvMipi.h>
#include <DrvMss.h>
#include <swcLeonUtils.h>
#include "TimeSyncMgr.h"
#include "PlgSrcIspApi.h"

#define HS_MIPI_CTRL_OFSET_ 0x100

extern void plgSrcIspCreateOpipe(PlgSrcIsp *obj, icSourceConfig  *sCfg);
extern void TryStartOpipe       (PlgSrcIsp *obj);

static        int32_t init(FramePool *outputPools, int nOutputPools, void *pluginObject);
static        int32_t fini(void *pluginObject);
static inline void    mipiPhyCfg(icSourceConfig *mipiRxConfiguration, PlgSrcIsp *obj);

volatile uint32_t ispfc = 0;
//##################################################################################
void PlgSrcIspCreate(void *pluginObject, icSourceInstance instId)
{
    PlgSrcIsp *object       = (PlgSrcIsp*)pluginObject;
    memset(object, 0, sizeof(PlgSrcIsp));

    object->plg.init        = init; //to associate output pool
    object->plg.fini        = fini; //to mark STOP
    object->plg.status      = PLG_STATS_IDLE;
    object->status          = PLG_SRC_ISP_CREATED;
    object->srcId           = instId;

    object->eofEvent = NULL;
    object->sofEvent = NULL;
    object->hitEvent = NULL;

    computePolyFirInit(&object->scale);
}

//Rx pipes are concurrent, so need to alloc per instance !
//(allocation amount is internally determined)
//This should be invoked only for Sources that map on SippRx HW block.
//Currently this is done via AllocOpipeRxCmxBuffs, which performs this check
void PlgSrcIspCmxAlloc(PlgSrcIsp *obj, icSourceSetup *src, MemPoolId id)
{
    void plgSrcIspOpipeAlloc(PlgSrcIsp *obj, MemPoolId id, icSourceSetup *src);
    plgSrcIspOpipeAlloc(obj, id, src);
}

//
void PlgSrcIspSetLineHit(void *pluginObject, uint32_t lineNo)
{
    PlgSrcIsp *obj = (PlgSrcIsp *)pluginObject;

    //Snap value to be multiple of IRQ-RATE, else won't be detected by Opipe
    obj->pRxIsp.p.targetLine[HIT_IDX] = (lineNo + (IRQ_RATE-1)) & (~(IRQ_RATE-1));
}

//##################################################################################
void PlgSrcIspStart(void *plgObject, icSourceConfig  *srcCfg)
{
    PlgSrcIsp *obj  = (PlgSrcIsp *)plgObject;
    obj->mipiRxData = srcCfg->mipiRxData;
    obj->plg.status = PLG_STATS_RUNNING;

    ispfc = 0;
    plgSrcIspCreateOpipe(obj, srcCfg);
    TryStartOpipe(obj);

#if !defined(MIPI_INTERNAL_LOOPBACK)
    mipiPhyCfg(srcCfg, obj);
#endif

    DrvMssConnectMipiToDevice(obj->mipiRxData.controllerNo, DRV_MSS_SIPP_RX);
}

static int32_t init(FramePool *outputPools, int nOutputPools, void *pluginObject)
{
    UNUSED(nOutputPools);
    PlgSrcIsp* obj = (PlgSrcIsp*)pluginObject;
    // this plugin produce just 1 output frame, so not take in consideration nOutputPools params,
    // as this have to be 1
    obj->outputPools = outputPools;
    return 0;
}
extern void OpipeHwReset(uint32_t mask);
//
static int32_t fini(void *pluginObject)
{
    PlgSrcIsp *obj = (PlgSrcIsp *)pluginObject;

    //Check if already stopped. Can occur if tearing down the pipe.
    if (obj->plg.status == PLG_STATS_IDLE){
        return 0;
    }

    // added in order to allow stop the mipiCtrl just in vertical period time, and when isp finish processing
    ispfc = 1;
    while(1 == ispfc) {
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
    }
    // disable controler in order to fix sensor hank after a reset
    CLR_REG_BITS_MASK(MIPI0_HS_CTRL_ADDR + obj->mipiRxData.controllerNo * HS_MIPI_CTRL_OFSET_ ,1);
    uint32_t disableCtrlMask = (1<<obj->mipiRxData.controllerNo);
    CLR_REG_BITS_MASK(MSS_MIPI_CFG_ADR, disableCtrlMask);

    // clear conection Mipi-SippRx
    CLR_REG_BITS_MASK(MSS_MIPI_SIPP_CFG_ADR, (0x3F<<(obj->mipiRxData.recNrl-IC_SIPP_DEVICE0)));

// reset sipp Rx to, in order to not remain old interrupts
    OpipeHwReset(obj->pRxIsp.p.enMask);
    obj->plg.status = PLG_STATS_IDLE;

    return 0;
}
//
static inline void mipiPhyCfg(icSourceConfig *mipiRxConfiguration, PlgSrcIsp *obj) {
    UNUSED(obj);
    // set mipi controler
    tDrvMipiControllerConfigStruct rxConfig;
    rxConfig.controllerNo      = mipiRxConfiguration->mipiRxData.controllerNo;
    rxConfig.type              = MIPI_CSI;
    rxConfig.direction         = MIPI_RX;
    rxConfig.noLanes           = mipiRxConfiguration->mipiRxData.noLanes;
    rxConfig.laneRateMbps      = mipiRxConfiguration->mipiRxData.laneRateMbps;
    rxConfig.rxPacket.dataMode = MIPI_D_MODE_0;
    rxConfig.rxPacket.dataType = mipiRxConfiguration->mipiRxData.dataType;
    rxConfig.width             = mipiRxConfiguration->cameraOutputSize.w;
    rxConfig.height            = mipiRxConfiguration->cameraOutputSize.h;
    //rxConfig->mipiCtrlCfg.refClkKHz         = mipiRxConfiguration->sensorSpec->idealRefFreq; // TODO: ask !
    assert(DRV_MIPI_CFG_DONE == DrvMipiInit(&rxConfig));
}



