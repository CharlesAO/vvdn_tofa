///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     visionHAL implementation
///

#include "visionHal.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "superFrame.h"
#include <imuFrame.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string.h>
#include "XLink.h"
#include "swcFrameTypes.h"
#include "commStructs.h"
#include <semaphore.h>
#include "opencv2/core/core.hpp"
#include "pcCfgMsg.h"

using namespace cv;
using namespace std;

#define X_LINK_STREAM_NAME "DCStream"
#define X_LINK_STREAM_CONFIG_NAME "config"

static streamId_t usbLinkStream;
static streamId_t configStream;

static XLinkHandler_t handler;
static XLinkGlobalHandler_t glHandler;

pthread_mutex_t lock;
sem_t mutex;
#define MAXNAMESIZE 32
#define ROUND_UP(x,y) (((x) + (y) - 1) / (y) * (y))
static void fullRead(void* buffer, int cnt);
uint8_t conf_global;
void cofidenceToVisionHAL(uint8_t conf)
{
    conf_global = conf;
}

typedef enum moviEvalCommand_t{
    START,
    STOP,
    ADD_ALGO,
    BPP,
    RES,
}moviEvalCommand_t;

typedef enum consumerID{
    CONS_FRAME = 0,
    CONS_VTRACK,
    CONS_STEREO,
    CONS_IMU

}consumerID;


typedef struct moviEvalConfig{
    moviEvalCommand_t cmd;
    algoType type;
    int value;
}moviEvalConfig;


visionHal::visionHal(int num_images,
                     FrameInput _inputType) :  m_numImages(num_images),
                                              inputType(_inputType)
{
    frame_count = 0;
    vtrackCb = NULL;
    imuCb = NULL;
    stereoCb = NULL;
    webcam = NULL;
    frameCb = NULL;
    filesRead = false;
    frameConsCnt = 0;
    glHandler.loglevel = 0;
    glHandler.profEnable = 0;
    glHandler.protocol = USB_VSC;
    resolution = RES_VGA;
    frameBpp = 1;
if(inputType == FRAME_INPUT_MYRIAD)
    {
    sem_init(&mutex, 0, 1);
    XLinkInitialize(&glHandler);

    char name[MAXNAMESIZE];
    XLinkError_t rc = XLinkGetDeviceName(0, name, sizeof(name));
    if (rc != X_LINK_SUCCESS)
        printf("Could not get device name: %d \n\n", rc);

    handler.devicePath = name;
    rc = X_LINK_COMMUNICATION_UNKNOWN_ERROR;
    while(rc != X_LINK_SUCCESS) {rc = XLinkConnect(&handler);}
    usbLinkStream = XLinkOpenStream(0, X_LINK_STREAM_NAME, 5*1024*1024);
    configStream = XLinkOpenStream(0, X_LINK_STREAM_CONFIG_NAME, 5*1024*1024);

    assert(usbLinkStream >=0);
}
}
visionError visionHal::init(std::string mvcmdName){
    algos.clear();
    return MIG_OK;

}
static void* threadStub(void* ctx){

    ((visionHal*)ctx)->evalThreadFunc(ctx);
    return NULL;
}

static void* threadStubDyncal(void* ctx){
    ((visionHal*)ctx)->evalThreadDyncalFunc(ctx);
    return NULL;
}

static Mat convToMat(VisionFrame* frame){
    Mat im_gray = Mat(Size(frame->width, frame->height),  CV_8UC1);
    im_gray.data = (unsigned char*)frame->p1;
    Mat im_col(Size(frame->width, frame->height),  CV_8UC3);
    cvtColor(im_gray, im_col, COLOR_GRAY2RGB);

    return im_col;
}

static int seq_num = 0;
void* visionHal::evalThreadDyncalFunc(void* ctx){

    pthread_mutex_lock(&lock);
    Mat left_img = left_image.clone();
    Mat right_img = right_image.clone();
    pthread_mutex_unlock(&lock);

    Mat rot_mat = Mat::eye(3, 3, CV_64F);

    // send data back to myriad
    PCMessage homog_message;
    homog_message.type = HOMOGRAPHY;
    memcpy(homog_message.data, rot_mat.data, rot_mat.total()*rot_mat.elemSize());

    sem_wait(&mutex);
    homog_message.seq_num = seq_num++;
    XLinkWriteData(configStream, (uint8_t *)&homog_message, sizeof(PCMessage));
    sem_post(&mutex);
}

void visionHal::setStereoConf(uint8_t conf){
    PCMessage conf_message;
    conf_message.type = CONFIDENCE;

    sem_wait(&mutex);
    memcpy(conf_message.data, &conf, sizeof(uint8_t));
    conf_message.seq_num = seq_num++;
    XLinkWriteData(configStream, (uint8_t *)&conf_message, sizeof(PCMessage));
    sem_post(&mutex);
    //printf("conf %d %d\n", conf, conf_message.data[0]);
}

void* visionHal::evalThreadFunc(void* ctx){

    uint8_t conf = 127;
    lFrameL.registerConsumer(CONS_FRAME);
    lFrameR.registerConsumer(CONS_FRAME);
    lIMU.registerConsumer(CONS_IMU);
    while(1){
        metaResultsBuf* meta1;
        metaResultsBuf* meta2;
        frameMetadataGeneric* metaInfo;

        VisionFrame* frame;
        VisionFrame* frameL, *frameR;

        imuFrame* imuData;
        int count;
        if(inputType == FRAME_INPUT_MYRIAD)
            fullRead((void*)&count, sizeof(int));
        else
            count = 1;
        int consumedCnt = 0;
        while(count--){
            //Read MsgUid
            MsgGid muid;
            int subId;

            if(inputType == FRAME_INPUT_MYRIAD)
            {
                fullRead((void*)&muid, sizeof(MsgGid));
                fullRead((void*)&subId, sizeof(int));
            }
            else if(inputType == FRAME_INPUT_WEBCAM)
            {
                muid = GID_CAMERA;
                subId = 0;
            }
            else
            {
                assert("Not yet implemented" && 0);
            }

            //find the index associated with uid
            switch(muid)
            {
            case GID_IMU:
                imuData = new imuFrame;
                readIMU(imuData);
                lIMU.push(imuData, 0);

                break;
            case GID_CAMERA:
                readImage(&frame); //Read into buffer associated with its uid
                if(subId == 0){
                    lFrameL.push(frame, frame->frameCount);
                }else{
                    lFrameR.push(frame, frame->frameCount);
                }

                break;
            case GID_VTRACK:{
                readMeta(&metaInfo, &meta1);

                vTrackBulkResult bulkResult;
                bulkResult.setAddrAndPtrs(meta1->buf);
                t_vTrackResultSF* vData;
                bulkResult.getConfigAndResults(NULL, &vData);

                lVtrack.push(meta1, vData->header.frameId);
                delete metaInfo;
                break;
            }
            case GID_STEREO:{
                readMeta(&metaInfo, &meta2);
                frameThroughMeta *stereoData = (frameThroughMeta*)meta2->buf;
                VisionFrame* frame = new VisionFrame(stereoData->spec.width, stereoData->spec.height, stereoData->spec.stride);
                frame->exposureNs = metaInfo->exposureNs;
                frame->frameCount = metaInfo->frameCount;
                frame->timestampNs = metaInfo->timestampNs;
                frame->latencyNs = metaInfo->latencyNs;
                frame->frameType = metaInfo->frameType;
                frame->stride = stereoData->spec.stride;
                frame->width = stereoData->spec.width;
                frame->height = stereoData->spec.height;
                frame->bpp = stereoData->spec.bytesPP;
                frame->frameType = stereoData->spec.type;
                memcpy(frame->p1, stereoData->frameData, frame->stride * frame->height); //TODO: this is the third copy of this frame :(
                delete meta2;
                delete metaInfo;
                //decision on this should go to a stereo-specific location
                if(subId == 0){ // disparity
                    lDisp.push(frame, frame->frameCount);
                }else{
                    lDepth.push(frame, frame->frameCount);
                }
                break;
            }
            default:
                assert(0 && "not implemented");
            }
        }
        //run algorithms if needed and push back results
        frameL = lFrameL.get((char)CONS_FRAME);
        frameR = lFrameR.get((char)CONS_FRAME);
        if(frameL || frameR){
            if(frameCb){
                consumedCnt++;
                frameCb(frameL, frameR);
            }
            if(frameL)
                lFrameL.consume(frameL->frameCount, CONS_FRAME);
            if(frameR)
                lFrameR.consume(frameR->frameCount, CONS_FRAME);
            for(std::vector<algoDescriptor>::iterator it = algos.begin(); it != algos.end(); ++it) {

                if (it->algo->where == ALGO_PROCESS_MYRIAD){
                }else{
                    if (it->type == VTRACK_ALGO){
                        meta1 = new metaResultsBuf(FRAME_CUSTOM_META_SIZE);
                        vTrackBulkResult bulkResult;
                        bulkResult.setAddrAndPtrs(meta1->buf);
                        t_vTrackResultSF* vData;
                        it->algo->runAlgOnFrame(frameL, frameR, (void*)meta1->buf);
                        bulkResult.getConfigAndResults(NULL, &vData);
                        lVtrack.push(meta1, frameL->frameCount);
                    }else{
                        assert(0); //stereo implementation
                    }
                }
            }
        }



        if(!lVtrack.empty() && !lFrameL.empty()){
            //TODO: synchronization, use all queues, call multiple handlers
            metaResultsBuf* resBuf = (metaResultsBuf*) lVtrack.get((char)CONS_VTRACK);
            t_vTrackResultSF *vTrackData;
            VisionFrame *frm;
            if(resBuf) {
                vTrackBulkResult bulkResult;
                bulkResult.setAddrAndPtrs(resBuf->buf);
                bulkResult.getConfigAndResults(NULL, &vTrackData);
                frm = lFrameL.get(vTrackData->header.frameId, (char)CONS_VTRACK);

                int frameCountToConsume = vTrackData->header.frameId;
                if(vtrackCb && vTrackData && frm){
                    consumedCnt++;
                    vtrackCb(frm, vTrackData);
                }
                if(frm){
                    lFrameL.consume(frameCountToConsume, CONS_VTRACK);
                }
                lVtrack.consume(frameCountToConsume, CONS_VTRACK);
            }
            else {
                vTrackData = NULL;
            }
        }

        if(!lDisp.empty() && !lDepth.empty() && !lFrameR.empty() && !lFrameL.empty()){
            VisionFrame *disp = lDisp.get((char)CONS_STEREO);
            VisionFrame *frmL = lFrameL.get(disp->frameCount, (char)CONS_STEREO);
            VisionFrame *frmR = lFrameR.get(disp->frameCount, (char)CONS_STEREO);
            VisionFrame *depth = lDepth.get(disp->frameCount, (char)CONS_STEREO);
            int frameCountToConsume = disp->frameCount;
            if(stereoCb){
                consumedCnt++;
                if(frmL && frmR && disp && depth){
                    stereoCb(frmL, frmR, disp, depth);
                }else{
                }
            }

            if(stereoCb){
                setStereoConf(conf_global);
            }

            ++frame_count;

            lDisp.consume(frameCountToConsume, CONS_STEREO);
            lDepth.consume(frameCountToConsume, CONS_STEREO);
            lFrameL.consume(frameCountToConsume, CONS_STEREO);
            lFrameR.consume(frameCountToConsume, CONS_STEREO);
        }
        if(!lIMU.empty()){
            imuFrame* imu = lIMU.get((char)CONS_IMU);
            if(imuCb)
                imuCb(imu);
            lIMU.consume(0, CONS_IMU);
        }
    }
}

visionError visionHal::registerVtrackListener(vTrackHandler handler, iVisionAlgorithm* algo){
    if (frameBpp == 2){
        printf("ERROR. Can't register vTrack algo if 2 BPP configuration is set. vTrack supports 1BPP input data only.\n");
        return MIG_ERROR;
    }
    addAlgo(algo, VTRACK_ALGO);
    lVtrack.registerConsumer(CONS_VTRACK);
    lFrameL.registerConsumer(CONS_VTRACK);
    vtrackCb = handler;
    frameConsCnt++;
    return MIG_OK;
}
visionError visionHal::registerStereoListener(stereoHandler handler, iVisionAlgorithm* algo){
    addAlgo(algo, STEREO_ALGO);
    lFrameL.registerConsumer(CONS_STEREO);
    lFrameR.registerConsumer(CONS_STEREO);
    lDisp.registerConsumer(CONS_STEREO);
    lDepth.registerConsumer(CONS_STEREO);

    stereoCb = handler;
    frameConsCnt++;
    return MIG_OK;
}
visionError visionHal::registerFrameListener(frameHandler handler){
    frameCb = handler;
    frameConsCnt++;
    return MIG_OK;
}


visionError visionHal::start(){
    pthread_attr_t attr;
    int s;
    moviEvalConfig start;

    if(inputType == FRAME_INPUT_MYRIAD)
    {
    start.cmd = BPP;
    start.value = frameBpp;
    XLinkWriteData(configStream, (uint8_t*)&start, sizeof(moviEvalConfig));
    start.cmd = RES;
    start.value = (int) resolution;
    XLinkWriteData(configStream, (uint8_t*)&start, sizeof(moviEvalConfig));

    start.cmd = START;
    XLinkWriteData(configStream, (uint8_t*)&start, sizeof(moviEvalConfig));
    }
    s = pthread_attr_init(&attr);
    if (s != 0){
        printf("can't initialize thread attributes");
        return MIG_ERROR;
    }
    pthread_mutex_init(&lock, NULL);
    s = pthread_create(&threadId, &attr, &threadStub, this);
    return MIG_OK;
}
visionError visionHal::stop(){
    void* retVal;
    pthread_join(threadId, &retVal);
    pthread_mutex_destroy(&lock);
    //consume everything else
    lFrameL.clearQueue();
    lFrameR.clearQueue();
    lDepth.clearQueue();
    lDisp.clearQueue();
    lIMU.clearQueue();
    lVtrack.clearQueue();
    return MIG_OK;

}
visionError visionHal::addAlgo(iVisionAlgorithm* algo, algoType type ){
    algoDescriptor d;
    d.algo = algo;
    d.type = type;
    algos.push_back(d);
    if (algo->where == ALGO_PROCESS_PC)
        algo->initAlgorithm();
    else{
        char* data;
        int size;
        algo->getConfigSerialization(&data, &size);
        moviEvalConfig cfg;
        cfg.cmd = ADD_ALGO;
        cfg.type = type;
        XLinkWriteData(configStream, (uint8_t*)&cfg, sizeof(moviEvalConfig));
        XLinkWriteData(configStream, (uint8_t*)data, size);
        free(data);
    }
    return MIG_OK;

}
visionError visionHal::configureKeepData(bool keep){

    lFrameL.setKeep(keep);
    lFrameR.setKeep(keep);
    lDepth.setKeep(keep);
    lDisp.setKeep(keep);

    lIMU.setKeep(keep);
    lVtrack.setKeep(keep);
    return MIG_OK;
}
visionHal::~visionHal(){
    if(webcam)
        delete webcam;
    int status;
    status = XLinkCloseStream(usbLinkStream);
    status += XLinkCloseStream(configStream);
    status += XLinkResetRemote(0);

    if (status == 0)
    {
    }
    else
    {
        printf("close and/or reset failed: %x\n", status);
    }
}

static void fullRead(void* buffer, int cnt)
{
    streamPacketDesc_t * packet;
    XLinkError_t rc = XLinkReadData(usbLinkStream,&packet);
    if (packet == NULL || rc != X_LINK_SUCCESS)
    {
        printf("can't read data\n");
    }
    assert(cnt == packet->length);
    memcpy(buffer, packet->data, packet->length);
    rc = XLinkReleaseData(usbLinkStream);
    assert(rc == X_LINK_SUCCESS);
}

void
visionHal::readImage(VisionFrame** image)
{
    frameSpec frSpec;
    frameMetadataGeneric frameInfo;
    //TODO: this may go to an image parser class with subclasses for each case
    switch(inputType){
    case FRAME_INPUT_MYRIAD:{
        fullRead((void*)&frameInfo, sizeof(frameInfo));
        fullRead((void*)&frSpec, sizeof(frSpec));
        m_frameWidth = frSpec.width;
        m_frameHeight = frSpec.height;
        VisionFrame* frame = new VisionFrame(frSpec.width, frSpec.height, frSpec.stride);
        *image = frame;
        fullRead(frame->p1, frSpec.stride * frSpec.height);
        frame->bpp = frSpec.bytesPP;
        frame->exposureNs = frameInfo.exposureNs;
        frame->frameCount = frameInfo.frameCount;
        frame->timestampNs = frameInfo.timestampNs;
        frame->latencyNs = frameInfo.latencyNs;
        frame->frameType = frameInfo.frameType;
        return;
        break;
    }
    case FRAME_INPUT_WEBCAM:{
        readWebcamFrame(roi);
        break;
    }
    case FRAME_INPUT_DATASET:{
        readFileFrame(roi);
        break;
    }
    default:
        break;
    }
    cv::Mat grayImg;
    if(roi.type() == CV_8UC1)
        grayImg = roi.clone();
    else
        cvtColor(roi, grayImg, CV_BGR2GRAY);
    VisionFrame* frame = new VisionFrame(grayImg.cols, grayImg.rows);
    //TODO: fill frame info for datasets
    *image = frame;
    frame->bpp = 1;
    frame->frameType = RAW8;
    memcpy(frame->p1, grayImg.data, grayImg.cols * grayImg.rows);
}
void
visionHal::readMeta(frameMetadataGeneric** info, metaResultsBuf** meta){
    int metaSize;
    *info =  new frameMetadataGeneric;
    fullRead((uint8_t*)&metaSize, sizeof(int));
    fullRead((uint8_t*)*info, sizeof(frameMetadataGeneric));
    fullRead((uint8_t*)&metaSize, sizeof(int));
    *meta =  new metaResultsBuf(metaSize);
    fullRead((*meta)->buf, metaSize);
}
void visionHal::readIMU(imuFrame* imuData)
{
    int imuSize;
    fullRead((uint8_t*) &imuSize, sizeof(int));
    fullRead((uint8_t*) imuData, imuSize);
}

void
visionHal::openWebcam()
{
    if(inputPath.length() == 0)
        inputPath = "/dev/video0";
    std::size_t idx = inputPath.find_last_of("/");
    if(inputPath.length() - idx < 6)
        assert(0);
    if(inputPath.substr(idx, 6) != "/video")
        assert(0);
    int camIdx = stoi(inputPath.substr(idx + 6));

    webcam = new cv::VideoCapture(camIdx);
    if (!webcam->isOpened())
    {
        cout << "ERROR: No input from camera!";
        exit(0);
    }

    // Set camera properties with multiple resolutions for testing
    m_frameWidth = webcam->get(CV_CAP_PROP_FRAME_WIDTH);
    m_frameHeight = webcam->get(CV_CAP_PROP_FRAME_HEIGHT);

    // Capture a first frame
    if (!webcam->read(roi))
    {
        cout << "error reading first frame" << endl;
        exit(1);
    }

}

void
visionHal::readWebcamFrame(cv::Mat &input)
{
    if(webcam == NULL)
    {
        openWebcam();
    }
    (*webcam) >> roi;
}

void
visionHal::openFiles()
{
    string command;

    command += "ls ";
    command += inputPath;
    command += " | sort -V";

    FILE *fpipe;
    fpipe = popen(command.c_str(), "r");
    if (fpipe == NULL)
        exit(1);

    char buf[4096 + 1];
    while (fgets(buf, sizeof(buf) - 1, fpipe))
    {
        string fn = buf;
        fn.pop_back();
        fn = inputPath + "/" + fn;
        filesQueue.push(fn);
    }
    fclose(fpipe);
}

void
visionHal::readFileFrame(cv::Mat &input)
{
    if(!filesRead)
    {
        openFiles();
        filesRead = true;
    }
    if(filesQueue.empty())
       exit(1);
    string filename = filesQueue.front();
    filesQueue.pop();

    string extension = filename.substr(filename.length() - 3);
    if(extension == "raw" || extension == "yuv")
    {
        FILE *fin = fopen(filename.c_str(), "rb");
        if(!fin)
            exit(1);
        input = cv::Mat(m_frameHeight, m_frameWidth, CV_8UC1);
        if(m_frameHeight != fread(input.data, m_frameWidth, m_frameHeight, fin))
            exit(1);
        fclose(fin);
    }
    else
        input = cv::imread(filename.c_str());
}
