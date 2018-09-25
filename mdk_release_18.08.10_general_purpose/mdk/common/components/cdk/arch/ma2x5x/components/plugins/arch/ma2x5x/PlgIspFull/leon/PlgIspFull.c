/**************************************************************************************************

 @File         : PlgIspFullApi.c
 @Author       : AL
 @Brief        : Contain Opipe full Isp plug-in implementation
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :

 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "DrvTimer.h"
#include "swcLeonUtils.h"
#include "Opipe.h"
#include "TimeSyncMgr.h"
#include "PlgIspFullApi.h"
#include "FrameMgrApi.h"
#include "ipipeDbg.h"
#include "ipipeUtils.h"


/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/




//Plugin status
enum {
    PLG_OFF = 0,
    PLG_ON  = 1
};

/**************************************************************************************************

 ~~~ Local Functions Implementation
 **************************************************************************************************/

//
static void plgIspFullSetParams(PlgIspFull *obj) {

    Opipe       *p  = &obj->op.p;
    icIspConfig *ic =  obj->ispCfg;

    fetchIcIspConfig(p, ic); //icIspConfig -> Opipe translation
    computePolyfirParams(&obj->scale, ic);

    //Default CFG words:
    OpipeDefaultCfg(p, SIPP_SIGMA_ID  );
    OpipeDefaultCfg(p, SIPP_RAW_ID    );
    OpipeDefaultCfg(p, SIPP_DBYR_ID   );
    OpipeDefaultCfg(p, SIPP_DOGL_ID   );
    OpipeDefaultCfg(p, SIPP_SHARPEN_ID);
    OpipeDefaultCfg(p, SIPP_CGEN_ID   );
    OpipeDefaultCfg(p, SIPP_MED_ID    );
    OpipeDefaultCfg(p, SIPP_CHROMA_ID );
    OpipeDefaultCfg(p, SIPP_CC_ID     );
    OpipeDefaultCfg(p, SIPP_LUT_ID    );
}

int32_t cbTrigerCapture(FrameT *frame, void *params, int (*callback)(int status), void *pluginObj) {
//    icSize size;
    PlgIspFull* obj = (PlgIspFull*)pluginObj;

    UNUSED(callback);

    assert(frame);
    assert(params);
    if(PLG_ON == obj->crtStatus)
    {
        obj->plg.status = PLG_STATS_RUNNING;
        obj->ispCfg = (icIspConfig  *)params;
        //Get a frame from output mempool
        FrameT *oFrame = FrameMgrAcquireFrame(obj->outputPools);

        //Skip input frame if no more output buffer available
        if (NULL == oFrame) {
            if (obj->procesIspError) {
                obj->procesIspError(pluginObj, IC_SEVERITY_NORMAL, IC_ERROR_RT_OUT_BUFFERS_NOT_AVAILABLE, ((icIspConfig  *)frame->appSpecificData)->userData);
            } else {
                assert (0);
            }
            FrameMgrReleaseFrame(frame);
            obj->plg.status = PLG_STATS_IDLE;
            return -1;
        }

        oFrame->appSpecificData = frame->appSpecificData;

        //Else, we can process
        if (obj->procesStart) obj->procesStart((void*)obj, oFrame->seqNo, obj->ispCfg->userData);

        //From currently set "icIspConfig" to Opipe regs
        plgIspFullSetParams(obj);
        OpipeSetSizeMF (&obj->op, obj->frmSz.w, obj->frmSz.h);
        //Frame pointers:
        obj->op.pIn->ddr.base    = (uint32_t)frame->fbPtr[0];
        obj->op.pOutY->ddr.base  = (uint32_t)oFrame->fbPtr[0];
        obj->op.pOutUV->ddr.base = (uint32_t)oFrame->fbPtr[1];

        //References needed in Opipe EOF handler to update plugin info
        obj->op.p.params[0] = (void*)obj;
        obj->op.p.params[1] = (void*)frame;
        obj->op.p.params[2] = (void*)oFrame;
        //The kick
        OpipeStart(&obj->op.p);
    }
    return 0;
}
//
static void cbNewInputFrame(FrameT *iFrame, void *pluginObject) {
    PlgIspFull* obj = (PlgIspFull*)pluginObject;
    assert(iFrame);
    if(PLG_ON == obj->crtStatus)
    {
        obj->plg.status = PLG_STATS_RUNNING;

        if(iFrame->appSpecificData) {
            obj->ispCfg = (icIspConfig  *)iFrame->appSpecificData;
        }
        else {
            FrameMgrReleaseFrame(iFrame);
            obj->plg.status = PLG_STATS_IDLE;
            return;
        }

       //Else, can try capture (will succeed if oFrame can be allocated)
        cbTrigerCapture(iFrame, obj->ispCfg, NULL, pluginObject);
    }
}
//Opipe EOF callback: adjust associated Plugin Frame buffers
void opipeIspEof(Opipe *p) {
    PlgIspFull *obj    = (PlgIspFull *)p->params[0];
    FrameT     *iFrame = (FrameT     *)p->params[1];
    FrameT     *oFrame = (FrameT     *)p->params[2];

    assert(iFrame);
    assert(oFrame);

  //Set correct resolution to out frame (now that frame's produced out res is also known)
    //assert(FRAME_T_FORMAT_YUV420 == oFrame->type);
    oFrame->stride[0] = obj->op.p.pUpfirdn0Cfg->oW;
    oFrame->height[0] = obj->op.p.pUpfirdn0Cfg->oH;
    oFrame->stride[1] = obj->op.p.pUpfirdn12Cfg->oW<<1;//*2 (got 2 UV planes)
    oFrame->height[1] = obj->op.p.pUpfirdn12Cfg->oH;
    //if(obj->op.p.flags & MISSING_DATA)
    if (obj->procesEnd)   obj->procesEnd((void*)obj, oFrame->seqNo, obj->ispCfg->userData);

    //new frame produced, updating times informations
    FrameMgrAddTimeStampHist(oFrame, iFrame);
    FrameMgrAndAddTimeStamp (oFrame, TimeSyncMsgGetTimeUs());

    FrameMgrReleaseFrame(iFrame);
    FrameMgrProduceFrame(oFrame);
    obj->plg.status = PLG_STATS_IDLE;
}

//
static int32_t fini(void *pluginObject) {
    PlgIspFull *object = (PlgIspFull*)pluginObject;
    object->crtStatus  = PLG_OFF;
    return 0;
}

//
static int32_t init(FramePool *outputPools, int nOutputPools, void *pluginObject) {
    PlgIspFull* obj = (PlgIspFull*)pluginObject;

    UNUSED(nOutputPools);

    // this plugin produce just 1 output frame, so not take in consideration nOutputPools params,
    // as this have to be 1
    obj->outputPools = outputPools;
    return 0;
}

//Opipe Full Isp yuv420 output creation
static void CreateOpipe(PlgIspFull *o, uint32_t inFmt, uint32_t prevAble)
{
    OpipeMF *opF = &o->op;

    if(o->cSigma   == NULL) o->cSigma   = ogcBuff[SIPP_MIPI_RX0_ID ];
    if(o->cDbyrY   == NULL) o->cDbyrY   = ogcBuff[SIPP_DBYR_LUMA_ID];
    if(o->cSharpY  == NULL) o->cSharpY  = ogcBuff[SIPP_SHARPEN_ID  ];
    if(o->cLut     == NULL) o->cLut     = ogcBuff[SIPP_LUT_ID      ];
    if(o->cUpfirDn == NULL) o->cUpfirDn = ogcBuff[SIPP_UPFIRDN0_ID ];
    if(o->cDbyrIn  == NULL) o->cDbyrIn  = ogcBuff[SIPP_RAW_ID      ]; //potentially NULL (!NULL if preview enabled)

    assert(o->cSigma  != NULL);
    assert(o->cDbyrY  != NULL);
    assert(o->cSharpY != NULL);
    assert(o->cLut    != NULL);
    assert(o->cUpfirDn!= NULL);

  //Must specify buffers first
    opF->in.cBufSigma.base = (uint32_t)o->cSigma;   opF->in.cBufSigma.h = I_CBUFF_H;
    opF->cBufDbyrY.base    = (uint32_t)o->cDbyrY;   opF->cBufDbyrY.h    = DBYR_Y_H;
    opF->cBufSharp.base    = (uint32_t)o->cSharpY;  opF->cBufSharp.h    = SHARP_Y_H;
    opF->cBufLut.base      = (uint32_t)o->cLut;     opF->cBufLut.h      = LUT_H;
    opF->cBufPoly.base     = (uint32_t)o->cUpfirDn; opF->cBufPoly.h     = O_CBUFF_H;

  #if defined(OPIPE_PREVIEW_ABLE)
    if(prevAble){
      assert(o->cDbyrIn != NULL);
      opF->cBufDbyrIn.base = (uint32_t)o->cDbyrIn;  opF->cBufDbyrIn.h   = O_CBUFF_H;
    }
  #else
    UNUSED(prevAble);
  #endif

    OpipeCreateFull(opF, inFmt);
    initLutCopyTask();
}

//Opipe related: returns Opipe circular buffer requirements in bytes.
//WARNING: using SIPP_MIPI_RX0_ID for Sigma Input !
// wSig  : width for RAW filters (Sigma, Lsc, Raw, Dbyr input)
// wMain : width for filters below debayer (can be wSig/2 if preview is enabled)
// wPoly : width for Polyfir output (based on associated source max N,D params)
// sz    : required circular-output-buffer size in bytes
static void QueryCircBuffSizes(PlgIspBase *me, uint32_t wSig, uint32_t wMain, uint32_t wPoly, uint32_t* req)
{
    UNUSED(me);

  //Enlarge line widths a bit to accommodate internal padding:
   wSig += 8; wMain += 8; wPoly += 16;

  //                         height    * pl * width * bpp
   req[SIPP_MIPI_RX0_ID ] =  I_CBUFF_H      * wSig     ; // bpp:N/A
   req[SIPP_DBYR_LUMA_ID] =  DBYR_Y_H       * wMain * 2; // bpp:2(fp16)
   req[SIPP_SHARPEN_ID  ] =  SHARP_Y_H      * wMain * 2; // bpp:2(fp16)
   req[SIPP_LUT_ID      ] =  LUT_H     * 3  * wMain * 1; // bpp:1(  u8)
   req[SIPP_UPFIRDN0_ID ] =  O_CBUFF_H * 2  * wPoly * 1; // bpp:1(  u8)

   #if defined(OPIPE_PREVIEW_ABLE)
   //dbyr In buffer
   req[SIPP_RAW_ID      ] =  O_CBUFF_H      * wMain * 2; // bpp:2(fp16)
   #endif
}


/**************************************************************************************************
 ~~~ Exported Functions Implementation
 **************************************************************************************************/

//
void PlgIspFullConfig(void *plgObject, icSize frameSz, uint32_t inFmt, uint32_t prevAble)
{
    PlgIspFull *obj   = (PlgIspFull *)plgObject;
    obj->frmSz  = frameSz;
    //Must know resolution in order do create an Opipe object
    if (PLG_ISPFULL_CREATED == obj->status) {
        CreateOpipe(obj, inFmt, prevAble);
        obj->status = PLG_ISPFULL_INUSE;
    }
    else {
        if(PLG_ISPFULL_INUSE == obj->status) {
            //Assume that change resolution, so update it
            //Resolution updates (if needed)
            //OpipeSetRes(&obj->op.p, frameSz.w, frameSz.h);
        }
        else {
            // try to start a source but not create is first,
            // or a memory corruption overwrite plug-in structure
            assert(0);
        }
    }

    obj->crtStatus = PLG_ON;
    //In Opipe EOF callback, will update Ipipe Frame buffers status
    obj->op.p.cbEndOfFrame = opipeIspEof;
}


//
void PlgIspFullCreate(void *pluginObject) {
    memset(pluginObject, 0, sizeof(PlgIspFull));
    PlgIspFull *object          = (PlgIspFull*)pluginObject;
    // init hw things, or all the init side that not need params,
    object->plg.init            = init; //to associate output pool
    object->plg.fini            = fini; //to mark STOP
    object->plg.trigger         = cbTrigerCapture;
    object->cbList[0].callback  = cbNewInputFrame;
    object->cbList[0].pluginObj = pluginObject;
    object->plg.callbacks       = object->cbList;
    object->plg.status          = PLG_STATS_IDLE;
    object->crtStatus           = PLG_OFF;
    object->status              = PLG_ISPFULL_CREATED;

    object->base.fnQueryMemReq  = QueryCircBuffSizes;
    computePolyFirInit(&object->scale);
}
