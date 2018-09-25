#ifndef FLIC_PLG_MEST_H__
#define FLIC_PLG_MEST_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>
#include <BehQueueOvr.h>
#include <MestApi.h>
#include "messages.h"
#include "visionBasePlg.h"

class plgFlicMest: public visionBasePlg
{
public:

    plgFlicMest(customProcCtx _ctx);

    plgFlicMest();

    ~plgFlicMest();

    virtual void releaseAllFrms();

    void init(const mestResourceConfig_t &resources, const mestMode_t mode =
            MEST_MODE_NORMAL);

    void init(const mestResourceConfig_t &resources, const mestMode_t mode,
            const mestParamConfig_t &staticConfig);

    void init(const mestResourceConfig_t &resources, const mestMode_t mode,
            const mestParamConfig_t &staticConfig,
            const mestRuntimeConfig_t &runtimeConfig);

    void initRuntime(const mestRuntimeConfig_t &runtimeConfig);

    void addROI(const mestROI_t &roi, mestFeatureSelPolicy_t policy);

    void addROI(const mestROI_t &roi, mestFeatureSelPolicy_t policy,
            xyPos_t *ptsArr, uint32_t ptsCnt);

private:
    void* pluginSpecificInit(void);
    void* pluginSpecificProcess();

    ImgFramePtr prevImg;
    mestHandler_t mestHandler;
    frameBuffer* prevBuffer;
    mestResourceConfig_t resources;
    mestParamConfig_t staticConfig;
    mestRuntimeConfig_t runtimeConfig;
    bool runtimeConfigGiven;
    mestMode_t mode;
};

#endif // FLIC_PLG_MEST_H__
