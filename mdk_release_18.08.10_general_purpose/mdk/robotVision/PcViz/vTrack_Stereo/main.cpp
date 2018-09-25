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
#include <array>

#ifdef GUI_VISUALIZER
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/mman.h>
#endif

// common components
#include <imuFrame.h>
#include <mvLog.h>
#include "visionHal.h"
// local headers
#include "VtrackDrawer.h"
#include "VtrackDrawerMoviOf.h"
#include "VtrackDrawerMest.h"
#include "StereoDrawer.h"

#include "pcVtrack.h"
#include "pcStereo.h"

using namespace std;

const uint32_t GRAPHS_WIDTH = 640;
const uint32_t GRAPHS_HEIGHT = 480;
const uint32_t VTRACK_MAX_FEATURES = 1024;


#ifdef GUI_VISUALIZER
int fd_status, fd_left, fd_right, fd_disp, fd_rdisp, fd_vtrack;
int* conf_pointer;
#endif

array<float, 9> calib_array_f {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
float base_line = 0.035;

map<string, string> optChecker;

static void read_calib_bin(string& calib_bin_fp, array<char, 36>& calib_array){

  ifstream file (calib_bin_fp, ios::in|ios::binary);
  if (file.is_open())
  {
    file.read (calib_array.data(), calib_array.size());
    file.close();
  }
  else cout << "Unable to open calibration file." <<endl;
}

void displayHelp()
{
    map<string, string> opt;
    size_t maxLength = 0;

    opt.insert(make_pair("--alg", "[vtrack|stereo]"));
    opt.insert(make_pair("--vis", "[basic|detailed]"));
    opt.insert(make_pair("--of", "[movi|mest]"));
    opt.insert(make_pair("--res", "[VGA|720P]"));
    opt.insert(make_pair("-h", "[path to calibration file]"));

   /* opt.insert(make_pair("--srctype", "[stream|file]"));
    opt.insert(make_pair("--srcpath", "[path to file/device]"));
*/
    puts("vTrackStereo --device myriad [OPTION1]...");
    puts("Options:");
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
        store_option_val(opt, "--of", options);
        store_option_val(opt, "--vis", options);
        store_option_val(opt, "--alg", options);
        store_option_val(opt, "--res", options);
        store_option_val(opt, "--srctype", options);
        store_option_val(opt, "--srcpath", options);
        store_option_val(opt, "-h", options);
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
    grayscaleFrame = cv::Mat(cv::Size(frame->width, frame->height),  CV_8UC1);
    grayscaleFrame.data = (unsigned char*)frame->p1;
    cv::namedWindow( winString.c_str());
    cv::imshow(winString.c_str(), grayscaleFrame);
    keyPress = cvWaitKey(1);
    if(keyPress == 27) { //ESC
        exit(0); //TODO;
    }
}

VtrackDrawer* algDrawer;
StereoDrawer* stereoDrawer;


void vtrackCb(VisionFrame* frameData, struct t_vTrackResultSF* vTrackData){
    #ifdef GUI_VISUALIZER
        cv::Mat* out;
        out = algDrawer->draw_on_memory(frameData,(uint8_t*)vTrackData);

        cv::Mat vtrackMat(cv::Size(frameData->width, frameData->height),  CV_8UC3);
        cv::cvtColor(*out, vtrackMat, CV_BGR2RGB);

        write(fd_vtrack, &(frameData->width), 4);
        write(fd_vtrack, &(frameData->height), 4);
        write(fd_vtrack, &(algDrawer->avgFps), 4);
        write(fd_vtrack, vtrackMat.data, frameData->width*frameData->height*3);
        lseek(fd_vtrack, 0, SEEK_SET);
        out->release();
    #else
        int keyPress;
        keyPress = algDrawer->draw(frameData,(uint8_t*)vTrackData);
        if(keyPress == 27) { //ESC
        exit(0); //TODO;
        }
    #endif
}

unsigned int status = 0;

void stereoCb(VisionFrame* frameL, VisionFrame* frameR, VisionFrame* disp, VisionFrame* depth){
    #ifdef GUI_VISUALIZER
        cv::Mat im_gray = cv::Mat(cv::Size(disp->width, disp->height),  CV_8UC1);
        im_gray.data = (unsigned char*)disp->p1;
        medianBlur(im_gray, im_gray, 5);

        float fps = stereoDrawer->getAvgFPS(frameL->timestampNs);
        write(fd_rdisp, &(calib_array_f[0]), 4); //TODO this is not the focal
        write(fd_rdisp, &(base_line), 4);
        write(fd_rdisp, &(disp->width), 4);
        write(fd_rdisp, &(disp->height), 4);
        write(fd_rdisp, &fps, 4);
        write(fd_rdisp, im_gray.data, disp->width*disp->height);
        lseek(fd_rdisp, 0, SEEK_SET);

        im_gray = im_gray.mul(2.5);

        cv::Mat dispMat(cv::Size(disp->width, disp->height),  CV_8UC3);
        cv::applyColorMap(im_gray, dispMat, cv::COLORMAP_JET);
        cv::cvtColor(dispMat, dispMat, CV_BGR2RGB);
        write(fd_disp, &(disp->width), 4);
        write(fd_disp, &(disp->height), 4);
        write(fd_disp, &fps, 4);
        write(fd_disp, dispMat.data, disp->width*disp->height*3);
        lseek(fd_disp, 0, SEEK_SET);
    #else
        stereoDrawer->draw(frameL, frameR, disp, depth);
    #endif
}
void frameCb(VisionFrame* frameL, VisionFrame* frameR){
    #ifdef GUI_VISUALIZER
        //printf("c: %d\n", *conf_pointer);
        cofidenceToVisionHAL(*conf_pointer);

        float fake_fps = 0.0;
        write(fd_status, &(status), 4);
        status++;
        lseek(fd_status, 0, SEEK_SET);
        
        write(fd_left, &(frameL->width), 4);
        write(fd_left, &(frameL->height), 4);
        write(fd_left, &fake_fps, 4);
        write(fd_left, frameL->p1, frameL->width*frameL->height);
        lseek(fd_left, 0, SEEK_SET);

        write(fd_right, &(frameL->width), 4);
        write(fd_right, &(frameL->height), 4);
        write(fd_right, &fake_fps, 4);
        write(fd_right, frameR->p1, frameL->width*frameL->height);
        lseek(fd_right, 0, SEEK_SET);

    #else
	 cofidenceToVisionHAL(200);
        if(frameL)
        drawFrame(frameL, "left");
        if(frameR)
        drawFrame(frameR, "right");
    #endif
}

int main(int argc, const char *argv[])
{
    algoResolution algoRes = RES_720P;
    #ifdef GUI_VISUALIZER
        char * mmap_file_confidence = (char*)"/tmp/mem_confidence";
        char * mmap_file_status = (char*)"/tmp/mem_mapped_status";
        char * mmap_file_left = (char*)"/tmp/mem_mapped_left";
        char * mmap_file_right = (char*)"/tmp/mem_mapped_right";
        char * mmap_file_rdisp = (char*)"/tmp/mem_mapped_rdisp";
        char * mmap_file_disp = (char*)"/tmp/mem_mapped_disp";
        char * mmap_file_vtrack = (char*)"/tmp/mem_mapped_vtrack";

        int fd_confidence = open(mmap_file_confidence, O_RDONLY);
        conf_pointer = (int*) mmap(0, 4, PROT_READ, MAP_SHARED, fd_confidence, (mode_t)0);
        if(conf_pointer==(void*)-1){
            printf("Error: /tmp/mem_confidence is not available\n");
            return -1;
        }

        fd_status = open(mmap_file_status, O_WRONLY | O_CREAT, 0700);
        mmap(0, 0, PROT_WRITE, MAP_SHARED, fd_status, (mode_t)0700);
        fd_left = open(mmap_file_left, O_WRONLY | O_CREAT, 0700);
        mmap(0, 0, PROT_WRITE, MAP_SHARED, fd_left, (mode_t)0700);
        fd_right = open(mmap_file_right, O_WRONLY | O_CREAT, 0700);
        mmap(0, 0, PROT_WRITE, MAP_SHARED, fd_right, (mode_t)0700);
        fd_disp = open(mmap_file_disp, O_WRONLY | O_CREAT, 0700);
        mmap(0, 0, PROT_WRITE, MAP_SHARED, fd_disp, (mode_t)0700);
        fd_rdisp = open(mmap_file_rdisp, O_WRONLY | O_CREAT, 0700);
        mmap(0, 0, PROT_WRITE, MAP_SHARED, fd_rdisp, (mode_t)0700);
        fd_vtrack = open(mmap_file_vtrack, O_WRONLY | O_CREAT, 0700);
        mmap(0, 0, PROT_WRITE, MAP_SHARED, fd_vtrack, (mode_t)0700);
        write(fd_status, &(status), 4);
        printf("Running Robot Vision Evaluation Kit.\n");
    #else
        printf("Running Robot Vision Evaluation Kit. Select any window and press Esc to exit \n");
    #endif


    // Usage
    if (argc < 2) {
        puts("For help");
        puts("./vTrackStereo --help");
        return 1;
    }

    map<string, string> options;
    parseCommandOptions(argc, argv, options);
    if(options.find("--help") != options.end())
    {
        displayHelp();
        return 0;
    }

    visionHal* eval = NULL;

    auto typeIt = options.find("--device");
    if(typeIt != options.end())
    {
        if(typeIt->second == "pc")
            eval = new visionHal(1, FRAME_INPUT_WEBCAM);
        else if(typeIt->second == "myriad")
            eval = new visionHal(1, FRAME_INPUT_MYRIAD);
        else
        {
            assert(0);
        }
    }

    eval->init("notyetused.mvcmd");
    VisionFrame *frameL, *frameR;
    imuFrame* imudata;

    bool showGraphs = false;
    auto visIt = options.find("--vis");
    if (visIt != options.end()) {
        if(visIt->second == "detailed") {
            showGraphs = true;
        }
    }
    visIt = options.find("--res");
    if (visIt != options.end()) {
        if(visIt->second == "VGA") {
            algoRes = RES_VGA;
        } else if(visIt->second == "720P") {
            algoRes = RES_720P;
        } else {
            printf("Invalid resolution option\n");
            return 1;
        }
    }
    //TODO: to be moved to vTrackDrawer initialize
    auto it = options.find("--of");
    PcVtrack* vtrack;
    #ifdef GUI_VISUALIZER
        algDrawer = new VtrackDrawerMoviOf(500, 400, VTRACK_MAX_FEATURES, showGraphs);
        vtrack = new PcVtrack(OF_MOVI);
    #else
        if (it == options.end() || it->second == "movi")
        {
        printf("Using vTrack with OF \n");
        algDrawer = new VtrackDrawerMoviOf(GRAPHS_WIDTH, GRAPHS_HEIGHT, VTRACK_MAX_FEATURES, showGraphs);
        vtrack = new PcVtrack(OF_MOVI);
        }
        else if (it->second == "mest")
        {
        printf("Using vTrack with MEST \n");
        algDrawer = new VtrackDrawerMest(GRAPHS_WIDTH, GRAPHS_HEIGHT, VTRACK_MAX_FEATURES, showGraphs);
        vtrack = new PcVtrack(OF_MEST);
        }
        else
        {
        assert(0);
        }
    #endif

    algDrawer->initDrawer();
    #ifdef GUI_VISUALIZER
        eval->setInputType(FRAME_INPUT_MYRIAD);
        vtrack->where = ALGO_PROCESS_MYRIAD; //TODO, split image source and algo processing into separate params
    #else
        if (options.at("--device") == "myriad")
        {
        eval->setInputType(FRAME_INPUT_MYRIAD);
        vtrack->where = ALGO_PROCESS_MYRIAD; //TODO, split image source and algo processing into separate params
        }
        else if (options.at("--device") == "pc")
        {
        eval->setInputType(FRAME_INPUT_WEBCAM);
        vtrack->where = ALGO_PROCESS_PC;
        }
        else
        {
        printf("Invalid Option\n");
        return -1;
        }
    #endif

    array<char, 36> calib_array;
    memcpy(calib_array.data(), calib_array_f.data(), 36);
    
    it = options.find("-h");
    if (it != options.end())
    {
        read_calib_bin(options.at("-h"), calib_array);
        memcpy(calib_array_f.data(), calib_array.data(), 36);
    }
    else
    {
        cout << "Warning!: No calibration binary provided, the performance of the stereo depth algorithm reduced. Provide calibration binary with -h <path_to_calibration_bin>" << endl;
    }
    
    PcStereo* stereo = new PcStereo(calib_array);
    
    stereoDrawer = new StereoDrawer(false);
    #ifndef GUI_VISUALIZER
        stereoDrawer->initDrawer();
    #endif

    eval->registerFrameListener(frameCb);
    auto algIt = options.find("--alg");
    if (algIt == options.end()) { //no alg provided
        printf("Running vTrack and Stereo\n");
        eval->registerVtrackListener(vtrackCb, (iVisionAlgorithm*)vtrack);
        eval->registerStereoListener(stereoCb, (iVisionAlgorithm*)stereo);
    } else if(algIt->second == "vtrack") {
        printf("Running vTrack only\n");
        eval->registerVtrackListener(vtrackCb, (iVisionAlgorithm*)vtrack);
    } else if(algIt->second == "stereo") {
        printf("Running Stereo only\n");
        eval->registerStereoListener(stereoCb, (iVisionAlgorithm*)stereo);
    } else {
        printf("Invalid option\n");
    }

    eval->setFrameResolution(algoRes);
    eval->start();
    eval->stop(); // no support for stop, This function will hang infinitely for now.

    if(stereoDrawer)
        delete stereoDrawer;
    if(stereo)
        delete stereo;
    if(algDrawer)
        delete algDrawer;
    if(vtrack)
        delete vtrack;
    delete eval;
    printf("APP finished\n");

    return 0;
}
