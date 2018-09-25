/**************************************************************************************************

 @File         : PlgSource.c
 @Author       : AL
 @Brief        : Contain Source configuration plug-in interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :

 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "TimeSyncMgr.h"
#include "PlgSourceApi.h"
#include "FrameMgrApi.h"
#include <string.h>
#include <DrvTimer.h>
#include "swcLeonUtils.h"
#include "Opipe.h"
#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include "swcWhoAmI.h"
#include "DrvCpr.h"
#include "DrvGpio.h"
#include "DrvMipi.h"
#include "DrvCif.h"
#include "DrvMss.h"
#include "assert.h"
#include "ipipeDbg.h"
#include "PlgSourceCif.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/
#define D_MIPI_CSI_FRAME_START          0x00
#define D_MIPI_CSI_FRAME_END            0x01
#define D_MIPI_IRQ_STAT_HS_RX_BITS_POS  14
#define D_CIF_CTRL_WINDOW_DISAB_MASK    ((u32)~(D_CIF_CTRL_WINDOW_EN))
/**************************************************************************************************
 ~~~  Basic typedefs
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/
static PlgSource *plugSourcesLc[DRV_MIPI_NO_CTRL];
uint32_t mipiCtrlIntStatus = 0;

/**************************************************************************************************
 ~~~ Imported functions declarations
 **************************************************************************************************/
extern void plgSrcCreateRxOpipe(PlgSource *obj, icSourceConfig  *sCfg, uint32_t outFmt);


/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
static        int32_t init(FramePool *outputPools, int nOutputPools, void *pluginObject);
static        int32_t fini(void *pluginObject);
static inline void    mipiPhyCfg(icSourceConfig *mipiRxConfiguration, PlgSource *obj);
static        void    setupMipiIrqHandler(uint32_t unused);
static        void    sourceSetupMipiIrq(uint32_t MipiCtrlNo);


/**************************************************************************************************
 ~~~ Exported Functions Implementation
 **************************************************************************************************/
//
void PlgSourceCreate(void *pluginObject, icSourceInstance instId)
{
    PlgSource *object           = (PlgSource*)pluginObject;
    // init hw things, or all the init side that not need params,
    object->plg.init            = init; //to associate output pool
    object->plg.fini            = fini; //to mark STOP
    object->plg.status          = PLG_STATS_IDLE;
    object->status              = PLG_SOURCE_CREATED;
    object->srcId               = instId;
    object->conectedToController= 0;
    //no implicit callbacks
    object->eofEvent = NULL;
    object->sofEvent = NULL;
    object->hitEvent = NULL;
    //
}

//Rx pipes are concurrent, so need to alloc per instance !
//(allocation amount is internally determined)
//This should be invoked only for Sources that map on SippRx HW block.
//Currently this is done via AllocOpipeRxCmxBuffs, which performs this check
void PlgSourceCmxAlloc(uint32_t instNo, icSourceSetup *src, MemPoolId id)
{
    void plgSrcOpipeAlloc(uint32_t no, MemPoolId id, icSourceSetup *src);
    plgSrcOpipeAlloc(instNo, id, src);
}

//
void PlgSourceSetLineHit(void *pluginObject, uint32_t lineNo)
{
    PlgSource *obj = (PlgSource *)pluginObject;

    if((obj->mipiRxData.recNrl >= IC_SIPP_DEVICE0) &&
            (obj->mipiRxData.recNrl <= IC_SIPP_DEVICE3) )
    {
        //Snap value to be multiple of IRQ-RATE, else won't be detected by Opipe
        obj->pRx.p.targetLine[HIT_IDX_SRC] = (lineNo + (IRQ_RATE-1)) & (~(IRQ_RATE-1));
        //TBD: check if exceeds height ?
    }
    else{
        //TBD: CIF handling...
    }
}

//
void PlgSourceStart(void *plgObject, icSourceConfig  *srcCfg, uint32_t outFmt)
{
    PlgSource *obj = (PlgSource *)plgObject;
    obj->mipiRxData = srcCfg->mipiRxData;
    obj->plg.status = PLG_STATS_RUNNING;

    //If source is a SippRx block, use OPIPE construct
    if((srcCfg->mipiRxData.recNrl >= IC_SIPP_DEVICE0) &&
            (srcCfg->mipiRxData.recNrl <= IC_SIPP_DEVICE3) )
    {
        //Get new output frame
        uint32_t   oBuffAddr = 0x80000000; //dummy
        FrameT *oFrame = FrameMgrAcquireFrame(obj->outputPools);
        obj->frame = oFrame;
        if(oFrame){
            oBuffAddr  = (uint32_t)oFrame->fbPtr[0];
        }
        plgSrcCreateRxOpipe(obj, srcCfg, outFmt);
        obj->pRx.pOut->ddr.base = oBuffAddr;

        //References needed in Opipe EOF handler to update plugin info
        obj->pRx.p.params[0] = (void*)obj;
        obj->pRx.p.params[1] = (void*)oFrame;

        //If out buff == NULL, override sel = 0 to disable writing to memory
        //(this only applies for current frame !)
        if(0==oFrame) obj->rxCfg.sel01 =      0;
        else          obj->rxCfg.sel01 = 0xF<<8;

        OpipeStart(&obj->pRx.p);
    }
    else{
        DrvCifInit(PLGCIF_GET_ID(srcCfg->mipiRxData.recNrl));
        PlgSourceCifCfg(srcCfg, obj);
    }
#if !defined(MIPI_INTERNAL_LOOPBACK)
    mipiPhyCfg(srcCfg, obj);
#endif
    // setup mipi Controller irq
    plugSourcesLc[srcCfg->mipiRxData.controllerNo] = obj;
    if (mipiCtrlIntStatus == 0) {
        DrvIcbSetupIrq(IRQ_MIPI, 8, POS_EDGE_INT, &setupMipiIrqHandler);
    }
    mipiCtrlIntStatus++;
    sourceSetupMipiIrq(srcCfg->mipiRxData.controllerNo);

    if(obj->mipiRxData.recNrl>=IC_CIF0_DEVICE4)
        DrvMssConnectMipiToDevice(obj->mipiRxData.controllerNo, DRV_MSS_CIF);
    else
        DrvMssConnectMipiToDevice(obj->mipiRxData.controllerNo, DRV_MSS_SIPP_RX);
    obj->conectedToController = 1;
}
/**************************************************************************************************
 ~~~ Local Functions Implementation
 **************************************************************************************************/
//
static int32_t init(FramePool *outputPools, int nOutputPools, void *pluginObject)
{
    PlgSource* obj = (PlgSource*)pluginObject;

    UNUSED(nOutputPools);

    // this plugin produce just 1 output frame, so not take in consideration nOutputPools params,
    // as this have to be 1
    obj->outputPools = outputPools;
    return 0;
}
//
static int32_t fini(void *pluginObject)
{
    PlgSource *obj = (PlgSource *)pluginObject;
    uint32_t disableCtrlMask = (1<<obj->mipiRxData.controllerNo);
    CLR_REG_BITS_MASK(MSS_MIPI_CFG_ADR, disableCtrlMask);
    if(obj->mipiRxData.recNrl <= IC_SIPP_DEVICE3) {
        // clear conection Mipi-SippRx
        CLR_REG_BITS_MASK(MSS_MIPI_SIPP_CFG_ADR, (0x3F<<(obj->mipiRxData.recNrl-IC_SIPP_DEVICE0)));
        // TODO: kill sippRx block
    }
    else {
        CLR_REG_BITS_MASK(MSS_MIPI_CIF_CFG_ADR, (0x3F<<(obj->mipiRxData.recNrl-IC_CIF0_DEVICE4)));
        PlgSourceCifStop(obj);
        DrvCifInit(PLGCIF_GET_ID(obj->mipiRxData.recNrl));
    }
    // disable interrupts
    SET_REG_WORD(MIPI_IRQ_ENABLE_ADDR, (GET_REG_WORD_VAL(MIPI_IRQ_ENABLE_ADDR)) &
            (~(D_MIPI_IRQ_STAT_HS_RX_EVENT_0 << obj->mipiRxData.controllerNo)));
    SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, (D_MIPI_IRQ_STAT_HS_RX_EVENT_0 << obj->mipiRxData.controllerNo));
    mipiCtrlIntStatus--;
    if(0 == mipiCtrlIntStatus) { // disable mipi controler irq
        DrvIcbDisableIrq(IRQ_MIPI);
    }
    obj->conectedToController = 0;
    obj->plg.status = PLG_STATS_IDLE;

    return 0;
}
//
static inline void mipiPhyCfg(icSourceConfig *mipiRxConfiguration, PlgSource *obj)
{
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

//
static void setupMipiIrqHandler(uint32_t unused)
{
    uint32_t intStatus;
    uint32_t mipiControler = 0;
    uint32_t dataIdentifier;

    UNUSED(unused);

    // Get/clear the status
    intStatus = GET_REG_WORD_VAL(MIPI_IRQ_STATUS_ADDR);
    SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, intStatus);
    DrvIcbIrqClear( IRQ_MIPI);
    intStatus = (intStatus >> D_MIPI_IRQ_STAT_HS_RX_BITS_POS) & 0x3F;

    for (mipiControler=0; mipiControler<DRV_MIPI_NO_CTRL; mipiControler++) {
        if(intStatus & (1<<mipiControler)) {
            PlgSource *plug = plugSourcesLc[mipiControler];
            assert(0 != plug);// Interrupt from uninitialized mipi controller
            dataIdentifier =  GET_REG_WORD_VAL( MIPI0_RX_HS_PH_ADDR + 0x100 * mipiControler);
            dataIdentifier =  (dataIdentifier>>24)&0x3F;
            switch(dataIdentifier) {
            case D_MIPI_CSI_FRAME_START:
                if(0 == plug->conectedToController) { // ensure connection at sof for cif or sipp
                    if(plug->mipiRxData.recNrl>=IC_CIF0_DEVICE4)
                        DrvMssConnectMipiToDevice(plug->mipiRxData.controllerNo, DRV_MSS_CIF);
                    else
                        DrvMssConnectMipiToDevice(plug->mipiRxData.controllerNo, DRV_MSS_SIPP_RX);
                    plug->conectedToController = 1;
                }
                if(plug->mipiRxData.recNrl>=IC_CIF0_DEVICE4) { // sof event TODO: add same approach for sipp side
                    IpipeMipiRxCifSof(PLGCIF_GET_ID(plug->mipiRxData.recNrl));
                }
                break;
            case D_MIPI_CSI_FRAME_END:
                if(1 == plug->conectedToController) { // ensure connection at sof for cif or sipp
                    if(plug->mipiRxData.recNrl>=IC_CIF0_DEVICE4) { // eof event TODO: add same approach for sipp side
                        // ~~~~~ workaround for window cif issue, vor v0 of cipp ~~~~
                        uint32_t cippId = GET_REG_WORD_VAL(CPR_VERSION_ID_ADR);
                        cippId = (cippId>>18) & 15; // TODO: this is not possible to be day_data as in specification, probably is version. Update doc if is true
                        if (0 == cippId) { // version 1, cif window workaround applied
                            // Disable and re-enable CIF0 windowing
                            uint32_t cifBaseAddr = CIF1_CONTROL_ADR;
                            if(IC_CIF1_DEVICE5 == plug->mipiRxData.recNrl) cifBaseAddr = CIF2_CONTROL_ADR;
                            uint32_t cifCtrl = GET_REG_WORD_VAL(cifBaseAddr);
                            SET_REG_WORD(cifBaseAddr, cifCtrl & D_CIF_CTRL_WINDOW_DISAB_MASK);
                            SET_REG_WORD(cifBaseAddr, cifCtrl);
                        }

                        IpipeMipiRxCifEof(PLGCIF_GET_ID(plug->mipiRxData.recNrl));

                        // Specific order assumed for mipi controller/cif interrupts
                        plug->mipiCtrlEofIdx++;
                        producedFrameCifCall(plug);
                    }
                }
                break;
            default:
                assert(0);// there are no other interrupt associated with mipi controller
                break;
            }
        }
    }
}
//
static void sourceSetupMipiIrq(uint32_t MipiCtrlNo) {
    u64 pktHeaderFilter = (1 << D_MIPI_CSI_FRAME_END) | (1 << D_MIPI_CSI_FRAME_START); //- ACTUALLY SAME CODE as CSI FE!
    assert(MipiCtrlNo < DRV_MIPI_NO_CTRL);
    SET_REG_WORD(MIPI0_RX_HS_PH_FILTER0_ADDR + 0x100 * MipiCtrlNo, pktHeaderFilter & 0xffffffff);
    SET_REG_WORD(MIPI0_RX_HS_PH_FILTER1_ADDR + 0x100 * MipiCtrlNo, pktHeaderFilter >> 32);
    SET_REG_WORD(MIPI_IRQ_ENABLE_ADDR, (GET_REG_WORD_VAL(MIPI_IRQ_ENABLE_ADDR)) |
            (D_MIPI_IRQ_STAT_HS_RX_EVENT_0 << MipiCtrlNo));
}

