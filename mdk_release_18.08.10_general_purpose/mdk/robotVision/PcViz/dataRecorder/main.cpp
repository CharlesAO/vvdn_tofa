
///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main file of the pc application
///
// std
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <iostream>
#include <cassert>
#include <iomanip>
#include <string>
#include <vector>
// common components
#include <imuFrame.h>
#include <mvLog.h>
// local headers
#include "visionHal.h"

using namespace std;

#define CHECK_MALLOC(x) \
    if(!x){ \
        saveAndExit(MALLOC_FAIL); \
    }
#define MAX_FRAME_COUNT    (30 * 30 * 60) // 30 mins
#define ROUND_UP(x,y) ((x + y - 1) / y * y)
#define OUTPUT_DIR          "output"

typedef enum exitReason
{
    MALLOC_FAIL,
    MAX_FRAMES,
    USER_EXIT
}exitReason;

const uint32_t FRAME_WIDTH = 640;
const uint32_t FRAME_HEIGHT = 480;
int frameCount = 0;
VisionFrame** frames[2];
imuFrame** imus;
float ms1, ms2, ms3, ms4, ms5, ms6, time1;
struct timespec spec;
map<string, string> optChecker;

void writeFrame(VisionFrame* imgBuffer, string filename)
{
    int type = imgBuffer->bpp == 1 ? CV_8UC1 : CV_16UC1;

    cv::Mat TempMat(imgBuffer->height, imgBuffer->width, type, imgBuffer->p1);
    imwrite(filename, TempMat);
}


void writeImu(imuFrame* imuBuffer, string filename)
{
    ofstream imuFile(filename.c_str(), ios::binary);
    imuFile.write((const char*)imuBuffer, sizeof(imuFrame));
    imuFile.close();
}
void saveAndExit(exitReason reason)
{
    printf("Exiting because ");
    switch (reason)
    {
    case USER_EXIT:
        printf("user requested halt\n");
        break;
    case MALLOC_FAIL:
        printf("not enough memory\n");
        break;
    case MAX_FRAMES:
        printf("reached maximum number of frames\n");
        break;

    }
    int frameId;
    for(frameId = 0; frameId < frameCount; frameId++)
    {
        if (frameId % 100 == 0)
            printf("writing frame %d\n", frameId);
        stringstream f_ss;
        f_ss << frameId;
        string idxStr = f_ss.str();

        writeFrame(frames[0][frameId], OUTPUT_DIR "/image0_" + idxStr + ".png");
        writeFrame(frames[1][frameId], OUTPUT_DIR "/image1_" + idxStr + ".png");
        writeImu(imus[frameId], OUTPUT_DIR "/image1_" + idxStr + "_imu.bin");
        if ((frameId + 1 < frameCount) &&
                        (frames[0][frameId + 1]->timestampNs - frames[0][frameId]->timestampNs > 34*1000*1000))
            printf("timestamp difference too big at frame %d: %d ns\n", frameId, frames[0][frameId]->timestampNs - frames[0][frameId + 1]->timestampNs);
        if ((frameId + 1 < frameCount) &&
                        (frames[0][frameId + 1]->frameCount - frames[0][frameId]->frameCount > 1))
            printf("frame count difference too big at frame %d: %d\n", frameId, frames[0][frameId]->frameCount - frames[0][frameId + 1]->frameCount);
        delete frames[0][frameId];
        delete frames[1][frameId];
        free(imus[frameId]);
    }
    printf("done, exiting\n");
    exit(0);
}
void displayHelp()
{
    map<string, string> opt;
    size_t maxLength = 0;
    /*
    opt.insert(make_pair("--srctype", "[stream|file]"));
    opt.insert(make_pair("--srcpath", "[path to file/device]"));
    */
    puts("dataRecorder --device myriad");
   // puts("Options:");
    for(auto it : opt)
    {
        maxLength = max(maxLength, it.first.length());
    }
    maxLength += 10;

    for(auto it : opt)
    {
        cout << "\t" << setw(maxLength) << left << it.first << " ";
        cout << it.second << "\n";
    }
}

void parseCommandOptions(int argc, const char *argv[], map<string, string> &options)
{
    int i = 1;
    bool parsed;
    options.clear();

    auto store_option_val = [argc, argv, &i, &parsed] (const string &optCmd, const char *optReal,
                    map<string, string> &options)
            {
                if(optCmd == optReal)
                {
                    if(i + 1 >= argc)
                    {
                        mvLog(MVLOG_ERROR, "Missing argument for '%s'\n", optReal);
                        exit(1);
                    }
                    string val = argv[++i];
                    options[optReal] = val;
                    parsed = true;
                }
            };

    auto store_option = [argc, argv, &parsed] (const string &optCmd, const char *optReal,
            map<string, string> &options)
    {
        if(optCmd == optReal)
        {
            options[optReal] = "";
            parsed = true;
        }
    };

    for(; i < argc; i++)
    {
        string opt = string(argv[i]);
        parsed = false;
        store_option(opt, "--help", options);
        store_option_val(opt, "--device", options);
        store_option_val(opt, "--srctype", options);
        store_option_val(opt, "--srcpath", options);
        if(!parsed)
        {
            mvLog(MVLOG_ERROR, "Unknown option: %s\n", opt.c_str());
            exit(2);
        }
    }
}
void drawFrame(VisionFrame *frame,std::string winString){
    int keyPress;
    cv::Mat grayscaleFrame;
    int type = frame->bpp == 1 ? CV_8UC1 : CV_16UC1;
    grayscaleFrame = cv::Mat(cv::Size(frame->width, frame->height), type);
    grayscaleFrame.data = (unsigned char*)frame->p1;
    cv::namedWindow( winString.c_str());
    cv::imshow(winString.c_str(), grayscaleFrame);
}

void frameCb(VisionFrame* frameL, VisionFrame* frameR){
    int key;
    drawFrame(frameL, "left");
    drawFrame(frameR, "right");
    // get current time
    //printf("-----------------------\n");
    clock_gettime(CLOCK_REALTIME, &spec);
    ms1 = spec.tv_nsec/1000000;
    frames[0][frameCount] = frameL;
    frames[1][frameCount] = frameR;
    frameCount++;
    key = cv::waitKey(1);
    if(key == 27 || frameCount >= MAX_FRAME_COUNT)
        saveAndExit(USER_EXIT);
}

int main(int argc, const char *argv[])
{
    printf("Data recorder application. Select any window and press Esc to stop capture and save data\n");
    printf("WARNING. Data is kept in RAM while running, be careful.\n");
    // Usage
    if (argc < 2) {
        puts("For help");
        puts("./dataRecorder --help");
        return 1;
    }

    map<string, string> options;
    parseCommandOptions(argc, argv, options);
    if(options.find("--help") != options.end())
    {
        displayHelp();
        return 0;
    }

    system("mkdir " OUTPUT_DIR);

    visionHal* eval = new visionHal(1);
    eval->init("bla");
    VisionFrame *frameL, *frameR;
    imuFrame* imudata;

    frames[0] = (VisionFrame**) malloc(MAX_FRAME_COUNT * sizeof(VisionFrame*));
    frames[1] = (VisionFrame**) malloc(MAX_FRAME_COUNT * sizeof(VisionFrame*));
    imus = (imuFrame**) malloc(MAX_FRAME_COUNT * sizeof(imuFrame**));
    eval->configureKeepData(true); //frame deallocation will be done in the app
    eval->setFrameBpp(1);
    eval->registerFrameListener(frameCb);
    eval->start();

    while(1){
        sleep(1);
    }
    eval->stop();
    printf("APP finished\n");

    return 0;
}
