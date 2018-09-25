///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#ifndef __OPTICAL_FLOW_PIPE_API_H__
#define __OPTICAL_FLOW_PIPE_API_H__

#include "mv_types.h"
#ifndef __PC__
#include "theDynContext.h"
#endif
#include "vTrackOutput.h" //tvXYLoc
#include <mvMacros.h>
#include "global_constants.h"
#include "opticalFlowTypes.h"

#ifdef MA2480
#include <MestApi.h>
#endif

#define MAX_OF_TASKS 20
#define MAX_SHAVES_OF 10
    class OpticalFlowShave;
    typedef u32 swcShaveUnit_t;

   typedef struct ofResourceCfg {
        u32 numShaves;
        swcShaveUnit_t*  shaveList;
        u32** initEntry;
        u32** runEntry;
        uint8_t cachePartData;
        uint8_t cachePartInstr;
    } ofResourceCfg_t;

    class OpticalFlow {
        public:
            OpticalFlow();
            ~OpticalFlow();
            void ofInit(/*tvPyramidBuffer* pyramid,*/
                    tvOpticalFlowCfg* algConfig,
                    ofResourceCfg_t* resCfg);

            int ofRun(tvPyramidBuffer* pyrImgPrev,
                    tvPyramidBuffer* pyrImgCur,
                    tvXYLoc featuresPrev[],
                    tvXYLoc featuresCur[],
                    fp32 featuresError[],
                    u32 featuresCount[],
                    u32 numCells,
                    u32 maxFeatPerCell) ;
			uint8_t* MEST_COORDS_HEAP;
			uint8_t* MEST_RES_TMP_HEAP;
			uint8_t* MEST_RES_HEAP;
        private:
            u32 numShaves;
            swcShaveUnit_t*  shaveList;

            tvOpticalFlowCfg ofAlgConfig;
            uint8_t cacheData;
            uint8_t cacheInstr;

#if defined(MA2480)
            // MEST variables
            mestHandler_t mestHandler;
			mestResourceConfig_t mestRes;
			mestROIId_t mestId;
			mestROI_t mestRoi;
			int mestInitStatus;
			mestParamConfig_t mestStaticConfig;
			mestRuntimeConfig_t mestRuntimeConfig;
#endif


#ifndef __PC__

            DynamicContext_t ofContext ALIGNED(64);
            DynamicContextInstances_elm ofContextInstanceData ALIGNED(64);

            DynamicContextInstancesPtr ofContextInstanceDataPtr ALIGNED(64);
#endif
    }ALIGNED(64);

#endif


