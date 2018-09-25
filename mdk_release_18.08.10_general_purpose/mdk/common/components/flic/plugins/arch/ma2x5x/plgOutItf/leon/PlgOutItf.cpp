///
/// @file      PlgOutItf.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application for PlgOutItf.
///            Platform(s) supported : ma2x5x
///

#include <stdio.h>
#include <ImgFrame.h>
#include "ipipe.h"
#include "IspMsg.h"
#include "PlgOutItf.h"

uint32_t aa;
//#############################################
void PlgOutItf::Create() {
    //Create receivers:
    sync.Create();
    in.Create(16, &sync);
    inCmd.Create(16, &sync);

    //Create receivers
    Add(&in); Add(&inCmd);
    Add(&outCmd);

    convertFrmLutInit();
    resetQ();
}

//TBD: PlgOutput::Stop , Delete
void PlgOutItf::Stop() {
    sync.Destroy ();
    IPlugin::Stop();
}

class outputIfQue {

    void getElmQ(FrameT *frm);
};

//#############################################
void * PlgOutItf::threadFunc(void *) {
    IspCmdMsg inCmdMsg;
    IspCmdMsg outCmdMsg;
    SendQ* elm = NULL;
    rtems_object_set_name(RTEMS_SELF, "PlgOutItf");
    while(Alive())
    {
        //1)Wait for something to happen
        if(OK != sync.Wait()) return NULL;

        if(NULL == elm) {
            elm = getAFreeElement();
            assert(elm); // que is full, output not work
        }

        if(OK == in.TryReceive(&elm->iFrm)) {

            ImgFrame *imgFrm = ((ImgFrame*)(elm->iFrm.ptr));
            FrameT *frm = &elm->frm;
            // copy buffers
            frm->fbPtr[0] = imgFrm->fb.p1;
            frm->fbPtr[1] = imgFrm->fb.p2;
            frm->fbPtr[2] = imgFrm->fb.p3;
            // copy sizes
            frm->width [0] = imgFrm->fb.spec.width;
            frm->stride[0] = imgFrm->fb.spec.width;
            frm->height[0] = imgFrm->fb.spec.height;
            frm->tSize [0] = imgFrm->fb.spec.width * imgFrm->fb.spec.height;

            if(RAW8 == imgFrm->fb.spec.type) {
                // TODO: fix hardcode for YUV420P format send
                frm->width [1] = 0;
                frm->stride[1] = 0;
                frm->height[1] = 0;
                frm->tSize [1] = 0;
                frm->nPlanes   = 1; //imgFrm->nPlanes;
            }
            else {  // YUV420 default
                // TODO: fix hardcode for YUV420P format send
                frm->width [1] = imgFrm->fb.spec.width;
                frm->stride[1] = imgFrm->fb.spec.width;
                frm->height[1] = imgFrm->fb.spec.height>>1;
                frm->tSize [1] = imgFrm->fb.spec.width * imgFrm->fb.spec.height >> 1;
                frm->nPlanes  = 2; //imgFrm->nPlanes;
            }

            frm->width [2] = 0;
            frm->stride[2] = 0;
            frm->height[2] = 0;
            frm->tSize [2] = 0;

            frm->fmt      = frmConvert[imgFrm->fb.spec.type];
            frm->type     = (ImgFrmType)(imgFrm->categ);
            frm->instId   = imgFrm->instNo;
            frm->userData = imgFrm->userData;

            frm->seqNo    = imgFrm->seqNo;

            // TODO: develop in order to suport and other formats
            assert(FRAME_FMT_YUV420P != frm->fmt);

            //assert(FRAME_TYPE_NONE != frm->type);
            outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_SEND_OUTPUT_DATA;
            outCmdMsg.cmdMsg.u.sentOutput.outputId = frm->instId;
            outCmdMsg.cmdMsg.u.sentOutput.dataBufStruct =  frm;
            outCmd.Send(&outCmdMsg);
            elm = NULL;
            aa++;
        }
        else {
            if(OK == inCmd.TryReceive(&inCmdMsg)) {
                SendQ* queS;
                assert(IC_EVENT_TYPE_OUTPUT_DATA_RECEIVED == (inCmdMsg.cmdMsg.ctrl & IC_EVENT_CTRL_TYPE_MASK));
                queS = releaseElmFromQue((FrameT *)inCmdMsg.cmdMsg.u.dataWasSent.dataBufStruct);
                assert(queS);
                //assert(&frm == (inCmdMsg.cmdMsg.u.dataWasSent.dataBufStruct));
                queS->iFrm = nullObj;
            }
        }
    }
    return NULL;
}


void PlgOutItf::resetQ() {
    for (int i = 0; i < SendQMaxSize; i++) {
        sendQ[i].frmQueStatus = 0;
    }
}

PlgOutItf::SendQ* PlgOutItf::getAFreeElement(void) {
    int i = 0;
    for (i = 0; i < SendQMaxSize; i++) {
        if(0 == sendQ[i].frmQueStatus) {
            sendQ[i].frmQueStatus = 1;
            return(&sendQ[i]);
        }
    }
    return NULL;
}

PlgOutItf::SendQ* PlgOutItf::releaseElmFromQue(FrameT *frm) {
    int i = 0;
    for (i = 0; i < SendQMaxSize; i++) {
        if(frm == &sendQ[i].frm) {
            assert(sendQ[i].frmQueStatus);
            sendQ[i].frmQueStatus = 0;
            return(&sendQ[i]);
        }
    }
    return NULL;
}



void PlgOutItf::convertFrmLutInit() {
    frmConvert[YUV422i  ] = FRAME_FMT_422I;
    frmConvert[YUV420p  ] = FRAME_FMT_YUV420;
    frmConvert[YUV422p  ] = FRAME_FMT_YUV420P;
    frmConvert[YUV400p  ] = FRAME_FMT_NV12_Y;
    frmConvert[RGBA8888 ] = FRAME_FMT_USER_01;
    frmConvert[RGB888   ] = FRAME_FMT_RGB_888;
    frmConvert[LUT2     ] = FRAME_FMT_BINARY;
    frmConvert[LUT4     ] = FRAME_FMT_BINARY;
    frmConvert[LUT16    ] = FRAME_FMT_BINARY;
    frmConvert[RAW16    ] = FRAME_FMT_RAW_16;
    frmConvert[RAW8     ] = FRAME_FMT_RAW_8;
    frmConvert[YUV444i  ] = FRAME_FMT_YUV444;
    frmConvert[NV12     ] = FRAME_FMT_NV12;
    frmConvert[NONE     ] = FRAME_FMT_NONE;
}
