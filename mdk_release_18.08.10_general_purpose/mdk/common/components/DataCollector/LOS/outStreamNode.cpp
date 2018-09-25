
#include "outStreamNode.h"
#include <VcsHooksApi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <superFrame.h>
#include "XLink.h"
#define MVLOG_UNIT_NAME outStream
#include <mvLog.h>
#include "commStructs.h"
const char * X_LINK_STREAM_NAME ="DCStream";
//#define DISABLE_USB_SEND
int PlgOutStream::AddMetaInput(MsgGid grp){
    if (lowLatencyEnable){//low latency config
        syncIn[metaGroupCount].Create(1, QueueCmdOvr<StereoImgAndMetaMsg>::Inst(), &sync);
    }else{// no drop politics
        syncIn[metaGroupCount].Create(10, &sync);
    }
    Add(&syncIn[metaGroupCount], ".syncIn");
    inputGid[metaGroupCount] = grp;
    return metaGroupCount++;
}
void PlgOutStream::Create(){
    metaGroupCount = 0;
    mvLogLevelSet(MVLOG_INFO);
    sync.Create();
    inIMU.Create(128);
    Add(&inIMU, ".inIMU");
    if (lowLatencyEnable){//low latency config
        inFrames.Create(1, QueueCmdOvr<StereoImgMsg>::Inst(), &sync);
    }else{// no drop politics
        inFrames.Create(10, &sync);
    }
    Add(&inFrames, ".inFrames");
#ifndef DISABLE_USB_SEND
    usbLinkStream = XLinkOpenStream(0, (char *)X_LINK_STREAM_NAME, (camCnt * 2 + 1) *2*1024*1024);
#endif
}

int outStreamInit(void)
{
    return 0;
}

char inputImageData[4096];


void PlgOutStream::outStreamUSBFullWrite(void* buff, u32 cnt)
{
#ifndef DISABLE_USB_SEND
    // send the actual data
    if (buff)
    {
        XLinkWriteData(usbLinkStream, (const uint8_t*) buff, cnt); //ugly const cast
    }
#else
    UNUSED(buff);
    UNUSED(cnt);
    usleep(10000);
#endif
}

void  PlgOutStream::outStreamSendOverUsb(void *data, outStreamType_t type, MsgGid Id, int subId)
{

    outStreamUSBFullWrite(&Id,sizeof(MsgGid));
    outStreamUSBFullWrite(&subId,sizeof(int));

    switch (type)
    {
    case FRAME_BUFF: {
        ImgFrame* frm = (ImgFrame* )data;
        int full_size = frm->fb.spec.height * frm->fb.spec.stride;
        frameMetadataGeneric frameInfo;
        frameInfo.exposureNs = 0;
        frameInfo.frameCount = frm->seqNo;
        frameInfo.timestampNs = frm->ts;
        u64 ts = rtems_clock_get_uptime_nanoseconds ();
        frameInfo.latencyNs = ts - frm->ts;
        frameInfo.frameType = frm->fb.spec.type;
        outStreamUSBFullWrite(&frameInfo,  sizeof(frameInfo));
        outStreamUSBFullWrite(&frm->fb.spec,  sizeof(frameSpec));
        outStreamUSBFullWrite(frm->fb.p1, full_size);
    }break;
    case META: {
        frameMeta* meta = (frameMeta*)data;
        int full_size = sizeof(frameMetadataGeneric);
        outStreamUSBFullWrite(&full_size, sizeof(uint32_t));
        outStreamUSBFullWrite(&meta->genericMeta, full_size);
        full_size = meta->customMetaSize;
        
        outStreamUSBFullWrite(&full_size, sizeof(uint32_t));
        outStreamUSBFullWrite(&meta->customMetadata, full_size);

    }break;
    case IMU: {
        imuFrame* imu = (imuFrame*)data;
        int full_size = imu->count * sizeof(mvImuData_t) + sizeof(uint32_t);
        outStreamUSBFullWrite(&full_size, sizeof(uint32_t));
        if (full_size != 0)
            outStreamUSBFullWrite(data, full_size);
    }break;
    default:
        assert(0);
    }
}

void* PlgOutStream::threadFunc(void *ctx)
{
    UNUSED(ctx);
    while(1)
    {
        StereoImgAndMetaMsg msg[MAX_META_GROUPS];
        u8 isValid[MAX_META_GROUPS];
        u8 isFrameValid;

        ImgFramePtr msgImu;
        int i;
        StereoImgMsg stereo;
        int numItemToSend = 0;
        isFrameValid = 0;
        if(OK == sync.Wait())
        {
            //collect all valid inputs
            if(OK == inFrames.TryReceive(&stereo)){
                numItemToSend = camCnt + 1; // IMU will be sent together with frames for now.
                isFrameValid = 1;
            }
            for(i = 0; i < metaGroupCount; i++){
                isValid[i] = 0;
                if(OK == syncIn[i].TryReceive(&msg[i])){
                    isValid[i] = 1;
                    numItemToSend += msg[i].count;
                }
                else 
                {
                    mvLog(MVLOG_DEBUG, "OUT FRAME DROPED\n");
                }
            }
        }else{
            assert(0);
        }
        if(!numItemToSend){
            continue;
        }
        outStreamUSBFullWrite(&numItemToSend,sizeof(int));
        int j;
        for(i = 0; i < camCnt; i++){
            //int rc1 = inFrame.Receive(&msg);    UNUSED(rc1);
            //int rc2 = inMeta.Receive(&msgMeta); UNUSED(rc2);

            u64 ts = rtems_clock_get_uptime_nanoseconds ();
            //DrvTimerGetSystemTicks64(&ts);
            //ts = 1000 * ts / (DrvCprGetSysClockKhz() / 1000);
            if(isFrameValid){
                ImgFrame* frm = (ImgFrame*) stereo.frame[i].ptr;
#ifdef CONFIG_TEARING_CHECK
//                assert(((u32*)frm->fb.p1)[0] == ((u32*)(((u8*)frm->fb.p1) + frm->fb.spec.stride * frm->fb.spec.height - 4))[0]);
#endif
                
                outStreamSendOverUsb(frm, FRAME_BUFF,GID_CAMERA, i);
            }
//            msg.Release();
            for(j = 0; j < metaGroupCount; j++)
                if( isValid[j] && i < msg[j].count){
//                    assert(stereo.frame[i].ptr->seqNo == msg[j].frame[i].ptr->seqNo);
                    frameMeta* meta =  (frameMeta*)msg[j].meta[i].ptr->fb.p1;
                    meta->genericMeta.latencyNs = ts - meta->genericMeta.timestampNs;
                    mvLog(MVLOG_DEBUG, "Latency in outStream %lu\n", meta->genericMeta.latencyNs);
                    outStreamSendOverUsb(meta, META, inputGid[j], i);
                }
//            msgMeta.Release();
        }

        imuPacket.count = 0;
        if (isFrameValid && camCnt > 1){
            //stereo frame, check timestamp difference

            int tsDiff = stereo.frame[0].ptr->ts - stereo.frame[1].ptr->ts;
            if (tsDiff < 0)
                tsDiff *= -1;
            mvLog(MVLOG_DEBUG, "Timestamp difference %d\n", tsDiff);
            if ( tsDiff > 50000)
                mvLog(MVLOG_WARN, "Timestamp difference bigger than 50 us. Check camera synchronization. Diff: %d\n", tsDiff);
        }
        if(isFrameValid){ //Send IMU only when frames are available
            while( inIMU.TryReceive(&msgImu) == 0){
                memcpy(imuPacket.data, msgImu.ptr->fb.p1, sizeof(mvImuData_t));
                imuPacket.count++;
                //            msg.Release();
            }
            outStreamSendOverUsb(&imuPacket, IMU, GID_IMU, 0);
        }

    }
    return NULL;
}
