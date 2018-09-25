#include <cstring>

#include <pthread.h>

#include "superFrame.h"
#include "flicPlgMest.h"

#define MVLOG_UNIT_NAME plgFlicMest

#include <mvLog.h>

plgFlicMest::plgFlicMest()
{
    mestHandler.privateData = NULL;
    prevBuffer = NULL;
    mode = MEST_MODE_NORMAL;
    runtimeConfigGiven = false;
    prevImg = nullptr;
}

plgFlicMest::plgFlicMest(customProcCtx _ctx) :
        plgFlicMest()
{
    ctx = _ctx;
    gyroAssist = false;
}

plgFlicMest::~plgFlicMest()
{

}

void* plgFlicMest::pluginSpecificInit(void)
{
    assert(ctx.camCnt == 1 &&
            "This plugin supports single camera only - Probably forgot to call Init");

    mestMutexInit();
    mestInit(&mestHandler, mode, &resources, &this->staticConfig);
    if(runtimeConfigGiven)
        mestSetRuntimeConfig(&mestHandler, &this->runtimeConfig);

    assert(mestHandler.privateData != NULL &&
            "Please initialize Resource configuration before calling Create()"
            " - Probably forgot to call 'init'");

    mvLog(MVLOG_INFO, "MEST init done\n");
    return NULL;
}

void* plgFlicMest::pluginSpecificProcess()
{
    frameBuffer *currBuffer;
    assert(ctx.camCnt == 1);

    currBuffer = &frameToProcess[0]->ptr->fb;

    mestError_t mestStatus = MEST_SUCCESS;
    frameMeta* metadata;
    mestBulkOutput_t mestRes;

    metadata = ((frameMeta*) metaOut[0]->ptr->fb.p1);
    mestRes.address = metadata->customMetadata;
    mestRes.size = mestGetResultSize(&mestHandler);
    metaOut[0]->ptr->size = mestRes.size;

    if(prevBuffer)
    {
        mestStatus = mestRun(&mestHandler, *prevBuffer, *currBuffer, &mestRes);
    }
    else
    {
        ((mestOutput_t*)mestRes.address)->header.outputFlowCnt = (uint32_t)(-1);
    }

    if(mestStatus == MEST_BUFFER_NOT_ALIGNED)
        mvLog(MVLOG_ERROR, "Input frame buffers must be aligned to 128 bytes for MEST!");

    assert(metaOut[0]->ptr->size <= FRAME_CUSTOM_META_SIZE);

    prevImg = *frameToProcess[0];
    prevBuffer = &prevImg.ptr->fb;

//    assert(mestStatus == MEST_SUCCESS);
    return NULL;
}

void plgFlicMest::init(const mestResourceConfig_t &resources,
        const mestMode_t mode)
{
    mestInitStaticConfigVGA(&this->staticConfig); // Default VGA static config
    this->resources = resources;
    this->mode = mode;
}

void plgFlicMest::init(const mestResourceConfig_t &resources,
        const mestMode_t mode, const mestParamConfig_t &staticConfig)
{
    init(resources, mode);
    this->staticConfig = staticConfig;
}

void plgFlicMest::init(const mestResourceConfig_t &resources,
        const mestMode_t mode, const mestParamConfig_t &staticConfig,
        const mestRuntimeConfig_t &runtimeConfig)
{
    init(resources, mode, staticConfig);
    initRuntime(runtimeConfig);
}

void plgFlicMest::initRuntime(const mestRuntimeConfig_t &runtimeConfig)
{
    this->runtimeConfig = runtimeConfig;
    runtimeConfigGiven = true;
}

void plgFlicMest::addROI(const mestROI_t &roi, mestFeatureSelPolicy_t policy)
{
    mestAddROI(&mestHandler, &roi, NULL, 0, policy, -1);
}

void plgFlicMest::addROI(const mestROI_t &roi, mestFeatureSelPolicy_t policy,
        xyPos_t *ptsArr, uint32_t ptsCnt)
{
    mestAddROI(&mestHandler, &roi, ptsArr, ptsCnt, policy, -1);
}

void plgFlicMest::releaseAllFrms()
{
    visionBasePlg::releaseAllFrms();
    prevImg = nullptr;
}
