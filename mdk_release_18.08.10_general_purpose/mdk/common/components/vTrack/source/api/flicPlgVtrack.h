#ifndef FLIC_PLG_VTRACK_H__
#define FLIC_PLG_VTRACK_H__
#include <pthread.h>
#include <mvLog.h>
#include <mvMacros.h>
#include <ImgFrame.h>
#include "visionBasePlg.h"
#include "superFrame.h"
#include "vTrack.h"
#include "vPipe.h"
// plgFlicVTrack implements pluginSpecificInit and pluginSpecific of visionBasePlugin
// NB: must initialise before pluginSpecificInit is called this can be done either by:
//            (1) Dynamically construct plgFlicVTrack Using the constructor with initialisation
//            (2) Use the default constructor to create plgFlicVTrack and then call Init on it
//
// Inputs and outputs of plgFlicVTrack are those of vision base
// 
//                          VISION BASE Plugin
//
//
//                             +-----------------+
//      Left Image  +--------->|                 |+---------> meta0
//                             |   Vision Base   |
//      Right Image +--------->|      Plugin     |+---------> Stereo Images + metadata
//                             |                 |
//                             |                 |+---------> meta1
//                             |                 |
//                             +-----------------+
//                                           ^
//                                           |
//                                           +
//                                      Empty metadata




class plgFlicVTrack : public visionBasePlg{
    public:
        pthread_mutex_t* mutex; // external mutex to avoid vTrack processing to overlap with concurrent operations

        // Constructor with all the parameters. If vTrack is constructed dynamically, this can be used. Will avoid the need
        // to call another Init
        plgFlicVTrack(
                t_vPipeResourceCfg* vResourceCfg,
                volatile t_vPipeMode vp_mode,
                t_vPipeAlgoCfg* _algoCfg,
                t_vPipeParams* _params,
                customProcCtx _ctx,
                bool _gyroAssist
                ) : vpResCfg(vResourceCfg),
        m_vp_mode(vp_mode),
        algoCfg(_algoCfg),
        params(_params) {
            mutex = NULL;
            ctx =_ctx;
            gyroAssist = _gyroAssist;
        }


        // Default constructor. If vTrack is constructed statically, this would need to be used. The Init function would
        // then need to be called to initialise the config stuff
        plgFlicVTrack(
                ) : vpResCfg(0),
        m_vp_mode(VPIPE_MODE_LAST),
        algoCfg(0),
        params(0),
        initComplete(false) {

        }

        void
            Init(
                    t_vPipeResourceCfg* vResourceCfg,
                    volatile t_vPipeMode vp_mode,
                    t_vPipeAlgoCfg* _algoCfg,
                    t_vPipeParams* _params,
                    customProcCtx _ctx,
                    bool _gyroAssist
                ) {
                vpResCfg = vResourceCfg;
                m_vp_mode = vp_mode;
                algoCfg = _algoCfg;
                params = _params;
                ctx =_ctx;
                gyroAssist = _gyroAssist;
            }

        t_vTrackResultConfig getResultConfig(){
            assert(initComplete);
            return vp.vPipeGetResultConfig();
        }
    private:
        void* pluginSpecificInit(void) {
            assert(ctx.camCnt == 1 && "This plugin supports single camera only - Probably forgot to call Init");
            assert(vpResCfg != NULL && "Please initialize Resource configuration before calling Create() - Probably forgot to call Init");
            assert(algoCfg != NULL && "Please initialize algorithm configuration before calling Create() - Probably forgot to call Init");
            assert(params != NULL && "Please initialize runtime configuration before calling Create() - Probably forgot to call Init");
            assert(m_vp_mode != VPIPE_MODE_LAST && "Please initialize mode before calling Create() - Probably forgot to call Init");

            mvLog(MVLOG_INFO, "Starting\n");

            vp.vPipeInit(vpResCfg,m_vp_mode, algoCfg);
            initComplete = true;
            mvLog(MVLOG_INFO, "VPipeInit done\n");
            return NULL;
        }
        void* pluginSpecificProcess() {
            assert(ctx.camCnt == 1);


            frameBuffer* currBuffer;
                currBuffer = &frameToProcess[0]->ptr->fb;

            frameMeta* metadata = ((frameMeta*)metaOut[0]->ptr->fb.p1);

            t_vTrackResultConfig resultCfg = getResultConfig();
            uint32_t buffSize = bulkResult.setConfigAndPtrs(resultCfg, metadata->customMetadata);
            assert(buffSize < FRAME_CUSTOM_META_SIZE);

            metaOut[0]->ptr->size = resultCfg.resultsBufferLength;

            //populate frameId and timestamp in results struct
            t_vTrackResultSF* result;
            bulkResult.getConfigAndResults(NULL, &result);
            result->header.frameId = frameToProcess[0]->ptr->seqNo;
            result->header.timestampNs = frameToProcess[0]->ptr->ts;

            int status;
            if(mutex){
                pthread_mutex_lock(mutex);
            }
            if(gyroAssist){
                status = vp.vPipeRun(params, currBuffer, &rotationVector, &bulkResult);
            } else {
                status = vp.vPipeRun(params, currBuffer, NULL, &bulkResult);
            }
            if(mutex){
                pthread_mutex_unlock(mutex);
            }

            (void) status; //TODO: decide how to deal with it


            return NULL;

        }
        Vpipe vp;
        vTrackBulkResult bulkResult;
        t_vTrackDebug debug;

        t_vPipeResourceCfg* vpResCfg;
        volatile t_vPipeMode m_vp_mode;
        t_vPipeAlgoCfg* algoCfg;
        t_vPipeParams* params;
        bool initComplete;
};



#endif
