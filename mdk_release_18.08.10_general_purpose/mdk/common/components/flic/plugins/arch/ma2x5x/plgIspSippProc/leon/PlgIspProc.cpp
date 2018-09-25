///
/// @file      PlgIspProc.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application for PlgIspProc.
///            Platform(s) supported : ma2x5x
///

#include "PlgIspProc.h"
#include "IspCommon.h"
#include "IspMsg.h"
#include "sipp_isp.h"
#include "sipp_isp_types.h"

#define DYNAMIC_IRQ_SIPP_IBFL_DEC  IRQ_DYNAMIC_0
#define DYNAMIC_IRQ_SIPP_OBFL_INC  IRQ_DYNAMIC_1
#define DYNAMIC_IRQ_SIPP_FRAME_END IRQ_DYNAMIC_2

pthread_t PlgIspProc::waitingThr;

void IspStart(ImgFrame *fIn, ImgFrame *fOut, IspPipeline ispPipe, PipeConfig  iCfg);
void IspWait ();

uint32_t nFrmDone __attribute__((section(".cmx.cdmaDescriptors")));

//##############################################################################
void appSippCallback (SippPipeline *pPipeline, eSIPP_PIPELINE_EVENT eEvent, SIPP_PIPELINE_EVENT_DATA * ptEventData)
{
    UNUSED (pPipeline);
    UNUSED (ptEventData);

    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        nFrmDone++;
        rtems_event_send(PlgIspProc::waitingThr, RTEMS_EVENT_1);
    }

}
//#############################################
void PlgIspProc::Create(uint32_t insId)
{
    //Create receivers
    inI.Create(8); //in frames

    Add(&inI );Add(&inO);
    Add(&outF);Add(&outE);

    id = insId;

    //------Init sequence - move to sippIsp?--------------------
    SippIspInit ();

    if (PROCESS_LEON_OS == swcWhoAmI ())
    {
        printf ("App Starting on LEON OS\n");
        printf ("Dynamically routing interrupts from LeonRT ICB to LeonOS ICB\n");

        // Re-route irqs to Leon OS
        sippDynRouteIrq (DYNAMIC_IRQ_SIPP_IBFL_DEC,
                DYNAMIC_IRQ_SIPP_OBFL_INC,
                DYNAMIC_IRQ_SIPP_FRAME_END);
    }
    else
    {
        printf ("App Starting on LEON RT\n");
    }

    DrvCmxDmaInitDefault();
    //----------------------------------------------------------

    SippIspCreatePipe(&ispPipe, &iCfg, appSippCallback, SIPP_ISP_BAYER);
}

void PlgIspProc::Delete() {
    IPlugin::Delete();
    //TODO: replace with SIPP ?
//    plgOpipe.IspDestroy();
}

//#############################################
void PlgIspProc::Stop(){
    IPlugin::Stop();
    //TBD: destroy ISP wait sem event
}
//#############################################
void * PlgIspProc::threadFunc(void *)
{
    while(Alive())
    {
        ImgFramePtr  fInp;
        ImgFramePtr  fOut;
        int   err = 0;

        //1) Wait on inputs to be available
        err += inI.Receive(&fInp);
        err += inO.Receive(&fOut);

        //2) Process
        if(!err)
        {
            ImgFrame *iImg = fInp.ptr;
            ImgFrame *oImg = fOut.ptr;

            iCfg.iBuf = (uint16_t *)iImg->base;
            iCfg.oBuf = (uint8_t  *)oImg->base;

            IspStart(iImg, oImg, ispPipe, iCfg);
            IspWait ();

            //3) Done
            // send OutE message, to inform that isp config was consumed and statistics was generated
            sendIspEndEvent(((icIspConfig*)iImg->userData)->userData);

            oImg->instNo = id; // populate frame user data with isp id information
            outF.Send(&fOut); //Forward output
        }
    }
    return NULL;
}

void PlgIspProc::sendIspEndEvent(void *userData) {
    IspCmdMsg outCmdMsg;
    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ISP_END;
    outCmdMsg.cmdMsg.u.ispEvent.ispInstance = id;
    outCmdMsg.cmdMsg.u.ispEvent.seqNo = 0;
    outCmdMsg.cmdMsg.u.ispEvent.userData = userData;
    outE.Send(&outCmdMsg);
}

//##############################################################################
void IspStart(ImgFrame *fInp, ImgFrame *fOut, IspPipeline ispPipe, PipeConfig  iCfg)
{

    PlgIspProc::waitingThr = pthread_self();

    SippIspConfigPipe (&ispPipe, &iCfg);

    // Normally this should do nothing after its first call
    SippIspFinalizePipe (&ispPipe);

    SippIspStartPipeline (&ispPipe);

    // assume out res = input res, should really use scale factor param !!!
    fOut->fb.spec.width  = fInp->fb.spec.width;
    fOut->fb.spec.height = fInp->fb.spec.height;
    fOut->fb.spec.stride = fInp->fb.spec.stride;

    fOut->fb.p1   = (unsigned char*) fOut->base;
    fOut->fb.p2   = (unsigned char*)((uint32_t)fOut->base + fInp->fb.spec.width * fInp->fb.spec.height);
    fOut->fb.spec.type = YUV420p;

    // this field can be overwrite at app level if this isp is associated with still image
    fOut->categ   = FRAME_TYPE_PREVIEW;
}

//##############################################################################
void IspWait()
{
    rtems_event_set events;
    rtems_event_receive(RTEMS_EVENT_1, RTEMS_EVENT_ANY | RTEMS_WAIT, RTEMS_NO_TIMEOUT, &events);
}
