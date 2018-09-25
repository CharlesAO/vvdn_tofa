///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     visionHal public interface
///


#ifndef VISION_HAL_H
#define VISION_HAL_H

#include <string>
#include <list>
#include <queue>
#include <string>
#include <algorithm>
#include "vTrackOutput.h"
#include "dataQueue.h"
#include "iVisionAlgorithm.h"
#include "opencv2/opencv.hpp"
#include "visionFrame.h"

// forward declarations
struct imuFrame;
struct frameMetadataGeneric;

class metaResultsBuf {
public:
    metaResultsBuf(int size) : buf(new uint8_t[size]) {}
    ~metaResultsBuf() { delete[] buf;}
    uint8_t* buf;
};


enum FrameInput
{
    FRAME_INPUT_MYRIAD, FRAME_INPUT_DATASET, FRAME_INPUT_WEBCAM,
};

enum visionError
{
   MIG_OK = 0,
   MIG_ERROR,
};

typedef enum{
    VTRACK_ALGO= 0xAA,
    STEREO_ALGO= 0x55,
}algoType;
typedef enum{
    RES_720P= 0x0,
    RES_VGA
}algoResolution;


typedef struct{
    iVisionAlgorithm* algo;
    algoType type;
}algoDescriptor;
typedef void (*vTrackHandler)(VisionFrame* frameData, struct t_vTrackResultSF* vTrackData);
typedef void (*stereoHandler)(VisionFrame* frameL, VisionFrame* frameR, VisionFrame* disp, VisionFrame* depth);
typedef void (*frameHandler)(VisionFrame* frameL, VisionFrame* frameR);

typedef void (*imuHandler)(imuFrame* imuData);

class visionHal {
protected:
    uint32_t m_frameWidth;
    uint32_t m_frameHeight;
    FrameInput inputType;
    std::string inputPath;
public:
    visionHal(int num_images, FrameInput inputType = FRAME_INPUT_MYRIAD);
    visionError init(std::string mvcmdName);
    void setStereoConf(uint8_t new_conf);

    // if set to true, moviEval will not free up any frames, application is responsible to free up the data
    visionError configureKeepData(bool keep);

    visionError start();
    visionError stop();
//    migError getImages(MigFrame** frameLData, MigFrame** frameRData);
//    migError getImu(imuFrame** imuData);
    visionError registerFrameListener(frameHandler handler);
    visionError registerVtrackListener(vTrackHandler handler, iVisionAlgorithm* algo);
    visionError registerStereoListener(stereoHandler handler, iVisionAlgorithm* algo);

//    migError getStereo(struct stereoOutput* stereoData);
    void* evalThreadFunc(void*);
    void* evalThreadDyncalFunc(void* ctx);

    // Empty virtual destructor
    virtual ~visionHal();

    void setInputType(FrameInput type, std::string path = "")
    {
        this->inputType = type;
        this->inputPath = path;
    }
    visionError setFrameResolution(algoResolution res)
    {
        this->resolution = res;
    }
    visionError setFrameBpp(int bpp)
    {
        if (bpp != 1 && bpp != 2){
            printf("ERROR. Supported BPP is 1 or 2\n");
            return MIG_ERROR;
        }
        if (bpp == 2 && vtrackCb){
            printf("ERROR. vTrack is supporting 1 BPP data only.\n");
            return MIG_ERROR;
        }
        this->frameBpp = bpp;
        return MIG_OK;
    }
private:
    visionError addAlgo(iVisionAlgorithm *algo, algoType type);
    //TODO: parser class
    void readImage(VisionFrame** image);// to be deleted
    void readMeta(frameMetadataGeneric** info, metaResultsBuf** meta);
    void readIMU(imuFrame* imuData); // to be deleted
    void openWebcam();
    void readWebcamFrame(cv::Mat &input);
    void openFiles();
    void readFileFrame(cv::Mat &input);
    std::queue<std::string> filesQueue;
    bool filesRead;
    cv::VideoCapture* webcam;
    cv::Mat roi;
    int frame_count;
    algoResolution resolution;
    int frameBpp;
    vTrackHandler vtrackCb;
    stereoHandler stereoCb;
    frameHandler frameCb;
    int frameConsCnt;
    imuHandler imuCb;
    std::vector<algoDescriptor> algos;
    pthread_t threadId;
    int m_numImages;
    DataQueue<VisionFrame*> lFrameL;
    DataQueue<VisionFrame*> lFrameR;
    DataQueue<VisionFrame*> lDepth;
    DataQueue<VisionFrame*> lDisp;

    DataQueue<imuFrame*> lIMU;
    DataQueue<metaResultsBuf*> lVtrack;

    cv::Mat left_image;
    cv::Mat right_image;
    uint8_t stereo_conf;
};

// TODO
void cofidenceToVisionHAL(uint8_t conf);
#endif
