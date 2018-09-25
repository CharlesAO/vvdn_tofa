///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vtrack PC wrapper
///

#ifndef PC_VTRACK_H
#define PC_VTRACK_H

#include "iVisionAlgorithm.h"
#include "visionFrame.h"
#include "swcFrameTypes.h"
#include "vTrackOutput.h"
#include "vTrack.h"
#include "vPipe.h"

enum VTRACK_OF_MODE {
    OF_MOVI, OF_MEST
};

//TODO: make singleton
class PcVtrack: public iVisionAlgorithm {
    public:
        PcVtrack();
        PcVtrack(VTRACK_OF_MODE ofType);
        virtual ~PcVtrack();
        void configure(const std::map<std::string, std::string> &options);
        void getConfigSerialization(char** data, int* size);

        void initAlgorithm();
        int runAlgOnFrame(VisionFrame* frameL, VisionFrame* frameR, void* resultsPtr);
        int visualize();//TODO:params
        int visualizeKPI();//TODO:


    private:
        void initBuffers();

        static int vPipeDone;  //note this means we can't create more than one instance of PcVtrack at a time
        frameBuffer inputImage[2];
        vTrackBulkResult bulkResult;
        t_vTrackDebug debug;
        t_vPipeParams params;
        t_vPipeResourceCfg vpResCfg;
        t_vPipeAlgoCfg algoCfg;
        bool isInitialized;

        VTRACK_OF_MODE ofMode;
};
#endif
